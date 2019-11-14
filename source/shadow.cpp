//=============================================================================
//
// 影処理 [shadow.cpp]
// Author : Takuto Ishida
//
//=============================================================================
#include "shadow.h"
#include "manager.h"
#include "renderer.h"
#include "light.h"
#include "meshField.h"
#include "scene.h"
#include "polygon.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define SHADOW_MODEL_FILE	"data/MODEL/shadow000.x"

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================

//==================================
// 生成処理
//==================================
CShadow *CShadow::Create(D3DXVECTOR3 *pPos)
{
	CShadow *pShadow;

	pShadow = new CShadow;	// メモリを確保

	if (pShadow != NULL)
	{// メモリ確保成功
		pShadow->m_pos = pPos;
		pShadow->Init();
	}

	return pShadow;
}

//=========================================
// コンストラクタ
//=========================================
CShadow::CShadow(int nPriority, CScene::OBJTYPE objType) : CScene(nPriority, objType)
{
	m_pBigScreen = NULL;
	m_pBuffMat = NULL;
	m_pMesh = NULL;
}

//=========================================
// デストラクタ
//=========================================
CShadow::~CShadow()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CShadow::Init(void)
{
	// デバイスの取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	// Xファイルの読み込み
	D3DXLoadMeshFromX(SHADOW_MODEL_FILE,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_pBuffMat,
		NULL,
		&m_nNumMat,
		&m_pMesh);

	// 2Dポリゴンの生成
	m_pBigScreen = CPolygon::Create(1,D3DXVECTOR3(0.0f,0.0f,0.0f),SCREEN_WIDTH,SCREEN_HEIGHT);
	m_pBigScreen->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.8f));
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CShadow::Uninit(void)
{
	if (NULL != m_pBigScreen)
	{// 破棄
		m_pBigScreen->Uninit();
		delete m_pBigScreen;
		m_pBigScreen = NULL;
	}

	if (NULL != m_pBuffMat)
	{// 破棄
		m_pBuffMat->Release();
		m_pBuffMat = NULL;
	}

	if (NULL != m_pMesh)
	{// 破棄
		m_pMesh->Release();
		m_pMesh = NULL;
	}

	// オブジェクトの破棄
	CScene::Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CShadow::Update(void)
{
}

//=============================================================================
// 描画処理
//=============================================================================
void CShadow::Draw(void)
{
	// デバイスの取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;	// 計算用マトリックス

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 移動を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos->x, m_pos->y, m_pos->z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// ステンシルバッファテストの設定
	pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);						// ステンシルバッファを有効にする
	pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0x00000000);			// 描画をしないようにする
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);						// Zバッファに書き込まない

	pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);				// ステンシルバッファテストを常に通す
	pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_ZERO);			// ステンシルテスト、Zテスト共に合格した場合
	pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_INCR);			// ステンシルテストに合格、Zテストに不合格した場合
	pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_ZERO);			// ステンシルテストに不合格した場合

	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);					// 表面をカリング

	for (int nCntMat = 0; nCntMat < (int)m_nNumMat; nCntMat++)
	{// モデルの描画
		m_pMesh->DrawSubset(nCntMat);
	}

	// ステンシルバッファテストの設定
	pDevice->SetRenderState(D3DRS_STENCILREF, 0x01);						// 対象とする数値
	pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);						// ステンシルバッファを有効にする
	pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);				// ステンシルバッファテストを常に通す
	pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_INCR);			// ステンシルテスト、Zテスト共に合格した場合
	pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_ZERO);			// ステンシルテストに合格、Zテストに不合格した場合
	pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_ZERO);			// ステンシルテストに不合格した場合

	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);					// 裏面をカリング

	for (int nCntMat = 0; nCntMat < (int)m_nNumMat; nCntMat++)
	{// モデルの描画
		m_pMesh->DrawSubset(nCntMat);
	}

	pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0x0000000f);			// 元に戻す

	// ステンシルバッファテストの設定
	pDevice->SetRenderState(D3DRS_STENCILREF, 0x02);						// 対象とする数値
	pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);						// ステンシルバッファを有効にする
	pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);				// ステンシルバッファテストが同じときに通す
	pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);			// ステンシルテスト、Zテスト共に合格した場合
	pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);			// ステンシルテストに合格、Zテストに不合格した場合
	pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);			// ステンシルテストに不合格した場合
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);						// Zバッファに書き込めるようにする

	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);						// Zバッファに書き込めるようにする
	// 画面を黒く塗りつぶす
	m_pBigScreen->Draw();


	// ステンシルバッファテストの設定
	pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);					// ステンシルバッファ無効にする
}
