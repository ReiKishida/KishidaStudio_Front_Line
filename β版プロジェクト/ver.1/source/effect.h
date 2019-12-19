//=============================================================================
//
// エフェクト処理 [effect.h]
// Author : Ishida Takuto
//
//=============================================================================
#ifndef _EFFECT_H_
#define _EFFECT_H_

#include "scene3DBill.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define EFFECT_PRIORITY		(6)		// エフェクトの優先順位
#define EFFECT_NUMTEXTURE	(2)		// テクスチャの数

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CEffect : public CScene3DBill
{
public:
	CEffect(int nPriority = EFFECT_PRIORITY, CScene::OBJTYPE objType = CScene::OBJTYPE_EFFECT);
	virtual ~CEffect();
	virtual HRESULT Init(D3DXVECTOR3 pos, D3DXCOLOR col, float fRange, int nLife);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

	static CEffect* Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fRange, int nLife);

private:
	int m_nLife;
};

#endif