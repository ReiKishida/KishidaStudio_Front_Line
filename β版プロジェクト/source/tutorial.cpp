//=============================================================================
//
// チュートリアル処理 [tutorial.cpp]
// Author : Takuto Ishida
//
//=============================================================================
#include "tutorial.h"
#include "renderer.h"
#include "fade.h"
#include "input.h"
#include "manager.h"
#include "debugProc.h"
#include "logo.h"
#include "sound.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TUTORIAL_PRESSANY_POS		(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, 650.0f, 0.0f))	// 入力待ち表示の位置
#define TUTORIAL_PRESSANY_WIDTH		(500.0f)	// 入力待ちの幅
#define TUTORIAL_PRESSANY_HEIGHT	(100.0f)	// 入力待ちの高さ
#define TUTORIAL_BEFORE_FLASH		(60)		// ボタンを入力する前の点滅
#define TUTORIAL_AFTER_FLASH		(5)			// ボタンを入力した後の点滅

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************

//=========================================
// コンストラクタ
//=========================================
CTutorial::CTutorial()
{
	m_pTexture = NULL;
}

//=========================================
// デストラクタ
//=========================================
CTutorial::~CTutorial()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CTutorial::Init(void)
{
	// オブジェクトタイプの設定
	CScene::SetObjType(CScene::OBJTYPE_TUTORIAL);

	// 一枚絵の生成
	//m_pPolygon = CScene2D::Create();
	//m_pPolygon->SetSize(SCREEN_WIDTH, SCREEN_HEIGHT);

	// ロゴの生成
	m_pPressButton = CLogoPressButton::Create(TUTORIAL_PRESSANY_POS, TUTORIAL_PRESSANY_WIDTH, TUTORIAL_PRESSANY_HEIGHT);

	// 色の設定
	m_pPressButton->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));

	// デバイスの取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	// 点滅する間隔
	m_nFlash = TUTORIAL_BEFORE_FLASH;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CTutorial::Uninit(void)
{
	if (NULL != m_pPressButton)
	{// ロゴの破棄
		m_pPressButton->Uninit();
		m_pPressButton = NULL;
	}

	if (NULL != m_pPolygon)
	{// 一枚絵の破棄
		m_pPolygon->Uninit();
		m_pPolygon = NULL;
	}

	// 自分の破棄
	CScene::Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CTutorial::Update(void)
{
	// 入力の情報を取得
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();
	CXInput *pXInput = CManager::GetXInput();

	// サウンドの取得
	CSound *pSound = CManager::GetSound();

	m_pPressButton->Flashing(m_nFlash);	// 入力待ちテクスチャを点滅

	if (pKeyboard->GetAnyKey() || pXInput->GetAnyButton(0))
	{// 画面遷移
		if (CFade::GetFade() == CFade::FADE_NONE)
		{// フェードがないとき
			CFade::Create(CManager::MODE_GAME);
			//pSound->PlaySound(CSound::SOUND_LABEL_DECIDE);

			// 点滅する速度を上げる
			m_nFlash = TUTORIAL_AFTER_FLASH;
		}
	}

	CDebugProc::Print("チュートリアル");
}

//=============================================================================
// 描画処理
//=============================================================================
void CTutorial::Draw(void)
{
}
