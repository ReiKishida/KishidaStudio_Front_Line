//=============================================================================
//
// マッチング画面処理 [matching.cpp]
// Author : Ayaka Hakozaki & Jukiya Hayakawa
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
#define WIDTH					(130.0f)		// 画面端からどのぐらい離れているか

#define UP_POS_Y					((SCREEN_HEIGHT / 2) - PLAYER_NUM_HEIGHT)		// 1Pと3PのPOS_Xで使用
#define RIGHT_POS_X					(SCREEN_WIDTH - WIDTH)							// 3Pと4PのPOS_Xで使用
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
	m_nCntFade = 0;
	m_bFade = false;

	for (int nCntUITex = 0; nCntUITex < MATCHING_UITEX; nCntUITex++)
	{
		m_pUITex[nCntUITex] = NULL;
	}

	for (int nCntPlayerUI = 0; nCntPlayerUI < MATCHING_UI_PLAYER; nCntPlayerUI++)
	{
		m_pPlayerUI[nCntPlayerUI] = NULL;
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
	m_nCntFade = 0;		//フェードのカウンター
	m_bFade = false;	//フェードをするかどうか
	m_nNumBlue = 0;		//青チームの総数
	m_nNumRed = 0;		//赤チームの総数

	for (int nCntConnect = 0; nCntConnect < MAX_PLAYER_CONNECT; nCntConnect++)
	{
		m_bConnect[nCntConnect] = false;
	}

	if (CMenu::GetMode() == CMenu::MODE_MULTI)
	{//マルチプレイの場合
		//クライアントの生成
		CManager::CreateClient();
	}

	//UIテクスチャの生成処理
	CreateUITex();

	//プレイヤーUIの生成処理
	CreatePlayerUI();

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

	//プレイヤーUIの破棄
	for (int nCntPlayerUI = 0; nCntPlayerUI < MATCHING_UI_PLAYER; nCntPlayerUI++)
	{
		if (m_pPlayerUI[nCntPlayerUI] != NULL)
		{
			m_pPlayerUI[nCntPlayerUI]->Uninit();
			m_pPlayerUI[nCntPlayerUI] = NULL;
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
	CClient *pClient = CManager::GetClient();
	if (pClient != NULL)
	{
		if (pClient->GetConnect() == true)
		{
			//m_bConnect[pClient->GetPlayerIdx()] = true;
		}
	}
	//必要な情報を書き込む処理
	PrintData();

	//情報を読み取る処理
	ReadMessage();

	//UIのスクロール処理
	ScrollUI();

	//フェードをするかの確認処理
	CheckFade();

	CDebugProc::Print("マッチング画面");
}

//=============================================================================
// 描画処理
//=============================================================================
void CMatching::Draw(void)
{
}

//=============================================================================
// UIテクスチャの生成処理
//=============================================================================
void CMatching::CreateUITex(void)
{
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

	// ロゴ
	CUI_TEXTURE::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), 150.0f, 150.0f, CUI_TEXTURE::UIFLAME_VS);
}

//=============================================================================
// プレイヤーUIの処理
//=============================================================================
void CMatching::CreatePlayerUI(void)
{
	//プレイヤー0番のUI
	m_pPlayerUI[0] = CScene2D::Create();
	m_pPlayerUI[0]->SetPos(D3DXVECTOR3((SCREEN_WIDTH / 2) - 300.0f, 150.0f, 0.0f));
	m_pPlayerUI[0]->SetSize(250.0f, 250.0f);
	m_pPlayerUI[0]->BindTexture(CTexture::GetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_MECHA_SELECT)));
	m_pPlayerUI[0]->SwapPriority(7);

	//プレイヤー番のUI
	m_pPlayerUI[1] = CScene2D::Create();
	m_pPlayerUI[1]->SetPos(D3DXVECTOR3((SCREEN_WIDTH / 2) - 300.0f, SCREEN_HEIGHT - 250.0f, 0.0f));
	m_pPlayerUI[1]->SetSize(250.0f, 250.0f);
	m_pPlayerUI[1]->BindTexture(CTexture::GetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_MECHA_SELECT)));
	m_pPlayerUI[1]->SwapPriority(7);

	//プレイヤー2番のUI
	m_pPlayerUI[2] = CScene2D::Create();
	m_pPlayerUI[2]->SetPos(D3DXVECTOR3((SCREEN_WIDTH / 2) + 300.0f, 150.0f, 0.0f));
	m_pPlayerUI[2]->SetSize(250.0f, 250.0f);
	m_pPlayerUI[2]->BindTexture(CTexture::GetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_MECHA_SELECT)));
	m_pPlayerUI[2]->SwapPriority(7);

	//プレイヤー3番のUI
	m_pPlayerUI[3] = CScene2D::Create();
	m_pPlayerUI[3]->SetPos(D3DXVECTOR3((SCREEN_WIDTH / 2) + 300.0f, SCREEN_HEIGHT - 250.0f, 0.0f));
	m_pPlayerUI[3]->SetSize(250.0f, 250.0f);
	m_pPlayerUI[3]->BindTexture(CTexture::GetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_MECHA_SELECT)));
	m_pPlayerUI[3]->SwapPriority(7);
}

//=============================================================================
// UIのスクロール処理
//=============================================================================
void CMatching::ScrollUI(void)
{
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
			//機体の種類情報を代入
			pClient->SetMechaType(pClient->GetPlayerIdx(), CMechaSelect::GetMechaType());

			//プレイヤー番号を書きこむ
			pClient->Printf("%d", pClient->GetPlayerIdx());
			pClient->Printf(" ");

			//機体の種類情報を書き込む
			pClient->Printf("%d", pClient->GetMechaType(pClient->GetPlayerIdx()));
			pClient->Printf(" ");

			if (pClient->GetPlayerIdx() == 0)
			{
				//フェードのしているかどうか書き込む
				switch (m_bFade)
				{
				case true:
					pClient->Printf("1");
					break;
				case false:
					pClient->Printf("0");
					break;
				}
			}
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
		if (pClient->GetConnect() == true)
		{
			char *pStr = pClient->GetReceiveData();
			int nWord = 0;
			int nPlayerIdx = 0;
			int nMechatype = 0;
			int nFade = 0;

			//頭出し処理
			pStr = CServerFunction::HeadPutout(pStr, "");

			if (CServerFunction::Memcmp(pStr, SERVER_CONNECT_DATA) == 0)
			{//接続総数を示している場合
				pStr += strlen(SERVER_CONNECT_DATA);								//頭出し
				pClient->SetNumConnect(CServerFunction::ReadInt(pStr, ""));		//総数の設置処理
				nWord = CServerFunction::PopString(pStr, "");					//文字数カウント
				pStr += nWord;													//頭出し

				pClient->SetMinIdx(CServerFunction::ReadInt(pStr, ""));
				nWord = CServerFunction::PopString(pStr, "");					//文字数カウント
				pStr += nWord;													//頭出し

				pClient->SetMaxIdx(CServerFunction::ReadInt(pStr, ""));
				nWord = CServerFunction::PopString(pStr, "");					//文字数カウント
				pStr += nWord;													//頭出し

				m_nNumBlue = CServerFunction::ReadInt(pStr, "");
				nWord = CServerFunction::PopString(pStr, "");					//文字数カウント
				pStr += nWord;													//頭出し

				m_nNumRed = CServerFunction::ReadInt(pStr, "");
				nWord = CServerFunction::PopString(pStr, "");					//文字数カウント
				pStr += nWord;													//頭出し

				for (int nCntPlayerConnect = 0; nCntPlayerConnect < MAX_PLAYER_CONNECT; nCntPlayerConnect++)
				{
					m_bConnect[nCntPlayerConnect] = CServerFunction::ReadBool(pStr, "");
					nWord = CServerFunction::PopString(pStr, "");					//文字数カウント
					pStr += nWord;													//頭出し
				}
			}
			if (CServerFunction::Memcmp(pStr, SERVER_PLAYER_START) == 0)
			{//プレイヤーの開始を示している場合
				pStr += strlen(SERVER_PLAYER_START);

				for (int nCntClient = pClient->GetMinIdx(); nCntClient < pClient->GetMaxIdx(); nCntClient++)
				{
					//番号を代入
					nPlayerIdx = CServerFunction::ReadInt(pStr, "");
					nWord = CServerFunction::PopString(pStr, "");		//文字数カウント
					pStr += nWord;										//頭出し

					//メカの種類を代入												//番号を代入
					nMechatype = CServerFunction::ReadInt(pStr, "");
					nWord = CServerFunction::PopString(pStr, "");		//文字数カウント
					pStr += nWord;										//頭出し

					if (nPlayerIdx == 0)
					{
						//フェード情報を代入																		//メカの種類を代入												//番号を代入
						nFade = CServerFunction::ReadInt(pStr, "");
						nWord = CServerFunction::PopString(pStr, "");		//文字数カウント
						pStr += nWord;										//頭出し
					}

					//
					if (nFade == 1)
					{
						m_bFade = true;
					}

					if (nPlayerIdx != pClient->GetPlayerIdx())
					{
						pClient->SetMechaType(nPlayerIdx, nMechatype);
					}
				}
			}
			CDebugProc::Print("種類：%d %d %d %d", pClient->GetMechaType(0), pClient->GetMechaType(1), pClient->GetMechaType(2), pClient->GetMechaType(3));
			CDebugProc::Print("最小：%d", pClient->GetMinIdx());
			CDebugProc::Print("最大：%d", pClient->GetMaxIdx());
		}
		for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER_CONNECT; nCntPlayer++)
		{//
			if (m_bConnect[nCntPlayer] == true)
			{
				m_pPlayerUI[nCntPlayer]->SetTex(pClient->GetMechaType(nCntPlayer) + 1, 1, 5);
			}
			else if (m_bConnect[nCntPlayer] == false)
			{
				m_pPlayerUI[nCntPlayer]->SetTex(0, 1, 5);
			}
		}

	}
}

//=============================================================================
// フェードをするか確認する処理
//=============================================================================
void CMatching::CheckFade(void)
{
	//クライアントの取得
	CClient *pClient = CManager::GetClient();

	// 入力の情報を取得
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();
	CXInput *pXInput = CManager::GetXInput();

	// サウンドの取得
	CSound *pSound = CManager::GetSound();

	if (pClient != NULL)
	{
		//if (pClient->GetNumConnect() >= 2 /*MAX_CONNECT*/ &&
		//	m_bFade == false)
		//if(m_nNumBlue >= 1 && m_nNumRed >= 1 && m_bFade == false)
		//{//接続されている総数が一定数以上の場合かつフェードをしていない場合
		//	//if (pKeyboard->GetAnyKey() || pXInput->GetAnyButton(0))
		//	if(pKeyboard->GetTrigger(DIK_RETURN) ==true)
		//	{//ボタンを押された場合
		//		m_bFade = true;	//フェードをする状態にする
		//	}
		//}
		if (m_nNumBlue == 2 && m_nNumRed == 2 && m_bFade == false)
		{
			if (pClient->GetPlayerIdx() == 0)
			{
				m_bFade = true;
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
		}
	}

}