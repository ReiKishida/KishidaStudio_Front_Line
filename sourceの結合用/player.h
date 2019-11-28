//=============================================================================
//
// プレイヤー処理 [player.h]
// Author : Takuto Ishida
//
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "scene.h"
#include "mechaSelect.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define PLAYER_PRIORITY		(4)		// 処理の優先番号
#define TEAM_BLUE			(100)
#define TEAM_RED			(100)
#define PLAYER_UI_NUM		(2)		// 数字UIの枚数
#define COLLECTIONDATA_MAX	(12)	// 収集するデータの最大数
#define RANDOM_MOVE_POINT	(10)	// ランダム移動の地点数
#define ENEMY_PLAYER_MAX	(2)		// 敵プレイヤーの数
#define NODE_MAX			(256)	// ノードの最大数

// リロード
#define RELOAD_TEX									(2)			// リロードで使用するテクスチャ数

// リスポーン
#define RESPAWN_TEX								(3)			// リスポーンで使用するテクスチャ数

// リスポーン位置選択
#define SELECTRESPAWN_BOTTON			(4)			// リスポーン位置選択ボタン
#define SERECTRESPAWN_TEX					(4)			// リスポーン位置選択中のテクスチャ数

// ラジオチャット処理
#define RADIOCHAT_BOTTON					(8)			// ラジオチャットボタンの数
#define RADIOCHAT_MESSAGE					(2)			// ラジオチャットボタンの数
#define RADIOCHAT_BOTTON_PATTERN	(8)			// ラジオチャット画像のパターン数
#define RADIOCHAT_BOTTON_WIDTH		(200.0f)	// ボタンの幅
#define RADIOCHAT_BOTTON_HEIGHT	(200.0f)	// ボタンの高さ
#define RADIOCHAT_MESS_WIDTH			(400.0f)	// メッセージの幅
#define RADIOCHAT_MESS_HEIGHT			(80.0f)		// メッセージの高さ

#define AI_MAX					(2)		// 一人当たりのAIの数

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CModel;
class CMotion;
class CShadow;
class CScene3DBill;
class CUI_TEXTURE;
class CGauge2D;
class CUI_NUMBER;
class CMouseCursor2D;
class CButton2D;
class CInputMouse;
class CAIMecha;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CPlayer : public CScene
{
public:
	typedef enum
	{// 状態
		STATE_APPEAR = 0,	// 出現
		STATE_NORMAL,		// 通常
		STATE_DAMAGE,		// ダメージ
		STATE_DEATH,		// 死亡
		STATE_MAX
	}STATE;

	typedef enum
	{	// リスポーン時の状態
		RESPAWN_NONE = 0,		// 通常状態
		RESPAWN_START,				// 戦闘開始
		RESPAWN_DEATH,				// 行動不能
		RESPAWN_SELECT,			// リスポーン選択
		RESPAWN_MAX
	} RESPAWN;

	typedef enum
	{	// リスポーン地点
		POINT_A = 0,
		POINT_B,
		POINT_C,
		POINT_D,
		POINT_MAX
	} POINT;

	typedef enum
	{	// ラジオチャットの種類
		RADIOCHAT_OK = 0,			// 了解
		RADIOCHAT_OPPSITE,		// 反対
		RADIOCHAT_THANKS,		// 感謝
		RADIOCHAT_APOLOGY,		// 謝罪
		RADIOCHAT_NICESHOOT,	// ナイスショット
		RADIOCHAT_FOLLOW,		// ついてこい！
		RADIOCHAT_RECESSION,	// 後退しろ！
		RADIOCHAT_SUPPORT,		// 援護しろ！
		RADIOCHAT_MAX
	} RADIOCHAT;

	// =============================================================
	// ダイクストラ法によるルート探索
	// =============================================================
	typedef struct
	{
		std::vector<int> to;		// どのノードとつながっているか
		std::vector<float> cost;	// エッジのコスト

		// ダイクストラ法のために必要な情報
		bool done;		// 確定ノードかどうか
		float minCost;	// スタートノードからの最小コスト
		int from;		// どのノードから来たか
	}Node;

	typedef struct
	{
		int nodeMax;							// ノードの総数
		int	index[NODE_MAX];					// 自分のノード番号
		int connectNum[NODE_MAX];				// 接続ノード数
		int connectIndex[NODE_MAX][NODE_MAX];	// 接続ノード番号
		D3DXVECTOR3 pos[NODE_MAX];				// 各ノードの位置
	}NodeState;

	// =============================================================

	CPlayer(int nPriority = PLAYER_PRIORITY, CScene::OBJTYPE objType = CScene::OBJTYPE_PLAYER);
	~CPlayer();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CPlayer* Create(int nPlayerIdx, CMechaSelect::MECHATYPE mecha, D3DXVECTOR3 pos, bool bConnect);

	D3DXVECTOR3 GetPos(void) { return m_pos; };					// 位置の取得
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; };		// 位置の設定

	D3DXVECTOR3 GetRot(void) { return m_rot; };					// 角度の取得
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; };		// 角度の設定

	D3DXVECTOR3 GetMove(void) { return m_move; };				// 移動量の取得
	void SetMove(D3DXVECTOR3 move) { m_move = move; };	// 移動量の設定

	D3DXMATRIX GetMtxWorld(void) { return m_mtxWorld; };					// ワールドマトリックスの取得
	void SetMtxWorld(D3DXMATRIX mtxWorld) { m_mtxWorld = mtxWorld; };	// ワールドマトリックスの設定

	D3DXVECTOR3 GetVtxMax(void) { return m_vtxMax; }
	D3DXVECTOR3 GetVtxMin(void) { return m_vtxMin; }

	void Damage(int nDamage);

	D3DXVECTOR3 CalcScreenToWorld(float fScreenX, float fScreenY);

	CModel *GetModel(int nIdx) { return m_pModel[nIdx]; };

	int GetPlayerIdx(void) { return m_nPlayerIdx; };
	void SetPlayerIdx(int nIdx) { m_nPlayerIdx = nIdx; };

	bool GetShoot(void) { return m_bShoot; };
	void SetShoot(bool bShoot) { m_bShoot = bShoot; };

	float GetRotDest(void) { return m_fRotDest; };
	void SetRotDest(float fRotDest) { m_fRotDest = fRotDest; };

	int GetBulletCapacity(void) { return m_nCapacity; }
	int GetAttack(void) { return m_nAttack; }

	RESPAWN GetRespawn(void) { return m_Respawn; }

	float GetAngle(int nIdx) { return m_pAngle[nIdx]; }
	float GetAngleV(int nIdx) { return m_pAngleV[nIdx]; }

	int GetNumShoot(void) { return m_nNumShoot; }

	void SetTeam(int nTeam) { m_nTeam = nTeam; };
	int GetTeam(void) { return m_nTeam; };
	bool &GetDeath(void) { return m_bDeath; };

	int GetLifeMax(void) { return m_nLifeMax; }

	// ラジオチャット
	RADIOCHAT GetRadioChat(void) { return m_radiochat; }									// ラジオチャット情報の取得
	void SetRadioChat(RADIOCHAT radiochat) { m_radiochat = radiochat; }			// ラジオチャットの設定
	bool GetChat(void) { return m_bChat; }																// チャット情報の取得
	void SetAllyChat(bool bAllyChat) { m_bAllyChat = bAllyChat; }							// 味方のチャットが使用しているかどうかの設定
	void SetAllyRadioChat(RADIOCHAT allyRadioChat) { m_allyRadiochat = allyRadioChat; }		// 味方のチャット情報の設定
	bool GetChatBotton(void) { return m_bChatBotton; }

	// =============================================================
	// AI関係
	// =============================================================
	void AIUpdate(void);	// AIの更新
	void AutoMove(void);	// 自動移動
	void NodeSearch(void);	// プレイヤー座標からノード検索
	void RootSearch(void);	// 最短経路検索
	void AddEdge(int first, int second, float weight, Node *node);	// エッジの追加
	void Dijkstra(int nodeMax, int start, int end, Node *node);	// 経路探索
	void FileLoad(char* pFileName);

	CPlayer *GetPlayer(void) { return m_pPlayer; };
	static void SetSearchPos(D3DXVECTOR3 pos) { m_searchPos = pos; };
	// =============================================================

	CAIMecha *GetMyAI(int nIdx) { return m_pAI[nIdx]; };

private:
	void Movement(void);
	void Shoot(void);
	void Angle(void);
	void SetParticle(void);
	void Reload(bool bReload);						// リロード処理
	void Respawn(RESPAWN respawn);		// ライフが0になった時の処理
	void SelectRespawn(void);						// リスポーン位置選択処理
	void ChatBotton(void);							// ラジオチャットボタンの生成
	void ChatMess(bool bChat);					// ボタンが押されて、メッセージ表示

	D3DXMATRIX		m_mtxWorld;			// ワールドマトリックス
	D3DXVECTOR3		m_pos;				// 位置
	D3DXVECTOR3		m_rot;				// 回転
	D3DXVECTOR3		m_move;				// 移動量
	D3DXVECTOR3		m_vtxMax;			// 頂点座標の最大値
	D3DXVECTOR3		m_vtxMin;			// 頂点座標の最小値
	CModel			**m_pModel;			// モデルクラスのポインタ変数
	CMotion			*m_pUpperMotion;	// 上半身モーションクラスのポインタ変数
	CMotion			*m_pLowerMotion;	// 下半身モーションクラスのポインタ変数
	CShadow			*m_pShadow;			// 影のポインタ変数
	float			m_fRotDest;			// 目的の角度
	int				m_nCntState;		// ステートのカウンタ
	int				m_nNumParts;		// パーツ数
	STATE			m_state;			// 状態
	float			m_fCameraAngle;		// カメラの向き
	int				m_nDamageTime;		// ダメージを受けた時の硬直時間
	CScene3DBill	*m_pReticle;		// レティクル
	int m_nTimer;					// タイマー

	int				m_nPlayerIdx;		// プレイヤー番号
	D3DXVECTOR3		m_posOld;			// 過去の位置
	bool			m_bShoot;			// 弾を発射しているかどうか
	int				m_nLife;			// 耐久力

	int				m_nCapacity;		// 装弾数
	int				m_nAttack;			// 攻撃力
	int				m_nNumShoot;		// 同時発射数
	int				m_nDispertion;		// ばらつき
	int				m_nReload;			// リロード時間
	int				m_nLifeMax;			// 耐久力
	float			m_fSpeed;			// 移動量

	CMechaSelect::MECHATYPE	m_mecha;	// 機体の種類
	float			*m_pAngle;
	float			*m_pAngleV;
	int				m_nTeam;
	bool			m_bDeath;
	int				m_nCntShoot;		// 発射間隔
	bool			m_bShootButton;		// 弾の発射ボタン押下フラグ

	bool			m_bConnect;			// 接続しているかどうか

	// =============================================================
	// UI関係
	// =============================================================
	CUI_NUMBER	*m_pUINum[PLAYER_UI_NUM];			// 数字UIのポインタ
	CMouseCursor2D *m_pCursor;										// カーソルクラスのポインタ変数（ラジオチャット、リスポーン位置選択）
	int m_nDiff;																		// 差分
	int m_nRemBullet;															// 残弾

	// リロード
	CUI_TEXTURE		*m_pUITexReload[RELOAD_TEX];		// リロードで使用するテクスチャ
	CGauge2D			*m_pGauge;											// ゲージ
	int						m_nCntReRoad;
	bool						m_bReload;											// リロード状態かどうか

	// リスポーン
	CUI_TEXTURE		*m_pUITexRespawn[RESPAWN_TEX];		// リスポーンで使用するテクスチャ
	CUI_NUMBER		*m_pUINumRespawn;									// 戦線復帰カウンター
	RESPAWN			m_Respawn;												// リスポーンタイプの取得
	int						m_nRespawnTimer;										// タイマー
	float						m_nDisTime;												// ロゴ表示時間

	// リスポーン位置選択
	CButton2D			*m_pUISelectResBotton[SELECTRESPAWN_BOTTON];	// ラジオチャットボタンUI
	CUI_TEXTURE		*m_pUITexSelectRes[SERECTRESPAWN_TEX];				// ラジオチャットメッセージUITex
	POINT					m_point;																				// リスポーン位置の管理

	// ラジオチャット
	CButton2D			*m_pUIRadioBotton[RADIOCHAT_BOTTON];		// ラジオチャットボタンUI
	CUI_TEXTURE		*m_pUITexRadio;														// ラジオチャットメッセージUITex
	CUI_TEXTURE		*m_pUITexAllyRadio;												// 味方ラジオチャットメッセージUITex
	RADIOCHAT		m_radiochat;															// ラジオチャット管理
	RADIOCHAT		m_allyRadiochat;														// 味方のチャット情報
	bool						m_bChat;																	// チャット開始かどうか
	bool						m_bAllyChat;															// 味方のチャット
	bool						m_bCol;																	// 色の管理
	bool						m_bChatBotton;														// チャットボタン生成中かどうか
	int						m_moveSpeed;														// テクスチャ動くスピード
	int						m_nTexTimer;															// テクスチャ表示タイマー

	// =============================================================
	// AI関係
	// =============================================================
	D3DXVECTOR3 m_waypoint[NODEPOINT_MAX];	// 中間地点
	D3DXVECTOR3 m_collectionPos[ENEMY_PLAYER_MAX][COLLECTIONDATA_MAX];	// 収集したデータ
	D3DXVECTOR3	m_posDest;				// 目標位置
	D3DXVECTOR3	m_rotDest;				// 目標方向
	D3DXVECTOR3 m_totalCollectPos;		// 収集したデータの合計値
	int m_nCollectionTimer;				// 収集する時間
	int m_nBreaktime;					// 休憩時間
	int m_nStartNode;					// 開始ノード番号
	int m_nEndNode;						// 終了ノード番号
	int m_nNewEndNode;					// 新規終了ノード番号
	int m_nCountPoint;					// 目標までの移動回数
	int m_nCountCollect;				// 収集したデータの数
	int m_nPoint;						// 現在の移動回数
	int m_nNearTotalCollectNumber;		// 平均値に一番近いノードのナンバー
	int m_nGoalCount;					// 最終目的地に到達した回数
	int m_nVigilanceCount;					// 警戒時の見る方向のカウント
	bool m_bGoal;						// 目的地に到着したか
	bool m_bPartSwitch;					// アクションとストラテジーの切り替え
	bool m_bCollectSwitch;				// 平均値割り出し方法の切り替え
	static	D3DXVECTOR3	m_searchPos;	// クリック時位置

	int m_nMovePoint[RANDOM_MOVE_POINT] = {5, 26, 49, 73, 92, 115, 143, 171, 176, 202 };	// ランダム移動の登録地点

	NodeState m_NodeData;						// マップ情報へのポインタ
	static CPlayer *m_pPlayer;					// プレイヤークラスへのポインタ
	CAIMecha *m_pAI[AI_MAX];			// 自分のAIクラスへのポインタ
};

#endif