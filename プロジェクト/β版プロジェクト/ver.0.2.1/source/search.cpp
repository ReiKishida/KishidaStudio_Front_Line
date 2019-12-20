//=============================================================================
//
// ���f������ [model.cpp]
// Author : Takuto Ishida
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "search.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "debugProc.h"
#include "light.h"
#include "camera.h"
#include "player.h"

#include <stdio.h>

//=========================================
// �}�N����`
//=========================================

//=========================================
// �ÓI�����o�ϐ��錾
//=========================================

//=========================================
// ���f���̐���
//=========================================
CSearch *CSearch::Create(D3DXMATRIX *pParent)
{
	CSearch *pSearch = NULL;

	if (pSearch == NULL)
	{// ���������m�ۂ���
		pSearch = new CSearch;	// �������m��
		pSearch->m_pMtxParent = pParent;
	}

	return pSearch;
}

//=========================================
// �p�x���
//=========================================
void CSearch::SetRot(D3DXVECTOR3 rot)
{
	D3DXVECTOR3 Pos;

	m_rot = rot;

	m_pos.x = (m_pos.x * cosf(m_rot.y)) - (m_pos.z * sinf(m_rot.y));
	m_pos.y = 0.0f;
	m_pos.z = (m_pos.x * sinf(m_rot.y)) + (m_pos.z * cosf(m_rot.y));

}

//=========================================
// �p�x���
//=========================================
bool CSearch::Distance(void)
{
	bool bFind = false;

	return bFind;
}

//=========================================
// �R���X�g���N�^
//=========================================
CSearch::CSearch()
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ʒu
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �p�x

	m_pParent = NULL;
}

//=========================================
// �f�X�g���N�^
//=========================================
CSearch::~CSearch()
{
}

//=========================================
// ����������
//=========================================
HRESULT CSearch::Init()
{
	return S_OK;
}

//=========================================
// �I������
//=========================================
void CSearch::Uninit(void)
{

}

//=========================================
// �X�V����
//=========================================
void CSearch::Update(void)
{


}

//=========================================
// �`�揈��
//=========================================
void CSearch::Draw(void)
{
	// �f�o�C�X�̎擾
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans, mtxParent;	// �v�Z�p�}�g���b�N�X

	if (m_pParent != NULL)
	{// �e�̃}�g���b�N�X���擾
		mtxParent = m_pParent->GetMtxWorld();
	}
	else if (m_pMtxParent != NULL)
	{
		mtxParent = *m_pMtxParent;
	}

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// �ړ��𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	if (m_pMtxParent != NULL)
	{// �e�̃}�g���b�N�X�Ɗ|�����킹��
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxParent);
	}

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

}

//=========================================
// �x�N�g���v�Z�̏���
//=========================================
D3DXVECTOR3 CSearch::Sub_Vector(const D3DXVECTOR3 & Pos0, const D3DXVECTOR3 & Pos1)
{
	D3DXVECTOR3 Vec;

	Vec.x = Pos0.x - Pos1.x;
	Vec.y = Pos0.y - Pos1.y;
	Vec.z = Pos0.z - Pos1.z;

	return Vec;
}

//=========================================
// �O�όv�Z�̏���
//=========================================
D3DXVECTOR3 CSearch::Cross_Product(const D3DXVECTOR3 & VecA, const D3DXVECTOR3 & VecB)
{
	D3DXVECTOR3 Vec;
	Vec.x = (VecA.y * VecB.z) - (VecA.z * VecB.y);
	Vec.y = (VecA.z * VecB.x) - (VecA.x * VecB.z);
	Vec.z = (VecA.x * VecB.y) - (VecA.y * VecB.x);

	return Vec;
}
