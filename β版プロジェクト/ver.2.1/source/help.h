//=============================================================================
//
// ヘルプ処理 [help.h]
// Author : Ayaka Hakozaki
//
//=============================================================================
#ifndef _HELP_H_
#define _HELP_H_

#include "scene.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define HELP_PRIORITY				(7)		// 優先番号
#define HELP_NUM_BUTTON		(4)		// ボタンの数
#define HELP_NUM_UI					(3)		// UIの数
#define HELP_INST						(2)		// 操作説明

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CBg;
class CButton2D;
class CMouseCursor2D;
class CScene2D;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CHelp : public CScene
{
public:
	CHelp(int nPriority = HELP_PRIORITY, CScene::OBJTYPE objType = CScene::OBJTYPE_HELP);
	~CHelp();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void HelpCreate(void);

private:
	CBg							*m_pBg;														// 背景のポインタ変数
	CButton2D				*m_apButtonUI[HELP_NUM_BUTTON];	// ボタンクラスのポインタ変数
	CButton2D				*m_pButtonUIback;										// backボタンクラスのポインタ変数
	CMouseCursor2D	*m_pCursor;												// カーソルクラスのポインタ変数
	CScene2D				*m_apUI[HELP_NUM_UI];							// UIのポリゴン
	CScene2D				*m_apUIInst[HELP_INST];							//操作説明 UIのポリゴン
};

#endif