//=============================================================================
//
// 3Dオブジェクト処理 [scene3D.h]
// Author : Ishida Takuto
//
//=============================================================================
#ifndef _SCENE3D_H_
#define _SCENE3D_H_

#include "scene.h"

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CScene3D : public CScene
{
public:
	CScene3D(int nPriority = 3, CScene::OBJTYPE objType = CScene::OBJTYPE_NONE);
	~CScene3D();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CScene3D *Create(void);

	D3DXVECTOR3 GetPos(void) { return m_pos; };
	void SetPos(D3DXVECTOR3 pos);

	D3DXVECTOR3 GetRot(void) { return m_rot; };
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; };

	D3DXVECTOR3 GetSize(void) { return m_size; };
	void SetSize(D3DXVECTOR3 size);

	float GetWidth(void) { return m_size.x; };
	void SetWidth(float fWidth);

	float GetHeight(void) { return m_size.y; };
	void SetHeight(float fHeight);

	float GetDepth(void) { return m_size.z; };
	void SetDepth(float fDepth);

	void SetColor(D3DXCOLOR col);
	void AddColor(D3DXCOLOR col);
	D3DXCOLOR GetColor(void) { return m_col; };

	void BindTexture(LPDIRECT3DTEXTURE9 pTexture);

	void SetTexPos(D3DXVECTOR2 *texPos);
	void MoveTexPos(D3DXVECTOR2 speed);

	void SetParent(D3DXMATRIX *pParent) { m_pMtxParent = pParent; };

	D3DBLENDOP GetBlendOp(void) { return m_blendOp; };
	void SetBlendOp(D3DBLENDOP blendOp) { m_blendOp = blendOp; };

	LPDIRECT3DVERTEXBUFFER9 GetVtxBuff(void) { return m_pVtxBuff; };
	void SetVtxBuff(LPDIRECT3DVERTEXBUFFER9 pVtxBuff) { m_pVtxBuff = pVtxBuff; };

	bool Collision(D3DXVECTOR3 pos, float fRadius);

	bool GetBoolLighting(void) { return m_bLighting; }
	void SetBoolLighting(bool bLighting) { m_bLighting = bLighting; }

	bool GetBoolZtest(void) { return m_bZtest; }
	void SetBoolZtest(bool bZtest) { m_bZtest = bZtest; }

	D3DXMATRIX GetWorldMatrix(void) { return m_mtxWorld; }

protected:

private:
	LPDIRECT3DTEXTURE9		m_pTexture;		// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// 頂点バッファへのポインタ
	D3DXMATRIX				m_mtxWorld;		// ワールドマトリックス
	D3DXMATRIX				*m_pMtxParent;	// 親のマトリックス

	D3DXVECTOR3				m_pos;			// ポリゴンの位置
	D3DXVECTOR3				m_rot;			// 回転
	D3DXVECTOR3				m_size;			// 頂点からの距離
	D3DXCOLOR				m_col;			// 色
	D3DBLENDOP				m_blendOp;		// ブレンドオペレーション
	bool					m_bLighting;	// ライティングを有効にするか
	bool					m_bZtest;		// Zテストをするかどうか
};

#endif