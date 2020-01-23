//=============================================================================
//
// 背景処理 [bg.h]
// Author : Takuto Ishida
//
//=============================================================================
#ifndef _BG_H_
#define _BG_H_

#include "scene2D.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define BG_PRIORITY	(1)		// 処理の優先番号
#define MAX_BG		(2)		// 背景の最大数

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CScene2D;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CBg : public CScene
{
public:
	CBg(int nPriority = BG_PRIORITY, CScene::OBJTYPE objType = CScene::OBJTYPE_BG);
	~CBg();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CBg* Create(void);

	D3DXCOLOR GetColor(void) { return m_col; }
	void SetColor(D3DXCOLOR col);

private:
	CScene2D **m_pPolygon;	// 背景のポリゴン
	D3DXCOLOR	m_col;		// 色
};

#endif
