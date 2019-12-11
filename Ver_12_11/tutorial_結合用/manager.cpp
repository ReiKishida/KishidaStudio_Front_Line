//=============================================================================
//
// マネージャー処理 [manager.cpp]
// Author : Takuto Ishida
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "input.h"
#include "camera.h"
#include "light.h"
#include "debugProc.h"
#include "title.h"
#include "menu.h"
#include "tutorial.h"
#include "game.h"
#include "result.h"
#include "pause.h"
#include "meshField.h"
#include "sound.h"
#include "texture.h"
#include "mechaSelect.h"
#include "server.h"
#include "matching.h"
#include "particle.h"
#include "tutorialmenu.h"

#include <stdio.h>
//=========================================
// マクロ定義
//=========================================
#define SYSTEM_FILE	"data/TEXT/system.ini"		// 地面の情報テキスト

//=========================================
// 静的メンバ変数
//=========================================
CRenderer			*CManager::m_pRenderer = NULL;			// レンダラークラスのポインタ変数
CInputKeyboard		*CManager::m_pInputKeyboard = NULL;		// インプットクラスのポインタ変数
CInputMouse			*CManager::m_pInputMouse = NULL;		// マウスクラスのポインタ変数
CDirectInput		*CManager::m_pDirectInput = NULL;		// DirectInputクラス型のポインタ
CXInput				*CManager::m_pXInput = NULL;			// XInputクラス型のポインタ
CCamera				*CManager::m_pCamera = NULL;			// カメラクラスのポインタ変数
CLight				*CManager::m_pLight = NULL;				// ライトクラスのポインタ変数
CDebugProc			*CManager::m_pDebugProc = NULL;			// デバッグ文字クラスのポインタ変数
CSound				*CManager::m_pSound = NULL;				// サウンドクラスのポインタ変数
CTitle				*CManager::m_pTitle = NULL;				// タイトルクラスのポインタ変数
CMenu				*CManager::m_pMenu = NULL;				// メニュークラスのポインタ変数
CMechaSelect		*CManager::m_pMechaSelect = NULL;
CMatching			*CManager::m_pMatching = NULL;			// マッチングクラスのポインタ情報
CTutorial			*CManager::m_pTutorial = NULL;			// チュートリアルクラスのポインタ変数
CTutorialMenu		*CManager::m_pTutorialMenu = NULL;		// チュートリアルクラスのポインタ変数
CGame				*CManager::m_pGame = NULL;				// ゲームクラスのポインタ変数
CResult				*CManager::m_pResult = NULL;			// リザルトクラスのポインタ変数
CRanking			*CManager::m_pRanking = NULL;			// ランキングクラスのポインタ変数
CManager::MODE		CManager::m_mode = CManager::MODE_TITLE;

int  CManager::m_nNumStage = 0;

CServer				*CManager::m_pServer = NULL;			// サーバークラスのポインタ変数
CClient				*CManager::m_pClient = NULL;			// クライアントクラスのポインタ変数

//=========================================
// コンストラクタ
//=========================================
CManager::CManager()
{
}

//=========================================
// デストラクタ
//=========================================
CManager::~CManager()
{
}

//=========================================
// 初期化処理
//=========================================
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, bool bWindow)
{
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// サーバーの初期化処理
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	CServer::Startup();

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// レンダリングクラスの生成
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	if (NULL == m_pRenderer)
	{// メモリが空
		m_pRenderer = new CRenderer;	// メモリ確保

		if (NULL != m_pRenderer)
		{// メモリが確保できた
			if (FAILED(m_pRenderer->Init(hInstance, hWnd, bWindow)))	//FALSEでフルスクリーン
			{// 初期化処理
				return -1;
			}
		}
		else
		{// メモリ確保に失敗
			MessageBox(0, "レンダリングクラスのメモリが確保できませんでした", "失敗", MB_OK);
		}
	}
	else
	{// 空でなかったとき
		MessageBox(0, "レンダリングクラスに何か入っています", "失敗", MB_OK);
	}

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// インプット(キーボード)クラスの生成
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	if (NULL == m_pInputKeyboard)
	{// メモリが空
		m_pInputKeyboard = new CInputKeyboard;	// キーボードの入力用

		if (NULL != m_pInputKeyboard)
		{// メモリ確保
			if (FAILED(m_pInputKeyboard->Init(hInstance, hWnd)))
			{// 初期化処理
				return -1;
			}
		}
		else
		{// メモリ確保に失敗
			MessageBox(0, "インプット(キーボード)クラスのメモリが確保できませんでした", "失敗", MB_OK);
		}
	}
	else
	{// 空でなかったとき
		MessageBox(0, "インプット(キーボード)クラスに何か入っています", "失敗", MB_OK);
	}


	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// インプット(キーボード)クラスの生成
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	if (NULL == m_pInputMouse)
	{// メモリが空
		m_pInputMouse = new CInputMouse;	// マウスの入力用

		if (NULL != m_pInputMouse)
		{// メモリ確保
			if (FAILED(m_pInputMouse->Init(hInstance, hWnd)))
			{// 初期化処理
				return -1;
			}
		}
		else
		{// メモリ確保に失敗
			MessageBox(0, "インプット(マウス)クラスのメモリが確保できませんでした", "失敗", MB_OK);
		}
	}
	else
	{// 空でなかったとき
		MessageBox(0, "インプット(マウス)クラスに何か入っています", "失敗", MB_OK);
	}

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// インプット(DirectInput)クラスの生成
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	if (NULL == m_pDirectInput)
	{// メモリが空
		m_pDirectInput = new CDirectInput;	// キーボードの入力用

		if (NULL != m_pDirectInput)
		{// メモリ確保
			if (FAILED(m_pDirectInput->Init(hInstance, hWnd)))
			{// 初期化処理
				return -1;
			}
		}
		else
		{// メモリ確保に失敗
			MessageBox(0, "インプット(DirectInput)クラスのメモリが確保できませんでした", "失敗", MB_OK);
		}
	}

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// インプット(XInput)クラスの生成
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	if (NULL == m_pXInput)
	{// メモリが空
		m_pXInput = new CXInput;	// キーボードの入力用

		if (NULL != m_pXInput)
		{// メモリ確保
			if (FAILED(m_pXInput->Init()))
			{// 初期化処理
				return -1;
			}
		}
		else
		{// メモリ確保に失敗
			MessageBox(0, "インプット(XInput)クラスのメモリが確保できませんでした", "失敗", MB_OK);
		}
	}
	else
	{// 空でなかったとき
		MessageBox(0, "インプット(XInput)クラスに何か入っています", "失敗", MB_OK);
	}

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// カメラクラスの生成
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	if (NULL == m_pCamera)
	{// ヌルチェック
		m_pCamera = new CCamera;	// メモリ確保

		if (NULL != m_pCamera)
		{// メモリを確保できたかどうか
			m_pCamera->Init();
		}
		else
		{// メモリ確保に失敗
			MessageBox(0, "カメラクラスのメモリが確保できませんでした", "失敗", MB_OK);
		}
	}
	else
	{// NULLでなかったとき
		MessageBox(0, "カメラクラスに何か入っています", "失敗", MB_OK);
	}

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// ライトクラスの生成
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	if (NULL == m_pLight)
	{// ヌルチェック
		m_pLight = new CLight;	// メモリ確保

		if (NULL != m_pLight)
		{// メモリを確保できたかどうか
			m_pLight->Init();
		}
		else
		{// メモリ確保に失敗
			MessageBox(0, "ライトクラスのメモリが確保できませんでした", "失敗", MB_OK);
		}
	}
	else
	{// NULLでなかったとき
		MessageBox(0, "ライトクラスに何か入っています", "失敗", MB_OK);
	}

#ifdef _DEBUG
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// デバッグ文字クラスの生成
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	if (NULL == m_pDebugProc)
	{// ヌルチェック
		m_pDebugProc = new CDebugProc;	// メモリ確保

		if (NULL != m_pDebugProc)
		{// メモリを確保できたかどうか
			m_pDebugProc->Init();
		}
		else
		{// メモリ確保に失敗
			MessageBox(0, "デバッグ文字クラスのメモリが確保できませんでした", "失敗", MB_OK);
		}
	}
	else
	{// NULLでなかったとき
		MessageBox(0, "デバッグ文字クラスに何か入っています", "失敗", MB_OK);
	}
#endif

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// サウンドクラスの生成
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	if (NULL == m_pSound)
	{// ヌルチェック
		m_pSound = new CSound;	// メモリ確保

		if (NULL != m_pSound)
		{// メモリを確保できたかどうか
			m_pSound->Init(hWnd);
		}
		else
		{// メモリ確保に失敗
			MessageBox(0, "サウンドクラスのメモリが確保できませんでした", "失敗", MB_OK);
		}
	}
	else
	{// NULLでなかったとき
		MessageBox(0, "サウンドクラスに何か入っています", "失敗", MB_OK);
	}

	////+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//// サーバー関係の生成
	////+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//switch (SERVER_MODE)
	//{
	//case 0:
	//	if (m_pServer == NULL)
	//	{
	//		m_pServer = CServer::Create();
	//	}
	//	break;
	//case 1:
	//	if (m_pClient == NULL)
	//	{
	//		//m_pClient = CClient::Create();
	//	}
	//	break;
	//}

	// テクスチャの読み込み
	CTexture::Load();
	CParData::Load();

	// 現在のモードを設定
	SetMode(m_mode);

	// デバッグ文字の表示
	m_bDebugProc = true;

	return S_OK;
}

//=========================================
// 終了処理
//=========================================
void CManager::Uninit(void)
{
	// データの破棄
	CScene::ReleaseAll();	// 全てのオブジェクトの破棄
	CParData::Unload();

	CTexture::Unload();

	if (NULL != m_pRenderer)
	{// レンダリングクラスの破棄
		m_pRenderer->Uninit();
		delete m_pRenderer;
		m_pRenderer = NULL;
	}

	if (NULL != m_pInputKeyboard)
	{// インプット(キーボード)クラスの破棄
		m_pInputKeyboard->Uninit();
		delete m_pInputKeyboard;
		m_pInputKeyboard = NULL;
	}

	if (NULL != m_pInputMouse)
	{// インプット(マウス)クラスの破棄
		m_pInputMouse->Uninit();
		delete m_pInputMouse;
		m_pInputMouse = NULL;
	}

	if (NULL != m_pDirectInput)
	{// インプット(DirectInput)クラスの破棄
		m_pDirectInput->Uninit();
		delete m_pDirectInput;
		m_pDirectInput = NULL;
	}

	if (NULL != m_pXInput)
	{// インプット(XInput)クラスの破棄
		m_pXInput->Uninit();
		delete m_pXInput;
		m_pXInput = NULL;
	}

	if (NULL != m_pCamera)
	{// カメラクラスの破棄
		m_pCamera->Uninit();
		delete m_pCamera;
		m_pCamera = NULL;
	}

	if (NULL != m_pLight)
	{// ライトクラスの破棄
		m_pLight->Uninit();
		delete m_pLight;
		m_pLight = NULL;
	}

#ifdef _DEBUG
	if (NULL != m_pDebugProc)
	{// デバッグ文字クラスの破棄
		m_pDebugProc->Uninit();
		delete m_pDebugProc;
		m_pDebugProc = NULL;
	}
#endif

	if (NULL != m_pSound)
	{// サウンドクラスの破棄
		m_pSound->Uninit();
		delete m_pSound;
		m_pSound = NULL;
	}

	if (m_pServer != NULL)
	{//サーバークラスの破棄
		m_pServer->Uninit();
		delete m_pServer;
		m_pServer = NULL;
	}

	//クライアントの開放処理
	ReleaseClient();

	//サーバーの終了
	CServer::Cleanup();
}

//=========================================
// 更新処理
//=========================================
void CManager::Update(void)
{
	m_pInputKeyboard->Update();		// インプットクラスの更新
	m_pInputMouse->Update();
	m_pDirectInput->Update();
	m_pXInput->Update();

	switch (SERVER_MODE)
	{
	case 0:
		if (m_pServer != NULL)
		{
			m_pServer->Update();
			CDebugProc::Print("サーバー\n");
		}
		break;
	case 1:
		if (m_pClient != NULL)
		{
			m_pClient->Update();
			CDebugProc::Print("クライアント\n");
		}
		break;
	}

	if (m_pGame != NULL)
	{// ゲーム中
		if (!m_pGame->GetPauseBool())
		{// ポーズ中でないとき
			m_pRenderer->Update();			// レンダラークラスの更新
			m_pLight->Update();				// ライトクラスの更新
		}
		else
		{// ポーズ中
			CPause *pPause = m_pGame->GetPause();
			if (pPause != NULL)
			{
				pPause->Update();
			}

			m_pGame->Update();
		}
	}
	else
	{// ゲーム以外
		m_pRenderer->Update();			// レンダラークラスの更新
		m_pLight->Update();				// ライトクラスの更新
	}

#ifdef _DEBUG
	if (m_pInputKeyboard->GetTrigger(DIK_F1))
	{
		m_bDebugProc = m_bDebugProc ? false : true;
		m_pDebugProc->DebugDisp(m_bDebugProc);
	}
#endif
}

//=========================================
// 描画処理
//=========================================
void CManager::Draw(void)
{
	m_pRenderer->Draw();	// 描画
}

//=============================================================================
// モードの終了・初期化処理
//=============================================================================
const void CManager::SetMode(MODE mode)
{
	// フェード以外のオブジェクトを破棄
	CScene::ModeUninit();

	switch (m_mode)
	{
	case MODE_TITLE:
		if (m_pTitle != NULL)
		{// タイトル
			m_pSound->StopSound(CSound::SOUND_LABEL_TITLE);
			m_pTitle = NULL;
		}
		break;

	case MODE_MENU:
		if (m_pMenu != NULL)
		{// メニュー
			m_pMenu = NULL;

		}
		break;

	case MODE_MECHASELECT:
		if (m_pMechaSelect != NULL)
		{// メニュー
			m_pMechaSelect = NULL;
			m_pSound->StopSound(CSound::SOUND_LABEL_MENU);

		}
		break;

	case MODE_MATCHING:
		if (m_pMatching != NULL)
		{// メニュー
			m_pMatching = NULL;
			m_pSound->StopSound(CSound::SOUND_LABEL_MATCHING);
		}
		break;

	case MODE_TUTORIAL:
		if (m_pTutorial != NULL)
		{// チュートリアル
			m_pSound->StopSound(CSound::SOUND_LABEL_TUTORIAL);
			m_pTutorial = NULL;
			m_pSound->PlaySound(CSound::SOUND_LABEL_MENU);

		}
		break;

	case MODE_TUTORIALMENU:
		if (m_pTutorialMenu != NULL)
		{// チュートリアル
			m_pSound->StopSound(CSound::SOUND_LABEL_MENU);
			m_pTutorialMenu = NULL;
		}
		break;

	case MODE_GAME:
		if (m_pGame != NULL)
		{// ゲーム
			//m_pSound->StopSound(CSound::SOUND_LABEL_GAME);
			m_pGame = NULL;
		}
		break;

	case MODE_RESULT:
		if (m_pResult != NULL)
		{// クリア
			m_pResult = NULL;
		}
		break;
	}

	m_mode = mode;

	//新しい画面(モード)の初期化処理
	switch (mode)
	{
	case MODE_TITLE:
		if (m_pTitle == NULL)
		{// タイトル
			m_pSound->StopSound(CSound::SOUND_LABEL_MENU);
			m_pSound->PlaySound(CSound::SOUND_LABEL_TITLE);
			m_pCamera->Init();
			m_pTitle = new CTitle;
			m_pTitle->Init();
		}
		break;

	case MODE_MENU:
		if (m_pMenu == NULL)
		{// メニュー
			m_pSound->PlaySound(CSound::SOUND_LABEL_MENU);

			m_pMenu = new CMenu;
			m_pMenu->Init();
		}
		break;

	case MODE_MECHASELECT:
		if (m_pMechaSelect == NULL)
		{// メニュー
			m_pMechaSelect = new CMechaSelect;
			m_pMechaSelect->Init();
		}
		break;

	case MODE_MATCHING:
		if (m_pMatching == NULL)
		{// メニュー
			m_pSound->PlaySound(CSound::SOUND_LABEL_MATCHING);
			m_pMatching = new CMatching;
			m_pMatching->Init();
		}
		break;

	case MODE_TUTORIALMENU:
		if (m_pTutorial == NULL)
		{// チュートリアル
			m_pTutorialMenu = new CTutorialMenu;
			m_pTutorialMenu->Init();
		}
		break;

	case MODE_TUTORIAL:
		if (m_pTutorial == NULL)
		{// タイトル
			m_pSound->StopSound(CSound::SOUND_LABEL_MENU);
			m_pSound->PlaySound(CSound::SOUND_LABEL_TUTORIAL);
			m_pCamera->Init();
			m_pTutorial = new CTutorial;
			m_pTutorial->Init();
		}
		break;

	case MODE_GAME:
		if (m_pGame == NULL)
		{// ゲーム
			//m_pSound->PlaySound(CSound::SOUND_LABEL_GAME);
			m_pCamera->Init();
			m_pGame = new CGame;
			m_pGame->Init();
		}
		break;

	case MODE_RESULT:
		if (m_pResult == NULL)
		{// クリア
			m_pCamera->Init();
			//m_pSound->PlaySound(CSound::SOUND_LABEL_CLEAR);
			m_pResult = new CResult;
			m_pResult->Init();
		}
		break;
	}
}

//=============================================================================
// クライアントの生成処理
//=============================================================================
void CManager::CreateClient(void)
{
	if (m_pClient == NULL)
	{
		m_pClient = CClient::Create();
	}
}

//=============================================================================
// クライアントの開放処理
//=============================================================================
void CManager::ReleaseClient(void)
{
	if (m_pClient != NULL)
	{//クライアントクラスの破棄
		m_pClient->Uninit();
		delete m_pClient;
		m_pClient = NULL;
	}
}