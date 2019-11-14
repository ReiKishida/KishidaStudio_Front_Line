//=============================================================================
//
// 親子関係処理 [model.h]
// Author : Takuto Ishida
//
//=============================================================================
#ifndef _MODEL_H_
#define _MODEL_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// 前方宣言
//*****************************************************************************

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CModel
{
public:
	CModel();
	~CModel();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CModel *Create(D3DXMATRIX *pParent = NULL);

	D3DXVECTOR3 GetPos(void) { return m_pos; };					// 位置の取得
	const void SetPos(D3DXVECTOR3 pos) { m_pos = pos; };		// 位置の設定

	D3DXVECTOR3 GetRot(void) { return m_rot; };					// 角度の取得
	const void SetRot(D3DXVECTOR3 rot) { m_rot = rot; };		// 角度の設定

	D3DXMATRIX &GetMtxWorld(void) { return m_mtxWorld; };					// ワールドマトリックスの取得
	const void SetMtxWorld(D3DXMATRIX mtxWorld) { m_mtxWorld = mtxWorld; };	// ワールドマトリックスの設定

	int GetModel(void) { return m_nIdxModel; };						// 位置の取得
	void SetModel(char *pModelName);							// モデルの設定

	CModel *GetParent(void) { return m_pParent; };					// 親の取得
	void SetParent(CModel *parent) { m_pParent = parent; };	// 親の設定

	D3DXVECTOR3 GetVtxMax(void) { return m_vtxMax; };				// 頂点座標の最大
	D3DXVECTOR3 GetVtxMin(void) { return m_vtxMin; };				// 頂点座標の最小

	LPD3DXMESH GetMesh(void) { return m_pMesh; };					// メッシュ情報の取得

	void SetColor(D3DXCOLOR col);									// 色の設定
	void AddColor(D3DXCOLOR col);

	bool Collision(D3DXVECTOR3 pos, float fRange);					// 当たり判定

protected:

private:
	LPDIRECT3DTEXTURE9	*m_pTexture;	// テクスチャへのポインタ
	LPD3DXMESH			m_pMesh;		// メッシュ情報へのポインタ
	LPD3DXBUFFER		m_pBuffMat;		// マテリアル情報へのポインタ
	DWORD				m_nNumMat;		// マテリアル情報の数

	D3DXVECTOR3			m_vtxMin, m_vtxMax;	// モデルの最小値、最大値

	D3DXMATRIX			m_mtxWorld;			// ワールドマトリックス
	D3DXMATRIX			*m_pMtxParent;			// 親のワールドマトリックス
	CModel				*m_pParent;			// 親のモデル
	D3DXVECTOR3			m_pos;				// 位置
	D3DXVECTOR3			m_rot;				// 回転
	int					m_nIdxModel;		// 使うモデル
};
#endif