//=============================================================================
//
// ���͂̏���[input.cpp]
// Author : Takuto Ishida
//
//=============================================================================
#include "input.h"
#include "manager.h"
#include "renderer.h"
#include "game.h"
#include "tutorial.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define REPEAT_FRAME	(20)	// ���s�[�g�����N�����n�߂�t���[�����̍��v
#define REPEAT_TRIGGER	(5)		// ���s�[�g�����N������t���[���̊Ԋu

//*****************************************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************************************
LPDIRECTINPUT8	CInput::m_pInput = NULL;		// DirectInput�I�u�W�F�N�g�ւ̃|�C���^
HWND			CInput::m_hWnd = 0;				// �E�B���h�E�n���h��

 //*****************************************************************************
 // CInput�̏���
 //*****************************************************************************
 //=============================================================================
 // �R���X�g���N�^
 //=============================================================================
CInput::CInput(int nNumDevice)
{
	m_pDevice = NULL;           // ���̓f�o�C�X�ւ̃|�C���^
	m_nDeviceNum = nNumDevice;  // ���̓f�o�C�X�̐�����
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CInput::~CInput()
{
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CInput::Init(HINSTANCE hInstance, HWND hWnd)
{
	// DirectInput�I�u�W�F�N�g�̐���
	if (m_pInput == NULL)
	{// �܂�����Ă��Ȃ�
		if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInput, NULL)))
		{
			return E_FAIL;
		}
		m_hWnd = hWnd;
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CInput::Uninit(void)
{
	// ���̓f�o�C�X�̊J��
	if (m_pDevice != NULL)
	{
		for (int nCntDevice = 0; nCntDevice < m_nDeviceNum; nCntDevice++)
		{// �f�o�C�X���m�ۂ����������J��Ԃ�
			if (m_pDevice[nCntDevice] != NULL)
			{// �f�o�C�X���m�ۂ���Ă���
				m_pDevice[nCntDevice]->Unacquire();   // �f�o�C�X�ւ̃A�N�Z�X�������
				m_pDevice[nCntDevice]->Release();
				m_pDevice[nCntDevice] = NULL;
			}
		}
		// �������̊J��
		delete m_pDevice;
		m_pDevice = NULL;
	}

	// DirectInput�I�u�W�F�N�g�̊J��
	if (m_pInput != NULL)
	{
		m_pInput->Release();
		m_pInput = NULL;
	}
}

//*****************************************************************************
// CInputKeyboard�̏���
//*****************************************************************************
//=============================================================================
// �R���X�g���N�^
//=============================================================================
CInputKeyboard::CInputKeyboard(int nNumDevice) : CInput(nNumDevice)
{
	for (int nCntKeyboard = 0; nCntKeyboard < NUM_KEY_MAX; nCntKeyboard++)
	{// �L�[�{�[�h�̃L�[�̐������J��Ԃ�
		// �L�[�{�[�h�̏���������
		m_aKeyState[nCntKeyboard] = 0x00;			// �v���X���
		m_aKeyStateTrigger[nCntKeyboard] = 0x00;	// �g���K�[���
		m_aKeyStateRelease[nCntKeyboard] = 0x00;	// �����[�X���
		m_aKeyStateRepeat[nCntKeyboard] = 0x00;		// ���s�[�g���
		m_aKeyboardCounter[nCntKeyboard] = 0;		// ���̓J�E���^�[
	}
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CInputKeyboard::~CInputKeyboard()
{
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CInputKeyboard::Init(HINSTANCE hInstance, HWND hWnd)
{
	// ���̓I�u�W�F�N�g�̍쐬
	CInput::Init(hInstance, hWnd);

	// ���̓f�o�C�X�𓮓I�Ɋm�ۂ���
	m_pDevice = new LPDIRECTINPUTDEVICE8[m_nDeviceNum];
	for (int nCntDevice = 0; nCntDevice < m_nDeviceNum; nCntDevice++)
	{// ��������f�o�C�X�̐������J��Ԃ�
		m_pDevice[nCntDevice] = NULL;
	}

	// ���̓f�o�C�X����������Ă��Ȃ�
	for (int nCntDevice = 0; nCntDevice < m_nDeviceNum; nCntDevice++)
	{// �f�o�C�X�̐����������J��Ԃ�
		if (m_pDevice[nCntDevice] == NULL)
		{// ���������m�ۂł���
			// ���̓f�o�C�X(�L�[�{�[�h)�̐���
			if (FAILED(m_pInput->CreateDevice(GUID_SysKeyboard, &m_pDevice[nCntDevice], NULL)))
			{
				return E_FAIL;
			}

			// �f�[�^�t�H�[�}�b�g��ݒ�
			if (FAILED(m_pDevice[nCntDevice]->SetDataFormat(&c_dfDIKeyboard)))
			{
				return E_FAIL;
			}

			// �������[�h��ݒ�
			if (FAILED(m_pDevice[nCntDevice]->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
			{
				return E_FAIL;
			}

			// �L�[�{�[�h�ւ̃A�N�Z�X�����l��
			m_pDevice[nCntDevice]->Acquire();
		}
	}
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CInputKeyboard::Uninit(void)
{
	// Input�N���X�̊J��
	CInput::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CInputKeyboard::Update(void)
{
	BYTE aKeyState[NUM_KEY_MAX]; // �L�[�{�[�h�̓��͏��

	// ���̓f�o�C�X(�L�[�{�[�h)����f�[�^���擾
	for (int nCntDevice = 0; nCntDevice < m_nDeviceNum; nCntDevice++)
	{// �f�o�C�X�̐����������J��Ԃ�
		if (m_pDevice[nCntDevice] != NULL)
		{// �����ł���
			if (SUCCEEDED(m_pDevice[nCntDevice]->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
			{// �f�o�C�X�̓��͏�Ԃ��擾�ł���
				for (int nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
				{// �L�[�{�[�h�̃L�[�̐������J��Ԃ�
					m_aKeyStateTrigger[nCntKey] = (m_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & aKeyState[nCntKey];   // �L�[�{�[�h�̓��͏��(�g���K�[���)�ۑ�
					m_aKeyStateRelease[nCntKey] = (m_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & m_aKeyState[nCntKey]; // �L�[�{�[�h�̓��͏��(�����[�X���)�ۑ�
					if (aKeyState[nCntKey] == 0x80)
					{// �L�[�{�[�h�����͂���Ă���
						m_aKeyboardCounter[nCntKey]++;    // �J�E���^�[��i�߂�
						if (m_aKeyboardCounter[nCntKey] >= REPEAT_FRAME || 1 == m_aKeyboardCounter[nCntKey])
						{// 20�t���[�����L�[��������Ă���
							if (m_aKeyboardCounter[nCntKey] % REPEAT_TRIGGER == 0 || 1 == m_aKeyboardCounter[nCntKey])
							{// 3�t���[������
								m_aKeyStateRepeat[nCntKey] = 0x80;	// �L�[�{�[�h�̓��͏��(���s�[�g���)���N��
							}
							else
							{// ����ȊO�̃t���[��
								m_aKeyStateRepeat[nCntKey] = 0;		// �L�[�{�[�h�̓��͏��(���s�[�g���)���ꎞ��~
							}
						}
						else
						{// 20�t���[���ȑO
							m_aKeyStateRepeat[nCntKey] = 0;			// �L�[�{�[�h�̓��͏��(���s�[�g���)���ꎞ��~
						}
					}
					else
					{// �L�[�{�[�h�����͂���Ă��Ȃ�
						m_aKeyboardCounter[nCntKey] = 0;		// �J�E���^�[��߂�
						m_aKeyStateRepeat[nCntKey] = 0;			// �L�[�{�[�h�̓��͏��(���s�[�g���)���~
					}
					m_aKeyState[nCntKey] = aKeyState[nCntKey];	// �L�[�{�[�h�̓��͏��(�v���X���)�ۑ�
				}
			}
			else
			{// �f�o�C�X�̓��͏�Ԃ��擾�ł��Ȃ�����
				m_pDevice[nCntDevice]->Acquire();				// �L�[�{�[�h�ւ̃A�N�Z�X�����l��
			}
		}
	}
}

//=============================================================================
// �����ꂩ�̃L�[�̓��͏����擾
//=============================================================================
bool CInputKeyboard::GetAnyKey(void)
{
	bool bKey = false;

	for (int nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
	{//�L�[�{�[�h�̓��͏��ۑ�
		//�g���K�[(��񂾂�)
		bKey = (m_aKeyStateTrigger[nCntKey] & 0x80) ? true : false;

		if (bKey == true)
		{// ������Ă���L�[������΃u���C�N����
			break;
		}
	}

	return bKey;
}

/****************************************************************************/
/*							CInputMouse�̏���								*/
/****************************************************************************/
//=============================================================================
// �R���X�g���N�^
//=============================================================================
CInputMouse::CInputMouse(int nNumDevice) : CInput(nNumDevice)
{
	for (int nCntMouse = 0; nCntMouse < DIMS_BUTTON_MAX; nCntMouse++)
	{// �}�E�X�̃{�^���̐������J��Ԃ�
		// �}�E�X�̏���������
		m_aMouseState[nCntMouse] = 0x00;		// �v���X���
		m_aMouseStateTrigger[nCntMouse] = 0x00;	// �g���K�[���
		m_aMouseStateRelease[nCntMouse] = 0x00;	// �����[�X���
		m_aMouseStateRepeat[nCntMouse] = 0x00;	// ���s�[�g���
		m_aMouseCounter[nCntMouse] = 0;			// ���̓J�E���^�[
	}

	m_MouseXAxis = 0;	// �}�E�X��X��
	m_MouseYAxis = 0;	// �}�E�X��Y��
	m_MouseZAxis = 0;	// �}�E�X��Z��
	m_nDiffPointX = 0;	// �O��̍��W�Ƃ̍���
	m_nDiffPointY = 0;	// �O��̍��W�Ƃ̍���
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CInputMouse::~CInputMouse()
{
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CInputMouse::Init(HINSTANCE hInstance, HWND hWnd)
{
	// ���̓I�u�W�F�N�g�̍쐬
	CInput::Init(hInstance, hWnd);

	// ���̓f�o�C�X�𓮓I�Ɋm�ۂ���
	m_pDevice = new LPDIRECTINPUTDEVICE8[m_nDeviceNum];
	for (int nCntDevice = 0; nCntDevice < m_nDeviceNum; nCntDevice++)
	{// ��������f�o�C�X�̐������J��Ԃ�
		m_pDevice[nCntDevice] = NULL;
	}

	// ���̓f�o�C�X����������Ă��Ȃ�
	for (int nCntDevice = 0; nCntDevice < m_nDeviceNum; nCntDevice++)
	{// �f�o�C�X�̐����������J��Ԃ�
		if (m_pDevice[nCntDevice] == NULL)
		{// ���������m�ۂł���
			// ���̓f�o�C�X(�}�E�X)�̐���
			if (FAILED(m_pInput->CreateDevice(GUID_SysMouse, &m_pDevice[nCntDevice], NULL)))
			{
				return E_FAIL;
			}

			// �f�[�^�t�H�[�}�b�g��ݒ�
			if (FAILED(m_pDevice[nCntDevice]->SetDataFormat(&c_dfDIMouse2)))
			{
				return E_FAIL;
			}

			// �������[�h��ݒ�
			if (FAILED(m_pDevice[nCntDevice]->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
			{
				return E_FAIL;
			}

			// ���̐ݒ�
			DIPROPDWORD diprop;
			diprop.diph.dwSize = sizeof(diprop);
			diprop.diph.dwHeaderSize = sizeof(diprop.diph);
			diprop.diph.dwObj = 0;
			diprop.diph.dwHow = DIPH_DEVICE;
			diprop.dwData = DIPROPAXISMODE_REL;	// ���Βl���[�h�Őݒ�i��Βl��DIPROPAXISMODE_ABS�j
			m_pDevice[nCntDevice]->SetProperty(DIPROP_AXISMODE, &diprop.diph);

			// �E�B���h�E�n���h�����L�����Ă���
			m_hWnd = hWnd;

			// �}�E�X�ւ̃A�N�Z�X�����l��
			m_pDevice[nCntDevice]->Acquire();
		}
	}
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CInputMouse::Uninit(void)
{
	// Input�N���X�̊J��
	CInput::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CInputMouse::Update(void)
{
	DIMOUSESTATE2 aMouseState;	// �}�E�X�̓��͏��

	// ���̓f�o�C�X(�}�E�X)����f�[�^���擾
	for (int nCntDevice = 0; nCntDevice < m_nDeviceNum; nCntDevice++)
	{// �f�o�C�X�̐����������J��Ԃ�
		if (m_pDevice[nCntDevice] != NULL)
		{// �����ł���
			if (SUCCEEDED(m_pDevice[nCntDevice]->GetDeviceState(sizeof(DIMOUSESTATE2), &aMouseState)))
			{// �f�o�C�X�̓��͏�Ԃ��擾�ł���
				// ���̎擾
				m_MouseXAxis = aMouseState.lX;	// �}�E�X��X�����i�[
				m_MouseYAxis = aMouseState.lY;	// �}�E�X��Y�����i�[
				m_MouseZAxis = aMouseState.lZ;	// �}�E�X��Z�����i�[

				// �E�B���h�E�̋�`���擾
				RECT rect;
				GetWindowRect(m_hWnd, &rect);

				// �O��̃J�[�\�����W���L��
				m_PointOld = m_Point;

				// �J�[�\���ʒu�̐�΍��W���擾
				GetCursorPos(&m_Point);
				m_nDiffPointX = (int)m_Point.x - (int)m_PointOld.x;
				m_nDiffPointY = (int)m_Point.y - (int)m_PointOld.y;

				if (CManager::MODE_GAME == CManager::GetMode() && NULL != CManager::GetGame())
				{// �Q�[�����[�h
					if (CGame::PART_ACTION == CManager::GetGame()->GetPart())
					{// �A�N�V�����p�[�g
						if (CManager::GetRenderer()->GetWindowMode())
						{// �E�B���h�E���[�h
							CDebugProc::Print("rect:%.2f %.2f %.2f %.2f", rect.left, rect.right, rect.top, rect.bottom);
							// X���̐���
							if (rect.left + 50 > m_Point.x) { m_Point.x = rect.right - 50; }
							else if (rect.right - 50 < m_Point.x) { m_Point.x = rect.left + 50; }

							// Y���̐���
							if (rect.top + 50 > m_Point.y) { m_Point.y = rect.bottom - 50; }
							else if (rect.bottom - 50 < m_Point.y) { m_Point.y = rect.top + 50; }
						}
						else
						{// �t���X�N���[��
							if (1 > m_Point.x) { m_Point.x = 1278; }
							else if (1278 < m_Point.x) { m_Point.x = 1; }

							// Y���̐���
							if (1 > m_Point.y) { m_Point.y = 718; }
							else if (718 < m_Point.y) { m_Point.y = 1; }
						}
					}
				}
				else if (CManager::MODE_MENU == CManager::GetMode())
				{// ���j���[���[�h
					// X���̐���
					if (rect.left + (LONG)5.0f > m_Point.x) { m_Point.x = rect.left + (LONG)5.0f; }
					else if (rect.right - (LONG)40.0f < m_Point.x) { m_Point.x = rect.right - (LONG)40.0f; }

					// Y���̐���
					if (rect.top + (LONG)30.0f > m_Point.y) { m_Point.y = rect.top + (LONG)30.0f; }
					else if (rect.bottom - (LONG)40.0f < m_Point.y) { m_Point.y = rect.bottom - (LONG)40.0f; }
				}
				else if (CManager::MODE_TUTORIAL == CManager::GetMode() && NULL != CManager::GetTutorial())
				{// �Q�[�����[�h
					if (CTutorial::PART_ACTION == CManager::GetTutorial()->GetPart())
					{// �A�N�V�����p�[�g
						if (CManager::GetRenderer()->GetWindowMode())
						{// �E�B���h�E���[�h
							CDebugProc::Print("rect:%.2f %.2f %.2f %.2f", rect.left, rect.right, rect.top, rect.bottom);
							// X���̐���
							if (rect.left + 50 > m_Point.x) { m_Point.x = rect.right - 50; }
							else if (rect.right - 50 < m_Point.x) { m_Point.x = rect.left + 50; }

							// Y���̐���
							if (rect.top + 50 > m_Point.y) { m_Point.y = rect.bottom - 50; }
							else if (rect.bottom - 50 < m_Point.y) { m_Point.y = rect.top + 50; }
						}
						else
						{// �t���X�N���[��
							if (1 > m_Point.x) { m_Point.x = 1278; }
							else if (1278 < m_Point.x) { m_Point.x = 1; }

							// Y���̐���
							if (1 > m_Point.y) { m_Point.y = 718; }
							else if (718 < m_Point.y) { m_Point.y = 1; }
						}
					}
				}
				// ����ɃT�C�Y�ύX���l�����āA���݂̃T�C�Y�ŕ␳�i�g�T�C�Y������̂Ō����ł͂Ȃ��j
				m_Point.x = LONG((FLOAT)m_Point.x * ((FLOAT)rect.right / (FLOAT)rect.right));
				m_Point.y = LONG((FLOAT)m_Point.y * ((FLOAT)rect.bottom / (FLOAT)rect.bottom));

				// �J�[�\���ʒu�̐ݒ�
				SetCursorPos(m_Point.x, m_Point.y);

				for (int nCntMouse = 0; nCntMouse < DIMS_BUTTON_MAX; nCntMouse++)
				{// �}�E�X�̃{�^���̐������J��Ԃ�
					m_aMouseStateTrigger[nCntMouse] = (m_aMouseState[nCntMouse] ^ aMouseState.rgbButtons[nCntMouse]) & aMouseState.rgbButtons[nCntMouse];	// �}�E�X�̓��͏��(�g���K�[���)�ۑ�
					m_aMouseStateRelease[nCntMouse] = (m_aMouseState[nCntMouse] ^ aMouseState.rgbButtons[nCntMouse]) & m_aMouseState[nCntMouse];			// �}�E�X�̓��͏��(�����[�X���)�ۑ�
					if (aMouseState.rgbButtons[nCntMouse] == 0x80)
					{// �}�E�X�����͂���Ă���
						m_aMouseCounter[nCntMouse]++;    // �J�E���^�[��i�߂�
						if (m_aMouseCounter[nCntMouse] >= REPEAT_FRAME)
						{// ���t���[�����L�[��������Ă���
							if (m_aMouseCounter[nCntMouse] % REPEAT_TRIGGER == 0)
							{// ���t���[������
								m_aMouseStateRepeat[nCntMouse] = 0x80;	// �}�E�X�̓��͏��(���s�[�g���)���N��
							}
							else
							{// ����ȊO�̃t���[��
								m_aMouseStateRepeat[nCntMouse] = 0;		// �}�E�X�̓��͏��(���s�[�g���)���ꎞ��~
							}
						}
					}
					else
					{// ���͂���Ă��Ȃ�
						m_aMouseCounter[nCntMouse] = 0;			// �J�E���^�[��߂�
						m_aMouseStateRepeat[nCntMouse] = 0;		// �}�E�X�̓��͏��(���s�[�g���)���~
					}
					m_aMouseState[nCntMouse] = aMouseState.rgbButtons[nCntMouse];	// �}�E�X�̓��͏��(�v���X���)�ۑ�
				}
			}
			else
			{// �f�o�C�X�̓��͏�Ԃ��擾�ł��Ȃ�����
				m_pDevice[nCntDevice]->Acquire();	// �}�E�X�ւ̃A�N�Z�X�����l��
			}
		}
	}
}

//****************************************************************************
//
//							DirectInput�̏���
//****************************************************************************
CDirectInput::GamePad CDirectInput::m_aGamePad;
int		   	   CDirectInput::m_GamePadNumber = 0;				// joystick�p�R�[���o�b�N�֐��Ŏg���W���C�R���̔ԍ�
int		   	   CDirectInput::m_GamepadAxesNumber = 0;			// joystick�̋@�\�R�[���o�b�N�֐��Ŏg���W���C�R���̔ԍ�
DWORD			CDirectInput::m_NumForceFeedbackAxis = 0;
//==============================================================================================================================
// �R���X�g���N�^
//==============================================================================================================================
CDirectInput::CDirectInput()
{
}
//==============================================================================================================================
// �f�X�g���N�^
//==============================================================================================================================
CDirectInput::~CDirectInput()
{

}
//==============================================================================================================================
// �Q�[���p�b�h�̏�����
//==============================================================================================================================
HRESULT CDirectInput::Init(HINSTANCE hInstance, HWND hWnd)
{

	m_aGamePad.pDevGamePad = NULL;

	m_GamePadNumber = 0;
	m_GamepadAxesNumber = 0;

	HRESULT hr;
	// �I�u�W�F�N�g����
	CInput::Init(hInstance, hWnd);

	// ���̓f�o�C�X(�R���g���[���[)�̐���
	if (FAILED(m_pInput->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumJoysticksCallback, NULL, DIEDFL_ATTACHEDONLY)))		// ��4���� (ATTACHEDONLY = ���ݐڑ�����Ă���W���C�X�e�B�b�N�̂݌��o����)
	{
		return FALSE;
	}

	// �t�H�[�X�t�B�[�h�o�b�N�@�\�̂�����̓f�o�C�X�̐���
	if (FAILED(m_pInput->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumJoysticksCallback, NULL, DIEDFL_ATTACHEDONLY | DIEDFL_FORCEFEEDBACK)))
	{
		return E_FAIL;
	}

	if (m_aGamePad.pDevGamePad != NULL)
	{// �R���g���[���[���q�����Ă��Ȃ��Ƃ��������Ȃ�
	 // �f�o�C�X�Ƀf�[�^�t�H�[�}�b�g��ݒ�
		if (FAILED(hr = m_aGamePad.pDevGamePad->SetDataFormat(&c_dfDIJoystick2)))
		{
			return FALSE;
		}

		// �������[�h��ݒ�
		if (FAILED(hr = m_aGamePad.pDevGamePad->SetCooperativeLevel(hWnd, DISCL_EXCLUSIVE | DISCL_FOREGROUND)))
		{
			return FALSE;
		}

		// �Q�[���p�b�h�̋@�\�̏������W
		m_aGamePad.GamepadCaps.dwSize = sizeof(DIDEVCAPS);

		if (FAILED(hr = m_aGamePad.pDevGamePad->GetCapabilities(&m_aGamePad.GamepadCaps)))
		{
			m_aGamePad.pDevGamePad->Release();
			return FALSE;
		}

		m_NumForceFeedbackAxis = 0;

		// �e���̃��[�h�ݒ�
		hr = m_aGamePad.pDevGamePad->EnumObjects(EnumAxesCallback, (void*)m_aGamePad.pDevGamePad, DIDFT_AXIS);

		m_GamepadAxesNumber++;	// �ԍ���i�߂�

		if (m_NumForceFeedbackAxis > 2) m_NumForceFeedbackAxis = 2;

		// �G�t�F�N�g�I�u�W�F�N�g�̐���
		if (!(CreateEffect(hWnd)))
		{
			m_aGamePad.pEffect = NULL;
		}

		// �Q�[���p�b�h�ւ̃A�N�Z�X�����l��(���͐���J�n)
		hr = m_aGamePad.pDevGamePad->Poll();

		if (FAILED(hr))
		{
			hr = m_aGamePad.pDevGamePad->Acquire();

			while (hr == DIERR_INPUTLOST)
			{
				hr = m_aGamePad.pDevGamePad->Acquire();
			}
		}
	}


	return S_OK;
}

//==============================================================================================================================
// �Q�[���p�b�h�̏I������
//==============================================================================================================================
void CDirectInput::Uninit(void)
{
	// �U����~
	SetVibration(0, 0);

	// ���̓f�o�C�X�̊J��
	if (m_aGamePad.pDevGamePad != NULL)
	{
		m_aGamePad.pDevGamePad->Unacquire();
		m_aGamePad.pDevGamePad->Release();
		m_aGamePad.pDevGamePad = NULL;
	}

	// �G�t�F�N�g�I�u�W�F�N�g�̊J��
	if (m_aGamePad.pEffect != NULL)
	{
		m_aGamePad.pEffect->Release();
		m_aGamePad.pEffect = NULL;
	}


	// DirectInput�I�u�W�F�N�g�̊J��
	CInput::Uninit();
}

//==============================================================================================================================
// �Q�[���p�b�h�̍X�V����
//==============================================================================================================================
void CDirectInput::Update(void)
{
	if (m_aGamePad.pDevGamePad != NULL)
	{
		DIJOYSTATE2 aGamePadKeyState;								// �R���g���[���[�̓��͏��
		BYTE aLeftAnalogStick[ANALOG_STICK_MAX] = { 0,0,0,0 };		// �X�e�B�b�N�̓��͏��
		BYTE aPov[POV_MAX] = { 0,0,0,0 };							// �\���L�[�̓��͏��

		HRESULT hr;
		hr = m_aGamePad.pDevGamePad->Poll();

		// ���̓f�o�C�X����f�[�^���擾
		if (SUCCEEDED(hr = m_aGamePad.pDevGamePad->GetDeviceState(sizeof(DIJOYSTATE2), &aGamePadKeyState)))
		{
			// �{�^��
			for (int nCntKey = 0; nCntKey < GAMEPAD_NUM_KEY_MAX; nCntKey++)
			{
				// �R���g���[���[�̓��͏��̕ۑ�(�g���K�[)
				m_aGamePad.aGamePadKeyStateTrigger[nCntKey] = (m_aGamePad.aGamePadKeyState[nCntKey] ^ aGamePadKeyState.rgbButtons[nCntKey]) & aGamePadKeyState.rgbButtons[nCntKey];
				// �R���g���[���[�̓��͏��̕ۑ�(�����[�X)
				m_aGamePad.aGamePadKeyStateRelease[nCntKey] = (aGamePadKeyState.rgbButtons[nCntKey] ^ m_aGamePad.aGamePadKeyState[nCntKey]) & m_aGamePad.aGamePadKeyState[nCntKey];
				// �R���g���[���[�̓��͏��̕ۑ�(�v���X)
				m_aGamePad.aGamePadKeyState[nCntKey] = aGamePadKeyState.rgbButtons[nCntKey];
			}

			//--------------------------------------------------------
			// ���A�i���O�X�e�B�b�N�̓��͏��̎擾
			//--------------------------------------------------------
			if (aGamePadKeyState.lY < -GAMEPAD_DEADZONE * 100 && aGamePadKeyState.lX < GAMEPAD_DEADZONE * 100 && aGamePadKeyState.lX > -GAMEPAD_DEADZONE * 100)
			{// ���X�e�B�b�N����ɓ|����
				aLeftAnalogStick[ANALOG_STICK_UP] = 128;			// ���͂��ꂽ
			}
			else if (aGamePadKeyState.lY > GAMEPAD_DEADZONE * 100 && aGamePadKeyState.lX < GAMEPAD_DEADZONE * 100 && aGamePadKeyState.lX > -GAMEPAD_DEADZONE * 100)
			{// ���X�e�B�b�N�����ɓ|����
				aLeftAnalogStick[ANALOG_STICK_DOWN] = 128;			// ���͂��ꂽ
			}
			else if (aGamePadKeyState.lX > GAMEPAD_DEADZONE * 100)
			{// ��g�X�e�B�b�N���E�ɓ|����
				aLeftAnalogStick[ANALOG_STICK_RIGHT] = 128;			// ���͂��ꂽ
			}
			else if (aGamePadKeyState.lX < -GAMEPAD_DEADZONE * 100)
			{// ���X�e�B�b�N�����ɓ|����
				aLeftAnalogStick[ANALOG_STICK_LEFT] = 128;			// ���͂��ꂽ
			}

			for (int nCntAnalogStick = 0; nCntAnalogStick < ANALOG_STICK_MAX; nCntAnalogStick++)
			{// ���A�i���O�X�e�B�b�N�̕����̎�ޕ����[�v
			 // ���X�e�B�b�N�̃g���K�[���
				m_aGamePad.aLeftAnalogStickTrigger[nCntAnalogStick] = (m_aGamePad.aLeftAnalogStick[nCntAnalogStick] ^ aLeftAnalogStick[nCntAnalogStick])&aLeftAnalogStick[nCntAnalogStick];		// ���A�i���O�X�e�B�b�N�̃g���K�[���
																																																// ���X�e�B�b�N�̃v���X���
				m_aGamePad.aLeftAnalogStick[nCntAnalogStick] = aLeftAnalogStick[nCntAnalogStick];																						// ���A�i���O�X�e�B�b�N�̃v���X���
			}
			//--------------------------------------------------------
			// �\���L�[�̓��͏��̎擾
			//--------------------------------------------------------
			if (aGamePadKeyState.rgdwPOV[0] == 0.0f)
			{// �\���L�[�̏��������
				aPov[POV_UP] = 128;			// ���͂��ꂽ
			}
			if (aGamePadKeyState.rgdwPOV[0] == 18000.0f)
			{// �\���L�[�̉���������
				aPov[POV_DOWN] = 128;			// ���͂��ꂽ
			}
			if (aGamePadKeyState.rgdwPOV[0] == 9000.0f)
			{// �\���L�[�̉E��������
				aPov[POV_RIGHT] = 128;			// ���͂��ꂽ
			}
			if (aGamePadKeyState.rgdwPOV[0] == 27000.0f)
			{// �\���L�[�̍���������
				aPov[POV_LEFT] = 128;			// ���͂��ꂽ
			}

			for (int nCntPov = 0; nCntPov < POV_MAX; nCntPov++)
			{// �\���L�[�̕����̎�ޕ����[�v
			 // �\���L�[�̃g���K�[���
				m_aGamePad.aPovTrigger[nCntPov] = (m_aGamePad.aPov[nCntPov] ^ aPov[nCntPov])&aPov[nCntPov];		// �\���L�[�̃g���K�[���

																												// �\���L�[�̃v���X���
				m_aGamePad.aPov[nCntPov] = aPov[nCntPov];		// �\���L�[�̃v���X���
			}
			m_aGamePad.aGamePad = aGamePadKeyState;			// ���݂̏�Ԃ�ۑ�
		}
		else
		{
			m_aGamePad.pDevGamePad->Acquire();				// �Q�[���p�b�h�ւ̃A�N�Z�X�����l��
		}

		if (m_aGamePad.nCounterVibration > 0)
		{
			m_aGamePad.nCounterVibration--;
			if (m_aGamePad.nCounterVibration <= 0)
			{// �J�E���^�[��0�ɂȂ�����U����~
				SetVibration(0, 0);
			}
		}
	}

}

//=============================================================================
// �U���̐ݒ�
//=============================================================================
void CDirectInput::SetVibration(int nPower, int nTime)
{
	if (m_aGamePad.pEffect == NULL) return;

	if (nPower > 100)
	{// 100%�𒴂��Ă���ꍇ
		nPower = 100;
	}
	else if (nPower < 0)
	{// 0%�����̏ꍇ
		nPower = 0;
	}

	if (m_aGamePad.pDevGamePad != NULL)
	{// �R���g���[���[���q�����Ă��Ȃ��Ƃ��������Ȃ�
		if (nPower > 0)
		{
			DIEFFECT diEffect;

			ZeroMemory(&diEffect, sizeof(diEffect));
			diEffect.dwSize = sizeof(DIEFFECT);
			diEffect.dwGain = DI_FFNOMINALMAX / 100 * nPower;
			m_aGamePad.pEffect->SetParameters(&diEffect, DIEP_GAIN);

			m_aGamePad.pEffect->Start(1, 0);
		}
		else
		{
			m_aGamePad.pEffect->Stop();
		}
		m_aGamePad.nCounterVibration = nTime;
	}
}

//==============================================================================================================================
// �Q�[���p�b�h�̓��͏��(�X�e�B�b�N)���擾
//==============================================================================================================================
CDirectInput::GamePad *CDirectInput::GetgamePadStick(void)
{

	if (m_aGamePad.pDevGamePad != NULL)
	{// �f�o�C�X��NULL����Ȃ�������
	 // �W���C�X�e�B�b�N�̏����擾
		m_aGamePad.pDevGamePad->GetDeviceState(sizeof(DIJOYSTATE2), &m_aGamePad.aGamePad);
	}

	return &m_aGamePad;			// �W���C�X�e�B�b�N�̏���Ԃ�
}

//==============================================================================================================================
// �Q�[���p�b�h�̓��͏��(�v���X���)���擾
//==============================================================================================================================
bool CDirectInput::GetGamePadPress(int nKey)
{
	if (m_aGamePad.pDevGamePad == NULL)
	{// �ڑ�����Ă��Ȃ��Ȃ�FALSE��Ԃ�
		return false;
	}
	return(m_aGamePad.aGamePadKeyState[nKey] & 0x80) ? true : false;
}

//==============================================================================================================================
// �Q�[���p�b�h�̓��͏��(�g���K�[���)���擾
//==============================================================================================================================
bool CDirectInput::GetGamePadTrigger(int nKey)
{
	if (m_aGamePad.pDevGamePad == NULL)
	{// �ڑ�����Ă��Ȃ��Ȃ�FALSE��Ԃ�
		return false;
	}
	return(m_aGamePad.aGamePadKeyStateTrigger[nKey] & 0x80) ? true : false;
}

//==============================================================================================================================
// �Q�[���p�b�h�̓��͏��(�����[�X���)���擾
//==============================================================================================================================
bool CDirectInput::GetGamePadRelease(int nKey)
{
	if (m_aGamePad.pDevGamePad == NULL)
	{// �ڑ�����Ă��Ȃ��Ȃ�FALSE��Ԃ�
		return false;
	}
	return(m_aGamePad.aGamePadKeyStateRelease[nKey] & 0x80) ? true : false;
}
//==============================================================================================================================
// �Q�[���p�b�h�̃X�e�B�b�N���͏��(�v���X���)���擾
//==============================================================================================================================
bool CDirectInput::GetGamePadStickPress(int nKey)
{
	if (m_aGamePad.pDevGamePad == NULL)
	{// �ڑ�����Ă��Ȃ��Ȃ�FALSE��Ԃ�
		return false;
	}
	return (m_aGamePad.aLeftAnalogStick[nKey] & 0x80) ? true : false;
}

//==============================================================================================================================
// �Q�[���p�b�h�̃X�e�B�b�N���͏��(�g���K�[���)���擾
//==============================================================================================================================
bool CDirectInput::GetGamePadStickTrigger(int nKey)
{
	if (m_aGamePad.pDevGamePad == NULL)
	{// �ڑ�����Ă��Ȃ��Ȃ�FALSE��Ԃ�
		return false;
	}
	return (m_aGamePad.aLeftAnalogStickTrigger[nKey] & 0x80) ? true : false;
}

//=============================================================================
// �\���L�[���͏��(�v���X�[���)���擾
//=============================================================================
bool CDirectInput::GetPovTpress(int nAngle)
{
	if (m_aGamePad.pDevGamePad == NULL)
	{// �ڑ�����Ă��Ȃ��Ȃ�FALSE��Ԃ�
		return false;
	}
	return (m_aGamePad.aPov[nAngle] & 0x80) ? true : false;
}

//=============================================================================
// �\���L�[���͏��(�g���K�[���)���擾
//=============================================================================
bool CDirectInput::GetPovTtigger(int nAngle)
{
	if (m_aGamePad.pDevGamePad == NULL)
	{// �ڑ�����Ă��Ȃ��Ȃ�FALSE��Ԃ�
		return false;
	}
	return (m_aGamePad.aPovTrigger[nAngle] & 0x80) ? true : false;
}

//*****************************************************************************
// joystick�p�R�[���o�b�N�֐�
//*****************************************************************************
BOOL CALLBACK CDirectInput::EnumJoysticksCallback(const DIDEVICEINSTANCE* pdidinstance, VOID* pContext)
{
	HRESULT hr;

	// �񋓂��ꂽ�W���C�X�e�B�b�N�ւ̃C���^�[�t�F�C�X���擾
	hr = m_pInput->CreateDevice(pdidinstance->guidInstance, &m_aGamePad.pDevGamePad, NULL);

	// �f�o�C�X�����o�o���Ȃ������ꍇ
	if (FAILED(hr))
	{
		return DIENUM_CONTINUE;
	}

	m_GamePadNumber++;		// �ԍ���i�߂�

	return DIENUM_CONTINUE;
}

//*****************************************************************************
// joystick�̋@�\�R�[���o�b�N�֐�
//*****************************************************************************
BOOL CALLBACK CDirectInput::EnumAxesCallback(const DIDEVICEOBJECTINSTANCE *pdidoi, VOID *pContext)
{
	HRESULT hr;
	DIPROPRANGE diprg;

	diprg.diph.dwSize = sizeof(DIPROPRANGE);
	diprg.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	diprg.diph.dwHow = DIPH_BYID;
	diprg.diph.dwObj = pdidoi->dwType;
	diprg.lMin = -(GAMEPAD_DEADZONE / GAMEPAD_DEADZONE * MAX_ZONE);				// �\���L�[�̍ŏ��l [��] [��]
	diprg.lMax = GAMEPAD_DEADZONE / GAMEPAD_DEADZONE * MAX_ZONE;				// �\���L�[�̍ő�l [��] [�E]

																				// �f�o�C�X�ɐݒ�
	hr = m_aGamePad.pDevGamePad->SetProperty(DIPROP_RANGE, &diprg.diph);

	if (FAILED(hr))
	{
		return DIENUM_STOP;
	}

	if ((pdidoi->dwFlags & DIDOI_FFACTUATOR) != 0)
	{// �t�H�[�X�t�B�[�h�o�b�N�̎����J�E���g
		m_NumForceFeedbackAxis++;
	}

	return DIENUM_CONTINUE;
}

//*****************************************************************************
// �G�t�F�N�g�I�u�W�F�N�g�̐���
//*****************************************************************************
BOOL CDirectInput::CreateEffect(HWND hWnd)
{
	DWORD           rgdwAxes[2] = { DIJOFS_X , DIJOFS_Y };
	LONG            rglDirection[2] = { 1 , 1 };
	DICONSTANTFORCE cf;
	DIEFFECT        eff;
	HRESULT         hr;

	cf.lMagnitude = DI_FFNOMINALMAX;

	ZeroMemory(&eff, sizeof(eff));
	eff.dwSize = sizeof(DIEFFECT);
	eff.dwFlags = DIEFF_CARTESIAN | DIEFF_OBJECTOFFSETS;
	eff.dwDuration = INFINITE;
	eff.dwSamplePeriod = 0;
	eff.dwGain = DI_FFNOMINALMAX;
	eff.dwTriggerButton = DIEB_NOTRIGGER;
	eff.dwTriggerRepeatInterval = 0;
	eff.cAxes = m_NumForceFeedbackAxis;
	eff.rgdwAxes = rgdwAxes;
	eff.rglDirection = rglDirection;
	eff.lpEnvelope = 0;
	eff.cbTypeSpecificParams = sizeof(DICONSTANTFORCE);
	eff.lpvTypeSpecificParams = &cf;
	eff.dwStartDelay = 0;

	hr = m_aGamePad.pDevGamePad->CreateEffect(GUID_ConstantForce, &eff, &m_aGamePad.pEffect, NULL);
	if (FAILED(hr))
	{
		return FALSE;
	}

	return TRUE;
}

//*****************************************************************************
// �Q�[���p�b�h�̎h�����Ă��鐔�̎擾
//*****************************************************************************
int CDirectInput::GetGamePadNum(void)
{
	return m_GamePadNumber;
}

/****************************************************************************/
/*								XInput�̏���								*/
//***************************************************************************/
//=============================================================================
// �R���X�g���N�^
//=============================================================================
CXInput::CXInput()
{
	// �e��l�̃N���A
	for (int nCntPad = 0; nCntPad < MAX_DEVICE; nCntPad++)
	{// �����ł���f�o�C�X�̐������J��Ԃ�
		for (int nCntButton = 0; nCntButton < XIJS_BUTTON_MAX; nCntButton++)
		{// �{�^���̐������J��Ԃ�
			m_aGamePadState[nCntPad][nCntButton] = 0x00;          // �v���X���
			m_aGamePadStateTrigger[nCntPad][nCntButton] = 0x00;   // �g���K�[���
			m_aGamePadStateRelease[nCntPad][nCntButton] = 0x00;   // �����[�X���
			m_aGamePadStateRepeat[nCntPad][nCntButton] = 0x00;    // ���s�[�g���
			m_aGamePadCounter[nCntPad][nCntButton] = 0;           // ���̓J�E���^�[
		}
		m_aGamePad[nCntPad].m_bConnected = false;                 // �ڑ��̏��
		m_aGamePad[nCntPad].m_State.Gamepad.wButtons = 0;         // �{�^���̃r�b�g���
		m_aGamePad[nCntPad].m_State.Gamepad.bLeftTrigger = 0;     // LT�{�^���̏��
		m_aGamePad[nCntPad].m_State.Gamepad.bRightTrigger = 0;    // RT�{�^���̏��
		m_aGamePad[nCntPad].m_State.Gamepad.sThumbLX = 0;         // ���X�e�B�b�N��X��
		m_aGamePad[nCntPad].m_State.Gamepad.sThumbLY = 0;         // ���X�e�B�b�N��Y��
		m_aGamePad[nCntPad].m_State.Gamepad.sThumbRX = 0;         // �E�X�e�B�b�N��X��
		m_aGamePad[nCntPad].m_State.Gamepad.sThumbRY = 0;         // �E�X�e�B�b�N��Y��

		m_bLeftTrigger[nCntPad] = 0;                              // LT�{�^���̏��
		m_bRightTrigger[nCntPad] = 0;                             // RT�{�^���̏��
		m_sThumbLX[nCntPad] = 0;                                  // ���X�e�B�b�N��X���̒l
		m_sThumbLY[nCntPad] = 0;                                  // ���X�e�B�b�N��Y���̒l
		m_sThumbRX[nCntPad] = 0;                                  // �E�X�e�B�b�N��X���̒l
		m_sThumbRY[nCntPad] = 0;                                  // �E�X�e�B�b�N��Y���̒l
		m_wButtons[nCntPad] = 0;                                  // �{�^���̏��
	}
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CXInput::~CXInput()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CXInput::Init(void)
{
	// �Q�[���p�b�h�̃N���A
	ZeroMemory(m_aGamePad, sizeof(CONTROLER_STATE) * MAX_DEVICE);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CXInput::Uninit(void)
{
	// �Q�[���p�b�h�̊J��
	XInputEnable(false);
}

//=============================================================================
// �X�V����
//=============================================================================
void CXInput::Update(void)
{
	DWORD dwResult;                             // �Q�[���p�b�h���擾�ł������ǂ���
	BYTE aGamePadState[XIJS_BUTTON_MAX] = {};   // �Q�[���p�b�h�̃{�^�����͏��
	for (DWORD nCntPad = 0; nCntPad < MAX_DEVICE; nCntPad++)
	{// �ڑ��ł���Q�[���p�b�h(�ő�4�܂�)�̐������J��Ԃ�
	 // �Q�[���p�b�h�̏�Ԃ��擾
		dwResult = XInputGetState(nCntPad, &m_aGamePad[nCntPad].m_State);
		if (dwResult == ERROR_SUCCESS)
		{// ��Ԏ擾�ɐ�������
		 // ��Ԃ��i�[����
			m_aGamePad[nCntPad].m_bConnected = true;                                       // �ڑ����ꂽ��Ԃɂ���
			m_wButtons[nCntPad] = m_aGamePad[nCntPad].m_State.Gamepad.wButtons;            // �{�^���̃r�b�g�����擾
			m_bLeftTrigger[nCntPad] = m_aGamePad[nCntPad].m_State.Gamepad.bLeftTrigger;    // LT�{�^���̏�Ԃ��擾
			m_bRightTrigger[nCntPad] = m_aGamePad[nCntPad].m_State.Gamepad.bRightTrigger;  // RT�{�^���̏�Ԃ��擾
			m_sThumbLX[nCntPad] = m_aGamePad[nCntPad].m_State.Gamepad.sThumbLX;            // ���X�e�B�b�N��X�����擾
			m_sThumbLY[nCntPad] = m_aGamePad[nCntPad].m_State.Gamepad.sThumbLY;            // ���X�e�B�b�N��Y�����擾
			m_sThumbRX[nCntPad] = m_aGamePad[nCntPad].m_State.Gamepad.sThumbRX;            // �E�X�e�B�b�N��X�����擾
			m_sThumbRY[nCntPad] = m_aGamePad[nCntPad].m_State.Gamepad.sThumbRY;            // �E�X�e�B�b�N��Y�����擾

																						   // ���͂̏�Ԃ��擾����
																						   // �{�^��
			DWORD wButtons = m_wButtons[nCntPad]; // �{�^���̃r�b�g���
			if (wButtons >= XINPUT_GAMEPAD_Y)
			{// Y�{�^����������Ă���
				aGamePadState[XIJS_BUTTON_13] = 0x80;   // ���͂��N��
				wButtons -= XINPUT_GAMEPAD_Y;
			}
			if (wButtons >= XINPUT_GAMEPAD_X)
			{// X�{�^����������Ă���
				aGamePadState[XIJS_BUTTON_12] = 0x80;   // ���͂��N��
				wButtons -= XINPUT_GAMEPAD_X;
			}
			if (wButtons >= XINPUT_GAMEPAD_B)
			{// B�{�^����������Ă���
				aGamePadState[XIJS_BUTTON_11] = 0x80;   // ���͂��N��
				wButtons -= XINPUT_GAMEPAD_B;
			}
			if (wButtons >= XINPUT_GAMEPAD_A)
			{// A�{�^����������Ă���
				aGamePadState[XIJS_BUTTON_10] = 0x80;   // ���͂��N��
				wButtons -= XINPUT_GAMEPAD_A;
			}
			if (wButtons >= XINPUT_GAMEPAD_RIGHT_SHOULDER)
			{// RB�{�^����������Ă���
				aGamePadState[XIJS_BUTTON_9] = 0x80;	// ���͂��N��
				wButtons -= XINPUT_GAMEPAD_RIGHT_SHOULDER;
			}
			if (wButtons >= XINPUT_GAMEPAD_LEFT_SHOULDER)
			{// LB�{�^����������Ă���
				aGamePadState[XIJS_BUTTON_8] = 0x80;	// ���͂��N��
				wButtons -= XINPUT_GAMEPAD_LEFT_SHOULDER;
			}
			if (wButtons >= XINPUT_GAMEPAD_RIGHT_THUMB)
			{// �E�X�e�B�b�N���������܂�Ă���
				aGamePadState[XIJS_BUTTON_7] = 0x80;	// ���͂��N��
				wButtons -= XINPUT_GAMEPAD_RIGHT_THUMB;
			}
			if (wButtons >= XINPUT_GAMEPAD_LEFT_THUMB)
			{// �E�X�e�B�b�N���������܂�Ă���
				aGamePadState[XIJS_BUTTON_6] = 0x80;	// ���͂��N��
				wButtons -= XINPUT_GAMEPAD_LEFT_THUMB;
			}
			if (wButtons >= XINPUT_GAMEPAD_BACK)
			{// BACK�{�^����������Ă���
				aGamePadState[XIJS_BUTTON_5] = 0x80;	// ���͂��N��
				wButtons -= XINPUT_GAMEPAD_BACK;
			}
			if (wButtons >= XINPUT_GAMEPAD_START)
			{// BACK�{�^����������Ă���
				aGamePadState[XIJS_BUTTON_4] = 0x80;	// ���͂��N��
				wButtons -= XINPUT_GAMEPAD_START;
			}
			if (wButtons >= XINPUT_GAMEPAD_DPAD_RIGHT)
			{// �\���L�[�̉E��������Ă���
				aGamePadState[XIJS_BUTTON_3] = 0x80;	// ���͂��N��
				wButtons -= XINPUT_GAMEPAD_DPAD_RIGHT;
			}
			if (wButtons >= XINPUT_GAMEPAD_DPAD_LEFT)
			{// �\���L�[�̍���������Ă���
				aGamePadState[XIJS_BUTTON_2] = 0x80;	// ���͂��N��
				wButtons -= XINPUT_GAMEPAD_DPAD_LEFT;
			}
			if (wButtons >= XINPUT_GAMEPAD_DPAD_DOWN)
			{// �\���L�[�̉���������Ă���
				aGamePadState[XIJS_BUTTON_1] = 0x80;	// ���͂��N��
				wButtons -= XINPUT_GAMEPAD_DPAD_DOWN;
			}
			if (wButtons >= XINPUT_GAMEPAD_DPAD_UP)
			{// �\���L�[�̏オ������Ă���
				aGamePadState[XIJS_BUTTON_0] = 0x80;	// ���͂��N��
			}

			// LT�g���K�[
			if (m_bLeftTrigger[nCntPad] >= MIN_GAMEPAD_LEFT_TRIGGER)
			{// LT�g���K�[��������Ă���
				aGamePadState[XIJS_BUTTON_14] = 0x80;   // ���͂��N������
			}
			// RT�g���K�[
			if (m_bRightTrigger[nCntPad] >= MIN_GAMEPAD_RIGHT_TRIGGER)
			{// LR�g���K�[��������Ă���
				aGamePadState[XIJS_BUTTON_15] = 0x80;   // ���͂��N������
			}

			// ���X�e�B�b�N
			if (m_sThumbLX[nCntPad] >= MIN_GAMEPAD_LEFT_THUMB_X)
			{// ���X�e�B�b�N���E�ɓ|���ꂽ
				aGamePadState[XIJS_BUTTON_19] = 0x80;   // ���͂��N������
			}
			if (m_sThumbLX[nCntPad] <= -MIN_GAMEPAD_LEFT_THUMB_X)
			{// ���X�e�B�b�N�����ɓ|���ꂽ
				aGamePadState[XIJS_BUTTON_18] = 0x80;   // ���͂��N������
			}
			if (m_sThumbLY[nCntPad] >= MIN_GAMEPAD_LEFT_THUMB_Y)
			{// ���X�e�B�b�N����ɓ|���ꂽ
				aGamePadState[XIJS_BUTTON_16] = 0x80;   // ���͂��N������
			}
			if (m_sThumbLY[nCntPad] <= -MIN_GAMEPAD_LEFT_THUMB_Y)
			{// ���X�e�B�b�N�����ɓ|���ꂽ
				aGamePadState[XIJS_BUTTON_17] = 0x80;   // ���͂��N������
			}

			// �E�X�e�B�b�N
			if (m_sThumbRX[nCntPad] >= MIN_GAMEPAD_LEFT_THUMB_X)
			{// �E�X�e�B�b�N���E�ɓ|���ꂽ
				aGamePadState[XIJS_BUTTON_23] = 0x80;   // ���͂��N������
			}
			if (m_sThumbRX[nCntPad] <= -MIN_GAMEPAD_LEFT_THUMB_X)
			{// �E�X�e�B�b�N�����ɓ|���ꂽ
				aGamePadState[XIJS_BUTTON_22] = 0x80;   // ���͂��N������
			}
			if (m_sThumbRY[nCntPad] >= MIN_GAMEPAD_LEFT_THUMB_Y)
			{// �E�X�e�B�b�N����ɓ|���ꂽ
				aGamePadState[XIJS_BUTTON_20] = 0x80;   // ���͂��N������
			}
			if (m_sThumbRY[nCntPad] <= -MIN_GAMEPAD_LEFT_THUMB_Y)
			{// �E�X�e�B�b�N�����ɓ|���ꂽ
				aGamePadState[XIJS_BUTTON_21] = 0x80;   // ���͂��N������
			}

			// �{�^���̏�Ԃ��i�[����
			for (int nCntButton = 0; nCntButton < XIJS_BUTTON_MAX; nCntButton++)
			{// �{�^���̐������J��Ԃ�
				m_aGamePadStateTrigger[nCntPad][nCntButton] = (m_aGamePadState[nCntPad][nCntButton] ^ aGamePadState[nCntButton]) & aGamePadState[nCntButton];            // �{�^���̓��͏��(�g���K�[���)�ۑ�
				m_aGamePadStateRelease[nCntPad][nCntButton] = (m_aGamePadState[nCntPad][nCntButton] ^ aGamePadState[nCntButton]) & m_aGamePadState[nCntPad][nCntButton]; // �{�^���̓��͏��(�����[�X���)�ۑ�
				if (aGamePadState[nCntButton] == 0x80)
				{// �{�^�������͂���Ă���
					m_aGamePadCounter[nCntPad][nCntButton]++;    // �J�E���^�[��i�߂�
					if (m_aGamePadCounter[nCntPad][nCntButton] >= REPEAT_FRAME || 1 == m_aGamePadCounter[nCntPad][nCntButton])
					{// 20�t���[�����{�^����������Ă���
						if (0 == m_aGamePadCounter[nCntPad][nCntButton] % REPEAT_TRIGGER || 1 == m_aGamePadCounter[nCntPad][nCntButton])
						{// 3�t���[������
							m_aGamePadStateRepeat[nCntPad][nCntButton] = 0x80;   // �{�^���̓��͏��(���s�[�g���)���N��
						}
						else
						{// ����ȊO�̃t���[��
							m_aGamePadStateRepeat[nCntPad][nCntButton] = 0;      // �{�^���̓��͏��(���s�[�g���)���ꎞ��~
						}
					}
					else
					{// 20�t���[���ȑO
						m_aGamePadStateRepeat[nCntPad][nCntButton] = 0;      // �{�^���̓��͏��(���s�[�g���)���ꎞ��~
					}
				}
				else
				{// �Q�[���p�b�h�̃{�^�������͂���Ă��Ȃ�
					m_aGamePadCounter[nCntPad][nCntButton] = 0;        // �J�E���^�[��߂�
					m_aGamePadStateRepeat[nCntPad][nCntButton] = 0;    // �{�^���̓��͏��(���s�[�g���)���~
				}
				m_aGamePadState[nCntPad][nCntButton] = aGamePadState[nCntButton];  // �{�^���̓��͏��(�v���X���)�ۑ�
			}
		}
		else
		{// ��Ԏ擾�Ɏ��s����
			m_aGamePad[nCntPad].m_bConnected = false;  // �ڑ�����Ă��Ȃ���Ԃɂ���
		}
	}
}

//=============================================================================
// �Q�[���p�b�h�̓��͏����擾
//=============================================================================
bool CXInput::GetAnyButton(int nCntPad)
{
	for (int nCount = 0; nCount < XIJS_BUTTON_16; nCount++)
	{// �X�e�B�b�N�ȊO�̃{�^���𒲂ׂ�
		if (m_aGamePadStateTrigger[nCntPad][nCount] == 0x80)
		{// ���͂��ꂽ
			return true;
		}
	}

	// ���͂��ꂽ�{�^�����Ȃ�����
	return false;
}
