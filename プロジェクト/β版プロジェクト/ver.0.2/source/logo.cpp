//=============================================================================
//
// ロゴの処理 [logo.cpp]
// Author : Takuto Ishida
//
//=============================================================================
#include "logo.h"
#include "texture.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define PRESS_TEXTURE		"data/TEXTURE/press_any.png"
#define TUTORIAL_TEXTURE	"data/TEXTURE/tutorial.png"

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************

//=============================================================================
// 生成処理
//=============================================================================
CLogo* CLogo::Create(D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	CLogo *pLogo = NULL;	// ロゴクラスのポインタ変数

	pLogo = new CLogo;		// メモリを確保

	if (NULL != pLogo)
	{// メモリを確保できた
		pLogo->Init();						// 初期化処理
		pLogo->SetPos(pos);					// 位置の設定
		pLogo->SetSize(fWidth, fHeight);	// サイズの設定
	}

	return pLogo;
}

//=============================================================================
// コンストラクタ
//=============================================================================
CLogo::CLogo(int nPriority, CScene::OBJTYPE objType) : CScene2D(nPriority, objType)
{
}

//=============================================================================
// デストラクタ
//=============================================================================
CLogo::~CLogo()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CLogo::Init(void)
{
	// 初期化処理
	CScene2D::Init();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CLogo::Uninit(void)
{
	// オブジェクトを破棄
	CScene2D::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CLogo::Update(void)
{
}

//=============================================================================
// 描画処理
//=============================================================================
void CLogo::Draw(void)
{
	CScene2D::Draw();	// 描画処理
}

/*==================================================*/
/*			  ボタン入力待ち表示クラス				*/
/*==================================================*/
//=============================================================================
// 生成処理
//=============================================================================
CLogoPressButton* CLogoPressButton::Create(D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	CLogoPressButton *pLogoPressButtion = NULL;	// ロゴクラスのポインタ変数

	pLogoPressButtion = new CLogoPressButton;		// メモリを確保

	if (NULL != pLogoPressButtion)
	{// メモリを確保できた
		pLogoPressButtion->Init();						// 初期化処理
		pLogoPressButtion->SetPos(pos);					// 位置の設定
		pLogoPressButtion->SetSize(fWidth, fHeight);	// サイズの設定
	}

	return pLogoPressButtion;
}

//=============================================================================
// コンストラクタ
//=============================================================================
CLogoPressButton::CLogoPressButton(int nPriority, CScene::OBJTYPE objType) : CLogo(nPriority, objType)
{
	m_pTexture = NULL;
	m_nCntFlash = 0;
}

//=============================================================================
// デストラクタ
//=============================================================================
CLogoPressButton::~CLogoPressButton()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CLogoPressButton::Init(void)
{
	// 初期化処理
	CLogo::Init();

	// テクスチャの設定
	CLogo::BindTexture(CTexture::GetTexture(CTexture::TEXTURE_PRESS_BUTTON));

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CLogoPressButton::Uninit(void)
{
	if (NULL != m_pTexture)
	{// テクスチャの開放
		m_pTexture->Release();
		m_pTexture = NULL;
	}

	// オブジェクトを破棄
	CLogo::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CLogoPressButton::Update(void)
{
}

//=============================================================================
// 描画処理
//=============================================================================
void CLogoPressButton::Draw(void)
{
	// 描画処理
	CLogo::Draw();
}

/*==================================================*/
/*				  操作説明表示クラス				*/
/*==================================================*/
//=============================================================================
// 生成処理
//=============================================================================
CLogoTutorial* CLogoTutorial::Create(D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	CLogoTutorial *pLogoPressButtion = NULL;	// ロゴクラスのポインタ変数

	pLogoPressButtion = new CLogoTutorial;		// メモリを確保

	if (NULL != pLogoPressButtion)
	{// メモリを確保できた
		pLogoPressButtion->Init();						// 初期化処理
		pLogoPressButtion->SetPos(pos + D3DXVECTOR3(-fWidth * 0.5f, -fHeight * 0.5f, 0.0f));	// 位置の設定
		pLogoPressButtion->SetSize(fWidth, fHeight);	// サイズの設定
	}

	return pLogoPressButtion;
}

//=============================================================================
// コンストラクタ
//=============================================================================
CLogoTutorial::CLogoTutorial(int nPriority, CScene::OBJTYPE objType) : CLogo(nPriority, objType)
{
	m_pTexture = NULL;
}

//=============================================================================
// デストラクタ
//=============================================================================
CLogoTutorial::~CLogoTutorial()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CLogoTutorial::Init(void)
{
	// 初期化処理
	CLogo::Init();

	CLogo::BindTexture(NULL);		// テクスチャの設定

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CLogoTutorial::Uninit(void)
{
	if (NULL != m_pTexture)
	{// テクスチャの開放
		m_pTexture->Release();
		m_pTexture = NULL;
	}

	// オブジェクトを破棄
	CLogo::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CLogoTutorial::Update(void)
{
}

//=============================================================================
// 描画処理
//=============================================================================
void CLogoTutorial::Draw(void)
{
	// 描画処理
	CLogo::Draw();
}
