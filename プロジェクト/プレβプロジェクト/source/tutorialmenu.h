//=============================================================================
//
// �`���[�g���A�����j���[���� [tutorialmenu.h]
// Author : Takuto Ishida & Ayaka Hakozaki &Rei Kishida
//
//=============================================================================
#ifndef _TUTORIALMENU_H_
#define _TUTORIALMENU_H_

#include "scene.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TUTORIALMENU_PRIORITY				(7)		// �D��ԍ�
#define TUTORIALMENU_NUM_BUTTON				(3)		// �{�^���̐�
#define TUTORIALMENU_NUM_UI					(2)		// UI�̐�
#define TUTO_TEX_MENU_FLAME			(3)		// ���j���[�e�N�X�`���ő吔
#define TUTO_TEX_MENU_LOGO				(4)		// ���j���[���S�̐�
#define TUTO_TEX_MENU_MESS				(4)		// ���b�Z�[�W�̐�

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
class CTutorialMenu : public CScene
{
public:
	typedef enum
	{//�`���[�g���A���^�C�v
		TYPE_NONE = -1,
		TYPE_MOVEMENT,
		TYPE_ACTION,
		TYPE_STRATEGY,
		TYPE_MAX
	}TYPE;

	CTutorialMenu(int nPriority = TUTORIALMENU_PRIORITY, CScene::OBJTYPE objType = CScene::OBJTYPE_MENU);
	~CTutorialMenu();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static TYPE GetMode(void) { return m_Type; };

private:
	CBg						*m_pBg;									// �w�i�̃|�C���^�ϐ�
	CButton2D				*m_apButtonUI[TUTORIALMENU_NUM_BUTTON];	// �{�^���N���X�̃|�C���^�ϐ�
	CButton2D				*m_pBackButton;							// �߂�{�^��
	CMouseCursor2D			*m_pCursor;								// �J�[�\���N���X�̃|�C���^�ϐ�
	CScene2D				*m_apUI[TUTORIALMENU_NUM_UI];			// UI�̃|���S��
	CUI_TEXTURE				*m_apUITexMess;							// ���b�Z�[�WUITex
	static TYPE				m_Type;									// ���[�h���
	int						m_nCntBgMove;							// �X�N���[���J�E���^�[
};

#endif