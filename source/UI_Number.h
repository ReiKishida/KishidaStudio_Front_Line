//=============================================================================
//
// UI_Number処理 [UI_Number.h]
// Author : Ayaka Hakozaki
//
//=============================================================================
#ifndef _UI_NUMBER_H_
#define _UI_NUMBER_H_

#include "scene2D.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define UI_NUMBER_PRIORITY	(6)		// 処理の優先順位

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CNumber;
class CGauge2D;

//*****************************************************************************
// 数字UIクラス
//*****************************************************************************
class CUI_NUMBER : public  CScene2D
{
public:
	typedef enum
	{
		UI_NUMTYPE_NONE,						// 何も設定しない
		UI_NUMTYPE_REMAINBULLET,	// 残弾
		UI_NUMTYPE_PLAYER_HP,			// プレイヤーの体力
		UI_NUMTYPE_AI00_HP,					// ドローンの体力
		UI_NUMTYPE_AI01_HP,					// ワーカーの体力
		UI_NUMTYPE_BLUE,						// BLUEチームのチケット数
		UI_NUMTYPE_RED,						// REDチームのチケット数
		UI_NUMTYPE_CNTRESPAWN,		// 戦線復帰カウント
		UINUMTYPE_MAX							// タイプの最大数
	} UI_NUMTYPE;

	CUI_NUMBER(int nPriority = UI_NUMBER_PRIORITY, CScene::OBJTYPE objType = CScene::OBJTYPE_UI_NUMBER);
	~CUI_NUMBER();
	static CUI_NUMBER *Create(D3DXVECTOR3 pos, float fWidth, float fHeight, float fInt, UI_NUMTYPE UINumType, int nUV, int nUV_X, int nUV_Y);		// 生成
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//****************************************
	// 各設定
	//****************************************
	void SetNumColor(int nNum, int nDefNum);										// 値によって色変わる処理
	void SetNumDisPlay(int nGetValue, UI_NUMTYPE UINumType);		// 数字を表示するだけの処理
	void SetNum(int nCalcNum, int nDefNum, D3DXCOLOR col);			// 数字を更新する処理
	void SetTime(int nTime) { m_nTimer = nTime; }								// タイマーの設定
	int GetTime(void) { return m_nTimer; }												// タイマーの取得

																						//****************************************
																						// 各タイプの取得と設定
																						//****************************************
																						// タイプ
	void SetNumType(UI_NUMTYPE UINumType) { m_UINumType = UINumType; };
	UI_NUMTYPE GetNumType(void) { return m_UINumType; }

	// 桁
	int GetDigits(void) { return m_nDigits; }
	void SetDigits(int nDigits) { m_nDigits = nDigits; }

	// 残弾
	int GetRemainBullet(void) { return m_nRemBullet; };
	void SetRemainBullet(int nRemBullet) { m_nRemBullet = nRemBullet; }

	// プレイヤー体力
	int GetPlayerLife(void) { return m_nPlayerLife; }
	void SetPlayerLife(int nPlayerLife) { m_nPlayerLife = nPlayerLife; }

	// BLUEチケット
	int GetTeamBlue(void) { return m_nTeamBlue; }
	void SetTeamBlue(int nTeamBlue) { m_nTeamBlue = nTeamBlue; }

	// REDチケット
	int GetTeamRed(void) { return m_nTeamRed; }
	void SetTeamRed(int nTeamRed) { m_nTeamRed = nTeamRed; }

	// 戦線復帰カウンター
	int GetRespawn(void) { return m_nCntRespawn; }
	void SetRespawn(int nCntRespawn) { m_nCntRespawn = nCntRespawn; }

	//****************************************
	// public変数
	//****************************************
	UI_NUMTYPE		m_UINumType;		// 数字UIのタイプ管理

private:
	static int				m_nRemBullet;							// 取得してきた残弾の管理
	static int				m_nPlayerLife;							// プレイヤーのライフ
	static int				m_nTeamBlue, m_nTeamRed;		// BLUEチーム、REDチーム
	static int				m_nCntRespawn;						// 戦線復帰カウンター
	D3DXVECTOR3	m_pos;											// 位置
	D3DXCOLOR		m_col;											// カラー
	float						m_fWidth, m_fHeight;					// 幅、高さ
	float						 m_fInt;										// 数字の間隔
	CNumber				**m_apNumber;							// ナンバー変数へのポインタ
	int						m_nDigits;									// 桁数
	int						m_nDiff;										// 差分
	int						m_nUV, m_nUV_X, m_nUV_Y;		// テクスチャUV
	int						m_nTimer;									// カウンター
	int						m_nDecrease;
	CGauge2D			*m_pGaugeBlue;							// BLUEチームゲージ
	CGauge2D			*m_pGaugeRed;							// REDチームゲージ
	int						m_nDamege, m_nInitGauge;		// ダメージ量保存、ゲージ値保存
	int						m_nNum;										// ゲージの値計算用、ダメージ量
};

#endif