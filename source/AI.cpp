//=============================================================================
//
// AI���� [AI.cpp]
// Author : Takuto Ishida
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "AI.h"
#include "player.h"
#include "manager.h"
#include "renderer.h"
#include "debugProc.h"
#include "model.h"
#include "motion.h"
#include "game.h"
#include "bullet.h"
#include "server.h"
#include "collisionSet.h"
#include <stdio.h>

//==================================
// �}�N����`
//==================================
#define WALKER_FILE					"data/TEXT/AI/walker/model_walker.txt"
#define DRONE_FILE					"data/TEXT/AI/drone/model_drone.txt"

//==================================
// �ÓI�����o�ϐ��錾
//==================================

//==================================
// ��������
//==================================
CAIMecha* CAIMecha::Create(CPlayer *pPlayer, MECHATYPE mecha, D3DXVECTOR3 pos)
{
	CAIMecha *pAI;

	pAI = new CAIMecha;	// ���������m��

	if (pAI != NULL)
	{// �������m�ې���
		pAI->m_mechaType = mecha;
		pAI->m_pos = pos;
		pAI->m_pPlayer = pPlayer;
		pAI->Init();
	}

	return pAI;
}

//=========================================
// �R���X�g���N�^
//=========================================
CAIMecha::CAIMecha(int nPriority, CScene::OBJTYPE objType) : CScene(nPriority, objType)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nCntState = 0;
	m_pModel = NULL;
	m_pMotion = NULL;
	m_nNumParts = 0;
	m_vtxMax = D3DXVECTOR3(-1000.0f, -1000.0f, -1000.0f);
	m_vtxMin = D3DXVECTOR3(1000.0f, 1000.0f, 1000.0f);
	m_nLife = 0;
	m_fSpeed = 0.0f;
	m_nAttack = 0;
	m_mechaType = MECHATYPE_WALKER;
	m_nCapacity = 0;
	m_nCurMotion = 0;
	m_nLifeMax = 0;
	m_nNumShoot = 0;
	m_pPlayer = NULL;
	m_nTeam = 0;
}

//=========================================
// �f�X�g���N�^
//=========================================
CAIMecha::~CAIMecha()
{
}

//=========================================
// ����������
//=========================================
HRESULT CAIMecha::Init(void)
{
	// �f�o�C�X�̎擾
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	FILE *pFile;
	int nCntParts = 0;		// �p�[�c���̃J�E���^
	int nCntModel = 0;
	float fX, fY, fZ;
	int nId;
	int nParent;
	char **pModelName = NULL;
	char *pFileName = "";

	if (MECHATYPE_WALKER == m_mechaType) { pFileName = WALKER_FILE; }
	else if (MECHATYPE_DRONE == m_mechaType) { pFileName = DRONE_FILE; }

	// �t�@�C�����J��
	pFile = fopen(pFileName, "r");

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

					if (strcmp(aStr, "NUM_MODEL") == 0)
					{// ���f����
						int nNumModel;
						fscanf(pFile, " = %d", &nNumModel);

						pModelName = new char*[nNumModel];		// �������m��
					}
					if (strcmp(aStr, "MODEL_FILENAME") == 0)
					{// �L�����N�^�[���
						fscanf(pFile, " = %s", &aStr);

						pModelName[nCntModel] = new char[64];		// �������m��
						strcpy(pModelName[nCntModel], aStr);
						nCntModel++;
					}
					if (strcmp(aStr, "CHARACTERSET") == 0)
					{// �L�����N�^�[���
						while (strcmp(aStr, "END_CHARACTERSET") != 0)
						{// �L�����N�^�[���I�����b�Z�[�W�܂Ń��[�v
							fscanf(pFile, "%s", aStr);

							if (strcmp(aStr, "NUM_PARTS") == 0)
							{// �p�[�c��
								fscanf(pFile, " = %d", &m_nNumParts);
								m_pModel = new CModel*[m_nNumParts];

								for (int nCntModelParts = 0; nCntModelParts < m_nNumParts; nCntModelParts++)
								{// �p�[�c�������[�v
									m_pModel[nCntModelParts] = CModel::Create(&m_mtxWorld);
								}
							}
							else if (strcmp(aStr, "CAPACITY") == 0)
							{// ���e��
								fscanf(pFile, " = %d", &m_nCapacity);
							}
							else if (strcmp(aStr, "ATTACK") == 0)
							{// �U����
								fscanf(pFile, " = %d", &m_nAttack);
							}
							else if (strcmp(aStr, "SHOOTS") == 0)
							{// ���ː�
								fscanf(pFile, " = %d", &m_nNumShoot);
							}
							else if (strcmp(aStr, "LIFE") == 0)
							{// �ϋv��
								fscanf(pFile, " = %d", &m_nLifeMax);
							}
							else if (strcmp(aStr, "SPEED") == 0)
							{// �ړ���
								fscanf(pFile, " = %f", &m_fSpeed);
							}
							else if (strcmp(aStr, "PARTSSET") == 0)
							{// �p�[�c�̐ݒ�
								while (strcmp(aStr, "END_PARTSSET") != 0)
								{// �p�[�c�̐ݒ�I�����b�Z�[�W�܂Ń��[�v
									if (m_pModel[nCntParts] != NULL)
									{// �������m�ې���
										fscanf(pFile, "%s", &aStr);

										if (strcmp(aStr, "INDEX") == 0)
										{// ���f���ԍ�
											fscanf(pFile, " = %d", &nId);

											// �g�����f�����w��
											m_pModel[nCntParts]->SetModel(pModelName[nId]);
											m_pModel[nCntParts]->Init();
										}
										else if (strcmp(aStr, "PARENT") == 0)
										{// �e�����߂�
											fscanf(pFile, " = %d", &nParent);

											if (nParent >= 0)
											{// �e�̃|�C���^��ݒ�
												m_pModel[nCntParts]->SetParent(m_pModel[nParent]);
											}
											else
											{// �e�����Ȃ��ꍇ
												m_pModel[nCntParts]->SetParent(NULL);
											}
										}
										else if (strcmp(aStr, "POS") == 0)
										{// �ʒu�̐ݒ�
											fscanf(pFile, " = %f %f %f", &fX, &fY, &fZ);

											// �ʒu�̐ݒ�
											m_pModel[nCntParts]->SetPos(D3DXVECTOR3(fX, fY, fZ));
										}
										else if (strcmp(aStr, "ROT") == 0)
										{// �����̐ݒ�
											fscanf(pFile, " = %f %f %f", &fX, &fY, &fZ);

											// �����̐ݒ�
											m_pModel[nCntParts]->SetRot(D3DXVECTOR3(fX, fY, fZ));
										}
									}
								}
								nCntParts++;
							}
						}
					}
				}
				break;
			}
		}
		fclose(pFile);	// �t�@�C�������
	}
	else
	{// �t�@�C�����Ȃ��Ƃ�
		MessageBox(0, "�t�@�C��������܂���", "�v���C���[���f���e�L�X�g", MB_YESNO);
	}

	if (NULL != pModelName)
	{// �������̊J��
		for (int nCnt = 0; nCnt < m_nNumParts; nCnt++)
		{// �������̊J��
			delete[] pModelName[nCnt];
		}

		// �������̊J��
		delete[] pModelName;
	}

	if (NULL == m_pMotion)
	{// ���[�V�����N���X�̐���
		m_pMotion = CMotion::Create(m_mechaType, m_nNumParts, m_pModel);
		m_pMotion->SetMotion(CMotionManager::TYPE_NEUTRAL);	// �j���[�g�������[�V������ݒ�
	}

	// ���_���W�̍ŏ��l�ƍő�l�����߂�
	D3DXVECTOR3 pos, posMax, posMin;
	D3DXVECTOR3 posMaxVtx, posMinVtx;
	posMax = m_vtxMax;
	posMin = m_vtxMin;
	for (int nCntModel = 0; nCntModel < m_nNumParts; nCntModel++)
	{
		pos = m_pModel[nCntModel]->GetPos();
		posMaxVtx = m_pModel[nCntModel]->GetVtxMax() + pos;
		posMinVtx = m_pModel[nCntModel]->GetVtxMin() + pos;

		// X�̒l�̔�r
		if (posMin.x > posMinVtx.x)
		{// �ŏ��l�Ɣ�r
			posMin.x = posMinVtx.x;	// �ŏ��l��菬������Α��
		}

		if (posMax.x < posMaxVtx.x)
		{// �ő�l�Ɣ�r
			posMax.x = posMaxVtx.x;	// �ő�l���傫����Α��
		}

		// Y�̒l�̔�r
		if (posMin.y > posMinVtx.y)
		{// �ŏ��l�Ɣ�r
			posMin.y = posMinVtx.y;	// �ŏ��l��菬������Α��
		}

		if (posMax.y < posMaxVtx.y)
		{// �ő�l�Ɣ�r
			posMax.y = posMaxVtx.y;	// �ő�l���傫����Α��
		}

		// Z�̒l�̔�r
		if (posMin.z > posMinVtx.z)
		{// �ŏ��l�Ɣ�r
			posMin.z = posMinVtx.z;	// �ŏ��l��菬������Α��
		}

		if (posMax.z < posMaxVtx.z)
		{// �ő�l�Ɣ�r
			posMax.z = posMaxVtx.z;	// �ő�l���傫����Α��
		}
	}

	// �S�p�[�c�̒��_���W�̍ŏ��l�E�ő�l
	m_vtxMin.x = posMin.x;
	m_vtxMin.y = posMin.y;
	m_vtxMin.z = posMin.z;
	m_vtxMax.x = posMax.x;
	m_vtxMax.y = posMax.y;
	m_vtxMax.z = posMax.z;

	m_nLife = m_nLifeMax;	// ���C�t�̏����l

	if (m_pPlayer->GetTeam() == 0 || m_pPlayer->GetTeam() == 1)
	{
		for (int nCntModel = 0; nCntModel < m_nNumParts; nCntModel++)
		{// �`�[�����Ƃ̐F�ɐݒ�
			m_pModel[nCntModel]->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
		}

		// �`�[���̐ݒ�
		m_nTeam = 0;
	}
	else if (m_pPlayer->GetTeam() == 2 || m_pPlayer->GetTeam() == 3)
	{
		for (int nCntModel = 0; nCntModel < m_nNumParts; nCntModel++)
		{// �`�[�����Ƃ̐F�ɐݒ�
			m_pModel[nCntModel]->SetColor(D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
		}

		// �`�[���̐ݒ�
		m_nTeam = 1;
	}

	return S_OK;
}

//=========================================
// �I������
//=========================================
void CAIMecha::Uninit(void)
{
	if (NULL != m_pMotion)
	{// ���[�V�����N���X�̔j��
		m_pMotion->Uninit();
		m_pMotion = NULL;
	}

	if (NULL != m_pModel)
	{
		for (int nCntModel = 0; nCntModel < m_nNumParts; nCntModel++)
		{// ���f���̔j��
			if (NULL != m_pModel[nCntModel])
			{
				m_pModel[nCntModel]->Uninit();
				delete m_pModel[nCntModel];
				m_pModel[nCntModel] = NULL;
			}
		}

		// �������̊J��
		delete[] m_pModel;
		m_pModel = NULL;
	}

	// �I�u�W�F�N�g�̔j��
	CScene::Release();
}

//=========================================
// �X�V����
//=========================================
void CAIMecha::Update(void)
{
	if (NULL != m_pMotion && MECHATYPE_WALKER == m_mechaType)
	{// ���[�V����
		m_pMotion->SetMotion(CMotionManager::TYPE_NEUTRAL);
	}

	if (MECHATYPE_DRONE == m_mechaType)
	{// �h���[���̃v���y������
		D3DXVECTOR3 rot = m_pModel[1]->GetRot();
		rot.y += 2.0f;
		m_pModel[1]->SetRot(rot);
	}
}

//=========================================
// �`�揈��
//=========================================
void CAIMecha::Draw(void)
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

	for (int nCntModel = 0; nCntModel < m_nNumParts; nCntModel++)
	{// ���f���̕`�揈��
		m_pModel[nCntModel]->Draw();
	}
}
