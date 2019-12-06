////=============================================================================
////
//// 敵処理 [enemy.h]
//// Author : Takuto Ishida
////
////=============================================================================
//#ifndef _ENEMY_H_
//#define _ENEMY_H_
//
//#include "scene.h"
//#include "motion.h"
//#include <vector>
//#include <math.h>
//
////*****************************************************************************
//// マクロ定義
////*****************************************************************************
//#define ENEMY_PRIORITY	(4)
//
//#define NODE_MAX	(256)		// ノードの最大数
////*****************************************************************************
//// 前方宣言
////*****************************************************************************
//class CModel;
//class CShadow;
//class CLife3D;
//class CScene3D;
//
////*****************************************************************************
//// クラス定義
////*****************************************************************************
//class CEnemy : public CScene
//{
//public:
//	// =============================================================
//	// ダイクストラ法によるルート探索
//	// =============================================================
//	typedef struct
//	{
//		std::vector<int> to;		// どのノードとつながっているか
//		std::vector<float> cost;	// エッジのコスト
//
//		// ダイクストラ法のために必要な情報
//		bool done;		// 確定ノードかどうか
//		float minCost;	// スタートノードからの最小コスト
//		int from;		// どのノードから来たか
//	}Node;
//
//	typedef struct
//	{
//		int nodeMax;							// ノードの総数
//		int edgeMax;							// エッジの総数
//		int	index[NODE_MAX];					// 自分のノード番号
//		int connectNum[NODE_MAX];				// 接続ノード数
//		int connectIndex[NODE_MAX][NODE_MAX];	// 接続ノード番号
//		D3DXVECTOR3 pos[NODE_MAX];				// 各ノードの位置
//	}NodeState;
//	// =============================================================
//
//	CEnemy(int nPriority = ENEMY_PRIORITY, CScene::OBJTYPE objType = CScene::OBJTYPE_ENEMY);
//	~CEnemy();
//	HRESULT Init(void);
//	void Uninit(void);
//	void Update(void);
//	void Draw(void);
//
//	static CEnemy* Create(D3DXVECTOR3 pos, int nType);
//
//	static HRESULT Load(void);				// テクスチャを読み込む（managerにいれる）
//	static void Unload(void);				// 読み込んだテクスチャを解放
//
//	int GetType(void) { return m_nType; };
//
//	float GetAttackRange(void) { return m_fAttackRange; };
//	void SetAttackRange(float fAttackRange) { m_fAttackRange = fAttackRange; };
//
//	D3DXVECTOR3 GetPos(void) { return m_pos; };
//	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; };
//
//	void Damage(int nDamage, int nMotion);		// 攻撃を受けたときの処理
//	bool Collision(D3DXVECTOR3 pos, float fRange);	// 当たり判定
//
//	// =============================================================
//	// ダイクストラ法によるルート探索
//	// =============================================================
//	static void SetSearchPos(D3DXVECTOR3 pos) { m_searchPos = pos; };
//	static bool GetPartSwitch(void) { return m_bPartSwitch; };
//
//	void AIUpdate(void);
//	void NodeSearch(bool node);
//	void RootSearch(void);
//	void RallyRootSearch(void);
//	void AutoMove(void);
//	void AddEdge(int first, int second, float weight, Node *node);
//	void Dijkstra(int nodeMax, int start, int end, Node *node);
//	void FileLoad(char* pFileName);
//	// =============================================================
//
//private:
//	typedef struct
//	{// ファイル読み込み構造体
//		int nIdx;			// モデル番号
//		int nParent;		// 親番号
//		D3DXVECTOR3 pos;	// 位置
//		D3DXVECTOR3 rot;	// 向き
//	}DATA;
//
//	void GetModelData(CModel **&pModel);
//
//	static int		*m_pNumParts;				// パーツ数
//	static char		***m_pModelName;			// モデル名
//	static DATA		**m_pFileData;				// ファイル読み取り
//	static int		m_nNumType;					// 種類の数
//
//	D3DXMATRIX		m_mtxWorld;					// ワールドマトリックス
//	D3DXVECTOR3		m_pos;						// 位置
//	D3DXVECTOR3		m_rot;						// 回転
//	float			m_fRotDest;					// 目的の角度
//	D3DXVECTOR3		m_move;						// 移動量
//	int				m_nCntDamage[ATTACK_MOTION];	// ダメージのカウンタ
//	int				m_nCntBullet;				// 弾を撃つまでのカウンタ
//	int				m_nType;					// 敵の種類
//	CShadow			*m_pShadow;					// 影のポインタ変数
//	CModel			**m_pModel;					// モデルクラスのポインタ変数
//	CMotion			*m_pMotion;					// モーションクラスのポインタ変数
//	float			m_fWalkRange;				// 索敵範囲
//	float			m_fAttackRange;				// 攻撃をする範囲
//	int				m_nIntervalAttack;			// 攻撃間隔
//	bool			m_bAttack;					// 攻撃フラグ
//	CLife3D			*m_pLife;					// 3D体力クラスのポインタ変数
//	bool			m_bDeath;					// 死亡フラグ
//	float			m_fAlpha;					// 透明度
//	bool			m_bDamage[ATTACK_MOTION];	// ダメージ状態
//
//	// =============================================================
//	// ダイクストラ法によるルート探索
//	// =============================================================
//	NodeState m_RootData;					// マップ情報
//
//	// パート関係
//	static bool m_bPartSwitch;				// アクションとストラテジーの切り替え
//
//	// ノード関係
//	Node m_node[NODE_MAX][NODE_MAX];
//	int m_nStartNode;						// 開始ノード番号
//	int m_nEndNode;							// 終了ノード番号
//	int m_nNodeOld;							// 前回のノード番号
//
//	// ラリー関係
//	int m_nRallyEndNode[NODE_MAX];			// ラリー時の終了ノード番号[num]
//	bool m_bRally;							// ラリー状態か
//	int m_nRallyCount;						// クリック数
//	int m_nRallyCountOld;					// 前回のクリック数
//
//	// ルート検索関係
//	static	D3DXVECTOR3	m_searchPos;		// クリック時位置
//
//	// 自動移動関係
//	D3DXVECTOR3 m_waypoint[NODE_MAX];		// 中間地点
//	D3DXVECTOR3	m_posDest;					// 目標位置
//	int m_nBreaktime;						// 休憩時間
//	int m_nCountPoint;						// 目標までの移動回数
//	int m_nPoint;							// 現在の移動回数
//	bool m_bGoal;							// 目的地に到着したか
//
//	// ポインタ関係
//	static CScene3D *m_pScene3D[NODE_MAX];	// 3Dポリゴンへのポインタ
//	// =============================================================
//
//};
//
//#endif