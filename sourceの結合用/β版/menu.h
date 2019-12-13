//=============================================================================
//
// メニュー処理 [menu.h]
// Author : Takuto Ishida
//
//=============================================================================
#ifndef _MENU_H_
#define _MENU_H_

#include "scene.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MENU_PRIORITY				(7)		// 優先番号
#define MENU_NUM_BUTTON		(5)		// ボタンの数
#define MENU_NUM_UI					(2)		// UIの数
#define TEX_MENU_FLAME			(5)		// メニューテクスチャ最大数
#define TEX_MENU_LOGO				(4)		// メニューロゴの数
#define TEX_MENU_MESS				(5)		// メッセージの数

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CBg;
class CButton2D;
class CMouseCursor2D;
class CScene2D;
class CUI_TEXTURE;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CMenu : public CScene
{
public:
	typedef enum
	{// モードの種類
		MODE_SINGLE = 0,
		MODE_MULTI,
		MODE_TUTORIAL,
		MODE_QUIT,
		MODE_HELP,
		MODE_MAX
	}MODE;

	CMenu(int nPriority = MENU_PRIORITY, CScene::OBJTYPE objType = CScene::OBJTYPE_MENU);
	~CMenu();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static MODE GetMode(void) { return m_mode; };

	void MenuCreate(void);		// ロゴなどの生成

private:
	CBg							*m_pBg;														// 背景のポインタ変数
	CButton2D				*m_apButtonUI[MENU_NUM_BUTTON];	// ボタンクラスのポインタ変数
	CMouseCursor2D	*m_pCursor;												// カーソルクラスのポインタ変数
	CScene2D				*m_apUI[MENU_NUM_UI];							// UIのポリゴン
	CUI_TEXTURE			*m_apUITexMess;										// メッセージUITex
	static MODE			m_mode;														// モード情報
	int							m_nCntBgMove;											// スクロールカウンター
};

#endif