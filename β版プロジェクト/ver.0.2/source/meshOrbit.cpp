//=============================================================================
//
// �O�Տ��� [meshOrbit.cpp]
// Author : Takuto Ishida
//
//=============================================================================
#include "meshOrbit.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "debugProc.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_NAME00	"data/TEXTURE/gradation004.jpg"
#define ORBIT_POLYGON	(MAX_ORBIT_VERTEX - 2)	// �|���S����

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================
LPDIRECT3DTEXTURE9 CMeshOrbit::m_pTexture[FIELD_TEXTURE] = {};

//==================================
// ��������
//==================================
CMeshOrbit* CMeshOrbit::Create(D3DXVECTOR3 offset, int nLife, D3DXMATRIX *pMtx)
{
	CMeshOrbit *pOrbit;

	pOrbit = new CMeshOrbit;		// ���������m��

	if (pOrbit != NULL)
	{// �������m�ې���
		pOrbit->m_aOffset[1] = offset;	// �I�t�Z�b�g
		pOrbit->m_nLife = nLife;		// ����
		pOrbit->m_pMtxParent = pMtx;	// �e�̃}�g���b�N�X
		pOrbit->Init();					// ����������
	}

	return pOrbit;
}

//=========================================
// �e�N�X�`���̓ǂݍ���
//=========================================
HRESULT CMeshOrbit::Load(void)
{
	// �f�o�C�X�̎擾
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME00, &m_pTexture[0]);

	return S_OK;
}

//=========================================
// �ǂݍ��񂾃e�N�X�`���̉��
//=========================================
void CMeshOrbit::Unload(void)
{
	// �e�N�X�`���̔j��
	for (int nCntTexture = 0; nCntTexture < FIELD_TEXTURE; nCntTexture++)
	{
		if (NULL != m_pTexture[nCntTexture])
		{
			m_pTexture[nCntTexture]->Release();
			m_pTexture[nCntTexture] = NULL;
		}
	}
}

//=========================================
// �R���X�g���N�^
//=========================================
CMeshOrbit::CMeshOrbit(int nPriority, CScene::OBJTYPE objType) : CScene(nPriority, objType)
{
	for (int nCntOrbit = 0; nCntOrbit < MAX_ORBIT_VERTEX; nCntOrbit++)
	{// ���_���W�̌v�Z�p
		m_aPosVertex[nCntOrbit] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	for (int nCntOffset = 0; nCntOffset < 2; nCntOffset++)
	{// �I�t�Z�b�g
		m_aOffset[nCntOffset] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	m_nLife = 0;
	m_pMtxParent = NULL;
	m_pVtxBuff = NULL;
}

//=========================================
// �f�X�g���N�^
//=========================================
CMeshOrbit::~CMeshOrbit()
{
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CMeshOrbit::Init(void)
{
	// �f�o�C�X�̎擾
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME00, &m_pTexture[0]);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * MAX_ORBIT_VERTEX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// ���_����ݒ�
	VERTEX_3D *pVtx;	// ���_���̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	if (NULL != m_pMtxParent)
	{// �e������Ƃ�
		for (int nCntOrbit = 0; nCntOrbit < MAX_ORBIT_VERTEX / 2; nCntOrbit++)
		{
			// ���_���W
			pVtx[0 + (nCntOrbit * 2)].pos = D3DXVECTOR3(m_pMtxParent->_41, m_pMtxParent->_42, m_pMtxParent->_43);
			pVtx[1 + (nCntOrbit * 2)].pos = D3DXVECTOR3(m_pMtxParent->_41, m_pMtxParent->_42, m_pMtxParent->_43) + m_aOffset[1];

			// �@���̏�����
			pVtx[0 + (nCntOrbit * 2)].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[1 + (nCntOrbit * 2)].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			// ���_�J���[
			pVtx[0 + (nCntOrbit * 2)].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f);
			pVtx[1 + (nCntOrbit * 2)].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f);

			//�e�N�X�`�����W
			pVtx[0 + (nCntOrbit * 2)].tex = D3DXVECTOR2((float)nCntOrbit * (1 / (MAX_ORBIT_VERTEX / 2)), 1.0f);
			pVtx[1 + (nCntOrbit * 2)].tex = D3DXVECTOR2((float)nCntOrbit * (1 / (MAX_ORBIT_VERTEX / 2)), 0.0f);

			// ���_���W�̌v�Z�p
			m_aPosVertex[0 + (nCntOrbit * 2)] = D3DXVECTOR3(m_pMtxParent->_41, m_pMtxParent->_42, m_pMtxParent->_43);
			m_aPosVertex[1 + (nCntOrbit * 2)] = D3DXVECTOR3(m_pMtxParent->_41, m_pMtxParent->_42, m_pMtxParent->_43) + m_aOffset[1];
		}
	}

	// ���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CMeshOrbit::Uninit(void)
{
	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CMeshOrbit::Update(void)
{
	if (m_nLife >= 0)
	{// �����������Ȃ���
		for (int nCntOrbit = MAX_ORBIT_VERTEX - 1; nCntOrbit >= 2; nCntOrbit--)
		{// ���_���W�̓���ւ�
			m_aPosVertex[nCntOrbit] = m_aPosVertex[nCntOrbit - 2];
		}
	}

	// ���_����ݒ�
	VERTEX_3D *pVtx;	// ���_���̃|�C���^
	D3DXCOLOR col;

	// ���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntOrbit = 0; nCntOrbit < MAX_ORBIT_VERTEX; nCntOrbit++)
	{
		// ���_���W
		pVtx[nCntOrbit].pos = m_aPosVertex[nCntOrbit];
		col = pVtx[nCntOrbit].col;
		col -= D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.01f);
		if (col.a <= 0.0f) { col.a = 0.0f; };
		pVtx[nCntOrbit].col = col;
	}

	// ���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();

	m_nLife--;	// ���������炷

	if (col.a <= 0)
	{// �����������Ȃ���
		Uninit();
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CMeshOrbit::Draw(void)
{
	// �f�o�C�X�̎擾
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	// �J�����O�����Ȃ�
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	// ���e�X�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// ���u�����f�B���O�����Z�����ɐݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	if (m_nLife >= 0)
	{// ����������
		VERTEX_3D *pVtx;	// ���_���̃|�C���^

		// ���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// �I�t�Z�b�g�Ɛe�̃}�g���b�N�X���|�����킹��
		D3DXVec3TransformCoord(&m_aPosVertex[0], &m_aOffset[0], m_pMtxParent);
		pVtx[0].pos = m_aPosVertex[0];

		// �I�t�Z�b�g�Ɛe�̃}�g���b�N�X���|�����킹��
		D3DXVec3TransformCoord(&m_aPosVertex[1], &m_aOffset[1], m_pMtxParent);
		pVtx[1].pos = m_aPosVertex[1];

		// ���_�o�b�t�@���A�����b�N
		m_pVtxBuff->Unlock();
	}

	// ���C�g����
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, m_pTexture[0]);

	// �`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, ORBIT_POLYGON);

	// ���C�g�L��
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// ���e�X�g�̐ݒ��߂�
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// ���u�����f�B���O�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// ���ʂ��J�����O
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}
