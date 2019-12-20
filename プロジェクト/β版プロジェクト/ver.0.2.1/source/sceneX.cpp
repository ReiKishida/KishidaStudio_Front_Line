//=============================================================================
//
// 3Dモデルオブジェクト処理 [sceneX.cpp]
// Author : Ishida Takuto
//
//=============================================================================
#include "sceneX.h"
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
// モデル情報を割り当てる
//=========================================
const void CSceneX::BindInfo(LPDIRECT3DTEXTURE9 pTexture, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat)
{
	m_pTexture = pTexture;
	m_pMesh = pMesh;
	m_pBuffMat = pBuffMat;
	m_nNumMat = nNumMat;
}

//=========================================
// コンストラクタ
//=========================================
CSceneX::CSceneX(int nPriority, CScene::OBJTYPE objType) : CScene(nPriority, objType)
{
	m_pTexture = NULL;		// テクスチャへのポインタ
	m_pBuffMat = NULL;		// マテリアルへのポインタ
	m_pMesh = NULL;
	m_vtxMin = D3DXVECTOR3(10000.0f, 10000.0f, 10000.0f);
	m_vtxMax = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// ポリゴンの位置
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// ポリゴンの向き
}

//=========================================
// デストラクタ
//=========================================
CSceneX::~CSceneX()
{
}

//=========================================
// ポリゴンの初期化処理
//=========================================
HRESULT CSceneX::Init(void)
{
	int nNumVtx;		// 頂点数
	DWORD sizeFVF;		// 頂点フォーマットのサイズ
	BYTE *pVtxBuff;		// 頂点バッファへのポインタ

	// 頂点数を取得
	nNumVtx = m_pMesh->GetNumVertices();

	// 頂点フォーマットのサイズを取得
	sizeFVF = D3DXGetFVFVertexSize(m_pMesh->GetFVF());

	// 頂点バッファをロック
	m_pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{// 頂点座標の比較
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;	// 頂点座標の代入

		// Xの値の比較
		if (m_vtxMin.x > vtx.x)
		{// 最小値と比較
			m_vtxMin.x = vtx.x;	// 最小値より小さければ代入
		}

		if (m_vtxMax.x < vtx.x)
		{// 最大値と比較
			m_vtxMax.x = vtx.x;	// 最大値より大きければ代入
		}

		// Yの値の比較
		if (m_vtxMin.y > vtx.y)
		{// 最小値と比較
			m_vtxMin.y = vtx.y;	// 最小値より小さければ代入
		}

		if (m_vtxMax.y < vtx.y)
		{// 最大値と比較
			m_vtxMax.y = vtx.y;	// 最大値より大きければ代入
		}

		// Zの値の比較
		if (m_vtxMin.z > vtx.z)
		{// 最小値と比較
			m_vtxMin.z = vtx.z;	// 最小値より小さければ代入
		}

		if (m_vtxMax.z < vtx.z)
		{// 最大値と比較
			m_vtxMax.z = vtx.z;	// 最大値より大きければ代入
		}

		pVtxBuff += sizeFVF;	// サイズ分ポインタを進める
	}

	// 頂点バッファをアンロック
	m_pMesh->UnlockVertexBuffer();

	return S_OK;
}

//=========================================
// ポリゴンの終了処理
//=========================================
void CSceneX::Uninit(void)
{
	// 自分の破棄
	Release();
}

//=========================================
// ポリゴンの更新処理
//=========================================
void CSceneX::Update(void)
{
}

//=========================================
// ポリゴンの描画処理
//=========================================
void CSceneX::Draw(void)
{
	// デバイスの取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;	// 計算用マトリックス
	D3DMATERIAL9 matDef;			// 現在のマテリアル保存用

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

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// マテリアルデータへのポインタを取得
	m_pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_nNumMat; nCntMat++)
	{
		// マテリアルの設定
		pDevice->SetMaterial(&m_pMat[nCntMat].MatD3D);

		if (m_pMat[nCntMat].pTextureFilename != NULL)
		{// オブジェクトのテクスチャーをステージ０にセットする
			pDevice->SetTexture(0, m_pTexture);
		}
		else
		{// テクスチャがないとき
			pDevice->SetTexture(0, NULL);
		}

		// モデルの描画
		m_pMesh->DrawSubset(nCntMat);
	}

	// マテリアルをデフォルトに戻す
	pDevice->SetMaterial(&matDef);
}

//=========================================
// 四角形の当たり判定
//=========================================
int CSceneX::CollisionBox(D3DXVECTOR3 *pos, D3DXVECTOR3 posOld, D3DXVECTOR3 rot)
{
	int nCollision = 0;
	if (m_pos.x + m_vtxMax.x >= pos->x && m_pos.x + m_vtxMin.x <= pos->x &&
		m_pos.z + m_vtxMax.z >= pos->z && m_pos.z + m_vtxMin.z <= pos->z)
	{// xとz軸が範囲内
		if (m_pos.y + m_vtxMax.y >= pos->y && m_pos.y + m_vtxMin.y <= posOld.y ||
			m_pos.y + m_vtxMin.y >= pos->y && m_pos.y + m_vtxMax.y <= posOld.y)
		{// 同じ高さ
			pos->y = m_pos.y + m_vtxMax.y;	// 高さをモデルに合わせる

			nCollision = 1;
		}
		else
		{// 落ちたとき
			nCollision = 2;
		}
	}

	return nCollision;
}

//=========================================
// ワールドマトリックスの設定
//=========================================
D3DXMATRIX CSceneX::SetWorldMatrix(D3DXMATRIX *mtxWorld)
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

	D3DXMatrixMultiply(mtxWorld, mtxWorld, &m_mtxWorld);

	return *mtxWorld;
}

//=========================================
// 色の設定
//=========================================
const void CSceneX::SetColor(D3DXCOLOR col)
{
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	D3DMATERIAL9 matDef;			// 現在のマテリアル保存用

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// マテリアルデータへのポインタを取得
	m_pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	// 色を付ける
	m_pMat->MatD3D.Diffuse = col;
	m_pMat->MatD3D.Ambient = col;

	// マテリアルをデフォルトに戻す
	pDevice->SetMaterial(&matDef);
}

//=========================================
// 色の加算
//=========================================
const void CSceneX::AddColor(D3DXCOLOR col)
{
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	D3DMATERIAL9 matDef;			// 現在のマテリアル保存用
									// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// マテリアルデータへのポインタを取得
	m_pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	// 色を付ける
	m_pMat->MatD3D.Diffuse.r += col.r;
	m_pMat->MatD3D.Diffuse.g += col.g;
	m_pMat->MatD3D.Diffuse.b += col.b;
	m_pMat->MatD3D.Diffuse.a += col.a;

	m_pMat->MatD3D.Ambient.r += col.r;
	m_pMat->MatD3D.Ambient.g += col.g;
	m_pMat->MatD3D.Ambient.b += col.b;
	m_pMat->MatD3D.Ambient.a += col.a;

	// マテリアルをデフォルトに戻す
	pDevice->SetMaterial(&matDef);
}
