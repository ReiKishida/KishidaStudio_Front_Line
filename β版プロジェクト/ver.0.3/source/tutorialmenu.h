//=============================================================================
//
// チュートリアルメニュー処理 [tutorialmenu.h]
// Author : Takuto Ishida & Ayaka Hakozaki &Rei Kishida
//
//=============================================================================
#ifndef _TUTORIALMENU_H_
#define _TUTORIALMENU_H_

#include "scene.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TUTORIALMENU_PRIORITY				(7)		// 優先番号
#define TUTORIALMENU_NUM_BUTTON				(3)		// ボタンの数
#define TUTORIALMENU_NUM_UI					(2)		// UIの数
#define TUTO_TEX_MENU_FLAME			(3)		// メニューテクスチャ最大数
#define TUTO_TEX_MENU_LOGO				(4)		// メニューロゴの数
#define TUTO_TEX_MENU_MESS				(4)		// メッセージの数

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
class CTutorialMenu : public CScene
{
public:
	typedef enum
	{//チュートリアルタイプ
		TYPE_NONE = -1,
		TYPE_MOVEMENT,
		TYPE_ACTION,
		TYPE_STRATEGY,
		TYPE_MAX
	}TYPE;

	CTutorialMenu(int nPriority = TUTORIALMENU_PRIORITY, CScene::OBJTYPE objType = CScene::OBJTYPE_MENU);
	~CTutorialMenu();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static TYPE GetMode(void) { return m_Type; };

private:
	CBg						*m_pBg;									// 背景のポインタ変数
	CButton2D				*m_apButtonUI[TUTORIALMENU_NUM_BUTTON];	// ボタンクラスのポインタ変数
	CButton2D				*m_pBackButton;							// 戻るボタン
	CMouseCursor2D			*m_pCursor;								// カーソルクラスのポインタ変数
	CScene2D				*m_apUI[TUTORIALMENU_NUM_UI];			// UIのポリゴン
	CUI_TEXTURE				*m_apUITexMess;							// メッセージUITex
	static TYPE				m_Type;									// モード情報
	int						m_nCntBgMove;							// スクロールカウンター
};

#endif