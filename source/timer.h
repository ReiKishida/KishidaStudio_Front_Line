//=============================================================================
//
// タイマー処理 [timer.h]
// Author : Takuto Ishida
//
//=============================================================================
#ifndef _TIMER_H_
#define _TIMER_H_

#include "scene.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TIMER_PRIORITY	(6)		// 処理の優先順位

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CNumber;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CTimer : public CScene
{
public:
	CTimer(int nPriority = TIMER_PRIORITY, CScene::OBJTYPE objType = CScene::OBJTYPE_TIME);
	~CTimer();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CTimer* Create(D3DXVECTOR3 pos, int nTime);

	static int GetTime(void) { return m_nTimer; };
	static void SetTime(int nTime) { m_nTimer = nTime; }

private:
	D3DXVECTOR3		m_pos;				// 位置
	CNumber			**m_pNumber;		// ナンバー変数へのポインタ
	static int		m_nTimer;			// 現在の時間
	int				m_nCntSecond;		// 秒の計算に必要
	int				m_nDigits;			// 桁数
};

#endif
