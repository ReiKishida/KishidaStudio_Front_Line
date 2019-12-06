//=============================================================================
//
// 数字処理 [number.h]
// Author : Takuto Ishida
//
//=============================================================================
#ifndef _NUMBER_H_
#define _NUMBER_H_

#include "scene2D.h"
#include "UI_Number.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define NUMBER_PRIORITY		(6)		// 処理の優先順位

//*****************************************************************************
// 前方宣言
//*****************************************************************************

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CNumber : CScene2D
{
public:
	CNumber(int nPriority = NUMBER_PRIORITY, CScene::OBJTYPE objType = CScene::OBJTYPE_NUMBER);
	~CNumber();
	HRESULT Init(D3DXVECTOR3 pos, float fWidth, float fHeight);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CNumber* Create(D3DXVECTOR3 pos, float fWidth, float fHeight, CUI_NUMBER::UI_NUMTYPE UINumType,
		int nUV, int nUV_X, int nUV_Y);

	int GetNumber(void) { return m_nNumber; };
	const void SetNumber(int nNumber);
	const void SetTexNumber(int nNumber);

	D3DXVECTOR3 GetPos(void) { return CScene2D::GetPos(); };
	const void SetPos(D3DXVECTOR3 pos) { CScene2D::SetPos(pos); };

	D3DXCOLOR GetColor(void) { return CScene2D::GetColor(); };
	const void SetColor(D3DXCOLOR col) { CScene2D::SetColor(col); };

	void GetSize(float *pWidth, float *pHeight);
	const void SetSize(float fWidth, float fHeight) { CScene2D::SetSize(fWidth, fHeight); };

private:
	int	m_nNumber;	// 数字
	CUI_NUMBER *m_UINum;
	CUI_NUMBER::UI_NUMTYPE		m_TeamType;		// BLUEチームか、REDチームか
	int m_nUV, m_nUV_X, m_nUV_Y;
};
#endif