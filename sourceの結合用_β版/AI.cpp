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
#include "input.h"
#include "button.h"
#include "mouseCursor.h"

//==================================
// �}�N����`
//==================================
#define WALKER_FILE	"data/TEXT/AI/walker/model_walker.txt"
#define DRONE_FILE	"data/TEXT/AI/drone/model_drone.txt"

// ���[�g�T���p
#define	LOAD_FILENAME		("data/TEXT/NODE_DATA/NodeData.txt")
#define ENEMY_BREAKTIME		(1)			// �x�e����(�t���[��)
#define MOUSE_ACCEPTABLE	(20.0f)		// �}�E�X�̌덷�̋��e�͈�
#define MOVE_ACCEPTABLE		(25.0f)		// �ړ��̌덷�̋��e�͈�
#define POS_ACCEPTABLE		(100.0f)	// �ʒu�̌덷�̋��e�͈�
#define AI_SPEED			(3.0f)		// �ړ����x

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
CAIMecha::~CAIMecha() {}

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

	if (m_pPlayer->GetTeam() == 0)
	{
		for (int nCntModel = 0; nCntModel < m_nNumParts; nCntModel++)
		{// �`�[�����Ƃ̐F�ɐݒ�
			m_pModel[nCntModel]->SetColor(D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
		}

		// �`�[���̐ݒ�
		m_nTeam = 0;
	}
	else if (m_pPlayer->GetTeam() == 1)
	{
		for (int nCntModel = 0; nCntModel < m_nNumParts; nCntModel++)
		{// �`�[�����Ƃ̐F�ɐݒ�
			m_pModel[nCntModel]->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
		}

		// �`�[���̐ݒ�
		m_nTeam = 1;
	}

	// ���l�̏�����==============================================================================
	// �p�[�g�֌W
	m_bPartSwitch = CGame::PART_ACTION;
	m_bPartSwitchOld = CGame::PART_ACTION;

	// �m�[�h�֌W
	m_nStartNode = 0;
	m_nEndNode = 0;
	m_nNodeOld = 0;

	// �����[�֌W
	m_nRallyCount = 0;
	m_nRallyCountOld = 0;

	// �����ړ��֌W
	m_posDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nBreaktime = 0;
	m_nCountPoint = 0;
	m_nPoint = 0;
	m_bGoal = false;

	for (int nCntRally = 0; nCntRally < RALLYPOINT_MAX; nCntRally++)
	{// �����[�|�C���g�̍ő吔�����
		for (int nCntNode = 0; nCntNode < NODEPOINT_MAX; nCntNode++)
		{// �m�[�h�̍ő吔�����
			m_node[nCntRally][nCntNode] = {};
		}
	}

	for (int nCntEnemy = 0; nCntEnemy < ENEMY_PLAYER_MAX; nCntEnemy++)
	{// �G�l�~�[�̍ő吔�����
		m_nRallyEndNode[nCntEnemy] = 0;
	}

	for (int nCntNode = 0; nCntNode < NODEPOINT_MAX; nCntNode++)
	{// �m�[�h�̍ő�l�����
		m_waypoint[nCntNode] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	for (int nCntAction = 0; nCntAction < 4; nCntAction++)
	{// �s�����̕����
		m_AIAction[nCntAction] = AI_ACTION_NONE;
		m_LogicTree[nCntAction] = -1;
	}

	// �}�b�v�f�[�^�t�@�C���̓ǂݍ���
	CAIMecha::FileLoad(LOAD_FILENAME);

	// �J�n���_�̃m�[�h�̏�����
	float fMinLength = 100000, fLength = 100000;	// �����n
	for (int nCntNode = 0; nCntNode < m_NodeData.nodeMax; nCntNode++)
	{// �m�[�h�̐��������
	 // ���������߂�
		fLength = (m_NodeData.pos[nCntNode].x - m_pos.x) * (m_NodeData.pos[nCntNode].x - m_pos.x) + (m_NodeData.pos[nCntNode].z - m_pos.z) * (m_NodeData.pos[nCntNode].z - m_pos.z);

		if (fMinLength > fLength)
		{// �����̍ŏ��l�����߂�
			fMinLength = fLength;
			m_nStartNode = nCntNode;
		}
	}
	m_nEndNode = m_nStartNode;

	// �|�C���g�ւ̌o�H�T��
	CAIMecha::RootSearch();

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

	// AI�֌W�̍X�V����
	CAIMecha::AIUpdate();
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


//=============================================================================
// �_���[�W����
//=============================================================================
void CAIMecha::Damage(int nDamage)
{
	if (m_nLife > 0)
	{
		m_state = STATE_DAMAGE;								// �_���[�W���󂯂Ă����Ԃɂ���

		m_nLife -= nDamage;

		if (0 >= m_nLife)
		{
			m_nLife = 0;

			switch (m_nTeam)
			{
			case 0:
				CManager::GetGame()->SetBlueLinkEnergy(CManager::GetGame()->GetBlueLinkEnergy() - 20);
				break;
			case 1:
				CManager::GetGame()->SetRedLinkEnergy(CManager::GetGame()->GetRedLinkEnergy() - 20);
				break;
			}
		}
		//CSound *pSound = CManager::GetSound();				// �T�E���h�̎擾
		//pSound->PlaySound(CSound::SOUND_LABEL_DAMAGE);		// �_���[�W�����Đ�
	}
}

//=============================================================================
//	AI�X�V����
//=============================================================================
void CAIMecha::AIUpdate()
{
	// �p�[�g���	�X�g���e�W�[�Ftrue	�A�N�V�����Ffalse
	//CDebugProc::Print("========AI========\n");
	//CDebugProc::Print("���݂̃p�[�g : %s\n", m_bPartSwitch ? "�X�g���e�W�[" : "�A�N�V����");
	//CDebugProc::Print("�S�[���ɓ���%s�B\n", m_bGoal ? "���܂���" : "���Ă܂���");
	//CDebugProc::Print("AIPos :%.1f, %.1f\n", m_pos.x, m_pos.z);
	//CDebugProc::Print("\n");

	//CDebugProc::Print("�N���b�N�� : %d\n", m_nRallyCount);

	//if (m_AIAction[2] == AI_ACTION_FOCUS_GOAL)
	//{// �ʏ펞
	//	CDebugProc::Print("�ڕW�D����\n");
	//	CDebugProc::Print("�J�n�n�_ : %d\n", m_nStartNode);
	//	CDebugProc::Print("�I���n�_ : %d\n", m_nEndNode);
	//	CDebugProc::Print("\n");
	//}
	//else if (m_AIAction[2] == AI_ACTION_RALLY)
	//{// �����[��
	//	CDebugProc::Print("�����[���\n");
	//	CDebugProc::Print("�J�n�n�_ : %d\n", m_nRallyEndNode[0]);
	//	for (int nCntRally = 1; nCntRally < m_nRallyCount; nCntRally++)
	//	{
	//		CDebugProc::Print("���Ԓn�_[%d] : %d\n", nCntRally, m_nRallyEndNode[nCntRally]);
	//	}
	//	CDebugProc::Print("�I���n�_ : %d\n", m_nRallyEndNode[m_nRallyCount]);
	//	CDebugProc::Print("\n");
	//}
	//else if (m_AIAction[2] == AI_ACTION_ROUND_TRIP)
	//{// ������
	//	CDebugProc::Print("�������\n");
	//	CDebugProc::Print("�J�n�n�_ : %d\n", m_nRallyEndNode[0]);
	//	for (int nCntRally = 1; nCntRally < m_nRallyCount; nCntRally++)
	//	{
	//		CDebugProc::Print("���Ԓn�_[%d] : %d\n", nCntRally, m_nRallyEndNode[nCntRally]);
	//	}
	//	CDebugProc::Print("�I���n�_ : %d\n", m_nRallyEndNode[m_nRallyCount]);
	//	CDebugProc::Print("\n");
	//}
	//else if (m_AIAction[1] = AI_ACTION_FOLLOW)
	//{// �Ǐ]��
	//	CDebugProc::Print("�Ǐ]���\n");
	//	CDebugProc::Print("�J�n�n�_ : %d\n", m_nStartNode);
	//	CDebugProc::Print("�I���n�_ : %d\n", m_nEndNode);
	//	CDebugProc::Print("\n");
	//}

	//CDebugProc::Print("���݂̈ړ��� : %d\n", m_nPoint);
	//CDebugProc::Print("�ڕW�܂ł̈ړ��� : %d\n", m_nCountPoint);
	//CDebugProc::Print("\n");

	CInputMouse *pMouse = CManager::GetInputMouse();	// �}�E�X�̓��͂��擾
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();	// �L�[�{�[�h�̓��͂��擾

	// �O��̃p�[�g���̕ۑ�
	m_bPartSwitchOld = m_bPartSwitch;
	// ���݂̃p�[�g�����擾
	m_bPartSwitch = CManager::GetGame()->GetPart();

	// ���f����
	Cancel();

	if (m_bPartSwitch == CGame::PART_STRATEGY && CManager::GetGame()->GetButtonManager() != NULL)
	{// �X�g���e�W�[�p�[�g���Ń{�^���}�l�[�W���[��NULL����Ȃ��ꍇ
		if (CManager::GetGame()->GetButtonManager()->GetSelectFinish() == true)
		{// AI�̍s�������肵�Ă���ꍇ

			for (int nCntAction = 0; nCntAction < 4; nCntAction++)
			{// �s�����̕����
			 // �f�[�^�̏�����
				m_AIAction[nCntAction] = AI_ACTION_NONE;
				m_LogicTree[nCntAction] = -1;
			}

			for (int nCntButton = 0; nCntButton < 4; nCntButton++)
			{// �{�^���̐��������
			 // �w���̎擾
				m_LogicTree[nCntButton] = CManager::GetGame()->GetButtonManager()->GetSelectLogic(nCntButton);
			}

			CManager::GetGame()->GetButtonManager()->GetSelectFinish() = false;	// �������Ԃɖ߂�
		}
	}

	// �s���̐ݒ�
	if (m_bPartSwitch == CGame::PART_STRATEGY)
	{// �X�g���e�W�[�p�[�g�̏ꍇ
		if (m_LogicTree[0] == 0)// �ړ�
		{
			m_AIAction[0] = AI_ACTION_MOVE;

			if (m_LogicTree[1] == 0)// �h���^
			{
				m_AIAction[1] = AI_ACTION_DISPATCH;

				if (m_LogicTree[2] == 0)// �ڕW�d��
				{
					m_AIAction[2] = AI_ACTION_FOCUS_GOAL;
				}
				else if (m_LogicTree[2] == 1)// �����[�|�C���g
				{
					m_AIAction[2] = AI_ACTION_RALLY;
				}
				else if (m_LogicTree[2] == 2)// ����
				{
					m_AIAction[2] = AI_ACTION_ROUND_TRIP;
				}
			}
			else if (m_LogicTree[1] == 1)// �Ǐ]�^
			{
				m_AIAction[1] = AI_ACTION_FOLLOW;

				if (m_LogicTree[2] == 0)// �����F��
				{
					m_AIAction[2] = AI_ACTION_FOLLOW_LONG;
				}
				else if (m_LogicTree[2] == 1)// �����F�Z
				{
					m_AIAction[2] = AI_ACTION_FOLLOW_SHORT;
				}
			}

			if (m_LogicTree[3] == 0)// �ړ�
			{
				m_AIAction[3] = AI_ACTION_MOVE;
			}
			else if (m_LogicTree[3] == 1)// �U��
			{
				m_AIAction[3] = AI_ACTION_ATTACK;
			}
		}
		else if (m_LogicTree[0] == 1)// �ҋ@
		{
			m_AIAction[0] = AI_ACTION_WAIT;

			if (m_LogicTree[1] == 0)// ����
			{
				m_AIAction[1] = AI_ACTION_BATTLE;

				if (m_LogicTree[2] == 0)// �ǐ�
				{
					m_AIAction[2] = AI_ACTION_PURSUIT;
				}
				else if (m_LogicTree[2] == 1)// ��ǐ�
				{
					m_AIAction[2] = AI_ACTION_NOT_PURSUIT;
				}
			}
			else if (m_LogicTree[1] == 1)// ���
			{
				m_AIAction[1] = AI_ACTION_AVOIDANCE;

				if (m_LogicTree[2] == 0)// ����
				{
					m_AIAction[2] = AI_ACTION_DISTURBANCE;
				}
				else if (m_LogicTree[2] == 1)// �P��
				{
					m_AIAction[2] = AI_ACTION_WITHDRAWAL;
				}
			}
		}
	}

	if (m_AIAction[0] == AI_ACTION_NONE)
	{// AI�̍s�������肵�Ă��Ȃ��ꍇ
	 // �Ǐ]���[�h�ɐݒ肷��
		m_AIAction[0] = AI_ACTION_MOVE;
		m_AIAction[1] = AI_ACTION_FOLLOW;
		m_AIAction[2] = AI_ACTION_FOLLOW_SHORT;
	}
	else if (m_AIAction[0] != AI_ACTION_NONE)
	{// AI�̍s�������肵�Ă���ꍇ
		if (m_AIAction[0] == AI_ACTION_MOVE)
		{// �ړ��̏ꍇ
			if (m_AIAction[1] == AI_ACTION_DISPATCH)
			{// �h���^�̏ꍇ
				if (pMouse->GetTrigger(CInputMouse::DIMS_BUTTON_0) == true)
				{// ���N���b�N�̂݉���
					if (m_nRallyCount < RALLYPOINT_MAX)
					{// �J�E���g���ő吔�܂œ��B���Ă��Ȃ��ꍇ
					 // �O��̏��̕ۑ�
						m_nRallyCountOld = m_nRallyCount;

						if (m_AIAction[2] == AI_ACTION_ROUND_TRIP || m_AIAction[2] == AI_ACTION_RALLY)
						{// �����[���܂��͉�����
							m_nRallyCount++;
						}
						else if (m_AIAction[2] == AI_ACTION_FOCUS_GOAL)
						{// �ڕW�D��s����
							m_nRallyCount = 1;
						}

						// �|�C���g����
						CAIMecha::NodeSearch(m_bGoal);
					}
				}
			}
			else if (m_AIAction[1] == AI_ACTION_FOLLOW)
			{// �Ǐ]�^�̏ꍇ
			 // ��l�ɒǏ]����
				CAIMecha::Follow();
			}
		}
		else if (m_AIAction[0] == AI_ACTION_WAIT)
		{// �ҋ@�̏ꍇ
		}
	}

	if (m_bPartSwitch == CGame::PART_ACTION)
	{// �A�N�V�����p�[�g�̏ꍇ
		m_nRallyCount = 0;

		// �����ړ�����
		CAIMecha::AutoMove();
	}
	else if (m_bPartSwitch == CGame::PART_STRATEGY)
	{// �X�g���e�W�[�p�[�g�̏ꍇ
		if (m_LogicTree[0] != -1 && pMouse->GetTrigger(CInputMouse::DIMS_BUTTON_0) == true)
		{// AI�̍s�������肵�Ă����Ԃō��N���b�N���ꂽ�ꍇ
			m_nPoint = 0;
			m_nCountPoint = -1;

			// �o�H�T�����@
			if (m_AIAction[2] == AI_ACTION_ROUND_TRIP)// ������
			{// �|�C���g�Ԃ�p�j����o�H�T��
				CAIMecha::PatrolRootSearch();
			}
			else if (m_AIAction[2] == AI_ACTION_RALLY)// �����[��
			{// �����[�|�C���g�ɂ��o�H�T��
				CAIMecha::RallyRootSearch();
			}
			if (m_AIAction[2] == AI_ACTION_FOCUS_GOAL)// �ڕW�D�掞
			{// �|�C���g�ւ̌o�H�T��
				CAIMecha::RootSearch();
			}
		}
	}
}

//=============================================================================
//	�����ړ�����
//=============================================================================
void CAIMecha::AutoMove()
{
	// �ڕW�n�_��ݒ�
	m_posDest = m_waypoint[m_nPoint];

	// �ړI�Ƃ̍������o��
	float fLength = (m_pos.x - m_posDest.x) * (m_pos.x - m_posDest.x) + (m_pos.z - m_posDest.z) * (m_pos.z - m_posDest.z);
	m_nBreaktime--;

	if (fLength > MOVE_ACCEPTABLE)
	{// ���������e�l���Ɏ��܂�܂ŖړI�n�Ɉړ�����
		m_move.x = sinf(atan2f(m_posDest.x - m_pos.x, m_posDest.z - m_pos.z)) * AI_SPEED;
		m_move.z = cosf(atan2f(m_posDest.x - m_pos.x, m_posDest.z - m_pos.z)) * AI_SPEED;
		m_rotDest.y = atan2f(m_posDest.x - m_pos.x, m_posDest.z - m_pos.z) + D3DX_PI;
	}
	else if (m_nBreaktime < 0)
	{// �ړ���x�e
		m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_nBreaktime = ENEMY_BREAKTIME;

		if (m_nCountPoint > 0 && m_nCountPoint == m_nPoint && !m_bGoal)
		{// �I���m�[�h�ɓ��������Ƃ�
			m_bGoal = true;
		}
	}
	else if (m_nBreaktime == 0)
	{// �x�e�I��
		if (m_nCountPoint > m_nPoint)
		{// �ڕW�n�_�ɓ��B���Ă��Ȃ��ꍇ
			m_nPoint++;
		}
	}

	if (m_AIAction[2] == AI_ACTION_ROUND_TRIP && m_nCountPoint <= m_nPoint)
	{// ������ԂŖڕW�n�_�ɓ��B���Ă���ꍇ
		m_nPoint = 0;
	}

	if (m_rotDest.y > D3DX_PI) { m_rotDest.y -= D3DX_PI * 2.0f; }
	if (m_rotDest.y < -D3DX_PI) { m_rotDest.y += D3DX_PI * 2.0f; }

	if (m_rot.y > D3DX_PI) { m_rot.y -= D3DX_PI * 2.0f; }
	if (m_rot.y < -D3DX_PI) { m_rot.y += D3DX_PI * 2.0f; }

	// �ڕW�����֌���
	m_rot.y += (m_rotDest.y - m_rot.y) * 0.1f;

	// �ʒu�̍X�V
	m_pos.x += m_move.x;
	m_pos.z += m_move.z;
}

//=============================================================================
//	�Ǐ]����
//=============================================================================
void CAIMecha::Follow()
{
	float fMinLength = 100000, fLength = 100000;	// �����n
	int nNearNode = 0;

	// �����̈ʒu�ɍł��߂��m�[�h����������
	for (int nCntNode = 0; nCntNode < m_NodeData.nodeMax; nCntNode++)
	{// �m�[�h�̐��������
	 // ���������߂�
		if (m_pPlayer != NULL)
		{// �v���C���[��NULL�`�F�b�N
			fLength = (m_NodeData.pos[nCntNode].x - m_pos.x) * (m_NodeData.pos[nCntNode].x - m_pos.x) + (m_NodeData.pos[nCntNode].z - m_pos.z) * (m_NodeData.pos[nCntNode].z - m_pos.z);

			if (fMinLength > fLength)
			{// �����̍ŏ��l�����߂�
				fMinLength = fLength;
				nNearNode = nCntNode;
			}
		}
	}

	// ���ݒn���J�n�m�[�h�ɐݒ肷��
	m_nStartNode = nNearNode;

	fMinLength = 100000;
	fLength = 100000;
	nNearNode = 0;

	// �����̎�l�̈ʒu�ɍł��߂��m�[�h����������
	for (int nCntNode = 0; nCntNode < m_NodeData.nodeMax; nCntNode++)
	{// �m�[�h�̐��������
	 // ���������߂�
		if (m_pPlayer != NULL)
		{// �v���C���[��NULL�`�F�b�N
			fLength = (m_NodeData.pos[nCntNode].x - m_pPlayer->GetPos().x) * (m_NodeData.pos[nCntNode].x - m_pPlayer->GetPos().x) + (m_NodeData.pos[nCntNode].z - m_pPlayer->GetPos().z) * (m_NodeData.pos[nCntNode].z - m_pPlayer->GetPos().z);

			if (fMinLength > fLength)
			{// �����̍ŏ��l�����߂�
				fMinLength = fLength;
				nNearNode = nCntNode;
			}
		}
	}

	// �O��̏���ۑ�
	m_nNodeOld = m_nEndNode;

	// �V�K�̖ړI�n��ݒ肷��
	m_nEndNode = nNearNode;

	if (m_nStartNode == m_nEndNode)
	{//	��l�̂��Ƃ֒H�蒅�����ꍇ
		m_nPoint = 0;
		m_nCountPoint = -1;

		for (int nCntAll = 0; nCntAll < m_NodeData.nodeMax; nCntAll++)
		{// �m�[�h�̐��������
			m_waypoint[nCntAll] = m_NodeData.pos[nNearNode];
		}
	}

	if (m_nNodeOld != m_nEndNode)
	{// ��l���O��̃m�[�h����ړ����Ă���ꍇ
		m_nPoint = 0;
		m_nCountPoint = -1;

		// ���������������Ă�m�[�h����������
		for (int nCntNode = 0; nCntNode < m_NodeData.nodeMax; nCntNode++)
		{// �m�[�h�̐��������
			fLength = (m_NodeData.pos[nCntNode].x - m_posDest.x) * (m_NodeData.pos[nCntNode].x - m_posDest.x) + (m_NodeData.pos[nCntNode].z - m_posDest.z) * (m_NodeData.pos[nCntNode].z - m_posDest.z);

			if (fMinLength > fLength)
			{// �����̍ŏ��l�����߂�
				fMinLength = fLength;
				nNearNode = nCntNode;
			}
		}

		// ���������������Ă�m�[�h���X�^�[�g�n�_�ɂ���
		m_nStartNode = nNearNode;

		for (int nCntAll = 0; nCntAll < m_NodeData.nodeMax; nCntAll++)
		{// �m�[�h�̐��������
			m_waypoint[nCntAll] = m_NodeData.pos[m_nStartNode];
		}

		// �|�C���g�ւ̌o�H�T��
		CAIMecha::RootSearch();
	}
}

//=============================================================================
//	�m�[�h��������
//=============================================================================
void CAIMecha::NodeSearch(bool node)
{
	float fMinLength = 100000, fLength = 100000;	// �����n
	int nNearNode = 0;

	if (node)
	{// �J�n�m�[�h�̐ݒ�
		fMinLength = 100000;
		fLength = 100000;
		nNearNode = 0;

		// �����̈ʒu�ɍł��߂��m�[�h����������
		for (int nCntNode = 0; nCntNode < m_NodeData.nodeMax; nCntNode++)
		{// �m�[�h�̐��������
		 // ���������߂�
			if (m_pPlayer != NULL)
			{// �v���C���[��NULL�`�F�b�N
				fLength = (m_NodeData.pos[nCntNode].x - m_pos.x) * (m_NodeData.pos[nCntNode].x - m_pos.x) + (m_NodeData.pos[nCntNode].z - m_pos.z) * (m_NodeData.pos[nCntNode].z - m_pos.z);

				if (fMinLength > fLength)
				{// �����̍ŏ��l�����߂�
					fMinLength = fLength;
					nNearNode = nCntNode;
				}
			}
		}

		// ���ݒn���J�n�m�[�h�ɐݒ肷��
		m_nStartNode = nNearNode;
		m_bGoal = false;
	}
	else if (!node)
	{// �I���m�[�h�̐ݒ�
		fMinLength = 100000;
		fLength = 100000;
		nNearNode = 0;

		// �w�肳�ꂽ�ʒu�ɍł��߂��m�[�h����������
		for (int nCntNode = 0; nCntNode < m_NodeData.nodeMax; nCntNode++)
		{// �m�[�h�̐��������
		 // ���������߂�
			fLength = (m_NodeData.pos[nCntNode].x - CManager::GetGame()->GetMouse()->Getsetpos().x) * (m_NodeData.pos[nCntNode].x - CManager::GetGame()->GetMouse()->Getsetpos().x) + (m_NodeData.pos[nCntNode].z - CManager::GetGame()->GetMouse()->Getsetpos().z) * (m_NodeData.pos[nCntNode].z - CManager::GetGame()->GetMouse()->Getsetpos().z);

			if (fMinLength > fLength)
			{// �����̍ŏ��l�����߂�
				fMinLength = fLength;
				nNearNode = nCntNode;
			}
		}

		// �O��̏���ۑ�
		m_nNodeOld = m_nEndNode;

		// ���������ʒu���I���m�[�h�ɐݒ肷��
		m_nEndNode = nNearNode;

		if (m_nRallyCount > 0)
		{// 1��ȏ�N���b�N���Ă���ꍇ
			m_nRallyEndNode[m_nRallyCount] = nNearNode;
		}
	}
}

//=============================================================================
//	�ŒZ�o�H��������
//=============================================================================
void CAIMecha::RootSearch()
{
	Node node[NODEPOINT_MAX];		// �m�[�h�̏��
	float weight[NODEPOINT_MAX];	// �e�G�b�W�̃R�X�g
	int nCntWeight = 0;				// �R�X�g�̃J�E���^
	std::vector<int> path;			// �ŒZ�o�H�̏���ێ�����vector

									//======= �G�b�W�R�X�g�̎Z�o =========================================================================
	for (int nCntNode = 0; nCntNode < m_NodeData.nodeMax; nCntNode++, nCntWeight++)
	{// �m�[�h�̐��������
		weight[nCntWeight] = sqrt((m_NodeData.pos[m_nStartNode].x - m_NodeData.pos[nCntNode].x) * (m_NodeData.pos[m_nStartNode].x - m_NodeData.pos[nCntNode].x) + (m_NodeData.pos[m_nStartNode].z - m_NodeData.pos[nCntNode].z) * (m_NodeData.pos[m_nStartNode].z - m_NodeData.pos[nCntNode].z));
	}

	//======= �G�b�W�ǉ� =========================================================================
	for (int nCntNode = 0; nCntNode < m_NodeData.nodeMax; nCntNode++)
	{// �m�[�h�̐��������
		for (int nCntConnect = 0; nCntConnect < m_NodeData.connectNum[nCntNode]; nCntConnect++)
		{// �q�����Ă�m�[�h�̐��������
			CAIMecha::AddEdge(nCntNode, m_NodeData.connectIndex[nCntNode][nCntConnect], weight[nCntNode], node);
		}
	}

	//======= �ŒZ�o�H�𒲂ׂ� =========================================================================
	CAIMecha::Dijkstra(m_NodeData.nodeMax, m_nStartNode, m_nEndNode, node);

	for (int nCntNode = m_nEndNode; nCntNode != m_nStartNode; nCntNode = node[nCntNode].from)
	{// �ŒZ�o�H���S�[�����珇�ɃX�^�[�g�܂ł��ǂ�
		path.push_back(nCntNode);
	}
	path.push_back(m_nStartNode);

	//======= �ŒZ�o�H�̏o�� =========================================================================
	for (int nCntNode = path.size() - 1; nCntNode >= 0; nCntNode--)
	{
		if (m_nCountPoint < nCntNode)
		{// �ő�l����
			m_nCountPoint = nCntNode;
		}
	}

	//======= �ڕW�n�_�̐ݒ� =========================================================================
	for (int nCntNodeMax = 0; nCntNodeMax < m_NodeData.nodeMax; )
	{// �m�[�h�̐��������
		for (int nCntNode = path.size() - 1; nCntNode >= 0; nCntNode--, nCntNodeMax++)
		{
			m_waypoint[nCntNodeMax] = m_NodeData.pos[path[nCntNode]];
		}
	}
}

//=============================================================================
//	�����[�o�H��������
//=============================================================================
void CAIMecha::RallyRootSearch()
{
	float weight[NODEPOINT_MAX];	// �e�G�b�W�̃R�X�g
	int nCntWeight = 0;		// �R�X�g�̃J�E���^
	std::vector<int> path;	// �ŒZ�o�H�̏���ێ�����vector

							//======= �G�b�W�R�X�g�̎Z�o =========================================================================
	for (int nCntNode = 0; nCntNode < m_NodeData.nodeMax; nCntNode++, nCntWeight++)
	{// �m�[�h�̐��������
		if (m_nRallyCount != 0)
		{// 2�|�C���g�ڈȍ~
			weight[nCntWeight] = sqrt(
				(m_NodeData.pos[m_nRallyEndNode[m_nRallyCount - 1]].x - m_NodeData.pos[nCntNode].x) *
				(m_NodeData.pos[m_nRallyEndNode[m_nRallyCount - 1]].x - m_NodeData.pos[nCntNode].x) +
				(m_NodeData.pos[m_nRallyEndNode[m_nRallyCount - 1]].z - m_NodeData.pos[nCntNode].z) *
				(m_NodeData.pos[m_nRallyEndNode[m_nRallyCount - 1]].z - m_NodeData.pos[nCntNode].z));
		}
		else
		{// 1�|�C���g��
			weight[nCntWeight] = sqrt(
				(m_NodeData.pos[m_nRallyEndNode[m_nRallyCount]].x - m_NodeData.pos[nCntNode].x) *
				(m_NodeData.pos[m_nRallyEndNode[m_nRallyCount]].x - m_NodeData.pos[nCntNode].x) +
				(m_NodeData.pos[m_nRallyEndNode[m_nRallyCount]].z - m_NodeData.pos[nCntNode].z) *
				(m_NodeData.pos[m_nRallyEndNode[m_nRallyCount]].z - m_NodeData.pos[nCntNode].z));
		}
	}

	//======= �G�b�W�ǉ� =========================================================================
	for (int nCntNode = 0; nCntNode < m_NodeData.nodeMax; nCntNode++)
	{// �m�[�h�̐��������
		for (int nCntConnect = 0; nCntConnect < m_NodeData.connectNum[nCntNode]; nCntConnect++)
		{// �q�����Ă�m�[�h�̐��������
			CAIMecha::AddEdge(nCntNode, m_NodeData.connectIndex[nCntNode][nCntConnect], weight[nCntNode], m_node[m_nRallyCount]);
		}
	}

	//======= �ŒZ�o�H�𒲂ׂ� =========================================================================
	if (m_nRallyCount > 1)
	{// 2�|�C���g�ڈȍ~
		for (int nCntRally = 1; nCntRally < m_nRallyCount; nCntRally++)
		{// 2�|�C���g�ȍ~�̐��������
		 // �_�C�N�X�g���@�ōŒZ�o�H�����߂�
			CAIMecha::Dijkstra(m_NodeData.nodeMax, m_nRallyEndNode[m_nRallyCount - nCntRally], m_nRallyEndNode[m_nRallyCount - (nCntRally - 1)], m_node[m_nRallyCount - (nCntRally - 1)]);

			for (int nCntNode = m_nRallyEndNode[m_nRallyCount - (nCntRally - 1)]; nCntNode != m_nRallyEndNode[m_nRallyCount - nCntRally]; nCntNode = m_node[m_nRallyCount - (nCntRally - 1)][nCntNode].from)
			{// �ŒZ�o�H���S�[�����珇�ɃX�^�[�g�܂ł��ǂ�
				path.push_back(nCntNode);
			}
			path.push_back(m_nRallyEndNode[m_nRallyCount - nCntRally]);
		}
	}

	if (m_nRallyCount > 0)
	{// 1�|�C���g��
	 // �_�C�N�X�g���@�ōŒZ�o�H�����߂�
		CAIMecha::Dijkstra(m_NodeData.nodeMax, m_nStartNode, m_nRallyEndNode[m_nRallyCount - (m_nRallyCount - 1)], m_node[m_nRallyCount - (m_nRallyCount - 1)]);

		for (int nCntNode = m_nRallyEndNode[m_nRallyCount - (m_nRallyCount - 1)]; nCntNode != m_nStartNode; nCntNode = m_node[m_nRallyCount - (m_nRallyCount - 1)][nCntNode].from)
		{// �ŒZ�o�H���S�[�����珇�ɃX�^�[�g�܂ł��ǂ�
			path.push_back(nCntNode);
		}
		path.push_back(m_nStartNode);
	}

	if (m_nRallyCount == 0)
	{// �X�^�[�g�ʒu
	 //�_�C�N�X�g���@�ōŒZ�o�H�����߂�
		CAIMecha::Dijkstra(m_NodeData.nodeMax, m_nStartNode, m_nRallyEndNode[m_nRallyCount], m_node[m_nRallyCount]);

		for (int nCntNode = m_nRallyEndNode[m_nRallyCount]; nCntNode != m_nStartNode; nCntNode = m_node[m_nRallyCount][nCntNode].from)
		{// �ŒZ�o�H���S�[�����珇�ɃX�^�[�g�܂ł��ǂ�
			path.push_back(nCntNode);
		}
		path.push_back(m_nStartNode);
	}

	//======= �ŒZ�o�H�̏o�� =========================================================================
	for (int nCntNode = path.size() - 1; nCntNode >= 0; nCntNode--)
	{
		if (m_nCountPoint < nCntNode)
		{// �ő�l����
			m_nCountPoint = nCntNode;
		}
	}

	//======= �ڕW�n�_�̐ݒ� =========================================================================
	for (int nCntNodeMax = 0; nCntNodeMax < m_NodeData.nodeMax; )
	{// �m�[�h�̐��������
		for (int nCntNode = path.size() - 1; nCntNode >= 0; nCntNode--, nCntNodeMax++)
		{// �S�[������H��
			m_waypoint[nCntNodeMax] = m_NodeData.pos[path[nCntNode]];
		}
	}
}

//=============================================================================
//	�����o�H��������
//=============================================================================
void CAIMecha::PatrolRootSearch()
{
	float weight[NODEPOINT_MAX];	// �e�G�b�W�̃R�X�g
	int nCntWeight = 0;		// �R�X�g�̃J�E���^
	std::vector<int> path;	// �ŒZ�o�H�̏���ێ�����vector

							//======= �G�b�W�R�X�g�̎Z�o =========================================================================
	for (int nCntNode = 0; nCntNode < m_NodeData.nodeMax; nCntNode++, nCntWeight++)
	{// �m�[�h�̐��������
		if (m_nRallyCount != 0)
		{// 2�|�C���g�ڈȍ~
			weight[nCntWeight] = sqrt(
				(m_NodeData.pos[m_nRallyEndNode[m_nRallyCount - 1]].x - m_NodeData.pos[nCntNode].x) *
				(m_NodeData.pos[m_nRallyEndNode[m_nRallyCount - 1]].x - m_NodeData.pos[nCntNode].x) +
				(m_NodeData.pos[m_nRallyEndNode[m_nRallyCount - 1]].z - m_NodeData.pos[nCntNode].z) *
				(m_NodeData.pos[m_nRallyEndNode[m_nRallyCount - 1]].z - m_NodeData.pos[nCntNode].z));
		}
		else
		{// 1�|�C���g��
			weight[nCntWeight] = sqrt(
				(m_NodeData.pos[m_nRallyEndNode[m_nRallyCount]].x - m_NodeData.pos[nCntNode].x) *
				(m_NodeData.pos[m_nRallyEndNode[m_nRallyCount]].x - m_NodeData.pos[nCntNode].x) +
				(m_NodeData.pos[m_nRallyEndNode[m_nRallyCount]].z - m_NodeData.pos[nCntNode].z) *
				(m_NodeData.pos[m_nRallyEndNode[m_nRallyCount]].z - m_NodeData.pos[nCntNode].z));
		}

	}

	//======= �G�b�W�ǉ� =========================================================================
	for (int nCntNode = 0; nCntNode < m_NodeData.nodeMax; nCntNode++)
	{// �m�[�h�̐��������
		for (int nCntConnect = 0; nCntConnect < m_NodeData.connectNum[nCntNode]; nCntConnect++)
		{// �q�����Ă�m�[�h�̐��������
			CAIMecha::AddEdge(nCntNode, m_NodeData.connectIndex[nCntNode][nCntConnect], weight[nCntNode], m_node[m_nRallyCount]);
		}
	}

	//======= �t���ŒZ�o�H�𒲂ׂ� =========================================================================
	if (m_nRallyCount == 0)
	{// �X�^�[�g�ʒu
	 //�_�C�N�X�g���@�ōŒZ�o�H�����߂�
		CAIMecha::Dijkstra(m_NodeData.nodeMax, m_nRallyEndNode[m_nRallyCount], m_nStartNode, m_node[m_nRallyCount]);

		for (int nCntNode = m_nStartNode; nCntNode != m_nRallyEndNode[m_nRallyCount]; nCntNode = m_node[m_nRallyCount][nCntNode].from)
		{// �ŒZ�o�H���X�^�[�g���珇�ɃS�[���܂ł��ǂ�
			path.push_back(nCntNode);
		}
		path.push_back(m_nRallyEndNode[m_nRallyCount]);
	}

	if (m_nRallyCount > 0)
	{// 1�|�C���g��
	 // �_�C�N�X�g���@�ōŒZ�o�H�����߂�
		CAIMecha::Dijkstra(m_NodeData.nodeMax, m_nRallyEndNode[m_nRallyCount - (m_nRallyCount - 1)], m_nStartNode, m_node[m_nRallyCount - (m_nRallyCount - 1)]);

		for (int nCntNode = m_nStartNode; nCntNode != m_nRallyEndNode[m_nRallyCount - (m_nRallyCount - 1)]; nCntNode = m_node[m_nRallyCount - (m_nRallyCount - 1)][nCntNode].from)
		{// �ŒZ�o�H���X�^�[�g���珇�ɃS�[���܂ł��ǂ�
			path.push_back(nCntNode);
		}
		path.push_back(m_nRallyEndNode[m_nRallyCount - (m_nRallyCount - 1)]);
	}

	if (m_nRallyCount > 1)
	{// 2�|�C���g�ڈȍ~
		for (int nCntRally = m_nRallyCount - 1; nCntRally >= 1; nCntRally--)
		{// 2�|�C���g�ȍ~�̐��������
		 // �_�C�N�X�g���@�ōŒZ�o�H�����߂�
			CAIMecha::Dijkstra(m_NodeData.nodeMax, m_nRallyEndNode[m_nRallyCount - (nCntRally - 1)], m_nRallyEndNode[m_nRallyCount - nCntRally], m_node[m_nRallyCount - (nCntRally - 1)]);

			for (int nCntNode = m_nRallyEndNode[m_nRallyCount - nCntRally]; nCntNode != m_nRallyEndNode[m_nRallyCount - (nCntRally - 1)]; nCntNode = m_node[m_nRallyCount - (nCntRally - 1)][nCntNode].from)
			{// �ŒZ�o�H���X�^�[�g���珇�ɃS�[���܂ł��ǂ�
				path.push_back(nCntNode);
			}
			path.push_back(m_nRallyEndNode[m_nRallyCount - (nCntRally - 1)]);
		}
	}

	//======= �����ŒZ�o�H�𒲂ׂ� =========================================================================
	if (m_nRallyCount > 1)
	{// 2�|�C���g�ڈȍ~
		for (int nCntRally = 1; nCntRally < m_nRallyCount; nCntRally++)
		{// 2�|�C���g�ȍ~�̐��������
		 // �_�C�N�X�g���@�ōŒZ�o�H�����߂�
			CAIMecha::Dijkstra(m_NodeData.nodeMax, m_nRallyEndNode[m_nRallyCount - nCntRally], m_nRallyEndNode[m_nRallyCount - (nCntRally - 1)], m_node[m_nRallyCount - (nCntRally - 1)]);

			for (int nCntNode = m_nRallyEndNode[m_nRallyCount - (nCntRally - 1)]; nCntNode != m_nRallyEndNode[m_nRallyCount - nCntRally]; nCntNode = m_node[m_nRallyCount - (nCntRally - 1)][nCntNode].from)
			{// �ŒZ�o�H���S�[�����珇�ɃX�^�[�g�܂ł��ǂ�
				path.push_back(nCntNode);
			}
			path.push_back(m_nRallyEndNode[m_nRallyCount - nCntRally]);
		}
	}

	if (m_nRallyCount > 0)
	{// 1�|�C���g��
	 // �_�C�N�X�g���@�ōŒZ�o�H�����߂�
		CAIMecha::Dijkstra(m_NodeData.nodeMax, m_nStartNode, m_nRallyEndNode[m_nRallyCount - (m_nRallyCount - 1)], m_node[m_nRallyCount - (m_nRallyCount - 1)]);

		for (int nCntNode = m_nRallyEndNode[m_nRallyCount - (m_nRallyCount - 1)]; nCntNode != m_nStartNode; nCntNode = m_node[m_nRallyCount - (m_nRallyCount - 1)][nCntNode].from)
		{// �ŒZ�o�H���S�[�����珇�ɃX�^�[�g�܂ł��ǂ�
			path.push_back(nCntNode);
		}
		path.push_back(m_nStartNode);
	}

	if (m_nRallyCount == 0)
	{// �X�^�[�g�ʒu
	 //�_�C�N�X�g���@�ōŒZ�o�H�����߂�
		CAIMecha::Dijkstra(m_NodeData.nodeMax, m_nStartNode, m_nRallyEndNode[m_nRallyCount], m_node[m_nRallyCount]);

		for (int nCntNode = m_nRallyEndNode[m_nRallyCount]; nCntNode != m_nStartNode; nCntNode = m_node[m_nRallyCount][nCntNode].from)
		{// �ŒZ�o�H���S�[�����珇�ɃX�^�[�g�܂ł��ǂ�
			path.push_back(nCntNode);
		}
		path.push_back(m_nStartNode);
	}

	//======= �ŒZ�o�H�̏o�� =========================================================================
	for (int nCntNode = path.size() - 1; nCntNode >= 0; nCntNode--)
	{
		if (m_nCountPoint < nCntNode)
		{// �ő�l����
			m_nCountPoint = nCntNode;
		}
	}

	//======= �ڕW�n�_�̐ݒ� =========================================================================
	for (int nCntNodeMax = 0; nCntNodeMax < m_NodeData.nodeMax; )
	{// �m�[�h�̐��������
		for (int nCntNode = path.size() - 1; nCntNode >= 0; nCntNode--, nCntNodeMax++)
		{// �S�[������H��
			m_waypoint[nCntNodeMax] = m_NodeData.pos[path[nCntNode]];
		}
	}
}

//=============================================================================
//	���f����
//=============================================================================
void CAIMecha::Cancel()
{
	if (m_bPartSwitchOld == CGame::PART_ACTION && m_bPartSwitch == CGame::PART_STRATEGY)
	{// �X�g���e�W�[�֕ύX�����ꍇ
		int nNear = 0;
		float fMinLength = 100000, fLength = 100000;
		for (int nCntAll = 0; nCntAll < m_NodeData.nodeMax; nCntAll++)
		{// �S�Ẵm�[�h�ƃv���C���[�Ƃ̍������o��
			fLength = (m_pos.x - m_NodeData.pos[nCntAll].x) * (m_pos.x - m_NodeData.pos[nCntAll].x) + (m_pos.z - m_NodeData.pos[nCntAll].z) * (m_pos.z - m_NodeData.pos[nCntAll].z);
			if (fMinLength > fLength)
			{// �����̍ŏ��l�����߂�
				fMinLength = fLength;
				nNear = nCntAll;
			}
		}

		// ��ԋ߂��m�[�h�Œ�~���ăf�[�^��������
		m_bGoal = false;
		m_nRallyCount = 0;
		m_nPoint = 0;
		m_nCountPoint = -1;
		m_nStartNode = nNear;
		m_nEndNode = nNear;
		for (int nCntRally = 0; nCntRally < RALLYPOINT_MAX; nCntRally++)
		{// �����[�|�C���g�̍ő吔�����
			for (int nCntNode = 0; nCntNode < NODEPOINT_MAX; nCntNode++)
			{// �m�[�h�̍ő吔�����
				m_node[nCntRally][nCntNode] = {};
			}
		}

		for (int nCntAll = 0; nCntAll < m_NodeData.nodeMax; nCntAll++)
		{// �m�[�h�̐��������
			m_nRallyEndNode[nCntAll] = nNear;
			m_waypoint[nCntAll] = m_NodeData.pos[nNear];
		}

		for (int nCntAction = 0; nCntAction < 4; nCntAction++)
		{// �s�����̕����
			m_AIAction[nCntAction] = AI_ACTION_NONE;
			m_LogicTree[nCntAction] = -1;
		}
		m_posDest = m_NodeData.pos[nNear];
	}
}

//=============================================================================
// �G�b�W�ǉ�����
//=============================================================================
void CAIMecha::AddEdge(int first, int second, float weight, Node *node)
{
	// �m�[�hu�̓m�[�hv�ƂȂ����Ă����������
	node[second].to.push_back(first);
	// �m�[�hu�ƃm�[�hv�̃G�b�W�̏d�݂�����
	node[second].cost.push_back(weight);
}

//=============================================================================
// �_�C�N�X�g���@
//=============================================================================
void CAIMecha::Dijkstra(int nodeMax, int start, int end, Node *node)
{
	// �ϐ��̏�����
	for (int nCntNode = 0; nCntNode < nodeMax; nCntNode++)
	{
		node[nCntNode].done = false;
		node[nCntNode].minCost = -1;
	}

	node[start].minCost = 0;	// �X�^�[�g�m�[�h�܂ł̃R�X�g��0
	while (1)
	{
		int doneNode = -1;	// �ŐV�̊m�肵���m�[�h�ԍ�(-1��NULL�̂����)
		for (int nCntNode = 0; nCntNode < nodeMax; nCntNode++)
		{
			if (node[nCntNode].done == true)
			{// �m�[�hi���m�肵�Ă���Ƃ�continue
				continue;
			}

			if (node[nCntNode].minCost < 0)
			{// �m�[�hi�܂ł̌����_�ł̍ŏ��R�X�g���s���̂Ƃ�
				continue;
			}

			// �m�肵���m�[�h�ԍ���-1���m�[�hi�̌����_�̍ŏ��R�X�g���������Ƃ�
			// �m��m�[�h�ԍ����X�V����
			if (doneNode < 0 || node[nCntNode].minCost < node[doneNode].minCost)
			{
				doneNode = nCntNode;
			}
		}

		if (doneNode == -1)
		{
			break;	// ���ׂẴm�[�h���m�肵����I��
		}

		node[doneNode].done = true;	// �m�[�h���m�肳����

		for (int nCntNodeSize = 0; nCntNodeSize < (int)node[doneNode].to.size(); nCntNodeSize++)
		{
			int to = node[doneNode].to[nCntNodeSize];
			float cost = node[doneNode].minCost + node[doneNode].cost[nCntNodeSize];

			// �m�[�hto�͂܂��K��Ă��Ȃ��m�[�h
			// �܂��̓m�[�hto�ւ�菬�����R�X�g�̌o�H��������
			// �m�[�hto�̍ŏ��R�X�g���X�V
			if (node[to].minCost < 0.0f || cost < node[to].minCost)
			{
				node[to].minCost = cost;
				node[to].from = doneNode;
			}
		}
	}
}

//=============================================================================
// ���[�g�T���p�t�@�C���̓ǂݍ���
//=============================================================================
void CAIMecha::FileLoad(char* pFileName)
{
	FILE* pFile = NULL;		// �t�@�C���|�C���^
	char ReadText[256];		// �ǂݍ��񂾕���������Ă���
	char HeadText[256];		// ��r�p
	char DustBox[256];		// �g�p���Ȃ����̂����Ă���
	int nCount = 0;
	int nCntIndex = 0;

	// �ꎞ�f�[�^�x�[�X
	std::vector<NodeState> LoadState; LoadState.clear();

	// ������
	NodeState OneState = {};

	// �t�@�C���I�[�v��
	pFile = fopen(LOAD_FILENAME, "r");

	if (pFile != NULL)
	{// �t�@�C�����J����Ă����
		while (strcmp(HeadText, "START_LOAD") != 0)
		{// "START_LOAD" ���ǂݍ��܂��܂ŌJ��Ԃ��������ǂݎ��
			fgets(ReadText, sizeof(ReadText), pFile);
			sscanf(ReadText, "%s", &HeadText);
		}
		if (strcmp(HeadText, "START_LOAD") == 0)
		{// "START_LOAD" ���ǂݎ�ꂽ�ꍇ�A�����J�n
			while (strcmp(HeadText, "END_LOAD") != 0)
			{// "END_LOAD" ���ǂݍ��܂��܂ŌJ��Ԃ��������ǂݎ��
				fgets(ReadText, sizeof(ReadText), pFile);
				sscanf(ReadText, "%s", &HeadText);

				if (strcmp(HeadText, "\n") == 0)
				{// ������̐擪�� [\n](���s) �̏ꍇ�������Ȃ�

				}
				else if (strcmp(HeadText, "START_DATA") == 0)
				{// "START_DATA" ���ǂݎ�ꂽ�ꍇ
					nCount = 0;
					while (strcmp(HeadText, "END_DATA") != 0)
					{// "END_DATA" ���ǂݍ��܂��܂ŌJ��Ԃ��������ǂݎ��
						fgets(ReadText, sizeof(ReadText), pFile);
						sscanf(ReadText, "%s", &HeadText);

						if (strcmp(HeadText, "\n") == 0)
						{// ������̐擪�� [\n](���s) �̏ꍇ�������Ȃ�

						}
						else if (strcmp(HeadText, "NODESET") == 0)
						{// "NODESET" ���ǂݎ�ꂽ�ꍇ
							while (strcmp(HeadText, "END_NODESET") != 0)
							{// "END_NODESET" ���ǂݍ��܂��܂ŌJ��Ԃ��������ǂݎ��
								fgets(ReadText, sizeof(ReadText), pFile);
								sscanf(ReadText, "%s", &HeadText);

								if (strcmp(HeadText, "\n") == 0)
								{// ������̐擪�� [\n](���s) �̏ꍇ�������Ȃ�

								}
								else if (strcmp(HeadText, "NODE_POS") == 0)	// �m�[�h�̈ʒu
								{
									sscanf(ReadText, "%s %c %f %f %f",
										&DustBox, &DustBox,
										&OneState.pos[nCount].x,
										&OneState.pos[nCount].y,
										&OneState.pos[nCount].z);
								}
								else if (strcmp(HeadText, "CONNECT_NUM") == 0)	// �ڑ��m�[�h�̐�
								{
									sscanf(ReadText, "%s %c %d",
										&DustBox, &DustBox,
										&OneState.connectNum[nCount]);
								}
								else if (strcmp(HeadText, "CONNECT_INDEX") == 0)	// �ڑ��m�[�h�̔ԍ�
								{
									sscanf(ReadText, "%s %c %d",
										&DustBox, &DustBox,
										&OneState.connectIndex[nCount][nCntIndex]);
									nCntIndex++;
								}
							}

							OneState.index[nCount] = nCount;
							nCntIndex = 0;
							nCount++;
						}
					}
					OneState.nodeMax = nCount; // �m�[�h�̑���

											   // ��̃f�[�^��ǂݍ��񂾌�,�ꎞ�f�[�^�x�[�X�Ɋi�[
					LoadState.emplace_back(OneState);
				}
			}
		}

		// �t�@�C���N���[�Y
		if (pFile != NULL)
		{
			fclose(pFile);
			pFile = NULL;
		}
	}

	m_NodeData = OneState;	// �f�[�^�̑��
}