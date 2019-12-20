//=============================================================================
//
// インデックス3Dオブジェクト処理 [scene3DIndex.h]
// Author : Takuto Ishida
//
//=============================================================================
#ifndef _SCENE3DINDEX_H_
#define _SCENE3DINDEX_H_

#include "scene.h"

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CScene3DIndex : public CScene
{
public:
	typedef enum
	{
		TYPE_FIELD = 0,
		TYPE_CYLINDER,
		TYPE_MAX
	}TYPE;

	CScene3DIndex(int nPriority = 3, OBJTYPE objType = OBJTYPE_FIELD);
	~CScene3DIndex();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void MakeVertex(void);				// 頂点の作成
	void SetVtx(void);					// 頂点情報の設定
	void SetNor(VERTEX_3D *pVtx);		// 法線の計算
	void SetInfo(float fWidth, float fHeight, float fDepth, int nMeshX, int nMeshY, int nMeshZ);
	void GetInfo(float *pWidth, float *pHeight, float *pDepth, int *pMeshX, int *pMeshY, int *pMeshZ);

	D3DXVECTOR3 GetPos(void) { return m_pos; };
	const void SetPos(D3DXVECTOR3 pos) { m_pos = pos; };

	D3DXVECTOR3 GetRot(void) { return m_rot; };
	const void SetRot(D3DXVECTOR3 rot) { m_rot = rot; };

	void BindTexture(LPDIRECT3DTEXTURE9 pTexture);

	// 頂点バッファ
	LPDIRECT3DVERTEXBUFFER9 GetVtxBuff(void) { return m_pVtxBuff; };
	const void SetVtxBuff(LPDIRECT3DVERTEXBUFFER9 vtxBuff) { m_pVtxBuff = vtxBuff; };

	// インデックスバッファ
	LPDIRECT3DINDEXBUFFER9 GetIdxBuff(void) { return m_pIdxBuff; };
	const void SetIdxBuff(LPDIRECT3DINDEXBUFFER9 IdxBuff) { m_pIdxBuff = IdxBuff; };

	int GetNumVtx(void) { return m_nNumVtx; };
	int GetNumPolygon(void) { return m_nNumPolygon; };

	TYPE GetType(void) { return m_type; };
	void SetType(TYPE type) { m_type = type; };

	D3DXMATRIX GetMtxWorld(void) { return m_mtxWorld; };
	void SetMtxWorld(D3DXMATRIX mtxWorld) { m_mtxWorld = mtxWorld; };

	int GetNumVtxX(void) { return m_nMeshX; };
	int GetNumVtxY(void) { return m_nMeshY; };
	int GetNumVtxZ(void) { return m_nMeshZ; };

protected:

private:
	LPDIRECT3DTEXTURE9		m_pTexture;				// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;				// 頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9	m_pIdxBuff;				// インデックスバッファへのポインタ
	D3DXMATRIX				m_mtxWorld;				// ワールドマトリックス

	D3DXVECTOR3				m_pos;					// ポリゴンの位置
	D3DXVECTOR3				m_rot;					// 回転
	int						m_nNumVtx;				// 頂点数
	int						m_nNumPolygon;			// ポリゴン数
	TYPE					m_type;					// 自分の種類
	float					m_fLengthX;				// 頂点のX軸の距離
	float					m_fLengthY;				// 頂点のY軸の距離
	float					m_fLengthZ;				// 頂点のZ軸の距離
	int						m_nMeshX;				// X軸の分割数
	int						m_nMeshY;				// Y軸の分割数
	int						m_nMeshZ;				// Z軸の分割数
};

#endif