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
#include "tutorial.h"
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
#include "search.h"
#include "damageDirection.h"
#include "nodeDataFiler.h"
#include "pin.h"
#include "tutorialmenu.h"

//==================================
// マクロ定義
//==================================
#define ASSULT_FILE				"data/TEXT/PLAYER/assult/model_assult.txt"
#define LIGHT_FILE				"data/TEXT/PLAYER/light/model_light.txt"
#define HEAVY_FILE				"data/TEXT/PLAYER/heavy/model_heavy.txt"
#define SNIPE_FILE				"data/TEXT/PLAYER/snipe/model_snipe.txt"

// 戦闘用
#define ASSULT_BATTLE_FILE		"data/TEXT/PLAYER/assult/battle_assult.txt"
#define LIGHT_BATTLE_FILE		"data/TEXT/PLAYER/light/battle_light.txt"
#define HEAVY_BATTLE_FILE		"data/TEXT/PLAYER/heavy/battle_heavy.txt"
#define SNIPE_BATTLE_FILE		"data/TEXT/PLAYER/snipe/battle_snipe.txt"

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
#define RADIOCHAT_MESS_SPEED	(10)		// ラジオチャットメッセージの速さ
#define RADIOCHAT_DISPLAY_TIME	(60 * 2)	// メッセージ表示時間
#define RADIOCHAT_COL			(0.05f)		// 透明度の減算

// ピン
#define PIN_LIFE				(900)		// ピンの表示時間

// =============================================================
// AI関係
// =============================================================
#define	LOAD_FILENAME		("data/TEXT/NODE_DATA/NodeData.txt")	// マップデータを読み込むファイルの名前
//#define	LOAD_FILENAME	("data/TEXT/NODE_DATA/NodeDataTutorial.txt")	// 読み込むファイルのパス
#define MOVE_ACCEPTABLE		(50.0f)		// 移動時の誤差の許容範囲
#define POS_ACCEPTABLE		(30.0f)		// 検索時の誤差の許容範囲
#define MOUSE_ACCEPTABLE	(20.0f)		// マウスの誤差の許容範囲
#define DATA_REFERENCE_TIME	(5)			// データの参照を行う間隔(回)
#define PLAYER_BREAKTIME	(1)			// 休憩時間(フレーム)
#define PLAYER_FINALPOINT_BREAKTIME	(180)	// 最終地点の休憩時間(フレーム)

#define MAX_CHAR (254)					//読み取る文字数
#define MAX_SEARCH (4)					//センサー数
#define FIND_FIND_CHARACTER_PRIORITY (4)//探すプレイヤーの優先順位

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
	//m_pShadow = NULL;
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
	m_nTexTimer = 0;
	m_nAllyTimer = 0;
	m_bOption = false;
	m_pUITexOption = NULL;
	m_pUIButtonBack = NULL;
	m_nSelectOption = 2;
	m_pUIButtonOption = NULL;
	m_nRadioChat = 0;
	m_nKillPlayerIdx = 0;
	m_fBulletSpeed = 0;
	m_nCntCPURespawn = 0;
	m_pUINumRespawn = NULL;
	m_bWince = false;

	// ピン関係の変数
	m_pPin = NULL;
	m_pPinBullet = NULL;
	m_pAllyPin = NULL;
	m_bPin = false;
	m_bAllyPin = false;
	m_nPinLife = 0;
	m_nAllyPinLife = 0;
	m_PinPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_AllyPinPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_AllyPinPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_AllyPosPinPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_pAllyPosPin = NULL;
	m_pPlayerIcon = NULL;

	for (int nCntAI = 0; nCntAI < AI_MAX; nCntAI++)
	{// AIの数だけ回る
	 // 自分のAIの初期化
		m_pAIPin[nCntAI] = NULL;
		m_bAIPin[nCntAI] = false;
		m_nAIPinLife[nCntAI] = 0;
		m_AIPinPos[nCntAI] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		// 味方のAIの初期化
		m_pAllyAIPin[nCntAI] = NULL;
		m_bAllyAIPin[nCntAI] = false;
		m_nAllyAIPinLife[nCntAI] = 0;
		m_AllyAIPinPos[nCntAI] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	//AI戦闘系の変数
	m_pSearch = NULL;
	m_fSearchLength = 0.0f;
	m_fSearchAngle = 0.0f;
	m_SearchVec_0 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_SearchVec_1 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_SearchVec_2 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fRange = 0.0f;
	m_fRotDestUpper = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_bFind = false;


	for (int nCnt = 0; nCnt < SERECTRESPAWN_TEX; nCnt++)
	{
		m_pUITexRespawn[nCnt] = NULL;
	}

	for (int nCnt = 0; nCnt < OPTION_SELECT; nCnt++)
	{	// カメラ速度設定（オプション）
		m_pUIButtonSelect[nCnt] = NULL;
	}

	for (int nCnt = 0; nCnt < AI_MAX; nCnt++)
	{
		m_pAI[nCnt] = NULL;
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
	else if (CMechaSelect::MECHATYPE_SNIPE == m_mecha) { pFileName = SNIPE_FILE; }

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
									m_pModel[nCntModelParts] = CModel::Create(this,&m_mtxWorld);
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
							else if (strcmp(aStr, "BULLETSPEED") == 0)
							{// 弾速
								fscanf(pFile, " = %f", &m_fBulletSpeed);
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

	//if (NULL == m_pShadow)
	{// 影の生成
		//m_pShadow = CShadow::Create(m_pos, PLAYER_PRIORITY - 1);
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
	else if (CManager::GetMode() == CManager::MODE_TUTORIAL)
	{
		if (m_bConnect == true)
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

	m_nLife = m_nLifeMax;	// ライフの初期値

	// ラジオチャットメッセージで使用
	m_moveSpeed = RADIOCHAT_MESS_SPEED;

	// ゲーム開始時
	if (CManager::GetMode() == CManager::MODE_GAME) { m_Respawn = RESPAWN_START; }
	else if (CManager::GetMode() == CManager::MODE_TUTORIAL) { m_Respawn = RESPAWN_NONE; }
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
		if (CManager::GetMode() == CManager::MODE_TUTORIAL)
		{
			// カメラの向きの設定
			CManager::GetCamera()->SetRot(D3DXVECTOR3(D3DX_PI * 0.5f, D3DX_PI * 0.5f, 0.0f));

		}
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
	if (CManager::GetMode() == CManager::MODE_GAME)
	{
		if (m_pAI[0] == NULL)
		{// ドローンタイプのAIの生成
			m_pAI[0] = m_pAI[0]->Create(this, CAIMecha::MECHATYPE_DRONE, m_pos + D3DXVECTOR3(0.0f, 70.0f, 0.0f));
		}
		if (m_pAI[1] == NULL)
		{// ウォーカータイプのAIの生成
			m_pAI[1] = m_pAI[1]->Create(this, CAIMecha::MECHATYPE_WORKER, m_pos);
		}
	}
	else if (CManager::GetMode() == CManager::MODE_TUTORIAL)
	{
		if (CTutorialMenu::GetMode() == CTutorialMenu::TYPE_STRATEGY)
		{
			if (m_bConnect == true)
			{
				if (m_pAI[0] == NULL)
				{// ドローンタイプのAIの生成
					m_pAI[0] = m_pAI[0]->Create(this, CAIMecha::MECHATYPE_DRONE, m_pos + D3DXVECTOR3(0.0f, 70.0f, 0.0f));
				}
			}
		}
	}

	// ピンの表示時間の初期化
	m_nPinLife = PIN_LIFE;
	m_nAllyPinLife = PIN_LIFE;
	for (int nCnt = 0; nCnt < AI_MAX; nCnt++)
	{// 味方AIの数だけ回る
		m_nAIPinLife[nCnt] = PIN_LIFE;
		m_nAllyAIPinLife[nCnt] = PIN_LIFE;
	}

	// 移動系AI変数の初期化
	m_pNodeData = CGame::GetNodeFiler();	// ファイル情報の取得
	m_posDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nPoint = 0;
	m_nCountPoint = 0;
	m_nCountCollect = 0;
	m_nBreaktime = 0;
	m_nGoalCount = 0;
	m_nVigilanceCount = 0;
	m_bGoal = false;
	m_bPartSwitch = false;
	m_bCollectSwitch = false;

	for (int nCntCollect = 0; nCntCollect < COLLECTIONDATA_MAX; nCntCollect++)
	{// 収集データの最大値分回る
		m_collectionPos[nCntCollect] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	for (int nCntNode = 0; nCntNode < NODEPOINT_MAX; nCntNode++)
	{// ノードの最大値分回る
		m_waypoint[nCntNode] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	if (CManager::GetMode() == CManager::MODE_GAME)
	{
		if (!m_bConnect)
		{// 人間が接続していない場合
			LoadBattleFile();
			m_fRotDestUpper = D3DXVECTOR3(D3DX_PI * 0.5f, D3DX_PI, 0.0f);

			m_pNodeData->FileLoad(LOAD_FILENAME);

			// 開始時点のノードの初期化
			float fMinLength = 100000, fLength = 100000;	// 差分系

			// 開始時点のノードの初期化
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
			m_nEndNode = m_nMovePoint[rand() % RANDOM_MOVE_POINT];

			// ポイントへの経路探索
			CPlayer::RootSearch();
		}
	}

	if (m_pPlayerIcon == NULL)
	{
		m_pPlayerIcon = CScene3D::Create();
		m_pPlayerIcon->CScene3D::SetPos(D3DXVECTOR3(m_pos.x, m_pos.y + 500.0f, m_pos.z));
		m_pPlayerIcon->CScene3D::SetSize(D3DXVECTOR3(100.0f, 0.0f, 100.0f));
		if (m_nTeam == 0)
		{
			m_pPlayerIcon->BindTexture(CTexture::GetTexture(CTexture::TEXTURE_BLUE_DISP_PLAYER));

		}
		else
		{
			m_pPlayerIcon->BindTexture(CTexture::GetTexture(CTexture::TEXTURE_RED_DISP_PLAYER));

		}
	}

	return S_OK;
}

//=========================================
// 終了処理
//=========================================
void CPlayer::Uninit(void)
{
	if (m_pPin != NULL)
	{// ピンの破棄
		m_pPin->Uninit();
		m_pPin = NULL;
	}

	if (m_pPinBullet != NULL)
	{// ピン立て弾の破棄
		m_pPinBullet->Uninit();
		m_pPinBullet = NULL;
	}

	if (m_pAllyPin != NULL)
	{// 味方ピンの破棄
		m_pAllyPin->Uninit();
		m_pAllyPin = NULL;
	}

	if (m_pAllyPosPin != NULL)
	{// 味方位置ピンの破棄
		m_pAllyPosPin->Uninit();
		m_pAllyPosPin = NULL;
	}

	for (int nCntAI = 0; nCntAI < AI_MAX; nCntAI++)
	{
		if (m_pAIPin[nCntAI] != NULL)
		{// AIピンの破棄
			m_pAIPin[nCntAI]->Uninit();
			m_pAIPin[nCntAI] = NULL;
		}

		if (m_pAllyAIPin[nCntAI] != NULL)
		{// 味方AIピンの破棄
			m_pAllyAIPin[nCntAI]->Uninit();
			m_pAllyAIPin[nCntAI] = NULL;
		}
	}

	if (m_pReticle != NULL)
	{// レティクルがNULLじゃない
		m_pReticle->Uninit();
		m_pReticle = NULL;
	}

	//if (m_pShadow != NULL)
	//{// 影がNULLじゃない
	//	m_pShadow->Uninit();
	//	m_pShadow = NULL;
	//}

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


	if (NULL != m_pSearch)
	{
		for (int nCntSearch = 0; nCntSearch < MAX_SEARCH; nCntSearch++)
		{// モデルの破棄
			if (NULL != m_pSearch[nCntSearch])
			{
				m_pSearch[nCntSearch]->Uninit();
				delete m_pSearch[nCntSearch];
				m_pSearch[nCntSearch] = NULL;
			}
		}

		// メモリの開放
		delete[] m_pSearch;
		m_pSearch = NULL;
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
		if (CManager::GetGame()->GetGameState() == CGame::STATE_STARTING_FADE_IN)
		{
			// カメラの向きの設定
			CManager::GetCamera()->SetRot(D3DXVECTOR3(D3DX_PI * 0.5f, D3DX_PI * -1, 0.0f));
		}

		if (CManager::GetGame()->GetGameState() == CGame::STATE_NORMAL)
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
				}
				// 前回位置の保存
				m_posOld = m_pos;

			}

			if (m_pUpperMotion->GetMotion() == CMotionManager::TYPE_NEUTRAL || m_pUpperMotion->GetMotion() == CMotionManager::TYPE_WALK)
			{
				m_bWince = false;
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

							if (m_bWince == false)
							{
								// 移動の処理
								Movement();
							}
							// 弾を撃つ
							Shoot();

							// ピン関係の更新処理
							CPlayer::PinUpdateMulti();

							if (m_bChat == false)
							{//チャットを使用していない場合
							 //ラジオチャットボタンの生成
								ChatBotton();
							}
							if (m_bChat == true)
							{//チャットを使用している場合
							 //チャットのメッセージ表示処理
								ChatMess(m_bChat);
							}
							if (m_bOption == true)
							{	// オプション設定中
								Option(m_bOption);
							}

							if (m_bAllyChat == true)
							{//仲間のチャットが使用されている場合
							 //仲間のメッセージを表示する処理
								AllyChatMess();
							}

							D3DXVECTOR3 rotCamera = CManager::GetCamera()->GetRot();
							D3DXVECTOR3 posR = CManager::GetCamera()->GetPosR();

							m_pReticle->SetDisp(true);
							m_pReticle->SetPos(D3DXVECTOR3(sinf(rotCamera.y) * PLAYER_RETICLE_LENGTH, cosf(rotCamera.x) * PLAYER_RETICLE_LENGTH, cosf(rotCamera.y) * PLAYER_RETICLE_LENGTH) + D3DXVECTOR3(m_pos.x, 0.0f, m_pos.z));

							for (int nCntModel = 0; nCntModel < m_nNumParts; nCntModel++)
							{// パーツモデルの更新
								m_pModel[nCntModel]->Update();
							}
							//CDebugProc::Print("位置：%.2f %.2f %.2f", m_pos.x, m_pos.y, m_pos.z);
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

						if (!m_bConnect)
						{// コンピュータが操作する場合
							AIUpdate();
						}
					}
				}
			}
			else if (CMenu::GetMode() == CMenu::MODE_SINGLE)
			{
				// 前回位置の保存
				m_posOld = m_pos;

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

							if (m_bWince == false)
							{
								// 移動の処理
								Movement();
							}

							// 弾を撃つ
							Shoot();

							// ピン関係の更新処理
							CPlayer::PinUpdateSingle();

							if (m_bChat == false)
							{	// ラジオチャットしていない
								ChatBotton();
							}

							if (m_bOption == true)
							{	// オプション設定中
								Option(m_bOption);
							}

							if (m_bChat == true)
							{	// チャットしてる
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
							//CDebugProc::Print("位置：%.2f %.2f %.2f", m_pos.x, m_pos.y, m_pos.z);
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
					}
				}
			}

			if (CManager::GetMode() == CManager::MODE_GAME)
			{
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
							Angle();
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
			}
			SetParticle();
		}
		else
		{
			if (m_pReticle != NULL)
			{
				m_pReticle->SetDisp(false);
			}
		}
		m_pPlayerIcon->CScene3D::SetPos(D3DXVECTOR3(m_pos.x, m_pos.y + 150.0f, m_pos.z));
	}
	else if (CManager::GetMode() == CManager::MODE_TUTORIAL)
	{
		// 前回位置の保存
		m_posOld = m_pos;

		if (m_Respawn == RESPAWN_START)
		{	// 戦闘開始 状態
			Respawn(RESPAWN_START);
		}

		if (m_nPlayerIdx == 0)
		{
			m_pReticle->SetDisp(false);

			if (CManager::GetTutorial()->GetPart() == CGame::PART_ACTION)
			{	// アクションパート
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
					CCamera *pCamera = CManager::GetCamera();
					D3DXVECTOR3 CameraRot = pCamera->GetRot();
					D3DXMATRIX mtxHead = m_pModel[1]->GetMtxWorld();
					//CDebugProc::Print("ライフ：%d", m_nLife);
					//CDebugProc::Print("位置：%.2f %.2f %.2f", m_pos.x, m_pos.y, m_pos.z);
					//CDebugProc::Print("カメラ角度：%.2f %.2f %.2f", CameraRot.x, CameraRot.y, CameraRot.z);
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
			}
		}
		else
		{
			CDebugProc::Print("ライフ：%d", m_nLife);
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

	if (CManager::GetMode() == CManager::MODE_GAME)
	{
		if (m_bConnect != true)
		{
			for (int nCntSearch = 0; nCntSearch < MAX_SEARCH; nCntSearch++)
			{// モデルの描画処理
				m_pSearch[nCntSearch]->Draw();
			}
		}
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
	if (pKeyboard->GetTrigger(DIK_7))
	{
		Damage(m_nLifeMax, this);
		CParticle::Create(m_pModel[0]->GetWorldPos(), 4);
		CParticle::Create(m_pModel[0]->GetWorldPos(), 5);

		for (int nCntModel = 0; nCntModel < m_nNumParts; nCntModel++)
		{
			m_pModel[nCntModel]->SetDisp(false);
		}
	}

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

#ifdef _DEBUG

	// ダッシュ
	if (pKeyboard->GetPress(DIK_SPACE))
	{
		m_move += m_move * 1.5f;
	}

#endif // DEBUG

	if (NULL != m_pUpperMotion && NULL != m_pLowerMotion)
	{// モーションクラスが使われている
		if (m_pUpperMotion->GetMotion() != CMotionManager::TYPE_DAMAGE_FRONT && m_pUpperMotion->GetMotion() != CMotionManager::TYPE_DAMAGE_BACK)
		{
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
	CSound *pSound = CManager::GetSound();				// サウンドのポインタ

	if (pMouse->GetPress(CInputMouse::DIMS_BUTTON_0) && m_nRemBullet > 0 && m_bChatBotton == false && m_bReload == false && m_bOption == false)
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
				CBulletPlayer::Create(posCanon, m_pAngle[nCntShoots * 2], m_pAngleV[nCntShoots * 2], m_nAttack, m_nTeam, this,m_fBulletSpeed);
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
				CBulletPlayer::Create(posCanon, m_pAngle[nCntShoots * 2 + 1], m_pAngleV[nCntShoots * 2 + 1], m_nAttack, m_nTeam, this,m_fBulletSpeed);
				CParticle::Create(posCanon, 2);

				switch (m_mecha)
				{
				case CMechaSelect::MECHATYPE_LIGHT:
					pSound->PlaySound(CSound::SOUND_LABEL_MACHINEGUN);
					break;
				case CMechaSelect::MECHATYPE_ASSULT:
					pSound->PlaySound(CSound::SOUND_LABEL_SHOTGUN);
					break;
				case CMechaSelect::MECHATYPE_HEAVY:
					pSound->PlaySound(CSound::SOUND_LABEL_RIFLE);
					break;
				case CMechaSelect::MECHATYPE_SNIPE:
					pSound->PlaySound(CSound::SOUND_LABEL_SNIPE);
					break;
				}

				m_bShoot = true;
			}

			//float rot = D3DX_PI;

			//// 差分の調節
			//if (rot > D3DX_PI) { rot -= D3DX_PI * 2.0f; }
			//if (rot < -D3DX_PI) { rot += D3DX_PI * 2.0f; }

			//	D3DXVECTOR3 pos = D3DXVECTOR3(sinf(rot) * 100.0f, 25.0f, cosf(rot) * 100.0f) + m_pos;
			//	CBulletPlayer::Create(pos, atan2f(m_pos.x - pos.x, m_pos.z - pos.z), D3DX_PI * 0.5f, m_nAttack, (m_nTeam + 1) % 2);
			//	m_bShoot = true;

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

	//D3DXVECTOR3 rot = m_pModel[1]->GetRot();

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


	if (!m_bConnect)
	{
		// 角度を求める
		D3DXVECTOR3 rotCamera = m_fRotDestUpper;
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
// パーティクルを発生させる
//=========================================
void CPlayer::SetParticle(void)
{
	if (m_bDeath) { return; }

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
void CPlayer::Damage(int nDamage, CScene *pScene)
{
	if (CMenu::GetMode() == CMenu::MODE_SINGLE)
	{//シングルプレイの場合
		if (NULL != m_pUpperMotion && NULL != m_pLowerMotion)
		{// モーションクラスが使われている
			if (m_nLife > 0 && m_bDeath == false)
			{//体力が０より大きく且つ死亡していない場合

				if (pScene->GetObjType() == CScene::OBJTYPE_PLAYER)
				{//オブジェクトの種類がプレイヤーの場合
					CPlayer *pPlayer = (CPlayer*)pScene;
					if (pPlayer->GetMechaType() == CMechaSelect::MECHATYPE_HEAVY || pPlayer->GetMechaType() == CMechaSelect::MECHATYPE_SNIPE)
					{
						if (CManager::GetGame()->GetDamageDirection()->GetDamageDirection(CDamageDirection::DIRECTION_FRONT))
						{
							m_pUpperMotion->SetMotion(CMotionManager::TYPE_DAMAGE_FRONT);	// ダメージモーションを再生
							m_pLowerMotion->SetMotion(CMotionManager::TYPE_DAMAGE_FRONT);	// ダメージモーションを再生
							m_bWince = true;
						}
						else if (CManager::GetGame()->GetDamageDirection()->GetDamageDirection(CDamageDirection::DIRECTION_BACK) ||
							CManager::GetGame()->GetDamageDirection()->GetDamageDirection(CDamageDirection::DIRECTION_RIGHT) ||
							CManager::GetGame()->GetDamageDirection()->GetDamageDirection(CDamageDirection::DIRECTION_LEFT))
						{
							m_pUpperMotion->SetMotion(CMotionManager::TYPE_DAMAGE_BACK);	// ダメージモーションを再生
							m_pLowerMotion->SetMotion(CMotionManager::TYPE_DAMAGE_BACK);	// ダメージモーションを再生
							m_bWince = true;
						}
					}
				}

				m_state = STATE_DAMAGE;								// ダメージを受けている状態にする

				m_nLife -= nDamage;									//体力の減算

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
									{//NULLではない場合
										m_nKillPlayerIdx = pPlayer->GetPlayerIdx();								//キルプレイヤーの番号を設置処理
										CManager::GetGame()->SetKillIdx(nCntKill, pPlayer->GetPlayerIdx());		//キルプレイヤーの番号を設置処理
										CManager::GetGame()->SetDeathIdx(nCntKill, m_nPlayerIdx);				//デスプレイヤーの番号を設置処理
										CManager::GetGame()->SetPlayerType(0, CGame::TYPE_PLAYER);				//プレイヤーの種類を設置処理
										CManager::GetGame()->SetPlayerType(1, CGame::TYPE_PLAYER);				//プレイヤーの種類を設置処理
										CManager::GetGame()->SetLog(nCntKill, true);							//ログの設置処理
									}
								}
								else if (pScene->GetObjType() == CScene::OBJTYPE_AI)
								{//オブジェクトの種類がＡＩの場合
									CAIMecha *pAIMecha = (CAIMecha*)pScene;
									if (pAIMecha != NULL)
									{//NULLの場合
										m_nKillPlayerIdx = pAIMecha->GetPlayer()->GetPlayerIdx();							//キルプレイヤーの番号を設置処理
										CManager::GetGame()->SetKillIdx(nCntKill, pAIMecha->GetPlayer()->GetPlayerIdx());	//キルプレイヤーの番号を設置処理
										CManager::GetGame()->SetDeathIdx(nCntKill, m_nPlayerIdx);							//デスプレイヤーの番号を設置処理
										if (pAIMecha->GetMechaType() == CAIMecha::MECHATYPE_DRONE)
										{//オブジェクトの種類がドローンの場合
											CManager::GetGame()->SetPlayerType(0, CGame::TYPE_DROWN);						//プレイヤーの種類を設置処理
											CManager::GetGame()->SetPlayerType(1, CGame::TYPE_PLAYER);						//プレイヤーの種類を設置処理
											CManager::GetGame()->SetLog(nCntKill, true);									//ログの設置処理
										}
										else if (pAIMecha->GetMechaType() == CAIMecha::MECHATYPE_WORKER)
										{//オブジェクトの種類がワーカーの場合
											CManager::GetGame()->SetPlayerType(0, CGame::TYPE_WORKER);						//プレイヤーの種類を設置処理
											CManager::GetGame()->SetPlayerType(1, CGame::TYPE_PLAYER);						//プレイヤーの種類を設置処理
											CManager::GetGame()->SetLog(nCntKill, true);									//ログの設置処理
										}
									}
								}
								break;
							}
						}

						for (int nCntAI = 0; nCntAI < AI_MAX; nCntAI++)
						{
							//AIを死亡状態にする
							m_pAI[nCntAI]->SetDeath(true);

							//パーティクルを生成
							CParticle::Create(m_pAI[nCntAI]->GetModel(0)->GetWorldPos(), 4);
							CParticle::Create(m_pAI[nCntAI]->GetModel(0)->GetWorldPos(), 5);

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
						CManager::GetGame()->SetBlueLinkEnergy(CManager::GetGame()->GetBlueLinkEnergy() - 30);
						break;
					case 1:
						CManager::GetGame()->SetRedLinkEnergy(CManager::GetGame()->GetRedLinkEnergy() - 30);
						break;
					}
				}
			}
		}
	}
	else
	{//マルチプレイの場合
		if (CManager::GetClient() != NULL)
		{//NULLではない場合
			if (CManager::GetClient()->GetPlayerIdx() == m_nPlayerIdx)
			{//プレイヤー番号とクライアント番号が同じ場合
				if (NULL != m_pUpperMotion && NULL != m_pLowerMotion)
				{// モーションクラスが使われている
					if (m_nLife > 0 && m_bDeath == false)
					{//体力が０より大きく且つ死亡していない場合
						if (pScene->GetObjType() == CScene::OBJTYPE_PLAYER)
						{//オブジェクトの種類がプレイヤーの場合
							CPlayer *pPlayer = (CPlayer*)pScene;
							if (pPlayer->GetMechaType() == CMechaSelect::MECHATYPE_HEAVY || pPlayer->GetMechaType() == CMechaSelect::MECHATYPE_SNIPE)
							{
								if (CManager::GetGame()->GetDamageDirection()->GetDamageDirection(CDamageDirection::DIRECTION_FRONT))
								{
									m_pUpperMotion->SetMotion(CMotionManager::TYPE_DAMAGE_FRONT);	// ダメージモーションを再生
									m_pLowerMotion->SetMotion(CMotionManager::TYPE_DAMAGE_FRONT);	// ダメージモーションを再生
									m_bWince = true;
								}
								else if (CManager::GetGame()->GetDamageDirection()->GetDamageDirection(CDamageDirection::DIRECTION_BACK) ||
									CManager::GetGame()->GetDamageDirection()->GetDamageDirection(CDamageDirection::DIRECTION_RIGHT) ||
									CManager::GetGame()->GetDamageDirection()->GetDamageDirection(CDamageDirection::DIRECTION_LEFT))
								{
									m_pUpperMotion->SetMotion(CMotionManager::TYPE_DAMAGE_BACK);	// ダメージモーションを再生
									m_pLowerMotion->SetMotion(CMotionManager::TYPE_DAMAGE_BACK);	// ダメージモーションを再生
									m_bWince = true;
								}
							}
						}

						m_state = STATE_DAMAGE;				// ダメージを受けている状態にする

						m_nLife -= nDamage;					// 体力の減算

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
											{//NULLではない場合
												m_nKillPlayerIdx = pPlayer->GetPlayerIdx();								//キルプレイヤーの番号を設置処理
												CManager::GetGame()->SetKillIdx(nCntKill, pPlayer->GetPlayerIdx());		//キルプレイヤーの番号を設置処理
												CManager::GetGame()->SetDeathIdx(nCntKill, m_nPlayerIdx);				//デスプレイヤーの番号を設置処理
												CManager::GetGame()->SetPlayerType(0, CGame::TYPE_PLAYER);				//プレイヤーの種類を設置処理
												CManager::GetGame()->SetPlayerType(1, CGame::TYPE_PLAYER);				//プレイヤーの種類を設置処理
												CManager::GetGame()->SetLog(nCntKill, true);							//ログの設置処理
											}
										}
										else if (pScene->GetObjType() == CScene::OBJTYPE_AI)
										{//オブジェクトの種類がAIの場合
											CAIMecha *pAIMecha = (CAIMecha*)pScene;
											if (pAIMecha != NULL)
											{//NULLではない場合
												m_nKillPlayerIdx = pAIMecha->GetPlayer()->GetPlayerIdx();							//キルプレイヤーの番号を設置処理
												CManager::GetGame()->SetKillIdx(nCntKill, pAIMecha->GetPlayer()->GetPlayerIdx());	//キルプレイヤーの番号を設置処理
												CManager::GetGame()->SetDeathIdx(nCntKill, m_nPlayerIdx);							//デスプレイヤーの番号を設置処理
												if (pAIMecha->GetMechaType() == CAIMecha::MECHATYPE_DRONE)
												{//オブジェクトの種類がドローンの場合
													CManager::GetGame()->SetPlayerType(0, CGame::TYPE_DROWN);						//プレイヤーの種類を設置処理
													CManager::GetGame()->SetPlayerType(1, CGame::TYPE_PLAYER);						//プレイヤーの種類を設置処理
													CManager::GetGame()->SetLog(nCntKill, true);									//ログの設置処理
												}
												else if (pAIMecha->GetMechaType() == CAIMecha::MECHATYPE_WORKER)
												{//オブジェクトの種類がワーカーの場合
													CManager::GetGame()->SetPlayerType(0, CGame::TYPE_WORKER);						//プレイヤーの種類を設置処理
													CManager::GetGame()->SetPlayerType(1, CGame::TYPE_PLAYER);						//プレイヤーの種類を設置処理
													CManager::GetGame()->SetLog(nCntKill, true);									//ログの設置処理
												}
											}
										}
										break;
									}
								}
							}

							for (int nCntAI = 0; nCntAI < AI_MAX; nCntAI++)
							{
								for (int nCntParts = 0; nCntParts < m_pAI[nCntAI]->GetNumParts(); nCntParts++)
								{
									//AIを表示させない
									m_pAI[nCntAI]->GetModel(nCntParts)->SetDisp(false);
								}

								//パーティクルを生成
								CParticle::Create(m_pAI[nCntAI]->GetModel(0)->GetWorldPos(), 4);
								CParticle::Create(m_pAI[nCntAI]->GetModel(0)->GetWorldPos(), 5);

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
								CManager::GetGame()->SetBlueLinkEnergy(CManager::GetGame()->GetBlueLinkEnergy() - 30);
								break;
							case 1:
								CManager::GetGame()->SetRedLinkEnergy(CManager::GetGame()->GetRedLinkEnergy() - 30);
								break;
							}
						}
					}
				}
			}
			if (CManager::GetClient()->GetPlayerIdx() == 0 && m_bConnect == false)
			{//CPUの場合
				if (NULL != m_pUpperMotion && NULL != m_pLowerMotion)
				{// モーションクラスが使われている
					if (m_nLife > 0 && m_bDeath == false)
					{//体力が０より大きく且つ死亡していない場合
						if (pScene->GetObjType() == CScene::OBJTYPE_PLAYER)
						{//オブジェクトの種類がプレイヤーの場合
							CPlayer *pPlayer = (CPlayer*)pScene;
							if (pPlayer->GetMechaType() == CMechaSelect::MECHATYPE_HEAVY || pPlayer->GetMechaType() == CMechaSelect::MECHATYPE_SNIPE)
							{
								if (CManager::GetGame()->GetDamageDirection()->GetDamageDirection(CDamageDirection::DIRECTION_FRONT))
								{
									m_pUpperMotion->SetMotion(CMotionManager::TYPE_DAMAGE_FRONT);	// ダメージモーションを再生
									m_pLowerMotion->SetMotion(CMotionManager::TYPE_DAMAGE_FRONT);	// ダメージモーションを再生
									m_bWince = true;
								}
								else if (CManager::GetGame()->GetDamageDirection()->GetDamageDirection(CDamageDirection::DIRECTION_BACK) ||
									CManager::GetGame()->GetDamageDirection()->GetDamageDirection(CDamageDirection::DIRECTION_RIGHT) ||
									CManager::GetGame()->GetDamageDirection()->GetDamageDirection(CDamageDirection::DIRECTION_LEFT))
								{
									m_pUpperMotion->SetMotion(CMotionManager::TYPE_DAMAGE_BACK);	// ダメージモーションを再生
									m_pLowerMotion->SetMotion(CMotionManager::TYPE_DAMAGE_BACK);	// ダメージモーションを再生
									m_bWince = true;
								}
							}
						}

						m_state = STATE_DAMAGE;				// ダメージを受けている状態にする

						m_nLife -= nDamage;					// 体力の減算

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
											{//NULLではない場合
												m_nKillPlayerIdx = pPlayer->GetPlayerIdx();								//キルプレイヤーの番号を設置処理
												CManager::GetGame()->SetKillIdx(nCntKill, pPlayer->GetPlayerIdx());		//キルプレイヤーの番号を設置処理
												CManager::GetGame()->SetDeathIdx(nCntKill, m_nPlayerIdx);				//デスプレイヤーの番号を設置処理
												CManager::GetGame()->SetPlayerType(0, CGame::TYPE_PLAYER);				//プレイヤーの種類を設置処理
												CManager::GetGame()->SetPlayerType(1, CGame::TYPE_PLAYER);				//プレイヤーの種類を設置処理
												CManager::GetGame()->SetLog(nCntKill, true);							//ログの設置処理
											}
										}
										else if (pScene->GetObjType() == CScene::OBJTYPE_AI)
										{//オブジェクトの種類がAIの場合
											CAIMecha *pAIMecha = (CAIMecha*)pScene;
											if (pAIMecha != NULL)
											{//NULLではない場合
												m_nKillPlayerIdx = pAIMecha->GetPlayer()->GetPlayerIdx();							//キルプレイヤーの番号を設置処理
												CManager::GetGame()->SetKillIdx(nCntKill, pAIMecha->GetPlayer()->GetPlayerIdx());	//キルプレイヤーの番号を設置処理
												CManager::GetGame()->SetDeathIdx(nCntKill, m_nPlayerIdx);							//デスプレイヤーの番号を設置処理
												if (pAIMecha->GetMechaType() == CAIMecha::MECHATYPE_DRONE)
												{//オブジェクトの種類がドローンの場合
													CManager::GetGame()->SetPlayerType(0, CGame::TYPE_DROWN);						//プレイヤーの種類を設置処理
													CManager::GetGame()->SetPlayerType(1, CGame::TYPE_PLAYER);						//プレイヤーの種類を設置処理
													CManager::GetGame()->SetLog(nCntKill, true);									//ログの設置処理
												}
												else if (pAIMecha->GetMechaType() == CAIMecha::MECHATYPE_WORKER)
												{//オブジェクトの種類がワーカーの場合
													CManager::GetGame()->SetPlayerType(0, CGame::TYPE_WORKER);						//プレイヤーの種類を設置処理
													CManager::GetGame()->SetPlayerType(1, CGame::TYPE_PLAYER);						//プレイヤーの種類を設置処理
													CManager::GetGame()->SetLog(nCntKill, true);									//ログの設置処理
												}
											}
										}
										break;
									}
								}
							}

							for (int nCntAI = 0; nCntAI < AI_MAX; nCntAI++)
							{
								for (int nCntParts = 0; nCntParts < m_pAI[nCntAI]->GetNumParts(); nCntParts++)
								{
									//AIを表示させない
									m_pAI[nCntAI]->GetModel(nCntParts)->SetDisp(false);
								}

								//パーティクルを生成
								CParticle::Create(m_pAI[nCntAI]->GetModel(0)->GetWorldPos(), 4);
								CParticle::Create(m_pAI[nCntAI]->GetModel(0)->GetWorldPos(), 5);

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
								CManager::GetGame()->SetBlueLinkEnergy(CManager::GetGame()->GetBlueLinkEnergy() - 30);
								break;
							case 1:
								CManager::GetGame()->SetRedLinkEnergy(CManager::GetGame()->GetRedLinkEnergy() - 30);
								break;
							}
						}
					}
				}
			}
		}
	}

	if (CMenu::GetMode() == CMenu::MODE_TUTORIAL)
	{//シングルプレイの場合
		if (NULL != m_pUpperMotion && NULL != m_pLowerMotion)
		{// モーションクラスが使われている
			if (m_nLife > 0 && m_bDeath == false)
			{//体力が０より大きく且つ死亡していない場合

				m_state = STATE_DAMAGE;								// ダメージを受けている状態にする

				m_nLife -= nDamage;									//体力の減算

				if (0 >= m_nLife)
				{//体力が０以下の場合
					m_nLife = 0;		//体力を０にする
					m_bDeath = true;	//死亡状態にする

					if (m_bDeath == true && CManager::GetMode() == CManager::MODE_TUTORIAL)
					{//死亡している場合
						CSound *pSound = CManager::GetSound();
						pSound->PlaySound(CSound::SOUND_LABEL_EXPLOSION);

						//パーティクルを生成
						CParticle::Create(m_pModel[0]->GetWorldPos(), 4);
						CParticle::Create(m_pModel[0]->GetWorldPos(), 5);

						for (int nCntModel = 0; nCntModel < m_nNumParts; nCntModel++)
						{//表示しない
							m_pModel[nCntModel]->SetDisp(false);
						}

					}
				}
			}
		}
	}

}

//=========================================
// ひるみ状態の取得
//=========================================
bool CPlayer::GetWince(void)
{
	if (m_pUpperMotion->GetMotion() != CMotionManager::TYPE_DAMAGE_FRONT && m_pUpperMotion->GetMotion() != CMotionManager::TYPE_DAMAGE_BACK)
	{
		return false;
	}

	return true;
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
		if (m_bConnect == true)
		{
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
		}
		else
		{
			m_nCntCPURespawn++;
			if (m_nCntCPURespawn % 300 == 0)
			{
				respawn = RESPAWN_SELECT;		// リスポーン選択状態に設定
				m_nCntCPURespawn = 0;
			}
		}
		break;
	}

	m_Respawn = respawn;

	//CDebugProc::Print("m_nDisTime : %d\n", m_nDisTime);
	//CDebugProc::Print("m_nRespawnTimer : %d\n", m_nRespawnTimer);
}

//=========================================
// リスポーン選択処理
//=========================================
void CPlayer::SelectRespawn(void)
{
	if (m_bConnect == true)
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

				if (m_bConnect == false)
				{
					// 開始時点のノードの初期化
					float fMinLength = 100000, fLength = 100000;	// 差分系

					// 開始時点のノードの初期化
					for (int nCntNode = 0; nCntNode < m_pNodeData->GetLoadData().nodeMax; nCntNode++)
					{// ノードの数だけ回る
					 // 差分を求める
						fLength = (m_pNodeData->GetLoadData().pos[nCntNode].x - m_pos.x) *
							(m_pNodeData->GetLoadData().pos[nCntNode].x - m_pos.x) +
							(m_pNodeData->GetLoadData().pos[nCntNode].z - m_pos.z) *
							(m_pNodeData->GetLoadData().pos[nCntNode].z - m_pos.z);

						if (fMinLength > fLength)
						{// 差分の最小値を求める
							fMinLength = fLength;
							m_nStartNode = nCntNode;
							m_nEndNode = nCntNode;
						}
					}

					// 経路探索
					CPlayer::RootSearch();
				}

				// 通常状態に戻る
				m_Respawn = RESPAWN_NONE;

				// ライフの設定
				m_nLife = m_nLifeMax;

				//死亡していないようにする
				m_bDeath = false;

				//AIの初期化処理
				m_pAI[0]->SetPos(m_pos + D3DXVECTOR3(0.0f, 70.0f, 0.0f));
				m_pAI[1]->SetPos(m_pos);

				for (int nCntAI = 0; nCntAI < AI_MAX; nCntAI++)
				{
					m_pAI[nCntAI]->SetLife(m_pAI[nCntAI]->GetLifeMax());
					m_pAI[nCntAI]->SetDeath(false);
					// 描画する
					for (int nCntParts = 0; nCntParts < m_pAI[nCntAI]->GetNumParts(); nCntParts++)
					{
						m_pAI[nCntAI]->GetModel(nCntParts)->SetDisp(true);
					}
				}
				// 描画する
				for (int nCntParts = 0; nCntParts < m_nNumParts; nCntParts++)
				{
					m_pModel[nCntParts]->SetDisp(true);
				}

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
	}
	else
	{

		m_point = (POINT)(rand() % POINT_MAX);

		m_pos = CManager::GetGame()->GetRespawnPos(m_nTeam, m_point);

		if (m_bConnect == false)
		{
			// 開始時点のノードの初期化
			float fMinLength = 100000, fLength = 100000;	// 差分系

															// 開始時点のノードの初期化
			for (int nCntNode = 0; nCntNode < m_pNodeData->GetLoadData().nodeMax; nCntNode++)
			{// ノードの数だけ回る
			 // 差分を求める
				fLength = (m_pNodeData->GetLoadData().pos[nCntNode].x - m_pos.x) *
					(m_pNodeData->GetLoadData().pos[nCntNode].x - m_pos.x) +
					(m_pNodeData->GetLoadData().pos[nCntNode].z - m_pos.z) *
					(m_pNodeData->GetLoadData().pos[nCntNode].z - m_pos.z);

				if (fMinLength > fLength)
				{// 差分の最小値を求める
					fMinLength = fLength;
					m_nStartNode = nCntNode;
					m_nEndNode = nCntNode;
				}
			}

			// 経路探索
			CPlayer::RootSearch();
		}

		// 通常状態に戻る
		m_Respawn = RESPAWN_NONE;

		// ライフの設定
		m_nLife = m_nLifeMax;

		//死亡していないようにする
		m_bDeath = false;

		//AIの初期化処理
		m_pAI[0]->SetPos(m_pos + D3DXVECTOR3(0.0f, 70.0f, 0.0f));
		m_pAI[1]->SetPos(m_pos);

		for (int nCntAI = 0; nCntAI < AI_MAX; nCntAI++)
		{
			m_pAI[nCntAI]->SetLife(m_pAI[nCntAI]->GetLifeMax());
			m_pAI[nCntAI]->SetDeath(false);
			// 描画する
			for (int nCntParts = 0; nCntParts < m_pAI[nCntAI]->GetNumParts(); nCntParts++)
			{
				m_pAI[nCntAI]->GetModel(nCntParts)->SetDisp(true);
			}
		}
		// 描画する
		for (int nCntParts = 0; nCntParts < m_nNumParts; nCntParts++)
		{
			m_pModel[nCntParts]->SetDisp(true);
		}
	}
	//CDebugProc::Print("リスポーン選択中");
}
//=============================================================================
//	チャットボタンの生成
//=============================================================================
void CPlayer::CreateRadioChatButton(void)
{
	if (m_pUIRadioBotton[0] == NULL && m_pUIRadioBotton[1] == NULL && m_pUIRadioBotton[2] == NULL && m_pUIRadioBotton[3] == NULL
		&& m_pUIRadioBotton[4] == NULL && m_pUIRadioBotton[5] == NULL && m_pUIRadioBotton[6] == NULL && m_pUIRadioBotton[7] == NULL && m_pCursor == NULL)
	{	// ボタンとカーソルの生成
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

	if (m_pUIButtonOption == NULL)
	{	// オプションボタン生成
		m_pUIButtonOption = CButton2D::Create(D3DXVECTOR3(970.0f, 530.0f, 0.0f), 100.0f, 120.0f);
		m_pUIButtonOption->BindTexture(CTexture::GetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_OPTION_ICON)));
	}
}

//=============================================================================
//	チャットボタンの破棄
//=============================================================================
void CPlayer::UninitRadioChatButton(void)
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

	if (m_pUIButtonOption != NULL)
	{	// オプションボタン破棄
		m_pUIButtonOption->Uninit();
		m_pUIButtonOption = NULL;
	}
}
//=============================================================================
//	ラジオチャットボタン生成処理
//=============================================================================
void CPlayer::ChatBotton(void)
{
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();	// キーボードの入力を取得
	CDirectInput *pDirectInput = CManager::GetDirectInput();	//DirectInputの取得
	CDirectInput::GamePad *DirectInputStick = pDirectInput->GetgamePadStick();

	if (pKeyboard->GetTrigger(DIK_C) || pDirectInput->GetGamePadTrigger(1))
	{
		m_nRadioChat = (m_nRadioChat + 1) % 2;

		if (m_nRadioChat == 1)
		{
			m_bChatBotton = true;		// ボタン表示中
			CreateRadioChatButton();	// チャットボタンの生成
		}
		else
		{
			m_bChatBotton = false;		// ボタン非表示中
			UninitRadioChatButton();	// チャットボタンの破棄
		}
	}

	if (m_pUIRadioBotton[0] != NULL && m_pUIRadioBotton[1] != NULL && m_pUIRadioBotton[2] != NULL && m_pUIRadioBotton[3] != NULL
		&& m_pUIRadioBotton[4] != NULL && m_pUIRadioBotton[5] != NULL && m_pUIRadioBotton[6] != NULL && m_pUIRadioBotton[7] != NULL && m_pCursor != NULL && m_pUIButtonOption != NULL)
	{	// ボタンとカーソルが生成された
		m_bChat = false;
		int nSelect = -1;

		// オプションボタンの設定
		if (m_pUIButtonOption->InRangeMenu(m_pCursor->GetMousePosition()))
		{// 範囲内かチェック
			if (m_pUIButtonOption->ClickRelease())
			{// クリックされた
				m_bOption = true;				// オプション設定中
				m_bChatBotton = false;		// ボタン非表示
			}
		}

		// チャットボタンの判定
		if (m_bOption == false)
		{	// オプション設定じゃない時
			for (int nCntButton = 0; nCntButton < RADIOCHAT_BOTTON; nCntButton++)
			{
				if (m_pUIRadioBotton[nCntButton]->InRangeMenu(m_pCursor->GetMousePosition()))
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
		m_pUITexRadio->SetObjType(OBJTYPE_MESS);
	}
	if (m_pUITexRadio != NULL)
	{

		UninitRadioChatButton();
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
			m_nRadioChat = 0;				// 切り替えリセット
		}
	}
}

//=============================================================================
//	仲間のメッセージを表示する処理
//=============================================================================
void CPlayer::AllyChatMess(void)
{
	if (m_pUITexAllyRadio == NULL)
	{//NULLの場合
		m_pUITexAllyRadio = CUI_TEXTURE::Create(D3DXVECTOR3(1280.0f, 550.0f, 0.0f), RADIOCHAT_MESS_WIDTH, RADIOCHAT_MESS_HEIGHT, CUI_TEXTURE::UIFLAME_RADIOCHAT_MESS);
		m_pUITexAllyRadio->SetTex(m_allyRadiochat, 1, RADIOCHAT_BOTTON_PATTERN);
		m_pUITexAllyRadio->SetObjType(OBJTYPE_MESS);
	}

	if (m_pUITexAllyRadio != NULL)
	{//NULLではない場合
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
			m_nAllyTimer++;		// カウンター加算
			if (m_nAllyTimer % RADIOCHAT_DISPLAY_TIME == 0)
			{	// 5秒経ったら
				m_nAllyTimer = 0;
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
		}
	}
}

//=============================================================================
//	オプションの破棄
//=============================================================================
void CPlayer::UninitOption(void)
{
	for (int nCnt = 0; nCnt < OPTION_SELECT; nCnt++)
	{
		if (m_pUIButtonSelect[nCnt] != NULL)
		{	// 項目の破棄
			m_pUIButtonSelect[nCnt]->Uninit();
			m_pUIButtonSelect[nCnt] = NULL;
		}
	}

	if (m_pUITexOption != NULL)
	{	// フレームの破棄
		m_pUITexOption->Uninit();
		m_pUITexOption = NULL;
	}

	if (m_pCursor != NULL)
	{	// カーソルの破棄
		m_pCursor->Uninit();
		m_pCursor = NULL;
	}

	if (m_pUIButtonBack != NULL)
	{	// ×ボタンの破棄
		m_pUIButtonBack->Uninit();
		m_pUIButtonBack = NULL;
	}
}

//=============================================================================
//	オプション設定処理
//=============================================================================
void CPlayer::Option(bool bOption)
{
	CCamera *pCamera = NULL;

	if (bOption == true)
	{
		if (m_pUITexOption == NULL && m_pUIButtonSelect[0] == NULL && m_pUIButtonSelect[1] == NULL
			&& m_pUIButtonSelect[2] == NULL && m_pUIButtonSelect[3] == NULL && m_pUIButtonSelect[4] == NULL && m_pUIButtonBack == NULL)
		{	// マウス設定フレーム、トグルスイッチ生成
			m_pUITexOption = CUI_TEXTURE::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), 450.0f, 200.0f, CUI_TEXTURE::UIFLAME_OPTION_FLAME);

			for (int nCnt = 0; nCnt < OPTION_SELECT; nCnt++)
			{	// トグルスイッチの生成
				m_pUIButtonSelect[nCnt] = CButton2D::Create(D3DXVECTOR3(460.0f + (nCnt * 92.0f), SCREEN_HEIGHT / 2 + 30.0f, 0.0f), 30.0f, 30.0f);
				m_pUIButtonSelect[nCnt]->BindTexture(CTexture::GetTexture(CTexture::TEXTURE_OPTION_TOGGlE));
				m_pUIButtonSelect[nCnt]->SetTex(0, 1, 2);		// 最初は黒テクスチャ
			}

			// ×ボタン
			m_pUIButtonBack = CButton2D::Create(D3DXVECTOR3(820.0f, 300.0f, 0.0f), 65.0f, 40.0f);
			m_pUIButtonBack->BindTexture(CTexture::GetTexture(CTexture::TEXTURE_OPTION_BATSU));
		}

		// 使用しないテクスチャは破棄する
		for (int nCnt = 0; nCnt < RADIOCHAT_BOTTON; nCnt++)
		{
			if (m_pUIRadioBotton[nCnt] != NULL)
			{	// ラジオチャットボタンの破棄
				m_pUIRadioBotton[nCnt]->Uninit();
				m_pUIRadioBotton[nCnt] = NULL;
			}
		}

		if (m_pUIButtonOption != NULL)
		{	// オプションボタンの破棄
			m_pUIButtonOption->Uninit();
			m_pUIButtonOption = NULL;
		}

		if (m_pUITexOption != NULL && m_pUIButtonSelect[0] != NULL&& m_pUIButtonSelect[1] != NULL
			&& m_pUIButtonSelect[2] != NULL && m_pUIButtonSelect[3] != NULL && m_pUIButtonSelect[4] != NULL)
		{	// マウス設定フレーム、トグルスイッチ生成された
			bool bOptionSelect = false;		// 何も選択されていない状態

											// カメラ速度設定ボタン
			for (int nCntButton = 0; nCntButton < OPTION_SELECT; nCntButton++)
			{
				if (m_pUIButtonSelect[nCntButton]->InRangeMenu(m_pCursor->GetMousePosition()))
				{// 範囲内かチェック
					if (m_pUIButtonSelect[nCntButton]->ClickRelease())
					{// クリックされた
						bOptionSelect = true;
						m_nSelectOption = nCntButton;			// クリックされたやつ
						SetSelectOption(m_nSelectOption);	// 選択項目の設定
						break;
					}
				}

				if (nCntButton == m_nSelectOption)
				{	// 選択したやつ
					m_pUIButtonSelect[nCntButton]->SetTex(1, 1, 2);
					m_pUIButtonSelect[nCntButton]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				}
				else
				{	// それ以外
					m_pUIButtonSelect[nCntButton]->SetTex(0, 1, 2);
					m_pUIButtonSelect[nCntButton]->SetColor(D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f));
				}
			}

			// ×ボタン
			if (m_pUIButtonBack->InRangeMenu(m_pCursor->GetMousePosition()))
			{	// 範囲内かチェック
				if (m_pUIButtonBack->ClickRelease())
				{	// クリックされた
					UninitOption();			// オプションの破棄
					m_bOption = false;		// オプション設定OFF
					m_nRadioChat = 0;		// 切り替えリセット

				}
			}
		}
	}
}

//=============================================================================
//	戦闘用テキスト読み込み
//=============================================================================
void CPlayer::LoadBattleFile(void)
{
	FILE *pFile;									// 読み込むファイル
	char acLine[MAX_CHAR];							// 1行読み込む変数
	char acData[MAX_CHAR];							// 1文読み込む変数

	char *pBattleFileName = "";	//読み込むテキスト
	if (CMechaSelect::MECHATYPE_ASSULT == m_mecha) { pBattleFileName = ASSULT_BATTLE_FILE; }
	else if (CMechaSelect::MECHATYPE_LIGHT == m_mecha) { pBattleFileName = LIGHT_BATTLE_FILE; }
	else if (CMechaSelect::MECHATYPE_HEAVY == m_mecha) { pBattleFileName = HEAVY_BATTLE_FILE; }
	else if (CMechaSelect::MECHATYPE_SNIPE == m_mecha) { pBattleFileName = SNIPE_BATTLE_FILE; }

	// ファイルを開く
	pFile = fopen(pBattleFileName, "r");

	if (pFile != NULL)
	{
		sscanf(&acLine[0], "%s", &acData);				// 文をデータにコピー

		while (fgets(&acLine[0], MAX_CHAR, pFile) != NULL)
		{
			sscanf(&acLine[0], "%s", &acData);				// 文をデータにコピー

			if (memcmp(acData, "SCRIPT", 6) == 0)
			{// "SCRIPT"の文字列があったら
				while (strcmp(acData, "END_SCRIPT") != 0)
				{// "END_SCRIPT"が来るまで情報を取得し続ける
					fgets(&acLine[0], MAXCHAR, pFile);					// 行を取得
					sscanf(&acLine[0], "%s", &acData);					// データをコピー
					if (memcmp(acData, "CHARACTERSET", 12) == 0)
					{// "CHARACTERSET"の文字列があったら

						m_pSearch = new CSearch*[MAX_SEARCH];
						for (int nCntSearch = 0; nCntSearch < MAX_SEARCH; nCntSearch++)
						{//数分ループ
							m_pSearch[nCntSearch] = CSearch::Create(&m_pModel[1]->GetMtxWorld());

							m_pSearch[nCntSearch]->SetParent(NULL);
						}

						while (strcmp(acData, "END_CHARACTERSET") != 0)
						{// "END_CHARACTERSET"が来るまで情報を取得し続ける
							fgets(&acLine[0], MAXCHAR, pFile);					// 行を取得
							sscanf(&acLine[0], "%s", &acData);					// データをコピー
							if (strcmp(acData, "SEARCH_LENGTH") == 0)
							{// SEARCH_LENGTHがあったら
								sscanf(&acLine[0], "%s %s %f", &acData[0], &acData[0], &m_fSearchLength);	// 認識距離
								for (int nCntSearch = 0; nCntSearch < 3; nCntSearch++)
								{//数分ループ
									m_pSearch[nCntSearch]->SetPos(D3DXVECTOR3(0.0f, 0.0f, -m_fSearchLength));
								}
								m_pSearch[3]->SetPos(D3DXVECTOR3(0.0f, 0.0f, 150.0f));
							}
							if (strcmp(acData, "SEARCH_ANGLE") == 0)
							{// SEARCH_ANGLEがあったら
								float rot;
								sscanf(&acLine[0], "%s %s %f", &acData[0], &acData[0], &rot);	// 認識角度
																								//テキストの角度通りに代入する
								m_fSearchAngle = (D3DX_PI * 2 / 360.0f * rot);
								for (int nCntSearch = 1; nCntSearch < 3; nCntSearch++)
								{//数分ループ
									if (nCntSearch = 1)
									{
										m_pSearch[nCntSearch]->SetRot(D3DXVECTOR3(0.0f, m_fSearchAngle, 0.0f));
									}
									if (nCntSearch = 2)
									{
										m_pSearch[nCntSearch]->SetRot(D3DXVECTOR3(0.0f, -m_fSearchAngle, 0.0f));
									}
								}
							}
						}
					}
				}
			}//SCRIPT
		}//while
		fclose(pFile);// 開いたファイルを閉じる
	}//ファイルが開けてたら
	else
	{// ファイルがないとき
		MessageBox(0, "ファイルがありません", "戦闘用テキスト", MB_YESNO);
	}
}


//=============================================================================
//	探索
//=============================================================================
bool CPlayer::Distance(void)
{
	bool bFind = false;

	D3DXMATRIX mtxSearch1 = m_pSearch[1]->GetMtxWorld();
	D3DXMATRIX mtxSearch2 = m_pSearch[2]->GetMtxWorld();

	//空のベクトルに代入
	D3DXVECTOR3 VecPos_0 = D3DXVECTOR3(mtxSearch1._41, mtxSearch1._42, mtxSearch1._43);//左側
	D3DXVECTOR3 VecPos_1 = D3DXVECTOR3(mtxSearch2._41, mtxSearch2._42, mtxSearch2._43);//右側

	D3DXVECTOR3 EmpVec_0;//左側
	D3DXVECTOR3 EmpVec_1;//右側
	D3DXVECTOR3 EmpVec_2;//左右間

	D3DXVECTOR3 Vec_C_P;//CPUとプレイヤーのベクトル
	D3DXVECTOR3 Vec_0_P;//点０とプレイヤーのベクトル
	D3DXVECTOR3 Vec_1_P;//点１とプレイヤーのベクトル


	float fCrossProduct0;//外積計算用
	float fCrossProduct1;//外積計算用
	float fCrossProduct2;//外積計算用

	// 敵を探す
	CScene *pScene = CScene::GetSceneTop(FIND_FIND_CHARACTER_PRIORITY);
	CScene *pSceneNext = NULL;
	while (pScene != NULL)
	{// NULLになるまでループ
		pSceneNext = pScene->GetSceneNext();
		CScene::OBJTYPE objType = pScene->GetObjType();

		if (CScene::OBJTYPE_PLAYER == objType)
		{//プレイヤーオブジェクトのとき
			CPlayer *pPlayer = (CPlayer*)pScene;
			int nTeam = pPlayer->GetTeam();
			int nIdxPlayer = pPlayer->GetPlayerIdx();
			if (m_nTeam != nTeam)
			{//チームが違うとき
			 //認識用ベクトルの計算
				m_SearchVec_0 = CSearch::Sub_Vector(VecPos_1, m_pos);
				EmpVec_0 = CSearch::Sub_Vector(pPlayer->GetPos(), VecPos_1);

				m_SearchVec_1 = CSearch::Sub_Vector(VecPos_0, VecPos_1);
				EmpVec_1 = CSearch::Sub_Vector(pPlayer->GetPos(), VecPos_0);

				m_SearchVec_2 = CSearch::Sub_Vector(m_pos, VecPos_0);
				EmpVec_2 = CSearch::Sub_Vector(pPlayer->GetPos(), m_pos);

				//外積計算
				fCrossProduct0 = (m_SearchVec_0.x * EmpVec_0.z) - (m_SearchVec_0.z * EmpVec_0.x);
				fCrossProduct1 = (m_SearchVec_1.x * EmpVec_1.z) - (m_SearchVec_1.z * EmpVec_1.x);
				fCrossProduct2 = (m_SearchVec_2.x * EmpVec_2.z) - (m_SearchVec_2.z * EmpVec_2.x);

				//CDebugProc::Print("CPUベクトル１：%.2f %.2f %.2f", m_SearchVec_0.x, m_SearchVec_0.y, m_SearchVec_0.z);
				//CDebugProc::Print("CPUベクトル２：%.2f %.2f %.2f", m_SearchVec_1.x, m_SearchVec_1.y, m_SearchVec_1.z);
				//CDebugProc::Print("CPUベクトル３：%.2f %.2f %.2f", m_SearchVec_2.x, m_SearchVec_2.y, m_SearchVec_2.z);

				//CDebugProc::Print("fCrossProduct0：%.2f", fCrossProduct0);
				//CDebugProc::Print("fCrossProduct1：%.2f", fCrossProduct1);
				//CDebugProc::Print("fCrossProduct2：%.2f", fCrossProduct2);

				if ((fCrossProduct0 > 0.0f && fCrossProduct1 > 0.0f && fCrossProduct2 > 0.0f) || (fCrossProduct0 < 0.0f && fCrossProduct1 < 0.0f && fCrossProduct2 < 0.0f))
				{// 三角形の内側に敵がいる

					// 発見した敵プレイヤーの情報を取得
					m_pFindPlayer = pPlayer;

					if (m_nCountCollect >= COLLECTIONDATA_MAX)
					{// 収集データが最大まで集まった場合
						m_bCollectSwitch = true;// 平均値の割り出し方法を切り替える
						m_nCountCollect = 0;// カウントを初期化する
					}
					m_collectionPos[m_nCountCollect] = m_pFindPlayer->GetPos();	// 敵プレイヤーの位置情報を取得
					m_nCountCollect++;	// カウントを進める

					// ピンの配置
					m_PinPos = m_pFindPlayer->GetPos();
					m_bPin = true;

					bFind = true;
					return bFind;
				}
				else
				{
					// 発見しているプレイヤーの情報を削除する
					m_pFindPlayer = NULL;
					m_bPin = false;
					bFind = false;
				}

				//CDebugProc::Print("bFind = %d", bFind);
			}
		}
		// 次のオブジェクトを見る
		pScene = pSceneNext;
	}

	return bFind;
}

//=============================================================================
//	戦闘
//=============================================================================
void CPlayer::Battle(void)
{
	CScene *pScene = CScene::GetSceneTop(FIND_FIND_CHARACTER_PRIORITY);
	CScene *pSceneNext = NULL;
	D3DXVECTOR3 dispertion; // ブレ

	while (pScene != NULL)
	{// NULLになるまでループ
		pSceneNext = pScene->GetSceneNext();
		CScene::OBJTYPE objType = pScene->GetObjType();

		if (CScene::OBJTYPE_PLAYER == objType)
		{// プレイヤーオブジェクトのとき
			CPlayer *pPlayer = (CPlayer*)pScene;
			int nTeam = pPlayer->GetTeam();

			if (m_nTeam != nTeam)
			{// チームが違うとき
				if (rand() % 20 == 0)
				{
					// 弾撃ち
					CpuShoot();
				}
			}
		}

		 // 次のオブジェクトを見る
		pScene = pSceneNext;
	}
}

//=============================================================================
//	移動
//=============================================================================
void CPlayer::BattleMovent(void)
{
	bool bKey = false;	// ボタン押下フラグ
	float fDirMove;

	if (m_pFindPlayer != NULL)
	{// 敵プレイヤーを発見している場合
		// 発見した敵プレイヤーの方向へ移動する
		fDirMove = atan2f(m_pFindPlayer->GetPos().x - m_pos.x, m_pFindPlayer->GetPos().z - m_pos.z);

		// 発見した敵プレイヤーの方向を見る
		m_rotDest.y = atan2f(m_pFindPlayer->GetPos().x - m_pos.x, m_pFindPlayer->GetPos().z - m_pos.z);
		bKey = true;
	}

	if (bKey)
	{// 移動量の加算
		m_move = D3DXVECTOR3(sinf(fDirMove), 0.0f, cosf(fDirMove)) * 3.0f;
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
	else { m_pos += m_move; }
}

//=============================================================================
//	弾発射処理
//=============================================================================
void CPlayer::CpuShoot(void)
{
	D3DXVECTOR3 dispertion;	// 弾のブレ

	// 弾の発射間隔
	m_nCntShoot = (m_nCntShoot + 1) % 7;

	if (m_nRemBullet > 0)
	{// 弾が残っている場合
		for (int nCntShoots = 0; nCntShoots < m_nNumShoot; nCntShoots++)
		{
			// カメラの角度と注視点を取得
			D3DXVECTOR3 rotCamera = CManager::GetCamera()->GetRot();
			D3DXVECTOR3 posR = CManager::GetCamera()->GetPosR();

			// レティクル（目的の位置）の取得
			D3DXMATRIX MtxSearch = m_pSearch[0]->GetMtxWorld();
			D3DXVECTOR3 posReticle = D3DXVECTOR3(MtxSearch._41, MtxSearch._42, MtxSearch._43);
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
			CBulletPlayer::Create(posCanon, m_pAngle[nCntShoots * 2], m_pAngleV[nCntShoots * 2], m_nAttack, m_nTeam, this,m_fBulletSpeed);

			// レティクル（目的の位置）の取得
			posReticle = D3DXVECTOR3(MtxSearch._41, MtxSearch._42, MtxSearch._43);
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

			// 弾の発射
			CBulletPlayer::Create(posCanon, m_pAngle[nCntShoots * 2 + 1], m_pAngleV[nCntShoots * 2 + 1], m_nAttack, m_nTeam, this,m_fBulletSpeed);
		}

		// 弾を減らす
		m_nRemBullet--;
	}
	else if (m_nRemBullet <= 0)
	{// 弾がなくなった場合
		m_bReload = true;
	}
}

//=============================================================================
//	シングル時ピン関係更新処理
//=============================================================================
void CPlayer::PinUpdateSingle()
{
	CInputMouse *pMouse = CManager::GetInputMouse(); // マウスの入力を取得

	//===================================
	// 自分のピン配置
	//===================================
	if (pMouse->GetTrigger(CInputMouse::DIMS_BUTTON_2) == true || pMouse->GetMouseAxisZ() >= 120.0f || pMouse->GetMouseAxisZ() <= -120.0f)
	{// ホイールクリックorロール
		// カメラの角度と注視点を取得
		D3DXVECTOR3 rotCamera = CManager::GetCamera()->GetRot();
		D3DXVECTOR3 posR = CManager::GetCamera()->GetPosR();

		// レティクル（目的の位置）の取得
		D3DXVECTOR3 posReticle = m_pReticle->GetPos();

		// 射出口の位置の取得
		D3DXMATRIX mtxCanon = m_pModel[2]->GetMtxWorld();
		D3DXVECTOR3 posCanon = D3DXVECTOR3(mtxCanon._41, mtxCanon._42, mtxCanon._43) + D3DXVECTOR3(sinf(rotCamera.y) * 30.0f, cosf(rotCamera.x) * 30.0f, cosf(rotCamera.y) * 30.0f);

		// 水平方向の角度の計算
		m_pAngle[0] = atan2f(posReticle.x - posCanon.x, posReticle.z - posCanon.z);
		m_pAngleV[0] = rotCamera.x;

		if (m_pPinBullet != NULL)
		{// 前回の弾の破棄
			m_pPinBullet->Uninit();
			m_pPinBullet = NULL;
		}

		// 弾の生成
		m_pPinBullet = m_pPinBullet->Create(posCanon, m_pAngle[0], m_pAngleV[0], m_nTeam, this);
	}

	if (m_pPinBullet != NULL)
	{// 判定用の弾がNULLじゃないとき
		if (m_pPinBullet->BulletCollision())
		{// 弾が当たったとき
		 // ピン立て用弾の判定チェック
			m_PinPos = m_pPinBullet->GetHitPos();
			m_pPinBullet->Uninit();
			m_pPinBullet = NULL;
		}

		if (m_PinPos != D3DXVECTOR3(0.0f, 0.0f, 0.0f))
		{// 弾が当たったとき
			if (m_pPin == NULL)
			{// ピンが生成されていない場合
			 // ピンの配置
				m_pPin = m_pPin->Create(D3DXVECTOR3(m_PinPos.x, 30.0f, m_PinPos.z));
				m_bPin = true;
			}
			else
			{// ピンが生成されている場合
			 // ピンの位置更新
				m_pPin->GetPos() = D3DXVECTOR3(m_PinPos.x, 30.0f, m_PinPos.z);
			}
			m_nPinLife = PIN_LIFE;
		}
	}

	if (m_bPin)
	{// ピンが使用されている場合
	 // ピンの更新
		m_nPinLife--;

		if (m_nPinLife <= 0)
		{// 一定時間経過でピンを削除
			m_PinPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			m_bPin = false;
			m_pPin->Uninit();
			m_pPin = NULL;
		}
	}

	//===================================
	// AIのピン配置
	//===================================
	CScene *pAIScene = CScene::GetSceneTop(AI_PRIORITY);
	CScene *pAISceneNext = NULL;
	int nCntAIMax = 0;

	while (pAIScene != NULL)
	{// NULLになるまでループ
		pAISceneNext = pAIScene->GetSceneNext();
		CScene::OBJTYPE objType = pAIScene->GetObjType();

		if (objType == CScene::OBJTYPE_AI)
		{// AIオブジェクトのとき
			CAIMecha *pAI = (CAIMecha*)pAIScene;

			if (m_nTeam == pAI->GetTeam() && pAI->GetPlayer() == this)
			{// 自分のAIとき
				// AIのピン情報を取得する
				m_AIPinPos[nCntAIMax] = pAI->GetPinPos();
				m_bAIPin[nCntAIMax] = pAI->GetPinUse();

				if (m_pAIPin[nCntAIMax] != NULL)
				{// ピンが生成されている場合
					if (m_nAIPinLife[nCntAIMax] <= 0)
					{// 一定時間経過でピンを削除
						pAI->GetPinUse() = false;
						m_bAIPin[nCntAIMax] = false;
						m_pAIPin[nCntAIMax]->Uninit();
						m_pAIPin[nCntAIMax] = NULL;
					}
				}
				nCntAIMax++;
			}
		}

		// 次のオブジェクトを見る
		pAIScene = pAISceneNext;
	}

	for (int nCntAI = 0; nCntAI < AI_MAX; nCntAI++)
	{// AI機の数だけ回る
		if (m_bAIPin[nCntAI])
		{// AIのピンが使われている場合
			if (m_pAIPin[nCntAI] == NULL)
			{// ピンが生成されていない場合
			 // ピンの配置
				m_nAIPinLife[nCntAI] = PIN_LIFE;
				m_pAIPin[nCntAI] = m_pAIPin[nCntAI]->Create(m_AIPinPos[nCntAI] + D3DXVECTOR3(0.0f, 70.0f, 0.0f));
				m_pAIPin[nCntAI]->GetTexture() = CTexture::GetTexture(CTexture::TEXTURE_ENEMY_PIN);
				m_bAIPin[nCntAI] = true;
			}
			else
			{// ピンが生成されている場合
			 // ピンの更新
				m_nAIPinLife[nCntAI]--;
				m_pAIPin[nCntAI]->GetPos() = m_AIPinPos[nCntAI] + D3DXVECTOR3(0.0f, 70.0f, 0.0f);
			}
		}
	}

	//===================================
	// 味方プレイヤーのピン配置
	//===================================
	CScene *pAllyPlayerScene = CScene::GetSceneTop(PLAYER_PRIORITY);
	CScene *pAllyPlayerSceneNext = NULL;
	D3DXVECTOR3 AllyPlayerPinPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	while (pAllyPlayerScene != NULL)
	{// NULLになるまでループ
		pAllyPlayerSceneNext = pAllyPlayerScene->GetSceneNext();
		CScene::OBJTYPE objType = pAllyPlayerScene->GetObjType();

		if (objType == CScene::OBJTYPE_PLAYER)
		{// プレイヤーオブジェクトのとき
			CPlayer *pPlayer = (CPlayer*)pAllyPlayerScene;
			if (m_nTeam == pPlayer->GetTeam() && pPlayer != this)
			{// チームが同じで自分自身じゃないとき

			 // 味方のピン情報を取得する
				m_bAllyPin = pPlayer->GetPinUse();
				AllyPlayerPinPos = pPlayer->GetPinPos();
				m_AllyPosPinPos = pPlayer->GetPos();

				if (m_bAllyPin)
				{// 味方のピンが使われている場合
					if (m_pAllyPin != NULL)
					{// ピンが生成されている場合
						if (m_nPinLife <= 0)
						{// 一定時間経過でピンを削除
							pPlayer->GetPinUse() = false;
							m_bAllyPin = false;
							m_pAllyPin->Uninit();
							m_pAllyPin = NULL;
						}
					}
				}
			}
		}

		// 次のオブジェクトを見る
		pAllyPlayerScene = pAllyPlayerSceneNext;
	}

	if (m_bAllyPin)
	{// 味方のピンが使われている場合
		if (m_pAllyPin == NULL)
		{// 味方のピンが生成されていない場合
		 // ピンの配置
			m_nAllyPinLife = PIN_LIFE;
			m_pAllyPin = m_pAllyPin->Create(AllyPlayerPinPos + D3DXVECTOR3(0.0f, 70.0f, 0.0f));
			m_pAllyPin->GetTexture() = CTexture::GetTexture(CTexture::TEXTURE_ENEMY_PIN);
		}
		else
		{// 味方のピンが生成されている場合
		 // ピンの更新
			m_nPinLife--;
			m_pAllyPin->GetPos() = AllyPlayerPinPos + D3DXVECTOR3(0.0f, 70.0f, 0.0f);
		}
	}

	if (m_pAllyPosPin == NULL)
	{// 味方の位置ピンが生成されていない場合
	 // ピンの配置
		m_pAllyPosPin = m_pAllyPosPin->Create(m_AllyPosPinPos + D3DXVECTOR3(0.0f, 70.0f, 0.0f));
		m_pAllyPosPin->GetTexture() = CTexture::GetTexture(CTexture::TEXTURE_FRIEND_PIN);
	}
	else
	{// 味方の位置ピンが生成されている場合
	 // ピンの更新
		m_pAllyPosPin->GetPos() = m_AllyPosPinPos + D3DXVECTOR3(0.0f, 70.0f, 0.0f);
	}

	//===================================
	// 味方AIのピン配置
	//===================================
	CScene *pAllyAIScene = CScene::GetSceneTop(AI_PRIORITY);
	CScene *pAllyAISceneNext = NULL;
	int nCntAllyAIMax = 0;

	while (pAllyAIScene != NULL)
	{// NULLになるまでループ
		pAllyAISceneNext = pAllyAIScene->GetSceneNext();
		CScene::OBJTYPE objType = pAllyAIScene->GetObjType();

		if (objType == CScene::OBJTYPE_AI)
		{// AIオブジェクトのとき
			CAIMecha *pAllyAI = (CAIMecha*)pAllyAIScene;

			if (m_nTeam == pAllyAI->GetTeam() && pAllyAI->GetPlayer() != this)
			{// チームが同じで自分のAIじゃないとき
			 // 仲間のピン情報を取得する
				m_AllyAIPinPos[nCntAllyAIMax] = pAllyAI->GetPinPos();
				m_bAllyAIPin[nCntAllyAIMax] = pAllyAI->GetPinUse();

				if (m_pAllyAIPin[nCntAllyAIMax] != NULL)
				{// ピンが生成されている場合
					if (m_nAllyAIPinLife[nCntAllyAIMax] <= 0)
					{// 一定時間経過でピンを削除
						pAllyAI->GetPinUse() = false;
						m_bAllyAIPin[nCntAllyAIMax] = false;
						m_pAllyAIPin[nCntAllyAIMax]->Uninit();
						m_pAllyAIPin[nCntAllyAIMax] = NULL;
					}
				}
				nCntAllyAIMax++;
			}
		}

		// 次のオブジェクトを見る
		pAllyAIScene = pAllyAISceneNext;
	}

	for (int nCntAllyAI = 0; nCntAllyAI < AI_MAX; nCntAllyAI++)
	{// 味方AI機の数だけ回る
		if (m_bAllyAIPin[nCntAllyAI])
		{// 味方AIのピンが使われている場合
			if (m_pAllyAIPin[nCntAllyAI] == NULL)
			{// ピンが生成されていない場合
			 // ピンの配置
				m_nAllyAIPinLife[nCntAllyAI] = PIN_LIFE;
				m_pAllyAIPin[nCntAllyAI] = m_pAllyAIPin[nCntAllyAI]->Create(m_AllyAIPinPos[nCntAllyAI] + D3DXVECTOR3(0.0f, 70.0f, 0.0f));
				m_pAllyAIPin[nCntAllyAI]->GetTexture() = CTexture::GetTexture(CTexture::TEXTURE_ENEMY_PIN);
			}
			else
			{// ピンが生成されている場合
			 // ピンの更新
				m_nAllyAIPinLife[nCntAllyAI]--;
				m_pAllyAIPin[nCntAllyAI]->GetPos() = m_AllyAIPinPos[nCntAllyAI] + D3DXVECTOR3(0.0f, 70.0f, 0.0f);
			}
		}
	}
}

//=============================================================================
//	マルチ時ピン関係更新処理
//=============================================================================
void CPlayer::PinUpdateMulti()
{
	CInputMouse *pMouse = CManager::GetInputMouse();	// マウスの入力を取得

	//===================================
	// 自分のピン配置
	//===================================
	if (pMouse->GetTrigger(CInputMouse::DIMS_BUTTON_2) == true || pMouse->GetMouseAxisZ() >= 120.0f || pMouse->GetMouseAxisZ() <= -120.0f)
	{// ホイールクリックorロール
		// カメラの角度と注視点を取得
		D3DXVECTOR3 rotCamera = CManager::GetCamera()->GetRot();
		D3DXVECTOR3 posR = CManager::GetCamera()->GetPosR();

		// レティクル（目的の位置）の取得
		D3DXVECTOR3 posReticle = m_pReticle->GetPos();

		// 射出口の位置の取得
		D3DXMATRIX mtxCanon = m_pModel[2]->GetMtxWorld();
		D3DXVECTOR3 posCanon = D3DXVECTOR3(mtxCanon._41, mtxCanon._42, mtxCanon._43) + D3DXVECTOR3(sinf(rotCamera.y) * 30.0f, cosf(rotCamera.x) * 30.0f, cosf(rotCamera.y) * 30.0f);

		// 水平方向の角度の計算
		m_pAngle[0] = atan2f(posReticle.x - posCanon.x, posReticle.z - posCanon.z);
		m_pAngleV[0] = rotCamera.x;

		if (m_pPinBullet != NULL)
		{// 前回の弾の破棄
			m_pPinBullet->Uninit();
			m_pPinBullet = NULL;
		}

		// 弾の生成
		m_pPinBullet = m_pPinBullet->Create(posCanon, m_pAngle[0], m_pAngleV[0], m_nTeam, this);
	}

	if (m_pPinBullet != NULL)
	{// 判定用の弾がNULLじゃないとき
		if (m_pPinBullet->BulletCollision())
		{// 弾が当たったとき
			// ピン立て用弾の判定チェック
			m_PinPos = m_pPinBullet->GetHitPos();
			m_pPinBullet->Uninit();
			m_pPinBullet = NULL;
		}

		if (m_PinPos != D3DXVECTOR3(0.0f, 0.0f, 0.0f))
		{// 弾が当たったとき
			if (m_pPin == NULL)
			{// ピンが生成されていない場合
				// ピンの配置
				m_pPin = m_pPin->Create(D3DXVECTOR3(m_PinPos.x, 30.0f, m_PinPos.z));
				m_bPin = true;
			}
			else
			{// ピンが生成されている場合
				// ピンの位置更新
				m_pPin->GetPos() = D3DXVECTOR3(m_PinPos.x, 30.0f, m_PinPos.z);
			}
			m_nPinLife = PIN_LIFE;
		}
	}

	if (m_bPin)
	{// ピンが使用されている場合
	 // ピンの更新
		m_nPinLife--;

		if (m_nPinLife <= 0)
		{// 一定時間経過でピンを削除
			m_PinPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			m_bPin = false;
			m_pPin->Uninit();
			m_pPin = NULL;
		}
	}

	//===================================
	// 自分のAIのピン配置
	//===================================
	CScene *pAIScene = CScene::GetSceneTop(AI_PRIORITY);
	CScene *pAISceneNext = NULL;
	int nCntAIMax = 0;

	while (pAIScene != NULL)
	{// NULLになるまでループ
		pAISceneNext = pAIScene->GetSceneNext();
		CScene::OBJTYPE objType = pAIScene->GetObjType();

		if (objType == CScene::OBJTYPE_AI)
		{// AIオブジェクトのとき
			CAIMecha *pAI = (CAIMecha*)pAIScene;

			if (m_nTeam == pAI->GetTeam() && pAI->GetPlayer() == this)
			{// 自分のAIの時
			 // 仲間のピン情報を取得する
				m_AIPinPos[nCntAIMax] = pAI->GetPinPos();
				m_bAIPin[nCntAIMax] = pAI->GetPinUse();

				if (m_pAIPin[nCntAIMax] != NULL)
				{// ピンが生成されている場合
					if (m_nAIPinLife[nCntAIMax] <= 0)
					{// 一定時間経過でピンを削除
						pAI->GetPinUse() = false;
						m_bAIPin[nCntAIMax] = false;
						m_pAIPin[nCntAIMax]->Uninit();
						m_pAIPin[nCntAIMax] = NULL;
					}
				}
				nCntAIMax++;
			}
		}

		// 次のオブジェクトを見る
		pAIScene = pAISceneNext;
	}

	for (int nCntAI = 0; nCntAI < AI_MAX; nCntAI++)
	{// AI機の数だけ回る
		if (m_bAIPin[nCntAI])
		{// AIのピンが使われている場合
			if (m_pAIPin[nCntAI] == NULL)
			{// ピンが生成されていない場合
			 // ピンの配置
				m_nAIPinLife[nCntAI] = PIN_LIFE;
				m_pAIPin[nCntAI] = m_pAIPin[nCntAI]->Create(m_AIPinPos[nCntAI] + D3DXVECTOR3(0.0f, 70.0f, 0.0f));
				m_pAIPin[nCntAI]->GetTexture() = CTexture::GetTexture(CTexture::TEXTURE_ENEMY_PIN);
				m_bAIPin[nCntAI] = true;
			}
			else
			{// ピンが生成されている場合
			 // ピンの更新
				m_nAIPinLife[nCntAI]--;
				m_pAIPin[nCntAI]->GetPos() = m_AIPinPos[nCntAI] + D3DXVECTOR3(0.0f, 70.0f, 0.0f);
			}
		}
	}

	//===================================
	// 味方プレイヤーのピン配置
	//===================================
	if (m_bAllyPin)
	{// 仲間のピンが使われている場合
		if (m_pAllyPin == NULL)
		{// ピンが生成されていない場合
		 // ピンの配置
			m_pAllyPin = m_pAllyPin->Create(D3DXVECTOR3(m_AllyPinPos.x, 30.0f, m_AllyPinPos.z));
			m_pAllyPin->GetTexture() = CTexture::GetTexture(CTexture::TEXTURE_GOAL_PIN);
		}
		else
		{// ピンが生成されている場合
		 // ピンの更新
			m_nAllyPinLife--;
			m_pAllyPin->GetPos() = D3DXVECTOR3(m_AllyPinPos.x, 30.0f, m_AllyPinPos.z);

			if (m_nAllyPinLife <= 0)
			{// 一定時間経過でピンを削除
				m_bAllyPin = false;
				m_pAllyPin->Uninit();
				m_pAllyPin = NULL;
			}
		}
	}

	//===================================
	// 味方AIのピン配置
	//===================================
	for (int nCntAllyAI = 0; nCntAllyAI < AI_MAX; nCntAllyAI++)
	{// 味方AI機の数だけ回る
		if (m_bAllyAIPin[nCntAllyAI])
		{// 味方AIのピンが使われている場合
			if (m_pAllyAIPin[nCntAllyAI] == NULL)
			{// ピンが生成されていない場合
			 // ピンの配置
				m_pAllyAIPin[nCntAllyAI] = m_pAllyAIPin[nCntAllyAI]->Create(m_AllyAIPinPos[nCntAllyAI] + D3DXVECTOR3(0.0f, 70.0f, 0.0f));
				m_pAllyAIPin[nCntAllyAI]->GetTexture() = CTexture::GetTexture(CTexture::TEXTURE_ENEMY_PIN);
			}
			else
			{// ピンが生成されている場合
			 // ピンの更新
				m_nAllyAIPinLife[nCntAllyAI]--;
				m_pAllyAIPin[nCntAllyAI]->GetPos() = m_AllyAIPinPos[nCntAllyAI] + D3DXVECTOR3(0.0f, 70.0f, 0.0f);

				if (m_nAllyAIPinLife[nCntAllyAI] <= 0)
				{// 一定時間経過でピンを削除
					m_bAllyAIPin[nCntAllyAI] = false;
					m_pAllyAIPin[nCntAllyAI]->Uninit();
					m_pAllyAIPin[nCntAllyAI] = NULL;
				}
			}
		}
	}

	//===================================
	// 味方の位置ピン配置
	//===================================
	CScene *pAllyPlayerScene = CScene::GetSceneTop(PLAYER_PRIORITY);
	CScene *pAllyPlayerSceneNext = NULL;
	D3DXVECTOR3 AllyPlayerPinPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	while (pAllyPlayerScene != NULL)
	{// NULLになるまでループ
		pAllyPlayerSceneNext = pAllyPlayerScene->GetSceneNext();
		CScene::OBJTYPE objType = pAllyPlayerScene->GetObjType();

		if (objType == CScene::OBJTYPE_PLAYER)
		{// プレイヤーオブジェクトのとき
			CPlayer *pPlayer = (CPlayer*)pAllyPlayerScene;
			if (m_nTeam == pPlayer->GetTeam() && pPlayer != this)
			{// チームが同じで自分自身じゃないとき
				m_AllyPosPinPos = pPlayer->GetPos();
			}
		}

		// 次のオブジェクトを見る
		pAllyPlayerScene = pAllyPlayerSceneNext;
	}

	if (m_pAllyPosPin == NULL)
	{// 味方のピンが生成されていない場合
	 // ピンの配置
		m_pAllyPosPin = m_pAllyPosPin->Create(m_AllyPosPinPos + D3DXVECTOR3(0.0f, 70.0f, 0.0f));
		m_pAllyPosPin->GetTexture() = CTexture::GetTexture(CTexture::TEXTURE_FRIEND_PIN);
	}
	else
	{// 味方のピンが生成されている場合
	 // ピンの更新
		m_pAllyPosPin->GetPos() = m_AllyPosPinPos + D3DXVECTOR3(0.0f, 70.0f, 0.0f);
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

	// 前回の目標角度の保存
	m_rotDestOld = m_rotDest;

	for (int nCntSearch = 0; nCntSearch < MAX_SEARCH; nCntSearch++)
	{// 探索の更新処理
		m_pSearch[nCntSearch]->Update();
	}

	// 前回の発見状態の取得
	m_bFindOld = m_bFind;

	// 発見状態の取得
	m_bFind = Distance();

	// 現在地の更新
	if (m_bFindOld != m_bFind)
	{// 敵を見失った場合　または　敵を発見した場合
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
	}

	// 行動の設定
	if (m_bFind == true)
	{// 敵を発見している場合
		//戦闘時の移動処理
		BattleMovent();

		//戦闘の処理
		Battle();
	}
	else
	{
		// 自動移動処理
		CPlayer::AutoMove();
	}
	//CDebugProc::Print("m_bFind: %d", m_bFind);

	// リロード処理
	if (m_bReload == true)
	{// リロード状態の場合
		m_nCntReRoad++;		// カウンター加算

		if (m_nCntReRoad % m_nReload == 0)
		{// 設定時間経過でリロードを完了させる
			m_nRemBullet = m_nCapacity;	// 弾を装填
			m_bReload = false;			// リロードを完了させる
		}
	}

	//===========================================//
	//               収集データ参照              //
	//===========================================//
	float fMinLength = 100000, fLength = 100000;	// 差分系
	D3DXVECTOR3 total = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 収集したデータの合計値

	if (!m_bCollectSwitch)
	{// 1週目
		for (int nCntCollect = 0; nCntCollect < m_nCountCollect + 1; nCntCollect++)
		{// 収集したデータの数だけ回る
			total += m_collectionPos[nCntCollect];	// 収集データを合計する
		}
	}
	else
	{// 2週目以降
		for (int nCntCollect = 0; nCntCollect < COLLECTIONDATA_MAX; nCntCollect++)
		{// 収集できるデータの最大数だけ回る
			total += m_collectionPos[nCntCollect];	// 収集データを合計する
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

	// 平均値に最も近いノードを検索する
	for (int nCntNode = 0; nCntNode < m_pNodeData->GetLoadData().nodeMax; nCntNode++)
	{// ノードの数だけ回る
		// 差分を求める
		fLength =
			(m_pNodeData->GetLoadData().pos[nCntNode].x - m_totalCollectPos.x) *
			(m_pNodeData->GetLoadData().pos[nCntNode].x - m_totalCollectPos.x) +
			(m_pNodeData->GetLoadData().pos[nCntNode].z - m_totalCollectPos.z) *
			(m_pNodeData->GetLoadData().pos[nCntNode].z - m_totalCollectPos.z);

		if (fMinLength > fLength)
		{// 差分の最小値を求める
			fMinLength = fLength;
			m_nNearTotalCollectNumber = nCntNode;
		}
	}

	if (m_rotDest.y > D3DX_PI) { m_rotDest.y -= D3DX_PI * 2.0f; }
	if (m_rotDest.y < -D3DX_PI) { m_rotDest.y += D3DX_PI * 2.0f; }

	if (m_fRotDestUpper.y > D3DX_PI) { m_fRotDestUpper.y -= D3DX_PI * 2.0f; }
	if (m_fRotDestUpper.y < -D3DX_PI) { m_fRotDestUpper.y += D3DX_PI * 2.0f; }

	if (m_rotDest.y - m_fRotDestUpper.y > D3DX_PI || m_rotDest.y - m_fRotDestUpper.y < -D3DX_PI)
	{// 差分が1周以上ある場合
		m_fRotDestUpper.y -= (m_rotDest.y - m_fRotDestUpper.y) * (m_fSpeed * 0.03f);
	}
	else
	{
		// 目標方向へ向く
		m_fRotDestUpper.y += (m_rotDest.y - m_fRotDestUpper.y) * (m_fSpeed * 0.03f);
	}
}

//=============================================================================
//	自動移動処理
//=============================================================================
void CPlayer::AutoMove()
{
	CMotionManager::TYPE type = CMotionManager::TYPE_NEUTRAL;	// モーションの種類
	bool bMove = false;	// ボタン押下フラグ
	float fVigilanceRot[CONNECT_MAX];

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

		if (!m_bFind)
		{// 敵未発見時
			m_rotDest.y = atan2f(m_posDest.x - m_pos.x, m_posDest.z - m_pos.z);
		}
	}
	else if (m_nBreaktime < 0)
	{// 移動中
		bMove = true;
		m_nBreaktime = PLAYER_BREAKTIME;

		if (m_nPoint == m_nCountPoint)
		{// 最終目標地点に到着したら次の目的地を探す
			m_nBreaktime = PLAYER_FINALPOINT_BREAKTIME;
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

		for (int nCntConnect = 0; nCntConnect < m_pNodeData->GetLoadData().connectNum[m_nStartNode]; nCntConnect++)
		{// 繋がってるノードの数だけ回る
			fVigilanceRot[nCntConnect] = atan2f(m_pNodeData->GetLoadData().pos[m_pNodeData->GetLoadData().connectIndex[m_nStartNode][nCntConnect]].x - m_pos.x, m_pNodeData->GetLoadData().pos[m_pNodeData->GetLoadData().connectIndex[m_nStartNode][nCntConnect]].z - m_pos.z) + D3DX_PI;
		}

		if (!m_bFind && (m_nBreaktime == PLAYER_FINALPOINT_BREAKTIME - 1 || m_nBreaktime % (PLAYER_FINALPOINT_BREAKTIME / m_pNodeData->GetLoadData().connectNum[m_nStartNode]) == 0))
		{// 敵未発見時に繋がってるノードの方向すべてを見る
			m_rotDest.y = fVigilanceRot[m_nVigilanceCount] + D3DX_PI;

			m_nVigilanceCount++;

			if (m_nVigilanceCount >= m_pNodeData->GetLoadData().connectNum[m_nStartNode])
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
	for (int nCntNode = 0; nCntNode < m_pNodeData->GetLoadData().nodeMax; nCntNode++, nCntWeight++)
	{// ノードの数だけ回る
		weight[nCntWeight] = sqrt((m_pNodeData->GetLoadData().pos[m_nStartNode].x - m_pNodeData->GetLoadData().pos[nCntNode].x) * (m_pNodeData->GetLoadData().pos[m_nStartNode].x - m_pNodeData->GetLoadData().pos[nCntNode].x) + (m_pNodeData->GetLoadData().pos[m_nStartNode].z - m_pNodeData->GetLoadData().pos[nCntNode].z) * (m_pNodeData->GetLoadData().pos[m_nStartNode].z - m_pNodeData->GetLoadData().pos[nCntNode].z));
	}

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
	for (int nCntNode = path.size() - 1; nCntNode >= 0; nCntNode--)
	{
		if (m_nCountPoint < nCntNode)
		{// 最大値を代入
		 // 目的までの移動回数を取得する
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
	}
}