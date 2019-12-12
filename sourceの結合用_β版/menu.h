//=============================================================================
//
// ���j���[���� [menu.h]
// Author : Takuto Ishida
//
//=============================================================================
#ifndef _MENU_H_
#define _MENU_H_

#include "scene.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MENU_PRIORITY				(7)		// �D��ԍ�
#define MENU_NUM_BUTTON		(5)		// �{�^���̐�
#define MENU_NUM_UI					(2)		// UI�̐�
#define TEX_MENU_FLAME			(5)		// ���j���[�e�N�X�`���ő吔
#define TEX_MENU_LOGO				(4)		// ���j���[���S�̐�
#define TEX_MENU_MESS				(5)		// ���b�Z�[�W�̐�

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CBg;
class CButton2D;
class CMouseCursor2D;
class CScene2D;
class CUI_TEXTURE;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CMenu : public CScene
{
public:
	typedef enum
	{// ���[�h�̎��
		MODE_SINGLE = 0,
		MODE_MULTI,
		MODE_TUTORIAL,
		MODE_QUIT,
		MODE_HELP,
		MODE_MAX
	}MODE;

	CMenu(int nPriority = MENU_PRIORITY, CScene::OBJTYPE objType = CScene::OBJTYPE_MENU);
	~CMenu();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static MODE GetMode(void) { return m_mode; };

	void MenuCreate(void);		// ���S�Ȃǂ̐���

private:
	CBg							*m_pBg;														// �w�i�̃|�C���^�ϐ�
	CButton2D				*m_apButtonUI[MENU_NUM_BUTTON];	// �{�^���N���X�̃|�C���^�ϐ�
	CMouseCursor2D	*m_pCursor;												// �J�[�\���N���X�̃|�C���^�ϐ�
	CScene2D				*m_apUI[MENU_NUM_UI];							// UI�̃|���S��
	CUI_TEXTURE			*m_apUITexMess;										// ���b�Z�[�WUITex
	static MODE			m_mode;														// ���[�h���
	int							m_nCntBgMove;											// �X�N���[���J�E���^�[
};

#endif