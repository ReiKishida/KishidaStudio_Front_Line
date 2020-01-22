//=============================================================================
//
// ゲーム処理 [game.cpp]
// Author : Takuto Ishida
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "game.h"
#include "camera.h"
#include "renderer.h"
#include "fade.h"
#include "input.h"
#include "manager.h"
#include "scene.h"
#include "camera.h"
#include "debugProc.h"
#include "result.h"
#include "sound.h"
#include "life.h"
#include "modelSet.h"
#include "player.h"
#include "meshField.h"
#include "motion.h"
#include "pause.h"
#include "meshField.h"
#include "number.h"
#include "bullet.h"
#include "button.h"
#include "mouseCursor.h"
#include "UI_Number.h"
#include "UI_Texture.h"
#include "texture.h"
#include <stdio.h>

#include "model.h"
#include "gauge.h"

#include "server.h"
#include "serverfunction.h"

#include "collisionSet.h"
#include "scene3D.h"
#include "menu.h"

#include "shadow.h"

#include "particle.h"

#include "AI.h"

#include "damageDirection.h"
#include "nodeDataFiler.h"

#include "matching.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define FIELD_MODEL_NAME	"data/MODEL/map_no_floor.x"
#define FLOOR_MODEL_NAME	"data/MODEL/floor.x"
#define SKY_MODEL_NAME		"data/MODEL/skydome.x"
#define NUMTEX_UV_X		(1)
#define NUMTEX_UV_Y		(3)
#define GAME_INI			"data/TEXT/game.ini"

#define GAME_SCRIPT			"SCRIPT"
#define GAME_END_SCRIPT		"END_SCRIPT"
#define GAME_BLUE			"BLUE"
#define GAME_BLUE_END		"BLUE_END"
#define GAME_RED			"RED"
#define GAME_RED_END		"RED_END"
#define GAME_SPAWN_A		"A"
#define GAME_SPAWN_B		"B"
#define GAME_SPAWN_C		"C"
#define GAME_SPAWN_D		"D"

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
CGame::STATE CGame::m_state = CGame::STATE_NONE;
int CGame::m_nCurStage = 0;
CPlayer *CGame::m_pPlayer[MAX_PLAYER_CONNECT] = {};
CMechaSelect::MECHATYPE CGame::m_aMechaType[MAX_PLAYER_CONNECT] = { CMechaSelect::MECHATYPE_EMPTY,CMechaSelect::MECHATYPE_EMPTY ,CMechaSelect::MECHATYPE_EMPTY ,CMechaSelect::MECHATYPE_EMPTY };
CDamageDirection *CGame::m_pDamageDirection = NULL;
CNodeDataFiler *CGame::m_pNodeFiler = NULL;			// マップデータクラスのポインタ変数

//=============================================================================
// コンストラクタ
//=============================================================================
CGame::CGame(int nPriority, CScene::OBJTYPE objType) : CScene(nPriority, objType)
{
	m_modeCounter = 0;
	m_pMouseCursor = NULL;
	m_pPause = NULL;
	m_bPause = false;
	m_pTimer = NULL;
	m_pButtonManager = NULL;
	m_part = PART_ACTION;
	m_pSky = NULL;
	m_pMouse = NULL;
	m_pField = NULL;
	m_nBlueLinkEnergy = 0;
	m_nRedLinkEnergy = 0;
	m_pFadeBG = NULL;
	m_pEndBG = NULL;
	m_pEndResultLogo = NULL;
	m_pEndMyTeamLogo = NULL;
	m_pFloor = NULL;

	for (int nCntConnect = 0; nCntConnect < MAX_PLAYER_CONNECT; nCntConnect++)
	{
		//m_bConnect[nCntConnect] = false;
		m_bPlayerDeath[nCntConnect] = false;
	}

	for (int nCntTeam = 0; nCntTeam < NUM_TEAM; nCntTeam++)
	{
		m_pEndScore[nCntTeam] = NULL;
		m_pEndTeamEmbrem[nCntTeam] = NULL;

		for (int nCntRespawn = 0; nCntRespawn < NUM_RESPAWN_POS; nCntRespawn++)
		{
			m_aRespawnPos[nCntTeam][nCntRespawn] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		}
	}

	for (int nCntConnect = 0; nCntConnect < 2; nCntConnect++)
	{
		for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER_CONNECT; nCntPlayer++)
		{
			m_bAIDeath[nCntConnect][nCntPlayer] = false;
		}
	}

	for (int nCntKill = 0; nCntKill < NUM_KILL_LOG; nCntKill++)
	{
		m_apKillLogBase[nCntKill] = NULL;
		m_bLog[nCntKill] = false;
		m_nKillIdx[nCntKill] = 0;
		m_nDeathIdx[nCntKill] = 0;
		m_nCntDrawLog[nCntKill] = 0;

		for (int nCntPlayer = 0; nCntPlayer < NUM_KILL_LOG_PLAYER; nCntPlayer++)
		{
			m_apKillLogPlayerIcon[nCntKill][nCntPlayer] = NULL;
			m_apKillLogPlayerIdx[nCntKill][nCntPlayer] = NULL;
		}
	}

	for (int nCntPlayer = 0; nCntPlayer < NUM_KILL_LOG_PLAYER; nCntPlayer++)
	{
		m_playerType[nCntPlayer] = TYPE_PLAYER;

	}

}

//=============================================================================
// デストラクタ
//=============================================================================
CGame::~CGame()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CGame::Init(void)
{
	m_nBlueLinkEnergy = MAX_LINKENERGY;
	m_nRedLinkEnergy = MAX_LINKENERGY;

	//リスポーン位置の読み込み
	LoadRespawnPos();

	// 読み込み
	CMotionManager::Load();

	m_pFloor = CModel::Create();
	m_pFloor->SetModel(FLOOR_MODEL_NAME, false);
	m_pFloor->SetPos(D3DXVECTOR3(-15.0f, 0.0f, 30.0f));

	m_pField = CModel::Create();
	m_pField->SetModel(FIELD_MODEL_NAME);
	m_pField->SetPos(D3DXVECTOR3(-15.0f, 0.0f, 30.0f));
	m_pField->SetShader();

	m_pSky = CModel::Create();
	m_pSky->SetModel(SKY_MODEL_NAME, false);
	m_pSky->SetPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	//=====================================================================================
	// マップデータファイラーの生成
	//=====================================================================================
	if (m_pNodeFiler == NULL)
	{// NULLチェック
		m_pNodeFiler = CNodeDataFiler::Create();
	}

	// プレイヤーの生成
	CreatePlayer();

	//CShadow::Create();

	// 弾の当たり判定クラスの生成
	CBulletCollision::Create();

	// マップの当たり判定の読み込み
	CCollision::Load();

	////****************************************
	//// 2DUI生成（フレーム）
	////****************************************
	//// 武器フレーム
	//CUI_TEXTURE::Create(D3DXVECTOR3(1100.0f, 650.0f, 0.0f), 350.0f, 120.0f, CUI_TEXTURE::UIFLAME_WEAPON);		// 武器

	//																											// プレイヤー体力フレーム
	//CUI_TEXTURE::Create(D3DXVECTOR3(180.0f, 650.0f, 0.0f), 350.0f, 125.0f, CUI_TEXTURE::UIFLAME_PLAYER_HP);	// 胴体

	//																										// AIチケット数フレーム
	//CUI_TEXTURE::Create(D3DXVECTOR3(125.0f, 530.0f, 0.0f), 230.0f, 100.0f, CUI_TEXTURE::UIFLAME_DRONE);		// AI01
	//CUI_TEXTURE::Create(D3DXVECTOR3(125.0f, 420.0f, 0.0f), 230.0f, 100.0f, CUI_TEXTURE::UIFLAME_WORKER);	// AI02

	//																										// チームのチケット数フレーム
	//CUI_TEXTURE::Create(D3DXVECTOR3(80.0f, 40.0f, 0.0f), 120.0f, 50.0f, CUI_TEXTURE::UIFLAME_TEAM_BLUE);	// BLUE
	//CUI_TEXTURE::Create(D3DXVECTOR3(80.0f, 100.0f, 0.0f), 120.0f, 50.0f, CUI_TEXTURE::UIFLAME_TEAM_RED);	// RED

	//// パート切り替えテクスチャ
	////CUI_TEXTURE::Create(D3DXVECTOR3(125.0f, 50.0f, 0.0f), 200.0f, 90.0f, CUI_TEXTURE::UIFLAME_STRATEGY_PART);	// ストラテジーパート
	////CUI_TEXTURE::Create(D3DXVECTOR3(165.0f, 90.0f, 0.0f), 200.0f, 90.0f, CUI_TEXTURE::UIFLAME_ACTION_PART);		// アクションパート

	//// パート切り替え（チェンジ）
	////CUI_TEXTURE::Create(D3DXVECTOR3(50.0f, 135.0f, 0.0f), 100.0f, 110.0f, CUI_TEXTURE::UIFLAME_CHANGE);

	////****************************************
	//// UI生成（数字）
	////****************************************
	//CUI_NUMBER::Create(D3DXVECTOR3(370.0f, 40.0f, 0.0f), 430.0f, 30.0f, 0.0f, CUI_NUMBER::UI_NUMTYPE_BLUE, 0, NUMTEX_UV_X, NUMTEX_UV_Y);							// BLUEチームチケット
	//CUI_NUMBER::Create(D3DXVECTOR3(370.0f, 100.0f, 0.0f), 430.0f, 30.0f, 0.0f, CUI_NUMBER::UI_NUMTYPE_RED, 0, NUMTEX_UV_X, NUMTEX_UV_Y);							// REDチームチケット
	//CUI_NUMBER::Create(D3DXVECTOR3(970.0f, 660.0f, 0.0f), 150.0f, 90.0f, 60.0f, CUI_NUMBER::UI_NUMTYPE_REMAINBULLET, 0, NUMTEX_UV_X, NUMTEX_UV_Y);		// 残弾
	//CUI_NUMBER::Create(D3DXVECTOR3(110.0f, 530.0f, 0.0f), 130.0f, 70.0f, 50.0f, CUI_NUMBER::UI_NUMTYPE_DRONE_HP, 0, NUMTEX_UV_X, NUMTEX_UV_Y);				// ドローンライフ
	//CUI_NUMBER::Create(D3DXVECTOR3(110.0f, 420.0f, 0.0f), 130.0f, 70.0f, 50.0f, CUI_NUMBER::UI_NUMTYPE_WORKER_HP, 0, NUMTEX_UV_X, NUMTEX_UV_Y);			// ワーカーライフ
	//CUI_NUMBER::Create(D3DXVECTOR3(195.0f, 650.0f, 0.0f), 170.0f, 110.0f, 70.0f, CUI_NUMBER::UI_NUMTYPE_PLAYER_HP, 0, NUMTEX_UV_X, NUMTEX_UV_Y);			// プレイヤーライフ

	if (NULL == m_pDamageDirection)
	{
		m_pDamageDirection = CDamageDirection::Create();
	}

	m_state = STATE_STARTING;
	CManager::GetCamera()->Init();
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CGame::Uninit(void)
{
	// データの破棄
	CMotionManager::Unload();
	CModelSetManager::Unload();
	//CEnemy::Unload();

	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER_CONNECT; nCntPlayer++)
	{
		if (NULL != m_pPlayer[nCntPlayer])
		{// プレイヤークラスの破棄
			m_pPlayer[nCntPlayer]->Uninit();
			m_pPlayer[nCntPlayer] = NULL;
		}
	}

	if (NULL != m_pButtonManager)
	{// ボタン管理クラスの破棄
		m_pButtonManager->Uninit();
		m_pButtonManager = NULL;
	}

	if (NULL != m_pField)
	{// 地面の破棄
		m_pField->Uninit();
		delete m_pField;
		m_pField = NULL;
	}

	if (NULL != m_pFloor)
	{// 地面の破棄
		m_pFloor->Uninit();
		delete m_pFloor;
		m_pFloor = NULL;
	}

	if (NULL != m_pSky)
	{// スカイドームの破棄
		m_pSky->Uninit();
		delete m_pSky;
		m_pSky = NULL;
	}

	if (NULL != m_pMouseCursor)
	{// マウスカーソルの破棄
		m_pMouseCursor->Uninit();
		m_pMouseCursor = NULL;
	}

	if (NULL != m_pMouse)
	{// マウスカーソルの破棄
		m_pMouse->Uninit();
		m_pMouse = NULL;
	}

	if (NULL != m_pDamageDirection)
	{// 攻撃を受けた方向表示の破棄
		m_pDamageDirection->Uninit();
		m_pDamageDirection = NULL;
	}

	if (m_pNodeFiler != NULL)
	{// マップデータファイラーの破棄
		m_pNodeFiler->Uninit();
		m_pNodeFiler = NULL;
	}

	// オブジェクトを破棄
	CScene::Release();
}

//=============================================================================
// ゲーム更新処理
//=============================================================================
void CGame::Update(void)
{
	CInputKeyboard	*pKeyboard = CManager::GetInputKeyboard();	// キーボードの取得
	CXInput			*pXInput = CManager::GetXInput();			// XInputの取得
	CSound *pSound = CManager::GetSound();						// サウンドの取得
	CInputMouse *pMouse = CManager::GetInputMouse();
	CDirectInput *pDirectInput = CManager::GetDirectInput();	//DirectInputの取得
	CDirectInput::GamePad *DirectInputStick = pDirectInput->GetgamePadStick();

	if (CMenu::GetMode() == CMenu::MODE_MULTI)
	{
		if (CManager::GetClient() != NULL)
		{
			if (CManager::GetClient()->GetGameMode() == CClient::GAME_MODE_PLAYER)
			{
				/*if (CManager::GetClient()->GetPlayerIdx() == 0)
				{
					if (m_state == STATE_NORMAL)
					{
						m_state = STATE_END;
					}
				}*/

				//必要な情報を書き込む処理
				PrintData();
			}
			//情報を読み取る処理
			ReadMessage();

			if (m_pPlayer[CManager::GetClient()->GetPlayerIdx()]->GetRespawn() == CPlayer::RESPAWN_NONE)
			{
				//パートの切り替え処理
				SwicthPart();
			}
		}
	}
	else
	{
		if (m_pPlayer[0]->GetRespawn() == CPlayer::RESPAWN_NONE)
		{
			//パートの切り替え処理
			SwicthPart();
		}
	}

#ifdef _DEBUG
	if (pKeyboard->GetTrigger(DIK_O))
	{
		CFade::Create(CManager::MODE_MENU);
	}
#endif

	if (NULL != m_pMouse)
		CDebugProc::Print("マウスカーソル：%.2f %.2f", m_pMouse->GetPos().x, m_pMouse->GetPos().z);

	// フェードの取得
	CFade::FADE fade = CFade::GetFade();

#ifdef _DEBUG
	if (fade == CFade::FADE_NONE)
	{// フェード中はポーズにできなくする
		if (pKeyboard->GetTrigger(DIK_RETURN))
		{
			//CFade::Create(CManager::MODE_RESULT);
		}

		if (pKeyboard->GetTrigger(DIK_P) || pXInput->GetTrigger(0, CXInput::XIJS_BUTTON_4))
		{// ポーズON/OFF
			m_bPause = m_bPause ? false : true;

			if (m_bPause)
			{// ポーズになったとき
				if (m_pPause == NULL)
				{
					//pSound->PlaySound(CSound::SOUND_LABEL_PAUSEON);
					m_pPause = CPause::Create();	// ポーズクラスを生成
				}
			}
		}

		if (!m_bPause)
		{// ポーズを閉じるとき
			if (m_pPause != NULL)
			{// ポーズでなくなったら破棄
				m_pPause->Uninit();
				m_pPause = NULL;
			}
		}
	}
#endif // !_DEBUG

	//キルログの生成処理
	CreateKillLog();

	//キルログの更新処理
	UpdateKillLog();

	//状態別の処理
	switch (m_state)
	{
	case STATE_STARTING:
		break;
	case STATE_STARTING_FADE_OUT:
		StartingFadeOutUpdate();
		break;
	case STATE_STARTING_FADE_IN:
		StartingFadeInUpdate();
		break;
	case STATE_NORMAL:
		NomalUpdate();
		break;
	case STATE_END:
		EndUpdate();
	break;
	case STATE_END_FADE_OUT:
		EndFadeOutUpdate();
		break;
	case STATE_FADE:
		FadeUpdate();
		break;
	}

	CDebugProc::Print("ゲーム");
}

//=============================================================================
// ゲーム描画処理
//=============================================================================
void CGame::Draw(void)
{
	if (NULL != m_pFloor)
	{// 地面の描画
		m_pFloor->Draw();
	}

	if (NULL != m_pField)
	{// 地面の描画
		m_pField->Draw();
	}

	// デバイスの取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	// ライティングOFF
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	if (NULL != m_pSky)
	{// スカイドームの描画
		m_pSky->Draw();
	}

	// ライティングON
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//=============================================================================
// 開始前のフェードアウトの更新処理
//=============================================================================
void CGame::StartingFadeOutUpdate(void)
{
	if (m_pFadeBG == NULL)
	{//NULLの場合
		//背景の生成
		m_pFadeBG = CUI_TEXTURE::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), SCREEN_WIDTH, SCREEN_HEIGHT, CUI_TEXTURE::UIFLAME_NONE);
		m_pFadeBG->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));
	}
	else
	{//NULLではない場合
		//色を取得し透明度を加算
		D3DXCOLOR col = m_pFadeBG->GetColor();
		col.a += 0.01f;

		//色の設置処理
		m_pFadeBG->SetColor(col);

		if (col.a >= 1.0f)
		{//透明度が１以上の場合
			//開始前のフェードインにする
			m_state = STATE_STARTING_FADE_IN;
		}
	}
}

//=============================================================================
// 開始前のフェードインの更新処理
//=============================================================================
void CGame::StartingFadeInUpdate(void)
{
	if (m_pFadeBG != NULL)
	{//NULLではない場合
	 //色を取得し透明度を減算
		D3DXCOLOR col = m_pFadeBG->GetColor();
		col.a -= 0.01f;

		//色の設置処理
		m_pFadeBG->SetColor(col);
		if (col.a <= 0.0f)
		{//透明度が０以下の場合
		 //通常状態にする
			m_state = STATE_NORMAL;

			//背景の開放
			if (m_pFadeBG != NULL)
			{
				m_pFadeBG->Uninit();
				m_pFadeBG = NULL;
			}

			//****************************************
			// 2DUI生成（フレーム）
			//****************************************
			// 武器フレーム
			CUI_TEXTURE::Create(D3DXVECTOR3(1100.0f, 650.0f, 0.0f), 350.0f, 120.0f, CUI_TEXTURE::UIFLAME_WEAPON);		// 武器

																														// プレイヤー体力フレーム
			CUI_TEXTURE::Create(D3DXVECTOR3(180.0f, 650.0f, 0.0f), 350.0f, 125.0f, CUI_TEXTURE::UIFLAME_PLAYER_HP);	// 胴体

																													// AIチケット数フレーム
			CUI_TEXTURE::Create(D3DXVECTOR3(125.0f, 530.0f, 0.0f), 230.0f, 100.0f, CUI_TEXTURE::UIFLAME_DRONE);		// AI01
			CUI_TEXTURE::Create(D3DXVECTOR3(125.0f, 420.0f, 0.0f), 230.0f, 100.0f, CUI_TEXTURE::UIFLAME_WORKER);	// AI02

																													// チームのチケット数フレーム
			CUI_TEXTURE::Create(D3DXVECTOR3(80.0f, 40.0f, 0.0f), 120.0f, 50.0f, CUI_TEXTURE::UIFLAME_TEAM_BLUE);	// BLUE
			CUI_TEXTURE::Create(D3DXVECTOR3(80.0f, 100.0f, 0.0f), 120.0f, 50.0f, CUI_TEXTURE::UIFLAME_TEAM_RED);	// RED

																													//****************************************
																													// UI生成（数字）
																													//****************************************
			CUI_NUMBER::Create(D3DXVECTOR3(370.0f, 40.0f, 0.0f), 430.0f, 30.0f, 0.0f, CUI_NUMBER::UI_NUMTYPE_BLUE, 0, NUMTEX_UV_X, NUMTEX_UV_Y);							// BLUEチームチケット
			CUI_NUMBER::Create(D3DXVECTOR3(370.0f, 100.0f, 0.0f), 430.0f, 30.0f, 0.0f, CUI_NUMBER::UI_NUMTYPE_RED, 0, NUMTEX_UV_X, NUMTEX_UV_Y);							// REDチームチケット
			CUI_NUMBER::Create(D3DXVECTOR3(1120.0f, 660.0f, 0.0f), 150.0f, 90.0f, 60.0f, CUI_NUMBER::UI_NUMTYPE_REMAINBULLET, 0, NUMTEX_UV_X, NUMTEX_UV_Y);		// 残弾
			CUI_NUMBER::Create(D3DXVECTOR3(260.0f, 530.0f, 0.0f), 100.0f, 60.0f, 40.0f, CUI_NUMBER::UI_NUMTYPE_DRONE_HP, 0, NUMTEX_UV_X, NUMTEX_UV_Y);				// ドローンライフ
			CUI_NUMBER::Create(D3DXVECTOR3(260.0f, 420.0f, 0.0f), 100.0f, 60.0f, 40.0f, CUI_NUMBER::UI_NUMTYPE_WORKER_HP, 0, NUMTEX_UV_X, NUMTEX_UV_Y);			// ワーカーライフ
			CUI_NUMBER::Create(D3DXVECTOR3(385.0f, 650.0f, 0.0f), 150.0f, 100.0f, 60.0f, CUI_NUMBER::UI_NUMTYPE_PLAYER_HP, 0, NUMTEX_UV_X, NUMTEX_UV_Y);			// プレイヤーライフ
		}
	}
}

//=============================================================================
// 通常の更新処理
//=============================================================================
void CGame::NomalUpdate (void)
{
	if (CMenu::GetMode() == CMenu::MODE_MULTI)
	{//マルチプレイの場合
		if (CManager::GetClient() != NULL)
		{//NULLではない場合
			if (CManager::GetClient()->GetPlayerIdx() == 0)
			{//プレイヤー番号が０の場合
				if (m_nBlueLinkEnergy <= 0)
				{//ブルーチームのリンクエネルギーが０以下の場合
					//終了状態にする
					m_state = STATE_END;
					//レッドチームの勝利にする
					CResult::SetTeamWin(CResult::TEAM_WIN_RED);
				}
				else if (m_nRedLinkEnergy <= 0)
				{//レッドチームのリンクエネルギーが0以下の場合
					//終了状態にする
					m_state = STATE_END;
					//ブルーチームの勝利にする
					CResult::SetTeamWin(CResult::TEAM_WIN_BLUE);
				}
			}
		}
	}
	else if (CMenu::GetMode() == CMenu::MODE_SINGLE)
	{//シングルプレイの場合
		if (m_nBlueLinkEnergy <= 0)
		{//ブルーチームのリンクエネルギーが０以下の場合
			//終了状態にする
			m_state = STATE_END;
			//レッドチームの勝利にする
			CResult::SetTeamWin(CResult::TEAM_WIN_RED);
		}
		else if (m_nRedLinkEnergy <= 0)
		{//レッドチームのリンクエネルギーが0以下の場合
			//終了状態にする
			m_state = STATE_END;
			//ブルーチームの勝利にする
			CResult::SetTeamWin(CResult::TEAM_WIN_BLUE);
		}
	}
	CDebugProc::Print("接続状況 ");
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER_CONNECT; nCntPlayer++)
	{
		switch (CMatching::GetConnect(nCntPlayer))
		{
		case true:
			CDebugProc::Print("1");
			break;
		case false:
			CDebugProc::Print("0");
			break;
		}
	}
}

//=============================================================================
// 終了の更新処理
//=============================================================================
void CGame::EndUpdate(void)
{
	//UIの破棄
	CScene::UIUninit();

	//バグ対策用にプレイヤーＵＩの破棄処理
	if (CMenu::MODE_MULTI == CMenu::GetMode())
	{
		if (CManager::GetClient() != NULL)
		{
			m_pPlayer[CManager::GetClient()->GetPlayerIdx()]->ReleasePlayerUI();
		}
	}
	else if (CMenu::MODE_SINGLE == CMenu::GetMode())
	{
		m_pPlayer[0]->ReleasePlayerUI();
	}

	if (m_pEndBG == NULL)
	{//NULLの場合
		//背景の生成
		m_pEndBG = CUI_TEXTURE::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), SCREEN_WIDTH, SCREEN_HEIGHT, CUI_TEXTURE::UIFLAME_NONE);

		if (CResult::GetTeamWin() == CResult::TEAM_WIN_BLUE)
		{//ブルーチームの勝利の場合
			//背景の色を青にする
			m_pEndBG->SetColor(D3DXCOLOR(0.0f, 0.0f, 1.0f, 0.0f));
		}
		else if (CResult::GetTeamWin() == CResult::TEAM_WIN_RED)
		{//レッドチームの勝利の場合
			//背景の色を赤にする
			m_pEndBG->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.0f));
		}
		//終了のフェードアウトにする
		m_state = STATE_END_FADE_OUT;
	}
}

//=============================================================================
// 終了のフェードアウトの更新処理
//=============================================================================
void CGame::EndFadeOutUpdate(void)
{
	if (m_pEndBG != NULL)
	{//NULLではない場合
		//色を取得し透明度を加算
		D3DXCOLOR col = m_pEndBG->GetColor();
		col.a += 0.01f;

		//色の設置処理
		m_pEndBG->SetColor(col);

		if (col.a >= 0.5f)
		{//透明度が0.5以上の場合
			//フェード状態にする
			m_state = STATE_FADE;

			//結果表示に必要なUIを生成

			if (m_pEndScore[0] == NULL)
			{//NULLの場合
				//ブルーチームのスコアを生成
				m_pEndScore[0] = CUI_NUMBER::Create(D3DXVECTOR3(515.0f, 500.0f, 0.0f), 150.0f, 150.0f, 75.0f, CUI_NUMBER::UI_NUMTYPE_NONE, 0, 1, 3);
				//m_pEndScore[0]->SetNumDisPlay(m_nBlueLinkEnergy, CUI_NUMBER::UI_NUMTYPE_NONE);
				m_pEndScore[0]->SetNum(m_nBlueLinkEnergy);
			}
			if (m_pEndScore[1] == NULL)
			{//NULLの場合
				//レッドチームのスコアを生成
				m_pEndScore[1] = CUI_NUMBER::Create(D3DXVECTOR3(950.0f, 500.0f, 0.0f), 150.0f, 150.0f, 75.0f, CUI_NUMBER::UI_NUMTYPE_NONE, 0, 1, 3);
				//m_pEndScore[1]->SetNumDisPlay(m_nRedLinkEnergy, CUI_NUMBER::UI_NUMTYPE_NONE);
				m_pEndScore[1]->SetNum(m_nRedLinkEnergy);
			}

			//チームエムブレムを生成
			if (m_pEndTeamEmbrem[0] == NULL) { m_pEndTeamEmbrem[0] = CUI_TEXTURE::Create(D3DXVECTOR3(180.0f, 500.0f, 0.0f), 200.0f, 200.0f, CUI_TEXTURE::UIFLAME_RESULT_BLUE_EMBREM); }
			if (m_pEndTeamEmbrem[1] == NULL) { m_pEndTeamEmbrem[1] = CUI_TEXTURE::Create(D3DXVECTOR3(1130.0f, 500.0f, 0.0f), 200.0f, 200.0f, CUI_TEXTURE::UIFLAME_RESULT_RED_EMBREM); }


			if (CMenu::GetMode() == CMenu::MODE_SINGLE)
			{//シングルプレイの場合
				if (m_pEndResultLogo == NULL)
				{//NULLの場合
					//勝敗のロゴを生成
					if (CResult::GetTeamWin() == CResult::TEAM_WIN_BLUE) { m_pEndResultLogo = CUI_TEXTURE::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, (SCREEN_HEIGHT / 2) - 150.0f, 0.0f), SCREEN_WIDTH / 2, 200.0f, CUI_TEXTURE::UIFLAME_RESULT_VICTORY); }
					else if (CResult::GetTeamWin() == CResult::TEAM_WIN_RED) { m_pEndResultLogo = CUI_TEXTURE::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, (SCREEN_HEIGHT / 2) - 150.0f, 0.0f), SCREEN_WIDTH / 2, 200.0f, CUI_TEXTURE::UIFLAME_RESULT_DEFEAT); }
				}
				//自分のチームを示すロゴを生成
				m_pEndMyTeamLogo = CUI_TEXTURE::Create(D3DXVECTOR3(160.0f, 650.0f, 0.0f), 150.0f, 50.0f, CUI_TEXTURE::UIFLAME_RESULT_MYTEAMLOGO);
			}
			else if (CMenu::GetMode() == CMenu::MODE_MULTI)
			{//マルチプレイの場合
				if (CManager::GetClient() != NULL)
				{//NULLではない場合
					if (CManager::GetClient()->GetTeam() == 0)
					{//チームが0の場合
						if (m_pEndResultLogo == NULL)
						{//NULLの場合
							//勝敗のロゴを生成
							if (CResult::GetTeamWin() == CResult::TEAM_WIN_BLUE) { m_pEndResultLogo = CUI_TEXTURE::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, (SCREEN_HEIGHT / 2) - 150.0f, 0.0f), SCREEN_WIDTH / 2, 200.0f, CUI_TEXTURE::UIFLAME_RESULT_VICTORY); }
							else if (CResult::GetTeamWin() == CResult::TEAM_WIN_RED) { m_pEndResultLogo = CUI_TEXTURE::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, (SCREEN_HEIGHT / 2) - 150.0f, 0.0f), SCREEN_WIDTH / 2, 200.0f, CUI_TEXTURE::UIFLAME_RESULT_DEFEAT); }
						}
						//自分のチームを示すロゴを生成
						m_pEndMyTeamLogo = CUI_TEXTURE::Create(D3DXVECTOR3(160.0f, 650.0f, 0.0f), 150.0f, 50.0f, CUI_TEXTURE::UIFLAME_RESULT_MYTEAMLOGO);
					}
					else if (CManager::GetClient()->GetTeam() == 1)
					{//チームが1の場合
						if (m_pEndResultLogo == NULL)
						{//NULLの場合
							//勝敗のロゴを生成
							if (CResult::GetTeamWin() == CResult::TEAM_WIN_BLUE) { m_pEndResultLogo = CUI_TEXTURE::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, (SCREEN_HEIGHT / 2) - 150.0f, 0.0f), SCREEN_WIDTH / 2, 200.0f, CUI_TEXTURE::UIFLAME_RESULT_DEFEAT); }
							else if (CResult::GetTeamWin() == CResult::TEAM_WIN_RED) { m_pEndResultLogo = CUI_TEXTURE::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, (SCREEN_HEIGHT / 2) - 150.0f, 0.0f), SCREEN_WIDTH / 2, 200.0f, CUI_TEXTURE::UIFLAME_RESULT_VICTORY); }
						}
						//自分のチームを示すロゴを生成
						m_pEndMyTeamLogo = CUI_TEXTURE::Create(D3DXVECTOR3(1130.0f, 650.0f, 0.0f), 150.0f, 50.0f, CUI_TEXTURE::UIFLAME_RESULT_MYTEAMLOGO);
					}
				}
			}
		}
	}
}

//=============================================================================
// フェードの更新処理
//=============================================================================
void CGame::FadeUpdate(void)
{
	//遷移するまでのカウンターを加算
	m_modeCounter++;
	if (m_modeCounter >= 200)
	{//戦死するまでのカウンターが200以上の場合

		//背景の開放
		if (m_pEndBG != NULL)
		{
			m_pEndBG->Uninit();
			m_pEndBG = NULL;

		}

		//勝敗のロゴの開放
		if (m_pEndResultLogo != NULL)
		{
			m_pEndResultLogo->Uninit();
			m_pEndResultLogo = NULL;
		}

		//自分のチームを示すロゴの開放
		if (m_pEndMyTeamLogo != NULL)
		{
			m_pEndMyTeamLogo->Uninit();
			m_pEndMyTeamLogo = NULL;
		}

		for (int nCntResult = 0; nCntResult < NUM_TEAM; nCntResult++)
		{
			//スコアの破棄
			if (m_pEndScore[nCntResult] != NULL)
			{
				m_pEndScore[nCntResult]->Uninit();
				m_pEndScore[nCntResult] = NULL;
			}

			//チームエムブレムの破棄
			if (m_pEndTeamEmbrem[nCntResult] != NULL)
			{
				m_pEndTeamEmbrem[nCntResult]->Uninit();
				m_pEndTeamEmbrem[nCntResult] = NULL;
			}
		}

		//何もしていない状態にする
		m_state = STATE_NONE;

		// 画面(モード)の設定
		CFade::Create(CManager::MODE_RESULT);
	}
}

//=============================================================================
// ゲームの状態の設定
//=============================================================================
const void CGame::SetGameState(STATE state)
{
	m_state = state;
}

//=============================================================================
// ステージの設定
//=============================================================================
void CGame::SetStage(int nStage)
{
	// 現在のステージを設定
	m_nCurStage = nStage;
}

//=============================================================================
// パートの切り替え処理
//=============================================================================
void CGame::SwicthPart(void)
{
	CInputKeyboard	*pKeyboard = CManager::GetInputKeyboard();	// キーボードの取得
	CXInput			*pXInput = CManager::GetXInput();			// XInputの取得
	CInputMouse *pMouse = CManager::GetInputMouse();
	CDirectInput *pDirectInput = CManager::GetDirectInput();	//DirectInputの取得
	CDirectInput::GamePad *DirectInputStick = pDirectInput->GetgamePadStick();

	if (m_state == STATE_NORMAL)
	{
		if (pKeyboard->GetTrigger(DIK_Z) || pDirectInput->GetGamePadTrigger(0))
		{// パート切り替え
			m_part = (PART)((m_part + 1) % PART_MAX);

			CScene::UIUninit();

			if (PART_STRATEGY == m_part)
			{
				//バグ対策用にプレイヤーＵＩの破棄処理
				if (CMenu::MODE_MULTI == CMenu::GetMode())
				{
					if (CManager::GetClient() != NULL)
					{
						m_pPlayer[CManager::GetClient()->GetPlayerIdx()]->ReleasePlayerUI();
					}
				}
				else if (CMenu::MODE_SINGLE == CMenu::GetMode())
				{
					m_pPlayer[0]->ReleasePlayerUI();
				}

				CreateStrategyUI();
			}
			else
			{
				CreateActionUI();
			}
		}
	}
	else if (m_state == STATE_END)
	{
		if (NULL != m_pButtonManager)
		{// ボタン管理クラスの破棄
			m_pButtonManager->Uninit();
			m_pButtonManager = NULL;
		}
		if (NULL != m_pMouseCursor)
		{// マウスカーソルの破棄
			m_pMouseCursor->Uninit();
			m_pMouseCursor = NULL;
		}

		if (NULL != m_pMouse)
		{
			//m_pMouse->Uninit();
			//m_pMouse = NULL;
			m_pMouse->SetDisp(false);
		}
		m_part = PART_ACTION;

	}
}

//=============================================================================
// アクションパートのUI生成処理
//=============================================================================
void CGame::CreateActionUI(void)
{
	if (NULL != m_pButtonManager)
	{// ボタン管理クラスの破棄
		m_pButtonManager->Uninit();
		m_pButtonManager = NULL;
	}

	if (NULL != m_pMouseCursor)
	{// マウスカーソルの破棄
		m_pMouseCursor->Uninit();
		m_pMouseCursor = NULL;
	}

	if (NULL != m_pMouse)
	{
		//m_pMouse->Uninit();
		//m_pMouse = NULL;
		m_pMouse->SetDisp(false);
	}

	//****************************************
	// 2DUI生成（フレーム）
	//****************************************
	// 武器フレーム
	CUI_TEXTURE::Create(D3DXVECTOR3(1100.0f, 650.0f, 0.0f), 350.0f, 120.0f, CUI_TEXTURE::UIFLAME_WEAPON);		// 武器

																												// プレイヤー体力フレーム
	CUI_TEXTURE::Create(D3DXVECTOR3(180.0f, 650.0f, 0.0f), 350.0f, 125.0f, CUI_TEXTURE::UIFLAME_PLAYER_HP);	// 胴体

																											// AIチケット数フレーム
	CUI_TEXTURE::Create(D3DXVECTOR3(125.0f, 530.0f, 0.0f), 230.0f, 100.0f, CUI_TEXTURE::UIFLAME_DRONE);		// AI01
	CUI_TEXTURE::Create(D3DXVECTOR3(125.0f, 420.0f, 0.0f), 230.0f, 100.0f, CUI_TEXTURE::UIFLAME_WORKER);	// AI02

																											// チームのチケット数フレーム
	CUI_TEXTURE::Create(D3DXVECTOR3(80.0f, 40.0f, 0.0f), 120.0f, 50.0f, CUI_TEXTURE::UIFLAME_TEAM_BLUE);	// BLUE
	CUI_TEXTURE::Create(D3DXVECTOR3(80.0f, 100.0f, 0.0f), 120.0f, 50.0f, CUI_TEXTURE::UIFLAME_TEAM_RED);	// RED

																											//****************************************
																											// UI生成（数字）
																											//****************************************
	CUI_NUMBER::Create(D3DXVECTOR3(370.0f, 40.0f, 0.0f), 430.0f, 30.0f, 0.0f, CUI_NUMBER::UI_NUMTYPE_BLUE, 0, NUMTEX_UV_X, NUMTEX_UV_Y);							// BLUEチームチケット
	CUI_NUMBER::Create(D3DXVECTOR3(370.0f, 100.0f, 0.0f), 430.0f, 30.0f, 0.0f, CUI_NUMBER::UI_NUMTYPE_RED, 0, NUMTEX_UV_X, NUMTEX_UV_Y);							// REDチームチケット
	CUI_NUMBER::Create(D3DXVECTOR3(1120.0f, 660.0f, 0.0f), 150.0f, 90.0f, 60.0f, CUI_NUMBER::UI_NUMTYPE_REMAINBULLET, 0, NUMTEX_UV_X, NUMTEX_UV_Y);		// 残弾
	CUI_NUMBER::Create(D3DXVECTOR3(260.0f, 530.0f, 0.0f), 100.0f, 60.0f, 40.0f, CUI_NUMBER::UI_NUMTYPE_DRONE_HP, 0, NUMTEX_UV_X, NUMTEX_UV_Y);				// ドローンライフ
	CUI_NUMBER::Create(D3DXVECTOR3(260.0f, 420.0f, 0.0f), 100.0f, 60.0f, 40.0f, CUI_NUMBER::UI_NUMTYPE_WORKER_HP, 0, NUMTEX_UV_X, NUMTEX_UV_Y);			// ワーカーライフ
	CUI_NUMBER::Create(D3DXVECTOR3(385.0f, 650.0f, 0.0f), 150.0f, 100.0f, 60.0f, CUI_NUMBER::UI_NUMTYPE_PLAYER_HP, 0, NUMTEX_UV_X, NUMTEX_UV_Y);			// プレイヤーライフ

}

//=============================================================================
// ストラテジーパートのUI生成処理
//=============================================================================
void CGame::CreateStrategyUI(void)
{
	//****************************************
	// 2DUI生成（フレーム）
	//****************************************
	// 背景
	CUI_TEXTURE::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), SCREEN_WIDTH, SCREEN_HEIGHT, CUI_TEXTURE::UIFLAME_STRATEGY_BG);	// 背景

																																				// フレーム
	CUI_TEXTURE::Create(D3DXVECTOR3(785.0f, 420.0f, 0.0f), 990.0f, 590.0f, CUI_TEXTURE::UIFLAME_FLAME_BLUE);	// 大枠
	CUI_TEXTURE::Create(D3DXVECTOR3(790.0f, 190.0f, 0.0f), 970.0f, 100.0f, CUI_TEXTURE::UIFLAME_FLAME_WHITE);	// ロジックライン
	CUI_TEXTURE::Create(D3DXVECTOR3(405.0f, 470.0f, 0.0f), 200.0f, 450.0f, CUI_TEXTURE::UIFLAME_FLAME_WHITE);	// AI表示
	CUI_TEXTURE::Create(D3DXVECTOR3(895.0f, 470.0f, 0.0f), 760.0f, 450.0f, CUI_TEXTURE::UIFLAME_FLAME_WHITE);	// ロジック部分

																												// チームのチケット数フレーム
	CUI_TEXTURE::Create(D3DXVECTOR3(80.0f, 40.0f, 0.0f), 120.0f, 50.0f, CUI_TEXTURE::UIFLAME_TEAM_BLUE);	// BLUE
	CUI_TEXTURE::Create(D3DXVECTOR3(80.0f, 100.0f, 0.0f), 120.0f, 50.0f, CUI_TEXTURE::UIFLAME_TEAM_RED);	// RED

																											//****************************************
																											// 1P小隊情報
																											//****************************************
	CUI_TEXTURE::Create(D3DXVECTOR3(150.0f, 325.0f, 0.0f), 280.0f, 260.0f, CUI_TEXTURE::UIFLAME_1P_INFO);

	// プレイヤー体力フレーム
	CUI_TEXTURE::Create(D3DXVECTOR3(150.0f, 260.0f, 0.0f), 255.0f, 80.0f, CUI_TEXTURE::UIFLAME_PLAYER_HP);

	// AIチケット数フレーム
	CUI_TEXTURE::Create(D3DXVECTOR3(150.0f, 335.0f, 0.0f), 255.0f, 80.0f, CUI_TEXTURE::UIFLAME_WORKER);		// ワーカー
	CUI_TEXTURE::Create(D3DXVECTOR3(150.0f, 410.0f, 0.0f), 255.0f, 80.0f, CUI_TEXTURE::UIFLAME_DRONE);		// ドローン

																											// 各ライフの数字生成
	CUI_NUMBER::Create(D3DXVECTOR3(305.0f, 260.0f, 0.0f), 115.0f, 70.0f, 45.0f, CUI_NUMBER::UI_NUMTYPE_PLAYER_HP, 0, NUMTEX_UV_X, NUMTEX_UV_Y);			// 1Pプレイヤー
	CUI_NUMBER::Create(D3DXVECTOR3(305.0f, 335.0f, 0.0f), 115.0f, 70.0f, 45.0f, CUI_NUMBER::UI_NUMTYPE_WORKER_HP, 0, NUMTEX_UV_X, NUMTEX_UV_Y);		// 1Pワーカー
	CUI_NUMBER::Create(D3DXVECTOR3(305.0f, 410.0f, 0.0f), 115.0f, 70.0f, 45.0f, CUI_NUMBER::UI_NUMTYPE_DRONE_HP, 0, NUMTEX_UV_X, NUMTEX_UV_Y);			// 1Pドローン

																																						//****************************************
																																						// 2P小隊情報
																																						//****************************************
	CUI_TEXTURE::Create(D3DXVECTOR3(150.0f, 590.0f, 0.0f), 280.0f, 260.0f, CUI_TEXTURE::UIFLAME_2P_INFO);

	// プレイヤー体力フレーム
	CUI_TEXTURE::Create(D3DXVECTOR3(150.0f, 525.0f, 0.0f), 255.0f, 80.0f, CUI_TEXTURE::UIFLAME_ALLY_HP);

	// AIチケット数フレーム
	CUI_TEXTURE::Create(D3DXVECTOR3(150.0f, 600.0f, 0.0f), 255.0f, 80.0f, CUI_TEXTURE::UIFLAME_WORKER);		// ワーカー
	CUI_TEXTURE::Create(D3DXVECTOR3(150.0f, 675.0f, 0.0f), 255.0f, 80.0f, CUI_TEXTURE::UIFLAME_DRONE);		// ドローン

																											// 各ライフの数字生成
	CUI_NUMBER::Create(D3DXVECTOR3(305.0f, 525.0f, 0.0f), 115.0f, 70.0f, 45.0f, CUI_NUMBER::UI_NUMTYPE_ALLY_HP, 0, NUMTEX_UV_X, NUMTEX_UV_Y);				// 2Pプレイヤー
	CUI_NUMBER::Create(D3DXVECTOR3(305.0f, 600.0f, 0.0f), 115.0f, 70.0f, 45.0f, CUI_NUMBER::UI_NUMTYPE_ALLY_WORKER_HP, 0, NUMTEX_UV_X, NUMTEX_UV_Y);		// 2Pワーカー
	CUI_NUMBER::Create(D3DXVECTOR3(305.0f, 675.0f, 0.0f), 115.0f, 70.0f, 45.0f, CUI_NUMBER::UI_NUMTYPE_ALLY_DRONE_HP, 0, NUMTEX_UV_X, NUMTEX_UV_Y);		// 2Pドローン

																																						// ストラテジー操作説明
	CUI_TEXTURE::Create(D3DXVECTOR3(940.0f, 70.0f, 0.0f), 640.0f, 80.0f, CUI_TEXTURE::UIFLAME_STRATEGY_INST);

	//****************************************
	// UI生成（数字）
	//****************************************
	CUI_NUMBER::Create(D3DXVECTOR3(370.0f, 40.0f, 0.0f), 430.0f, 30.0f, 0.0f, CUI_NUMBER::UI_NUMTYPE_BLUE, 0, NUMTEX_UV_X, NUMTEX_UV_Y);							// BLUEチームチケット
	CUI_NUMBER::Create(D3DXVECTOR3(370.0f, 100.0f, 0.0f), 430.0f, 30.0f, 0.0f, CUI_NUMBER::UI_NUMTYPE_RED, 0, NUMTEX_UV_X, NUMTEX_UV_Y);							// REDチームチケット

	if (NULL == m_pButtonManager)
	{// ボタン管理クラスの生成
		m_pButtonManager = CButtonManagerStrategy::Create();
	}

	if (NULL == m_pMouseCursor)
	{// マウスカーソルの生成
		m_pMouseCursor = CMouseCursor2D::Create();
	}

	if (NULL == m_pMouse)
	{
		m_pMouse = CMouseCursor::Create();
	}

	if (NULL != m_pMouse)
	{
		m_pMouse->SetDisp(true);
	}
}

//=============================================================================
// プレイヤーの生成処理
//=============================================================================
void CGame::CreatePlayer(void)
{
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER_CONNECT; nCntPlayer++)
	{
		int nTeam = 0;	//チーム情報
		D3DXVECTOR3 respawnPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		if (nCntPlayer == 0 || nCntPlayer == 1)
		{//プレイヤー番号が０か１の場合
			nTeam = 0;	//青チーム
		}
		else if (nCntPlayer == 2 || nCntPlayer == 3)
		{//プレイヤー番号が２か３の場合
			nTeam = 1;	//赤チーム
		}

		switch (nCntPlayer)
		{
		case 0:
			respawnPos = m_aRespawnPos[nTeam][1];
			break;
		case 1:
			respawnPos = m_aRespawnPos[nTeam][2];
			break;
		case 2:
			respawnPos = m_aRespawnPos[nTeam][1];
			break;
		case 3:
			respawnPos = m_aRespawnPos[nTeam][2];
			break;
		}

		if (CMenu::GetMode() == CMenu::MODE_MULTI)
		{//マルチプレイの場合
			bool bConnect = false;	//接続しているかどうか

			//クライアントの取得
			CClient *pClient = CManager::GetClient();

			//if (m_aMechaType[nCntPlayer] == -1)
			//{//機体番号が-1の場合
			// //ランダムで機体を決める
			//	m_aMechaType[nCntPlayer] = (CMechaSelect::MECHATYPE)(rand() % CMechaSelect::MECHATYPE_MAX);
			//	m_pPlayer[nCntPlayer] = CPlayer::Create(nCntPlayer, m_aMechaType[nCntPlayer], respawnPos, m_bConnect[nCntPlayer]);
			//}
			//else
			//{//それ以外の場合
			//	m_bConnect[nCntPlayer] = true;	//接続している状態にする
			//	m_pPlayer[nCntPlayer] = CPlayer::Create(nCntPlayer, m_aMechaType[nCntPlayer], respawnPos, m_bConnect[nCntPlayer]);

			//}

			if (CMatching::GetConnect(nCntPlayer) == false)
			{//マッチング時点で接続されていなかった場合
				m_pPlayer[nCntPlayer] = CPlayer::Create(nCntPlayer, m_aMechaType[nCntPlayer], respawnPos, false);
			}
			else if (CMatching::GetConnect(nCntPlayer) == true)
			{
				m_pPlayer[nCntPlayer] = CPlayer::Create(nCntPlayer, m_aMechaType[nCntPlayer], respawnPos, true);
			}
		}
		else
		{//シングルプレイの場合
			if (nCntPlayer == 0)
			{//プレイヤー番号が０番の場合
				m_aMechaType[nCntPlayer] = CMechaSelect::GetMechaType();
				m_pPlayer[nCntPlayer] = CPlayer::Create(nCntPlayer, m_aMechaType[nCntPlayer], respawnPos, true);
			}
			else
			{//それ以外の場合
				m_aMechaType[nCntPlayer] = (CMechaSelect::MECHATYPE)(rand() % CMechaSelect::MECHATYPE_MAX);
				m_pPlayer[nCntPlayer] = CPlayer::Create(nCntPlayer, m_aMechaType[nCntPlayer], respawnPos, false);
			}
		}
	}
}

//=============================================================================
// 必要な情報を書き込む処理
//=============================================================================
void CGame::PrintData(void)
{
	//クライアントの取得
	CClient *pClient = CManager::GetClient();
#if 1
	if (pClient != NULL && pClient->GetConnect() == true)
	{//NULLではない場合
		if (m_pPlayer[pClient->GetPlayerIdx()] != NULL)
		{//NULLではない場合
			pClient->Printf(SERVER_PLAYER_DATA);

			//プレイヤー番号を書き込む
			pClient->Printf("%d", CManager::GetClient()->GetPlayerIdx());
			pClient->Printf(" ");

			//チームの番号を書き込む
			pClient->Printf("%d", CManager::GetClient()->GetTeam());
			pClient->Printf(" ");

			//プレイヤーのx,y軸の位置を書き込む
			pClient->Printf("%.1f %.1f", m_pPlayer[pClient->GetPlayerIdx()]->GetPos().x, m_pPlayer[pClient->GetPlayerIdx()]->GetPos().z);
			pClient->Printf(" ");

			//プレイヤーのy軸の向きを書き込む
			pClient->Printf("%.1f", m_pPlayer[pClient->GetPlayerIdx()]->GetRot().y);
			pClient->Printf(" ");

			//モデル0番のy軸の向きを書き込む
			pClient->Printf("%.1f", m_pPlayer[pClient->GetPlayerIdx()]->GetModel(0)->GetRot().y);
			pClient->Printf(" ");

			//モデル1番の向きを書き込む
			pClient->Printf("%.1f %.1f %.1f", m_pPlayer[pClient->GetPlayerIdx()]->GetModel(1)->GetRot().x, m_pPlayer[pClient->GetPlayerIdx()]->GetModel(1)->GetRot().y, m_pPlayer[pClient->GetPlayerIdx()]->GetModel(1)->GetRot().z);
			pClient->Printf(" ");

			//カメラの向きを書き込む
			pClient->Printf("%.2f %.2f %.2f", CManager::GetCamera()->GetRot().x, CManager::GetCamera()->GetRot().y, CManager::GetCamera()->GetRot().z);
			pClient->Printf(" ");

			//死亡しているかどうか
			if (m_pPlayer[pClient->GetPlayerIdx()]->GetDeath() == true)
			{
				//死亡していることを書き込む
				pClient->Printf("1");
				pClient->Printf(" ");

				//キルプレイヤーの番号を書き込む
				pClient->Printf("%d", m_pPlayer[pClient->GetPlayerIdx()]->GetKillPlayerIdx());
				pClient->Printf(" ");

				//キルプレイヤーの種類を書き込む
				pClient->Printf("%d", (int)m_playerType[0]);
				pClient->Printf(" ");

			}
			else
			{
				//死亡していないことを書き込む
				pClient->Printf("0");
				pClient->Printf(" ");

				//体力の表示
				pClient->Printf("%d", m_pPlayer[pClient->GetPlayerIdx()]->GetLife());
				pClient->Printf(" ");
			}

			//弾を発射しているかどうかを書き込む
			if (m_pPlayer[pClient->GetPlayerIdx()]->GetShoot() == true)
			{//発射されている場合
			 //発射している情報を書き込む
				pClient->Printf("1");
				pClient->Printf(" ");

				//弾の同時発射数を書き込む
				pClient->Printf("%d", m_pPlayer[pClient->GetPlayerIdx()]->GetNumShoot());
				pClient->Printf(" ");

				//攻撃力を書き込む
				pClient->Printf("%d", m_pPlayer[pClient->GetPlayerIdx()]->GetAttack());
				pClient->Printf(" ");

				//水平角度の情報を書き込む
				for (int nCntShoot = 0; nCntShoot < m_pPlayer[pClient->GetPlayerIdx()]->GetNumShoot() * 2; nCntShoot++)
				{
					pClient->Printf("%.2f %.2f", m_pPlayer[pClient->GetPlayerIdx()]->GetAngle(nCntShoot), m_pPlayer[pClient->GetPlayerIdx()]->GetAngleV(nCntShoot));
					pClient->Printf(" ");

				}

				//発射していない状態に戻す
				m_pPlayer[pClient->GetPlayerIdx()]->SetShoot(false);
			}
			else
			{
				//発射していない情報を書き込む
				pClient->Printf("0");
				pClient->Printf(" ");
			}

			//チャットをしているかどうかを書き込む
			if (m_pPlayer[pClient->GetPlayerIdx()]->GetChat() == true)
			{//チャットをしている場合
			 //チャットをしていることを書き込む
				pClient->Printf("1");
				pClient->Printf(" ");

				//チャットの番号を書き込む
				pClient->Printf("%d", m_pPlayer[pClient->GetPlayerIdx()]->GetRadioChat());
				pClient->Printf(" ");
			}
			else
			{
				//チャットをしていないことを書き込む
				pClient->Printf("0");
				pClient->Printf(" ");
			}

			for (int nCntAI = 0; nCntAI < AI_MAX; nCntAI++)
			{
				//AIの位置を書き込む
				pClient->Printf("%.1f %.1f %.1f", m_pPlayer[pClient->GetPlayerIdx()]->GetMyAI(nCntAI)->GetPos().x, m_pPlayer[pClient->GetPlayerIdx()]->GetMyAI(nCntAI)->GetPos().y, m_pPlayer[pClient->GetPlayerIdx()]->GetMyAI(nCntAI)->GetPos().z);
				pClient->Printf(" ");

				//AIの向きを書き込む
				pClient->Printf("%.1f", m_pPlayer[pClient->GetPlayerIdx()]->GetMyAI(nCntAI)->GetRot().y);
				pClient->Printf(" ");

				//AIの死亡しているかどうかを書き込む
				if (m_pPlayer[pClient->GetPlayerIdx()]->GetMyAI(nCntAI)->GetDeath() == true)
				{//死亡している場合
				 //死亡している情報を書きこむ
					pClient->Printf("1");
					pClient->Printf(" ");

					//キルプレイヤーの番号を書き込む
					pClient->Printf("%d", m_pPlayer[pClient->GetPlayerIdx()]->GetMyAI(nCntAI)->GetKillPlayerIdx());
					pClient->Printf(" ");

					//キルプレイヤーの種類を書き込む
					pClient->Printf("%d", (int)m_playerType[0]);
					pClient->Printf(" ");

				}
				else
				{
					//死亡していないことを書き込む
					pClient->Printf("0");
					pClient->Printf(" ");

					//体力の表示
					pClient->Printf("%d", m_pPlayer[pClient->GetPlayerIdx()]->GetMyAI(nCntAI)->GetLife());
					pClient->Printf(" ");

				}

				//弾を発射しているかどうかを書き込む
				if (m_pPlayer[pClient->GetPlayerIdx()]->GetMyAI(nCntAI)->GetShoot() == true)
				{//発射されている場合
					//発射している情報を書き込む
					pClient->Printf("1");
					pClient->Printf(" ");

					//攻撃力を書き込む
					pClient->Printf("%d", m_pPlayer[pClient->GetPlayerIdx()]->GetMyAI(nCntAI)->GetAttack());
					pClient->Printf(" ");

					//水平角度の情報を書き込む
					pClient->Printf("%.2f %.2f", m_pPlayer[pClient->GetPlayerIdx()]->GetMyAI(nCntAI)->GetAngle(), m_pPlayer[pClient->GetPlayerIdx()]->GetMyAI(nCntAI)->GetAngleV());
					pClient->Printf(" ");

					//発射していない状態に戻す
					m_pPlayer[pClient->GetPlayerIdx()]->GetMyAI(nCntAI)->SetShoot(false);
				}
				else
				{
					//発射していない情報を書き込む
					pClient->Printf("0");
					pClient->Printf(" ");
				}

			}

			// ピンを使用しているかどうかを書き込む
			if (m_pPlayer[pClient->GetPlayerIdx()]->GetPinUse() == true)
			{// ピンを使用している場合
				pClient->Printf("1");
				pClient->Printf(" ");

				// 位置を書き込む
				pClient->Printf("%.1f %.1f %.1f", m_pPlayer[pClient->GetPlayerIdx()]->GetPinPos().x, m_pPlayer[pClient->GetPlayerIdx()]->GetPinPos().y, m_pPlayer[pClient->GetPlayerIdx()]->GetPinPos().z);
				pClient->Printf(" ");

				//// 表示時間を書き込む
				//pClient->Printf("%d", m_pPlayer[pClient->GetPlayerIdx()]->GetPinLife());
				//pClient->Printf(" ");
			}
			else
			{// ピンを使用していない場合
				pClient->Printf("0");
				pClient->Printf(" ");
			}

			for (int nCntAI = 0; nCntAI < AI_MAX; nCntAI++)
			{// AIの数だけ回る
			 // AIがピンを使用しているかどうかを書き込む
				if (m_pPlayer[pClient->GetPlayerIdx()]->GetAIPinUse(nCntAI) == true)
				{// AIがピンを使用している場合
					pClient->Printf("1");
					pClient->Printf(" ");

					// 位置を書き込む
					pClient->Printf("%.1f %.1f %.1f", m_pPlayer[pClient->GetPlayerIdx()]->GetAIPinPos(nCntAI).x, m_pPlayer[pClient->GetPlayerIdx()]->GetAIPinPos(nCntAI).y, m_pPlayer[pClient->GetPlayerIdx()]->GetAIPinPos(nCntAI).z);
					pClient->Printf(" ");
				}
				else
				{// ピンを使用していない場合
					pClient->Printf("0");
					pClient->Printf(" ");
				}
			}

#if 0
			//AIの位置を書き込む
			pClient->Printf("%.1f %.1f %.1f", m_pPlayer[pClient->GetPlayerIdx()]->GetMyAI(0)->GetPos().x, m_pPlayer[pClient->GetPlayerIdx()]->GetMyAI(0)->GetPos().y, m_pPlayer[pClient->GetPlayerIdx()]->GetMyAI(0)->GetPos().z);
			pClient->Printf(" ");

			//AIの向きを書き込む
			pClient->Printf("%.1f", m_pPlayer[pClient->GetPlayerIdx()]->GetMyAI(0)->GetRot().y);
			pClient->Printf(" ");

			//AIの死亡しているかどうかを書き込む
			if (m_pPlayer[pClient->GetPlayerIdx()]->GetMyAI(0)->GetDeath() == true)
			{//死亡している場合
			 //死亡している情報を書きこむ
				pClient->Printf("1");
				pClient->Printf(" ");

				//キルプレイヤーの番号を書き込む
				pClient->Printf("%d", m_pPlayer[pClient->GetPlayerIdx()]->GetMyAI(0)->GetKillPlayerIdx());
				pClient->Printf(" ");

				//キルプレイヤーの種類を書き込む
				pClient->Printf("%d", (int)m_playerType[0]);
				pClient->Printf(" ");

			}
			else
			{
				//死亡していないことを書き込む
				pClient->Printf("0");
				pClient->Printf(" ");
			}
#endif
			if (pClient->GetPlayerIdx() == 0)
			{//ホストの場合
				if (m_state == STATE_END)
				{//ゲームオーバーの場合
				 //ゲーム終了を書き込む
					pClient->Printf("1");
					pClient->Printf(" ");

					//勝ったチームの情報を書き込む
					pClient->Printf("%d", CResult::GetTeamWin());
					pClient->Printf(" ");
				}
				else
				{
					pClient->Printf("0");
					pClient->Printf(" ");
				}
				//CPUのデータ情報を書き込む処理
				PrintCPUData();
			}
		}
	}
#endif
}

//=============================================================================
// CPUのデータ情報を書き込む処理
//=============================================================================
void CGame::PrintCPUData(void)
{
	CClient *pClient = CManager::GetClient();
	if (pClient != NULL)
	{
		for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER_CONNECT; nCntPlayer++)
		{
			if (pClient->GetPlayerIdx() != nCntPlayer)
			{
				if (CMatching::GetConnect(nCntPlayer) == false)
				{
					//AIプレイヤー情報を書き込む
					pClient->Printf(SERVER_AI_PLAYER_DATA);

					//プレイヤー番号を書き込む
					pClient->Printf("%d", nCntPlayer);
					pClient->Printf(" ");

					//チームの番号を書き込む
					pClient->Printf("%d", m_pPlayer[nCntPlayer]->GetTeam());
					pClient->Printf(" ");

					//プレイヤーのx,y軸の位置を書き込む
					pClient->Printf("%.1f %.1f", m_pPlayer[nCntPlayer]->GetPos().x, m_pPlayer[nCntPlayer]->GetPos().z);
					pClient->Printf(" ");

					//プレイヤーのy軸の向きを書き込む
					pClient->Printf("%.1f", m_pPlayer[nCntPlayer]->GetRot().y);
					pClient->Printf(" ");

					//モデル0番のy軸の向きを書き込む
					pClient->Printf("%.1f", m_pPlayer[nCntPlayer]->GetModel(0)->GetRot().y);
					pClient->Printf(" ");

					//モデル1番の向きを書き込む
					pClient->Printf("%.1f %.1f %.1f", m_pPlayer[nCntPlayer]->GetModel(1)->GetRot().x, m_pPlayer[nCntPlayer]->GetModel(1)->GetRot().y, m_pPlayer[nCntPlayer]->GetModel(1)->GetRot().z);
					pClient->Printf(" ");

					//上半身のの向きを書き込む
					pClient->Printf("%.2f %.2f %.2f", m_pPlayer[nCntPlayer]->GetDestUpper().x, m_pPlayer[nCntPlayer]->GetDestUpper().y, m_pPlayer[nCntPlayer]->GetDestUpper().z);
					pClient->Printf(" ");

					//死亡しているかどうか
					if (m_pPlayer[nCntPlayer]->GetDeath() == true)
					{
						//死亡していることを書き込む
						pClient->Printf("1");
						pClient->Printf(" ");

						//キルプレイヤーの番号を書き込む
						pClient->Printf("%d", m_pPlayer[nCntPlayer]->GetKillPlayerIdx());
						pClient->Printf(" ");

						//キルプレイヤーの種類を書き込む
						pClient->Printf("%d", (int)m_playerType[0]);
						pClient->Printf(" ");

					}
					else
					{
						//死亡していないことを書き込む
						pClient->Printf("0");
						pClient->Printf(" ");

						//体力を書き込む
						pClient->Printf("%d", m_pPlayer[nCntPlayer]->GetLife());
						pClient->Printf(" ");

					}

					//弾を発射しているかどうかを書き込む
					if (m_pPlayer[nCntPlayer]->GetShoot() == true)
					{//発射されている場合

						pClient->Printf("1");
						pClient->Printf(" ");

						//弾の同時発射数を書き込む
						pClient->Printf("%d", m_pPlayer[nCntPlayer]->GetNumShoot());
						pClient->Printf(" ");

						//攻撃力を書き込む
						pClient->Printf("%d", m_pPlayer[nCntPlayer]->GetAttack());
						pClient->Printf(" ");

						//水平角度の情報を書き込む
						for (int nCntShoot = 0; nCntShoot < m_pPlayer[nCntPlayer]->GetNumShoot() * 2; nCntShoot++)
						{
							pClient->Printf("%.2f %.2f", m_pPlayer[nCntPlayer]->GetAngle(nCntShoot), m_pPlayer[nCntPlayer]->GetAngleV(nCntShoot));
							pClient->Printf(" ");

						}

						//発射していない状態に戻す
						m_pPlayer[nCntPlayer]->SetShoot(false);
					}
					else
					{
						pClient->Printf("0");
						pClient->Printf(" ");
					}


					for (int nCntAI = 0; nCntAI < AI_MAX; nCntAI++)
					{
						//AIの位置を書き込む
						pClient->Printf("%.1f %.1f %.1f", m_pPlayer[nCntPlayer]->GetMyAI(nCntAI)->GetPos().x, m_pPlayer[nCntPlayer]->GetMyAI(nCntAI)->GetPos().y, m_pPlayer[nCntPlayer]->GetMyAI(nCntAI)->GetPos().z);
						pClient->Printf(" ");

						//AIの向きを書き込む
						pClient->Printf("%.1f", m_pPlayer[nCntPlayer]->GetMyAI(nCntAI)->GetRot().y);
						pClient->Printf(" ");

						//AIの死亡しているかどうかを書き込む
						if (m_pPlayer[nCntPlayer]->GetMyAI(nCntAI)->GetDeath() == true)
						{//死亡している場合
						 //死亡している情報を書きこむ
							pClient->Printf("1");
							pClient->Printf(" ");

							//キルプレイヤーの番号を書き込む
							pClient->Printf("%d", m_pPlayer[nCntPlayer]->GetMyAI(nCntAI)->GetKillPlayerIdx());
							pClient->Printf(" ");

							//キルプレイヤーの種類を書き込む
							pClient->Printf("%d", (int)m_playerType[0]);
							pClient->Printf(" ");

						}
						else
						{
							//死亡していないことを書き込む
							pClient->Printf("0");
							pClient->Printf(" ");

							//体力の表示
							pClient->Printf("%d", m_pPlayer[nCntPlayer]->GetMyAI(nCntAI)->GetLife());
							pClient->Printf(" ");
						}

						//弾を発射しているかどうかを書き込む
						if (m_pPlayer[nCntPlayer]->GetMyAI(nCntAI)->GetShoot() == true)
						{//発射されている場合
						 //発射している情報を書き込む
							pClient->Printf("1");
							pClient->Printf(" ");

							//攻撃力を書き込む
							pClient->Printf("%d", m_pPlayer[nCntPlayer]->GetMyAI(nCntAI)->GetAttack());
							pClient->Printf(" ");

							//水平角度の情報を書き込む
							pClient->Printf("%.2f %.2f", m_pPlayer[nCntPlayer]->GetMyAI(nCntAI)->GetAngle(), m_pPlayer[nCntPlayer]->GetMyAI(nCntAI)->GetAngleV());
							pClient->Printf(" ");

							//発射していない状態に戻す
							m_pPlayer[nCntPlayer]->GetMyAI(nCntAI)->SetShoot(false);
						}
						else
						{
							//発射していない情報を書き込む
							pClient->Printf("0");
							pClient->Printf(" ");
						}
					}

					// ピンを使用しているかどうかを書き込む
					if (m_pPlayer[nCntPlayer]->GetPinUse() == true)
					{// ピンを使用している場合
						pClient->Printf("1");
						pClient->Printf(" ");

						// 位置を書き込む
						pClient->Printf("%.1f %.1f %.1f", m_pPlayer[nCntPlayer]->GetPinPos().x, m_pPlayer[nCntPlayer]->GetPinPos().y, m_pPlayer[nCntPlayer]->GetPinPos().z);
						pClient->Printf(" ");
					}
					else
					{// ピンを使用していない場合
						pClient->Printf("0");
						pClient->Printf(" ");
					}

					for (int nCntAI = 0; nCntAI < AI_MAX; nCntAI++)
					{// AIの数だけ回る
					 // AIがピンを使用しているかどうかを書き込む
						if (m_pPlayer[nCntPlayer]->GetAIPinUse(nCntAI) == true)
						{// AIがピンを使用している場合
							pClient->Printf("1");
							pClient->Printf(" ");

							// 位置を書き込む
							pClient->Printf("%.1f %.1f %.1f", m_pPlayer[nCntPlayer]->GetAIPinPos(nCntAI).x, m_pPlayer[nCntPlayer]->GetAIPinPos(nCntAI).y, m_pPlayer[nCntPlayer]->GetAIPinPos(nCntAI).z);
							pClient->Printf(" ");
						}
						else
						{// ピンを使用していない場合
							pClient->Printf("0");
							pClient->Printf(" ");
						}
					}
				}
			}
		}
	}
}
//=============================================================================
// 情報を読み取る処理
//=============================================================================
void CGame::ReadMessage(void)
{
	CClient *pClient = CManager::GetClient();			//クライアントのポインタ情報
	char *pStr = pClient->GetReceiveData();				//サーバーから受け取ったメッセージ情報

	if (pClient != NULL && pClient->GetConnect() == true)
	{
		//頭出し処理
		pStr = CServerFunction::HeadPutout(pStr, "");

		if (CServerFunction::Memcmp(pStr, SERVER_CONNECT_DATA) == 0)
		{//接続総数を示している場合
			pStr += strlen(SERVER_CONNECT_DATA);			//頭出し

															//接続情報の読み取り処理
			pStr = ReadConnectData(pStr);
		}
		if (CServerFunction::Memcmp(pStr, SERVER_PLAYER_START) == 0)
		{//プレイヤーの開始を示している場合
			pStr += strlen(SERVER_PLAYER_START);			//頭出し
			pStr += strlen(" ");

			for (int nCntClient = 0; nCntClient < pClient->GetNumConnect() - 1; nCntClient++)
			{
				if (CServerFunction::Memcmp(pStr, SERVER_PLAYER_DATA) == 0)
				{
					pStr += strlen(SERVER_PLAYER_DATA);
					pStr = ReadPlayerData(pStr);
				}
			}
		}
	}
}

//=============================================================================
// 接続の情報を読み取る処理
//=============================================================================
char *CGame::ReadConnectData(char *pStr)
{
	//クライアントの取得
	CClient *pClient = CManager::GetClient();
	if (pClient != NULL)
	{
		if (pClient->GetConnect() == true)
		{
			int nWord = 0;		//文字の頭出し用

								//接続総数の設置処理
			pClient->SetNumConnect(CServerFunction::ReadInt(pStr, ""));
			nWord = CServerFunction::PopString(pStr, "");					//文字数カウント
			pStr += nWord;													//頭出し

																			//最小の番号の設置処理
			pClient->SetMinIdx(CServerFunction::ReadInt(pStr, ""));
			nWord = CServerFunction::PopString(pStr, "");					//文字数カウント
			pStr += nWord;													//頭出し

																			//最大の番号の設置処理
			pClient->SetMaxIdx(CServerFunction::ReadInt(pStr, ""));
			nWord = CServerFunction::PopString(pStr, "");					//文字数カウント
			pStr += nWord;													//頭出し
		}
	}
	return pStr;
}


//=============================================================================
// プレイヤーの情報を読み取る処理
//=============================================================================
char *CGame::ReadPlayerData(char *pStr)
{
	D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				//位置
	D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				//向き
	D3DXVECTOR3 modelRotDown = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//モデルの下半身の向き
	D3DXVECTOR3	modelRotUp = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//モデルの上半身の向き
	D3DXVECTOR3 cameraRot;											//カメラの向き
	int nPlayerIdx = 0;												//プレイヤーの番号
	int nNumConnect = 0;											//接続総数
	int nWord = 0;													//文字の頭出し用
	int nCntClient = 0;												//クライアントのカウンター
	int nTeam = 0;													//チーム情報
	float *pAngle = NULL;											//水平角度
	float *pAngleV = NULL;											//水平角度
	bool bShoot = false;											//弾を発射しているかどうか
	int nNumShoot = 0;												//同時発射数
	int nAttack = 0;												//攻撃力
	bool bChat = false;												//チャットしているかどうか
	CPlayer::RADIOCHAT radioChat = CPlayer::RADIOCHAT_OK;			//ラジオチャット
	int nBlueLinkEnergy = 0;										//ブルーチームのリンクエネルギー
	int nRedLinkEnergy = 0;											//レッドチームのリンクエネルギー
	int nState = 0;													//状態情報
	int nLife = 0;													//体力情報
	int nKillPlayerIdx = 0;											//キルプレイヤーの番号
	TYPE playerType = TYPE_PLAYER;									//キルプレイヤーの種類

	bool bPinUse = false;											// ピンを使用しているかどうか
	D3DXVECTOR3 pinPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// ピンの位置
	bool bAIPinUse[AI_MAX] = { false, false };						// AIがピンを使用しているかどうか
	D3DXVECTOR3 AIPinPos[AI_MAX] = { D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f) };// AIのピンの位置

	D3DXVECTOR3 AIPos[AI_MAX] = {D3DXVECTOR3(0.0f,0.0f,0.0f),D3DXVECTOR3(0.0f,0.0f,0.0f)};		//AIの位置
	D3DXVECTOR3 AIRot[AI_MAX] = { D3DXVECTOR3(0.0f,0.0f,0.0f) ,D3DXVECTOR3(0.0f,0.0f,0.0f) };	//AIの向き
	bool bAIDeath[AI_MAX] = {false ,false};														//AIが死んでいるかどうか
	int nAIKillPlayerIdx[AI_MAX] = {0,0};														//AIキルプレイヤーの番号
	TYPE AIPlayerType[AI_MAX] = {TYPE_PLAYER,TYPE_PLAYER};										//AIキルプレイヤーの種類
	bool bAIShoot[AI_MAX] = { false,false };													//AI弾発射したかどうか
	float fAIAngle[AI_MAX] = { 0.0f,0.0f };														//AI角度
	float fAIAngleV[AI_MAX] = { 0.0f,0.0f };													//AI角度
	int nAIAttack[AI_MAX] = { 0, 0 };															//AI攻撃力
	int nAILife[AI_MAX] = { 0,0 };																//AI体力

#if 0
	D3DXVECTOR3 AIPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				//AIの位置
	D3DXVECTOR3 AIRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				//AIの向き
	bool bAIDeath = false;											//AIが死んでいるかどうか
	int nAIKillPlayerIdx = 0;										//AIキルプレイヤーの番号
	TYPE AIPlayerType = TYPE_PLAYER;								//AIキルプレイヤーの種類
#endif
																	//クライアントの取得
	CClient *pClient = CManager::GetClient();
	if (pClient != NULL)
	{
		if (pClient->GetConnect() == true)
		{
			//プレイヤー番号を代入
			nPlayerIdx = CServerFunction::ReadInt(pStr, "");
			nWord = CServerFunction::PopString(pStr, "");		//文字数カウント
			pStr += nWord;										//頭出し

			//チーム情報の代入
			nTeam = CServerFunction::ReadInt(pStr, "");
			nWord = CServerFunction::PopString(pStr, "");
			pStr += nWord;

			//プレイヤーの位置を代入
			pos.x = CServerFunction::ReadFloat(pStr, "");
			nWord = CServerFunction::PopString(pStr, "");
			pStr += nWord;
			pos.z = CServerFunction::ReadFloat(pStr, "");
			nWord = CServerFunction::PopString(pStr, "");
			pStr += nWord;

			//プレイヤーの向きを代入
			rot.y = CServerFunction::ReadFloat(pStr, "");
			nWord = CServerFunction::PopString(pStr, "");
			pStr += nWord;

			//モデル0番の向きを代入
			modelRotDown.y = CServerFunction::ReadFloat(pStr, "");
			nWord = CServerFunction::PopString(pStr, "");
			pStr += nWord;

			//モデル1番の向きを代入
			modelRotUp.x = CServerFunction::ReadFloat(pStr, "");
			nWord = CServerFunction::PopString(pStr, "");
			pStr += nWord;
			modelRotUp.y = CServerFunction::ReadFloat(pStr, "");
			nWord = CServerFunction::PopString(pStr, "");
			pStr += nWord;
			modelRotUp.z = CServerFunction::ReadFloat(pStr, "");
			nWord = CServerFunction::PopString(pStr, "");
			pStr += nWord;

			//カメラの向きを代入
			cameraRot.x = CServerFunction::ReadFloat(pStr, "");
			nWord = CServerFunction::PopString(pStr, "");
			pStr += nWord;
			cameraRot.y = CServerFunction::ReadFloat(pStr, "");
			nWord = CServerFunction::PopString(pStr, "");
			pStr += nWord;
			cameraRot.z = CServerFunction::ReadFloat(pStr, "");
			nWord = CServerFunction::PopString(pStr, "");
			pStr += nWord;

			//死亡しているかしているかどうかを代入
			bool bDeath = CServerFunction::ReadBool(pStr, "");
			nWord = CServerFunction::PopString(pStr, "");
			pStr += nWord;

			if (bDeath == true)
			{//死亡していた場合
			 //キルプレイヤーの番号を代入
				nKillPlayerIdx = CServerFunction::ReadInt(pStr, "");
				nWord = CServerFunction::PopString(pStr, "");
				pStr += nWord;

				//キルプレイヤーの種類を代入
				playerType = (TYPE)CServerFunction::ReadInt(pStr, "");
				nWord = CServerFunction::PopString(pStr, "");
				pStr += nWord;
			}
			else if (bDeath == false)
			{//死亡していない場合
			 //体力の情報を代入
				nLife = CServerFunction::ReadInt(pStr, "");
				nWord = CServerFunction::PopString(pStr, "");
				pStr += nWord;
			}

			//弾を発射しているかどうかを代入
			bShoot = CServerFunction::ReadBool(pStr, "");
			nWord = CServerFunction::PopString(pStr, "");
			pStr += nWord;

			if (bShoot == true)
			{//弾を発射している場合
			 //弾の同時発射数をを代入
				nNumShoot = CServerFunction::ReadInt(pStr, "");
				nWord = CServerFunction::PopString(pStr, "");
				pStr += nWord;

				//水平角度を同時発射数分動的確保
				pAngle = new float[nNumShoot * 2];
				pAngleV = new float[nNumShoot * 2];

				//攻撃力を代入
				nAttack = CServerFunction::ReadInt(pStr, "");
				nWord = CServerFunction::PopString(pStr, "");
				pStr += nWord;

				//水平角度を代入
				for (int nCntShoot = 0; nCntShoot < nNumShoot * 2; nCntShoot++)
				{
					pAngle[nCntShoot] = CServerFunction::ReadFloat(pStr, "");
					nWord = CServerFunction::PopString(pStr, "");
					pStr += nWord;

					pAngleV[nCntShoot] = CServerFunction::ReadFloat(pStr, "");
					nWord = CServerFunction::PopString(pStr, "");
					pStr += nWord;

				}
			}

			//チャットをしているかどうかを代入
			bChat = CServerFunction::ReadBool(pStr, "");
			nWord = CServerFunction::PopString(pStr, "");
			pStr += nWord;

			if (bChat == true)
			{//チャットをしている場合
				radioChat = (CPlayer::RADIOCHAT)CServerFunction::ReadInt(pStr, "");
				nWord = CServerFunction::PopString(pStr, "");
				pStr += nWord;
			}

			for (int nCntAI = 0; nCntAI < AI_MAX; nCntAI++)
			{
				//AIの位置を代入
				AIPos[nCntAI].x = CServerFunction::ReadFloat(pStr, "");
				nWord = CServerFunction::PopString(pStr, "");
				pStr += nWord;
				AIPos[nCntAI].y = CServerFunction::ReadFloat(pStr, "");
				nWord = CServerFunction::PopString(pStr, "");
				pStr += nWord;
				AIPos[nCntAI].z = CServerFunction::ReadFloat(pStr, "");
				nWord = CServerFunction::PopString(pStr, "");
				pStr += nWord;

				//AIの向きを代入
				AIRot[nCntAI].y = CServerFunction::ReadFloat(pStr, "");
				nWord = CServerFunction::PopString(pStr, "");
				pStr += nWord;

				//AIの死亡しているかどうかを代入
				bAIDeath[nCntAI] = CServerFunction::ReadBool(pStr, "");
				nWord = CServerFunction::PopString(pStr, "");
				pStr += nWord;

				if (bAIDeath[nCntAI] == true)
				{
					//キルプレイヤーの番号を代入
					nAIKillPlayerIdx[nCntAI] = CServerFunction::ReadInt(pStr, "");
					nWord = CServerFunction::PopString(pStr, "");
					pStr += nWord;

					//キルプレイヤーの種類を代入
					AIPlayerType[nCntAI] = (TYPE)CServerFunction::ReadInt(pStr, "");
					nWord = CServerFunction::PopString(pStr, "");
					pStr += nWord;
				}
				else
				{
					nAILife[nCntAI] = CServerFunction::ReadInt(pStr, "");
					nWord = CServerFunction::PopString(pStr, "");
					pStr += nWord;
				}

				//弾を発射しているかどうかを代入
				bAIShoot[nCntAI] = CServerFunction::ReadBool(pStr, "");
				nWord = CServerFunction::PopString(pStr, "");
				pStr += nWord;

				if (bAIShoot[nCntAI] == true)
				{//弾を発射している場合
					//攻撃力を代入
					nAIAttack[nCntAI] = CServerFunction::ReadInt(pStr, "");
					nWord = CServerFunction::PopString(pStr, "");
					pStr += nWord;

					//水平角度の代入
					fAIAngle[nCntAI] = CServerFunction::ReadFloat(pStr, "");
					nWord = CServerFunction::PopString(pStr, "");
					pStr += nWord;

					fAIAngleV[nCntAI] = CServerFunction::ReadFloat(pStr, "");
					nWord = CServerFunction::PopString(pStr, "");
					pStr += nWord;
				}

			}

			//ピンを使用してるかどうかを代入
			bPinUse = CServerFunction::ReadBool(pStr, "");
			nWord = CServerFunction::PopString(pStr, "");
			pStr += nWord;

			if (bPinUse == true)
			{// ピンを使用している場合
			 // ピンの位置を代入
				pinPos.x = CServerFunction::ReadFloat(pStr, "");
				nWord = CServerFunction::PopString(pStr, "");
				pStr += nWord;
				pinPos.y = CServerFunction::ReadFloat(pStr, "");
				nWord = CServerFunction::PopString(pStr, "");
				pStr += nWord;
				pinPos.z = CServerFunction::ReadFloat(pStr, "");
				nWord = CServerFunction::PopString(pStr, "");
				pStr += nWord;
			}

			for (int nCntAI = 0; nCntAI < AI_MAX; nCntAI++)
			{// AIの数だけ回る
			 // AIがピンを使用してるかどうかを代入
				bAIPinUse[nCntAI] = CServerFunction::ReadBool(pStr, "");
				nWord = CServerFunction::PopString(pStr, "");
				pStr += nWord;

				if (bAIPinUse[nCntAI] == true)
				{// AIがピンを使用している場合
				 // AIのピンの位置を代入
					AIPinPos[nCntAI].x = CServerFunction::ReadFloat(pStr, "");
					nWord = CServerFunction::PopString(pStr, "");
					pStr += nWord;
					AIPinPos[nCntAI].y = CServerFunction::ReadFloat(pStr, "");
					nWord = CServerFunction::PopString(pStr, "");
					pStr += nWord;
					AIPinPos[nCntAI].z = CServerFunction::ReadFloat(pStr, "");
					nWord = CServerFunction::PopString(pStr, "");
					pStr += nWord;
				}
			}

			if (nPlayerIdx == 0)
			{//ホストの場合
			 //現在の状態を代入
				nState = CServerFunction::ReadInt(pStr, "");
				nWord = CServerFunction::PopString(pStr, "");
				pStr += nWord;

				if (nState == 1)
				{//終了情報
					CResult::SetTeamWin((CResult::TEAM_WIN)CServerFunction::ReadInt(pStr, ""));
				}
				//CPU情報の読み取り
				pStr = ReadCPUData(pStr);
			}

			if (bDeath == true)
			{//死亡していた場合
				m_pPlayer[nPlayerIdx]->GetDeath() = true;	//死亡情報を代入
				if (m_bPlayerDeath[nPlayerIdx] == false)
				{//
					m_bPlayerDeath[nPlayerIdx] = true;		//

					if (m_bPlayerDeath[nPlayerIdx] == true)
					{//死亡している場合
					 //パーティクルの生成
						CParticle::Create(m_pPlayer[nPlayerIdx]->GetModel(0)->GetWorldPos(), 4);
						CParticle::Create(m_pPlayer[nPlayerIdx]->GetModel(0)->GetWorldPos(), 5);

						for (int nCntModel = 0; nCntModel < m_pPlayer[nPlayerIdx]->GetNumParts(); nCntModel++)
						{
							//描画しないようにする
							m_pPlayer[nPlayerIdx]->GetModel(nCntModel)->SetDisp(false);
						}

						for (int nCntAI = 0; nCntAI < AI_MAX; nCntAI++)
						{
							for (int nCntParts = 0; nCntParts < m_pPlayer[nPlayerIdx]->GetMyAI(nCntAI)->GetNumParts(); nCntParts++)
							{
								//AIを表示させない
								m_pPlayer[nPlayerIdx]->GetMyAI(nCntAI)->GetModel(nCntParts)->SetDisp(false);
							}

							//パーティクルを生成
							CParticle::Create(m_pPlayer[nPlayerIdx]->GetMyAI(nCntAI)->GetModel(0)->GetWorldPos(), 4);
							CParticle::Create(m_pPlayer[nPlayerIdx]->GetMyAI(nCntAI)->GetModel(0)->GetWorldPos(), 5);

						}

						//チーム別の処理
						switch (nTeam)
						{
						case 0:
							m_nBlueLinkEnergy -= 30;	//ブルーチームのリンクエネルギー減算
							break;
						case 1:
							m_nRedLinkEnergy -= 30;		//レッドチームのリンクエネルギー減算
							break;
						}

						//キルログの設置処理
						for (int nCntKill = 0; nCntKill < NUM_KILL_LOG; nCntKill++)
						{
							if (CManager::GetGame()->GetLog(nCntKill) == false)
							{
								CManager::GetGame()->SetKillIdx(nCntKill, nKillPlayerIdx);
								CManager::GetGame()->SetDeathIdx(nCntKill, nPlayerIdx);
								CManager::GetGame()->SetPlayerType(0, playerType);
								CManager::GetGame()->SetPlayerType(1, TYPE_PLAYER);
								CManager::GetGame()->SetLog(nCntKill, true);
								break;
							}
						}
					}
				}
			}
			else
			{//プレイヤー
				if (m_bPlayerDeath[nPlayerIdx] == true)
				{
					m_bPlayerDeath[nPlayerIdx] = false;
					m_pPlayer[nPlayerIdx]->GetDeath() = false;
					for (int nCntModel = 0; nCntModel < m_pPlayer[nPlayerIdx]->GetNumParts(); nCntModel++)
					{
						m_pPlayer[nPlayerIdx]->GetModel(nCntModel)->SetDisp(true);
					}

					for (int nCntAI = 0; nCntAI < AI_MAX; nCntAI++)
					{
						for (int nCntParts = 0; nCntParts < m_pPlayer[nPlayerIdx]->GetMyAI(nCntAI)->GetNumParts(); nCntParts++)
						{
							//AIを表示させる
							m_pPlayer[nPlayerIdx]->GetMyAI(nCntAI)->GetModel(nCntParts)->SetDisp(true);
						}
					}
				}
			}

			for (int nCntAI = 0; nCntAI < AI_MAX; nCntAI++)
			{

				if (bAIDeath[nCntAI] == true)
				{//AIが死亡している場合
					m_pPlayer[nPlayerIdx]->GetMyAI(nCntAI)->SetDeath(bAIDeath[nCntAI]);	//AIの死亡設置処理
					if (m_bAIDeath[nCntAI][nPlayerIdx] == false)
					{//
						m_bAIDeath[nCntAI][nPlayerIdx] = true;
						if (m_bAIDeath[nCntAI][nPlayerIdx] == true)
						{//
						 //パーティクルを生成
							CParticle::Create(m_pPlayer[nPlayerIdx]->GetMyAI(nCntAI)->GetModel(0)->GetWorldPos(), 4);
							CParticle::Create(m_pPlayer[nPlayerIdx]->GetMyAI(nCntAI)->GetModel(0)->GetWorldPos(), 5);
							for (int nCntModel = 0; nCntModel < m_pPlayer[nPlayerIdx]->GetMyAI(nCntAI)->GetNumParts(); nCntModel++)
							{//表示しない
								m_pPlayer[nPlayerIdx]->GetMyAI(nCntAI)->GetModel(nCntModel)->SetDisp(false);
							}

							//チーム別処理
							switch (nTeam)
							{
							case 0:
								m_nBlueLinkEnergy -= 10;	//ブルーチームのリンクエネルギー減算
								break;
							case 1:
								m_nRedLinkEnergy -= 10;		//レッドチームのリンクエネルギー減算
								break;
							}

							//キルログの設置処理
							for (int nCntKill = 0; nCntKill < NUM_KILL_LOG; nCntKill++)
							{
								if (CManager::GetGame()->GetLog(nCntKill) == false)
								{
									CManager::GetGame()->SetKillIdx(nCntKill, nAIKillPlayerIdx[nCntAI]);
									CManager::GetGame()->SetDeathIdx(nCntKill, nPlayerIdx);
									CManager::GetGame()->SetPlayerType(0, AIPlayerType[nCntAI]);
									if (m_pPlayer[nPlayerIdx]->GetMyAI(nCntAI)->GetMechaType() == CAIMecha::MECHATYPE_DRONE)
									{
										CManager::GetGame()->SetPlayerType(1, TYPE_DROWN);
									}
									else if (m_pPlayer[nPlayerIdx]->GetMyAI(nCntAI)->GetMechaType() == CAIMecha::MECHATYPE_WORKER)
									{
										CManager::GetGame()->SetPlayerType(1, TYPE_WORKER);
									}
									CManager::GetGame()->SetLog(nCntKill, true);
									break;
								}
							}
						}
					}
				}
				else
				{
					if (m_bAIDeath[nCntAI][nPlayerIdx] == true)
					{
						m_bAIDeath[nCntAI][nPlayerIdx] = false;
						m_pPlayer[nPlayerIdx]->GetMyAI(nCntAI)->SetDeath(bAIDeath[nCntAI]);

						//表示する
						for (int nCntModel = 0; nCntModel < m_pPlayer[nPlayerIdx]->GetMyAI(nCntAI)->GetNumParts(); nCntModel++)
						{
							m_pPlayer[nPlayerIdx]->GetMyAI(nCntAI)->GetModel(nCntModel)->SetDisp(true);
						}
					}
				}
			}
			if (nPlayerIdx != pClient->GetPlayerIdx())
			{//プレイヤーの番号がクライアント番号と違う場合
				if (m_pPlayer[nPlayerIdx] != NULL)
				{
					//チーム情報をの設置処理
					//m_pPlayer[nPlayerIdx]->SetTeam(nTeam);

					SetPlayerData(nPlayerIdx, pos, rot, modelRotUp, modelRotDown, cameraRot);

					//体力の設置処理
					m_pPlayer[nPlayerIdx]->SetLife(nLife);

					for (int nCntAI = 0; nCntAI < AI_MAX; nCntAI++)
					{
						//位置の設置処理
						m_pPlayer[nPlayerIdx]->GetMyAI(nCntAI)->SetPos(AIPos[nCntAI]);

						//向きの設置処理
						m_pPlayer[nPlayerIdx]->GetMyAI(nCntAI)->SetRot(AIRot[nCntAI]);

						//体力の設置処理
						m_pPlayer[nPlayerIdx]->GetMyAI(nCntAI)->SetLife(nAILife[nCntAI]);

						if (bAIShoot[nCntAI] == true)
						{
							D3DXMATRIX mtxCanon = m_pPlayer[nPlayerIdx]->GetMyAI(nCntAI)->GetMtxWorld();

							D3DXVECTOR3 posCanon = D3DXVECTOR3(mtxCanon._41, mtxCanon._42 - 8.0f, mtxCanon._43);
							// 弾の生成
							CBulletPlayer::Create(posCanon, fAIAngle[nCntAI], fAIAngleV[nCntAI], nAIAttack[nCntAI], nTeam, m_pPlayer[nPlayerIdx]->GetMyAI(nCntAI), m_pPlayer[nPlayerIdx]->GetBulletSpeed(), m_pPlayer[nPlayerIdx]->GetBulletLife());

							m_pPlayer[nPlayerIdx]->GetMyAI(nCntAI)->SetShoot(false);
						}
					}

					if (bShoot == true)
					{//弾を発射している場合
						CreatePlayerBullet(nPlayerIdx, nNumShoot, nAttack, cameraRot, pAngle, pAngleV);
					}

					if (bChat == true && m_pPlayer[pClient->GetPlayerIdx()]->GetTeam() == nTeam)
					{//チャットしていてかつ同じチームの場合
						SetChatData(nPlayerIdx, (int)radioChat);
					}

					// ピンの更新
					SetPinData(nPlayerIdx, pinPos,bPinUse);

					for (int nCntAI = 0; nCntAI < AI_MAX; nCntAI++)
					{// AIの数だけ回る
						// AIピンの更新
						SetAIPinData(nPlayerIdx, nCntAI, AIPinPos[nCntAI], bAIPinUse[nCntAI]);
					}

					if (nState == 1)
					{//終了している場合
						m_state = STATE_END;
					}
				}
			}
		}
	}

	if (pAngle != NULL)
	{
		delete[] pAngle;
		pAngle = NULL;
	}
	if (pAngleV != NULL)
	{
		delete[] pAngleV;
		pAngleV = NULL;
	}

	return pStr;
}
//=============================================================================
// CPUプレイヤー情報を読み取る処理
//=============================================================================
char *CGame::ReadCPUData(char *pStr)
{
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER_CONNECT; nCntPlayer++)
	{
		if (CMatching::GetConnect(nCntPlayer) == false)
		{
			D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				//位置
			D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				//向き
			D3DXVECTOR3 modelRotDown = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//モデルの下半身の向き
			D3DXVECTOR3	modelRotUp = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//モデルの上半身の向き
			D3DXVECTOR3 rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			int nPlayerIdx = 0;												//プレイヤーの番号
			int nNumConnect = 0;											//接続総数
			int nWord = 0;													//文字の頭出し用
			int nCntClient = 0;												//クライアントのカウンター
			int nTeam = 0;													//チーム情報
			float *pAngle = NULL;											//水平角度
			float *pAngleV = NULL;											//水平角度
			bool bShoot = false;											//弾を発射しているかどうか
			int nNumShoot = 0;												//同時発射数
			int nAttack = 0;												//攻撃力
			int nLife = 0;													//体力
			int nKillPlayerIdx = 0;											//キルプレイヤーの番号
			TYPE playerType = TYPE_PLAYER;									//キルプレイヤーの種類

			bool bPinUse = false;											// ピンを使用しているかどうか
			D3DXVECTOR3 pinPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// ピンの位置
			bool bAIPinUse[AI_MAX] = { false, false };						// AIがピンを使用しているかどうか
			D3DXVECTOR3 AIPinPos[AI_MAX] = { D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f) };// AIのピンの位置

			D3DXVECTOR3 AIPos[AI_MAX] = { D3DXVECTOR3(0.0f,0.0f,0.0f),D3DXVECTOR3(0.0f,0.0f,0.0f) };		//AIの位置
			D3DXVECTOR3 AIRot[AI_MAX] = { D3DXVECTOR3(0.0f,0.0f,0.0f) ,D3DXVECTOR3(0.0f,0.0f,0.0f) };	//AIの向き
			bool bAIDeath[AI_MAX] = { false ,false };														//AIが死んでいるかどうか
			int nAIKillPlayerIdx[AI_MAX] = { 0,0 };														//AIキルプレイヤーの番号
			TYPE AIPlayerType[AI_MAX] = { TYPE_PLAYER,TYPE_PLAYER };										//AIキルプレイヤーの種類
			bool bAIShoot[AI_MAX] = { false,false };													//AI弾発射したかどうか
			float fAIAngle[AI_MAX] = { 0.0f,0.0f };														//AI角度
			float fAIAngleV[AI_MAX] = { 0.0f,0.0f };													//AI角度
			int nAIAttack[AI_MAX] = { 0, 0 };															//AI攻撃力
			int nAILife[AI_MAX] = { 0,0 };																//AI体力

			if (CServerFunction::Memcmp(pStr, SERVER_AI_PLAYER_DATA) == 0)
			{
				pStr += strlen(SERVER_AI_PLAYER_DATA);

				//番号を代入
				nPlayerIdx = CServerFunction::ReadInt(pStr, "");
				nWord = CServerFunction::PopString(pStr, "");		//文字数カウント
				pStr += nWord;										//頭出し

																	//チーム情報の代入
				nTeam = CServerFunction::ReadInt(pStr, "");
				nWord = CServerFunction::PopString(pStr, "");
				pStr += nWord;

				//プレイヤーの位置を代入
				pos.x = CServerFunction::ReadFloat(pStr, "");
				nWord = CServerFunction::PopString(pStr, "");
				pStr += nWord;
				pos.z = CServerFunction::ReadFloat(pStr, "");
				nWord = CServerFunction::PopString(pStr, "");
				pStr += nWord;

				//プレイヤーの向きを代入
				rot.y = CServerFunction::ReadFloat(pStr, "");
				nWord = CServerFunction::PopString(pStr, "");
				pStr += nWord;

				//モデル0番の向きを代入
				modelRotDown.y = CServerFunction::ReadFloat(pStr, "");
				nWord = CServerFunction::PopString(pStr, "");
				pStr += nWord;

				//モデル1番の向きを代入
				modelRotUp.x = CServerFunction::ReadFloat(pStr, "");
				nWord = CServerFunction::PopString(pStr, "");
				pStr += nWord;
				modelRotUp.y = CServerFunction::ReadFloat(pStr, "");
				nWord = CServerFunction::PopString(pStr, "");
				pStr += nWord;
				modelRotUp.z = CServerFunction::ReadFloat(pStr, "");
				nWord = CServerFunction::PopString(pStr, "");
				pStr += nWord;

				//モデル1番の向きを代入
				rotDest.x = CServerFunction::ReadFloat(pStr, "");
				nWord = CServerFunction::PopString(pStr, "");
				pStr += nWord;
				rotDest.y = CServerFunction::ReadFloat(pStr, "");
				nWord = CServerFunction::PopString(pStr, "");
				pStr += nWord;
				rotDest.z = CServerFunction::ReadFloat(pStr, "");
				nWord = CServerFunction::PopString(pStr, "");
				pStr += nWord;

				//死亡しているかどうかを代入
				bool bDeath = CServerFunction::ReadBool(pStr, "");
				nWord = CServerFunction::PopString(pStr, "");
				pStr += nWord;

				if (bDeath == false)
				{//死亡していない場合
				 //体力の代入
					nLife = CServerFunction::ReadInt(pStr, "");
					nWord = CServerFunction::PopString(pStr, "");
					pStr += nWord;
				}
				else
				{
					nKillPlayerIdx = CServerFunction::ReadInt(pStr, "");
					nWord = CServerFunction::PopString(pStr, "");
					pStr += nWord;

					//キルプレイヤーの種類を代入
					playerType = (TYPE)CServerFunction::ReadInt(pStr, "");
					nWord = CServerFunction::PopString(pStr, "");
					pStr += nWord;
				}

				//弾を発射しているかどうかを代入
				bShoot = CServerFunction::ReadBool(pStr, "");
				nWord = CServerFunction::PopString(pStr, "");
				pStr += nWord;

				if (bShoot == true)
				{//弾を発射している場合
				 //弾の同時発射数を代入
					nNumShoot = CServerFunction::ReadInt(pStr, "");
					nWord = CServerFunction::PopString(pStr, "");
					pStr += nWord;

					//水平角度の動的確保
					pAngle = new float[nNumShoot * 2];
					pAngleV = new float[nNumShoot * 2];

					//攻撃力を代入
					nAttack = CServerFunction::ReadInt(pStr, "");
					nWord = CServerFunction::PopString(pStr, "");
					pStr += nWord;

					//水平角度を代入
					for (int nCntShoot = 0; nCntShoot < nNumShoot * 2; nCntShoot++)
					{
						pAngle[nCntShoot] = CServerFunction::ReadFloat(pStr, "");
						nWord = CServerFunction::PopString(pStr, "");
						pStr += nWord;

						pAngleV[nCntShoot] = CServerFunction::ReadFloat(pStr, "");
						nWord = CServerFunction::PopString(pStr, "");
						pStr += nWord;

					}
				}

				for (int nCntAI = 0; nCntAI < AI_MAX; nCntAI++)
				{
					//AIの位置を代入
					AIPos[nCntAI].x = CServerFunction::ReadFloat(pStr, "");
					nWord = CServerFunction::PopString(pStr, "");
					pStr += nWord;
					AIPos[nCntAI].y = CServerFunction::ReadFloat(pStr, "");
					nWord = CServerFunction::PopString(pStr, "");
					pStr += nWord;
					AIPos[nCntAI].z = CServerFunction::ReadFloat(pStr, "");
					nWord = CServerFunction::PopString(pStr, "");
					pStr += nWord;

					//AIの向きを代入
					AIRot[nCntAI].y = CServerFunction::ReadFloat(pStr, "");
					nWord = CServerFunction::PopString(pStr, "");
					pStr += nWord;

					//AIの死亡しているかどうかを代入
					bAIDeath[nCntAI] = CServerFunction::ReadBool(pStr, "");
					nWord = CServerFunction::PopString(pStr, "");
					pStr += nWord;

					if (bAIDeath[nCntAI] == true)
					{
						//キルプレイヤーの番号を代入
						nAIKillPlayerIdx[nCntAI] = CServerFunction::ReadInt(pStr, "");
						nWord = CServerFunction::PopString(pStr, "");
						pStr += nWord;

						//キルプレイヤーの種類を代入
						AIPlayerType[nCntAI] = (TYPE)CServerFunction::ReadInt(pStr, "");
						nWord = CServerFunction::PopString(pStr, "");
						pStr += nWord;
					}
					else
					{
						nAILife[nCntAI] = CServerFunction::ReadInt(pStr, "");
						nWord = CServerFunction::PopString(pStr, "");
						pStr += nWord;
					}

					//弾を発射しているかどうかを代入
					bAIShoot[nCntAI] = CServerFunction::ReadBool(pStr, "");
					nWord = CServerFunction::PopString(pStr, "");
					pStr += nWord;

					if (bAIShoot[nCntAI] == true)
					{//弾を発射している場合
					 //攻撃力を代入
						nAIAttack[nCntAI] = CServerFunction::ReadInt(pStr, "");
						nWord = CServerFunction::PopString(pStr, "");
						pStr += nWord;

						//水平角度の代入
						fAIAngle[nCntAI] = CServerFunction::ReadFloat(pStr, "");
						nWord = CServerFunction::PopString(pStr, "");
						pStr += nWord;

						fAIAngleV[nCntAI] = CServerFunction::ReadFloat(pStr, "");
						nWord = CServerFunction::PopString(pStr, "");
						pStr += nWord;
					}
				}

				//ピンを使用してるかどうかを代入
				bPinUse = CServerFunction::ReadBool(pStr, "");
				nWord = CServerFunction::PopString(pStr, "");
				pStr += nWord;

				if (bPinUse == true)
				{// ピンを使用している場合
				 // ピンの位置を代入
					pinPos.x = CServerFunction::ReadFloat(pStr, "");
					nWord = CServerFunction::PopString(pStr, "");
					pStr += nWord;
					pinPos.y = CServerFunction::ReadFloat(pStr, "");
					nWord = CServerFunction::PopString(pStr, "");
					pStr += nWord;
					pinPos.z = CServerFunction::ReadFloat(pStr, "");
					nWord = CServerFunction::PopString(pStr, "");
					pStr += nWord;
				}

				for (int nCntAI = 0; nCntAI < AI_MAX; nCntAI++)
				{// AIの数だけ回る
				 // AIがピンを使用してるかどうかを代入
					bAIPinUse[nCntAI] = CServerFunction::ReadBool(pStr, "");
					nWord = CServerFunction::PopString(pStr, "");
					pStr += nWord;

					if (bAIPinUse[nCntAI] == true)
					{// AIがピンを使用している場合
					 // AIのピンの位置を代入
						AIPinPos[nCntAI].x = CServerFunction::ReadFloat(pStr, "");
						nWord = CServerFunction::PopString(pStr, "");
						pStr += nWord;
						AIPinPos[nCntAI].y = CServerFunction::ReadFloat(pStr, "");
						nWord = CServerFunction::PopString(pStr, "");
						pStr += nWord;
						AIPinPos[nCntAI].z = CServerFunction::ReadFloat(pStr, "");
						nWord = CServerFunction::PopString(pStr, "");
						pStr += nWord;
					}
				}

				if (bDeath == true)
				{//死亡している場合
					m_pPlayer[nPlayerIdx]->GetDeath() = true;
					if (m_bPlayerDeath[nPlayerIdx] == false)
					{
						m_bPlayerDeath[nPlayerIdx] = true;
						if (m_bPlayerDeath[nPlayerIdx] == true)
						{
							//パーティクルの生成
							CParticle::Create(m_pPlayer[nPlayerIdx]->GetModel(0)->GetWorldPos(), 4);
							CParticle::Create(m_pPlayer[nPlayerIdx]->GetModel(0)->GetWorldPos(), 5);

							for (int nCntModel = 0; nCntModel < m_pPlayer[nPlayerIdx]->GetNumParts(); nCntModel++)
							{
								//描画しないようにする
								m_pPlayer[nPlayerIdx]->GetModel(nCntModel)->SetDisp(false);
							}

							for (int nCntAI = 0; nCntAI < AI_MAX; nCntAI++)
							{
								for (int nCntParts = 0; nCntParts < m_pPlayer[nPlayerIdx]->GetMyAI(nCntAI)->GetNumParts(); nCntParts++)
								{
									//AIを表示させない
									m_pPlayer[nPlayerIdx]->GetMyAI(nCntAI)->GetModel(nCntParts)->SetDisp(false);
								}

								//パーティクルを生成
								CParticle::Create(m_pPlayer[nPlayerIdx]->GetMyAI(nCntAI)->GetModel(0)->GetWorldPos(), 4);
								CParticle::Create(m_pPlayer[nPlayerIdx]->GetMyAI(nCntAI)->GetModel(0)->GetWorldPos(), 5);

							}

							switch (nTeam)
							{
							case 0:
								m_nBlueLinkEnergy -= 30;	//ブルーチームのリンクエネルギー減算
								break;
							case 1:
								m_nRedLinkEnergy -= 30;		//レッドチームのリンクエネルギー減算
								break;
							}

							//キルログの設置処理
							for (int nCntKill = 0; nCntKill < NUM_KILL_LOG; nCntKill++)
							{
								if (CManager::GetGame()->GetLog(nCntKill) == false)
								{
									CManager::GetGame()->SetKillIdx(nCntKill, nKillPlayerIdx);
									CManager::GetGame()->SetDeathIdx(nCntKill, nPlayerIdx);
									CManager::GetGame()->SetPlayerType(0, playerType);
									CManager::GetGame()->SetPlayerType(1, TYPE_PLAYER);
									CManager::GetGame()->SetLog(nCntKill, true);
									break;
								}
							}
						}
					}
				}
				else
				{//死亡していない場合
					m_bPlayerDeath[nPlayerIdx] = false;
				}

				for (int nCntAI = 0; nCntAI < AI_MAX; nCntAI++)
				{

					if (bAIDeath[nCntAI] == true)
					{//AIが死亡している場合
						m_pPlayer[nPlayerIdx]->GetMyAI(nCntAI)->SetDeath(bAIDeath[nCntAI]);	//AIの死亡設置処理
						if (m_bAIDeath[nCntAI][nPlayerIdx] == false)
						{//
							m_bAIDeath[nCntAI][nPlayerIdx] = true;
							if (m_bAIDeath[nCntAI][nPlayerIdx] == true)
							{//
							 //パーティクルを生成
								CParticle::Create(m_pPlayer[nPlayerIdx]->GetMyAI(nCntAI)->GetModel(0)->GetWorldPos(), 4);
								CParticle::Create(m_pPlayer[nPlayerIdx]->GetMyAI(nCntAI)->GetModel(0)->GetWorldPos(), 5);
								for (int nCntModel = 0; nCntModel < m_pPlayer[nPlayerIdx]->GetMyAI(nCntAI)->GetNumParts(); nCntModel++)
								{//表示しない
									m_pPlayer[nPlayerIdx]->GetMyAI(nCntAI)->GetModel(nCntModel)->SetDisp(false);
								}

								//チーム別処理
								switch (nTeam)
								{
								case 0:
									m_nBlueLinkEnergy -= 10;	//ブルーチームのリンクエネルギー減算
									break;
								case 1:
									m_nRedLinkEnergy -= 10;		//レッドチームのリンクエネルギー減算
									break;
								}

								//キルログの設置処理
								for (int nCntKill = 0; nCntKill < NUM_KILL_LOG; nCntKill++)
								{
									if (CManager::GetGame()->GetLog(nCntKill) == false)
									{
										CManager::GetGame()->SetKillIdx(nCntKill, nAIKillPlayerIdx[nCntAI]);
										CManager::GetGame()->SetDeathIdx(nCntKill, nPlayerIdx);
										CManager::GetGame()->SetPlayerType(0, AIPlayerType[nCntAI]);
										if (m_pPlayer[nPlayerIdx]->GetMyAI(nCntAI)->GetMechaType() == CAIMecha::MECHATYPE_DRONE)
										{
											CManager::GetGame()->SetPlayerType(1, TYPE_DROWN);
										}
										else if (m_pPlayer[nPlayerIdx]->GetMyAI(nCntAI)->GetMechaType() == CAIMecha::MECHATYPE_WORKER)
										{
											CManager::GetGame()->SetPlayerType(1, TYPE_WORKER);
										}
										CManager::GetGame()->SetLog(nCntKill, true);
										break;
									}
								}
							}
						}
					}
					else
					{
						if (m_bAIDeath[nCntAI][nPlayerIdx] == true)
						{
							m_bAIDeath[nCntAI][nPlayerIdx] = false;
							m_pPlayer[nPlayerIdx]->GetMyAI(nCntAI)->SetDeath(bAIDeath[nCntAI]);

							//表示する
							for (int nCntModel = 0; nCntModel < m_pPlayer[nPlayerIdx]->GetMyAI(nCntAI)->GetNumParts(); nCntModel++)
							{
								m_pPlayer[nPlayerIdx]->GetMyAI(nCntAI)->GetModel(nCntModel)->SetDisp(true);
							}
						}
					}
				}

				//CPUプレイヤーの情報を設置処理
				SetCPUData(nPlayerIdx, pos, rot);

				// ピンの更新
				SetPinData(nPlayerIdx, pinPos, bPinUse);

				for (int nCntAI = 0; nCntAI < AI_MAX; nCntAI++)
				{// AIの数だけ回る
				 // AIピンの更新
					SetAIPinData(nPlayerIdx, nCntAI, AIPinPos[nCntAI], bAIPinUse[nCntAI]);
				}

				//// 角度を求める
				//D3DXVECTOR3 rotCamera = m_pPlayer[nPlayerIdx]->GetDestUpper();
				//float fDiffRot;
				//float fAngle = D3DX_PI + rotCamera.y;
				//float fRotDest = m_pPlayer[nPlayerIdx]->GetRotDest();

				//// 目的の角度の調節
				//if (fRotDest > D3DX_PI) { fRotDest -= D3DX_PI * 2.0f; }
				//if (fRotDest < -D3DX_PI) { fRotDest += D3DX_PI * 2.0f; }

				//// 目的の角度への差分
				//fDiffRot = fRotDest - rot.y;
				//if (fDiffRot > D3DX_PI) { fDiffRot -= D3DX_PI * 2.0f; }
				//if (fDiffRot < -D3DX_PI) { fDiffRot += D3DX_PI * 2.0f; }

				//// 角度の更新
				//rot.y += fDiffRot * 0.05f;
				//if (rot.y > D3DX_PI) { rot.y -= D3DX_PI * 2.0f; }
				//if (rot.y < -D3DX_PI) { rot.y += D3DX_PI * 2.0f; }

				//float cameraAngle = fAngle - rot.y;

				//// 差分の調節
				//if (cameraAngle > D3DX_PI) { cameraAngle -= D3DX_PI * 2.0f; }
				//if (cameraAngle < -D3DX_PI) { cameraAngle += D3DX_PI * 2.0f; }

				//if (fRotDest <= D3DX_PI * 0.5f && fRotDest >= D3DX_PI * -0.5f)
				//{// 下半身の動きを進行方向に合わせる
				//	D3DXVECTOR3 modelRot =m_pPlayer[nPlayerIdx]->GetModel(0)->GetRot();
				//	m_pPlayer[nPlayerIdx]->GetModel(0)->SetRot(D3DXVECTOR3(modelRot.x, rot.y + cameraAngle, modelRot.z));
				//	modelRot = m_pPlayer[nPlayerIdx]->GetModel(0)->GetRot();
				//	modelRot = D3DXVECTOR3(-rotCamera.x + (D3DX_PI * 0.5f), cameraAngle - fAngle, modelRot.z);
				//	m_pPlayer[nPlayerIdx]->GetModel(0)->SetRot(modelRot);
				//}
				//else
				//{// 斜め後ろ向きのとき
				//	D3DXVECTOR3 modelRot = m_pPlayer[nPlayerIdx]->GetModel(1)->GetRot();
				//	modelRot = D3DXVECTOR3(-rotCamera.x + (D3DX_PI * 0.5f), cameraAngle - (fAngle - D3DX_PI), modelRot.z);
				//	m_pPlayer[nPlayerIdx]->GetModel(1)->SetRot(modelRot);
				//}

				float fDiffRot;
				float fAngle = D3DX_PI + rotDest.y;
				float fRotDest = m_pPlayer[nPlayerIdx]->GetRotDest();

				// 目的の角度の調節
				if (fRotDest > D3DX_PI) { fRotDest -= D3DX_PI * 2.0f; }
				if (fRotDest < -D3DX_PI) { fRotDest += D3DX_PI * 2.0f; }

				// 目的の角度への差分
				fDiffRot = fRotDest - rot.y;
				if (fDiffRot > D3DX_PI) { fDiffRot -= D3DX_PI * 2.0f; }
				if (fDiffRot < -D3DX_PI) { fDiffRot += D3DX_PI * 2.0f; }

				// 角度の更新
				rot.y += fDiffRot * 0.05f;
				if (rot.y > D3DX_PI) { rot.y -= D3DX_PI * 2.0f; }
				if (rot.y < -D3DX_PI) { rot.y += D3DX_PI * 2.0f; }

				float fCameraAngle = fAngle - rot.y;

				// 差分の調節
				if (fCameraAngle > D3DX_PI) { fCameraAngle -= D3DX_PI * 2.0f; }
				if (fCameraAngle < -D3DX_PI) { fCameraAngle += D3DX_PI * 2.0f; }

				if (fRotDest <= D3DX_PI * 0.5f && fRotDest >= D3DX_PI * -0.5f)
				{// 下半身の動きを進行方向に合わせる
					m_pPlayer[nPlayerIdx]->GetModel(0)->SetRot(D3DXVECTOR3(modelRotDown.x, rot.y + fCameraAngle, modelRotDown.z));
					m_pPlayer[nPlayerIdx]->GetModel(1)->SetRot(D3DXVECTOR3(-rotDest.x + (D3DX_PI * 0.5f), fCameraAngle - fAngle, modelRotUp.z));
				}
				else
				{// 斜め後ろ向きのとき
					m_pPlayer[nPlayerIdx]->GetModel(1)->SetRot(D3DXVECTOR3(-rotDest.x + (D3DX_PI * 0.5f), fCameraAngle - (fAngle - D3DX_PI), modelRotUp.z));
				}

				//プレイヤーの向きの設置処理
				m_pPlayer[nPlayerIdx]->SetRot(rot);

				m_pPlayer[nPlayerIdx]->SetDestUpper(rotDest);

				//プレイヤーの目的の角度の設置処理
				//m_pPlayer[nPlayerIdx]->SetRotDest(fRotDest);

				//体力の設置処理
				m_pPlayer[nPlayerIdx]->SetLife(nLife);

				if (bShoot == true)
				{//CPUプレイヤーの弾生成処理
					CreateCPUBullet(nPlayerIdx, nNumShoot, nAttack, rotDest, pAngle, pAngleV);
				}

				for (int nCntAI = 0; nCntAI < AI_MAX; nCntAI++)
				{
					m_pPlayer[nPlayerIdx]->GetMyAI(nCntAI)->SetPos(AIPos[nCntAI]);
					m_pPlayer[nPlayerIdx]->GetMyAI(nCntAI)->SetRot(AIRot[nCntAI]);
					m_pPlayer[nPlayerIdx]->GetMyAI(nCntAI)->SetLife(nAILife[nCntAI]);
					if (bAIShoot[nCntAI] == true)
					{
						D3DXMATRIX mtxCanon = m_pPlayer[nPlayerIdx]->GetMyAI(nCntAI)->GetMtxWorld();

						D3DXVECTOR3 posCanon = D3DXVECTOR3(mtxCanon._41, mtxCanon._42 - 8.0f, mtxCanon._43);
						// 弾の生成
						CBulletPlayer::Create(posCanon, fAIAngle[nCntAI], fAIAngleV[nCntAI], nAIAttack[nCntAI], nTeam, m_pPlayer[nPlayerIdx]->GetMyAI(nCntAI), m_pPlayer[nPlayerIdx]->GetBulletSpeed(), m_pPlayer[nPlayerIdx]->GetBulletLife());

						m_pPlayer[nPlayerIdx]->GetMyAI(nCntAI)->SetShoot(false);
					}
				}


			}

			//水平角度の破棄
			if (pAngle != NULL)
			{
				delete[] pAngle;
				pAngle = NULL;
			}
			if (pAngleV != NULL)
			{
				delete[] pAngleV;
				pAngleV = NULL;
			}

		}
	}
	return pStr;
}

//=============================================================================
// プレイヤーの情報の設置処理
//=============================================================================
void CGame::SetPlayerData(int nPlayerIdx, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 modelRotUp, D3DXVECTOR3 modelRotDown, D3DXVECTOR3 cameraRot)
{
	//プレイヤーの位置の設置処理
	m_pPlayer[nPlayerIdx]->SetPos(pos);

	float fDiffRot;
	float fAngle = D3DX_PI + cameraRot.y;
	float fRotDest = m_pPlayer[nPlayerIdx]->GetRotDest();
	// 目的の角度の調節
	if (fRotDest > D3DX_PI) { fRotDest -= D3DX_PI * 2.0f; }
	if (fRotDest < -D3DX_PI) { fRotDest += D3DX_PI * 2.0f; }

	// 目的の角度への差分
	fDiffRot = fRotDest - rot.y;
	if (fDiffRot > D3DX_PI) { fDiffRot -= D3DX_PI * 2.0f; }
	if (fDiffRot < -D3DX_PI) { fDiffRot += D3DX_PI * 2.0f; }

	// 角度の更新
	rot.y += fDiffRot * 0.05f;
	if (rot.y > D3DX_PI) { rot.y -= D3DX_PI * 2.0f; }
	if (rot.y < -D3DX_PI) { rot.y += D3DX_PI * 2.0f; }

	float fCameraAngle = fAngle - rot.y;

	// 差分の調節
	if (fCameraAngle > D3DX_PI) { fCameraAngle -= D3DX_PI * 2.0f; }
	if (fCameraAngle < -D3DX_PI) { fCameraAngle += D3DX_PI * 2.0f; }

	if (fRotDest <= D3DX_PI * 0.5f && fRotDest >= D3DX_PI * -0.5f)
	{// 下半身の動きを進行方向に合わせる
		m_pPlayer[nPlayerIdx]->GetModel(0)->SetRot(D3DXVECTOR3(modelRotDown.x, rot.y + fCameraAngle, modelRotDown.z));
		m_pPlayer[nPlayerIdx]->GetModel(1)->SetRot(D3DXVECTOR3(-cameraRot.x + (D3DX_PI * 0.5f), fCameraAngle - fAngle, modelRotUp.z));
	}
	else
	{// 斜め後ろ向きのとき
		m_pPlayer[nPlayerIdx]->GetModel(1)->SetRot(D3DXVECTOR3(-cameraRot.x + (D3DX_PI * 0.5f), fCameraAngle - (fAngle - D3DX_PI), modelRotUp.z));
	}

	//プレイヤーの向きの設置処理
	m_pPlayer[nPlayerIdx]->SetRot(rot);

	//プレイヤーの目的の角度の設置処理
	m_pPlayer[nPlayerIdx]->SetRotDest(fRotDest);
}

//=============================================================================
// プレイヤーの弾の生成処理
//=============================================================================
void CGame::CreatePlayerBullet(int nPlayerIdx, int nNumShoot, int nAttack, D3DXVECTOR3 cameraRot, float *pAngle, float *pAngleV)
{
	for (int nCntShoot = 0; nCntShoot < nNumShoot; nCntShoot++)
	{
		// 弾の生成
		D3DXMATRIX mtxCanon = m_pPlayer[nPlayerIdx]->GetModel(2)->GetMtxWorld();
		D3DXVECTOR3 posCanon = D3DXVECTOR3(mtxCanon._41, mtxCanon._42, mtxCanon._43) + D3DXVECTOR3(sinf(cameraRot.y) * 30.0f, cosf(cameraRot.x) * 30.0f, cosf(cameraRot.y) * 30.0f);
		CBulletPlayer::Create(posCanon, pAngle[nCntShoot * 2], pAngleV[nCntShoot * 2], nAttack, m_pPlayer[nPlayerIdx]->GetTeam(), m_pPlayer[nPlayerIdx], m_pPlayer[nPlayerIdx]->GetBulletSpeed(), m_pPlayer[nPlayerIdx]->GetBulletLife());
		mtxCanon = m_pPlayer[nPlayerIdx]->GetModel(3)->GetMtxWorld();
		posCanon = D3DXVECTOR3(mtxCanon._41, mtxCanon._42, mtxCanon._43) + D3DXVECTOR3(sinf(cameraRot.y) * 30.0f, cosf(cameraRot.x) * 30.0f, cosf(cameraRot.y) * 30.0f);
		CBulletPlayer::Create(posCanon, pAngle[nCntShoot * 2 + 1], pAngleV[nCntShoot * 2 + 1], nAttack, m_pPlayer[nPlayerIdx]->GetTeam(), m_pPlayer[nPlayerIdx], m_pPlayer[nPlayerIdx]->GetBulletSpeed(), m_pPlayer[nPlayerIdx]->GetBulletLife());

		//弾を発射しているかどうかの設置処理
		m_pPlayer[nPlayerIdx]->SetShoot(false);
	}
}

//=============================================================================
// チャット情報の設置処理
//=============================================================================
void CGame::SetChatData(int nPlayerIdx, int radioChat)
{
	//プレイヤー番号別で処理分け
	switch (nPlayerIdx)
	{
	case 0:
		if (m_pPlayer[1]->GetAllyChat() == false)
		{
			m_pPlayer[1]->SetAllyRadioChat((CPlayer::RADIOCHAT)radioChat);
			m_pPlayer[1]->SetAllyChat(true);
		}
		break;
	case 1:
		if (m_pPlayer[0]->GetAllyChat() == false)
		{
			m_pPlayer[0]->SetAllyRadioChat((CPlayer::RADIOCHAT)radioChat);
			m_pPlayer[0]->SetAllyChat(true);
		}
		break;
	case 2:
		if (m_pPlayer[3]->GetAllyChat() == false)
		{
			m_pPlayer[3]->SetAllyRadioChat((CPlayer::RADIOCHAT)radioChat);
			m_pPlayer[3]->SetAllyChat(true);
		}
		break;
	case 3:
		if (m_pPlayer[2]->GetAllyChat() == false)
		{
			m_pPlayer[2]->SetAllyRadioChat((CPlayer::RADIOCHAT)radioChat);
			m_pPlayer[2]->SetAllyChat(true);
		}
		break;
	}
}

//=============================================================================
// ピン情報の設置処理
//=============================================================================
void CGame::SetPinData(int nPlayerIdx, D3DXVECTOR3 pinPos, bool use)
{
	// プレイヤー番号別で処理分け
	switch (nPlayerIdx)
	{
	case 0:
		// プレイヤー0
		m_pPlayer[1]->GetAllyPinPos() = pinPos;
		m_pPlayer[1]->GetAllyPinUse() = use;
		break;
	case 1:
		// プレイヤー1
		m_pPlayer[0]->GetAllyPinPos() = pinPos;
		m_pPlayer[0]->GetAllyPinUse() = use;
		break;
	case 2:
		// プレイヤー2
		m_pPlayer[3]->GetAllyPinPos() = pinPos;
		m_pPlayer[3]->GetAllyPinUse() = use;
		break;
	case 3:
		// プレイヤー3
		m_pPlayer[2]->GetAllyPinPos() = pinPos;
		m_pPlayer[2]->GetAllyPinUse() = use;
		break;
	}
}

//=============================================================================
// ピン情報の設置処理
//=============================================================================
void CGame::SetAIPinData(int nPlayerIdx, int AIInd, D3DXVECTOR3 AIPinPos, bool use)
{
	// プレイヤー番号別で処理分け
	switch (nPlayerIdx)
	{
	case 0:
		// プレイヤー0
		m_pPlayer[1]->GetAllyAIPinPos(AIInd) = AIPinPos;
		m_pPlayer[1]->GetAllyAIPinUse(AIInd) = use;
		break;
	case 1:
		// プレイヤー1
		m_pPlayer[0]->GetAllyAIPinPos(AIInd) = AIPinPos;
		m_pPlayer[0]->GetAllyAIPinUse(AIInd) = use;
		break;
	case 2:
		// プレイヤー2
		m_pPlayer[3]->GetAllyAIPinPos(AIInd) = AIPinPos;
		m_pPlayer[3]->GetAllyAIPinUse(AIInd) = use;
		break;
	case 3:
		// プレイヤー3
		m_pPlayer[2]->GetAllyAIPinPos(AIInd) = AIPinPos;
		m_pPlayer[2]->GetAllyAIPinUse(AIInd) = use;
		break;
	}
}


//=============================================================================
// CPUプレイヤーの情報設置処理
//=============================================================================
void CGame::SetCPUData(int nPlayerIdx, D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	//プレイヤーの位置の設置処理
	m_pPlayer[nPlayerIdx]->SetPos(pos);
	//プレイヤーの向きの設置処理
	m_pPlayer[nPlayerIdx]->SetRot(rot);
}

//=============================================================================
// CPUプレイヤーの弾の生成処理
//=============================================================================
void CGame::CreateCPUBullet(int nPlayerIdx, int nNumShoot, int nAttack, D3DXVECTOR3 cameraRot, float *pAngle, float *pAngleV)
{
	for (int nCntShoot = 0; nCntShoot < nNumShoot; nCntShoot++)
	{
		// 弾の生成
		D3DXMATRIX mtxCanon = m_pPlayer[nPlayerIdx]->GetModel(2)->GetMtxWorld();
		D3DXVECTOR3 posCanon = D3DXVECTOR3(mtxCanon._41, mtxCanon._42, mtxCanon._43) + D3DXVECTOR3(sinf(cameraRot.y) * 30.0f, cosf(cameraRot.x) * 30.0f, cosf(cameraRot.y) * 30.0f);
		CBulletPlayer::Create(posCanon, pAngle[nCntShoot * 2], pAngleV[nCntShoot * 2], nAttack, m_pPlayer[nPlayerIdx]->GetTeam(), m_pPlayer[nPlayerIdx], m_pPlayer[nPlayerIdx]->GetBulletSpeed(), m_pPlayer[nPlayerIdx]->GetBulletLife());
		mtxCanon = m_pPlayer[nPlayerIdx]->GetModel(3)->GetMtxWorld();
		posCanon = D3DXVECTOR3(mtxCanon._41, mtxCanon._42, mtxCanon._43) + D3DXVECTOR3(sinf(cameraRot.y) * 30.0f, cosf(cameraRot.x) * 30.0f, cosf(cameraRot.y) * 30.0f);
		CBulletPlayer::Create(posCanon, pAngle[nCntShoot * 2 + 1], pAngleV[nCntShoot * 2 + 1], nAttack, m_pPlayer[nPlayerIdx]->GetTeam(), m_pPlayer[nPlayerIdx], m_pPlayer[nPlayerIdx]->GetBulletSpeed(), m_pPlayer[nPlayerIdx]->GetBulletLife());

		//弾を発射しているかどうかの設置処理
		m_pPlayer[nPlayerIdx]->SetShoot(false);
	}
}

//=============================================================================
// リスポーン位置の読み込み
//=============================================================================
void CGame::LoadRespawnPos(void)
{
	FILE *pFile;	//ファイルのポインタ

					//ファイルの読み込み
	pFile = fopen(GAME_INI, "r");

	if (pFile == NULL)
	{//ファイルが読み込めていない場合
		return;
	}
	else if (pFile != NULL)
	{//ファイルが読み込めた場合
		char aStr[128] = "\0";

		while (1)
		{//ループ
			fscanf(pFile, "%s", aStr);
			if (strcmp(aStr, GAME_SCRIPT) == 0)
			{//スクリプトの開始を示している場合
				while (strcmp(aStr, GAME_END_SCRIPT) != 0)
				{//スクリプトの終了を示すまでループ
					fscanf(pFile, "%s", aStr);
					if (strcmp(aStr, GAME_BLUE) == 0)
					{//サーバーのIPアドレスを示している場合
						while (strcmp(aStr, GAME_BLUE_END) != 0)
						{
							fscanf(pFile, "%s", aStr);

							if (strcmp(aStr, GAME_SPAWN_A) == 0)
							{
								fscanf(pFile, " = %f %f %f", &m_aRespawnPos[0][0].x, &m_aRespawnPos[0][0].y, &m_aRespawnPos[0][0].z);
							}
							if (strcmp(aStr, GAME_SPAWN_B) == 0)
							{
								fscanf(pFile, " = %f %f %f", &m_aRespawnPos[0][1].x, &m_aRespawnPos[0][1].y, &m_aRespawnPos[0][1].z);
							}
							if (strcmp(aStr, GAME_SPAWN_C) == 0)
							{
								fscanf(pFile, " = %f %f %f", &m_aRespawnPos[0][2].x, &m_aRespawnPos[0][2].y, &m_aRespawnPos[0][2].z);
							}
							if (strcmp(aStr, GAME_SPAWN_D) == 0)
							{
								fscanf(pFile, " = %f %f %f", &m_aRespawnPos[0][3].x, &m_aRespawnPos[0][3].y, &m_aRespawnPos[0][3].z);
							}
						}
					}
					if (strcmp(aStr, GAME_RED) == 0)
					{//サーバーのIPアドレスを示している場合
						while (strcmp(aStr, GAME_RED_END) != 0)
						{
							fscanf(pFile, "%s", aStr);

							if (strcmp(aStr, GAME_SPAWN_A) == 0)
							{
								fscanf(pFile, " = %f %f %f", &m_aRespawnPos[1][0].x, &m_aRespawnPos[1][0].y, &m_aRespawnPos[1][0].z);
							}
							if (strcmp(aStr, GAME_SPAWN_B) == 0)
							{
								fscanf(pFile, " = %f %f %f", &m_aRespawnPos[1][1].x, &m_aRespawnPos[1][1].y, &m_aRespawnPos[1][1].z);
							}
							if (strcmp(aStr, GAME_SPAWN_C) == 0)
							{
								fscanf(pFile, " = %f %f %f", &m_aRespawnPos[1][2].x, &m_aRespawnPos[1][2].y, &m_aRespawnPos[1][2].z);
							}
							if (strcmp(aStr, GAME_SPAWN_D) == 0)
							{
								fscanf(pFile, " = %f %f %f", &m_aRespawnPos[1][3].x, &m_aRespawnPos[1][3].y, &m_aRespawnPos[1][3].z);
							}
						}
					}

				}
				break;
			}
		}
	}

}

//=============================================================================
// キルログの生成処理
//=============================================================================
void CGame::CreateKillLog(void)
{
	//if (m_apKillLogBase[0] == NULL)
	//{
	//	m_apKillLogBase[0] = CUI_TEXTURE::Create(D3DXVECTOR3(1125.0f, 30.0f, 0.0f), 250.0f, 50.0f, CUI_TEXTURE::UIFLAME_KILL_LOG_BG);
	//	m_apKillLogBase[0]->SetTex(0, 1, 2);

	//	m_apKillLogPlayerIcon[0][0] = CUI_TEXTURE::Create(D3DXVECTOR3(1035.0f, 30.0f, 0.0f), 45.0f, 45.0f, CUI_TEXTURE::UIFLAME_KILL_LOG_PLAYERICON);
	//	m_apKillLogPlayerIcon[0][0]->SetTex(m_aMechaType[m_nKillIdx[0]], 1, 6);
	//	m_apKillLogPlayerIcon[0][1] = CUI_TEXTURE::Create(D3DXVECTOR3(1215.0f, 30.0f, 0.0f), 45.0f, 45.0f, CUI_TEXTURE::UIFLAME_KILL_LOG_PLAYERICON);
	//	m_apKillLogPlayerIcon[0][1]->SetTex(m_aMechaType[m_nDeathIdx[0]], 1, 6);

	//	m_apKillLogPlayerIdx[0][0] = CUI_TEXTURE::Create(D3DXVECTOR3(1070.0f, 40.0f, 0.0f), 35.0f, 35.0f, CUI_TEXTURE::UIFLAME_KILL_LOG_PLAYERIDX);
	//	m_apKillLogPlayerIdx[0][0]->SetTex(m_nKillIdx[0], 1, 4);

	//	m_apKillLogPlayerIdx[0][1] = CUI_TEXTURE::Create(D3DXVECTOR3(1175.0f, 40.0f, 0.0f), 35.0f, 35.0f, CUI_TEXTURE::UIFLAME_KILL_LOG_PLAYERIDX);
	//	m_apKillLogPlayerIdx[0][1]->SetTex(m_nDeathIdx[0], 1, 4);

	//}

	for (int nCntLog = 0; nCntLog < NUM_KILL_LOG; nCntLog++)
	{
		if (m_bLog[nCntLog] == true && m_apKillLogBase[nCntLog] == NULL && m_apKillLogPlayerIcon[nCntLog][0] == NULL && m_apKillLogPlayerIcon[nCntLog][1] == NULL && m_apKillLogPlayerIdx[nCntLog][0] == NULL && m_apKillLogPlayerIdx[nCntLog][1] == NULL)
		{//ログが使用されている且つ対象のＵＩが全てNULLの場合
		 //UIの生成処理
			m_apKillLogBase[nCntLog] = CUI_TEXTURE::Create(D3DXVECTOR3(1125.0f, 30.0f, 0.0f), 250.0f, 50.0f, CUI_TEXTURE::UIFLAME_KILL_LOG_BG);
			m_apKillLogBase[nCntLog]->SetObjType(OBJTYPE_KILLLOG);
			if (m_nKillIdx[nCntLog] == 0 || m_nKillIdx[nCntLog] == 1)
			{
				m_apKillLogBase[nCntLog]->SetTex(0, 1, 2);
			}
			if (m_nKillIdx[nCntLog] == 2 || m_nKillIdx[nCntLog] == 3)
			{
				m_apKillLogBase[nCntLog]->SetTex(1, 1, 2);
			}

			m_apKillLogPlayerIcon[nCntLog][0] = CUI_TEXTURE::Create(D3DXVECTOR3(1035.0f, 30.0f, 0.0f), 45.0f, 45.0f, CUI_TEXTURE::UIFLAME_KILL_LOG_PLAYERICON);
			m_apKillLogPlayerIcon[nCntLog][0]->SetObjType(OBJTYPE_KILLLOG);

			m_apKillLogPlayerIcon[nCntLog][1] = CUI_TEXTURE::Create(D3DXVECTOR3(1215.0f, 30.0f, 0.0f), 45.0f, 45.0f, CUI_TEXTURE::UIFLAME_KILL_LOG_PLAYERICON);
			m_apKillLogPlayerIcon[nCntLog][1]->SetObjType(OBJTYPE_KILLLOG);

			if (m_playerType[0] == TYPE_PLAYER)
			{
				m_apKillLogPlayerIcon[nCntLog][0]->SetTex(m_aMechaType[m_nKillIdx[nCntLog]], 1, 6);
			}
			else if (m_playerType[0] == TYPE_DROWN)
			{
				m_apKillLogPlayerIcon[nCntLog][0]->SetTex(4, 1, 6);
			}
			else if (m_playerType[0] == TYPE_WORKER)
			{
				m_apKillLogPlayerIcon[nCntLog][0]->SetTex(5, 1, 6);
			}

			if (m_playerType[1] == TYPE_PLAYER)
			{
				m_apKillLogPlayerIcon[nCntLog][1]->SetTex(m_aMechaType[m_nDeathIdx[nCntLog]], 1, 6);
			}
			else if (m_playerType[1] == TYPE_DROWN)
			{
				m_apKillLogPlayerIcon[nCntLog][1]->SetTex(4, 1, 6);
			}
			else if (m_playerType[1] == TYPE_WORKER)
			{
				m_apKillLogPlayerIcon[nCntLog][1]->SetTex(5, 1, 6);
			}

			m_apKillLogPlayerIdx[nCntLog][0] = CUI_TEXTURE::Create(D3DXVECTOR3(1070.0f, 40.0f, 0.0f), 35.0f, 35.0f, CUI_TEXTURE::UIFLAME_KILL_LOG_PLAYERIDX);
			m_apKillLogPlayerIdx[nCntLog][0]->SetObjType(OBJTYPE_KILLLOG);
			m_apKillLogPlayerIdx[nCntLog][0]->SetTex(m_nKillIdx[nCntLog], 1, 4);

			m_apKillLogPlayerIdx[nCntLog][1] = CUI_TEXTURE::Create(D3DXVECTOR3(1175.0f, 40.0f, 0.0f), 35.0f, 35.0f, CUI_TEXTURE::UIFLAME_KILL_LOG_PLAYERIDX);
			m_apKillLogPlayerIdx[nCntLog][1]->SetObjType(OBJTYPE_KILLLOG);
			m_apKillLogPlayerIdx[nCntLog][1]->SetTex(m_nDeathIdx[nCntLog], 1, 4);

			//m_apKillLogBase[nCntLog] = CUI_TEXTURE::Create(D3DXVECTOR3(1000.0f, 100.0f, 0.0f), 300.0f, 100.0f, CUI_TEXTURE::UIFLAME_KILL_LOG_BG);
			//m_apKillLogPlayerIcon[nCntLog][0] = CUI_TEXTURE::Create(D3DXVECTOR3(1000.0f, 100.0f, 0.0f), 150.0f, 100.0f, CUI_TEXTURE::UIFLAME_KILL_LOG_BG);
			if (nCntLog >= 1)
			{//カウンターが１以上の場合
				for (int nCntDown = 0; nCntDown < nCntLog; nCntDown++)
				{
					if (nCntDown != nCntLog)
					{//ログカウンターとログを下げるカウンターが違う場合
					 //UIを下げる
						m_apKillLogBase[nCntDown]->SetPos(D3DXVECTOR3(m_apKillLogBase[nCntDown]->GetPos().x, m_apKillLogBase[nCntDown]->GetPos().y + 50.0f, m_apKillLogBase[nCntDown]->GetPos().z));
						m_apKillLogPlayerIcon[nCntDown][0]->SetPos(D3DXVECTOR3(m_apKillLogPlayerIcon[nCntDown][0]->GetPos().x, m_apKillLogPlayerIcon[nCntDown][0]->GetPos().y + 50.0f, m_apKillLogPlayerIcon[nCntDown][0]->GetPos().z));
						m_apKillLogPlayerIcon[nCntDown][1]->SetPos(D3DXVECTOR3(m_apKillLogPlayerIcon[nCntDown][1]->GetPos().x, m_apKillLogPlayerIcon[nCntDown][1]->GetPos().y + 50.0f, m_apKillLogPlayerIcon[nCntDown][1]->GetPos().z));
						m_apKillLogPlayerIdx[nCntDown][0]->SetPos(D3DXVECTOR3(m_apKillLogPlayerIdx[nCntDown][0]->GetPos().x, m_apKillLogPlayerIdx[nCntDown][0]->GetPos().y + 50.0f, m_apKillLogPlayerIdx[nCntDown][0]->GetPos().z));
						m_apKillLogPlayerIdx[nCntDown][1]->SetPos(D3DXVECTOR3(m_apKillLogPlayerIdx[nCntDown][1]->GetPos().x, m_apKillLogPlayerIdx[nCntDown][1]->GetPos().y + 50.0f, m_apKillLogPlayerIdx[nCntDown][1]->GetPos().z));
					}
				}
			}
		}
	}

}


//=============================================================================
// キルログの更新処理
//=============================================================================
void CGame::UpdateKillLog(void)
{
	for (int nCntLog = 0; nCntLog < NUM_KILL_LOG; nCntLog++)
	{
		if (m_bLog[nCntLog] == true)
		{//ログが使用されている且つ対象のＵＩが全てNULLではない場合
			m_nCntDrawLog[nCntLog]++;	//ログの描画カウンターを加算
			if (m_nCntDrawLog[nCntLog] >= 300)
			{//ログの描画カウンターが300以上の場合
				//色を取得

				D3DXCOLOR col = m_apKillLogBase[nCntLog]->GetColor();
				col.a -= 0.05f;	//色の減算

				//UIの色を設置処理
				if (m_apKillLogBase[nCntLog] != NULL) { m_apKillLogBase[nCntLog]->SetColor(col); }
				if (m_apKillLogPlayerIcon[nCntLog][0] != NULL) {m_apKillLogPlayerIcon[nCntLog][0]->SetColor(col);}
				if (m_apKillLogPlayerIcon[nCntLog][1] != NULL) {m_apKillLogPlayerIcon[nCntLog][1]->SetColor(col);}
				if(m_apKillLogPlayerIdx[nCntLog][0] != NULL){ m_apKillLogPlayerIdx[nCntLog][0]->SetColor(col); }
				if(m_apKillLogPlayerIdx[nCntLog][1] != NULL){ m_apKillLogPlayerIdx[nCntLog][1]->SetColor(col); }

				if (col.a <= 0.0f)
				{//透明度が0以下の場合
					m_nCntDrawLog[nCntLog] = 0;	//ログの描画カウンタ－を初期化
					m_bLog[nCntLog] = false;	//ログを使用していない状態にする

					//ログの開放処理
					ReleaseKillLog(nCntLog);
				}
			}
		}
	}
}

//=============================================================================
// キルログの開放処理
//=============================================================================
void CGame::ReleaseKillLog(int nIdx)
{
	if (m_apKillLogBase[nIdx] != NULL)
	{
		m_apKillLogBase[nIdx]->Uninit();
		m_apKillLogBase[nIdx] = NULL;
	}
	for (int nCntPlayer = 0; nCntPlayer < NUM_KILL_LOG_PLAYER; nCntPlayer++)
	{
		if (m_apKillLogPlayerIcon[nIdx][nCntPlayer] != NULL)
		{
			m_apKillLogPlayerIcon[nIdx][nCntPlayer]->Uninit();
			m_apKillLogPlayerIcon[nIdx][nCntPlayer] = NULL;

		}
		if (m_apKillLogPlayerIdx[nIdx][nCntPlayer] != NULL)
		{
			m_apKillLogPlayerIdx[nIdx][nCntPlayer]->Uninit();
			m_apKillLogPlayerIdx[nIdx][nCntPlayer] = NULL;

		}

	}
}