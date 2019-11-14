//=============================================================================
//
// メカ選択処理 [mechaSelect.cpp]
// Author : Ayaka Hakozaki
//
//=============================================================================
#include "mechaSelect.h"
#include "fade.h"
#include "input.h"
#include "debugProc.h"
#include "sound.h"
#include "texture.h"
#include "bg.h"
#include "mouseCursor.h"
#include "button.h"
#include "server.h"
#include "menu.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MENU_LOGO_WIDTH		(450.0f)	// メニューロゴの幅
#define MENU_LOGO_HEIGHT	(120.0f)	// メニューロゴの高さ
#define MENU_LOGO_POS		(D3DXVECTOR3(MENU_LOGO_WIDTH / 2.0f + 20.0f, MENU_LOGO_HEIGHT / 2.0f + 20.0f, 0.0f))			// メニューロゴの位置

#define MENU_SELECT_WIDTH	(630.0f)	// 選択項目表示の幅
#define MENU_SELECT_HEIGHT	(130.0f)	// 選択項目表示の高さ
#define MENU_SELECT_POS		(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f + (MENU_SELECT_HEIGHT / 2.0f + 20.0f), 0.0f))		// 選択項目表示の位置

#define MENU_BUTTON_WIDTH	(350.0f)	// ボタンの幅
#define MENU_BUTTON_HEIGHT	(130.0f)	// ボタンの高さ

#define BOTTON_INTERVAL			(40.0f)		// ボタンとボタンの間隔

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
CMechaSelect::MECHATYPE CMechaSelect::m_mechaType = CMechaSelect::MECHATYPE_SHOOTER;

//=========================================
// コンストラクタ
//=========================================
CMechaSelect::CMechaSelect(int nPriority, CScene::OBJTYPE objType) : CScene(nPriority, objType)
{
	m_pBg = NULL;
	m_pCursor = NULL;

	for (int nCntButton = 0; nCntButton < MECHASEL_NUM_BUTTON; nCntButton++)
	{
		m_apButtonUI[nCntButton] = NULL;
	}

	for (int nCntUI = 0; nCntUI < MECHASEL_NUM_UI; nCntUI++)
	{
		m_apUI[nCntUI] = NULL;
	}
}

//=========================================
// デストラクタ
//=========================================
CMechaSelect::~CMechaSelect()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CMechaSelect::Init(void)
{
	// 背景の生成
	m_pBg = CBg::Create();

	// メニューロゴ
	m_apUI[0] = CScene2D::Create();
	m_apUI[0]->SetPos(MENU_LOGO_POS);
	m_apUI[0]->SetSize(MENU_LOGO_WIDTH, MENU_LOGO_HEIGHT);
	m_apUI[0]->BindTexture(CTexture::GetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_LOGO)));

	// 選択中の機体説明
	m_apUI[1] = CScene2D::Create();
	m_apUI[1]->SetPos(D3DXVECTOR3(340.0f, 630.0f, 0.0f));
	m_apUI[1]->SetSize(MENU_SELECT_WIDTH, MENU_SELECT_HEIGHT);
	m_apUI[1]->BindTexture(CTexture::GetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_MECHASEL_INST)));

	// 選択中の機体表示
	m_apUI[2] = CScene2D::Create();
	m_apUI[2]->SetPos(D3DXVECTOR3(300.0f, 350.0f, 0.0f));
	m_apUI[2]->SetSize(400.0f, 400.0f);
	m_apUI[2]->BindTexture(CTexture::GetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_AI_MECHA_FLAME)));

	// 適正距離表示
	m_apUI[3] = CScene2D::Create();
	m_apUI[3]->SetPos(D3DXVECTOR3(800.0f, SCREEN_HEIGHT / 2, 0.0f));
	m_apUI[3]->SetSize(80.0f, 660.0f);
	m_apUI[3]->BindTexture(CTexture::GetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_ADAPT_RANGE)));

	// ボタン（機体のタイプ）の生成
	for (int nCntButton = 0; nCntButton < MECHASEL_NUM_BUTTON; nCntButton++)
	{
		if (m_apButtonUI[nCntButton] == NULL)
		{
			m_apButtonUI[nCntButton] = CButton2D::Create(D3DXVECTOR3(1070.0f, 100.0f + (nCntButton * (MENU_BUTTON_HEIGHT + BOTTON_INTERVAL)), 0.0f), MENU_BUTTON_WIDTH, MENU_BUTTON_HEIGHT);
			m_apButtonUI[nCntButton]->BindTexture(CTexture::GetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_MECHATYPE)));
			m_apButtonUI[nCntButton]->SetTex(nCntButton, 1, 4);
		}
	}

	// カーソルの生成
	m_pCursor = CMouseCursor2D::Create();

	//メカ選択の初期化
	m_mechaType = MECHATYPE_LIGHT;
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CMechaSelect::Uninit(void)
{
	for (int nCntButton = 0; nCntButton < MECHASEL_NUM_BUTTON; nCntButton++)
	{
		if (NULL != m_apButtonUI[nCntButton])
		{// ボタンUIの破棄
			m_apButtonUI[nCntButton]->Uninit();
			m_apButtonUI[nCntButton] = NULL;
		}
	}

	for (int nCntUI = 0; nCntUI < MECHASEL_NUM_UI; nCntUI++)
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
void CMechaSelect::Update(void)
{
	// 入力の情報を取得
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();
	CXInput *pXInput = CManager::GetXInput();

	// サウンドの取得
	CSound *pSound = CManager::GetSound();

	bool bButtonSwitch = false;
	int nSelect = -1;
	//MECHATYPE mechaType = MECHATYPE_LIGHT;

	for (int nCntButton = 0; nCntButton < MECHASEL_NUM_BUTTON; nCntButton++)
	{// ボタンの判定
		if (m_apButtonUI[nCntButton]->InRange(m_pCursor->GetMousePosition()))
		{// 範囲内かチェック
			if (m_apButtonUI[nCntButton]->ClickRelease())
			{// クリックされた
				bButtonSwitch = true;
				//mechaType = (MECHATYPE)nCntButton;
				m_mechaType = (MECHATYPE)nCntButton;
				break;
			}

			nSelect = nCntButton;
		}
	}

	//**********************************************
	// 選択されていないとき選択項目を非表示にする
	//**********************************************
	// 選択中の機体説明
	if (-1 == nSelect) { m_apUI[1]->SetDisp(false); }
	else{ m_apUI[1]->SetDisp(true); }

	switch (nSelect)
	{
	case 0:		// タイプA
		m_apUI[1]->SetTex(0, 1, 4);
		break;

	case 1:		// タイプB
		m_apUI[1]->SetTex(1, 1, 4);
		break;

	case 2:		// タイプC
		m_apUI[1]->SetTex(2, 1, 4);
		break;

	case 3:		// タイプD
		m_apUI[1]->SetTex(3, 1, 4);
		break;
	}

	// 選択中の機体表示
	if (-1 == nSelect) { m_apUI[2]->SetDisp(false); }
	else { m_apUI[2]->SetDisp(true); }

	switch (nSelect)
	{
	case 0:		// タイプA
		m_apUI[2]->SetTex(0, 1, 2);
		break;

	case 1:		// タイプB
		m_apUI[2]->SetTex(1, 1, 2);
		break;

	case 2:		// タイプC
		m_apUI[2]->SetTex(0, 1, 2);
		break;

	case 3:		// タイプD
		m_apUI[2]->SetTex(1, 1, 2);
		break;
	}

	if (bButtonSwitch)
	{// 画面遷移
		if (CFade::GetFade() == CFade::FADE_NONE)
		{// フェードがないとき
			CManager::MODE mode;

			if (CMenu::GetMode() == CMenu::MODE_MULTI)
			{
				mode = CManager::MODE_MATCHING;
			}
			else
			{
				mode = CManager::MODE_GAME;
			}

			switch (m_mechaType)
			{
			case MECHATYPE_LIGHT:
				CFade::Create(mode);
				break;

			case MECHATYPE_SHOOTER:
				CFade::Create(mode);
				break;

			case MECHATYPE_HEAVY:
				CFade::Create(mode);
				break;

			case MECHATYPE_ASSULT:
				CFade::Create(mode);
				break;
			}
		}
	}

	CDebugProc::Print("メカ選択画面");
}

//=============================================================================
// 描画処理
//=============================================================================
void CMechaSelect::Draw(void)
{
}