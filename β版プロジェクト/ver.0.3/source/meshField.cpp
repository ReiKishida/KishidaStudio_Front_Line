//=============================================================================
//
// ���b�V���t�B�[���h���� [meshField.cpp]
// Author : Takuto Ishida
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "meshField.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "debugProc.h"
#include "texture.h"

#include <stdio.h>

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================

//==================================
// ��������
//==================================
CMeshField* CMeshField::Create(D3DXVECTOR3 pos, int nField)
{
	CMeshField *pMeshField;

	pMeshField = new CMeshField;	// ���������m��

	if (pMeshField != NULL)
	{// �������m�ې���
		pMeshField->m_pos = pos;
		pMeshField->Init(nField);
	}

	return pMeshField;
}

//=========================================
// �R���X�g���N�^
//=========================================
CMeshField::CMeshField(int nPriority, CScene::OBJTYPE objType) : CScene3DIndex(nPriority, objType)
{
	m_nWire = 0;
	m_fRipples = 0.0f;
	m_nType = 0;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//=========================================
// �f�X�g���N�^
//=========================================
CMeshField::~CMeshField()
{
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CMeshField::Init(int nField)
{
	// ��ނ̐ݒ�
	CScene3DIndex::SetType(CScene3DIndex::TYPE_FIELD);

	// ����������
	CScene3DIndex::Init();

	// ���_�̏��ǂݍ���
	LoadInfo(nField);

	// ���_�̍쐬
	CScene3DIndex::MakeVertex();

	// ���_���̓ǂݍ���
	LoadVtx();

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CMeshField::Uninit(void)
{
	// �I������
	CScene3DIndex::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CMeshField::Update(void)
{
	CScene3DIndex::Update();

	// �L�[�{�[�h�̓��͂��擾
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();

	if (pKeyboard->GetTrigger(DIK_F8) == true)
	{// ���C���[�t���[���\��
		m_nWire = (1 + m_nWire) % 3;
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CMeshField::Draw(void)
{
	// �f�o�C�X�̎擾
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	// ���C���[�t���[����\��
	pDevice->SetRenderState(D3DRS_FILLMODE, m_nWire);

	CScene3DIndex::Draw();

	// ���C���[�t���[�����\��
	pDevice->SetRenderState(D3DRS_FILLMODE, 0);
}

//=========================================
// �n�ʂ̍����̎擾
//=========================================
float CMeshField::GetHeight(D3DXVECTOR3 pos)
{
	float fHeight = 0.0f;	// �Ԃ�����
	float fDiff = 0.0f;		// ����p��Y������������ۑ�
	int nPosBrock[2];		// �v���C���[�̂���|���S���̃u���b�N
	int nVtxNumber[4];		// �u���b�N�̒��_�ԍ�

	// �v���C���[�̃x�N�g�������߂�
	D3DXVECTOR3 vecPlayer = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	float fWidth, fDepth;	// ���_�Ԃ̋���
	int nMeshX, nMeshZ;		// ������

	// �f�[�^�̎擾
	CScene3DIndex::GetInfo(&fWidth, NULL, &fDepth, &nMeshX, NULL, &nMeshZ);

	// ���_����ݒ�
	VERTEX_3D *pVtx;	// ���_���̃|�C���^

	// ���_�o�b�t�@�̎擾
	LPDIRECT3DVERTEXBUFFER9 vtxBuff = CScene3DIndex::GetVtxBuff();

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	vtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	D3DXVECTOR3 vecA, vecB, collisionNor, collisionPos[3];

	// �v���C���[�̂���|���S������
	nPosBrock[0] = (int)(pos.x + (fWidth * 0.5f)) / ((int)fWidth / nMeshX);		// X���̃u���b�N����o��
	nPosBrock[1] = (int)(pos.z - (fDepth * 0.5f)) / ((int)-fDepth / nMeshX);	// Z���̃u���b�N����o��

	if (nPosBrock[0] >= nMeshX || (int)(pos.x + (fWidth * 0.5f)) < 0 ||
		nPosBrock[1] >= nMeshZ || (int)(pos.z - (fDepth * 0.5f)) > 0)
	{// �|���S���ɏ���Ă��Ȃ��Ƃ�
		CDebugProc::Print("����Ă��Ȃ�");
		return 0;
	}

	// ���_�ԍ����v�Z
	nVtxNumber[0] = nPosBrock[0] + ((nPosBrock[1] + 1) * (nMeshX + 1));			// �����̃u���b�N�̍����ԍ�
	nVtxNumber[1] = nPosBrock[0] + (nPosBrock[1] * (nMeshX + 1));					// �����̃u���b�N�̍���ԍ�
	nVtxNumber[2] = (nPosBrock[0] + 1) + ((nPosBrock[1] + 1) * (nMeshX + 1));		// �����̃u���b�N�̉E���ԍ�
	nVtxNumber[3] = (nPosBrock[0] + 1) + (nPosBrock[1] * (nMeshX + 1));			// �����̃u���b�N�̉E��ԍ�
	CDebugProc::Print("%d %d", nPosBrock[0], nPosBrock[1]);

																					// �O�ςō��E�ǂ���̃|���S��������
	vecA = pVtx[nVtxNumber[2]].pos - pVtx[nVtxNumber[1]].pos;	// ���_����ړI�̒��_�ւ̕���
	vecB = pos - pVtx[nVtxNumber[1]].pos;						// �v���C���[�@�|�@vecA�̎n�܂�̒��_

	if ((vecA.z * vecB.x) - (vecA.x * vecB.z) >= 0)
	{// ����
	 // ����p���_���W
		collisionPos[0] = pVtx[nVtxNumber[0]].pos;
		collisionPos[1] = pVtx[nVtxNumber[1]].pos;
		collisionPos[2] = pVtx[nVtxNumber[2]].pos;
	}
	else
	{// �E��
	 // ����p���_���W
		collisionPos[0] = pVtx[nVtxNumber[3]].pos;
		collisionPos[1] = pVtx[nVtxNumber[2]].pos;
		collisionPos[2] = pVtx[nVtxNumber[1]].pos;
	}

	// ����p�ɉ�����l��ۑ�
	fDiff = collisionPos[0].y;

	// ���S�̒��_�̍�����0�ɂȂ�悤�Ɉ���
	collisionPos[0].y -= fDiff;
	collisionPos[1].y -= fDiff;
	collisionPos[2].y -= fDiff;

	// ����p�@��
	vecA = collisionPos[1] - collisionPos[0];	// ����
	vecB = collisionPos[2] - collisionPos[0];	// �E��
	D3DXVec3Cross(&collisionNor, &vecA, &vecB);			// �O�ς����߂�
	D3DXVec3Normalize(&collisionNor, &collisionNor);	// ���K��

	// �v���C���[�̃x�N�g��
	vecPlayer = pos - collisionPos[0];

	// �v���C���[��Y�������߂�
	fHeight = (-(collisionNor.x * vecPlayer.x) - (collisionNor.z * vecPlayer.z)) / collisionNor.y;

	// ����p�ɉ���������␳
	fHeight += fDiff;

	// ���_�o�b�t�@���A�����b�N
	vtxBuff->Unlock();

	return fHeight;
}

//=========================================================================================================================
// �X�΂ɂ��ړ��ʂ̎擾
//=========================================================================================================================
float CMeshField::GetAngle(D3DXVECTOR3 pos, float fRot)
{
	float fSpeed = 0.0f;	// �Ԃ����x
	int nPosBrock[2];		// �v���C���[�̂���|���S���̃u���b�N
	int nVtxNumber[4];		// �u���b�N�̒��_�ԍ�
	D3DXVECTOR3 vecPlayer;	// �v���C���[�̌����̃x�N�g��
	float fAngle;			// �v���C���[�̌����Ɩ@���̂Ȃ��p

	float fWidth, fDepth;	// ���_�Ԃ̋���
	int nMeshX, nMeshZ;		// ������

	// �f�[�^�̎擾
	CScene3DIndex::GetInfo(&fWidth, NULL, &fDepth, &nMeshX, NULL, &nMeshZ);

	// ���_����ݒ�
	VERTEX_3D *pVtx;	// ���_���̃|�C���^

	// ���_�o�b�t�@�̎擾
	LPDIRECT3DVERTEXBUFFER9 vtxBuff = CScene3DIndex::GetVtxBuff();

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	vtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	D3DXVECTOR3 vecA, vecB, collisionNor, collisionPos[3];

	// �v���C���[�̂���|���S������
	nPosBrock[0] = (int)(pos.x + (fWidth * 0.5f)) / (int)fWidth;		// X���̃u���b�N����o��
	nPosBrock[1] = (int)(pos.z - (fDepth * 0.5f)) / (int)fDepth * -1;	// Z���̃u���b�N����o��

	if (nPosBrock[0] >= nMeshX || (int)(pos.x + (fWidth * 0.5f)) < 0 ||
		nPosBrock[1] >= nMeshZ || (int)(pos.z - (fDepth * 0.5f)) > 0)
	{// �|���S���ɏ���Ă��Ȃ��Ƃ�
		return 0;
	}

	// ���_�ԍ����v�Z
	nVtxNumber[0] = nPosBrock[0] + ((nPosBrock[1] + 1) * (nMeshX + 1));			// �����̃u���b�N�̍����ԍ�
	nVtxNumber[1] = nPosBrock[0] + (nPosBrock[1] * (nMeshX + 1));					// �����̃u���b�N�̍���ԍ�
	nVtxNumber[2] = (nPosBrock[0] + 1) + ((nPosBrock[1] + 1) * (nMeshX + 1));		// �����̃u���b�N�̉E���ԍ�
	nVtxNumber[3] = (nPosBrock[0] + 1) + (nPosBrock[1] * (nMeshX + 1));			// �����̃u���b�N�̉E��ԍ�

																					// �O�ςō��E�ǂ���̃|���S��������
	vecA = pVtx[nVtxNumber[2]].pos - pVtx[nVtxNumber[1]].pos;	// ���_����ړI�̒��_�ւ̕���
	vecB = pos - pVtx[nVtxNumber[1]].pos;						// �v���C���[�@�|�@vecA�̎n�܂�̒��_

	if ((vecA.z * vecB.x) - (vecA.x * vecB.z) >= 0)
	{// ����
	 // ����p���_���W
		collisionPos[0] = pVtx[nVtxNumber[0]].pos;
		collisionPos[1] = pVtx[nVtxNumber[1]].pos;
		collisionPos[2] = pVtx[nVtxNumber[2]].pos;
	}
	else
	{// �E��
	 // ����p���_���W
		collisionPos[0] = pVtx[nVtxNumber[3]].pos;
		collisionPos[1] = pVtx[nVtxNumber[2]].pos;
		collisionPos[2] = pVtx[nVtxNumber[1]].pos;
	}

	// �v���C���[�̂���|���S���̖@�����o��
	vecA = collisionPos[1] - collisionPos[0];	// ����
	vecB = collisionPos[2] - collisionPos[0];	// �E��
	D3DXVec3Cross(&collisionNor, &vecA, &vecB);			// �O�ς����߂�
	D3DXVec3Normalize(&collisionNor, &collisionNor);	// ���K��

														// �v���C���[�̌����̃x�N�g���̐���
	vecPlayer = D3DXVECTOR3(pos.x + sinf(fRot), pos.y, pos.z + cosf(fRot)) - pos;

	// �v���C���[�̌����Ɩ@���̂Ȃ��p�����߂�
	fAngle = (vecPlayer.x * collisionNor.x) + (vecPlayer.y * collisionNor.y) + (vecPlayer.z * collisionNor.z);
	fAngle = fAngle / (sqrtf(powf(vecPlayer.x, 2) + powf(vecPlayer.y, 2) + powf(vecPlayer.z, 2)) * (sqrtf(powf(collisionNor.x, 2) + powf(collisionNor.y, 2) + powf(collisionNor.z, 2))));

	return fAngle;
}

//=========================================================================================================================
// ��������̎擾
//=========================================================================================================================
float CMeshField::GetSlipAngle(D3DXVECTOR3 pos)
{
	int nPosBrock[2];		// �v���C���[�̂���|���S���̃u���b�N
	int nVtxNumber[4];		// �u���b�N�̒��_�ԍ�
	D3DXVECTOR3 vecPlayer;	// �v���C���[�̌����̃x�N�g��
	float fAngle;			// �v���C���[�̌����Ɩ@���̂Ȃ��p
	D3DXVECTOR3 vecA, vecB, collisionNor;	// �v�Z�p�x�N�g��

	float fWidth, fDepth;	// ���_�Ԃ̋���
	int nMeshX, nMeshZ;		// ������

	// �f�[�^�̎擾
	CScene3DIndex::GetInfo(&fWidth, NULL, &fDepth, &nMeshX, NULL, &nMeshZ);

	// ���_����ݒ�
	VERTEX_3D *pVtx;	// ���_���̃|�C���^

	// ���_�o�b�t�@�̎擾
	LPDIRECT3DVERTEXBUFFER9 vtxBuff = CScene3DIndex::GetVtxBuff();

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	vtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// �v���C���[�̂���|���S������
	nPosBrock[0] = (int)(pos.x + (fWidth * 0.5f)) / (int)fWidth;		// X���̃u���b�N����o��
	nPosBrock[1] = (int)(pos.z - (fDepth * 0.5f)) / (int)fDepth * -1;	// Z���̃u���b�N����o��

	if (nPosBrock[0] >= nMeshX || (int)(pos.x + (fWidth * 0.5f)) < 0 ||
		nPosBrock[1] >= nMeshZ || (int)(pos.z - (fDepth * 0.5f)) > 0)
	{// �|���S���ɏ���Ă��Ȃ��Ƃ�
		return 0;
	}

	// ���_�ԍ����v�Z
	nVtxNumber[0] = nPosBrock[0] + ((nPosBrock[1] + 1) * (nMeshX + 1));			// �����̃u���b�N�̍����ԍ�
	nVtxNumber[1] = nPosBrock[0] + (nPosBrock[1] * (nMeshX + 1));					// �����̃u���b�N�̍���ԍ�
	nVtxNumber[2] = (nPosBrock[0] + 1) + ((nPosBrock[1] + 1) * (nMeshX + 1));		// �����̃u���b�N�̉E���ԍ�
	nVtxNumber[3] = (nPosBrock[0] + 1) + (nPosBrock[1] * (nMeshX + 1));			// �����̃u���b�N�̉E��ԍ�

	// �O�ςō��E�ǂ���̃|���S��������
	vecA = pVtx[nVtxNumber[2]].pos - pVtx[nVtxNumber[1]].pos;	// ���_����ړI�̒��_�ւ̕���
	vecB = pos - pVtx[nVtxNumber[1]].pos;						// �v���C���[�@�|�@vecA�̎n�܂�̒��_

	if ((vecA.z * vecB.x) - (vecA.x * vecB.z) >= 0)
	{// ����
		collisionNor = pVtx[nVtxNumber[0]].nor;	// ���_�̖@����ۑ�
	}
	else
	{// �E��
		collisionNor = pVtx[nVtxNumber[3]].nor;	// ���_�̖@����ۑ�
	}

	// �x�N�g���̊p�x���擾
	fAngle = acosf(collisionNor.x / sqrtf((collisionNor.x * collisionNor.x) + (collisionNor.y * collisionNor.y)));
	if (collisionNor.y < 0)
	{// -��������
		fAngle = 2.0f * D3DX_PI - fAngle;
	}

	if (collisionNor.x < 0)
	{// ������n�ʂɂ��낦��
		fAngle *= -1.0f;
	}

	if (isnan(fAngle))
	{// nan�`�F�b�N
		fAngle = 0.0f;
	}

	// ���]���Ȃ��悤�Ɋp�x�𒲐�
	if (fAngle > D3DX_PI)
	{
		fAngle -= D3DX_PI * 2.0f;
	}
	if (fAngle < -D3DX_PI)
	{
		fAngle += D3DX_PI * 2.0f;
	}

	return fAngle;
}

//=========================================================================================================================
// �@���̎擾
//=========================================================================================================================
D3DXVECTOR3 CMeshField::GetNor(D3DXVECTOR3 pos)
{
	float fDiff = 0.0f;		// ����p��Y������������ۑ�
	int nPosBrock[2];		// �v���C���[�̂���|���S���̃u���b�N
	int nVtxNumber[4];		// �u���b�N�̒��_�ԍ�

	// �v���C���[�̃x�N�g�������߂�
	D3DXVECTOR3 vecPlayer = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	float fWidth, fDepth;	// ���_�Ԃ̋���
	int nMeshX, nMeshZ;		// ������

	// �f�[�^�̎擾
	CScene3DIndex::GetInfo(&fWidth, NULL, &fDepth, &nMeshX, NULL, &nMeshZ);

	// ���_����ݒ�
	VERTEX_3D *pVtx;	// ���_���̃|�C���^

	// ���_�o�b�t�@�̎擾
	LPDIRECT3DVERTEXBUFFER9 vtxBuff = CScene3DIndex::GetVtxBuff();

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	vtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	D3DXVECTOR3 vecA, vecB, collisionNor, collisionPos[3];

	// �v���C���[�̂���|���S������
	nPosBrock[0] = (int)(pos.x + (fWidth * 0.5f)) / (int)fWidth;		// X���̃u���b�N����o��
	nPosBrock[1] = (int)(pos.z - (fDepth * 0.5f)) / (int)fDepth * -1;	// Z���̃u���b�N����o��

	if (nPosBrock[0] >= nMeshX || (int)(pos.x + (fWidth * 0.5f)) < 0 ||
		nPosBrock[1] >= nMeshZ || (int)(pos.z - (fDepth * 0.5f)) > 0)
	{// �|���S���ɏ���Ă��Ȃ��Ƃ�
		return D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	// ���_�ԍ����v�Z
	nVtxNumber[0] = nPosBrock[0] + ((nPosBrock[1] + 1) * (nMeshX + 1));			// �����̃u���b�N�̍����ԍ�
	nVtxNumber[1] = nPosBrock[0] + (nPosBrock[1] * (nMeshX + 1));					// �����̃u���b�N�̍���ԍ�
	nVtxNumber[2] = (nPosBrock[0] + 1) + ((nPosBrock[1] + 1) * (nMeshX + 1));		// �����̃u���b�N�̉E���ԍ�
	nVtxNumber[3] = (nPosBrock[0] + 1) + (nPosBrock[1] * (nMeshX + 1));			// �����̃u���b�N�̉E��ԍ�

																					// �O�ςō��E�ǂ���̃|���S��������
	vecA = pVtx[nVtxNumber[2]].pos - pVtx[nVtxNumber[1]].pos;	// ���_����ړI�̒��_�ւ̕���
	vecB = pos - pVtx[nVtxNumber[1]].pos;						// �v���C���[�@�|�@vecA�̎n�܂�̒��_

	if ((vecA.z * vecB.x) - (vecA.x * vecB.z) >= 0)
	{// ����
	 // ����p���_���W
		collisionPos[0] = pVtx[nVtxNumber[0]].pos;
		collisionPos[1] = pVtx[nVtxNumber[1]].pos;
		collisionPos[2] = pVtx[nVtxNumber[2]].pos;
	}
	else
	{// �E��
	 // ����p���_���W
		collisionPos[0] = pVtx[nVtxNumber[3]].pos;
		collisionPos[1] = pVtx[nVtxNumber[2]].pos;
		collisionPos[2] = pVtx[nVtxNumber[1]].pos;
	}

	// ����p�@��
	vecA = collisionPos[1] - collisionPos[0];	// ����
	vecB = collisionPos[2] - collisionPos[0];	// �E��
	D3DXVec3Cross(&collisionNor, &vecA, &vecB);			// �O�ς����߂�
	D3DXVec3Normalize(&collisionNor, &collisionNor);	// ���K��

	// ���_�o�b�t�@���A�����b�N
	vtxBuff->Unlock();

	return collisionNor;
}

//=========================================================================================================================
// ���݂���n�ʂ��ǂ����̔���
//=========================================================================================================================
bool CMeshField::OnField(D3DXVECTOR3 pos, float fMaxHeight)
{
	// �v���C���[�ƃ��b�V���t�B�[���h�̈ʒu�̍������o��
	D3DXVECTOR3 posDiff = pos - m_pos;

	int nPosBrock[2];		// �v���C���[�̂���|���S���̃u���b�N
	int nVtxNumber[4];		// �u���b�N�̒��_�ԍ�
	float fWidth, fDepth;	// ���_�Ԃ̋���
	int nMeshX, nMeshZ;		// ������

	// �f�[�^�̎擾
	CScene3DIndex::GetInfo(&fWidth, NULL, &fDepth, &nMeshX, NULL, &nMeshZ);

	// �v���C���[�̂���|���S������
	nPosBrock[0] = (int)(pos.x + (fWidth * 0.5f)) / (int)fWidth;		// X���̃u���b�N����o��
	nPosBrock[1] = (int)(pos.z - (fDepth * 0.5f)) / (int)fDepth * -1;	// Z���̃u���b�N����o��

	// ���_�ԍ����v�Z
	nVtxNumber[0] = nPosBrock[0] + ((nPosBrock[1] + 1) * (nMeshX + 1));			// �����̃u���b�N�̍����ԍ�
	nVtxNumber[1] = nPosBrock[0] + (nPosBrock[1] * (nMeshX + 1));				// �����̃u���b�N�̍���ԍ�
	nVtxNumber[2] = (nPosBrock[0] + 1) + ((nPosBrock[1] + 1) * (nMeshX + 1));	// �����̃u���b�N�̉E���ԍ�
	nVtxNumber[3] = (nPosBrock[0] + 1) + (nPosBrock[1] * (nMeshX + 1));			// �����̃u���b�N�̉E��ԍ�

	//���_���ւ̃|�C���^
	VERTEX_3D *pVtx;

	// ���_�o�b�t�@�̎擾
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = CScene3DIndex::GetVtxBuff();

	//���_�o�b�t�@�����b�N�����_�f�[�^�ւ̃|�C���^���擾
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���݂̏���Ă���u���b�N�̒��_
	int nMostUnder, nUnder0, nUnder1;

	if (pVtx[nVtxNumber[0]].pos.y < pVtx[nVtxNumber[1]].pos.y) { nUnder0 = nVtxNumber[0]; }
	else { nUnder0 = nVtxNumber[1]; }

	if (pVtx[nVtxNumber[2]].pos.y < pVtx[nVtxNumber[3]].pos.y) { nUnder1 = nVtxNumber[2]; }
	else { nUnder1 = nVtxNumber[3]; }

	if (pVtx[nUnder0].pos.y < pVtx[nUnder1].pos.y) { nMostUnder = nUnder0; }
	else { nMostUnder = nUnder1; }

	if (pVtx[nMostUnder].pos.y + m_pos.y > pos.y + fMaxHeight + 10.0f)
	{
		// ���_�o�b�t�@���A�����b�N
		pVtxBuff->Unlock();

		return false;
	}

	// ���_�o�b�t�@���A�����b�N
	pVtxBuff->Unlock();

	return true;
}

//=============================================================================
// �n�ʂ̔g�䏈��
//=============================================================================
void CMeshField::SetRipples(D3DXVECTOR3 pos, float fRange, float fHeight)
{
	// ���_����ݒ�
	VERTEX_3D *pVtx;	// ���_���̃|�C���^

	// ���_�o�b�t�@�̎擾
	LPDIRECT3DVERTEXBUFFER9 vtxBuff = CScene3DIndex::GetVtxBuff();

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	vtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// �g��𓮂���
	m_fRipples += 0.08f;

	// ���_���̎擾
	int nNumVtx = CScene3DIndex::GetNumVtx();

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{// �S���_�̋������v�Z
		float fLength = powf(pVtx[nCntVtx].pos.x - pos.x, 2.0f) + powf(pVtx[nCntVtx].pos.z - pos.z, 2.0f);

		if (fLength <= powf(fRange, 2.0f))
		{// �͈͓��ɂ��钸�_
			float fVtxHeight = sinf(D3DX_PI * ((sqrtf(fLength) / fRange) * -14.0f) + m_fRipples) * (fHeight * (1.0f - (sqrtf(fLength) / fRange)));

			pVtx[nCntVtx].pos.y = fVtxHeight;
		}
	}

	// ���_�o�b�t�@���A�����b�N����
	vtxBuff->Unlock();
}

//=========================================================================================================================
// �n�ʂ̏����擾
//=========================================================================================================================
void CMeshField::LoadInfo(int nField)
{
	FILE *pFile = NULL;	// �t�@�C���̃|�C���^�ϐ�
	char aStr[128];	// �擾���镶����

	float fWidth = 0.0f, fDepth = 0.0f;		// ���_�Ԃ̋���
	int nBlockX = 0, nBlockZ = 0;			// ������
	char *FileName[1] =
	{
		"data/TEXT/FIELD/field.txt"
	};

	// �t�@�C�����J��
	pFile = fopen(FileName[nField], "r");

	if (NULL != pFile)
	{// �t�@�C����������
		while (strcmp(aStr, "END_FIELDSET") != 0)
		{
			fscanf(pFile, "%s", aStr);
			if (strcmp(aStr, "WIDTH") == 0)
			{// ��
				fscanf(pFile, " = %f", &fWidth);
			}
			if (strcmp(aStr, "DEPTH") == 0)
			{// ���s
				fscanf(pFile, " = %f", &fDepth);
			}
			if (strcmp(aStr, "XBLOCK") == 0)
			{// ���̕�����
				fscanf(pFile, " = %d", &nBlockX);
			}
			if (strcmp(aStr, "ZBLOCK") == 0)
			{// ���s�̕�����
				fscanf(pFile, " = %d", &nBlockZ);
			}
			if (strcmp(aStr, "VERTEX_FILENAME") == 0)
			{// �ǂݍ��ރo�C�i���t�@�C��
				fscanf(pFile, " = %s", &m_aFileName[0]);
			}
		}

		// �f�[�^�̐ݒ�
		CScene3DIndex::SetInfo(fWidth, 0.0f, fDepth, nBlockX, 0, nBlockZ);

		// �t�@�C�������
		fclose(pFile);
	}
	else
	{// �t�@�C�����J���Ȃ�
		MessageBox(0, "�t�@�C��������܂���I", "field.txt", MB_OK);
	}
}

//=========================================================================================================================
// ���_�����t�@�C������ǂݍ���
//=========================================================================================================================
void CMeshField::LoadVtx(void)
{
	// �t�@�C���̃|�C���^�ϐ�
	FILE *pFile;

	// �t�@�C�����J��
	pFile = fopen(&m_aFileName[0], "rb");

	if (NULL != pFile)
	{// �t�@�C�����������Ƃ�
		VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

		// ���_�o�b�t�@�̎擾
		LPDIRECT3DVERTEXBUFFER9 vtxBuff = CScene3DIndex::GetVtxBuff();

		// ���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
		vtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// ���_���̎擾
		int nNumVtx = CScene3DIndex::GetNumVtx();

		// ���_�����t�@�C���ɏ�������
		fread(pVtx, sizeof(VERTEX_3D), nNumVtx, pFile);

		// ���_�o�b�t�@���A�����b�N
		vtxBuff->Unlock();

		// ���_�o�b�t�@�̐ݒ�
		CScene3DIndex::SetVtxBuff(vtxBuff);

		// �t�@�C�������
		fclose(pFile);
	}
	else
	{// �t�@�C�����J���Ȃ�
		MessageBox(0, "�t�@�C��������܂���I", "field.bin", MB_OK);
	}
}