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
#include "particle.h"

//==================================
// マクロ定義
//==================================
#define ASSULT_FILE				"data/TEXT/PLAYER/assult/model_assult.txt"
#define LIGHT_FILE				"data/TEXT/PLAYER/light/model_light.txt"
#define HEAVY_FILE				"data/TEXT/PLAYER/heavy/model_heavy.txt"
#define SNIPE_FILE				"data/TEXT/PLAYER/snipe/model_snipe.txt"

#define PLAYER_DAMAGE_TIME		(60)		// ダメージを受けた時の無敵時間
#define PLAYER_DAMAGE_MOVE		(40)		// ダメージを受けてから動けるようになるまでの時間
#define PLAYER_RETICLE_LENGTH	(2500.0f)	// レティクルの距離

// =============================================================
// UI関係
// =============================================================
#define ANIM_SPEED				(10)		// タイルアニメーション
#define ANIM_PATTERN			(8)			// タイルのパターン数
#define PLAYER_BOTTON_WIDTH		(75.0f)		// リスポーンボタンの横幅
#define PLAYER_BOTTON_HEIGHT	(75.0f)		// リスポーンボタンの縦幅
#define PLAYER_BOTTON_INT		(40.0f)		// ボタンとボタンの間隔
#define PLAYER_UI_HEIGHT		(630.0f)

// ラジオチャット
#define RADIOCHAT_MESS_SPEED		(10)			// ラジオチャットメッセージの速さ
#define RADIOCHAT_DISPLAY_TIME	(60 * 2)	// メッセージ表示時間
#define RADIOCHAT_COL						(0.05f)		// 透明度の減算

// =============================================================
// AI関係
// =============================================================
#define	LOAD_FILENAME		("data/TEXT/NODE_DATA/NodeData.txt")	// マップデータを読み込むファイルの名前
#define MOVE_ACCEPTABLE		(50.0f)		// 移動時の誤差の許容範囲
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

//==================================
// 生成処理
//==================================
CPlayer* CPlayer::Create(int nPlayerIdx, CMechaSelect::MECHATYPE mecha, D3DXVECTOR3 pos, bool bConnect)
{
	CPlayer *pPlayer;

	pPlayer = new CPlayer;	// メモリを確保

	if (pPlayer != NULL)
	{// メモリ確保成功
		pPlayer->SetPlayerIdx(nPlayerIdx);
		pPlayer->m_mecha = mecha;
		pPlayer->m_pos = pos;
		pPlayer->m_bConnect = bConnect;
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
	m_pShadow = NULL;
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
	m_nDiff = 0;
	m_nRemBullet = 0;
	m_pUITexRadio = NULL;			// 自分
	m_pUITexAllyRadio = NULL;		// 味方
	m_radiochat = RADIOCHAT_OK;
	m_allyRadiochat = RADIOCHAT_OK;
	m_bChat = false;
	m_bAllyChat = false;
	m_moveSpeed = 0;
	m_bCol = false;
	m_bChatBotton = false;
	m_bReload = false;
	m_nRespawnTimer = 0;
	m_bAllyCol = false;

	for (int nCnt = 0; nCnt < AI_MAX; nCnt++)
	{
		m_pAI[nCnt] = NULL;
	}

	for (int nCnt = 0; nCnt < PLAYER_UI_NUM; nCnt++)
	{
		m_pUINum[nCnt] = NULL;
	}

	for (int nCnt = 0; nCnt < RELOAD_TEX; nCnt++)
	{	// リロード
		m_pUITexReload[nCnt] = NULL;
	}

	for (int nCnt = 0; nCnt < SELECTRESPAWN_BOTTON; nCnt++)
	{	// リスポーンボタン
		m_pUISelectResBotton[nCnt] = NULL;
		m_pUIRespawnPosIcon[nCnt] = NULL;
	}

	for (int nCnt = 0; nCnt < RESPAWN_TEX; nCnt++)
	{	// リスポーン
		m_pUITexRespawn[nCnt] = NULL;
	}

	for (int nCnt = 0; nCnt < RADIOCHAT_BOTTON; nCnt++)
	{	// ラジオチャットボタン
		m_pUIRadioBotton[nCnt] = NULL;
	}

	// カウンターの初期化
	m_nCntShoot = 0;
	m_bShootButton = false;
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
		//m_pShadow = CShadow::Create(&m_pos);
	}

	if (CManager::GetMode() == CManager::MODE_GAME)
	{
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
					m_pReticle->SetZBuffer(true, D3DCMP_ALWAYS);
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
				m_pReticle->SetZBuffer(true, D3DCMP_ALWAYS);
				m_pReticle->SwapPriority(6);
			}
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

	// ラジオチャットメッセージで使用
	m_moveSpeed = RADIOCHAT_MESS_SPEED;

	// ゲーム開始時
	m_Respawn = RESPAWN_START;
	m_nDisTime = 1.8f;

	if (m_nPlayerIdx == 0 || m_nPlayerIdx == 1)
	{
		for (int nCntModel = 0; nCntModel < m_nNumParts; nCntModel++)
		{
			m_pModel[nCntModel]->SetColor(D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
		}
		m_nTeam = 0;

		// カメラの向きの設定
		CManager::GetCamera()->SetRot(D3DXVECTOR3(D3DX_PI * 0.5f, D3DX_PI * -0.5f, 0.0f));
	}
	else if (m_nPlayerIdx == 2 || m_nPlayerIdx == 3)
	{
		for (int nCntModel = 0; nCntModel < m_nNumParts; nCntModel++)
		{
			m_pModel[nCntModel]->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
		}

		m_nTeam = 1;

		// カメラの向きの設定
		CManager::GetCamera()->SetRot(D3DXVECTOR3(D3DX_PI * 0.5f, D3DX_PI * 0.5f, 0.0f));
	}

	// AIの生成
	//m_pAI = m_pAI->Create(this, CAIMecha::MECHATYPE_DRONE, m_pos + D3DXVECTOR3(0.0f, 70.0f, 0.0f));

	if (CManager::GetMode() == CManager::MODE_GAME)
	{
		if (m_pAI[0] == NULL)
		{// ドローンタイプのAIの生成
			m_pAI[0] = m_pAI[0]->Create(this, CAIMecha::MECHATYPE_DRONE, m_pos + D3DXVECTOR3(0.0f, 70.0f, 0.0f));
		}
	}

	// 数値の初期化==============================================================================
	m_posDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nPoint = 0;
	m_nCountPoint = 0;
	m_nCountCollect = 0;
	m_nBreaktime = 0;
	m_nGoalCount = 0;
	m_nVigilanceCount = 0;
	m_nCollectionTimer = COLLECT_TIME * 60;
	m_bGoal = false;
	m_bPartSwitch = false;
	m_bCollectSwitch = false;

	for (int nCntEnemy = 0; nCntEnemy < ENEMY_PLAYER_MAX; nCntEnemy++)
	{// エネミーの最大値分回る
		for (int nCntCollect = 0; nCntCollect < COLLECTIONDATA_MAX; nCntCollect++)
		{// 収集データの最大値分回る
			m_collectionPos[nCntEnemy][nCntCollect] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		}
	}

	for (int nCntNode = 0; nCntNode < NODE_MAX; nCntNode++)
	{// ノードの最大値分回る
		m_waypoint[nCntNode] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	if (!m_bConnect)
	{// 人間が接続していない場合
		// マップ関係==============================================================================
		CPlayer::FileLoad(LOAD_FILENAME);

		// 開始時点のノードの初期化
		float fMinLength = 100000, fLength = 100000;	// 差分系

		// 開始時点のノードの初期化
		for (int nCntNode = 0; nCntNode < m_NodeData.nodeMax; nCntNode++)
		{// ノードの数だけ回る
			// 差分を求める
			fLength = (m_NodeData.pos[nCntNode].x - m_pos.x) * (m_NodeData.pos[nCntNode].x - m_pos.x) + (m_NodeData.pos[nCntNode].z - m_pos.z) * (m_NodeData.pos[nCntNode].z - m_pos.z);

			if (fMinLength > fLength)
			{// 差分の最小値を求める
				fMinLength = fLength;
				m_nStartNode = nCntNode;
			}
		}
		m_nEndNode = m_nMovePoint[rand() % 7];

		// ポイント検索
		CPlayer::NodeSearch();
		// ポイントへの経路探索
		CPlayer::RootSearch();
	}

	return S_OK;
}

//=========================================
// 終了処理
//=========================================
void CPlayer::Uninit(void)
{
	for (int nCntType = 0; nCntType < AI_MAX; nCntType++)
	{// AIの数だけ回る
		if (m_pAI[nCntType] != NULL)
		{// AIがNULLじゃない
			m_pAI[nCntType]->Uninit();
			m_pAI[nCntType] = NULL;
		}
	}

	if (m_pReticle != NULL)
	{// レティクルがNULLじゃない
		m_pReticle->Uninit();
		m_pReticle = NULL;
	}

	if (m_pShadow != NULL)
	{// 影がNULLじゃない
		m_pShadow->Uninit();
		m_pShadow = NULL;
	}

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
	if (CManager::GetMode() == CManager::MODE_GAME)
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

						if (m_bChat == false)
						{
							//ラジオチャットの生成
							ChatBotton();
						}
						else if (m_bChat == true)
						{
							ChatMess(m_bChat);
						}

						if (m_bAllyChat == true)
						{
							if (m_pUITexAllyRadio == NULL)
							{
								m_pUITexAllyRadio = CUI_TEXTURE::Create(D3DXVECTOR3(1280.0f, 550.0f, 0.0f), RADIOCHAT_MESS_WIDTH, RADIOCHAT_MESS_HEIGHT, CUI_TEXTURE::UIFLAME_RADIOCHAT_MESS);
								m_pUITexAllyRadio->SetTex((int)m_allyRadiochat, 1, RADIOCHAT_BOTTON_PATTERN);
							}
							else if (m_pUITexAllyRadio != NULL)
							{
								AllyChatMess();
							}
						}
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

					if (!m_bConnect)
					{// コンピュータが操作する場合
						AIUpdate();
					}
				}
			}
		}
		else if (CMenu::GetMode() == CMenu::MODE_SINGLE)
		{
			if (m_nPlayerIdx == 0)
			{
				m_pReticle->SetDisp(false);

				if (CManager::GetGame()->GetPart() == CGame::PART_ACTION)
				{// アクションパート
					CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();	// キーボードの入力を取得
					CXInput *pXInput = CManager::GetXInput();					// XInputの入力を取得

					if (pKeyboard->GetTrigger(DIK_K) == true)
					{
						Damage(m_nLifeMax);
					}

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

						if (m_bChat == false)
						{
							//ラジオチャットの生成
							ChatBotton();
						}
						else if(m_bChat == true)
						{
							ChatMess(m_bChat);
						}
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

		if (!m_bConnect)
		{// コンピュータが操作する場合
			if (CManager::GetGame()->GetPart() == CGame::PART_ACTION)
			{// アクションパート
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

					// AIの更新処理
					AIUpdate();
				}

				if (m_Respawn == RESPAWN_DEATH)
				{	// 行動不能状態
					Respawn(m_Respawn);		// リスポーン処理
				}

				if (m_Respawn == RESPAWN_SELECT)
				{	// リスポーン選択状態
					SelectRespawn();				// リスポーン位置選択
				}
			}
		}

		SetParticle();
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
		else if ((fDiffX < 10.0f || fDiffX > -10.0f) && !m_bChatBotton)
		{// 移動モーション
			m_pUpperMotion->SetMotion(CMotionManager::TYPE_WALK);
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

	if (pMouse->GetPress(CInputMouse::DIMS_BUTTON_0) && m_nRemBullet > 0 && m_bChatBotton == false && m_bReload == false)
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
				CParticle::Create(posCanon, 2);

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
				CParticle::Create(posCanon, 2);

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

	if (m_nRemBullet <= 0)
	{	// 弾がなくなった
		m_bReload = true;
	}

	if (pMouse->GetTrigger(CInputMouse::DIMS_BUTTON_1))
	{
		if (m_nRemBullet == m_nCapacity)
		{	// 弾が初期値と同じなら、リロードしない
			m_bReload = false;
		}
		else
		{	// 減っている時
			m_bReload = true;
		}
	}

	if (m_bConnect)
	{// リロード処理
		Reload(m_bReload);
	}

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

	// 可動域外になった
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

//=========================================
// パーティクルを発生させる
//=========================================
void CPlayer::SetParticle(void)
{
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();	// キーボードの入力を取得

	if (pKeyboard->GetTrigger(DIK_R))
	{
		CParData::Unload();
		CParData::Load();
	}

	if (m_nLife < m_nLifeMax / 4)
	{// 耐久力が４分の１になった
		int nParts;
		D3DXMATRIX mtx;
		D3DXVECTOR3 pos;

		if (rand() % 20 == 0)
		{
			nParts = rand() % m_nNumParts;
			mtx = m_pModel[nParts]->GetMtxWorld();
			pos = D3DXVECTOR3(mtx._41, mtx._42, mtx._43);
			CParticle::Create(pos, 3);
		}

		nParts = rand() % m_nNumParts;
		mtx = m_pModel[nParts]->GetMtxWorld();
		pos = D3DXVECTOR3(mtx._41, mtx._42, mtx._43);
		CParticle::Create(pos, 0);
	}
	else if (m_nLife < m_nLifeMax / 2)
	{// 耐久力が半分になった
		if (rand() % 30 == 0)
		{
			int nParts = rand() % m_nNumParts;
			D3DXMATRIX mtx = m_pModel[nParts]->GetMtxWorld();
			D3DXVECTOR3 pos = D3DXVECTOR3(mtx._41, mtx._42, mtx._43);
			CParticle::Create(pos, 3);
		}
	}
}

//=========================================
// ダメージを受けたときの処理
//=========================================
void CPlayer::Damage(int nDamage)
{
	if (CMenu::GetMode() == CMenu::MODE_SINGLE)
	{
		if (NULL != m_pUpperMotion && NULL != m_pLowerMotion)
		{// モーションクラスが使われている
			if (m_pUpperMotion->GetType() != CMotionManager::TYPE_DAMAGE && m_pLowerMotion->GetType() != CMotionManager::TYPE_DAMAGE)
			{// ライフクラスが使われている
				if (m_nLife > 0 && m_bDeath == false)
				{
					m_pUpperMotion->SetMotion(CMotionManager::TYPE_DAMAGE);	// ダメージモーションを再生
					m_pLowerMotion->SetMotion(CMotionManager::TYPE_DAMAGE);	// ダメージモーションを再生

					m_state = STATE_DAMAGE;								// ダメージを受けている状態にする

					m_nLife -= nDamage;

					if (0 >= m_nLife)
					{
						m_nLife = 0;
						m_bDeath = true;

						switch (m_nTeam)
						{
						case 0:
							CManager::GetGame()->SetBlueLinkEnergy(CManager::GetGame()->GetBlueLinkEnergy() - 30);
							break;
						case 1:
							CManager::GetGame()->SetRedLinkEnergy(CManager::GetGame()->GetRedLinkEnergy() - 30);
							break;
						}
					}
					//CSound *pSound = CManager::GetSound();				// サウンドの取得
					//pSound->PlaySound(CSound::SOUND_LABEL_DAMAGE);		// ダメージ音を再生
				}
			}
		}
	}
	else
	{
		if (CManager::GetClient() != NULL)
		{
			if (CManager::GetClient()->GetPlayerIdx() == m_nPlayerIdx)
			{
				if (NULL != m_pUpperMotion && NULL != m_pLowerMotion)
				{// モーションクラスが使われている
					if (m_pUpperMotion->GetType() != CMotionManager::TYPE_DAMAGE && m_pLowerMotion->GetType() != CMotionManager::TYPE_DAMAGE)
					{// ライフクラスが使われている
						if (m_nLife > 0 && m_bDeath == false)
						{
							m_pUpperMotion->SetMotion(CMotionManager::TYPE_DAMAGE);	// ダメージモーションを再生
							m_pLowerMotion->SetMotion(CMotionManager::TYPE_DAMAGE);	// ダメージモーションを再生

							m_state = STATE_DAMAGE;								// ダメージを受けている状態にする

							m_nLife -= nDamage;

							if (0 >= m_nLife)
							{
								m_nLife = 0;
								m_bDeath = true;

								switch (m_nTeam)
								{
								case 0:
									CManager::GetGame()->SetBlueLinkEnergy(CManager::GetGame()->GetBlueLinkEnergy() - 30);
									break;
								case 1:
									CManager::GetGame()->SetRedLinkEnergy(CManager::GetGame()->GetRedLinkEnergy() - 30);
									break;
								}
							}
							//CSound *pSound = CManager::GetSound();				// サウンドの取得
							//pSound->PlaySound(CSound::SOUND_LABEL_DAMAGE);		// ダメージ音を再生
						}
					}
				}
			}
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
// リロード処理：弾が0ー＞リロードロゴ、ゲージ表示
//=========================================
void CPlayer::Reload(bool bReload)
{
	if(bReload == true)
	{
		m_nCntReRoad++;		// カウンター加算

		if (m_pUITexReload[0] == NULL || m_pUITexReload[1] == NULL || m_pGauge == NULL)
		{	// NULLチェックからのリロードロゴ生成
			m_pUITexReload[0] = CUI_TEXTURE::Create(D3DXVECTOR3(1100.0f, 650.0f, 0.0f), 200.0f, 80.0f, CUI_TEXTURE::UIFLAME_RELOAD);	// 弾のところ
			m_pUITexReload[1] = CUI_TEXTURE::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), 200.0f, 80.0f, CUI_TEXTURE::UIFLAME_RELOAD);	// 画面中央
			m_pGauge = CGauge2D::Create(2, D3DXVECTOR3(SCREEN_WIDTH / 2, 400.0f, 0.0f), 0.0f, 100.0f, 300.0f, 30.0f);
			m_pGauge->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), 0);	// 元の長さ
			m_pGauge->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 1);	// 現在の体力
		}
		if (m_pUITexReload[0] != NULL || m_pUITexReload[1] != NULL || m_pGauge != NULL)
		{	// リロードロゴ点滅
			m_pUITexReload[0]->Flashing(30);
			m_pUITexReload[1]->Flashing(30);

			// ゲージ増加
			m_pGauge->AddSubtract(100.0f / m_nReload);		// 100％ / 秒数
		}

		if (m_nCntReRoad % m_nReload == 0)
		{	// 設定した秒数後→ロゴ＆ゲージ破棄＆弾生成
			m_nRemBullet = m_nCapacity;			// 弾生成

			if (m_pGauge != NULL)
			{	// ゲージの破棄
				m_pGauge->Uninit();
				m_pGauge = NULL;
			}

			for (int nCnt = 0; nCnt < RELOAD_TEX; nCnt++)
			{	// リロードロゴの破棄
				if (m_pUITexReload[nCnt] != NULL)
				{
					m_pUITexReload[nCnt]->Uninit();
					m_pUITexReload[nCnt] = NULL;
				}
			}

			m_bReload = false;
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
		if (m_pUITexRespawn[0] == NULL || m_pUITexRespawn[1] == NULL)
		{
			m_pUITexRespawn[0] = CUI_TEXTURE::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), 500.0f, 260.0f, CUI_TEXTURE::UIFLAME_TILE_PATTERN);	// タイル
			m_pUITexRespawn[1] = CUI_TEXTURE::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), 350.0f, 150.0f, CUI_TEXTURE::UIFLAME_START);	// 戦闘開始ロゴ
		}
		if (m_pUITexRespawn[0] != NULL || m_pUITexRespawn[1] != NULL)
		{
			m_nRespawnTimer++;
			if (m_nRespawnTimer % 60 == 0)
			{
				m_nDisTime--;
			}
			if (m_nDisTime <= 0)
			{	//	表示時間0になった
				for (int nCnt = 0; nCnt < RESPAWN_TEX; nCnt++)
				{
					if (m_pUITexRespawn[nCnt] != NULL)
					{	// UIテクスチャの破棄
						m_pUITexRespawn[nCnt]->Uninit();
						m_pUITexRespawn[nCnt] = NULL;
					}
				}
				respawn = RESPAWN_NONE;
			}
		}
		break;

	case RESPAWN_DEATH:
		if (m_pUITexRespawn[0] == NULL || m_pUITexRespawn[1] == NULL || m_pUITexRespawn[2] == NULL || m_pUINumRespawn == NULL)
		{
			m_pUITexRespawn[0] = CUI_TEXTURE::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), 500.0f, 260.0f, CUI_TEXTURE::UIFLAME_TILE_PATTERN);	// タイル
			m_pUITexRespawn[1] = CUI_TEXTURE::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), 350.0f, 150.0f, CUI_TEXTURE::UIFLAME_DEATH);	// 行動不能ロゴ
			m_pUITexRespawn[2] = CUI_TEXTURE::Create(D3DXVECTOR3(560.0f, 460.0f, 0.0f), 200.0f, 30.0f, CUI_TEXTURE::UIFLAME_RESPAWN);	// 戦線復帰
			m_pUINumRespawn = CUI_NUMBER::Create(D3DXVECTOR3(750.0f, 460.0f, 0.0f), 60.0f, 60.0f, 30.0f, CUI_NUMBER::UI_NUMTYPE_CNTRESPAWN, 0, NUMTEX_UV_X, NUMTEX_UV_Y);	// 戦線復帰カウンター
		}
		if (m_pUITexRespawn[0] != NULL || m_pUITexRespawn[1] != NULL || m_pUITexRespawn[2] != NULL || m_pUINumRespawn != NULL)
		{	// 生成している時
			int nRespawn = m_pUINumRespawn->GetRespawn();			// CUI_NUMBERから、戦線復帰カウンター取得
			if (nRespawn <= 0)
			{	// カウンター0以下になった時
				respawn = RESPAWN_SELECT;		// リスポーン選択状態に設定

				for (int nCnt = 0; nCnt < RESPAWN_TEX; nCnt++)
				{
					if (m_pUITexRespawn[nCnt] != NULL)
					{	// UIテクスチャの破棄
						m_pUITexRespawn[nCnt]->Uninit();
						m_pUITexRespawn[nCnt] = NULL;
					}
				}

				if (m_pUINumRespawn != NULL)
				{	// 戦線復帰カウンターの破棄
					m_pUINumRespawn->Uninit();
					m_pUINumRespawn = NULL;
				}
			}
		}
		break;
	}

	m_Respawn = respawn;

#ifdef _DEBUG
	CDebugProc::Print("m_nDisTime : %d\n", m_nDisTime);
	CDebugProc::Print("m_nRespawnTimer : %d\n", m_nRespawnTimer);
#endif
}

//=========================================
// リスポーン選択処理
//=========================================
void CPlayer::SelectRespawn(void)
{
	if (m_pUITexSelectRes[0] == NULL || m_pUITexSelectRes[1] == NULL || m_pUITexSelectRes[2] == NULL || m_pUITexSelectRes[3] == NULL || m_pCursor == NULL)
	{	// UIの生成
		m_pUITexSelectRes[0] = CUI_TEXTURE::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), SCREEN_WIDTH, SCREEN_HEIGHT, CUI_TEXTURE::UIFLAME_NONE);	// 下地
		m_pUITexSelectRes[1] = CUI_TEXTURE::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, 360.0f, 0.0f), 650.0f, 370.0f, CUI_TEXTURE::UIFLAME_MAP);	// マップ
		m_pUITexSelectRes[2] = CUI_TEXTURE::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, 120.0f, 0.0f), 400.0f, 80.0f, CUI_TEXTURE::UIFLAME_TITLE);	// リスポーンタイトル
		m_pUITexSelectRes[3] = CUI_TEXTURE::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, PLAYER_UI_HEIGHT, 0.0f), 500.0f, 100.0f, CUI_TEXTURE::UIFLAME_RESPAWN_FLAME);		// フレーム
		m_pCursor = CMouseCursor2D::Create();		// カーソル
	}

	for (int nCnt = 0; nCnt < SELECTRESPAWN_BOTTON; nCnt++)
	{
		if (m_pUISelectResBotton[nCnt] == NULL)
		{	// ボタンの生成
			m_pUISelectResBotton[nCnt] = CButton2D::Create(D3DXVECTOR3(470.0f + (nCnt *(PLAYER_BOTTON_WIDTH + PLAYER_BOTTON_INT)), PLAYER_UI_HEIGHT, 0.0f),
				PLAYER_BOTTON_WIDTH, PLAYER_BOTTON_HEIGHT);  	// リスポーンボタンの横幅
			m_pUISelectResBotton[nCnt]->BindTexture(CTexture::GetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_SELECT_RESPAWN)));
			m_pUISelectResBotton[nCnt]->SetTex(nCnt, 1, 4);
		}
	}

	//リスポーン位置のアイコンの生成
	CreateRespawnPosIcon();

	if (m_pUISelectResBotton[0] != NULL || m_pUISelectResBotton[1] != NULL || m_pUISelectResBotton[2] != NULL || m_pUISelectResBotton[3] != NULL)
	{	// 生成されていた時
		bool bBottonSwitch = false;
		int nSelect = -1;
		m_point = POINT_A;

		// ボタンの判定
		for (int nCntButton = 0; nCntButton < SELECTRESPAWN_BOTTON; nCntButton++)
		{
			if (m_pUISelectResBotton[nCntButton]->InRange(m_pCursor->GetMousePosition()))
			{// 範囲内かチェック
				if (m_pUISelectResBotton[nCntButton]->ClickRelease())
				{// クリックされた
					bBottonSwitch = true;
					m_point = (POINT)nCntButton;
					break;
				}
				nSelect = nCntButton;
			}
		}

		for (int nCntButton = 0; nCntButton < SELECTRESPAWN_BOTTON; nCntButton++)
		{
			if (m_pUIRespawnPosIcon[nCntButton] != NULL)
			{
				if (nSelect == nCntButton)
				{
					m_pUIRespawnPosIcon[nCntButton]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				}
				else
				{
					m_pUIRespawnPosIcon[nCntButton]->SetColor(D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f));
				}
			}
		}

		// ボタン押された時の処理
		if (bBottonSwitch)
		{	// trueの時
			switch (m_point)
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

			m_pos = CManager::GetGame()->GetRespawnPos(m_nTeam, m_point);

			// 通常状態に戻る
			m_Respawn = RESPAWN_NONE;

			// ライフの設定
			m_nLife = m_nLifeMax;

			//死亡していないようにする
			m_bDeath = false;

			// リスポーン地点が決定したら破棄する
			for (int nCnt = 0; nCnt < SELECTRESPAWN_BOTTON; nCnt++)
			{
				if (m_pUISelectResBotton[nCnt] != NULL)
				{	// ボタンの破棄
					m_pUISelectResBotton[nCnt]->Uninit();
					m_pUISelectResBotton[nCnt] = NULL;
				}
			}

			for (int nCnt = 0; nCnt < SERECTRESPAWN_TEX; nCnt++)
			{
				if (m_pUITexSelectRes[nCnt] != NULL)
				{	// テクスチャの破棄
					m_pUITexSelectRes[nCnt]->Uninit();
					m_pUITexSelectRes[nCnt] = NULL;
				}
			}

			for (int nCnt = 0; nCnt < SELECTRESPAWN_BOTTON; nCnt++)
			{
				if (m_pUIRespawnPosIcon[nCnt] != NULL)
				{	// テクスチャの破棄
					m_pUIRespawnPosIcon[nCnt]->Uninit();
					m_pUIRespawnPosIcon[nCnt] = NULL;
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

//=============================================================================
//	ラジオチャットボタン生成処理
//=============================================================================
void CPlayer::ChatBotton(void)
{
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();	// キーボードの入力を取得
	CDirectInput *pDirectInput = CManager::GetDirectInput();	//DirectInputの取得
	CDirectInput::GamePad *DirectInputStick = pDirectInput->GetgamePadStick();

	if (pKeyboard->GetTrigger(DIK_M) || pDirectInput->GetGamePadTrigger(1))
	{
		if (m_pUIRadioBotton[0] == NULL || m_pUIRadioBotton[1] == NULL || m_pUIRadioBotton[2] == NULL || m_pUIRadioBotton[3] == NULL
			|| m_pUIRadioBotton[4] == NULL || m_pUIRadioBotton[5] == NULL || m_pUIRadioBotton[6] == NULL || m_pUIRadioBotton[7] == NULL || m_pCursor == NULL)
		{	// ボタンとカーソルの生成
			m_bChatBotton = true;		// ボタン表示中

			m_pUIRadioBotton[0] = CButton2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 + 100.0f, 180.0f, 0.0f), RADIOCHAT_BOTTON_WIDTH, RADIOCHAT_BOTTON_HEIGHT);
			m_pUIRadioBotton[0]->BindTexture(CTexture::GetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_RADIOCHAT)));
			m_pUIRadioBotton[0]->SetTex(0, 1, RADIOCHAT_BOTTON_PATTERN);

			m_pUIRadioBotton[1] = CButton2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 + 200.0f, 280.0f, 0.0f), RADIOCHAT_BOTTON_WIDTH, RADIOCHAT_BOTTON_HEIGHT);
			m_pUIRadioBotton[1]->BindTexture(CTexture::GetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_RADIOCHAT)));
			m_pUIRadioBotton[1]->SetTex(1, 1, RADIOCHAT_BOTTON_PATTERN);

			m_pUIRadioBotton[2] = CButton2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 + 200.0f, 470.0f, 0.0f), RADIOCHAT_BOTTON_WIDTH, RADIOCHAT_BOTTON_HEIGHT);
			m_pUIRadioBotton[2]->BindTexture(CTexture::GetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_RADIOCHAT)));
			m_pUIRadioBotton[2]->SetTex(2, 1, RADIOCHAT_BOTTON_PATTERN);

			m_pUIRadioBotton[3] = CButton2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 + 100.0f, 570.0f, 0.0f), RADIOCHAT_BOTTON_WIDTH, RADIOCHAT_BOTTON_HEIGHT);
			m_pUIRadioBotton[3]->BindTexture(CTexture::GetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_RADIOCHAT)));
			m_pUIRadioBotton[3]->SetTex(3, 1, RADIOCHAT_BOTTON_PATTERN);

			m_pUIRadioBotton[4] = CButton2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 - 97.0f, 180.0f, 0.0f), RADIOCHAT_BOTTON_WIDTH, RADIOCHAT_BOTTON_HEIGHT);
			m_pUIRadioBotton[4]->BindTexture(CTexture::GetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_RADIOCHAT)));
			m_pUIRadioBotton[4]->SetTex(7, 1, RADIOCHAT_BOTTON_PATTERN);

			m_pUIRadioBotton[5] = CButton2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 - 200.0f, 280.0f, 0.0f), RADIOCHAT_BOTTON_WIDTH, RADIOCHAT_BOTTON_HEIGHT);
			m_pUIRadioBotton[5]->BindTexture(CTexture::GetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_RADIOCHAT)));
			m_pUIRadioBotton[5]->SetTex(6, 1, RADIOCHAT_BOTTON_PATTERN);

			m_pUIRadioBotton[6] = CButton2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 - 200.0f, 470.0f, 0.0f), RADIOCHAT_BOTTON_WIDTH, RADIOCHAT_BOTTON_HEIGHT);
			m_pUIRadioBotton[6]->BindTexture(CTexture::GetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_RADIOCHAT)));
			m_pUIRadioBotton[6]->SetTex(5, 1, RADIOCHAT_BOTTON_PATTERN);

			m_pUIRadioBotton[7] = CButton2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 - 97.0f, 570.0f, 0.0f), RADIOCHAT_BOTTON_WIDTH, RADIOCHAT_BOTTON_HEIGHT);
			m_pUIRadioBotton[7]->BindTexture(CTexture::GetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_RADIOCHAT)));
			m_pUIRadioBotton[7]->SetTex(4, 1, RADIOCHAT_BOTTON_PATTERN);

			m_pCursor = CMouseCursor2D::Create();		// カーソル
		}
	}

	if (m_pUIRadioBotton[0] != NULL || m_pUIRadioBotton[1] != NULL || m_pUIRadioBotton[2] != NULL || m_pUIRadioBotton[3] != NULL
		|| m_pUIRadioBotton[4] != NULL || m_pUIRadioBotton[5] != NULL || m_pUIRadioBotton[6] != NULL || m_pUIRadioBotton[7] != NULL || m_pCursor != NULL)
	{	// ボタンとカーソルが生成された
		m_bChat = false;
		int nSelect = -1;
		//m_radiochat = RADIOCHAT_OK;

		// ボタンの判定
		for (int nCntButton = 0; nCntButton < RADIOCHAT_BOTTON; nCntButton++)
		{
			if (m_pUIRadioBotton[nCntButton]->InRange(m_pCursor->GetMousePosition()))
			{// 範囲内かチェック
				if (m_pUIRadioBotton[nCntButton]->ClickRelease())
				{// クリックされた
					m_bChat = true;
					m_radiochat = (RADIOCHAT)nCntButton;
					break;

				}
				nSelect = nCntButton;
			}
		}
	}
}

//=============================================================================
//	ボタンが押されて、メッセージ表示
//=============================================================================
void CPlayer::ChatMess(bool bChat)
{
	if (m_pUITexRadio == NULL)
	{
		m_bChatBotton = false;		// ボタン非表示中

		m_pUITexRadio = CUI_TEXTURE::Create(D3DXVECTOR3(1280.0f, 550.0f, 0.0f), RADIOCHAT_MESS_WIDTH, RADIOCHAT_MESS_HEIGHT, CUI_TEXTURE::UIFLAME_RADIOCHAT_MESS);
		m_pUITexRadio->SetTex(m_radiochat, 1, RADIOCHAT_BOTTON_PATTERN);
	}
	if (m_pUITexRadio != NULL)
	{
		for (int nCnt = 0; nCnt < RADIOCHAT_BOTTON; nCnt++)
		{
			if (m_pUIRadioBotton[nCnt] != NULL)
			{	// ラジオチャットボタンの破棄
				m_pUIRadioBotton[nCnt]->Uninit();
				m_pUIRadioBotton[nCnt] = NULL;
			}
		}

		if (m_pCursor != NULL)
		{	// カーソルの破棄
			m_pCursor->Uninit();
			m_pCursor = NULL;
		}

		bool bMove = false;		// 止まったかどうか
		D3DXVECTOR3 texPos = m_pUITexRadio->GetPos();		// 現在の位置を取得
		D3DXCOLOR texCol = m_pUITexRadio->GetColor();		// 現在の色を取得

		if (texPos.x >= 1080.0f)
		{	// 設定した位置以上だったら動く
			texPos.x -= m_moveSpeed;					// テクスチャ動かす
		}
		else
		{	// 設定した位置以下だったら、止まる
			bMove = true;		// 止まった！
		}

		if (bMove == true && texCol.a >= 1.0f)
		{	// 止まった && 透明度が1.0以上の時
			m_nTexTimer++;		// カウンター加算
			if (m_nTexTimer % RADIOCHAT_DISPLAY_TIME == 0)
			{	// 5秒経ったら
				m_nTexTimer = 0;
				m_bCol = true;
			}
		}
		if (m_bCol == true)
		{
			texCol.a -= RADIOCHAT_COL;
		}

		m_pUITexRadio->SetPos(texPos);			// 位置を更新
		m_pUITexRadio->SetColor(texCol);		// 色を更新

		if (texCol.a <= 0.0f)
		{	// 完全に色消えた
			if (m_pUITexRadio != NULL)
			{	// メッセージテクスチャの破棄
				m_pUITexRadio->Uninit();
				m_pUITexRadio = NULL;
			}
			m_bChat = false;					// チャットしていない
			bMove = false;
			m_bCol = false;

		}
	}
}

//=============================================================================
//	仲間のメッセージを表示する処理
//=============================================================================
void CPlayer::AllyChatMess(void)
{
	if (m_pUITexAllyRadio != NULL)
	{
		bool bMove = false;		// 止まったかどうか
		D3DXVECTOR3 texPos = m_pUITexAllyRadio->GetPos();		// 現在の位置を取得
		D3DXCOLOR texCol = m_pUITexAllyRadio->GetColor();		// 現在の色を取得

		if (texPos.x >= 1080.0f)
		{	// 設定した位置以上だったら動く
			texPos.x -= m_moveSpeed;					// テクスチャ動かす
		}
		else
		{	// 設定した位置以下だったら、止まる
			bMove = true;		// 止まった！
		}

		if (bMove == true && texCol.a >= 1.0f)
		{	// 止まった && 透明度が1.0以上の時
			m_nTexTimer++;		// カウンター加算
			if (m_nTexTimer % RADIOCHAT_DISPLAY_TIME == 0)
			{	// 5秒経ったら
				m_nTexTimer = 0;
				m_bAllyCol = true;
			}
		}

		if (m_bAllyCol == true)
		{
			texCol.a -= RADIOCHAT_COL;
		}

		m_pUITexAllyRadio->SetPos(texPos);			// 位置を更新
		m_pUITexAllyRadio->SetColor(texCol);		// 色を更新

		if (texCol.a <= 0.0f)
		{	// 完全に色消えた
			if (m_pUITexAllyRadio != NULL)
			{	// メッセージテクスチャの破棄
				m_pUITexAllyRadio->Uninit();
				m_pUITexAllyRadio = NULL;
			}
			m_bAllyChat = false;					// チャットしていない
			bMove = false;
			m_bAllyCol = false;
			m_bAllyChat = false;
		}
	}
}

//=============================================================================
//	AI更新処理
//=============================================================================
void CPlayer::AIUpdate(void)
{
	//CDebugProc::Print("========プレイヤー========\n");
	//CDebugProc::Print("プレイヤーの位置 x : %.1f / z : %.1f\n", m_pos.x, m_pos.z);
	//CDebugProc::Print("開始地点 : %d\n", m_nStartNode);
	//CDebugProc::Print("目的地 : %d\n", m_nEndNode);
	//CDebugProc::Print("休憩時間: %d\n", m_nBreaktime);
	//CDebugProc::Print("データ収集カウンター : %d\n", m_nCollectionTimer);
	//CDebugProc::Print("現在の移動回数: %d\n", m_nPoint);
	//CDebugProc::Print("目標までの移動回数: %d\n", m_nCountPoint);
	//CDebugProc::Print("ゴールした回数: %d\n", m_nGoalCount);
	//CDebugProc::Print("\n");

	// 自動移動処理
	CPlayer::AutoMove();
}

//=============================================================================
//	自動移動処理
//=============================================================================
void CPlayer::AutoMove()
{
	CMotionManager::TYPE type = CMotionManager::TYPE_NEUTRAL;	// モーションの種類
	bool bMove = false;	// ボタン押下フラグ
	float VigilanceRot[CONNECT_MAX];

	// 目標地点を設定
	m_posDest = m_waypoint[m_nPoint];

	// 目的との差分を出す
	float fLength = (m_pos.x - m_posDest.x) * (m_pos.x - m_posDest.x) + (m_pos.z - m_posDest.z) * (m_pos.z - m_posDest.z);
	m_nBreaktime--;

	if (fLength > MOVE_ACCEPTABLE)
	{// 差分が許容値内に収まるまで目的地に移動する
		bMove = true;
		m_move.x = sinf(atan2f(m_posDest.x - m_pos.x, m_posDest.z - m_pos.z)) * m_fSpeed;
		m_move.z = cosf(atan2f(m_posDest.x - m_pos.x, m_posDest.z - m_pos.z)) * m_fSpeed;
		m_rot.y = atan2f(m_posDest.x - m_pos.x, m_posDest.z - m_pos.z) + D3DX_PI;
	}
	else if (m_nBreaktime < 0)
	{// 移動中
		bMove = true;
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
		bMove = true;
		m_nPoint++;
	}
	else if (m_nBreaktime > 0)
	{// 休憩中
		bMove = false;
		m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 停止する

		for (int nCntConnect = 0; nCntConnect < m_NodeData.connectNum[m_nStartNode]; nCntConnect++)
		{// 繋がってるノードの数だけ回る
			VigilanceRot[nCntConnect] = atan2f(m_NodeData.pos[m_NodeData.connectIndex[m_nStartNode][nCntConnect]].x - m_pos.x, m_NodeData.pos[m_NodeData.connectIndex[m_nStartNode][nCntConnect]].z - m_pos.z) + D3DX_PI;
		}

		if (m_nBreaktime == PLAYER_FINALPOINT_BREAKTIME - 1 || m_nBreaktime % (PLAYER_FINALPOINT_BREAKTIME / m_NodeData.connectNum[m_nStartNode]) == 0)
		{// 繋がってるノードの方向すべてを見る
			m_rot.y = VigilanceRot[m_nVigilanceCount];
			m_nVigilanceCount++;

			if (m_nVigilanceCount >= m_NodeData.connectNum[m_nStartNode])
			{// 上限まで行ったら初期化
				m_nVigilanceCount = 0;
			}
		}
	}

	if (m_pUpperMotion && m_pLowerMotion)
	{// モーションクラスが使われている
		if (bMove)
		{// 移動している
			// 移動モーション
			m_pUpperMotion->SetMotion(CMotionManager::TYPE_WALK);
			m_pLowerMotion->SetMotion(CMotionManager::TYPE_WALK);
		}
		else
		{// 移動していない
			// ニュートラルモーション
			m_pUpperMotion->SetMotion(CMotionManager::TYPE_NEUTRAL);
			m_pLowerMotion->SetMotion(CMotionManager::TYPE_NEUTRAL);
		}
	}

	m_move.x += (0 - m_move.x) * 0.4f;
	m_move.z += (0 - m_move.z) * 0.4f;

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

	// 移動先の設定
	do
	{// 同じ地点だった場合はもう1度決める
		nMovePoint = m_nMovePoint[rand() % RANDOM_MOVE_POINT];
	} while (m_nNewEndNode == nMovePoint);
	m_nNewEndNode = nMovePoint;

	for (int nCntNode = 0; nCntNode < m_NodeData.nodeMax; nCntNode++)
	{// ノードの数だけ回る
		if (m_NodeData.pos[nCntNode].x + POS_ACCEPTABLE > m_pos.x
			&& m_NodeData.pos[nCntNode].x - POS_ACCEPTABLE < m_pos.x
			&& m_NodeData.pos[nCntNode].z + POS_ACCEPTABLE > m_pos.z
			&& m_NodeData.pos[nCntNode].z - POS_ACCEPTABLE < m_pos.z)
		{// プレイヤーの位置が許容範囲内
			m_nStartNode = m_nEndNode;	// 前回の目的地を開始地点として登録
		}
	}

	for (int nCntNode = 0; nCntNode < m_NodeData.nodeMax; nCntNode++)
	{// ノードの数だけ回る
		if (m_NodeData.pos[nCntNode].x + POS_ACCEPTABLE > m_pos.x
			&& m_NodeData.pos[nCntNode].x - POS_ACCEPTABLE < m_pos.x
			&& m_NodeData.pos[nCntNode].z + POS_ACCEPTABLE > m_pos.z
			&& m_NodeData.pos[nCntNode].z - POS_ACCEPTABLE < m_pos.z)
		{// プレイヤーの位置が目的地の許容範囲内
			if (m_NodeData.pos[nCntNode] != m_NodeData.pos[m_nNewEndNode])
			{// 新規目的地が前回の目的地と同じじゃない場合
				m_nEndNode = m_nNewEndNode;	// 目的地を登録
			}
		}
	}
}

//=============================================================================
//	ルート検索処理
//=============================================================================
void CPlayer::RootSearch()
{
	Node node[NODEPOINT_MAX];		// ノードの情報
	float weight[NODEPOINT_MAX];	// 各エッジのコスト
	int nCntWeight = 0;				// コストのカウンタ
	std::vector<int> path;			// 最短経路の情報を保持するvector

	//======= エッジコストの算出 =========================================================================
	for (int nCntNode = 0; nCntNode < m_NodeData.nodeMax; nCntNode++, nCntWeight++)
	{// ノードの数だけ回る
		weight[nCntWeight] = sqrt((m_NodeData.pos[m_nStartNode].x - m_NodeData.pos[nCntNode].x) * (m_NodeData.pos[m_nStartNode].x - m_NodeData.pos[nCntNode].x) + (m_NodeData.pos[m_nStartNode].z - m_NodeData.pos[nCntNode].z) * (m_NodeData.pos[m_nStartNode].z - m_NodeData.pos[nCntNode].z));
	}

	//======= エッジ追加 =========================================================================
	for (int nCntNode = 0; nCntNode < m_NodeData.nodeMax; nCntNode++)
	{// ノードの数だけ回る
		for (int nCntConnect = 0; nCntConnect < m_NodeData.connectNum[nCntNode]; nCntConnect++)
		{// 繋がってるノードの数だけ回る
			CPlayer::AddEdge(nCntNode, m_NodeData.connectIndex[nCntNode][nCntConnect], weight[nCntNode], node);
		}
	}

	//======= 最短経路を調べる =========================================================================
	CPlayer::Dijkstra(m_NodeData.nodeMax, m_nStartNode, m_nEndNode, node);

	for (int nCntNode = m_nEndNode; nCntNode != m_nStartNode; nCntNode = node[nCntNode].from)
	{// 最短経路をゴールから順にスタートまでたどる
		path.push_back(nCntNode);
	}
	path.push_back(m_nStartNode);

	//======= 最短経路の出力 =========================================================================
	for (int nCntNode = path.size() - 1; nCntNode >= 0; nCntNode--)
	{
		if (m_nCountPoint < nCntNode)
		{// 最大値を代入
		 // 目的までの移動回数を取得する
			m_nCountPoint = nCntNode;
		}
	}

	//======= 目標地点の設定 =========================================================================
	for (int nCntNodeMax = 0; nCntNodeMax < m_NodeData.nodeMax; )
	{// ノードの数だけ回る
		for (int nCntNode = path.size() - 1; nCntNode >= 0; nCntNode--, nCntNodeMax++)
		{
			m_waypoint[nCntNodeMax] = m_NodeData.pos[path[nCntNode]];
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

//=============================================================================
// ルート探索用ファイルの読み込み
//=============================================================================
void CPlayer::FileLoad(char* pFileName)
{
	FILE* pFile = NULL;		// ファイルポインタ
	char ReadText[256];		// 読み込んだ文字列を入れておく
	char HeadText[256];		// 比較用
	char DustBox[256];		// 使用しないものを入れておく
	int nCount = 0;
	int nCntIndex = 0;

	// 一時データベース
	std::vector<NodeState> LoadState; LoadState.clear();

	// 初期化
	NodeState OneState = {};

	// ファイルオープン
	pFile = fopen(pFileName, "r");

	if (pFile != NULL)
	{// ファイルが開かれていれば
		while (strcmp(HeadText, "START_LOAD") != 0)
		{// "START_LOAD" が読み込まれるまで繰り返し文字列を読み取る
			fgets(ReadText, sizeof(ReadText), pFile);
			sscanf(ReadText, "%s", &HeadText);
		}
		if (strcmp(HeadText, "START_LOAD") == 0)
		{// "START_LOAD" が読み取れた場合、処理開始
			while (strcmp(HeadText, "END_LOAD") != 0)
			{// "END_LOAD" が読み込まれるまで繰り返し文字列を読み取る
				fgets(ReadText, sizeof(ReadText), pFile);
				sscanf(ReadText, "%s", &HeadText);

				if (strcmp(HeadText, "\n") == 0)
				{// 文字列の先頭が [\n](改行) の場合処理しない

				}
				else if (strcmp(HeadText, "START_DATA") == 0)
				{// "START_DATA" が読み取れた場合
					nCount = 0;
					while (strcmp(HeadText, "END_DATA") != 0)
					{// "END_DATA" が読み込まれるまで繰り返し文字列を読み取る
						fgets(ReadText, sizeof(ReadText), pFile);
						sscanf(ReadText, "%s", &HeadText);

						if (strcmp(HeadText, "\n") == 0)
						{// 文字列の先頭が [\n](改行) の場合処理しない

						}
						else if (strcmp(HeadText, "NODESET") == 0)
						{// "NODESET" が読み取れた場合
							while (strcmp(HeadText, "END_NODESET") != 0)
							{// "END_NODESET" が読み込まれるまで繰り返し文字列を読み取る
								fgets(ReadText, sizeof(ReadText), pFile);
								sscanf(ReadText, "%s", &HeadText);

								if (strcmp(HeadText, "\n") == 0)
								{// 文字列の先頭が [\n](改行) の場合処理しない

								}
								else if (strcmp(HeadText, "NODE_POS") == 0)	// ノードの位置
								{
									sscanf(ReadText, "%s %c %f %f %f",
										&DustBox, &DustBox,
										&OneState.pos[nCount].x,
										&OneState.pos[nCount].y,
										&OneState.pos[nCount].z);
								}
								else if (strcmp(HeadText, "CONNECT_NUM") == 0)	// 接続ノードの数
								{
									sscanf(ReadText, "%s %c %d",
										&DustBox, &DustBox,
										&OneState.connectNum[nCount]);
								}
								else if (strcmp(HeadText, "CONNECT_INDEX") == 0)	// 接続ノードの番号
								{
									sscanf(ReadText, "%s %c %d",
										&DustBox, &DustBox,
										&OneState.connectIndex[nCount][nCntIndex]);
									nCntIndex++;
								}
							}

							OneState.index[nCount] = nCount;
							nCntIndex = 0;
							nCount++;
						}
					}
					OneState.nodeMax = nCount; // ノードの総数

											   // 一つのデータを読み込んだ後,一時データベースに格納
					LoadState.emplace_back(OneState);
				}
			}
		}

		// ファイルクローズ
		if (pFile != NULL)
		{
			fclose(pFile);
			pFile = NULL;
		}
	}

	m_NodeData = OneState;	// データの代入
}

//=============================================================================
// リスポーン位置のアイコンを生成
//=============================================================================
void CPlayer::CreateRespawnPosIcon(void)
{
	//スポーン位置のUI生成
	switch (m_nTeam)
	{//チームによって生成分け
	case 0:
		if (m_pUIRespawnPosIcon[0] == NULL && m_pUIRespawnPosIcon[1] == NULL && m_pUIRespawnPosIcon[2] == NULL && m_pUIRespawnPosIcon[3] == NULL)
		{
			m_pUIRespawnPosIcon[0] = CUI_TEXTURE::Create(D3DXVECTOR3(355.0f, 210.0f, 0.0f), 30.0f, 30.0f, CUI_TEXTURE::UI_FLAME_RESPAWNPOS);
			m_pUIRespawnPosIcon[0]->BindTexture(CTexture::GetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_SELECT_RESPAWN)));
			m_pUIRespawnPosIcon[0]->SetTex(0, 1, 4);
			m_pUIRespawnPosIcon[0]->SetColor(D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f));

			m_pUIRespawnPosIcon[1] = CUI_TEXTURE::Create(D3DXVECTOR3(355.0f, 330.0f, 0.0f), 30.0f, 30.0f, CUI_TEXTURE::UI_FLAME_RESPAWNPOS);
			m_pUIRespawnPosIcon[1]->BindTexture(CTexture::GetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_SELECT_RESPAWN)));
			m_pUIRespawnPosIcon[1]->SetTex(1, 1, 4);
			m_pUIRespawnPosIcon[1]->SetColor(D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f));

			m_pUIRespawnPosIcon[2] = CUI_TEXTURE::Create(D3DXVECTOR3(355.0f, 390.0f, 0.0f), 30.0f, 30.0f, CUI_TEXTURE::UI_FLAME_RESPAWNPOS);
			m_pUIRespawnPosIcon[2]->BindTexture(CTexture::GetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_SELECT_RESPAWN)));
			m_pUIRespawnPosIcon[2]->SetTex(2, 1, 4);
			m_pUIRespawnPosIcon[2]->SetColor(D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f));

			m_pUIRespawnPosIcon[3] = CUI_TEXTURE::Create(D3DXVECTOR3(355.0f, 485.0f, 0.0f), 30.0f, 30.0f, CUI_TEXTURE::UI_FLAME_RESPAWNPOS);
			m_pUIRespawnPosIcon[3]->BindTexture(CTexture::GetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_SELECT_RESPAWN)));
			m_pUIRespawnPosIcon[3]->SetTex(3, 1, 4);
			m_pUIRespawnPosIcon[3]->SetColor(D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f));
		}
		break;
	case 1:
		if (m_pUIRespawnPosIcon[0] == NULL && m_pUIRespawnPosIcon[1] == NULL && m_pUIRespawnPosIcon[2] == NULL && m_pUIRespawnPosIcon[3] == NULL)
		{
			m_pUIRespawnPosIcon[0] = CUI_TEXTURE::Create(D3DXVECTOR3(925.0f, 210.0f, 0.0f), 30.0f, 30.0f, CUI_TEXTURE::UI_FLAME_RESPAWNPOS);
			m_pUIRespawnPosIcon[0]->BindTexture(CTexture::GetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_SELECT_RESPAWN)));
			m_pUIRespawnPosIcon[0]->SetTex(0, 1, 4);
			m_pUIRespawnPosIcon[0]->SetColor(D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f));

			m_pUIRespawnPosIcon[1] = CUI_TEXTURE::Create(D3DXVECTOR3(925.0f, 330.0f, 0.0f), 30.0f, 30.0f, CUI_TEXTURE::UI_FLAME_RESPAWNPOS);
			m_pUIRespawnPosIcon[1]->BindTexture(CTexture::GetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_SELECT_RESPAWN)));
			m_pUIRespawnPosIcon[1]->SetTex(1, 1, 4);
			m_pUIRespawnPosIcon[1]->SetColor(D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f));

			m_pUIRespawnPosIcon[2] = CUI_TEXTURE::Create(D3DXVECTOR3(925.0f, 390.0f, 0.0f), 30.0f, 30.0f, CUI_TEXTURE::UI_FLAME_RESPAWNPOS);
			m_pUIRespawnPosIcon[2]->BindTexture(CTexture::GetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_SELECT_RESPAWN)));
			m_pUIRespawnPosIcon[2]->SetTex(2, 1, 4);
			m_pUIRespawnPosIcon[2]->SetColor(D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f));

			m_pUIRespawnPosIcon[3] = CUI_TEXTURE::Create(D3DXVECTOR3(925.0f, 510.0f, 0.0f), 30.0f, 30.0f, CUI_TEXTURE::UI_FLAME_RESPAWNPOS);
			m_pUIRespawnPosIcon[3]->BindTexture(CTexture::GetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_SELECT_RESPAWN)));
			m_pUIRespawnPosIcon[3]->SetTex(3, 1, 4);
			m_pUIRespawnPosIcon[3]->SetColor(D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f));
		}
		break;
		break;
	}

}
