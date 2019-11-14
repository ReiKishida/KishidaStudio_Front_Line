//=============================================================================
//
// ポリゴン処理 [polygon.cpp]
// Author : Ishida Takuto
//
//=============================================================================
#include "polygon.h"
#include "manager.h"
#include "renderer.h"

//==================================
// 静的メンバ変数宣言
//==================================

//==================================
// 生成処理
//==================================
CPolygon* CPolygon::Create(int nType, D3DXVECTOR3 pos, float fWidth, float fHeight, int nTexType)
{
	CPolygon *pPolygon;

	pPolygon = new CPolygon;	// メモリを確保

	if (NULL != pPolygon)
	{// メモリ確保成功
		pPolygon->Init(pos, fWidth, fHeight, nType);	// 初期化処理
		pPolygon->m_nTexType = nTexType;				// テクスチャの種類
	}

	return pPolygon;
}

//=========================================
// テクスチャの読み込み
//=========================================
HRESULT CPolygon::Load(void)
{
	return S_OK;
}

//=========================================
// 読み込んだテクスチャの解放
//=========================================
void CPolygon::Unload(void)
{
}

//=========================================
// コンストラクタ
//=========================================
CPolygon::CPolygon()
{
	m_fWidth = 0.0f;
	m_fHeight = 0.0f;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	m_nTexType = 0;
	m_nCntColor = 0;
}

//=========================================
// デストラクタ
//=========================================
CPolygon::~CPolygon()
{
}

//=========================================
// 初期化処理
//=========================================
HRESULT CPolygon::Init(D3DXVECTOR3 pos, float fWidth, float fHeight, int nType)
{
	// デバイスの取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	// ポリゴンの設定
	m_pos = pos;				// 位置
	m_fWidth = fWidth;			// 幅
	m_fHeight = fHeight;		// 高さ
	m_nType = nType;			// 作り方の種類
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// 色の初期値

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// 頂点情報を設定
	VERTEX_2D *pVtx;	// 頂点情報のポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標
	if (nType == 0)
	{// 真ん中が中心
		pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_fWidth, m_pos.y - m_fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_fWidth, m_pos.y - m_fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(m_pos.x - m_fWidth, m_pos.y + m_fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_fWidth, m_pos.y + m_fHeight, 0.0f);
	}
	else if (nType == 1)
	{// 左上が中心
		pVtx[0].pos = D3DXVECTOR3(m_pos.x, m_pos.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_fWidth, m_pos.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(m_pos.x, m_pos.y + m_fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_fWidth, m_pos.y + m_fHeight, 0.0f);
	}

	// 1.0fで固定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 頂点カラー
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=========================================
// 終了処理
//=========================================
void CPolygon::Uninit(void)
{
	if (NULL != m_pVtxBuff)
	{// 頂点バッファのポインタを開放
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
}

//=========================================
// 更新処理
//=========================================
void CPolygon::Update(void)
{
}

//=========================================
// 描画処理
//=========================================
void CPolygon::Draw(void)
{
	// デバイスの取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	// αテストの設定
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	if (m_nTexType == -1)
	{
		pDevice->SetTexture(0, NULL);
	}
	else
	{
		pDevice->SetTexture(0, NULL);
	}

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	// αテストの設定を戻す
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
}

//=========================================
// 位置の設定
//=========================================
const void CPolygon::SetPos(D3DXVECTOR3 pos)
{
	m_pos = pos;

	// 頂点情報を設定
	VERTEX_2D *pVtx;	// 頂点情報のポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標
	if (m_nType == 0)
	{// 真ん中が中心
		pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_fWidth, m_pos.y - m_fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_fWidth, m_pos.y - m_fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(m_pos.x - m_fWidth, m_pos.y + m_fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_fWidth, m_pos.y + m_fHeight, 0.0f);
	}
	else if (m_nType == 1)
	{// 左上が中心
		pVtx[0].pos = D3DXVECTOR3(m_pos.x, m_pos.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_fWidth, m_pos.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(m_pos.x, m_pos.y + m_fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_fWidth, m_pos.y + m_fHeight, 0.0f);
	}

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//=========================================
// Ｘ軸サイズの設定
//=========================================
const void CPolygon::SetWidth(float fWidth)
{
	m_fWidth = fWidth;

	// 頂点情報を設定
	VERTEX_2D *pVtx;	// 頂点情報のポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標
	if (m_nType == 0)
	{// 真ん中が中心
		pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_fWidth, m_pos.y - m_fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_fWidth, m_pos.y - m_fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(m_pos.x - m_fWidth, m_pos.y + m_fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_fWidth, m_pos.y + m_fHeight, 0.0f);
	}
	else if (m_nType == 1)
	{// 左上が中心
		pVtx[0].pos = D3DXVECTOR3(m_pos.x, m_pos.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_fWidth, m_pos.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(m_pos.x, m_pos.y + m_fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_fWidth, m_pos.y + m_fHeight, 0.0f);
	}

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//=========================================
// Ｙ軸サイズの設定
//=========================================
const void CPolygon::SetHeight(float fHeight)
{
	m_fHeight = fHeight;

	// 頂点情報を設定
	VERTEX_2D *pVtx;	// 頂点情報のポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標
	if (m_nType == 0)
	{// 真ん中が中心
		pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_fWidth, m_pos.y - m_fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_fWidth, m_pos.y - m_fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(m_pos.x - m_fWidth, m_pos.y + m_fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_fWidth, m_pos.y + m_fHeight, 0.0f);
	}
	else if (m_nType == 1)
	{// 左上が中心
		pVtx[0].pos = D3DXVECTOR3(m_pos.x, m_pos.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_fWidth, m_pos.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(m_pos.x, m_pos.y + m_fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_fWidth, m_pos.y + m_fHeight, 0.0f);
	}

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//=========================================
// 色の設定
//=========================================
const void CPolygon::SetColor(D3DXCOLOR col)
{
	m_col = col;

	// 頂点情報を設定
	VERTEX_2D *pVtx;	// 頂点情報のポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点カラー
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//=========================================
// 色の加算
//=========================================
const void CPolygon::AddColor(D3DXCOLOR col)
{
	m_col += col;

	// 頂点情報を設定
	VERTEX_2D *pVtx;	// 頂点情報のポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点カラー
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//=========================================
// 点滅処理
//=========================================
void CPolygon::Flashing(int nFlame)
{
	// 指定したフレームで
	if (m_nCntColor < nFlame)
	{// 透明にする
		m_col.a -= 1.0f / nFlame;
	}
	else
	{// 不透明にする
		m_col.a += 1.0f / nFlame;
	}

	m_nCntColor = (1 + m_nCntColor) % (nFlame * 2);		// カウントを進める

	if (m_col.a <= 0)
	{// アルファ値が０以下のとき
		m_col.a = 0;
		m_nCntColor = nFlame;
	}
	else if (m_col.a >= 1)
	{// アルファ値が１以上のとき
		m_col.a = 1;
		m_nCntColor = 0;
	}

	// 頂点情報を設定
	VERTEX_2D *pVtx;	// 頂点情報のポインタ

						//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点カラー
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}