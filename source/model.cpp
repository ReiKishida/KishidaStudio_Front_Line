//=============================================================================
//
// ���f������ [model.cpp]
// Author : Takuto Ishida
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "model.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "debugProc.h"
#include "light.h"
#include "camera.h"

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
CModel *CModel::Create(D3DXMATRIX *pParent)
{
	CModel *pModel = NULL;

	if (pModel == NULL)
	{// ���������m�ۂ���
		pModel = new CModel;	// �������m��
		pModel->m_pMtxParent = pParent;
	}

	return pModel;
}

//=========================================
// �R���X�g���N�^
//=========================================
CModel::CModel()
{
	m_vtxMin = D3DXVECTOR3(10000.0f, 10000.0f, 10000.0f);
	m_vtxMax = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ʒu
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �p�x

	m_pTexture = NULL;
	m_pMesh = NULL;
	m_pBuffMat = NULL;
	m_nNumMat = 0;
}

//=========================================
// �f�X�g���N�^
//=========================================
CModel::~CModel()
{
}

//=========================================
// ����������
//=========================================
HRESULT CModel::Init()
{
	int nNumVtx;		// ���_��
	DWORD sizeFVF;		// ���_�t�H�[�}�b�g�̃T�C�Y
	BYTE *pVtxBuff;		// ���_�o�b�t�@�ւ̃|�C���^

	// ���_�����擾
	nNumVtx = m_pMesh->GetNumVertices();

	// ���_�t�H�[�}�b�g�̃T�C�Y���擾
	sizeFVF = D3DXGetFVFVertexSize(m_pMesh->GetFVF());

	// ���_�o�b�t�@�����b�N
	m_pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{// ���_���W�̔�r
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;	// ���_���W�̑��

		// X�̒l�̔�r
		if (m_vtxMin.x > vtx.x)
		{// �ŏ��l�Ɣ�r
			m_vtxMin.x = vtx.x;	// �ŏ��l��菬������Α��
		}

		if (m_vtxMax.x < vtx.x)
		{// �ő�l�Ɣ�r
			m_vtxMax.x = vtx.x;	// �ő�l���傫����Α��
		}

		// Y�̒l�̔�r
		if (m_vtxMin.y > vtx.y)
		{// �ŏ��l�Ɣ�r
			m_vtxMin.y = vtx.y;	// �ŏ��l��菬������Α��
		}

		if (m_vtxMax.y < vtx.y)
		{// �ő�l�Ɣ�r
			m_vtxMax.y = vtx.y;	// �ő�l���傫����Α��
		}

		// Z�̒l�̔�r
		if (m_vtxMin.z > vtx.z)
		{// �ŏ��l�Ɣ�r
			m_vtxMin.z = vtx.z;	// �ŏ��l��菬������Α��
		}

		if (m_vtxMax.z < vtx.z)
		{// �ő�l�Ɣ�r
			m_vtxMax.z = vtx.z;	// �ő�l���傫����Α��
		}

		pVtxBuff += sizeFVF;	// �T�C�Y���|�C���^��i�߂�
	}

	// ���_�o�b�t�@���A�����b�N
	m_pMesh->UnlockVertexBuffer();

	return S_OK;
}

//=========================================
// �I������
//=========================================
void CModel::Uninit(void)
{
	if (NULL != m_pTexture)
	{// �e�N�X�`���̔j��
		for (int nCntTex = 0; nCntTex < (int)m_nNumMat; nCntTex++)
		{
			if (NULL != m_pTexture[nCntTex])
			{
				m_pTexture[nCntTex]->Release();
				m_pTexture[nCntTex] = NULL;
			}
		}

		// �������̊J��
		delete[] m_pTexture;
		m_pTexture = NULL;
	}

	if (NULL != m_pMesh)
	{// ���b�V�����̔j��
		m_pMesh->Release();
		m_pMesh = NULL;
	}

	if (NULL != m_pBuffMat)
	{// �}�e���A�����̔j��
		m_pBuffMat->Release();
		m_pBuffMat = NULL;
	}

	if (NULL != m_nNumMat)
	{// �}�e���A���̐���j��
		m_nNumMat = NULL;
	}
}

//=========================================
// �X�V����
//=========================================
void CModel::Update(void)
{
}

//=========================================
// �`�揈��
//=========================================
void CModel::Draw(void)
{
	// �f�o�C�X�̎擾
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans, mtxParent;	// �v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;					// ���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;						// �}�e���A���f�[�^�ւ̃|�C���^

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

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	// �}�e���A���f�[�^�ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_nNumMat; nCntMat++)
	{
		// �}�e���A���̐ݒ�
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		//�I�u�W�F�N�g�̃e�N�X�`���[���X�e�[�W�O�ɃZ�b�g����
		pDevice->SetTexture(0, m_pTexture[nCntMat]);

		// ���f���̕`��
		m_pMesh->DrawSubset(nCntMat);
	}

	// �}�e���A�����f�t�H���g�ɖ߂�
	pDevice->SetMaterial(&matDef);
}

//=========================================
// �}�g���b�N�X�̌v�Z
//=========================================
void CModel::MatrixCalculation(void)
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
// �g�����f���̐ݒ�
//=========================================
void CModel::SetModel(char *pModelName)
{
	// �f�o�C�X�̎擾
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	D3DXLoadMeshFromX(pModelName,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_pBuffMat,
		NULL,
		&m_nNumMat,
		&m_pMesh);

	D3DXMATERIAL *pMat;				// �}�e���A���f�[�^�ւ̃|�C���^

	// �}�e���A�������e�N�X�`����p��
	m_pTexture = new LPDIRECT3DTEXTURE9[(int)m_nNumMat];

	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();
	for (int nCntMat = 0; nCntMat < (int)m_nNumMat; nCntMat++)
	{// �e�N�X�`���̐���
		m_pTexture[nCntMat] = NULL;
		D3DXCreateTextureFromFile(pDevice, pMat[nCntMat].pTextureFilename, &m_pTexture[nCntMat]);
	}
}

//=========================================
// �F�̐ݒ�
//=========================================
void CModel::SetColor(D3DXCOLOR col)
{
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	D3DMATERIAL9 matDef;	// ���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;	// �}�e���A���f�[�^�ւ̃|�C���^

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	// �}�e���A���f�[�^�ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	// �F��t����
	pMat->MatD3D.Diffuse = col;
	pMat->MatD3D.Ambient = col;

	// �}�e���A�����f�t�H���g�ɖ߂�
	pDevice->SetMaterial(&matDef);
}

//=========================================
// �F�̉��Z
//=========================================
void CModel::AddColor(D3DXCOLOR col)
{
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	D3DMATERIAL9 matDef;	// ���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;		// �}�e���A���f�[�^�ւ̃|�C���^

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	// �}�e���A���f�[�^�ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	// �F��t����
	pMat->MatD3D.Diffuse.r += col.r;
	pMat->MatD3D.Diffuse.g += col.g;
	pMat->MatD3D.Diffuse.b += col.b;
	pMat->MatD3D.Diffuse.a += col.a;

	pMat->MatD3D.Ambient.r += col.r;
	pMat->MatD3D.Ambient.g += col.g;
	pMat->MatD3D.Ambient.b += col.b;
	pMat->MatD3D.Ambient.a += col.a;

	// �}�e���A�����f�t�H���g�ɖ߂�
	pDevice->SetMaterial(&matDef);
}

//=========================================================================================================================
// �����蔻��
//=========================================================================================================================
bool CModel::Collision(D3DXVECTOR3 pos, float fRange)
{
	bool bHit = false;

	// �����XY�͈̔�
	float fLength = powf(fRange, 2);

	float fLengthX = pos.x - m_mtxWorld._41;						// X�̍�
	float fLengthY = pos.y - m_mtxWorld._42;						// Y�̍�
	float fLengthZ = pos.z - m_mtxWorld._43;						// Y�̍�
	float fLengthTotal = powf(fLengthX, 2) + powf(fLengthY, 2) + powf(fLengthZ, 2);		// XY�̍��̓��

	if (fLength >= fLengthTotal)
	{// ����Ǝ����̔���
		if (false == bHit)
		{// �����L���ɂ���
			bHit = true;
		}
	}

	return bHit;
}
