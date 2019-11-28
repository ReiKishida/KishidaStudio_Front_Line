//=============================================================================
//
// モデル処理 [model.cpp]
// Author : Takuto Ishida
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "search.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "debugProc.h"
#include "light.h"
#include "camera.h"
#include "player.h"

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
CSearch *CSearch::Create(D3DXMATRIX *pParent)
{
	CSearch *pSearch = NULL;

	if (pSearch == NULL)
	{// メモリを確保する
		pSearch = new CSearch;	// メモリ確保
		pSearch->m_pMtxParent = pParent;
	}

	return pSearch;
}

//=========================================
// 角度代入
//=========================================
void CSearch::SetRot(D3DXVECTOR3 rot)
{
	D3DXVECTOR3 Pos;

	m_rot = rot;

	m_pos.x = (m_pos.x * cosf(m_rot.y)) - (m_pos.z * sinf(m_rot.y));
	m_pos.y = 0.0f;
	m_pos.z = (m_pos.x * sinf(m_rot.y)) + (m_pos.z * cosf(m_rot.y));

}

//=========================================
// 角度代入
//=========================================
bool CSearch::Distance(void)
{
	bool bFind = false;

	return bFind;
}

//=========================================
// コンストラクタ
//=========================================
CSearch::CSearch()
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 位置
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 角度

	m_pParent = NULL;
}

//=========================================
// デストラクタ
//=========================================
CSearch::~CSearch()
{
}

//=========================================
// 初期化処理
//=========================================
HRESULT CSearch::Init()
{
	return S_OK;
}

//=========================================
// 終了処理
//=========================================
void CSearch::Uninit(void)
{

}

//=========================================
// 更新処理
//=========================================
void CSearch::Update(void)
{


}

//=========================================
// 描画処理
//=========================================
void CSearch::Draw(void)
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
// ベクトル計算の処理
//=========================================
D3DXVECTOR3 CSearch::Sub_Vector(const D3DXVECTOR3 & Pos0, const D3DXVECTOR3 & Pos1)
{
	D3DXVECTOR3 Vec;

	Vec.x = Pos0.x - Pos1.x;
	Vec.y = Pos0.y - Pos1.y;
	Vec.z = Pos0.z - Pos1.z;

	return Vec;
}

//=========================================
// 外積計算の処理
//=========================================
D3DXVECTOR3 CSearch::Cross_Product(const D3DXVECTOR3 & VecA, const D3DXVECTOR3 & VecB)
{
	D3DXVECTOR3 Vec;
	Vec.x = (VecA.y * VecB.z) - (VecA.z * VecB.y);
	Vec.y = (VecA.z * VecB.x) - (VecA.x * VecB.z);
	Vec.z = (VecA.x * VecB.y) - (VecA.y * VecB.x);

	return Vec;
}
