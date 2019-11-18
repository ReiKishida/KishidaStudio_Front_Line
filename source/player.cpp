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
#include "server.h"
#include "collisionSet.h"
#include "mouseCursor.h"
#include "button.h"
#include "menu.h"
#include "AI.h"

//==================================
// マクロ定義
//==================================
#define ASSULT_FILE					"data/TEXT/PLAYER/assult/model_assult.txt"
#define LIGHT_FILE					"data/TEXT/PLAYER/light/model_light.txt"
#define HEAVY_FILE					"data/TEXT/PLAYER/heavy/model_heavy.txt"
#define SNIPE_FILE					"data/TEXT/PLAYER/snipe/model_snipe.txt"

#define PLAYER_DAMAGE_TIME			(60)		// ダメージを受けた時の無敵時間
#define PLAYER_DAMAGE_MOVE			(40)		// ダメージを受けてから動けるようになるまでの時間
#define PLAYER_RETICLE_LENGTH		(2500.0f)	// レティクルの距離
#define ANIM_SPEED							(10)		// タイルアニメーション
#define ANIM_PATTERN						(8)		// タイルのパターン数
#define PLAYER_BOTTON_WIDTH	(75.0f)	// リスポーンボタンの横幅
#define PLAYER_BOTTON_HEIGHT	(75.0f)	// リスポーンボタンの縦幅
#define PLAYER_BOTTON_INT			(40.0f)	// ボタンとボタンの間隔
#define PLAYER_UI_HEIGHT				(630.0f)

//==================================
// 静的メンバ変数宣言
//==================================

//==================================
// 生成処理
//==================================
CPlayer* CPlayer::Create(int nPlayerIdx, CMechaSelect::MECHATYPE mecha, D3DXVECTOR3 pos)
{
	CPlayer *pPlayer;

	pPlayer = new CPlayer;	// メモリを確保

	if (pPlayer != NULL)
	{// メモリ確保成功
		pPlayer->SetPlayerIdx(nPlayerIdx);
		pPlayer->m_mecha = mecha;
		pPlayer->m_pos = pos;
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
	m_nPlayerIdx = 0;
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_bShoot = false;
	m_vtxMax = D3DXVECTOR3(-1000.0f, -1000.0f, -1000.0f);
	m_vtxMin = D3DXVECTOR3(1000.0f, 1000.0f, 1000.0f);
	m_pCursor = NULL;
	m_nLife = 0;
	m_nTimer = 0;
	m_nDisTime = 0;
	m_mecha = CMechaSelect::MECHATYPE_ASSULT;
	m_nTeam = 0;
	m_bDeath = false;
	m_pGauge = NULL;
	m_nCntReRoad = 0;

	for (int nCnt = 0; nCnt < PLAYER_UI_NUM; nCnt++)
	{
		m_pUINum[nCnt] = NULL;
	}

	for (int nCnt = 0; nCnt < PLAYER_BOTTON; nCnt++)
	{
		m_apButtonUI[nCnt] = NULL;
	}

	for (int nCnt = 0; nCnt < MAX_UITEX; nCnt++)
	{// ポインタの3つ目から使用しているため、初期値は2
		m_pUITex[nCnt] = NULL;
	}

	// カウンターの初期化
	m_nCntAnim = 0;
	m_nPatternAnim = 0;
	m_nCntShoot = 0;
	m_bShootButton = false;
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
	int nFootIdx = 0;
	float fX, fY, fZ;
	int nId;
	int nParent;
	char **pModelName = NULL;
	char *pFileName = "";

	if (CMechaSelect::MECHATYPE_ASSULT == m_mecha) { pFileName = ASSULT_FILE; }
	else if (CMechaSelect::MECHATYPE_LIGHT == m_mecha) { pFileName = LIGHT_FILE; }
	else if (CMechaSelect::MECHATYPE_HEAVY == m_mecha) { pFileName = HEAVY_FILE; }
	else if (CMechaSelect::MECHATYPE_SHOOTER == m_mecha) { pFileName = SNIPE_FILE; }

	// ファイルを開く
	pFile = fopen(pFileName, "r");

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
							else if (strcmp(aStr, "CAPACITY") == 0)
							{// 総弾数
								fscanf(pFile, " = %d", &m_nCapacity);
							}
							else if (strcmp(aStr, "ATTACK") == 0)
							{// 攻撃力
								fscanf(pFile, " = %d", &m_nAttack);
							}
							else if (strcmp(aStr, "SHOOTS") == 0)
							{// 同時発射数
								fscanf(pFile, " = %d", &m_nNumShoot);
								m_pAngle = new float[m_nNumShoot * 2];
								m_pAngleV = new float[m_nNumShoot * 2];
							}
							else if (strcmp(aStr, "DISPERTION") == 0)
							{// ばらつき
								fscanf(pFile, " = %d", &m_nDispertion);
							}
							else if (strcmp(aStr, "RELOAD") == 0)
							{// リロード時間
								fscanf(pFile, " = %d", &m_nReload);
							}
							else if (strcmp(aStr, "LIFE") == 0)
							{// 耐久力
								fscanf(pFile, " = %d", &m_nLifeMax);
							}
							else if (strcmp(aStr, "SPEED") == 0)
							{// 移動量
								fscanf(pFile, " = %f", &m_fSpeed);
							}
							else if (strcmp(aStr, "FOOT") == 0)
							{// 足以下の番号
								fscanf(pFile, " = %d", &nFootIdx);
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
											m_pModel[nCntParts]->Init();
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
		m_pUpperMotion = CMotion::Create(m_mecha, nFootIdx, m_pModel);
		m_pUpperMotion->SetMotion(CMotionManager::TYPE_NEUTRAL);	// ニュートラルモーションを設定
	}

	if (NULL == m_pLowerMotion)
	{// モーションクラスの生成
		m_pLowerMotion = CMotion::Create(m_mecha, m_nNumParts, m_pModel, nFootIdx);
		m_pLowerMotion->SetMotion(CMotionManager::TYPE_NEUTRAL);	// ニュートラルモーションを設定
	}

	if (NULL == m_pShadow)
	{// 影の生成
		m_pShadow = CShadow::Create(&m_pos);
	}

	if (CMenu::GetMode() == CMenu::MODE_MULTI)
	{// マルチモード
		CClient *pClient = CManager::GetClient();

		if (m_nPlayerIdx == pClient->GetPlayerIdx())
		{
			if (NULL == m_pReticle)
			{// レティクルの生成
				m_pReticle = CScene3DBill::Create();
				m_pReticle->BindTexture(CTexture::GetTexture(CTexture::TEXTURE_RETICLE));
				m_pReticle->SetSize(D3DXVECTOR3(50.0f, 50.0f, 0.0f));
				m_pReticle->SetLighting(false);
				m_pReticle->SetZBuffer(D3DCMP_ALWAYS);
				m_pReticle->SwapPriority(6);
			}
		}
	}
	else
	{// シングルモード
		if (NULL == m_pReticle && m_nPlayerIdx == 0)
		{// レティクルの生成
			m_pReticle = CScene3DBill::Create();
			m_pReticle->BindTexture(CTexture::GetTexture(CTexture::TEXTURE_RETICLE));
			m_pReticle->SetSize(D3DXVECTOR3(50.0f, 50.0f, 0.0f));
			m_pReticle->SetLighting(false);
			m_pReticle->SetZBuffer(D3DCMP_ALWAYS);
			m_pReticle->SwapPriority(6);
		}
	}

	m_nRemBullet = m_nCapacity;

	// 頂点座標の最小値と最大値を求める
	D3DXVECTOR3 pos, posMax, posMin;
	D3DXVECTOR3 posMaxVtx, posMinVtx;
	posMax = m_vtxMax;
	posMin = m_vtxMin;
	for (int nCntModel = 0; nCntModel < m_nNumParts; nCntModel++)
	{
		pos = m_pModel[nCntModel]->GetPos();
		posMaxVtx = m_pModel[nCntModel]->GetVtxMax() + pos;
		posMinVtx = m_pModel[nCntModel]->GetVtxMin() + pos;

		// Xの値の比較
		if (posMin.x > posMinVtx.x)
		{// 最小値と比較
			posMin.x = posMinVtx.x;	// 最小値より小さければ代入
		}

		if (posMax.x < posMaxVtx.x)
		{// 最大値と比較
			posMax.x = posMaxVtx.x;	// 最大値より大きければ代入
		}

		// Yの値の比較
		if (posMin.y > posMinVtx.y)
		{// 最小値と比較
			posMin.y = posMinVtx.y;	// 最小値より小さければ代入
		}

		if (posMax.y < posMaxVtx.y)
		{// 最大値と比較
			posMax.y = posMaxVtx.y;	// 最大値より大きければ代入
		}

		// Zの値の比較
		if (posMin.z > posMinVtx.z)
		{// 最小値と比較
			posMin.z = posMinVtx.z;	// 最小値より小さければ代入
		}

		if (posMax.z < posMaxVtx.z)
		{// 最大値と比較
			posMax.z = posMaxVtx.z;	// 最大値より大きければ代入
		}
	}

	m_vtxMin.x = posMin.x;
	m_vtxMin.y = posMin.y;
	m_vtxMin.z = posMin.z;
	m_vtxMax.x = posMax.x;
	m_vtxMax.y = posMax.y;
	m_vtxMax.z = posMax.z;

	m_nRemBullet = m_nCapacity;		// 弾の初期値
	m_pUINum[0]->SetRemainBullet(m_nRemBullet);

	m_nLife = m_nLifeMax;	// ライフの初期値
	m_pUINum[1]->SetPlayerLife(m_nLife);

	// ゲーム開始時
	m_Respawn = RESPAWN_START;
	m_nDisTime = 1.8f;

	if (m_nPlayerIdx == 0 || m_nPlayerIdx == 1)
	{
		for (int nCntModel = 0; nCntModel < m_nNumParts; nCntModel++)
		{
			m_pModel[nCntModel]->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
		}
		m_nTeam = 0;

		// カメラの向きの設定
		CManager::GetCamera()->SetRot(D3DXVECTOR3(D3DX_PI * 0.5f, D3DX_PI * 0.5f, 0.0f));
	}
	else if(m_nPlayerIdx == 2 || m_nPlayerIdx == 3)
	{
		for (int nCntModel = 0; nCntModel < m_nNumParts; nCntModel++)
		{
			m_pModel[nCntModel]->SetColor(D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
		}

		m_nTeam = 1;

		// カメラの向きの設定
		CManager::GetCamera()->SetRot(D3DXVECTOR3(D3DX_PI * 0.5f, D3DX_PI * -0.5f, 0.0f));
	}

	CAIMecha::Create(this, CAIMecha::MECHATYPE_DRONE, D3DXVECTOR3(0.0f, 70.0f, 0.0f));

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

	if (NULL != m_pAngle)
	{// 弾の角度の破棄
		delete[] m_pAngle;
		m_pAngle = NULL;
	}

	if (NULL != m_pAngleV)
	{// 弾の垂直方向角度の破棄
		delete[] m_pAngleV;
		m_pAngleV = NULL;
	}

	if (NULL != m_pCursor)
	{// カーソルの破棄
		m_pCursor->Uninit();
		m_pCursor = NULL;
	}

	// オブジェクトの破棄
	CScene::Release();
}

//=========================================
// 更新処理
//=========================================
void CPlayer::Update(void)
{
	if (m_Respawn == RESPAWN_START)
	{	// 戦闘開始 状態
		Respawn(RESPAWN_START);
	}

	if (CMenu::GetMode() == CMenu::MODE_MULTI)
	{
		if (m_nPlayerIdx != CManager::GetClient()->GetPlayerIdx())
		{// プレイヤーの番号がクライアント番号と違う場合
			if (m_pUpperMotion != NULL && m_pLowerMotion != NULL)
			{// モーションクラスが使われている
				if (m_pos != m_posOld)
				{//過去の位置と現在の位置がずれていた場合
				 //歩きモーションにする
					m_pUpperMotion->SetMotion(CMotionManager::TYPE_WALK);
					m_pLowerMotion->SetMotion(CMotionManager::TYPE_WALK);
				}
				else
				{//ニュートラルモーションにする
					m_pUpperMotion->SetMotion(CMotionManager::TYPE_NEUTRAL);
					m_pLowerMotion->SetMotion(CMotionManager::TYPE_NEUTRAL);
				}
			}
			m_posOld = m_pos;
		}
	}

	if (CMenu::GetMode() == CMenu::MODE_MULTI)
	{
		if (m_nPlayerIdx == CManager::GetClient()->GetPlayerIdx())
		{//プレイヤーの番号がクライアント番号と同じ場合
			m_pReticle->SetDisp(false);

			if (CManager::GetGame()->GetPart() == CGame::PART_ACTION)
			{// アクションパート
				CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();	// キーボードの入力を取得
				CXInput *pXInput = CManager::GetXInput();					// XInputの入力を取得

				if (m_nLife >= 0 && m_Respawn == RESPAWN_NONE)
				{	// ライフある && 戦闘開始状態の時
					if (m_nDiff > 0)
					{	// ダメージ量が0以上の時
						m_nLife--;
						m_nDiff--;
					}
					if (m_nLife <= 0)
					{	// ライフがなくなった
						m_Respawn = RESPAWN_DEATH;		// 行動不能状態
					}

					// 移動の処理
					Movement();

					// 弾を撃つ
					Shoot();

					D3DXVECTOR3 rotCamera = CManager::GetCamera()->GetRot();
					D3DXVECTOR3 posR = CManager::GetCamera()->GetPosR();

					m_pReticle->SetDisp(true);
					m_pReticle->SetPos(D3DXVECTOR3(sinf(rotCamera.y) * PLAYER_RETICLE_LENGTH, cosf(rotCamera.x) * PLAYER_RETICLE_LENGTH, cosf(rotCamera.y) * PLAYER_RETICLE_LENGTH) + D3DXVECTOR3(m_pos.x, 0.0f, m_pos.z));

					for (int nCntModel = 0; nCntModel < m_nNumParts; nCntModel++)
					{// パーツモデルの更新
						m_pModel[nCntModel]->Update();
					}
					CDebugProc::Print("位置：%.2f %.2f %.2f", m_pos.x, m_pos.y, m_pos.z);
				}
				if (m_Respawn == RESPAWN_DEATH)
				{	// 行動不能状態
					Respawn(m_Respawn);		// リスポーン処理
				}
				if (m_Respawn == RESPAWN_SELECT)
				{	// リスポーン選択状態
					SelectRespawn();				// リスポーン位置選択
				}

				// 角度の更新
				Angle();

				// ライフの設定
				m_pUINum[1]->SetPlayerLife(m_nLife);
			}
		}
	}
	else if(CMenu::GetMode() == CMenu::MODE_SINGLE)
	{
		if (m_nPlayerIdx == 0)
		{
			m_pReticle->SetDisp(false);

			if (CManager::GetGame()->GetPart() == CGame::PART_ACTION)
			{// アクションパート
				CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();	// キーボードの入力を取得
				CXInput *pXInput = CManager::GetXInput();					// XInputの入力を取得

				if (m_nLife >= 0 && m_Respawn == RESPAWN_NONE)
				{	// ライフある && 戦闘開始状態の時
					if (m_nDiff > 0)
					{	// ダメージ量が0以上の時
						m_nLife--;
						m_nDiff--;
					}
					if (m_nLife <= 0)
					{	// ライフがなくなった
						m_Respawn = RESPAWN_DEATH;		// 行動不能状態
					}

					// 移動の処理
					Movement();

					// 弾を撃つ
					Shoot();

					// 重力
					//m_move.y -= GRAVITY;

					D3DXVECTOR3 rotCamera = CManager::GetCamera()->GetRot();
					D3DXVECTOR3 posR = CManager::GetCamera()->GetPosR();

					m_pReticle->SetDisp(true);
					m_pReticle->SetPos(D3DXVECTOR3(sinf(rotCamera.y) * PLAYER_RETICLE_LENGTH, cosf(rotCamera.x) * PLAYER_RETICLE_LENGTH, cosf(rotCamera.y) * PLAYER_RETICLE_LENGTH) + D3DXVECTOR3(m_pos.x, 0.0f, m_pos.z));

					for (int nCntModel = 0; nCntModel < m_nNumParts; nCntModel++)
					{// パーツモデルの更新
						m_pModel[nCntModel]->Update();
					}
					CDebugProc::Print("位置：%.2f %.2f %.2f", m_pos.x, m_pos.y, m_pos.z);
				}
				if (m_Respawn == RESPAWN_DEATH)
				{	// 行動不能状態
					Respawn(m_Respawn);		// リスポーン処理
				}
				if (m_Respawn == RESPAWN_SELECT)
				{	// リスポーン選択状態
					SelectRespawn();				// リスポーン位置選択
				}

				// 角度の更新
				Angle();

				// ライフの設定
				m_pUINum[1]->SetPlayerLife(m_nLife);
			}
		}

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
	{// モデルの描画処理
		m_pModel[nCntModel]->Draw();
	}
}

//=========================================
// 移動の処理
//=========================================
void CPlayer::Movement(void)
{
	m_posOld = m_pos;
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
			if (fDirMove >= D3DX_PI * 0.75f || fDirMove <= D3DX_PI * -0.75f)
			{// バックラン
				m_pUpperMotion->SetMotion(CMotionManager::TYPE_BACK);
				m_pLowerMotion->SetMotion(CMotionManager::TYPE_BACK);
			}
			else
			{// 通常移動
				m_pUpperMotion->SetMotion(CMotionManager::TYPE_WALK);
				m_pLowerMotion->SetMotion(CMotionManager::TYPE_WALK);
			}
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

	D3DXVECTOR3 pos = m_pos + m_move;

	// マップの当たり判定
	if (CCollision::Collision(&pos, m_posOld, m_vtxMax, m_vtxMin)) { m_pos = pos; }
	else{ m_pos += m_move; }
}

//=========================================
// 弾を撃つ処理
//=========================================
void CPlayer::Shoot(void)
{
	CInputMouse *pMouse = CManager::GetInputMouse();	// マウスの入力を取得
	CXInput *pXInput = CManager::GetXInput();			// XInputの入力を取得
	D3DXVECTOR3 dispertion;								// ブレ

	if (pMouse->GetPress(CInputMouse::DIMS_BUTTON_0) && m_nRemBullet > 0)
	{
		// 弾の発射間隔
		m_nCntShoot = (m_nCntShoot + 1) % 7;

		if ((m_mecha != CMechaSelect::MECHATYPE_LIGHT && !m_bShootButton) || (m_mecha == CMechaSelect::MECHATYPE_LIGHT &&m_nCntShoot == 1))
		{
			for (int nCntShoots = 0; nCntShoots < m_nNumShoot; nCntShoots++)
			{
				// カメラの角度と注視点を取得
				D3DXVECTOR3 rotCamera = CManager::GetCamera()->GetRot();
				D3DXVECTOR3 posR = CManager::GetCamera()->GetPosR();

				// レティクル（目的の位置）の取得
				D3DXVECTOR3 posReticle = m_pReticle->GetPos();
				if (m_nDispertion != 0)
				{// ブレが０でないとき
					dispertion = D3DXVECTOR3((float)(m_nDispertion - rand() % (m_nDispertion * 2)), 0.0f, (float)(m_nDispertion - rand() % (m_nDispertion * 2)));
					posReticle += dispertion;
				}

				// 射出口の位置の取得
				D3DXMATRIX mtxCanon = m_pModel[2]->GetMtxWorld();
				D3DXVECTOR3 posCanon = D3DXVECTOR3(mtxCanon._41, mtxCanon._42, mtxCanon._43) + D3DXVECTOR3(sinf(rotCamera.y) * 30.0f, cosf(rotCamera.x) * 30.0f, cosf(rotCamera.y) * 30.0f);

				// 水平方向の角度の計算
				m_pAngle[nCntShoots * 2] = atan2f(posReticle.x - posCanon.x, posReticle.z - posCanon.z);
				m_pAngleV[nCntShoots * 2] = rotCamera.x;
				if (m_nDispertion != 0) { m_pAngleV[nCntShoots * 2] += (float)(m_nDispertion - (rand() % m_nDispertion * 2)) * 0.0005f; }

				// 弾の生成
				CBulletPlayer::Create(posCanon, m_pAngle[nCntShoots * 2], m_pAngleV[nCntShoots * 2], m_nAttack, m_nTeam);

				// レティクル（目的の位置）の取得
				posReticle = m_pReticle->GetPos();
				if (m_nDispertion != 0)
				{// ブレが０でないとき
					dispertion = D3DXVECTOR3((float)(m_nDispertion - rand() % (m_nDispertion * 2)), 0.0f, (float)(m_nDispertion - rand() % (m_nDispertion * 2)));
					posReticle += dispertion;
				}

				// 射出口の位置の取得
				mtxCanon = m_pModel[3]->GetMtxWorld();
				posCanon = D3DXVECTOR3(mtxCanon._41, mtxCanon._42, mtxCanon._43) + D3DXVECTOR3(sinf(rotCamera.y) * 30.0f, cosf(rotCamera.x) * 30.0f, cosf(rotCamera.y) * 30.0f);

				// 水平方向の角度の計算
				m_pAngle[nCntShoots * 2 + 1] = atan2f(posReticle.x - posCanon.x, posReticle.z - posCanon.z);
				m_pAngleV[nCntShoots * 2 + 1] = rotCamera.x;
				if (m_nDispertion != 0) { m_pAngleV[nCntShoots * 2 + 1] += (float)(m_nDispertion - (rand() % m_nDispertion * 2)) * 0.0005f; }

				// 弾の生成
				CBulletPlayer::Create(posCanon, m_pAngle[nCntShoots * 2 + 1], m_pAngleV[nCntShoots * 2 + 1], m_nAttack, m_nTeam);

				m_bShoot = true;
			}

			m_nRemBullet--;
			m_bShootButton = true;
		}
	}
	else
	{
		// 発射ボタン押下フラグを負にする
		m_bShootButton = false;
	}

	// リロード処理
	Reload();

	// 残弾の設定
	m_pUINum[0]->SetRemainBullet(m_nRemBullet);
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

			m_nLife -= nDamage;

			if (0 >= m_nLife)
			{
				m_nLife = 0;
				m_bDeath = true;
			}
			//CSound *pSound = CManager::GetSound();				// サウンドの取得
			//pSound->PlaySound(CSound::SOUND_LABEL_DAMAGE);		// ダメージ音を再生
		}
	}
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
			m_pUITex[0] = CUI_TEXTURE::Create(D3DXVECTOR3(1100.0f, 650.0f, 0.0f), 200.0f, 80.0f, CUI_TEXTURE::UIFLAME_RELOAD);
		}
		if (m_pUITex[1] == NULL)
		{	// 画面中央
			m_pUITex[1] = CUI_TEXTURE::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), 200.0f, 80.0f, CUI_TEXTURE::UIFLAME_RELOAD);
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
		}

		// ゲージ増加
		m_pGauge->AddSubtract(100.0f / m_nReload);		// 100％ / 秒数

															//*******************************************
															// 設定した秒数後→ロゴ＆ゲージ破棄＆弾生成
															//*******************************************
		if (m_nCntReRoad % m_nReload == 0)
		{
			// 弾生成
			m_nRemBullet = m_nCapacity;

			if (m_pGauge != NULL)
			{	// ゲージの破棄
				m_pGauge->Uninit();
				m_pGauge = NULL;
			}

			for (int nCnt = 0; nCnt < 2; nCnt++)
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

//=========================================
// リスポーン処理
//=========================================
void CPlayer::Respawn(RESPAWN respawn)
{
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();	// キーボードの入力を取得
	CXInput *pXInput = CManager::GetXInput();					// XInputの入力を取得

	switch (respawn)
	{
	case RESPAWN_START:
		if (m_pUITex[2] == NULL || m_pUITex[3] == NULL)
		{	// 生成する
			m_pUITex[2] = CUI_TEXTURE::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), 500.0f, 260.0f, CUI_TEXTURE::UIFLAME_TILE_PATTERN);	// タイル
			m_pUITex[2]->SetTex(0, 1, 8);		// 初期タイルパターン
			m_pUITex[3] = CUI_TEXTURE::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), 350.0f, 150.0f, CUI_TEXTURE::UIFLAME_START);	// 戦闘開始ロゴ
		}
		if (m_pUITex[2] != NULL || m_pUITex[3] != NULL)
		{	// 生成された
			m_nCntAnim++;		// アニメーション
			m_nTimer++;			// ロゴが消えるまでのタイマー
			if ((m_nCntAnim % ANIM_SPEED) == 0)
			{	// アニメーション処理
				m_nPatternAnim++;		// アニメーションパターン数
				m_pUITex[2]->SetTex(m_nPatternAnim, 1, ANIM_PATTERN);

				if (m_nPatternAnim > (ANIM_PATTERN - 1))
				{	// 8パターン目まできたら、強制的に8パターン目にする
					m_pUITex[2]->SetTex((ANIM_PATTERN - 1), 1, ANIM_PATTERN);

					if (m_nTimer % 60 == 0)
					{	// ロゴが消えるまでの時間
						m_nDisTime--;
					}
					if (m_nDisTime <= 0)
					{	// 0になった
						for (int nCnt = 2; nCnt < MAX_UITEX; nCnt++)
						{	// ポインタの3つ目から使用しているため、初期値は2
							if (m_pUITex[nCnt] != NULL)
							{	// UIテクスチャの破棄
								m_pUITex[nCnt]->Uninit();
								m_pUITex[nCnt] = NULL;
							}
						}
						respawn = RESPAWN_NONE;		// 通常状態
						m_nCntAnim = 0;
						m_nPatternAnim = 0;
					}	// 0になった時
				}	// パターン数最大まできた
			}	// アニメーションの処理
		}	// 生成された
		break;

	case RESPAWN_DEATH:
		if (m_pUITex[2] == NULL || m_pUITex[3] == NULL || m_pUITex[4] == NULL || m_pUINum[2] == NULL)
		{	// NULLの時、生成＆最初のテクスチャを設定
			m_pUITex[2] = CUI_TEXTURE::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), 500.0f, 260.0f, CUI_TEXTURE::UIFLAME_TILE_PATTERN);	// タイル
			m_pUITex[2]->SetTex(0, 1, 8);		// 初期タイルパターン
			m_pUITex[3] = CUI_TEXTURE::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), 350.0f, 150.0f, CUI_TEXTURE::UIFLAME_DEATH);	// 行動不能ロゴ
			m_pUITex[4] = CUI_TEXTURE::Create(D3DXVECTOR3(560.0f, 460.0f, 0.0f), 200.0f, 30.0f, CUI_TEXTURE::UIFLAME_RESPAWN);	// 戦線復帰
			m_pUINum[2] = CUI_NUMBER::Create(D3DXVECTOR3(750.0f, 460.0f, 0.0f), 60.0f, 60.0f, 30.0f, CUI_NUMBER::UI_NUMTYPE_CNTRESPAWN, 0, NUMTEX_UV_X, NUMTEX_UV_Y);	// 戦線復帰カウンター
		}
		if (m_pUITex[2] != NULL || m_pUITex[3] != NULL || m_pUITex[4] != NULL || m_pUINum[2] != NULL)
		{	// 生成している時
			m_nCntAnim++;
			if ((m_nCntAnim % ANIM_SPEED) == 0)
			{	// アニメーション処理
				m_nPatternAnim++;
				m_pUITex[2]->SetTex(m_nPatternAnim, 1, ANIM_PATTERN);

				if (m_nPatternAnim > (ANIM_PATTERN - 1))
				{	// 8パターン目まできたら、強制的に8パターン目にする
					m_pUITex[2]->SetTex((ANIM_PATTERN - 1), 1, ANIM_PATTERN);
				}
			}

			// CUI_NUMBERから、戦線復帰カウンター取得
			int nRespawn = m_pUINum[2]->GetRespawn();
			if (nRespawn <= 0)
			{	// カウンター0以下になった時
				respawn = RESPAWN_SELECT;		// リスポーン選択状態に設定

				for (int nCnt = 2; nCnt < MAX_UITEX; nCnt++)
				{	// ポインタの3つ目から使用しているため、初期値は2
					if (m_pUITex[nCnt] != NULL)
					{	// UIテクスチャの破棄
						m_pUITex[nCnt]->Uninit();
						m_pUITex[nCnt] = NULL;
					}
				}

				if (m_pUINum[2] != NULL)
				{	// 戦線復帰カウンターの破棄
					m_pUINum[2]->Uninit();
					m_pUINum[2] = NULL;
				}

				// カウンターの初期化
				m_nCntAnim = 0;
				m_nPatternAnim = 0;
			}
		}
		break;
	}

	m_Respawn = respawn;

#ifdef _DEBUG
	CDebugProc::Print("m_nCntAnim : %d\n", m_nCntAnim);
	CDebugProc::Print("m_nPatternAnim : %d\n", m_nPatternAnim);
	CDebugProc::Print("m_nDisTime : %d\n", m_nDisTime);
	CDebugProc::Print("m_nTimer : %d\n", m_nTimer);
#endif
}

//=========================================
// リスポーン選択処理
//=========================================
void CPlayer::SelectRespawn(void)
{
	if (m_pUITex[5] == NULL || m_pUITex[6] == NULL || m_pUITex[7] == NULL || m_pUITex[5] == NULL || m_pCursor == NULL)
	{	// UIの生成
		m_pUITex[5] = CUI_TEXTURE::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), SCREEN_WIDTH, SCREEN_HEIGHT, CUI_TEXTURE::UIFLAME_NONE);	// 下地
		m_pUITex[6] = CUI_TEXTURE::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, 360.0f, 0.0f), 500.0f, 370.0f, CUI_TEXTURE::UIFLAME_MAP);	// マップ
		m_pUITex[7] = CUI_TEXTURE::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, 120.0f, 0.0f), 400.0f, 80.0f, CUI_TEXTURE::UIFLAME_TITLE);	// リスポーンタイトル
		m_pUITex[8] = CUI_TEXTURE::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, PLAYER_UI_HEIGHT, 0.0f), 500.0f, 100.0f, CUI_TEXTURE::UIFLAME_RESPAWN_FLAME);		// フレーム
		m_pCursor = CMouseCursor2D::Create();		// カーソル
	}

	for (int nCnt = 0; nCnt < PLAYER_BOTTON; nCnt++)
	{
		if (m_apButtonUI[nCnt] == NULL)
		{	// ボタンの生成
			m_apButtonUI[nCnt] = CButton2D::Create(D3DXVECTOR3(470.0f + (nCnt *(PLAYER_BOTTON_WIDTH + PLAYER_BOTTON_INT)), PLAYER_UI_HEIGHT, 0.0f),
				PLAYER_BOTTON_WIDTH, PLAYER_BOTTON_HEIGHT);  	// リスポーンボタンの横幅
			m_apButtonUI[nCnt]->BindTexture(CTexture::GetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_SELECT_RESPAWN)));
			m_apButtonUI[nCnt]->SetTex(nCnt, 1, 4);
		}
	}

	if (m_apButtonUI[0] != NULL || m_apButtonUI[1] != NULL || m_apButtonUI[2] != NULL || m_apButtonUI[3] != NULL)
	{	// 生成されていた時
		bool bBottonSwitch = false;
		int nSelect = -1;
		POINT point = POINT_A;

		// ボタンの判定
		for (int nCntButton = 0; nCntButton < PLAYER_BOTTON; nCntButton++)
		{
			if (m_apButtonUI[nCntButton]->InRange(m_pCursor->GetMousePosition()))
			{// 範囲内かチェック
				if (m_apButtonUI[nCntButton]->ClickRelease())
				{// クリックされた
					bBottonSwitch = true;
					point = (POINT)nCntButton;
					break;
				}
				nSelect = nCntButton;
			}
		}

		// ボタン押された時の処理
		if (bBottonSwitch)
		{	// trueの時
			switch (point)
			{
			case POINT_A:
				break;

			case POINT_B:
				break;

			case POINT_C:
				break;

			case POINT_D:
				break;
			}

			// 通常状態に戻る
			m_Respawn = RESPAWN_NONE;

			// ライフの設定
			m_nLife = m_nLifeMax;

			// リスポーン地点が決定したら破棄する
			for (int nCnt = 0; nCnt < PLAYER_BOTTON; nCnt++)
			{
				if (m_apButtonUI[nCnt] != NULL)
				{	// ボタンの破棄
					m_apButtonUI[nCnt]->Uninit();
					m_apButtonUI[nCnt] = NULL;
				}
			}

			for (int nCnt = 5; nCnt < MAX_UITEX; nCnt++)
			{
				if (m_pUITex[nCnt] != NULL)
				{	// ポインタの6つ目から使用しているため、初期値は5
					m_pUITex[nCnt]->Uninit();
					m_pUITex[nCnt] = NULL;
				}
			}

			if (m_pCursor != NULL)
			{	// カーソルの破棄
				m_pCursor->Uninit();
				m_pCursor = NULL;
			}
		}	// ボタン押された時
	}	// 生成された

#ifdef _DEBUG
	CDebugProc::Print("リスポーン選択中");
#endif
}