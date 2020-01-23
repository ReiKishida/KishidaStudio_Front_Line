//=============================================================================
//
// �w�i���� [bg.h]
// Author : Takuto Ishida
//
//=============================================================================
#ifndef _BG_H_
#define _BG_H_

#include "scene2D.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define BG_PRIORITY	(1)		// �����̗D��ԍ�
#define MAX_BG		(2)		// �w�i�̍ő吔

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CScene2D;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CBg : public CScene
{
public:
	CBg(int nPriority = BG_PRIORITY, CScene::OBJTYPE objType = CScene::OBJTYPE_BG);
	~CBg();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CBg* Create(void);

	D3DXCOLOR GetColor(void) { return m_col; }
	void SetColor(D3DXCOLOR col);

private:
	CScene2D **m_pPolygon;	// �w�i�̃|���S��
	D3DXCOLOR	m_col;		// �F
};

#endif
