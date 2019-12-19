//=============================================================================
//
// 影処理 [shadow.h]
// Author : Takuto Ishida
//
//=============================================================================
#ifndef _SHADOW_H_
#define _SHADOW_H_

#include "scene.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define SHADOW_PRIORITY		(6)

//*****************************************************************************
// 前方宣言
//*****************************************************************************

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CShadow : public CScene
{
public:
	CShadow(int nPriority = SHADOW_PRIORITY, CScene::OBJTYPE objType = CScene::OBJTYPE_SHADOW);
	~CShadow();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CShadow *Create(CScene *pParent);

	void Set(D3DXMATRIX mtxWorld, LPD3DXMESH mesh, int nNumMat, D3DXVECTOR3 _pos, float fHeight = 1.0f);

	static bool RangeShadow(D3DXVECTOR3 pos, CScene *pObject, D3DXVECTOR3 vtxDiff);

private:
	D3DXMATRIX	m_mtxShadow;		// シャドウマトリックス
	D3DXPLANE	m_planeField;		// 地面の平面
	CScene		*m_pParent;			// オブジェクトのポインタ
	D3DXVECTOR3	m_ObjPos;			// オブジェクトの位置
};

#endif
