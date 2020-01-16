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
#include "menu.h"
#include "gauge.h"
#include "AI.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define UI_POS	(D3DXVECTOR3((m_pos.x - (nCntDigits * m_fInt)) - m_fWidth / 2, m_pos.y, 0.0f))

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************

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
	m_pGaugeBlue = NULL;
	m_pGaugeRed = NULL;
	m_nDamege = 0;
	m_nInitGauge = 0;
	m_nDiff = 0;
	m_pUITex = NULL;
	m_nCntMove = 0;
	m_nPlayerLife = 0;
	m_nDroneLife = 0;
	m_nWorkerLife = 0;
	m_nRemBullet = 0;
	m_nCntRespawn = 0;
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
		if (CMenu::GetMode() == CMenu::MODE_MULTI)
		{
			if (CManager::GetClient() != NULL)
			{
				m_nRemBullet = CManager::GetGame()->GetPlayer(CManager::GetClient()->GetPlayerIdx())->GetRemainBullet();
			}
		}
		else if (CMenu::GetMode() == CMenu::MODE_SINGLE)
		{	// プレイヤーから弾を取得
			m_nRemBullet = CManager::GetGame()->GetPlayer(0)->GetBulletCapacity();
		}
		break;

	case UI_NUMTYPE_PLAYER_HP:
		if (CMenu::GetMode() == CMenu::MODE_MULTI)
		{
			if (CManager::GetClient() != NULL)
			{
				m_nPlayerLife = CManager::GetGame()->GetPlayer(CManager::GetClient()->GetPlayerIdx())->GetLife();
			}
		}
		else if (CMenu::GetMode() == CMenu::MODE_SINGLE)
		{	// プレイヤーからライフを取得
			m_nPlayerLife = CManager::GetGame()->GetPlayer(0)->GetLife();
		}
		break;

	case UI_NUMTYPE_ALLY_HP:		// チーム味方
		if (CMenu::GetMode() == CMenu::MODE_MULTI)
		{
			if (CManager::GetClient() != NULL)
			{
				int nPlayerIdx = 0;
				switch (CManager::GetClient()->GetPlayerIdx())
				{
				case 0:
					nPlayerIdx = 1;
					break;
				case 1:
					nPlayerIdx = 0;
					break;
				case 2:
					nPlayerIdx = 3;
					break;
				case 3:
					nPlayerIdx = 2;
					break;
				}
				m_nPlayerLife = CManager::GetGame()->GetPlayer(nPlayerIdx)->GetLife();
			}
		}
		else if (CMenu::GetMode() == CMenu::MODE_SINGLE)
		{	// プレイヤーからライフを取得
			m_nPlayerLife = CManager::GetGame()->GetPlayer(1)->GetLife();
		}
		break;

	case UI_NUMTYPE_DRONE_HP:		// 自分のドローン
		if (CMenu::GetMode() == CMenu::MODE_MULTI)
		{
			if (CManager::GetClient() != NULL)
			{	// 全プレイヤーから、ライフを取得
				m_nDroneLife = CManager::GetGame()->GetPlayer(CManager::GetClient()->GetPlayerIdx())->GetMyAI(0)->GetLife();
			}
		}
		else if (CMenu::GetMode() == CMenu::MODE_SINGLE)
		{	// プレイヤーからライフを取得
			m_nDroneLife = CManager::GetGame()->GetPlayer(0)->GetMyAI(0)->GetLife();		// 1P
		}
		break;

	case UI_NUMTYPE_WORKER_HP:		// 自分のワーカー
		if (CMenu::GetMode() == CMenu::MODE_MULTI)
		{
			if (CManager::GetClient() != NULL)
			{	// 全プレイヤーから、ライフを取得
				m_nWorkerLife = CManager::GetGame()->GetPlayer(CManager::GetClient()->GetPlayerIdx())->GetMyAI(1)->GetLife();
			}
		}
		if (CMenu::GetMode() == CMenu::MODE_SINGLE)
		{	// プレイヤーからライフを取得
			m_nWorkerLife = CManager::GetGame()->GetPlayer(0)->GetMyAI(1)->GetLife();	// 1P
		}
		break;

	case UI_NUMTYPE_ALLY_DRONE_HP:		// 同チーム味方のドローン
		if (CMenu::GetMode() == CMenu::MODE_MULTI)
		{
			if (CManager::GetClient() != NULL)
			{
				int nPlayerIdx = 0;
				switch (CManager::GetClient()->GetPlayerIdx())
				{
				case 0:
					nPlayerIdx = 1;
					break;

				case 1:
					nPlayerIdx = 0;
					break;

				case 2:
					nPlayerIdx = 3;
					break;

				case 3:
					nPlayerIdx = 2;
					break;
				}
				m_nDroneLife = CManager::GetGame()->GetPlayer(nPlayerIdx)->GetMyAI(0)->GetLife();
			}
		}
		else if (CMenu::GetMode() == CMenu::MODE_SINGLE)
		{	// プレイヤーからライフを取得
			m_nDroneLife = CManager::GetGame()->GetPlayer(1)->GetMyAI(0)->GetLife();		// 2P
		}
		break;

	case UI_NUMTYPE_ALLY_WORKER_HP:		// 同チーム味方のワーカー
		if (CMenu::GetMode() == CMenu::MODE_MULTI)
		{
			if (CManager::GetClient() != NULL)
			{
				int nPlayerIdx = 0;
				switch (CManager::GetClient()->GetPlayerIdx())
				{
				case 0:
					nPlayerIdx = 1;
					break;

				case 1:
					nPlayerIdx = 0;
					break;

				case 2:
					nPlayerIdx = 3;
					break;

				case 3:
					nPlayerIdx = 2;
					break;
				}
				m_nWorkerLife = CManager::GetGame()->GetPlayer(nPlayerIdx)->GetMyAI(1)->GetLife();
			}
		}
		else if (CMenu::GetMode() == CMenu::MODE_SINGLE)
		{	// プレイヤーからライフを取得
			m_nWorkerLife = CManager::GetGame()->GetPlayer(1)->GetMyAI(1)->GetLife();	// 2P
		}
		break;

	case UI_NUMTYPE_CNTRESPAWN:
		m_nCntRespawn = 5;		// カウント初期値
		break;

	case UI_NUMTYPE_BLUE:
		m_nInitGauge = MAX_LINKENERGY;
		if (m_pGaugeBlue == NULL)
		{
			m_pGaugeBlue = CGauge2D::Create(2, m_pos, 0.0f, (float)m_nInitGauge, m_fWidth, m_fHeight);
			m_nInitGauge = CManager::GetGame()->GetBlueLinkEnergy();		// ゲームからBLUEチームチケット取得
			m_pGaugeBlue->AddSubtract((float)m_nInitGauge);			// ゲージの増減
			m_pGaugeBlue->SetValue((float)m_nInitGauge);			// ゲージの値設定

		}
		if (m_pGaugeBlue != NULL)
		{
			m_pGaugeBlue->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), 0);	// 元の長さ
			m_pGaugeBlue->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 1);	// 現在の体力
			m_pGaugeBlue->BindTexture(CTexture::GetTexture(CTexture::TEXTURE_LINK_BLUE), 1);
		}
		if (m_pUITex == NULL)
		{
			m_pUITex = CUI_TEXTURE::Create(m_pos, m_fWidth, m_fHeight, CUI_TEXTURE::UIFLAME_LINK_FLAME);
		}
		break;

	case UI_NUMTYPE_RED:
		m_nInitGauge = MAX_LINKENERGY;

		if (m_pGaugeRed == NULL)
		{
			m_pGaugeRed = CGauge2D::Create(2, m_pos, 0.0f, (float)m_nInitGauge, m_fWidth, m_fHeight);
			m_pGaugeRed->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), 0);	// 元の長さ
			m_pGaugeRed->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 1);	// 現在の体力
			m_pGaugeRed->BindTexture(CTexture::GetTexture(CTexture::TEXTURE_LINK_RED), 1);
			m_nInitGauge = CManager::GetGame()->GetRedLinkEnergy();		// ゲームからBLUEチームチケット取得
			m_pGaugeRed->AddSubtract((float)m_nInitGauge);			// ゲージの増減
			m_pGaugeRed->SetValue((float)m_nInitGauge);			// ゲージの値設定
		}
		if (m_pUITex == NULL)
		{
			m_pUITex = CUI_TEXTURE::Create(m_pos, m_fWidth, m_fHeight, CUI_TEXTURE::UIFLAME_LINK_FLAME);
		}
		break;
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CUI_NUMBER::Uninit(void)
{
	if (m_pUITex != NULL)
	{	// リンクゲージの破棄
		m_pUITex->Uninit();
		m_pUITex = NULL;
	}

	if (m_pGaugeBlue != NULL)
	{	// BLUEゲージ
		m_pGaugeBlue->Uninit();
		m_pGaugeBlue = NULL;
	}

	if (m_pGaugeRed != NULL)
	{	// REDゲージ
		m_pGaugeRed->Uninit();
		m_pGaugeRed = NULL;
	}

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

																	// カウンター加算
	m_nCntMove++;

	//****************************************
	// タイプ別処理
	//****************************************
	switch (m_UINumType)
	{
	case UI_NUMTYPE_REMAINBULLET:
		if (CMenu::GetMode() == CMenu::MODE_MULTI)
		{
			if (CManager::GetClient() != NULL)
			{
				if (CManager::GetClient()->GetConnect() == true)
				{
					m_nRemBullet = CGame::GetPlayer(CManager::GetClient()->GetPlayerIdx())->GetRemainBullet();			// プレイヤーから現在の弾の値を取得
					SetNumColor(m_nRemBullet, CGame::GetPlayer(CManager::GetClient()->GetPlayerIdx())->GetBulletCapacity());				// 値によって色変化する

					if (m_nRemBullet >= 0)
					{	// 弾が残っていたら更新
						SetNum(m_nRemBullet);
					}
				}
			}
			else
			{
				m_nRemBullet = CGame::GetPlayer(0)->GetRemainBullet();			// プレイヤーから現在の弾の値を取得
				SetNumColor(m_nRemBullet, CGame::GetPlayer(0)->GetBulletCapacity());				// 値によって色変化する

				if (m_nRemBullet >= 0)
				{	// 弾が残っていたら更新
					SetNum(m_nRemBullet);
				}
			}
		}
		else if (CMenu::GetMode() == CMenu::MODE_SINGLE)
		{
			m_nRemBullet = CGame::GetPlayer(0)->GetRemainBullet();									// プレイヤーから現在の弾の値を取得
			SetNumColor(m_nRemBullet, CGame::GetPlayer(0)->GetBulletCapacity());			// 値によって色変化する

			if (m_nRemBullet >= 0)
			{	// 弾が残っていたら更新
				SetNum(m_nRemBullet);
			}
		}
		break;

	case UI_NUMTYPE_PLAYER_HP:
		if (CMenu::GetMode() == CMenu::MODE_MULTI)
		{
			if (CManager::GetClient() != NULL)
			{
				if (CManager::GetClient()->GetConnect() == true)
				{
					m_nPlayerLife = CGame::GetPlayer(CManager::GetClient()->GetPlayerIdx())->GetLife();
					SetNumColor(m_nPlayerLife, CGame::GetPlayer(CManager::GetClient()->GetPlayerIdx())->GetLifeMax());

					if (m_nPlayerLife >= 0)
					{	// 0以上の時、数字更新
						SetNum(m_nPlayerLife);
					}
				}
			}
			else
			{
				m_nPlayerLife = CGame::GetPlayer(CManager::GetClient()->GetPlayerIdx())->GetLife();
				SetNumColor(m_nPlayerLife, CGame::GetPlayer(0)->GetLifeMax());

				if (m_nPlayerLife >= 0)
				{	// 0以上の時、数字更新
					SetNum(m_nPlayerLife);
				}
			}
		}
		else if (CMenu::GetMode() == CMenu::MODE_SINGLE)
		{
			m_nPlayerLife = CGame::GetPlayer(0)->GetLife();
			SetNumColor(m_nPlayerLife, CGame::GetPlayer(0)->GetLifeMax());

			if (m_nPlayerLife >= 0)
			{	// 0以上の時、数字更新
				SetNum(m_nPlayerLife);
			}
		}
		break;

	case UI_NUMTYPE_ALLY_HP:
		if (CMenu::GetMode() == CMenu::MODE_MULTI)
		{
			if (CManager::GetClient() != NULL)
			{
				if (CManager::GetClient()->GetConnect() == true)
				{
					int nPlayerIdx = 0;
					switch (CManager::GetClient()->GetPlayerIdx())
					{
					case 0:
						nPlayerIdx = 1;
						break;
					case 1:
						nPlayerIdx = 0;
						break;
					case 2:
						nPlayerIdx = 3;
						break;
					case 3:
						nPlayerIdx = 2;
						break;
					}

					m_nPlayerLife = CManager::GetGame()->GetPlayer(nPlayerIdx)->GetLife();	// 味方のライフを取得
					SetNumColor(m_nPlayerLife, CGame::GetPlayer(CManager::GetClient()->GetPlayerIdx())->GetLifeMax());

					if (m_nPlayerLife >= 0)
					{	// 0以上の時、数字更新
						SetNum(m_nPlayerLife);
					}
				}
			}
			else
			{
				int nPlayerIdx = 0;
				switch (CManager::GetClient()->GetPlayerIdx())
				{
				case 0:
					nPlayerIdx = 1;
					break;
				case 1:
					nPlayerIdx = 0;
					break;
				case 2:
					nPlayerIdx = 3;
					break;
				case 3:
					nPlayerIdx = 2;
					break;
				}

				m_nPlayerLife = CManager::GetGame()->GetPlayer(nPlayerIdx)->GetLife();	// 味方のライフを取得
				SetNumColor(m_nPlayerLife, CGame::GetPlayer(1)->GetLifeMax());

				if (m_nPlayerLife >= 0)
				{	// 0以上の時、数字更新
					SetNum(m_nPlayerLife);
				}
			}
		}
		else if (CMenu::GetMode() == CMenu::MODE_SINGLE)
		{
			m_nPlayerLife = CGame::GetPlayer(1)->GetLife();
			SetNumColor(m_nPlayerLife, CGame::GetPlayer(1)->GetLifeMax());

			if (m_nPlayerLife >= 0)
			{	// 0以上の時、数字更新
				SetNum(m_nPlayerLife);
			}
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
			SetNum(m_nCntRespawn);
		}

		SetTime(m_nTimer);
		break;

	case UI_NUMTYPE_BLUE:
		if (m_pGaugeBlue != NULL)
		{
			m_nDiff = m_nInitGauge - CManager::GetGame()->GetBlueLinkEnergy();		// ダメージ量取得

			if (m_nDiff > 0)
			{
				m_nInitGauge--;
				m_nDiff--;
			}

			m_pGaugeBlue->AddSubtract((float)m_nInitGauge);	// ゲージの増減
			m_pGaugeBlue->SetValue((float)m_nInitGauge);			// ゲージの値設定
		}
		break;

	case UI_NUMTYPE_RED:
		if (m_pGaugeRed != NULL)
		{
			m_nDiff = m_nInitGauge - CManager::GetGame()->GetRedLinkEnergy();		// ダメージ量取得

			if (m_nDiff > 0)
			{
				m_nInitGauge--;
				m_nDiff--;
			}

			m_pGaugeRed->AddSubtract((float)m_nInitGauge);		// ゲージの増減
			m_pGaugeRed->SetValue((float)m_nInitGauge);			// ゲージの値設定
		}
		break;

	case UI_NUMTYPE_DRONE_HP:
		if (CMenu::GetMode() == CMenu::MODE_MULTI)
		{
			if (CManager::GetClient() != NULL)
			{
				if (CManager::GetClient()->GetConnect() == true)
				{
					m_nDroneLife = CGame::GetPlayer(CManager::GetClient()->GetPlayerIdx())->GetMyAI(0)->GetLife();
					SetNumColor(m_nDroneLife, CGame::GetPlayer(CManager::GetClient()->GetPlayerIdx())->GetMyAI(0)->GetLifeMax());

					if (m_nDroneLife >= 0)
					{	// 0以上の時、数字更新
						SetNum(m_nDroneLife);
					}
				}
			}
			else
			{
				m_nDroneLife = CGame::GetPlayer(CManager::GetClient()->GetPlayerIdx())->GetMyAI(0)->GetLife();
				SetNumColor(m_nDroneLife, CGame::GetPlayer(0)->GetLifeMax());

				if (m_nDroneLife >= 0)
				{	// 0以上の時、数字更新
					SetNum(m_nDroneLife);
				}
			}
		}
		else if (CMenu::GetMode() == CMenu::MODE_SINGLE)
		{
			m_nDroneLife = CManager::GetGame()->GetPlayer(0)->GetMyAI(0)->GetLife();
			SetNumColor(m_nDroneLife, CGame::GetPlayer(0)->GetMyAI(0)->GetLifeMax());

			if (m_nDroneLife >= 0)
			{	// 0以上の時、数字更新
				SetNum(m_nDroneLife);
			}
		}
		break;

	case UI_NUMTYPE_WORKER_HP:
		if (CMenu::GetMode() == CMenu::MODE_MULTI)
		{
			if (CManager::GetClient() != NULL)
			{
				if (CManager::GetClient()->GetConnect() == true)
				{
					m_nWorkerLife = CGame::GetPlayer(CManager::GetClient()->GetPlayerIdx())->GetMyAI(1)->GetLife();
					SetNumColor(m_nWorkerLife, CGame::GetPlayer(CManager::GetClient()->GetPlayerIdx())->GetMyAI(1)->GetLifeMax());

					if (m_nWorkerLife >= 0)
					{	// 0以上の時、数字更新
						SetNum(m_nWorkerLife);
					}
				}
			}
			else
			{
				m_nWorkerLife = CGame::GetPlayer(CManager::GetClient()->GetPlayerIdx())->GetMyAI(1)->GetLife();
				SetNumColor(m_nWorkerLife, CGame::GetPlayer(0)->GetLifeMax());

				if (m_nWorkerLife >= 0)
				{	// 0以上の時、数字更新
					SetNum(m_nWorkerLife);
				}
			}
		}
		else if (CMenu::GetMode() == CMenu::MODE_SINGLE)
		{
			m_nWorkerLife = CManager::GetGame()->GetPlayer(0)->GetMyAI(1)->GetLife();
			SetNumColor(m_nWorkerLife, CGame::GetPlayer(0)->GetMyAI(1)->GetLifeMax());

			if (m_nWorkerLife >= 0)
			{	// 0以上の時、数字更新
				SetNum(m_nWorkerLife);
			}
		}
		break;

	case UI_NUMTYPE_ALLY_DRONE_HP:
		if (CMenu::GetMode() == CMenu::MODE_MULTI)
		{
			if (CManager::GetClient() != NULL)
			{
				if (CManager::GetClient()->GetConnect() == true)
				{
					int nPlayerIdx = 0;
					switch (CManager::GetClient()->GetPlayerIdx())
					{
					case 0:
						nPlayerIdx = 1;
						break;
					case 1:
						nPlayerIdx = 0;
						break;
					case 2:
						nPlayerIdx = 3;
						break;
					case 3:
						nPlayerIdx = 2;
						break;
					}

					m_nDroneLife = CManager::GetGame()->GetPlayer(nPlayerIdx)->GetMyAI(0)->GetLife();	// 味方ドローンライフを取得
					SetNumColor(m_nDroneLife, CGame::GetPlayer(CManager::GetClient()->GetPlayerIdx())->GetMyAI(0)->GetLifeMax());

					if (m_nDroneLife >= 0)
					{	// 0以上の時、数字更新
						SetNum(m_nDroneLife);
					}
				}
			}
			else
			{
				int nPlayerIdx = 0;
				switch (CManager::GetClient()->GetPlayerIdx())
				{
				case 0:
					nPlayerIdx = 1;
					break;
				case 1:
					nPlayerIdx = 0;
					break;
				case 2:
					nPlayerIdx = 3;
					break;
				case 3:
					nPlayerIdx = 2;
					break;
				}

				m_nDroneLife = CManager::GetGame()->GetPlayer(nPlayerIdx)->GetMyAI(0)->GetLife();	// 味方ドローンライフを取得
				SetNumColor(m_nDroneLife, CGame::GetPlayer(0)->GetLifeMax());

				if (m_nDroneLife >= 0)
				{	// 0以上の時、数字更新
					SetNum(m_nDroneLife);
				}
			}
		}
		else if (CMenu::GetMode() == CMenu::MODE_SINGLE)
		{
			m_nDroneLife = CManager::GetGame()->GetPlayer(1)->GetMyAI(0)->GetLife();
			SetNumColor(m_nDroneLife, CGame::GetPlayer(1)->GetMyAI(0)->GetLifeMax());

			if (m_nDroneLife >= 0)
			{	// 0以上の時、数字更新
				SetNum(m_nDroneLife);
			}
		}
		break;

	case UI_NUMTYPE_ALLY_WORKER_HP:
		if (CMenu::GetMode() == CMenu::MODE_MULTI)
		{
			if (CManager::GetClient() != NULL)
			{
				if (CManager::GetClient()->GetConnect() == true)
				{
					int nPlayerIdx = 0;
					switch (CManager::GetClient()->GetPlayerIdx())
					{
					case 0:
						nPlayerIdx = 1;
						break;
					case 1:
						nPlayerIdx = 0;
						break;
					case 2:
						nPlayerIdx = 3;
						break;
					case 3:
						nPlayerIdx = 2;
						break;
					}

					m_nWorkerLife = CManager::GetGame()->GetPlayer(nPlayerIdx)->GetMyAI(1)->GetLife();	// 味方ワーカーライフを取得
					SetNumColor(m_nWorkerLife, CGame::GetPlayer(CManager::GetClient()->GetPlayerIdx())->GetMyAI(1)->GetLifeMax());

					if (m_nWorkerLife >= 0)
					{	// 0以上の時、数字更新
						SetNum(m_nWorkerLife);
					}
				}
			}
			else
			{
				int nPlayerIdx = 0;
				switch (CManager::GetClient()->GetPlayerIdx())
				{
				case 0:
					nPlayerIdx = 1;
					break;
				case 1:
					nPlayerIdx = 0;
					break;
				case 2:
					nPlayerIdx = 3;
					break;
				case 3:
					nPlayerIdx = 2;
					break;
				}

				m_nWorkerLife = CManager::GetGame()->GetPlayer(nPlayerIdx)->GetMyAI(1)->GetLife();	// 味方ワーカーライフを取得
				SetNumColor(m_nWorkerLife, CGame::GetPlayer(CManager::GetClient()->GetPlayerIdx())->GetMyAI(1)->GetLifeMax());

				if (m_nWorkerLife >= 0)
				{	// 0以上の時、数字更新
					SetNum(m_nWorkerLife);
				}
			}
		}
		else if (CMenu::GetMode() == CMenu::MODE_SINGLE)
		{
			m_nWorkerLife = CManager::GetGame()->GetPlayer(1)->GetMyAI(1)->GetLife();
			SetNumColor(m_nWorkerLife, CGame::GetPlayer(1)->GetMyAI(1)->GetLifeMax());

			if (m_nWorkerLife >= 0)
			{	// 0以上の時、数字更新
				SetNum(m_nWorkerLife);
			}
		}
		break;
	}
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

	if (nNum <= nDefNum / 2)
	{	// 半分になったら黄色
		m_col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
	}

	if (nNum <= nDefNum / 4)
	{	// 4/1になったら赤
		m_col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	}
}

//=============================================================================
// 現在の値・初期値・色：取得してきた数字を表示して更新する処理
//=============================================================================
void CUI_NUMBER::SetNum(int nCalcNum)
{
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