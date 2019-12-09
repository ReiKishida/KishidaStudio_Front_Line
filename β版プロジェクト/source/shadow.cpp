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
#include "debugProc.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define SHADOW_MODEL_FILE_000	"data/MODEL/shadow/shadow_000.x"
#define SHADOW_MODEL_FILE_001	"data/MODEL/shadow/shadow_001.x"
#define SHADOW_MODEL_FILE_002	"data/MODEL/shadow/shadow_002.x"
#define SHADOW_MODEL_FILE_003	"data/MODEL/shadow/shadow_003.x"
#define SHADOW_MODEL_FILE_004	"data/MODEL/shadow/shadow_004.x"
#define SHADOW_MODEL_FILE_005	"data/MODEL/shadow/shadow_005.x"
#define SHADOW_MODEL_FILE_006	"data/MODEL/shadow/shadow_006.x"
#define SHADOW_MODEL_FILE_007	"data/MODEL/shadow/shadow_007.x"
#define SHADOW_MODEL_FILE_008	"data/MODEL/shadow/shadow_008.x"
#define SHADOW_MODEL_FILE_009	"data/MODEL/shadow/shadow_009.x"
#define SHADOW_MODEL_FILE_010	"data/MODEL/shadow/shadow_010.x"
#define SHADOW_MODEL_FILE_011	"data/MODEL/shadow/shadow_011.x"
#define SHADOW_MODEL_FILE_012	"data/MODEL/shadow/shadow_012.x"
#define SHADOW_MODEL_FILE_013	"data/MODEL/shadow/shadow_013.x"
#define SHADOW_MODEL_FILE_014	"data/MODEL/shadow/shadow_014.x"
#define SHADOW_MODEL_FILE_015	"data/MODEL/shadow/shadow_015.x"

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================

//==================================
// 生成処理
//==================================
CShadow *CShadow::Create(D3DXVECTOR3 pos, int nPriority, int nModel)
{
	CShadow *pShadow;

	pShadow = new CShadow;	// メモリを確保

	if (pShadow != NULL)
	{// メモリ確保成功
		pShadow->m_pos = pos;
		pShadow->m_nModel = nModel;
		pShadow->Init();
		pShadow->SwapPriority(nPriority);
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
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
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

	char *FileName[16] = {
		SHADOW_MODEL_FILE_001, SHADOW_MODEL_FILE_002, SHADOW_MODEL_FILE_003, SHADOW_MODEL_FILE_004,
		SHADOW_MODEL_FILE_005, SHADOW_MODEL_FILE_006, SHADOW_MODEL_FILE_007, SHADOW_MODEL_FILE_008, SHADOW_MODEL_FILE_009,
		SHADOW_MODEL_FILE_010, SHADOW_MODEL_FILE_011, SHADOW_MODEL_FILE_012, SHADOW_MODEL_FILE_013, SHADOW_MODEL_FILE_014,
		SHADOW_MODEL_FILE_015, SHADOW_MODEL_FILE_000
	};

	// Xファイルの読み込み
	D3DXLoadMeshFromX(FileName[m_nModel],
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_pBuffMat,
		NULL,
		&m_nNumMat,
		&m_pMesh);

	// 2Dポリゴンの生成
	m_pBigScreen = CPolygon::Create(1, D3DXVECTOR3(0.0f, 0.0f, 0.0f), SCREEN_WIDTH, SCREEN_HEIGHT);
	m_pBigScreen->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.7f));
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
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

#if 0
	// 深度バッファに書き込みはしない
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	// レンダリングターゲットに書き込みはしない
	pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, FALSE);

	// 両面描く
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	// 両面ステンシルを使用する
	pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_TWOSIDEDSTENCILMODE, TRUE);

	// ステンシルテストは常に合格する（＝テストしない）
	pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_CCW_STENCILFUNC, D3DCMP_ALWAYS);

	// ステンシルバッファの増減を1に設定する
	pDevice->SetRenderState(D3DRS_STENCILREF, 0x1);
	pDevice->SetRenderState(D3DRS_STENCILMASK, 0xffffffff);
	pDevice->SetRenderState(D3DRS_STENCILWRITEMASK, 0xffffffff);

	// 表面は深度テストに合格したらステンシルバッファの内容を+1する
	pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_INCR);
	pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);
	pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);

	// 裏面は深度テストに合格したらステンシルバッファの内容を-1する
	pDevice->SetRenderState(D3DRS_CCW_STENCILPASS, D3DSTENCILOP_DECR);
	pDevice->SetRenderState(D3DRS_CCW_STENCILZFAIL, D3DSTENCILOP_KEEP);
	pDevice->SetRenderState(D3DRS_CCW_STENCILFAIL, D3DSTENCILOP_KEEP);

	for (int nCntMat = 0; nCntMat < (int)m_nNumMat; nCntMat++)
	{// モデルの描画
		m_pMesh->DrawSubset(nCntMat);
	}

	// 状態を元に戻す
	//pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0xf);
	pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_TWOSIDEDSTENCILMODE, FALSE);

	// 深度テストはしない
	pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);

	// ステンシルテストはする
	pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);

	// アルファブレンディングは線形に掛ける
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// ステンシルバッファの値が１以上のときに書き込む
	pDevice->SetRenderState(D3DRS_STENCILREF, 0x1);
	pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);
	pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);

	// 画面を黒く塗りつぶす
	m_pBigScreen->Draw();

	// 状態を元に戻す
	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
#endif

#if 1
	// ステンシルバッファテストの設定
	pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);						// ステンシルバッファを有効にする
	pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0x00000000);			// 描画をしないようにする
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);						// Zバッファに書き込まない

	pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);				// ステンシルバッファテストを常に通す
	pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_ZERO);			// ステンシルテスト、Zテスト共に合格した場合
	pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_INCR);			// ステンシルテストに合格、Zテストに不合格した場合
	pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_ZERO);			// ステンシルテストに不合格した場合

	//pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

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

	// ステンシルバッファテストの設定
	pDevice->SetRenderState(D3DRS_STENCILREF, 0x01);						// 対象とする数値
	pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);						// ステンシルバッファを有効にする
	pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_LESSEQUAL);			// ステンシルバッファテストを通す条件
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
	pDevice->SetRenderState(D3DRS_STENCILREF, 0x01);						// 対象とする数値
	pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);						// ステンシルバッファを有効にする
	pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);				// ステンシルバッファテストが同じときに通す
	pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);			// ステンシルテスト、Zテスト共に合格した場合
	pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);			// ステンシルテストに合格、Zテストに不合格した場合
	pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);			// ステンシルテストに不合格した場合
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);						// Zバッファに書き込めるようにする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

	// 画面を黒く塗りつぶす
	m_pBigScreen->Draw();

	// ステンシルバッファテストの設定
	pDevice->SetRenderState(D3DRS_STENCILREF, 0x02);						// 対象とする数値
	pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);						// ステンシルバッファを有効にする
	pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);				// ステンシルバッファテストが同じときに通す
	pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);			// ステンシルテスト、Zテスト共に合格した場合
	pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);			// ステンシルテストに合格、Zテストに不合格した場合
	pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);			// ステンシルテストに不合格した場合
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);						// Zバッファに書き込めるようにする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

	// 画面を黒く塗りつぶす
	m_pBigScreen->Draw();

	// ステンシルバッファテストの設定
	pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);					// ステンシルバッファ無効にする
#endif
}