//=============================================================================
//
// AI処理 [AI.cpp]
// Author : Komatsu Keisuke
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "AI.h"
#include "player.h"
#include "manager.h"
#include "camera.h"
#include "renderer.h"
#include "debugProc.h"
#include "model.h"
#include "motion.h"
#include "game.h"
#include "bullet.h"
#include "server.h"
#include "collisionSet.h"
#include "input.h"
#include "button.h"
#include "mouseCursor.h"
#include "menu.h"
#include "nodeDataFiler.h"
#include "search.h"
#include "particle.h"
#include "game.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define WALKER_FILE	"data/TEXT/AI/walker/model_walker.txt"
#define DRONE_FILE	"data/TEXT/AI/drone/model_drone.txt"
#define AI_SPEED			(2.7f)		// 移動速度

// 移動系AI関連
#define	LOAD_FILENAME		("data/TEXT/NODE_DATA/NodeData.txt")
//#define	LOAD_FILENAME	("data/TEXT/NODE_DATA/NodeDataTutorial.txt")	// 読み込むファイルのパス
#define ENEMY_BREAKTIME		(1)			// 休憩時間(フレーム)
#define MOUSE_ACCEPTABLE	(20.0f)		// マウスの誤差の許容範囲
#define MOVE_ACCEPTABLE		(25.0f)		// 移動の誤差の許容範囲
#define POS_ACCEPTABLE		(100.0f)	// 位置の誤差の許容範囲

// 戦闘系AI関連
#define ATTACK_DISPERTION	(50)			// 弾のブレ
#define ATTACK_AREA			(70000.0f)		// 攻撃範囲
#define MAX_CHAR			(254)			// 読み取る文字数
#define MAX_SEARCH			(4)				// センサー数
#define FIND_FIND_CHARACTER_PRIORITY (4)	// 探すプレイヤーの優先順位

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================

//=============================================================================
// 生成処理
//=============================================================================
CAIMecha* CAIMecha::Create(CPlayer *pPlayer, MECHATYPE mecha, D3DXVECTOR3 pos)
{
	CAIMecha *pAI;

	pAI = new CAIMecha;	// メモリを確保

	if (pAI != NULL)
	{// メモリ確保成功
		pAI->m_mechaType = mecha;
		pAI->m_pos = pos;
		pAI->m_pPlayer = pPlayer;
		pAI->Init();
	}

	return pAI;
}

//=============================================================================
// コンストラクタ
//=============================================================================
CAIMecha::CAIMecha(int nPriority, CScene::OBJTYPE objType) : CScene(nPriority, objType)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nCntState = 0;
	m_pModel = NULL;
	m_pMotion = NULL;
	m_nNumParts = 0;
	m_vtxMax = D3DXVECTOR3(-1000.0f, -1000.0f, -1000.0f);
	m_vtxMin = D3DXVECTOR3(1000.0f, 1000.0f, 1000.0f);
	m_nLife = 0;
	m_fSpeed = 0.0f;
	m_nAttack = 0;
	m_mechaType = MECHATYPE_WALKER;
	m_nCapacity = 0;
	m_nCurMotion = 0;
	m_nLifeMax = 0;
	m_nNumShoot = 0;
	m_pPlayer = NULL;
	m_nTeam = 0;
	m_bDeath = false;
	m_nKillPlayerIdx = 0;
}

//=============================================================================
// デストラクタ
//=============================================================================
CAIMecha::~CAIMecha() {}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CAIMecha::Init(void)
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
	char *pFileName = "";

	if (MECHATYPE_WALKER == m_mechaType) { pFileName = WALKER_FILE; }
	else if (MECHATYPE_DRONE == m_mechaType) { pFileName = DRONE_FILE; }

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
							{// 装弾数
								fscanf(pFile, " = %d", &m_nCapacity);
							}
							else if (strcmp(aStr, "ATTACK") == 0)
							{// 攻撃力
								fscanf(pFile, " = %d", &m_nAttack);
							}
							else if (strcmp(aStr, "SHOOTS") == 0)
							{// 発射数
								fscanf(pFile, " = %d", &m_nNumShoot);
							}
							else if (strcmp(aStr, "LIFE") == 0)
							{// 耐久力
								fscanf(pFile, " = %d", &m_nLifeMax);
							}
							else if (strcmp(aStr, "SPEED") == 0)
							{// 移動量
								fscanf(pFile, " = %f", &m_fSpeed);
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

	if (NULL == m_pMotion)
	{// モーションクラスの生成
		m_pMotion = CMotion::Create(m_mechaType, m_nNumParts, m_pModel);
		m_pMotion->SetMotion(CMotionManager::TYPE_NEUTRAL);	// ニュートラルモーションを設定
	}

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

	// 全パーツの頂点座標の最小値・最大値
	m_vtxMin.x = posMin.x;
	m_vtxMin.y = posMin.y;
	m_vtxMin.z = posMin.z;
	m_vtxMax.x = posMax.x;
	m_vtxMax.y = posMax.y;
	m_vtxMax.z = posMax.z;

	m_nLife = m_nLifeMax;	// ライフの初期値

	if (m_pPlayer->GetTeam() == 0)
	{
		for (int nCntModel = 0; nCntModel < m_nNumParts; nCntModel++)
		{// チームごとの色に設定
			m_pModel[nCntModel]->SetColor(D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
		}

		// チームの設定
		m_nTeam = 0;
	}
	else if (m_pPlayer->GetTeam() == 1)
	{
		for (int nCntModel = 0; nCntModel < m_nNumParts; nCntModel++)
		{// チームごとの色に設定
			m_pModel[nCntModel]->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
		}

		// チームの設定
		m_nTeam = 1;
	}

	// 戦闘系AI数値の初期化
	m_bFind = false;

	// 移動系AI数値の初期化
	m_pNodeData = CGame::GetNodeFiler();	// ファイル情報の取得

											// パート関係
	m_bPartSwitch = CGame::PART_ACTION;
	m_bPartSwitchOld = CGame::PART_ACTION;

	// ノード関係
	m_nStartNode = 0;
	m_nEndNode = 0;
	m_nNodeOld = 0;

	// ラリー関係
	m_nRallyCount = 0;
	m_nRallyCountOld = 0;

	// パトロール関係
	m_bPatrol = false;
	m_nPatrolStartNode = 0;
	m_nCountPatrolPoint = 0;

	// 自動移動関係
	m_posDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nBreaktime = 0;
	m_nCountPoint = 0;
	m_nPoint = 0;
	m_bGoal = false;

	for (int nCntRally = 0; nCntRally < RALLYPOINT_MAX; nCntRally++)
	{// ラリーポイントの最大数分回る
		for (int nCntNode = 0; nCntNode < NODEPOINT_MAX; nCntNode++)
		{// ノードの最大数分回る
			m_node[nCntRally][nCntNode] = {};
		}
	}

	for (int nCntEnemy = 0; nCntEnemy < ENEMY_PLAYER_MAX; nCntEnemy++)
	{// エネミーの最大数分回る
		m_nRallyEndNode[nCntEnemy] = 0;
	}

	for (int nCntNode = 0; nCntNode < NODEPOINT_MAX; nCntNode++)
	{// ノードの最大値数回る
		m_waypoint[nCntNode] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_patrolWaypoint[nCntNode] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	for (int nCntAction = 0; nCntAction < 4; nCntAction++)
	{// 行動数の分回る
		m_AIAction[nCntAction] = AI_ACTION_NONE;
		m_LogicTree[nCntAction] = -1;
	}

	// マップデータファイルの読み込み
	m_pNodeData->FileLoad(LOAD_FILENAME);

	// 開始時点のノードの初期化
	float fMinLength = 100000, fLength = 100000;	// 差分系
	for (int nCntNode = 0; nCntNode < m_pNodeData->GetLoadData().nodeMax; nCntNode++)
	{// ノードの数だけ回る
	 // 差分を求める
		fLength = (m_pNodeData->GetLoadData().pos[nCntNode].x - m_pos.x) * (m_pNodeData->GetLoadData().pos[nCntNode].x - m_pos.x) + (m_pNodeData->GetLoadData().pos[nCntNode].z - m_pos.z) * (m_pNodeData->GetLoadData().pos[nCntNode].z - m_pos.z);

		if (fMinLength > fLength)
		{// 差分の最小値を求める
			fMinLength = fLength;
			m_nStartNode = nCntNode;
		}
	}
	m_nEndNode = m_nStartNode;

	// ポイントへの経路探索
	CAIMecha::RootSearch();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CAIMecha::Uninit(void)
{
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

	// オブジェクトの破棄
	CScene::Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CAIMecha::Update(void)
{
	m_posOld = m_pos;

	if (NULL != m_pMotion && MECHATYPE_WALKER == m_mechaType)
	{// モーション
		m_pMotion->SetMotion(CMotionManager::TYPE_NEUTRAL);
	}

	if (MECHATYPE_DRONE == m_mechaType)
	{// ドローンのプロペラを回す
		D3DXVECTOR3 rot = m_pModel[1]->GetRot();
		rot.y += 2.0f;
		m_pModel[1]->SetRot(rot);
	}

	if (m_bDeath == false)
	{
		for (int nCntModel = 0; nCntModel < m_nNumParts; nCntModel++)
		{//表示しない
			m_pModel[nCntModel]->SetDisp(false);
		}

		// AI関係の更新処理
		CAIMecha::AIUpdate();

	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CAIMecha::Draw(void)
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


//=============================================================================
// ダメージ処理
//=============================================================================
void CAIMecha::Damage(int nDamage, CScene *pScene)
{
	if (CMenu::GetMode() == CMenu::MODE_SINGLE)
	{//シングルプレイの場合
		if (m_nLife > 0 && m_bDeath == false)
		{
			m_state = STATE_DAMAGE;	// ダメージを受けている状態にする

			m_nLife -= nDamage;	//体力の減算

			if (0 >= m_nLife)
			{//体力が０以下の場合
				m_nLife = 0;			//体力を０にする
				m_bDeath = true;		//死亡状態にする

				if (m_bDeath == true && CManager::GetMode() == CManager::MODE_GAME)
				{//死亡している場合
					for (int nCntKill = 0; nCntKill < NUM_KILL_LOG; nCntKill++)
					{
						//キルログの表示処理
						if (CManager::GetGame()->GetLog(nCntKill) == false)
						{//ログが使用されていない場合
							if (pScene->GetObjType() == CScene::OBJTYPE_PLAYER)
							{//オブジェクトの種類がプレイヤーの場合
								CPlayer *pPlayer = (CPlayer*)pScene;
								if (pPlayer != NULL)
								{//NULLではない場合
									m_nKillPlayerIdx = pPlayer->GetPlayerIdx();									//キルプレイヤーの番号を設置処理
									CManager::GetGame()->SetKillIdx(nCntKill, pPlayer->GetPlayerIdx());			//キルプレイヤーの番号を設置処理
									CManager::GetGame()->SetDeathIdx(nCntKill, m_pPlayer->GetPlayerIdx());		//デスプレイヤーの番号を設置処理
									CManager::GetGame()->SetPlayerType(0, CGame::TYPE_PLAYER);					//プレイヤーの種類を設置処理
									CManager::GetGame()->SetPlayerType(1, CGame::TYPE_DROWN);					//プレイヤーの種類を設置処理
									CManager::GetGame()->SetLog(nCntKill, true);								//ログの設置処理
								}
							}
							else if (pScene->GetObjType() == CScene::OBJTYPE_AI)
							{//オブジェクトの種類がＡＩの場合
								CAIMecha *pAIMecha = (CAIMecha*)pScene;
								if (pAIMecha != NULL)
								{//NULLの場合
									m_nKillPlayerIdx = pAIMecha->GetPlayer()->GetPlayerIdx();								//キルプレイヤーの番号を設置処理
									CManager::GetGame()->SetKillIdx(nCntKill, pAIMecha->GetPlayer()->GetPlayerIdx());		//キルプレイヤーの番号を設置処理
									CManager::GetGame()->SetDeathIdx(nCntKill, m_pPlayer->GetPlayerIdx());					//デスプレイヤーの番号を設置処理
									if (pAIMecha->GetMechaType() == CAIMecha::MECHATYPE_DRONE)
									{//オブジェクトの種類がドローンの場合
										CManager::GetGame()->SetPlayerType(0, CGame::TYPE_DROWN);							//プレイヤーの種類を設置処理
										CManager::GetGame()->SetPlayerType(1, CGame::TYPE_DROWN);							//プレイヤーの種類を設置処理
										CManager::GetGame()->SetLog(nCntKill, true);										//ログの設置処理
									}
									else if (pAIMecha->GetMechaType() == CAIMecha::MECHATYPE_WALKER)
									{//オブジェクトの種類がワーカーの場合
										CManager::GetGame()->SetPlayerType(0, CGame::TYPE_WALKER);							//プレイヤーの種類を設置処理
										CManager::GetGame()->SetPlayerType(1, CGame::TYPE_DROWN);							//プレイヤーの種類を設置処理
										CManager::GetGame()->SetLog(nCntKill, true);										//ログの設置処理
									}
								}
							}
							break;
						}
					}
				}

				//パーティクルを生成
				CParticle::Create(m_pModel[0]->GetWorldPos(), 4);
				CParticle::Create(m_pModel[0]->GetWorldPos(), 5);

				for (int nCntModel = 0; nCntModel < m_nNumParts; nCntModel++)
				{//表示しない
					m_pModel[nCntModel]->SetDisp(false);
				}

				//チーム別で処理分け
				switch (m_nTeam)
				{
				case 0:
					CManager::GetGame()->SetBlueLinkEnergy(CManager::GetGame()->GetBlueLinkEnergy() - 20);
					break;
				case 1:
					CManager::GetGame()->SetRedLinkEnergy(CManager::GetGame()->GetRedLinkEnergy() - 20);
					break;
				}
			}
		}
	}
	else
	{//マルチプレイの場合
		if (CManager::GetClient() != NULL)
		{//NULLではない場合
			if (CManager::GetClient()->GetPlayerIdx() == m_pPlayer->GetPlayerIdx())
			{//プレイヤー番号とクライアント番号が同じ場合
				if (m_nLife > 0 && m_bDeath == false)
				{//体力が０より大きく且つ死亡していない場合
					m_state = STATE_DAMAGE;								// ダメージを受けている状態にする

					m_nLife -= nDamage;									// 体力の減算

					if (0 >= m_nLife)
					{//体力が０以下の場合
						m_nLife = 0;		//体力を０にする
						m_bDeath = true;	//死亡状態にする

						if (m_bDeath == true && CManager::GetMode() == CManager::MODE_GAME)
						{//死亡している場合
							for (int nCntKill = 0; nCntKill < NUM_KILL_LOG; nCntKill++)
							{
								//キルログの表示処理
								if (CManager::GetGame()->GetLog(nCntKill) == false)
								{//ログが使用されていない場合
									if (pScene->GetObjType() == CScene::OBJTYPE_PLAYER)
									{//オブジェクトの種類がプレイヤーの場合
										CPlayer *pPlayer = (CPlayer*)pScene;
										if (pPlayer != NULL)
										{
											m_nKillPlayerIdx = pPlayer->GetPlayerIdx();								//キルプレイヤーの番号を設置処理
											CManager::GetGame()->SetKillIdx(nCntKill, pPlayer->GetPlayerIdx());		//キルプレイヤーの番号を設置処理
											CManager::GetGame()->SetDeathIdx(nCntKill, m_pPlayer->GetPlayerIdx());	//デスプレイヤーの番号を設置処理
											CManager::GetGame()->SetPlayerType(0, CGame::TYPE_PLAYER);				//プレイヤーの種類を設置処理
											CManager::GetGame()->SetPlayerType(1, CGame::TYPE_DROWN);				//プレイヤーの種類を設置処理
											CManager::GetGame()->SetLog(nCntKill, true);							//ログの設置処理
										}
									}
									else if (pScene->GetObjType() == CScene::OBJTYPE_AI)
									{//オブジェクトの種類がAIの場合
										CAIMecha *pAIMecha = (CAIMecha*)pScene;
										if (pAIMecha != NULL)
										{
											m_nKillPlayerIdx = pAIMecha->GetPlayer()->GetPlayerIdx();							//キルプレイヤーの番号を設置処理
											CManager::GetGame()->SetKillIdx(nCntKill, pAIMecha->GetPlayer()->GetPlayerIdx());	//キルプレイヤーの番号を設置処理
											CManager::GetGame()->SetDeathIdx(nCntKill, m_pPlayer->GetPlayerIdx());				//デスプレイヤーの番号を設置処理
											if (pAIMecha->GetMechaType() == CAIMecha::MECHATYPE_DRONE)
											{//オブジェクトの種類がドローンの場合
												CManager::GetGame()->SetPlayerType(0, CGame::TYPE_DROWN);						//プレイヤーの種類を設置処理
												CManager::GetGame()->SetPlayerType(1, CGame::TYPE_DROWN);						//プレイヤーの種類を設置処理
												CManager::GetGame()->SetLog(nCntKill, true);									//ログの設置処理
											}
											else if (pAIMecha->GetMechaType() == CAIMecha::MECHATYPE_WALKER)
											{//オブジェクトの種類がワーカーの場合
												CManager::GetGame()->SetPlayerType(0, CGame::TYPE_WALKER);						//プレイヤーの種類を設置処理
												CManager::GetGame()->SetPlayerType(1, CGame::TYPE_DROWN);						//プレイヤーの種類を設置処理
												CManager::GetGame()->SetLog(nCntKill, true);									//ログの設置処理
											}
										}
									}
									break;
								}
							}
						}

						//パーティクルを生成
						CParticle::Create(m_pModel[0]->GetWorldPos(), 4);
						CParticle::Create(m_pModel[0]->GetWorldPos(), 5);

						for (int nCntModel = 0; nCntModel < m_nNumParts; nCntModel++)
						{
							//表示をしない処理
							m_pModel[nCntModel]->SetDisp(false);
						}

						//チーム別で処理分け
						switch (m_nTeam)
						{
						case 0:
							CManager::GetGame()->SetBlueLinkEnergy(CManager::GetGame()->GetBlueLinkEnergy() - 20);
							break;
						case 1:
							CManager::GetGame()->SetRedLinkEnergy(CManager::GetGame()->GetRedLinkEnergy() - 20);
							break;
						}
					}
				}
			}
		}
	}
}

//=============================================================================
//	AI更新処理
//=============================================================================
void CAIMecha::AIUpdate()
{
	// パート情報
	//CDebugProc::Print("========AI========\n");
	//CDebugProc::Print("ゴールに到着%s。\n", m_bGoal ? "しました" : "してません");
	//CDebugProc::Print("AIPos :%.1f, %.1f\n", m_pos.x, m_pos.z);
	//CDebugProc::Print("\n");

	//CDebugProc::Print("クリック回数 : %d\n", m_nRallyCount);

	//if (m_AIAction[2] == AI_ACTION_FOCUS_GOAL)
	//{// 通常時
	//	CDebugProc::Print("目標優先状態\n");
	//	CDebugProc::Print("開始地点 : %d\n", m_nStartNode);
	//	CDebugProc::Print("終了地点 : %d\n", m_nEndNode);
	//	CDebugProc::Print("\n");
	//}
	//else if (m_AIAction[2] == AI_ACTION_RALLY)
	//{// ラリー時
	//	CDebugProc::Print("ラリー状態\n");
	//	CDebugProc::Print("開始地点 : %d\n", m_nRallyEndNode[0]);
	//	for (int nCntRally = 1; nCntRally < m_nRallyCount; nCntRally++)
	//	{
	//		CDebugProc::Print("中間地点[%d] : %d\n", nCntRally, m_nRallyEndNode[nCntRally]);
	//	}
	//	CDebugProc::Print("終了地点 : %d\n", m_nRallyEndNode[m_nRallyCount]);
	//	CDebugProc::Print("\n");
	//}
	//else if (m_AIAction[2] == AI_ACTION_ROUND_TRIP)
	//{// 往復移動時
	//	CDebugProc::Print("往復移動状態\n");
	//	CDebugProc::Print("往復移動開始地点: %d\n", m_nStartNode);
	//	CDebugProc::Print("開始地点 : %d\n", m_nRallyEndNode[0]);
	//	for (int nCntRally = 1; nCntRally < m_nRallyCount; nCntRally++)
	//	{
	//		CDebugProc::Print("中間地点[%d] : %d\n", nCntRally, m_nRallyEndNode[nCntRally]);
	//	}
	//	CDebugProc::Print("終了地点 : %d\n", m_nRallyEndNode[m_nRallyCount]);
	//	CDebugProc::Print("\n");
	//}
	//else if (m_AIAction[1] == AI_ACTION_FOLLOW)
	//{// 追従時
	//	CDebugProc::Print("追従状態\n");
	//	CDebugProc::Print("開始地点 : %d\n", m_nStartNode);
	//	CDebugProc::Print("終了地点 : %d\n", m_nEndNode);
	//	CDebugProc::Print("\n");
	//}

	//if (m_AIAction[2] != AI_ACTION_ROUND_TRIP)
	//{// 往復移動以外
	//	CDebugProc::Print("現在の移動回数 : %d\n", m_nPoint);
	//	CDebugProc::Print("目標までの移動回数 : %d\n", m_nCountPoint);
	//	CDebugProc::Print("\n");
	//}
	//else
	//{// 往復移動時
	//	CDebugProc::Print("現在の移動回数 : %d\n", m_nPoint);
	//	CDebugProc::Print("目標までの移動回数 : %d\n", m_nCountPatrolPoint);
	//	CDebugProc::Print("\n");
	//}

	//CDebugProc::Print("m_bFind : %s\n", m_bFind ? "true" : "false");

	CInputMouse *pMouse = CManager::GetInputMouse();			// マウスの入力を取得
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();	// キーボードの入力を取得

																// 前回のパート情報の保存
	m_bPartSwitchOld = m_bPartSwitch;
	// 現在のパート情報を取得
	m_bPartSwitch = CManager::GetGame()->GetPart();

	// 中断処理
	Cancel();

	// 攻撃関係処理
	Attack();

	// AI行動設定
	AIActionSet(pMouse);

	if (m_bPartSwitch == CGame::PART_ACTION)
	{// アクションパートの場合
		m_nRallyCount = 0;

		// 自動移動処理
		CAIMecha::AutoMove();
	}
	else if (m_bPartSwitch == CGame::PART_STRATEGY)
	{// ストラテジーパートの場合
		if (m_LogicTree[0] != -1 && pMouse->GetTrigger(CInputMouse::DIMS_BUTTON_0) == true)
		{// AIの行動が決定している状態で左クリックされた場合
			m_nPoint = 0;
			m_nCountPoint = -1;

			// 経路探索方法
			if (m_AIAction[2] == AI_ACTION_ROUND_TRIP)// 往復移動時
			{// ポイント間を徘徊する経路探索
				CAIMecha::RootSearch();
				CAIMecha::PatrolRootSearch();
			}
			else if (m_AIAction[2] == AI_ACTION_RALLY)// ラリー時
			{// ラリーポイントによる経路探索
				CAIMecha::RallyRootSearch();
			}
			if (m_AIAction[2] == AI_ACTION_FOCUS_GOAL)// 目標優先時
			{// ポイントへの経路探索
				CAIMecha::RootSearch();
			}
		}
	}
}

//=============================================================================
//	AI行動設定処理
//=============================================================================
void CAIMecha::AIActionSet(CInputMouse *pMouse)
{
	if (m_bPartSwitch == CGame::PART_STRATEGY && CManager::GetGame()->GetButtonManager() != NULL)
	{// ストラテジーパート時でボタンマネージャーがNULLじゃない場合
		if (CManager::GetGame()->GetButtonManager()->GetSelectFinish() == true)
		{// AIの行動が決定している場合
			if (CManager::GetGame()->GetButtonManager()->GetSelectAIType() == m_mechaType)
			{// 自分のメカタイプが選択されている場合
				for (int nCntAction = 0; nCntAction < 4; nCntAction++)
				{// 行動数の分回る
				 // データの初期化
					m_AIAction[nCntAction] = AI_ACTION_NONE;
					m_LogicTree[nCntAction] = -1;
				}

				for (int nCntButton = 0; nCntButton < 4; nCntButton++)
				{// ボタンの数だけ回る
				 // 指示の取得
					m_LogicTree[nCntButton] = CManager::GetGame()->GetButtonManager()->GetSelectLogic(nCntButton);
				}

				CManager::GetGame()->GetButtonManager()->GetSelectFinish() = false;	// 未決定状態に戻す
			}
		}
	}

	// 行動の設定
	if (m_bPartSwitch == CGame::PART_STRATEGY)
	{// ストラテジーパートの場合
		if (m_LogicTree[0] == 0)// 移動
		{
			m_AIAction[0] = AI_ACTION_MOVE;

			if (m_LogicTree[1] == 0)// 派遣型
			{
				m_AIAction[1] = AI_ACTION_DISPATCH;

				if (m_LogicTree[2] == 0)// 目標重視
				{
					m_AIAction[2] = AI_ACTION_FOCUS_GOAL;
				}
				else if (m_LogicTree[2] == 1)// ラリーポイント
				{
					m_AIAction[2] = AI_ACTION_RALLY;
				}
				else if (m_LogicTree[2] == 2)// 往復移動
				{
					m_AIAction[2] = AI_ACTION_ROUND_TRIP;
				}
			}
			else if (m_LogicTree[1] == 1)// 追従型
			{
				m_AIAction[1] = AI_ACTION_FOLLOW;

				if (m_LogicTree[2] == 0)// 距離：長
				{
					m_AIAction[2] = AI_ACTION_FOLLOW_LONG;
				}
				else if (m_LogicTree[2] == 1)// 距離：短
				{
					m_AIAction[2] = AI_ACTION_FOLLOW_SHORT;
				}
			}

			if (m_LogicTree[3] == 0)// 移動
			{
				m_AIAction[3] = AI_ACTION_MOVE;
			}
			else if (m_LogicTree[3] == 1)// 攻撃
			{
				m_AIAction[3] = AI_ACTION_ATTACK;
			}
		}
		else if (m_LogicTree[0] == 1)// 待機
		{
			m_AIAction[0] = AI_ACTION_WAIT;

			if (m_LogicTree[1] == 0)// 応戦
			{
				m_AIAction[1] = AI_ACTION_BATTLE;

				if (m_LogicTree[2] == 0)// 追跡
				{
					m_AIAction[2] = AI_ACTION_PURSUIT;
				}
				else if (m_LogicTree[2] == 1)// 非追跡
				{
					m_AIAction[2] = AI_ACTION_NOT_PURSUIT;
				}
			}
			else if (m_LogicTree[1] == 1)// 回避
			{
				m_AIAction[1] = AI_ACTION_AVOIDANCE;

				if (m_LogicTree[2] == 0)// 攪乱
				{
					m_AIAction[2] = AI_ACTION_DISTURBANCE;
				}
				else if (m_LogicTree[2] == 1)// 撤退
				{
					m_AIAction[2] = AI_ACTION_WITHDRAWAL;
				}
			}
		}
	}

	// 行動ごとの処理
	if (m_AIAction[0] == AI_ACTION_NONE)
	{// AIの行動が決定していない場合
	 // 追従モードに設定する
		m_AIAction[0] = AI_ACTION_MOVE;
		m_AIAction[1] = AI_ACTION_FOLLOW;
		m_AIAction[2] = AI_ACTION_FOLLOW_SHORT;
	}
	else if (m_AIAction[0] != AI_ACTION_NONE)
	{// AIの行動が決定している場合
		if (m_AIAction[0] == AI_ACTION_MOVE)
		{// 移動の場合
			if (m_AIAction[1] == AI_ACTION_DISPATCH)
			{// 派遣型の場合
				if (pMouse->GetTrigger(CInputMouse::DIMS_BUTTON_0) == true && m_bPartSwitch == CGame::PART_STRATEGY)
				{// ストラテジーパートに左クリックされた場合
					if (m_nRallyCount < RALLYPOINT_MAX)
					{// カウントが最大数まで到達していない場合
					 // 前回の情報の保存
						m_nRallyCountOld = m_nRallyCount;

						if (m_AIAction[2] == AI_ACTION_ROUND_TRIP || m_AIAction[2] == AI_ACTION_RALLY)
						{// ラリー時または往復移動時
							m_nRallyCount++;
						}
						else if (m_AIAction[2] == AI_ACTION_FOCUS_GOAL)
						{// 目標優先行動時
							m_nRallyCount = 1;
						}

						// ポイント検索
						CAIMecha::NodeSearch(m_bGoal);
					}
				}
			}
			else if (m_AIAction[1] == AI_ACTION_FOLLOW)
			{// 追従型の場合
			 // 主人に追従する
				CAIMecha::Follow();
			}
		}
		else if (m_AIAction[0] == AI_ACTION_WAIT)
		{// 待機の場合
		}
	}
}

//=============================================================================
//	攻撃関係処理
//=============================================================================
void CAIMecha::Attack()
{
	CScene *pScene = CScene::GetSceneTop(PLAYER_PRIORITY);
	CScene *pSceneNext = NULL;
	int nCntEnemyPlayer = 0;	// 敵プレイヤーのカウント
	float fAttackLength = 0.0f;	// 差分
	bool bFind[2] = { false, false };	// 発見状態

	while (pScene != NULL)
	{// NULLになるまでループ
		pSceneNext = pScene->GetSceneNext();
		CScene::OBJTYPE objType = pScene->GetObjType();

		if (CScene::OBJTYPE_PLAYER == objType)
		{// プレイヤーオブジェクトのとき
			CPlayer *pPlayer = (CPlayer*)pScene;
			int nTeam = pPlayer->GetTeam();

			//if(nCntEnemyPlayer < ENEMY_PLAYER_MAX)
			if (m_nTeam != nTeam)
			{// チームが違うとき
				m_pEnemyPlayer[nCntEnemyPlayer] = pPlayer;
				nCntEnemyPlayer++;
			}
		}

		// 次のオブジェクトを見る
		pScene = pSceneNext;
	}

	for (int nCntPlayer = 0; nCntPlayer < nCntEnemyPlayer; nCntPlayer++)
	{// 敵プレイヤーの数分回る
		if (m_pEnemyPlayer[nCntPlayer] != NULL)
		{// NULLチェック
			fAttackLength =
				(m_pEnemyPlayer[nCntPlayer]->GetPos().x - m_pos.x) *
				(m_pEnemyPlayer[nCntPlayer]->GetPos().x - m_pos.x) +
				(m_pEnemyPlayer[nCntPlayer]->GetPos().z - m_pos.z) *
				(m_pEnemyPlayer[nCntPlayer]->GetPos().z - m_pos.z);

			if (fAttackLength < ATTACK_AREA)
			{// 範囲内に敵が入った場合

			 // 発見状態にする
				bFind[nCntPlayer] = true;

				// 見つけた敵の方向を向く
				m_rotDest.y = atan2f(m_pEnemyPlayer[nCntPlayer]->GetPos().x - m_pos.x, m_pEnemyPlayer[nCntPlayer]->GetPos().z - m_pos.z) + D3DX_PI;

				// 射出口の位置の取得
				D3DXMATRIX mtxCanon = m_pModel[0]->GetMtxWorld();
				D3DXVECTOR3 posCanon = D3DXVECTOR3(mtxCanon._41, mtxCanon._42 - 8.0f, mtxCanon._43);

				if (rand() % 30 == 0)
				{// ランダムなタイミングで攻撃

				 // 左右攻撃方向の設定
					float fAngle = m_rot.y + D3DX_PI;

					// 上下攻撃方向の設定
					float fAngleV =
						(m_pos.y - m_pEnemyPlayer[nCntPlayer]->GetPos().y) *
						(m_pos.y - m_pEnemyPlayer[nCntPlayer]->GetPos().y) +
						(m_pos.y - m_pEnemyPlayer[nCntPlayer]->GetPos().y) *
						(m_pos.y - m_pEnemyPlayer[nCntPlayer]->GetPos().y);

					// 弾をばらつかせる
					fAngle += (float)(ATTACK_DISPERTION - (rand() % ATTACK_DISPERTION * 2)) * 0.005f;
					fAngleV += (float)(ATTACK_DISPERTION - (rand() % ATTACK_DISPERTION * 2)) * 0.003f;

					// 弾の生成
					CBulletPlayer::Create(posCanon, fAngle, fAngleV, m_nAttack, m_nTeam, this);
				}
			}
			else
			{// 範囲内に敵がいない場合
				bFind[nCntPlayer] = false;
			}
		}
	}

	// 発見状態の遷移
	if (bFind[0] || bFind[1])
	{// どちらか一方でも発見している
		m_bFind = true;
	}
	else if (!bFind[0] && !bFind[1])
	{// どっちも発見していない
		m_bFind = false;
	}
}

//=============================================================================
//	自動移動処理
//=============================================================================
void CAIMecha::AutoMove()
{
	if (m_bPatrol)
	{// 往復移動時
		m_posDest = m_patrolWaypoint[m_nPoint];
	}
	else
	{
		// 目標地点を設定
		m_posDest = m_waypoint[m_nPoint];
	}

	// 目的との差分を出す
	float fLength = (m_pos.x - m_posDest.x) * (m_pos.x - m_posDest.x) + (m_pos.z - m_posDest.z) * (m_pos.z - m_posDest.z);
	m_nBreaktime--;

	if (fLength > MOVE_ACCEPTABLE)
	{// 差分が許容値内に収まるまで目的地に移動する
		m_move.x = sinf(atan2f(m_posDest.x - m_pos.x, m_posDest.z - m_pos.z)) * AI_SPEED;
		m_move.z = cosf(atan2f(m_posDest.x - m_pos.x, m_posDest.z - m_pos.z)) * AI_SPEED;

		if (!m_bFind)
		{// 未発見時
			m_rotDest.y = atan2f(m_posDest.x - m_pos.x, m_posDest.z - m_pos.z) + D3DX_PI;
		}
	}
	else if (m_nBreaktime < 0)
	{// 移動後休憩
		m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_nBreaktime = ENEMY_BREAKTIME;

		if (m_nCountPoint > 0 && m_nCountPoint == m_nPoint && !m_bGoal)
		{// 終了ノードに到着したとき
			m_bGoal = true;

			if (m_AIAction[2] == AI_ACTION_ROUND_TRIP)
			{// 往復移動時
				m_bPatrol = true;
				m_nPoint = 0;
			}
		}
	}
	else if (m_nBreaktime == 0)
	{// 休憩終了
		if (m_nCountPoint > m_nPoint || m_nCountPatrolPoint > m_nPoint)
		{// 目標地点に到達していない場合
			m_nPoint++;
		}
	}

	if (m_AIAction[2] == AI_ACTION_ROUND_TRIP && m_nCountPatrolPoint <= m_nPoint)
	{// 往復移動状態で目標地点に到達している場合
		m_nPoint = 0;
	}

	if (m_rotDest.y > D3DX_PI) { m_rotDest.y -= D3DX_PI * 2.0f; }
	if (m_rotDest.y < -D3DX_PI) { m_rotDest.y += D3DX_PI * 2.0f; }

	if (m_rot.y > D3DX_PI) { m_rot.y -= D3DX_PI * 2.0f; }
	if (m_rot.y < -D3DX_PI) { m_rot.y += D3DX_PI * 2.0f; }

	if (m_rotDest.y - m_rot.y > D3DX_PI || m_rotDest.y - m_rot.y < -D3DX_PI)
	{// 差分が1周以上ある場合
		m_rot.y -= (m_rotDest.y - m_rot.y) * 0.1f;
	}
	else
	{
		// 目標方向へ向く
		m_rot.y += (m_rotDest.y - m_rot.y) * 0.1f;
	}

	// 位置の更新
	m_pos.x += m_move.x;
	m_pos.z += m_move.z;

	//CDebugProc::Print("m_rot.y : %.1f ", m_rot.y);
	//CDebugProc::Print("m_rotDest.y : %.1f ", m_rotDest.y);
}

//=============================================================================
//	追従処理
//=============================================================================
void CAIMecha::Follow()
{
	float fMinLength = 100000, fLength = 100000;	// 差分系
	int nNearNode = 0;

	// 自分の位置に最も近いノードを検索する
	for (int nCntNode = 0; nCntNode < m_pNodeData->GetLoadData().nodeMax; nCntNode++)
	{// ノードの数だけ回る
	 // 差分を求める
		if (m_pPlayer != NULL)
		{// プレイヤーのNULLチェック
			fLength =
				(m_pNodeData->GetLoadData().pos[nCntNode].x - m_pos.x) *
				(m_pNodeData->GetLoadData().pos[nCntNode].x - m_pos.x) +
				(m_pNodeData->GetLoadData().pos[nCntNode].z - m_pos.z) *
				(m_pNodeData->GetLoadData().pos[nCntNode].z - m_pos.z);

			if (fMinLength > fLength)
			{// 差分の最小値を求める
				fMinLength = fLength;
				nNearNode = nCntNode;
			}
		}
	}

	// 現在地を開始ノードに設定する
	m_nStartNode = nNearNode;

	fMinLength = 100000;
	fLength = 100000;
	nNearNode = 0;

	// 自分の主人の位置に最も近いノードを検索する
	for (int nCntNode = 0; nCntNode < m_pNodeData->GetLoadData().nodeMax; nCntNode++)
	{// ノードの数だけ回る
	 // 差分を求める
		if (m_pPlayer != NULL)
		{// プレイヤーのNULLチェック
			fLength =
				(m_pNodeData->GetLoadData().pos[nCntNode].x - m_pPlayer->GetPos().x) *
				(m_pNodeData->GetLoadData().pos[nCntNode].x - m_pPlayer->GetPos().x) +
				(m_pNodeData->GetLoadData().pos[nCntNode].z - m_pPlayer->GetPos().z) *
				(m_pNodeData->GetLoadData().pos[nCntNode].z - m_pPlayer->GetPos().z);

			if (fMinLength > fLength)
			{// 差分の最小値を求める
				fMinLength = fLength;
				nNearNode = nCntNode;
			}
		}
	}

	// 前回の情報を保存
	m_nNodeOld = m_nEndNode;

	// 新規の目的地を設定する
	m_nEndNode = nNearNode;

	if (m_nStartNode == m_nEndNode)
	{//	主人のもとへ辿り着いた場合
		m_nPoint = 0;
		m_nCountPoint = -1;

		for (int nCntAll = 0; nCntAll < m_pNodeData->GetLoadData().nodeMax; nCntAll++)
		{// ノードの数だけ回る
			m_waypoint[nCntAll] = m_pNodeData->GetLoadData().pos[nNearNode];
		}
	}

	if (m_nNodeOld != m_nEndNode)
	{// 主人が前回のノードから移動している場合
		m_nPoint = 0;
		m_nCountPoint = -1;

		// 今自分が向かってるノードを検索する
		for (int nCntNode = 0; nCntNode < m_pNodeData->GetLoadData().nodeMax; nCntNode++)
		{// ノードの数だけ回る
			fLength =
				(m_pNodeData->GetLoadData().pos[nCntNode].x - m_posDest.x) *
				(m_pNodeData->GetLoadData().pos[nCntNode].x - m_posDest.x) +
				(m_pNodeData->GetLoadData().pos[nCntNode].z - m_posDest.z) *
				(m_pNodeData->GetLoadData().pos[nCntNode].z - m_posDest.z);

			if (fMinLength > fLength)
			{// 差分の最小値を求める
				fMinLength = fLength;
				nNearNode = nCntNode;
			}
		}

		// 今自分が向かってるノードをスタート地点にする
		m_nStartNode = nNearNode;

		for (int nCntAll = 0; nCntAll < m_pNodeData->GetLoadData().nodeMax; nCntAll++)
		{// ノードの数だけ回る
			m_waypoint[nCntAll] = m_pNodeData->GetLoadData().pos[m_nStartNode];
		}

		// ポイントへの経路探索
		CAIMecha::RootSearch();
	}
}

//=============================================================================
//	ノード検索処理
//=============================================================================
void CAIMecha::NodeSearch(bool node)
{
	float fMinLength = 100000, fLength = 100000;	// 差分系
	int nNearNode = 0;

	if (node)
	{// 開始ノードの設定
		fMinLength = 100000;
		fLength = 100000;
		nNearNode = 0;

		// 自分の位置に最も近いノードを検索する
		for (int nCntNode = 0; nCntNode < m_pNodeData->GetLoadData().nodeMax; nCntNode++)
		{// ノードの数だけ回る
		 // 差分を求める
			if (m_pPlayer != NULL)
			{// プレイヤーのNULLチェック
				fLength =
					(m_pNodeData->GetLoadData().pos[nCntNode].x - m_pos.x) *
					(m_pNodeData->GetLoadData().pos[nCntNode].x - m_pos.x) +
					(m_pNodeData->GetLoadData().pos[nCntNode].z - m_pos.z) *
					(m_pNodeData->GetLoadData().pos[nCntNode].z - m_pos.z);

				if (fMinLength > fLength)
				{// 差分の最小値を求める
					fMinLength = fLength;
					nNearNode = nCntNode;
				}
			}
		}

		// 現在地を開始ノードに設定する
		m_nStartNode = nNearNode;
		m_bGoal = false;
	}
	else if (!node)
	{// 終了ノードの設定
		fMinLength = 100000;
		fLength = 100000;
		nNearNode = 0;

		// 指定された位置に最も近いノードを検索する
		for (int nCntNode = 0; nCntNode < m_pNodeData->GetLoadData().nodeMax; nCntNode++)
		{// ノードの数だけ回る
		 // 差分を求める
			fLength =
				(m_pNodeData->GetLoadData().pos[nCntNode].x - CManager::GetGame()->GetMouse()->Getsetpos().x) *
				(m_pNodeData->GetLoadData().pos[nCntNode].x - CManager::GetGame()->GetMouse()->Getsetpos().x) +
				(m_pNodeData->GetLoadData().pos[nCntNode].z - CManager::GetGame()->GetMouse()->Getsetpos().z) *
				(m_pNodeData->GetLoadData().pos[nCntNode].z - CManager::GetGame()->GetMouse()->Getsetpos().z);

			if (fMinLength > fLength)
			{// 差分の最小値を求める
				fMinLength = fLength;
				nNearNode = nCntNode;
			}
		}

		// 前回の情報を保存
		m_nNodeOld = m_nEndNode;

		// 検索した位置を終了ノードに設定する
		m_nEndNode = nNearNode;
		m_nRallyEndNode[m_nRallyCount] = nNearNode;

		if (m_AIAction[2] == AI_ACTION_ROUND_TRIP)
		{// 往復移動状態の場合
			if (m_nRallyCount <= 1)
			{// 1クリック目
				m_nPatrolStartNode = nNearNode;
			}
			m_nRallyEndNode[0] = m_nPatrolStartNode;
			m_nEndNode = m_nPatrolStartNode;
		}
	}
}

//=============================================================================
//	最短経路検索処理
//=============================================================================
void CAIMecha::RootSearch()
{
	Node node[NODEPOINT_MAX];		// ノードの情報
	float weight[NODEPOINT_MAX];	// 各エッジのコスト
	int nCntWeight = 0;				// コストのカウンタ
	std::vector<int> path;			// 最短経路の情報を保持するvector

									//======= エッジコストの算出 =========================================================================
	for (int nCntNode = 0; nCntNode < m_pNodeData->GetLoadData().nodeMax; nCntNode++, nCntWeight++)
	{// ノードの数だけ回る
		weight[nCntWeight] = sqrt(
			(m_pNodeData->GetLoadData().pos[m_nStartNode].x - m_pNodeData->GetLoadData().pos[nCntNode].x) *
			(m_pNodeData->GetLoadData().pos[m_nStartNode].x - m_pNodeData->GetLoadData().pos[nCntNode].x) +
			(m_pNodeData->GetLoadData().pos[m_nStartNode].z - m_pNodeData->GetLoadData().pos[nCntNode].z) *
			(m_pNodeData->GetLoadData().pos[m_nStartNode].z - m_pNodeData->GetLoadData().pos[nCntNode].z));
	}

	//======= エッジ追加 =========================================================================
	for (int nCntNode = 0; nCntNode < m_pNodeData->GetLoadData().nodeMax; nCntNode++)
	{// ノードの数だけ回る
		for (int nCntConnect = 0; nCntConnect < m_pNodeData->GetLoadData().connectNum[nCntNode]; nCntConnect++)
		{// 繋がってるノードの数だけ回る
			CAIMecha::AddEdge(nCntNode, m_pNodeData->GetLoadData().connectIndex[nCntNode][nCntConnect], weight[nCntNode], node);
		}
	}

	//======= 最短経路を調べる =========================================================================
	CAIMecha::Dijkstra(m_pNodeData->GetLoadData().nodeMax, m_nStartNode, m_nEndNode, node);

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
			m_nCountPoint = nCntNode;
		}
	}

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
//	ラリー経路検索処理
//=============================================================================
void CAIMecha::RallyRootSearch()
{
	float weight[NODEPOINT_MAX];	// 各エッジのコスト
	int nCntWeight = 0;		// コストのカウンタ
	std::vector<int> path;	// 最短経路の情報を保持するvector

							//======= エッジコストの算出 =========================================================================
	for (int nCntNode = 0; nCntNode < m_pNodeData->GetLoadData().nodeMax; nCntNode++, nCntWeight++)
	{// ノードの数だけ回る
		if (m_nRallyCount != 0)
		{// 2ポイント目以降
			weight[nCntWeight] = sqrt(
				(m_pNodeData->GetLoadData().pos[m_nRallyEndNode[m_nRallyCount - 1]].x - m_pNodeData->GetLoadData().pos[nCntNode].x) *
				(m_pNodeData->GetLoadData().pos[m_nRallyEndNode[m_nRallyCount - 1]].x - m_pNodeData->GetLoadData().pos[nCntNode].x) +
				(m_pNodeData->GetLoadData().pos[m_nRallyEndNode[m_nRallyCount - 1]].z - m_pNodeData->GetLoadData().pos[nCntNode].z) *
				(m_pNodeData->GetLoadData().pos[m_nRallyEndNode[m_nRallyCount - 1]].z - m_pNodeData->GetLoadData().pos[nCntNode].z));
		}
		else
		{// 1ポイント目
			weight[nCntWeight] = sqrt(
				(m_pNodeData->GetLoadData().pos[m_nRallyEndNode[m_nRallyCount]].x - m_pNodeData->GetLoadData().pos[nCntNode].x) *
				(m_pNodeData->GetLoadData().pos[m_nRallyEndNode[m_nRallyCount]].x - m_pNodeData->GetLoadData().pos[nCntNode].x) +
				(m_pNodeData->GetLoadData().pos[m_nRallyEndNode[m_nRallyCount]].z - m_pNodeData->GetLoadData().pos[nCntNode].z) *
				(m_pNodeData->GetLoadData().pos[m_nRallyEndNode[m_nRallyCount]].z - m_pNodeData->GetLoadData().pos[nCntNode].z));
		}
	}

	//======= エッジ追加 =========================================================================
	for (int nCntNode = 0; nCntNode < m_pNodeData->GetLoadData().nodeMax; nCntNode++)
	{// ノードの数だけ回る
		for (int nCntConnect = 0; nCntConnect < m_pNodeData->GetLoadData().connectNum[nCntNode]; nCntConnect++)
		{// 繋がってるノードの数だけ回る
			CAIMecha::AddEdge(nCntNode, m_pNodeData->GetLoadData().connectIndex[nCntNode][nCntConnect], weight[nCntNode], m_node[m_nRallyCount]);
		}
	}

	//======= 最短経路を調べる =========================================================================
	if (m_nRallyCount > 1)
	{// 2ポイント目以降
		for (int nCntRally = 1; nCntRally < m_nRallyCount; nCntRally++)
		{// 2ポイント以降の数だけ回る
		 // ダイクストラ法で最短経路を求める
			CAIMecha::Dijkstra(m_pNodeData->GetLoadData().nodeMax, m_nRallyEndNode[m_nRallyCount - nCntRally], m_nRallyEndNode[m_nRallyCount - (nCntRally - 1)], m_node[m_nRallyCount - (nCntRally - 1)]);

			for (int nCntNode = m_nRallyEndNode[m_nRallyCount - (nCntRally - 1)]; nCntNode != m_nRallyEndNode[m_nRallyCount - nCntRally]; nCntNode = m_node[m_nRallyCount - (nCntRally - 1)][nCntNode].from)
			{// 最短経路をゴールから順にスタートまでたどる
				path.push_back(nCntNode);
			}
			path.push_back(m_nRallyEndNode[m_nRallyCount - nCntRally]);
		}
	}

	if (m_nRallyCount > 0)
	{// 1ポイント目
	 // ダイクストラ法で最短経路を求める
		CAIMecha::Dijkstra(m_pNodeData->GetLoadData().nodeMax, m_nStartNode, m_nRallyEndNode[m_nRallyCount - (m_nRallyCount - 1)], m_node[m_nRallyCount - (m_nRallyCount - 1)]);

		for (int nCntNode = m_nRallyEndNode[m_nRallyCount - (m_nRallyCount - 1)]; nCntNode != m_nStartNode; nCntNode = m_node[m_nRallyCount - (m_nRallyCount - 1)][nCntNode].from)
		{// 最短経路をゴールから順にスタートまでたどる
			path.push_back(nCntNode);
		}
		path.push_back(m_nStartNode);
	}

	if (m_nRallyCount == 0)
	{// スタート位置
	 //ダイクストラ法で最短経路を求める
		CAIMecha::Dijkstra(m_pNodeData->GetLoadData().nodeMax, m_nStartNode, m_nRallyEndNode[m_nRallyCount], m_node[m_nRallyCount]);

		for (int nCntNode = m_nRallyEndNode[m_nRallyCount]; nCntNode != m_nStartNode; nCntNode = m_node[m_nRallyCount][nCntNode].from)
		{// 最短経路をゴールから順にスタートまでたどる
			path.push_back(nCntNode);
		}
		path.push_back(m_nStartNode);
	}

	//======= 最短経路の出力 =========================================================================
	for (int nCntNode = path.size() - 1; nCntNode >= 0; nCntNode--)
	{
		if (m_nCountPoint < nCntNode)
		{// 最大値を代入
			m_nCountPoint = nCntNode;
		}
	}

	//======= 目標地点の設定 =========================================================================
	for (int nCntNodeMax = 0; nCntNodeMax < m_pNodeData->GetLoadData().nodeMax; )
	{// ノードの数だけ回る
		for (int nCntNode = path.size() - 1; nCntNode >= 0; nCntNode--, nCntNodeMax++)
		{// ゴールから辿る
			m_waypoint[nCntNodeMax] = m_pNodeData->GetLoadData().pos[path[nCntNode]];
		}
	}
}

//=============================================================================
//	往復移動経路検索処理
//=============================================================================
void CAIMecha::PatrolRootSearch()
{
	float weight[NODEPOINT_MAX];	// 各エッジのコスト
	int nCntWeight = 0;		// コストのカウンタ
	std::vector<int> path;	// 最短経路の情報を保持するvector

							//======= エッジコストの算出 =========================================================================
	for (int nCntNode = 0; nCntNode < m_pNodeData->GetLoadData().nodeMax; nCntNode++, nCntWeight++)
	{// ノードの数だけ回る
		if (m_nRallyCount != 0)
		{// 2ポイント目以降
			weight[nCntWeight] = sqrt(
				(m_pNodeData->GetLoadData().pos[m_nRallyEndNode[m_nRallyCount - 1]].x - m_pNodeData->GetLoadData().pos[nCntNode].x) *
				(m_pNodeData->GetLoadData().pos[m_nRallyEndNode[m_nRallyCount - 1]].x - m_pNodeData->GetLoadData().pos[nCntNode].x) +
				(m_pNodeData->GetLoadData().pos[m_nRallyEndNode[m_nRallyCount - 1]].z - m_pNodeData->GetLoadData().pos[nCntNode].z) *
				(m_pNodeData->GetLoadData().pos[m_nRallyEndNode[m_nRallyCount - 1]].z - m_pNodeData->GetLoadData().pos[nCntNode].z));
		}
		else
		{// 1ポイント目
			weight[nCntWeight] = sqrt(
				(m_pNodeData->GetLoadData().pos[m_nRallyEndNode[m_nRallyCount]].x - m_pNodeData->GetLoadData().pos[nCntNode].x) *
				(m_pNodeData->GetLoadData().pos[m_nRallyEndNode[m_nRallyCount]].x - m_pNodeData->GetLoadData().pos[nCntNode].x) +
				(m_pNodeData->GetLoadData().pos[m_nRallyEndNode[m_nRallyCount]].z - m_pNodeData->GetLoadData().pos[nCntNode].z) *
				(m_pNodeData->GetLoadData().pos[m_nRallyEndNode[m_nRallyCount]].z - m_pNodeData->GetLoadData().pos[nCntNode].z));
		}

	}

	//======= エッジ追加 =========================================================================
	for (int nCntNode = 0; nCntNode < m_pNodeData->GetLoadData().nodeMax; nCntNode++)
	{// ノードの数だけ回る
		for (int nCntConnect = 0; nCntConnect < m_pNodeData->GetLoadData().connectNum[nCntNode]; nCntConnect++)
		{// 繋がってるノードの数だけ回る
			CAIMecha::AddEdge(nCntNode, m_pNodeData->GetLoadData().connectIndex[nCntNode][nCntConnect], weight[nCntNode], m_node[m_nRallyCount]);
		}
	}

	//======= 逆順最短経路を調べる =========================================================================
	if (m_nRallyCount == 0)
	{// スタート位置
	 //ダイクストラ法で最短経路を求める
		CAIMecha::Dijkstra(m_pNodeData->GetLoadData().nodeMax, m_nRallyEndNode[m_nRallyCount], m_nPatrolStartNode, m_node[m_nRallyCount]);

		for (int nCntNode = m_nPatrolStartNode; nCntNode != m_nRallyEndNode[m_nRallyCount]; nCntNode = m_node[m_nRallyCount][nCntNode].from)
		{// 最短経路をスタートから順にゴールまでたどる
			path.push_back(nCntNode);
		}
		path.push_back(m_nRallyEndNode[m_nRallyCount]);
	}

	if (m_nRallyCount > 0)
	{// 1ポイント目
	 // ダイクストラ法で最短経路を求める
		CAIMecha::Dijkstra(m_pNodeData->GetLoadData().nodeMax, m_nRallyEndNode[m_nRallyCount - (m_nRallyCount - 1)], m_nPatrolStartNode, m_node[m_nRallyCount - (m_nRallyCount - 1)]);

		for (int nCntNode = m_nPatrolStartNode; nCntNode != m_nRallyEndNode[m_nRallyCount - (m_nRallyCount - 1)]; nCntNode = m_node[m_nRallyCount - (m_nRallyCount - 1)][nCntNode].from)
		{// 最短経路をスタートから順にゴールまでたどる
			path.push_back(nCntNode);
		}
		path.push_back(m_nRallyEndNode[m_nRallyCount - (m_nRallyCount - 1)]);
	}

	if (m_nRallyCount > 1)
	{// 2ポイント目以降
		for (int nCntRally = m_nRallyCount - 1; nCntRally >= 1; nCntRally--)
		{// 2ポイント以降の数だけ回る
		 // ダイクストラ法で最短経路を求める
			CAIMecha::Dijkstra(m_pNodeData->GetLoadData().nodeMax, m_nRallyEndNode[m_nRallyCount - (nCntRally - 1)], m_nRallyEndNode[m_nRallyCount - nCntRally], m_node[m_nRallyCount - (nCntRally - 1)]);

			for (int nCntNode = m_nRallyEndNode[m_nRallyCount - nCntRally]; nCntNode != m_nRallyEndNode[m_nRallyCount - (nCntRally - 1)]; nCntNode = m_node[m_nRallyCount - (nCntRally - 1)][nCntNode].from)
			{// 最短経路をスタートから順にゴールまでたどる
				path.push_back(nCntNode);
			}
			path.push_back(m_nRallyEndNode[m_nRallyCount - (nCntRally - 1)]);
		}
	}

	//======= 正順最短経路を調べる =========================================================================
	if (m_nRallyCount > 1)
	{// 2ポイント目以降
		for (int nCntRally = 1; nCntRally < m_nRallyCount; nCntRally++)
		{// 2ポイント以降の数だけ回る
		 // ダイクストラ法で最短経路を求める
			CAIMecha::Dijkstra(m_pNodeData->GetLoadData().nodeMax, m_nRallyEndNode[m_nRallyCount - nCntRally], m_nRallyEndNode[m_nRallyCount - (nCntRally - 1)], m_node[m_nRallyCount - (nCntRally - 1)]);

			for (int nCntNode = m_nRallyEndNode[m_nRallyCount - (nCntRally - 1)]; nCntNode != m_nRallyEndNode[m_nRallyCount - nCntRally]; nCntNode = m_node[m_nRallyCount - (nCntRally - 1)][nCntNode].from)
			{// 最短経路をゴールから順にスタートまでたどる
				path.push_back(nCntNode);
			}
			path.push_back(m_nRallyEndNode[m_nRallyCount - nCntRally]);
		}
	}

	if (m_nRallyCount > 0)
	{// 1ポイント目
	 // ダイクストラ法で最短経路を求める
		CAIMecha::Dijkstra(m_pNodeData->GetLoadData().nodeMax, m_nPatrolStartNode, m_nRallyEndNode[m_nRallyCount - (m_nRallyCount - 1)], m_node[m_nRallyCount - (m_nRallyCount - 1)]);

		for (int nCntNode = m_nRallyEndNode[m_nRallyCount - (m_nRallyCount - 1)]; nCntNode != m_nPatrolStartNode; nCntNode = m_node[m_nRallyCount - (m_nRallyCount - 1)][nCntNode].from)
		{// 最短経路をゴールから順にスタートまでたどる
			path.push_back(nCntNode);
		}
		path.push_back(m_nPatrolStartNode);
	}

	if (m_nRallyCount == 0)
	{// スタート位置
	 //ダイクストラ法で最短経路を求める
		CAIMecha::Dijkstra(m_pNodeData->GetLoadData().nodeMax, m_nPatrolStartNode, m_nRallyEndNode[m_nRallyCount], m_node[m_nRallyCount]);

		for (int nCntNode = m_nRallyEndNode[m_nRallyCount]; nCntNode != m_nPatrolStartNode; nCntNode = m_node[m_nRallyCount][nCntNode].from)
		{// 最短経路をゴールから順にスタートまでたどる
			path.push_back(nCntNode);
		}
		path.push_back(m_nPatrolStartNode);
	}

	//======= 最短経路の出力 =========================================================================
	for (int nCntNode = path.size() - 1; nCntNode >= 0; nCntNode--)
	{
		if (m_nCountPatrolPoint < nCntNode)
		{// 最大値を代入
			m_nCountPatrolPoint = nCntNode;
		}
	}

	//======= 目標地点の設定 =========================================================================
	for (int nCntNodeMax = 0; nCntNodeMax < m_pNodeData->GetLoadData().nodeMax; )
	{// ノードの数だけ回る
		for (int nCntNode = path.size() - 1; nCntNode >= 0; nCntNode--, nCntNodeMax++)
		{// ゴールから辿る
			m_patrolWaypoint[nCntNodeMax] = m_pNodeData->GetLoadData().pos[path[nCntNode]];
		}
	}
}

//=============================================================================
//	中断処理
//=============================================================================
void CAIMecha::Cancel()
{
	if (m_bPartSwitchOld == CGame::PART_ACTION && m_bPartSwitch == CGame::PART_STRATEGY)
	{// ストラテジーへ変更した場合
		int nNear = 0;
		float fMinLength = 100000, fLength = 100000;
		for (int nCntAll = 0; nCntAll < m_pNodeData->GetLoadData().nodeMax; nCntAll++)
		{// 全てのノードとプレイヤーとの差分を出す
			fLength =
				(m_pos.x - m_pNodeData->GetLoadData().pos[nCntAll].x) *
				(m_pos.x - m_pNodeData->GetLoadData().pos[nCntAll].x) +
				(m_pos.z - m_pNodeData->GetLoadData().pos[nCntAll].z) *
				(m_pos.z - m_pNodeData->GetLoadData().pos[nCntAll].z);

			if (fMinLength > fLength)
			{// 差分の最小値を求める
				fMinLength = fLength;
				nNear = nCntAll;
			}
		}

		// 一番近いノードで停止してデータを初期化
		m_bPatrol = false;
		m_bGoal = false;
		m_nRallyCount = 0;
		m_nPoint = 0;
		m_nCountPoint = -1;
		m_nCountPatrolPoint = -1;
		m_nStartNode = nNear;
		m_nEndNode = nNear;
		m_nPatrolStartNode = nNear;

		for (int nCntRally = 0; nCntRally < RALLYPOINT_MAX; nCntRally++)
		{// ラリーポイントの最大数分回る
			for (int nCntNode = 0; nCntNode < NODEPOINT_MAX; nCntNode++)
			{// ノードの最大数分回る
				m_node[nCntRally][nCntNode] = {};
			}
		}

		for (int nCntAll = 0; nCntAll < m_pNodeData->GetLoadData().nodeMax; nCntAll++)
		{// ノードの数だけ回る
			m_nRallyEndNode[nCntAll] = nNear;
			m_waypoint[nCntAll] = m_pNodeData->GetLoadData().pos[nNear];
			m_patrolWaypoint[nCntAll] = m_pNodeData->GetLoadData().pos[nNear];
		}

		for (int nCntAction = 0; nCntAction < 4; nCntAction++)
		{// 行動数の分回る
			m_AIAction[nCntAction] = AI_ACTION_NONE;
			m_LogicTree[nCntAction] = -1;
		}
		m_posDest = m_pNodeData->GetLoadData().pos[nNear];
	}
}

//=============================================================================
// エッジ追加処理
//=============================================================================
void CAIMecha::AddEdge(int first, int second, float weight, Node *node)
{
	// ノードuはノードvとつながっている情報を入れる
	node[second].to.push_back(first);
	// ノードuとノードvのエッジの重みを入れる
	node[second].cost.push_back(weight);
}

//=============================================================================
// ダイクストラ法
//=============================================================================
void CAIMecha::Dijkstra(int nodeMax, int start, int end, Node *node)
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