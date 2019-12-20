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
class CUI_TEXTURE;

//*****************************************************************************
// 数字UIクラス
//*****************************************************************************
class CUI_NUMBER : public  CScene2D
{
public:
	typedef enum
	{
		UI_NUMTYPE_NONE,								// 何も設定しない
		UI_NUMTYPE_REMAINBULLET,			// 残弾
		UI_NUMTYPE_PLAYER_HP,					// プレイヤーの体力
		UI_NUMTYPE_ALLY_HP,						// 味方の体力
		UI_NUMTYPE_DRONE_HP,					// ドローンの体力
		UI_NUMTYPE_WORKER_HP,					// ワーカーの体力
		UI_NUMTYPE_ALLY_DRONE_HP,			// 味方ドローンの体力
		UI_NUMTYPE_ALLY_WORKER_HP,		// 味方ワーカーの体力
		UI_NUMTYPE_BLUE,								// BLUEチームのチケット数
		UI_NUMTYPE_RED,								// REDチームのチケット数
		UI_NUMTYPE_CNTRESPAWN,				// 戦線復帰カウント
		UINUMTYPE_MAX									// タイプの最大数
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
	void SetNum(int nCalcNum);																// 数字を更新する処理

	UI_NUMTYPE GetNumType(void) { return m_UINumType; }				// UIタイプの取得
	int GetRespawn(void) { return m_nCntRespawn; }								// 戦線復帰カウンターの取得
	void SetTime(int nTime) { m_nTimer = nTime; }								// タイマーの設定
	int GetDigits(void) { return m_nDigits; }												// 現在の数字の桁を取得
	void SetDigits(int nDigits) { m_nDigits = nDigits; }							// 桁の設定

																					// public変数
	UI_NUMTYPE		m_UINumType;		// 数字UIのタイプ管理

private:
	int						m_nRemBullet;							// 取得してきた残弾の管理
	int						m_nPlayerLife;							// プレイヤーのライフ
	int						m_nCntRespawn;						// 戦線復帰カウンター
	int						m_nDroneLife, m_nWorkerLife;	// AIのライフ
	D3DXVECTOR3	m_pos;											// 位置
	D3DXCOLOR		m_col;											// カラー
	float						m_fWidth, m_fHeight;					// 幅、高さ
	float						 m_fInt;										// 数字の間隔
	CNumber				**m_apNumber;							// ナンバー変数へのポインタ
	int						m_nDigits;									// 桁数
	int						m_nDiff;										// 差分
	int						m_nUV, m_nUV_X, m_nUV_Y;		// テクスチャUV
	int						m_nTimer;									// カウンター
	CGauge2D			*m_pGaugeBlue;							// BLUEチームゲージ
	CGauge2D			*m_pGaugeRed;							// REDチームゲージ
	int						m_nDamege, m_nInitGauge;		// ダメージ量保存、ゲージ値保存
	CUI_TEXTURE		*m_pUITex;									// ゲージフレーム
	int						m_nCntMove;								// スクロールカウンター
};

#endif