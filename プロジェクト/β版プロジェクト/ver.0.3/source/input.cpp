//=============================================================================
//
// 入力の処理[input.cpp]
// Author : Takuto Ishida
//
//=============================================================================
#include "input.h"
#include "manager.h"
#include "renderer.h"
#include "game.h"
#include "tutorial.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define REPEAT_FRAME	(20)	// リピート情報を起動し始めるフレーム数の合計
#define REPEAT_TRIGGER	(5)		// リピート情報を起動するフレームの間隔

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************
LPDIRECTINPUT8	CInput::m_pInput = NULL;		// DirectInputオブジェクトへのポインタ
HWND			CInput::m_hWnd = 0;				// ウィンドウハンドル

 //*****************************************************************************
 // CInputの処理
 //*****************************************************************************
 //=============================================================================
 // コンストラクタ
 //=============================================================================
CInput::CInput(int nNumDevice)
{
	m_pDevice = NULL;           // 入力デバイスへのポインタ
	m_nDeviceNum = nNumDevice;  // 入力デバイスの生成数
}

//=============================================================================
// デストラクタ
//=============================================================================
CInput::~CInput()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CInput::Init(HINSTANCE hInstance, HWND hWnd)
{
	// DirectInputオブジェクトの生成
	if (m_pInput == NULL)
	{// まだ作られていない
		if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInput, NULL)))
		{
			return E_FAIL;
		}
		m_hWnd = hWnd;
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CInput::Uninit(void)
{
	// 入力デバイスの開放
	if (m_pDevice != NULL)
	{
		for (int nCntDevice = 0; nCntDevice < m_nDeviceNum; nCntDevice++)
		{// デバイスを確保した数だけ繰り返し
			if (m_pDevice[nCntDevice] != NULL)
			{// デバイスが確保されている
				m_pDevice[nCntDevice]->Unacquire();   // デバイスへのアクセス権を放棄
				m_pDevice[nCntDevice]->Release();
				m_pDevice[nCntDevice] = NULL;
			}
		}
		// メモリの開放
		delete m_pDevice;
		m_pDevice = NULL;
	}

	// DirectInputオブジェクトの開放
	if (m_pInput != NULL)
	{
		m_pInput->Release();
		m_pInput = NULL;
	}
}

//*****************************************************************************
// CInputKeyboardの処理
//*****************************************************************************
//=============================================================================
// コンストラクタ
//=============================================================================
CInputKeyboard::CInputKeyboard(int nNumDevice) : CInput(nNumDevice)
{
	for (int nCntKeyboard = 0; nCntKeyboard < NUM_KEY_MAX; nCntKeyboard++)
	{// キーボードのキーの数だけ繰り返し
		// キーボードの情報を初期化
		m_aKeyState[nCntKeyboard] = 0x00;			// プレス情報
		m_aKeyStateTrigger[nCntKeyboard] = 0x00;	// トリガー情報
		m_aKeyStateRelease[nCntKeyboard] = 0x00;	// リリース情報
		m_aKeyStateRepeat[nCntKeyboard] = 0x00;		// リピート情報
		m_aKeyboardCounter[nCntKeyboard] = 0;		// 入力カウンター
	}
}

//=============================================================================
// デストラクタ
//=============================================================================
CInputKeyboard::~CInputKeyboard()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CInputKeyboard::Init(HINSTANCE hInstance, HWND hWnd)
{
	// 入力オブジェクトの作成
	CInput::Init(hInstance, hWnd);

	// 入力デバイスを動的に確保する
	m_pDevice = new LPDIRECTINPUTDEVICE8[m_nDeviceNum];
	for (int nCntDevice = 0; nCntDevice < m_nDeviceNum; nCntDevice++)
	{// 生成するデバイスの数だけ繰り返し
		m_pDevice[nCntDevice] = NULL;
	}

	// 入力デバイスが生成されていない
	for (int nCntDevice = 0; nCntDevice < m_nDeviceNum; nCntDevice++)
	{// デバイスの生成数だけ繰り返し
		if (m_pDevice[nCntDevice] == NULL)
		{// メモリが確保できた
			// 入力デバイス(キーボード)の生成
			if (FAILED(m_pInput->CreateDevice(GUID_SysKeyboard, &m_pDevice[nCntDevice], NULL)))
			{
				return E_FAIL;
			}

			// データフォーマットを設定
			if (FAILED(m_pDevice[nCntDevice]->SetDataFormat(&c_dfDIKeyboard)))
			{
				return E_FAIL;
			}

			// 協調モードを設定
			if (FAILED(m_pDevice[nCntDevice]->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
			{
				return E_FAIL;
			}

			// キーボードへのアクセス権を獲得
			m_pDevice[nCntDevice]->Acquire();
		}
	}
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CInputKeyboard::Uninit(void)
{
	// Inputクラスの開放
	CInput::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CInputKeyboard::Update(void)
{
	BYTE aKeyState[NUM_KEY_MAX]; // キーボードの入力情報

	// 入力デバイス(キーボード)からデータを取得
	for (int nCntDevice = 0; nCntDevice < m_nDeviceNum; nCntDevice++)
	{// デバイスの生成数だけ繰り返し
		if (m_pDevice[nCntDevice] != NULL)
		{// 生成できた
			if (SUCCEEDED(m_pDevice[nCntDevice]->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
			{// デバイスの入力状態を取得できた
				for (int nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
				{// キーボードのキーの数だけ繰り返し
					m_aKeyStateTrigger[nCntKey] = (m_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & aKeyState[nCntKey];   // キーボードの入力情報(トリガー情報)保存
					m_aKeyStateRelease[nCntKey] = (m_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & m_aKeyState[nCntKey]; // キーボードの入力情報(リリース情報)保存
					if (aKeyState[nCntKey] == 0x80)
					{// キーボードが入力されている
						m_aKeyboardCounter[nCntKey]++;    // カウンターを進める
						if (m_aKeyboardCounter[nCntKey] >= REPEAT_FRAME || 1 == m_aKeyboardCounter[nCntKey])
						{// 20フレーム分キーが押されている
							if (m_aKeyboardCounter[nCntKey] % REPEAT_TRIGGER == 0 || 1 == m_aKeyboardCounter[nCntKey])
							{// 3フレームごと
								m_aKeyStateRepeat[nCntKey] = 0x80;	// キーボードの入力情報(リピート情報)を起動
							}
							else
							{// それ以外のフレーム
								m_aKeyStateRepeat[nCntKey] = 0;		// キーボードの入力情報(リピート情報)を一時停止
							}
						}
						else
						{// 20フレーム以前
							m_aKeyStateRepeat[nCntKey] = 0;			// キーボードの入力情報(リピート情報)を一時停止
						}
					}
					else
					{// キーボードが入力されていない
						m_aKeyboardCounter[nCntKey] = 0;		// カウンターを戻す
						m_aKeyStateRepeat[nCntKey] = 0;			// キーボードの入力情報(リピート情報)を停止
					}
					m_aKeyState[nCntKey] = aKeyState[nCntKey];	// キーボードの入力情報(プレス情報)保存
				}
			}
			else
			{// デバイスの入力状態を取得できなかった
				m_pDevice[nCntDevice]->Acquire();				// キーボードへのアクセス権を獲得
			}
		}
	}
}

//=============================================================================
// いずれかのキーの入力情報を取得
//=============================================================================
bool CInputKeyboard::GetAnyKey(void)
{
	bool bKey = false;

	for (int nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
	{//キーボードの入力情報保存
		//トリガー(一回だけ)
		bKey = (m_aKeyStateTrigger[nCntKey] & 0x80) ? true : false;

		if (bKey == true)
		{// 押されているキーがあればブレイクする
			break;
		}
	}

	return bKey;
}

/****************************************************************************/
/*							CInputMouseの処理								*/
/****************************************************************************/
//=============================================================================
// コンストラクタ
//=============================================================================
CInputMouse::CInputMouse(int nNumDevice) : CInput(nNumDevice)
{
	for (int nCntMouse = 0; nCntMouse < DIMS_BUTTON_MAX; nCntMouse++)
	{// マウスのボタンの数だけ繰り返し
		// マウスの情報を初期化
		m_aMouseState[nCntMouse] = 0x00;		// プレス情報
		m_aMouseStateTrigger[nCntMouse] = 0x00;	// トリガー情報
		m_aMouseStateRelease[nCntMouse] = 0x00;	// リリース情報
		m_aMouseStateRepeat[nCntMouse] = 0x00;	// リピート情報
		m_aMouseCounter[nCntMouse] = 0;			// 入力カウンター
	}

	m_MouseXAxis = 0;	// マウスのX軸
	m_MouseYAxis = 0;	// マウスのY軸
	m_MouseZAxis = 0;	// マウスのZ軸
	m_nDiffPointX = 0;	// 前回の座標との差分
	m_nDiffPointY = 0;	// 前回の座標との差分
}

//=============================================================================
// デストラクタ
//=============================================================================
CInputMouse::~CInputMouse()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CInputMouse::Init(HINSTANCE hInstance, HWND hWnd)
{
	// 入力オブジェクトの作成
	CInput::Init(hInstance, hWnd);

	// 入力デバイスを動的に確保する
	m_pDevice = new LPDIRECTINPUTDEVICE8[m_nDeviceNum];
	for (int nCntDevice = 0; nCntDevice < m_nDeviceNum; nCntDevice++)
	{// 生成するデバイスの数だけ繰り返し
		m_pDevice[nCntDevice] = NULL;
	}

	// 入力デバイスが生成されていない
	for (int nCntDevice = 0; nCntDevice < m_nDeviceNum; nCntDevice++)
	{// デバイスの生成数だけ繰り返し
		if (m_pDevice[nCntDevice] == NULL)
		{// メモリが確保できた
			// 入力デバイス(マウス)の生成
			if (FAILED(m_pInput->CreateDevice(GUID_SysMouse, &m_pDevice[nCntDevice], NULL)))
			{
				return E_FAIL;
			}

			// データフォーマットを設定
			if (FAILED(m_pDevice[nCntDevice]->SetDataFormat(&c_dfDIMouse2)))
			{
				return E_FAIL;
			}

			// 協調モードを設定
			if (FAILED(m_pDevice[nCntDevice]->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
			{
				return E_FAIL;
			}

			// 軸の設定
			DIPROPDWORD diprop;
			diprop.diph.dwSize = sizeof(diprop);
			diprop.diph.dwHeaderSize = sizeof(diprop.diph);
			diprop.diph.dwObj = 0;
			diprop.diph.dwHow = DIPH_DEVICE;
			diprop.dwData = DIPROPAXISMODE_REL;	// 相対値モードで設定（絶対値はDIPROPAXISMODE_ABS）
			m_pDevice[nCntDevice]->SetProperty(DIPROP_AXISMODE, &diprop.diph);

			// ウィンドウハンドルを記憶しておく
			m_hWnd = hWnd;

			// マウスへのアクセス権を獲得
			m_pDevice[nCntDevice]->Acquire();
		}
	}
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CInputMouse::Uninit(void)
{
	// Inputクラスの開放
	CInput::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CInputMouse::Update(void)
{
	DIMOUSESTATE2 aMouseState;	// マウスの入力情報

	// 入力デバイス(マウス)からデータを取得
	for (int nCntDevice = 0; nCntDevice < m_nDeviceNum; nCntDevice++)
	{// デバイスの生成数だけ繰り返し
		if (m_pDevice[nCntDevice] != NULL)
		{// 生成できた
			if (SUCCEEDED(m_pDevice[nCntDevice]->GetDeviceState(sizeof(DIMOUSESTATE2), &aMouseState)))
			{// デバイスの入力状態を取得できた
				// 軸の取得
				m_MouseXAxis = aMouseState.lX;	// マウスのX軸を格納
				m_MouseYAxis = aMouseState.lY;	// マウスのY軸を格納
				m_MouseZAxis = aMouseState.lZ;	// マウスのZ軸を格納

				// ウィンドウの矩形を取得
				RECT rect;
				GetWindowRect(m_hWnd, &rect);

				// 前回のカーソル座標を記憶
				m_PointOld = m_Point;

				// カーソル位置の絶対座標を取得
				GetCursorPos(&m_Point);
				m_nDiffPointX = (int)m_Point.x - (int)m_PointOld.x;
				m_nDiffPointY = (int)m_Point.y - (int)m_PointOld.y;

				if (CManager::MODE_GAME == CManager::GetMode() && NULL != CManager::GetGame())
				{// ゲームモード
					if (CGame::PART_ACTION == CManager::GetGame()->GetPart())
					{// アクションパート
						if (CManager::GetRenderer()->GetWindowMode())
						{// ウィンドウモード
							CDebugProc::Print("rect:%.2f %.2f %.2f %.2f", rect.left, rect.right, rect.top, rect.bottom);
							// X軸の制御
							if (rect.left + 50 > m_Point.x) { m_Point.x = rect.right - 50; }
							else if (rect.right - 50 < m_Point.x) { m_Point.x = rect.left + 50; }

							// Y軸の制御
							if (rect.top + 50 > m_Point.y) { m_Point.y = rect.bottom - 50; }
							else if (rect.bottom - 50 < m_Point.y) { m_Point.y = rect.top + 50; }
						}
						else
						{// フルスクリーン
							if (1 > m_Point.x) { m_Point.x = 1278; }
							else if (1278 < m_Point.x) { m_Point.x = 1; }

							// Y軸の制御
							if (1 > m_Point.y) { m_Point.y = 718; }
							else if (718 < m_Point.y) { m_Point.y = 1; }
						}
					}
				}
				else if (CManager::MODE_MENU == CManager::GetMode())
				{// メニューモード
					// X軸の制御
					if (rect.left + (LONG)5.0f > m_Point.x) { m_Point.x = rect.left + (LONG)5.0f; }
					else if (rect.right - (LONG)40.0f < m_Point.x) { m_Point.x = rect.right - (LONG)40.0f; }

					// Y軸の制御
					if (rect.top + (LONG)30.0f > m_Point.y) { m_Point.y = rect.top + (LONG)30.0f; }
					else if (rect.bottom - (LONG)40.0f < m_Point.y) { m_Point.y = rect.bottom - (LONG)40.0f; }
				}
				else if (CManager::MODE_TUTORIAL == CManager::GetMode() && NULL != CManager::GetTutorial())
				{// ゲームモード
					if (CTutorial::PART_ACTION == CManager::GetTutorial()->GetPart())
					{// アクションパート
						if (CManager::GetRenderer()->GetWindowMode())
						{// ウィンドウモード
							CDebugProc::Print("rect:%.2f %.2f %.2f %.2f", rect.left, rect.right, rect.top, rect.bottom);
							// X軸の制御
							if (rect.left + 50 > m_Point.x) { m_Point.x = rect.right - 50; }
							else if (rect.right - 50 < m_Point.x) { m_Point.x = rect.left + 50; }

							// Y軸の制御
							if (rect.top + 50 > m_Point.y) { m_Point.y = rect.bottom - 50; }
							else if (rect.bottom - 50 < m_Point.y) { m_Point.y = rect.top + 50; }
						}
						else
						{// フルスクリーン
							if (1 > m_Point.x) { m_Point.x = 1278; }
							else if (1278 < m_Point.x) { m_Point.x = 1; }

							// Y軸の制御
							if (1 > m_Point.y) { m_Point.y = 718; }
							else if (718 < m_Point.y) { m_Point.y = 1; }
						}
					}
				}
				// さらにサイズ変更を考慮して、現在のサイズで補正（枠サイズ等あるので厳密ではない）
				m_Point.x = LONG((FLOAT)m_Point.x * ((FLOAT)rect.right / (FLOAT)rect.right));
				m_Point.y = LONG((FLOAT)m_Point.y * ((FLOAT)rect.bottom / (FLOAT)rect.bottom));

				// カーソル位置の設定
				SetCursorPos(m_Point.x, m_Point.y);

				for (int nCntMouse = 0; nCntMouse < DIMS_BUTTON_MAX; nCntMouse++)
				{// マウスのボタンの数だけ繰り返し
					m_aMouseStateTrigger[nCntMouse] = (m_aMouseState[nCntMouse] ^ aMouseState.rgbButtons[nCntMouse]) & aMouseState.rgbButtons[nCntMouse];	// マウスの入力情報(トリガー情報)保存
					m_aMouseStateRelease[nCntMouse] = (m_aMouseState[nCntMouse] ^ aMouseState.rgbButtons[nCntMouse]) & m_aMouseState[nCntMouse];			// マウスの入力情報(リリース情報)保存
					if (aMouseState.rgbButtons[nCntMouse] == 0x80)
					{// マウスが入力されている
						m_aMouseCounter[nCntMouse]++;    // カウンターを進める
						if (m_aMouseCounter[nCntMouse] >= REPEAT_FRAME)
						{// 一定フレーム分キーが押されている
							if (m_aMouseCounter[nCntMouse] % REPEAT_TRIGGER == 0)
							{// 一定フレームごと
								m_aMouseStateRepeat[nCntMouse] = 0x80;	// マウスの入力情報(リピート情報)を起動
							}
							else
							{// それ以外のフレーム
								m_aMouseStateRepeat[nCntMouse] = 0;		// マウスの入力情報(リピート情報)を一時停止
							}
						}
					}
					else
					{// 入力されていない
						m_aMouseCounter[nCntMouse] = 0;			// カウンターを戻す
						m_aMouseStateRepeat[nCntMouse] = 0;		// マウスの入力情報(リピート情報)を停止
					}
					m_aMouseState[nCntMouse] = aMouseState.rgbButtons[nCntMouse];	// マウスの入力情報(プレス情報)保存
				}
			}
			else
			{// デバイスの入力状態を取得できなかった
				m_pDevice[nCntDevice]->Acquire();	// マウスへのアクセス権を獲得
			}
		}
	}
}

//****************************************************************************
//
//							DirectInputの処理
//****************************************************************************
CDirectInput::GamePad CDirectInput::m_aGamePad;
int		   	   CDirectInput::m_GamePadNumber = 0;				// joystick用コールバック関数で使うジョイコンの番号
int		   	   CDirectInput::m_GamepadAxesNumber = 0;			// joystickの機能コールバック関数で使うジョイコンの番号
DWORD			CDirectInput::m_NumForceFeedbackAxis = 0;
//==============================================================================================================================
// コンストラクタ
//==============================================================================================================================
CDirectInput::CDirectInput()
{
}
//==============================================================================================================================
// デストラクタ
//==============================================================================================================================
CDirectInput::~CDirectInput()
{

}
//==============================================================================================================================
// ゲームパッドの初期化
//==============================================================================================================================
HRESULT CDirectInput::Init(HINSTANCE hInstance, HWND hWnd)
{

	m_aGamePad.pDevGamePad = NULL;

	m_GamePadNumber = 0;
	m_GamepadAxesNumber = 0;

	HRESULT hr;
	// オブジェクト生成
	CInput::Init(hInstance, hWnd);

	// 入力デバイス(コントローラー)の生成
	if (FAILED(m_pInput->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumJoysticksCallback, NULL, DIEDFL_ATTACHEDONLY)))		// 第4引数 (ATTACHEDONLY = 現在接続されているジョイスティックのみ検出する)
	{
		return FALSE;
	}

	// フォースフィードバック機能のある入力デバイスの生成
	if (FAILED(m_pInput->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumJoysticksCallback, NULL, DIEDFL_ATTACHEDONLY | DIEDFL_FORCEFEEDBACK)))
	{
		return E_FAIL;
	}

	if (m_aGamePad.pDevGamePad != NULL)
	{// コントローラーが繋がっていないとき処理しない
	 // デバイスにデータフォーマットを設定
		if (FAILED(hr = m_aGamePad.pDevGamePad->SetDataFormat(&c_dfDIJoystick2)))
		{
			return FALSE;
		}

		// 協調モードを設定
		if (FAILED(hr = m_aGamePad.pDevGamePad->SetCooperativeLevel(hWnd, DISCL_EXCLUSIVE | DISCL_FOREGROUND)))
		{
			return FALSE;
		}

		// ゲームパッドの機能の情報を収集
		m_aGamePad.GamepadCaps.dwSize = sizeof(DIDEVCAPS);

		if (FAILED(hr = m_aGamePad.pDevGamePad->GetCapabilities(&m_aGamePad.GamepadCaps)))
		{
			m_aGamePad.pDevGamePad->Release();
			return FALSE;
		}

		m_NumForceFeedbackAxis = 0;

		// 各軸のモード設定
		hr = m_aGamePad.pDevGamePad->EnumObjects(EnumAxesCallback, (void*)m_aGamePad.pDevGamePad, DIDFT_AXIS);

		m_GamepadAxesNumber++;	// 番号を進める

		if (m_NumForceFeedbackAxis > 2) m_NumForceFeedbackAxis = 2;

		// エフェクトオブジェクトの生成
		if (!(CreateEffect(hWnd)))
		{
			m_aGamePad.pEffect = NULL;
		}

		// ゲームパッドへのアクセス権を獲得(入力制御開始)
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
// ゲームパッドの終了処理
//==============================================================================================================================
void CDirectInput::Uninit(void)
{
	// 振動停止
	SetVibration(0, 0);

	// 入力デバイスの開放
	if (m_aGamePad.pDevGamePad != NULL)
	{
		m_aGamePad.pDevGamePad->Unacquire();
		m_aGamePad.pDevGamePad->Release();
		m_aGamePad.pDevGamePad = NULL;
	}

	// エフェクトオブジェクトの開放
	if (m_aGamePad.pEffect != NULL)
	{
		m_aGamePad.pEffect->Release();
		m_aGamePad.pEffect = NULL;
	}


	// DirectInputオブジェクトの開放
	CInput::Uninit();
}

//==============================================================================================================================
// ゲームパッドの更新処理
//==============================================================================================================================
void CDirectInput::Update(void)
{
	if (m_aGamePad.pDevGamePad != NULL)
	{
		DIJOYSTATE2 aGamePadKeyState;								// コントローラーの入力情報
		BYTE aLeftAnalogStick[ANALOG_STICK_MAX] = { 0,0,0,0 };		// スティックの入力情報
		BYTE aPov[POV_MAX] = { 0,0,0,0 };							// 十字キーの入力情報

		HRESULT hr;
		hr = m_aGamePad.pDevGamePad->Poll();

		// 入力デバイスからデータを取得
		if (SUCCEEDED(hr = m_aGamePad.pDevGamePad->GetDeviceState(sizeof(DIJOYSTATE2), &aGamePadKeyState)))
		{
			// ボタン
			for (int nCntKey = 0; nCntKey < GAMEPAD_NUM_KEY_MAX; nCntKey++)
			{
				// コントローラーの入力情報の保存(トリガー)
				m_aGamePad.aGamePadKeyStateTrigger[nCntKey] = (m_aGamePad.aGamePadKeyState[nCntKey] ^ aGamePadKeyState.rgbButtons[nCntKey]) & aGamePadKeyState.rgbButtons[nCntKey];
				// コントローラーの入力情報の保存(リリース)
				m_aGamePad.aGamePadKeyStateRelease[nCntKey] = (aGamePadKeyState.rgbButtons[nCntKey] ^ m_aGamePad.aGamePadKeyState[nCntKey]) & m_aGamePad.aGamePadKeyState[nCntKey];
				// コントローラーの入力情報の保存(プレス)
				m_aGamePad.aGamePadKeyState[nCntKey] = aGamePadKeyState.rgbButtons[nCntKey];
			}

			//--------------------------------------------------------
			// 左アナログスティックの入力情報の取得
			//--------------------------------------------------------
			if (aGamePadKeyState.lY < -GAMEPAD_DEADZONE * 100 && aGamePadKeyState.lX < GAMEPAD_DEADZONE * 100 && aGamePadKeyState.lX > -GAMEPAD_DEADZONE * 100)
			{// 左スティックを上に倒した
				aLeftAnalogStick[ANALOG_STICK_UP] = 128;			// 入力された
			}
			else if (aGamePadKeyState.lY > GAMEPAD_DEADZONE * 100 && aGamePadKeyState.lX < GAMEPAD_DEADZONE * 100 && aGamePadKeyState.lX > -GAMEPAD_DEADZONE * 100)
			{// 左スティックを下に倒した
				aLeftAnalogStick[ANALOG_STICK_DOWN] = 128;			// 入力された
			}
			else if (aGamePadKeyState.lX > GAMEPAD_DEADZONE * 100)
			{// 左gスティックを右に倒した
				aLeftAnalogStick[ANALOG_STICK_RIGHT] = 128;			// 入力された
			}
			else if (aGamePadKeyState.lX < -GAMEPAD_DEADZONE * 100)
			{// 左スティックを左に倒した
				aLeftAnalogStick[ANALOG_STICK_LEFT] = 128;			// 入力された
			}

			for (int nCntAnalogStick = 0; nCntAnalogStick < ANALOG_STICK_MAX; nCntAnalogStick++)
			{// 左アナログスティックの方向の種類分ループ
			 // 左スティックのトリガー情報
				m_aGamePad.aLeftAnalogStickTrigger[nCntAnalogStick] = (m_aGamePad.aLeftAnalogStick[nCntAnalogStick] ^ aLeftAnalogStick[nCntAnalogStick])&aLeftAnalogStick[nCntAnalogStick];		// 左アナログスティックのトリガー情報
																																																// 左スティックのプレス情報
				m_aGamePad.aLeftAnalogStick[nCntAnalogStick] = aLeftAnalogStick[nCntAnalogStick];																						// 左アナログスティックのプレス情報
			}
			//--------------------------------------------------------
			// 十字キーの入力情報の取得
			//--------------------------------------------------------
			if (aGamePadKeyState.rgdwPOV[0] == 0.0f)
			{// 十字キーの上を押した
				aPov[POV_UP] = 128;			// 入力された
			}
			if (aGamePadKeyState.rgdwPOV[0] == 18000.0f)
			{// 十字キーの下を押した
				aPov[POV_DOWN] = 128;			// 入力された
			}
			if (aGamePadKeyState.rgdwPOV[0] == 9000.0f)
			{// 十字キーの右を押した
				aPov[POV_RIGHT] = 128;			// 入力された
			}
			if (aGamePadKeyState.rgdwPOV[0] == 27000.0f)
			{// 十字キーの左を押した
				aPov[POV_LEFT] = 128;			// 入力された
			}

			for (int nCntPov = 0; nCntPov < POV_MAX; nCntPov++)
			{// 十字キーの方向の種類分ループ
			 // 十字キーのトリガー情報
				m_aGamePad.aPovTrigger[nCntPov] = (m_aGamePad.aPov[nCntPov] ^ aPov[nCntPov])&aPov[nCntPov];		// 十字キーのトリガー情報

																												// 十字キーのプレス情報
				m_aGamePad.aPov[nCntPov] = aPov[nCntPov];		// 十字キーのプレス情報
			}
			m_aGamePad.aGamePad = aGamePadKeyState;			// 現在の状態を保存
		}
		else
		{
			m_aGamePad.pDevGamePad->Acquire();				// ゲームパッドへのアクセス権を獲得
		}

		if (m_aGamePad.nCounterVibration > 0)
		{
			m_aGamePad.nCounterVibration--;
			if (m_aGamePad.nCounterVibration <= 0)
			{// カウンターが0になったら振動停止
				SetVibration(0, 0);
			}
		}
	}

}

//=============================================================================
// 振動の設定
//=============================================================================
void CDirectInput::SetVibration(int nPower, int nTime)
{
	if (m_aGamePad.pEffect == NULL) return;

	if (nPower > 100)
	{// 100%を超えている場合
		nPower = 100;
	}
	else if (nPower < 0)
	{// 0%未満の場合
		nPower = 0;
	}

	if (m_aGamePad.pDevGamePad != NULL)
	{// コントローラーが繋がっていないとき処理しない
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
// ゲームパッドの入力情報(スティック)を取得
//==============================================================================================================================
CDirectInput::GamePad *CDirectInput::GetgamePadStick(void)
{

	if (m_aGamePad.pDevGamePad != NULL)
	{// デバイスがNULLじゃなかったら
	 // ジョイスティックの情報を取得
		m_aGamePad.pDevGamePad->GetDeviceState(sizeof(DIJOYSTATE2), &m_aGamePad.aGamePad);
	}

	return &m_aGamePad;			// ジョイスティックの情報を返す
}

//==============================================================================================================================
// ゲームパッドの入力情報(プレス情報)を取得
//==============================================================================================================================
bool CDirectInput::GetGamePadPress(int nKey)
{
	if (m_aGamePad.pDevGamePad == NULL)
	{// 接続されていないならFALSEを返す
		return false;
	}
	return(m_aGamePad.aGamePadKeyState[nKey] & 0x80) ? true : false;
}

//==============================================================================================================================
// ゲームパッドの入力情報(トリガー情報)を取得
//==============================================================================================================================
bool CDirectInput::GetGamePadTrigger(int nKey)
{
	if (m_aGamePad.pDevGamePad == NULL)
	{// 接続されていないならFALSEを返す
		return false;
	}
	return(m_aGamePad.aGamePadKeyStateTrigger[nKey] & 0x80) ? true : false;
}

//==============================================================================================================================
// ゲームパッドの入力情報(リリース情報)を取得
//==============================================================================================================================
bool CDirectInput::GetGamePadRelease(int nKey)
{
	if (m_aGamePad.pDevGamePad == NULL)
	{// 接続されていないならFALSEを返す
		return false;
	}
	return(m_aGamePad.aGamePadKeyStateRelease[nKey] & 0x80) ? true : false;
}
//==============================================================================================================================
// ゲームパッドのスティック入力情報(プレス情報)を取得
//==============================================================================================================================
bool CDirectInput::GetGamePadStickPress(int nKey)
{
	if (m_aGamePad.pDevGamePad == NULL)
	{// 接続されていないならFALSEを返す
		return false;
	}
	return (m_aGamePad.aLeftAnalogStick[nKey] & 0x80) ? true : false;
}

//==============================================================================================================================
// ゲームパッドのスティック入力情報(トリガー情報)を取得
//==============================================================================================================================
bool CDirectInput::GetGamePadStickTrigger(int nKey)
{
	if (m_aGamePad.pDevGamePad == NULL)
	{// 接続されていないならFALSEを返す
		return false;
	}
	return (m_aGamePad.aLeftAnalogStickTrigger[nKey] & 0x80) ? true : false;
}

//=============================================================================
// 十字キー入力情報(プレスー情報)を取得
//=============================================================================
bool CDirectInput::GetPovTpress(int nAngle)
{
	if (m_aGamePad.pDevGamePad == NULL)
	{// 接続されていないならFALSEを返す
		return false;
	}
	return (m_aGamePad.aPov[nAngle] & 0x80) ? true : false;
}

//=============================================================================
// 十字キー入力情報(トリガー情報)を取得
//=============================================================================
bool CDirectInput::GetPovTtigger(int nAngle)
{
	if (m_aGamePad.pDevGamePad == NULL)
	{// 接続されていないならFALSEを返す
		return false;
	}
	return (m_aGamePad.aPovTrigger[nAngle] & 0x80) ? true : false;
}

//*****************************************************************************
// joystick用コールバック関数
//*****************************************************************************
BOOL CALLBACK CDirectInput::EnumJoysticksCallback(const DIDEVICEINSTANCE* pdidinstance, VOID* pContext)
{
	HRESULT hr;

	// 列挙されたジョイスティックへのインターフェイスを取得
	hr = m_pInput->CreateDevice(pdidinstance->guidInstance, &m_aGamePad.pDevGamePad, NULL);

	// デバイスを検出出来なかった場合
	if (FAILED(hr))
	{
		return DIENUM_CONTINUE;
	}

	m_GamePadNumber++;		// 番号を進める

	return DIENUM_CONTINUE;
}

//*****************************************************************************
// joystickの機能コールバック関数
//*****************************************************************************
BOOL CALLBACK CDirectInput::EnumAxesCallback(const DIDEVICEOBJECTINSTANCE *pdidoi, VOID *pContext)
{
	HRESULT hr;
	DIPROPRANGE diprg;

	diprg.diph.dwSize = sizeof(DIPROPRANGE);
	diprg.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	diprg.diph.dwHow = DIPH_BYID;
	diprg.diph.dwObj = pdidoi->dwType;
	diprg.lMin = -(GAMEPAD_DEADZONE / GAMEPAD_DEADZONE * MAX_ZONE);				// 十字キーの最小値 [上] [左]
	diprg.lMax = GAMEPAD_DEADZONE / GAMEPAD_DEADZONE * MAX_ZONE;				// 十字キーの最大値 [下] [右]

																				// デバイスに設定
	hr = m_aGamePad.pDevGamePad->SetProperty(DIPROP_RANGE, &diprg.diph);

	if (FAILED(hr))
	{
		return DIENUM_STOP;
	}

	if ((pdidoi->dwFlags & DIDOI_FFACTUATOR) != 0)
	{// フォースフィードバックの軸をカウント
		m_NumForceFeedbackAxis++;
	}

	return DIENUM_CONTINUE;
}

//*****************************************************************************
// エフェクトオブジェクトの生成
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
// ゲームパッドの刺さっている数の取得
//*****************************************************************************
int CDirectInput::GetGamePadNum(void)
{
	return m_GamePadNumber;
}

/****************************************************************************/
/*								XInputの処理								*/
//***************************************************************************/
//=============================================================================
// コンストラクタ
//=============================================================================
CXInput::CXInput()
{
	// 各種値のクリア
	for (int nCntPad = 0; nCntPad < MAX_DEVICE; nCntPad++)
	{// 生成できるデバイスの数だけ繰り返し
		for (int nCntButton = 0; nCntButton < XIJS_BUTTON_MAX; nCntButton++)
		{// ボタンの数だけ繰り返し
			m_aGamePadState[nCntPad][nCntButton] = 0x00;          // プレス情報
			m_aGamePadStateTrigger[nCntPad][nCntButton] = 0x00;   // トリガー情報
			m_aGamePadStateRelease[nCntPad][nCntButton] = 0x00;   // リリース情報
			m_aGamePadStateRepeat[nCntPad][nCntButton] = 0x00;    // リピート情報
			m_aGamePadCounter[nCntPad][nCntButton] = 0;           // 入力カウンター
		}
		m_aGamePad[nCntPad].m_bConnected = false;                 // 接続の状態
		m_aGamePad[nCntPad].m_State.Gamepad.wButtons = 0;         // ボタンのビット情報
		m_aGamePad[nCntPad].m_State.Gamepad.bLeftTrigger = 0;     // LTボタンの状態
		m_aGamePad[nCntPad].m_State.Gamepad.bRightTrigger = 0;    // RTボタンの状態
		m_aGamePad[nCntPad].m_State.Gamepad.sThumbLX = 0;         // 左スティックのX軸
		m_aGamePad[nCntPad].m_State.Gamepad.sThumbLY = 0;         // 左スティックのY軸
		m_aGamePad[nCntPad].m_State.Gamepad.sThumbRX = 0;         // 右スティックのX軸
		m_aGamePad[nCntPad].m_State.Gamepad.sThumbRY = 0;         // 右スティックのY軸

		m_bLeftTrigger[nCntPad] = 0;                              // LTボタンの状態
		m_bRightTrigger[nCntPad] = 0;                             // RTボタンの状態
		m_sThumbLX[nCntPad] = 0;                                  // 左スティックのX軸の値
		m_sThumbLY[nCntPad] = 0;                                  // 左スティックのY軸の値
		m_sThumbRX[nCntPad] = 0;                                  // 右スティックのX軸の値
		m_sThumbRY[nCntPad] = 0;                                  // 右スティックのY軸の値
		m_wButtons[nCntPad] = 0;                                  // ボタンの状態
	}
}

//=============================================================================
// デストラクタ
//=============================================================================
CXInput::~CXInput()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CXInput::Init(void)
{
	// ゲームパッドのクリア
	ZeroMemory(m_aGamePad, sizeof(CONTROLER_STATE) * MAX_DEVICE);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CXInput::Uninit(void)
{
	// ゲームパッドの開放
	XInputEnable(false);
}

//=============================================================================
// 更新処理
//=============================================================================
void CXInput::Update(void)
{
	DWORD dwResult;                             // ゲームパッドが取得できたかどうか
	BYTE aGamePadState[XIJS_BUTTON_MAX] = {};   // ゲームパッドのボタン入力情報
	for (DWORD nCntPad = 0; nCntPad < MAX_DEVICE; nCntPad++)
	{// 接続できるゲームパッド(最大4つまで)の数だけ繰り返し
	 // ゲームパッドの状態を取得
		dwResult = XInputGetState(nCntPad, &m_aGamePad[nCntPad].m_State);
		if (dwResult == ERROR_SUCCESS)
		{// 状態取得に成功した
		 // 状態を格納する
			m_aGamePad[nCntPad].m_bConnected = true;                                       // 接続された状態にする
			m_wButtons[nCntPad] = m_aGamePad[nCntPad].m_State.Gamepad.wButtons;            // ボタンのビット情報を取得
			m_bLeftTrigger[nCntPad] = m_aGamePad[nCntPad].m_State.Gamepad.bLeftTrigger;    // LTボタンの状態を取得
			m_bRightTrigger[nCntPad] = m_aGamePad[nCntPad].m_State.Gamepad.bRightTrigger;  // RTボタンの状態を取得
			m_sThumbLX[nCntPad] = m_aGamePad[nCntPad].m_State.Gamepad.sThumbLX;            // 左スティックのX軸を取得
			m_sThumbLY[nCntPad] = m_aGamePad[nCntPad].m_State.Gamepad.sThumbLY;            // 左スティックのY軸を取得
			m_sThumbRX[nCntPad] = m_aGamePad[nCntPad].m_State.Gamepad.sThumbRX;            // 右スティックのX軸を取得
			m_sThumbRY[nCntPad] = m_aGamePad[nCntPad].m_State.Gamepad.sThumbRY;            // 右スティックのY軸を取得

																						   // 入力の状態を取得する
																						   // ボタン
			DWORD wButtons = m_wButtons[nCntPad]; // ボタンのビット情報
			if (wButtons >= XINPUT_GAMEPAD_Y)
			{// Yボタンが押されている
				aGamePadState[XIJS_BUTTON_13] = 0x80;   // 入力を起動
				wButtons -= XINPUT_GAMEPAD_Y;
			}
			if (wButtons >= XINPUT_GAMEPAD_X)
			{// Xボタンが押されている
				aGamePadState[XIJS_BUTTON_12] = 0x80;   // 入力を起動
				wButtons -= XINPUT_GAMEPAD_X;
			}
			if (wButtons >= XINPUT_GAMEPAD_B)
			{// Bボタンが押されている
				aGamePadState[XIJS_BUTTON_11] = 0x80;   // 入力を起動
				wButtons -= XINPUT_GAMEPAD_B;
			}
			if (wButtons >= XINPUT_GAMEPAD_A)
			{// Aボタンが押されている
				aGamePadState[XIJS_BUTTON_10] = 0x80;   // 入力を起動
				wButtons -= XINPUT_GAMEPAD_A;
			}
			if (wButtons >= XINPUT_GAMEPAD_RIGHT_SHOULDER)
			{// RBボタンが押されている
				aGamePadState[XIJS_BUTTON_9] = 0x80;	// 入力を起動
				wButtons -= XINPUT_GAMEPAD_RIGHT_SHOULDER;
			}
			if (wButtons >= XINPUT_GAMEPAD_LEFT_SHOULDER)
			{// LBボタンが押されている
				aGamePadState[XIJS_BUTTON_8] = 0x80;	// 入力を起動
				wButtons -= XINPUT_GAMEPAD_LEFT_SHOULDER;
			}
			if (wButtons >= XINPUT_GAMEPAD_RIGHT_THUMB)
			{// 右スティックが押し込まれている
				aGamePadState[XIJS_BUTTON_7] = 0x80;	// 入力を起動
				wButtons -= XINPUT_GAMEPAD_RIGHT_THUMB;
			}
			if (wButtons >= XINPUT_GAMEPAD_LEFT_THUMB)
			{// 右スティックが押し込まれている
				aGamePadState[XIJS_BUTTON_6] = 0x80;	// 入力を起動
				wButtons -= XINPUT_GAMEPAD_LEFT_THUMB;
			}
			if (wButtons >= XINPUT_GAMEPAD_BACK)
			{// BACKボタンが押されている
				aGamePadState[XIJS_BUTTON_5] = 0x80;	// 入力を起動
				wButtons -= XINPUT_GAMEPAD_BACK;
			}
			if (wButtons >= XINPUT_GAMEPAD_START)
			{// BACKボタンが押されている
				aGamePadState[XIJS_BUTTON_4] = 0x80;	// 入力を起動
				wButtons -= XINPUT_GAMEPAD_START;
			}
			if (wButtons >= XINPUT_GAMEPAD_DPAD_RIGHT)
			{// 十字キーの右が押されている
				aGamePadState[XIJS_BUTTON_3] = 0x80;	// 入力を起動
				wButtons -= XINPUT_GAMEPAD_DPAD_RIGHT;
			}
			if (wButtons >= XINPUT_GAMEPAD_DPAD_LEFT)
			{// 十字キーの左が押されている
				aGamePadState[XIJS_BUTTON_2] = 0x80;	// 入力を起動
				wButtons -= XINPUT_GAMEPAD_DPAD_LEFT;
			}
			if (wButtons >= XINPUT_GAMEPAD_DPAD_DOWN)
			{// 十字キーの下が押されている
				aGamePadState[XIJS_BUTTON_1] = 0x80;	// 入力を起動
				wButtons -= XINPUT_GAMEPAD_DPAD_DOWN;
			}
			if (wButtons >= XINPUT_GAMEPAD_DPAD_UP)
			{// 十字キーの上が押されている
				aGamePadState[XIJS_BUTTON_0] = 0x80;	// 入力を起動
			}

			// LTトリガー
			if (m_bLeftTrigger[nCntPad] >= MIN_GAMEPAD_LEFT_TRIGGER)
			{// LTトリガーが押されている
				aGamePadState[XIJS_BUTTON_14] = 0x80;   // 入力を起動する
			}
			// RTトリガー
			if (m_bRightTrigger[nCntPad] >= MIN_GAMEPAD_RIGHT_TRIGGER)
			{// LRトリガーが押されている
				aGamePadState[XIJS_BUTTON_15] = 0x80;   // 入力を起動する
			}

			// 左スティック
			if (m_sThumbLX[nCntPad] >= MIN_GAMEPAD_LEFT_THUMB_X)
			{// 左スティックが右に倒された
				aGamePadState[XIJS_BUTTON_19] = 0x80;   // 入力を起動する
			}
			if (m_sThumbLX[nCntPad] <= -MIN_GAMEPAD_LEFT_THUMB_X)
			{// 左スティックが左に倒された
				aGamePadState[XIJS_BUTTON_18] = 0x80;   // 入力を起動する
			}
			if (m_sThumbLY[nCntPad] >= MIN_GAMEPAD_LEFT_THUMB_Y)
			{// 左スティックが上に倒された
				aGamePadState[XIJS_BUTTON_16] = 0x80;   // 入力を起動する
			}
			if (m_sThumbLY[nCntPad] <= -MIN_GAMEPAD_LEFT_THUMB_Y)
			{// 左スティックが下に倒された
				aGamePadState[XIJS_BUTTON_17] = 0x80;   // 入力を起動する
			}

			// 右スティック
			if (m_sThumbRX[nCntPad] >= MIN_GAMEPAD_LEFT_THUMB_X)
			{// 右スティックが右に倒された
				aGamePadState[XIJS_BUTTON_23] = 0x80;   // 入力を起動する
			}
			if (m_sThumbRX[nCntPad] <= -MIN_GAMEPAD_LEFT_THUMB_X)
			{// 右スティックが左に倒された
				aGamePadState[XIJS_BUTTON_22] = 0x80;   // 入力を起動する
			}
			if (m_sThumbRY[nCntPad] >= MIN_GAMEPAD_LEFT_THUMB_Y)
			{// 右スティックが上に倒された
				aGamePadState[XIJS_BUTTON_20] = 0x80;   // 入力を起動する
			}
			if (m_sThumbRY[nCntPad] <= -MIN_GAMEPAD_LEFT_THUMB_Y)
			{// 右スティックが下に倒された
				aGamePadState[XIJS_BUTTON_21] = 0x80;   // 入力を起動する
			}

			// ボタンの状態を格納する
			for (int nCntButton = 0; nCntButton < XIJS_BUTTON_MAX; nCntButton++)
			{// ボタンの数だけ繰り返し
				m_aGamePadStateTrigger[nCntPad][nCntButton] = (m_aGamePadState[nCntPad][nCntButton] ^ aGamePadState[nCntButton]) & aGamePadState[nCntButton];            // ボタンの入力情報(トリガー情報)保存
				m_aGamePadStateRelease[nCntPad][nCntButton] = (m_aGamePadState[nCntPad][nCntButton] ^ aGamePadState[nCntButton]) & m_aGamePadState[nCntPad][nCntButton]; // ボタンの入力情報(リリース情報)保存
				if (aGamePadState[nCntButton] == 0x80)
				{// ボタンが入力されている
					m_aGamePadCounter[nCntPad][nCntButton]++;    // カウンターを進める
					if (m_aGamePadCounter[nCntPad][nCntButton] >= REPEAT_FRAME || 1 == m_aGamePadCounter[nCntPad][nCntButton])
					{// 20フレーム分ボタンが押されている
						if (0 == m_aGamePadCounter[nCntPad][nCntButton] % REPEAT_TRIGGER || 1 == m_aGamePadCounter[nCntPad][nCntButton])
						{// 3フレームごと
							m_aGamePadStateRepeat[nCntPad][nCntButton] = 0x80;   // ボタンの入力情報(リピート情報)を起動
						}
						else
						{// それ以外のフレーム
							m_aGamePadStateRepeat[nCntPad][nCntButton] = 0;      // ボタンの入力情報(リピート情報)を一時停止
						}
					}
					else
					{// 20フレーム以前
						m_aGamePadStateRepeat[nCntPad][nCntButton] = 0;      // ボタンの入力情報(リピート情報)を一時停止
					}
				}
				else
				{// ゲームパッドのボタンが入力されていない
					m_aGamePadCounter[nCntPad][nCntButton] = 0;        // カウンターを戻す
					m_aGamePadStateRepeat[nCntPad][nCntButton] = 0;    // ボタンの入力情報(リピート情報)を停止
				}
				m_aGamePadState[nCntPad][nCntButton] = aGamePadState[nCntButton];  // ボタンの入力情報(プレス情報)保存
			}
		}
		else
		{// 状態取得に失敗した
			m_aGamePad[nCntPad].m_bConnected = false;  // 接続されていない状態にする
		}
	}
}

//=============================================================================
// ゲームパッドの入力情報を取得
//=============================================================================
bool CXInput::GetAnyButton(int nCntPad)
{
	for (int nCount = 0; nCount < XIJS_BUTTON_16; nCount++)
	{// スティック以外のボタンを調べる
		if (m_aGamePadStateTrigger[nCntPad][nCount] == 0x80)
		{// 入力された
			return true;
		}
	}

	// 入力されたボタンがなかった
	return false;
}
