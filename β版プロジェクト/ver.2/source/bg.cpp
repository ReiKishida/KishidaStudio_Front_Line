//=============================================================================
//
// 背景処理 [bg.cpp]
// Author : Takuto Ishida
//
//=============================================================================
#include "bg.h"
#include "scene2D.h"
#include "texture.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define BG_SCROLL_SPEED		(0.005f)		// スクロールの速度

//==================================
// 静的メンバ変数宣言
//==================================

//==================================
// 生成処理
//==================================
CBg* CBg::Create(void)
{
	CBg *pBg;
	pBg = new CBg;	// メモリを確保

	if (pBg != NULL)
	{// メモリ確保成功
		pBg->Init();	// 初期化処理
	}

	return pBg;
}

//=========================================
// コンストラクタ
//=========================================
CBg::CBg(int nPriority, CScene::OBJTYPE objType) : CScene(nPriority, objType)
{
	for (int nCntBG = 0; nCntBG < MAX_BG; nCntBG++)
	{
		m_pPolygon = NULL;
	}
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
}

//=========================================
// デストラクタ
//=========================================
CBg::~CBg()
{
}

//=========================================
// 初期化処理
//=========================================
HRESULT CBg::Init()
{
	m_pPolygon = new CScene2D*[MAX_BG];
	for (int nCntBG = 0; nCntBG < MAX_BG; nCntBG++)
	{// 背景ポリゴンの生成
		m_pPolygon[nCntBG] = CScene2D::Create();
		m_pPolygon[nCntBG]->SetPos(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f));
		m_pPolygon[nCntBG]->SetSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	}

	m_pPolygon[0]->BindTexture(CTexture::GetTexture(CTexture::TEXTURE_BG_00));
	m_pPolygon[1]->BindTexture(CTexture::GetTexture(CTexture::TEXTURE_BG_01));

	return S_OK;
}

//=========================================
// 終了処理
//=========================================
void CBg::Uninit(void)
{
	if (m_pPolygon != NULL)
	{
		for (int nCntBG = 0; nCntBG < MAX_BG; nCntBG++)
		{// ポリゴンの破棄
			if (m_pPolygon[nCntBG] != NULL)
			{
				m_pPolygon[nCntBG]->Uninit();
				m_pPolygon[nCntBG] = NULL;
			}
		}

		delete[] m_pPolygon;
		m_pPolygon = NULL;
	}

	// 自分を破棄
	CScene::Release();
}

//=========================================
// 更新処理
//=========================================
void CBg::Update(void)
{
	LPDIRECT3DVERTEXBUFFER9 vtxBuff;
	for (int nCntBG = 0; nCntBG < MAX_BG; nCntBG++)
	{
		// 頂点情報の取得
		vtxBuff = m_pPolygon[nCntBG]->GetVtxBuff();

		// 頂点情報を設定
		VERTEX_2D *pVtx;	// 頂点情報のポインタ

		// 頂点バッファをロックし、頂点データへのポインタを取得
		vtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		if (0 == nCntBG)
		{//テクスチャの座標設定
			pVtx[0].tex.x += BG_SCROLL_SPEED;
			pVtx[1].tex.x += BG_SCROLL_SPEED;
			pVtx[2].tex.x += BG_SCROLL_SPEED;
			pVtx[3].tex.x += BG_SCROLL_SPEED;
		}
		else
		{//テクスチャの座標設定
			pVtx[0].tex.x -= BG_SCROLL_SPEED;
			pVtx[1].tex.x -= BG_SCROLL_SPEED;
			pVtx[2].tex.x -= BG_SCROLL_SPEED;
			pVtx[3].tex.x -= BG_SCROLL_SPEED;
		}

		// 頂点バッファをアンロック
		vtxBuff->Unlock();

		// 頂点バッファを設定
		m_pPolygon[nCntBG]->SetVtxBuff(vtxBuff);
	}
}

//=========================================
// 描画処理
//=========================================
void CBg::Draw(void)
{
}

//=========================================
// 色の設定
//=========================================
void CBg::SetColor(D3DXCOLOR col)
{
	m_col = col;

	for (int nCntBG = 0; nCntBG < MAX_BG; nCntBG++)
	{// 色の設定
		m_pPolygon[nCntBG]->SetColor(m_col);
	}
}