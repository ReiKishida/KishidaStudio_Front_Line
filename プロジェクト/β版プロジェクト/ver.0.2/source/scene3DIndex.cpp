//=============================================================================
//
// インデックス3Dオブジェクト処理 [scene3DIndex.h]
// Author : Takuto Ishida
//
//=============================================================================
#include "scene3DIndex.h"
#include "renderer.h"
#include "manager.h"
#include "debugProc.h"
#include "input.h"
#include "light.h"
#include "camera.h"

//==================================
// マクロ定義
//==================================
#define SIZE_X	(50.0f)	// ポリゴンの横の長さ
#define SIZE_Y	(50.0f)	// ポリゴンの縦の長さ

//==================================
// 静的メンバ変数宣言
//==================================

//=========================================
// テクスチャを割り当てる
//=========================================
void CScene3DIndex::BindTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
}

//=========================================
// コンストラクタ
//=========================================
CScene3DIndex::CScene3DIndex(int nPriority, OBJTYPE objType) : CScene(nPriority, objType)
{
	m_pTexture = NULL;		// テクスチャへのポインタ
	m_pVtxBuff = NULL;		// 頂点バッファへのポインタ

	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ポリゴンの位置
	m_fLengthX = 0;								// 一つのブロックの幅
	m_fLengthY = 0;								// 一つのブロックの高さ
	m_fLengthZ = 0;								// 一つのブロックの奥行
	m_nMeshX = 0;								// X軸の分割数
	m_nMeshY = 0;								// Y軸の分割数
	m_nMeshZ = 0;								// Z軸の分割数
}

//=========================================
// デストラクタ
//=========================================
CScene3DIndex::~CScene3DIndex()
{
}

//=========================================
// ポリゴンの初期化処理
//=========================================
HRESULT CScene3DIndex::Init(void)
{
	return S_OK;
}

//=========================================
// ポリゴンの終了処理
//=========================================
void CScene3DIndex::Uninit(void)
{
	// 頂点バッファの破棄
	if (NULL != m_pVtxBuff)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	// 死亡フラグ
	Release();
}

//=========================================
// ポリゴンの更新処理
//=========================================
void CScene3DIndex::Update(void)
{
}

//=========================================
// ポリゴンの描画処理
//=========================================
void CScene3DIndex::Draw(void)
{
	// デバイスの取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;	// ワールドマトリックスの保存用

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// インデックスバッファをデータストリームに設定
	pDevice->SetIndices(m_pIdxBuff);

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(0, m_pTexture);

	// ポリゴンの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, m_nNumVtx, 0, m_nNumPolygon);
}

//=========================================================================================================================
// 頂点情報の作成
//=========================================================================================================================
void CScene3DIndex::MakeVertex(void)
{
	// デバイスの取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	// 頂点数とポリゴン数の計算
	m_nNumVtx = (m_nMeshX + 1) * ((m_nMeshZ + m_nMeshY) + 1);
	m_nNumPolygon = m_nMeshX * (m_nMeshZ + m_nMeshY) * 2 + (4 * ((m_nMeshZ + m_nMeshY) - 1));

	// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * m_nNumVtx,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// インデックスバッファを生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * (m_nMeshX * (m_nMeshZ + m_nMeshY) * 2 + (4 * ((m_nMeshZ + m_nMeshY) - 1)) + 2),
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,			// 16ビットのデータを確保
		D3DPOOL_MANAGED,
		&m_pIdxBuff,
		NULL);

	// 頂点情報の計算
	SetVtx();
}

//=========================================================================================================================
// 頂点情報の設定
//=========================================================================================================================
void CScene3DIndex::SetVtx(void)
{
	// カウント用
	int nCntMeshYZ, nCntMeshX, nCntIdx;

	VERTEX_3D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntMeshYZ = 0; nCntMeshYZ <= m_nMeshZ + m_nMeshY; nCntMeshYZ++)
	{
		for (nCntMeshX = 0; nCntMeshX <= m_nMeshX; nCntMeshX++)
		{
			// 頂点座標
			switch (CScene::GetObjType())
			{
			case CScene::OBJTYPE_FIELD:		// フィールド
				pVtx[0 + (nCntMeshX + (nCntMeshYZ * (m_nMeshX + 1)))].pos =
					D3DXVECTOR3(-(m_fLengthX * 0.5f) + (m_fLengthX / m_nMeshX) * nCntMeshX,
						0.0f,
						(m_fLengthZ * 0.5f) - (m_fLengthZ / m_nMeshZ) * nCntMeshYZ);
				break;
			}

			// 頂点カラー
			pVtx[nCntMeshX + (nCntMeshYZ * (m_nMeshX + 1))].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			//テクスチャ座標
			pVtx[nCntMeshX + (nCntMeshYZ * (m_nMeshX + 1))].tex = D3DXVECTOR2(nCntMeshX * 1.0f, nCntMeshYZ * 1.0f);
		}
	}

	// 法線の計算
	SetNor(pVtx);

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	WORD *pIdx;		// インデックスデータへのポインタ

	// インデックスバッファをロックし、インデックスデータへのポインタを取得
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	// 地面
	switch (CScene::GetObjType())
	{
	case CScene::OBJTYPE_FIELD:		// フィールド
		for (nCntMeshYZ = 0, nCntIdx = 0; nCntMeshYZ < m_nMeshZ; nCntMeshYZ++)
		{
			for (nCntMeshX = 0; nCntMeshX <= m_nMeshX; nCntMeshX++, nCntIdx++)
			{

				pIdx[0] = (m_nMeshX + 1) + nCntIdx;
				pIdx[1] = 0 + nCntIdx;

				pIdx += 2;

				if (nCntMeshYZ < m_nMeshZ - 1 && nCntMeshX == m_nMeshX)
				{// 折り返し時
					pIdx[0] = 0 + nCntIdx;
					pIdx[1] = (m_nMeshX + 1) + nCntIdx + 1;

					pIdx += 2;
				}
			}
		}
		break;
	}

	// インデックスバッファをアンロック
	m_pIdxBuff->Unlock();
}

//=========================================================================================================================
// 法線の計算
//=========================================================================================================================
void CScene3DIndex::SetNor(VERTEX_3D *pVtx)
{
	// カウント用
	int nCntMeshYZ, nCntMeshX, nCntVtx;

	for (nCntVtx = 0; nCntVtx < m_nNumVtx; nCntVtx++)
	{// 全頂点の法線を初期化
		pVtx[nCntVtx].nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	for (nCntMeshYZ = 0; nCntMeshYZ < m_nMeshZ; nCntMeshYZ++)
	{
		for (nCntMeshX = 0; nCntMeshX < m_nMeshX; nCntMeshX++)
		{// 法線
			D3DXVECTOR3 vecA;	// 頂点０から頂点１への方向
			D3DXVECTOR3 vecB;	// 頂点０から頂点２への方向
			D3DXVECTOR3 nor[2];	// 法線の計算結果
			int nVtxNumber[4];	// ４頂点の番号

			nVtxNumber[0] = nCntMeshX + ((nCntMeshYZ + 1) * (m_nMeshX + 1));			// 自分のブロックの左下番号
			nVtxNumber[1] = nCntMeshX + (nCntMeshYZ * (m_nMeshX + 1));					// 自分のブロックの左上番号
			nVtxNumber[2] = (nCntMeshX + 1) + ((nCntMeshYZ + 1) * (m_nMeshX + 1));		// 自分のブロックの右下番号
			nVtxNumber[3] = (nCntMeshX + 1) + (nCntMeshYZ * (m_nMeshX + 1));			// 自分のブロックの右上番号

																						// 各ポリゴンの頂点を求める
																						// 左側-------------------------------------------------------------------------------------------
																						// 0
			vecA = pVtx[nVtxNumber[1]].pos - pVtx[nVtxNumber[0]].pos;	// 左側
			vecB = pVtx[nVtxNumber[2]].pos - pVtx[nVtxNumber[0]].pos;	// 右側

			D3DXVec3Cross(&nor[0], &vecA, &vecB);	// 外積を求める
			D3DXVec3Normalize(&nor[0], &nor[0]);	// 正規化

													// 右側-------------------------------------------------------------------------------------------
													// 3
			vecA = pVtx[nVtxNumber[2]].pos - pVtx[nVtxNumber[3]].pos;	// 左側
			vecB = pVtx[nVtxNumber[1]].pos - pVtx[nVtxNumber[3]].pos;	// 右側

			D3DXVec3Cross(&nor[1], &vecA, &vecB);	// 外積を求める
			D3DXVec3Normalize(&nor[1], &nor[1]);	// 正規化

													// 各頂点の法線を算出
			pVtx[nVtxNumber[0]].nor += nor[0];
			pVtx[nVtxNumber[1]].nor += nor[0] + nor[1];
			pVtx[nVtxNumber[2]].nor += nor[0] + nor[1];
			pVtx[nVtxNumber[3]].nor += nor[1];
		}
	}

	for (nCntMeshYZ = 0; nCntMeshYZ < m_nMeshZ - 1; nCntMeshYZ++)
	{
		for (nCntMeshX = 0; nCntMeshX < m_nMeshX - 1; nCntMeshX++)
		{// 法線の平均を出す
			D3DXVECTOR3 vecA;	// 頂点０から頂点１への方向
			D3DXVECTOR3 vecB;	// 頂点０から頂点２への方向
			int nVtxNumber[4];	// ４頂点の番号

			nVtxNumber[0] = nCntMeshX + ((nCntMeshYZ + 1) * (m_nMeshX + 1));			// ブロックの左下番号
			nVtxNumber[1] = nCntMeshX + (nCntMeshYZ * (m_nMeshX + 1));					// ブロックの左上番号
			nVtxNumber[2] = (nCntMeshX + 1) + ((nCntMeshYZ + 1) * (m_nMeshX + 1));		// ブロックの右下番号
			nVtxNumber[3] = (nCntMeshX + 1) + (nCntMeshYZ * (m_nMeshX + 1));			// ブロックの右上番号

			if (nCntMeshX == 0 && nCntMeshYZ == 0)
			{// 左上端
				pVtx[nVtxNumber[0]].nor /= 3;
				pVtx[nVtxNumber[1]].nor /= 2;
				pVtx[nVtxNumber[2]].nor /= 6;
				pVtx[nVtxNumber[3]].nor /= 3;
			}
			else if (nCntMeshX == 0 && nCntMeshYZ > 0 && nCntMeshYZ < m_nMeshZ - 1)
			{// 左端
				pVtx[nVtxNumber[0]].nor /= 3;
				pVtx[nVtxNumber[2]].nor /= 6;
			}
			else if (nCntMeshX == m_nMeshX - 1 && nCntMeshYZ > 0 && nCntMeshYZ < m_nMeshZ - 1)
			{
				pVtx[nVtxNumber[2]].nor /= 3;
			}
			else if (nCntMeshYZ == 0)
			{// 上端
				if (nCntMeshX == m_nMeshX - 1)
				{// 右上端
					pVtx[nVtxNumber[2]].nor /= 3;
				}
				else
				{// 上中
					pVtx[nVtxNumber[2]].nor /= 6;
					pVtx[nVtxNumber[3]].nor /= 3;
				}
			}
			else if (nCntMeshYZ == m_nMeshZ - 1)
			{// 下端
				if (nCntMeshX == m_nMeshX - 1)
				{// 右下端
					pVtx[nVtxNumber[2]].nor /= 2;
				}
				else
				{// 下中
					pVtx[nVtxNumber[2]].nor /= 3;
				}
			}
			else if (nCntMeshX > 0 && nCntMeshYZ > 0 && nCntMeshX < m_nMeshX - 1 && nCntMeshYZ < m_nMeshZ - 1)
			{// 中
				pVtx[nVtxNumber[2]].nor /= 6;
			}
		}
	}
}

//=========================================================================================================================
// 頂点情報の設定
//=========================================================================================================================
void CScene3DIndex::SetInfo(float fWidth, float fHeight, float fDepth, int nMeshX, int nMeshY, int nMeshZ)
{
	m_fLengthX = fWidth;
	m_fLengthY = fHeight;
	m_fLengthZ = fDepth;
	m_nMeshX = nMeshX;
	m_nMeshY = nMeshY;
	m_nMeshZ = nMeshZ;
}

//=========================================================================================================================
// 頂点情報の取得
//=========================================================================================================================
void CScene3DIndex::GetInfo(float *pWidth, float *pHeight, float *pDepth, int *pMeshX, int *pMeshY, int *pMeshZ)
{
	if (pWidth != NULL) { *pWidth = m_fLengthX; }
	if (pHeight != NULL) { *pHeight = m_fLengthY; }
	if (pDepth != NULL) { *pDepth = m_fLengthZ; }
	if (pMeshX != NULL) { *pMeshX = m_nMeshX; }
	if (pMeshY != NULL) { *pMeshY = m_nMeshY; }
	if (pMeshZ != NULL) { *pMeshZ = m_nMeshZ; }
}