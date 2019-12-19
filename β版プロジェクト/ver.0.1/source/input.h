//=============================================================================
//
// ���͂̏���[input.h]
// Author : Takuto Ishida
//
//=============================================================================
#ifndef _INPUT_H_
#define _INPUT_H_

//*****************************************************************************
// �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define NUM_KEY_MAX		(256)		// �L�[�{�[�h�̍ő吔
#define MAX_DEVICE		(4)			// �ڑ��ł�����̓f�o�C�X�̐�

#define GAMEPAD_DEADZONE		(3)
#define MAX_ZONE				(1000)
#define	GAMEPAD_NUM_KEY_MAX		(128)		// �L�[�̍ő吔

#define BUTTON_X	   (CDirectInput::ELECOM_X)		// X�{�^���̔ԍ�
#define BUTTON_Y	   (CDirectInput::ELECOM_Y)		// Y�{�^���̔ԍ�
#define BUTTON_A	   (CDirectInput::ELECOM_A)		// A�{�^���̔ԍ�
#define BUTTON_B	   (CDirectInput::ELECOM_B)		// B�{�^���̔ԍ�
#define BUTTON_L1	   (CDirectInput::ELECOM_L1)		// L1�{�^���̔ԍ�
#define BUTTON_R1	   (CDirectInput::ELECOM_R1)		// R1�{�^���̔ԍ�
#define BUTTON_L2	   (CDirectInput::ELECOM_L2)		// L2�{�^���̔ԍ�
#define BUTTON_R2	   (CDirectInput::ELECOM_R2)		// R2�{�^���̔ԍ�
#define BUTTON_L_STICK (CDirectInput::ELECOM_L_STICK)	// L_STICK�{�^���̔ԍ�
#define BUTTON_R_STICK (CDirectInput::ELECOM_R_STICK)	// R_STICK�{�^���̔ԍ�
#define BUTTON_BACK	   (CDirectInput::ELECOM_BACK)	// BACK�{�^���̔ԍ�
#define BUTTON_START   (CDirectInput::ELECOM_START)	// START�{�^���̔ԍ�
#define BUTTON_ARCADESTART   (9)						// �A�P�R��START�{�^���̔ԍ�
#define BUTTON_GUIDE   (CDirectInput::ELECOM_GUIDE)	// GUIDE�{�^���̔ԍ�
#define BUTTON_MAX	   (CDirectInput::ELECOM_MAX)		// �{�^���̍ő�

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CInput
{
public:
	CInput(int nNumDevice = 1);
	virtual ~CInput();
	virtual HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	virtual void Uninit(void);
	virtual void Update(void) = 0;

protected:
	static HWND				m_hWnd;			// �E�B���h�E�n���h��
	static LPDIRECTINPUT8	m_pInput;		// DirectInput�I�u�W�F�N�g�ւ̃|�C���^
	LPDIRECTINPUTDEVICE8	*m_pDevice;		// ���̓f�o�C�X�ւ̃|�C���^
	int						m_nDeviceNum;	// ���̓f�o�C�X����������������
};

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �L�[�{�[�h�N���X�̒�`
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class CInputKeyboard : public CInput
{
public:
	CInputKeyboard(int nNumDevice = 1);
	~CInputKeyboard();
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);

	bool GetPress(int nKey) { return (m_aKeyState[nKey] & 0x80) ? true : false; };			// �L�[�{�[�h�̓��͏��(�v���X���)���擾
	bool GetTrigger(int nKey) { return (m_aKeyStateTrigger[nKey] & 0x80) ? true : false; };	// �L�[�{�[�h�̓��͏��(�g���K�[���)���擾
	bool GetRelease(int nKey) { return (m_aKeyStateRelease[nKey] & 0x80) ? true : false; };	// �L�[�{�[�h�̓��͏��(�����[�X���)���擾
	bool GetRepeat(int nKey) { return (m_aKeyStateRepeat[nKey] & 0x80) ? true : false; };	// �L�[�{�[�h�̓��͏��(���s�[�g���)���擾
	bool GetAnyKey(void);

private:
	BYTE   m_aKeyState[NUM_KEY_MAX];			// �L�[�{�[�h�̓��͏��(�v���X���)
	BYTE   m_aKeyStateTrigger[NUM_KEY_MAX];		// �L�[�{�[�h�̓��͏��(�g���K�[���)
	BYTE   m_aKeyStateRelease[NUM_KEY_MAX];		// �L�[�{�[�h�̓��͏��(�����[�X���)
	BYTE   m_aKeyStateRepeat[NUM_KEY_MAX];		// �L�[�{�[�h�̓��͏��(���s�[�g���)
	DWORD  m_aKeyboardCounter[NUM_KEY_MAX];		// �L�[�{�[�h�����͂���Ă�Ԃ𐔂���J�E���^�[
};

//*****************************************************************************
// �}�E�X�N���X�̒�`
//*****************************************************************************
class CInputMouse : public CInput
{
public:
	typedef enum
	{// �{�^���ԍ�
		DIMS_BUTTON_0 = 0,	// �}�E�X�̍��{�^��
		DIMS_BUTTON_1,		// �}�E�X�̉E�{�^��
		DIMS_BUTTON_2,		// �}�E�X�̃z�C�[���{�^��
		DIMS_BUTTON_3,		// �ȉ��̓{�^�����̂��Ȃ��}�E�X������
		DIMS_BUTTON_4,
		DIMS_BUTTON_5,
		DIMS_BUTTON_6,
		DIMS_BUTTON_7,
		DIMS_BUTTON_MAX
	}DIMS_BUTTON;

	CInputMouse(int nNumDevice = 1);
	~CInputMouse();
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);

	bool GetPress(int nButton) {return (m_aMouseState[nButton] & 0x80) ? true : false; };			// �}�E�X�̓��͏��(�v���X���)���擾
	bool GetTrigger(int nButton) { return (m_aMouseStateTrigger[nButton] & 0x80) ? true : false; };	// �}�E�X�̓��͏��(�g���K�[���)���擾
	bool GetRelease(int nButton) { return (m_aMouseStateRelease[nButton] & 0x80) ? true : false; };	// �}�E�X�̓��͏��(�����[�X���)���擾
	bool GetRepeat(int nButton) { return (m_aMouseStateRepeat[nButton] & 0x80) ? true : false; };	// �}�E�X�̓��͏��(���s�[�g���)���擾

	LONG GetMouseAxisX(void) { return m_MouseXAxis; };	// �}�E�X��X�����擾
	LONG GetMouseAxisY(void) { return m_MouseYAxis; };	// �}�E�X��Y�����擾
	LONG GetMouseAxisZ(void) { return m_MouseZAxis; };	// �}�E�X��Z�����擾

	POINT GetPoint(void) { return m_Point; };			// �}�E�X�̌��݂̃J�[�\�����W���擾
	POINT GetPointOld(void) { return m_PointOld; };		// �}�E�X�̑O��̃J�[�\�����W���擾

	int GetDiffPointX(void) { return m_nDiffPointX; };	// �}�E�X�J�[�\�����W�̍������擾
	int GetDiffPointY(void) { return m_nDiffPointY; };	// �}�E�X�J�[�\�����W�̍������擾

private:    // ���̃N���X�������A�N�Z�X�\
	BYTE	m_aMouseState[DIMS_BUTTON_MAX];			// �}�E�X�̓��͏��(�v���X���)
	BYTE	m_aMouseStateTrigger[DIMS_BUTTON_MAX];	// �}�E�X�̓��͏��(�g���K�[���)
	BYTE	m_aMouseStateRelease[DIMS_BUTTON_MAX];	// �}�E�X�̓��͏��(�����[�X���)
	BYTE	m_aMouseStateRepeat[DIMS_BUTTON_MAX];	// �}�E�X�̓��͏��(���s�[�g���)
	DWORD	m_aMouseCounter[DIMS_BUTTON_MAX];		// �}�E�X�����͂���Ă�Ԃ𐔂���J�E���^�[
	LONG	m_MouseXAxis;							// �}�E�X��X�����i�[����
	LONG	m_MouseYAxis;							// �}�E�X��Y�����i�[����
	LONG	m_MouseZAxis;							// �}�E�X��Z�����i�[����
	HWND	m_hWnd;									// �E�B���h�E�n���h��(�J�[�\�����W���擾���邽�߂ɕK�v)
	POINT	m_Point;								// ���݂̃}�E�X�̃J�[�\�����W
	POINT	m_PointOld;								// �O��̃}�E�X�̃J�[�\�����W
	int		m_nDiffPointX;							// �}�E�X�J�[�\�����W�̍���
	int		m_nDiffPointY;							// �}�E�X�J�[�\�����W�̍���
};

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// DirectInput�N���X�̒�`
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class CDirectInput :public CInput
{
public:
	typedef enum
	{// �A�i���O�X�e�B�b�N�̕����̎��(4����)
		ANALOG_STICK_UP = 0,
		ANALOG_STICK_RIGHT,
		ANALOG_STICK_LEFT,
		ANALOG_STICK_DOWN,
		ANALOG_STICK_MAX
	}ANALOG_STICK;
	typedef enum
	{// �\���L�[�̕����̎��
		POV_UP = 0,
		POV_DOWN,
		POV_RIGHT,
		POV_LEFT,
		POV_MAX
	}POV;
	typedef enum
	{// �{�^���̎��
		ELECOM_X = 0,
		ELECOM_Y,
		ELECOM_A,
		ELECOM_B,
		ELECOM_L1,
		ELECOM_R1,
		ELECOM_L2,
		ELECOM_R2,
		ELECOM_L_STICK,
		ELECOM_R_STICK,
		ELECOM_BACK,
		ELECOM_START,
		ELECOM_GUIDE,
		ELECOM_MAX
	}ELECOM_CTRL;
	typedef struct
	{
		LPDIRECTINPUTDEVICE8 pDevGamePad;							// ���̓f�o�C�X(k�R���g���[���[)�ւ̃|�C���^
		DIDEVCAPS GamepadCaps;										// �Q�[���p�b�h�̋@�\���
		LPDIRECTINPUTEFFECT	pEffect;								// �G�t�F�N�g�I�u�W�F�N�g

		DIJOYSTATE2 aGamePad;										// �R���g���[���[�̏��

		BYTE		aGamePadKeyState[GAMEPAD_NUM_KEY_MAX];			// �W���C�X�e�B�b�N�̃v���X���͏��
		BYTE		aGamePadKeyStateTrigger[GAMEPAD_NUM_KEY_MAX];	// �W���C�X�e�B�b�N�̃g���K�[���͏��
		BYTE		aGamePadKeyStateRelease[GAMEPAD_NUM_KEY_MAX];	// �W���C�X�e�B�b�N�̃����[�X���͏��
		BYTE		aLeftAnalogStick[ANALOG_STICK_MAX];				// ���A�i���O�X�e�B�b�N�̃v���X���
		BYTE		aLeftAnalogStickTrigger[ANALOG_STICK_MAX];		// ���A�i���O�X�e�B�b�N�̃g���K�[���
		BYTE		aPov[POV_MAX];									// �\���L�[�̃v���X���
		BYTE		aPovTrigger[POV_MAX];							// �\���L�[�̃g���K�[���

		int			nCounterVibration;								// �U���p�̃J�E���^�[
	}GamePad;

	CDirectInput();
	~CDirectInput();
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);
	void SetVibration(int nPower, int nTime);

	// �Q�[���p�b�h�̃{�^�����͏��
	bool GetGamePadPress(int nKey);
	bool GetGamePadTrigger(int nKey);
	bool GetGamePadRelease(int nKey);

	// �Q�[���p�b�h�̃X�e�B�b�N�̓��͏��
	bool GetGamePadStickPress(int nKey);
	bool GetGamePadStickTrigger(int nKey);

	// �Q�[���p�b�h�̏\���L�[�̓��͏��
	bool GetPovTpress(int nAngle);
	bool GetPovTtigger(int nAngle);

	GamePad *GetgamePadStick(void);
	int GetGamePadNum(void);
	static BOOL CALLBACK EnumJoysticksCallback(const DIDEVICEINSTANCE* pdidinstance, VOID* pContext);
	static BOOL CALLBACK EnumAxesCallback(const DIDEVICEOBJECTINSTANCE* pdidoi, VOID* pContext);
	BOOL CreateEffect(HWND hWnd);
private:
	static GamePad  m_aGamePad;	// �Q�[���p�b�h�̏��
	static int		m_GamePadNumber;				// �R�[���o�b�N�֐��Ŏg�ԍ�
	static int		m_GamepadAxesNumber;			// j�R�[���o�b�N�֐��Ŏg���ԍ�
	static DWORD	m_NumForceFeedbackAxis;		// �t�H�[�X�t�B�[�h�o�b�N
};

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// XInput�N���X�̒�`
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class CXInput
{
public:
#define MIN_GAMEPAD_LEFT_THUMB_X  (25000)	// ���X�e�B�b�N�̉������̓��͂��󂯕t���鎲�̍ŏ��l
#define MIN_GAMEPAD_LEFT_THUMB_Y  (25000)	// ���X�e�B�b�N�̏c�����̓��͂��󂯕t���鎲�̍ŏ��l

#define MIN_GAMEPAD_RIGHT_THUMB_X (25000)	// �E�X�e�B�b�N�̉������̓��͂��󂯕t���鎲�̍ŏ��l
#define MIN_GAMEPAD_RIGHT_THUMB_Y (25000)	// �E�X�e�B�b�N�̏c�����̓��͂��󂯕t���鎲�̍ŏ��l

#define MIN_GAMEPAD_LEFT_TRIGGER  (130)		// LT�{�^���̓��͂��󂯕t����l�̍ŏ��l
#define MIN_GAMEPAD_RIGHT_TRIGGER (130)		// RT�{�^���̓��͂��󂯕t����l�̍ŏ��l
			//------------------------
			//  �{�^���̗�
			//------------------------
			// XINPUT_GAMEPAD_DPAD_UP          // �\���L�[�����
			// XINPUT_GAMEPAD_DPAD_DOWN        // �\���L�[������
			// XINPUT_GAMEPAD_DPAD_LEFT        // �\���L�[������
			// XINPUT_GAMEPAD_DPAD_RIGHT       // �\���L�[�E����
			// XINPUT_GAMEPAD_START            // START�{�^��
			// XINPUT_GAMEPAD_BACK             // BACK�{�^��
			// XINPUT_GAMEPAD_LEFT_THUMB       // ���X�e�B�b�N�{�^��
			// XINPUT_GAMEPAD_RIGHT_THUMB      // �E�X�e�B�b�N�{�^��
			// XINPUT_GAMEPAD_LEFT_SHOULDER    // LB�{�^��
			// XINPUT_GAMEPAD_RIGHT_SHOULDER   // RB�{�^��
			// XINPUT_GAMEPAD_A                // A�{�^��
			// XINPUT_GAMEPAD_B                // B�{�^��
			// XINPUT_GAMEPAD_X                // X�{�^��
			// XINPUT_GAMEPAD_Y                // Y�{�^��
	typedef enum
	{
		XIJS_BUTTON_0,		// �\���L�[�����
		XIJS_BUTTON_1,		// �\���L�[������
		XIJS_BUTTON_2,		// �\���L�[������
		XIJS_BUTTON_3,		// �\���L�[�E����
		XIJS_BUTTON_4,		// START�{�^��
		XIJS_BUTTON_5,		// BACK�{�^��
		XIJS_BUTTON_6,		// ���X�e�B�b�N�������ݓ���
		XIJS_BUTTON_7,		// �E�X�e�B�b�N�������ݓ���
		XIJS_BUTTON_8,		// LB�{�^��
		XIJS_BUTTON_9,		// RB�{�^��
		XIJS_BUTTON_10,		// A�{�^��
		XIJS_BUTTON_11,		// B�{�^��
		XIJS_BUTTON_12,		// X�{�^��
		XIJS_BUTTON_13,		// Y�{�^��
		XIJS_BUTTON_14,		// LT�{�^��
		XIJS_BUTTON_15,		// RT�{�^��
		XIJS_BUTTON_16,		// ���X�e�B�b�N�����
		XIJS_BUTTON_17,		// ���X�e�B�b�N������
		XIJS_BUTTON_18,		// ���X�e�B�b�N������
		XIJS_BUTTON_19,		// ���X�e�B�b�N�E����
		XIJS_BUTTON_20,		// �E�X�e�B�b�N�����
		XIJS_BUTTON_21,		// �E�X�e�B�b�N������
		XIJS_BUTTON_22,		// �E�X�e�B�b�N������
		XIJS_BUTTON_23,		// �E�X�e�B�b�N�E����

						   // �ȉ����蓖�ĂȂ�
						   XIJS_BUTTON_24,
						   XIJS_BUTTON_25,
						   XIJS_BUTTON_26,
						   XIJS_BUTTON_27,
						   XIJS_BUTTON_28,
						   XIJS_BUTTON_29,
						   XIJS_BUTTON_30,
						   XIJS_BUTTON_31,
						   XIJS_BUTTON_MAX,
	}XIJS_BUTTON;

	CXInput();
	~CXInput();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

	BYTE GetLeftTrigger(int nCntPad) { return m_bLeftTrigger[nCntPad]; };	// LT�{�^���̏�Ԃ��擾
	BYTE GetRightTrigger(int nCntPad) { return m_bRightTrigger[nCntPad]; };	// RT�{�^���̏�Ԃ��擾
	float GetLeftAxiz(int nCntPad) { return atan2f(m_sThumbLX[nCntPad], m_sThumbLY[nCntPad]); };	// ���X�e�B�b�N�̌������擾
	float GetRightAxiz(int nCntPad) { return atan2f(m_sThumbRX[nCntPad], m_sThumbRY[nCntPad]); };	// �E�X�e�B�b�N�̌������擾
	SHORT GetThumbLX(int nCntPad) { return m_sThumbLX[nCntPad]; };	// ���X�e�B�b�N��X�����擾
	SHORT GetThumbLY(int nCntPad) { return m_sThumbLY[nCntPad]; };	// ���X�e�B�b�N��Y�����擾
	SHORT GetThumbRX(int nCntPad) { return m_sThumbRX[nCntPad]; };	// �E�X�e�B�b�N��X�����擾
	SHORT GetThumbRY(int nCntPad) { return m_sThumbRY[nCntPad]; };	// �E�X�e�B�b�N��Y�����擾

	bool GetPress(int nCntPad, XIJS_BUTTON Button) { return (m_aGamePadState[nCntPad][Button] & 0x80) ? true : false; };			// �Q�[���p�b�h�̓��͏��(�v���X���)���擾
	bool GetTrigger(int nCntPad, XIJS_BUTTON Button) { return (m_aGamePadStateTrigger[nCntPad][Button] & 0x80) ? true : false; };	// �Q�[���p�b�h�̓��͏��(�g���K�[���)���擾
	bool GetRelease(int nCntPad, XIJS_BUTTON Button) { return (m_aGamePadStateRelease[nCntPad][Button] & 0x80) ? true : false; };	// �Q�[���p�b�h�̓��͏��(�����[�X���)���擾
	bool GetRepeat(int nCntPad, XIJS_BUTTON Button) { return (m_aGamePadStateRepeat[nCntPad][Button] & 0x80) ? true : false; };		// �Q�[���p�b�h�̓��͏��(���s�[�g���)���擾
	bool GetAnyButton(int nCntPad);

private:
	typedef struct
	{
		XINPUT_STATE m_State;	// �Q�[���p�b�h�̏��
		bool m_bConnected;		// �ڑ����ꂽ���ǂ���
	}CONTROLER_STATE;

	CONTROLER_STATE	m_aGamePad[MAX_DEVICE];									// �Q�[���p�b�h�̏��
	DWORD			m_wButtons[MAX_DEVICE];									// �{�^���̏��
	BYTE			m_bLeftTrigger[MAX_DEVICE];								// LT�{�^���̏��
	BYTE			m_bRightTrigger[MAX_DEVICE];							// RT�{�^���̏��
	SHORT			m_sThumbLX[MAX_DEVICE];									// ���X�e�B�b�N��X��
	SHORT			m_sThumbLY[MAX_DEVICE];									// ���X�e�B�b�N��Y��
	SHORT			m_sThumbRX[MAX_DEVICE];									// �E�X�e�B�b�N��X��
	SHORT			m_sThumbRY[MAX_DEVICE];									// �E�X�e�B�b�N��Y��
	BYTE			m_aGamePadState[MAX_DEVICE][XIJS_BUTTON_MAX];			// �Q�[���p�b�h�̓��͏��(�v���X���)
	BYTE			m_aGamePadStateTrigger[MAX_DEVICE][XIJS_BUTTON_MAX];	// �Q�[���p�b�h�̓��͏��(�g���K�[���)
	BYTE			m_aGamePadStateRelease[MAX_DEVICE][XIJS_BUTTON_MAX];	// �Q�[���p�b�h�̓��͏��(�����[�X���)
	BYTE			m_aGamePadStateRepeat[MAX_DEVICE][XIJS_BUTTON_MAX];		// �Q�[���p�b�h�̓��͏��(���s�[�g���)
	DWORD			m_aGamePadCounter[MAX_DEVICE][XIJS_BUTTON_MAX];			// �Q�[���p�b�h�̓��͂���Ă�Ԃ𐔂���J�E���^�[
};
#endif