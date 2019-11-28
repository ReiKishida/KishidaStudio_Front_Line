//=============================================================================
//
// �v���C���[���� [player.cpp]
// Author : Takuto Ishida
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "player.h"
#include "input.h"
#include "manager.h"
#include "renderer.h"
#include "debugProc.h"
#include "meshField.h"
#include "camera.h"
#include "model.h"
#include "motion.h"
#include "shadow.h"
#include "game.h"
#include "enemy.h"
#include "fade.h"
#include "life.h"
#include "particle.h"
#include "sound.h"
#include "bullet.h"
#include "scene3DBill.h"
#include "texture.h"
#include "UI_Texture.h"
#include "gauge.h"
#include "UI_Number.h"
#include <stdio.h>
#include "server.h"
#include "collisionSet.h"
#include "mouseCursor.h"
#include "button.h"
#include "menu.h"
#include "AI.h"
#include "particle.h"

#include "search.h"
//==================================
// �}�N����`
//==================================
#define ASSULT_FILE				"data/TEXT/PLAYER/assult/model_assult.txt"
#define LIGHT_FILE				"data/TEXT/PLAYER/light/model_light.txt"
#define HEAVY_FILE				"data/TEXT/PLAYER/heavy/model_heavy.txt"
#define SNIPE_FILE				"data/TEXT/PLAYER/snipe/model_snipe.txt"
//�퓬�p
#define ASSULT_BATTLE_FILE		"data/TEXT/PLAYER/assult/battle_assult.txt"
#define LIGHT_BATTLE_FILE		"data/TEXT/PLAYER/light/battle_light.txt"
#define HEAVY_BATTLE_FILE		"data/TEXT/PLAYER/heavy/battle_heavy.txt"
#define SNIPE_BATTLE_FILE		"data/TEXT/PLAYER/snipe/battle_snipe.txt"


#define PLAYER_DAMAGE_TIME		(60)		// �_���[�W���󂯂����̖��G����
#define PLAYER_DAMAGE_MOVE		(40)		// �_���[�W���󂯂Ă��瓮����悤�ɂȂ�܂ł̎���
#define PLAYER_RETICLE_LENGTH	(2500.0f)	// ���e�B�N���̋���

// =============================================================
// UI�֌W
// =============================================================
#define ANIM_SPEED				(10)		// �^�C���A�j���[�V����
#define ANIM_PATTERN			(8)			// �^�C���̃p�^�[����
#define PLAYER_BOTTON_WIDTH		(75.0f)		// ���X�|�[���{�^���̉���
#define PLAYER_BOTTON_HEIGHT	(75.0f)		// ���X�|�[���{�^���̏c��
#define PLAYER_BOTTON_INT		(40.0f)		// �{�^���ƃ{�^���̊Ԋu
#define PLAYER_UI_HEIGHT		(630.0f)

// ���W�I�`���b�g
#define RADIOCHAT_MESS_SPEED		(10)			// ���W�I�`���b�g���b�Z�[�W�̑���
#define RADIOCHAT_DISPLAY_TIME	(60 * 2)	// ���b�Z�[�W�\������
#define RADIOCHAT_COL						(0.05f)		// �����x�̌��Z

// =============================================================
// AI�֌W
// =============================================================
#define	LOAD_FILENAME		("data/TEXT/NODE_DATA/NodeData.txt")	// �}�b�v�f�[�^��ǂݍ��ރt�@�C���̖��O
#define MOVE_ACCEPTABLE		(50.0f)		// �ړ����̌덷�̋��e�͈�
#define POS_ACCEPTABLE		(30.0f)		// �������̌덷�̋��e�͈�
#define MOUSE_ACCEPTABLE	(20.0f)		// �}�E�X�̌덷�̋��e�͈�
#define COLLECT_TIME		(5)			// �f�[�^�̎��W���s���Ԋu(�b)
#define DATA_REFERENCE_TIME	(5)			// �f�[�^�̎Q�Ƃ��s���Ԋu(��)
#define PLAYER_BREAKTIME	(1)			// �x�e����(�t���[��)
#define PLAYER_FINALPOINT_BREAKTIME	(120)	// �ŏI�n�_�̋x�e����(�t���[��)

#define MAX_CHAR (254)					//�ǂݎ�镶����
#define MAX_SEARCH (4)	//�Z���T�[��
#define FIND_FIND_CHARACTER_PRIORITY (4)				//�T���v���C���[�̗D�揇��

//==================================
// �ÓI�����o�ϐ��錾
//==================================
D3DXVECTOR3 CPlayer::m_searchPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
CPlayer *CPlayer::m_pPlayer = NULL;
//==================================
// ��������
//==================================
CPlayer* CPlayer::Create(int nPlayerIdx, CMechaSelect::MECHATYPE mecha, D3DXVECTOR3 pos, bool bConnect)
{
	CPlayer *pPlayer;

	pPlayer = new CPlayer;	// ���������m��

	if (pPlayer != NULL)
	{// �������m�ې���
		pPlayer->SetPlayerIdx(nPlayerIdx);
		pPlayer->m_mecha = mecha;
		pPlayer->m_pos = pos;
		pPlayer->m_bConnect = bConnect;
		pPlayer->Init();
	}

	return pPlayer;
}

//=========================================
// �R���X�g���N�^
//=========================================
CPlayer::CPlayer(int nPriority, CScene::OBJTYPE objType) : CScene(nPriority, objType)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nCntState = 0;
	m_fRotDest = 0.0f;
	m_fSpeed = 0.0f;
	m_fCameraAngle = 0.0f;
	m_pModel = NULL;
	m_nNumParts = 0;
	m_pUpperMotion = NULL;
	m_pLowerMotion = NULL;
	m_pReticle = NULL;
	m_nPlayerIdx = 0;
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_bShoot = false;
	m_vtxMax = D3DXVECTOR3(-1000.0f, -1000.0f, -1000.0f);
	m_vtxMin = D3DXVECTOR3(1000.0f, 1000.0f, 1000.0f);
	m_pCursor = NULL;
	m_nLife = 0;
	m_nTimer = 0;
	m_nDisTime = 0;
	m_mecha = CMechaSelect::MECHATYPE_ASSULT;
	m_nTeam = 0;
	m_bDeath = false;
	m_pGauge = NULL;
	m_nCntReRoad = 0;
	m_nDiff = 0;
	m_nRemBullet = 0;
	m_pUITexRadio = NULL;			// ����
	m_pUITexAllyRadio = NULL;		// ����
	m_radiochat = RADIOCHAT_OK;
	m_allyRadiochat = RADIOCHAT_OK;
	m_bChat = false;
	m_bAllyChat = false;
	m_moveSpeed = 0;
	m_bCol = false;
	m_bChatBotton = false;


	//AI�퓬�n�̕ϐ�
	m_pSearch = NULL;
	m_fSearchLength = 0.0f;
	m_fSearchAngle = 0.0f;
	m_SearchVec_0 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_SearchVec_1 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_SearchVec_2 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fRange = 0.0f;
	m_fRotDestUpper = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_bFind = false;

	for (int nCnt = 0; nCnt < PLAYER_UI_NUM; nCnt++)
	{
		m_pUINum[nCnt] = NULL;
	}

	for (int nCnt = 0; nCnt < RELOAD_TEX; nCnt++)
	{	// �����[�h
		m_pUITexReload[nCnt] = NULL;
	}

	for (int nCnt = 0; nCnt < SELECTRESPAWN_BOTTON; nCnt++)
	{	// ���X�|�[���{�^��
		m_pUISelectResBotton[nCnt] = NULL;
	}

	for (int nCnt = 0; nCnt < RESPAWN_TEX; nCnt++)
	{	// ���X�|�[��
		m_pUITexRespawn[nCnt] = NULL;
	}

	for (int nCnt = 0; nCnt < RADIOCHAT_BOTTON; nCnt++)
	{	// ���W�I�`���b�g�{�^��
		m_pUIRadioBotton[nCnt] = NULL;
	}

	// �J�E���^�[�̏�����
	m_nCntShoot = 0;
	m_bShootButton = false;
}

//=========================================
// �f�X�g���N�^
//=========================================
CPlayer::~CPlayer(){}

//=========================================
// ����������
//=========================================
HRESULT CPlayer::Init(void)
{
	srand((unsigned int)time(0));

	// �f�o�C�X�̎擾
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	FILE *pFile;
	int nCntParts = 0;		// �p�[�c���̃J�E���^
	int nCntModel = 0;
	int nFootIdx = 0;
	float fX, fY, fZ;
	int nId;
	int nParent;
	char **pModelName = NULL;
	char *pFileName = "";

	if (CMechaSelect::MECHATYPE_ASSULT == m_mecha) { pFileName = ASSULT_FILE; }
	else if (CMechaSelect::MECHATYPE_LIGHT == m_mecha) { pFileName = LIGHT_FILE; }
	else if (CMechaSelect::MECHATYPE_HEAVY == m_mecha) { pFileName = HEAVY_FILE; }
	else if (CMechaSelect::MECHATYPE_SHOOTER == m_mecha) { pFileName = SNIPE_FILE; }

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
							{// �������ː�
								fscanf(pFile, " = %d", &m_nNumShoot);
								m_pAngle = new float[m_nNumShoot * 2];
								m_pAngleV = new float[m_nNumShoot * 2];
							}
							else if (strcmp(aStr, "DISPERTION") == 0)
							{// �΂��
								fscanf(pFile, " = %d", &m_nDispertion);
							}
							else if (strcmp(aStr, "RELOAD") == 0)
							{// �����[�h����
								fscanf(pFile, " = %d", &m_nReload);
							}
							else if (strcmp(aStr, "LIFE") == 0)
							{// �ϋv��
								fscanf(pFile, " = %d", &m_nLifeMax);
							}
							else if (strcmp(aStr, "SPEED") == 0)
							{// �ړ���
								fscanf(pFile, " = %f", &m_fSpeed);
							}
							else if (strcmp(aStr, "FOOT") == 0)
							{// ���ȉ��̔ԍ�
								fscanf(pFile, " = %d", &nFootIdx);
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

	if (NULL == m_pUpperMotion)
	{// ���[�V�����N���X�̐���
		m_pUpperMotion = CMotion::Create(m_mecha, nFootIdx, m_pModel);
		m_pUpperMotion->SetMotion(CMotionManager::TYPE_NEUTRAL);	// �j���[�g�������[�V������ݒ�
	}

	if (NULL == m_pLowerMotion)
	{// ���[�V�����N���X�̐���
		m_pLowerMotion = CMotion::Create(m_mecha, m_nNumParts, m_pModel, nFootIdx);
		m_pLowerMotion->SetMotion(CMotionManager::TYPE_NEUTRAL);	// �j���[�g�������[�V������ݒ�
	}

	if (NULL == m_pShadow)
	{// �e�̐���
		m_pShadow = CShadow::Create(&m_pos);
	}

	if (CMenu::GetMode() == CMenu::MODE_MULTI)
	{// �}���`���[�h
		CClient *pClient = CManager::GetClient();

		if (m_nPlayerIdx == pClient->GetPlayerIdx())
		{
			if (NULL == m_pReticle)
			{// ���e�B�N���̐���
				m_pReticle = CScene3DBill::Create();
				m_pReticle->BindTexture(CTexture::GetTexture(CTexture::TEXTURE_RETICLE));
				m_pReticle->SetSize(D3DXVECTOR3(50.0f, 50.0f, 0.0f));
				m_pReticle->SetLighting(false);
				m_pReticle->SetZBuffer(true, D3DCMP_ALWAYS);
				m_pReticle->SwapPriority(6);
			}
		}
	}
	else
	{// �V���O�����[�h
		if (NULL == m_pReticle && m_nPlayerIdx == 0)
		{// ���e�B�N���̐���
			m_pReticle = CScene3DBill::Create();
			m_pReticle->BindTexture(CTexture::GetTexture(CTexture::TEXTURE_RETICLE));
			m_pReticle->SetSize(D3DXVECTOR3(50.0f, 50.0f, 0.0f));
			m_pReticle->SetLighting(false);
			m_pReticle->SetZBuffer(true, D3DCMP_ALWAYS);
			m_pReticle->SwapPriority(6);
		}
	}

	m_nRemBullet = m_nCapacity;

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

	m_vtxMin.x = posMin.x;
	m_vtxMin.y = posMin.y;
	m_vtxMin.z = posMin.z;
	m_vtxMax.x = posMax.x;
	m_vtxMax.y = posMax.y;
	m_vtxMax.z = posMax.z;

	m_nRemBullet = m_nCapacity;		// �e�̏����l
	m_pUINum[0]->SetRemainBullet(m_nRemBullet);

	m_nLife = m_nLifeMax;	// ���C�t�̏����l
	m_pUINum[1]->SetPlayerLife(m_nLife);

	// ���W�I�`���b�g���b�Z�[�W�Ŏg�p
	m_moveSpeed = RADIOCHAT_MESS_SPEED;

	// �Q�[���J�n��
	m_Respawn = RESPAWN_START;
	m_nDisTime = 1.8f;

	if (m_nPlayerIdx == 0 || m_nPlayerIdx == 1)
	{
		for (int nCntModel = 0; nCntModel < m_nNumParts; nCntModel++)
		{
			m_pModel[nCntModel]->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
		}
		m_nTeam = 0;

		// �J�����̌����̐ݒ�
		CManager::GetCamera()->SetRot(D3DXVECTOR3(D3DX_PI * 0.5f, D3DX_PI * 0.5f, 0.0f));
	}
	else if (m_nPlayerIdx == 2 || m_nPlayerIdx == 3)
	{
		for (int nCntModel = 0; nCntModel < m_nNumParts; nCntModel++)
		{
			m_pModel[nCntModel]->SetColor(D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
		}

		m_nTeam = 1;

		// �J�����̌����̐ݒ�
		CManager::GetCamera()->SetRot(D3DXVECTOR3(D3DX_PI * 0.5f, D3DX_PI * -0.5f, 0.0f));
	}

	// AI�̐���
	m_pAI = m_pAI->Create(this, CAIMecha::MECHATYPE_DRONE, m_pos + D3DXVECTOR3(0.0f, 70.0f, 0.0f));

	// ���l�̏�����==============================================================================
	m_posDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nPoint = 0;
	m_nCountPoint = 0;
	m_nCountCollect = 0;
	m_nBreaktime = 0;
	m_nGoalCount = 0;
	m_nVigilanceCount = 0;
	m_nCollectionTimer = COLLECT_TIME * 60;
	m_bGoal = false;
	m_bPartSwitch = false;
	m_bCollectSwitch = false;

	for (int nCntEnemy = 0; nCntEnemy < ENEMY_PLAYER_MAX; nCntEnemy++)
	{// �G�l�~�[�̍ő�l�����
		for (int nCntCollect = 0; nCntCollect < COLLECTIONDATA_MAX; nCntCollect++)
		{// ���W�f�[�^�̍ő�l�����
			m_collectionPos[nCntEnemy][nCntCollect] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		}
	}

	for (int nCntNode = 0; nCntNode < NODE_MAX; nCntNode++)
	{// �m�[�h�̍ő�l�����
		m_waypoint[nCntNode] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	if (!m_bConnect)
	{// �l�Ԃ��ڑ����Ă��Ȃ��ꍇ
		LoadBattleFile();
		m_fRotDestUpper = D3DXVECTOR3(D3DX_PI * 0.5f, D3DX_PI, 0.0f);


		CPlayer::FileLoad(LOAD_FILENAME);

		// �}�b�v�֌W==============================================================================
		//�퓬�p�̃e�L�X�g�ǂݍ���

		// �J�n���_�̃m�[�h�̏�����
		float fMinLength = 100000, fLength = 100000;	// �����n

		// �J�n���_�̃m�[�h�̏�����
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
		m_nEndNode = m_nMovePoint[rand() % 7];

		// �|�C���g����
		CPlayer::NodeSearch();
		// �|�C���g�ւ̌o�H�T��
		CPlayer::RootSearch();
	}

	return S_OK;
}

//=========================================
// �I������
//=========================================
void CPlayer::Uninit(void)
{
	if (NULL != m_pUpperMotion)
	{// ���[�V�����N���X�̔j��
		m_pUpperMotion->Uninit();
		m_pUpperMotion = NULL;
	}

	if (NULL != m_pLowerMotion)
	{// ���[�V�����N���X�̔j��
		m_pLowerMotion->Uninit();
		m_pLowerMotion = NULL;
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

	if (NULL != m_pAngle)
	{// �e�̊p�x�̔j��
		delete[] m_pAngle;
		m_pAngle = NULL;
	}

	if (NULL != m_pAngleV)
	{// �e�̐��������p�x�̔j��
		delete[] m_pAngleV;
		m_pAngleV = NULL;
	}

	if (NULL != m_pCursor)
	{// �J�[�\���̔j��
		m_pCursor->Uninit();
		m_pCursor = NULL;
	}

	if (NULL != m_pSearch)
	{
		for (int nCntSearch = 0; nCntSearch < MAX_SEARCH; nCntSearch++)
		{// ���f���̔j��
			if (NULL != m_pSearch[nCntSearch])
			{
				m_pSearch[nCntSearch]->Uninit();
				delete m_pSearch[nCntSearch];
				m_pSearch[nCntSearch] = NULL;
			}
		}

		// �������̊J��
		delete[] m_pSearch;
		m_pSearch = NULL;
	}

	// �I�u�W�F�N�g�̔j��
	CScene::Release();
}

//=========================================
// �X�V����
//=========================================
void CPlayer::Update(void)
{
	if (m_bConnect)
	{
		if (m_Respawn == RESPAWN_START)
		{	// �퓬�J�n ���
			Respawn(RESPAWN_START);
		}

		if (CMenu::GetMode() == CMenu::MODE_MULTI)
		{
			if (m_nPlayerIdx != CManager::GetClient()->GetPlayerIdx())
			{// �v���C���[�̔ԍ����N���C�A���g�ԍ��ƈႤ�ꍇ
				if (m_pUpperMotion != NULL && m_pLowerMotion != NULL)
				{// ���[�V�����N���X���g���Ă���
					if (m_pos != m_posOld)
					{//�ߋ��̈ʒu�ƌ��݂̈ʒu������Ă����ꍇ
					 //�������[�V�����ɂ���
						m_pUpperMotion->SetMotion(CMotionManager::TYPE_WALK);
						m_pLowerMotion->SetMotion(CMotionManager::TYPE_WALK);
					}
					else
					{//�j���[�g�������[�V�����ɂ���
						m_pUpperMotion->SetMotion(CMotionManager::TYPE_NEUTRAL);
						m_pLowerMotion->SetMotion(CMotionManager::TYPE_NEUTRAL);
					}
				}
				m_posOld = m_pos;
			}
		}

		if (CMenu::GetMode() == CMenu::MODE_MULTI)
		{
			if (m_nPlayerIdx == CManager::GetClient()->GetPlayerIdx())
			{//�v���C���[�̔ԍ����N���C�A���g�ԍ��Ɠ����ꍇ
				m_pReticle->SetDisp(false);

				if (CManager::GetGame()->GetPart() == CGame::PART_ACTION)
				{// �A�N�V�����p�[�g
					CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();	// �L�[�{�[�h�̓��͂��擾
					CXInput *pXInput = CManager::GetXInput();					// XInput�̓��͂��擾

					if (m_nLife >= 0 && m_Respawn == RESPAWN_NONE)
					{	// ���C�t���� && �퓬�J�n��Ԃ̎�
						if (m_nDiff > 0)
						{	// �_���[�W�ʂ�0�ȏ�̎�
							m_nLife--;
							m_nDiff--;
						}
						if (m_nLife <= 0)
						{	// ���C�t���Ȃ��Ȃ���
							m_Respawn = RESPAWN_DEATH;		// �s���s�\���
						}

						// �ړ��̏���
						Movement();

						// �e������
						Shoot();

						D3DXVECTOR3 rotCamera = CManager::GetCamera()->GetRot();
						D3DXVECTOR3 posR = CManager::GetCamera()->GetPosR();

						m_pReticle->SetDisp(true);
						m_pReticle->SetPos(D3DXVECTOR3(sinf(rotCamera.y) * PLAYER_RETICLE_LENGTH, cosf(rotCamera.x) * PLAYER_RETICLE_LENGTH, cosf(rotCamera.y) * PLAYER_RETICLE_LENGTH) + D3DXVECTOR3(m_pos.x, 0.0f, m_pos.z));

						for (int nCntModel = 0; nCntModel < m_nNumParts; nCntModel++)
						{// �p�[�c���f���̍X�V
							m_pModel[nCntModel]->Update();
						}
						CDebugProc::Print("�ʒu�F%.2f %.2f %.2f", m_pos.x, m_pos.y, m_pos.z);
					}
					if (m_Respawn == RESPAWN_DEATH)
					{	// �s���s�\���
						Respawn(m_Respawn);		// ���X�|�[������
					}
					if (m_Respawn == RESPAWN_SELECT)
					{	// ���X�|�[���I�����
						SelectRespawn();				// ���X�|�[���ʒu�I��
					}

					// �p�x�̍X�V
					Angle();

					// ���C�t�̐ݒ�
					m_pUINum[1]->SetPlayerLife(m_nLife);
				}
			}
		}
		else if (CMenu::GetMode() == CMenu::MODE_SINGLE)
		{
			if (m_nPlayerIdx == 0)
			{
				m_pReticle->SetDisp(false);

				if (CManager::GetGame()->GetPart() == CGame::PART_ACTION)
				{// �A�N�V�����p�[�g
					CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();	// �L�[�{�[�h�̓��͂��擾
					CXInput *pXInput = CManager::GetXInput();					// XInput�̓��͂��擾

					if (m_nLife >= 0 && m_Respawn == RESPAWN_NONE)
					{	// ���C�t���� && �퓬�J�n��Ԃ̎�
						if (m_nDiff > 0)
						{	// �_���[�W�ʂ�0�ȏ�̎�
							m_nLife--;
							m_nDiff--;
						}
						if (m_nLife <= 0)
						{	// ���C�t���Ȃ��Ȃ���
							m_Respawn = RESPAWN_DEATH;		// �s���s�\���
						}

						// �ړ��̏���
						Movement();

						// �e������
						Shoot();

						// �`���b�g
						if (m_bChat == false)
						{
							ChatBotton();
						}

						if (m_bChat == true)
						{	// �`���b�g���b�Z�[�W
							ChatMess(m_bChat);
						}

						// �d��
						//m_move.y -= GRAVITY;

						D3DXVECTOR3 rotCamera = CManager::GetCamera()->GetRot();
						D3DXVECTOR3 posR = CManager::GetCamera()->GetPosR();

						m_pReticle->SetDisp(true);
						m_pReticle->SetPos(D3DXVECTOR3(sinf(rotCamera.y) * PLAYER_RETICLE_LENGTH, cosf(rotCamera.x) * PLAYER_RETICLE_LENGTH, cosf(rotCamera.y) * PLAYER_RETICLE_LENGTH) + D3DXVECTOR3(m_pos.x, 0.0f, m_pos.z));

						for (int nCntModel = 0; nCntModel < m_nNumParts; nCntModel++)
						{// �p�[�c���f���̍X�V
							m_pModel[nCntModel]->Update();
						}
						CDebugProc::Print("�ʒu�F%.2f %.2f %.2f", m_pos.x, m_pos.y, m_pos.z);
					}
					if (m_Respawn == RESPAWN_DEATH)
					{	// �s���s�\���
						Respawn(m_Respawn);		// ���X�|�[������
					}
					if (m_Respawn == RESPAWN_SELECT)
					{	// ���X�|�[���I�����
						SelectRespawn();				// ���X�|�[���ʒu�I��
					}

					// �p�x�̍X�V
					Angle();

					if (pKeyboard->GetTrigger(DIK_1))
					{
						m_nLife -= 10;
					}
					SetParticle();
					// ���C�t�̐ݒ�
					m_pUINum[1]->SetPlayerLife(m_nLife);
				}
			}

		}
	}
	else
	{
		AIUpdate();
		// �p�x�̍X�V
		Angle();
	}
}

//=========================================
// �`�揈��
//=========================================
void CPlayer::Draw(void)
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

	if (m_bConnect != true)
	{
		for (int nCntSearch = 0; nCntSearch < MAX_SEARCH; nCntSearch++)
		{// ���f���̕`�揈��
			m_pSearch[nCntSearch]->Draw();
		}
	}
}

//=========================================
// �ړ��̏���
//=========================================
void CPlayer::Movement(void)
{
	m_posOld = m_pos;
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();	// �L�[�{�[�h�̓��͂��擾
	CXInput *pXInput = CManager::GetXInput();					// XInput�̓��͂��擾
	bool bKey = false;				// �{�^�������t���O
	float fRotY = CManager::GetCamera()->GetRotY();
	float fDirMove;

	CDirectInput *pDirectInput = CManager::GetDirectInput();	//DirectInput�̎擾
	CDirectInput::GamePad *DirectInputStick = pDirectInput->GetgamePadStick();

	// �L�[���͂ɂ��ړ�
	if (pKeyboard->GetPress(DIK_A) || pXInput->GetThumbLX(0) <= -MIN_GAMEPAD_LEFT_THUMB_X || DirectInputStick->aGamePad.lX < -GAMEPAD_DEADZONE)
	{// �������ֈړ�
		if (pKeyboard->GetPress(DIK_W) || pXInput->GetThumbLY(0) >= MIN_GAMEPAD_LEFT_THUMB_Y || DirectInputStick->aGamePad.lY < -GAMEPAD_DEADZONE)
		{// ���O
			m_fRotDest = D3DX_PI * -0.25f;
			fDirMove = D3DX_PI * -0.25f;
		}
		else if (pKeyboard->GetPress(DIK_S) || pXInput->GetThumbLY(0) <= -MIN_GAMEPAD_LEFT_THUMB_Y || DirectInputStick->aGamePad.lY > GAMEPAD_DEADZONE)
		{// �����
			m_fRotDest = D3DX_PI * 0.25f;
			fDirMove = D3DX_PI * -0.75f;
		}
		else
		{// ��
			m_fRotDest = D3DX_PI * -0.5f;
			fDirMove = D3DX_PI * -0.5f;
		}
		bKey = true;
	}
	else if (pKeyboard->GetPress(DIK_D) || pXInput->GetThumbLX(0) >= MIN_GAMEPAD_LEFT_THUMB_X || DirectInputStick->aGamePad.lX > GAMEPAD_DEADZONE)
	{// �������ֈړ�
		if (pKeyboard->GetPress(DIK_W) || pXInput->GetThumbLY(0) >= MIN_GAMEPAD_LEFT_THUMB_Y || DirectInputStick->aGamePad.lY < -GAMEPAD_DEADZONE)
		{// �E�O
			m_fRotDest = D3DX_PI * 0.25f;
			fDirMove = D3DX_PI * 0.25f;
		}
		else if (pKeyboard->GetPress(DIK_S) || pXInput->GetThumbLY(0) <= -MIN_GAMEPAD_LEFT_THUMB_Y || DirectInputStick->aGamePad.lY > GAMEPAD_DEADZONE)
		{// �E���
			m_fRotDest = D3DX_PI * -0.25f;
			fDirMove = D3DX_PI * 0.75f;
		}
		else
		{// �E
			m_fRotDest = D3DX_PI * 0.5f;
			fDirMove = D3DX_PI * 0.5f;
		}

		bKey = true;
	}
	else if (pKeyboard->GetPress(DIK_W) || pXInput->GetThumbLY(0) >= MIN_GAMEPAD_LEFT_THUMB_Y || DirectInputStick->aGamePad.lY < -GAMEPAD_DEADZONE)
	{// �������ֈړ�
		m_fRotDest = 0.0f;
		fDirMove = 0.0f;
		bKey = true;
	}
	else if (pKeyboard->GetPress(DIK_S) || pXInput->GetThumbLY(0) <= -MIN_GAMEPAD_LEFT_THUMB_Y || DirectInputStick->aGamePad.lY > GAMEPAD_DEADZONE)
	{// �������ֈړ�
		m_fRotDest = 0.0f;
		fDirMove = D3DX_PI;
		bKey = true;
	}

	if (bKey)
	{// �ړ��ʂ̉��Z
		m_move = D3DXVECTOR3(sinf(fDirMove + fRotY), 0.0f, cosf(fDirMove + fRotY)) * m_fSpeed;
	}

	if (NULL != m_pUpperMotion && NULL != m_pLowerMotion)
	{// ���[�V�����N���X���g���Ă���
		float fDiffX = (float)CManager::GetInputMouse()->GetDiffPointX();
		if (bKey)
		{// �ړ����[�V����
			if (fDirMove >= D3DX_PI * 0.75f || fDirMove <= D3DX_PI * -0.75f)
			{// �o�b�N����
				m_pUpperMotion->SetMotion(CMotionManager::TYPE_BACK);
				m_pLowerMotion->SetMotion(CMotionManager::TYPE_BACK);
			}
			else
			{// �ʏ�ړ�
				m_pUpperMotion->SetMotion(CMotionManager::TYPE_WALK);
				m_pLowerMotion->SetMotion(CMotionManager::TYPE_WALK);
			}
		}
		else if (fDiffX == 0.0f)
		{// �j���[�g�������[�V����
			m_pUpperMotion->SetMotion(CMotionManager::TYPE_NEUTRAL);
			m_pLowerMotion->SetMotion(CMotionManager::TYPE_NEUTRAL);
		}
		else if (fDiffX < 10.0f || fDiffX > -10.0f)
		{// �ړ����[�V����
			m_pLowerMotion->SetMotion(CMotionManager::TYPE_WALK);
		}
	}

	m_move.x += (0 - m_move.x) * 0.4f;
	m_move.z += (0 - m_move.z) * 0.4f;

	D3DXVECTOR3 pos = m_pos + m_move;

	// �}�b�v�̓����蔻��
	if (CCollision::Collision(&pos, m_posOld, m_vtxMax, m_vtxMin)) { m_pos = pos; }
	else{ m_pos += m_move; }
}

//=========================================
// �e��������
//=========================================
void CPlayer::Shoot(void)
{
	CInputMouse *pMouse = CManager::GetInputMouse();	// �}�E�X�̓��͂��擾
	CXInput *pXInput = CManager::GetXInput();			// XInput�̓��͂��擾
	D3DXVECTOR3 dispertion;								// �u��

	if (pMouse->GetPress(CInputMouse::DIMS_BUTTON_0) && m_nRemBullet > 0 && m_bChatBotton == false)
	{
		// �e�̔��ˊԊu
		m_nCntShoot = (m_nCntShoot + 1) % 7;

		if ((m_mecha != CMechaSelect::MECHATYPE_LIGHT && !m_bShootButton) || (m_mecha == CMechaSelect::MECHATYPE_LIGHT &&m_nCntShoot == 1))
		{
			for (int nCntShoots = 0; nCntShoots < m_nNumShoot; nCntShoots++)
			{
				// �J�����̊p�x�ƒ����_���擾
				D3DXVECTOR3 rotCamera = CManager::GetCamera()->GetRot();
				D3DXVECTOR3 posR = CManager::GetCamera()->GetPosR();

				// ���e�B�N���i�ړI�̈ʒu�j�̎擾
				D3DXVECTOR3 posReticle = m_pReticle->GetPos();
				if (m_nDispertion != 0)
				{// �u�����O�łȂ��Ƃ�
					dispertion = D3DXVECTOR3((float)(m_nDispertion - rand() % (m_nDispertion * 2)), 0.0f, (float)(m_nDispertion - rand() % (m_nDispertion * 2)));
					posReticle += dispertion;
				}

				// �ˏo���̈ʒu�̎擾
				D3DXMATRIX mtxCanon = m_pModel[2]->GetMtxWorld();
				D3DXVECTOR3 posCanon = D3DXVECTOR3(mtxCanon._41, mtxCanon._42, mtxCanon._43) + D3DXVECTOR3(sinf(rotCamera.y) * 30.0f, cosf(rotCamera.x) * 30.0f, cosf(rotCamera.y) * 30.0f);

				// ���������̊p�x�̌v�Z
				m_pAngle[nCntShoots * 2] = atan2f(posReticle.x - posCanon.x, posReticle.z - posCanon.z);
				m_pAngleV[nCntShoots * 2] = rotCamera.x;
				if (m_nDispertion != 0) { m_pAngleV[nCntShoots * 2] += (float)(m_nDispertion - (rand() % m_nDispertion * 2)) * 0.0005f; }

				// �e�̐���
				CBulletPlayer::Create(posCanon, m_pAngle[nCntShoots * 2], m_pAngleV[nCntShoots * 2], m_nAttack, m_nTeam);
				CParticle::Create(posCanon, 2);

				// ���e�B�N���i�ړI�̈ʒu�j�̎擾
				posReticle = m_pReticle->GetPos();
				if (m_nDispertion != 0)
				{// �u�����O�łȂ��Ƃ�
					dispertion = D3DXVECTOR3((float)(m_nDispertion - rand() % (m_nDispertion * 2)), 0.0f, (float)(m_nDispertion - rand() % (m_nDispertion * 2)));
					posReticle += dispertion;
				}

				// �ˏo���̈ʒu�̎擾
				mtxCanon = m_pModel[3]->GetMtxWorld();
				posCanon = D3DXVECTOR3(mtxCanon._41, mtxCanon._42, mtxCanon._43) + D3DXVECTOR3(sinf(rotCamera.y) * 30.0f, cosf(rotCamera.x) * 30.0f, cosf(rotCamera.y) * 30.0f);

				// ���������̊p�x�̌v�Z
				m_pAngle[nCntShoots * 2 + 1] = atan2f(posReticle.x - posCanon.x, posReticle.z - posCanon.z);
				m_pAngleV[nCntShoots * 2 + 1] = rotCamera.x;
				if (m_nDispertion != 0) { m_pAngleV[nCntShoots * 2 + 1] += (float)(m_nDispertion - (rand() % m_nDispertion * 2)) * 0.0005f; }

				// �e�̐���
				CBulletPlayer::Create(posCanon, m_pAngle[nCntShoots * 2 + 1], m_pAngleV[nCntShoots * 2 + 1], m_nAttack, m_nTeam);
				CParticle::Create(posCanon, 2);

				m_bShoot = true;
			}

			m_nRemBullet--;
			m_bShootButton = true;
		}
	}
	else
	{
		// ���˃{�^�������t���O�𕉂ɂ���
		m_bShootButton = false;
	}

	// �����[�h����
	if (pMouse->GetPress(CInputMouse::DIMS_BUTTON_1))
	{
		m_nRemBullet = m_nCapacity;
	}

	// �����[�h����
	Reload();

	// �c�e�̐ݒ�
	m_pUINum[0]->SetRemainBullet(m_nRemBullet);
}

//=========================================
// �p�x�X�V
//=========================================
void CPlayer::Angle(void)
{
	if (m_bConnect == true)
	{
		// �J�����̊p�x�����߂�
		D3DXVECTOR3 rotCamera = CManager::GetCamera()->GetRot();
		float fDiffRot;
		float fAngle = D3DX_PI + rotCamera.y;
		CDebugProc::Print("rotCamera: %.2f, %.2f, %.2f", rotCamera.x, rotCamera.y, rotCamera.z);

		// �ړI�̊p�x�̒���
		if (m_fRotDest > D3DX_PI) { m_fRotDest -= D3DX_PI * 2.0f; }
		if (m_fRotDest < -D3DX_PI) { m_fRotDest += D3DX_PI * 2.0f; }

		// �ړI�̊p�x�ւ̍���
		fDiffRot = m_fRotDest - m_rot.y;
		if (fDiffRot > D3DX_PI) { fDiffRot -= D3DX_PI * 2.0f; }
		if (fDiffRot < -D3DX_PI) { fDiffRot += D3DX_PI * 2.0f; }

		// �p�x�̍X�V
		m_rot.y += fDiffRot * 0.05f;
		if (m_rot.y > D3DX_PI) { m_rot.y -= D3DX_PI * 2.0f; }
		if (m_rot.y < -D3DX_PI) { m_rot.y += D3DX_PI * 2.0f; }

		m_fCameraAngle = fAngle - m_rot.y;

		// �����̒���
		if (m_fCameraAngle > D3DX_PI) { m_fCameraAngle -= D3DX_PI * 2.0f; }
		if (m_fCameraAngle < -D3DX_PI) { m_fCameraAngle += D3DX_PI * 2.0f; }

		D3DXVECTOR3 rot = m_pModel[1]->GetRot();

		if (m_fRotDest <= D3DX_PI * 0.5f && m_fRotDest >= D3DX_PI * -0.5f)
		{// �����g�̓�����i�s�����ɍ��킹��
			D3DXVECTOR3 rot = m_pModel[0]->GetRot();
			m_pModel[0]->SetRot(D3DXVECTOR3(rot.x, m_rot.y + m_fCameraAngle, rot.z));
			rot = m_pModel[1]->GetRot();
			rot = D3DXVECTOR3(-rotCamera.x + (D3DX_PI * 0.5f), m_fCameraAngle - fAngle, rot.z);
			m_pModel[1]->SetRot(rot);
		}
		else
		{// �΂ߌ������̂Ƃ�
			D3DXVECTOR3 rot = m_pModel[1]->GetRot();
			rot = D3DXVECTOR3(-rotCamera.x + (D3DX_PI * 0.5f), m_fCameraAngle - (fAngle - D3DX_PI), rot.z);
			m_pModel[1]->SetRot(rot);
		}
	}
	else
	{
		// �J�����̊p�x�����߂�
		D3DXVECTOR3 rotCamera = m_fRotDestUpper;
		float fDiffRot;
		float fAngle = D3DX_PI + rotCamera.y;

		// �ړI�̊p�x�̒���
		if (m_fRotDest > D3DX_PI) { m_fRotDest -= D3DX_PI * 2.0f; }
		if (m_fRotDest < -D3DX_PI) { m_fRotDest += D3DX_PI * 2.0f; }

		// �ړI�̊p�x�ւ̍���
		fDiffRot = m_fRotDest - m_rot.y;
		if (fDiffRot > D3DX_PI) { fDiffRot -= D3DX_PI * 2.0f; }
		if (fDiffRot < -D3DX_PI) { fDiffRot += D3DX_PI * 2.0f; }

		// �p�x�̍X�V
		m_rot.y += fDiffRot * 0.05f;
		if (m_rot.y > D3DX_PI) { m_rot.y -= D3DX_PI * 2.0f; }
		if (m_rot.y < -D3DX_PI) { m_rot.y += D3DX_PI * 2.0f; }

		m_fCameraAngle = fAngle - m_rot.y;

		// �����̒���
		if (m_fCameraAngle > D3DX_PI) { m_fCameraAngle -= D3DX_PI * 2.0f; }
		if (m_fCameraAngle < -D3DX_PI) { m_fCameraAngle += D3DX_PI * 2.0f; }

			if (m_fRotDest <= D3DX_PI * 0.5f && m_fRotDest >= D3DX_PI * -0.5f)
			{// �����g�̓�����i�s�����ɍ��킹��
				D3DXVECTOR3 rot = m_pModel[0]->GetRot();
				m_pModel[0]->SetRot(D3DXVECTOR3(rot.x, m_rot.y + m_fCameraAngle, rot.z));
				rot = m_pModel[1]->GetRot();
				rot = D3DXVECTOR3(-rotCamera.x + (D3DX_PI * 0.5f), m_fCameraAngle - fAngle, rot.z);
				m_pModel[1]->SetRot(rot);
			}
			else
			{// �΂ߌ������̂Ƃ�
				D3DXVECTOR3 rot = m_pModel[1]->GetRot();
				rot = D3DXVECTOR3(-rotCamera.x + (D3DX_PI * 0.5f), m_fCameraAngle - (fAngle - D3DX_PI), rot.z);
				m_pModel[1]->SetRot(rot);
			}


	}
}

//=========================================
// �p�[�e�B�N���𔭐�������
//=========================================
void CPlayer::SetParticle(void)
{
	if (m_nLife < m_nLifeMax / 4)
	{// �ϋv�͂��S���̂P�ɂȂ���
		int nParts;
		D3DXMATRIX mtx;
		D3DXVECTOR3 pos;

		if (rand() % 20 == 0)
		{
			nParts = rand() % m_nNumParts;
			mtx = m_pModel[nParts]->GetMtxWorld();
			pos = D3DXVECTOR3(mtx._41, mtx._42, mtx._43);
			CParticle::Create(pos, 3);
		}

		nParts = rand() % m_nNumParts;
		mtx = m_pModel[nParts]->GetMtxWorld();
		pos = D3DXVECTOR3(mtx._41, mtx._42, mtx._43);
		CParticle::Create(pos, 0);
	}
	else if (m_nLife < m_nLifeMax / 2)
	{// �ϋv�͂������ɂȂ���
		if (rand() % 30 == 0)
		{
			int nParts = rand() % m_nNumParts;
			D3DXMATRIX mtx = m_pModel[nParts]->GetMtxWorld();
			D3DXVECTOR3 pos = D3DXVECTOR3(mtx._41, mtx._42, mtx._43);
			CParticle::Create(pos, 3);
		}
	}
}

//=========================================
// �_���[�W���󂯂��Ƃ��̏���
//=========================================
void CPlayer::Damage(int nDamage)
{
	if (NULL != m_pUpperMotion && NULL != m_pLowerMotion)
	{// ���[�V�����N���X���g���Ă���
		if (m_pUpperMotion->GetType() != CMotionManager::TYPE_DAMAGE && m_pLowerMotion->GetType() != CMotionManager::TYPE_DAMAGE)
		{// ���C�t�N���X���g���Ă���
			m_pUpperMotion->SetMotion(CMotionManager::TYPE_DAMAGE);	// �_���[�W���[�V�������Đ�
			m_pLowerMotion->SetMotion(CMotionManager::TYPE_DAMAGE);	// �_���[�W���[�V�������Đ�

			m_state = STATE_DAMAGE;								// �_���[�W���󂯂Ă����Ԃɂ���

			m_nLife -= nDamage;

			if (0 >= m_nLife)
			{
				m_nLife = 0;
				m_bDeath = true;
			}
			//CSound *pSound = CManager::GetSound();				// �T�E���h�̎擾
			//pSound->PlaySound(CSound::SOUND_LABEL_DAMAGE);		// �_���[�W�����Đ�
		}
	}
}

//=========================================
// �X�N���[�����W�����[���h���W�ɕϊ�
//=========================================
D3DXVECTOR3 CPlayer::CalcScreenToWorld(float fScreenX, float fScreenY)
{
	// �e�s��̋t�s����Z�o
	D3DXMATRIX InvView, InvPrj, VP, InvViewport;
	CCamera *pCamera = CManager::GetCamera();
	D3DXMatrixInverse(&InvView, NULL, &pCamera->GetView());
	D3DXMatrixInverse(&InvPrj, NULL, &pCamera->GetProjection());
	D3DXMatrixIdentity(&VP);
	VP._11 = SCREEN_WIDTH / 2.0f; VP._22 = -SCREEN_HEIGHT / 2.0f;
	VP._41 = SCREEN_WIDTH / 2.0f; VP._42 = SCREEN_HEIGHT / 2.0f;
	D3DXMatrixInverse(&InvViewport, NULL, &VP);

	// �t�ϊ�
	D3DXMATRIX tmp = InvViewport * InvPrj * InvView;
	D3DXVECTOR3 pos;
	D3DXVec3TransformCoord(&pos, &D3DXVECTOR3(fScreenX, fScreenY, 1.0f), &tmp);

	return pos;
}

//=========================================
// �����[�h�����F�e��0�[�������[�h���S�A�Q�[�W�\��
//=========================================
void CPlayer::Reload(void)
{
	if (m_nRemBullet <= 0)
	{
		m_nCntReRoad++;		// �J�E���^�[���Z

		if (m_bConnect == true)
		{
			if (m_pUITexReload[0] == NULL || m_pUITexReload[1] == NULL || m_pGauge == NULL)
			{	// NULL�`�F�b�N����̃����[�h���S����
				m_pUITexReload[0] = CUI_TEXTURE::Create(D3DXVECTOR3(1100.0f, 650.0f, 0.0f), 200.0f, 80.0f, CUI_TEXTURE::UIFLAME_RELOAD);	// �e�̂Ƃ���
				m_pUITexReload[1] = CUI_TEXTURE::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), 200.0f, 80.0f, CUI_TEXTURE::UIFLAME_RELOAD);	// ��ʒ���
				m_pGauge = CGauge2D::Create(2, D3DXVECTOR3(SCREEN_WIDTH / 2, 400.0f, 0.0f), 0.0f, 100.0f, 300.0f, 30.0f);
				m_pGauge->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), 0);	// ���̒���
				m_pGauge->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 1);	// ���݂̗̑�
			}
			if (m_pUITexReload[0] != NULL || m_pUITexReload[1] != NULL || m_pGauge != NULL)
			{	// �����[�h���S�_��
				m_pUITexReload[0]->Flashing(30);
				m_pUITexReload[1]->Flashing(30);

				// �Q�[�W����
				m_pGauge->AddSubtract(100.0f / m_nReload);		// 100�� / �b��
			}
		}
		if (m_nCntReRoad % m_nReload == 0)
		{	// �ݒ肵���b���と���S���Q�[�W�j�����e����
			m_nRemBullet = m_nCapacity;			// �e����

			if (m_bConnect == true)
			{
				if (m_pGauge != NULL)
				{	// �Q�[�W�̔j��
					m_pGauge->Uninit();
					m_pGauge = NULL;
				}

				for (int nCnt = 0; nCnt < RELOAD_TEX; nCnt++)
				{	// �����[�h���S�̔j��
					if (m_pUITexReload[nCnt] != NULL)
					{
						m_pUITexReload[nCnt]->Uninit();
						m_pUITexReload[nCnt] = NULL;
					}
				}
			}
		}
	}
}

//=========================================
// ���X�|�[������
//=========================================
void CPlayer::Respawn(RESPAWN respawn)
{
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();	// �L�[�{�[�h�̓��͂��擾
	CXInput *pXInput = CManager::GetXInput();					// XInput�̓��͂��擾

	switch (respawn)
	{
	case RESPAWN_START:
		if (m_pUITexRespawn[0] == NULL || m_pUITexRespawn[1] == NULL)
		{
			m_pUITexRespawn[0] = CUI_TEXTURE::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), 500.0f, 260.0f, CUI_TEXTURE::UIFLAME_TILE_PATTERN);	// �^�C��
			m_pUITexRespawn[1] = CUI_TEXTURE::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), 350.0f, 150.0f, CUI_TEXTURE::UIFLAME_START);	// �퓬�J�n���S
		}
		if (m_pUITexRespawn[0] != NULL || m_pUITexRespawn[1] != NULL)
		{
			m_nRespawnTimer++;
			if (m_nRespawnTimer % 60 == 0)
			{
				m_nDisTime--;
			}
			if (m_nDisTime <= 0)
			{	//	�\������0�ɂȂ���
				for (int nCnt = 0; nCnt < RESPAWN_TEX; nCnt++)
				{
					if (m_pUITexRespawn[nCnt] != NULL)
					{	// UI�e�N�X�`���̔j��
						m_pUITexRespawn[nCnt]->Uninit();
						m_pUITexRespawn[nCnt] = NULL;
					}
				}
				respawn = RESPAWN_NONE;
			}
		}
		break;

	case RESPAWN_DEATH:
		if (m_pUITexRespawn[0] == NULL || m_pUITexRespawn[1] == NULL || m_pUITexRespawn[2] == NULL || m_pUINumRespawn == NULL)
		{
			m_pUITexRespawn[0] = CUI_TEXTURE::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), 500.0f, 260.0f, CUI_TEXTURE::UIFLAME_TILE_PATTERN);	// �^�C��
			m_pUITexRespawn[1] = CUI_TEXTURE::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), 350.0f, 150.0f, CUI_TEXTURE::UIFLAME_DEATH);	// �s���s�\���S
			m_pUITexRespawn[2] = CUI_TEXTURE::Create(D3DXVECTOR3(560.0f, 460.0f, 0.0f), 200.0f, 30.0f, CUI_TEXTURE::UIFLAME_RESPAWN);	// ������A
			m_pUINumRespawn = CUI_NUMBER::Create(D3DXVECTOR3(750.0f, 460.0f, 0.0f), 60.0f, 60.0f, 30.0f, CUI_NUMBER::UI_NUMTYPE_CNTRESPAWN, 0, NUMTEX_UV_X, NUMTEX_UV_Y);	// ������A�J�E���^�[
		}
		if (m_pUITexRespawn[0] != NULL || m_pUITexRespawn[1] != NULL || m_pUITexRespawn[2] != NULL || m_pUINumRespawn != NULL)
		{	// �������Ă��鎞
			int nRespawn = m_pUINumRespawn->GetRespawn();			// CUI_NUMBER����A������A�J�E���^�[�擾
			if (nRespawn <= 0)
			{	// �J�E���^�[0�ȉ��ɂȂ�����
				respawn = RESPAWN_SELECT;		// ���X�|�[���I����Ԃɐݒ�

				for (int nCnt = 0; nCnt < RESPAWN_TEX; nCnt++)
				{
					if (m_pUITexRespawn[nCnt] != NULL)
					{	// UI�e�N�X�`���̔j��
						m_pUITexRespawn[nCnt]->Uninit();
						m_pUITexRespawn[nCnt] = NULL;
					}
				}

				if (m_pUINumRespawn != NULL)
				{	// ������A�J�E���^�[�̔j��
					m_pUINumRespawn->Uninit();
					m_pUINumRespawn = NULL;
				}
			}
		}
		break;
	}

	m_Respawn = respawn;

#ifdef _DEBUG
	CDebugProc::Print("m_nDisTime : %d\n", m_nDisTime);
	CDebugProc::Print("m_nRespawnTimer : %d\n", m_nRespawnTimer);
#endif
}

//=========================================
// ���X�|�[���I������
//=========================================
void CPlayer::SelectRespawn(void)
{
	if (m_pUITexSelectRes[0] == NULL || m_pUITexSelectRes[1] == NULL || m_pUITexSelectRes[2] == NULL || m_pUITexSelectRes[3] == NULL || m_pCursor == NULL)
	{	// UI�̐���
		m_pUITexSelectRes[0] = CUI_TEXTURE::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), SCREEN_WIDTH, SCREEN_HEIGHT, CUI_TEXTURE::UIFLAME_NONE);	// ���n
		m_pUITexSelectRes[1] = CUI_TEXTURE::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, 360.0f, 0.0f), 500.0f, 370.0f, CUI_TEXTURE::UIFLAME_MAP);	// �}�b�v
		m_pUITexSelectRes[2] = CUI_TEXTURE::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, 120.0f, 0.0f), 400.0f, 80.0f, CUI_TEXTURE::UIFLAME_TITLE);	// ���X�|�[���^�C�g��
		m_pUITexSelectRes[3] = CUI_TEXTURE::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, PLAYER_UI_HEIGHT, 0.0f), 500.0f, 100.0f, CUI_TEXTURE::UIFLAME_RESPAWN_FLAME);		// �t���[��
		m_pCursor = CMouseCursor2D::Create();		// �J�[�\��
	}

	for (int nCnt = 0; nCnt < SELECTRESPAWN_BOTTON; nCnt++)
	{
		if (m_pUISelectResBotton[nCnt] == NULL)
		{	// �{�^���̐���
			m_pUISelectResBotton[nCnt] = CButton2D::Create(D3DXVECTOR3(470.0f + (nCnt *(PLAYER_BOTTON_WIDTH + PLAYER_BOTTON_INT)), PLAYER_UI_HEIGHT, 0.0f),
				PLAYER_BOTTON_WIDTH, PLAYER_BOTTON_HEIGHT);  	// ���X�|�[���{�^���̉���
			m_pUISelectResBotton[nCnt]->BindTexture(CTexture::GetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_SELECT_RESPAWN)));
			m_pUISelectResBotton[nCnt]->SetTex(nCnt, 1, 4);
		}
	}

	if (m_pUISelectResBotton[0] != NULL || m_pUISelectResBotton[1] != NULL || m_pUISelectResBotton[2] != NULL || m_pUISelectResBotton[3] != NULL)
	{	// ��������Ă�����
		bool bBottonSwitch = false;
		int nSelect = -1;
		m_point = POINT_A;

		// �{�^���̔���
		for (int nCntButton = 0; nCntButton < SELECTRESPAWN_BOTTON; nCntButton++)
		{
			if (m_pUISelectResBotton[nCntButton]->InRange(m_pCursor->GetMousePosition()))
			{// �͈͓����`�F�b�N
				if (m_pUISelectResBotton[nCntButton]->ClickRelease())
				{// �N���b�N���ꂽ
					bBottonSwitch = true;
					m_point = (POINT)nCntButton;
					break;
				}
				nSelect = nCntButton;
			}
		}

		// �{�^�������ꂽ���̏���
		if (bBottonSwitch)
		{	// true�̎�
			switch (m_point)
			{
			case POINT_A:
				break;

			case POINT_B:
				break;

			case POINT_C:
				break;

			case POINT_D:
				break;
			}

			// �ʏ��Ԃɖ߂�
			m_Respawn = RESPAWN_NONE;

			// ���C�t�̐ݒ�
			m_nLife = m_nLifeMax;

			// ���X�|�[���n�_�����肵����j������
			for (int nCnt = 0; nCnt < SELECTRESPAWN_BOTTON; nCnt++)
			{
				if (m_pUISelectResBotton[nCnt] != NULL)
				{	// �{�^���̔j��
					m_pUISelectResBotton[nCnt]->Uninit();
					m_pUISelectResBotton[nCnt] = NULL;
				}
			}

			for (int nCnt = 0; nCnt < SERECTRESPAWN_TEX; nCnt++)
			{
				if (m_pUITexSelectRes[nCnt] != NULL)
				{	// �e�N�X�`���̔j��
					m_pUITexSelectRes[nCnt]->Uninit();
					m_pUITexSelectRes[nCnt] = NULL;
				}
			}

			if (m_pCursor != NULL)
			{	// �J�[�\���̔j��
				m_pCursor->Uninit();
				m_pCursor = NULL;
			}
		}	// �{�^�������ꂽ��
	}	// �������ꂽ

#ifdef _DEBUG
	CDebugProc::Print("���X�|�[���I��");
#endif

}

//=============================================================================
//	���W�I�`���b�g�{�^����������
//=============================================================================
void CPlayer::ChatBotton(void)
{
}

//=============================================================================
//	�{�^����������āA���b�Z�[�W�\��
//=============================================================================
void CPlayer::ChatMess(bool bChat)
{
	if (m_pUITexRadio == NULL)
	{
		m_bChatBotton = false;		// �{�^����\����

		//m_pUITexRadio = CUI_TEXTURE::Create(D3DXVECTOR3(1280.0f, 550.0f, 0.0f), RADIOCHAT_MESS_WIDTH, RADIOCHAT_MESS_HEIGHT, CUI_TEXTURE::UIFLAME_RADIOCHAT_MESS);
		//m_pUITexRadio->SetTex(m_radiochat, 1, RADIOCHAT_BOTTON_PATTERN);
	}
	if (m_pUITexRadio != NULL)
	{
		for (int nCnt = 0; nCnt < RADIOCHAT_BOTTON; nCnt++)
		{
			if (m_pUIRadioBotton[nCnt] != NULL)
			{	// ���W�I�`���b�g�{�^���̔j��
				m_pUIRadioBotton[nCnt]->Uninit();
				m_pUIRadioBotton[nCnt] = NULL;
			}
		}

		if (m_pCursor != NULL)
		{	// �J�[�\���̔j��
			m_pCursor->Uninit();
			m_pCursor = NULL;
		}

		bool bMove = false;		// �~�܂������ǂ���
		D3DXVECTOR3 texPos = m_pUITexRadio->GetPos();		// ���݂̈ʒu���擾
		D3DXCOLOR texCol = m_pUITexRadio->GetColor();		// ���݂̐F���擾

		if (texPos.x >= 1080.0f)
		{	// �ݒ肵���ʒu�ȏゾ�����瓮��
			texPos.x -= m_moveSpeed;					// �e�N�X�`��������
		}
		else
		{	// �ݒ肵���ʒu�ȉ���������A�~�܂�
			bMove = true;		// �~�܂����I
		}

		if (bMove == true && texCol.a >= 1.0f)
		{	// �~�܂��� && �����x��1.0�ȏ�̎�
			m_nTexTimer++;		// �J�E���^�[���Z
			if (m_nTexTimer % RADIOCHAT_DISPLAY_TIME == 0)
			{	// 5�b�o������
				m_nTexTimer = 0;
				m_bCol = true;
			}
		}
		if (m_bCol == true)
		{
			texCol.a -= RADIOCHAT_COL;
		}

		m_pUITexRadio->SetPos(texPos);			// �ʒu���X�V
		m_pUITexRadio->SetColor(texCol);		// �F���X�V

		if (texCol.a <= 0.0f)
		{	// ���S�ɐF������
			if (m_pUITexRadio != NULL)
			{	// ���b�Z�[�W�e�N�X�`���̔j��
				m_pUITexRadio->Uninit();
				m_pUITexRadio = NULL;
			}
			m_bChat = false;					// �`���b�g���Ă��Ȃ�
			bMove = false;
			m_bCol = false;
		}
	}
}

//=============================================================================
//	AI�퓬�p�̃e�L�X�g�ǂݍ��ݏ���
//=============================================================================
void CPlayer::LoadBattleFile(void)
{
	FILE *pFile;									// �ǂݍ��ރt�@�C��
	char acLine[MAX_CHAR];							// 1�s�ǂݍ��ޕϐ�
	char acData[MAX_CHAR];							// 1���ǂݍ��ޕϐ�


	char *pBattleFileName = "";	//�ǂݍ��ރe�L�X�g
	if (CMechaSelect::MECHATYPE_ASSULT == m_mecha) { pBattleFileName = ASSULT_BATTLE_FILE; }
	else if (CMechaSelect::MECHATYPE_LIGHT == m_mecha) { pBattleFileName = LIGHT_BATTLE_FILE; }
	else if (CMechaSelect::MECHATYPE_HEAVY == m_mecha) { pBattleFileName = HEAVY_BATTLE_FILE; }
	else if (CMechaSelect::MECHATYPE_SHOOTER == m_mecha) { pBattleFileName = SNIPE_BATTLE_FILE; }

	// �t�@�C�����J��
	pFile = fopen(pBattleFileName, "r");


	if (pFile != NULL)
	{
		while (fgets(&acLine[0], MAX_CHAR, pFile) != NULL)
		{
			sscanf(&acLine[0], "%s", &acData);				// �����f�[�^�ɃR�s�[

			if (memcmp(acData, "SCRIPT", 6) == 0)
			{// "SCRIPT"�̕����񂪂�������
				while (strcmp(acData, "END_SCRIPT") != 0)
				{// "END_SCRIPT"������܂ŏ����擾��������
					fgets(&acLine[0], MAXCHAR, pFile);					// �s���擾
					sscanf(&acLine[0], "%s", &acData);					// �f�[�^���R�s�[
					if (memcmp(acData, "CHARACTERSET", 12) == 0)
					{// "CHARACTERSET"�̕����񂪂�������

						m_pSearch = new CSearch*[MAX_SEARCH];
						for (int nCntSearch = 0; nCntSearch < MAX_SEARCH; nCntSearch++)
						{//�������[�v
							m_pSearch[nCntSearch] = CSearch::Create(&m_pModel[1]->GetMtxWorld());

							m_pSearch[nCntSearch]->SetParent(NULL);
						}

						while (strcmp(acData, "END_CHARACTERSET") != 0)
						{// "END_CHARACTERSET"������܂ŏ����擾��������
							fgets(&acLine[0], MAXCHAR, pFile);					// �s���擾
							sscanf(&acLine[0], "%s", &acData);					// �f�[�^���R�s�[
							if (strcmp(acData, "SEARCH_LENGTH") == 0)
							{// SEARCH_LENGTH����������
								sscanf(&acLine[0], "%s %s %f", &acData[0], &acData[0], &m_fSearchLength);	// �F������
								for (int nCntSearch = 0; nCntSearch < 3; nCntSearch++)
								{//�������[�v
									m_pSearch[nCntSearch]->SetPos(D3DXVECTOR3(0.0f, 0.0f, -m_fSearchLength));
								}
								m_pSearch[3]->SetPos(D3DXVECTOR3(0.0f, 0.0f, 150.0f));
							}
							if (strcmp(acData, "SEARCH_ANGLE") == 0)
							{// SEARCH_ANGLE����������
								float rot;
								sscanf(&acLine[0], "%s %s %f", &acData[0], &acData[0], &rot);	// �F���p�x
																								//�e�L�X�g�̊p�x�ʂ�ɑ������
								m_fSearchAngle = (D3DX_PI * 2 / 360.0f * rot);
								for (int nCntSearch = 1; nCntSearch < 3; nCntSearch++)
								{//�������[�v
									if (nCntSearch = 1)
									{
										m_pSearch[nCntSearch]->SetRot(D3DXVECTOR3(0.0f, m_fSearchAngle, 0.0f));
									}
									if (nCntSearch = 2)
									{
										m_pSearch[nCntSearch]->SetRot(D3DXVECTOR3(0.0f, -m_fSearchAngle, 0.0f));
									}
								}
							}
						}
					}
				}
			}//SCRIPT
		}//while
		fclose(pFile);// �J�����t�@�C�������
	}//�t�@�C�����J���Ă���
	else
	{// �t�@�C�����Ȃ��Ƃ�
		MessageBox(0, "�t�@�C��������܂���", "�퓬�p�e�L�X�g", MB_YESNO);
	}

}

//=============================================================================
//	����̏���
//=============================================================================
bool CPlayer::Distance(void)
{
	bool bFind = false;

	D3DXMATRIX mtxSearch1 = m_pSearch[1]->GetMtxWorld();
	D3DXMATRIX mtxSearch2 = m_pSearch[2]->GetMtxWorld();


	//��̃x�N�g���ɑ��
	D3DXVECTOR3 VecPos_0 = D3DXVECTOR3(mtxSearch1._41, mtxSearch1._42, mtxSearch1._43);//����
	D3DXVECTOR3 VecPos_1 = D3DXVECTOR3(mtxSearch2._41, mtxSearch2._42, mtxSearch2._43);//�E��

	D3DXVECTOR3 EmpVec_0;//����
	D3DXVECTOR3 EmpVec_1;//�E��
	D3DXVECTOR3 EmpVec_2;//���E��

	D3DXVECTOR3 Vec_C_P;//CPU�ƃv���C���[�̃x�N�g��
	D3DXVECTOR3 Vec_0_P;//�_�O�ƃv���C���[�̃x�N�g��
	D3DXVECTOR3 Vec_1_P;//�_�P�ƃv���C���[�̃x�N�g��


	float fCrossProduct0;//�O�όv�Z�p
	float fCrossProduct1;//�O�όv�Z�p
	float fCrossProduct2;//�O�όv�Z�p


	// �G��T��
	CScene *pScene = CScene::GetSceneTop(FIND_FIND_CHARACTER_PRIORITY);
	CScene *pSceneNext = NULL;
	while (pScene != NULL)
	{// NULL�ɂȂ�܂Ń��[�v
		pSceneNext = pScene->GetSceneNext();
		CScene::OBJTYPE objType = pScene->GetObjType();

		if (CScene::OBJTYPE_PLAYER == objType)
		{//�v���C���[�I�u�W�F�N�g�̂Ƃ�
			CPlayer *pPlayer = (CPlayer*)pScene;
			int nTeam = pPlayer->GetTeam();
			int nIdxPlayer = pPlayer->GetPlayerIdx();
			if (m_nTeam != nTeam)
			{//�`�[�����Ⴄ�Ƃ�
					//�F���p�x�N�g���̌v�Z
					m_SearchVec_0 = CSearch::Sub_Vector(VecPos_1, m_pos);
					EmpVec_0 = CSearch::Sub_Vector(pPlayer->GetPos(), VecPos_1);

					m_SearchVec_1 = CSearch::Sub_Vector(VecPos_0, VecPos_1);
					EmpVec_1 = CSearch::Sub_Vector(pPlayer->GetPos(), VecPos_0);

					m_SearchVec_2 = CSearch::Sub_Vector(m_pos, VecPos_0);
					EmpVec_2 = CSearch::Sub_Vector(pPlayer->GetPos(), m_pos);

					//�O�όv�Z
					fCrossProduct0 = (m_SearchVec_0.x * EmpVec_0.z) - (m_SearchVec_0.z * EmpVec_0.x);
					fCrossProduct1 = (m_SearchVec_1.x * EmpVec_1.z) - (m_SearchVec_1.z * EmpVec_1.x);
					fCrossProduct2 = (m_SearchVec_2.x * EmpVec_2.z) - (m_SearchVec_2.z * EmpVec_2.x);



					CDebugProc::Print("CPU�x�N�g���P�F%.2f %.2f %.2f", m_SearchVec_0.x, m_SearchVec_0.y, m_SearchVec_0.z);
					CDebugProc::Print("CPU�x�N�g���Q�F%.2f %.2f %.2f", m_SearchVec_1.x, m_SearchVec_1.y, m_SearchVec_1.z);
					CDebugProc::Print("CPU�x�N�g���R�F%.2f %.2f %.2f", m_SearchVec_2.x, m_SearchVec_2.y, m_SearchVec_2.z);

					CDebugProc::Print("fCrossProduct0�F%.2f", fCrossProduct0);
					CDebugProc::Print("fCrossProduct1�F%.2f", fCrossProduct1);
					CDebugProc::Print("fCrossProduct2�F%.2f", fCrossProduct2);


					if ((fCrossProduct0 > 0.0f && fCrossProduct1 > 0.0f && fCrossProduct2 > 0.0f) || (fCrossProduct0 < 0.0f && fCrossProduct1 < 0.0f && fCrossProduct2 < 0.0f))
					{//�O�p�`�̓����ɓG������
						bFind = true;
					}
					else
					{
						bFind = false;
					}

					CDebugProc::Print("bFind = %d", bFind);
			}
		}
		// ���̃I�u�W�F�N�g������
		pScene = pSceneNext;
	}

	return bFind;
}

//=============================================================================
// �퓬���̏���
//=============================================================================
void CPlayer::Battle(void)
{
	CScene *pScene = CScene::GetSceneTop(FIND_FIND_CHARACTER_PRIORITY);
	CScene *pSceneNext = NULL;
	D3DXVECTOR3 dispertion;								// �u��

	while (pScene != NULL)
	{// NULL�ɂȂ�܂Ń��[�v
		pSceneNext = pScene->GetSceneNext();
		CScene::OBJTYPE objType = pScene->GetObjType();

		if (CScene::OBJTYPE_PLAYER == objType)
		{//�v���C���[�I�u�W�F�N�g�̂Ƃ�
			CPlayer *pPlayer = (CPlayer*)pScene;
			int nTeam = pPlayer->GetTeam();
			if (m_nTeam != nTeam)
			{//�`�[�����Ⴄ�Ƃ�

				if (rand() % 20 == 0)
				{
					//�e����
					CpuShoot();
				}

			}//�`�[�����Ⴄ�Ƃ�
		}//�v���C���[�I�u�W�F�N�g�̂Ƃ�
		// ���̃I�u�W�F�N�g������
		pScene = pSceneNext;
	}

}

//=============================================================================
// �퓬���̈ړ��̏���
//=============================================================================
void CPlayer::BattleMovent(void)
{
	m_posOld = m_pos;
	bool bKey = false;				// �{�^�������t���O
	float fDirMove;

	// �G��T��
	CScene *pScene = CScene::GetSceneTop(FIND_FIND_CHARACTER_PRIORITY);
	CScene *pSceneNext = NULL;
	D3DXVECTOR3 dispertion;								// �u��

	while (pScene != NULL)
	{// NULL�ɂȂ�܂Ń��[�v
		pSceneNext = pScene->GetSceneNext();
		CScene::OBJTYPE objType = pScene->GetObjType();

		if (CScene::OBJTYPE_PLAYER == objType)
		{//�v���C���[�I�u�W�F�N�g�̂Ƃ�
			CPlayer *pPlayer = (CPlayer*)pScene;
			int nTeam = pPlayer->GetTeam();
			int IdxPlayer = pPlayer->GetPlayerIdx();
			if (m_nTeam != nTeam)
			{//�`�[�����Ⴄ�Ƃ�
				if (IdxPlayer == 0)
				{
					m_fRotDestUpper.y = atan2f(pPlayer->GetPos().x - m_pos.x, pPlayer->GetPos().z - m_pos.z);			//�㔼�g�̕���


					fDirMove = atan2f(pPlayer->GetPos().x - m_pos.x, pPlayer->GetPos().z - m_pos.z);			//�ړ�����

					bKey = true;
				}
			}//�`�[�����Ⴄ�Ƃ�
		}//�v���C���[�I�u�W�F�N�g�̂Ƃ�

		// ���̃I�u�W�F�N�g������
		pScene = pSceneNext;
	}

	if (bKey)
	{// �ړ��ʂ̉��Z
		m_move = D3DXVECTOR3(sinf(fDirMove), 0.0f, cosf(fDirMove)) * 3.0f;
	}

	if (NULL != m_pUpperMotion && NULL != m_pLowerMotion)
	{// ���[�V�����N���X���g���Ă���
		float fDiffX = (float)CManager::GetInputMouse()->GetDiffPointX();
		if (bKey)
		{// �ړ����[�V����
			if (fDirMove >= D3DX_PI * 0.75f || fDirMove <= D3DX_PI * -0.75f)
			{// �o�b�N����
				m_pUpperMotion->SetMotion(CMotionManager::TYPE_BACK);
				m_pLowerMotion->SetMotion(CMotionManager::TYPE_BACK);
			}
			else
			{// �ʏ�ړ�
				m_pUpperMotion->SetMotion(CMotionManager::TYPE_WALK);
				m_pLowerMotion->SetMotion(CMotionManager::TYPE_WALK);
			}
		}
		else if (fDiffX == 0.0f)
		{// �j���[�g�������[�V����
			m_pUpperMotion->SetMotion(CMotionManager::TYPE_NEUTRAL);
			m_pLowerMotion->SetMotion(CMotionManager::TYPE_NEUTRAL);
		}
		else if (fDiffX < 10.0f || fDiffX > -10.0f)
		{// �ړ����[�V����
			m_pLowerMotion->SetMotion(CMotionManager::TYPE_WALK);
		}
	}

	m_move.x += (0 - m_move.x) * 0.4f;
	m_move.z += (0 - m_move.z) * 0.4f;

	D3DXVECTOR3 pos = m_pos + m_move;

	// �}�b�v�̓����蔻��
	if (CCollision::Collision(&pos, m_posOld, m_vtxMax, m_vtxMin)) { m_pos = pos; }
	else { m_pos += m_move; }

}

//=============================================================================
// �e���˂̏���
//=============================================================================
void CPlayer::CpuShoot(void)
{
	D3DXVECTOR3 dispertion;								// �u��

	// �e�̔��ˊԊu
	m_nCntShoot = (m_nCntShoot + 1) % 7;

	if (m_nRemBullet > 0)
	{
		if ((m_mecha != CMechaSelect::MECHATYPE_LIGHT && !m_bShootButton) || (m_mecha == CMechaSelect::MECHATYPE_LIGHT &&m_nCntShoot == 1))
		{
			for (int nCntShoots = 0; nCntShoots < m_nNumShoot; nCntShoots++)
			{
				// �J�����̊p�x�ƒ����_���擾
				D3DXVECTOR3 rotCamera = CManager::GetCamera()->GetRot();
				D3DXVECTOR3 posR = CManager::GetCamera()->GetPosR();

				// ���e�B�N���i�ړI�̈ʒu�j�̎擾
				D3DXMATRIX MtxSearch = m_pSearch[0]->GetMtxWorld();
				D3DXVECTOR3 posReticle = D3DXVECTOR3(MtxSearch._41, MtxSearch._42, MtxSearch._43);
				if (m_nDispertion != 0)
				{// �u�����O�łȂ��Ƃ�
					dispertion = D3DXVECTOR3((float)(m_nDispertion - rand() % (m_nDispertion * 2)), 0.0f, (float)(m_nDispertion - rand() % (m_nDispertion * 2)));
					posReticle += dispertion;
				}

				// �ˏo���̈ʒu�̎擾
				D3DXMATRIX mtxCanon = m_pModel[2]->GetMtxWorld();
				D3DXVECTOR3 posCanon = D3DXVECTOR3(mtxCanon._41, mtxCanon._42, mtxCanon._43) + D3DXVECTOR3(sinf(rotCamera.y) * 30.0f, cosf(rotCamera.x) * 30.0f, cosf(rotCamera.y) * 30.0f);

				// ���������̊p�x�̌v�Z
				m_pAngle[nCntShoots * 2] = atan2f(posReticle.x - posCanon.x, posReticle.z - posCanon.z);
				m_pAngleV[nCntShoots * 2] = rotCamera.x;
				if (m_nDispertion != 0) { m_pAngleV[nCntShoots * 2] += (float)(m_nDispertion - (rand() % m_nDispertion * 2)) * 0.0005f; }

				// �e�̐���
				CBulletPlayer::Create(posCanon, m_pAngle[nCntShoots * 2], m_pAngleV[nCntShoots * 2], m_nAttack, m_nTeam);

				// ���e�B�N���i�ړI�̈ʒu�j�̎擾
				posReticle = D3DXVECTOR3(MtxSearch._41, MtxSearch._42, MtxSearch._43);
				if (m_nDispertion != 0)
				{// �u�����O�łȂ��Ƃ�
					dispertion = D3DXVECTOR3((float)(m_nDispertion - rand() % (m_nDispertion * 2)), 0.0f, (float)(m_nDispertion - rand() % (m_nDispertion * 2)));
					posReticle += dispertion;
				}

				// �ˏo���̈ʒu�̎擾
				mtxCanon = m_pModel[3]->GetMtxWorld();
				posCanon = D3DXVECTOR3(mtxCanon._41, mtxCanon._42, mtxCanon._43) + D3DXVECTOR3(sinf(rotCamera.y) * 30.0f, cosf(rotCamera.x) * 30.0f, cosf(rotCamera.y) * 30.0f);

				// ���������̊p�x�̌v�Z
				m_pAngle[nCntShoots * 2 + 1] = atan2f(posReticle.x - posCanon.x, posReticle.z - posCanon.z);
				m_pAngleV[nCntShoots * 2 + 1] = rotCamera.x;
				if (m_nDispertion != 0) { m_pAngleV[nCntShoots * 2 + 1] += (float)(m_nDispertion - (rand() % m_nDispertion * 2)) * 0.0005f; }

				// �e�̐���
				CBulletPlayer::Create(posCanon, m_pAngle[nCntShoots * 2 + 1], m_pAngleV[nCntShoots * 2 + 1], m_nAttack, m_nTeam);

				m_bShoot = true;
			}

			m_nRemBullet--;
		}
		else
		{
			m_bShoot = false;
		}
	}

	// �����[�h����
	Reload();

}


//=============================================================================
//	AI�X�V����
//=============================================================================
void CPlayer::AIUpdate(void)
{
	CDebugProc::Print("#====CPU=====#");
	//CDebugProc::Print("LIFE: %d", m_nLife);
	//CDebugProc::Print("CPU�ʒu�F%.2f %.2f %.2f", m_pos.x, m_pos.y, m_pos.z);
	//CDebugProc::Print("m_fRotDestUpper: %.2f, %.2f, %.2f", m_fRotDestUpper.x, m_fRotDestUpper.y, m_fRotDestUpper.z);

	for (int nCntSearch = 0; nCntSearch < MAX_SEARCH; nCntSearch++)
	{// �T���̍X�V����
		m_pSearch[nCntSearch]->Update();
	}


	Distance();
	if (m_bFind == true)
	{
		//�퓬���̈ړ�����
		BattleMovent();

		//�퓬�̏���
		Battle();
	}
	else
	{
		// �����ړ�����
		//CPlayer::AutoMove();
	}
	CDebugProc::Print("m_bFind: %d", m_bFind);

	Reload();

}

//=============================================================================
//	�����ړ�����
//=============================================================================
void CPlayer::AutoMove()
{

	CMotionManager::TYPE type = CMotionManager::TYPE_NEUTRAL;	// ���[�V�����̎��
	bool bMove = false;	// �{�^�������t���O
	float VigilanceRot[CONNECT_MAX];

	// �ڕW�n�_��ݒ�
	m_posDest = m_waypoint[m_nPoint];

	// �ړI�Ƃ̍������o��
	float fLength = (m_pos.x - m_posDest.x) * (m_pos.x - m_posDest.x) + (m_pos.z - m_posDest.z) * (m_pos.z - m_posDest.z);
	m_nBreaktime--;

	if (fLength > MOVE_ACCEPTABLE)
	{// ���������e�l���Ɏ��܂�܂ŖړI�n�Ɉړ�����
		bMove = true;
		m_move.x = sinf(atan2f(m_posDest.x - m_pos.x, m_posDest.z - m_pos.z)) * m_fSpeed;
		m_move.z = cosf(atan2f(m_posDest.x - m_pos.x, m_posDest.z - m_pos.z)) * m_fSpeed;
		//m_rot.y = atan2f(m_posDest.x - m_pos.x, m_posDest.z - m_pos.z) + D3DX_PI;
		m_fRotDestUpper.y = atan2f(m_posDest.x - m_pos.x, m_posDest.z - m_pos.z) /*+ D3DX_PI*/;
	}
	else if (m_nBreaktime < 0)
	{// �ړ���
		bMove = true;
		m_nBreaktime = PLAYER_BREAKTIME;

		if (m_nPoint == m_nCountPoint)
		{// �ŏI�ڕW�n�_�ɓ��������玟�̖ړI�n��T��
			if (m_nGoalCount != 0)
			{// �ŏ��ȊO�ɍŏI�n�_�p�̋x�e���Ԃ�^����
				m_nBreaktime = PLAYER_FINALPOINT_BREAKTIME;
			}
			m_nPoint = 0;		// ���݂̈ړ��񐔂̏�����
			m_nCountPoint = -1;	// �ړI�܂ł̈ړ��񐔂̏�����
			m_nGoalCount++;		// �S�[�������񐔂𑝂₷

			// �|�C���g����
			CPlayer::NodeSearch();
			// �|�C���g�ւ̌o�H�T��
			CPlayer::RootSearch();
		}
	}
	else if (m_nBreaktime == 0 && m_nPoint < m_nCountPoint)
	{// �x�e�I��
		bMove = true;
		m_nPoint++;
	}
	else if (m_nBreaktime > 0)
	{// �x�e��
		bMove = false;
		m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ��~����

		for (int nCntConnect = 0; nCntConnect < m_NodeData.connectNum[m_nStartNode]; nCntConnect++)
		{// �q�����Ă�m�[�h�̐��������
			VigilanceRot[nCntConnect] = atan2f(m_NodeData.pos[m_NodeData.connectIndex[m_nStartNode][nCntConnect]].x - m_pos.x, m_NodeData.pos[m_NodeData.connectIndex[m_nStartNode][nCntConnect]].z - m_pos.z) + D3DX_PI;
		}

		if (m_nBreaktime == PLAYER_FINALPOINT_BREAKTIME - 1 || m_nBreaktime % (PLAYER_FINALPOINT_BREAKTIME / m_NodeData.connectNum[m_nStartNode]) == 0)
		{// �q�����Ă�m�[�h�̕������ׂĂ�����
			//m_rot.y = VigilanceRot[m_nVigilanceCount];
			m_fRotDestUpper.y = VigilanceRot[m_nVigilanceCount];
			m_nVigilanceCount++;

			if (m_nVigilanceCount >= m_NodeData.connectNum[m_nStartNode])
			{// ����܂ōs�����珉����
				m_nVigilanceCount = 0;
			}
		}
	}

	if (m_pUpperMotion && m_pLowerMotion)
	{// ���[�V�����N���X���g���Ă���
		if (bMove)
		{// �ړ����Ă���
			// �ړ����[�V����
			m_pUpperMotion->SetMotion(CMotionManager::TYPE_WALK);
			m_pLowerMotion->SetMotion(CMotionManager::TYPE_WALK);
		}
		else
		{// �ړ����Ă��Ȃ�
			// �j���[�g�������[�V����
			m_pUpperMotion->SetMotion(CMotionManager::TYPE_NEUTRAL);
			m_pLowerMotion->SetMotion(CMotionManager::TYPE_NEUTRAL);
		}
	}

	m_move.x += (0 - m_move.x) * 0.4f;
	m_move.z += (0 - m_move.z) * 0.4f;

	// �ʒu�̍X�V
	m_pos.x += m_move.x;
	m_pos.z += m_move.z;
}

//=============================================================================
//	�m�[�h��������
//=============================================================================
void CPlayer::NodeSearch()
{
	float fMinLength = 100000, fLength = 100000;	// �����n
	int nNearEnemyNumber = 0;	// �ł��߂��G�̔ԍ�
	int nMovePoint = 0;

	// �ړ���̐ݒ�
	do
	{// �����n�_�������ꍇ�͂���1�x���߂�
		nMovePoint = m_nMovePoint[rand() % RANDOM_MOVE_POINT];
	} while (m_nNewEndNode == nMovePoint);
	m_nNewEndNode = nMovePoint;

	for (int nCntNode = 0; nCntNode < m_NodeData.nodeMax; nCntNode++)
	{// �m�[�h�̐��������
		if (m_NodeData.pos[nCntNode].x + POS_ACCEPTABLE > m_pos.x
			&& m_NodeData.pos[nCntNode].x - POS_ACCEPTABLE < m_pos.x
			&& m_NodeData.pos[nCntNode].z + POS_ACCEPTABLE > m_pos.z
			&& m_NodeData.pos[nCntNode].z - POS_ACCEPTABLE < m_pos.z)
		{// �v���C���[�̈ʒu�����e�͈͓�
			m_nStartNode = m_nEndNode;	// �O��̖ړI�n���J�n�n�_�Ƃ��ēo�^
		}
	}

	for (int nCntNode = 0; nCntNode < m_NodeData.nodeMax; nCntNode++)
	{// �m�[�h�̐��������
		if (m_NodeData.pos[nCntNode].x + POS_ACCEPTABLE > m_pos.x
			&& m_NodeData.pos[nCntNode].x - POS_ACCEPTABLE < m_pos.x
			&& m_NodeData.pos[nCntNode].z + POS_ACCEPTABLE > m_pos.z
			&& m_NodeData.pos[nCntNode].z - POS_ACCEPTABLE < m_pos.z)
		{// �v���C���[�̈ʒu���ړI�n�̋��e�͈͓�
			if (m_NodeData.pos[nCntNode] != m_NodeData.pos[m_nNewEndNode])
			{// �V�K�ړI�n���O��̖ړI�n�Ɠ�������Ȃ��ꍇ
				m_nEndNode = m_nNewEndNode;	// �ړI�n��o�^
			}
		}
	}
}

//=============================================================================
//	���[�g��������
//=============================================================================
void CPlayer::RootSearch()
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
			CPlayer::AddEdge(nCntNode, m_NodeData.connectIndex[nCntNode][nCntConnect], weight[nCntNode], node);
		}
	}

	//======= �ŒZ�o�H�𒲂ׂ� =========================================================================
	CPlayer::Dijkstra(m_NodeData.nodeMax, m_nStartNode, m_nEndNode, node);

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
		 // �ړI�܂ł̈ړ��񐔂��擾����
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
// �G�b�W�ǉ�����
//=============================================================================
void CPlayer::AddEdge(int first, int second, float weight, Node *node)
{
	// �m�[�hu�̓m�[�hv�ƂȂ����Ă����������
	node[second].to.push_back(first);
	// �m�[�hu�ƃm�[�hv�̃G�b�W�̏d�݂�����
	node[second].cost.push_back(weight);
}

//=============================================================================
// �_�C�N�X�g���@
//=============================================================================
void CPlayer::Dijkstra(int nodeMax, int start, int end, Node *node)
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
void CPlayer::FileLoad(char* pFileName)
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
	pFile = fopen(pFileName, "r");

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