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
#include "debugProc.h"
#include "camera.h"

////*****************************************************************************
//// �}�N����`
////*****************************************************************************
//#define SHADOW_MODEL_FILE_000	"data/MODEL/shadow/shadow_000.x"
//#define SHADOW_MODEL_FILE_001	"data/MODEL/shadow/shadow_001.x"
//#define SHADOW_MODEL_FILE_002	"data/MODEL/shadow/shadow_002.x"
//#define SHADOW_MODEL_FILE_003	"data/MODEL/shadow/shadow_003.x"
//#define SHADOW_MODEL_FILE_004	"data/MODEL/shadow/shadow_004.x"
//#define SHADOW_MODEL_FILE_005	"data/MODEL/shadow/shadow_005.x"
//#define SHADOW_MODEL_FILE_006	"data/MODEL/shadow/shadow_006.x"
//#define SHADOW_MODEL_FILE_007	"data/MODEL/shadow/shadow_007.x"
//#define SHADOW_MODEL_FILE_008	"data/MODEL/shadow/shadow_008.x"
//#define SHADOW_MODEL_FILE_009	"data/MODEL/shadow/shadow_009.x"
//#define SHADOW_MODEL_FILE_010	"data/MODEL/shadow/shadow_010.x"
//#define SHADOW_MODEL_FILE_011	"data/MODEL/shadow/shadow_011.x"
//#define SHADOW_MODEL_FILE_012	"data/MODEL/shadow/shadow_012.x"
//#define SHADOW_MODEL_FILE_013	"data/MODEL/shadow/shadow_013.x"
//#define SHADOW_MODEL_FILE_014	"data/MODEL/shadow/shadow_014.x"
//#define SHADOW_MODEL_FILE_015	"data/MODEL/shadow/shadow_015.x"
//
////=============================================================================
//// �ÓI�����o�ϐ��錾
////=============================================================================
//
////==================================
//// ��������
////==================================
//CShadow *CShadow::Create(D3DXVECTOR3 pos, int nPriority, int nModel)
//{
//	CShadow *pShadow;
//
//	pShadow = new CShadow;	// ���������m��
//
//	if (pShadow != NULL)
//	{// �������m�ې���
//		pShadow->m_pos = pos;
//		pShadow->m_nModel = nModel;
//		pShadow->Init();
//		pShadow->SwapPriority(nPriority);
//	}
//
//	return pShadow;
//}
//
////=========================================
//// �R���X�g���N�^
////=========================================
//CShadow::CShadow(int nPriority, CScene::OBJTYPE objType) : CScene(nPriority, objType)
//{
//	m_pBigScreen = NULL;
//	m_pBuffMat = NULL;
//	m_pMesh = NULL;
//	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
//	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
//}
//
////=========================================
//// �f�X�g���N�^
////=========================================
//CShadow::~CShadow()
//{
//}
//
////=============================================================================
//// ����������
////=============================================================================
//HRESULT CShadow::Init(void)
//{
//	// �f�o�C�X�̎擾
//	CRenderer *pRenderer = CManager::GetRenderer();
//	LPDIRECT3DDEVICE9 pDevice;
//	pDevice = pRenderer->GetDevice();
//
//	char *FileName[16] = {
//		SHADOW_MODEL_FILE_001, SHADOW_MODEL_FILE_002, SHADOW_MODEL_FILE_003, SHADOW_MODEL_FILE_004,
//		SHADOW_MODEL_FILE_005, SHADOW_MODEL_FILE_006, SHADOW_MODEL_FILE_007, SHADOW_MODEL_FILE_008, SHADOW_MODEL_FILE_009,
//		SHADOW_MODEL_FILE_010, SHADOW_MODEL_FILE_011, SHADOW_MODEL_FILE_012, SHADOW_MODEL_FILE_013, SHADOW_MODEL_FILE_014,
//		SHADOW_MODEL_FILE_015, SHADOW_MODEL_FILE_000
//	};
//
//	// X�t�@�C���̓ǂݍ���
//	D3DXLoadMeshFromX(FileName[m_nModel],
//		D3DXMESH_SYSTEMMEM,
//		pDevice,
//		NULL,
//		&m_pBuffMat,
//		NULL,
//		&m_nNumMat,
//		&m_pMesh);
//
//	// 2D�|���S���̐���
//	m_pBigScreen = CPolygon::Create(1, D3DXVECTOR3(0.0f, 0.0f, 0.0f), SCREEN_WIDTH, SCREEN_HEIGHT);
//	m_pBigScreen->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.7f));
//	return S_OK;
//}
//
////=============================================================================
//// �I������
////=============================================================================
//void CShadow::Uninit(void)
//{
//	if (NULL != m_pBigScreen)
//	{// �j��
//		m_pBigScreen->Uninit();
//		delete m_pBigScreen;
//		m_pBigScreen = NULL;
//	}
//
//	if (NULL != m_pBuffMat)
//	{// �j��
//		m_pBuffMat->Release();
//		m_pBuffMat = NULL;
//	}
//
//	if (NULL != m_pMesh)
//	{// �j��
//		m_pMesh->Release();
//		m_pMesh = NULL;
//	}
//
//	// �I�u�W�F�N�g�̔j��
//	CScene::Release();
//}
//
////=============================================================================
//// �X�V����
////=============================================================================
//void CShadow::Update(void)
//{
//}
//
////=============================================================================
//// �`�揈��
////=============================================================================
//void CShadow::Draw(void)
//{
//	// �f�o�C�X�̎擾
//	CRenderer *pRenderer = CManager::GetRenderer();
//	LPDIRECT3DDEVICE9 pDevice;
//	pDevice = pRenderer->GetDevice();
//
//	D3DXMATRIX mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X
//
//	// ���[���h�}�g���b�N�X�̏�����
//	D3DXMatrixIdentity(&m_mtxWorld);
//
//	// ��]�𔽉f
//	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
//	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);
//
//	// �ړ��𔽉f
//	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
//	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);
//
//	// ���[���h�}�g���b�N�X�̐ݒ�
//	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);
//
//#if 1
//	// �[�x�o�b�t�@�ɏ������݂͂��Ȃ�
//	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
//
//	// �����_�����O�^�[�Q�b�g�ɏ������݂͂��Ȃ�
//	pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, FALSE);
//
//	// ���ʕ`��
//	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
//
//	// ���ʃX�e���V�����g�p����
//	pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);
//	pDevice->SetRenderState(D3DRS_TWOSIDEDSTENCILMODE, TRUE);
//
//	// �X�e���V���e�X�g�͏�ɍ��i����i���e�X�g���Ȃ��j
//	pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);
//	pDevice->SetRenderState(D3DRS_CCW_STENCILFUNC, D3DCMP_ALWAYS);
//
//	// �X�e���V���o�b�t�@�̑�����1�ɐݒ肷��
//	pDevice->SetRenderState(D3DRS_STENCILREF, 0x1);
//	pDevice->SetRenderState(D3DRS_STENCILMASK, 0xffffffff);
//	pDevice->SetRenderState(D3DRS_STENCILWRITEMASK, 0xffffffff);
//
//	// �\�ʂ͐[�x�e�X�g�ɍ��i������X�e���V���o�b�t�@�̓��e��+1����
//	pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_INCR);
//	pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);
//	pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);
//
//	// ���ʂ͐[�x�e�X�g�ɍ��i������X�e���V���o�b�t�@�̓��e��-1����
//	pDevice->SetRenderState(D3DRS_CCW_STENCILPASS, D3DSTENCILOP_DECR);
//	pDevice->SetRenderState(D3DRS_CCW_STENCILZFAIL, D3DSTENCILOP_KEEP);
//	pDevice->SetRenderState(D3DRS_CCW_STENCILFAIL, D3DSTENCILOP_KEEP);
//
//	for (int nCntMat = 0; nCntMat < (int)m_nNumMat; nCntMat++)
//	{// ���f���̕`��
//		m_pMesh->DrawSubset(nCntMat);
//	}
//
//	// ��Ԃ����ɖ߂�
//	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
//	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
//	pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0xf);
//	pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);
//	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
//	pDevice->SetRenderState(D3DRS_TWOSIDEDSTENCILMODE, FALSE);
//
//	// �[�x�e�X�g�͂��Ȃ�
//	pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
//
//	// �X�e���V���e�X�g�͂���
//	pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);
//
//	// �A���t�@�u�����f�B���O�͐��`�Ɋ|����
//	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
//	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
//	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
//
//	// �X�e���V���o�b�t�@�̒l���P�ȏ�̂Ƃ��ɏ�������
//	pDevice->SetRenderState(D3DRS_STENCILREF, 0x1);
//	pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);
//	pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);
//
//	// ��ʂ������h��Ԃ�
//	m_pBigScreen->Draw();
//
//	// ��Ԃ����ɖ߂�
//	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
//	pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);
//	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
//
//
//
//
//
//	// �[�x�o�b�t�@�ɏ������݂͂��Ȃ�
//	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
//
//	// �����_�����O�^�[�Q�b�g�ɏ������݂͂��Ȃ�
//	pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, FALSE);
//
//	// ���ʕ`��
//	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
//
//	// ���ʃX�e���V�����g�p����
//	pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);
//	pDevice->SetRenderState(D3DRS_TWOSIDEDSTENCILMODE, TRUE);
//
//	// �X�e���V���e�X�g�͏�ɍ��i����i���e�X�g���Ȃ��j
//	pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);
//	pDevice->SetRenderState(D3DRS_CCW_STENCILFUNC, D3DCMP_ALWAYS);
//
//	// �X�e���V���o�b�t�@�̑�����1�ɐݒ肷��
//	pDevice->SetRenderState(D3DRS_STENCILREF, 0x1);
//	pDevice->SetRenderState(D3DRS_STENCILMASK, 0xffffffff);
//	pDevice->SetRenderState(D3DRS_STENCILWRITEMASK, 0xffffffff);
//
//	// �\�ʂ͐[�x�e�X�g�ɍ��i������X�e���V���o�b�t�@�̓��e��+1����
//	pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_INCR);
//	pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_DECR);
//	pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);
//
//	// ���ʂ͐[�x�e�X�g�ɍ��i������X�e���V���o�b�t�@�̓��e��-1����
//	pDevice->SetRenderState(D3DRS_CCW_STENCILPASS, D3DSTENCILOP_DECR);
//	pDevice->SetRenderState(D3DRS_CCW_STENCILZFAIL, D3DSTENCILOP_INCR);
//	pDevice->SetRenderState(D3DRS_CCW_STENCILFAIL, D3DSTENCILOP_KEEP);
//
//	for (int nCntMat = 0; nCntMat < (int)m_nNumMat; nCntMat++)
//	{// ���f���̕`��
//		m_pMesh->DrawSubset(nCntMat);
//	}
//
//	// ��Ԃ����ɖ߂�
//	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
//	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
//	pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0xf);
//	pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);
//	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
//	pDevice->SetRenderState(D3DRS_TWOSIDEDSTENCILMODE, FALSE);
//
//	// �[�x�e�X�g�͂��Ȃ�
//	pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
//
//	// �X�e���V���e�X�g�͂���
//	pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);
//
//	// �A���t�@�u�����f�B���O�͐��`�Ɋ|����
//	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
//	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
//	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
//
//	// �X�e���V���o�b�t�@�̒l���P�ȏ�̂Ƃ��ɏ�������
//	pDevice->SetRenderState(D3DRS_STENCILREF, 0x1);
//	pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);
//	pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);
//
//	// ��ʂ������h��Ԃ�
//	m_pBigScreen->Draw();
//
//	// ��Ԃ����ɖ߂�
//	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
//	pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);
//	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
//
//#endif
//
//#if 0
//	// �X�e���V���o�b�t�@�e�X�g�̐ݒ�
//	pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);						// �X�e���V���o�b�t�@��L���ɂ���
//	pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0x00000000);			// �`������Ȃ��悤�ɂ���
//	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);						// Z�o�b�t�@�ɏ������܂Ȃ�
//
//	pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);				// �X�e���V���o�b�t�@�e�X�g����ɒʂ�
//	pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_ZERO);			// �X�e���V���e�X�g�AZ�e�X�g���ɍ��i�����ꍇ
//	pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_INCR);			// �X�e���V���e�X�g�ɍ��i�AZ�e�X�g�ɕs���i�����ꍇ
//	pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_ZERO);			// �X�e���V���e�X�g�ɕs���i�����ꍇ
//
//	//pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
//
//	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);					// �\�ʂ��J�����O
//
//	for (int nCntMat = 0; nCntMat < (int)m_nNumMat; nCntMat++)
//	{// ���f���̕`��
//		m_pMesh->DrawSubset(nCntMat);
//	}
//
//	// �X�e���V���o�b�t�@�e�X�g�̐ݒ�
//	pDevice->SetRenderState(D3DRS_STENCILREF, 0x01);						// �ΏۂƂ��鐔�l
//	pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);						// �X�e���V���o�b�t�@��L���ɂ���
//	pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);				// �X�e���V���o�b�t�@�e�X�g����ɒʂ�
//	pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_INCR);			// �X�e���V���e�X�g�AZ�e�X�g���ɍ��i�����ꍇ
//	pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_ZERO);			// �X�e���V���e�X�g�ɍ��i�AZ�e�X�g�ɕs���i�����ꍇ
//	pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_ZERO);			// �X�e���V���e�X�g�ɕs���i�����ꍇ
//
//	// �X�e���V���o�b�t�@�e�X�g�̐ݒ�
//	pDevice->SetRenderState(D3DRS_STENCILREF, 0x01);						// �ΏۂƂ��鐔�l
//	pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);						// �X�e���V���o�b�t�@��L���ɂ���
//	pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_LESSEQUAL);			// �X�e���V���o�b�t�@�e�X�g��ʂ�����
//	pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_INCR);			// �X�e���V���e�X�g�AZ�e�X�g���ɍ��i�����ꍇ
//	pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_ZERO);			// �X�e���V���e�X�g�ɍ��i�AZ�e�X�g�ɕs���i�����ꍇ
//	pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_ZERO);			// �X�e���V���e�X�g�ɕs���i�����ꍇ
//
//	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);					// ���ʂ��J�����O
//
//	for (int nCntMat = 0; nCntMat < (int)m_nNumMat; nCntMat++)
//	{// ���f���̕`��
//		m_pMesh->DrawSubset(nCntMat);
//	}
//
//	pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0x0000000f);			// ���ɖ߂�
//
//	// �X�e���V���o�b�t�@�e�X�g�̐ݒ�
//	pDevice->SetRenderState(D3DRS_STENCILREF, 0x01);						// �ΏۂƂ��鐔�l
//	pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);						// �X�e���V���o�b�t�@��L���ɂ���
//	pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);				// �X�e���V���o�b�t�@�e�X�g�������Ƃ��ɒʂ�
//	pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);			// �X�e���V���e�X�g�AZ�e�X�g���ɍ��i�����ꍇ
//	pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);			// �X�e���V���e�X�g�ɍ��i�AZ�e�X�g�ɕs���i�����ꍇ
//	pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);			// �X�e���V���e�X�g�ɕs���i�����ꍇ
//	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);						// Z�o�b�t�@�ɏ������߂�悤�ɂ���
//	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
//
//	// ��ʂ������h��Ԃ�
//	m_pBigScreen->Draw();
//
//	// �X�e���V���o�b�t�@�e�X�g�̐ݒ�
//	pDevice->SetRenderState(D3DRS_STENCILREF, 0x02);						// �ΏۂƂ��鐔�l
//	pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);						// �X�e���V���o�b�t�@��L���ɂ���
//	pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);				// �X�e���V���o�b�t�@�e�X�g�������Ƃ��ɒʂ�
//	pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);			// �X�e���V���e�X�g�AZ�e�X�g���ɍ��i�����ꍇ
//	pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);			// �X�e���V���e�X�g�ɍ��i�AZ�e�X�g�ɕs���i�����ꍇ
//	pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);			// �X�e���V���e�X�g�ɕs���i�����ꍇ
//	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);						// Z�o�b�t�@�ɏ������߂�悤�ɂ���
//	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
//
//	// ��ʂ������h��Ԃ�
//	m_pBigScreen->Draw();
//
//	// �X�e���V���o�b�t�@�e�X�g�̐ݒ�
//	pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);					// �X�e���V���o�b�t�@�����ɂ���
//#endif
//}

//=========================================
// �R���X�g���N�^
//=========================================
CShadow *CShadow::Create(void)
{
	CShadow *pShadow = NULL;
	pShadow = new CShadow;

	if (NULL != pShadow)
	{// �������m�ې���
		pShadow->Init();
	}

	return pShadow;
}

//=========================================
// �R���X�g���N�^
//=========================================
CShadow::CShadow(int nPriority, CScene::OBJTYPE objType) : CScene(nPriority, objType)
{
	m_pBuffMat = NULL;
	m_pMesh = NULL;
	m_cpShadowMapTex = NULL;
	m_nNumMat = 0;
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
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
	// Direct3D�̏�����
	LPDIRECT3D9 g_pD3D;
	LPDIRECT3DDEVICE9 g_pD3DDev;
	if (!(g_pD3D = Direct3DCreate9(D3D_SDK_VERSION))) return 0;
	IKD::Com_ptr<IDirect3D9> spD3D(g_pD3D);

	D3DPRESENT_PARAMETERS d3dpp = { 0,0,D3DFMT_UNKNOWN,0,D3DMULTISAMPLE_NONE,0,
		D3DSWAPEFFECT_DISCARD,NULL,TRUE,TRUE,D3DFMT_D16,0,D3DPRESENT_RATE_DEFAULT,D3DPRESENT_INTERVAL_DEFAULT };

	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, CManager::GetRenderer()->GetHWnd(), D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &g_pD3DDev)))
		if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, CManager::GetRenderer()->GetHWnd(), D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &g_pD3DDev)))
			return 0;
	IKD::Com_ptr<IDirect3DDevice9> cpDev(g_pD3DDev);

	/////////////////////////////////////////////////////
	// Z�l�e�N�X�`�������I�u�W�F�N�g�̐����Ə�����
	/////////////
	UINT TexSize = 1024;	// �e�N�X�`���T�C�Y
	if (!m_ZTexCreator.Init(cpDev, TexSize, TexSize, D3DFMT_A8R8G8B8))	// �ʏ�͂��̃e�N�X�`�����쐬�ł��܂�
		if (!m_ZTexCreator.Init(cpDev, TexSize, TexSize, D3DFMT_R5G6B5))		// �������16bit�ɗ��Ƃ��܂�
			return 0;
	m_ZTexCreator.GetZTex(m_cpShadowMapTex);

	/////////////////////////////////////////////////////
	// �[�x�o�b�t�@�V���h�E�I�u�W�F�N�g�̐����Ə�����
	/////////////
	m_DepthBS.Init(cpDev);
	m_DepthBS.SetShadowMap(m_cpShadowMapTex);	// �V���h�E�}�b�v�e�N�X�`����o�^

	// �e�̃��f������
	D3DXLoadMeshFromX("data/MODEL/map_UV_bill.x", D3DXMESH_MANAGED, cpDev.GetPtr(), NULL, m_pBuffMat.ToCreator(), NULL, &m_nNumMat, m_pMesh.ToCreator());

	D3DXMATRIX LightView, LightProj;	// ���C�g�r���[�ϊ��E�ˉe�ϊ�
	float LightScale = 1.5f;

	D3DXMatrixPerspectiveFovLH(&LightProj, D3DXToRadian(40), 1.0f, 40.0f, 300.0f);
	D3DXMatrixLookAtLH(&LightView, &D3DXVECTOR3(LightScale * 100, LightScale * 55, LightScale * 100), &D3DXVECTOR3(0, -20, 0), &D3DXVECTOR3(0, 1, 0));

	// Z�l�e�N�X�`��OBJ�֓o�^
	m_ZTexCreator.SetViewMatrix(&LightView);
	m_ZTexCreator.SetProjMatrix(&LightProj);

	// �[�x�o�b�t�@�V���h�EOBJ�֓o�^
	m_DepthBS.SetLightViewMatrix(&LightView);
	m_DepthBS.SetLightProjMatrix(&LightProj);
	m_DepthBS.SetCameraProjMatrix(&CManager::GetCamera()->GetProjection());

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CShadow::Uninit(void)
{
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
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	///////////////////////////////////////
	//���p�X1 : Z�l�e�N�X�`���Ƀ��C�g��������`��
	m_ZTexCreator.Begin();	// �V�F�[�_�J�n

	m_ZTexCreator.SetWorldMatrix(&m_mtxWorld);	// �����̂̃��[���h�ϊ��s���o�^

	for (int nCntMat = 0; nCntMat < (int)m_nNumMat; nCntMat++)
	{
		m_ZTexCreator.SetParamToEffect();		// �`��̒��O�ɌĂяo���K�v������܂�
		m_ZTexCreator.BeginPass();
		m_pMesh->DrawSubset(nCntMat);			// ���b�V���`��
		m_ZTexCreator.EndPass();
	}

	m_ZTexCreator.End();	// �V�F�[�_�I��

	///////////////////////////////////////
	//���p�X2 : �[�x�o�b�t�@�V���h�E�`��
	m_DepthBS.SetCameraViewMatrix(&CManager::GetCamera()->GetView());
	m_DepthBS.Begin();
	m_DepthBS.SetWorldMatrix(&m_mtxWorld);
	for (int nCntMat = 0; nCntMat < (int)m_nNumMat; nCntMat++)
	{
		m_DepthBS.SetParamToEffect();		// �`��̒��O�ɌĂяo���K�v������܂�
		m_DepthBS.BeginPass();
		m_pMesh->DrawSubset(nCntMat);		// ���b�V���`��
		m_DepthBS.EndPass();
	}
	m_DepthBS.End();
}