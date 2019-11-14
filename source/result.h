//=============================================================================
//
// リザルト処理 [result.h]
// Author : Takuto Ishida
//
//=============================================================================
#ifndef _RESULT_H_
#define _RESULT_H_

#include "scene.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CNumber;
class CLogo;
class CLogoPressButton;
class CScore;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CResult : public CScene
{
public:
	CResult();
	~CResult();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static void SetScore(int nScore) { m_nScore = nScore; };

private:
	static int	m_nScore;		// スコア
	int			m_nDigit;		// 桁数
	int			m_nCntResult;	// リザルトの時間
	int			m_nFlash;		// 点滅させる速度

	CNumber				**m_pNumber;			// 数字クラスのポインタ変数
	CLogo				*m_pLogo;				// ロゴクラスのポインタ変数
	CLogoPressButton	*m_pPressButton;		// 入力待ち表示クラスのポインタ変数
	CScore				*m_pScore;				// スコアクラスのポインタ変数
};

#endif