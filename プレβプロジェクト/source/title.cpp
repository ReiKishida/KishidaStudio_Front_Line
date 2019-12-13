//=============================================================================
//
// タイトル処理 [title.cpp]
// Author : Takuto Ishida
//
//=============================================================================
#include "title.h"
#include "renderer.h"
#include "fade.h"
#include "input.h"
#include "manager.h"
#include "debugProc.h"
#include "logo.h"
#include "sound.h"
#include "UI_Texture.h"		// 追加
#include "texture.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TITLE_TIME		(1200)		// ランキング画面までの時間

#define TITLE_LOGO_POS		(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, 250.0f, 0.0f))	// タイトルロゴの位置
#define TITLE_LOGO_WIDTH	(700.0f)	// タイトルロゴの幅
#define TITLE_LOGO_HEIGHT	(300.0f)	// タイトルロゴの高さ

#define TITLE_PRESSANY_POS		(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, 600.0f, 0.0f))	// 入力待ち表示の位置
#define TITLE_PRESSANY_WIDTH	(500.0f)	// 入力待ちの幅
#define TITLE_PRESSANY_HEIGHT	(100.0f)	// 入力待ちの高さ
#define TITLE_BEFORE_FLASH		(60)		// ボタンを入力する前の点滅
#define TITLE_AFTER_FLASH		(5)			// ボタンを入力した後の点滅

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
CUI_TEXTURE *CTitle::m_pUI[TITLE_UI] = {};

//=========================================
// コンストラクタ
//=========================================
CTitle::CTitle(int nPriority, CScene::OBJTYPE objType) : CScene(nPriority, objType)
{
	m_nFlash = 0;
	m_pBg = NULL;
	m_pUI[TITLE_UI] = {};
}

//=========================================
// デストラクタ
//=========================================
CTitle::~CTitle()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CTitle::Init(void)
{
	// 点滅する間隔
	m_nFlash = TITLE_BEFORE_FLASH;

	//CTexture::Load();

	// Bgスクロール
	CUI_TEXTURE::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), 1280.0f, 720.0f, CUI_TEXTURE::UIFLAME_BG_00);	// bg01
	CUI_TEXTURE::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), 1280.0f, 720.0f, CUI_TEXTURE::UIFLAME_BG_01);	// bg02

	// UI表示
	m_pUI[0] = CUI_TEXTURE::Create(TITLE_LOGO_POS, TITLE_LOGO_WIDTH, TITLE_LOGO_HEIGHT, CUI_TEXTURE::UIFLAME_FLONTLINE);		// フロントライン
	m_pUI[1] = CUI_TEXTURE::Create(TITLE_PRESSANY_POS, TITLE_PRESSANY_WIDTH, TITLE_PRESSANY_HEIGHT, CUI_TEXTURE::UIFLAME_PRESSBOTTON);		// プレスボタン

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CTitle::Uninit(void)
{
	for (int nCntUI = 0; nCntUI < TITLE_UI; nCntUI++)
	{	// UIの破棄
		if (m_pUI[nCntUI] != NULL)
		{
			m_pUI[nCntUI]->Uninit();
			m_pUI[nCntUI] = NULL;
		}
	}

	//if (NULL != m_pPressButton)
	//{// ロゴの破棄
	//	m_pPressButton->Uninit();
	//	m_pPressButton = NULL;
	//}

	// 自分の破棄
	CScene::Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CTitle::Update(void)
{
	// 入力の情報を取得
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();
	CXInput *pXInput = CManager::GetXInput();
	CInputMouse *pMouse = CManager::GetInputMouse();

	// サウンドの取得
	CSound *pSound = CManager::GetSound();

	// プレスボタンだけ、点滅する
	m_pUI[1]->Flashing(m_nFlash);

	if (pKeyboard->GetAnyKey() || pXInput->GetAnyButton(0) || pMouse->GetTrigger(CInputMouse::DIMS_BUTTON_0))
	{// 画面遷移
		if (CFade::GetFade() == CFade::FADE_NONE)
		{// フェードがないとき
			CFade::Create(CManager::MODE_MENU);
			pSound->PlaySound(CSound::SOUND_LABEL_DECIDE);
		}
	}

	CDebugProc::Print("タイトル");
}

//=============================================================================
// 描画処理
//=============================================================================
void CTitle::Draw(void)
{
}