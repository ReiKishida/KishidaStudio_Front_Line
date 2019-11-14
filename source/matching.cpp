//=============================================================================
//
// マッチング画面処理 [matching.cpp]
// Author : Ayaka Hakozaki
//
//=============================================================================
#include "matching.h"
#include "fade.h"
#include "input.h"
#include "debugProc.h"
#include "sound.h"
#include "texture.h"
#include "UI_Texture.h"
#include "server.h"
#include "menu.h"
#include "serverfunction.h"
#include "mechaSelect.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define PLAYER_NUM_WIDTH		(150.0f)		// プレイヤーナンバーロゴの幅
#define PLAYER_NUM_HEIGHT		(70.0f)			// プレイヤーナンバーロゴの高さ
#define WIDTH								(130.0f)		// 画面端からどのぐらい離れているか

#define UP_POS_Y							((SCREEN_HEIGHT / 2) - PLAYER_NUM_HEIGHT)		// 1Pと3PのPOS_Xで使用
#define RIGHT_POS_X					(SCREEN_WIDTH - WIDTH)											// 3Pと4PのPOS_Xで使用
#define DOWN_POS_Y					((SCREEN_HEIGHT - PLAYER_NUM_HEIGHT))			// 2Pと4PのPOS_Yで使用

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************

//=========================================
// コンストラクタ
//=========================================
CMatching::CMatching(int nPriority, CScene::OBJTYPE objType) : CScene(nPriority, objType)
{
	m_nCntBgMove = 0;
	m_pUITex[0] = NULL;
	m_pUITex[1] = NULL;
	m_pUITex[2] = NULL;
	m_pUITex[3] = NULL;
	m_nCntFade = 0;
	m_bFade = false;

	for (int nCntPlayerUI = 0; nCntPlayerUI < MATCHING_UI_PLAYER; nCntPlayerUI++)
	{
		m_pMatchingPlayerUI[nCntPlayerUI] = NULL;
	}

}

//=========================================
// デストラクタ
//=========================================
CMatching::~CMatching()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CMatching::Init(void)
{
	m_nCntFade = 0;
	m_bFade = false;
	if (CMenu::GetMode() == CMenu::MODE_MULTI)
	{
		CManager::CreateClient();
	}

	// 背景下地
	CUI_TEXTURE::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), SCREEN_WIDTH, SCREEN_HEIGHT, CUI_TEXTURE::UIFLAME_MATCHING_BG);

	// BLUEチーム背景スクロール
	m_pUITex[0] = CUI_TEXTURE::Create(D3DXVECTOR3(SCREEN_WIDTH / 4, SCREEN_HEIGHT / 2, 0.0f), SCREEN_WIDTH / 2, SCREEN_HEIGHT, CUI_TEXTURE::UIFLAME_MATCHING_BG_00);
	m_pUITex[1] = CUI_TEXTURE::Create(D3DXVECTOR3(SCREEN_WIDTH / 4, SCREEN_HEIGHT / 2, 0.0f), SCREEN_WIDTH / 2, SCREEN_HEIGHT, CUI_TEXTURE::UIFLAME_MATCHING_BG_01);

	// REDチーム背景スクロール
	m_pUITex[2] = CUI_TEXTURE::Create(D3DXVECTOR3(SCREEN_WIDTH - (SCREEN_WIDTH / 4), SCREEN_HEIGHT / 2, 0.0f), SCREEN_WIDTH / 2, SCREEN_HEIGHT, CUI_TEXTURE::UIFLAME_MATCHING_BG_00);
	m_pUITex[3] = CUI_TEXTURE::Create(D3DXVECTOR3(SCREEN_WIDTH - (SCREEN_WIDTH / 4), SCREEN_HEIGHT / 2, 0.0f), SCREEN_WIDTH / 2, SCREEN_HEIGHT, CUI_TEXTURE::UIFLAME_MATCHING_BG_01);

	// 1Pロゴ
	m_pUITex[4] = CUI_TEXTURE::Create(D3DXVECTOR3(WIDTH, UP_POS_Y, 0.0f), PLAYER_NUM_WIDTH, PLAYER_NUM_HEIGHT, CUI_TEXTURE::UIFLAME_PLAYER_NUM);
	m_pUITex[4]->SetTex(0, 1, 4);

	// 2Pロゴ
	m_pUITex[5] = CUI_TEXTURE::Create(D3DXVECTOR3(WIDTH, DOWN_POS_Y, 0.0f), PLAYER_NUM_WIDTH, PLAYER_NUM_HEIGHT, CUI_TEXTURE::UIFLAME_PLAYER_NUM);
	m_pUITex[5]->SetTex(1, 1, 4);

	// 3Pロゴ
	m_pUITex[6] = CUI_TEXTURE::Create(D3DXVECTOR3(RIGHT_POS_X, UP_POS_Y, 0.0f), PLAYER_NUM_WIDTH, PLAYER_NUM_HEIGHT, CUI_TEXTURE::UIFLAME_PLAYER_NUM);
	m_pUITex[6]->SetTex(2, 1, 4);

	// 4Pロゴ
	m_pUITex[7] = CUI_TEXTURE::Create(D3DXVECTOR3(RIGHT_POS_X, DOWN_POS_Y, 0.0f), PLAYER_NUM_WIDTH, PLAYER_NUM_HEIGHT, CUI_TEXTURE::UIFLAME_PLAYER_NUM);
	m_pUITex[7]->SetTex(3, 1, 4);

	m_pMatchingPlayerUI[0] = CScene2D::Create();
	m_pMatchingPlayerUI[0]->SetPos(D3DXVECTOR3((SCREEN_WIDTH / 2) - 300.0f, 150.0f, 0.0f));
	m_pMatchingPlayerUI[0]->SetSize(250.0f, 250.0f);
	m_pMatchingPlayerUI[0]->BindTexture(CTexture::GetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_MECHA_SELECT)));
	m_pMatchingPlayerUI[0]->SwapPriority(7);

	m_pMatchingPlayerUI[1] = CScene2D::Create();
	m_pMatchingPlayerUI[1]->SetPos(D3DXVECTOR3((SCREEN_WIDTH / 2) - 300.0f, SCREEN_HEIGHT - 250.0f, 0.0f));
	m_pMatchingPlayerUI[1]->SetSize(250.0f, 250.0f);
	m_pMatchingPlayerUI[1]->BindTexture(CTexture::GetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_MECHA_SELECT)));
	m_pMatchingPlayerUI[1]->SwapPriority(7);

	m_pMatchingPlayerUI[2] = CScene2D::Create();
	m_pMatchingPlayerUI[2]->SetPos(D3DXVECTOR3((SCREEN_WIDTH / 2) + 300.0f, 150.0f, 0.0f));
	m_pMatchingPlayerUI[2]->SetSize(250.0f, 250.0f);
	m_pMatchingPlayerUI[2]->BindTexture(CTexture::GetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_MECHA_SELECT)));
	m_pMatchingPlayerUI[2]->SwapPriority(7);

	m_pMatchingPlayerUI[3] = CScene2D::Create();
	m_pMatchingPlayerUI[3]->SetPos(D3DXVECTOR3((SCREEN_WIDTH / 2) + 300.0f, SCREEN_HEIGHT - 250.0f, 0.0f));
	m_pMatchingPlayerUI[3]->SetSize(250.0f, 250.0f);
	m_pMatchingPlayerUI[3]->BindTexture(CTexture::GetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_MECHA_SELECT)));
	m_pMatchingPlayerUI[3]->SwapPriority(7);

	// ロゴ
	CUI_TEXTURE::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), 150.0f, 150.0f, CUI_TEXTURE::UIFLAME_VS);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CMatching::Uninit(void)
{
	// UIの破棄（背景スクロール）
	for (int nCntUITex = 0; nCntUITex < MATCHING_UITEX; nCntUITex++)
	{
		if (m_pUITex[nCntUITex] != NULL)
		{
			m_pUITex[nCntUITex]->Uninit();
			m_pUITex[nCntUITex] = NULL;
		}
	}

	// 自分の破棄
	CScene::Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CMatching::Update(void)
{
	//必要な情報を書き込む処理
	PrintData();

	//情報を読み取る処理
	ReadMessage();

	// 入力の情報を取得
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();
	CXInput *pXInput = CManager::GetXInput();

	// サウンドの取得
	CSound *pSound = CManager::GetSound();

	// カウンター加算
	m_nCntBgMove++;

	//****************************************
	//  BLUEチーム：色とスクロール処理
	//****************************************
	m_pUITex[0]->SetColor(D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
	m_pUITex[0]->SetBgMove(0, -m_nCntBgMove, 0.0025f);				// 下

	m_pUITex[1]->SetColor(D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
	m_pUITex[1]->SetBgMove(0, m_nCntBgMove, 0.0025f);				// 上

																	//****************************************
																	//  REDチーム：色とスクロール処理
																	//****************************************
	m_pUITex[2]->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	m_pUITex[2]->SetBgMove(0, -m_nCntBgMove, 0.0020f);			// 下

	m_pUITex[3]->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	m_pUITex[3]->SetBgMove(0, m_nCntBgMove, 0.0020f);			// 上

																//****************************************
																// 画面遷移
																//****************************************
	if (pKeyboard->GetAnyKey() || pXInput->GetAnyButton(0))
	{
		if (CFade::GetFade() == CFade::FADE_NONE)
		{// フェードがないとき
		 //CFade::Create(CManager::MODE_GAME);
		 //pSound->PlaySound(CSound::SOUND_LABEL_DECIDE);
		}
	}

	CDebugProc::Print("マッチング画面");

	CClient *pClient = CManager::GetClient();			//クライアントのポインタ情報

	if (pClient != NULL)
	{
		char *pStr = pClient->GetReceiveData();				//サーバーから受け取ったメッセージ情報

															//頭出し処理
		pStr = CServerFunction::HeadPutout(pStr, "");

		if (CServerFunction::Memcmp(pStr, SERVER_NUM_CONNECT) == 0)
		{//接続総数を示している場合
			pStr += strlen(SERVER_NUM_CONNECT);								//頭出し
			pClient->SetNumConnect(CServerFunction::ReadInt(pStr, ""));		//総数の設置処理
		}

		if (pClient->GetNumConnect() >= /*2*/ MAX_CONNECT)
		{
			if (m_bFade == false)
			{
				m_bFade = true;
			}
			if (CFade::GetFade() == CFade::FADE_NONE)
			{// フェードがないとき

			}

		}
		if (pClient->GetConnect() == true)
		{
			for (int nCntPlayer = 0; nCntPlayer < MAX_CONNECT; nCntPlayer++)
			{

				m_pMatchingPlayerUI[nCntPlayer]->SetTex(pClient->GetMechaType(nCntPlayer), 1, 4);
			}
		}

	}

	if (m_bFade == true)
	{
		m_nCntFade++;
	}
	if (m_nCntFade >= 200)
	{
		if (CFade::GetFade() == CFade::FADE_NONE)
		{// フェードがないとき

			CFade::Create(CManager::MODE_GAME);
			//pSound->PlaySound(CSound::SOUND_LABEL_DECIDE);
		}

	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CMatching::Draw(void)
{
}

//=============================================================================
// 必要な情報を書き込む処理
//=============================================================================
void CMatching::PrintData(void)
{
	//クライアントの取得
	CClient *pClient = CManager::GetClient();
	if (pClient != NULL)
	{
		if (pClient->GetConnect() == true)
		{
			int nIdx = pClient->GetClientIdx();
			pClient->SetMechaType(pClient->GetPlayerIdx(), CMechaSelect::GetMechaType());
			//m_pMatchingPlayerUI[nIdx]->BindTexture(CTexture::GetTexture((CTexture::TEXTURE)CMechaSelect::GetMechaType()));
			pClient->Printf("%d", pClient->GetPlayerIdx());
			pClient->Printf(" ");
			pClient->Printf("%d", pClient->GetMechaType(pClient->GetPlayerIdx()));
			pClient->Printf(" ");
			CDebugProc::Print("プレイヤー番号%d", pClient->GetPlayerIdx());
		}
	}
}

//=============================================================================
// 情報を読み取る処理
//=============================================================================
void CMatching::ReadMessage(void)
{
	//クライアントの取得
	CClient *pClient = CManager::GetClient();

	if (pClient != NULL)
	{
		if (pClient->GetConnect())
		{
			char *pStr = pClient->GetReceiveData();
			int nWord = 0;
			int nPlayerIdx = 0;
			int nMechatype = 0;

			//頭出し処理
			pStr = CServerFunction::HeadPutout(pStr, "");

			if (CServerFunction::Memcmp(pStr, SERVER_NUM_CONNECT) == 0)
			{//接続総数を示している場合
				pStr += strlen(SERVER_NUM_CONNECT);								//頭出し
				pClient->SetNumConnect(CServerFunction::ReadInt(pStr, ""));		//総数の設置処理
				nWord = CServerFunction::PopString(pStr, "");					//文字数カウント
				pStr += nWord;													//頭出し
			}
			if (CServerFunction::Memcmp(pStr, SERVER_PLAYER_START) == 0)
			{//プレイヤーの開始を示している場合
				pStr += strlen(SERVER_PLAYER_START);

				for (int nCntClient = 0; nCntClient < pClient->GetNumConnect() - 1; nCntClient++)
				{
					//番号を代入
					nPlayerIdx = CServerFunction::ReadInt(pStr, "");
					nWord = CServerFunction::PopString(pStr, "");		//文字数カウント
					pStr += nWord;										//頭出し

																		//メカの種類を代入												//番号を代入
					nMechatype = CServerFunction::ReadInt(pStr, "");
					nWord = CServerFunction::PopString(pStr, "");		//文字数カウント
					pStr += nWord;										//頭出し

					if (nPlayerIdx != pClient->GetPlayerIdx())
					{
						pClient->SetMechaType(nPlayerIdx, nMechatype);
					}
				}
			}
			CDebugProc::Print("種類：%d %d %d %d", pClient->GetMechaType(0), pClient->GetMechaType(1), pClient->GetMechaType(2), pClient->GetMechaType(3));
		}
	}
}