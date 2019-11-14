//=============================================================================
//
// モデル処理 [model.cpp]
// Author : Takuto Ishida
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "model.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "debugProc.h"
#include "light.h"
#include "camera.h"

#include <stdio.h>

//=========================================
// マクロ定義
//=========================================

//=========================================
// 静的メンバ変数宣言
//=========================================

//=========================================
// モデルの生成
//=========================================
CModel *CModel::Create(D3DXMATRIX *pParent)
{
	CModel *pModel = NULL;

	if (pModel == NULL)
	{// メモリを確保する
		pModel = new CModel;	// メモリ確保
		pModel->m_pMtxParent = pParent;
	}

	return pModel;
}

//=========================================
// コンストラクタ
//=========================================
CModel::CModel()
{
	m_vtxMin = D3DXVECTOR3(10000.0f, 10000.0f, 10000.0f);
	m_vtxMax = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 位置
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 角度

	m_pTexture = NULL;
	m_pMesh = NULL;
	m_pBuffMat = NULL;
	m_nNumMat = 0;
}

//=========================================
// デストラクタ
//=========================================
CModel::~CModel()
{
}

//=========================================
// 初期化処理
//=========================================
HRESULT CModel::Init()
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
// 終了処理
//=========================================
void CModel::Uninit(void)
{
	if (NULL != m_pTexture)
	{// テクスチャの破棄
		for (int nCntTex = 0; nCntTex < (int)m_nNumMat; nCntTex++)
		{
			if (NULL != m_pTexture[nCntTex])
			{
				m_pTexture[nCntTex]->Release();
				m_pTexture[nCntTex] = NULL;
			}
		}

		// メモリの開放
		delete[] m_pTexture;
		m_pTexture = NULL;
	}

	if (NULL != m_pMesh)
	{// メッシュ情報の破棄
		m_pMesh->Release();
		m_pMesh = NULL;
	}

	if (NULL != m_pBuffMat)
	{// マテリアル情報の破棄
		m_pBuffMat->Release();
		m_pBuffMat = NULL;
	}

	if (NULL != m_nNumMat)
	{// マテリアルの数を破棄
		m_nNumMat = NULL;
	}
}

//=========================================
// 更新処理
//=========================================
void CModel::Update(void)
{
}

//=========================================
// 描画処理
//=========================================
void CModel::Draw(void)
{
	// デバイスの取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans, mtxParent;	// 計算用マトリックス
	D3DMATERIAL9 matDef;					// 現在のマテリアル保存用
	D3DXMATERIAL *pMat;						// マテリアルデータへのポインタ

	if (m_pParent != NULL)
	{// 親のマトリックスを取得
		mtxParent = m_pParent->GetMtxWorld();
	}
	else if (m_pMtxParent != NULL)
	{
		mtxParent = *m_pMtxParent;
	}

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 移動を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	if (m_pMtxParent != NULL)
	{// 親のマトリックスと掛け合わせる
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxParent);
	}

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_nNumMat; nCntMat++)
	{
		// マテリアルの設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		//オブジェクトのテクスチャーをステージ０にセットする
		pDevice->SetTexture(0, m_pTexture[nCntMat]);

		// モデルの描画
		m_pMesh->DrawSubset(nCntMat);
	}

	// マテリアルをデフォルトに戻す
	pDevice->SetMaterial(&matDef);
}

//=========================================
// マトリックスの計算
//=========================================
void CModel::MatrixCalculation(void)
{
	// デバイスの取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans, mtxParent;	// 計算用マトリックス

	if (m_pParent != NULL)
	{// 親のマトリックスを取得
		mtxParent = m_pParent->GetMtxWorld();
	}
	else if (m_pMtxParent != NULL)
	{
		mtxParent = *m_pMtxParent;
	}

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 移動を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	if (m_pMtxParent != NULL)
	{// 親のマトリックスと掛け合わせる
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxParent);
	}

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);
}

//=========================================
// 使うモデルの設定
//=========================================
void CModel::SetModel(char *pModelName)
{
	// デバイスの取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	D3DXLoadMeshFromX(pModelName,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_pBuffMat,
		NULL,
		&m_nNumMat,
		&m_pMesh);

	D3DXMATERIAL *pMat;				// マテリアルデータへのポインタ

	// マテリアル数分テクスチャを用意
	m_pTexture = new LPDIRECT3DTEXTURE9[(int)m_nNumMat];

	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();
	for (int nCntMat = 0; nCntMat < (int)m_nNumMat; nCntMat++)
	{// テクスチャの生成
		m_pTexture[nCntMat] = NULL;
		D3DXCreateTextureFromFile(pDevice, pMat[nCntMat].pTextureFilename, &m_pTexture[nCntMat]);
	}
}

//=========================================
// 色の設定
//=========================================
void CModel::SetColor(D3DXCOLOR col)
{
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	D3DMATERIAL9 matDef;	// 現在のマテリアル保存用
	D3DXMATERIAL *pMat;	// マテリアルデータへのポインタ

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	// 色を付ける
	pMat->MatD3D.Diffuse = col;
	pMat->MatD3D.Ambient = col;

	// マテリアルをデフォルトに戻す
	pDevice->SetMaterial(&matDef);
}

//=========================================
// 色の加算
//=========================================
void CModel::AddColor(D3DXCOLOR col)
{
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	D3DMATERIAL9 matDef;	// 現在のマテリアル保存用
	D3DXMATERIAL *pMat;		// マテリアルデータへのポインタ

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	// 色を付ける
	pMat->MatD3D.Diffuse.r += col.r;
	pMat->MatD3D.Diffuse.g += col.g;
	pMat->MatD3D.Diffuse.b += col.b;
	pMat->MatD3D.Diffuse.a += col.a;

	pMat->MatD3D.Ambient.r += col.r;
	pMat->MatD3D.Ambient.g += col.g;
	pMat->MatD3D.Ambient.b += col.b;
	pMat->MatD3D.Ambient.a += col.a;

	// マテリアルをデフォルトに戻す
	pDevice->SetMaterial(&matDef);
}

//=========================================================================================================================
// 当たり判定
//=========================================================================================================================
bool CModel::Collision(D3DXVECTOR3 pos, float fRange)
{
	bool bHit = false;

	// 相手のXYの範囲
	float fLength = powf(fRange, 2);

	float fLengthX = pos.x - m_mtxWorld._41;						// Xの差
	float fLengthY = pos.y - m_mtxWorld._42;						// Yの差
	float fLengthZ = pos.z - m_mtxWorld._43;						// Yの差
	float fLengthTotal = powf(fLengthX, 2) + powf(fLengthY, 2) + powf(fLengthZ, 2);		// XYの差の二乗

	if (fLength >= fLengthTotal)
	{// 相手と自分の判定
		if (false == bHit)
		{// 判定を有効にする
			bHit = true;
		}
	}

	return bHit;
}
