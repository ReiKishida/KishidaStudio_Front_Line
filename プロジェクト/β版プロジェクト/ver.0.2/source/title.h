//=============================================================================
//
// �^�C�g������ [title.h]
// Author : Takuto Ishida
//
//=============================================================================
#ifndef _TITLE_H_
#define _TITLE_H_

#include "scene.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TITLE_PRIORITY		(7)				// �D��ԍ�
#define TITLE_UI					(2)				// UI�̌�

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CBg;
class CUI_TEXTURE;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CTitle : public CScene
{
public:
	CTitle(int nPriority = TITLE_PRIORITY, CScene::OBJTYPE objType = CScene::OBJTYPE_TITLE);
	~CTitle();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	int m_nFlash;	// �_�ŗp
	CBg *m_pBg;	// �w�i�̃|�C���^�ϐ�
	static CUI_TEXTURE *m_pUI[TITLE_UI];
};

#endif