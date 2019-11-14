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
#define PLAYER_PRIORITY		(4)		// 処理の優先番号
#define PLAYER_LIFE				(100)
#define TEAM_BLUE				(100)
#define TEAM_RED				(100)
#define PLAYER_BULLET			(100)
#define MAX_UITEX				(2)

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

	CPlayer(int nPriority = PLAYER_PRIORITY, CScene::OBJTYPE objType = CScene::OBJTYPE_PLAYER);
	~CPlayer();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CPlayer* Create(void);

	D3DXVECTOR3 GetPos(void) { return m_pos; };					// 位置の取得
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; };		// 位置の設定

	D3DXVECTOR3 GetRot(void) { return m_rot; };					// 角度の取得
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; };		// 角度の設定

	D3DXVECTOR3 GetMove(void) { return m_move; };				// 移動量の取得
	void SetMove(D3DXVECTOR3 move) { m_move = move; };	// 移動量の設定

	D3DXMATRIX GetMtxWorld(void) { return m_mtxWorld; };					// ワールドマトリックスの取得
	void SetMtxWorld(D3DXMATRIX mtxWorld) { m_mtxWorld = mtxWorld; };	// ワールドマトリックスの設定

	D3DXVECTOR3 GetVtxMax(void);

	void Damage(int nDamage);

	D3DXVECTOR3 CalcScreenToWorld(float fScreenX, float fScreenY);

	void Reload(void);

private:
	void Movement(void);
	void Shoot(void);
	void Angle(void);
	void FieldWalk(void);

	D3DXMATRIX		m_mtxWorld;			// ワールドマトリックス
	D3DXVECTOR3		m_pos;				// 位置
	D3DXVECTOR3		m_rot;				// 回転
	D3DXVECTOR3		m_move;				// 移動量
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
	int				m_nRemBullet;
	int				m_nCntReRoad;
	CUI_TEXTURE *m_pUITex[MAX_UITEX];
	CUI_NUMBER	*m_pUINum;
	CGauge2D *m_pGauge;
};

#endif