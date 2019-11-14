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
	// 背景下地
	CUI_TEXTURE::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), SCREEN_WIDTH , SCREEN_HEIGHT, CUI_TEXTURE::UIFLAME_MATCHING_BG);

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
			CFade::Create(CManager::MODE_GAME);
			//pSound->PlaySound(CSound::SOUND_LABEL_DECIDE);
		}
	}

	CDebugProc::Print("マッチング画面");
}

//=============================================================================
// 描画処理
//=============================================================================
void CMatching::Draw(void)
{
}