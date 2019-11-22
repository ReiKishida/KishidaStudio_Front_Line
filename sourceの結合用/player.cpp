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
#include "stage.h"
#include "modelSet.h"
#include "fade.h"
#include "life.h"
#include "particle.h"
#include "sound.h"
#include "scene3D.h"
#include "modelSet.h"
#include "file.h"

//==================================
// マクロ定義
//==================================
#define MODEL_FILE				"data/TEXT/PLAYER/model.txt"
#define JUDG_ICON_TEXTURE		"data/TEXTURE/judg_icon.png"
#define MOVE_SPEED				(3.0f)		// 移動の速度
#define PLAYER_SLIP_ANGLE		(0.8f)		// 滑るようになる角度
#define JUMP_SPEED				(4.0f)		// ジャンプの速度
#define SLIP_ENABLE_SPEED		(1.0f)		// 滑るようになる速度
#define PLAYER_LIFE				(100)		// 体力
#define	PLAYER_LIFE_WIDTH		(500.0f)	// 体力バーの幅
#define PLAYER_LIFE_HEIGHT		(50.0f)		// 体力バーの高さ
#define PLAYER_DAMAGE_TIME		(60)		// ダメージを受けた時の無敵時間
#define PLAYER_DAMAGE_MOVE		(40)		// ダメージを受けてから動けるようになるまでの時間
#define PLAYER_CLIMB_SPEED		(1.1f)		// 登るときの速度の減衰量
#define PLAYER_SLIP_SPEED		(1.7f)		// 滑る速度
#define PLAYER_SLIP_JUMP		(0.75f)		// 吹っ飛びジャンプ速度補正
#define PLAYER_JUMP_DASH		(25.0f)		// ジャンプ中の空中ダッシュの速度補正
#define PLAYER_POS_JUMP_JUDGE	(50.0f)		// ジャンプ判定用のプレイヤーからの距離

// =============================================================
// AI関係
// =============================================================
#define	LOAD_FILENAME		("data/TEXT/NODE_DATA/NodeData.txt")	// マップデータを読み込むファイルの名前
#define POINT_SIZE_X		(18.0f)		// 仮地点用ポリゴンの大きさX
#define POINT_SIZE_Z		(17.0f)		// 仮地点用ポリゴンの大きさY
#define MOVE_ACCEPTABLE		(20.0f)		// 移動字の誤差の許容範囲
#define POS_ACCEPTABLE		(30.0f)		// 検索時の誤差の許容範囲
#define MOUSE_ACCEPTABLE	(20.0f)		// マウスの誤差の許容範囲
#define COLLECT_TIME		(5)			// データの収集を行う間隔(秒)
#define DATA_REFERENCE_TIME	(5)			// データの参照を行う間隔(回)
#define PLAYER_BREAKTIME	(1)			// 休憩時間(フレーム)
#define PLAYER_FINALPOINT_BREAKTIME	(120)	// 最終地点の休憩時間(フレーム)

//==================================
// 静的メンバ変数宣言
//==================================
D3DXVECTOR3 CPlayer::m_searchPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
CPlayer *CPlayer::m_pPlayer = NULL;
CEnemy *CPlayer::m_pEnemy[ENEMY_MAX] = {};
CScene3D *CPlayer::m_pScene3D[NODEPOINT_MAX] = {};

//==================================
// 生成処理
//==================================
CPlayer* CPlayer::Create(void)
{
	CPlayer *pPlayer;

	pPlayer = new CPlayer;	// メモリを確保
	m_pPlayer = pPlayer;

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
	m_rotDest = 0.0f;
	m_fSpeed = 0.0f;
	m_fSlipSpeed = 0.0f;
	m_bJump = false;
	m_bSlip = false;
	m_fJumpSpeed = 0.0f;
	m_pModel = NULL;
	m_nNumParts = 0;
	m_fAngleSlip = 0.0f;
	m_pMotion = NULL;
	m_pLife = NULL;
	m_bDamage = false;
	m_bMove = true;
	m_nCntParticle = 0;
	m_bDash = false;
	m_bJumpKey = false;
	m_judgJump.bJump = false;
	m_judgJump.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_judgJump.pIcon = NULL;
	m_judgJump.fAngle = 0.0f;
}

//=========================================
// デストラクタ
//=========================================
CPlayer::~CPlayer(){}

//=========================================
// 初期化処理
//=========================================
HRESULT CPlayer::Init(void)
{
	srand((unsigned int)time(0));

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
		MessageBox(0, "ファイルがありません", "player/model.txt", MB_YESNO);
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

	// ポリゴンの位置を設定
	m_fSpeed = MOVE_SPEED;	// 速度の設定

	if (NULL == m_pMotion)
	{// モーションクラスの生成
		m_pMotion = CMotion::Create(0, m_nNumParts, m_pModel);
		m_pMotion->SetMotion(CMotionManager::TYPE_NEUTRAL);	// ニュートラルモーションを設定
	}

	if (NULL == m_pShadow)
	{// 影の生成
		m_pShadow = CShadow::Create(&m_pos);
	}

	if (NULL == m_pLife && CManager::MODE_GAME == CManager::GetMode())
	{// 体力の生成
		//m_pLife = CLife2D::Create(PLAYER_LIFE, D3DXVECTOR3(10.0f, 0.0f, 0.0f), PLAYER_LIFE_WIDTH, PLAYER_LIFE_HEIGHT);
	}

	m_judgJump.pIcon = CScene3DBill::Create();
	m_judgJump.pIcon->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
	m_judgJump.pIcon->SetSize(D3DXVECTOR3(20.0f, 20.0f, 0.0f));
	m_judgJump.pIcon->SetLighting(false);

	// テクスチャの生成
	LPDIRECT3DTEXTURE9 pTexture;
	D3DXCreateTextureFromFile(pDevice, JUDG_ICON_TEXTURE, &pTexture);

	m_judgJump.pIcon->BindTexture(pTexture);

	// 数値の初期化==============================================================================
	m_pNodeData = CGame::GetFile();
	m_posDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nPoint = 0;
	m_nCountPoint = 0;
	m_nCountCollect = 0;
	m_nBreaktime = 0;
	m_nGoalCount = 0;
	m_nCollectionTimer = COLLECT_TIME * 60;
	m_bGoal = false;
	m_bPartSwitch = false;
	m_bCollectSwitch = false;

	for (int nCntEnemy = 0; nCntEnemy < ENEMY_MAX; nCntEnemy++)
	{// エネミーの最大値分回る
		for (int nCntCollect = 0; nCntCollect < COLLECTIONDATA_MAX; nCntCollect++)
		{// 収集データの最大値分回る
			m_collectionPos[nCntEnemy][nCntCollect] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		}
	}

	// マップ関係==============================================================================
	m_pNodeData->FileLoad();	// ファイル読み込み

	// 開始時点のノードの初期化
	for (int nCntNode = 0; nCntNode < m_pNodeData->GetLoadData().nodeMax; nCntNode++)
	{// ノードの数だけ回る
		if (m_pNodeData->GetLoadData().pos[nCntNode].x + POS_ACCEPTABLE > m_pos.x
			&& m_pNodeData->GetLoadData().pos[nCntNode].x - POS_ACCEPTABLE < m_pos.x
			&& m_pNodeData->GetLoadData().pos[nCntNode].z + POS_ACCEPTABLE > m_pos.z
			&& m_pNodeData->GetLoadData().pos[nCntNode].z - POS_ACCEPTABLE < m_pos.z)
		{// 誤差が+-10以内
			m_nStartNode = nCntNode;
		}
	}
	m_nEndNode = m_nMovePoint[rand() % 7];

	//// 地点可視化用
	//for (int nCntNode = 0; nCntNode < m_pNodeData->GetLoadData().nodeMax; nCntNode++)
	//{// 全ノードの数だけ回る
	//	if (m_pScene3D[nCntNode] == NULL)
	//	{// 3Dポリゴンの生成
	//		m_pScene3D[nCntNode] = m_pScene3D[nCntNode]->Create();
	//		if (m_pScene3D[nCntNode] != NULL)
	//		{// 3Dポリゴンの設定
	//			m_pNodeData->GetLoadData().pos[nCntNode].y += 3.0f;
	//			m_pScene3D[nCntNode]->SetPos(m_pNodeData->GetLoadData().pos[nCntNode]);
	//			m_pScene3D[nCntNode]->SetWidth(POINT_SIZE_X);
	//			m_pScene3D[nCntNode]->SetDepth(POINT_SIZE_Z);
	//			m_pScene3D[nCntNode]->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	//		}
	//	}
	//}

	return S_OK;
}

//=========================================
// 終了処理
//=========================================
void CPlayer::Uninit(void)
{
	if (m_pScene3D != NULL)
	{// 仮設置地点の破棄
		for (int nCntNode = 0; nCntNode < NODEPOINT_MAX; nCntNode++)
		{
			if (m_pScene3D[nCntNode] != NULL)
			{
				m_pScene3D[nCntNode]->Uninit();
				m_pScene3D[nCntNode] = NULL;
			}
		}
	}

	if (NULL != m_pMotion)
	{// モーションクラスの破棄
		m_pMotion->Uninit();
		m_pMotion = NULL;
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

	if (NULL != m_pLife)
	{// ライフの破棄
		m_pLife->Uninit();
		m_pLife = NULL;
	}

	if (NULL != m_judgJump.pIcon)
	{// ジャンプタイミング判定用アイコンの破棄
		m_judgJump.pIcon->Uninit();
	}

	// オブジェクトの破棄
	CScene::Release();
}

//=========================================
// 更新処理
//=========================================
void CPlayer::Update(void)
{
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();	// キーボードの入力を取得
	CInputMouse *pMouse = CManager::GetInputMouse();	// マウスの入力を取得
	D3DXVECTOR3 posOld = m_pos;	// 位置の保存
	bool bJumpOld = m_bJump;	// ジャンプフラグの記憶

	// 速度を入れる
	m_fSpeed = MOVE_SPEED;

	// AI関係の更新
	CPlayer::AIUpdate(pMouse);

	// 移動の処理
	//CPlayer::Movement();

	// 重力
	m_move.y -= GRAVITY;

	for (int nCntModel = 0; nCntModel < m_nNumParts; nCntModel++)
	{// パーツモデルの更新
		m_pModel[nCntModel]->Update();
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
		if (m_bDamage)
		{// ダメージを受けている状態
			m_pModel[nCntModel]->AddColor(D3DXCOLOR(0.7f, 0.0f, 0.0f, -0.5f));
		}

		// モデルの描画処理
		m_pModel[nCntModel]->Draw();

		if (m_bDamage)
		{// ダメージを受けている状態
			m_pModel[nCntModel]->AddColor(D3DXCOLOR(-0.7f, 0.0f, 0.0f, 0.5f));
		}
	}
}

//=========================================
// 移動の処理
//=========================================
void CPlayer::Movement(void)
{
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();	// キーボードの入力を取得
	bool bMoveKey = false;			// 移動キー押下フラグ

	// カメラの角度を取得
	float rotCamera = CManager::GetCamera()->GetRotY();

	if (pKeyboard->GetPress(DIK_W) == true)
	{// 上移動
		if (pKeyboard->GetPress(DIK_D) == true)
		{// 右上移動
			bMoveKey = true;
			m_rotDest = (D3DX_PI * 0.25f + rotCamera) - m_rot.y;
			m_move.x -= sinf(D3DX_PI * 0.25f + rotCamera) * m_fSpeed;
			m_move.z -= cosf(D3DX_PI * 0.25f + rotCamera) * m_fSpeed;
		}
		else if (pKeyboard->GetPress(DIK_A) == true)
		{// 左上移動
			bMoveKey = true;
			m_rotDest = (D3DX_PI * -0.25f + rotCamera) - m_rot.y;
			m_move.x += sinf(D3DX_PI * 0.75f + rotCamera) * m_fSpeed;
			m_move.z += cosf(D3DX_PI * 0.75f + rotCamera) * m_fSpeed;
		}
		else
		{// 上のみ
			bMoveKey = true;
			m_rotDest = (D3DX_PI * 0.0f + rotCamera) - m_rot.y;
			m_move.x -= sinf(D3DX_PI * 0.0f + rotCamera) * m_fSpeed;
			m_move.z -= cosf(D3DX_PI * 0.0f + rotCamera) * m_fSpeed;
		}
	}
	else if (pKeyboard->GetPress(DIK_S) == true)
	{// 下移動
		if (pKeyboard->GetPress(DIK_D) == true)
		{// 右下移動
			bMoveKey = true;
			m_rotDest = (D3DX_PI * 0.75f + rotCamera) - m_rot.y;
			m_move.x -= sinf(D3DX_PI * 0.75f + rotCamera) * m_fSpeed;
			m_move.z -= cosf(D3DX_PI * 0.75f + rotCamera) * m_fSpeed;
		}
		else if (pKeyboard->GetPress(DIK_A) == true)
		{// 左下移動
			bMoveKey = true;
			m_rotDest = (D3DX_PI * -0.75f + rotCamera) - m_rot.y;
			m_move.x += sinf(D3DX_PI * 0.25f + rotCamera) * m_fSpeed;
			m_move.z += cosf(D3DX_PI * 0.25f + rotCamera) * m_fSpeed;
		}
		else
		{// 下のみ
			bMoveKey = true;
			m_rotDest = (D3DX_PI * 1.0f + rotCamera) - m_rot.y;
			m_move.x += sinf(D3DX_PI * 0.0f + rotCamera) * m_fSpeed;
			m_move.z += cosf(D3DX_PI * 0.0f + rotCamera) * m_fSpeed;
		}
	}
	else if (pKeyboard->GetPress(DIK_D) == true)
	{// 右移動
		bMoveKey = true;
		m_rotDest = (D3DX_PI * 0.5f + rotCamera) - m_rot.y;
		m_move.x -= sinf(D3DX_PI * 0.5f + rotCamera) * m_fSpeed;
		m_move.z -= cosf(D3DX_PI * 0.5f + rotCamera) * m_fSpeed;
	}
	else if (pKeyboard->GetPress(DIK_A) == true)
	{// 左移動
		bMoveKey = true;
		m_rotDest = (D3DX_PI * -0.5f + rotCamera) - m_rot.y;
		m_move.x += sinf(D3DX_PI * 0.5f + rotCamera) * m_fSpeed;
		m_move.z += cosf(D3DX_PI * 0.5f + rotCamera) * m_fSpeed;
	}

	if (m_pMotion)
	{// モーションクラスが使われている
		if (bMoveKey && !m_bJump)
		{// 移動モーション
			m_pMotion->SetMotion(CMotionManager::TYPE_WALK);
		}
		else if (!bMoveKey && (CMotionManager::TYPE_WALK == m_pMotion->GetType() || CMotionManager::TYPE_SLIP == m_pMotion->GetType() && !m_bSlip))
		{// ニュートラルモーション
			m_pMotion->SetMotion(CMotionManager::TYPE_NEUTRAL);
		}
	}

	// 目標の方向の修正
	if (m_rotDest < -D3DX_PI)
	{
		m_rotDest += D3DX_PI * 2.0f;
	}
	if (m_rotDest > D3DX_PI)
	{
		m_rotDest -= D3DX_PI * 2.0f;
	}

	// 移動と回転の設定
	m_pos += m_move;
	m_rot.y += m_rotDest * 0.1f;

	// 慣性
	m_move.x += (0.0f - m_move.x) * 0.2f;
	m_move.z += (0.0f - m_move.z) * 0.2f;

	// 方向の修正
	if (m_rot.y < -D3DX_PI)
	{
		m_rot.y += D3DX_PI * 2.0f;
	}
	if (m_rot.y > D3DX_PI)
	{
		m_rot.y -= D3DX_PI * 2.0f;
	}

	// 目標の方向の初期化
	m_rotDest = 0.0f;

	float fHeight = 0.0f;	// 起伏の高さを求める
	CScene *pScene = NULL;	// オブジェクト管理クラスのポインタ変数

	// 地面オブジェクトを探す
	pScene = CScene::GetSceneTop(MESHFIELD_PRIORITY);

	while (pScene != NULL)
	{// NULLになるまでループ
		CScene *pSceneNext = pScene->GetSceneNext();
		CScene::OBJTYPE objType = pScene->GetObjType();

		if (objType == CScene::OBJTYPE_FIELD)
		{// 地面だったとき
			CMeshField *pMeshField = (CMeshField*)pScene;	// 地面のポインタを取得

			fHeight = pMeshField->GetHeight(m_pos);			// 地面の高さの取得
		}

		// 次のオブジェクトを見る
		pScene = pSceneNext;
	}

	if (m_pos.y <= fHeight)
	{// 地面以下になったとき
		m_pos.y = fHeight;	// 高さを座標に入れる
		m_move.y = 0.0f;	// Y軸の移動量を無くす
	}
}

//=========================================
// ジャンプの処理
//=========================================
void CPlayer::Jump(float fAngleOld)
{
	if (m_fAngle < 0.0f && fAngleOld >= 0.0f && !m_bJump && m_bSlip)
	{// 上っていて地面につかなくなったとき
		m_move.y = fAngleOld * m_fSlipSpeed * PLAYER_SLIP_JUMP;
		if (m_move.y > 0.0f)
		{// 上に飛んだ時ジャンプ状態にする
			m_bJump = true;
			m_bDash = false;
			m_judgJump.bJump = false;

			CSound *pSound = CManager::GetSound();		// サウンドの取得
			pSound->PlaySound(CSound::SOUND_LABEL_JUMP);
		}
		m_move.x = sinf(m_rotDest) * m_fSlipSpeed * PLAYER_SLIP_JUMP;
		m_pMotion->SetMotion(CMotionManager::TYPE_JUMP);
	}
}

//=========================================
// 傾斜の処理
//=========================================
void CPlayer::Tilt(void)
{
	CSound *pSound = CManager::GetSound();			// サウンドの取得
	float fCosTheta = 0.0f;							// 計算結果
	D3DXVECTOR3 nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 法線

														// 地面オブジェクトを探す
	CScene *pScene = CScene::GetSceneTop(MESHFIELD_PRIORITY);

	while (pScene != NULL)
	{// NULLになるまでループ
		CScene *pSceneNext = pScene->GetSceneNext();
		CScene::OBJTYPE objType = pScene->GetObjType();

		if (objType == CScene::OBJTYPE_FIELD)
		{// 地面だったとき
			CMeshField *pMeshField = (CMeshField*)pScene;

			m_fAngle = pMeshField->GetAngle(m_pos, m_rot.y);	// 傾斜の角度の取得
			nor = pMeshField->GetNor(m_pos);					// 地面の法線の取得

			if (!m_bJump)
			{// 坂の下り方向を取得
				m_fAngleSlip = pMeshField->GetSlipAngle(m_pos);
			}
		}

		// 次のオブジェクトを見る
		pScene = pSceneNext;
	}

	CDebugProc::Print("地面とのcosθ：%.2f", m_fAngle);

	// カメラの取得
	CCamera *pCamera = CManager::GetCamera();

	if (!m_bSlip)
	{// 滑っていない
		if (!m_bJump && nor.y < PLAYER_SLIP_ANGLE && nor.y != 0.0f && 0 > m_fAngle)
		{// ジャンプしてないかつ角度が一定以下で滑る
			m_bSlip = true;
			pSound->PlaySound(CSound::SOUND_LABEL_SLIP);

			if (m_fAngle < 0.0f)
			{// 滑り下りるとき速度を入れる
				m_fSlipSpeed = m_fSpeed;
			}
		}

		if (!m_bJump)
		{// 地面
			if (m_fAngle < 0.0f)
			{// 下りるときに速度を上げる
				m_fSpeed += MOVE_SPEED * (1.0f - nor.y);
			}
			else if (m_fAngle > 0.0f)
			{// 上るときに速度を下げる
				m_fSpeed -= MOVE_SPEED * (1.0f - nor.y) * PLAYER_CLIMB_SPEED;
			}
		}
	}
	else
	{// 滑っている
		if (!m_bJump)
		{// 滑る移動量を増やす
			m_fSlipSpeed += PLAYER_SLIP_SPEED * (1.0f - nor.y) * -m_fAngle;

			// 滑り落ちるときのパーティクル
			CParticle::Create(m_pos, 2);

			if (m_fAngle > 0.0f)
			{// 登っているとき
				m_fSlipSpeed -= PLAYER_SLIP_SPEED * m_fAngle;
			}

			m_move.x += sinf(m_fAngleSlip) * m_fSlipSpeed;

			if (0.0 <= m_rot.y && 0 < m_move.x)
			{// 移動量を逆にする
				m_move.x *= -1.0f;
			}
			else if (0.0 >= m_rot.y && 0 > m_move.x)
			{// 移動量を逆にする
				m_move.x *= -1.0f;
			}
		}
		else
		{// ジャンプ中
			pSound->StopSound(CSound::SOUND_LABEL_SLIP);
		}
	}

	if (m_fSlipSpeed > -3.0f && m_fSlipSpeed < 3.0f)
	{// 速度が一定以下で通常へ
		m_bSlip = false;
		pSound->StopSound(CSound::SOUND_LABEL_SLIP);
		m_fSlipSpeed = 0;
	}

	// 移動量のベクトル
	D3DXVECTOR3 vecMove = (m_pos + m_move) - m_pos;

	// 移動量ベクトルと法線のcosθを求める
	fCosTheta = (vecMove.x * nor.x) + (vecMove.y * nor.y) + (vecMove.z * nor.z);
	fCosTheta = fCosTheta / (sqrtf(powf(vecMove.x, 2) + powf(vecMove.y, 2) + powf(vecMove.z, 2)) * (sqrtf(powf(nor.x, 2) + powf(nor.y, 2) + powf(nor.z, 2))));

	if (isnan(fCosTheta))
	{// nanチェック
		fCosTheta = 0.0f;
	}

	// 角度によってパーティクルを変える
	if (nor.y < PLAYER_SLIP_ANGLE && 0 < m_fAngle && (m_move.x >= 0.01f || m_move.x <= -0.01f))
	{// 汗を出す
		m_nCntParticle = (1 + m_nCntParticle) % 15;

		if (m_nCntParticle == 0)
		{// パーティクルを出す
			CParticle::Create(m_pos + D3DXVECTOR3(0.0f, 40.0f, 0.0f), 0);
		}
	}

	CDebugProc::Print("滑る速度：%.2f", m_fSlipSpeed);
	CDebugProc::Print("ジャンプ速度：%.2f", m_fJumpSpeed);
}

//=========================================
// 当たり判定
//=========================================
bool CPlayer::Collision(D3DXVECTOR3 pos, float fRange)
{
	bool bHit = false;

	// 相手のXYの範囲
	float fLength = powf(fRange, 2);

	float fLengthX = pos.x - m_mtxWorld._41;	// Xの差
	float fLengthY = pos.y - m_mtxWorld._42;	// Yの差
	float fLengthZ = pos.z - m_mtxWorld._43;	// Zの差

	float fLengthTotal = powf(fLengthX, 2) + powf(fLengthY, 2) + powf(fLengthZ, 2);		// XYZの差の二乗

	if (fLength >= fLengthTotal)
	{// 相手と自分の判定
		bHit = true;	// 判定を有効にする
	}

	return bHit;	// 判定結果を返す
}

//=========================================
// ダメージを受けたときの処理
//=========================================
void CPlayer::Damage(int nDamage)
{
	if (m_pMotion)
	{// モーションクラスが使われている
		if (NULL != m_pLife && m_pMotion->GetType() != CMotionManager::TYPE_DAMAGE && !m_bDamage)
		{// ライフクラスが使われている
			m_pLife->AddSubtract(nDamage);
			m_pMotion->SetMotion(CMotionManager::TYPE_DAMAGE);	// ダメージモーションを再生
			m_bDamage = true;									// ダメージを受けている状態にする
			CSound *pSound = CManager::GetSound();				// サウンドの取得
			pSound->PlaySound(CSound::SOUND_LABEL_DAMAGE);		// ダメージ音を再生

			if (0 >= m_pLife->GetLife())
			{// 体力が0以下になった
				Uninit();
				CGame::SetGameState(CGame::STATE_END);
			}
		}
	}
}

//=============================================================================
//	AI更新処理
//=============================================================================
void CPlayer::AIUpdate(CInputMouse *pMouse)
{
	//CDebugProc::Print("========プレイヤー========\n");
	//CDebugProc::Print("プレイヤーの位置 x : %.1f / z : %.1f\n", m_pos.x, m_pos.z);
	CDebugProc::Print("開始地点 : %d\n", m_nStartNode);
	CDebugProc::Print("目的地 : %d\n", m_nEndNode);	
	//CDebugProc::Print("休憩時間: %d\n", m_nBreaktime);
	CDebugProc::Print("データ収集カウンター : %d\n", m_nCollectionTimer);
	//CDebugProc::Print("現在の移動回数: %d\n", m_nPoint);
	//CDebugProc::Print("目標までの移動回数: %d\n", m_nCountPoint);
	CDebugProc::Print("ゴールした回数: %d\n", m_nGoalCount);
	CDebugProc::Print("合計値 x : %.1f / z : %.1f\n", m_totalCollectPos.x, m_totalCollectPos.z);
	CDebugProc::Print("平均値に最も近いノード[%d] x : %.1f / z : %.1f\n", m_nNearTotalCollectNumber, m_pNodeData->GetLoadData().pos[m_nNearTotalCollectNumber].x, m_pNodeData->GetLoadData().pos[m_nNearTotalCollectNumber].z);
	CDebugProc::Print("\n");

	for (int nCntEnemy = 0; nCntEnemy < CEnemy::GetEnemyMax(); nCntEnemy++)
	{// エネミーの数分回る
		CDebugProc::Print("エネミー[%d]\n", nCntEnemy);
		for (int nCntCollect = 0; nCntCollect < COLLECTIONDATA_MAX; nCntCollect++)
		{// 収集データの最大値分回る
			CDebugProc::Print("収集したデータ[%d] x : %.1f / z : %.1f\n", nCntCollect, m_collectionPos[nCntEnemy][nCntCollect].x, m_collectionPos[nCntEnemy][nCntCollect].z);
		}
		CDebugProc::Print("\n");
	}

	float fMinLength = 100000, fLength = 100000;	// 差分系
	D3DXVECTOR3 total = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 収集したデータの合計値
	int nCntEnemyMax = 0;	// エネミー検索時のカウンタ
	CScene *pScene = CScene::GetSceneTop(ENEMY_PRIORITY);// プライオリティーチェック
	m_nCollectionTimer--;	// データ収集のカウンタを進める

	// エネミーのポインタの取得
	while (pScene != NULL)
	{// シーンのNULLチェック
	 // UpdateでUninitされてしまう場合　Nextが消える可能性があるからNextにデータを残しておく
		CScene *pSceneNext = pScene->GetSceneNext();
		if (pScene->GetDeath() == false)
		{// 死亡フラグが立っていない
			if (pScene->GetObjType() == OBJTYPE_ENEMY)
			{// タイプがエネミー
				m_pEnemy[nCntEnemyMax] = ((CEnemy*)pScene)->GetEnemy();// エネミーの情報の取得
				nCntEnemyMax++;// カウンタを進める
			}
		}
		// Nextに次のSceneを入れる
		pScene = pSceneNext;
	}

	// データの収集
	if (m_nCollectionTimer <= 0)
	{// 既定時間ごとにデータ収集
		// 位置データの取得
		for (int nCntEnemy = 0; nCntEnemy < nCntEnemyMax; nCntEnemy++)
		{// エネミーの数だけ回る
			if (m_pEnemy[nCntEnemy] != NULL)
			{// エネミーのNULLチェック
				m_collectionPos[nCntEnemy][m_nCountCollect] = m_pEnemy[nCntEnemy]->GetPos();	// 敵の位置情報を取得
				m_nCollectionTimer = COLLECT_TIME * 60;	// 時間を戻す
			}
		}

		// 収集したデータを合計する
		for (int nCntEnemy = 0; nCntEnemy < nCntEnemyMax; nCntEnemy++)
		{// エネミーの数だけ回る
			if (m_pEnemy[nCntEnemy] != NULL)
			{// エネミーのNULLチェック
				if (!m_bCollectSwitch)
				{// 1週目
					for (int nCntCollect = 0; nCntCollect < m_nCountCollect + 1; nCntCollect++)
					{// 収集したデータの数だけ回る
						total += m_collectionPos[nCntEnemy][nCntCollect];	// 収集データを合計する
					}
				}
				else
				{// 2週目以降
					for (int nCntCollect = 0; nCntCollect < COLLECTIONDATA_MAX; nCntCollect++)
					{// 収集できるデータの最大数だけ回る
						total += m_collectionPos[nCntEnemy][nCntCollect];	// 収集データを合計する
					}
				}
			}
		}

		// 収集データの平均値を取る
		if (!m_bCollectSwitch)
		{// 1週目の場合
			m_totalCollectPos = total / (float)(m_nCountCollect + 1);
		}
		else
		{// 2週目以降
			m_totalCollectPos = total / (float)COLLECTIONDATA_MAX;
		}

		// データを収集した回数のカウント
		if (m_nCountCollect < COLLECTIONDATA_MAX - 1)
		{// 収集最大に達していない場合
			m_nCountCollect++;	// 収集カウントを進める
		}
		else
		{// 収集最大に達した場合
			m_bCollectSwitch = true;// 平均値の割り出し方法を切り替える
			m_nCountCollect = 0;	// 収集カウントを最初からにする
		}

		// 平均値の最も近いノードを検索する
		for (int nCntNode = 0; nCntNode < m_pNodeData->GetLoadData().nodeMax; nCntNode++)
		{// ノードの数だけ回る
		 // 差分を求める
			float fLength = (m_pNodeData->GetLoadData().pos[nCntNode].x - m_totalCollectPos.x) * (m_pNodeData->GetLoadData().pos[nCntNode].x - m_totalCollectPos.x) + (m_pNodeData->GetLoadData().pos[nCntNode].z - m_totalCollectPos.z) * (m_pNodeData->GetLoadData().pos[nCntNode].z - m_totalCollectPos.z);

			if (fMinLength > fLength)
			{// 差分の最小値を求める
				fMinLength = fLength;
				m_nNearTotalCollectNumber = nCntNode;
			}
		}
	}

	// 自動移動処理
	CPlayer::AutoMove();
}

//=============================================================================
//	自動移動処理
//=============================================================================
void CPlayer::AutoMove()
{
	CMotionManager::TYPE type = CMotionManager::TYPE_NEUTRAL;	// モーションの種類
	bool bMoveKey = false;	// 移動キー押下フラグ

	// 目標地点を設定
	m_posDest = m_waypoint[m_nPoint];

	// 目的との差分を出す
	float fLength = (m_pos.x - m_posDest.x) * (m_pos.x - m_posDest.x) + (m_pos.z - m_posDest.z) * (m_pos.z - m_posDest.z);
	m_nBreaktime--;

	if (fLength > MOVE_ACCEPTABLE)
	{// 差分が許容値内に収まるまで目的地に移動する
		bMoveKey = true;
		m_move.x = sinf(atan2f(m_posDest.x - m_pos.x, m_posDest.z - m_pos.z)) * m_fSpeed;
		m_move.z = cosf(atan2f(m_posDest.x - m_pos.x, m_posDest.z - m_pos.z)) * m_fSpeed;
		m_rot.y = atan2f(m_posDest.x - m_pos.x, m_posDest.z - m_pos.z) + D3DX_PI;
	}
	else if (m_nBreaktime < 0)
	{// 移動後休憩
		bMoveKey = false;
		m_nBreaktime = PLAYER_BREAKTIME;

		if (m_nPoint == m_nCountPoint)
		{// 最終目標地点に到着したら次の目的地を探す
			if (m_nGoalCount != 0)
			{// 最初以外に最終地点用の休憩時間を与える
				m_nBreaktime = PLAYER_FINALPOINT_BREAKTIME;
			}
			m_nPoint = 0;		// 現在の移動回数の初期化
			m_nCountPoint = -1;	// 目的までの移動回数の初期化
			m_nGoalCount++;		// ゴールした回数を増やす

			// ポイント検索
			CPlayer::NodeSearch();
			// ポイントへの経路探索
			CPlayer::RootSearch();
		}
	}
	else if (m_nBreaktime == 0 && m_nPoint < m_nCountPoint)
	{// 休憩終了
		m_nPoint++;
	}
	else if(m_nBreaktime > 0)
	{// 休憩中
		m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 停止する
	}

	if (m_pMotion)
	{// 使われている
		if (type != CMotionManager::TYPE_ATTACK01 && type != CMotionManager::TYPE_DAMAGE)
		{// 攻撃モーションでない時
			if (m_move.x > 0.1f || m_move.x < -0.1f || m_move.z > 0.1f || m_move.z < -0.1f)
			{// 移動モーションを設定
				m_pMotion->SetMotion(CMotionManager::TYPE_WALK);		// モーションの設定
			}
			else
			{// ニュートラルモーションを設定
				m_pMotion->SetMotion(CMotionManager::TYPE_NEUTRAL);		// モーションの設定
			}
		}
	}

	// 位置の更新
	m_pos.x += m_move.x;
	m_pos.z += m_move.z;
}

//=============================================================================
//	ノード検索処理
//=============================================================================
void CPlayer::NodeSearch()
{
	float fMinLength = 100000, fLength = 100000;	// 差分系
	int nNearEnemyNumber = 0;	// 最も近い敵の番号
	int nMovePoint = 0;

	// 目的の設定
	if (m_nGoalCount % DATA_REFERENCE_TIME == 0)
	{// 定期的に収集したデータに基づいた移動を行う
		m_nNewEndNode = m_nNearTotalCollectNumber;
	}
	else
	{// 新規目的地を指定地点からランダムで決定する
		do
		{// 同じ地点だった場合はもう1度決める
			nMovePoint = m_nMovePoint[rand() % RANDOM_MOVE_POINT];
		} while (m_nNewEndNode == nMovePoint);
		m_nNewEndNode = nMovePoint;
	}

	for (int nCntNode = 0; nCntNode < m_pNodeData->GetLoadData().nodeMax; nCntNode++)
	{// ノードの数だけ回る
		if (m_pNodeData->GetLoadData().pos[nCntNode].x + POS_ACCEPTABLE > m_pos.x
			&& m_pNodeData->GetLoadData().pos[nCntNode].x - POS_ACCEPTABLE < m_pos.x
			&& m_pNodeData->GetLoadData().pos[nCntNode].z + POS_ACCEPTABLE > m_pos.z
			&& m_pNodeData->GetLoadData().pos[nCntNode].z - POS_ACCEPTABLE < m_pos.z)
		{// プレイヤーの位置が許容範囲内		
			m_nStartNode = m_nEndNode;	// 前回の目的地を開始地点として登録
		}
	}

	for (int nCntNode = 0; nCntNode < m_pNodeData->GetLoadData().nodeMax; nCntNode++)
	{// ノードの数だけ回る
		if (m_pNodeData->GetLoadData().pos[nCntNode].x + POS_ACCEPTABLE > m_pos.x
			&& m_pNodeData->GetLoadData().pos[nCntNode].x - POS_ACCEPTABLE < m_pos.x
			&& m_pNodeData->GetLoadData().pos[nCntNode].z + POS_ACCEPTABLE > m_pos.z
			&& m_pNodeData->GetLoadData().pos[nCntNode].z - POS_ACCEPTABLE < m_pos.z)
		{// プレイヤーの位置が目的地の許容範囲内
			if (m_pNodeData->GetLoadData().pos[nCntNode] != m_pNodeData->GetLoadData().pos[m_nNewEndNode])
			{// 新規目的地が前回の目的地と同じじゃない場合
				m_nEndNode = m_nNewEndNode;	// 目的地を登録
			}
		}
	}

	//// 自分に最も近いエネミーを探す
	//for (int nCntEnemy = 0; nCntEnemy < CEnemy::GetEnemyMax(); nCntEnemy++)
	//{// エネミーの数だけ回る
	//	if (m_pEnemy[nCntEnemy] != NULL)
	//	{// エネミーのNULLチェック
	//	 // 全てのエネミーとプレイヤーとの差分を出す
	//		fLength = (m_pEnemy[nCntEnemy]->GetPos().x - m_pos.x) * (m_pEnemy[nCntEnemy]->GetPos().x - m_pos.x) + (m_pEnemy[nCntEnemy]->GetPos().z - m_pos.z) * (m_pEnemy[nCntEnemy]->GetPos().z - m_pos.z);

	//		if (fMinLength > fLength)
	//		{// 差分の最小値を求める
	//			fMinLength = fLength;
	//			nNearEnemyNumber = nCntEnemy;
	//		}
	//	}
	//}

	//for (int nCntNode = 0; nCntNode < m_pNodeData->GetLoadData().nodeMax; nCntNode++)
	//{// ノードの数だけ回る
	//	if (m_pEnemy[nNearEnemyNumber] != NULL)
	//	{// エネミーのNULLチェック
	//		if (m_pNodeData->GetLoadData().pos[nCntNode].x + POS_ACCEPTABLE > m_pEnemy[nNearEnemyNumber]->GetPos().x
	//			&& m_pNodeData->GetLoadData().pos[nCntNode].x - POS_ACCEPTABLE < m_pEnemy[nNearEnemyNumber]->GetPos().x
	//			&& m_pNodeData->GetLoadData().pos[nCntNode].z + POS_ACCEPTABLE > m_pEnemy[nNearEnemyNumber]->GetPos().z
	//			&& m_pNodeData->GetLoadData().pos[nCntNode].z - POS_ACCEPTABLE < m_pEnemy[nNearEnemyNumber]->GetPos().z)
	//		{// 最も近い敵の位置が許容範囲内
	//			m_nEndNode = nCntNode;
	//		}
	//	}
	//}
}

//=============================================================================
//	ルート検索処理
//=============================================================================
void CPlayer::RootSearch()
{
	Node node[NODEPOINT_MAX];	// ノードの情報
	float weight[NODEPOINT_MAX];	// 各エッジのコスト
	int nCntWeight = 0;		// コストのカウンタ
	std::vector<int> path;	// 最短経路の情報を保持するvector

	//======= エッジコストの算出 =========================================================================
	for (int nCntNode = 0; nCntNode < m_pNodeData->GetLoadData().nodeMax; nCntNode++, nCntWeight++)
	{// ノードの数だけ回る
		weight[nCntWeight] = sqrt((m_pNodeData->GetLoadData().pos[m_nStartNode].x - m_pNodeData->GetLoadData().pos[nCntNode].x) * (m_pNodeData->GetLoadData().pos[m_nStartNode].x - m_pNodeData->GetLoadData().pos[nCntNode].x) + (m_pNodeData->GetLoadData().pos[m_nStartNode].z - m_pNodeData->GetLoadData().pos[nCntNode].z) * (m_pNodeData->GetLoadData().pos[m_nStartNode].z - m_pNodeData->GetLoadData().pos[nCntNode].z));
	}

	////======= コスト表の出力 =========================================================================
	//CDebugProc::Print("========移動コスト表========\n");
	//for (int nCntNode = 0; nCntNode < m_pNodeData->GetLoadData().nodeMax; nCntNode++)
	//{// ノードの数だけ回る
	//	CDebugProc::Print(" No.%d : %.1f\n", nCntNode, weight[nCntNode]);
	//}
	//CDebugProc::Print("=============================\n\n");

	//======= エッジ追加 =========================================================================
	for (int nCntNode = 0; nCntNode < m_pNodeData->GetLoadData().nodeMax; nCntNode++)
	{// ノードの数だけ回る
		for (int nCntConnect = 0; nCntConnect < m_pNodeData->GetLoadData().connectNum[nCntNode]; nCntConnect++)
		{// 繋がってるノードの数だけ回る
			CPlayer::AddEdge(nCntNode, m_pNodeData->GetLoadData().connectIndex[nCntNode][nCntConnect], weight[nCntNode], node);
		}
	}

	//======= 最短経路を調べる =========================================================================
	CPlayer::Dijkstra(m_pNodeData->GetLoadData().nodeMax, m_nStartNode, m_nEndNode, node);

	for (int nCntNode = m_nEndNode; nCntNode != m_nStartNode; nCntNode = node[nCntNode].from)
	{// 最短経路をゴールから順にスタートまでたどる
		path.push_back(nCntNode);
	}
	path.push_back(m_nStartNode);

	//======= 最短経路の出力 =========================================================================
	//CDebugProc::Print(" 最短経路は【");
	for (int nCntNode = path.size() - 1; nCntNode >= 0; nCntNode--)
	{
		//CDebugProc::Print(" %d ", path[nCntNode]);
		if (m_nCountPoint < nCntNode)
		{// 最大値を代入
			// 目的までの移動回数を取得する
			m_nCountPoint = nCntNode;
		}
	}
	//CDebugProc::Print("】の順です。\n\n");

	//======= 目標地点の設定 =========================================================================
	for (int nCntNodeMax = 0; nCntNodeMax < m_pNodeData->GetLoadData().nodeMax; )
	{// ノードの数だけ回る
		for (int nCntNode = path.size() - 1; nCntNode >= 0; nCntNode--, nCntNodeMax++)
		{
			m_waypoint[nCntNodeMax] = m_pNodeData->GetLoadData().pos[path[nCntNode]];
		}
	}
}

//=============================================================================
// エッジ追加処理
//=============================================================================
void CPlayer::AddEdge(int first, int second, float weight, Node *node)
{
	// ノードuはノードvとつながっている情報を入れる
	node[second].to.push_back(first);
	// ノードuとノードvのエッジの重みを入れる
	node[second].cost.push_back(weight);
	//// ノードvはノードuとつながっている情報を入れる
	//node[second].to.push_back(first);
	//// ノードvとノードuのエッジの重みを入れる
	//node[second].cost.push_back(weight);
}

//=============================================================================
// ダイクストラ法
//=============================================================================
void CPlayer::Dijkstra(int nodeMax, int start, int end, Node *node)
{
	// 変数の初期化
	for (int nCntNode = 0; nCntNode < nodeMax; nCntNode++)
	{
		node[nCntNode].done = false;
		node[nCntNode].minCost = -1;
	}

	node[start].minCost = 0;	// スタートノードまでのコストは0
	while (1)
	{
		int doneNode = -1;	// 最新の確定したノード番号(-1はNULLのかわり)
		for (int nCntNode = 0; nCntNode < nodeMax; nCntNode++)
		{
			if (node[nCntNode].done == true)
			{// ノードiが確定しているときcontinue
				continue;
			}

			if (node[nCntNode].minCost < 0)
			{// ノードiまでの現時点での最小コストが不明のとき
				continue;
			}

			// 確定したノード番号が-1かノードiの現時点の最小コストが小さいとき
			// 確定ノード番号を更新する
			if (doneNode < 0 || node[nCntNode].minCost < node[doneNode].minCost)
			{
				doneNode = nCntNode;
			}
		}

		if (doneNode == -1)
		{
			break;	// すべてのノードが確定したら終了
		}

		node[doneNode].done = true;	// ノードを確定させる

		for (int nCntNodeSize = 0; nCntNodeSize < (int)node[doneNode].to.size(); nCntNodeSize++)
		{
			int to = node[doneNode].to[nCntNodeSize];
			float cost = node[doneNode].minCost + node[doneNode].cost[nCntNodeSize];

			// ノードtoはまだ訪れていないノード
			// またはノードtoへより小さいコストの経路だったら
			// ノードtoの最小コストを更新
			if (node[to].minCost < 0.0f || cost < node[to].minCost)
			{
				node[to].minCost = cost;
				node[to].from = doneNode;
			}
		}
	}
}