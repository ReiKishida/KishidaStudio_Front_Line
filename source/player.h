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
#define TEAM_BLUE				(100)
#define TEAM_RED				(100)
#define MAX_UITEX				(10)		// UIテクスチャ枚数
#define PLAYER_UI_NUM		(2)		// 数字UIの枚数
#define PLAYER_BOTTON		(4)		// リスポーン時ボタンの数
#define COLLECTIONDATA_MAX	(12)// 収集するデータの最大
#define RANDOM_MOVE_POINT	(9)	// ランダム移動の地点数
#define ENEMY_PLAYER_MAX	(2)	// 敵プレイヤーの数
#define NODE_MAX	(256)		// ノードの最大数

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
		int edgeMax;							// エッジの総数
		int	index[NODE_MAX];					// 自分のノード番号
		int connectNum[NODE_MAX];				// 接続ノード数
		int connectIndex[NODE_MAX][NODE_MAX];	// 接続ノード番号
		D3DXVECTOR3 pos[NODE_MAX];				// 各ノードの位置
	}NodeState;

	// =============================================================

	void Reload(void);			// リロード処理
	void Respawn(RESPAWN respawn);		// ライフが0になった時の処理
	void SelectRespawn(void);

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

private:
	void Movement(void);
	void Shoot(void);
	void Angle(void);
	void FieldWalk(void);

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
	int				m_nCurMotion;		// 現在のモーション
	STATE			m_state;			// 状態
	float			m_fCameraAngle;		// カメラの向き
	int				m_nDamageTime;		// ダメージを受けた時の硬直時間
	CScene3DBill	*m_pReticle;		// レティクル
	int				m_nRemBullet;
	int				m_nCntReRoad;
	CUI_TEXTURE *m_pUITex[MAX_UITEX];
	CUI_NUMBER	*m_pUINum[PLAYER_UI_NUM];
	CGauge2D *m_pGauge;
	int m_nDiff;
	int m_nCntAnim, m_nPatternAnim;		// アニメーション
	RESPAWN m_Respawn;		// リスポーンタイプの取得
	CButton2D *m_apButtonUI[PLAYER_BOTTON];	// ボタンクラスのポインタ変数
	CMouseCursor2D *m_pCursor;								// カーソルクラスのポインタ変数
	int m_nTimer;					// タイマー
	float m_nDisTime;			// ロゴ表示時間

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
	// AI関係
	// =============================================================
	D3DXVECTOR3 m_waypoint[NODEPOINT_MAX];	// 中間地点
	D3DXVECTOR3 m_collectionPos[ENEMY_PLAYER_MAX][COLLECTIONDATA_MAX];	// 収集したデータ
	D3DXVECTOR3	m_posDest;				// 目標位置
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
	bool m_bGoal;						// 目的地に到着したか
	bool m_bPartSwitch;					// アクションとストラテジーの切り替え
	bool m_bCollectSwitch;				// 平均値割り出し方法の切り替え
	static	D3DXVECTOR3	m_searchPos;	// クリック時位置

	int m_nMovePoint[RANDOM_MOVE_POINT] = { 26, 49, 73, 92, 115, 143, 171, 176, 202 };	// ランダム移動の登録地点

	NodeState m_NodeData;						// マップ情報へのポインタ
	static CPlayer *m_pPlayer;					// プレイヤークラスへのポインタ
	//static CEnemy *m_pEnemy[ENEMY_PLAYER_MAX];			// エネミーへのポインタ
};

#endif