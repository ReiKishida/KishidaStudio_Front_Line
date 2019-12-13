//=============================================================================
//
// �w���v���� [help.h]
// Author : Ayaka Hakozaki
//
//=============================================================================
#ifndef _HELP_H_
#define _HELP_H_

#include "scene.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define HELP_PRIORITY				(7)		// �D��ԍ�
#define HELP_NUM_BUTTON		(4)		// �{�^���̐�
#define HELP_NUM_UI					(3)		// UI�̐�
#define HELP_INST						(2)		// �������

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CBg;
class CButton2D;
class CMouseCursor2D;
class CScene2D;

//*****************************************************************************
// �N���X��`
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
	CBg							*m_pBg;														// �w�i�̃|�C���^�ϐ�
	CButton2D				*m_apButtonUI[HELP_NUM_BUTTON];	// �{�^���N���X�̃|�C���^�ϐ�
	CButton2D				*m_pButtonUIback;										// back�{�^���N���X�̃|�C���^�ϐ�
	CMouseCursor2D	*m_pCursor;												// �J�[�\���N���X�̃|�C���^�ϐ�
	CScene2D				*m_apUI[HELP_NUM_UI];							// UI�̃|���S��
	CScene2D				*m_apUIInst[HELP_INST];							//������� UI�̃|���S��
};

#endif