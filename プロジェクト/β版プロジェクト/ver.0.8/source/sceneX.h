//=============================================================================
//
// 3Dモデルオブジェクト処理 [sceneX.h]
// Author : Ishida Takuto
//
//=============================================================================
#ifndef _SCENEX_H_
#define _SCENEX_H_

#include "scene.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// 前方宣言
//*****************************************************************************

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CSceneX : public CScene
{
public:
	CSceneX(int nPriority = 4, CScene::OBJTYPE objType = CScene::OBJTYPE_NONE);
	~CSceneX();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	const void BindInfo(LPDIRECT3DTEXTURE9 pTexture, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat);	// モデル情報の受け渡し

	D3DXVECTOR3 GetPos(void) { return m_pos; };
	const void SetPos(D3DXVECTOR3 pos) { m_pos = pos; };

	const void SetColor(D3DXCOLOR col);
	const void AddColor(D3DXCOLOR col);

	D3DXVECTOR3 GetMaxVtx(void) { return m_vtxMax; };
	D3DXVECTOR3 GetMinVtx(void) { return m_vtxMin; };

	D3DXVECTOR3 GetRot(void) { return m_rot; };
	const void SetRot(D3DXVECTOR3 rot) { m_rot = rot; };

	int CollisionBox(D3DXVECTOR3 *pos, D3DXVECTOR3 posOld, D3DXVECTOR3 rot);

	D3DXMATRIX GetMtxWorld(void) { return m_mtxWorld; };
	virtual D3DXMATRIX SetWorldMatrix(D3DXMATRIX *m_mtxWorld);

	LPD3DXMESH GetMesh(void) { return m_pMesh; };

protected:

private:
	LPDIRECT3DTEXTURE9			m_pTexture;			// テクスチャへのポインタ
	LPD3DXMESH					m_pMesh;			// メッシュ情報へのポインタ
	LPD3DXBUFFER				m_pBuffMat;			// マテリアル情報へのポインタ
	DWORD						m_nNumMat;			// マテリアル情報の数

	D3DXMATERIAL				*m_pMat;			// マテリアルデータへのポインタ

	D3DXMATRIX					m_mtxWorld;			// ワールドマトリックス
	D3DXVECTOR3					m_vtxMin, m_vtxMax;	// モデルの最小値、最大値

	D3DXVECTOR3					m_pos;				// 位置
	D3DXVECTOR3					m_rot;				// 回転
};

#endif