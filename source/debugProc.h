//=============================================================================
//
// デバッグ情報表示処理 [debugProc.h]
// Author : Takuto Ishida
//
//=============================================================================
#ifndef _DEBUGPROC_H_
#define _DEBUGPROC_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_DEBUG_TEXT	(2048)		// デバッグ情報の最大文字数
#define MAX_FONT		(2)			// フォントの最大数

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CDebugProc
{// デバッグ情報
public:
	CDebugProc();
	~CDebugProc();
	void Init(void);
	void Uninit(void);

	static void Print(char *fmt, ...);
	static void Draw(void);
	void DebugDisp(bool bDisp);
	static bool GetDisp(void) { return m_bDisp; };

private:
	static LPD3DXFONT m_pFont[MAX_FONT];	// 文字を表示するのに必要
	static char m_aStr[MAX_DEBUG_TEXT];		// 表示できる文字数
	static bool m_bDisp;					// 表示するかどうか
};

#endif
