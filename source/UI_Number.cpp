//=============================================================================
//
// UI_Number処理 [UI_Number.cpp]
// Author : Ayaka Hakozaki
//
//=============================================================================
#include "UI_Texture.h"
#include "texture.h"
#include "number.h"
#include "input.h"
#include "manager.h"
#include "debugProc.h"
#include "fade.h"
#include "UI_Number.h"
#include "game.h"
#include "player.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define UI_POS	(D3DXVECTOR3((m_pos.x - (nCntDigits * m_fInt)) + (m_fWidth * (m_nDigits / 2.0f)) - m_fWidth / 2, m_pos.y, 0.0f))

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************
int CUI_NUMBER::m_nRemBullet = 0;
int CUI_NUMBER::m_nPlayerLife = 0;
int CUI_NUMBER::m_nTeamBlue = 0;
int CUI_NUMBER::m_nTeamRed = 0;
int CUI_NUMBER::m_nCntRespawn = 0;

//=============================================================================
// コンストラクタ
//=============================================================================
CUI_NUMBER::CUI_NUMBER(int nPriority, CScene::OBJTYPE objType) : CScene2D(nPriority, objType)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_fWidth = 0.0f;
	m_fHeight = 0.0f;
	m_fInt = 0.0f;
	m_apNumber = NULL;
	m_nUV = 0;
	m_nUV_X = 0;
	m_nUV_Y = 0;
	m_nDigits = 0;
	m_nTimer = 0;
	m_nDecrease = 0;
}

//=============================================================================
// デストラクタ
//=============================================================================
CUI_NUMBER::~CUI_NUMBER()
{
}

//=============================================================================
// 生成処理
//=============================================================================
CUI_NUMBER *CUI_NUMBER::Create(D3DXVECTOR3 pos, float fWidth, float fHeight, float fInt, UI_NUMTYPE UINumType, int nUV, int nUV_X, int nUV_Y)
{
	CUI_NUMBER *pUINum = NULL;	// UIクラスのポインタ変数

	pUINum = new CUI_NUMBER;		// メモリ確保

	if (NULL != pUINum)
	{	// メモリ確保できた
		pUINum->m_UINumType = UINumType;	// タイプ
		pUINum->m_pos = pos;								// 位置
		pUINum->m_fWidth = fWidth;					// 幅
		pUINum->m_fHeight = fHeight;					// 高さ
		pUINum->m_fInt = fInt;								// 数字の間隔
		pUINum->m_nUV = nUV;							// (0 : W, 1 : B, 2 : R)
		pUINum->m_nUV_X = nUV_X;					// ナンバーテクスチャUV_X
		pUINum->m_nUV_Y = nUV_Y;					// ナンバーテクスチャUV_Y
		pUINum->Init();											// 初期化処理
	}

	return pUINum;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CUI_NUMBER::Init(void)
{
	// Scene2Dの初期化
	CScene2D::Init();

	// 位置の設定
	SetPos(m_pos);

	//****************************************
	// タイプ別処理
	//****************************************
	switch (m_UINumType)
	{
	case UI_NUMTYPE_NONE:
		break;

	case UI_NUMTYPE_REMAINBULLET:
		GetRemainBullet();								// プレイヤーから残弾の取得
		SetNumDisPlay(m_nRemBullet, m_UINumType);		// 残弾を表示
		break;

	case UI_NUMTYPE_PLAYER_HP:
		GetPlayerLife();									// プレイヤーからライフ取得
		SetNumDisPlay(m_nPlayerLife, m_UINumType);		// ライフを設定
		break;

	case UI_NUMTYPE_BLUE:
		GetTeamBlue();									// プレイヤーからBLUEチームチケット取得
		SetNumDisPlay(m_nTeamBlue, m_UINumType);		// チケット設定
		break;

	case UI_NUMTYPE_RED:
		GetTeamRed();									// プレイヤーからREDチームチケット取得
		SetNumDisPlay(m_nTeamRed, m_UINumType);		// チケット設定
		break;

	case UI_NUMTYPE_CNTRESPAWN:
		m_nCntRespawn = CNT_RESPAWN;		// カウント初期値
		SetNumDisPlay(m_nCntRespawn, m_UINumType);		// チケット設定
		break;
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CUI_NUMBER::Uninit(void)
{
	if (NULL != m_apNumber)
	{// 使われている場合破棄する
		for (int nCntScore = 0; nCntScore < m_nDigits; nCntScore++)
		{// 桁数分ループ
			if (NULL != m_apNumber[nCntScore])
			{// 使われている場合破棄する
				m_apNumber[nCntScore]->Uninit();
				m_apNumber[nCntScore] = NULL;
			}
		}

		delete[] m_apNumber;
		m_apNumber = NULL;
	}

	// Scene2Dの破棄
	CScene2D::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CUI_NUMBER::Update(void)
{
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();	// キーボードの入力を取得
	m_UINumType = CUI_NUMBER::GetNumType();							// 現在のタイプを取得

	//****************************************
	// タイプ別処理
	//****************************************
	switch (m_UINumType)
	{
	case UI_NUMTYPE_REMAINBULLET:
		SetNumColor(m_nRemBullet, PLAYER_BULLET);				// 値によって色変化する
		SetNum(m_nRemBullet, PLAYER_BULLET, m_col);
		break;

	case UI_NUMTYPE_PLAYER_HP:
		SetNumColor(m_nPlayerLife, PLAYER_LIFE);

		if (m_nPlayerLife >= 0)
		{	// 0以上の時、数字更新
			SetNum(m_nPlayerLife, PLAYER_LIFE, m_col);
		}
		break;

	case UI_NUMTYPE_BLUE:
		if (pKeyboard->GetTrigger(DIK_F3))
		{
			m_nDiff += 15;		// ダメージ量設定
		}
		if (m_nDiff > 0)
		{	// ダメージ量が0以上の時
			m_nTeamBlue--;
			m_nDiff--;
		}

		if (m_nTeamBlue >= 0)
		{	// 0以上の時、数字更新
			SetNum(m_nTeamBlue, TEAM_BLUE, m_col);
		}
		break;

	case UI_NUMTYPE_RED:
		if (pKeyboard->GetTrigger(DIK_F3))
		{
			m_nDiff += 15;		// ダメージ量設定
		}
		if (m_nDiff > 0)
		{	// ダメージ量が0以上の時
			m_nTeamRed--;
			m_nDiff--;
		}

		if (m_nTeamRed >= 0)
		{	// 0以上の時、数字更新
			SetNum(m_nTeamRed, TEAM_RED, m_col);
		}
		break;

	case UI_NUMTYPE_CNTRESPAWN:
		m_nTimer++;
		if (m_nTimer % 60 == 0)
		{
			m_nCntRespawn--;
		}

		if (m_nCntRespawn >= 0)
		{	// カウンター0以上
			SetNum(m_nCntRespawn, CNT_RESPAWN, m_col);
		}
		else
		{	// カウンター0になった
			CGame::SetGameState(CGame::STATE_END);
		}

		SetTime(m_nTimer);

		break;
	}

	// 各数字の設定
	SetPlayerLife(m_nPlayerLife);		// ライフ
	SetTeamBlue(m_nTeamBlue);		// BLUEチーム
	SetTeamRed(m_nTeamRed);			// REDチーム
	SetRespawn(m_nCntRespawn);		// 戦線復帰カウンター
}

//=============================================================================
// 描画処理
//=============================================================================
void CUI_NUMBER::Draw(void)
{
	// Scene2Dの描画
	CScene2D::Draw();
}

//=============================================================================
// 現在の値・初期値：取得時の値から計算し、それによって色変わる処理
//=============================================================================
void CUI_NUMBER::SetNumColor(int nNum, int nDefNum)
{
	// 基本、白
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	if (CManager::GetGame()->GetPart() == CGame::PART_STRATEGY)
	{
		if (UI_NUMTYPE_REMAINBULLET == m_UINumType || UI_NUMTYPE_PLAYER_HP == m_UINumType)
		{
			m_col = D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f);
		}
	}

	if (nNum <= nDefNum / 2)
	{	// 半分になったら黄色
		if (CManager::GetGame()->GetPart() == CGame::PART_STRATEGY)
		{// ストラテジーパート
			if (UI_NUMTYPE_REMAINBULLET == m_UINumType || UI_NUMTYPE_PLAYER_HP == m_UINumType)
			{
				m_col = D3DXCOLOR(0.1f, 0.1f, 0.0f, 1.0f);
			}
		}
		else
		{// アクションパート
			m_col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
		}
	}

	if (nNum <= nDefNum / 4)
	{	// 4/1になったら赤
		if (CManager::GetGame()->GetPart() == CGame::PART_STRATEGY)
		{// ストラテジーパート
			if (UI_NUMTYPE_REMAINBULLET == m_UINumType || UI_NUMTYPE_PLAYER_HP == m_UINumType)
			{
				m_col = D3DXCOLOR(0.1f, 0.0f, 0.0f, 1.0f);
			}
		}
		else
		{// アクションパート
			m_col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		}
	}
}

//=============================================================================
// 取得してきた値・タイプの取得：取得してきた数字を表示する処理
//=============================================================================
void CUI_NUMBER::SetNumDisPlay(int nGetValue, UI_NUMTYPE UINumType)
{
	//****************************************
	//数字の表示初期設定
	//****************************************
	int m_nDigits = (int)log10f((float)nGetValue) + 1;
	if (m_nDigits <= 0) { m_nDigits = 1; }			// 桁数が0以下のとき

	m_apNumber = new CNumber*[m_nDigits];			//桁数分 メモリを確保

	if (m_apNumber)
	{// メモリ確保成功
		for (int nCntDigits = 0; nCntDigits < m_nDigits; nCntDigits++)
		{// 桁数分ループ
			m_apNumber[nCntDigits] = CNumber::Create(UI_POS, m_fWidth, m_fHeight, m_UINumType, m_nUV, m_nUV_X, m_nUV_Y);

			// 数字の設定
			int aNumber = nGetValue % (int)powf(10.0f, (float)(nCntDigits + 1)) / (int)powf(10.0f, (float)nCntDigits);
			m_apNumber[nCntDigits]->SetTexNumber(aNumber);

			// 色の設定
			m_apNumber[nCntDigits]->SetColor(m_col);
		}
	}

	// 桁の設定
	SetDigits(m_nDigits);
}

//=============================================================================
// 現在の値・初期値・色：取得してきた数字を表示して更新する処理
//=============================================================================
void CUI_NUMBER::SetNum(int nCalcNum, int nDefNum, D3DXCOLOR col)
{
	m_nDigits = GetDigits();			// SetNumDisPlayで設定した桁を取得

	//****************************************
	//現在の値の表示設定
	//****************************************
	int nDigits = (int)log10f((float)nCalcNum) + 1;
	if (nDigits <= 0) { nDigits = 1; }		// 桁数が0以下のとき

	if (nDigits != m_nDigits)
	{// 桁数が変わった
		for (int nCntScore = 0; nCntScore < m_nDigits; nCntScore++)
		{// 桁数分ループ
			if (NULL != m_apNumber[nCntScore])
			{// 使われている場合破棄する
				m_apNumber[nCntScore]->Uninit();
				m_apNumber[nCntScore] = NULL;
			}
		}

		if (NULL != m_apNumber)
		{// 使われている場合破棄する
			delete[] m_apNumber;
			m_apNumber = NULL;
		}

		// 桁数の設定（取得してきた桁に上書き）
		m_nDigits = nDigits;

		// メモリ確保
		m_apNumber = new CNumber*[m_nDigits];

		if (m_apNumber)
		{// メモリ確保できた
			for (int nCntDigits = 0; nCntDigits < m_nDigits; nCntDigits++)
			{// 桁数分ループ
				m_apNumber[nCntDigits] = CNumber::Create(UI_POS, m_fWidth, m_fHeight, m_UINumType, m_nUV, m_nUV_X, m_nUV_Y);
			}
		}
	}

	// 数字の設定
	int aNumber;
	for (int nCntDigits = 0; nCntDigits < m_nDigits; nCntDigits++)
	{// 桁の小さい方から設定
		aNumber = nCalcNum % (int)powf(10.0f, (float)(nCntDigits + 1)) / (int)powf(10.0f, (float)nCntDigits);	// 数字の計算
		m_apNumber[nCntDigits]->SetTexNumber(aNumber);		// 数字の設定
		m_apNumber[nCntDigits]->SetColor(m_col);
	}
}