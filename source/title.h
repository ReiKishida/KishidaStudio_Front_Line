//=============================================================================
//
// タイトル処理 [title.h]
// Author : Takuto Ishida
//
//=============================================================================
#ifndef _TITLE_H_
#define _TITLE_H_

#include "scene.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TITLE_PRIORITY		(7)				// 優先番号
#define TITLE_UI					(2)				// UIの個数

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CBg;
class CUI_TEXTURE;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CTitle : public CScene
{
public:
	CTitle(int nPriority = TITLE_PRIORITY, CScene::OBJTYPE objType = CScene::OBJTYPE_TITLE);
	~CTitle();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	int m_nFlash;	// 点滅用
	CBg *m_pBg;	// 背景のポインタ変数
	static CUI_TEXTURE *m_pUI[TITLE_UI];
};

#endif