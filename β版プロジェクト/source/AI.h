//=============================================================================
//
// AI処理 [AI.h]
// Author : Takuto Ishida
//
//=============================================================================
#ifndef _AI_H_
#define _AI_H_

#include "main.h"
#include "scene.h"
#include "game.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define AI_PRIORITY		(4)		// 処理の優先番号
#define RALLYPOINT_MAX	(4)		// ラリーポイントの最大設置数

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CModel;
class CMotion;
class CPlayer;
class CButtonManagerStrategy;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CAIMecha : public CScene
{
public:
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
		int nodeMax;								// ノードの総数
		int	index[NODEPOINT_MAX];					// 自分のノード番号
		int connectNum[NODEPOINT_MAX];				// 接続ノード数
		int connectIndex[NODEPOINT_MAX][CONNECT_MAX];	// 接続ノード番号
		D3DXVECTOR3 pos[NODEPOINT_MAX];				// 各ノードの位置
	}NodeState;

	typedef enum
	{
		AI_ACTION_NONE = 0,		// なし
		AI_ACTION_MOVE,			// 移動
		AI_ACTION_WAIT,			// 待機
		AI_ACTION_DISPATCH,		// 派遣
		AI_ACTION_FOLLOW,		// 追従
		AI_ACTION_FOCUS_GOAL,	// 目標重視
		AI_ACTION_RALLY,		// ラリーポイント
		AI_ACTION_ROUND_TRIP,	// 往復
		AI_ACTION_FOLLOW_LONG,	// 遠距離追従
		AI_ACTION_FOLLOW_SHORT,	// 短距離追従
		AI_ACTION_ATTACK,		// 攻撃
		AI_ACTION_BATTLE,		// 応戦
		AI_ACTION_AVOIDANCE,	// 回避
		AI_ACTION_PURSUIT,		// 追跡
		AI_ACTION_NOT_PURSUIT,	// 非追跡
		AI_ACTION_DISTURBANCE,	// 攪乱
		AI_ACTION_WITHDRAWAL,	// 撤退
		AI_ACTION_MAX,			// 最大値
	}AI_ACTION;
	// =============================================================

	typedef enum
	{// 状態
		STATE_APPEAR = 0,	// 出現
		STATE_NORMAL,		// 通常
		STATE_DAMAGE,		// ダメージ
		STATE_DEATH,		// 死亡
		STATE_MAX
	}STATE;

	typedef enum
	{// 機体の種類
		MECHATYPE_WALKER = 0,
		MECHATYPE_DRONE,
		MECHATYPE_MAX
	}MECHATYPE;

	CAIMecha(int nPriority = AI_PRIORITY, CScene::OBJTYPE objType = CScene::OBJTYPE_AI);
	~CAIMecha();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CAIMecha* Create(CPlayer *pPlayer, MECHATYPE mecha, D3DXVECTOR3 pos);

	D3DXVECTOR3 GetPos(void) { return m_pos; };			// 位置の取得
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; };		// 位置の設定

	D3DXVECTOR3 GetRot(void) { return m_rot; };			// 角度の取得
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; };		// 角度の設定

	D3DXVECTOR3 GetMove(void) { return m_move; };		// 移動量の取得
	void SetMove(D3DXVECTOR3 move) { m_move = move; };	// 移動量の設定

	D3DXMATRIX GetMtxWorld(void) { return m_mtxWorld; };				// ワールドマトリックスの取得
	void SetMtxWorld(D3DXMATRIX mtxWorld) { m_mtxWorld = mtxWorld; };	// ワールドマトリックスの設定

	D3DXVECTOR3 GetVtxMax(void) { return m_vtxMax; }
	D3DXVECTOR3 GetVtxMin(void) { return m_vtxMin; }

	void Damage(int nDamage);

	CModel *GetModel(int nIdx) { return m_pModel[nIdx]; };

	MECHATYPE GetMechaType(void) { return m_mechaType; }

	int GetTeam(void) { return m_nTeam; };
	void SetDeath(bool bDeath) { m_bDeath = bDeath; };
	bool GetDeath(void) { return m_bDeath; };
	// =============================================================
	// ダイクストラ法によるルート探索
	// =============================================================
	void AIUpdate(void);			// AIの更新
	void Follow(void);				// 追従処理
	void NodeSearch(bool node);		// マウス座標からノード検索
	void AutoMove(void);			// 自動移動
	void RootSearch(void);			// 最短経路検索
	void RallyRootSearch(void);		// ラリーポイントでの最短経路検索
	void PatrolRootSearch(void);	// 往復用の最短経路検索
	void Cancel(void);				// 中断
	void AddEdge(int first, int second, float weight, Node *node);	// エッジの追加
	void Dijkstra(int nodeMax, int start, int end, Node *node);		// 経路探索
	void FileLoad(char* pFileName);	// マップデータファイルの読み込み
									// =============================================================

private:
	D3DXMATRIX		m_mtxWorld;			// ワールドマトリックス
	D3DXVECTOR3		m_pos;				// 位置
	D3DXVECTOR3		m_rot;				// 回転
	D3DXVECTOR3		m_move;				// 移動量
	D3DXVECTOR3		m_vtxMax;			// 頂点座標の最大値
	D3DXVECTOR3		m_vtxMin;			// 頂点座標の最小値
	CModel			**m_pModel;			// モデルクラスのポインタ変数
	CMotion			*m_pMotion;			// モーションクラスのポインタ変数
	int				m_nCntState;		// ステートのカウンタ
	int				m_nNumParts;		// パーツ数
	int				m_nCurMotion;		// 現在のモーション
	STATE			m_state;			// 状態
	MECHATYPE		m_mechaType;		// 機体の種類
	CPlayer			*m_pPlayer;			// プレイヤーのポインタ変数
	float			m_fSpeed;			// 移動量
	int				m_nLife;			// 現在の耐久力
	int				m_nLifeMax;			// 最大耐久力
	int				m_nCapacity;		// 装弾数
	int				m_nAttack;			// 攻撃力
	int				m_nNumShoot;		// 発射数
	int				m_nTeam;			// チーム

										// =============================================================
										// ダイクストラ法によるルート探索
										// =============================================================
	NodeState		m_NodeData;								// マップ情報

															// パート関係
	CGame::PART		m_bPartSwitch;							// パート情報
	CGame::PART		m_bPartSwitchOld;						// 前回のパート情報

															// ノード関係
	int				m_nStartNode;							// 開始ノード番号
	int				m_nEndNode;								// 終了ノード番号
	int				m_nNodeOld;								// 前回のノード番号

															// ラリー関係
	Node			m_node[NODEPOINT_MAX][NODEPOINT_MAX];	// ラリー時のノードの情報
	int				m_nRallyEndNode[NODEPOINT_MAX];			// ラリー時の終了ノード番号
	int				m_nRallyCount;							// クリック数
	int				m_nRallyCountOld;						// 前回のクリック数

															// パトロール関係
	bool			m_bPatrol;								// 往復移動状態か
	int				m_nPatrolStartNode;						// パトロール時の開始ノード番号
	D3DXVECTOR3		m_patrolWaypoint[NODEPOINT_MAX];		// パトロール時の中間地点
	int				m_nCountPatrolPoint;					// パトロール時の目標までの移動回数

															// 自動移動関係
	D3DXVECTOR3		m_waypoint[NODEPOINT_MAX];				// 中間地点
	D3DXVECTOR3		m_posDest;								// 目標位置
	D3DXVECTOR3		m_rotDest;								// 目標位置
	int				m_nBreaktime;							// 休憩時間
	int				m_nCountPoint;							// 目標までの移動回数
	int				m_nPoint;								// 現在の移動回数
	bool			m_bGoal;								// 目的地に到着したか

															// ロジックツリー関係の情報
	int				m_LogicTree[4];							// AIへの指示の情報
	AI_ACTION		m_AIAction[4];							// AIの行動

	bool			m_bDeath;								//死亡しているかどうか
};

#endif