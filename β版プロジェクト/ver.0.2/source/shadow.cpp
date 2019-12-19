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
#include "debugProc.h"
#include "line.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================

//==================================
// 生成処理
//==================================
CShadow *CShadow::Create(CScene *pParent)
{
	CShadow *pShadow;

	pShadow = new CShadow;	// メモリを確保

	if (pShadow != NULL)
	{// メモリ確保成功
		pShadow->m_pParent = pParent;
		pShadow->Init();
	}

	return pShadow;
}

//=========================================
// コンストラクタ
//=========================================
CShadow::CShadow(int nPriority, CScene::OBJTYPE objType) : CScene(nPriority, objType)
{
	m_pParent = NULL;
	m_ObjPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
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
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CShadow::Uninit(void)
{
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
}

//=============================================================================
// モデルに影をつける
//=============================================================================
void CShadow::Set(D3DXMATRIX mtxWorld, LPD3DXMESH mesh, int nNumMat, D3DXVECTOR3 _pos, float fHeight)
{
	m_ObjPos = _pos;

	// デバイスの取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	D3DXVECTOR4 vecLight;		// ライトのベクトル
	D3DXVECTOR3 pos, normal;	// 位置と法線

	// 被らないようにする
	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

	// αブレンディングを減算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// ライティングを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// シャドウマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxShadow);

	// ライトのベクトルの設定
	D3DXVECTOR3 vecDir = CLight::GetDirection();
	vecLight = D3DXVECTOR4(-vecDir.x, -vecDir.y, -vecDir.z, 0.0f);

	// 影の位置
	pos = D3DXVECTOR3(0.0f, fHeight, 0.0f);

	// 影の法線
	normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	// 点と法線から平面を作成する
	D3DXPlaneFromPointNormal(&m_planeField, &pos, &normal);

	// 平面に射影するマトリックスを作成
	D3DXMatrixShadow(&m_mtxShadow, &vecLight, &m_planeField);

	// ワールドマトリックスと掛け合わせる
	D3DXMatrixMultiply(&m_mtxShadow, &mtxWorld, &m_mtxShadow);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxShadow);

	// テクスチャをNULLにする
	pDevice->SetTexture(0, NULL);

	for (int nCntMat = 0; nCntMat < nNumMat; nCntMat++)
	{// 影の描画
		mesh->DrawSubset(nCntMat);
	}

	// ライティングを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// 被らないようにする
	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
}

//=============================================================================
// 影の範囲内かどうか
//=============================================================================
bool CShadow::RangeShadow(D3DXVECTOR3 pos, CScene *pObject, D3DXVECTOR3 vtxDiff)
{
	// モデルオブジェクトを探す
	CScene *pScene = CScene::GetSceneTop(SHADOW_PRIORITY);

	while (pScene != NULL)
	{// NULLになるまでループ
		CScene *pSceneNext = pScene->GetSceneNext();
		CScene::OBJTYPE objType = pScene->GetObjType();

		if (CScene::OBJTYPE_SHADOW == objType)
		{// 当たり判定の場合
			CShadow *pShadow = (CShadow*)pScene;
			if (pShadow->m_pParent != pObject)
			{// 関数を呼び出したオブジェクト以外
				D3DXVECTOR3 vecLight = -CManager::GetLight()->GetDirection();
				float fAngle = acosf(vecLight.x / sqrtf(vecLight.x * vecLight.x + vecLight.z * vecLight.z));
				if (vecLight.z < 0) { fAngle = D3DX_PI - fAngle; }

				// 判定用頂点
				D3DXVECTOR3 vtx0 = D3DXVECTOR3(pShadow->m_ObjPos.x - vtxDiff.x, 0, pShadow->m_ObjPos.z);
				D3DXVECTOR3 vtx1 = D3DXVECTOR3(pShadow->m_ObjPos.x,				0, pShadow->m_ObjPos.z);
				D3DXVECTOR3 vtx2 = D3DXVECTOR3(pShadow->m_ObjPos.x - vtxDiff.x, 0, pShadow->m_ObjPos.z - vtxDiff.z);
				D3DXVECTOR3 vtx3 = D3DXVECTOR3(pShadow->m_ObjPos.x,				0, pShadow->m_ObjPos.z - vtxDiff.z);

				// 回転させる
				vtx0.x *= sinf(fAngle); vtx0.z *= cosf(fAngle);
				vtx1.x *= sinf(fAngle); vtx1.z *= cosf(fAngle);
				vtx2.x *= sinf(fAngle); vtx2.z *= cosf(fAngle);
				vtx3.x *= sinf(fAngle); vtx3.z *= cosf(fAngle);

				if (NULL == pShadow->m_pParent)
				{
					CLine::Create(vtx0, vtx1, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
					CLine::Create(vtx1, vtx3, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
					CLine::Create(vtx3, vtx2, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
					CLine::Create(vtx2, vtx0, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));

					D3DXVECTOR3 lineY = D3DXVECTOR3(0.0f, 40.0f, 0.0f);
					CLine::Create(vtx0, vtx0 + lineY, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
					CLine::Create(vtx1, vtx1 + lineY, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
					CLine::Create(vtx2, vtx2 + lineY, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
					CLine::Create(vtx3, vtx3 + lineY, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));

					CLine::Create(vtx0 + lineY, vtx1 + lineY, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
					CLine::Create(vtx1 + lineY, vtx3 + lineY, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
					CLine::Create(vtx3 + lineY, vtx2 + lineY, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
					CLine::Create(vtx2 + lineY, vtx0 + lineY, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
				}

				D3DXVECTOR3 vecA, vecB;
				bool bRangeX = false, bRangeZ = false;
				/********************************/
				/*			Xの外積計算			*/
				/********************************/
				// 最小値側の判定
				vecA = vtx0 - vtx2;
				vecB = pos - vtx2;

				if ((vecA.z * vecB.x) - (vecA.x * vecB.z) > 0)
				{// 範囲内
				 // 最大数側の判定
					vecA = vtx3 - vtx1;
					vecB = pos - vtx1;

					if ((vecA.z * vecB.x) - (vecA.x * vecB.z) > 0)
					{// 範囲内
						bRangeX = true;
					}
				}

				/********************************/
				/*			Zの外積計算			*/
				/********************************/
				// 最小値側の判定
				vecA = vtx1 - vtx0;
				vecB = pos - vtx0;

				if ((vecA.z * vecB.x) - (vecA.x * vecB.z) > 0)
				{// 範囲内
				 // 最大数側の判定
					vecA = vtx2 - vtx3;
					vecB = pos - vtx3;

					if ((vecA.z * vecB.x) - (vecA.x * vecB.z) > 0)
					{// 範囲内
						bRangeZ = true;
					}
				}

				if (bRangeX && bRangeZ)
				{// 平面に乗っている
					if (pShadow->m_pParent == NULL)CDebugProc::Print("影の範囲内");
					return true;
				}
			}
		}

		// 次のオブジェクトを見る
		pScene = pSceneNext;
	}

	return false;
}