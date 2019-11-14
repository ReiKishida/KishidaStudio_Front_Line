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
#include "enemy.h"
#include "mouseCursor.h"
#include "UI_Number.h"
#include "UI_Texture.h"
#include "texture.h"
#include <stdio.h>

#include "model.h"
#include "gauge.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define FIELD_MODEL_NAME	"data/MODEL/map_UV_bill.x"
#define NUMTEX_UV_X		(1)
#define NUMTEX_UV_Y		(3)

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
CGame::STATE CGame::m_state = CGame::STATE_NONE;
int CGame::m_nCurStage = 0;
CPlayer *CGame::m_pPlayer = NULL;

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
	// 読み込み
	CMotionManager::Load();

	m_pField = CModel::Create();
	m_pField->SetModel(FIELD_MODEL_NAME);

	// プレイヤーの生成
	m_pPlayer = CPlayer::Create();

	CBulletCollision::Create();

	CEnemy::Load();
	CEnemy::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);

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
	CUI_TEXTURE::Create(D3DXVECTOR3(345.0f, 60.0f, 0.0f), 120.0f, 50.0f, CUI_TEXTURE::UIFLAME_TEAM_BLUE);	// BLUE
	CUI_TEXTURE::Create(D3DXVECTOR3(935.0f, 60.0f, 0.0f), 120.0f, 50.0f, CUI_TEXTURE::UIFLAME_TEAM_RED);	// RED

																											// タイマーフレーム
	CUI_TEXTURE::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, 30.0f, 0.0f), 100.0f, 35.0f, CUI_TEXTURE::UIFLAME_TIMER);	// タイムロゴ

																												// パート切り替えテクスチャ
	CUI_TEXTURE::Create(D3DXVECTOR3(125.0f, 50.0f, 0.0f), 200.0f, 90.0f, CUI_TEXTURE::UIFLAME_STRATEGY_PART);	// ストラテジーパート
	CUI_TEXTURE::Create(D3DXVECTOR3(165.0f, 90.0f, 0.0f), 200.0f, 90.0f, CUI_TEXTURE::UIFLAME_ACTION_PART);		// アクションパート

																												// パート切り替え（チェンジ）
	CUI_TEXTURE::Create(D3DXVECTOR3(50.0f, 135.0f, 0.0f), 100.0f, 110.0f, CUI_TEXTURE::UIFLAME_CHANGE);

	//****************************************
	// UI生成（数字）
	//****************************************
	CUI_NUMBER::Create(D3DXVECTOR3(970.0f, 660.0f, 0.0f), 150.0f, 90.0f, 60.0f, CUI_NUMBER::UI_NUMTYPE_REMAINBULLET, 0, NUMTEX_UV_X, NUMTEX_UV_Y);		// 残弾
	CUI_NUMBER::Create(D3DXVECTOR3(195.0f, 650.0f, 0.0f), 170.0f, 110.0f, 70.0f, CUI_NUMBER::UI_NUMTYPE_PLAYER_HP, 0, NUMTEX_UV_X, NUMTEX_UV_Y);				// プレイヤーライフ
	CUI_NUMBER::Create(D3DXVECTOR3(430.0f, 60.0f, 0.0f), 120.0f, 80.0f, 55.0f, CUI_NUMBER::UI_NUMTYPE_BLUE, 1, NUMTEX_UV_X, NUMTEX_UV_Y);							// BLUEチームチケット
	CUI_NUMBER::Create(D3DXVECTOR3(720.0f, 60.0f, 0.0f), 120.0f, 80.0f, 55.0f, CUI_NUMBER::UI_NUMTYPE_RED, 2, NUMTEX_UV_X, NUMTEX_UV_Y);							// REDチームチケット

																																									//CUI_NUMBER::Create(D3DXVECTOR3(200.0f, 260.0f, 0.0f), 150.0f, 75.0f, 40.0f, CUI_NUMBER::UI_NUMTYPE_PLAYER_HP);			// プレイヤーライフ
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
	CEnemy::Unload();

	if (NULL != m_pPlayer)
	{// プレイヤークラスの破棄
		m_pPlayer->Uninit();
		m_pPlayer = NULL;
	}

	if (NULL != m_pField)
	{// 地面の破棄
		m_pField->Uninit();
		delete m_pField;
		m_pField = NULL;
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

#ifdef _DEBUG
	if (pKeyboard->GetTrigger(DIK_O))
	{
		CFade::Create(CManager::MODE_MENU);
	}
#endif

	if (pKeyboard->GetTrigger(DIK_Z) || pDirectInput->GetGamePadTrigger(0))
	{// パート切り替え
		m_part = (PART)((m_part + 1) % PART_MAX);

		CScene::UIUninit();
		if (PART_STRATEGY == m_part)
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
			CUI_TEXTURE::Create(D3DXVECTOR3(790, 30.0f, 0.0f), 100.0f, 35.0f, CUI_TEXTURE::UIFLAME_TIMER);	// タイムロゴ

																											// パート切り替えテクスチャ
			CUI_TEXTURE::Create(D3DXVECTOR3(125.0f, 50.0f, 0.0f), 200.0f, 90.0f, CUI_TEXTURE::UIFLAME_ACTION_PART);	// アクションパート
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
			//																										//****************************************
			CUI_NUMBER::Create(D3DXVECTOR3(630.0f, 60.0f, 0.0f), 120.0f, 80.0f, 55.0f, CUI_NUMBER::UI_NUMTYPE_BLUE, 1, NUMTEX_UV_X, NUMTEX_UV_Y);							// BLUEチームチケット
			CUI_NUMBER::Create(D3DXVECTOR3(920.0f, 60.0f, 0.0f), 120.0f, 80.0f, 55.0f, CUI_NUMBER::UI_NUMTYPE_RED, 2, NUMTEX_UV_X, NUMTEX_UV_Y);							// REDチームチケット

			if (NULL == m_pButtonManager)
			{// ボタン管理クラスの生成
				m_pButtonManager = CButtonManagerStrategy::Create();
			}

			if (NULL == m_pMouseCursor)
			{// マウスカーソルの生成
				m_pMouseCursor = CMouseCursor::Create();
			}
		}
		else
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
			CUI_TEXTURE::Create(D3DXVECTOR3(345.0f, 60.0f, 0.0f), 120.0f, 50.0f, CUI_TEXTURE::UIFLAME_TEAM_BLUE);	// BLUE
			CUI_TEXTURE::Create(D3DXVECTOR3(935.0f, 60.0f, 0.0f), 120.0f, 50.0f, CUI_TEXTURE::UIFLAME_TEAM_RED);	// RED

																													// タイマーフレーム
			CUI_TEXTURE::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, 30.0f, 0.0f), 100.0f, 35.0f, CUI_TEXTURE::UIFLAME_TIMER);	// タイムロゴ

																														// パート切り替えテクスチャ
			CUI_TEXTURE::Create(D3DXVECTOR3(125.0f, 50.0f, 0.0f), 200.0f, 90.0f, CUI_TEXTURE::UIFLAME_STRATEGY_PART);	// ストラテジーパート
			CUI_TEXTURE::Create(D3DXVECTOR3(165.0f, 90.0f, 0.0f), 200.0f, 90.0f, CUI_TEXTURE::UIFLAME_ACTION_PART);		// アクションパート

																														// パート切り替え（チェンジ）
			CUI_TEXTURE::Create(D3DXVECTOR3(50.0f, 135.0f, 0.0f), 100.0f, 110.0f, CUI_TEXTURE::UIFLAME_CHANGE);

			//****************************************
			// UI生成（数字）
			//****************************************
			CUI_NUMBER::Create(D3DXVECTOR3(970.0f, 660.0f, 0.0f), 150.0f, 90.0f, 60.0f, CUI_NUMBER::UI_NUMTYPE_REMAINBULLET, 0, NUMTEX_UV_X, NUMTEX_UV_Y);		// 残弾
			CUI_NUMBER::Create(D3DXVECTOR3(195.0f, 650.0f, 0.0f), 170.0f, 110.0f, 70.0f, CUI_NUMBER::UI_NUMTYPE_PLAYER_HP, 0, NUMTEX_UV_X, NUMTEX_UV_Y);				// プレイヤーライフ
			CUI_NUMBER::Create(D3DXVECTOR3(430.0f, 60.0f, 0.0f), 120.0f, 80.0f, 55.0f, CUI_NUMBER::UI_NUMTYPE_BLUE, 1, NUMTEX_UV_X, NUMTEX_UV_Y);							// BLUEチームチケット
			CUI_NUMBER::Create(D3DXVECTOR3(720.0f, 60.0f, 0.0f), 120.0f, 80.0f, 55.0f, CUI_NUMBER::UI_NUMTYPE_RED, 2, NUMTEX_UV_X, NUMTEX_UV_Y);							// REDチームチケット

		}
	}

	// フェードの取得
	CFade::FADE fade = CFade::GetFade();

	if (fade == CFade::FADE_NONE)
	{// フェード中はポーズにできなくする
		if (pKeyboard->GetTrigger(DIK_RETURN))
		{
			CFade::Create(CManager::MODE_RESULT);
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

		if(!m_bPause)
		{// ポーズを閉じるとき
			if (m_pPause != NULL)
			{// ポーズでなくなったら破棄
				m_pPause->Uninit();
				m_pPause = NULL;
				pSound->PlaySound(CSound::SOUND_LABEL_PAUSEOFF);
			}
		}
	}

	switch (m_state)
	{
	case STATE_NORMAL:
		break;

	case STATE_END:
		m_modeCounter++;
		if (m_modeCounter >= 60)
		{
			m_state = STATE_NONE;

			// 画面(モード)の設定
			//CFade::Create(CManager::MODE_RESULT);
		}
		break;
	}

	CDebugProc::Print("ゲーム");
}

//=============================================================================
// ゲーム描画処理
//=============================================================================
void CGame::Draw(void)
{
	if (NULL != m_pField)
	{// 地面の描画
		m_pField->Draw();
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
