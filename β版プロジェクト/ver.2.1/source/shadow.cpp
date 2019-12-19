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
#include "debugProc.h"
#include "line.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================

//==================================
// ��������
//==================================
CShadow *CShadow::Create(CScene *pParent)
{
	CShadow *pShadow;

	pShadow = new CShadow;	// ���������m��

	if (pShadow != NULL)
	{// �������m�ې���
		pShadow->m_pParent = pParent;
		pShadow->Init();
	}

	return pShadow;
}

//=========================================
// �R���X�g���N�^
//=========================================
CShadow::CShadow(int nPriority, CScene::OBJTYPE objType) : CScene(nPriority, objType)
{
	m_pParent = NULL;
	m_ObjPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
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
}

//=============================================================================
// ���f���ɉe������
//=============================================================================
void CShadow::Set(D3DXMATRIX mtxWorld, LPD3DXMESH mesh, int nNumMat, D3DXVECTOR3 _pos, float fHeight)
{
	m_ObjPos = _pos;

	// �f�o�C�X�̎擾
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	D3DXVECTOR4 vecLight;		// ���C�g�̃x�N�g��
	D3DXVECTOR3 pos, normal;	// �ʒu�Ɩ@��

	// ���Ȃ��悤�ɂ���
	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

	// ���u�����f�B���O�����Z�����ɐݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// ���C�e�B���O�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// �V���h�E�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxShadow);

	// ���C�g�̃x�N�g���̐ݒ�
	D3DXVECTOR3 vecDir = CLight::GetDirection();
	vecLight = D3DXVECTOR4(-vecDir.x, -vecDir.y, -vecDir.z, 0.0f);

	// �e�̈ʒu
	pos = D3DXVECTOR3(0.0f, fHeight, 0.0f);

	// �e�̖@��
	normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	// �_�Ɩ@�����畽�ʂ��쐬����
	D3DXPlaneFromPointNormal(&m_planeField, &pos, &normal);

	// ���ʂɎˉe����}�g���b�N�X���쐬
	D3DXMatrixShadow(&m_mtxShadow, &vecLight, &m_planeField);

	// ���[���h�}�g���b�N�X�Ɗ|�����킹��
	D3DXMatrixMultiply(&m_mtxShadow, &mtxWorld, &m_mtxShadow);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxShadow);

	// �e�N�X�`����NULL�ɂ���
	pDevice->SetTexture(0, NULL);

	for (int nCntMat = 0; nCntMat < nNumMat; nCntMat++)
	{// �e�̕`��
		mesh->DrawSubset(nCntMat);
	}

	// ���C�e�B���O��L���ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// ���u�����f�B���O�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// ���Ȃ��悤�ɂ���
	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
}

//=============================================================================
// �e�͈͓̔����ǂ���
//=============================================================================
bool CShadow::RangeShadow(D3DXVECTOR3 pos, CScene *pObject, D3DXVECTOR3 vtxDiff)
{
	// ���f���I�u�W�F�N�g��T��
	CScene *pScene = CScene::GetSceneTop(SHADOW_PRIORITY);

	while (pScene != NULL)
	{// NULL�ɂȂ�܂Ń��[�v
		CScene *pSceneNext = pScene->GetSceneNext();
		CScene::OBJTYPE objType = pScene->GetObjType();

		if (CScene::OBJTYPE_SHADOW == objType)
		{// �����蔻��̏ꍇ
			CShadow *pShadow = (CShadow*)pScene;
			if (pShadow->m_pParent != pObject)
			{// �֐����Ăяo�����I�u�W�F�N�g�ȊO
				D3DXVECTOR3 vecLight = -CManager::GetLight()->GetDirection();
				float fAngle = acosf(vecLight.x / sqrtf(vecLight.x * vecLight.x + vecLight.z * vecLight.z));
				if (vecLight.z < 0) { fAngle = D3DX_PI - fAngle; }

				// ����p���_
				D3DXVECTOR3 vtx0 = D3DXVECTOR3(pShadow->m_ObjPos.x - vtxDiff.x, 0, pShadow->m_ObjPos.z);
				D3DXVECTOR3 vtx1 = D3DXVECTOR3(pShadow->m_ObjPos.x,				0, pShadow->m_ObjPos.z);
				D3DXVECTOR3 vtx2 = D3DXVECTOR3(pShadow->m_ObjPos.x - vtxDiff.x, 0, pShadow->m_ObjPos.z - vtxDiff.z);
				D3DXVECTOR3 vtx3 = D3DXVECTOR3(pShadow->m_ObjPos.x,				0, pShadow->m_ObjPos.z - vtxDiff.z);

				// ��]������
				vtx0.x *= sinf(fAngle); vtx0.z *= cosf(fAngle);
				vtx1.x *= sinf(fAngle); vtx1.z *= cosf(fAngle);
				vtx2.x *= sinf(fAngle); vtx2.z *= cosf(fAngle);
				vtx3.x *= sinf(fAngle); vtx3.z *= cosf(fAngle);

				if (NULL == pShadow->m_pParent)
				{
					CLine::Create(vtx0, vtx1, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
					CLine::Create(vtx1, vtx3, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
					CLine::Create(vtx3, vtx2, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
					CLine::Create(vtx2, vtx0, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));

					D3DXVECTOR3 lineY = D3DXVECTOR3(0.0f, 40.0f, 0.0f);
					CLine::Create(vtx0, vtx0 + lineY, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
					CLine::Create(vtx1, vtx1 + lineY, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
					CLine::Create(vtx2, vtx2 + lineY, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
					CLine::Create(vtx3, vtx3 + lineY, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));

					CLine::Create(vtx0 + lineY, vtx1 + lineY, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
					CLine::Create(vtx1 + lineY, vtx3 + lineY, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
					CLine::Create(vtx3 + lineY, vtx2 + lineY, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
					CLine::Create(vtx2 + lineY, vtx0 + lineY, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
				}

				D3DXVECTOR3 vecA, vecB;
				bool bRangeX = false, bRangeZ = false;
				/********************************/
				/*			X�̊O�όv�Z			*/
				/********************************/
				// �ŏ��l���̔���
				vecA = vtx0 - vtx2;
				vecB = pos - vtx2;

				if ((vecA.z * vecB.x) - (vecA.x * vecB.z) > 0)
				{// �͈͓�
				 // �ő吔���̔���
					vecA = vtx3 - vtx1;
					vecB = pos - vtx1;

					if ((vecA.z * vecB.x) - (vecA.x * vecB.z) > 0)
					{// �͈͓�
						bRangeX = true;
					}
				}

				/********************************/
				/*			Z�̊O�όv�Z			*/
				/********************************/
				// �ŏ��l���̔���
				vecA = vtx1 - vtx0;
				vecB = pos - vtx0;

				if ((vecA.z * vecB.x) - (vecA.x * vecB.z) > 0)
				{// �͈͓�
				 // �ő吔���̔���
					vecA = vtx2 - vtx3;
					vecB = pos - vtx3;

					if ((vecA.z * vecB.x) - (vecA.x * vecB.z) > 0)
					{// �͈͓�
						bRangeZ = true;
					}
				}

				if (bRangeX && bRangeZ)
				{// ���ʂɏ���Ă���
					if (pShadow->m_pParent == NULL)CDebugProc::Print("�e�͈͓̔�");
					return true;
				}
			}
		}

		// ���̃I�u�W�F�N�g������
		pScene = pSceneNext;
	}

	return false;
}