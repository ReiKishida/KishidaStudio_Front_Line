//=============================================================================
//
// ゲーム処理 [game.h]
// Author : Takuto Ishida
//
//=============================================================================
#ifndef _GAME_H_
#define _GAME_H_

#include "scene.h"
#include "server.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define GAME_PRIORITY		(6)				// 優先番号
#define GRAVITY				(0.5f)			// 重力
#define NUMTEX_UV_X			(1)
#define NUMTEX_UV_Y			(3)

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CPause;
class CTimer;
class CPlayer;
class CButtonManagerStrategy;
class CMouseCursor;
class CGauge2D;
class CModel;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CGame : public CScene
{
public:
	typedef enum
	{
		STATE_NONE = 0,	// 何もしていない状態
		STATE_NORMAL,	// 通常状態
		STATE_END,		// ゲームオーバー
		STATE_CLEAR,	// ゲームクリア
		STATE_MAX
	}STATE;

	typedef enum
	{// パートの種類
		PART_ACTION = 0,
		PART_STRATEGY,
		PART_MAX,
	}PART;

	CGame(int nPriority = GAME_PRIORITY, CScene::OBJTYPE objType = CScene::OBJTYPE_GAME);
	~CGame();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static const void SetGameState(STATE state);
	STATE GetGameState(void) { return m_state; };

	const void SetPauseBool(bool bPause) { m_bPause = bPause; };
	bool GetPauseBool(void) { return m_bPause; };

	CPause *GetPause(void) { return m_pPause; };

	static int GetStage(void) { return m_nCurStage; };
	static void SetStage(int stage);

	static CPlayer *GetPlayer(int nPlayerIdx) { return m_pPlayer[nPlayerIdx]; };

	PART GetPart(void) { return m_part; };

	CMouseCursor *GetMouseCursor(void) { return m_pMouseCursor; }

private:
	void PrintData(void);
	void ReadMessage(void);
	char *ReadConnectData(char *pStr);
	char *ReadPlayerData(char *pStr);
	void SetPlayerData(int nPlayerIdx, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 modelRotUp, D3DXVECTOR3 modelRotDown, D3DXVECTOR3 cameraRot);
	void CreatePlayerBullet(int nPlayerIdx,int nNumShoot,int nAttack, D3DXVECTOR3 cameraRot, float *pAngle, float *pAngleV);
	void SetChatData(int nPlayerIdx, int radioChat);

	void CreateActionUI(void);
	void CreateStrategyUI(void);
	void SwicthPart(void);

	CPause *m_pPause;							// ポーズクラスのポインタ変数
	static STATE m_state;						// ゲームの状態
	bool m_bPause;								// ポーズON/OFFの切替
	int m_modeCounter;							// 遷移するまでのカウンタ
	int	m_nCntEnemyAppear;						// 敵を出現させるカウンタ
	CTimer *m_pTimer;							// タイマークラスのポインタ変数
	static int m_nCurStage;						// 現在のステージ
	static CPlayer *m_pPlayer[MAX_CONNECT];					// プレイヤークラスのポインタ変数
	CButtonManagerStrategy *m_pButtonManager;	// ボタンの管理クラスのポインタ変数
	CMouseCursor *m_pMouseCursor;				// マウスカーソル
	PART m_part;								// 現在のパート
	CModel *m_pField;							// 地面のモデル
	CModel *m_pSky;								// スカイドーム
};

#endif