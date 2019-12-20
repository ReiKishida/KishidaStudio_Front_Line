//=============================================================================
//
// 軌跡処理 [meshOrbit.cpp]
// Author : Takuto Ishida
//
//=============================================================================
#include "meshOrbit.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "debugProc.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_NAME00	"data/TEXTURE/gradation004.jpg"
#define ORBIT_POLYGON	(MAX_ORBIT_VERTEX - 2)	// ポリゴン数

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9 CMeshOrbit::m_pTexture[FIELD_TEXTURE] = {};

//==================================
// 生成処理
//==================================
CMeshOrbit* CMeshOrbit::Create(D3DXVECTOR3 offset, int nLife, D3DXMATRIX *pMtx)
{
	CMeshOrbit *pOrbit;

	pOrbit = new CMeshOrbit;		// メモリを確保

	if (pOrbit != NULL)
	{// メモリ確保成功
		pOrbit->m_aOffset[1] = offset;	// オフセット
		pOrbit->m_nLife = nLife;		// 寿命
		pOrbit->m_pMtxParent = pMtx;	// 親のマトリックス
		pOrbit->Init();					// 初期化処理
	}

	return pOrbit;
}

//=========================================
// テクスチャの読み込み
//=========================================
HRESULT CMeshOrbit::Load(void)
{
	// デバイスの取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME00, &m_pTexture[0]);

	return S_OK;
}

//=========================================
// 読み込んだテクスチャの解放
//=========================================
void CMeshOrbit::Unload(void)
{
	// テクスチャの破棄
	for (int nCntTexture = 0; nCntTexture < FIELD_TEXTURE; nCntTexture++)
	{
		if (NULL != m_pTexture[nCntTexture])
		{
			m_pTexture[nCntTexture]->Release();
			m_pTexture[nCntTexture] = NULL;
		}
	}
}

//=========================================
// コンストラクタ
//=========================================
CMeshOrbit::CMeshOrbit(int nPriority, CScene::OBJTYPE objType) : CScene(nPriority, objType)
{
	for (int nCntOrbit = 0; nCntOrbit < MAX_ORBIT_VERTEX; nCntOrbit++)
	{// 頂点座標の計算用
		m_aPosVertex[nCntOrbit] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	for (int nCntOffset = 0; nCntOffset < 2; nCntOffset++)
	{// オフセット
		m_aOffset[nCntOffset] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	m_nLife = 0;
	m_pMtxParent = NULL;
	m_pVtxBuff = NULL;
}

//=========================================
// デストラクタ
//=========================================
CMeshOrbit::~CMeshOrbit()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CMeshOrbit::Init(void)
{
	// デバイスの取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME00, &m_pTexture[0]);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * MAX_ORBIT_VERTEX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// 頂点情報を設定
	VERTEX_3D *pVtx;	// 頂点情報のポインタ

	// 頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	if (NULL != m_pMtxParent)
	{// 親がいるとき
		for (int nCntOrbit = 0; nCntOrbit < MAX_ORBIT_VERTEX / 2; nCntOrbit++)
		{
			// 頂点座標
			pVtx[0 + (nCntOrbit * 2)].pos = D3DXVECTOR3(m_pMtxParent->_41, m_pMtxParent->_42, m_pMtxParent->_43);
			pVtx[1 + (nCntOrbit * 2)].pos = D3DXVECTOR3(m_pMtxParent->_41, m_pMtxParent->_42, m_pMtxParent->_43) + m_aOffset[1];

			// 法線の初期化
			pVtx[0 + (nCntOrbit * 2)].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[1 + (nCntOrbit * 2)].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			// 頂点カラー
			pVtx[0 + (nCntOrbit * 2)].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f);
			pVtx[1 + (nCntOrbit * 2)].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f);

			//テクスチャ座標
			pVtx[0 + (nCntOrbit * 2)].tex = D3DXVECTOR2((float)nCntOrbit * (1 / (MAX_ORBIT_VERTEX / 2)), 1.0f);
			pVtx[1 + (nCntOrbit * 2)].tex = D3DXVECTOR2((float)nCntOrbit * (1 / (MAX_ORBIT_VERTEX / 2)), 0.0f);

			// 頂点座標の計算用
			m_aPosVertex[0 + (nCntOrbit * 2)] = D3DXVECTOR3(m_pMtxParent->_41, m_pMtxParent->_42, m_pMtxParent->_43);
			m_aPosVertex[1 + (nCntOrbit * 2)] = D3DXVECTOR3(m_pMtxParent->_41, m_pMtxParent->_42, m_pMtxParent->_43) + m_aOffset[1];
		}
	}

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CMeshOrbit::Uninit(void)
{
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CMeshOrbit::Update(void)
{
	if (m_nLife >= 0)
	{// 寿命が無くなった
		for (int nCntOrbit = MAX_ORBIT_VERTEX - 1; nCntOrbit >= 2; nCntOrbit--)
		{// 頂点座標の入れ替え
			m_aPosVertex[nCntOrbit] = m_aPosVertex[nCntOrbit - 2];
		}
	}

	// 頂点情報を設定
	VERTEX_3D *pVtx;	// 頂点情報のポインタ
	D3DXCOLOR col;

	// 頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntOrbit = 0; nCntOrbit < MAX_ORBIT_VERTEX; nCntOrbit++)
	{
		// 頂点座標
		pVtx[nCntOrbit].pos = m_aPosVertex[nCntOrbit];
		col = pVtx[nCntOrbit].col;
		col -= D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.01f);
		if (col.a <= 0.0f) { col.a = 0.0f; };
		pVtx[nCntOrbit].col = col;
	}

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	m_nLife--;	// 寿命を減らす

	if (col.a <= 0)
	{// 寿命が無くなった
		Uninit();
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CMeshOrbit::Draw(void)
{
	// デバイスの取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	// カリングをしない
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	// αテストの設定
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// αブレンディングを加算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	if (m_nLife >= 0)
	{// 寿命がある
		VERTEX_3D *pVtx;	// 頂点情報のポインタ

		// 頂点バッファをロックし、頂点データへのポインタを取得
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// オフセットと親のマトリックスを掛け合わせる
		D3DXVec3TransformCoord(&m_aPosVertex[0], &m_aOffset[0], m_pMtxParent);
		pVtx[0].pos = m_aPosVertex[0];

		// オフセットと親のマトリックスを掛け合わせる
		D3DXVec3TransformCoord(&m_aPosVertex[1], &m_aOffset[1], m_pMtxParent);
		pVtx[1].pos = m_aPosVertex[1];

		// 頂点バッファをアンロック
		m_pVtxBuff->Unlock();
	}

	// ライト無効
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(0, m_pTexture[0]);

	// 描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, ORBIT_POLYGON);

	// ライト有効
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// αテストの設定を戻す
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// 裏面をカリング
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}
