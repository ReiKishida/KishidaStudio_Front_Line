//=============================================================================
//
// 入力の処理[input.h]
// Author : Takuto Ishida
//
//=============================================================================
#ifndef _INPUT_H_
#define _INPUT_H_

//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define NUM_KEY_MAX		(256)		// キーボードの最大数
#define MAX_DEVICE		(4)			// 接続できる入力デバイスの数

#define GAMEPAD_DEADZONE		(3)
#define MAX_ZONE				(1000)
#define	GAMEPAD_NUM_KEY_MAX		(128)		// キーの最大数

#define BUTTON_X	   (CDirectInput::ELECOM_X)		// Xボタンの番号
#define BUTTON_Y	   (CDirectInput::ELECOM_Y)		// Yボタンの番号
#define BUTTON_A	   (CDirectInput::ELECOM_A)		// Aボタンの番号
#define BUTTON_B	   (CDirectInput::ELECOM_B)		// Bボタンの番号
#define BUTTON_L1	   (CDirectInput::ELECOM_L1)		// L1ボタンの番号
#define BUTTON_R1	   (CDirectInput::ELECOM_R1)		// R1ボタンの番号
#define BUTTON_L2	   (CDirectInput::ELECOM_L2)		// L2ボタンの番号
#define BUTTON_R2	   (CDirectInput::ELECOM_R2)		// R2ボタンの番号
#define BUTTON_L_STICK (CDirectInput::ELECOM_L_STICK)	// L_STICKボタンの番号
#define BUTTON_R_STICK (CDirectInput::ELECOM_R_STICK)	// R_STICKボタンの番号
#define BUTTON_BACK	   (CDirectInput::ELECOM_BACK)	// BACKボタンの番号
#define BUTTON_START   (CDirectInput::ELECOM_START)	// STARTボタンの番号
#define BUTTON_ARCADESTART   (9)						// アケコンSTARTボタンの番号
#define BUTTON_GUIDE   (CDirectInput::ELECOM_GUIDE)	// GUIDEボタンの番号
#define BUTTON_MAX	   (CDirectInput::ELECOM_MAX)		// ボタンの最大

//*****************************************************************************
// クラス定義
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
	static HWND				m_hWnd;			// ウィンドウハンドル
	static LPDIRECTINPUT8	m_pInput;		// DirectInputオブジェクトへのポインタ
	LPDIRECTINPUTDEVICE8	*m_pDevice;		// 入力デバイスへのポインタ
	int						m_nDeviceNum;	// 入力デバイスをいくつ生成したか
};

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// キーボードクラスの定義
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class CInputKeyboard : public CInput
{
public:
	CInputKeyboard(int nNumDevice = 1);
	~CInputKeyboard();
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);

	bool GetPress(int nKey) { return (m_aKeyState[nKey] & 0x80) ? true : false; };			// キーボードの入力情報(プレス情報)を取得
	bool GetTrigger(int nKey) { return (m_aKeyStateTrigger[nKey] & 0x80) ? true : false; };	// キーボードの入力情報(トリガー情報)を取得
	bool GetRelease(int nKey) { return (m_aKeyStateRelease[nKey] & 0x80) ? true : false; };	// キーボードの入力情報(リリース情報)を取得
	bool GetRepeat(int nKey) { return (m_aKeyStateRepeat[nKey] & 0x80) ? true : false; };	// キーボードの入力情報(リピート情報)を取得
	bool GetAnyKey(void);

private:
	BYTE   m_aKeyState[NUM_KEY_MAX];			// キーボードの入力情報(プレス情報)
	BYTE   m_aKeyStateTrigger[NUM_KEY_MAX];		// キーボードの入力情報(トリガー情報)
	BYTE   m_aKeyStateRelease[NUM_KEY_MAX];		// キーボードの入力情報(リリース情報)
	BYTE   m_aKeyStateRepeat[NUM_KEY_MAX];		// キーボードの入力情報(リピート情報)
	DWORD  m_aKeyboardCounter[NUM_KEY_MAX];		// キーボードが入力されてる間を数えるカウンター
};

//*****************************************************************************
// マウスクラスの定義
//*****************************************************************************
class CInputMouse : public CInput
{
public:
	typedef enum
	{// ボタン番号
		DIMS_BUTTON_0 = 0,	// マウスの左ボタン
		DIMS_BUTTON_1,		// マウスの右ボタン
		DIMS_BUTTON_2,		// マウスのホイールボタン
		DIMS_BUTTON_3,		// 以下はボタン自体がないマウスも存在
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

	bool GetPress(int nButton) {return (m_aMouseState[nButton] & 0x80) ? true : false; };			// マウスの入力情報(プレス情報)を取得
	bool GetTrigger(int nButton) { return (m_aMouseStateTrigger[nButton] & 0x80) ? true : false; };	// マウスの入力情報(トリガー情報)を取得
	bool GetRelease(int nButton) { return (m_aMouseStateRelease[nButton] & 0x80) ? true : false; };	// マウスの入力情報(リリース情報)を取得
	bool GetRepeat(int nButton) { return (m_aMouseStateRepeat[nButton] & 0x80) ? true : false; };	// マウスの入力情報(リピート情報)を取得

	LONG GetMouseAxisX(void) { return m_MouseXAxis; };	// マウスのX軸を取得
	LONG GetMouseAxisY(void) { return m_MouseYAxis; };	// マウスのY軸を取得
	LONG GetMouseAxisZ(void) { return m_MouseZAxis; };	// マウスのZ軸を取得

	POINT GetPoint(void) { return m_Point; };			// マウスの現在のカーソル座標を取得
	POINT GetPointOld(void) { return m_PointOld; };		// マウスの前回のカーソル座標を取得

	int GetDiffPointX(void) { return m_nDiffPointX; };	// マウスカーソル座標の差分を取得
	int GetDiffPointY(void) { return m_nDiffPointY; };	// マウスカーソル座標の差分を取得

private:    // このクラスだけがアクセス可能
	BYTE	m_aMouseState[DIMS_BUTTON_MAX];			// マウスの入力情報(プレス情報)
	BYTE	m_aMouseStateTrigger[DIMS_BUTTON_MAX];	// マウスの入力情報(トリガー情報)
	BYTE	m_aMouseStateRelease[DIMS_BUTTON_MAX];	// マウスの入力情報(リリース情報)
	BYTE	m_aMouseStateRepeat[DIMS_BUTTON_MAX];	// マウスの入力情報(リピート情報)
	DWORD	m_aMouseCounter[DIMS_BUTTON_MAX];		// マウスが入力されてる間を数えるカウンター
	LONG	m_MouseXAxis;							// マウスのX軸を格納する
	LONG	m_MouseYAxis;							// マウスのY軸を格納する
	LONG	m_MouseZAxis;							// マウスのZ軸を格納する
	HWND	m_hWnd;									// ウィンドウハンドル(カーソル座標を取得するために必要)
	POINT	m_Point;								// 現在のマウスのカーソル座標
	POINT	m_PointOld;								// 前回のマウスのカーソル座標
	int		m_nDiffPointX;							// マウスカーソル座標の差分
	int		m_nDiffPointY;							// マウスカーソル座標の差分
};

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// DirectInputクラスの定義
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class CDirectInput :public CInput
{
public:
	typedef enum
	{// アナログスティックの方向の種類(4方向)
		ANALOG_STICK_UP = 0,
		ANALOG_STICK_RIGHT,
		ANALOG_STICK_LEFT,
		ANALOG_STICK_DOWN,
		ANALOG_STICK_MAX
	}ANALOG_STICK;
	typedef enum
	{// 十字キーの方向の種類
		POV_UP = 0,
		POV_DOWN,
		POV_RIGHT,
		POV_LEFT,
		POV_MAX
	}POV;
	typedef enum
	{// ボタンの種類
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
		LPDIRECTINPUTDEVICE8 pDevGamePad;							// 入力デバイス(kコントローラー)へのポインタ
		DIDEVCAPS GamepadCaps;										// ゲームパッドの機能情報
		LPDIRECTINPUTEFFECT	pEffect;								// エフェクトオブジェクト

		DIJOYSTATE2 aGamePad;										// コントローラーの情報

		BYTE		aGamePadKeyState[GAMEPAD_NUM_KEY_MAX];			// ジョイスティックのプレス入力情報
		BYTE		aGamePadKeyStateTrigger[GAMEPAD_NUM_KEY_MAX];	// ジョイスティックのトリガー入力情報
		BYTE		aGamePadKeyStateRelease[GAMEPAD_NUM_KEY_MAX];	// ジョイスティックのリリース入力情報
		BYTE		aLeftAnalogStick[ANALOG_STICK_MAX];				// 左アナログスティックのプレス情報
		BYTE		aLeftAnalogStickTrigger[ANALOG_STICK_MAX];		// 左アナログスティックのトリガー情報
		BYTE		aPov[POV_MAX];									// 十字キーのプレス情報
		BYTE		aPovTrigger[POV_MAX];							// 十字キーのトリガー情報

		int			nCounterVibration;								// 振動用のカウンター
	}GamePad;

	CDirectInput();
	~CDirectInput();
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);
	void SetVibration(int nPower, int nTime);

	// ゲームパッドのボタン入力情報
	bool GetGamePadPress(int nKey);
	bool GetGamePadTrigger(int nKey);
	bool GetGamePadRelease(int nKey);

	// ゲームパッドのスティックの入力情報
	bool GetGamePadStickPress(int nKey);
	bool GetGamePadStickTrigger(int nKey);

	// ゲームパッドの十字キーの入力情報
	bool GetPovTpress(int nAngle);
	bool GetPovTtigger(int nAngle);

	GamePad *GetgamePadStick(void);
	int GetGamePadNum(void);
	static BOOL CALLBACK EnumJoysticksCallback(const DIDEVICEINSTANCE* pdidinstance, VOID* pContext);
	static BOOL CALLBACK EnumAxesCallback(const DIDEVICEOBJECTINSTANCE* pdidoi, VOID* pContext);
	BOOL CreateEffect(HWND hWnd);
private:
	static GamePad  m_aGamePad;	// ゲームパッドの情報
	static int		m_GamePadNumber;				// コールバック関数で使番号
	static int		m_GamepadAxesNumber;			// jコールバック関数で使う番号
	static DWORD	m_NumForceFeedbackAxis;		// フォースフィードバック
};

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// XInputクラスの定義
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class CXInput
{
public:
#define MIN_GAMEPAD_LEFT_THUMB_X  (25000)	// 左スティックの横方向の入力を受け付ける軸の最小値
#define MIN_GAMEPAD_LEFT_THUMB_Y  (25000)	// 左スティックの縦方向の入力を受け付ける軸の最小値

#define MIN_GAMEPAD_RIGHT_THUMB_X (25000)	// 右スティックの横方向の入力を受け付ける軸の最小値
#define MIN_GAMEPAD_RIGHT_THUMB_Y (25000)	// 右スティックの縦方向の入力を受け付ける軸の最小値

#define MIN_GAMEPAD_LEFT_TRIGGER  (130)		// LTボタンの入力を受け付ける値の最小値
#define MIN_GAMEPAD_RIGHT_TRIGGER (130)		// RTボタンの入力を受け付ける値の最小値
			//------------------------
			//  ボタンの列挙
			//------------------------
			// XINPUT_GAMEPAD_DPAD_UP          // 十字キー上入力
			// XINPUT_GAMEPAD_DPAD_DOWN        // 十字キー下入力
			// XINPUT_GAMEPAD_DPAD_LEFT        // 十字キー左入力
			// XINPUT_GAMEPAD_DPAD_RIGHT       // 十字キー右入力
			// XINPUT_GAMEPAD_START            // STARTボタン
			// XINPUT_GAMEPAD_BACK             // BACKボタン
			// XINPUT_GAMEPAD_LEFT_THUMB       // 左スティックボタン
			// XINPUT_GAMEPAD_RIGHT_THUMB      // 右スティックボタン
			// XINPUT_GAMEPAD_LEFT_SHOULDER    // LBボタン
			// XINPUT_GAMEPAD_RIGHT_SHOULDER   // RBボタン
			// XINPUT_GAMEPAD_A                // Aボタン
			// XINPUT_GAMEPAD_B                // Bボタン
			// XINPUT_GAMEPAD_X                // Xボタン
			// XINPUT_GAMEPAD_Y                // Yボタン
	typedef enum
	{
		XIJS_BUTTON_0,		// 十字キー上入力
		XIJS_BUTTON_1,		// 十字キー下入力
		XIJS_BUTTON_2,		// 十字キー左入力
		XIJS_BUTTON_3,		// 十字キー右入力
		XIJS_BUTTON_4,		// STARTボタン
		XIJS_BUTTON_5,		// BACKボタン
		XIJS_BUTTON_6,		// 左スティック押し込み入力
		XIJS_BUTTON_7,		// 右スティック押し込み入力
		XIJS_BUTTON_8,		// LBボタン
		XIJS_BUTTON_9,		// RBボタン
		XIJS_BUTTON_10,		// Aボタン
		XIJS_BUTTON_11,		// Bボタン
		XIJS_BUTTON_12,		// Xボタン
		XIJS_BUTTON_13,		// Yボタン
		XIJS_BUTTON_14,		// LTボタン
		XIJS_BUTTON_15,		// RTボタン
		XIJS_BUTTON_16,		// 左スティック上入力
		XIJS_BUTTON_17,		// 左スティック下入力
		XIJS_BUTTON_18,		// 左スティック左入力
		XIJS_BUTTON_19,		// 左スティック右入力
		XIJS_BUTTON_20,		// 右スティック上入力
		XIJS_BUTTON_21,		// 右スティック下入力
		XIJS_BUTTON_22,		// 右スティック左入力
		XIJS_BUTTON_23,		// 右スティック右入力

						   // 以下割り当てなし
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

	BYTE GetLeftTrigger(int nCntPad) { return m_bLeftTrigger[nCntPad]; };	// LTボタンの状態を取得
	BYTE GetRightTrigger(int nCntPad) { return m_bRightTrigger[nCntPad]; };	// RTボタンの状態を取得
	float GetLeftAxiz(int nCntPad) { return atan2f(m_sThumbLX[nCntPad], m_sThumbLY[nCntPad]); };	// 左スティックの向きを取得
	float GetRightAxiz(int nCntPad) { return atan2f(m_sThumbRX[nCntPad], m_sThumbRY[nCntPad]); };	// 右スティックの向きを取得
	SHORT GetThumbLX(int nCntPad) { return m_sThumbLX[nCntPad]; };	// 左スティックのX軸を取得
	SHORT GetThumbLY(int nCntPad) { return m_sThumbLY[nCntPad]; };	// 左スティックのY軸を取得
	SHORT GetThumbRX(int nCntPad) { return m_sThumbRX[nCntPad]; };	// 右スティックのX軸を取得
	SHORT GetThumbRY(int nCntPad) { return m_sThumbRY[nCntPad]; };	// 右スティックのY軸を取得

	bool GetPress(int nCntPad, XIJS_BUTTON Button) { return (m_aGamePadState[nCntPad][Button] & 0x80) ? true : false; };			// ゲームパッドの入力情報(プレス情報)を取得
	bool GetTrigger(int nCntPad, XIJS_BUTTON Button) { return (m_aGamePadStateTrigger[nCntPad][Button] & 0x80) ? true : false; };	// ゲームパッドの入力情報(トリガー情報)を取得
	bool GetRelease(int nCntPad, XIJS_BUTTON Button) { return (m_aGamePadStateRelease[nCntPad][Button] & 0x80) ? true : false; };	// ゲームパッドの入力情報(リリース情報)を取得
	bool GetRepeat(int nCntPad, XIJS_BUTTON Button) { return (m_aGamePadStateRepeat[nCntPad][Button] & 0x80) ? true : false; };		// ゲームパッドの入力情報(リピート情報)を取得
	bool GetAnyButton(int nCntPad);

private:
	typedef struct
	{
		XINPUT_STATE m_State;	// ゲームパッドの状態
		bool m_bConnected;		// 接続されたかどうか
	}CONTROLER_STATE;

	CONTROLER_STATE	m_aGamePad[MAX_DEVICE];									// ゲームパッドの情報
	DWORD			m_wButtons[MAX_DEVICE];									// ボタンの状態
	BYTE			m_bLeftTrigger[MAX_DEVICE];								// LTボタンの状態
	BYTE			m_bRightTrigger[MAX_DEVICE];							// RTボタンの状態
	SHORT			m_sThumbLX[MAX_DEVICE];									// 左スティックのX軸
	SHORT			m_sThumbLY[MAX_DEVICE];									// 左スティックのY軸
	SHORT			m_sThumbRX[MAX_DEVICE];									// 右スティックのX軸
	SHORT			m_sThumbRY[MAX_DEVICE];									// 右スティックのY軸
	BYTE			m_aGamePadState[MAX_DEVICE][XIJS_BUTTON_MAX];			// ゲームパッドの入力情報(プレス情報)
	BYTE			m_aGamePadStateTrigger[MAX_DEVICE][XIJS_BUTTON_MAX];	// ゲームパッドの入力情報(トリガー情報)
	BYTE			m_aGamePadStateRelease[MAX_DEVICE][XIJS_BUTTON_MAX];	// ゲームパッドの入力情報(リリース情報)
	BYTE			m_aGamePadStateRepeat[MAX_DEVICE][XIJS_BUTTON_MAX];		// ゲームパッドの入力情報(リピート情報)
	DWORD			m_aGamePadCounter[MAX_DEVICE][XIJS_BUTTON_MAX];			// ゲームパッドの入力されてる間を数えるカウンター
};
#endif