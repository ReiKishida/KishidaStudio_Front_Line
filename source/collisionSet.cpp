//=============================================================================
//
// ���b�V���t�B�[���h���� [meshField.cpp]
// Author : Ishida Takuto
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "collisionSet.h"
#include "manager.h"
#include "renderer.h"
#include "line.h"
#include "scene.h"
#include "debugProc.h"
#include <stdio.h>

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define COLLISION_FILENAME		"data/TEXT/FIELD/collision.txt"

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================

//=============================================================================
// �ǂݍ��ݏ���
//=============================================================================
void CCollision::Load(void)
{
	D3DXVECTOR3 pos, size;

	// ���f���I�u�W�F�N�g��T��
	CScene *pScene = CScene::GetSceneTop(COLLISION_PRIORITY);

	while (pScene != NULL)
	{// NULL�ɂȂ�܂Ń��[�v
		CScene *pSceneNext = pScene->GetSceneNext();
		CScene::OBJTYPE objType = pScene->GetObjType();

		if (CScene::OBJTYPE_COLLISION == objType)
		{// �����蔻��̏ꍇ
			CCollision *pCollision = (CCollision*)pScene;
			pCollision->Uninit();
		}

		// ���̃I�u�W�F�N�g������
		pScene = pSceneNext;
	}

	// �t�@�C�����J��
	FILE *pFile = fopen(COLLISION_FILENAME, "r");

	if (NULL != pFile)
	{// �t�@�C����������
		char aStr[128];

		while (1)
		{// �X�N���v�g��ǂݏI���܂Ń��[�v
			fscanf(pFile, "%s", aStr);

			if (strcmp(aStr, "SCRIPT") == 0)
			{// �X�N���v�g�J�n
				while (strcmp(aStr, "END_SCRIPT") != 0)
				{// �X�N���v�g�I�����b�Z�[�W�܂Ń��[�v
					fscanf(pFile, "%s", aStr);

					if (strcmp(aStr, "COLLISIONSET") == 0)
					{// ���f����
						while (strcmp(aStr, "END_COLLISIONSET") != 0)
						{// �I�����b�Z�[�W�܂Ń��[�v
							fscanf(pFile, "%s", aStr);
							if (strcmp(aStr, "POS") == 0)
							{// �ʒu
								fscanf(pFile, " = %f %f %f", &pos.x, &pos.y, &pos.z);
							}
							else if (strcmp(aStr, "SIZE") == 0)
							{// �傫��
								fscanf(pFile, " = %f %f %f", &size.x, &size.y, &size.z);
							}
						}

						// �����蔻��̐���
						CCollision::Create(pos, size.x, size.y, size.z);
					}
				}
				break;
			}
		}
		fclose(pFile);	// �t�@�C�������
	}
	else
	{// �t�@�C�����Ȃ��Ƃ�
		MessageBox(0, "�e�L�X�g�t�@�C��������܂���B", "ENEMY/model.txt", MB_YESNO);
	}
}

//==================================
// ��������
//==================================
CCollision* CCollision::Create(D3DXVECTOR3 pos, float fWidth, float fHeight, float fDepth)
{
	CCollision *pCollision = NULL;

	pCollision = new CCollision;	// ���������m��

	if (NULL != pCollision)
	{// �������m�ې���
		pCollision->m_pos = pos;
		pCollision->m_fWidth = fWidth;
		pCollision->m_fHeight = fHeight;
		pCollision->m_fDepth = fDepth;
		pCollision->Init();
	}

	return pCollision;
}

//=========================================
// �R���X�g���N�^
//=========================================
CCollision::CCollision(int nPriority, CScene::OBJTYPE objType) : CScene(nPriority, objType)
{
	m_fWidth = 0;
	m_fHeight = 0;
	m_fDepth = 0;
	m_pLine = NULL;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//=========================================
// �f�X�g���N�^
//=========================================
CCollision::~CCollision()
{
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CCollision::Init(void)
{
	// �ӂ̐����������m��
	m_pLine = new CLine*[SIDE_MAX];

	/*----------------------------------*/
	/*			���C���̐���			*/
	/*----------------------------------*/
	// ���
	m_pLine[SIDE_UPPER_FRONT] = CLine::Create(m_pos + D3DXVECTOR3(0.0f, m_fHeight, m_fDepth), m_pos + D3DXVECTOR3(m_fWidth, m_fHeight, m_fDepth), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	m_pLine[SIDE_UPPER_RIGHT] = CLine::Create(m_pos + D3DXVECTOR3(m_fWidth, m_fHeight, m_fDepth), m_pos + D3DXVECTOR3(m_fWidth, m_fHeight, 0.0f), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	m_pLine[SIDE_UPPER_BEHIND] = CLine::Create(m_pos + D3DXVECTOR3(m_fWidth, m_fHeight, 0.0f), m_pos + D3DXVECTOR3(0.0f, m_fHeight, 0.0f), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	m_pLine[SIDE_UPPER_LEFT] = CLine::Create(m_pos + D3DXVECTOR3(0.0f, m_fHeight, 0.0f), m_pos + D3DXVECTOR3(0.0f, m_fHeight, m_fDepth), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));

	// ����
	m_pLine[SIDE_MIDDLE_LEFT_FRONT] = CLine::Create(m_pos + D3DXVECTOR3(0.0f, m_fHeight, m_fDepth), m_pos + D3DXVECTOR3(0.0f, 0.0f, m_fDepth), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	m_pLine[SIDE_MIDDLE_RIGHT_FRONT] = CLine::Create(m_pos + D3DXVECTOR3(m_fWidth, m_fHeight, m_fDepth), m_pos + D3DXVECTOR3(m_fWidth, 0.0f, m_fDepth), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	m_pLine[SIDE_MIDDLE_RIGHT_BEHIND] = CLine::Create(m_pos + D3DXVECTOR3(m_fWidth, m_fHeight, 0.0f), m_pos + D3DXVECTOR3(m_fWidth, 0.0f, 0.0f), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	m_pLine[SIDE_MIDDLE_LEFT_BEHIND] = CLine::Create(m_pos + D3DXVECTOR3(0.0f, m_fHeight, 0.0f), m_pos + D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));

	// ���
	m_pLine[SIDE_LOWER_FRONT] = CLine::Create(m_pos + D3DXVECTOR3(0.0f, 0.0f, m_fDepth), m_pos + D3DXVECTOR3(m_fWidth, 0.0f, m_fDepth), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	m_pLine[SIDE_LOWER_RIGHT] = CLine::Create(m_pos + D3DXVECTOR3(m_fWidth, 0.0f, m_fDepth), m_pos + D3DXVECTOR3(m_fWidth, 0.0f, 0.0f), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	m_pLine[SIDE_LOWER_BEHIND] = CLine::Create(m_pos + D3DXVECTOR3(m_fWidth, 0.0f, 0.0f), m_pos + D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	m_pLine[SIDE_LOWER_LEFT] = CLine::Create(m_pos + D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_pos + D3DXVECTOR3(0.0f, 0.0f, m_fDepth), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CCollision::Uninit(void)
{
	if (NULL != m_pLine)
	{
		for (int nCntLine = 0; nCntLine < SIDE_MAX; nCntLine++)
		{// �j��
			m_pLine[nCntLine]->Uninit();
			m_pLine[nCntLine] = NULL;
		}

		// �������̊J��
		delete m_pLine;
		m_pLine = NULL;
	}

	// ������j��
	CScene::Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CCollision::Update(void)
{
}

//=============================================================================
// �`�揈��
//=============================================================================
void CCollision::Draw(void)
{
}

//=============================================================================
// �����蔻��
//=============================================================================
bool CCollision::Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 posOld, D3DXVECTOR3 vtxMax, D3DXVECTOR3 vtxMin)
{
	bool bHit = false;
	// ���f���I�u�W�F�N�g��T��
	CScene *pScene = CScene::GetSceneTop(COLLISION_PRIORITY);

	while (pScene != NULL)
	{// NULL�ɂȂ�܂Ń��[�v
		CScene *pSceneNext = pScene->GetSceneNext();
		CScene::OBJTYPE objType = pScene->GetObjType();

		if (CScene::OBJTYPE_COLLISION == objType)
		{// �����蔻��̏ꍇ
			CCollision *pCollision = (CCollision*)pScene;
			if (pCollision->CollisionBox(pPos, posOld, vtxMax, vtxMin))
			{// �{�b�N�X�R���W�����ɏՓ˂��Ă���
				bHit = true;
			}
		}

		// ���̃I�u�W�F�N�g������
		pScene = pSceneNext;
	}

	return bHit;
}

//=============================================================================
// �{�b�N�X�^�̓����蔻��
//=============================================================================
bool CCollision::CollisionBox(D3DXVECTOR3 *pPos, D3DXVECTOR3 posOld, D3DXVECTOR3 vtxMax, D3DXVECTOR3 vtxMin)
{
	bool bHitX = false, bHitY = false, bHitZ = false;
	D3DXVECTOR3 modelVtxMax, modelVtxMin;
	modelVtxMax = *pPos + vtxMax;
	modelVtxMin = *pPos + vtxMin;

	D3DXVECTOR3 vecA, vecB, vecC;

	/********************************/
	/*			X�̊O�όv�Z			*/
	/********************************/
	// �ŏ��l���̔���
	vecA = m_pLine[SIDE_LOWER_LEFT]->GetVector();
	vecB = modelVtxMax - m_pLine[SIDE_LOWER_LEFT]->GetStart();
	vecC = modelVtxMin - m_pLine[SIDE_LOWER_LEFT]->GetStart();

	if ((vecA.z * vecB.x) - (vecA.x * vecB.z) > 0 || (vecA.z * vecC.x) - (vecA.x * vecC.z) > 0)
	{// �͈͓�
	 // �ő吔���̔���
		vecA = m_pLine[SIDE_LOWER_RIGHT]->GetVector();
		vecB = modelVtxMin - m_pLine[SIDE_LOWER_RIGHT]->GetStart();
		vecC = modelVtxMax - m_pLine[SIDE_LOWER_RIGHT]->GetStart();

		if ((vecA.z * vecB.x) - (vecA.x * vecB.z) > 0 || (vecA.z * vecC.x) - (vecA.x * vecC.z) > 0)
		{// �͈͓�
			bHitX = true;
		}
		else { return false; }
	}
	else { return false; }

	/********************************/
	/*			Z�̊O�όv�Z			*/
	/********************************/
	// �ŏ��l���̔���
	vecA = m_pLine[SIDE_LOWER_BEHIND]->GetVector();
	vecB = modelVtxMax - m_pLine[SIDE_LOWER_BEHIND]->GetStart();
	vecC = modelVtxMin - m_pLine[SIDE_LOWER_BEHIND]->GetStart();

	if ((vecA.z * vecB.x) - (vecA.x * vecB.z) > 0 || (vecA.z * vecC.x) - (vecA.x * vecC.z) > 0)
	{// �͈͓�
	 // �ő吔���̔���
		vecA = m_pLine[SIDE_LOWER_FRONT]->GetVector();
		vecB = modelVtxMin - m_pLine[SIDE_LOWER_FRONT]->GetStart();
		vecC = modelVtxMax - m_pLine[SIDE_LOWER_FRONT]->GetStart();

		if ((vecA.z * vecB.x) - (vecA.x * vecB.z) > 0 || (vecA.z * vecC.x) - (vecA.x * vecC.z) > 0)
		{// �͈͓�
			bHitZ = true;
		}
		else { return false; }
	}
	else { return false; }

	if (bHitX && bHitZ)
	{// �͈͓��ɓ����Ă���
		if (pPos->x - vtxMax.x <= m_fWidth + m_pos.x && posOld.x - (vtxMax.x - 1.0f) >= m_fWidth + m_pos.x)
		{// �E������Փ�
			pPos->x = vtxMax.x + m_fWidth + m_pos.x;
		}
		else if (pPos->x + vtxMax.z > m_pos.x && posOld.x + (vtxMax.z - 1.0f) <= m_pos.x)
		{// ��������Փ�
			pPos->x = m_pos.x - vtxMax.z;
		}

		if (pPos->z - vtxMax.x <= m_fDepth + m_pos.z && posOld.z - (vtxMax.x - 1.0f) >= m_fDepth + m_pos.z)
		{// �O������Փ�
			pPos->z = vtxMax.x + m_fDepth + m_pos.z;
		}
		else if (pPos->z + vtxMax.z >= m_pos.z && posOld.z + (vtxMax.z - 1.0f) <= m_pos.z)
		{// �㑤����Փ�
			pPos->z = m_pos.z - vtxMax.z;
		}

		return true;
	}

	return false;
}

//=============================================================================
// �ʒu�̐ݒ�
//=============================================================================
void CCollision::SetPos(D3DXVECTOR3 pos)
{
	// �ʒu�̐ݒ�
	m_pos = pos;

	// ���
	m_pLine[SIDE_UPPER_FRONT]->SetPoint(m_pos + D3DXVECTOR3(0.0f, m_fHeight, m_fDepth), m_pos + D3DXVECTOR3(m_fWidth, m_fHeight, m_fDepth));
	m_pLine[SIDE_UPPER_RIGHT]->SetPoint(m_pos + D3DXVECTOR3(m_fWidth, m_fHeight, m_fDepth), m_pos + D3DXVECTOR3(m_fWidth, m_fHeight, 0.0f));
	m_pLine[SIDE_UPPER_BEHIND]->SetPoint(m_pos + D3DXVECTOR3(m_fWidth, m_fHeight, 0.0f), m_pos + D3DXVECTOR3(0.0f, m_fHeight, 0.0f));
	m_pLine[SIDE_UPPER_LEFT]->SetPoint(m_pos + D3DXVECTOR3(0.0f, m_fHeight, 0.0f), m_pos + D3DXVECTOR3(0.0f, m_fHeight, m_fDepth));

	// ����
	m_pLine[SIDE_MIDDLE_LEFT_FRONT]->SetPoint(m_pos + D3DXVECTOR3(0.0f, m_fHeight, m_fDepth), m_pos + D3DXVECTOR3(0.0f, 0.0f, m_fDepth));
	m_pLine[SIDE_MIDDLE_RIGHT_FRONT]->SetPoint(m_pos + D3DXVECTOR3(m_fWidth, m_fHeight, m_fDepth), m_pos + D3DXVECTOR3(m_fWidth, 0.0f, m_fDepth));
	m_pLine[SIDE_MIDDLE_RIGHT_BEHIND]->SetPoint(m_pos + D3DXVECTOR3(m_fWidth, m_fHeight, 0.0f), m_pos + D3DXVECTOR3(m_fWidth, 0.0f, 0.0f));
	m_pLine[SIDE_MIDDLE_LEFT_BEHIND]->SetPoint(m_pos + D3DXVECTOR3(0.0f, m_fHeight, 0.0f), m_pos + D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	// ���
	m_pLine[SIDE_LOWER_FRONT]->SetPoint(m_pos + D3DXVECTOR3(0.0f, 0.0f, m_fDepth), m_pos + D3DXVECTOR3(m_fWidth, 0.0f, m_fDepth));
	m_pLine[SIDE_LOWER_RIGHT]->SetPoint(m_pos + D3DXVECTOR3(m_fWidth, 0.0f, m_fDepth), m_pos + D3DXVECTOR3(m_fWidth, 0.0f, 0.0f));
	m_pLine[SIDE_LOWER_BEHIND]->SetPoint(m_pos + D3DXVECTOR3(m_fWidth, 0.0f, 0.0f), m_pos + D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_pLine[SIDE_LOWER_LEFT]->SetPoint(m_pos + D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_pos + D3DXVECTOR3(0.0f, 0.0f, m_fDepth));
}

//=============================================================================
// �T�C�Y�̐ݒ�
//=============================================================================
void CCollision::SetSize(D3DXVECTOR3 size)
{
	// �T�C�Y�̐ݒ�
	m_fWidth = size.x;
	m_fHeight = size.y;
	m_fDepth = size.z;

	// ���
	m_pLine[SIDE_UPPER_FRONT]->SetPoint(m_pos + D3DXVECTOR3(0.0f, m_fHeight, m_fDepth), m_pos + D3DXVECTOR3(m_fWidth, m_fHeight, m_fDepth));
	m_pLine[SIDE_UPPER_RIGHT]->SetPoint(m_pos + D3DXVECTOR3(m_fWidth, m_fHeight, m_fDepth), m_pos + D3DXVECTOR3(m_fWidth, m_fHeight, 0.0f));
	m_pLine[SIDE_UPPER_BEHIND]->SetPoint(m_pos + D3DXVECTOR3(m_fWidth, m_fHeight, 0.0f), m_pos + D3DXVECTOR3(0.0f, m_fHeight, 0.0f));
	m_pLine[SIDE_UPPER_LEFT]->SetPoint(m_pos + D3DXVECTOR3(0.0f, m_fHeight, 0.0f), m_pos + D3DXVECTOR3(0.0f, m_fHeight, m_fDepth));

	// ����
	m_pLine[SIDE_MIDDLE_LEFT_FRONT]->SetPoint(m_pos + D3DXVECTOR3(0.0f, m_fHeight, m_fDepth), m_pos + D3DXVECTOR3(0.0f, 0.0f, m_fDepth));
	m_pLine[SIDE_MIDDLE_RIGHT_FRONT]->SetPoint(m_pos + D3DXVECTOR3(m_fWidth, m_fHeight, m_fDepth), m_pos + D3DXVECTOR3(m_fWidth, 0.0f, m_fDepth));
	m_pLine[SIDE_MIDDLE_RIGHT_BEHIND]->SetPoint(m_pos + D3DXVECTOR3(m_fWidth, m_fHeight, 0.0f), m_pos + D3DXVECTOR3(m_fWidth, 0.0f, 0.0f));
m_pLine[SIDE_MIDDLE_LEFT_BEHIND]->SetPoint(m_pos + D3DXVECTOR3(0.0f, m_fHeight, 0.0f), m_pos + D3DXVECTOR3(0.0f, 0.0f, 0.0f));

// ���
m_pLine[SIDE_LOWER_FRONT]->SetPoint(m_pos + D3DXVECTOR3(0.0f, 0.0f, m_fDepth), m_pos + D3DXVECTOR3(m_fWidth, 0.0f, m_fDepth));
m_pLine[SIDE_LOWER_RIGHT]->SetPoint(m_pos + D3DXVECTOR3(m_fWidth, 0.0f, m_fDepth), m_pos + D3DXVECTOR3(m_fWidth, 0.0f, 0.0f));
m_pLine[SIDE_LOWER_BEHIND]->SetPoint(m_pos + D3DXVECTOR3(m_fWidth, 0.0f, 0.0f), m_pos + D3DXVECTOR3(0.0f, 0.0f, 0.0f));
m_pLine[SIDE_LOWER_LEFT]->SetPoint(m_pos + D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_pos + D3DXVECTOR3(0.0f, 0.0f, m_fDepth));
}
