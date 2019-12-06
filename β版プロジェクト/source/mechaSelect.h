//=============================================================================
//
// メカ選択処理 [mechaSelect.h]
// Author : Ayaka Hakozaki
//
//=============================================================================
#ifndef _MECHASELECT_H_
#define _MECHASELECT_H_

#include "scene.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MECHASEL_PRIORITY				(7)		// 優先番号
#define MECHASEL_NUM_BUTTON		(4)		// ボタンの数
#define MECHASEL_NUM_UI					(5)		// UIの数

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
class CMechaSelect : public CScene
{
public:
	typedef enum
	{
		MECHATYPE_EMPTY = -1,			// 未選択
		MECHATYPE_ASSULT,				// 強襲型
		MECHATYPE_LIGHT,				// 軽量型
		MECHATYPE_HEAVY,				// 重量型
		MECHATYPE_SHOOTER,				// 支援型
		MECHATYPE_MAX					// 最大数
	} MECHATYPE;

	CMechaSelect(int nPriority = MECHASEL_PRIORITY, CScene::OBJTYPE objType = CScene::OBJTYPE_MENU);
	~CMechaSelect();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static MECHATYPE GetMechaType(void) { return m_mechaType; };
	void MechaSelectCreate(void);

	// 変数
	static MECHATYPE m_mechaType;			// タイプ分け変数

private:
	CBg							*m_pBg;																// 背景のポインタ変数
	CButton2D				*m_apButtonUI[MECHASEL_NUM_BUTTON];	// ボタンクラスのポインタ変数
	CButton2D				*m_pButtonUIback;												// backボタンクラスのポインタ変数
	CMouseCursor2D	*m_pCursor;														// カーソルクラスのポインタ変数
	CScene2D				*m_apUI[MECHASEL_NUM_UI];						// UIのポリゴン
};

#endif