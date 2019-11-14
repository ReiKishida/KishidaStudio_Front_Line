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

#include "server.h"
#include "serverfunction.h"

#include "collisionSet.h"

#include "menu.h"

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
CPlayer *CGame::m_pPlayer[MAX_CONNECT] = {};

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

	D3DXVECTOR3 aPos[4];
	aPos[0] = D3DXVECTOR3(590.0f, 0.0f, 40.0f);
	aPos[1] = D3DXVECTOR3(590.0f, 0.0f, -60.0f);
	aPos[2] = D3DXVECTOR3(-760.0f, 0.0f, 40.0f);
	aPos[3] = D3DXVECTOR3(-760.0f, 0.0f, -60.0f);

	// プレイヤーの生成
	for (int nCntPlayer = 0; nCntPlayer < MAX_CONNECT; nCntPlayer++)
	{
		if (CMenu::GetMode() == CMenu::MODE_MULTI)
		{
			CClient *pClient = CManager::GetClient();
			if (pClient != NULL)
			{
				m_pPlayer[nCntPlayer] = CPlayer::Create(nCntPlayer, (CMechaSelect::MECHATYPE)pClient->GetMechaType(nCntPlayer), aPos[nCntPlayer]);
			}
		}
		else
		{
			m_pPlayer[nCntPlayer] = CPlayer::Create(nCntPlayer, CMechaSelect::GetMechaType(), aPos[nCntPlayer]);
		}
	}

	CBulletCollision::Create();

	//CEnemy::Load();
	//CEnemy::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);

	CCollision::Load();

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

	for (int nCntPlayer = 0; nCntPlayer < MAX_CONNECT; nCntPlayer++)
	{
		if (NULL != m_pPlayer[nCntPlayer])
		{// プレイヤークラスの破棄
			m_pPlayer[nCntPlayer]->Uninit();
			m_pPlayer[nCntPlayer] = NULL;
		}
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

	if (CMenu::GetMode() == CMenu::MODE_MULTI)
	{
		if (CManager::GetClient() != NULL)
		{
			if (CManager::GetClient()->GetGameMode() == CClient::GAME_MODE_PLAYER)
			{
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

	for (int nCntPlayer = 0; nCntPlayer < MAX_CONNECT; nCntPlayer++)
	{
		if (m_pPlayer[nCntPlayer]->GetDeath() == true)
		{
			m_state = STATE_END;
		}
	}
	// フェードの取得
	CFade::FADE fade = CFade::GetFade();

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
			CFade::Create(CManager::MODE_RESULT);
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
	//****************************************
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
		 //クライアント番号を書き込む
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
		}
	}
#endif
}

//=============================================================================
// 情報を読み取る処理
//=============================================================================
void CGame::ReadMessage(void)
{
	CClient *pClient = CManager::GetClient();			//クライアントのポインタ情報
	char *pStr = pClient->GetReceiveData();				//サーバーから受け取ったメッセージ情報
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

	if (pClient != NULL && pClient->GetConnect() == true)
	{
		//頭出し処理
		pStr = CServerFunction::HeadPutout(pStr, "");

		if (CServerFunction::Memcmp(pStr, SERVER_NUM_CONNECT) == 0)
		{//接続総数を示している場合
			pStr += strlen(SERVER_NUM_CONNECT);								//頭出し
			pClient->SetNumConnect(CServerFunction::ReadInt(pStr, ""));		//総数の設置処理
			nWord = CServerFunction::PopString(pStr, "");					//文字数カウント
			pStr += nWord;													//頭出し
		}
		if (CServerFunction::Memcmp(pStr, SERVER_PLAYER_START) == 0)
		{//プレイヤーの開始を示している場合
			pStr += strlen(SERVER_PLAYER_START);							//頭出し

			for (int nCntClient = 0; nCntClient < pClient->GetNumConnect() - 1; nCntClient++)
			{
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

				//下半身のモデルの向きを代入
				modelRotDown.y = CServerFunction::ReadFloat(pStr, "");
				nWord = CServerFunction::PopString(pStr, "");
				pStr += nWord;

				//上半身のモデルの向きを代入
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

				if (nPlayerIdx != pClient->GetPlayerIdx())
				{//プレイヤーの番号がクライアント番号と違う場合
					if (m_pPlayer[nPlayerIdx] != NULL)
					{
						//チーム情報をの設置処理
						//m_pPlayer[nPlayerIdx]->SetTeam(nTeam);

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

						if (bShoot == true)
						{//弾を発射している場合

							for (int nCntShoot = 0; nCntShoot < nNumShoot; nCntShoot++)
							{
								// 弾の生成
								D3DXMATRIX mtxCanon = m_pPlayer[nPlayerIdx]->GetModel(2)->GetMtxWorld();
								D3DXVECTOR3 posCanon = D3DXVECTOR3(mtxCanon._41, mtxCanon._42, mtxCanon._43) + D3DXVECTOR3(sinf(cameraRot.y) * 30.0f, cosf(cameraRot.x) * 30.0f, cosf(cameraRot.y) * 30.0f);
								CBulletPlayer::Create(posCanon, pAngle[nCntShoot * 2], pAngleV[nCntShoot * 2], nAttack,m_pPlayer[nPlayerIdx]->GetTeam());
								mtxCanon = m_pPlayer[nPlayerIdx]->GetModel(3)->GetMtxWorld();
								posCanon = D3DXVECTOR3(mtxCanon._41, mtxCanon._42, mtxCanon._43) + D3DXVECTOR3(sinf(cameraRot.y) * 30.0f, cosf(cameraRot.x) * 30.0f, cosf(cameraRot.y) * 30.0f);
								CBulletPlayer::Create(posCanon, pAngle[nCntShoot * 2 + 1], pAngleV[nCntShoot * 2 + 1], nAttack, m_pPlayer[nPlayerIdx]->GetTeam());

								//弾を発射しているかどうかの設置処理
								m_pPlayer[nPlayerIdx]->SetShoot(false);
							}

							//D3DXMATRIX mtxCanon = m_pPlayer[nPlayerIdx]->GetModel(3)->GetMtxWorld();
							//CBulletPlayer::Create(D3DXVECTOR3(mtxCanon._41, mtxCanon._42, mtxCanon._43) + D3DXVECTOR3(sinf(cameraRot.y) * 30.0f, cosf(cameraRot.x) * 30.0f, cosf(cameraRot.y) * 30.0f), cameraRot.y, cameraRot.x + (D3DX_PI * 0.5f), CBulletPlayer::TYPE_NORMAL);
							//mtxCanon = m_pPlayer[nPlayerIdx]->GetModel(5)->GetMtxWorld();
							//CBulletPlayer::Create(D3DXVECTOR3(mtxCanon._41, mtxCanon._42, mtxCanon._43) + D3DXVECTOR3(sinf(cameraRot.y) * 30.0f, cosf(cameraRot.x) * 30.0f, cosf(cameraRot.y) * 30.0f), cameraRot.y, cameraRot.x + (D3DX_PI * 0.5f), CBulletPlayer::TYPE_NORMAL);
						}
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
}
