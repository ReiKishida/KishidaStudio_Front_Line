//=============================================================================
//
// メニュー処理 [menu.cpp]
// Author : Takuto Ishida
//
//=============================================================================
#include "menu.h"
#include "fade.h"
#include "input.h"
#include "debugProc.h"
#include "sound.h"
#include "texture.h"
#include "bg.h"
#include "mouseCursor.h"
#include "button.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MENU_LOGO_WIDTH		(450.0f)	// メニューロゴの幅
#define MENU_LOGO_HEIGHT	(120.0f)	// メニューロゴの高さ
#define MENU_LOGO_POS		(D3DXVECTOR3(MENU_LOGO_WIDTH / 2.0f + 20.0f, MENU_LOGO_HEIGHT / 2.0f + 20.0f, 0.0f))			// メニューロゴの位置

#define MENU_SELECT_WIDTH	(550.0f)	// 選択項目表示の幅
#define MENU_SELECT_HEIGHT	(130.0f)	// 選択項目表示の高さ
#define MENU_SELECT_POS		(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f + (MENU_SELECT_HEIGHT / 2.0f + 20.0f), 0.0f))		// 選択項目表示の位置

#define MENU_MESSAGE_WIDTH	(1200.0f)	// メッセージ表示の幅
#define MENU_MESSAGE_HEIGHT	(190.0f)	// メッセージ表示の高さ
#define MENU_MESSAGE_POS	(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT - (MENU_MESSAGE_HEIGHT / 2.0f + 10.0f), 0.0f))	// メッセージ表示の位置

#define MENU_BUTTON_WIDTH	(380.0f)	// ボタンの幅
#define MENU_BUTTON_HEIGHT	(170.0f)	// ボタンの高さ

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
CMenu::MODE CMenu::m_mode = CMenu::MODE_SINGLE;

//=========================================
// コンストラクタ
//=========================================
CMenu::CMenu(int nPriority, CScene::OBJTYPE objType) : CScene(nPriority, objType)
{
	m_pBg = NULL;
	m_pCursor = NULL;

	for (int nCntButton = 0; nCntButton < MENU_NUM_BUTTON; nCntButton++)
	{
		m_apButtonUI[nCntButton] = NULL;
	}

	for (int nCntUI = 0; nCntUI < MENU_NUM_UI; nCntUI++)
	{
		m_apUI[nCntUI] = NULL;
	}
}

//=========================================
// デストラクタ
//=========================================
CMenu::~CMenu()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CMenu::Init(void)
{
	// 背景の生成
	m_pBg = CBg::Create();

	// メニューロゴ
	m_apUI[0] = CScene2D::Create();
	m_apUI[0]->SetPos(MENU_LOGO_POS);
	m_apUI[0]->SetSize(MENU_LOGO_WIDTH, MENU_LOGO_HEIGHT);
	m_apUI[0]->BindTexture(CTexture::GetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_LOGO)));
	m_apUI[0]->SetTex(0, 1, 2);

	// 選択項目表示
	m_apUI[1] = CScene2D::Create();
	m_apUI[1]->SetPos(MENU_SELECT_POS);
	m_apUI[1]->SetSize(MENU_SELECT_WIDTH, MENU_SELECT_HEIGHT);
	m_apUI[1]->BindTexture(CTexture::GetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_MENU_DISP)));

	// メッセージ表示
	m_apUI[2] = CScene2D::Create();
	m_apUI[2]->SetPos(MENU_MESSAGE_POS);
	m_apUI[2]->SetSize(MENU_MESSAGE_WIDTH, MENU_MESSAGE_HEIGHT);
	m_apUI[2]->BindTexture(CTexture::GetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_MENU_INST)));

	// ボタンの生成
	for (int nCntButton = 0; nCntButton < MENU_NUM_BUTTON; nCntButton++)
	{
		float fPosX = SCREEN_WIDTH / 2.0f + (nCntButton * (MENU_BUTTON_WIDTH + 50.0f)) - ((MENU_BUTTON_WIDTH + 50.0f) * (MENU_NUM_BUTTON / 2.0f)) + ((MENU_BUTTON_WIDTH + 50.0f) / 2.0f);
		m_apButtonUI[nCntButton] = CButton2D::Create(D3DXVECTOR3(fPosX, SCREEN_HEIGHT / 2.0f - (MENU_BUTTON_HEIGHT / 2.0f), 0.0f), MENU_BUTTON_WIDTH, MENU_BUTTON_HEIGHT);
		m_apButtonUI[nCntButton]->BindTexture(CTexture::GetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_MENU_FLAME)));
		m_apButtonUI[nCntButton]->SetTex(nCntButton, 1, 3);
	}

	// カーソルの生成
	m_pCursor = CMouseCursor2D::Create();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CMenu::Uninit(void)
{
	for (int nCntButton = 0; nCntButton < MENU_NUM_BUTTON; nCntButton++)
	{
		if (NULL != m_apButtonUI[nCntButton])
		{// ボタンUIの破棄
			m_apButtonUI[nCntButton]->Uninit();
			m_apButtonUI[nCntButton] = NULL;
		}
	}

	for (int nCntUI = 0; nCntUI < MENU_NUM_UI; nCntUI++)
	{
		if (NULL != m_apUI[nCntUI])
		{// UIの破棄
			m_apUI[nCntUI]->Uninit();
			m_apUI[nCntUI] = NULL;
		}
	}

	if (NULL != m_pBg)
	{// 背景の破棄
		m_pBg->Uninit();
		m_pBg = NULL;
	}

	if (NULL != m_pCursor)
	{// カーソルの破棄
		m_pCursor->Uninit();
		m_pCursor = NULL;
	}

	// 自分の破棄
	CScene::Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CMenu::Update(void)
{
	// 入力の情報を取得
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();
	CXInput *pXInput = CManager::GetXInput();

	// サウンドの取得
	CSound *pSound = CManager::GetSound();

	bool bButtonSwitch = false;
	int nSelect = -1;
	//MODE mode = MODE_SINGLE;

	for (int nCntButton = 0; nCntButton < MENU_NUM_BUTTON; nCntButton++)
	{// ボタンの判定
		if (m_apButtonUI[nCntButton]->InRange(m_pCursor->GetMousePosition()))
		{// 範囲内かチェック
			if (m_apButtonUI[nCntButton]->ClickRelease())
			{// クリックされた
				bButtonSwitch = true;
				//mode = (MODE)nCntButton;
				m_mode = (MODE)nCntButton;
				break;
			}

			nSelect = nCntButton;
		}
	}

	//**********************************************
	// モード説明ロゴ表示
	//**********************************************
	// 選択されていないとき選択項目を非表示にする
	if (-1 == nSelect) { m_apUI[1]->SetDisp(false); }
	else{ m_apUI[1]->SetDisp(true); }

	switch (nSelect)
	{
	case 0:		// シングルプレイ
		m_apUI[1]->SetTex(0, 1, 3);
		break;

	case 1:		// マルチプレイ
		m_apUI[1]->SetTex(1, 1, 3);
		break;

	case 2:		// タイトル戻る
		m_apUI[1]->SetTex(2, 1, 3);
		break;
	}

	//**********************************************
	// モード概要表示
	//**********************************************
	// 選択されていないとき選択項目を非表示にする
	if (-1 == nSelect) { m_apUI[2]->SetDisp(false); }
	else { m_apUI[2]->SetDisp(true); }

	switch (nSelect)
	{
	case 0:		// シングルプレイ
		m_apUI[2]->SetTex(0, 1, 3);
		break;

	case 1:		// マルチプレイ
		m_apUI[2]->SetTex(1, 1, 3);
		break;

	case 2:		// タイトル戻る
		m_apUI[2]->SetTex(2, 1, 3);
		break;
	}

	if (bButtonSwitch)
	{// 画面遷移
		if (CFade::GetFade() == CFade::FADE_NONE)
		{// フェードがないとき
			switch(m_mode)
			{
			case MODE_SINGLE:
				CFade::Create(CManager::MODE_MECHASELECT);
				//pSound->PlaySound(CSound::SOUND_LABEL_DECIDE);
				break;

			case MODE_MULTI:
				CFade::Create(CManager::MODE_MECHASELECT);
				break;

			case MODE_QUIT:
				CFade::Create(CManager::MODE_TITLE);
				break;
			}
		}
	}

	CDebugProc::Print("メニュー");
}

//=============================================================================
// 描画処理
//=============================================================================
void CMenu::Draw(void)
{
}