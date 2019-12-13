//=============================================================================
//
// 軌跡処理 [meshOrbit.h]
// Author : Takuto Ishida
//
//=============================================================================
#ifndef _MESHORBIT_H_
#define _MESHORBIT_H_

#include "scene.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define FIELD_TEXTURE	(1)		// テクスチャ数
#define MAX_ORBIT_VERTEX	(128)		// テクスチャ数

//*****************************************************************************
// 前方宣言
//*****************************************************************************

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CMeshOrbit : public CScene
{
public:
	CMeshOrbit(int nPriority = 5, CScene::OBJTYPE objType = CScene::OBJTYPE_ORBIT);
	~CMeshOrbit();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CMeshOrbit* Create(D3DXVECTOR3 offset, int nLife, D3DXMATRIX *pMtx);

	static HRESULT Load(void);				// テクスチャを読み込む
	static void Unload(void);				// 読み込んだテクスチャを解放

private:
	static LPDIRECT3DTEXTURE9 m_pTexture[FIELD_TEXTURE];	// 共有テクスチャへのポインタ

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;				// 頂点バッファへのポインタ
	D3DXMATRIX				m_mtxWorld;				// ワールドマトリックス
	D3DXMATRIX				*m_pMtxParent;			// 親のマトリックスへのポインタ
	D3DXVECTOR3				m_aPosVertex[MAX_ORBIT_VERTEX];		// 算出した頂点座標
	D3DXVECTOR3				m_aOffset[2];						// 両端のオフセット

	D3DXCOLOR				m_col;					// 色
	int						m_nLife;				// 寿命
};

#endif
