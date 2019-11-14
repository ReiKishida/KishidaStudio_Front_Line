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
#define PLAYER_PRIORITY		(4)				// 処理の優先番号
#define PLAYER_LIFE				(100)
#define TEAM_BLUE				(100)
#define TEAM_RED				(100)
#define MAX_UITEX				(10)		// UIテクスチャ枚数
#define PLAYER_UI_NUM		(2)				// 数字UIの枚数
#define PLAYER_BOTTON		(4)				// リスポーン時ボタンの数

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

	void Reload(void);			// リロード処理
	void Respawn(RESPAWN respawn);		// ライフが0になった時の処理
	void SelectRespawn(void);

	CPlayer(int nPriority = PLAYER_PRIORITY, CScene::OBJTYPE objType = CScene::OBJTYPE_PLAYER);
	~CPlayer();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CPlayer* Create(int nPlayerIdx, CMechaSelect::MECHATYPE mecha,D3DXVECTOR3 pos);

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

	float GetAngle(int nIdx) { return m_pAngle[nIdx]; };
	float GetAngleV(int nIdx) { return m_pAngleV[nIdx]; };

	int GetNumShoot(void) { return m_nNumShoot; };
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
	float			m_fSpeed;			// 速度
	int				m_nLife;			// 体力
	int				m_nDamageTime;		// ダメージを受けた時の硬直時間
	CScene3DBill	*m_pReticle;		// レティクル
	int				m_nRemBullet, m_nPlayerLife;
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
	float m_nDisTime;				// ロゴ表示時間
	float *m_pAngle;
	float *m_pAngleV;

	int				m_nPlayerIdx;		// プレイヤー番号
	D3DXVECTOR3		m_posOld;			// 過去の位置
	bool			m_bShoot;			// 弾を発射しているかどうか

	int				m_nCapacity;		// 装弾数
	int				m_nAttack;			// 攻撃力
	int				m_nNumShoot;		// 同時発射数
	int				m_nDispertion;		// ばらつき
	int				m_nReload;			// リロード時間
	CMechaSelect::MECHATYPE	m_mecha;	// 機体の種類

};

#endif