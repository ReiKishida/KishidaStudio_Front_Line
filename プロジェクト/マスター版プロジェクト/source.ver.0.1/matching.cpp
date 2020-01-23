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
#include "game.h"
#include "button.h"
#include "mouseCursor.h"

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
bool CMatching::m_bConnect[MAX_PLAYER_CONNECT] = { false,false,false,false };

//=========================================
// コンストラクタ
//=========================================
CMatching::CMatching(int nPriority, CScene::OBJTYPE objType) : CScene(nPriority, objType)
{
	m_nCntBgMove = 0;
	m_nCntFade = 0;
	m_bFade = false;
	m_pCursor = NULL;
	m_pButtonUI = NULL;

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
		CGame::SetMechaType(nCntConnect, CMechaSelect::MECHATYPE_EMPTY);
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

	if (NULL != m_pButtonUI)
	{// ボタンUIの破棄
		m_pButtonUI->Uninit();
		m_pButtonUI = NULL;
	}

	if (NULL != m_pCursor)
	{// カーソルの破棄
		m_pCursor->Uninit();
		m_pCursor = NULL;
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
		//if (pClient->GetPlayerIdx() == 0)
		//{//プレイヤー番号が０の場合

		//	if (CFade::GetFade() == CFade::FADE_NONE && m_nNumBlue == 2 && m_nNumRed == 2)
		//	{// フェードがないとき
		//		m_bFade = true;
		//	}
		//}


		CGame::SetMechaType(pClient->GetPlayerIdx(), CMechaSelect::GetMechaType());
		//if (pClient->GetConnect() == true)
		//{
		//	m_bConnect[pClient->GetPlayerIdx()] = true;
		//}

		if (pClient->GetPlayerIdx() == 0)
		{//プレイヤー番号が０の場合
			if (m_nNumBlue >= 1 && m_nNumRed >= 1)
			{//ブルーチームとレッドチーム両方に１人以上いる場合
				if (m_pButtonUI == NULL && m_bFade == false)
				{//NULLの場合
					//ボタンの生成
					m_pButtonUI = CButton2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, (SCREEN_HEIGHT / 2) + 250.0f, 0.0f), 300.0f, 150.0f);				// シングルプレイ
					m_pButtonUI->BindTexture(CTexture::GetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_BATTLESTART)));
				}
				if(m_pButtonUI != NULL )
				{//NULLではない場合
					if (m_pButtonUI->InRangeMenu(m_pCursor->GetMousePosition()))
					{// 範囲内かチェック
						if (m_pButtonUI->ClickRelease() == true)
						{// クリックされた
							if (m_bFade == false)
							{
								m_bFade = true;	//フェードをする状態にする

#if 1
								for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER_CONNECT; nCntPlayer++)
								{
									if (m_bConnect[nCntPlayer] == false)
									{
										CGame::SetMechaType(nCntPlayer, (CMechaSelect::MECHATYPE)(rand() % CMechaSelect::MECHATYPE_MAX));
									}
								}
#endif
								m_pButtonUI->Uninit();
								m_pButtonUI = NULL;
							}
						}
					}

				}

				if (m_pCursor == NULL)
				{//NULLの場合
					//カーソルの生成
					m_pCursor = CMouseCursor2D::Create();
				}
			}
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

			pClient->Printf(SERVER_PLAYER_DATA);

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
					pClient->Printf(SERVER_FADE_OUT);
					pClient->Printf(" ");

					for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER_CONNECT; nCntPlayer++)
					{
						if (m_bConnect[nCntPlayer] == false)
						{
							pClient->Printf("%d", CGame::GetMechaType(nCntPlayer));
							pClient->Printf(" ");
						}
					}

					break;
				case false:
					pClient->Printf(SERVER_FADE_NONE);
					break;
				}
				pClient->Printf(" ");
			}

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

																					//接続総数を設置処理
				pClient->SetNumConnect(CServerFunction::ReadInt(pStr, ""));		//総数の設置処理
				nWord = CServerFunction::PopString(pStr, "");					//文字数カウント
				pStr += nWord;													//頭出し

																				//最小の番号を設置処理
				pClient->SetMinIdx(CServerFunction::ReadInt(pStr, ""));
				nWord = CServerFunction::PopString(pStr, "");					//文字数カウント
				pStr += nWord;													//頭出し

																				//最大の番号を設置処理
				pClient->SetMaxIdx(CServerFunction::ReadInt(pStr, ""));
				nWord = CServerFunction::PopString(pStr, "");					//文字数カウント
				pStr += nWord;													//頭出し

																				//青チームの総数を代入
				m_nNumBlue = CServerFunction::ReadInt(pStr, "");
				nWord = CServerFunction::PopString(pStr, "");					//文字数カウント
				pStr += nWord;													//頭出し

																				//赤チームの総数を代入
				m_nNumRed = CServerFunction::ReadInt(pStr, "");
				nWord = CServerFunction::PopString(pStr, "");					//文字数カウント
				pStr += nWord;													//頭出し

			}
			if (CServerFunction::Memcmp(pStr, SERVER_CONNECT_STATE) == 0)
			{
				pStr += strlen(SERVER_CONNECT_DATA);								//頭出し

				//接続状況を代入
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
				pStr += strlen(" ");

				for (int nCntClient = pClient->GetMinIdx(); nCntClient < pClient->GetMaxIdx(); nCntClient++)
				{
					if (CServerFunction::Memcmp(pStr, SERVER_PLAYER_DATA) == 0)
					{
						pStr += strlen(SERVER_PLAYER_DATA);

						//番号を代入
						nPlayerIdx = CServerFunction::ReadInt(pStr, "");
						nWord = CServerFunction::PopString(pStr, "");		//文字数カウント
						pStr += nWord;										//頭出し

																			//メカの種類を代入
						nMechatype = CServerFunction::ReadInt(pStr, "");
						nWord = CServerFunction::PopString(pStr, "");		//文字数カウント
						pStr += nWord;										//頭出し

						if (nPlayerIdx == 0)
						{//プレイヤー番号が０の場合
							if (CServerFunction::Memcmp(pStr, SERVER_FADE_NONE) == 0)
							{//フェードしていない状態の場合
								m_bFade = false; //フェードしていない状態にする
								pStr += strlen(SERVER_FADE_NONE);
								pStr += strlen(" ");
							}
							if (CServerFunction::Memcmp(pStr, SERVER_FADE_OUT) == 0)
							{//フェードアウトをしている場合
								m_bFade = true;	//フェードしている状態にする
								pStr += strlen(SERVER_FADE_OUT);
								pStr += strlen(" ");

								for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER_CONNECT; nCntPlayer++)
								{
									if (m_bConnect[nCntPlayer] == false)
									{
										int nMechaType = 0;
										nMechaType = CServerFunction::ReadInt(pStr, "");
										nWord = CServerFunction::PopString(pStr, "");
										pStr += nWord;

										CGame::SetMechaType(nCntPlayer, (CMechaSelect::MECHATYPE)nMechaType);
									}
								}

							}
						}

						if (CFade::GetFade() == CFade::FADE_NONE || CFade::GetFade() == CFade::FADE_IN)
						{//フェードしていない場合またはフェードインしている場合
							if (CGame::GetMechaType(nPlayerIdx) == CMechaSelect::MECHATYPE_EMPTY)
							{//機体が何も選択されていない場合
								CGame::SetMechaType(nPlayerIdx, (CMechaSelect::MECHATYPE)nMechatype);
							}

							CGame::SetMechaType(0, (CMechaSelect::MECHATYPE)pClient->GetMechaType(0));

							if (nPlayerIdx != pClient->GetPlayerIdx())
							{//プレイヤー番号がクライアントの所持しているプレイヤー番号と同じ場合
								pClient->SetMechaType(nPlayerIdx, nMechatype);
							}
						}
					}
				}
			}
			CDebugProc::Print("種類：%d %d %d %d", CGame::GetMechaType(0), CGame::GetMechaType(1), CGame::GetMechaType(2), CGame::GetMechaType(3));
			CDebugProc::Print("最小：%d", pClient->GetMinIdx());
			CDebugProc::Print("最大：%d", pClient->GetMaxIdx());
		}
		for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER_CONNECT; nCntPlayer++)
		{
			if (m_bConnect[nCntPlayer] == true)
			{//接続されている場合
			 //選択された機体のテクスチャを設定
				m_pPlayerUI[nCntPlayer]->SetTex(CGame::GetMechaType(nCntPlayer) + 1/*pClient->GetMechaType(nCntPlayer) + 1*/, 1, 5);
			}
			else if (m_bConnect[nCntPlayer] == false)
			{//接続されていない場合
			 //選択されていない画像を設定
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
	{//NULLではない場合
		if (m_nNumBlue >= 1 && m_nNumRed >= 1 && m_bFade == false)
		{//接続されている総数が一定数以上の場合かつフェードをしていない場合
		 //if (pKeyboard->GetAnyKey() || pXInput->GetAnyButton(0))
			if (pKeyboard->GetTrigger(DIK_RETURN) == true)
			{//ボタンを押された場合
				m_bFade = true;	//フェードをする状態にする
			}
		}

		//if (m_nNumBlue == 2 && m_nNumRed == 2 && m_bFade == false)
		//{
		//	if (pClient->GetPlayerIdx() == 0)
		//	{
		//		m_bFade = true;
		//	}
		//}

		/*if (pClient->GetNumConnect() == 2)
		{
		if (pClient->GetPlayerIdx() == 0)
		{
		m_bFade = true;
		}
		}*/
	}
	if (m_bFade == true)
	{//フェードされている場合
		m_nCntFade++;	//フェードまでのカウンターを加算
	}

	if (m_nCntFade >= 200)
	{//フェードカウンターが200以上の場合
		if (CFade::GetFade() == CFade::FADE_NONE)
		{// フェードがないとき
			CFade::Create(CManager::MODE_GAME);
		}
	}

}