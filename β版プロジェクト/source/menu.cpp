//=============================================================================
//
// メニュー処理 [menu.cpp]
// Author : Takuto Ishida & Ayaka Hakozaki
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
#include "UI_Texture.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MENU_LOGO_WIDTH		(420.0f)	// メニューロゴの幅
#define MENU_LOGO_HEIGHT		(90.0f)		// メニューロゴの高さ
#define MENU_LOGO_POS		(D3DXVECTOR3(MENU_LOGO_WIDTH / 2.0f + 10.0f, MENU_LOGO_HEIGHT / 2.0f + 10.0f, 0.0f))			// メニューロゴの位置

#define MENU_MESSAGE_WIDTH		(1280.0f)	// メッセージ表示の幅
#define MENU_MESSAGE_HEIGHT	(70.0f)	// メッセージ表示の高さ

#define MENU_BUTTON_WIDTH	(330.0f)	// ボタンの幅
#define MENU_BUTTON_HEIGHT	(290.0f)	// ボタンの高さ

#define MESS_SCROLL_SPEED		(0.003f)	// スクロールスピード

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
	m_nCntBgMove = 0;
	m_apUITexMess = NULL;

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
	// 生成
	MenuCreate();

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

	if (m_apUITexMess != NULL)
	{	// メッセージの破棄
		m_apUITexMess->Uninit();
		m_apUITexMess = NULL;
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

	for (int nCntButton = 0; nCntButton < MENU_NUM_BUTTON; nCntButton++)
	{// ボタンの判定
		if (m_apButtonUI[nCntButton]->InRangeMenu(m_pCursor->GetMousePosition()))
		{// 範囲内かチェック
			if (m_apButtonUI[nCntButton]->ClickRelease())
			{// クリックされた
				bButtonSwitch = true;
				m_mode = (MODE)nCntButton;
				break;
			}

			nSelect = nCntButton;
		}
	}

	// スクロールカウンター
	m_nCntBgMove++;

	//**********************************************
	// モード説明ロゴ表示
	//**********************************************
	// 選択されていないとき選択項目を非表示にする
	if (-1 == nSelect) { m_apUITexMess->SetDisp(false); }
	else { m_apUITexMess->SetDisp(true); }

		switch (nSelect)
		{
		case 0:		// シングル
			m_apUITexMess->SetTexMove(0, 1, TEX_MENU_MESS, m_nCntBgMove, 0, MESS_SCROLL_SPEED);
			break;

		case 1:		// マルチ
			m_apUITexMess->SetTexMove(1, 1, TEX_MENU_MESS, m_nCntBgMove, 0, MESS_SCROLL_SPEED);
			break;

		case 2:		// チュートリアル
			m_apUITexMess->SetTexMove(2, 1, TEX_MENU_MESS, m_nCntBgMove, 0, MESS_SCROLL_SPEED);
			break;

		case 3:		// quit
			m_apUITexMess->SetTexMove(4, 1, TEX_MENU_MESS, m_nCntBgMove, 0, MESS_SCROLL_SPEED);
			break;

		case 4:		// credit
			m_apUITexMess->SetTexMove(3, 1, TEX_MENU_MESS, m_nCntBgMove, 0, MESS_SCROLL_SPEED);
			break;
		}

	//**********************************************
	// ボタン押された
	//**********************************************
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

			case MODE_TUTORIAL:
				CFade::Create(CManager::MODE_TUTORIAL);
				break;

			case MODE_QUIT:
				CFade::Create(CManager::MODE_TITLE);
				break;

			case MODE_CREDIT:
				//CFade::Create(CManager::MODE_TUTORIAL);
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

//=============================================================================
// ロゴの生成
//=============================================================================
void CMenu::MenuCreate(void)
{
	// 背景の生成
	m_pBg = CBg::Create();

	// メニューロゴ
	if (m_apUI[0] == NULL && m_apUI[1] == NULL)
	{
		m_apUI[0] = CScene2D::Create();
		m_apUI[0]->SetPos(MENU_LOGO_POS);
		m_apUI[0]->SetSize(MENU_LOGO_WIDTH, MENU_LOGO_HEIGHT);
		m_apUI[0]->BindTexture(CTexture::GetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_LOGO)));
		m_apUI[0]->SetTex(0, 1, TEX_MENU_LOGO);

		// インフォメーション
		m_apUI[1] = CScene2D::Create();
		m_apUI[1]->SetPos(D3DXVECTOR3(SCREEN_WIDTH / 2, 650.0f, 0.0f));
		m_apUI[1]->SetSize(SCREEN_WIDTH, 140.0f);
		m_apUI[1]->BindTexture(CTexture::GetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_MENU_INFOMETION)));
	}

	// メニューメッセージ
	if (m_apUITexMess == NULL)
	{
		m_apUITexMess = CUI_TEXTURE::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, 685.0f, 0.0f), MENU_MESSAGE_WIDTH, MENU_MESSAGE_HEIGHT, CUI_TEXTURE::UIFLAME_MENU_MESS);
	}

	// ボタンの生成
	if (m_apButtonUI[0] == NULL && m_apButtonUI[1] == NULL && m_apButtonUI[2] == NULL && m_apButtonUI[3] == NULL && m_apButtonUI[4] == NULL && m_apButtonUI[5] == NULL)
	{
		m_apButtonUI[0] = CButton2D::Create(D3DXVECTOR3(250.0f, 325.0f, 0.0f), MENU_BUTTON_WIDTH, MENU_BUTTON_HEIGHT);				// シングルプレイ
		m_apButtonUI[0]->BindTexture(CTexture::GetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_MENU_FLAME)));
		m_apButtonUI[0]->SetTex(0, 1, TEX_MENU_FLAME);

		m_apButtonUI[1] = CButton2D::Create(D3DXVECTOR3(495.0f, 465.0f, 0.0f), MENU_BUTTON_WIDTH, MENU_BUTTON_HEIGHT);				// マルチプレイ
		m_apButtonUI[1]->BindTexture(CTexture::GetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_MENU_FLAME)));
		m_apButtonUI[1]->SetTex(1, 1, TEX_MENU_FLAME);

		m_apButtonUI[2] = CButton2D::Create(D3DXVECTOR3(740.0f, 325.0f, 0.0f), MENU_BUTTON_WIDTH, MENU_BUTTON_HEIGHT);				// チュートリアル
		m_apButtonUI[2]->BindTexture(CTexture::GetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_MENU_FLAME)));
		m_apButtonUI[2]->SetTex(2, 1, TEX_MENU_FLAME);

		m_apButtonUI[3] = CButton2D::Create(D3DXVECTOR3(985.0f, 465.0f, 0.0f), MENU_BUTTON_WIDTH, MENU_BUTTON_HEIGHT);				// quit
		m_apButtonUI[3]->BindTexture(CTexture::GetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_MENU_FLAME)));
		m_apButtonUI[3]->SetTex(4, 1, TEX_MENU_FLAME);

		m_apButtonUI[4] = CButton2D::Create(D3DXVECTOR3(985.0f, 185.0f, 0.0f), MENU_BUTTON_WIDTH, MENU_BUTTON_HEIGHT);				// credit
		m_apButtonUI[4]->BindTexture(CTexture::GetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_MENU_FLAME)));
		m_apButtonUI[4]->SetTex(3, 1, TEX_MENU_FLAME);
	}

	// カーソルの生成
	if (m_pCursor == NULL)
	{
		m_pCursor = CMouseCursor2D::Create();
	}
}
