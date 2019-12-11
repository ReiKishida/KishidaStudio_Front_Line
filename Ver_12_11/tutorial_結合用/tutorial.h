//=============================================================================
//
// チュートリアル処理 [tutorial.h]
// Author : Takuto Ishida & Rei Kishida
//
//=============================================================================
#ifndef _TUTORIAL_H_
#define _TUTORIAL_H_

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
#define MAX_PLAYER_TUTORIAL (2)
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
class CUI_TEXTURE;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CTutorial : public CScene
{
public:
	typedef enum
	{
		TUTORIALSTATE_NONE = 0,
		TUTORIALSTATE_NORMAL,
		TUTORIALSTATE_CLEAR,
		TUTORIALSTATE_MAX
	}TUTORIALSTATE;

	typedef enum
	{
		CLEARSTATE_NONE = 0,
		CLEARSTATE_TRUE,
		CLEARSTATE_MAX
	}CLEARSTATE;

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

	CTutorial(int nPriority = GAME_PRIORITY, CScene::OBJTYPE objType = CScene::OBJTYPE_GAME);
	~CTutorial();
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

	static void SetMechaType(int nPlayerIdx, CMechaSelect::MECHATYPE type) { m_aMechaType[nPlayerIdx] = type; };
	static CMechaSelect::MECHATYPE GetMechaType(int nPlayerIdx) { return m_aMechaType[nPlayerIdx]; };

private:
	void TutorialBasicMovement(void);
	void TutorialAction(void);
	void TutorialStrategy(void);


	void SetTexTutorial(int nType, int nSteps);
	void LoadRespawnPos(void);

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

	CPause *m_pPause;							// ポーズクラスのポインタ変数
	static STATE m_state;						// ゲームの状態
	bool m_bPause;								// ポーズON/OFFの切替
	int m_modeCounter;							// 遷移するまでのカウンタ
	int	m_nCntEnemyAppear;						// 敵を出現させるカウンタ
	CTimer *m_pTimer;							// タイマークラスのポインタ変数
	static int m_nCurStage;						// 現在のステージ
	static CPlayer *m_pPlayer[MAX_PLAYER_TUTORIAL];					// プレイヤークラスのポインタ変数
	CButtonManagerStrategy *m_pButtonManager;	// ボタンの管理クラスのポインタ変数
	CMouseCursor2D *m_pMouseCursor;				// マウスカーソル
	PART m_part;								// 現在のパート
	CModel *m_pField;							// 地面のモデル
	CModel *m_pSky;								// スカイドーム
	bool m_bConnect[MAX_PLAYER_TUTORIAL];

	CMouseCursor *m_pMouse;

	int m_nBlueLinkEnergy;
	int m_nRedLinkEnergy;
	D3DXVECTOR3 m_aRespawnPos[NUM_TEAM][NUM_RESPAWN_POS];

	bool m_bPlayerDeath[MAX_PLAYER_TUTORIAL];
	bool m_bAIDeath[2][MAX_PLAYER_TUTORIAL];

	static CMechaSelect::MECHATYPE m_aMechaType[MAX_PLAYER_TUTORIAL];

	static int m_nType;	//チュートリアルの種類

	static int m_nSteps;	// 進行度
	static int m_nStepsMax;	// 進行度の最大数
	static int m_nInstMax;	// 説明の最大数


	//チュートリアルで使うテクスチャクラス
	CUI_TEXTURE *m_apSteps[3];	//UIテクスチャクラスのポインタ(進行度)
	CUI_TEXTURE *m_pMission;	//UIテクスチャクラスのポインタ(目標)
	CUI_TEXTURE *m_pInst;		//UIテクスチャクラスのポインタ(説明)

	//基本操作のクリア状態
	CLEARSTATE m_R_CameraState;	//右を向くときのクリア状態
	CLEARSTATE m_L_CameraState;	//左を向くときのクリア状態
	CLEARSTATE m_U_CameraState;	//上を向くときのクリア状態
	CLEARSTATE m_D_CameraState;	//下を向くときのクリア状態
	CLEARSTATE m_Move_State;	//移動のクリア状態

	//アクションパートのクリア状態
	CLEARSTATE m_AttackState;	//攻撃のクリア状態
	CLEARSTATE m_ReloadState;	//リロードのクリア状態

	//チュートリアルの状態
	static TUTORIALSTATE m_TutorialState;	//チュートリアルの状態
	CUI_TEXTURE *m_Succes;	//チュートリアルの完了のテクスチャ

	int m_nDisp;		//クリアを表示する時間


};

#endif