//=============================================================================
//
// リザルト処理 [result.cpp]
// Author : Ayaka Hakozaki
//
//=============================================================================
#include "result.h"
#include "fade.h"
#include "input.h"
#include "title.h"
#include "manager.h"
#include "debugProc.h"
#include "number.h"
#include "sound.h"
#include "UI_Texture.h"
#include "camera.h"
#include "model.h"
#include "player.h"
#include "mechaSelect.h"
#include "motion.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define RESULT_PRESS_WIDTH		(500.0f)		// 入力待ち表示の幅
#define RESULT_PRESS_HEIGHT		(100.0f)		// 入力待ち表示の高さ

#define RESULT_LOGO_WIDTH			(450.0f)		// ロゴの幅
#define RESULT_LOGO_HEIGHT		(150.0f)		// ロゴの高さ

#define TEAM_WIN_LOGO					(2)				// ロゴの数
#define RESULT_FLASH						(40)				// ボタンを入力する前の点滅
#define FIELD_MODEL_NAME			"data/MODEL/map_UV_bill.x"

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
CResult::TEAM_WIN CResult::m_teamWin = TEAM_WIN_BLUE;
CPlayer *CResult::m_pPlayer[MAX_PLAYER_CONNECT] = {};

//=========================================
// コンストラクタ
//=========================================
CResult::CResult(int nPriority, CScene::OBJTYPE objType) : CScene(nPriority, objType)
{
	for (int nCnt = 0; nCnt < RESULT_UI_TEX; nCnt++)
	{
		m_pUITex[nCnt] = NULL;
	}

	for (int nCntType = 0; nCntType < MAX_PLAYER_CONNECT; nCntType++)
	{
		m_mechaType[nCntType] = CMechaSelect::MECHATYPE_EMPTY;
	}

	m_pField = NULL;
	m_nCntFlash = 0;
}

//=========================================
// デストラクタ
//=========================================
CResult::~CResult()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CResult::Init(void)
{
	CMotionManager::Load();

	// オブジェクトタイプの設定
	//CScene::SetObjType(CScene::OBJTYPE_RESULT);

	// モードの取得
	CManager::MODE mode = CManager::GetMode();

	// フィールドの生成
	m_pField = CModel::Create();
	m_pField->SetModel(FIELD_MODEL_NAME);

	// 点滅
	m_nCntFlash = RESULT_FLASH;

	// プレスボタン生成
	if (m_pUITex[0] == NULL)
	{
		m_pUITex[0] = CUI_TEXTURE::Create(D3DXVECTOR3(1000.0f, 650.0f, 0.0f), RESULT_PRESS_WIDTH, RESULT_PRESS_HEIGHT, CUI_TEXTURE::UIFLAME_PRESSBOTTON);		// プレスボタン
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CResult::Uninit(void)
{
	CMotionManager::Unload();

	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER_CONNECT; nCntPlayer++)
	{
		if (m_pPlayer[nCntPlayer] != NULL)
		{
			m_pPlayer[nCntPlayer]->Uninit();
			m_pPlayer[nCntPlayer] = NULL;
		}
	}

	for (int nCnt = 0; nCnt < RESULT_UI_TEX; nCnt++)
	{
		if (m_pUITex[nCnt] != NULL)
		{	// UIテクスチャの破棄
			m_pUITex[nCnt]->Uninit();
			m_pUITex[nCnt] = NULL;
		}
	}

	if (m_pField != NULL)
	{	// フィールドの破棄
		m_pField->Uninit();
		delete m_pField;
		m_pField = NULL;
	}

	// クライアントの破棄
	CManager::ReleaseClient();

	// 自分を破棄
	CScene::Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CResult::Update(void)
{
	// 入力の情報を取得
	CInputKeyboard	*pKeyboard = CManager::GetInputKeyboard();	// キーボードの取得
	CXInput			*pXInput = CManager::GetXInput();			// XInputの取得
	CSound			*pSound = CManager::GetSound();				// サウンドの取得

	// プレスボタン点滅
	if (m_pUITex[0] != NULL)
	{
		m_pUITex[0]->Flashing(m_nCntFlash);
	}

	switch (m_teamWin)
	{
	case TEAM_WIN_BLUE:
		if (m_pUITex[1] == NULL)
		{
			m_pUITex[1] = CUI_TEXTURE::Create(D3DXVECTOR3(250.0f, 100.0f, 0.0f), RESULT_LOGO_WIDTH, RESULT_LOGO_HEIGHT, CUI_TEXTURE::UIFLAME_TEAM_WIN);		// チームロゴ
			m_pUITex[1]->SetTex(0, 1, TEAM_WIN_LOGO);

			if (m_pPlayer[0] == NULL && m_pPlayer[1] == NULL && m_pPlayer[2] == NULL && m_pPlayer[3] == NULL)
			{
				// BLUEが手前
				m_pPlayer[0] = CPlayer::Create(0, CGame::GetMechaType(0), D3DXVECTOR3(23.0f, 0.0f, -89.0f), true);
				m_pPlayer[0]->SetRot(D3DXVECTOR3(0.0f, 100.0f, 0.0f));
				m_pPlayer[1] = CPlayer::Create(1, CGame::GetMechaType(1), D3DXVECTOR3(-15.0f, 0.0f, -139.0f), true);
				m_pPlayer[1]->SetRot(D3DXVECTOR3(0.0f, 100.0f, 0.0f));

				// REDが奥
				m_pPlayer[2] = CPlayer::Create(2, CGame::GetMechaType(2), D3DXVECTOR3(-133.0f, 0.0f, -160.0f), true);
				m_pPlayer[2]->SetRot(D3DXVECTOR3(0.0f, -200.0f, 0.0f));
				m_pPlayer[3] = CPlayer::Create(3, CGame::GetMechaType(3), D3DXVECTOR3(-133.0f, 0.0f, -115.0f), true);
				m_pPlayer[3]->SetRot(D3DXVECTOR3(0.0f, -200.0f, 0.0f));
			}
		}
		break;

	case TEAM_WIN_RED:
		if (m_pUITex[1] == NULL)
		{
			m_pUITex[1] = CUI_TEXTURE::Create(D3DXVECTOR3(250.0f, 100.0f, 0.0f), RESULT_LOGO_WIDTH, RESULT_LOGO_HEIGHT, CUI_TEXTURE::UIFLAME_TEAM_WIN);		// チームロゴ
			m_pUITex[1]->SetTex(1, 1, TEAM_WIN_LOGO);
		}

		if (m_pPlayer[0] == NULL && m_pPlayer[1] == NULL && m_pPlayer[2] == NULL && m_pPlayer[3] == NULL)
		{
			// BLUEが奥
			m_pPlayer[0] = CPlayer::Create(0, CGame::GetMechaType(0), D3DXVECTOR3(-133.0f, 0.0f, -160.0f), true);
			m_pPlayer[0]->SetRot(D3DXVECTOR3(0.0f, -200.0f, 0.0f));
			m_pPlayer[1] = CPlayer::Create(1, CGame::GetMechaType(1), D3DXVECTOR3(-133.0f, 0.0f, -115.0f), true);
			m_pPlayer[1]->SetRot(D3DXVECTOR3(0.0f, -200.0f, 0.0f));

			// REDが手前
			m_pPlayer[2] = CPlayer::Create(2, CGame::GetMechaType(2), D3DXVECTOR3(23.0f, 0.0f, -89.0f), true);
			m_pPlayer[2]->SetRot(D3DXVECTOR3(0.0f, 100.0f, 0.0f));
			m_pPlayer[3] = CPlayer::Create(3, CGame::GetMechaType(3), D3DXVECTOR3(-15.0f, 0.0f, -139.0f), true);
			m_pPlayer[3]->SetRot(D3DXVECTOR3(0.0f, 100.0f, 0.0f));
		}
		break;
	}

	if (pKeyboard->GetTrigger(DIK_RETURN))
	{// 画面遷移
		if (CFade::GetFade() == CFade::FADE_NONE)
		{// フェードがないとき
			CFade::Create(CManager::MODE_TITLE);
			//pSound->PlaySound(CSound::SOUND_LABEL_DECIDE);
		}
	}

	CDebugProc::Print("リザルト");
}

//=============================================================================
// 描画処理
//=============================================================================
void CResult::Draw(void)
{
	if (m_pField != NULL)
	{	// フィールドの描画
		m_pField->Draw();
	}
}
