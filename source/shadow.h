//=============================================================================
//
// �e���� [shadow.h]
// Author : Takuto Ishida
//
//=============================================================================
#ifndef _SHADOW_H_
#define _SHADOW_H_

#include "scene.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define SHADOW_PRIORITY		(4)

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CPolygon;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CShadow : public CScene
{
public:
	CShadow(int nPriority = SHADOW_PRIORITY, CScene::OBJTYPE objType = CScene::OBJTYPE_SHADOW);
	~CShadow();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CShadow *Create(D3DXVECTOR3 *pPos);

private:
	LPD3DXMESH				m_pMesh;			// ���b�V�����ւ̃|�C���^
	LPD3DXBUFFER			m_pBuffMat;			// �}�e���A�����ւ̃|�C���^
	DWORD					m_nNumMat;			// �}�e���A�����̐�

	D3DXMATRIX				m_mtxWorld;		// ���[���h�}�g���b�N�X
	D3DXVECTOR3				*m_pos;			// �ʒu
	D3DXVECTOR3				m_rot;			// ��]
	CPolygon				*m_pBigScreen;	// ��ʂ�h��Ԃ��p
};

#endif
