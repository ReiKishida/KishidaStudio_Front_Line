//=============================================================================
//
// 当たり判定のライン処理 [line.cpp]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#include "line.h"
#include "manager.h"
#include "renderer.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//=============================================================================
// 生成処理
//=============================================================================
CLine *CLine::Create(D3DXVECTOR3 start, D3DXVECTOR3 end, D3DXCOLOR col)
{
	CLine *pLine = NULL;

	pLine = new CLine;	// メモリを確保

	if (pLine != NULL)
	{// メモリ確保成功
		pLine->m_start = start;	// 始点
		pLine->m_end = end;		// 終点
		pLine->m_col = col;		// 色
		pLine->Init();			// 初期化処理
	}

	return pLine;
}

//=============================================================================
// コンストラクタ
//=============================================================================
CLine::CLine()
{
	m_start = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_end = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_pVtxBuff = NULL;
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
}

//=============================================================================
// デストラクタ
//=============================================================================
CLine::~CLine()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CLine::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// デバイスの取得

	// 頂点情報の作成
	MakeVertex(pDevice);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CLine::Uninit(void)
{
	// 頂点バッファの開放
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	// 自分を破棄
	CScene::Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CLine::Update(void)
{
}

//=============================================================================
// 描画処理
//=============================================================================
void CLine::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// デバイスの取得
	D3DXMATRIX	mtxTrans;				// 計算用マトリックス

	// ライト無効
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, 0.0f, 0.0f, 0.0f);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(0, NULL);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_LINELIST, 0, 1);

	// ライト有効
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//=============================================================================
// 始点と終点の座標の設定
//=============================================================================
void CLine::SetPoint(D3DXVECTOR3 start, D3DXVECTOR3 end)
{
	// 終点の座標の設定
	m_end = end;
	m_start = start;

	VERTEX_3D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点の座標
	pVtx[0].pos = m_start;
	pVtx[1].pos = m_end;

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//=============================================================================
// 色の設定
//=============================================================================
void CLine::SetColor(D3DXCOLOR col)
{
	// 色の設定
	m_col = col;

	VERTEX_3D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点の座標
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
void CLine::MakeVertex(LPDIRECT3DDEVICE9 pDevice)
{
	// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 2
		,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	VERTEX_3D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点の座標
	pVtx[0].pos = m_start;
	pVtx[1].pos = m_end;

	// 法線の設定
	pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

	// 頂点カラー
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;

	//テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

/****************************************************************/
/*						2Dラインのクラス						*/
/****************************************************************/
//=============================================================================
// 生成処理
//=============================================================================
CLine2D *CLine2D::Create(D3DXVECTOR3 start, D3DXVECTOR3 end, D3DXCOLOR col)
{
	CLine2D *pLine = NULL;

	pLine = new CLine2D;	// メモリを確保

	if (pLine != NULL)
	{// メモリ確保成功
		pLine->m_start = start;	// 始点
		pLine->m_end = end;		// 終点
		pLine->m_col = col;		// 色
		pLine->Init();			// 初期化処理
	}

	return pLine;
}

//=============================================================================
// コンストラクタ
//=============================================================================
CLine2D::CLine2D(int nPriority, CScene::OBJTYPE objType) : CScene(nPriority, objType)
{
	m_start = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_end = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_pVtxBuff = NULL;
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//=============================================================================
// デストラクタ
//=============================================================================
CLine2D::~CLine2D()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CLine2D::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// デバイスの取得

	// 頂点情報の作成
	MakeVertex(pDevice);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CLine2D::Uninit(void)
{
	// 頂点バッファの開放
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	// 自分を破棄
	CScene::Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CLine2D::Update(void)
{
	m_rot.z += 1.0f;
}

//=============================================================================
// 描画処理
//=============================================================================
void CLine2D::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, NULL);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_LINELIST, 0, 1);
}

//=============================================================================
// 始点と終点の座標の設定
//=============================================================================
void CLine2D::SetPoint(D3DXVECTOR3 start, D3DXVECTOR3 end)
{
	// 終点の座標の設定
	m_end = end;
	m_start = start;

	VERTEX_2D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点の座標
	pVtx[0].pos = m_start;
	pVtx[1].pos = m_end;

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//=============================================================================
// 始点の設定
//=============================================================================
void CLine2D::SetStart(D3DXVECTOR3 start)
{
	m_start = start;

	VERTEX_2D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点の座標
	pVtx[0].pos = m_start;

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//=============================================================================
// 終点の設定
//=============================================================================
void CLine2D::SetEnd(D3DXVECTOR3 end)
{
	m_end = end;

	VERTEX_2D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点の座標
	pVtx[1].pos = m_end;

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//=============================================================================
// 色の設定
//=============================================================================
void CLine2D::SetColor(D3DXCOLOR col)
{
	// 色の設定
	m_col = col;

	VERTEX_2D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点の座標
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
void CLine2D::MakeVertex(LPDIRECT3DDEVICE9 pDevice)
{
	// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 2,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	VERTEX_2D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点の座標
	pVtx[0].pos = m_start;
	pVtx[1].pos = m_end;

	// 法線の設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;

	// 頂点カラー
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;

	//テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//=========================================
// 接続したとき
//=========================================
void CLine2D::Link(D3DXVECTOR3 end)
{
	// 頂点情報を設定
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos = m_start;
	pVtx[1].pos = m_end;

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}