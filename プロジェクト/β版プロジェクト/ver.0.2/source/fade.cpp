//=============================================================================
//
// フェード処理 [fade.cpp]
// Author : Takuto Ishida
//
//=============================================================================
#include "fade.h"
#include "input.h"
#include "renderer.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
CFade::FADE		CFade::m_fade = CFade::FADE_NONE;								// フェード状態
CManager::MODE	CFade::m_modeNext = CManager::MODE_TITLE;				// 次のモード
D3DXCOLOR		CFade::m_colFade = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);	// フェード色

//=============================================================================
// フェードの生成
//=============================================================================
CFade *CFade::Create(CManager::MODE modeNext)
{
	CFade *pFade = NULL;
	if (pFade == NULL && m_fade == FADE_NONE)
	{// フェードの状態が何もないときのみ生成される
		pFade = new CFade;

		if (pFade != NULL)
		{
			pFade->Init();	// 初期化処理

			// 値の初期化
			m_fade = FADE_OUT;			// フェードアウト状態に
			m_modeNext = modeNext;		// 次のシーンを記憶
		}
	}

	return pFade;
}

//=============================================================================
// コンストラクタ
//=============================================================================
CFade::CFade(int nPriority, CScene::OBJTYPE objType) : CScene2D(nPriority, objType)
{
	m_fade = FADE_NONE;
	m_colFade = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
}

//=============================================================================
// デストラクタ
//=============================================================================
CFade::~CFade()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CFade::Init(void)
{
	CScene2D::Init();

	// 位置の設定
	CScene2D::SetPos(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f));

	// 大きさの設定
	CScene2D::SetSize(SCREEN_WIDTH, SCREEN_HEIGHT);

	// 色の設定
	m_colFade = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	CScene2D::SetColor(m_colFade);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CFade::Uninit(void)
{
	CScene2D::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CFade::Update(void)
{
	bool bDelete = false;

	if (m_fade != FADE_NONE)
	{// フェードの状態が何もしていないときはスルー
		if (m_fade == FADE_IN)		//フェードイン状態
		{
			m_colFade.a -= 0.02f;	//画面を透明にしていく

			if (m_colFade.a <= 0.0f)
			{
				m_colFade.a = 0.0f;
				m_fade = FADE_NONE;	//何もしていない状態
				bDelete = true;
			}
		}
		else if (m_fade == FADE_OUT)
		{
			m_colFade.a += 0.02f;	//画面を不透明にしていく

			if (m_colFade.a >= 1.0f)
			{
				m_colFade.a = 1.0f;
				m_fade = FADE_IN;	//フェードイン状態に

				//モードの設定
				CManager::SetMode(m_modeNext);
			}
		}

		// 色の設定
		CScene2D::SetColor(m_colFade);

		if (bDelete == true)
		{// 終わったフェードを消す
			Uninit();
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CFade::Draw(void)
{
	// デバイスの取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	// Zテストを無効にする
	pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);

	CScene2D::Draw();

	// Zテストを有効にする
	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
}
