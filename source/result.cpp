//=============================================================================
//
// リザルト処理 [result.cpp]
// Author : Takuto Ishida
//
//=============================================================================
#include "result.h"
#include "fade.h"
#include "input.h"
#include "title.h"
#include "manager.h"
#include "debugProc.h"
#include "number.h"
#include "logo.h"
#include "sound.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define RESULT_TIME				(420)			// タイトル画面までの時間
#define RESULT_NUMBER_SIZE		(200.0f)		// 数字の大きさ
#define RESULT_LOGO_WIDTH		(600.0f)		// ロゴの幅
#define RESULT_LOGO_HEIGHT		(150.0f)		// ロゴの高さ

#define RESULT_PRESS_WIDTH		(500.0f)		// 入力待ち表示の幅
#define RESULT_PRESS_HEIGHT		(100.0f)		// 入力待ち表示の高さ
#define RESULT_BEFORE_FLASH		(60)			// ボタンを入力する前の点滅
#define RESULT_AFTER_FLASH		(5)				// ボタンを入力した後の点滅

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
int	CResult::m_nScore = 0;

//=========================================
// コンストラクタ
//=========================================
CResult::CResult()
{
	m_nCntResult = 0;
	m_nDigit = 0;
	m_nFlash = 0;
	m_pLogo = NULL;
	m_pPressButton = NULL;
	m_pNumber = NULL;
	m_pScore = NULL;
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
	// オブジェクトタイプの設定
	CScene::SetObjType(CScene::OBJTYPE_RESULT);

	// モードの取得
	CManager::MODE mode = CManager::GetMode();

	// ロゴを生成
	m_pLogo = CLogo::Create(D3DXVECTOR3(SCREEN_WIDTH / 2,100.0f,0.0f), RESULT_LOGO_WIDTH, RESULT_LOGO_HEIGHT);
	m_pLogo->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));	// 色の設定

	// 入力待ち表示を生成
	m_pPressButton = CLogoPressButton::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, 600.0f, 0.0f), RESULT_PRESS_WIDTH, RESULT_PRESS_HEIGHT);
	m_pPressButton->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));	// 色の設定

	// 入力待ち表示の点滅速度
	m_nFlash = RESULT_BEFORE_FLASH;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CResult::Uninit(void)
{
	if (NULL != m_pLogo)
	{// ロゴの破棄
		m_pLogo->Uninit();
		m_pLogo = NULL;
	}

	for (int nCntNumber = 0; nCntNumber < m_nDigit; nCntNumber++)
	{// 桁数分ループ
		if (NULL != m_pNumber[nCntNumber])
		{// 数字の破棄
			m_pNumber[nCntNumber]->Uninit();
			m_pNumber[nCntNumber] = NULL;

		}
	}

	if (NULL != m_pNumber)
	{// 数字の破棄
		delete m_pNumber;
		m_pNumber = NULL;
	}

	if (NULL != m_pPressButton)
	{// ポリゴンの破棄
		m_pPressButton->Uninit();
		m_pPressButton = NULL;
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
	// リザルトの経過時間をカウント
	m_nCntResult++;

	// 点滅
	m_pPressButton->Flashing(m_nFlash);

	// 入力の情報を取得
	CInputKeyboard	*pKeyboard = CManager::GetInputKeyboard();	// キーボードの取得
	CXInput			*pXInput = CManager::GetXInput();			// XInputの取得
	CSound			*pSound = CManager::GetSound();				// サウンドの取得


	if (pKeyboard->GetAnyKey() || pXInput->GetAnyButton(0))
	{// 画面遷移
		if (CFade::GetFade() == CFade::FADE_NONE)
		{// フェードがないとき
			CFade::Create(CManager::MODE_TITLE);
			//pSound->PlaySound(CSound::SOUND_LABEL_DECIDE);

			// 点滅する速度を上げる
			m_nFlash = RESULT_AFTER_FLASH;
		}
	}

	if (m_nCntResult >= RESULT_TIME)
	{// 画面遷移
		CFade::Create(CManager::MODE_TITLE);
	}

	CDebugProc::Print("リザルト");

}

//=============================================================================
// 描画処理
//=============================================================================
void CResult::Draw(void)
{
}
