//=============================================================================
//
// �e���� [shadow.cpp]
// Author : Takuto Ishida
//
//=============================================================================
#include "shadow.h"
#include "manager.h"
#include "renderer.h"
#include "light.h"
#include "meshField.h"
#include "scene.h"
#include "polygon.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define SHADOW_MODEL_FILE	"data/MODEL/shadow000.x"

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================

//==================================
// ��������
//==================================
CShadow *CShadow::Create(D3DXVECTOR3 *pPos)
{
	CShadow *pShadow;

	pShadow = new CShadow;	// ���������m��

	if (pShadow != NULL)
	{// �������m�ې���
		pShadow->m_pos = pPos;
		pShadow->Init();
	}

	return pShadow;
}

//=========================================
// �R���X�g���N�^
//=========================================
CShadow::CShadow(int nPriority, CScene::OBJTYPE objType) : CScene(nPriority, objType)
{
	m_pBigScreen = NULL;
	m_pBuffMat = NULL;
	m_pMesh = NULL;
}

//=========================================
// �f�X�g���N�^
//=========================================
CShadow::~CShadow()
{
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CShadow::Init(void)
{
	// �f�o�C�X�̎擾
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	// X�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX(SHADOW_MODEL_FILE,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_pBuffMat,
		NULL,
		&m_nNumMat,
		&m_pMesh);

	// 2D�|���S���̐���
	m_pBigScreen = CPolygon::Create(1,D3DXVECTOR3(0.0f,0.0f,0.0f),SCREEN_WIDTH,SCREEN_HEIGHT);
	m_pBigScreen->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.8f));
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CShadow::Uninit(void)
{
	if (NULL != m_pBigScreen)
	{// �j��
		m_pBigScreen->Uninit();
		delete m_pBigScreen;
		m_pBigScreen = NULL;
	}

	if (NULL != m_pBuffMat)
	{// �j��
		m_pBuffMat->Release();
		m_pBuffMat = NULL;
	}

	if (NULL != m_pMesh)
	{// �j��
		m_pMesh->Release();
		m_pMesh = NULL;
	}

	// �I�u�W�F�N�g�̔j��
	CScene::Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CShadow::Update(void)
{
}

//=============================================================================
// �`�揈��
//=============================================================================
void CShadow::Draw(void)
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
	D3DXMatrixTranslation(&mtxTrans, m_pos->x, m_pos->y, m_pos->z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// �X�e���V���o�b�t�@�e�X�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);						// �X�e���V���o�b�t�@��L���ɂ���
	pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0x00000000);			// �`������Ȃ��悤�ɂ���
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);						// Z�o�b�t�@�ɏ������܂Ȃ�

	pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);				// �X�e���V���o�b�t�@�e�X�g����ɒʂ�
	pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_ZERO);			// �X�e���V���e�X�g�AZ�e�X�g���ɍ��i�����ꍇ
	pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_INCR);			// �X�e���V���e�X�g�ɍ��i�AZ�e�X�g�ɕs���i�����ꍇ
	pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_ZERO);			// �X�e���V���e�X�g�ɕs���i�����ꍇ

	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);					// �\�ʂ��J�����O

	for (int nCntMat = 0; nCntMat < (int)m_nNumMat; nCntMat++)
	{// ���f���̕`��
		m_pMesh->DrawSubset(nCntMat);
	}

	// �X�e���V���o�b�t�@�e�X�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_STENCILREF, 0x01);						// �ΏۂƂ��鐔�l
	pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);						// �X�e���V���o�b�t�@��L���ɂ���
	pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);				// �X�e���V���o�b�t�@�e�X�g����ɒʂ�
	pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_INCR);			// �X�e���V���e�X�g�AZ�e�X�g���ɍ��i�����ꍇ
	pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_ZERO);			// �X�e���V���e�X�g�ɍ��i�AZ�e�X�g�ɕs���i�����ꍇ
	pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_ZERO);			// �X�e���V���e�X�g�ɕs���i�����ꍇ

	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);					// ���ʂ��J�����O

	for (int nCntMat = 0; nCntMat < (int)m_nNumMat; nCntMat++)
	{// ���f���̕`��
		m_pMesh->DrawSubset(nCntMat);
	}

	pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0x0000000f);			// ���ɖ߂�

	// �X�e���V���o�b�t�@�e�X�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_STENCILREF, 0x02);						// �ΏۂƂ��鐔�l
	pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);						// �X�e���V���o�b�t�@��L���ɂ���
	pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);				// �X�e���V���o�b�t�@�e�X�g�������Ƃ��ɒʂ�
	pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);			// �X�e���V���e�X�g�AZ�e�X�g���ɍ��i�����ꍇ
	pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);			// �X�e���V���e�X�g�ɍ��i�AZ�e�X�g�ɕs���i�����ꍇ
	pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);			// �X�e���V���e�X�g�ɕs���i�����ꍇ
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);						// Z�o�b�t�@�ɏ������߂�悤�ɂ���

	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);						// Z�o�b�t�@�ɏ������߂�悤�ɂ���
	// ��ʂ������h��Ԃ�
	m_pBigScreen->Draw();


	// �X�e���V���o�b�t�@�e�X�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);					// �X�e���V���o�b�t�@�����ɂ���
}
