//=============================================================================
//
// �����蔻��̐ݒu���� [collisionSet.h]
// Author : Ishida Takuto
//
//=============================================================================
#ifndef _COLLISIONSET_H_
#define _COLLISIONSET_H_

#include "scene.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define COLLISION_PRIORITY	(2)		// �����D�揇��

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CLine;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CCollision : public CScene
{
public:
	typedef enum
	{
		SIDE_UPPER_FRONT = 0,
		SIDE_UPPER_RIGHT,
		SIDE_UPPER_BEHIND,
		SIDE_UPPER_LEFT,
		SIDE_MIDDLE_LEFT_FRONT,
		SIDE_MIDDLE_RIGHT_FRONT,
		SIDE_MIDDLE_RIGHT_BEHIND,
		SIDE_MIDDLE_LEFT_BEHIND,
		SIDE_LOWER_FRONT,
		SIDE_LOWER_RIGHT,
		SIDE_LOWER_BEHIND,
		SIDE_LOWER_LEFT,
		SIDE_MAX
	}SIDE;

	CCollision(int nPriority = COLLISION_PRIORITY, CScene::OBJTYPE objType = CScene::OBJTYPE_COLLISION);
	~CCollision();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CCollision* Create(D3DXVECTOR3 pos, float fWidth, float fHeight, float fDepth);

	static void Load(void);

	D3DXVECTOR3 GetSize(void) { return D3DXVECTOR3(m_fWidth, m_fHeight, m_fDepth); }
	void SetSize(D3DXVECTOR3 size);

	D3DXVECTOR3 GetPos(void) { return m_pos; }
	void SetPos(D3DXVECTOR3 pos);

	static bool Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 posOld, D3DXVECTOR3 vtxMax, D3DXVECTOR3 vtxMin);
	bool CollisionBox(D3DXVECTOR3 *pPos, D3DXVECTOR3 posOld, D3DXVECTOR3 vtxMax, D3DXVECTOR3 vtxMin);

private:
	D3DXVECTOR3 m_pos;		// �ʒu
	CLine **m_pLine;		// ����p�{�b�N�X�������C��
	float m_fWidth;			// ��
	float m_fHeight;		// ����
	float m_fDepth;			// ���s
};

#endif
