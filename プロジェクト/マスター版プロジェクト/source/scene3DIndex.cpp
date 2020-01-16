//=============================================================================
//
// �C���f�b�N�X3D�I�u�W�F�N�g���� [scene3DIndex.h]
// Author : Takuto Ishida
//
//=============================================================================
#include "scene3DIndex.h"
#include "renderer.h"
#include "manager.h"
#include "debugProc.h"
#include "input.h"
#include "light.h"
#include "camera.h"

//==================================
// �}�N����`
//==================================
#define SIZE_X	(50.0f)	// �|���S���̉��̒���
#define SIZE_Y	(50.0f)	// �|���S���̏c�̒���

//==================================
// �ÓI�����o�ϐ��錾
//==================================

//=========================================
// �e�N�X�`�������蓖�Ă�
//=========================================
void CScene3DIndex::BindTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
}

//=========================================
// �R���X�g���N�^
//=========================================
CScene3DIndex::CScene3DIndex(int nPriority, OBJTYPE objType) : CScene(nPriority, objType)
{
	m_pTexture = NULL;		// �e�N�X�`���ւ̃|�C���^
	m_pVtxBuff = NULL;		// ���_�o�b�t�@�ւ̃|�C���^

	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �|���S���̈ʒu
	m_fLengthX = 0;								// ��̃u���b�N�̕�
	m_fLengthY = 0;								// ��̃u���b�N�̍���
	m_fLengthZ = 0;								// ��̃u���b�N�̉��s
	m_nMeshX = 0;								// X���̕�����
	m_nMeshY = 0;								// Y���̕�����
	m_nMeshZ = 0;								// Z���̕�����
}

//=========================================
// �f�X�g���N�^
//=========================================
CScene3DIndex::~CScene3DIndex()
{
}

//=========================================
// �|���S���̏���������
//=========================================
HRESULT CScene3DIndex::Init(void)
{
	return S_OK;
}

//=========================================
// �|���S���̏I������
//=========================================
void CScene3DIndex::Uninit(void)
{
	// ���_�o�b�t�@�̔j��
	if (NULL != m_pVtxBuff)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	// ���S�t���O
	Release();
}

//=========================================
// �|���S���̍X�V����
//=========================================
void CScene3DIndex::Update(void)
{
}

//=========================================
// �|���S���̕`�揈��
//=========================================
void CScene3DIndex::Draw(void)
{
	// �f�o�C�X�̎擾
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;	// ���[���h�}�g���b�N�X�̕ۑ��p

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// �C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetIndices(m_pIdxBuff);

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, m_pTexture);

	// �|���S���̕`��
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, m_nNumVtx, 0, m_nNumPolygon);
}

//=========================================================================================================================
// ���_���̍쐬
//=========================================================================================================================
void CScene3DIndex::MakeVertex(void)
{
	// �f�o�C�X�̎擾
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	// ���_���ƃ|���S�����̌v�Z
	m_nNumVtx = (m_nMeshX + 1) * ((m_nMeshZ + m_nMeshY) + 1);
	m_nNumPolygon = m_nMeshX * (m_nMeshZ + m_nMeshY) * 2 + (4 * ((m_nMeshZ + m_nMeshY) - 1));

	// ���_�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * m_nNumVtx,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// �C���f�b�N�X�o�b�t�@�𐶐�
	pDevice->CreateIndexBuffer(sizeof(WORD) * (m_nMeshX * (m_nMeshZ + m_nMeshY) * 2 + (4 * ((m_nMeshZ + m_nMeshY) - 1)) + 2),
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,			// 16�r�b�g�̃f�[�^���m��
		D3DPOOL_MANAGED,
		&m_pIdxBuff,
		NULL);

	// ���_���̌v�Z
	SetVtx();
}

//=========================================================================================================================
// ���_���̐ݒ�
//=========================================================================================================================
void CScene3DIndex::SetVtx(void)
{
	// �J�E���g�p
	int nCntMeshYZ, nCntMeshX, nCntIdx;

	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntMeshYZ = 0; nCntMeshYZ <= m_nMeshZ + m_nMeshY; nCntMeshYZ++)
	{
		for (nCntMeshX = 0; nCntMeshX <= m_nMeshX; nCntMeshX++)
		{
			// ���_���W
			switch (CScene::GetObjType())
			{
			case CScene::OBJTYPE_FIELD:		// �t�B�[���h
				pVtx[0 + (nCntMeshX + (nCntMeshYZ * (m_nMeshX + 1)))].pos =
					D3DXVECTOR3(-(m_fLengthX * 0.5f) + (m_fLengthX / m_nMeshX) * nCntMeshX,
						0.0f,
						(m_fLengthZ * 0.5f) - (m_fLengthZ / m_nMeshZ) * nCntMeshYZ);
				break;
			}

			// ���_�J���[
			pVtx[nCntMeshX + (nCntMeshYZ * (m_nMeshX + 1))].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			//�e�N�X�`�����W
			pVtx[nCntMeshX + (nCntMeshYZ * (m_nMeshX + 1))].tex = D3DXVECTOR2(nCntMeshX * 1.0f, nCntMeshYZ * 1.0f);
		}
	}

	// �@���̌v�Z
	SetNor(pVtx);

	// ���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();

	WORD *pIdx;		// �C���f�b�N�X�f�[�^�ւ̃|�C���^

	// �C���f�b�N�X�o�b�t�@�����b�N���A�C���f�b�N�X�f�[�^�ւ̃|�C���^���擾
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	// �n��
	switch (CScene::GetObjType())
	{
	case CScene::OBJTYPE_FIELD:		// �t�B�[���h
		for (nCntMeshYZ = 0, nCntIdx = 0; nCntMeshYZ < m_nMeshZ; nCntMeshYZ++)
		{
			for (nCntMeshX = 0; nCntMeshX <= m_nMeshX; nCntMeshX++, nCntIdx++)
			{

				pIdx[0] = (m_nMeshX + 1) + nCntIdx;
				pIdx[1] = 0 + nCntIdx;

				pIdx += 2;

				if (nCntMeshYZ < m_nMeshZ - 1 && nCntMeshX == m_nMeshX)
				{// �܂�Ԃ���
					pIdx[0] = 0 + nCntIdx;
					pIdx[1] = (m_nMeshX + 1) + nCntIdx + 1;

					pIdx += 2;
				}
			}
		}
		break;
	}

	// �C���f�b�N�X�o�b�t�@���A�����b�N
	m_pIdxBuff->Unlock();
}

//=========================================================================================================================
// �@���̌v�Z
//=========================================================================================================================
void CScene3DIndex::SetNor(VERTEX_3D *pVtx)
{
	// �J�E���g�p
	int nCntMeshYZ, nCntMeshX, nCntVtx;

	for (nCntVtx = 0; nCntVtx < m_nNumVtx; nCntVtx++)
	{// �S���_�̖@����������
		pVtx[nCntVtx].nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	for (nCntMeshYZ = 0; nCntMeshYZ < m_nMeshZ; nCntMeshYZ++)
	{
		for (nCntMeshX = 0; nCntMeshX < m_nMeshX; nCntMeshX++)
		{// �@��
			D3DXVECTOR3 vecA;	// ���_�O���璸�_�P�ւ̕���
			D3DXVECTOR3 vecB;	// ���_�O���璸�_�Q�ւ̕���
			D3DXVECTOR3 nor[2];	// �@���̌v�Z����
			int nVtxNumber[4];	// �S���_�̔ԍ�

			nVtxNumber[0] = nCntMeshX + ((nCntMeshYZ + 1) * (m_nMeshX + 1));			// �����̃u���b�N�̍����ԍ�
			nVtxNumber[1] = nCntMeshX + (nCntMeshYZ * (m_nMeshX + 1));					// �����̃u���b�N�̍���ԍ�
			nVtxNumber[2] = (nCntMeshX + 1) + ((nCntMeshYZ + 1) * (m_nMeshX + 1));		// �����̃u���b�N�̉E���ԍ�
			nVtxNumber[3] = (nCntMeshX + 1) + (nCntMeshYZ * (m_nMeshX + 1));			// �����̃u���b�N�̉E��ԍ�

																						// �e�|���S���̒��_�����߂�
																						// ����-------------------------------------------------------------------------------------------
																						// 0
			vecA = pVtx[nVtxNumber[1]].pos - pVtx[nVtxNumber[0]].pos;	// ����
			vecB = pVtx[nVtxNumber[2]].pos - pVtx[nVtxNumber[0]].pos;	// �E��

			D3DXVec3Cross(&nor[0], &vecA, &vecB);	// �O�ς����߂�
			D3DXVec3Normalize(&nor[0], &nor[0]);	// ���K��

													// �E��-------------------------------------------------------------------------------------------
													// 3
			vecA = pVtx[nVtxNumber[2]].pos - pVtx[nVtxNumber[3]].pos;	// ����
			vecB = pVtx[nVtxNumber[1]].pos - pVtx[nVtxNumber[3]].pos;	// �E��

			D3DXVec3Cross(&nor[1], &vecA, &vecB);	// �O�ς����߂�
			D3DXVec3Normalize(&nor[1], &nor[1]);	// ���K��

													// �e���_�̖@�����Z�o
			pVtx[nVtxNumber[0]].nor += nor[0];
			pVtx[nVtxNumber[1]].nor += nor[0] + nor[1];
			pVtx[nVtxNumber[2]].nor += nor[0] + nor[1];
			pVtx[nVtxNumber[3]].nor += nor[1];
		}
	}

	for (nCntMeshYZ = 0; nCntMeshYZ < m_nMeshZ - 1; nCntMeshYZ++)
	{
		for (nCntMeshX = 0; nCntMeshX < m_nMeshX - 1; nCntMeshX++)
		{// �@���̕��ς��o��
			D3DXVECTOR3 vecA;	// ���_�O���璸�_�P�ւ̕���
			D3DXVECTOR3 vecB;	// ���_�O���璸�_�Q�ւ̕���
			int nVtxNumber[4];	// �S���_�̔ԍ�

			nVtxNumber[0] = nCntMeshX + ((nCntMeshYZ + 1) * (m_nMeshX + 1));			// �u���b�N�̍����ԍ�
			nVtxNumber[1] = nCntMeshX + (nCntMeshYZ * (m_nMeshX + 1));					// �u���b�N�̍���ԍ�
			nVtxNumber[2] = (nCntMeshX + 1) + ((nCntMeshYZ + 1) * (m_nMeshX + 1));		// �u���b�N�̉E���ԍ�
			nVtxNumber[3] = (nCntMeshX + 1) + (nCntMeshYZ * (m_nMeshX + 1));			// �u���b�N�̉E��ԍ�

			if (nCntMeshX == 0 && nCntMeshYZ == 0)
			{// ����[
				pVtx[nVtxNumber[0]].nor /= 3;
				pVtx[nVtxNumber[1]].nor /= 2;
				pVtx[nVtxNumber[2]].nor /= 6;
				pVtx[nVtxNumber[3]].nor /= 3;
			}
			else if (nCntMeshX == 0 && nCntMeshYZ > 0 && nCntMeshYZ < m_nMeshZ - 1)
			{// ���[
				pVtx[nVtxNumber[0]].nor /= 3;
				pVtx[nVtxNumber[2]].nor /= 6;
			}
			else if (nCntMeshX == m_nMeshX - 1 && nCntMeshYZ > 0 && nCntMeshYZ < m_nMeshZ - 1)
			{
				pVtx[nVtxNumber[2]].nor /= 3;
			}
			else if (nCntMeshYZ == 0)
			{// ��[
				if (nCntMeshX == m_nMeshX - 1)
				{// �E��[
					pVtx[nVtxNumber[2]].nor /= 3;
				}
				else
				{// �㒆
					pVtx[nVtxNumber[2]].nor /= 6;
					pVtx[nVtxNumber[3]].nor /= 3;
				}
			}
			else if (nCntMeshYZ == m_nMeshZ - 1)
			{// ���[
				if (nCntMeshX == m_nMeshX - 1)
				{// �E���[
					pVtx[nVtxNumber[2]].nor /= 2;
				}
				else
				{// ����
					pVtx[nVtxNumber[2]].nor /= 3;
				}
			}
			else if (nCntMeshX > 0 && nCntMeshYZ > 0 && nCntMeshX < m_nMeshX - 1 && nCntMeshYZ < m_nMeshZ - 1)
			{// ��
				pVtx[nVtxNumber[2]].nor /= 6;
			}
		}
	}
}

//=========================================================================================================================
// ���_���̐ݒ�
//=========================================================================================================================
void CScene3DIndex::SetInfo(float fWidth, float fHeight, float fDepth, int nMeshX, int nMeshY, int nMeshZ)
{
	m_fLengthX = fWidth;
	m_fLengthY = fHeight;
	m_fLengthZ = fDepth;
	m_nMeshX = nMeshX;
	m_nMeshY = nMeshY;
	m_nMeshZ = nMeshZ;
}

//=========================================================================================================================
// ���_���̎擾
//=========================================================================================================================
void CScene3DIndex::GetInfo(float *pWidth, float *pHeight, float *pDepth, int *pMeshX, int *pMeshY, int *pMeshZ)
{
	if (pWidth != NULL) { *pWidth = m_fLengthX; }
	if (pHeight != NULL) { *pHeight = m_fLengthY; }
	if (pDepth != NULL) { *pDepth = m_fLengthZ; }
	if (pMeshX != NULL) { *pMeshX = m_nMeshX; }
	if (pMeshY != NULL) { *pMeshY = m_nMeshY; }
	if (pMeshZ != NULL) { *pMeshZ = m_nMeshZ; }
}