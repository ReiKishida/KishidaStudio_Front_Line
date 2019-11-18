//=============================================================================
//
// AI処理 [AI.h]
// Author : Takuto Ishida
//
//=============================================================================
#ifndef _AI_H_
#define _AI_H_

#include "scene.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define AI_PRIORITY		(4)		// 処理の優先番号

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CModel;
class CMotion;
class CPlayer;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CAIMecha : public CScene
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

	CModel *GetModel(int nIdx) { return m_pModel[nIdx]; };

	MECHATYPE GetMechaType(void) { return m_mechaType; }

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
};

#endif