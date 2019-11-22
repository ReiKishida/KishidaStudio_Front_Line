//=============================================================================
//
// プレイヤー処理 [player.h]
// Author : Takuto Ishida
//
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "scene.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define PLAYER_PRIORITY		(4)	// 処理の優先番号
#define COLLECTIONDATA_MAX	(12)// 収集するデータの最大
#define RANDOM_MOVE_POINT	(9)	// ランダム移動の地点数

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CModel;
class CMotion;
class CShadow;
class CLife2D;
class CScene3DBill;
class CScene3D;
class CEnemy;
class CInputMouse;
class CFile;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CPlayer : public CScene
{
public:
	typedef struct
	{// キー要素
		float posX;
		float posY;
		float posZ;
		float rotX;
		float rotY;
		float rotZ;
	}KEY;

	typedef struct
	{// キー情報
		int nFrame;		// フレーム
		KEY *pKey;	// キー要素
	}KEY_INFO;

	typedef struct
	{// ジャンプするタイミング判定
		D3DXVECTOR3		pos;		// 判定用の位置
		float			fAngle;		// 地面の角度
		bool			bJump;		// ジャンプボタン押下タイミング
		CScene3DBill	*pIcon;		// ジャンプタイミングに出すアイコン
	}JUDG_JUMP;

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
	// =============================================================

	CPlayer(int nPriority = PLAYER_PRIORITY, CScene::OBJTYPE objType = CScene::OBJTYPE_PLAYER);
	~CPlayer();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CPlayer* Create(void);

	D3DXVECTOR3 GetPos(void) { return m_pos; };					// 位置の取得
	const void SetPos(D3DXVECTOR3 pos) { m_pos = pos; };		// 位置の設定

	D3DXVECTOR3 GetRot(void) { return m_rot; };					// 角度の取得
	const void SetRot(D3DXVECTOR3 rot) { m_rot = rot; };		// 角度の設定

	D3DXVECTOR3 GetMove(void) { return m_move; };				// 移動量の取得
	const void SetMove(D3DXVECTOR3 move) { m_move = move; };	// 移動量の設定

	bool GetSlipFlag(void) { return m_bSlip; };					// 滑走状態の取得
	void SetSlipFlag(bool bSlip) { m_bSlip = bSlip; };			// 滑走状態の設定

	D3DXMATRIX GetMtxWorld(void) { return m_mtxWorld; };					// ワールドマトリックスの取得
	const void SetMtxWorld(D3DXMATRIX mtxWorld) { m_mtxWorld = mtxWorld; };	// ワールドマトリックスの設定

	bool Collision(D3DXVECTOR3 pos, float fRange);

	void Damage(int nDamage);

	// =============================================================
	// AI関係
	// =============================================================
	void AIUpdate(CInputMouse *pMouse);	// AIの更新
	void AutoMove(void);	// 自動移動
	void NodeSearch(void);	// プレイヤー座標からノード検索
	void RootSearch(void);	// 最短経路検索
	void AddEdge(int first, int second, float weight, Node *node);	// エッジの追加
	void Dijkstra(int nodeMax, int start, int end, Node *node);	// 経路探索
	
	CPlayer *GetPlayer(void) { return m_pPlayer; };
	static void SetSearchPos(D3DXVECTOR3 pos) { m_searchPos = pos; };
	// =============================================================

private:
	void Movement(void);
	void Jump(float fAngleOld);
	void Tilt(void);

	D3DXMATRIX		m_mtxWorld;			// ワールドマトリックス
	D3DXVECTOR3		m_pos;				// 位置
	D3DXVECTOR3		m_rot;				// 回転
	D3DXVECTOR3		m_move;				// 移動量
	float			m_rotDest;			// 目的の角度
	int				m_nCntState;		// ステートのカウンタ
	float			m_fSpeed;			// 速度
	float			m_fSlipSpeed;		// 滑る速度
	float			m_fAngleSlip;		// 滑る方向
	float			m_fJumpSpeed;		// ジャンプ時の速度
	bool			m_bJump;			// ジャンプフラグ
	bool			m_bSlip;			// 滑ってるかどうか
	CModel			**m_pModel;			// モデルクラスのポインタ変数
	int				m_nNumParts;		// パーツ数
	int				m_nCurMotion;		// 現在のモーション
	CMotion			*m_pMotion;			// モーションクラスのポインタ変数
	CShadow			*m_pShadow;			// 影のポインタ変数
	CLife2D			*m_pLife;			// ライフクラスのポインタ変数
	bool			m_bDamage;			// ダメージを受けたかどうか
	bool			m_bMove;			// 動けるかどうか
	int				m_nCntParticle;		// パーティクルを出すまでのカウンタ
	float			m_fAngle;			// 地面と向きの交点の角度
	bool			m_bDash;			// ジャンプ中に空中ダッシュを使ったかどうか
	bool			m_bStageFade;		// ステージ遷移中
	bool			m_bJumpKey;			// ジャンプボタン押下フラグ
	JUDG_JUMP		m_judgJump;			// ジャンプタイミング判定の構造体

	// =============================================================
	// AI関係
	// =============================================================
	D3DXVECTOR3 m_waypoint[NODEPOINT_MAX];	// 中間地点
	D3DXVECTOR3 m_collectionPos[ENEMY_MAX][COLLECTIONDATA_MAX];	// 収集したデータ
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

	CFile *m_pNodeData;							// マップ情報へのポインタ
	static CPlayer *m_pPlayer;					// プレイヤークラスへのポインタ
	static CScene3D *m_pScene3D[NODEPOINT_MAX];	// 3Dポリゴンへのポインタ
	static CEnemy *m_pEnemy[ENEMY_MAX];			// エネミーへのポインタ
};

#endif