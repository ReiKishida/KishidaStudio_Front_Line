//=============================================================================
//
// ゲーム処理 [game.cpp]
// Author : Takuto Ishida & Rei Kishida
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "tutorial.h"
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

#include "particle.h"
#include "AI.h"

#include "tutorialmenu.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define FIELD_MODEL_NAME	"data/MODEL/map_tutorial.x"
#define SKY_MODEL_NAME		"data/MODEL/sky_dome.x"
#define NUMTEX_UV_X		(1)
#define NUMTEX_UV_Y		(3)
#define TUTORIAL_INI			"data/TEXT/tutorial.ini"

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
#define NUMTEX_GAUGE_WIDTH		(430.0f)
#define NUMTEX_GAUGE_HEIGHT	(30.0f)


#define TUTORIAL_TITLE_WIDTH (450.0f)	//チュートリアル項目の幅
#define TUTORIAL_TITLE_HEIGHT (140.0f)	//チュートリアル項目の高さ

#define TUTORIAL_STEPS_WIDTH (50.0f)	//チュートリアル進行度の幅
#define TUTORIAL_STEPS_HEIGHT (50.0f)	//チュートリアル進行度の高さ

#define TUTORIAL_SUCCESS_WIDTH (520.0f)		//項目達成の幅
#define TUTORIAL_SUCCESS_HEIGHT (120.0f)	//項目達成の高さ


#define TUTORIAL_MISSION_WIDTH (750.0f)		//チュートリアル目標の幅
#define TUTORIAL_MISSION_HEIGHT (85.0f)	//チュートリアル目標の高さ

#define TUTORIAL_INST_WIDTH (350.0f)		//チュートリアル目標の幅
#define TUTORIAL_INST_HEIGHT (180.0f)	//チュートリアル目標の高さ


#define TUTORIAL_BASIC_STEPS_NUM (3)	//基本操作の項目数
#define TUTORIAL_ACTION_STEPS_NUM (3)	//アクションパートの項目数
#define TUTORIAL_STRATEGY_STEPS_NUM (3)	//ストラテジーパートの項目数

#define TUTORIAL_BASIC_INST_NUM (2)	//基本操作の操作項目数
#define TUTORIAL_ACTION_INST_NUM (3)	//アクションパートの操作項目数



//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
CTutorial::STATE CTutorial::m_state = CTutorial::STATE_NONE;
int CTutorial::m_nCurStage = 0;
CPlayer *CTutorial::m_pPlayer[MAX_PLAYER_TUTORIAL] = {};
CMechaSelect::MECHATYPE CTutorial::m_aMechaType[MAX_PLAYER_TUTORIAL] = { CMechaSelect::MECHATYPE_EMPTY,CMechaSelect::MECHATYPE_EMPTY };

int CTutorial::m_nType = 0;	//チュートリアルの種類
int CTutorial::m_nSteps = 0;	//進行度
int CTutorial::m_nStepsMax = 0;	//進行度の最大数
int CTutorial::m_nInstMax = 0;

CTutorial::TUTORIALSTATE CTutorial::m_TutorialState = TUTORIALSTATE_NONE;
//=============================================================================
// コンストラクタ
//=============================================================================
CTutorial::CTutorial(int nPriority, CScene::OBJTYPE objType) : CScene(nPriority, objType)
{
	m_modeCounter = 0;
	m_pMouseCursor = NULL;
	m_pPause = NULL;
	m_bPause = false;
	m_pButtonManager = NULL;
	m_part = PART_ACTION;
	m_pField = NULL;
	m_pSky = NULL;
	m_pMouse = NULL;
	m_nBlueLinkEnergy = 0;
	m_nRedLinkEnergy = 0;

	//チュートリアルの状態
	m_TutorialState = TUTORIALSTATE_NONE;

	//基本操作のチュートリアル
	m_R_CameraState = CLEARSTATE_NONE;
	m_L_CameraState = CLEARSTATE_NONE;
	m_U_CameraState = CLEARSTATE_NONE;
	m_D_CameraState = CLEARSTATE_NONE;
	m_Move_State = CLEARSTATE_NONE;

	//アクションパートのチュートリアル
	m_AttackState = CLEARSTATE_NONE;
	m_ReloadState = CLEARSTATE_NONE;


	m_nDisp = 0;


	m_nType = 0;
	m_nSteps = 0;
	m_nStepsMax = 0;
	m_nInstMax = 0;

	for (int nCntConnect = 0; nCntConnect < MAX_PLAYER_TUTORIAL; nCntConnect++)
	{
		m_bConnect[nCntConnect] = false;
	}

	for (int nCntTeam = 0; nCntTeam < NUM_TEAM; nCntTeam++)
	{
		for (int nCntRespawn = 0; nCntRespawn < NUM_RESPAWN_POS; nCntRespawn++)
		{
			m_aRespawnPos[nCntTeam][nCntRespawn] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		}
	}

	for (int nCntConnect = 0; nCntConnect < 2; nCntConnect++)
	{
		for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER_TUTORIAL; nCntPlayer++)
		{
			m_bAIDeath[nCntConnect][nCntPlayer] = false;
		}
	}

	for (int nCntSteps = 0; nCntSteps < 3; nCntSteps++)
	{
		m_apSteps[nCntSteps] = NULL;
	}

}

//=============================================================================
// デストラクタ
//=============================================================================
CTutorial::~CTutorial()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CTutorial::Init(void)
{
	m_nType = CTutorialMenu::GetMode();

	//チュートリアルの状態
	m_TutorialState = TUTORIALSTATE_NORMAL;

	//リスポーン位置の読み込み
	LoadRespawnPos();

	//// 読み込み
	CMotionManager::Load();

	m_pField = CModel::Create();
	m_pField->SetModel(FIELD_MODEL_NAME);
	m_pField->SetPos(D3DXVECTOR3(-10.0f, 0.0f, 40.0f));

	//m_pSky = CModel::Create();
	//m_pSky->SetModel(SKY_MODEL_NAME);

	//// 弾の当たり判定クラスの生成
	CBulletCollision::Create();

	//// マップの当たり判定の読み込み
	//CCollision::Load();

	//====================================================
	//チュートリアルのテクスチャ
	//====================================================
	switch (m_nType)
	{
	case 0:

		CUI_TEXTURE::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, 80.0f, 0.0f), TUTORIAL_TITLE_WIDTH, TUTORIAL_TITLE_HEIGHT, CUI_TEXTURE::UIFLAME_TUTORIAL_TITLE_BASIC);	// チュートリアルの項目
		m_nStepsMax = TUTORIAL_BASIC_STEPS_NUM;
		m_nInstMax = TUTORIAL_BASIC_INST_NUM;
		for (int nCntSteps = 0; nCntSteps < m_nStepsMax; nCntSteps++)
		{
			m_apSteps[nCntSteps] = CUI_TEXTURE::Create(D3DXVECTOR3((SCREEN_WIDTH / 2 - 80.0f) + (70.0f * nCntSteps), 180.0f, 0.0f), TUTORIAL_STEPS_WIDTH, TUTORIAL_STEPS_HEIGHT, CUI_TEXTURE::UIFLAME_TUTORIAL_NUMBER);	// チュートリアルの項目
			m_apSteps[nCntSteps]->SetTex(nCntSteps + 1, 10, 1);
		}
		m_pMission = CUI_TEXTURE::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, 250.0f, 0.0f), TUTORIAL_MISSION_WIDTH, TUTORIAL_MISSION_HEIGHT, CUI_TEXTURE::UIFLAME_TUTORIAL_LESSON_BASICMOVEMENT);	// 基本操作の目標
		m_pInst = CUI_TEXTURE::Create(D3DXVECTOR3(SCREEN_WIDTH - 180.0f, 450.0f, 0.0f), TUTORIAL_INST_WIDTH, TUTORIAL_INST_HEIGHT, CUI_TEXTURE::UIFLAME_TUTORIAL_INST_BASICMOVEMENT);	// 基本操作の目標
		break;
	case 1:
		CUI_TEXTURE::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, 80.0f, 0.0f), TUTORIAL_TITLE_WIDTH, TUTORIAL_TITLE_HEIGHT, CUI_TEXTURE::UIFLAME_TUTORIAL_TITLE_ACTION);	// チュートリアルの項目
		m_nStepsMax = TUTORIAL_ACTION_STEPS_NUM;
		m_nInstMax = TUTORIAL_ACTION_INST_NUM;
		for (int nCntSteps = 0; nCntSteps < m_nStepsMax; nCntSteps++)
		{
			m_apSteps[nCntSteps] = CUI_TEXTURE::Create(D3DXVECTOR3((SCREEN_WIDTH / 2 - 80.0f) + (70.0f * nCntSteps), 180.0f, 0.0f), TUTORIAL_STEPS_WIDTH, TUTORIAL_STEPS_HEIGHT, CUI_TEXTURE::UIFLAME_TUTORIAL_NUMBER);	// チュートリアルの項目
			m_apSteps[nCntSteps]->SetTex(nCntSteps + 1, 10, 1);
		}
		m_pMission = CUI_TEXTURE::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, 250.0f, 0.0f), TUTORIAL_MISSION_WIDTH, TUTORIAL_MISSION_HEIGHT, CUI_TEXTURE::UIFLAME_TUTORIAL_LESSON_ACTION);	// 基本操作の目標
		m_pInst = CUI_TEXTURE::Create(D3DXVECTOR3(SCREEN_WIDTH - 180.0f, 450.0f, 0.0f), TUTORIAL_INST_WIDTH, TUTORIAL_INST_HEIGHT, CUI_TEXTURE::UIFLAME_TUTORIAL_INST_ACTION);	// 基本操作の目標

		break;
	case 2:
		CUI_TEXTURE::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, 80.0f, 0.0f), TUTORIAL_TITLE_WIDTH, TUTORIAL_TITLE_HEIGHT, CUI_TEXTURE::UIFLAME_TUTORIAL_TITLE_STRATEGY);	// チュートリアルの項目
		break;
	}

	//====================================================
	//プレイヤーの生成
	//====================================================
	switch (m_nType)
	{
	case 0:
		m_pPlayer[0] = CPlayer::Create(0, CMechaSelect::MECHATYPE_LIGHT, m_aRespawnPos[0][2], true);

		break;
	case 1:
		m_pPlayer[0] = CPlayer::Create(0, CMechaSelect::MECHATYPE_LIGHT, m_aRespawnPos[0][2], true);
		m_pPlayer[1] = CPlayer::Create(2, CMechaSelect::MECHATYPE_LIGHT, D3DXVECTOR3(-30.0f, 0.0f, -40.0f), false);
		m_pPlayer[1]->SetLife(50);
		m_pPlayer[1]->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f));
		//****************************************
		// 2DUI生成（フレーム）
		//****************************************
		// 武器フレーム
		CUI_TEXTURE::Create(D3DXVECTOR3(1100.0f, 650.0f, 0.0f), 350.0f, 120.0f, CUI_TEXTURE::UIFLAME_WEAPON);		// 武器
																													// プレイヤー体力フレーム
		CUI_TEXTURE::Create(D3DXVECTOR3(180.0f, 650.0f, 0.0f), 350.0f, 125.0f, CUI_TEXTURE::UIFLAME_PLAYER_HP);		// 胴体

																													//****************************************
																													// UI生成（数字）
																													//****************************************
		CUI_NUMBER::Create(D3DXVECTOR3(970.0f, 660.0f, 0.0f), 150.0f, 90.0f, 60.0f, CUI_NUMBER::UI_NUMTYPE_REMAINBULLET, 0, NUMTEX_UV_X, NUMTEX_UV_Y);		// 残弾
		CUI_NUMBER::Create(D3DXVECTOR3(195.0f, 650.0f, 0.0f), 170.0f, 110.0f, 70.0f, CUI_NUMBER::UI_NUMTYPE_PLAYER_HP, 0, NUMTEX_UV_X, NUMTEX_UV_Y);				// プレイヤーライフ

		break;
	}
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CTutorial::Uninit(void)
{
	// データの破棄
	CMotionManager::Unload();

	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER_TUTORIAL; nCntPlayer++)
	{
		if (NULL != m_pPlayer[nCntPlayer])
		{// プレイヤークラスの破棄
			m_pPlayer[nCntPlayer]->Uninit();
			m_pPlayer[nCntPlayer] = NULL;
		}
	}

	for (int nCntSteps = 0; nCntSteps < m_nStepsMax; nCntSteps++)
	{
		if (NULL != m_apSteps[nCntSteps])
		{
			m_apSteps[nCntSteps]->Uninit();
			m_apSteps[nCntSteps] = NULL;
		}
	}

	if (NULL != m_pField)
	{// 地面の破棄
		m_pField->Uninit();
		delete m_pField;
		m_pField = NULL;
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


	// オブジェクトを破棄
	CScene::Release();
}

//=============================================================================
// ゲーム更新処理
//=============================================================================
void CTutorial::Update(void)
{
	CInputKeyboard	*pKeyboard = CManager::GetInputKeyboard();	// キーボードの取得
	CXInput			*pXInput = CManager::GetXInput();			// XInputの取得
	CSound *pSound = CManager::GetSound();						// サウンドの取得
	CInputMouse *pMouse = CManager::GetInputMouse();
	CDirectInput *pDirectInput = CManager::GetDirectInput();	//DirectInputの取得
	CDirectInput::GamePad *DirectInputStick = pDirectInput->GetgamePadStick();

	if (m_pPlayer[0]->GetRespawn() == CPlayer::RESPAWN_NONE)
	{
		//パートの切り替え処理
		SwicthPart();
	}

#ifdef _DEBUG
	if (pKeyboard->GetTrigger(DIK_O))
	{
		CFade::Create(CManager::MODE_MENU);
	}
	if (pKeyboard->GetTrigger(DIK_L))
	{
		m_nSteps++;
	}
#endif

	switch (m_TutorialState)
	{
	case TUTORIALSTATE_NONE:
		break;

	case TUTORIALSTATE_NORMAL:
		break;

	case TUTORIALSTATE_CLEAR:
		m_nDisp--;
		if (m_nDisp <= 0)
		{
			m_nDisp = 0;
			if (m_nSteps == m_nStepsMax - 1)
			{
				if (CFade::GetFade() == CFade::FADE_NONE)
				{// フェードがないとき
					CManager::MODE mode;

					mode = CManager::MODE_TUTORIALMENU;
					CFade::Create(mode);
				}
			}
			else
			{
				if (m_Succes != NULL)
				{
					m_Succes->Uninit();
				}
				m_nSteps++;
				m_TutorialState = TUTORIALSTATE_NORMAL;

			}
		}
		break;
	}

	CDebugProc::Print("現在のステップ : %d / %d", m_nSteps, m_nStepsMax);
	CDebugProc::Print("m_nDisp : %d", m_nDisp);
	CDebugProc::Print("m_TutorialState : %d", m_TutorialState);


	switch (m_nType)
	{
	case 0:
		TutorialBasicMovement();
		break;
	case 1:
		TutorialAction();

		break;
	case 2:
		TutorialStrategy();

		break;
	}


	// フェードの取得
	//CFade::FADE fade = CFade::GetFade();

	CDebugProc::Print("チュートリアル");
	//CDebugProc::Print("機種：%d %d %d %d", m_aMechaType[0], m_aMechaType[1], m_aMechaType[2], m_aMechaType[3]);
	switch (m_nType)
	{
	case 0:
		CDebugProc::Print("BASIC_MOVE");
		break;

	case 1:
		CDebugProc::Print("ACTION_PART");
		break;

	case 2:
		CDebugProc::Print("STRATEGY_PART");
		break;

	}


}

//=============================================================================
// ゲーム描画処理
//=============================================================================
void CTutorial::Draw(void)
{
	//// デバイスの取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();
	// ライティングOFF
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);



	if (NULL != m_pField)
	{// 地面の描画
		m_pField->Draw();
	}


	//if (NULL != m_pSky)
	//{// スカイドームの描画
	//	m_pSky->Draw();
	//}

	//// ライティングON
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//=============================================================================
// ゲームの状態の設定
//=============================================================================
const void CTutorial::SetGameState(STATE state)
{
	m_state = state;
}

//=============================================================================
// ステージの設定
//=============================================================================
void CTutorial::SetStage(int nStage)
{
	// 現在のステージを設定
	m_nCurStage = nStage;
}

//=============================================================================
// パートの切り替え処理
//=============================================================================
void CTutorial::SwicthPart(void)
{
	CInputKeyboard	*pKeyboard = CManager::GetInputKeyboard();	// キーボードの取得
	CXInput			*pXInput = CManager::GetXInput();			// XInputの取得
	CInputMouse *pMouse = CManager::GetInputMouse();
	CDirectInput *pDirectInput = CManager::GetDirectInput();	//DirectInputの取得
	CDirectInput::GamePad *DirectInputStick = pDirectInput->GetgamePadStick();

	if (pKeyboard->GetTrigger(DIK_Z) || pDirectInput->GetGamePadTrigger(0))
	{// パート切り替え
		m_part = (PART)((m_part + 1) % PART_MAX);

		CScene::UIUninit();

		if (PART_STRATEGY == m_part)
		{
			CreateStrategyUI();
		}
		else
		{
			CreateActionUI();
		}
	}
}

//=============================================================================
// アクションパートのUI生成処理
//=============================================================================
void CTutorial::CreateActionUI(void)
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
	CUI_TEXTURE::Create(D3DXVECTOR3(180.0f, 650.0f, 0.0f), 350.0f, 125.0f, CUI_TEXTURE::UIFLAME_PLAYER_HP);		// 胴体

																												// AIチケット数フレーム
	CUI_TEXTURE::Create(D3DXVECTOR3(125.0f, 530.0f, 0.0f), 230.0f, 100.0f, CUI_TEXTURE::UIFLAME_DRONE);			// AI01
	CUI_TEXTURE::Create(D3DXVECTOR3(125.0f, 420.0f, 0.0f), 230.0f, 100.0f, CUI_TEXTURE::UIFLAME_WORKER);		// AI02

																												// チームのチケット数フレーム
	CUI_TEXTURE::Create(D3DXVECTOR3(80.0f, 40.0f, 0.0f), 120.0f, 50.0f, CUI_TEXTURE::UIFLAME_TEAM_BLUE);		// BLUE
	CUI_TEXTURE::Create(D3DXVECTOR3(80.0f, 100.0f, 0.0f), 120.0f, 50.0f, CUI_TEXTURE::UIFLAME_TEAM_RED);		// RED

																												// パート切り替えテクスチャ
	CUI_TEXTURE::Create(D3DXVECTOR3(125.0f, 50.0f, 0.0f), 200.0f, 90.0f, CUI_TEXTURE::UIFLAME_STRATEGY_PART);	// ストラテジーパート
	CUI_TEXTURE::Create(D3DXVECTOR3(165.0f, 90.0f, 0.0f), 200.0f, 90.0f, CUI_TEXTURE::UIFLAME_ACTION_PART);		// アクションパート

																												// パート切り替え（チェンジ）
	CUI_TEXTURE::Create(D3DXVECTOR3(50.0f, 135.0f, 0.0f), 100.0f, 110.0f, CUI_TEXTURE::UIFLAME_CHANGE);

	//****************************************
	// UI生成（数字）
	//****************************************
	CUI_NUMBER::Create(D3DXVECTOR3(970.0f, 660.0f, 0.0f), 150.0f, 90.0f, 60.0f, CUI_NUMBER::UI_NUMTYPE_REMAINBULLET, 0, NUMTEX_UV_X, NUMTEX_UV_Y);											// 残弾
	CUI_NUMBER::Create(D3DXVECTOR3(195.0f, 650.0f, 0.0f), 170.0f, 110.0f, 70.0f, CUI_NUMBER::UI_NUMTYPE_PLAYER_HP, 0, NUMTEX_UV_X, NUMTEX_UV_Y);											// プレイヤーライフ
	CUI_NUMBER::Create(D3DXVECTOR3(370.0f, 40.0f, 0.0f), NUMTEX_GAUGE_WIDTH, NUMTEX_GAUGE_HEIGHT, 0.0f, CUI_NUMBER::UI_NUMTYPE_BLUE, 0, NUMTEX_UV_X, NUMTEX_UV_Y);							// BLUEチームチケット
	CUI_NUMBER::Create(D3DXVECTOR3(370.0f, 100.0f, 0.0f), NUMTEX_GAUGE_WIDTH, NUMTEX_GAUGE_HEIGHT, 0.0f, CUI_NUMBER::UI_NUMTYPE_RED, 0, NUMTEX_UV_X, NUMTEX_UV_Y);							// REDチームチケット

}

//=============================================================================
// ストラテジーパートのUI生成処理
//=============================================================================
void CTutorial::CreateStrategyUI(void)
{
	//****************************************
	// 2DUI生成（フレーム）
	//****************************************
	// 背景
	CUI_TEXTURE::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), SCREEN_WIDTH, SCREEN_HEIGHT, CUI_TEXTURE::UIFLAME_STRATEGY_BG);	// 背景

																																				// チームのチケット数フレーム
	CUI_TEXTURE::Create(D3DXVECTOR3(495.0f, 60.0f, 0.0f), 120.0f, 50.0f, CUI_TEXTURE::UIFLAME_TEAM_BLUE);	// BLUE
	CUI_TEXTURE::Create(D3DXVECTOR3(1085.0f, 60.0f, 0.0f), 120.0f, 50.0f, CUI_TEXTURE::UIFLAME_TEAM_RED);	// RED

																											// タイマーフレーム
	CUI_TEXTURE::Create(D3DXVECTOR3(790, 30.0f, 0.0f), 100.0f, 35.0f, CUI_TEXTURE::UIFLAME_TIMER);			// タイムロゴ

																											// パート切り替えテクスチャ
	CUI_TEXTURE::Create(D3DXVECTOR3(125.0f, 50.0f, 0.0f), 200.0f, 90.0f, CUI_TEXTURE::UIFLAME_ACTION_PART);			// アクションパート
	CUI_TEXTURE::Create(D3DXVECTOR3(165.0f, 90.0f, 0.0f), 200.0f, 90.0f, CUI_TEXTURE::UIFLAME_STRATEGY_PART);		// ストラテジーパート

																													// パート切り替え（チェンジ）
	CUI_TEXTURE::Create(D3DXVECTOR3(50.0f, 135.0f, 0.0f), 100.0f, 110.0f, CUI_TEXTURE::UIFLAME_CHANGE);

	// フレーム
	CUI_TEXTURE::Create(D3DXVECTOR3(785.0f, 420.0f, 0.0f), 990.0f, 590.0f, CUI_TEXTURE::UIFLAME_FLAME_BLUE);	// 大枠
	CUI_TEXTURE::Create(D3DXVECTOR3(790.0f, 190.0f, 0.0f), 970.0f, 100.0f, CUI_TEXTURE::UIFLAME_FLAME_WHITE);	// ロジックライン
	CUI_TEXTURE::Create(D3DXVECTOR3(405.0f, 470.0f, 0.0f), 200.0f, 450.0f, CUI_TEXTURE::UIFLAME_FLAME_WHITE);	// AI表示
	CUI_TEXTURE::Create(D3DXVECTOR3(895.0f, 470.0f, 0.0f), 760.0f, 450.0f, CUI_TEXTURE::UIFLAME_FLAME_WHITE);	// ロジック部分

																												//****************************************
																												// 1P小隊情報
																												//****************************************
	CUI_TEXTURE::Create(D3DXVECTOR3(150.0f, 325.0f, 0.0f), 280.0f, 260.0f, CUI_TEXTURE::UIFLAME_1P_INFO);

	// プレイヤー体力フレーム
	CUI_TEXTURE::Create(D3DXVECTOR3(150.0f, 260.0f, 0.0f), 255.0f, 80.0f, CUI_TEXTURE::UIFLAME_PLAYER_HP);

	// AIチケット数フレーム
	CUI_TEXTURE::Create(D3DXVECTOR3(150.0f, 335.0f, 0.0f), 255.0f, 80.0f, CUI_TEXTURE::UIFLAME_WORKER);		// ワーカー
	CUI_TEXTURE::Create(D3DXVECTOR3(150.0f, 410.0f, 0.0f), 255.0f, 80.0f, CUI_TEXTURE::UIFLAME_DRONE);		// ドローン

																											//****************************************
																											// 2P小隊情報
																											//****************************************
	CUI_TEXTURE::Create(D3DXVECTOR3(150.0f, 590.0f, 0.0f), 280.0f, 260.0f, CUI_TEXTURE::UIFLAME_2P_INFO);

	// プレイヤー体力フレーム
	CUI_TEXTURE::Create(D3DXVECTOR3(150.0f, 525.0f, 0.0f), 255.0f, 80.0f, CUI_TEXTURE::UIFLAME_PLAYER_HP);

	// AIチケット数フレーム
	CUI_TEXTURE::Create(D3DXVECTOR3(150.0f, 600.0f, 0.0f), 255.0f, 80.0f, CUI_TEXTURE::UIFLAME_WORKER);		// ワーカー
	CUI_TEXTURE::Create(D3DXVECTOR3(150.0f, 675.0f, 0.0f), 255.0f, 80.0f, CUI_TEXTURE::UIFLAME_DRONE);		// ドローン

																											//****************************************
																											// UI生成（数字）
																											//****************************************
	CUI_NUMBER::Create(D3DXVECTOR3(630.0f, 60.0f, 0.0f), 120.0f, 80.0f, 55.0f, CUI_NUMBER::UI_NUMTYPE_BLUE, 1, NUMTEX_UV_X, NUMTEX_UV_Y);							// BLUEチームチケット
	CUI_NUMBER::Create(D3DXVECTOR3(920.0f, 60.0f, 0.0f), 120.0f, 80.0f, 55.0f, CUI_NUMBER::UI_NUMTYPE_RED, 2, NUMTEX_UV_X, NUMTEX_UV_Y);							// REDチームチケット



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
// 必要な情報を書き込む処理
//=============================================================================
void CTutorial::PrintData(void)
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
			pClient->Printf("%.1f %.1f %.1f", CManager::GetCamera()->GetRot().x, CManager::GetCamera()->GetRot().y, CManager::GetCamera()->GetRot().z);
			pClient->Printf(" ");

			//死亡しているかどうか
			if (m_pPlayer[pClient->GetPlayerIdx()]->GetDeath() == true)
			{
				pClient->Printf("1");
				pClient->Printf(" ");
			}
			else
			{
				pClient->Printf("0");
				pClient->Printf(" ");

				//体力の表示
				pClient->Printf("%d", m_pPlayer[pClient->GetPlayerIdx()]->GetLife());
				pClient->Printf(" ");

			}

			//弾を発射しているかどうかを書き込む
			if (m_pPlayer[pClient->GetPlayerIdx()]->GetShoot() == true)
			{//発射されている場合

				pClient->Printf("1");
				pClient->Printf(" ");

				//弾の同時発射数を書き込む
				pClient->Printf("%d", m_pPlayer[pClient->GetPlayerIdx()]->GetNumShoot());
				pClient->Printf(" ");

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
				pClient->Printf("0");
				pClient->Printf(" ");
			}

			//チャットをしているかどうかを書き込む
			if (m_pPlayer[pClient->GetPlayerIdx()]->GetChat() == true)
			{//チャットをしている場合

				pClient->Printf("1");
				pClient->Printf(" ");

				//チャットの番号を書き込む
				pClient->Printf("%d", m_pPlayer[pClient->GetPlayerIdx()]->GetRadioChat());
				pClient->Printf(" ");
			}
			else
			{
				pClient->Printf("0");
				pClient->Printf(" ");
			}

			//AIの位置を書き込む
			pClient->Printf("%.1f %.1f %.1f", m_pPlayer[pClient->GetPlayerIdx()]->GetMyAI(0)->GetPos().x, m_pPlayer[pClient->GetPlayerIdx()]->GetMyAI(0)->GetPos().y, m_pPlayer[pClient->GetPlayerIdx()]->GetMyAI(0)->GetPos().z);
			pClient->Printf(" ");

			//AIの向きを書き込む
			pClient->Printf("%.1f", m_pPlayer[pClient->GetPlayerIdx()]->GetMyAI(0)->GetRot().y);
			pClient->Printf(" ");

			//AIの死亡しているかどうかを書き込む
			if (m_pPlayer[pClient->GetPlayerIdx()]->GetMyAI(0)->GetDeath() == true)
			{
				pClient->Printf("1");
				pClient->Printf(" ");
			}
			else
			{
				pClient->Printf("0");
				pClient->Printf(" ");
			}

			if (pClient->GetPlayerIdx() == 0)
			{//ホストの場合
				if (m_state == STATE_END)
				{
					pClient->Printf("1");
					pClient->Printf(" ");
					pClient->Printf("%d", CResult::GetTeamWin());
					pClient->Printf(" ");
				}
				else
				{
					pClient->Printf("0");
					pClient->Printf(" ");



				}
				//CPUのデータ情報を書き込む処理
				//PrintCPUData();

			}
		}
	}
#endif
}

//=============================================================================
// CPUのデータ情報を書き込む処理
//=============================================================================
void CTutorial::PrintCPUData(void)
{
	CClient *pClient = CManager::GetClient();
	if (pClient != NULL)
	{
		for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER_TUTORIAL; nCntPlayer++)
		{
			if (pClient->GetPlayerIdx() != nCntPlayer)
			{
				if (m_bConnect[nCntPlayer] == false)
				{
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

					//死亡しているかどうか
					if (m_pPlayer[nCntPlayer]->GetDeath() == true)
					{
						pClient->Printf("1");
						pClient->Printf(" ");
					}
					else
					{
						pClient->Printf("0");
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
				}
			}
		}
	}
}
//=============================================================================
// 情報を読み取る処理
//=============================================================================
void CTutorial::ReadMessage(void)
{
	CClient *pClient = CManager::GetClient();			//クライアントのポインタ情報
	char *pStr = pClient->GetReceiveData();				//サーバーから受け取ったメッセージ情報

	if (pClient != NULL && pClient->GetConnect() == true)
	{
		//頭出し処理
		pStr = CServerFunction::HeadPutout(pStr, "");

		if (CServerFunction::Memcmp(pStr, SERVER_CONNECT_DATA) == 0)
		{//接続総数を示している場合
			pStr += strlen(SERVER_CONNECT_DATA);								//頭出し

																				//接続情報の読み取り処理
			pStr = ReadConnectData(pStr);
		}
		if (CServerFunction::Memcmp(pStr, SERVER_PLAYER_START) == 0)
		{//プレイヤーの開始を示している場合
			pStr += strlen(SERVER_PLAYER_START);							//頭出し
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
char *CTutorial::ReadConnectData(char *pStr)
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
char *CTutorial::ReadPlayerData(char *pStr)
{
	D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);									//位置
	D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 modelRotDown = D3DXVECTOR3(0.0f, 0.0f, 0.0f), modelRotUp = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				//モデルの上半身と下半身の向き
	D3DXVECTOR3 cameraRot;								//カメラの向き
	int nPlayerIdx = 0;									//プレイヤーの番号
	int nNumConnect = 0;								//接続総数
	int nWord = 0;										//文字の頭出し用
	int nCntClient = 0;									//クライアントのカウンター
	int nTeam = 0;
	float *pAngle = NULL;
	float *pAngleV = NULL;
	bool bShoot = false;
	int nNumShoot = 0;
	int nAttack = 0;
	bool bChat = false;
	CPlayer::RADIOCHAT radioChat = CPlayer::RADIOCHAT_OK;
	int nBlueLinkEnergy = 0;
	int nRedLinkEnergy = 0;
	int nState = 0;
	int nLife = 0;
	D3DXVECTOR3 AIPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 AIRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	bool bAIDeath = false;

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

			//モデル0版の向きを代入
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

			if (bDeath == false)
			{
				nLife = CServerFunction::ReadInt(pStr, "");
				nWord = CServerFunction::PopString(pStr, "");
				pStr += nWord;
			}

			//弾を発射しているかどうかを代入
			bShoot = CServerFunction::ReadBool(pStr, "");
			nWord = CServerFunction::PopString(pStr, "");
			pStr += nWord;

			if (bShoot == true)
			{
				nNumShoot = CServerFunction::ReadInt(pStr, "");
				nWord = CServerFunction::PopString(pStr, "");
				pStr += nWord;

				pAngle = new float[nNumShoot * 2];
				pAngleV = new float[nNumShoot * 2];

				nAttack = CServerFunction::ReadInt(pStr, "");
				nWord = CServerFunction::PopString(pStr, "");
				pStr += nWord;

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
			{
				radioChat = (CPlayer::RADIOCHAT)CServerFunction::ReadInt(pStr, "");
				nWord = CServerFunction::PopString(pStr, "");
				pStr += nWord;
			}

			//AIの位置を代入
			AIPos.x = CServerFunction::ReadFloat(pStr, "");
			nWord = CServerFunction::PopString(pStr, "");
			pStr += nWord;
			AIPos.y = CServerFunction::ReadFloat(pStr, "");
			nWord = CServerFunction::PopString(pStr, "");
			pStr += nWord;
			AIPos.z = CServerFunction::ReadFloat(pStr, "");
			nWord = CServerFunction::PopString(pStr, "");
			pStr += nWord;

			//AIの向きを代入
			AIRot.y = CServerFunction::ReadFloat(pStr, "");
			nWord = CServerFunction::PopString(pStr, "");
			pStr += nWord;

			//AIの死亡しているかどうかを代入
			bAIDeath = CServerFunction::ReadBool(pStr, "");
			nWord = CServerFunction::PopString(pStr, "");
			pStr += nWord;

			if (nPlayerIdx == 0)
			{//ホストの場合

				nState = CServerFunction::ReadInt(pStr, "");
				nWord = CServerFunction::PopString(pStr, "");
				pStr += nWord;

				if (nState == 1)
				{
					CResult::SetTeamWin((CResult::TEAM_WIN)CServerFunction::ReadInt(pStr, ""));
				}
				else
				{
					//m_nBlueLinkEnergy = CServerFunction::ReadInt(pStr, "");
					//nWord = CServerFunction::PopString(pStr, "");
					//pStr += nWord;

					//m_nRedLinkEnergy = CServerFunction::ReadInt(pStr, "");
					//nWord = CServerFunction::PopString(pStr, "");
					//pStr += nWord;
				}
				//pStr = ReadCPUData(pStr);

			}
			if (bDeath == true)
			{
				m_pPlayer[nPlayerIdx]->GetDeath() = true;
				if (m_bPlayerDeath[nPlayerIdx] == false)
				{
					m_bPlayerDeath[nPlayerIdx] = true;
					if (m_bPlayerDeath[nPlayerIdx] == true)
					{
						switch (nTeam)
						{
						case 0:
							m_nBlueLinkEnergy -= 30;
							break;
						case 1:
							m_nRedLinkEnergy -= 30;
							break;
						}
					}
				}
			}
			else
			{
				m_bPlayerDeath[nPlayerIdx] = false;
			}

			if (bAIDeath == true)
			{
				m_pPlayer[nPlayerIdx]->GetMyAI(0)->SetDeath(bAIDeath);
				if (m_bAIDeath[0][nPlayerIdx] == false)
				{
					m_bAIDeath[0][nPlayerIdx] = true;
					if (m_bAIDeath[0][nPlayerIdx] == true)
					{
						switch (nTeam)
						{
						case 0:
							m_nBlueLinkEnergy -= 20;
							break;
						case 1:
							m_nRedLinkEnergy -= 20;
							break;
						}
					}
				}
			}
			else
			{
				if (m_bAIDeath[0][nPlayerIdx] == true)
				{
					m_bAIDeath[0][nPlayerIdx] = false;
					m_pPlayer[nPlayerIdx]->GetMyAI(0)->SetDeath(bAIDeath);
				}
			}
			if (nPlayerIdx != pClient->GetPlayerIdx())
			{//プレイヤーの番号がクライアント番号と違う場合
				if (m_pPlayer[nPlayerIdx] != NULL)
				{
					//チーム情報をの設置処理
					//m_pPlayer[nPlayerIdx]->SetTeam(nTeam);

					SetPlayerData(nPlayerIdx, pos, rot, modelRotUp, modelRotDown, cameraRot);

					m_pPlayer[nPlayerIdx]->SetLife(nLife);

					m_pPlayer[nPlayerIdx]->GetMyAI(0)->SetPos(AIPos);
					m_pPlayer[nPlayerIdx]->GetMyAI(0)->SetRot(AIRot);
					if (bShoot == true)
					{//弾を発射していintる場合
						CreatePlayerBullet(nPlayerIdx, nNumShoot, nAttack, cameraRot, pAngle, pAngleV);
					}

					if (bChat == true && m_pPlayer[pClient->GetPlayerIdx()]->GetTeam() == nTeam)
					{
						SetChatData(nPlayerIdx, (int)radioChat);
					}
					if (nState == 1)
					{
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
char *CTutorial::ReadCPUData(char *pStr)
{
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER_TUTORIAL; nCntPlayer++)
	{
		if (m_bConnect[nCntPlayer] == false)
		{
			D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);									//位置
			D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			D3DXVECTOR3 modelRotDown = D3DXVECTOR3(0.0f, 0.0f, 0.0f), modelRotUp = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				//モデルの上半身と下半身の向き
			D3DXVECTOR3 cameraRot;								//カメラの向き
			int nPlayerIdx = 0;									//プレイヤーの番号
			int nNumConnect = 0;								//接続総数
			int nWord = 0;										//文字の頭出し用
			int nCntClient = 0;									//クライアントのカウンター
			int nTeam = 0;
			float *pAngle = NULL;
			float *pAngleV = NULL;
			bool bShoot = false;
			int nNumShoot = 0;
			int nAttack = 0;

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

			//死亡しているかどうかを代入
			bool bDeath = CServerFunction::ReadBool(pStr, "");
			nWord = CServerFunction::PopString(pStr, "");
			pStr += nWord;

			//弾を発射しているかどうかを代入
			bShoot = CServerFunction::ReadBool(pStr, "");
			nWord = CServerFunction::PopString(pStr, "");
			pStr += nWord;

			if (bShoot == true)
			{
				nNumShoot = CServerFunction::ReadInt(pStr, "");
				nWord = CServerFunction::PopString(pStr, "");
				pStr += nWord;

				pAngle = new float[nNumShoot * 2];
				pAngleV = new float[nNumShoot * 2];

				nAttack = CServerFunction::ReadInt(pStr, "");
				nWord = CServerFunction::PopString(pStr, "");
				pStr += nWord;

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

			SetCPUData(nPlayerIdx, pos, rot);
			if (bShoot == true)
			{
				CreateCPUBullet(nPlayerIdx, nNumShoot, nAttack, cameraRot, pAngle, pAngleV);
			}


		}
	}
	return pStr;
}

//=============================================================================
// プレイヤーの情報の設置処理
//=============================================================================
void CTutorial::SetPlayerData(int nPlayerIdx, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 modelRotUp, D3DXVECTOR3 modelRotDown, D3DXVECTOR3 cameraRot)
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
void CTutorial::CreatePlayerBullet(int nPlayerIdx, int nNumShoot, int nAttack, D3DXVECTOR3 cameraRot, float *pAngle, float *pAngleV)
{
	for (int nCntShoot = 0; nCntShoot < nNumShoot; nCntShoot++)
	{
		// 弾の生成
		D3DXMATRIX mtxCanon = m_pPlayer[nPlayerIdx]->GetModel(2)->GetMtxWorld();
		D3DXVECTOR3 posCanon = D3DXVECTOR3(mtxCanon._41, mtxCanon._42, mtxCanon._43) + D3DXVECTOR3(sinf(cameraRot.y) * 30.0f, cosf(cameraRot.x) * 30.0f, cosf(cameraRot.y) * 30.0f);
		CBulletPlayer::Create(posCanon, pAngle[nCntShoot * 2], pAngleV[nCntShoot * 2], nAttack, m_pPlayer[nPlayerIdx]->GetTeam(), this, m_pPlayer[nPlayerIdx]->GetBulletSpeed());
		mtxCanon = m_pPlayer[nPlayerIdx]->GetModel(3)->GetMtxWorld();
		posCanon = D3DXVECTOR3(mtxCanon._41, mtxCanon._42, mtxCanon._43) + D3DXVECTOR3(sinf(cameraRot.y) * 30.0f, cosf(cameraRot.x) * 30.0f, cosf(cameraRot.y) * 30.0f);
		CBulletPlayer::Create(posCanon, pAngle[nCntShoot * 2 + 1], pAngleV[nCntShoot * 2 + 1], nAttack, m_pPlayer[nPlayerIdx]->GetTeam(), this, m_pPlayer[nPlayerIdx]->GetBulletSpeed());

		//弾を発射しているかどうかの設置処理
		m_pPlayer[nPlayerIdx]->SetShoot(false);
	}
}

//=============================================================================
// チャット情報の設置処理
//=============================================================================
void CTutorial::SetChatData(int nPlayerIdx, int radioChat)
{

	switch (nPlayerIdx)
	{
	case 0:
		m_pPlayer[1]->SetAllyRadioChat((CPlayer::RADIOCHAT)radioChat);
		m_pPlayer[1]->SetAllyChat(true);
		break;
	case 1:
		m_pPlayer[0]->SetAllyRadioChat((CPlayer::RADIOCHAT)radioChat);
		m_pPlayer[0]->SetAllyChat(true);
		break;
	case 2:
		m_pPlayer[3]->SetAllyRadioChat((CPlayer::RADIOCHAT)radioChat);
		m_pPlayer[3]->SetAllyChat(true);
		break;
	case 3:
		m_pPlayer[2]->SetAllyRadioChat((CPlayer::RADIOCHAT)radioChat);
		m_pPlayer[2]->SetAllyChat(true);
		break;
	}
}

//=============================================================================
// CPUプレイヤーの情報設置処理
//=============================================================================
void CTutorial::SetCPUData(int nPlayerIdx, D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	//プレイヤーの位置の設置処理
	m_pPlayer[nPlayerIdx]->SetPos(pos);
	//プレイヤーの向きの設置処理
	m_pPlayer[nPlayerIdx]->SetRot(rot);
}

//=============================================================================
// CPUプレイヤーの弾の生成処理
//=============================================================================
void CTutorial::CreateCPUBullet(int nPlayerIdx, int nNumShoot, int nAttack, D3DXVECTOR3 cameraRot, float *pAngle, float *pAngleV)
{
	for (int nCntShoot = 0; nCntShoot < nNumShoot; nCntShoot++)
	{
		// 弾の生成
		D3DXMATRIX mtxCanon = m_pPlayer[nPlayerIdx]->GetModel(2)->GetMtxWorld();
		D3DXVECTOR3 posCanon = D3DXVECTOR3(mtxCanon._41, mtxCanon._42, mtxCanon._43) + D3DXVECTOR3(sinf(cameraRot.y) * 30.0f, cosf(cameraRot.x) * 30.0f, cosf(cameraRot.y) * 30.0f);
		CBulletPlayer::Create(posCanon, pAngle[nCntShoot * 2], pAngleV[nCntShoot * 2], nAttack, m_pPlayer[nPlayerIdx]->GetTeam(), this, m_pPlayer[nPlayerIdx]->GetBulletSpeed());
		mtxCanon = m_pPlayer[nPlayerIdx]->GetModel(3)->GetMtxWorld();
		posCanon = D3DXVECTOR3(mtxCanon._41, mtxCanon._42, mtxCanon._43) + D3DXVECTOR3(sinf(cameraRot.y) * 30.0f, cosf(cameraRot.x) * 30.0f, cosf(cameraRot.y) * 30.0f);
		CBulletPlayer::Create(posCanon, pAngle[nCntShoot * 2 + 1], pAngleV[nCntShoot * 2 + 1], nAttack, m_pPlayer[nPlayerIdx]->GetTeam(), this, m_pPlayer[nPlayerIdx]->GetBulletSpeed());

		//弾を発射しているかどうかの設置処理
		m_pPlayer[nPlayerIdx]->SetShoot(false);
	}
}

//=============================================================================
// 基本操作のチュートリアル
//=============================================================================
void CTutorial::TutorialBasicMovement(void)
{
	// サウンドの取得
	CSound *pSound = CManager::GetSound();


	CPlayer *pPlayer = CTutorial::GetPlayer(0);
	D3DXVECTOR3 posPlayer = pPlayer->GetPos();

	CCamera *pCamera = CManager::GetCamera();
	D3DXVECTOR3 CameraRot = pCamera->GetRot();

	D3DXVECTOR3 posClear = D3DXVECTOR3(-30.0f, 0.0f, -40.0f);

	float fradius = 50.0f;

	float fcol = ((posPlayer.x - posClear.x) *(posPlayer.x - posClear.x)) + ((posPlayer.z - posClear.z) *(posPlayer.z - posClear.z));
	float fDiameter = fradius * fradius;

	switch (m_nSteps)
	{
	case 0:

		if (CameraRot.y >= 2.8f && CameraRot.y <= 3.1f)
		{
			if (m_R_CameraState == CLEARSTATE_NONE && m_R_CameraState != CLEARSTATE_TRUE)
			{
				pSound->PlaySound(CSound::SOUND_LABEL_SUCCESS);

				m_R_CameraState = CLEARSTATE_TRUE;
			}
		}
		else if (m_L_CameraState == CLEARSTATE_NONE && m_L_CameraState != CLEARSTATE_TRUE)
		{
			if (CameraRot.y <= 0.4f && CameraRot.y >= -1.5f)
			{
				pSound->PlaySound(CSound::SOUND_LABEL_SUCCESS);

				m_L_CameraState = CLEARSTATE_TRUE;
			}
		}

		if (m_R_CameraState == CLEARSTATE_TRUE && m_L_CameraState == CLEARSTATE_TRUE)
		{
			if (m_TutorialState == TUTORIALSTATE_NORMAL && m_TutorialState != TUTORIALSTATE_CLEAR)
			{
				pSound->PlaySound(CSound::SOUND_LABEL_STEPCLEAR);
				m_Succes = CUI_TEXTURE::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), TUTORIAL_SUCCESS_WIDTH, TUTORIAL_SUCCESS_HEIGHT, CUI_TEXTURE::UIFLAME_TUTORIAL_SUCCESS);	// チュートリアルの達成
				m_nDisp = 60;
				m_TutorialState = TUTORIALSTATE_CLEAR;
			}
		}
		CDebugProc::Print("クリア[R]：%d", m_R_CameraState);
		CDebugProc::Print("クリア[L]：%d", m_L_CameraState);

		break;

	case 1:
		if (CameraRot.x <= 1.3f && CameraRot.x >= 1.0f)
		{
			if (m_U_CameraState == CLEARSTATE_NONE && m_U_CameraState != CLEARSTATE_TRUE)
			{
				pSound->PlaySound(CSound::SOUND_LABEL_SUCCESS);

				m_U_CameraState = CLEARSTATE_TRUE;
			}
		}
		if (m_U_CameraState == CLEARSTATE_TRUE)
		{
			if (CameraRot.x <= 1.6f && CameraRot.x >= 1.4f)
			{
				if (m_D_CameraState == CLEARSTATE_NONE && m_D_CameraState != CLEARSTATE_TRUE)
				{
					pSound->PlaySound(CSound::SOUND_LABEL_SUCCESS);

					m_D_CameraState = CLEARSTATE_TRUE;
				}
			}
		}

		if (m_D_CameraState == CLEARSTATE_TRUE && m_U_CameraState == CLEARSTATE_TRUE)
		{
			if (m_TutorialState == TUTORIALSTATE_NORMAL && m_TutorialState != TUTORIALSTATE_CLEAR)
			{
				pSound->PlaySound(CSound::SOUND_LABEL_STEPCLEAR);
				m_Succes = CUI_TEXTURE::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), TUTORIAL_SUCCESS_WIDTH, TUTORIAL_SUCCESS_HEIGHT, CUI_TEXTURE::UIFLAME_TUTORIAL_SUCCESS);	// チュートリアルの達成
				m_nDisp = 60;
				m_TutorialState = TUTORIALSTATE_CLEAR;
			}
		}
		CDebugProc::Print("クリア[U]：%d", m_U_CameraState);
		CDebugProc::Print("クリア[D]：%d", m_D_CameraState);

		break;

	case 2:

		if (fcol <= fDiameter)
		{
			if (m_Move_State == CLEARSTATE_NONE && m_Move_State != CLEARSTATE_TRUE)
			{
				pSound->PlaySound(CSound::SOUND_LABEL_SUCCESS);

				m_Move_State = CLEARSTATE_TRUE;
			}
		}

		if (m_Move_State == CLEARSTATE_TRUE)
		{
			if (m_TutorialState == TUTORIALSTATE_NORMAL && m_TutorialState != TUTORIALSTATE_CLEAR)
			{
				pSound->PlaySound(CSound::SOUND_LABEL_STEPCLEAR);
				m_Succes = CUI_TEXTURE::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), TUTORIAL_SUCCESS_WIDTH, TUTORIAL_SUCCESS_HEIGHT, CUI_TEXTURE::UIFLAME_TUTORIAL_SUCCESS);	// チュートリアルの達成
				m_nDisp = 60;
				m_TutorialState = TUTORIALSTATE_CLEAR;
			}
		}

		break;
	}

	SetTexTutorial(m_nType, m_nSteps);
}

//=============================================================================
// アクションパートのチュートリアル
//=============================================================================
void CTutorial::TutorialAction(void)
{
	// サウンドの取得
	CSound *pSound = CManager::GetSound();

	D3DXVECTOR3 posClear = D3DXVECTOR3(-30.0f, 0.0f, -40.0f);

	CPlayer *pPlayer = CTutorial::GetPlayer(0);
	D3DXVECTOR3 posPlayer = pPlayer->GetPos();

	float fradius = 50.0f;

	float fcol = ((posPlayer.x - posClear.x) *(posPlayer.x - posClear.x)) + ((posPlayer.z - posClear.z) *(posPlayer.z - posClear.z));
	float fDiameter = fradius * fradius;


	switch (m_nSteps)
	{
	case 0:
		int nLife;
		nLife = m_pPlayer[1]->GetLife();
		if (nLife == 0)
		{
			if (m_AttackState == CLEARSTATE_NONE && m_AttackState != CLEARSTATE_TRUE)
			{
				m_AttackState = CLEARSTATE_TRUE;
				pSound->PlaySound(CSound::SOUND_LABEL_STEPCLEAR);
				m_Succes = CUI_TEXTURE::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), TUTORIAL_SUCCESS_WIDTH, TUTORIAL_SUCCESS_HEIGHT, CUI_TEXTURE::UIFLAME_TUTORIAL_SUCCESS);	// チュートリアルの達成
				m_nDisp = 60;
				m_TutorialState = TUTORIALSTATE_CLEAR;
			}
		}
		break;

	case 1:
		if (m_pPlayer[0]->GetReload() == true)
		{
			if (m_ReloadState == CLEARSTATE_NONE && m_ReloadState != CLEARSTATE_TRUE)
			{
				m_ReloadState = CLEARSTATE_TRUE;

				pSound->PlaySound(CSound::SOUND_LABEL_STEPCLEAR);
				m_Succes = CUI_TEXTURE::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), TUTORIAL_SUCCESS_WIDTH, TUTORIAL_SUCCESS_HEIGHT, CUI_TEXTURE::UIFLAME_TUTORIAL_SUCCESS);	// チュートリアルの達成
				m_nDisp = 60;
				m_TutorialState = TUTORIALSTATE_CLEAR;
			}
		}
		break;

	case 2:
		if (fcol <= fDiameter)
		{
			if (m_Move_State == CLEARSTATE_NONE && m_Move_State != CLEARSTATE_TRUE)
			{
				pSound->PlaySound(CSound::SOUND_LABEL_SUCCESS);

				m_Move_State = CLEARSTATE_TRUE;
			}
		}

		if (m_Move_State == CLEARSTATE_TRUE)
		{
			if (m_TutorialState == TUTORIALSTATE_NORMAL && m_TutorialState != TUTORIALSTATE_CLEAR)
			{
				pSound->PlaySound(CSound::SOUND_LABEL_STEPCLEAR);
				m_Succes = CUI_TEXTURE::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), TUTORIAL_SUCCESS_WIDTH, TUTORIAL_SUCCESS_HEIGHT, CUI_TEXTURE::UIFLAME_TUTORIAL_SUCCESS);	// チュートリアルの達成
				m_nDisp = 60;
				m_TutorialState = TUTORIALSTATE_CLEAR;
			}
		}

		break;
	}

	SetTexTutorial(m_nType, m_nSteps);
}

//=============================================================================
// ストラテジーパートのチュートリアル
//=============================================================================
void CTutorial::TutorialStrategy(void)
{
}

//=============================================================================
// 基本操作のテクスチャ座標ずらし
//=============================================================================
void CTutorial::SetTexTutorial(int nType, int nSteps)
{
	switch (nSteps)
	{
	case 0:
		m_apSteps[0]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		m_apSteps[1]->SetColor(D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.0f));
		m_apSteps[2]->SetColor(D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.0f));
		m_pMission->SetTex(nSteps, 1, m_nStepsMax);
		m_pInst->SetTex(0, 1, m_nInstMax);


		break;
	case 1:
		m_apSteps[0]->SetColor(D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.0f));
		m_apSteps[1]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		m_apSteps[2]->SetColor(D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.0f));
		m_pMission->SetTex(nSteps, 1, m_nStepsMax);
		switch (nType)
		{
		case 0:
			m_pInst->SetTex(0, 1, m_nInstMax);
			break;
		case 1:
			m_pInst->SetTex(nSteps, 1, m_nInstMax);
			break;
		case 2:
			m_pInst->SetTex(nSteps, 1, m_nInstMax);
			break;
		}		break;
	case 2:
		m_apSteps[0]->SetColor(D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.0f));
		m_apSteps[1]->SetColor(D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.0f));
		m_apSteps[2]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		m_pMission->SetTex(nSteps, 1, m_nStepsMax);
		switch (nType)
		{
		case 0:
			m_pInst->SetTex(1, 1, m_nInstMax);
			break;
		case 1:
			m_pInst->SetTex(nSteps, 1, m_nInstMax);
			break;
		case 2:
			m_pInst->SetTex(nSteps, 1, m_nInstMax);
			break;
		}
		break;
	}

}


//=============================================================================
// リスポーン位置の読み込み
//=============================================================================
void CTutorial::LoadRespawnPos(void)
{
	FILE *pFile;	//ファイルのポインタ

					//ファイルの読み込み
	pFile = fopen(TUTORIAL_INI, "r");

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
