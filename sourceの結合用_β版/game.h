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
#include "mechaSelect.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define GAME_PRIORITY		(6)				// 優先番号
#define GRAVITY				(0.5f)			// 重力
#define NUMTEX_UV_X			(1)
#define NUMTEX_UV_Y			(3)
#define NUM_TEAM			(2)
#define NUM_RESPAWN_POS		(4)
#define NUM_KILL_LOG		(4)
#define NUM_KILL_LOG_PLAYER (2)

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CPause;
class CTimer;
class CPlayer;
class CButtonManagerStrategy;
class CMouseCursor2D;
class CGauge2D;
class CModel;
class CScene3D;
class CMouseCursor;
class CDamageDirection;
class CUI_TEXTURE;
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

	typedef enum
	{// 種類
		TYPE_PLAYER = 0,
		TYPE_DROWN,
		TYPE_WALKER,
		TYPE_MAX,
	}TYPE;

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

	CMouseCursor2D *GetMouseCursor(void) { return m_pMouseCursor; }

	CModel *GetField(void) { return m_pField; };

	CMouseCursor *GetMouse(void) { return m_pMouse; }

	CButtonManagerStrategy *GetButtonManager(void) { return m_pButtonManager; };

	void SetBlueLinkEnergy(int nBlueLinkEnergy) { m_nBlueLinkEnergy = nBlueLinkEnergy; };
	int GetBlueLinkEnergy(void) { return m_nBlueLinkEnergy; };

	void SetRedLinkEnergy(int nRedLinkEnergy) { m_nRedLinkEnergy = nRedLinkEnergy; };
	int GetRedLinkEnergy(void) { return m_nRedLinkEnergy; };

	D3DXVECTOR3 GetRespawnPos(int nTeam, int nPoint) { return m_aRespawnPos[nTeam][nPoint]; };

	void SetKillIdx(int nIdx, int nPlayerIdx) { m_nKillIdx[nIdx] = nPlayerIdx; };
	void SetDeathIdx(int nIdx, int nPlayerIdx) { m_nDeathIdx[nIdx] = nPlayerIdx; };

	void SetLog(int nIdx, bool bLog) { m_bLog[nIdx] = bLog; };
	bool GetLog(int nIdx) { return m_bLog[nIdx]; };

	void SetPlayerType(int nIdx,TYPE type) { m_playerType[nIdx] = type; };

	static void SetMechaType(int nPlayerIdx, CMechaSelect::MECHATYPE type) { m_aMechaType[nPlayerIdx] = type; };
	static CMechaSelect::MECHATYPE GetMechaType(int nPlayerIdx) { return m_aMechaType[nPlayerIdx]; };

	static CDamageDirection *GetDamageDirection(void) { return m_pDamageDirection; };

private:
	void LoadRespawnPos(void);
	void CreatePlayer(void);
	void PrintData(void);
	void PrintCPUData(void);
	void ReadMessage(void);
	char *ReadConnectData(char *pStr);
	char *ReadPlayerData(char *pStr);
	char *ReadCPUData(char *pStr);

	void SetPlayerData(int nPlayerIdx, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 modelRotUp, D3DXVECTOR3 modelRotDown, D3DXVECTOR3 cameraRot);
	void CreatePlayerBullet(int nPlayerIdx, int nNumShoot, int nAttack, D3DXVECTOR3 cameraRot, float *pAngle, float *pAngleV);
	void SetChatData(int nPlayerIdx, int radioChat);
	void SetCPUData(int nPlayerIdx, D3DXVECTOR3 pos, D3DXVECTOR3 rot);
	void CreateCPUBullet(int nPlayerIdx, int nNumShoot, int nAttack, D3DXVECTOR3 cameraRot, float *pAngle, float *pAngleV);

	void CreateActionUI(void);
	void CreateStrategyUI(void);
	void SwicthPart(void);

	void CreateKillLog(void);
	void UpdateKillLog(void);
	void ReleaseKillLog(int nIdx);

	CPause *m_pPause;							// ポーズクラスのポインタ変数
	static STATE m_state;						// ゲームの状態
	bool m_bPause;								// ポーズON/OFFの切替
	int m_modeCounter;							// 遷移するまでのカウンタ
	int	m_nCntEnemyAppear;						// 敵を出現させるカウンタ
	CTimer *m_pTimer;							// タイマークラスのポインタ変数
	static int m_nCurStage;						// 現在のステージ
	static CPlayer *m_pPlayer[MAX_PLAYER_CONNECT];					// プレイヤークラスのポインタ変数
	CButtonManagerStrategy *m_pButtonManager;	// ボタンの管理クラスのポインタ変数
	CMouseCursor2D *m_pMouseCursor;				// マウスカーソル
	PART m_part;								// 現在のパート
	CModel *m_pField;							// 地面のモデル
	CModel *m_pSky;								// スカイドーム
	bool m_bConnect[MAX_PLAYER_CONNECT];

	CMouseCursor *m_pMouse;

	int m_nBlueLinkEnergy;
	int m_nRedLinkEnergy;
	D3DXVECTOR3 m_aRespawnPos[NUM_TEAM][NUM_RESPAWN_POS];

	bool m_bPlayerDeath[MAX_PLAYER_CONNECT];
	bool m_bAIDeath[2][MAX_PLAYER_CONNECT];

	int m_nKillIdx[NUM_KILL_LOG];	//倒した側の番号
	int m_nDeathIdx[NUM_KILL_LOG];	//倒された側の番号
	bool m_bLog[NUM_KILL_LOG];		//ログを使用しているかどうか
	int m_nCntDrawLog[NUM_KILL_LOG];	//ログの描画している間のカウンター
	TYPE m_playerType[NUM_KILL_LOG_PLAYER];
	CUI_TEXTURE *m_apKillLogBase[NUM_KILL_LOG];								// キルログの土台
	CUI_TEXTURE *m_apKillLogPlayerIcon[NUM_KILL_LOG][NUM_KILL_LOG_PLAYER];	// キルログのプレイヤーアイコン
	CUI_TEXTURE *m_apKillLogPlayerIdx[NUM_KILL_LOG][NUM_KILL_LOG_PLAYER];	// キルログのプレイヤー番号


	static CMechaSelect::MECHATYPE m_aMechaType[MAX_PLAYER_CONNECT];

	static CDamageDirection *m_pDamageDirection;	// 攻撃を受けた方向表示
};

#endif