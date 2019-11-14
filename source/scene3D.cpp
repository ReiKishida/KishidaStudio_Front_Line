//=============================================================================
//
// 3Dオブジェクト処理 [scene3D.cpp]
// Author : Ishida Takuto
//
//=============================================================================
#include "scene3D.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "debugProc.h"

//==================================
// マクロ定義
//==================================

//==================================
// 静的メンバ変数宣言
//==================================

//=========================================
// 生成処理
//=========================================
CScene3D *CScene3D::Create(void)
{
	CScene3D *pScene3D = new CScene3D;

	if (NULL != pScene3D)
	{// メモリ確保成功
		pScene3D->Init();
	}

	return pScene3D;
}

//=========================================
// テクスチャを割り当てる
//=========================================
void CScene3D::BindTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
}

//=========================================
// コンストラクタ
//=========================================
CScene3D::CScene3D(int nPriority, CScene::OBJTYPE objType) : CScene(nPriority, objType)
{
	m_pTexture = NULL;		// テクスチャへのポインタ
	m_pVtxBuff = NULL;		// 頂点バッファへのポインタ
	m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// ポリゴンの位置
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// ポリゴンの向き
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_pMtxParent = NULL;
	m_blendOp = D3DBLENDOP_FORCE_DWORD;
	m_bLighting = true;
	m_bZtest = true;
}

//=========================================
// デストラクタ
//=========================================
CScene3D::~CScene3D()
{
}

//=========================================
// 初期化処理
//=========================================
HRESULT CScene3D::Init(void)
{
	// デバイスの取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	// 色の設定
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

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

	// 頂点座標
	pVtx[0].pos = D3DXVECTOR3(-m_size.x, m_size.y, m_size.z);
	pVtx[1].pos = D3DXVECTOR3(m_size.x, m_size.y, m_size.z);
	pVtx[2].pos = D3DXVECTOR3(-m_size.x, -m_size.y, -m_size.z);
	pVtx[3].pos = D3DXVECTOR3(m_size.x, -m_size.y, -m_size.z);

	pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

	// 頂点カラー
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

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
void CScene3D::Uninit(void)
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
// 更新処理
//=========================================
void CScene3D::Update(void)
{
}

//=========================================
// 描画処理
//=========================================
void CScene3D::Draw(void)
{
	// デバイスの取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	if (!m_bLighting)
	{// ライティングOFF
		pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	}

	if (!m_bZtest)
	{// ZテストOFF
		pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	}

	// αテストの設定
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	if (D3DBLENDOP_FORCE_DWORD != m_blendOp)
	{// αブレンディングの設定
		pDevice->SetRenderState(D3DRS_BLENDOP, m_blendOp);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	}

	D3DXMATRIX mtxRot, mtxTrans;	// ワールドマトリックスの保存用

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

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

	if (D3DBLENDOP_FORCE_DWORD != m_blendOp)
	{// αブレンディングを元に戻す
		pDevice->SetRenderState(D3DRS_BLENDOP, m_blendOp);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	}

	// αテストの設定を戻す
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	if (!m_bZtest)
	{// ZテストON
		pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	}

	if (!m_bLighting)
	{// ライティングON
		pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	}
}

//=========================================
// 位置の設定
//=========================================
bool CScene3D::Collision(D3DXVECTOR3 pos, float fRadius)
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
// 位置の設定
//=========================================
void CScene3D::SetPos(D3DXVECTOR3 pos)
{
	m_pos = pos;	// 位置の設定

	//// 頂点情報を設定
	//VERTEX_3D *pVtx;	// 頂点情報のポインタ

	//// 頂点バッファをロックし、頂点データへのポインタを取得
	//m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//// 頂点座標
	//pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_size.x, m_pos.y + m_size.y, m_pos.z + m_size.z);
	//pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_size.x, m_pos.y + m_size.y, m_pos.z + m_size.z);
	//pVtx[2].pos = D3DXVECTOR3(m_pos.x - m_size.x, m_pos.y - m_size.y, m_pos.z - m_size.z);
	//pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_size.x, m_pos.y - m_size.y, m_pos.z - m_size.z);

	//// 頂点バッファをアンロック
	//m_pVtxBuff->Unlock();
}

//=========================================
// サイズの設定
//=========================================
void CScene3D::SetSize(D3DXVECTOR3 size)
{
	m_size = size;

	// 頂点情報を設定
	VERTEX_3D *pVtx;	// 頂点情報のポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos = D3DXVECTOR3(-m_size.x, m_size.y, m_size.z);
	pVtx[1].pos = D3DXVECTOR3(m_size.x, m_size.y, m_size.z);
	pVtx[2].pos = D3DXVECTOR3(-m_size.x, -m_size.y, -m_size.z);
	pVtx[3].pos = D3DXVECTOR3(m_size.x, -m_size.y, -m_size.z);

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//=========================================
// 幅の設定
//=========================================
void CScene3D::SetWidth(float fWidth)
{
	m_size.x = fWidth;

	// 頂点情報を設定
	VERTEX_3D *pVtx;	// 頂点情報のポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos.x = -m_size.x;
	pVtx[1].pos.x = m_size.x;
	pVtx[2].pos.x = -m_size.x;
	pVtx[3].pos.x = m_size.x;

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//=========================================
// 高さの設定
//=========================================
void CScene3D::SetHeight(float fHeight)
{
	m_size.y = fHeight;

	// 頂点情報を設定
	VERTEX_3D *pVtx;	// 頂点情報のポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos.y = m_size.y;
	pVtx[1].pos.y = m_size.y;
	pVtx[2].pos.y = -m_size.y;
	pVtx[3].pos.y = -m_size.y;

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//=========================================
// 奥行の設定
//=========================================
void CScene3D::SetDepth(float fDepth)
{
	m_size.z = fDepth;

	// 頂点情報を設定
	VERTEX_3D *pVtx;	// 頂点情報のポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos.z = m_size.z;
	pVtx[1].pos.z = m_size.z;
	pVtx[2].pos.z = -m_size.z;
	pVtx[3].pos.z = -m_size.z;

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//=========================================
// 色の設定
//=========================================
void CScene3D::SetColor(D3DXCOLOR col)
{
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
void CScene3D::AddColor(D3DXCOLOR col)
{
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
// テクスチャ座標の設定
//=========================================
void CScene3D::SetTexPos(D3DXVECTOR2 *texPos)
{
	// 頂点情報を設定
	VERTEX_3D *pVtx;		// 頂点情報のポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//テクスチャ座標
	pVtx[0].tex = texPos[0];
	pVtx[1].tex = texPos[1];
	pVtx[2].tex = texPos[2];
	pVtx[3].tex = texPos[3];

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//=========================================
// テクスチャ座標を動かす
//=========================================
void CScene3D::MoveTexPos(D3DXVECTOR2 speed)
{
	// 頂点情報を設定
	VERTEX_3D *pVtx;		// 頂点情報のポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//テクスチャ座標
	pVtx[0].tex += speed;
	pVtx[1].tex += speed;
	pVtx[2].tex += speed;
	pVtx[3].tex += speed;

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}
