//=============================================================================
//
// リザルト処理 [result.h]
// Author : Ayaka Hakozaki
//
//=============================================================================
#ifndef _RESULT_H_
#define _RESULT_H_

#include "scene.h"
#include "game.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define RESULT_UI_TEX			(2)		// 使用するUIテクスチャの枚数

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CNumber;
class CLogo;
class CLogoPressButton;
class CScore;
class CUI_TEXTURE;
class CModel;
class CPlayer;
class CMechaSelect;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CResult : public CScene
{
public:
	typedef enum
	{	// 勝ったチーム
		TEAM_WIN_BLUE = 0,
		TEAM_WIN_RED,
		TEAM_WIN_MAX
	} TEAM_WIN;

	CResult();
	~CResult();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static void SetTeamWin(TEAM_WIN teamWin) { m_teamWin = teamWin; }

private:
	static TEAM_WIN		m_teamWin;								// 勝ったチームを取得
	static CPlayer				*m_pPlayer[MAX_PLAYER_CONNECT];
	CMechaSelect::MECHATYPE	m_mechaType[MAX_PLAYER_CONNECT];
	CModel						*m_pField;									// 地面のモデル
	CUI_TEXTURE				*m_pUITex[RESULT_UI_TEX];	// UIテクスチャポインタ
	int								m_nCntFlash;								// プレスボタン点滅
};

#endif