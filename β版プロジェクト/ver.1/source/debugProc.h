//=============================================================================
//
// �f�o�b�O���\������ [debugProc.h]
// Author : Takuto Ishida
//
//=============================================================================
#ifndef _DEBUGPROC_H_
#define _DEBUGPROC_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_DEBUG_TEXT	(2048)		// �f�o�b�O���̍ő啶����

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CDebugProc
{// �f�o�b�O���
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
	static LPD3DXFONT m_pFont;	// ������\������̂ɕK�v
	static char m_aStr[MAX_DEBUG_TEXT];		// �\���ł��镶����
	static bool m_bDisp;					// �\�����邩�ǂ���
};

#endif
