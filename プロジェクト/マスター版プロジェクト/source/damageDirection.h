//=============================================================================
//
// �U�����󂯂������\������ [damageDirection.h]
// Author : Takuto Ishida
//
//=============================================================================
#ifndef _DAMAGEDIRECTION_H_
#define _DAMAGEDIRECTION_H_

#include "scene.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define DAMAGEDIRECTION_PRIORITY	(7)		// �����̗D��ԍ�

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CScene2D;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 2D�{�^���̃N���X
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class CDamageDirection : public CScene
{
public:
	typedef enum
	{// ����
		DIRECTION_FRONT = 0,	// �O
		DIRECTION_BACK,			// ��
		DIRECTION_RIGHT,		// �E
		DIRECTION_LEFT,			// ��
		DIRECTION_MAX
	}DIRECTION;

	CDamageDirection(int nPriority = DAMAGEDIRECTION_PRIORITY, CScene::OBJTYPE objType = CScene::OBJTYPE_DAMAGEDIRECTION);
	~CDamageDirection();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CDamageDirection* Create(void);

	void CreatePolygon(D3DXVECTOR3 dirSelf, D3DXVECTOR3 dirObj);

	bool GetDamageDirection(DIRECTION direction) { return m_bHitDirection[direction]; };

private:
	CScene2D *m_pPolygon[DIRECTION_MAX];	// �����������|���S��
	int m_nCntLife[DIRECTION_MAX];			// �\�����Ԃ̃J�E���^
	bool m_bHitDirection[DIRECTION_MAX];	// �����������ǂ���
};
#endif