//=============================================================================
//
// AI���� [AI.cpp]
// Author : Komatsu Keisuke
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "AI.h"
#include "player.h"
#include "manager.h"
#include "camera.h"
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
#include "menu.h"
#include "nodeDataFiler.h"
#include "search.h"
#include "particle.h"
#include "game.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define WALKER_FILE	"data/TEXT/AI/walker/model_walker.txt"
#define DRONE_FILE	"data/TEXT/AI/drone/model_drone.txt"
#define AI_SPEED			(2.7f)		// �ړ����x

// �ړ��nAI�֘A
#define	LOAD_FILENAME		("data/TEXT/NODE_DATA/NodeData.txt")
//#define	LOAD_FILENAME	("data/TEXT/NODE_DATA/NodeDataTutorial.txt")	// �ǂݍ��ރt�@�C���̃p�X
#define ENEMY_BREAKTIME		(1)			// �x�e����(�t���[��)
#define MOUSE_ACCEPTABLE	(20.0f)		// �}�E�X�̌덷�̋��e�͈�
#define MOVE_ACCEPTABLE		(25.0f)		// �ړ��̌덷�̋��e�͈�
#define POS_ACCEPTABLE		(100.0f)	// �ʒu�̌덷�̋��e�͈�

// �퓬�nAI�֘A
#define ATTACK_DISPERTION	(50)			// �e�̃u��
#define ATTACK_AREA			(70000.0f)		// �U���͈�
#define MAX_CHAR			(254)			// �ǂݎ�镶����
#define MAX_SEARCH			(4)				// �Z���T�[��
#define FIND_FIND_CHARACTER_PRIORITY (4)	// �T���v���C���[�̗D�揇��

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================

//=============================================================================
// ��������
//=============================================================================
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

//=============================================================================
// �R���X�g���N�^
//=============================================================================
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
	m_bDeath = false;
	m_nKillPlayerIdx = 0;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CAIMecha::~CAIMecha() {}

//=============================================================================
// ����������
//=============================================================================
HRESULT CAIMecha::Init(void)
{
	srand((unsigned int)time(0));

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

	// �퓬�nAI���l�̏�����
	m_bFind = false;

	// �ړ��nAI���l�̏�����
	m_pNodeData = CGame::GetNodeFiler();	// �t�@�C�����̎擾

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

	// �p�g���[���֌W
	m_bPatrol = false;
	m_nPatrolStartNode = 0;
	m_nCountPatrolPoint = 0;

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
		m_patrolWaypoint[nCntNode] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	for (int nCntAction = 0; nCntAction < 4; nCntAction++)
	{// �s�����̕����
		m_AIAction[nCntAction] = AI_ACTION_NONE;
		m_LogicTree[nCntAction] = -1;
	}

	// �}�b�v�f�[�^�t�@�C���̓ǂݍ���
	m_pNodeData->FileLoad(LOAD_FILENAME);

	// �J�n���_�̃m�[�h�̏�����
	float fMinLength = 100000, fLength = 100000;	// �����n
	for (int nCntNode = 0; nCntNode < m_pNodeData->GetLoadData().nodeMax; nCntNode++)
	{// �m�[�h�̐��������
	 // ���������߂�
		fLength = (m_pNodeData->GetLoadData().pos[nCntNode].x - m_pos.x) * (m_pNodeData->GetLoadData().pos[nCntNode].x - m_pos.x) + (m_pNodeData->GetLoadData().pos[nCntNode].z - m_pos.z) * (m_pNodeData->GetLoadData().pos[nCntNode].z - m_pos.z);

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

//=============================================================================
// �I������
//=============================================================================
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

//=============================================================================
// �X�V����
//=============================================================================
void CAIMecha::Update(void)
{
	m_posOld = m_pos;

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

	if (m_bDeath == false)
	{
		for (int nCntModel = 0; nCntModel < m_nNumParts; nCntModel++)
		{//�\�����Ȃ�
			m_pModel[nCntModel]->SetDisp(false);
		}

		// AI�֌W�̍X�V����
		CAIMecha::AIUpdate();

	}
}

//=============================================================================
// �`�揈��
//=============================================================================
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
void CAIMecha::Damage(int nDamage, CScene *pScene)
{
	if (CMenu::GetMode() == CMenu::MODE_SINGLE)
	{//�V���O���v���C�̏ꍇ
		if (m_nLife > 0 && m_bDeath == false)
		{
			m_state = STATE_DAMAGE;	// �_���[�W���󂯂Ă����Ԃɂ���

			m_nLife -= nDamage;	//�̗͂̌��Z

			if (0 >= m_nLife)
			{//�̗͂��O�ȉ��̏ꍇ
				m_nLife = 0;			//�̗͂��O�ɂ���
				m_bDeath = true;		//���S��Ԃɂ���

				if (m_bDeath == true && CManager::GetMode() == CManager::MODE_GAME)
				{//���S���Ă���ꍇ
					for (int nCntKill = 0; nCntKill < NUM_KILL_LOG; nCntKill++)
					{
						//�L�����O�̕\������
						if (CManager::GetGame()->GetLog(nCntKill) == false)
						{//���O���g�p����Ă��Ȃ��ꍇ
							if (pScene->GetObjType() == CScene::OBJTYPE_PLAYER)
							{//�I�u�W�F�N�g�̎�ނ��v���C���[�̏ꍇ
								CPlayer *pPlayer = (CPlayer*)pScene;
								if (pPlayer != NULL)
								{//NULL�ł͂Ȃ��ꍇ
									m_nKillPlayerIdx = pPlayer->GetPlayerIdx();									//�L���v���C���[�̔ԍ���ݒu����
									CManager::GetGame()->SetKillIdx(nCntKill, pPlayer->GetPlayerIdx());			//�L���v���C���[�̔ԍ���ݒu����
									CManager::GetGame()->SetDeathIdx(nCntKill, m_pPlayer->GetPlayerIdx());		//�f�X�v���C���[�̔ԍ���ݒu����
									CManager::GetGame()->SetPlayerType(0, CGame::TYPE_PLAYER);					//�v���C���[�̎�ނ�ݒu����
									CManager::GetGame()->SetPlayerType(1, CGame::TYPE_DROWN);					//�v���C���[�̎�ނ�ݒu����
									CManager::GetGame()->SetLog(nCntKill, true);								//���O�̐ݒu����
								}
							}
							else if (pScene->GetObjType() == CScene::OBJTYPE_AI)
							{//�I�u�W�F�N�g�̎�ނ��`�h�̏ꍇ
								CAIMecha *pAIMecha = (CAIMecha*)pScene;
								if (pAIMecha != NULL)
								{//NULL�̏ꍇ
									m_nKillPlayerIdx = pAIMecha->GetPlayer()->GetPlayerIdx();								//�L���v���C���[�̔ԍ���ݒu����
									CManager::GetGame()->SetKillIdx(nCntKill, pAIMecha->GetPlayer()->GetPlayerIdx());		//�L���v���C���[�̔ԍ���ݒu����
									CManager::GetGame()->SetDeathIdx(nCntKill, m_pPlayer->GetPlayerIdx());					//�f�X�v���C���[�̔ԍ���ݒu����
									if (pAIMecha->GetMechaType() == CAIMecha::MECHATYPE_DRONE)
									{//�I�u�W�F�N�g�̎�ނ��h���[���̏ꍇ
										CManager::GetGame()->SetPlayerType(0, CGame::TYPE_DROWN);							//�v���C���[�̎�ނ�ݒu����
										CManager::GetGame()->SetPlayerType(1, CGame::TYPE_DROWN);							//�v���C���[�̎�ނ�ݒu����
										CManager::GetGame()->SetLog(nCntKill, true);										//���O�̐ݒu����
									}
									else if (pAIMecha->GetMechaType() == CAIMecha::MECHATYPE_WALKER)
									{//�I�u�W�F�N�g�̎�ނ����[�J�[�̏ꍇ
										CManager::GetGame()->SetPlayerType(0, CGame::TYPE_WALKER);							//�v���C���[�̎�ނ�ݒu����
										CManager::GetGame()->SetPlayerType(1, CGame::TYPE_DROWN);							//�v���C���[�̎�ނ�ݒu����
										CManager::GetGame()->SetLog(nCntKill, true);										//���O�̐ݒu����
									}
								}
							}
							break;
						}
					}
				}

				//�p�[�e�B�N���𐶐�
				CParticle::Create(m_pModel[0]->GetWorldPos(), 4);
				CParticle::Create(m_pModel[0]->GetWorldPos(), 5);

				for (int nCntModel = 0; nCntModel < m_nNumParts; nCntModel++)
				{//�\�����Ȃ�
					m_pModel[nCntModel]->SetDisp(false);
				}

				//�`�[���ʂŏ�������
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
		}
	}
	else
	{//�}���`�v���C�̏ꍇ
		if (CManager::GetClient() != NULL)
		{//NULL�ł͂Ȃ��ꍇ
			if (CManager::GetClient()->GetPlayerIdx() == m_pPlayer->GetPlayerIdx())
			{//�v���C���[�ԍ��ƃN���C�A���g�ԍ��������ꍇ
				if (m_nLife > 0 && m_bDeath == false)
				{//�̗͂��O���傫�������S���Ă��Ȃ��ꍇ
					m_state = STATE_DAMAGE;								// �_���[�W���󂯂Ă����Ԃɂ���

					m_nLife -= nDamage;									// �̗͂̌��Z

					if (0 >= m_nLife)
					{//�̗͂��O�ȉ��̏ꍇ
						m_nLife = 0;		//�̗͂��O�ɂ���
						m_bDeath = true;	//���S��Ԃɂ���

						if (m_bDeath == true && CManager::GetMode() == CManager::MODE_GAME)
						{//���S���Ă���ꍇ
							for (int nCntKill = 0; nCntKill < NUM_KILL_LOG; nCntKill++)
							{
								//�L�����O�̕\������
								if (CManager::GetGame()->GetLog(nCntKill) == false)
								{//���O���g�p����Ă��Ȃ��ꍇ
									if (pScene->GetObjType() == CScene::OBJTYPE_PLAYER)
									{//�I�u�W�F�N�g�̎�ނ��v���C���[�̏ꍇ
										CPlayer *pPlayer = (CPlayer*)pScene;
										if (pPlayer != NULL)
										{
											m_nKillPlayerIdx = pPlayer->GetPlayerIdx();								//�L���v���C���[�̔ԍ���ݒu����
											CManager::GetGame()->SetKillIdx(nCntKill, pPlayer->GetPlayerIdx());		//�L���v���C���[�̔ԍ���ݒu����
											CManager::GetGame()->SetDeathIdx(nCntKill, m_pPlayer->GetPlayerIdx());	//�f�X�v���C���[�̔ԍ���ݒu����
											CManager::GetGame()->SetPlayerType(0, CGame::TYPE_PLAYER);				//�v���C���[�̎�ނ�ݒu����
											CManager::GetGame()->SetPlayerType(1, CGame::TYPE_DROWN);				//�v���C���[�̎�ނ�ݒu����
											CManager::GetGame()->SetLog(nCntKill, true);							//���O�̐ݒu����
										}
									}
									else if (pScene->GetObjType() == CScene::OBJTYPE_AI)
									{//�I�u�W�F�N�g�̎�ނ�AI�̏ꍇ
										CAIMecha *pAIMecha = (CAIMecha*)pScene;
										if (pAIMecha != NULL)
										{
											m_nKillPlayerIdx = pAIMecha->GetPlayer()->GetPlayerIdx();							//�L���v���C���[�̔ԍ���ݒu����
											CManager::GetGame()->SetKillIdx(nCntKill, pAIMecha->GetPlayer()->GetPlayerIdx());	//�L���v���C���[�̔ԍ���ݒu����
											CManager::GetGame()->SetDeathIdx(nCntKill, m_pPlayer->GetPlayerIdx());				//�f�X�v���C���[�̔ԍ���ݒu����
											if (pAIMecha->GetMechaType() == CAIMecha::MECHATYPE_DRONE)
											{//�I�u�W�F�N�g�̎�ނ��h���[���̏ꍇ
												CManager::GetGame()->SetPlayerType(0, CGame::TYPE_DROWN);						//�v���C���[�̎�ނ�ݒu����
												CManager::GetGame()->SetPlayerType(1, CGame::TYPE_DROWN);						//�v���C���[�̎�ނ�ݒu����
												CManager::GetGame()->SetLog(nCntKill, true);									//���O�̐ݒu����
											}
											else if (pAIMecha->GetMechaType() == CAIMecha::MECHATYPE_WALKER)
											{//�I�u�W�F�N�g�̎�ނ����[�J�[�̏ꍇ
												CManager::GetGame()->SetPlayerType(0, CGame::TYPE_WALKER);						//�v���C���[�̎�ނ�ݒu����
												CManager::GetGame()->SetPlayerType(1, CGame::TYPE_DROWN);						//�v���C���[�̎�ނ�ݒu����
												CManager::GetGame()->SetLog(nCntKill, true);									//���O�̐ݒu����
											}
										}
									}
									break;
								}
							}
						}

						//�p�[�e�B�N���𐶐�
						CParticle::Create(m_pModel[0]->GetWorldPos(), 4);
						CParticle::Create(m_pModel[0]->GetWorldPos(), 5);

						for (int nCntModel = 0; nCntModel < m_nNumParts; nCntModel++)
						{
							//�\�������Ȃ�����
							m_pModel[nCntModel]->SetDisp(false);
						}

						//�`�[���ʂŏ�������
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
				}
			}
		}
	}
}

//=============================================================================
//	AI�X�V����
//=============================================================================
void CAIMecha::AIUpdate()
{
	// �p�[�g���
	//CDebugProc::Print("========AI========\n");
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
	//{// �����ړ���
	//	CDebugProc::Print("�����ړ����\n");
	//	CDebugProc::Print("�����ړ��J�n�n�_: %d\n", m_nStartNode);
	//	CDebugProc::Print("�J�n�n�_ : %d\n", m_nRallyEndNode[0]);
	//	for (int nCntRally = 1; nCntRally < m_nRallyCount; nCntRally++)
	//	{
	//		CDebugProc::Print("���Ԓn�_[%d] : %d\n", nCntRally, m_nRallyEndNode[nCntRally]);
	//	}
	//	CDebugProc::Print("�I���n�_ : %d\n", m_nRallyEndNode[m_nRallyCount]);
	//	CDebugProc::Print("\n");
	//}
	//else if (m_AIAction[1] == AI_ACTION_FOLLOW)
	//{// �Ǐ]��
	//	CDebugProc::Print("�Ǐ]���\n");
	//	CDebugProc::Print("�J�n�n�_ : %d\n", m_nStartNode);
	//	CDebugProc::Print("�I���n�_ : %d\n", m_nEndNode);
	//	CDebugProc::Print("\n");
	//}

	//if (m_AIAction[2] != AI_ACTION_ROUND_TRIP)
	//{// �����ړ��ȊO
	//	CDebugProc::Print("���݂̈ړ��� : %d\n", m_nPoint);
	//	CDebugProc::Print("�ڕW�܂ł̈ړ��� : %d\n", m_nCountPoint);
	//	CDebugProc::Print("\n");
	//}
	//else
	//{// �����ړ���
	//	CDebugProc::Print("���݂̈ړ��� : %d\n", m_nPoint);
	//	CDebugProc::Print("�ڕW�܂ł̈ړ��� : %d\n", m_nCountPatrolPoint);
	//	CDebugProc::Print("\n");
	//}

	//CDebugProc::Print("m_bFind : %s\n", m_bFind ? "true" : "false");

	CInputMouse *pMouse = CManager::GetInputMouse();			// �}�E�X�̓��͂��擾
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();	// �L�[�{�[�h�̓��͂��擾

																// �O��̃p�[�g���̕ۑ�
	m_bPartSwitchOld = m_bPartSwitch;
	// ���݂̃p�[�g�����擾
	m_bPartSwitch = CManager::GetGame()->GetPart();

	// ���f����
	Cancel();

	// �U���֌W����
	Attack();

	// AI�s���ݒ�
	AIActionSet(pMouse);

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
			if (m_AIAction[2] == AI_ACTION_ROUND_TRIP)// �����ړ���
			{// �|�C���g�Ԃ�p�j����o�H�T��
				CAIMecha::RootSearch();
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
//	AI�s���ݒ菈��
//=============================================================================
void CAIMecha::AIActionSet(CInputMouse *pMouse)
{
	if (m_bPartSwitch == CGame::PART_STRATEGY && CManager::GetGame()->GetButtonManager() != NULL)
	{// �X�g���e�W�[�p�[�g���Ń{�^���}�l�[�W���[��NULL����Ȃ��ꍇ
		if (CManager::GetGame()->GetButtonManager()->GetSelectFinish() == true)
		{// AI�̍s�������肵�Ă���ꍇ
			if (CManager::GetGame()->GetButtonManager()->GetSelectAIType() == m_mechaType)
			{// �����̃��J�^�C�v���I������Ă���ꍇ
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
				else if (m_LogicTree[2] == 2)// �����ړ�
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

	// �s�����Ƃ̏���
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
				if (pMouse->GetTrigger(CInputMouse::DIMS_BUTTON_0) == true && m_bPartSwitch == CGame::PART_STRATEGY)
				{// �X�g���e�W�[�p�[�g�ɍ��N���b�N���ꂽ�ꍇ
					if (m_nRallyCount < RALLYPOINT_MAX)
					{// �J�E���g���ő吔�܂œ��B���Ă��Ȃ��ꍇ
					 // �O��̏��̕ۑ�
						m_nRallyCountOld = m_nRallyCount;

						if (m_AIAction[2] == AI_ACTION_ROUND_TRIP || m_AIAction[2] == AI_ACTION_RALLY)
						{// �����[���܂��͉����ړ���
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
}

//=============================================================================
//	�U���֌W����
//=============================================================================
void CAIMecha::Attack()
{
	CScene *pScene = CScene::GetSceneTop(PLAYER_PRIORITY);
	CScene *pSceneNext = NULL;
	int nCntEnemyPlayer = 0;	// �G�v���C���[�̃J�E���g
	float fAttackLength = 0.0f;	// ����
	bool bFind[2] = { false, false };	// �������

	while (pScene != NULL)
	{// NULL�ɂȂ�܂Ń��[�v
		pSceneNext = pScene->GetSceneNext();
		CScene::OBJTYPE objType = pScene->GetObjType();

		if (CScene::OBJTYPE_PLAYER == objType)
		{// �v���C���[�I�u�W�F�N�g�̂Ƃ�
			CPlayer *pPlayer = (CPlayer*)pScene;
			int nTeam = pPlayer->GetTeam();

			//if(nCntEnemyPlayer < ENEMY_PLAYER_MAX)
			if (m_nTeam != nTeam)
			{// �`�[�����Ⴄ�Ƃ�
				m_pEnemyPlayer[nCntEnemyPlayer] = pPlayer;
				nCntEnemyPlayer++;
			}
		}

		// ���̃I�u�W�F�N�g������
		pScene = pSceneNext;
	}

	for (int nCntPlayer = 0; nCntPlayer < nCntEnemyPlayer; nCntPlayer++)
	{// �G�v���C���[�̐������
		if (m_pEnemyPlayer[nCntPlayer] != NULL)
		{// NULL�`�F�b�N
			fAttackLength =
				(m_pEnemyPlayer[nCntPlayer]->GetPos().x - m_pos.x) *
				(m_pEnemyPlayer[nCntPlayer]->GetPos().x - m_pos.x) +
				(m_pEnemyPlayer[nCntPlayer]->GetPos().z - m_pos.z) *
				(m_pEnemyPlayer[nCntPlayer]->GetPos().z - m_pos.z);

			if (fAttackLength < ATTACK_AREA)
			{// �͈͓��ɓG���������ꍇ

			 // ������Ԃɂ���
				bFind[nCntPlayer] = true;

				// �������G�̕���������
				m_rotDest.y = atan2f(m_pEnemyPlayer[nCntPlayer]->GetPos().x - m_pos.x, m_pEnemyPlayer[nCntPlayer]->GetPos().z - m_pos.z) + D3DX_PI;

				// �ˏo���̈ʒu�̎擾
				D3DXMATRIX mtxCanon = m_pModel[0]->GetMtxWorld();
				D3DXVECTOR3 posCanon = D3DXVECTOR3(mtxCanon._41, mtxCanon._42 - 8.0f, mtxCanon._43);

				if (rand() % 30 == 0)
				{// �����_���ȃ^�C�~���O�ōU��

				 // ���E�U�������̐ݒ�
					float fAngle = m_rot.y + D3DX_PI;

					// �㉺�U�������̐ݒ�
					float fAngleV =
						(m_pos.y - m_pEnemyPlayer[nCntPlayer]->GetPos().y) *
						(m_pos.y - m_pEnemyPlayer[nCntPlayer]->GetPos().y) +
						(m_pos.y - m_pEnemyPlayer[nCntPlayer]->GetPos().y) *
						(m_pos.y - m_pEnemyPlayer[nCntPlayer]->GetPos().y);

					// �e���΂������
					fAngle += (float)(ATTACK_DISPERTION - (rand() % ATTACK_DISPERTION * 2)) * 0.005f;
					fAngleV += (float)(ATTACK_DISPERTION - (rand() % ATTACK_DISPERTION * 2)) * 0.003f;

					// �e�̐���
					CBulletPlayer::Create(posCanon, fAngle, fAngleV, m_nAttack, m_nTeam, this);
				}
			}
			else
			{// �͈͓��ɓG�����Ȃ��ꍇ
				bFind[nCntPlayer] = false;
			}
		}
	}

	// ������Ԃ̑J��
	if (bFind[0] || bFind[1])
	{// �ǂ��炩����ł��������Ă���
		m_bFind = true;
	}
	else if (!bFind[0] && !bFind[1])
	{// �ǂ������������Ă��Ȃ�
		m_bFind = false;
	}
}

//=============================================================================
//	�����ړ�����
//=============================================================================
void CAIMecha::AutoMove()
{
	if (m_bPatrol)
	{// �����ړ���
		m_posDest = m_patrolWaypoint[m_nPoint];
	}
	else
	{
		// �ڕW�n�_��ݒ�
		m_posDest = m_waypoint[m_nPoint];
	}

	// �ړI�Ƃ̍������o��
	float fLength = (m_pos.x - m_posDest.x) * (m_pos.x - m_posDest.x) + (m_pos.z - m_posDest.z) * (m_pos.z - m_posDest.z);
	m_nBreaktime--;

	if (fLength > MOVE_ACCEPTABLE)
	{// ���������e�l���Ɏ��܂�܂ŖړI�n�Ɉړ�����
		m_move.x = sinf(atan2f(m_posDest.x - m_pos.x, m_posDest.z - m_pos.z)) * AI_SPEED;
		m_move.z = cosf(atan2f(m_posDest.x - m_pos.x, m_posDest.z - m_pos.z)) * AI_SPEED;

		if (!m_bFind)
		{// ��������
			m_rotDest.y = atan2f(m_posDest.x - m_pos.x, m_posDest.z - m_pos.z) + D3DX_PI;
		}
	}
	else if (m_nBreaktime < 0)
	{// �ړ���x�e
		m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_nBreaktime = ENEMY_BREAKTIME;

		if (m_nCountPoint > 0 && m_nCountPoint == m_nPoint && !m_bGoal)
		{// �I���m�[�h�ɓ��������Ƃ�
			m_bGoal = true;

			if (m_AIAction[2] == AI_ACTION_ROUND_TRIP)
			{// �����ړ���
				m_bPatrol = true;
				m_nPoint = 0;
			}
		}
	}
	else if (m_nBreaktime == 0)
	{// �x�e�I��
		if (m_nCountPoint > m_nPoint || m_nCountPatrolPoint > m_nPoint)
		{// �ڕW�n�_�ɓ��B���Ă��Ȃ��ꍇ
			m_nPoint++;
		}
	}

	if (m_AIAction[2] == AI_ACTION_ROUND_TRIP && m_nCountPatrolPoint <= m_nPoint)
	{// �����ړ���ԂŖڕW�n�_�ɓ��B���Ă���ꍇ
		m_nPoint = 0;
	}

	if (m_rotDest.y > D3DX_PI) { m_rotDest.y -= D3DX_PI * 2.0f; }
	if (m_rotDest.y < -D3DX_PI) { m_rotDest.y += D3DX_PI * 2.0f; }

	if (m_rot.y > D3DX_PI) { m_rot.y -= D3DX_PI * 2.0f; }
	if (m_rot.y < -D3DX_PI) { m_rot.y += D3DX_PI * 2.0f; }

	if (m_rotDest.y - m_rot.y > D3DX_PI || m_rotDest.y - m_rot.y < -D3DX_PI)
	{// ������1���ȏ゠��ꍇ
		m_rot.y -= (m_rotDest.y - m_rot.y) * 0.1f;
	}
	else
	{
		// �ڕW�����֌���
		m_rot.y += (m_rotDest.y - m_rot.y) * 0.1f;
	}

	// �ʒu�̍X�V
	m_pos.x += m_move.x;
	m_pos.z += m_move.z;

	//CDebugProc::Print("m_rot.y : %.1f ", m_rot.y);
	//CDebugProc::Print("m_rotDest.y : %.1f ", m_rotDest.y);
}

//=============================================================================
//	�Ǐ]����
//=============================================================================
void CAIMecha::Follow()
{
	float fMinLength = 100000, fLength = 100000;	// �����n
	int nNearNode = 0;

	// �����̈ʒu�ɍł��߂��m�[�h����������
	for (int nCntNode = 0; nCntNode < m_pNodeData->GetLoadData().nodeMax; nCntNode++)
	{// �m�[�h�̐��������
	 // ���������߂�
		if (m_pPlayer != NULL)
		{// �v���C���[��NULL�`�F�b�N
			fLength =
				(m_pNodeData->GetLoadData().pos[nCntNode].x - m_pos.x) *
				(m_pNodeData->GetLoadData().pos[nCntNode].x - m_pos.x) +
				(m_pNodeData->GetLoadData().pos[nCntNode].z - m_pos.z) *
				(m_pNodeData->GetLoadData().pos[nCntNode].z - m_pos.z);

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
	for (int nCntNode = 0; nCntNode < m_pNodeData->GetLoadData().nodeMax; nCntNode++)
	{// �m�[�h�̐��������
	 // ���������߂�
		if (m_pPlayer != NULL)
		{// �v���C���[��NULL�`�F�b�N
			fLength =
				(m_pNodeData->GetLoadData().pos[nCntNode].x - m_pPlayer->GetPos().x) *
				(m_pNodeData->GetLoadData().pos[nCntNode].x - m_pPlayer->GetPos().x) +
				(m_pNodeData->GetLoadData().pos[nCntNode].z - m_pPlayer->GetPos().z) *
				(m_pNodeData->GetLoadData().pos[nCntNode].z - m_pPlayer->GetPos().z);

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

		for (int nCntAll = 0; nCntAll < m_pNodeData->GetLoadData().nodeMax; nCntAll++)
		{// �m�[�h�̐��������
			m_waypoint[nCntAll] = m_pNodeData->GetLoadData().pos[nNearNode];
		}
	}

	if (m_nNodeOld != m_nEndNode)
	{// ��l���O��̃m�[�h����ړ����Ă���ꍇ
		m_nPoint = 0;
		m_nCountPoint = -1;

		// ���������������Ă�m�[�h����������
		for (int nCntNode = 0; nCntNode < m_pNodeData->GetLoadData().nodeMax; nCntNode++)
		{// �m�[�h�̐��������
			fLength =
				(m_pNodeData->GetLoadData().pos[nCntNode].x - m_posDest.x) *
				(m_pNodeData->GetLoadData().pos[nCntNode].x - m_posDest.x) +
				(m_pNodeData->GetLoadData().pos[nCntNode].z - m_posDest.z) *
				(m_pNodeData->GetLoadData().pos[nCntNode].z - m_posDest.z);

			if (fMinLength > fLength)
			{// �����̍ŏ��l�����߂�
				fMinLength = fLength;
				nNearNode = nCntNode;
			}
		}

		// ���������������Ă�m�[�h���X�^�[�g�n�_�ɂ���
		m_nStartNode = nNearNode;

		for (int nCntAll = 0; nCntAll < m_pNodeData->GetLoadData().nodeMax; nCntAll++)
		{// �m�[�h�̐��������
			m_waypoint[nCntAll] = m_pNodeData->GetLoadData().pos[m_nStartNode];
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
		for (int nCntNode = 0; nCntNode < m_pNodeData->GetLoadData().nodeMax; nCntNode++)
		{// �m�[�h�̐��������
		 // ���������߂�
			if (m_pPlayer != NULL)
			{// �v���C���[��NULL�`�F�b�N
				fLength =
					(m_pNodeData->GetLoadData().pos[nCntNode].x - m_pos.x) *
					(m_pNodeData->GetLoadData().pos[nCntNode].x - m_pos.x) +
					(m_pNodeData->GetLoadData().pos[nCntNode].z - m_pos.z) *
					(m_pNodeData->GetLoadData().pos[nCntNode].z - m_pos.z);

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
		for (int nCntNode = 0; nCntNode < m_pNodeData->GetLoadData().nodeMax; nCntNode++)
		{// �m�[�h�̐��������
		 // ���������߂�
			fLength =
				(m_pNodeData->GetLoadData().pos[nCntNode].x - CManager::GetGame()->GetMouse()->Getsetpos().x) *
				(m_pNodeData->GetLoadData().pos[nCntNode].x - CManager::GetGame()->GetMouse()->Getsetpos().x) +
				(m_pNodeData->GetLoadData().pos[nCntNode].z - CManager::GetGame()->GetMouse()->Getsetpos().z) *
				(m_pNodeData->GetLoadData().pos[nCntNode].z - CManager::GetGame()->GetMouse()->Getsetpos().z);

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
		m_nRallyEndNode[m_nRallyCount] = nNearNode;

		if (m_AIAction[2] == AI_ACTION_ROUND_TRIP)
		{// �����ړ���Ԃ̏ꍇ
			if (m_nRallyCount <= 1)
			{// 1�N���b�N��
				m_nPatrolStartNode = nNearNode;
			}
			m_nRallyEndNode[0] = m_nPatrolStartNode;
			m_nEndNode = m_nPatrolStartNode;
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
	for (int nCntNode = 0; nCntNode < m_pNodeData->GetLoadData().nodeMax; nCntNode++, nCntWeight++)
	{// �m�[�h�̐��������
		weight[nCntWeight] = sqrt(
			(m_pNodeData->GetLoadData().pos[m_nStartNode].x - m_pNodeData->GetLoadData().pos[nCntNode].x) *
			(m_pNodeData->GetLoadData().pos[m_nStartNode].x - m_pNodeData->GetLoadData().pos[nCntNode].x) +
			(m_pNodeData->GetLoadData().pos[m_nStartNode].z - m_pNodeData->GetLoadData().pos[nCntNode].z) *
			(m_pNodeData->GetLoadData().pos[m_nStartNode].z - m_pNodeData->GetLoadData().pos[nCntNode].z));
	}

	//======= �G�b�W�ǉ� =========================================================================
	for (int nCntNode = 0; nCntNode < m_pNodeData->GetLoadData().nodeMax; nCntNode++)
	{// �m�[�h�̐��������
		for (int nCntConnect = 0; nCntConnect < m_pNodeData->GetLoadData().connectNum[nCntNode]; nCntConnect++)
		{// �q�����Ă�m�[�h�̐��������
			CAIMecha::AddEdge(nCntNode, m_pNodeData->GetLoadData().connectIndex[nCntNode][nCntConnect], weight[nCntNode], node);
		}
	}

	//======= �ŒZ�o�H�𒲂ׂ� =========================================================================
	CAIMecha::Dijkstra(m_pNodeData->GetLoadData().nodeMax, m_nStartNode, m_nEndNode, node);

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
	for (int nCntNodeMax = 0; nCntNodeMax < m_pNodeData->GetLoadData().nodeMax; )
	{// �m�[�h�̐��������
		for (int nCntNode = path.size() - 1; nCntNode >= 0; nCntNode--, nCntNodeMax++)
		{
			m_waypoint[nCntNodeMax] = m_pNodeData->GetLoadData().pos[path[nCntNode]];
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
	for (int nCntNode = 0; nCntNode < m_pNodeData->GetLoadData().nodeMax; nCntNode++, nCntWeight++)
	{// �m�[�h�̐��������
		if (m_nRallyCount != 0)
		{// 2�|�C���g�ڈȍ~
			weight[nCntWeight] = sqrt(
				(m_pNodeData->GetLoadData().pos[m_nRallyEndNode[m_nRallyCount - 1]].x - m_pNodeData->GetLoadData().pos[nCntNode].x) *
				(m_pNodeData->GetLoadData().pos[m_nRallyEndNode[m_nRallyCount - 1]].x - m_pNodeData->GetLoadData().pos[nCntNode].x) +
				(m_pNodeData->GetLoadData().pos[m_nRallyEndNode[m_nRallyCount - 1]].z - m_pNodeData->GetLoadData().pos[nCntNode].z) *
				(m_pNodeData->GetLoadData().pos[m_nRallyEndNode[m_nRallyCount - 1]].z - m_pNodeData->GetLoadData().pos[nCntNode].z));
		}
		else
		{// 1�|�C���g��
			weight[nCntWeight] = sqrt(
				(m_pNodeData->GetLoadData().pos[m_nRallyEndNode[m_nRallyCount]].x - m_pNodeData->GetLoadData().pos[nCntNode].x) *
				(m_pNodeData->GetLoadData().pos[m_nRallyEndNode[m_nRallyCount]].x - m_pNodeData->GetLoadData().pos[nCntNode].x) +
				(m_pNodeData->GetLoadData().pos[m_nRallyEndNode[m_nRallyCount]].z - m_pNodeData->GetLoadData().pos[nCntNode].z) *
				(m_pNodeData->GetLoadData().pos[m_nRallyEndNode[m_nRallyCount]].z - m_pNodeData->GetLoadData().pos[nCntNode].z));
		}
	}

	//======= �G�b�W�ǉ� =========================================================================
	for (int nCntNode = 0; nCntNode < m_pNodeData->GetLoadData().nodeMax; nCntNode++)
	{// �m�[�h�̐��������
		for (int nCntConnect = 0; nCntConnect < m_pNodeData->GetLoadData().connectNum[nCntNode]; nCntConnect++)
		{// �q�����Ă�m�[�h�̐��������
			CAIMecha::AddEdge(nCntNode, m_pNodeData->GetLoadData().connectIndex[nCntNode][nCntConnect], weight[nCntNode], m_node[m_nRallyCount]);
		}
	}

	//======= �ŒZ�o�H�𒲂ׂ� =========================================================================
	if (m_nRallyCount > 1)
	{// 2�|�C���g�ڈȍ~
		for (int nCntRally = 1; nCntRally < m_nRallyCount; nCntRally++)
		{// 2�|�C���g�ȍ~�̐��������
		 // �_�C�N�X�g���@�ōŒZ�o�H�����߂�
			CAIMecha::Dijkstra(m_pNodeData->GetLoadData().nodeMax, m_nRallyEndNode[m_nRallyCount - nCntRally], m_nRallyEndNode[m_nRallyCount - (nCntRally - 1)], m_node[m_nRallyCount - (nCntRally - 1)]);

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
		CAIMecha::Dijkstra(m_pNodeData->GetLoadData().nodeMax, m_nStartNode, m_nRallyEndNode[m_nRallyCount - (m_nRallyCount - 1)], m_node[m_nRallyCount - (m_nRallyCount - 1)]);

		for (int nCntNode = m_nRallyEndNode[m_nRallyCount - (m_nRallyCount - 1)]; nCntNode != m_nStartNode; nCntNode = m_node[m_nRallyCount - (m_nRallyCount - 1)][nCntNode].from)
		{// �ŒZ�o�H���S�[�����珇�ɃX�^�[�g�܂ł��ǂ�
			path.push_back(nCntNode);
		}
		path.push_back(m_nStartNode);
	}

	if (m_nRallyCount == 0)
	{// �X�^�[�g�ʒu
	 //�_�C�N�X�g���@�ōŒZ�o�H�����߂�
		CAIMecha::Dijkstra(m_pNodeData->GetLoadData().nodeMax, m_nStartNode, m_nRallyEndNode[m_nRallyCount], m_node[m_nRallyCount]);

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
	for (int nCntNodeMax = 0; nCntNodeMax < m_pNodeData->GetLoadData().nodeMax; )
	{// �m�[�h�̐��������
		for (int nCntNode = path.size() - 1; nCntNode >= 0; nCntNode--, nCntNodeMax++)
		{// �S�[������H��
			m_waypoint[nCntNodeMax] = m_pNodeData->GetLoadData().pos[path[nCntNode]];
		}
	}
}

//=============================================================================
//	�����ړ��o�H��������
//=============================================================================
void CAIMecha::PatrolRootSearch()
{
	float weight[NODEPOINT_MAX];	// �e�G�b�W�̃R�X�g
	int nCntWeight = 0;		// �R�X�g�̃J�E���^
	std::vector<int> path;	// �ŒZ�o�H�̏���ێ�����vector

							//======= �G�b�W�R�X�g�̎Z�o =========================================================================
	for (int nCntNode = 0; nCntNode < m_pNodeData->GetLoadData().nodeMax; nCntNode++, nCntWeight++)
	{// �m�[�h�̐��������
		if (m_nRallyCount != 0)
		{// 2�|�C���g�ڈȍ~
			weight[nCntWeight] = sqrt(
				(m_pNodeData->GetLoadData().pos[m_nRallyEndNode[m_nRallyCount - 1]].x - m_pNodeData->GetLoadData().pos[nCntNode].x) *
				(m_pNodeData->GetLoadData().pos[m_nRallyEndNode[m_nRallyCount - 1]].x - m_pNodeData->GetLoadData().pos[nCntNode].x) +
				(m_pNodeData->GetLoadData().pos[m_nRallyEndNode[m_nRallyCount - 1]].z - m_pNodeData->GetLoadData().pos[nCntNode].z) *
				(m_pNodeData->GetLoadData().pos[m_nRallyEndNode[m_nRallyCount - 1]].z - m_pNodeData->GetLoadData().pos[nCntNode].z));
		}
		else
		{// 1�|�C���g��
			weight[nCntWeight] = sqrt(
				(m_pNodeData->GetLoadData().pos[m_nRallyEndNode[m_nRallyCount]].x - m_pNodeData->GetLoadData().pos[nCntNode].x) *
				(m_pNodeData->GetLoadData().pos[m_nRallyEndNode[m_nRallyCount]].x - m_pNodeData->GetLoadData().pos[nCntNode].x) +
				(m_pNodeData->GetLoadData().pos[m_nRallyEndNode[m_nRallyCount]].z - m_pNodeData->GetLoadData().pos[nCntNode].z) *
				(m_pNodeData->GetLoadData().pos[m_nRallyEndNode[m_nRallyCount]].z - m_pNodeData->GetLoadData().pos[nCntNode].z));
		}

	}

	//======= �G�b�W�ǉ� =========================================================================
	for (int nCntNode = 0; nCntNode < m_pNodeData->GetLoadData().nodeMax; nCntNode++)
	{// �m�[�h�̐��������
		for (int nCntConnect = 0; nCntConnect < m_pNodeData->GetLoadData().connectNum[nCntNode]; nCntConnect++)
		{// �q�����Ă�m�[�h�̐��������
			CAIMecha::AddEdge(nCntNode, m_pNodeData->GetLoadData().connectIndex[nCntNode][nCntConnect], weight[nCntNode], m_node[m_nRallyCount]);
		}
	}

	//======= �t���ŒZ�o�H�𒲂ׂ� =========================================================================
	if (m_nRallyCount == 0)
	{// �X�^�[�g�ʒu
	 //�_�C�N�X�g���@�ōŒZ�o�H�����߂�
		CAIMecha::Dijkstra(m_pNodeData->GetLoadData().nodeMax, m_nRallyEndNode[m_nRallyCount], m_nPatrolStartNode, m_node[m_nRallyCount]);

		for (int nCntNode = m_nPatrolStartNode; nCntNode != m_nRallyEndNode[m_nRallyCount]; nCntNode = m_node[m_nRallyCount][nCntNode].from)
		{// �ŒZ�o�H���X�^�[�g���珇�ɃS�[���܂ł��ǂ�
			path.push_back(nCntNode);
		}
		path.push_back(m_nRallyEndNode[m_nRallyCount]);
	}

	if (m_nRallyCount > 0)
	{// 1�|�C���g��
	 // �_�C�N�X�g���@�ōŒZ�o�H�����߂�
		CAIMecha::Dijkstra(m_pNodeData->GetLoadData().nodeMax, m_nRallyEndNode[m_nRallyCount - (m_nRallyCount - 1)], m_nPatrolStartNode, m_node[m_nRallyCount - (m_nRallyCount - 1)]);

		for (int nCntNode = m_nPatrolStartNode; nCntNode != m_nRallyEndNode[m_nRallyCount - (m_nRallyCount - 1)]; nCntNode = m_node[m_nRallyCount - (m_nRallyCount - 1)][nCntNode].from)
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
			CAIMecha::Dijkstra(m_pNodeData->GetLoadData().nodeMax, m_nRallyEndNode[m_nRallyCount - (nCntRally - 1)], m_nRallyEndNode[m_nRallyCount - nCntRally], m_node[m_nRallyCount - (nCntRally - 1)]);

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
			CAIMecha::Dijkstra(m_pNodeData->GetLoadData().nodeMax, m_nRallyEndNode[m_nRallyCount - nCntRally], m_nRallyEndNode[m_nRallyCount - (nCntRally - 1)], m_node[m_nRallyCount - (nCntRally - 1)]);

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
		CAIMecha::Dijkstra(m_pNodeData->GetLoadData().nodeMax, m_nPatrolStartNode, m_nRallyEndNode[m_nRallyCount - (m_nRallyCount - 1)], m_node[m_nRallyCount - (m_nRallyCount - 1)]);

		for (int nCntNode = m_nRallyEndNode[m_nRallyCount - (m_nRallyCount - 1)]; nCntNode != m_nPatrolStartNode; nCntNode = m_node[m_nRallyCount - (m_nRallyCount - 1)][nCntNode].from)
		{// �ŒZ�o�H���S�[�����珇�ɃX�^�[�g�܂ł��ǂ�
			path.push_back(nCntNode);
		}
		path.push_back(m_nPatrolStartNode);
	}

	if (m_nRallyCount == 0)
	{// �X�^�[�g�ʒu
	 //�_�C�N�X�g���@�ōŒZ�o�H�����߂�
		CAIMecha::Dijkstra(m_pNodeData->GetLoadData().nodeMax, m_nPatrolStartNode, m_nRallyEndNode[m_nRallyCount], m_node[m_nRallyCount]);

		for (int nCntNode = m_nRallyEndNode[m_nRallyCount]; nCntNode != m_nPatrolStartNode; nCntNode = m_node[m_nRallyCount][nCntNode].from)
		{// �ŒZ�o�H���S�[�����珇�ɃX�^�[�g�܂ł��ǂ�
			path.push_back(nCntNode);
		}
		path.push_back(m_nPatrolStartNode);
	}

	//======= �ŒZ�o�H�̏o�� =========================================================================
	for (int nCntNode = path.size() - 1; nCntNode >= 0; nCntNode--)
	{
		if (m_nCountPatrolPoint < nCntNode)
		{// �ő�l����
			m_nCountPatrolPoint = nCntNode;
		}
	}

	//======= �ڕW�n�_�̐ݒ� =========================================================================
	for (int nCntNodeMax = 0; nCntNodeMax < m_pNodeData->GetLoadData().nodeMax; )
	{// �m�[�h�̐��������
		for (int nCntNode = path.size() - 1; nCntNode >= 0; nCntNode--, nCntNodeMax++)
		{// �S�[������H��
			m_patrolWaypoint[nCntNodeMax] = m_pNodeData->GetLoadData().pos[path[nCntNode]];
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
		for (int nCntAll = 0; nCntAll < m_pNodeData->GetLoadData().nodeMax; nCntAll++)
		{// �S�Ẵm�[�h�ƃv���C���[�Ƃ̍������o��
			fLength =
				(m_pos.x - m_pNodeData->GetLoadData().pos[nCntAll].x) *
				(m_pos.x - m_pNodeData->GetLoadData().pos[nCntAll].x) +
				(m_pos.z - m_pNodeData->GetLoadData().pos[nCntAll].z) *
				(m_pos.z - m_pNodeData->GetLoadData().pos[nCntAll].z);

			if (fMinLength > fLength)
			{// �����̍ŏ��l�����߂�
				fMinLength = fLength;
				nNear = nCntAll;
			}
		}

		// ��ԋ߂��m�[�h�Œ�~���ăf�[�^��������
		m_bPatrol = false;
		m_bGoal = false;
		m_nRallyCount = 0;
		m_nPoint = 0;
		m_nCountPoint = -1;
		m_nCountPatrolPoint = -1;
		m_nStartNode = nNear;
		m_nEndNode = nNear;
		m_nPatrolStartNode = nNear;

		for (int nCntRally = 0; nCntRally < RALLYPOINT_MAX; nCntRally++)
		{// �����[�|�C���g�̍ő吔�����
			for (int nCntNode = 0; nCntNode < NODEPOINT_MAX; nCntNode++)
			{// �m�[�h�̍ő吔�����
				m_node[nCntRally][nCntNode] = {};
			}
		}

		for (int nCntAll = 0; nCntAll < m_pNodeData->GetLoadData().nodeMax; nCntAll++)
		{// �m�[�h�̐��������
			m_nRallyEndNode[nCntAll] = nNear;
			m_waypoint[nCntAll] = m_pNodeData->GetLoadData().pos[nNear];
			m_patrolWaypoint[nCntAll] = m_pNodeData->GetLoadData().pos[nNear];
		}

		for (int nCntAction = 0; nCntAction < 4; nCntAction++)
		{// �s�����̕����
			m_AIAction[nCntAction] = AI_ACTION_NONE;
			m_LogicTree[nCntAction] = -1;
		}
		m_posDest = m_pNodeData->GetLoadData().pos[nNear];
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