//=============================================================================
//
// �}�l�[�W���[���� [manager.h]
// Author : Takuto Ishida
//
//=============================================================================
#ifndef _MANAGER_H_
#define _MANAGER_H_

#include "main.h"

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CInput;
class CRenderer;
class CInputKeyboard;
class CInputMouse;
class CDirectInput;
class CXInput;
class CCamera;
class CLight;
class CDebugProc;
class CSound;

class CTitle;
class CMenu;
class CMechaSelect;
class CMatching;
class CTutorial;
class CGame;
class CResult;
class CRanking;

//=====================================
// �N���X��`
//=====================================
class CManager
{
public:
	typedef enum
	{// �t���[�����[�N
		MODE_TITLE = 0,
		MODE_MENU,
		MODE_MECHASELECT,
		MODE_MATCHING,
		MODE_TUTORIAL,
		MODE_GAME,
		MODE_RESULT,
		MODE_MAX
	}MODE;

	CManager();
	~CManager();
	HRESULT Init(HINSTANCE hInstance, HWND hWnd, bool bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	const static void SetMode(MODE mode);
	static MODE GetMode(void) { return m_mode; };

	static CRenderer *GetRenderer(void) { return m_pRenderer; };					// �����_���[�̎擾
	static CInputKeyboard *GetInputKeyboard(void) { return m_pInputKeyboard; };		// �C���v�b�g(�L�[�{�[�h)�̎擾
	static CInputMouse *GetInputMouse(void) { return m_pInputMouse; };				// �C���v�b�g(�}�E�X)�̎擾
	static CDirectInput *GetDirectInput(void) { return m_pDirectInput; };				// �C���v�b�g(DirectInput)�̎擾
	static CXInput *GetXInput(void) { return m_pXInput; };							// �C���v�b�g(XInput)�̎擾
	static CSound *GetSound(void) { return m_pSound; };								// �T�E���h�̎擾
	static CCamera *GetCamera(void) { return m_pCamera; };							// �J�����̎擾
	static CLight *GetLight(void) { return m_pLight; };								// ���C�g�̎擾
	static CDebugProc *GetDebugProc(void) { return m_pDebugProc; };					// �f�o�b�O�����̎擾

	static CGame *GetGame(void) { return m_pGame; };

	static int GetNumStage(void) { return m_nNumStage; };

private:
	static CRenderer		*m_pRenderer;			// �����_�����O�N���X�̃|�C���^�ϐ�
	static CInputKeyboard	*m_pInputKeyboard;		// �C���v�b�g�N���X�̃|�C���^�ϐ�
	static CInputMouse		*m_pInputMouse;			// �}�E�X�N���X�̃|�C���^�ϐ�
	static CDirectInput		*m_pDirectInput;		// DirectInput�N���X�^�̃|�C���^�ϐ�
	static CXInput			*m_pXInput;				// XInput�N���X�^�̃|�C���^�ϐ�
	static CCamera			*m_pCamera;				// �J�����N���X�̃|�C���^�ϐ�
	static CLight			*m_pLight;				// ���C�g�N���X�̃|�C���^�ϐ�
	static CDebugProc		*m_pDebugProc;			// �f�o�b�O�����N���X�̃|�C���^�ϐ�
	static CSound			*m_pSound;				// �T�E���h�N���X�̃|�C���^�ϐ�

	static CTitle			*m_pTitle;				// �^�C�g���N���X�̃|�C���^�ϐ�
	static CMenu			*m_pMenu;				// ���j���[�N���X�̃|�C���^�ϐ�
	static CMechaSelect *m_pMechaSel;		// ���J�I���N���X�̃|�C���^�ϐ�
	static CMatching			*m_pMatching;	// �}�b�`���O�N���X�̃|�C���^�ϐ�
	static CTutorial		*m_pTutorial;			// �`���[�g���A���N���X�̃|�C���^�ϐ�
	static CGame			*m_pGame;				// �Q�[���N���X�̃|�C���^�ϐ�
	static CResult			*m_pResult;				// ���U���g�N���X�̃|�C���^�ϐ�
	static CRanking			*m_pRanking;			// �����L���O�N���X�̃|�C���^�ϐ�
	static MODE				m_mode;					// ���݂̃t���[�����[�N

	static int				m_nNumStage;			// �X�e�[�W��
	bool					m_bDebugProc;			// �f�o�b�O������\�������邩�ǂ���
};

#endif