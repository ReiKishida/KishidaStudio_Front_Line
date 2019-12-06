//=============================================================================
//
// �e���� [bullet.h]
// Author : Takuto Ishida
//
//=============================================================================
#ifndef _BULLET_H_
#define _BULLET_H_

#include "scene3DBill.h"

//�O���錾
class CPlayer;

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define BULLET_PRIORITY	(4)		// �����̗D��ԍ�

//*****************************************************************************
// �N���X��`
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
	D3DXVECTOR3	m_move;			// �ړ���
	int			m_nDamage;		// �^����_���[�W
	int			m_nLife;		// ����
	float		m_fSpeed;		// ���x
	D3DXVECTOR3	m_posOld;		// �O��̈ʒu
};

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �e�̓����蔻��N���X
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
// �v���C���[�̒e
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
	CPlayer *m_pPlayer;			// �v���C���[�N���X�̃|�C���^�ϐ�
	int		m_nTeam;			// �`�[���̏��
	float	m_fAngle;			// �i�s�����̊p�x
};

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �G�̒e
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class CBulletEnemy : public CBullet
{
public:
	typedef enum
	{// ���
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
	int	m_nCntAngle;	// �ǔ����鎞��
	float m_fAngle;		// �v���C���[�ւ̊p�x
	TYPE m_type;		// ���
};

#endif