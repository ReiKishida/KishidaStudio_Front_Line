//=============================================================================
//
// 3Dビルボードオブジェクト処理 [scene3DBill.h]
// Author : Ishida Takuto
//
//=============================================================================
#include "scene3DBill.h"
#include "renderer.h"
#include "manager.h"

//==================================
// マクロ定義
//==================================

//==================================
// 静的メンバ変数宣言
//==================================

//=========================================
// 生成処理
//=========================================
CScene3DBill *CScene3DBill::Create(void)
{
	CScene3DBill *pScene3DBill = new CScene3DBill;

	if (NULL != pScene3DBill)
	{// メモリ確保成功
		pScene3DBill->Init();
	}

	return pScene3DBill;
}

//=========================================
// テクスチャを割り当てる
//=========================================
void CScene3DBill::BindTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
}

//=========================================
// コンストラクタ
//=========================================
CScene3DBill::CScene3DBill(int nPriority, CScene::OBJTYPE objType) : CScene(nPriority, objType)
{
	m_pTexture = NULL;		// テクスチャへのポインタ
	m_pVtxBuff = NULL;		// 頂点バッファへのポインタ
	m_pMtxParent = NULL;	// 親のワールドマトリックス
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 位置
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 前回の位置
	m_fWidth = 0.0f;							// 幅
	m_fHeight = 0.0f;							// 高さ
	m_cmpFunc = D3DCMP_LESSEQUAL;				// Zバッファの設定
	m_bZBuffer = false;							// Zバッファの設定をするかどうか
	m_bLighting = true;							// ライティングの有無
	m_bDisp = true;
}

//=========================================
// デストラクタ
//=========================================
CScene3DBill::~CScene3DBill()
{
}

//=========================================
// ポリゴンの初期化処理
//=========================================
HRESULT CScene3DBill::Init(void)
{
	// デバイスの取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// 頂点情報を設定
	VERTEX_3D *pVtx;	// 頂点情報のポインタ

	// 頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos = D3DXVECTOR3(-m_fWidth, m_fHeight, 0.0f) * 0.5f;
	pVtx[1].pos = D3DXVECTOR3(m_fWidth, m_fHeight, 0.0f) * 0.5f;
	pVtx[2].pos = D3DXVECTOR3(-m_fWidth, -m_fHeight, 0.0f) * 0.5f;
	pVtx[3].pos = D3DXVECTOR3(m_fWidth, -m_fHeight, 0.0f) * 0.5f;

	// 法線
	pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

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
// ポリゴンの終了処理
//=========================================
void CScene3DBill::Uninit(void)
{
	if (NULL != m_pVtxBuff)
	{// 頂点バッファの破棄
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	// 自分を破棄
	CScene::Release();
}

//=========================================
// ポリゴンの更新処理
//=========================================
void CScene3DBill::Update(void)
{
}

//=========================================
// ポリゴンの描画処理
//=========================================
void CScene3DBill::Draw(void)
{
	if (!m_bDisp) { return; }

	// デバイスの取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	if (m_bZBuffer)
	{// Zバッファの設定
		pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
		pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	}

	// αテストの設定
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	if (!m_bLighting)
	{// ライティングを無効にする
		pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	}

	D3DXMATRIX mtxTrans, mtxView;	// ワールドマトリックスの保存用

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 回転を反映
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);
	m_mtxWorld._11 = mtxView._11;
	m_mtxWorld._12 = mtxView._21;
	m_mtxWorld._13 = mtxView._31;
	m_mtxWorld._21 = mtxView._12;
	m_mtxWorld._22 = mtxView._22;
	m_mtxWorld._23 = mtxView._32;
	m_mtxWorld._31 = mtxView._13;
	m_mtxWorld._32 = mtxView._23;
	m_mtxWorld._33 = mtxView._33;

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	if (m_pMtxParent)
	{// 親のマトリックスと掛け合わせる
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, m_pMtxParent);
	}

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(0, m_pTexture);

	// 描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	if (!m_bLighting)
	{// ライティングを有効にする
		pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	}

	if (m_bZBuffer)
	{// Zバッファの設定を戻す
		pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
		pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	}

	// αテストの設定を戻す
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
}

//=========================================
// 当たり判定
//=========================================
bool CScene3DBill::Collision(D3DXVECTOR3 pos, float fRadius)
{
	// 相手のXYZの範囲
	float fLength = powf(fRadius, 2);

	float fLengthX = pos.x - m_pos.x;	// Xの差
	float fLengthY = pos.y - m_pos.y;	// Yの差
	float fLengthZ = pos.z - m_pos.z;	// Zの差

	float fLengthTotal = powf(fLengthX, 2) + powf(fLengthY, 2) + powf(fLengthZ, 2);		// XYZの差の二乗

	if (fLength >= fLengthTotal)
	{// 範囲チェック
		return true;	// 判定を有効にする
	}

	return false;
}

//=========================================
// 色の設定
//=========================================
void CScene3DBill::SetColor(D3DXCOLOR col)
{
	// 色の設定
	m_col = col;

	// 頂点情報を設定
	VERTEX_3D *pVtx;		// 頂点情報のポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//テクスチャ座標
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//=========================================
// 色の加算
//=========================================
void CScene3DBill::AddColor(D3DXCOLOR col)
{
	// 色の加算
	m_col += col;

	// 頂点情報を設定
	VERTEX_3D *pVtx;		// 頂点情報のポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//テクスチャ座標
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//=========================================
// サイズの設定
//=========================================
void CScene3DBill::SetSize(D3DXVECTOR3 size)
{
	// サイズの設定
	m_fWidth = size.x;
	m_fHeight = size.y;

	// 頂点情報を設定
	VERTEX_3D *pVtx;	// 頂点情報のポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos = D3DXVECTOR3(-m_fWidth, m_fHeight, size.z) * 0.5f;
	pVtx[1].pos = D3DXVECTOR3(m_fWidth, m_fHeight, size.z) * 0.5f;
	pVtx[2].pos = D3DXVECTOR3(-m_fWidth, -m_fHeight, size.z) * 0.5f;
	pVtx[3].pos = D3DXVECTOR3(m_fWidth, -m_fHeight, size.z) * 0.5f;

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//=========================================
// 幅の設定
//=========================================
void CScene3DBill::SetWidth(float fWidth)
{
	m_fWidth = fWidth;

	// 頂点情報を設定
	VERTEX_3D *pVtx;	// 頂点情報のポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos.x = -m_fWidth * 0.5f;
	pVtx[1].pos.x = m_fWidth * 0.5f;
	pVtx[2].pos.x = -m_fWidth * 0.5f;
	pVtx[3].pos.x = m_fWidth * 0.5f;

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//=========================================
// 高さの設定
//=========================================
void CScene3DBill::SetHeight(float fHeight)
{
	m_fHeight = fHeight;

	// 頂点情報を設定
	VERTEX_3D *pVtx;	// 頂点情報のポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos.y = m_fHeight * 0.5f;
	pVtx[1].pos.y = m_fHeight * 0.5f;
	pVtx[2].pos.y = -m_fHeight * 0.5f;
	pVtx[3].pos.y = -m_fHeight * 0.5f;

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//=========================================
// Zバッファの設定
//=========================================
void CScene3DBill::SetZBuffer(D3DCMPFUNC cmpFunc)
{
	m_cmpFunc = cmpFunc;

	if (!m_bZBuffer)
	{// Zバッファの設定をする
		m_bZBuffer = true;
	}
}