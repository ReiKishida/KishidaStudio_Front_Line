//=============================================================================
//
// 3D���f���I�u�W�F�N�g���� [sceneX.cpp]
// Author : Ishida Takuto
//
//=============================================================================
#include "sceneX.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "debugProc.h"

//==================================
// �}�N����`
//==================================

//==================================
// �ÓI�����o�ϐ��錾
//==================================

//=========================================
// ���f���������蓖�Ă�
//=========================================
const void CSceneX::BindInfo(LPDIRECT3DTEXTURE9 pTexture, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat)
{
	m_pTexture = pTexture;
	m_pMesh = pMesh;
	m_pBuffMat = pBuffMat;
	m_nNumMat = nNumMat;
}

//=========================================
// �R���X�g���N�^
//=========================================
CSceneX::CSceneX(int nPriority, CScene::OBJTYPE objType) : CScene(nPriority, objType)
{
	m_pTexture = NULL;		// �e�N�X�`���ւ̃|�C���^
	m_pBuffMat = NULL;		// �}�e���A���ւ̃|�C���^
	m_pMesh = NULL;
	m_vtxMin = D3DXVECTOR3(10000.0f, 10000.0f, 10000.0f);
	m_vtxMax = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �|���S���̈ʒu
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �|���S���̌���
}

//=========================================
// �f�X�g���N�^
//=========================================
CSceneX::~CSceneX()
{
}

//=========================================
// �|���S���̏���������
//=========================================
HRESULT CSceneX::Init(void)
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
// �|���S���̏I������
//=========================================
void CSceneX::Uninit(void)
{
	// �����̔j��
	Release();
}

//=========================================
// �|���S���̍X�V����
//=========================================
void CSceneX::Update(void)
{
}

//=========================================
// �|���S���̕`�揈��
//=========================================
void CSceneX::Draw(void)
{
	// �f�o�C�X�̎擾
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;			// ���݂̃}�e���A���ۑ��p

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// �ړ��𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	// �}�e���A���f�[�^�ւ̃|�C���^���擾
	m_pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_nNumMat; nCntMat++)
	{
		// �}�e���A���̐ݒ�
		pDevice->SetMaterial(&m_pMat[nCntMat].MatD3D);

		if (m_pMat[nCntMat].pTextureFilename != NULL)
		{// �I�u�W�F�N�g�̃e�N�X�`���[���X�e�[�W�O�ɃZ�b�g����
			pDevice->SetTexture(0, m_pTexture);
		}
		else
		{// �e�N�X�`�����Ȃ��Ƃ�
			pDevice->SetTexture(0, NULL);
		}

		// ���f���̕`��
		m_pMesh->DrawSubset(nCntMat);
	}

	// �}�e���A�����f�t�H���g�ɖ߂�
	pDevice->SetMaterial(&matDef);
}

//=========================================
// �l�p�`�̓����蔻��
//=========================================
int CSceneX::CollisionBox(D3DXVECTOR3 *pos, D3DXVECTOR3 posOld, D3DXVECTOR3 rot)
{
	int nCollision = 0;
	if (m_pos.x + m_vtxMax.x >= pos->x && m_pos.x + m_vtxMin.x <= pos->x &&
		m_pos.z + m_vtxMax.z >= pos->z && m_pos.z + m_vtxMin.z <= pos->z)
	{// x��z�����͈͓�
		if (m_pos.y + m_vtxMax.y >= pos->y && m_pos.y + m_vtxMin.y <= posOld.y ||
			m_pos.y + m_vtxMin.y >= pos->y && m_pos.y + m_vtxMax.y <= posOld.y)
		{// ��������
			pos->y = m_pos.y + m_vtxMax.y;	// ���������f���ɍ��킹��

			nCollision = 1;
		}
		else
		{// �������Ƃ�
			nCollision = 2;
		}
	}

	return nCollision;
}

//=========================================
// ���[���h�}�g���b�N�X�̐ݒ�
//=========================================
D3DXMATRIX CSceneX::SetWorldMatrix(D3DXMATRIX *mtxWorld)
{
	// �f�o�C�X�̎擾
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// �ړ��𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	D3DXMatrixMultiply(mtxWorld, mtxWorld, &m_mtxWorld);

	return *mtxWorld;
}

//=========================================
// �F�̐ݒ�
//=========================================
const void CSceneX::SetColor(D3DXCOLOR col)
{
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	D3DMATERIAL9 matDef;			// ���݂̃}�e���A���ۑ��p

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	// �}�e���A���f�[�^�ւ̃|�C���^���擾
	m_pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	// �F��t����
	m_pMat->MatD3D.Diffuse = col;
	m_pMat->MatD3D.Ambient = col;

	// �}�e���A�����f�t�H���g�ɖ߂�
	pDevice->SetMaterial(&matDef);
}

//=========================================
// �F�̉��Z
//=========================================
const void CSceneX::AddColor(D3DXCOLOR col)
{
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	D3DMATERIAL9 matDef;			// ���݂̃}�e���A���ۑ��p
									// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	// �}�e���A���f�[�^�ւ̃|�C���^���擾
	m_pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	// �F��t����
	m_pMat->MatD3D.Diffuse.r += col.r;
	m_pMat->MatD3D.Diffuse.g += col.g;
	m_pMat->MatD3D.Diffuse.b += col.b;
	m_pMat->MatD3D.Diffuse.a += col.a;

	m_pMat->MatD3D.Ambient.r += col.r;
	m_pMat->MatD3D.Ambient.g += col.g;
	m_pMat->MatD3D.Ambient.b += col.b;
	m_pMat->MatD3D.Ambient.a += col.a;

	// �}�e���A�����f�t�H���g�ɖ߂�
	pDevice->SetMaterial(&matDef);
}
