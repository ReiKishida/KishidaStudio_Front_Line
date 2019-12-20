//=============================================================================
//
// 数字処理 [number.cpp]
// Author : Takuto Ishida
//
//=============================================================================
#include "number.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "UI_Number.h"

//==================================
// マクロ定義
//==================================

//==================================
// 静的メンバ変数宣言
//==================================

//==================================
// 生成処理
//==================================
CNumber* CNumber::Create(D3DXVECTOR3 pos, float fWidth, float fHeight, CUI_NUMBER::UI_NUMTYPE UINumType, int nUV, int nUV_X, int nUV_Y)
{
	CNumber *pNumber = NULL;

	pNumber = new CNumber;	// メモリを確保

	if (pNumber)
	{// メモリ確保成功
		pNumber->m_TeamType = UINumType;
		pNumber->m_nUV = nUV;
		pNumber->m_nUV_X = nUV_X;
		pNumber->m_nUV_Y = nUV_Y;
		pNumber->Init(pos, fWidth, fHeight);	// 初期化処理
	}

	return pNumber;
}

//=========================================
// コンストラクタ
//=========================================
CNumber::CNumber(int nPriority, CScene::OBJTYPE objType) : CScene2D(nPriority, objType)
{
	m_nNumber = 0;
	m_UINum = NULL;
	m_TeamType = m_UINum->UI_NUMTYPE_NONE;		// タイプ設定しない
	m_nUV = 0;
	m_nUV_X = 0;
	m_nUV_Y = 0;
}

//=========================================
// デストラクタ
//=========================================
CNumber::~CNumber()
{
}

//=========================================
// 初期化処理
//=========================================
HRESULT CNumber::Init(D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	// 初期化処理
	CScene2D::Init();

	if (m_TeamType == CUI_NUMBER::UI_NUMTYPE_BLUE)
	{
		CScene2D::BindTexture(CTexture::GetTexture(CTexture::TEXTURE_NUMBER));
		CScene2D::SetTex(m_nUV, m_nUV_X, m_nUV_Y);
	}
	else if (m_TeamType == CUI_NUMBER::UI_NUMTYPE_RED)
	{
		CScene2D::BindTexture(CTexture::GetTexture(CTexture::TEXTURE_NUMBER));
		CScene2D::SetTex(m_nUV, m_nUV_X, m_nUV_Y);
	}
	else
	{
		CScene2D::BindTexture(CTexture::GetTexture(CTexture::TEXTURE_NUMBER));
		CScene2D::SetTex(m_nUV, m_nUV_X, m_nUV_Y);
	}

	// 位置の設定
	CScene2D::SetPos(pos);

	// サイズの設定
	CScene2D::SetSize(fWidth, fHeight);

	return S_OK;
}

//=========================================
// 終了処理
//=========================================
void CNumber::Uninit(void)
{
	// 終了処理
	CScene2D::Uninit();
}

//=========================================
// 更新処理
//=========================================
void CNumber::Update(void)
{
}

//=========================================
// 描画処理
//=========================================
void CNumber::Draw(void)
{
	// 描画処理
	CScene2D::Draw();
}

//=========================================
// 数字の設定
//=========================================
const void CNumber::SetNumber(int nNumber)
{
	// 値の保存
	m_nNumber = nNumber;

	// 頂点情報の取得
	LPDIRECT3DVERTEXBUFFER9 vtxBuff = CScene2D::GetVtxBuff();

	// 頂点情報を設定
	VERTEX_2D *pVtx;	// 頂点情報のポインタ

						// 頂点バッファをロックし、頂点データへのポインタを取得
	vtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f + m_nNumber * 0.1f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.1f + m_nNumber * 0.1f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f + m_nNumber * 0.1f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.1f + m_nNumber * 0.1f, 1.0f);

	// 頂点バッファをアンロック
	vtxBuff->Unlock();
}

//=========================================
// 数字とUVの設定
//=========================================
const void CNumber::SetTexNumber(int nNumber)
{
	// 値の保存
	m_nNumber = nNumber;

	// 頂点情報の取得
	LPDIRECT3DVERTEXBUFFER9 vtxBuff = CScene2D::GetVtxBuff();

	// 頂点情報を設定
	VERTEX_2D *pVtx;	// 頂点情報のポインタ

	// 頂点バッファをロックし、頂点データへのポインタを取得
	vtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f + m_nNumber * 0.1f, (m_nUV / m_nUV_X) * (1.0f / m_nUV_Y));
	pVtx[1].tex = D3DXVECTOR2(0.1f + m_nNumber * 0.1f, (m_nUV / m_nUV_X) * (1.0f / m_nUV_Y));
	pVtx[2].tex = D3DXVECTOR2(0.0f + m_nNumber * 0.1f, (m_nUV / m_nUV_X) * (1.0f / m_nUV_Y) + (1.0f / m_nUV_Y));
	pVtx[3].tex = D3DXVECTOR2(0.1f + m_nNumber * 0.1f, (m_nUV / m_nUV_X) * (1.0f / m_nUV_Y) + (1.0f / m_nUV_Y));

	// 頂点バッファをアンロック
	vtxBuff->Unlock();
}

//=========================================
// サイズの取得
//=========================================
void CNumber::GetSize(float *pWidth, float *pHeight)
{
	if (pWidth)
	{// NULLでないとき値を入れる
		*pWidth = CScene2D::GetWidth();
	}

	if (pHeight)
	{// NULLでないとき値を入れる
		*pHeight = CScene2D::GetHeight();
	}
}
