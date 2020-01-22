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
#define RANDOM_MOVE_POINT	(11)	// ランダム移動の地点数
#define ENEMY_PLAYER_MAX	(2)		// 敵プレイヤーの数
#define AI_MAX				(2)		// 一人当たりのAIの数
#define PLAYER_MAX			(2)		// プレイヤーの数

// リロード
#define RELOAD_TEX					(2)			// リロードで使用するテクスチャ数

// リスポーン
#define RESPAWN_TEX					(3)			// リスポーンで使用するテクスチャ数

// リスポーン位置選択
#define SELECTRESPAWN_BOTTON		(4)			// リスポーン位置選択ボタン
#define SERECTRESPAWN_TEX			(4)			// リスポーン位置選択中のテクスチャ数

// ラジオチャット処理
#define RADIOCHAT_BOTTON			(8)			// ラジオチャットボタンの数
#define RADIOCHAT_MESSAGE			(2)			// ラジオチャットボタンの数
#define RADIOCHAT_BOTTON_PATTERN	(8)			// ラジオチャット画像のパターン数
#define RADIOCHAT_BOTTON_WIDTH		(200.0f)	// ボタンの幅
#define RADIOCHAT_BOTTON_HEIGHT		(200.0f)	// ボタンの高さ
#define RADIOCHAT_MESS_WIDTH		(400.0f)	// メッセージの幅
#define RADIOCHAT_MESS_HEIGHT		(80.0f)		// メッセージの高さ

// オプション
#define OPTION_SELECT							(5)			// カメラ速度設定の数

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
class CSearch;
class CNodeDataFiler;
class CPin;
class CBulletPin;
class CScene3D;

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
		RESPAWN_START,			// 戦闘開始
		RESPAWN_DEATH,			// 行動不能
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
		RADIOCHAT_OK = 0,		// 了解
		RADIOCHAT_OPPSITE,		// 反対
		RADIOCHAT_THANKS,		// 感謝
		RADIOCHAT_APOLOGY,		// 謝罪
		RADIOCHAT_NICESHOOT,	// ナイスショット
		RADIOCHAT_FOLLOW,		// ついてこい！
		RADIOCHAT_RECESSION,	// 後退しろ！
		RADIOCHAT_SUPPORT,		// 援護しろ！
		RADIOCHAT_MAX
	} RADIOCHAT;

	typedef struct
	{// ルート探索用
		std::vector<int> to;		// どのノードとつながっているか
		std::vector<float> cost;	// エッジのコスト

		// ダイクストラ法のために必要な情報
		bool done;		// 確定ノードかどうか
		float minCost;	// スタートノードからの最小コスト
		int from;		// どのノードから来たか
	}Node;

	CPlayer(int nPriority = PLAYER_PRIORITY, CScene::OBJTYPE objType = CScene::OBJTYPE_PLAYER);
	~CPlayer();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CPlayer* Create(int nPlayerIdx, CMechaSelect::MECHATYPE mecha, D3DXVECTOR3 pos, bool bConnect);

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

	void Damage(int nDamage, CScene *pScene);

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

	int GetBulletLife(void) { return m_nBulletLife; };

	void SetTeam(int nTeam) { m_nTeam = nTeam; };
	int GetTeam(void) { return m_nTeam; };
	bool &GetDeath(void) { return m_bDeath; };

	void SetLife(int nLife) { m_nLife = nLife; };
	int GetLife(void) { return m_nLife; };
	int GetLifeMax(void) { return m_nLifeMax; }

	bool GetWince(void);

	CPlayer *GetPlayer(void) { return m_pPlayer; };
	static void SetSearchPos(D3DXVECTOR3 pos) { m_searchPos = pos; };
	CAIMecha *GetMyAI(int nIdx) { return m_pAI[nIdx]; };
	bool &GetPlayerConnect(void) { return m_bConnect; };

	bool GetReload(void) { return m_bReload; };

	CMechaSelect::MECHATYPE	GetMechaType(void) { return m_mecha; };

	//==================
	//   ピン関係
	//==================
	// 自分のピン情報
	bool &GetPinUse(void) { return m_bPin; };
	int &GetPinLife(void) { return m_nPinLife; };
	D3DXVECTOR3 &GetPinPos(void) { return m_PinPos; };
	// AIのピン情報
	bool &GetAIPinUse(int nValue) { return m_bAIPin[nValue]; };
	D3DXVECTOR3 &GetAIPinPos(int nValue) { return m_AIPinPos[nValue]; };
	// 味方のピン情報
	bool &GetAllyPinUse(void) { return m_bAllyPin; };
	D3DXVECTOR3 &GetAllyPinPos(void) { return m_AllyPinPos; };
	// 味方AIのピン情報
	bool &GetAllyAIPinUse(int nValue) { return m_bAllyAIPin[nValue]; };
	D3DXVECTOR3 &GetAllyAIPinPos(int nValue) { return m_AllyAIPinPos[nValue]; };

	float GetBulletSpeed(void) { return m_fBulletSpeed; };

	// ラジオチャット
	RADIOCHAT GetRadioChat(void) { return m_radiochat; }									// ラジオチャット情報の取得
	void SetRadioChat(RADIOCHAT radiochat) { m_radiochat = radiochat; }						// ラジオチャットの設定
	void SetChat(bool bChat) { m_bChat = bChat; };											//チャット情報の設置処理
	bool GetChat(void) { return m_bChat; }													// チャット情報の取得
	void SetAllyChat(bool bAllyChat) { m_bAllyChat = bAllyChat; }							// 味方のチャットが使用しているかどうかの設定
	bool GetAllyChat(void) { return m_bAllyChat; };
	void SetAllyRadioChat(RADIOCHAT allyRadioChat) { m_allyRadiochat = allyRadioChat; }		// 味方のチャット情報の設定
	bool GetChatBotton(void) { return m_bChatBotton; }

	// オプション
	bool GetOption(void) { return m_bOption; }																// オプション状態の取得
	void SetOption(bool bOption) { m_bOption = bOption; }											// オプション状態の設定
	int GetSelectOption(void) { return m_nSelectOption; }												// 選択されたカメラ速度取得
	void SetSelectOption(int nSelectOption) { m_nSelectOption = nSelectOption; }		// 選択されたカメラ速度設定

	int GetKillPlayerIdx(void) { return m_nKillPlayerIdx; };
	int GetNumParts(void) { return m_nNumParts; };

	int GetRemainBullet(void) { return m_nRemBullet; };

	void SetDestUpper(D3DXVECTOR3 destUpper) { m_fRotDestUpper = destUpper; };
	D3DXVECTOR3 GetDestUpper(void) { return m_fRotDestUpper; };

	CScene3D *GetPlayerIcon(void) { return m_pPlayerIcon; };

	void ReleasePlayerUI(void);
private:
	void Movement(void);
	void Shoot(void);
	void Angle(void);
	void SetParticle(void);
	void Reload(bool bReload);			// リロード処理
	void Respawn(RESPAWN respawn);		// ライフが0になった時の処理
	void SelectRespawn(void);			// リスポーン位置選択処理
	void ChatBotton(void);				// ラジオチャットボタンの生成
	void ChatMess(bool bChat);			// ボタンが押されて、メッセージ表示
	void CreateRespawnPosIcon(void);
	void AllyChatMess(void);
	void Option(bool bOption);						// オプション設定
	void UninitOption(void);							// オプションの破棄
	void CreateRadioChatButton(void);		// チャットボタンの生成
	void UninitRadioChatButton(void);			// チャットボタンの破棄
	void PinUpdateSingle(void);			// シングル時のピン関係の更新
	void PinUpdateMulti(void);			// マルチ時のピン関係の更新

	// 戦闘系AIの処理
	void LoadBattleFile(void);	// 戦闘用のテキストの読み込み
	bool Distance(void);		// 範囲探索
	void Battle(void);			// 戦闘
	void BattleMovent(void);	// 戦闘時の移動
	void CpuShoot(void);		// 弾発射

	// 移動系AIの処理
	void AIUpdate(void);	// AIの更新
	void AutoMove(void);	// 自動移動
	void NodeSearch(void);	// プレイヤー座標からノード検索
	void RootSearch(void);	// 最短経路検索
	void AddEdge(int first, int second, float weight, Node *node);	// エッジの追加
	void Dijkstra(int nodeMax, int start, int end, Node *node);	// 経路探索

	// =============================================================
	// 基本情報の変数
	// =============================================================
	D3DXMATRIX		m_mtxWorld;			// ワールドマトリックス
	D3DXVECTOR3		m_pos;				// 位置
	D3DXVECTOR3		m_rot;				// 回転
	D3DXVECTOR3		m_move;				// 移動量
	D3DXVECTOR3		m_vtxMax;			// 頂点座標の最大値
	D3DXVECTOR3		m_vtxMin;			// 頂点座標の最小値
	CModel			**m_pModel;			// モデルクラスのポインタ変数
	CMotion			*m_pUpperMotion;	// 上半身モーションクラスのポインタ変数
	CMotion			*m_pLowerMotion;	// 下半身モーションクラスのポインタ変数
	//CShadow			*m_pShadow;			// 影のポインタ変数
	float			m_fRotDest;			// 目的の角度
	int				m_nCntState;		// ステートのカウンタ
	int				m_nNumParts;		// パーツ数
	STATE			m_state;			// 状態
	float			m_fCameraAngle;		// カメラの向き
	int				m_nDamageTime;		// ダメージを受けた時の硬直時間
	CScene3DBill	*m_pReticle;		// レティクル
	int m_nTimer;						// タイマー

	int				m_nPlayerIdx;		// プレイヤー番号
	D3DXVECTOR3		m_posOld;			// 過去の位置
	bool			m_bShoot;			// 弾を発射しているかどうか
	float			m_fBulletSpeed;		// 弾速
	int				m_nBulletLife;		// 弾の寿命
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
	bool			m_bWince;			// ひるんでいるかどうか

	// ピン
	CPin			*m_pPin;					// ピンのポインタ
	CBulletPin		*m_pPinBullet;				// ピン立て判定用の弾のポインタ
	bool			m_bPin;						// ピンを立てているか
	int				m_nPinLife;					// ピンの表示時間
	D3DXVECTOR3		m_PinPos;					// ピンの位置
												// AIのピン
	CPin			*m_pAIPin[AI_MAX];			// AIのピンのポインタ
	bool			m_bAIPin[AI_MAX];			// AIがピンを立てているか
	D3DXVECTOR3		m_AIPinPos[AI_MAX];			// AIのピンの位置
												// 味方のピン
	CPin			*m_pAllyPin;				// 味方のピンのポインタ
	bool			m_bAllyPin;					// 味方がピンを立てているか
	D3DXVECTOR3		m_AllyPinPos;				// 味方のピンの位置
												// 味方AIのピン
	CPin			*m_pAllyAIPin[AI_MAX];		// 味方AIのピンのポインタ
	bool			m_bAllyAIPin[AI_MAX];		// 味方AIがピンを立てているか
	D3DXVECTOR3		m_AllyAIPinPos[AI_MAX];		// 味方AIのピンの位置

	// 味方の位置ピン
	CPin			*m_pAllyPosPin;				// 味方の位置ピンのポインタ
	D3DXVECTOR3		m_AllyPosPinPos;			// 味方の位置

	int				m_nCntCPURespawn;			// CPUプレイヤーのリスポーンカウンター
	// =============================================================
	// UI関係
	// =============================================================
	CMouseCursor2D	*m_pCursor;							// カーソルクラスのポインタ変数（ラジオチャット、リスポーン位置選択）
	int				m_nDiff;							// 差分
	int				m_nRemBullet;						// 残弾

	// リロード
	CUI_TEXTURE		*m_pUITexReload[RELOAD_TEX];		// リロードで使用するテクスチャ
	CGauge2D		*m_pGauge;							// ゲージ
	int				m_nCntReRoad;
	bool			m_bReload;							// リロード状態かどうか

	// リスポーン
	CUI_TEXTURE		*m_pUITexRespawn[RESPAWN_TEX];				// リスポーンで使用するテクスチャ
	CUI_TEXTURE		*m_pUIRespawnPosIcon[SELECTRESPAWN_BOTTON];	// リスポーン位置のアイコン
	CUI_NUMBER		*m_pUINumRespawn;							// 戦線復帰カウンター
	RESPAWN			m_Respawn;									// リスポーンタイプの取得
	int				m_nRespawnTimer;							// タイマー
	float			m_nDisTime;									// ロゴ表示時間

	// リスポーン位置選択
	CButton2D		*m_pUISelectResBotton[SELECTRESPAWN_BOTTON];// ラジオチャットボタンUI
	CUI_TEXTURE		*m_pUITexSelectRes[SERECTRESPAWN_TEX];		// ラジオチャットメッセージUITex
	POINT			m_point;									// リスポーン位置の管理

	// ラジオチャット
	CButton2D		*m_pUIRadioBotton[RADIOCHAT_BOTTON];		// ラジオチャットボタンUI
	CUI_TEXTURE		*m_pUITexRadio;								// ラジオチャットメッセージUITex
	CUI_TEXTURE		*m_pUITexAllyRadio;							// 味方ラジオチャットメッセージUITex
	RADIOCHAT		m_radiochat;								// ラジオチャット管理
	RADIOCHAT		m_allyRadiochat;							// 味方のチャット情報
	bool			m_bChat;									// チャット開始かどうか
	bool			m_bAllyChat;								// 味方のチャット
	bool			m_bCol;										// 色の管理
	bool			m_bAllyCol;									// 味方の色の管理
	bool			m_bChatBotton;								// チャットボタン生成中かどうか
	int				m_moveSpeed;								// テクスチャ動くスピード
	int				m_nTexTimer;								// テクスチャ表示タイマー
	int				m_nAllyTimer;
	int				m_nRadioChat;														// ラジオチャット切り替え

	// オプション
	CButton2D			*m_pUIButtonOption;												// オプションUIポインタ
	bool						m_bOption;																// オプション状態かどうか
	CUI_TEXTURE		*m_pUITexOption;													// オプション設定で使用するUITex
	CButton2D			*m_pUIButtonBack;													// 戻るボタンUIポインタ
	CButton2D			*m_pUIButtonSelect[OPTION_SELECT];				// カメラの速度選択
	int						m_nSelectOption;													//	カメラ速度の項目

	int m_nKillPlayerIdx;					//キルプレイヤーの番号

	// =============================================================
	// 移動系AIの変数
	// =============================================================
	D3DXVECTOR3 m_waypoint[NODEPOINT_MAX];	// 中間地点
	D3DXVECTOR3 m_collectionPos[COLLECTIONDATA_MAX];	// 収集したデータ
	D3DXVECTOR3	m_posDest;				// 目標位置
	D3DXVECTOR3	m_rotDest;				// 目標方向
	D3DXVECTOR3	m_rotDestOld;			// 前回の目標方向
	D3DXVECTOR3 m_totalCollectPos;		// 収集したデータの合計値
	int m_nBreaktime;					// 休憩時間
	int m_nStartNode;					// 開始ノード番号
	int m_nEndNode;						// 終了ノード番号
	int m_nNewEndNode;					// 新規終了ノード番号
	int m_nCountPoint;					// 目標までの移動回数
	int m_nCountCollect;				// 収集したデータの数
	int m_nPoint;						// 現在の移動回数
	int m_nNearTotalCollectNumber;		// 平均値に一番近いノードのナンバー
	int m_nGoalCount;					// 最終目的地に到達した回数
	int m_nVigilanceCount;				// 警戒時の見る方向のカウント
	bool m_bGoal;						// 目的地に到着したか
	bool m_bPartSwitch;					// アクションとストラテジーの切り替え
	bool m_bCollectSwitch;				// 平均値割り出し方法の切り替え
	static	D3DXVECTOR3	m_searchPos;	// クリック時位置
	int m_nMovePoint[RANDOM_MOVE_POINT] = { 0, 24, 47, 72, 144, 183, 188, 229, 279, 287, 316 }; // ランダム移動の登録地点

	CNodeDataFiler *m_pNodeData;		// マップ情報へのポインタ
	static CPlayer *m_pPlayer;			// プレイヤークラスへのポインタ
	CAIMecha *m_pAI[AI_MAX];			// 自分のAIクラスへのポインタ

	// =============================================================
	//AI戦闘系の変数
	// =============================================================
	//戦闘に必要な変数
	CSearch **m_pSearch;			// 探索のポインタクラス

	//認識系
	CPlayer *m_pFindPlayer;			// 発見したプレイヤーのポインタ
	float m_fSearchLength;			// 認識距離
	float m_fSearchAngle;			// 認識角度
	D3DXVECTOR3 m_SearchVec_0;		// 認識用ベクトル１
	D3DXVECTOR3 m_SearchVec_1;		// 認識用ベクトル２
	D3DXVECTOR3 m_SearchVec_2;		// 認識用ベクトル２
	bool m_bFind;					// 発見の状態
	bool m_bFindOld;				// 前回の発見状態

	//攻撃系
	float	m_fRange;				// 範囲

	//移動系
	D3DXVECTOR3 m_fRotDestUpper;	// 上半身

	CScene3D *m_pPlayerIcon;
};

#endif