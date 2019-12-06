//=============================================================================
//
// 弾処理 [bullet.h]
// Author : Takuto Ishida
//
//=============================================================================
#ifndef _BULLET_H_
#define _BULLET_H_

#include "scene3DBill.h"

//前方宣言
class CPlayer;

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define BULLET_PRIORITY	(4)		// 処理の優先番号

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CBullet : public CScene3DBill
{
public:
	CBullet(int nPriority = BULLET_PRIORITY, CScene::OBJTYPE objType = CScene::OBJTYPE_BULLET);
	virtual ~CBullet();
	virtual HRESULT Init(D3DXVECTOR3 pos);
	virtual void Uninit(void) = 0;
	virtual void Update(void);
	virtual void Draw(void) = 0;

	int GetDamage(void) { return m_nDamage; };
	void SetDamage(int nDamage) { m_nDamage = nDamage; }

	int GetLife(void) { return m_nLife; };
	void SetLife(int nLife) { m_nLife = nLife; }

	D3DXVECTOR3 GetMove(void) { return m_move; };
	void SetMove(D3DXVECTOR3 move) { m_move = move; }

	D3DXVECTOR3 GetPosOld(void) { return m_posOld; }
	void SetPosOld(D3DXVECTOR3 posOld) { m_posOld = posOld; }

	virtual bool BulletCollision(void) = 0;

protected:

private:
	D3DXVECTOR3	m_move;			// 移動量
	int			m_nDamage;		// 与えるダメージ
	int			m_nLife;		// 寿命
	float		m_fSpeed;		// 速度
	D3DXVECTOR3	m_posOld;		// 前回の位置
};

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 弾の当たり判定クラス
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class CBulletCollision : public CScene
{
public:
	CBulletCollision(int nPriority = 6, CScene::OBJTYPE objType = CScene::OBJTYPE_BULLET_COLLISION);
	~CBulletCollision();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CBulletCollision *Create(void);

private:
};

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// プレイヤーの弾
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class CBulletPlayer : public CBullet
{
public:
	CBulletPlayer();
	~CBulletPlayer();
	HRESULT Init(D3DXVECTOR3 pos, float fAngle, float fAngleVertical, int nDamage);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CBulletPlayer *Create(D3DXVECTOR3 pos, float fAngle, float fAngleVertical, int nDamage,int nTeam);

	bool BulletCollision(void);

private:
	CPlayer *m_pPlayer;			// プレイヤークラスのポインタ変数
	int		m_nTeam;			// チームの情報
	float	m_fAngle;			// 進行方向の角度
};

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 敵の弾
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class CBulletEnemy : public CBullet
{
public:
	typedef enum
	{// 種類
		TYPE_NORMAL = 0,
		TYPE_MAX
	}TYPE;

	CBulletEnemy();
	~CBulletEnemy();
	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CBulletEnemy *Create(D3DXVECTOR3 pos, TYPE type, float fDestAngle);

	bool BulletCollision(void);

private:
	int	m_nCntAngle;	// 追尾する時間
	float m_fAngle;		// プレイヤーへの角度
	TYPE m_type;		// 種類
};

#endif