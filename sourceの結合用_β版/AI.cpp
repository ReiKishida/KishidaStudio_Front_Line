//=============================================================================
//
// AI処理 [AI.cpp]
// Author : Takuto Ishida
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "AI.h"
#include "player.h"
#include "manager.h"
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

//==================================
// マクロ定義
//==================================
#define WALKER_FILE	"data/TEXT/AI/walker/model_walker.txt"
#define DRONE_FILE	"data/TEXT/AI/drone/model_drone.txt"

// ルート探索用
#define	LOAD_FILENAME		("data/TEXT/NODE_DATA/NodeData.txt")
#define ENEMY_BREAKTIME		(1)			// 休憩時間(フレーム)
#define MOUSE_ACCEPTABLE	(20.0f)		// マウスの誤差の許容範囲
#define MOVE_ACCEPTABLE		(25.0f)		// 移動の誤差の許容範囲
#define POS_ACCEPTABLE		(100.0f)	// 位置の誤差の許容範囲
#define AI_SPEED			(3.0f)		// 移動速度

//==================================
// 静的メンバ変数宣言
//==================================

//==================================
// 生成処理
//==================================
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

//=========================================
// コンストラクタ
//=========================================
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
}

//=========================================
// デストラクタ
//=========================================
CAIMecha::~CAIMecha() {}

//=========================================
// 初期化処理
//=========================================
HRESULT CAIMecha::Init(void)
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

	// 数値の初期化==============================================================================
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
	}

	for (int nCntAction = 0; nCntAction < 4; nCntAction++)
	{// 行動数の分回る
		m_AIAction[nCntAction] = AI_ACTION_NONE;
		m_LogicTree[nCntAction] = -1;
	}

	// マップデータファイルの読み込み
	CAIMecha::FileLoad(LOAD_FILENAME);

	// 開始時点のノードの初期化
	float fMinLength = 100000, fLength = 100000;	// 差分系
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
	m_nEndNode = m_nStartNode;

	// ポイントへの経路探索
	CAIMecha::RootSearch();

	return S_OK;
}

//=========================================
// 終了処理
//=========================================
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

//=========================================
// 更新処理
//=========================================
void CAIMecha::Update(void)
{
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

	// AI関係の更新処理
	CAIMecha::AIUpdate();
}

//=========================================
// 描画処理
//=========================================
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
void CAIMecha::Damage(int nDamage)
{
	if (m_nLife > 0)
	{
		m_state = STATE_DAMAGE;								// ダメージを受けている状態にする

		m_nLife -= nDamage;

		if (0 >= m_nLife)
		{
			m_nLife = 0;

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
		//CSound *pSound = CManager::GetSound();				// サウンドの取得
		//pSound->PlaySound(CSound::SOUND_LABEL_DAMAGE);		// ダメージ音を再生
	}
}

//=============================================================================
//	AI更新処理
//=============================================================================
void CAIMecha::AIUpdate()
{
	// パート情報	ストラテジー：true	アクション：false
	//CDebugProc::Print("========AI========\n");
	//CDebugProc::Print("現在のパート : %s\n", m_bPartSwitch ? "ストラテジー" : "アクション");
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
	//{// 往復時
	//	CDebugProc::Print("往復状態\n");
	//	CDebugProc::Print("開始地点 : %d\n", m_nRallyEndNode[0]);
	//	for (int nCntRally = 1; nCntRally < m_nRallyCount; nCntRally++)
	//	{
	//		CDebugProc::Print("中間地点[%d] : %d\n", nCntRally, m_nRallyEndNode[nCntRally]);
	//	}
	//	CDebugProc::Print("終了地点 : %d\n", m_nRallyEndNode[m_nRallyCount]);
	//	CDebugProc::Print("\n");
	//}
	//else if (m_AIAction[1] = AI_ACTION_FOLLOW)
	//{// 追従時
	//	CDebugProc::Print("追従状態\n");
	//	CDebugProc::Print("開始地点 : %d\n", m_nStartNode);
	//	CDebugProc::Print("終了地点 : %d\n", m_nEndNode);
	//	CDebugProc::Print("\n");
	//}

	//CDebugProc::Print("現在の移動回数 : %d\n", m_nPoint);
	//CDebugProc::Print("目標までの移動回数 : %d\n", m_nCountPoint);
	//CDebugProc::Print("\n");

	CInputMouse *pMouse = CManager::GetInputMouse();	// マウスの入力を取得
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();	// キーボードの入力を取得

	// 前回のパート情報の保存
	m_bPartSwitchOld = m_bPartSwitch;
	// 現在のパート情報を取得
	m_bPartSwitch = CManager::GetGame()->GetPart();

	// 中断処理
	Cancel();

	if (m_bPartSwitch == CGame::PART_STRATEGY && CManager::GetGame()->GetButtonManager() != NULL)
	{// ストラテジーパート時でボタンマネージャーがNULLじゃない場合
		if (CManager::GetGame()->GetButtonManager()->GetSelectFinish() == true)
		{// AIの行動が決定している場合

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
				else if (m_LogicTree[2] == 2)// 往復
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
				if (pMouse->GetTrigger(CInputMouse::DIMS_BUTTON_0) == true)
				{// 左クリックのみ押下
					if (m_nRallyCount < RALLYPOINT_MAX)
					{// カウントが最大数まで到達していない場合
					 // 前回の情報の保存
						m_nRallyCountOld = m_nRallyCount;

						if (m_AIAction[2] == AI_ACTION_ROUND_TRIP || m_AIAction[2] == AI_ACTION_RALLY)
						{// ラリー時または往復時
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
			if (m_AIAction[2] == AI_ACTION_ROUND_TRIP)// 往復時
			{// ポイント間を徘徊する経路探索
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
//	自動移動処理
//=============================================================================
void CAIMecha::AutoMove()
{
	// 目標地点を設定
	m_posDest = m_waypoint[m_nPoint];

	// 目的との差分を出す
	float fLength = (m_pos.x - m_posDest.x) * (m_pos.x - m_posDest.x) + (m_pos.z - m_posDest.z) * (m_pos.z - m_posDest.z);
	m_nBreaktime--;

	if (fLength > MOVE_ACCEPTABLE)
	{// 差分が許容値内に収まるまで目的地に移動する
		m_move.x = sinf(atan2f(m_posDest.x - m_pos.x, m_posDest.z - m_pos.z)) * AI_SPEED;
		m_move.z = cosf(atan2f(m_posDest.x - m_pos.x, m_posDest.z - m_pos.z)) * AI_SPEED;
		m_rotDest.y = atan2f(m_posDest.x - m_pos.x, m_posDest.z - m_pos.z) + D3DX_PI;
	}
	else if (m_nBreaktime < 0)
	{// 移動後休憩
		m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_nBreaktime = ENEMY_BREAKTIME;

		if (m_nCountPoint > 0 && m_nCountPoint == m_nPoint && !m_bGoal)
		{// 終了ノードに到着したとき
			m_bGoal = true;
		}
	}
	else if (m_nBreaktime == 0)
	{// 休憩終了
		if (m_nCountPoint > m_nPoint)
		{// 目標地点に到達していない場合
			m_nPoint++;
		}
	}

	if (m_AIAction[2] == AI_ACTION_ROUND_TRIP && m_nCountPoint <= m_nPoint)
	{// 往復状態で目標地点に到達している場合
		m_nPoint = 0;
	}

	if (m_rotDest.y > D3DX_PI) { m_rotDest.y -= D3DX_PI * 2.0f; }
	if (m_rotDest.y < -D3DX_PI) { m_rotDest.y += D3DX_PI * 2.0f; }

	if (m_rot.y > D3DX_PI) { m_rot.y -= D3DX_PI * 2.0f; }
	if (m_rot.y < -D3DX_PI) { m_rot.y += D3DX_PI * 2.0f; }

	// 目標方向へ向く
	m_rot.y += (m_rotDest.y - m_rot.y) * 0.1f;

	// 位置の更新
	m_pos.x += m_move.x;
	m_pos.z += m_move.z;
}

//=============================================================================
//	追従処理
//=============================================================================
void CAIMecha::Follow()
{
	float fMinLength = 100000, fLength = 100000;	// 差分系
	int nNearNode = 0;

	// 自分の位置に最も近いノードを検索する
	for (int nCntNode = 0; nCntNode < m_NodeData.nodeMax; nCntNode++)
	{// ノードの数だけ回る
	 // 差分を求める
		if (m_pPlayer != NULL)
		{// プレイヤーのNULLチェック
			fLength = (m_NodeData.pos[nCntNode].x - m_pos.x) * (m_NodeData.pos[nCntNode].x - m_pos.x) + (m_NodeData.pos[nCntNode].z - m_pos.z) * (m_NodeData.pos[nCntNode].z - m_pos.z);

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
	for (int nCntNode = 0; nCntNode < m_NodeData.nodeMax; nCntNode++)
	{// ノードの数だけ回る
	 // 差分を求める
		if (m_pPlayer != NULL)
		{// プレイヤーのNULLチェック
			fLength = (m_NodeData.pos[nCntNode].x - m_pPlayer->GetPos().x) * (m_NodeData.pos[nCntNode].x - m_pPlayer->GetPos().x) + (m_NodeData.pos[nCntNode].z - m_pPlayer->GetPos().z) * (m_NodeData.pos[nCntNode].z - m_pPlayer->GetPos().z);

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

		for (int nCntAll = 0; nCntAll < m_NodeData.nodeMax; nCntAll++)
		{// ノードの数だけ回る
			m_waypoint[nCntAll] = m_NodeData.pos[nNearNode];
		}
	}

	if (m_nNodeOld != m_nEndNode)
	{// 主人が前回のノードから移動している場合
		m_nPoint = 0;
		m_nCountPoint = -1;

		// 今自分が向かってるノードを検索する
		for (int nCntNode = 0; nCntNode < m_NodeData.nodeMax; nCntNode++)
		{// ノードの数だけ回る
			fLength = (m_NodeData.pos[nCntNode].x - m_posDest.x) * (m_NodeData.pos[nCntNode].x - m_posDest.x) + (m_NodeData.pos[nCntNode].z - m_posDest.z) * (m_NodeData.pos[nCntNode].z - m_posDest.z);

			if (fMinLength > fLength)
			{// 差分の最小値を求める
				fMinLength = fLength;
				nNearNode = nCntNode;
			}
		}

		// 今自分が向かってるノードをスタート地点にする
		m_nStartNode = nNearNode;

		for (int nCntAll = 0; nCntAll < m_NodeData.nodeMax; nCntAll++)
		{// ノードの数だけ回る
			m_waypoint[nCntAll] = m_NodeData.pos[m_nStartNode];
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
		for (int nCntNode = 0; nCntNode < m_NodeData.nodeMax; nCntNode++)
		{// ノードの数だけ回る
		 // 差分を求める
			if (m_pPlayer != NULL)
			{// プレイヤーのNULLチェック
				fLength = (m_NodeData.pos[nCntNode].x - m_pos.x) * (m_NodeData.pos[nCntNode].x - m_pos.x) + (m_NodeData.pos[nCntNode].z - m_pos.z) * (m_NodeData.pos[nCntNode].z - m_pos.z);

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
		for (int nCntNode = 0; nCntNode < m_NodeData.nodeMax; nCntNode++)
		{// ノードの数だけ回る
		 // 差分を求める
			fLength = (m_NodeData.pos[nCntNode].x - CManager::GetGame()->GetMouse()->Getsetpos().x) * (m_NodeData.pos[nCntNode].x - CManager::GetGame()->GetMouse()->Getsetpos().x) + (m_NodeData.pos[nCntNode].z - CManager::GetGame()->GetMouse()->Getsetpos().z) * (m_NodeData.pos[nCntNode].z - CManager::GetGame()->GetMouse()->Getsetpos().z);

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

		if (m_nRallyCount > 0)
		{// 1回以上クリックしている場合
			m_nRallyEndNode[m_nRallyCount] = nNearNode;
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
	for (int nCntNode = 0; nCntNode < m_NodeData.nodeMax; nCntNode++, nCntWeight++)
	{// ノードの数だけ回る
		weight[nCntWeight] = sqrt((m_NodeData.pos[m_nStartNode].x - m_NodeData.pos[nCntNode].x) * (m_NodeData.pos[m_nStartNode].x - m_NodeData.pos[nCntNode].x) + (m_NodeData.pos[m_nStartNode].z - m_NodeData.pos[nCntNode].z) * (m_NodeData.pos[m_nStartNode].z - m_NodeData.pos[nCntNode].z));
	}

	//======= エッジ追加 =========================================================================
	for (int nCntNode = 0; nCntNode < m_NodeData.nodeMax; nCntNode++)
	{// ノードの数だけ回る
		for (int nCntConnect = 0; nCntConnect < m_NodeData.connectNum[nCntNode]; nCntConnect++)
		{// 繋がってるノードの数だけ回る
			CAIMecha::AddEdge(nCntNode, m_NodeData.connectIndex[nCntNode][nCntConnect], weight[nCntNode], node);
		}
	}

	//======= 最短経路を調べる =========================================================================
	CAIMecha::Dijkstra(m_NodeData.nodeMax, m_nStartNode, m_nEndNode, node);

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
	for (int nCntNodeMax = 0; nCntNodeMax < m_NodeData.nodeMax; )
	{// ノードの数だけ回る
		for (int nCntNode = path.size() - 1; nCntNode >= 0; nCntNode--, nCntNodeMax++)
		{
			m_waypoint[nCntNodeMax] = m_NodeData.pos[path[nCntNode]];
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
	for (int nCntNode = 0; nCntNode < m_NodeData.nodeMax; nCntNode++, nCntWeight++)
	{// ノードの数だけ回る
		if (m_nRallyCount != 0)
		{// 2ポイント目以降
			weight[nCntWeight] = sqrt(
				(m_NodeData.pos[m_nRallyEndNode[m_nRallyCount - 1]].x - m_NodeData.pos[nCntNode].x) *
				(m_NodeData.pos[m_nRallyEndNode[m_nRallyCount - 1]].x - m_NodeData.pos[nCntNode].x) +
				(m_NodeData.pos[m_nRallyEndNode[m_nRallyCount - 1]].z - m_NodeData.pos[nCntNode].z) *
				(m_NodeData.pos[m_nRallyEndNode[m_nRallyCount - 1]].z - m_NodeData.pos[nCntNode].z));
		}
		else
		{// 1ポイント目
			weight[nCntWeight] = sqrt(
				(m_NodeData.pos[m_nRallyEndNode[m_nRallyCount]].x - m_NodeData.pos[nCntNode].x) *
				(m_NodeData.pos[m_nRallyEndNode[m_nRallyCount]].x - m_NodeData.pos[nCntNode].x) +
				(m_NodeData.pos[m_nRallyEndNode[m_nRallyCount]].z - m_NodeData.pos[nCntNode].z) *
				(m_NodeData.pos[m_nRallyEndNode[m_nRallyCount]].z - m_NodeData.pos[nCntNode].z));
		}
	}

	//======= エッジ追加 =========================================================================
	for (int nCntNode = 0; nCntNode < m_NodeData.nodeMax; nCntNode++)
	{// ノードの数だけ回る
		for (int nCntConnect = 0; nCntConnect < m_NodeData.connectNum[nCntNode]; nCntConnect++)
		{// 繋がってるノードの数だけ回る
			CAIMecha::AddEdge(nCntNode, m_NodeData.connectIndex[nCntNode][nCntConnect], weight[nCntNode], m_node[m_nRallyCount]);
		}
	}

	//======= 最短経路を調べる =========================================================================
	if (m_nRallyCount > 1)
	{// 2ポイント目以降
		for (int nCntRally = 1; nCntRally < m_nRallyCount; nCntRally++)
		{// 2ポイント以降の数だけ回る
		 // ダイクストラ法で最短経路を求める
			CAIMecha::Dijkstra(m_NodeData.nodeMax, m_nRallyEndNode[m_nRallyCount - nCntRally], m_nRallyEndNode[m_nRallyCount - (nCntRally - 1)], m_node[m_nRallyCount - (nCntRally - 1)]);

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
		CAIMecha::Dijkstra(m_NodeData.nodeMax, m_nStartNode, m_nRallyEndNode[m_nRallyCount - (m_nRallyCount - 1)], m_node[m_nRallyCount - (m_nRallyCount - 1)]);

		for (int nCntNode = m_nRallyEndNode[m_nRallyCount - (m_nRallyCount - 1)]; nCntNode != m_nStartNode; nCntNode = m_node[m_nRallyCount - (m_nRallyCount - 1)][nCntNode].from)
		{// 最短経路をゴールから順にスタートまでたどる
			path.push_back(nCntNode);
		}
		path.push_back(m_nStartNode);
	}

	if (m_nRallyCount == 0)
	{// スタート位置
	 //ダイクストラ法で最短経路を求める
		CAIMecha::Dijkstra(m_NodeData.nodeMax, m_nStartNode, m_nRallyEndNode[m_nRallyCount], m_node[m_nRallyCount]);

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
	for (int nCntNodeMax = 0; nCntNodeMax < m_NodeData.nodeMax; )
	{// ノードの数だけ回る
		for (int nCntNode = path.size() - 1; nCntNode >= 0; nCntNode--, nCntNodeMax++)
		{// ゴールから辿る
			m_waypoint[nCntNodeMax] = m_NodeData.pos[path[nCntNode]];
		}
	}
}

//=============================================================================
//	往復経路検索処理
//=============================================================================
void CAIMecha::PatrolRootSearch()
{
	float weight[NODEPOINT_MAX];	// 各エッジのコスト
	int nCntWeight = 0;		// コストのカウンタ
	std::vector<int> path;	// 最短経路の情報を保持するvector

							//======= エッジコストの算出 =========================================================================
	for (int nCntNode = 0; nCntNode < m_NodeData.nodeMax; nCntNode++, nCntWeight++)
	{// ノードの数だけ回る
		if (m_nRallyCount != 0)
		{// 2ポイント目以降
			weight[nCntWeight] = sqrt(
				(m_NodeData.pos[m_nRallyEndNode[m_nRallyCount - 1]].x - m_NodeData.pos[nCntNode].x) *
				(m_NodeData.pos[m_nRallyEndNode[m_nRallyCount - 1]].x - m_NodeData.pos[nCntNode].x) +
				(m_NodeData.pos[m_nRallyEndNode[m_nRallyCount - 1]].z - m_NodeData.pos[nCntNode].z) *
				(m_NodeData.pos[m_nRallyEndNode[m_nRallyCount - 1]].z - m_NodeData.pos[nCntNode].z));
		}
		else
		{// 1ポイント目
			weight[nCntWeight] = sqrt(
				(m_NodeData.pos[m_nRallyEndNode[m_nRallyCount]].x - m_NodeData.pos[nCntNode].x) *
				(m_NodeData.pos[m_nRallyEndNode[m_nRallyCount]].x - m_NodeData.pos[nCntNode].x) +
				(m_NodeData.pos[m_nRallyEndNode[m_nRallyCount]].z - m_NodeData.pos[nCntNode].z) *
				(m_NodeData.pos[m_nRallyEndNode[m_nRallyCount]].z - m_NodeData.pos[nCntNode].z));
		}

	}

	//======= エッジ追加 =========================================================================
	for (int nCntNode = 0; nCntNode < m_NodeData.nodeMax; nCntNode++)
	{// ノードの数だけ回る
		for (int nCntConnect = 0; nCntConnect < m_NodeData.connectNum[nCntNode]; nCntConnect++)
		{// 繋がってるノードの数だけ回る
			CAIMecha::AddEdge(nCntNode, m_NodeData.connectIndex[nCntNode][nCntConnect], weight[nCntNode], m_node[m_nRallyCount]);
		}
	}

	//======= 逆順最短経路を調べる =========================================================================
	if (m_nRallyCount == 0)
	{// スタート位置
	 //ダイクストラ法で最短経路を求める
		CAIMecha::Dijkstra(m_NodeData.nodeMax, m_nRallyEndNode[m_nRallyCount], m_nStartNode, m_node[m_nRallyCount]);

		for (int nCntNode = m_nStartNode; nCntNode != m_nRallyEndNode[m_nRallyCount]; nCntNode = m_node[m_nRallyCount][nCntNode].from)
		{// 最短経路をスタートから順にゴールまでたどる
			path.push_back(nCntNode);
		}
		path.push_back(m_nRallyEndNode[m_nRallyCount]);
	}

	if (m_nRallyCount > 0)
	{// 1ポイント目
	 // ダイクストラ法で最短経路を求める
		CAIMecha::Dijkstra(m_NodeData.nodeMax, m_nRallyEndNode[m_nRallyCount - (m_nRallyCount - 1)], m_nStartNode, m_node[m_nRallyCount - (m_nRallyCount - 1)]);

		for (int nCntNode = m_nStartNode; nCntNode != m_nRallyEndNode[m_nRallyCount - (m_nRallyCount - 1)]; nCntNode = m_node[m_nRallyCount - (m_nRallyCount - 1)][nCntNode].from)
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
			CAIMecha::Dijkstra(m_NodeData.nodeMax, m_nRallyEndNode[m_nRallyCount - (nCntRally - 1)], m_nRallyEndNode[m_nRallyCount - nCntRally], m_node[m_nRallyCount - (nCntRally - 1)]);

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
			CAIMecha::Dijkstra(m_NodeData.nodeMax, m_nRallyEndNode[m_nRallyCount - nCntRally], m_nRallyEndNode[m_nRallyCount - (nCntRally - 1)], m_node[m_nRallyCount - (nCntRally - 1)]);

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
		CAIMecha::Dijkstra(m_NodeData.nodeMax, m_nStartNode, m_nRallyEndNode[m_nRallyCount - (m_nRallyCount - 1)], m_node[m_nRallyCount - (m_nRallyCount - 1)]);

		for (int nCntNode = m_nRallyEndNode[m_nRallyCount - (m_nRallyCount - 1)]; nCntNode != m_nStartNode; nCntNode = m_node[m_nRallyCount - (m_nRallyCount - 1)][nCntNode].from)
		{// 最短経路をゴールから順にスタートまでたどる
			path.push_back(nCntNode);
		}
		path.push_back(m_nStartNode);
	}

	if (m_nRallyCount == 0)
	{// スタート位置
	 //ダイクストラ法で最短経路を求める
		CAIMecha::Dijkstra(m_NodeData.nodeMax, m_nStartNode, m_nRallyEndNode[m_nRallyCount], m_node[m_nRallyCount]);

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
	for (int nCntNodeMax = 0; nCntNodeMax < m_NodeData.nodeMax; )
	{// ノードの数だけ回る
		for (int nCntNode = path.size() - 1; nCntNode >= 0; nCntNode--, nCntNodeMax++)
		{// ゴールから辿る
			m_waypoint[nCntNodeMax] = m_NodeData.pos[path[nCntNode]];
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
		for (int nCntAll = 0; nCntAll < m_NodeData.nodeMax; nCntAll++)
		{// 全てのノードとプレイヤーとの差分を出す
			fLength = (m_pos.x - m_NodeData.pos[nCntAll].x) * (m_pos.x - m_NodeData.pos[nCntAll].x) + (m_pos.z - m_NodeData.pos[nCntAll].z) * (m_pos.z - m_NodeData.pos[nCntAll].z);
			if (fMinLength > fLength)
			{// 差分の最小値を求める
				fMinLength = fLength;
				nNear = nCntAll;
			}
		}

		// 一番近いノードで停止してデータを初期化
		m_bGoal = false;
		m_nRallyCount = 0;
		m_nPoint = 0;
		m_nCountPoint = -1;
		m_nStartNode = nNear;
		m_nEndNode = nNear;
		for (int nCntRally = 0; nCntRally < RALLYPOINT_MAX; nCntRally++)
		{// ラリーポイントの最大数分回る
			for (int nCntNode = 0; nCntNode < NODEPOINT_MAX; nCntNode++)
			{// ノードの最大数分回る
				m_node[nCntRally][nCntNode] = {};
			}
		}

		for (int nCntAll = 0; nCntAll < m_NodeData.nodeMax; nCntAll++)
		{// ノードの数だけ回る
			m_nRallyEndNode[nCntAll] = nNear;
			m_waypoint[nCntAll] = m_NodeData.pos[nNear];
		}

		for (int nCntAction = 0; nCntAction < 4; nCntAction++)
		{// 行動数の分回る
			m_AIAction[nCntAction] = AI_ACTION_NONE;
			m_LogicTree[nCntAction] = -1;
		}
		m_posDest = m_NodeData.pos[nNear];
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

//=============================================================================
// ルート探索用ファイルの読み込み
//=============================================================================
void CAIMecha::FileLoad(char* pFileName)
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
	pFile = fopen(LOAD_FILENAME, "r");

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