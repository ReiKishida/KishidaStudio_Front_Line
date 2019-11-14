//=============================================================================
//
// プレイヤー処理 [player.cpp]
// Author : Takuto Ishida
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "player.h"
#include "input.h"
#include "manager.h"
#include "renderer.h"
#include "debugProc.h"
#include "meshField.h"
#include "camera.h"
#include "model.h"
#include "motion.h"
#include "shadow.h"
#include "game.h"
#include "enemy.h"
#include "fade.h"
#include "life.h"
#include "particle.h"
#include "sound.h"
#include "bullet.h"
#include "scene3DBill.h"
#include "texture.h"
#include "UI_Texture.h"
#include "gauge.h"
#include "UI_Number.h"
#include <stdio.h>

//==================================
// マクロ定義
//==================================
#define MODEL_FILE					"data/TEXT/chaser/model_chaser.txt"
#define PLAYER_DAMAGE_TIME			(60)		// ダメージを受けた時の無敵時間
#define PLAYER_DAMAGE_MOVE			(40)		// ダメージを受けてから動けるようになるまでの時間
#define PLAYER_RETICLE_LENGTH		(2500.0f)	// レティクルの距離
#define PLAYER_BULLET_DISPERTION	(40)		// 弾のばらつき
#define RELOAD_TIME					(60 * 2)

//==================================
// 静的メンバ変数宣言
//==================================

//==================================
// 生成処理
//==================================
CPlayer* CPlayer::Create(void)
{
	CPlayer *pPlayer;

	pPlayer = new CPlayer;	// メモリを確保

	if (pPlayer != NULL)
	{// メモリ確保成功
		pPlayer->Init();
	}

	return pPlayer;
}

//=========================================
// コンストラクタ
//=========================================
CPlayer::CPlayer(int nPriority, CScene::OBJTYPE objType) : CScene(nPriority, objType)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nCntState = 0;
	m_fRotDest = 0.0f;
	m_fSpeed = 0.0f;
	m_fCameraAngle = 0.0f;
	m_pModel = NULL;
	m_nNumParts = 0;
	m_pUpperMotion = NULL;
	m_pLowerMotion = NULL;
	m_pReticle = NULL;
}

//=========================================
// デストラクタ
//=========================================
CPlayer::~CPlayer()
{
}

//=========================================
// 初期化処理
//=========================================
HRESULT CPlayer::Init(void)
{
	// デバイスの取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	FILE *pFile;
	int nCntParts = 0;		// パーツ数のカウンタ
	int nCntModel = 0;
	float fX, fY, fZ;
	int nId;
	int nParent;
	char **pModelName = NULL;

	// ファイルを開く
	pFile = fopen(MODEL_FILE, "r");

	if (NULL != pFile)
	{// ファイルがあった
		char aStr[128];

		while (1)
		{// スクリプトを読み終わるまでループ
			fscanf(pFile, "%s", aStr);

			if (strcmp(aStr, "SCRIPT") == 0)
			{// スクリプト開始
				while (strcmp(aStr, "END_SCRIPT") != 0)
				{// スクリプト終了メッセージまでループ
					fscanf(pFile, "%s", aStr);

					if (strcmp(aStr, "NUM_MODEL") == 0)
					{// モデル数
						int nNumModel;
						fscanf(pFile, " = %d", &nNumModel);

						pModelName = new char*[nNumModel];		// メモリ確保
					}
					if (strcmp(aStr, "MODEL_FILENAME") == 0)
					{// キャラクター情報
						fscanf(pFile, " = %s", &aStr);

						pModelName[nCntModel] = new char[64];		// メモリ確保
						strcpy(pModelName[nCntModel], aStr);
						nCntModel++;
					}
					if (strcmp(aStr, "CHARACTERSET") == 0)
					{// キャラクター情報
						while (strcmp(aStr, "END_CHARACTERSET") != 0)
						{// キャラクター情報終了メッセージまでループ
							fscanf(pFile, "%s", aStr);

							if (strcmp(aStr, "NUM_PARTS") == 0)
							{// パーツ数
								fscanf(pFile, " = %d", &m_nNumParts);
								m_pModel = new CModel*[m_nNumParts];

								for (int nCntModelParts = 0; nCntModelParts < m_nNumParts; nCntModelParts++)
								{// パーツ数分ループ
									m_pModel[nCntModelParts] = CModel::Create(&m_mtxWorld);
								}
							}
							else if (strcmp(aStr, "PARTSSET") == 0)
							{// パーツの設定
								while (strcmp(aStr, "END_PARTSSET") != 0)
								{// パーツの設定終了メッセージまでループ
									if (m_pModel[nCntParts] != NULL)
									{// メモリ確保成功
										fscanf(pFile, "%s", &aStr);

										if (strcmp(aStr, "INDEX") == 0)
										{// モデル番号
											fscanf(pFile, " = %d", &nId);

											// 使うモデルを指定
											m_pModel[nCntParts]->SetModel(pModelName[nId]);
										}
										else if (strcmp(aStr, "PARENT") == 0)
										{// 親を決める
											fscanf(pFile, " = %d", &nParent);

											if (nParent >= 0)
											{// 親のポインタを設定
												m_pModel[nCntParts]->SetParent(m_pModel[nParent]);
											}
											else
											{// 親がいない場合
												m_pModel[nCntParts]->SetParent(NULL);
											}
										}
										else if (strcmp(aStr, "POS") == 0)
										{// 位置の設定
											fscanf(pFile, " = %f %f %f", &fX, &fY, &fZ);

											// 位置の設定
											m_pModel[nCntParts]->SetPos(D3DXVECTOR3(fX, fY, fZ));
										}
										else if (strcmp(aStr, "ROT") == 0)
										{// 向きの設定
											fscanf(pFile, " = %f %f %f", &fX, &fY, &fZ);

											// 向きの設定
											m_pModel[nCntParts]->SetRot(D3DXVECTOR3(fX, fY, fZ));
										}
									}
								}
								nCntParts++;
							}
						}
					}
				}
				break;
			}
		}
		fclose(pFile);	// ファイルを閉じる
	}
	else
	{// ファイルがないとき
		MessageBox(0, "ファイルがありません", "プレイヤーモデルテキスト", MB_YESNO);
	}

	if (NULL != pModelName)
	{// メモリの開放
		for (int nCnt = 0; nCnt < m_nNumParts; nCnt++)
		{// メモリの開放
			delete[] pModelName[nCnt];
		}

		// メモリの開放
		delete[] pModelName;
	}

	if (NULL == m_pUpperMotion)
	{// モーションクラスの生成
		m_pUpperMotion = CMotion::Create(0, 6, m_pModel);
		m_pUpperMotion->SetMotion(CMotionManager::TYPE_NEUTRAL);	// ニュートラルモーションを設定
	}

	if (NULL == m_pLowerMotion)
	{// モーションクラスの生成
		m_pLowerMotion = CMotion::Create(0, m_nNumParts, m_pModel, 5);
		m_pLowerMotion->SetMotion(CMotionManager::TYPE_NEUTRAL);	// ニュートラルモーションを設定
	}

	if (NULL == m_pShadow)
	{// 影の生成
		m_pShadow = CShadow::Create(&m_pos);
	}

	if (NULL == m_pReticle)
	{// レティクルの生成
		m_pReticle = CScene3DBill::Create();
		m_pReticle->BindTexture(CTexture::GetTexture(CTexture::TEXTURE_RETICLE));
		m_pReticle->SetSize(D3DXVECTOR3(50.0f, 50.0f, 0.0f));
		m_pReticle->SetLighting(false);
		m_pReticle->SetZBuffer(D3DCMP_ALWAYS);
		m_pReticle->SwapPriority(6);
	}

	m_fSpeed = 5.0f;
	m_nRemBullet = 5;
	m_pUINum->SetRemainBullet(m_nRemBullet);

	return S_OK;
}

//=========================================
// 終了処理
//=========================================
void CPlayer::Uninit(void)
{
	if (NULL != m_pUpperMotion)
	{// モーションクラスの破棄
		m_pUpperMotion->Uninit();
		m_pUpperMotion = NULL;
	}

	if (NULL != m_pLowerMotion)
	{// モーションクラスの破棄
		m_pLowerMotion->Uninit();
		m_pLowerMotion = NULL;
	}

	if (NULL != m_pModel)
	{
		for (int nCntModel = 0; nCntModel < m_nNumParts; nCntModel++)
		{// モデルの破棄
			if (NULL != m_pModel[nCntModel])
			{
				m_pModel[nCntModel]->Uninit();
				delete m_pModel[nCntModel];
				m_pModel[nCntModel] = NULL;
			}
		}

		// メモリの開放
		delete[] m_pModel;
		m_pModel = NULL;
	}

	// オブジェクトの破棄
	CScene::Release();
}

//=========================================
// 更新処理
//=========================================
void CPlayer::Update(void)
{
	if (CManager::GetGame()->GetPart() == CGame::PART_ACTION)
	{// アクションパート
		CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();	// キーボードの入力を取得
		CXInput *pXInput = CManager::GetXInput();					// XInputの入力を取得

		// 移動の処理
		Movement();

		// 弾を撃つ
		Shoot();

		// 角度の更新
		Angle();

		// 重力
		//m_move.y -= GRAVITY;

		// 地面オブジェクトを探す
		CScene *pScene = CScene::GetSceneTop(MESHFIELD_PRIORITY);
		while (pScene != NULL)
		{// NULLになるまでループ
			CScene *pSceneNext = pScene->GetSceneNext();
			CScene::OBJTYPE objType = pScene->GetObjType();

			if (objType == CScene::OBJTYPE_FIELD)
			{// 地面だったとき
				CMeshField *pMeshField = (CMeshField*)pScene;

				float fHeight = pMeshField->GetHeight(m_pos);		// 地面の高さの取得
				m_pos.y = fHeight;
				break;
			}

			// 次のオブジェクトを見る
			pScene = pSceneNext;
		}

		D3DXVECTOR3 rotCamera = CManager::GetCamera()->GetRot();
		D3DXVECTOR3 posR = CManager::GetCamera()->GetPosR();

		m_pReticle->SetPos(D3DXVECTOR3(sinf(rotCamera.y) * PLAYER_RETICLE_LENGTH, cosf(rotCamera.x) * PLAYER_RETICLE_LENGTH, cosf(rotCamera.y) * PLAYER_RETICLE_LENGTH) + D3DXVECTOR3(m_pos.x, 0.0f, m_pos.z));

		for (int nCntModel = 0; nCntModel < m_nNumParts; nCntModel++)
		{// パーツモデルの更新
			m_pModel[nCntModel]->Update();
		}
		CDebugProc::Print("位置：%.2f %.2f %.2f", m_pos.x, m_pos.y, m_pos.z);
	}
}

//=========================================
// 描画処理
//=========================================
void CPlayer::Draw(void)
{
	// デバイスの取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;	// 計算用マトリックス

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 移動を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	for (int nCntModel = 0; nCntModel < m_nNumParts; nCntModel++)
	{// パーツモデルの描画
		//if ()
		//{// ダメージを受けている状態
		//	m_pModel[nCntModel]->AddColor(D3DXCOLOR(0.7f, 0.0f, 0.0f, -0.5f));
		//}

		// モデルの描画処理
		m_pModel[nCntModel]->Draw();

		//if (m_bDamage)
		//{// ダメージを受けている状態
		//	m_pModel[nCntModel]->AddColor(D3DXCOLOR(-0.7f, 0.0f, 0.0f, 0.5f));
		//}
	}
}

//=========================================
// 移動の処理
//=========================================
void CPlayer::Movement(void)
{
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();	// キーボードの入力を取得
	CXInput *pXInput = CManager::GetXInput();					// XInputの入力を取得
	bool bKey = false;				// ボタン押下フラグ
	float fRotY = CManager::GetCamera()->GetRotY();
	float fDirMove;

	CDirectInput *pDirectInput = CManager::GetDirectInput();	//DirectInputの取得
	CDirectInput::GamePad *DirectInputStick = pDirectInput->GetgamePadStick();


	// キー入力による移動
	if (pKeyboard->GetPress(DIK_A) || pXInput->GetThumbLX(0) <= -MIN_GAMEPAD_LEFT_THUMB_X || DirectInputStick->aGamePad.lX < -GAMEPAD_DEADZONE)
	{// ←方向へ移動
		if (pKeyboard->GetPress(DIK_W) || pXInput->GetThumbLY(0) >= MIN_GAMEPAD_LEFT_THUMB_Y || DirectInputStick->aGamePad.lY < -GAMEPAD_DEADZONE)
		{// 左前
			m_fRotDest = D3DX_PI * -0.25f;
			fDirMove = D3DX_PI * -0.25f;
		}
		else if (pKeyboard->GetPress(DIK_S) || pXInput->GetThumbLY(0) <= -MIN_GAMEPAD_LEFT_THUMB_Y || DirectInputStick->aGamePad.lY > GAMEPAD_DEADZONE)
		{// 左後ろ
			m_fRotDest = D3DX_PI * 0.25f;
			fDirMove = D3DX_PI * -0.75f;
		}
		else
		{// 左
			m_fRotDest = D3DX_PI * -0.5f;
			fDirMove = D3DX_PI * -0.5f;
		}
		bKey = true;
	}
	else if (pKeyboard->GetPress(DIK_D) || pXInput->GetThumbLX(0) >= MIN_GAMEPAD_LEFT_THUMB_X || DirectInputStick->aGamePad.lX > GAMEPAD_DEADZONE)
	{// →方向へ移動
		if (pKeyboard->GetPress(DIK_W) || pXInput->GetThumbLY(0) >= MIN_GAMEPAD_LEFT_THUMB_Y || DirectInputStick->aGamePad.lY < -GAMEPAD_DEADZONE)
		{// 右前
			m_fRotDest = D3DX_PI * 0.25f;
			fDirMove = D3DX_PI * 0.25f;
		}
		else if (pKeyboard->GetPress(DIK_S) || pXInput->GetThumbLY(0) <= -MIN_GAMEPAD_LEFT_THUMB_Y || DirectInputStick->aGamePad.lY > GAMEPAD_DEADZONE)
		{// 右後ろ
			m_fRotDest = D3DX_PI * -0.25f;
			fDirMove = D3DX_PI * 0.75f;
		}
		else
		{// 右
			m_fRotDest = D3DX_PI * 0.5f;
			fDirMove = D3DX_PI * 0.5f;
		}

		bKey = true;
	}
	else if (pKeyboard->GetPress(DIK_W) || pXInput->GetThumbLY(0) >= MIN_GAMEPAD_LEFT_THUMB_Y || DirectInputStick->aGamePad.lY < -GAMEPAD_DEADZONE)
	{// ↑方向へ移動
		m_fRotDest = 0.0f;
		fDirMove = 0.0f;
		bKey = true;
	}
	else if (pKeyboard->GetPress(DIK_S) || pXInput->GetThumbLY(0) <= -MIN_GAMEPAD_LEFT_THUMB_Y || DirectInputStick->aGamePad.lY > GAMEPAD_DEADZONE)
	{// ↓方向へ移動
		m_fRotDest = 0.0f;
		fDirMove = D3DX_PI;
		bKey = true;
	}

	if (bKey)
	{// 移動量の加算
		m_move = D3DXVECTOR3(sinf(fDirMove + fRotY), 0.0f, cosf(fDirMove + fRotY)) * m_fSpeed;
	}

	if (NULL != m_pUpperMotion && NULL != m_pLowerMotion)
	{// モーションクラスが使われている
		float fDiffX = (float)CManager::GetInputMouse()->GetDiffPointX();
		if (bKey)
		{// 移動モーション
			m_pUpperMotion->SetMotion(CMotionManager::TYPE_WALK);
			m_pLowerMotion->SetMotion(CMotionManager::TYPE_WALK);
		}
		else if (fDiffX == 0.0f)
		{// ニュートラルモーション
			m_pUpperMotion->SetMotion(CMotionManager::TYPE_NEUTRAL);
			m_pLowerMotion->SetMotion(CMotionManager::TYPE_NEUTRAL);
		}
		else if (fDiffX < 10.0f || fDiffX > -10.0f)
		{// 移動モーション
			m_pLowerMotion->SetMotion(CMotionManager::TYPE_WALK);
		}
	}

	m_move.x += (0 - m_move.x) * 0.4f;
	m_move.z += (0 - m_move.z) * 0.4f;

	m_pos += m_move;
}

//=========================================
// 弾を撃つ処理
//=========================================
void CPlayer::Shoot(void)
{
	CInputMouse *pMouse = CManager::GetInputMouse();	// マウスの入力を取得
	CXInput *pXInput = CManager::GetXInput();			// XInputの入力を取得

	if (pMouse->GetTrigger(CInputMouse::DIMS_BUTTON_0) && m_nRemBullet > 0)
	{
		// カメラの角度と注視点を取得
		D3DXVECTOR3 rotCamera = CManager::GetCamera()->GetRot();
		D3DXVECTOR3 posR = CManager::GetCamera()->GetPosR();

		// レティクル（目的の位置）の取得
		D3DXVECTOR3 posReticle = m_pReticle->GetPos();
		D3DXVECTOR3 dispertion = D3DXVECTOR3((float)(PLAYER_BULLET_DISPERTION - rand() % (PLAYER_BULLET_DISPERTION * 2)), 0.0f, (float)(PLAYER_BULLET_DISPERTION - rand() % (PLAYER_BULLET_DISPERTION * 2)));
		posReticle += dispertion;

		// 射出口の位置の取得
		D3DXMATRIX mtxCanon = m_pModel[3]->GetMtxWorld();
		D3DXVECTOR3 posCanon = D3DXVECTOR3(mtxCanon._41, mtxCanon._42, mtxCanon._43) + D3DXVECTOR3(sinf(rotCamera.y) * 30.0f, cosf(rotCamera.x) * 30.0f, cosf(rotCamera.y) * 30.0f);

		// 水平方向の角度の計算
		float fAngle = atan2f(posReticle.x - posCanon.x, posReticle.z - posCanon.z);
		float fAngleV = rotCamera.x + (float)(PLAYER_BULLET_DISPERTION - (rand() % PLAYER_BULLET_DISPERTION * 2)) * 0.0005f;

		// 弾の生成
		CBulletPlayer::Create(posCanon, fAngle, fAngleV, CBulletPlayer::TYPE_NORMAL);

		// レティクル（目的の位置）の取得
		posReticle = m_pReticle->GetPos();
		dispertion = D3DXVECTOR3((float)(PLAYER_BULLET_DISPERTION - rand() % (PLAYER_BULLET_DISPERTION * 2)), 0.0f, (float)(PLAYER_BULLET_DISPERTION - rand() % (PLAYER_BULLET_DISPERTION * 2)));
		posReticle += dispertion;

		// 射出口の位置の取得
		mtxCanon = m_pModel[5]->GetMtxWorld();
		posCanon = D3DXVECTOR3(mtxCanon._41, mtxCanon._42, mtxCanon._43) + D3DXVECTOR3(sinf(rotCamera.y) * 30.0f, cosf(rotCamera.x) * 30.0f, cosf(rotCamera.y) * 30.0f);

		// 水平方向の角度の計算
		fAngle = atan2f(posReticle.x - posCanon.x, posReticle.z - posCanon.z);
		fAngleV = rotCamera.x + (float)(PLAYER_BULLET_DISPERTION - (rand() % PLAYER_BULLET_DISPERTION * 2)) * 0.0005f;

		// 弾の生成
		CBulletPlayer::Create(posCanon, fAngle, fAngleV, CBulletPlayer::TYPE_NORMAL);
		m_nRemBullet--;
	}

	Reload();

	m_pUINum->SetRemainBullet(m_nRemBullet);
	//if (m_nRemBullet == 0) { m_nRemBullet = 3; }
}

//=========================================
// 角度更新
//=========================================
void CPlayer::Angle(void)
{
	// カメラの角度を求める
	D3DXVECTOR3 rotCamera = CManager::GetCamera()->GetRot();
	float fDiffRot;
	float fAngle = D3DX_PI + rotCamera.y;

	// 目的の角度の調節
	if (m_fRotDest > D3DX_PI) { m_fRotDest -= D3DX_PI * 2.0f; }
	if (m_fRotDest < -D3DX_PI) { m_fRotDest += D3DX_PI * 2.0f; }

	// 目的の角度への差分
	fDiffRot = m_fRotDest - m_rot.y;
	if (fDiffRot > D3DX_PI) { fDiffRot -= D3DX_PI * 2.0f; }
	if (fDiffRot < -D3DX_PI) { fDiffRot += D3DX_PI * 2.0f; }

	// 角度の更新
	m_rot.y += fDiffRot * 0.05f;
	if (m_rot.y > D3DX_PI) { m_rot.y -= D3DX_PI * 2.0f; }
	if (m_rot.y < -D3DX_PI) { m_rot.y += D3DX_PI * 2.0f; }

	m_fCameraAngle = fAngle - m_rot.y;

	// 差分の調節
	if (m_fCameraAngle > D3DX_PI) { m_fCameraAngle -= D3DX_PI * 2.0f; }
	if (m_fCameraAngle < -D3DX_PI) { m_fCameraAngle += D3DX_PI * 2.0f; }

	D3DXVECTOR3 rot = m_pModel[1]->GetRot();

	//if(m_fCameraAngle + (D3DX_PI * 0.25f) < rot.y || m_fCameraAngle + (D3DX_PI * -0.25f) > rot.y)
	{// 可動域外になった
		if (m_fRotDest <= D3DX_PI * 0.5f && m_fRotDest >= D3DX_PI * -0.5f)
		{// 下半身の動きを進行方向に合わせる
			D3DXVECTOR3 rot = m_pModel[0]->GetRot();
			m_pModel[0]->SetRot(D3DXVECTOR3(rot.x, m_rot.y + m_fCameraAngle, rot.z));
			rot = m_pModel[1]->GetRot();
			rot = D3DXVECTOR3(-rotCamera.x + (D3DX_PI * 0.5f), m_fCameraAngle - fAngle, rot.z);
			m_pModel[1]->SetRot(rot);
		}
		else
		{// 斜め後ろ向きのとき
			D3DXVECTOR3 rot = m_pModel[1]->GetRot();
			rot = D3DXVECTOR3(-rotCamera.x + (D3DX_PI * 0.5f), m_fCameraAngle - (fAngle - D3DX_PI), rot.z);
			m_pModel[1]->SetRot(rot);
		}
	}
	//else
	//{// 可動域内
	//	if (m_fRotDest <= D3DX_PI * 0.5f && m_fRotDest >= D3DX_PI * -0.5f)
	//	{// 下半身の動きを進行方向に合わせる
	//		D3DXVECTOR3 rot = m_pModel[0]->GetRot();
	//		m_pModel[0]->SetRot(D3DXVECTOR3(rot.x, m_fCameraAngle - m_rot.y, rot.z));
	//		rot = m_pModel[1]->GetRot();
	//		m_pModel[1]->SetRot(D3DXVECTOR3(-rotCamera.x + (D3DX_PI * 0.5f), m_fCameraAngle - fAngle, rot.z));
	//	}
	//	else
	//	{// 斜め後ろ向きのとき
	//		D3DXVECTOR3 rot = m_pModel[1]->GetRot();
	//		m_pModel[1]->SetRot(D3DXVECTOR3(-rotCamera.x + (D3DX_PI * 0.5f), m_fCameraAngle - (fAngle - D3DX_PI), rot.z));
	//	}
	//	CDebugProc::Print("上半身角度:%.2f", m_pModel[1]->GetRot().y);

	//}
}

//=========================================
// 地面の起伏に乗る処理
//=========================================
void CPlayer::FieldWalk(void)
{
	// 地面のオブジェクトを探す
	CScene *pScene = CScene::GetSceneTop(MESHFIELD_PRIORITY);
	CScene *pSceneNext = NULL;
	while (NULL != pScene)
	{// NULLまでまわす
		pSceneNext = pScene->GetSceneNext();				// 次のオブジェクトを保管
		CScene::OBJTYPE objType = pScene->GetObjType();		// オブジェクトの種類を取得

		if (objType == CScene::OBJTYPE_FIELD)
		{// 地面だったとき

		}
	}
}

//=========================================
// ダメージを受けたときの処理
//=========================================
void CPlayer::Damage(int nDamage)
{
	if (NULL != m_pUpperMotion && NULL != m_pLowerMotion)
	{// モーションクラスが使われている
		if (m_pUpperMotion->GetType() != CMotionManager::TYPE_DAMAGE && m_pLowerMotion->GetType() != CMotionManager::TYPE_DAMAGE)
		{// ライフクラスが使われている
			m_pUpperMotion->SetMotion(CMotionManager::TYPE_DAMAGE);	// ダメージモーションを再生
			m_pLowerMotion->SetMotion(CMotionManager::TYPE_DAMAGE);	// ダメージモーションを再生

			m_state = STATE_DAMAGE;								// ダメージを受けている状態にする
			CSound *pSound = CManager::GetSound();				// サウンドの取得
			pSound->PlaySound(CSound::SOUND_LABEL_DAMAGE);		// ダメージ音を再生
		}
	}
}

//=========================================
// 頂点の最大値を取得
//=========================================
D3DXVECTOR3 CPlayer::GetVtxMax(void)
{
	if (NULL != m_pModel) { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }

	D3DXVECTOR3 vtxMax = D3DXVECTOR3(-10000.0f, -10000.0f, -10000.0f);
	for (int nCntParts = 0; nCntParts < m_nNumParts; nCntParts++)
	{// 全パーツを見る
		D3DXVECTOR3 vtx = m_pModel[nCntParts]->GetVtxMax();

		if (vtx.x > vtxMax.x) { vtxMax.x = vtx.x; }
		if (vtx.y > vtxMax.y) { vtxMax.y = vtx.y; }
		if (vtx.z > vtxMax.z) { vtxMax.z = vtx.z; }
	}

	return vtxMax;
}

//=========================================
// スクリーン座標をワールド座標に変換
//=========================================
D3DXVECTOR3 CPlayer::CalcScreenToWorld(float fScreenX, float fScreenY)
{
	// 各行列の逆行列を算出
	D3DXMATRIX InvView, InvPrj, VP, InvViewport;
	CCamera *pCamera = CManager::GetCamera();
	D3DXMatrixInverse(&InvView, NULL, &pCamera->GetView());
	D3DXMatrixInverse(&InvPrj, NULL, &pCamera->GetProjection());
	D3DXMatrixIdentity(&VP);
	VP._11 = SCREEN_WIDTH / 2.0f; VP._22 = -SCREEN_HEIGHT / 2.0f;
	VP._41 = SCREEN_WIDTH / 2.0f; VP._42 = SCREEN_HEIGHT / 2.0f;
	D3DXMatrixInverse(&InvViewport, NULL, &VP);

	// 逆変換
	D3DXMATRIX tmp = InvViewport * InvPrj * InvView;
	D3DXVECTOR3 pos;
	D3DXVec3TransformCoord(&pos, &D3DXVECTOR3(fScreenX, fScreenY, 1.0f), &tmp);

	return pos;
}

//=========================================
// リロード処理
//=========================================
void CPlayer::Reload(void)
{
	//****************************************
	// 弾が0ー＞リロードロゴ、ゲージ表示
	//****************************************
	if (m_nRemBullet <= 0)
	{
		m_nCntReRoad++;		// カウンター加算

							//****************************************
							// NULLチェックからのリロードロゴ生成
							//****************************************
		if (m_pUITex[0] == NULL)
		{	// 弾のところ
			m_pUITex[0] = CUI_TEXTURE::Create(D3DXVECTOR3(1100.0f, 650.0f, 0.0f), 200.0f, 80.0f, CUI_TEXTURE::UIFLAME_REROAD);
		}
		if (m_pUITex[1] == NULL)
		{	// 画面中央
			m_pUITex[1] = CUI_TEXTURE::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), 200.0f, 80.0f, CUI_TEXTURE::UIFLAME_REROAD);
		}

		// リロードロゴ点滅
		m_pUITex[0]->Flashing(30);
		m_pUITex[1]->Flashing(30);

		//****************************************
		//ゲージの生成
		//****************************************
		if (m_pGauge == NULL)
		{
			m_pGauge = CGauge2D::Create(2, D3DXVECTOR3(SCREEN_WIDTH / 2, 400.0f, 0.0f), 0.0f, 100.0f, 300.0f, 30.0f);
			m_pGauge->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), 0);	// 元の長さ
			m_pGauge->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 1);	// 現在の体力
			//m_pGauge->BindTexture(CTexture::GetTexture(CTexture::TEXTURE_GAUGE), 0);		// 元の長さ
			//m_pGauge->BindTexture(CTexture::GetTexture(CTexture::TEXTURE_GAUGE1), 1);	//  現在の体力
		}

		// ゲージ増加
		m_pGauge->AddSubtract(100.0f / RELOAD_TIME);		// 100％ / 秒数

															//*******************************************
															// 設定した秒数後→ロゴ＆ゲージ破棄＆弾生成
															//*******************************************
		if (m_nCntReRoad % RELOAD_TIME == 0)
		{
			// 弾生成
			m_nRemBullet = PLAYER_BULLET;

			if (m_pGauge != NULL)
			{	// ゲージの破棄
				m_pGauge->Uninit();
				m_pGauge = NULL;
			}

			for (int nCnt = 0; nCnt < MAX_UITEX; nCnt++)
			{	// リロードロゴの破棄
				if (m_pUITex[nCnt] != NULL)
				{
					m_pUITex[nCnt]->Uninit();
					m_pUITex[nCnt] = NULL;
				}
			}
		}
	}
}
