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
#include "tutorial.h"
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
#include "damageDirection.h"
#include "nodeDataFiler.h"
#include "pin.h"
#include "tutorialmenu.h"

//==================================
// �}�N����`
//==================================
#define ASSULT_FILE				"data/TEXT/PLAYER/assult/model_assult.txt"
#define LIGHT_FILE				"data/TEXT/PLAYER/light/model_light.txt"
#define HEAVY_FILE				"data/TEXT/PLAYER/heavy/model_heavy.txt"
#define SNIPE_FILE				"data/TEXT/PLAYER/snipe/model_snipe.txt"

// �퓬�p
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
#define RADIOCHAT_MESS_SPEED	(10)		// ���W�I�`���b�g���b�Z�[�W�̑���
#define RADIOCHAT_DISPLAY_TIME	(60 * 2)	// ���b�Z�[�W�\������
#define RADIOCHAT_COL			(0.05f)		// �����x�̌��Z

// �s��
#define PIN_LIFE				(900)		// �s���̕\������

// =============================================================
// AI�֌W
// =============================================================
#define	LOAD_FILENAME		("data/TEXT/NODE_DATA/NodeData.txt")	// �}�b�v�f�[�^��ǂݍ��ރt�@�C���̖��O
//#define	LOAD_FILENAME	("data/TEXT/NODE_DATA/NodeDataTutorial.txt")	// �ǂݍ��ރt�@�C���̃p�X
#define MOVE_ACCEPTABLE		(50.0f)		// �ړ����̌덷�̋��e�͈�
#define POS_ACCEPTABLE		(30.0f)		// �������̌덷�̋��e�͈�
#define MOUSE_ACCEPTABLE	(20.0f)		// �}�E�X�̌덷�̋��e�͈�
#define DATA_REFERENCE_TIME	(5)			// �f�[�^�̎Q�Ƃ��s���Ԋu(��)
#define PLAYER_BREAKTIME	(1)			// �x�e����(�t���[��)
#define PLAYER_FINALPOINT_BREAKTIME	(180)	// �ŏI�n�_�̋x�e����(�t���[��)

#define MAX_CHAR (254)					//�ǂݎ�镶����
#define MAX_SEARCH (4)					//�Z���T�[��
#define FIND_FIND_CHARACTER_PRIORITY (4)//�T���v���C���[�̗D�揇��

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
	//m_pShadow = NULL;
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
	m_bReload = false;
	m_nRespawnTimer = 0;
	m_bAllyCol = false;
	m_nTexTimer = 0;
	m_nAllyTimer = 0;
	m_bOption = false;
	m_pUITexOption = NULL;
	m_pUIButtonBack = NULL;
	m_nSelectOption = 2;
	m_pUIButtonOption = NULL;
	m_nRadioChat = 0;
	m_nKillPlayerIdx = 0;
	m_fBulletSpeed = 0;
	m_nCntCPURespawn = 0;
	m_pUINumRespawn = NULL;
	m_bWince = false;

	// �s���֌W�̕ϐ�
	m_pPin = NULL;
	m_pPinBullet = NULL;
	m_pAllyPin = NULL;
	m_bPin = false;
	m_bAllyPin = false;
	m_nPinLife = 0;
	m_nAllyPinLife = 0;
	m_PinPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_AllyPinPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_AllyPinPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_AllyPosPinPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_pAllyPosPin = NULL;
	m_pPlayerIcon = NULL;

	for (int nCntAI = 0; nCntAI < AI_MAX; nCntAI++)
	{// AI�̐��������
	 // ������AI�̏�����
		m_pAIPin[nCntAI] = NULL;
		m_bAIPin[nCntAI] = false;
		m_nAIPinLife[nCntAI] = 0;
		m_AIPinPos[nCntAI] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		// ������AI�̏�����
		m_pAllyAIPin[nCntAI] = NULL;
		m_bAllyAIPin[nCntAI] = false;
		m_nAllyAIPinLife[nCntAI] = 0;
		m_AllyAIPinPos[nCntAI] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

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


	for (int nCnt = 0; nCnt < SERECTRESPAWN_TEX; nCnt++)
	{
		m_pUITexRespawn[nCnt] = NULL;
	}

	for (int nCnt = 0; nCnt < OPTION_SELECT; nCnt++)
	{	// �J�������x�ݒ�i�I�v�V�����j
		m_pUIButtonSelect[nCnt] = NULL;
	}

	for (int nCnt = 0; nCnt < AI_MAX; nCnt++)
	{
		m_pAI[nCnt] = NULL;
	}

	for (int nCnt = 0; nCnt < RELOAD_TEX; nCnt++)
	{	// �����[�h
		m_pUITexReload[nCnt] = NULL;
	}

	for (int nCnt = 0; nCnt < SELECTRESPAWN_BOTTON; nCnt++)
	{	// ���X�|�[���{�^��
		m_pUISelectResBotton[nCnt] = NULL;
		m_pUIRespawnPosIcon[nCnt] = NULL;
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
	else if (CMechaSelect::MECHATYPE_SNIPE == m_mecha) { pFileName = SNIPE_FILE; }

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
									m_pModel[nCntModelParts] = CModel::Create(this,&m_mtxWorld);
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
							else if (strcmp(aStr, "BULLETSPEED") == 0)
							{// �e��
								fscanf(pFile, " = %f", &m_fBulletSpeed);
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

	//if (NULL == m_pShadow)
	{// �e�̐���
		//m_pShadow = CShadow::Create(m_pos, PLAYER_PRIORITY - 1);
	}

	if (CManager::GetMode() == CManager::MODE_GAME)
	{
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
	}
	else if (CManager::GetMode() == CManager::MODE_TUTORIAL)
	{
		if (m_bConnect == true)
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

	m_nLife = m_nLifeMax;	// ���C�t�̏����l

	// ���W�I�`���b�g���b�Z�[�W�Ŏg�p
	m_moveSpeed = RADIOCHAT_MESS_SPEED;

	// �Q�[���J�n��
	if (CManager::GetMode() == CManager::MODE_GAME) { m_Respawn = RESPAWN_START; }
	else if (CManager::GetMode() == CManager::MODE_TUTORIAL) { m_Respawn = RESPAWN_NONE; }
	m_nDisTime = 1.8f;

	if (m_nPlayerIdx == 0 || m_nPlayerIdx == 1)
	{
		for (int nCntModel = 0; nCntModel < m_nNumParts; nCntModel++)
		{
			m_pModel[nCntModel]->SetColor(D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
		}
		m_nTeam = 0;

		// �J�����̌����̐ݒ�
		CManager::GetCamera()->SetRot(D3DXVECTOR3(D3DX_PI * 0.5f, D3DX_PI * -0.5f, 0.0f));
		if (CManager::GetMode() == CManager::MODE_TUTORIAL)
		{
			// �J�����̌����̐ݒ�
			CManager::GetCamera()->SetRot(D3DXVECTOR3(D3DX_PI * 0.5f, D3DX_PI * 0.5f, 0.0f));

		}
	}
	else if (m_nPlayerIdx == 2 || m_nPlayerIdx == 3)
	{
		for (int nCntModel = 0; nCntModel < m_nNumParts; nCntModel++)
		{
			m_pModel[nCntModel]->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
		}

		m_nTeam = 1;

		// �J�����̌����̐ݒ�
		CManager::GetCamera()->SetRot(D3DXVECTOR3(D3DX_PI * 0.5f, D3DX_PI * 0.5f, 0.0f));
	}

	// AI�̐���
	if (CManager::GetMode() == CManager::MODE_GAME)
	{
		if (m_pAI[0] == NULL)
		{// �h���[���^�C�v��AI�̐���
			m_pAI[0] = m_pAI[0]->Create(this, CAIMecha::MECHATYPE_DRONE, m_pos + D3DXVECTOR3(0.0f, 70.0f, 0.0f));
		}
		if (m_pAI[1] == NULL)
		{// �E�H�[�J�[�^�C�v��AI�̐���
			m_pAI[1] = m_pAI[1]->Create(this, CAIMecha::MECHATYPE_WORKER, m_pos);
		}
	}
	else if (CManager::GetMode() == CManager::MODE_TUTORIAL)
	{
		if (CTutorialMenu::GetMode() == CTutorialMenu::TYPE_STRATEGY)
		{
			if (m_bConnect == true)
			{
				if (m_pAI[0] == NULL)
				{// �h���[���^�C�v��AI�̐���
					m_pAI[0] = m_pAI[0]->Create(this, CAIMecha::MECHATYPE_DRONE, m_pos + D3DXVECTOR3(0.0f, 70.0f, 0.0f));
				}
			}
		}
	}

	// �s���̕\�����Ԃ̏�����
	m_nPinLife = PIN_LIFE;
	m_nAllyPinLife = PIN_LIFE;
	for (int nCnt = 0; nCnt < AI_MAX; nCnt++)
	{// ����AI�̐��������
		m_nAIPinLife[nCnt] = PIN_LIFE;
		m_nAllyAIPinLife[nCnt] = PIN_LIFE;
	}

	// �ړ��nAI�ϐ��̏�����
	m_pNodeData = CGame::GetNodeFiler();	// �t�@�C�����̎擾
	m_posDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nPoint = 0;
	m_nCountPoint = 0;
	m_nCountCollect = 0;
	m_nBreaktime = 0;
	m_nGoalCount = 0;
	m_nVigilanceCount = 0;
	m_bGoal = false;
	m_bPartSwitch = false;
	m_bCollectSwitch = false;

	for (int nCntCollect = 0; nCntCollect < COLLECTIONDATA_MAX; nCntCollect++)
	{// ���W�f�[�^�̍ő�l�����
		m_collectionPos[nCntCollect] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	for (int nCntNode = 0; nCntNode < NODEPOINT_MAX; nCntNode++)
	{// �m�[�h�̍ő�l�����
		m_waypoint[nCntNode] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	if (CManager::GetMode() == CManager::MODE_GAME)
	{
		if (!m_bConnect)
		{// �l�Ԃ��ڑ����Ă��Ȃ��ꍇ
			LoadBattleFile();
			m_fRotDestUpper = D3DXVECTOR3(D3DX_PI * 0.5f, D3DX_PI, 0.0f);

			m_pNodeData->FileLoad(LOAD_FILENAME);

			// �J�n���_�̃m�[�h�̏�����
			float fMinLength = 100000, fLength = 100000;	// �����n

			// �J�n���_�̃m�[�h�̏�����
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
			m_nEndNode = m_nMovePoint[rand() % RANDOM_MOVE_POINT];

			// �|�C���g�ւ̌o�H�T��
			CPlayer::RootSearch();
		}
	}

	if (m_pPlayerIcon == NULL)
	{
		m_pPlayerIcon = CScene3D::Create();
		m_pPlayerIcon->CScene3D::SetPos(D3DXVECTOR3(m_pos.x, m_pos.y + 500.0f, m_pos.z));
		m_pPlayerIcon->CScene3D::SetSize(D3DXVECTOR3(100.0f, 0.0f, 100.0f));
		if (m_nTeam == 0)
		{
			m_pPlayerIcon->BindTexture(CTexture::GetTexture(CTexture::TEXTURE_BLUE_DISP_PLAYER));

		}
		else
		{
			m_pPlayerIcon->BindTexture(CTexture::GetTexture(CTexture::TEXTURE_RED_DISP_PLAYER));

		}
	}

	return S_OK;
}

//=========================================
// �I������
//=========================================
void CPlayer::Uninit(void)
{
	if (m_pPin != NULL)
	{// �s���̔j��
		m_pPin->Uninit();
		m_pPin = NULL;
	}

	if (m_pPinBullet != NULL)
	{// �s�����Ēe�̔j��
		m_pPinBullet->Uninit();
		m_pPinBullet = NULL;
	}

	if (m_pAllyPin != NULL)
	{// �����s���̔j��
		m_pAllyPin->Uninit();
		m_pAllyPin = NULL;
	}

	if (m_pAllyPosPin != NULL)
	{// �����ʒu�s���̔j��
		m_pAllyPosPin->Uninit();
		m_pAllyPosPin = NULL;
	}

	for (int nCntAI = 0; nCntAI < AI_MAX; nCntAI++)
	{
		if (m_pAIPin[nCntAI] != NULL)
		{// AI�s���̔j��
			m_pAIPin[nCntAI]->Uninit();
			m_pAIPin[nCntAI] = NULL;
		}

		if (m_pAllyAIPin[nCntAI] != NULL)
		{// ����AI�s���̔j��
			m_pAllyAIPin[nCntAI]->Uninit();
			m_pAllyAIPin[nCntAI] = NULL;
		}
	}

	if (m_pReticle != NULL)
	{// ���e�B�N����NULL����Ȃ�
		m_pReticle->Uninit();
		m_pReticle = NULL;
	}

	//if (m_pShadow != NULL)
	//{// �e��NULL����Ȃ�
	//	m_pShadow->Uninit();
	//	m_pShadow = NULL;
	//}

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

	// �I�u�W�F�N�g�̔j��
	CScene::Release();
}

//=========================================
// �X�V����
//=========================================
void CPlayer::Update(void)
{
	if (CManager::GetMode() == CManager::MODE_GAME)
	{
		if (CManager::GetGame()->GetGameState() == CGame::STATE_STARTING_FADE_IN)
		{
			// �J�����̌����̐ݒ�
			CManager::GetCamera()->SetRot(D3DXVECTOR3(D3DX_PI * 0.5f, D3DX_PI * -1, 0.0f));
		}

		if (CManager::GetGame()->GetGameState() == CGame::STATE_NORMAL)
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
				}
				// �O��ʒu�̕ۑ�
				m_posOld = m_pos;

			}

			if (m_pUpperMotion->GetMotion() == CMotionManager::TYPE_NEUTRAL || m_pUpperMotion->GetMotion() == CMotionManager::TYPE_WALK)
			{
				m_bWince = false;
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

							if (m_bWince == false)
							{
								// �ړ��̏���
								Movement();
							}
							// �e������
							Shoot();

							// �s���֌W�̍X�V����
							CPlayer::PinUpdateMulti();

							if (m_bChat == false)
							{//�`���b�g���g�p���Ă��Ȃ��ꍇ
							 //���W�I�`���b�g�{�^���̐���
								ChatBotton();
							}
							if (m_bChat == true)
							{//�`���b�g���g�p���Ă���ꍇ
							 //�`���b�g�̃��b�Z�[�W�\������
								ChatMess(m_bChat);
							}
							if (m_bOption == true)
							{	// �I�v�V�����ݒ蒆
								Option(m_bOption);
							}

							if (m_bAllyChat == true)
							{//���Ԃ̃`���b�g���g�p����Ă���ꍇ
							 //���Ԃ̃��b�Z�[�W��\�����鏈��
								AllyChatMess();
							}

							D3DXVECTOR3 rotCamera = CManager::GetCamera()->GetRot();
							D3DXVECTOR3 posR = CManager::GetCamera()->GetPosR();

							m_pReticle->SetDisp(true);
							m_pReticle->SetPos(D3DXVECTOR3(sinf(rotCamera.y) * PLAYER_RETICLE_LENGTH, cosf(rotCamera.x) * PLAYER_RETICLE_LENGTH, cosf(rotCamera.y) * PLAYER_RETICLE_LENGTH) + D3DXVECTOR3(m_pos.x, 0.0f, m_pos.z));

							for (int nCntModel = 0; nCntModel < m_nNumParts; nCntModel++)
							{// �p�[�c���f���̍X�V
								m_pModel[nCntModel]->Update();
							}
							//CDebugProc::Print("�ʒu�F%.2f %.2f %.2f", m_pos.x, m_pos.y, m_pos.z);
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

						if (!m_bConnect)
						{// �R���s���[�^�����삷��ꍇ
							AIUpdate();
						}
					}
				}
			}
			else if (CMenu::GetMode() == CMenu::MODE_SINGLE)
			{
				// �O��ʒu�̕ۑ�
				m_posOld = m_pos;

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

							if (m_bWince == false)
							{
								// �ړ��̏���
								Movement();
							}

							// �e������
							Shoot();

							// �s���֌W�̍X�V����
							CPlayer::PinUpdateSingle();

							if (m_bChat == false)
							{	// ���W�I�`���b�g���Ă��Ȃ�
								ChatBotton();
							}

							if (m_bOption == true)
							{	// �I�v�V�����ݒ蒆
								Option(m_bOption);
							}

							if (m_bChat == true)
							{	// �`���b�g���Ă�
								ChatMess(m_bChat);
							}

							D3DXVECTOR3 rotCamera = CManager::GetCamera()->GetRot();
							D3DXVECTOR3 posR = CManager::GetCamera()->GetPosR();

							m_pReticle->SetDisp(true);
							m_pReticle->SetPos(D3DXVECTOR3(sinf(rotCamera.y) * PLAYER_RETICLE_LENGTH, cosf(rotCamera.x) * PLAYER_RETICLE_LENGTH, cosf(rotCamera.y) * PLAYER_RETICLE_LENGTH) + D3DXVECTOR3(m_pos.x, 0.0f, m_pos.z));

							for (int nCntModel = 0; nCntModel < m_nNumParts; nCntModel++)
							{// �p�[�c���f���̍X�V
								m_pModel[nCntModel]->Update();
							}
							//CDebugProc::Print("�ʒu�F%.2f %.2f %.2f", m_pos.x, m_pos.y, m_pos.z);
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
					}
				}
			}

			if (CManager::GetMode() == CManager::MODE_GAME)
			{
				if (!m_bConnect)
				{// �R���s���[�^�����삷��ꍇ
					if (CManager::GetGame()->GetPart() == CGame::PART_ACTION)
					{// �A�N�V�����p�[�g
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

							// AI�̍X�V����
							AIUpdate();
							Angle();
						}

						if (m_Respawn == RESPAWN_DEATH)
						{	// �s���s�\���
							Respawn(m_Respawn);		// ���X�|�[������
						}

						if (m_Respawn == RESPAWN_SELECT)
						{	// ���X�|�[���I�����
							SelectRespawn();				// ���X�|�[���ʒu�I��
						}
					}
				}
			}
			SetParticle();
		}
		else
		{
			if (m_pReticle != NULL)
			{
				m_pReticle->SetDisp(false);
			}
		}
		m_pPlayerIcon->CScene3D::SetPos(D3DXVECTOR3(m_pos.x, m_pos.y + 150.0f, m_pos.z));
	}
	else if (CManager::GetMode() == CManager::MODE_TUTORIAL)
	{
		// �O��ʒu�̕ۑ�
		m_posOld = m_pos;

		if (m_Respawn == RESPAWN_START)
		{	// �퓬�J�n ���
			Respawn(RESPAWN_START);
		}

		if (m_nPlayerIdx == 0)
		{
			m_pReticle->SetDisp(false);

			if (CManager::GetTutorial()->GetPart() == CGame::PART_ACTION)
			{	// �A�N�V�����p�[�g
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
					CCamera *pCamera = CManager::GetCamera();
					D3DXVECTOR3 CameraRot = pCamera->GetRot();
					D3DXMATRIX mtxHead = m_pModel[1]->GetMtxWorld();
					//CDebugProc::Print("���C�t�F%d", m_nLife);
					//CDebugProc::Print("�ʒu�F%.2f %.2f %.2f", m_pos.x, m_pos.y, m_pos.z);
					//CDebugProc::Print("�J�����p�x�F%.2f %.2f %.2f", CameraRot.x, CameraRot.y, CameraRot.z);
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
			}
		}
		else
		{
			CDebugProc::Print("���C�t�F%d", m_nLife);
		}
		SetParticle();
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

	if (CManager::GetMode() == CManager::MODE_GAME)
	{
		if (m_bConnect != true)
		{
			for (int nCntSearch = 0; nCntSearch < MAX_SEARCH; nCntSearch++)
			{// ���f���̕`�揈��
				m_pSearch[nCntSearch]->Draw();
			}
		}
	}
}

//=========================================
// �ړ��̏���
//=========================================
void CPlayer::Movement(void)
{
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();	// �L�[�{�[�h�̓��͂��擾
	CXInput *pXInput = CManager::GetXInput();					// XInput�̓��͂��擾
	bool bKey = false;				// �{�^�������t���O
	float fRotY = CManager::GetCamera()->GetRotY();
	float fDirMove;
	if (pKeyboard->GetTrigger(DIK_7))
	{
		Damage(m_nLifeMax, this);
		CParticle::Create(m_pModel[0]->GetWorldPos(), 4);
		CParticle::Create(m_pModel[0]->GetWorldPos(), 5);

		for (int nCntModel = 0; nCntModel < m_nNumParts; nCntModel++)
		{
			m_pModel[nCntModel]->SetDisp(false);
		}
	}

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

#ifdef _DEBUG

	// �_�b�V��
	if (pKeyboard->GetPress(DIK_SPACE))
	{
		m_move += m_move * 1.5f;
	}

#endif // DEBUG

	if (NULL != m_pUpperMotion && NULL != m_pLowerMotion)
	{// ���[�V�����N���X���g���Ă���
		if (m_pUpperMotion->GetMotion() != CMotionManager::TYPE_DAMAGE_FRONT && m_pUpperMotion->GetMotion() != CMotionManager::TYPE_DAMAGE_BACK)
		{
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
			else if ((fDiffX < 10.0f || fDiffX > -10.0f) && !m_bChatBotton)
			{// �ړ����[�V����
				m_pUpperMotion->SetMotion(CMotionManager::TYPE_WALK);
				m_pLowerMotion->SetMotion(CMotionManager::TYPE_WALK);
			}
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
	CSound *pSound = CManager::GetSound();				// �T�E���h�̃|�C���^

	if (pMouse->GetPress(CInputMouse::DIMS_BUTTON_0) && m_nRemBullet > 0 && m_bChatBotton == false && m_bReload == false && m_bOption == false)
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
				CBulletPlayer::Create(posCanon, m_pAngle[nCntShoots * 2], m_pAngleV[nCntShoots * 2], m_nAttack, m_nTeam, this,m_fBulletSpeed);
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
				CBulletPlayer::Create(posCanon, m_pAngle[nCntShoots * 2 + 1], m_pAngleV[nCntShoots * 2 + 1], m_nAttack, m_nTeam, this,m_fBulletSpeed);
				CParticle::Create(posCanon, 2);

				switch (m_mecha)
				{
				case CMechaSelect::MECHATYPE_LIGHT:
					pSound->PlaySound(CSound::SOUND_LABEL_MACHINEGUN);
					break;
				case CMechaSelect::MECHATYPE_ASSULT:
					pSound->PlaySound(CSound::SOUND_LABEL_SHOTGUN);
					break;
				case CMechaSelect::MECHATYPE_HEAVY:
					pSound->PlaySound(CSound::SOUND_LABEL_RIFLE);
					break;
				case CMechaSelect::MECHATYPE_SNIPE:
					pSound->PlaySound(CSound::SOUND_LABEL_SNIPE);
					break;
				}

				m_bShoot = true;
			}

			//float rot = D3DX_PI;

			//// �����̒���
			//if (rot > D3DX_PI) { rot -= D3DX_PI * 2.0f; }
			//if (rot < -D3DX_PI) { rot += D3DX_PI * 2.0f; }

			//	D3DXVECTOR3 pos = D3DXVECTOR3(sinf(rot) * 100.0f, 25.0f, cosf(rot) * 100.0f) + m_pos;
			//	CBulletPlayer::Create(pos, atan2f(m_pos.x - pos.x, m_pos.z - pos.z), D3DX_PI * 0.5f, m_nAttack, (m_nTeam + 1) % 2);
			//	m_bShoot = true;

			m_nRemBullet--;
			m_bShootButton = true;
		}
	}
	else
	{
		// ���˃{�^�������t���O�𕉂ɂ���
		m_bShootButton = false;
	}

	if (m_nRemBullet <= 0)
	{	// �e���Ȃ��Ȃ���
		m_bReload = true;
	}

	if (pMouse->GetTrigger(CInputMouse::DIMS_BUTTON_1))
	{
		if (m_nRemBullet == m_nCapacity)
		{	// �e�������l�Ɠ����Ȃ�A�����[�h���Ȃ�
			m_bReload = false;
		}
		else
		{	// �����Ă��鎞
			m_bReload = true;
		}
	}

	if (m_bConnect)
	{// �����[�h����
		Reload(m_bReload);
	}
}

//=========================================
// �p�x�X�V
//=========================================
void CPlayer::Angle(void)
{
	// �J�����̊p�x�����߂�
	D3DXVECTOR3 rotCamera = CManager::GetCamera()->GetRot();
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

	//D3DXVECTOR3 rot = m_pModel[1]->GetRot();

	// ����O�ɂȂ���
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


	if (!m_bConnect)
	{
		// �p�x�����߂�
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
	if (m_bDeath) { return; }

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
void CPlayer::Damage(int nDamage, CScene *pScene)
{
	if (CMenu::GetMode() == CMenu::MODE_SINGLE)
	{//�V���O���v���C�̏ꍇ
		if (NULL != m_pUpperMotion && NULL != m_pLowerMotion)
		{// ���[�V�����N���X���g���Ă���
			if (m_nLife > 0 && m_bDeath == false)
			{//�̗͂��O���傫�������S���Ă��Ȃ��ꍇ

				if (pScene->GetObjType() == CScene::OBJTYPE_PLAYER)
				{//�I�u�W�F�N�g�̎�ނ��v���C���[�̏ꍇ
					CPlayer *pPlayer = (CPlayer*)pScene;
					if (pPlayer->GetMechaType() == CMechaSelect::MECHATYPE_HEAVY || pPlayer->GetMechaType() == CMechaSelect::MECHATYPE_SNIPE)
					{
						if (CManager::GetGame()->GetDamageDirection()->GetDamageDirection(CDamageDirection::DIRECTION_FRONT))
						{
							m_pUpperMotion->SetMotion(CMotionManager::TYPE_DAMAGE_FRONT);	// �_���[�W���[�V�������Đ�
							m_pLowerMotion->SetMotion(CMotionManager::TYPE_DAMAGE_FRONT);	// �_���[�W���[�V�������Đ�
							m_bWince = true;
						}
						else if (CManager::GetGame()->GetDamageDirection()->GetDamageDirection(CDamageDirection::DIRECTION_BACK) ||
							CManager::GetGame()->GetDamageDirection()->GetDamageDirection(CDamageDirection::DIRECTION_RIGHT) ||
							CManager::GetGame()->GetDamageDirection()->GetDamageDirection(CDamageDirection::DIRECTION_LEFT))
						{
							m_pUpperMotion->SetMotion(CMotionManager::TYPE_DAMAGE_BACK);	// �_���[�W���[�V�������Đ�
							m_pLowerMotion->SetMotion(CMotionManager::TYPE_DAMAGE_BACK);	// �_���[�W���[�V�������Đ�
							m_bWince = true;
						}
					}
				}

				m_state = STATE_DAMAGE;								// �_���[�W���󂯂Ă����Ԃɂ���

				m_nLife -= nDamage;									//�̗͂̌��Z

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
									{//NULL�ł͂Ȃ��ꍇ
										m_nKillPlayerIdx = pPlayer->GetPlayerIdx();								//�L���v���C���[�̔ԍ���ݒu����
										CManager::GetGame()->SetKillIdx(nCntKill, pPlayer->GetPlayerIdx());		//�L���v���C���[�̔ԍ���ݒu����
										CManager::GetGame()->SetDeathIdx(nCntKill, m_nPlayerIdx);				//�f�X�v���C���[�̔ԍ���ݒu����
										CManager::GetGame()->SetPlayerType(0, CGame::TYPE_PLAYER);				//�v���C���[�̎�ނ�ݒu����
										CManager::GetGame()->SetPlayerType(1, CGame::TYPE_PLAYER);				//�v���C���[�̎�ނ�ݒu����
										CManager::GetGame()->SetLog(nCntKill, true);							//���O�̐ݒu����
									}
								}
								else if (pScene->GetObjType() == CScene::OBJTYPE_AI)
								{//�I�u�W�F�N�g�̎�ނ��`�h�̏ꍇ
									CAIMecha *pAIMecha = (CAIMecha*)pScene;
									if (pAIMecha != NULL)
									{//NULL�̏ꍇ
										m_nKillPlayerIdx = pAIMecha->GetPlayer()->GetPlayerIdx();							//�L���v���C���[�̔ԍ���ݒu����
										CManager::GetGame()->SetKillIdx(nCntKill, pAIMecha->GetPlayer()->GetPlayerIdx());	//�L���v���C���[�̔ԍ���ݒu����
										CManager::GetGame()->SetDeathIdx(nCntKill, m_nPlayerIdx);							//�f�X�v���C���[�̔ԍ���ݒu����
										if (pAIMecha->GetMechaType() == CAIMecha::MECHATYPE_DRONE)
										{//�I�u�W�F�N�g�̎�ނ��h���[���̏ꍇ
											CManager::GetGame()->SetPlayerType(0, CGame::TYPE_DROWN);						//�v���C���[�̎�ނ�ݒu����
											CManager::GetGame()->SetPlayerType(1, CGame::TYPE_PLAYER);						//�v���C���[�̎�ނ�ݒu����
											CManager::GetGame()->SetLog(nCntKill, true);									//���O�̐ݒu����
										}
										else if (pAIMecha->GetMechaType() == CAIMecha::MECHATYPE_WORKER)
										{//�I�u�W�F�N�g�̎�ނ����[�J�[�̏ꍇ
											CManager::GetGame()->SetPlayerType(0, CGame::TYPE_WORKER);						//�v���C���[�̎�ނ�ݒu����
											CManager::GetGame()->SetPlayerType(1, CGame::TYPE_PLAYER);						//�v���C���[�̎�ނ�ݒu����
											CManager::GetGame()->SetLog(nCntKill, true);									//���O�̐ݒu����
										}
									}
								}
								break;
							}
						}

						for (int nCntAI = 0; nCntAI < AI_MAX; nCntAI++)
						{
							//AI�����S��Ԃɂ���
							m_pAI[nCntAI]->SetDeath(true);

							//�p�[�e�B�N���𐶐�
							CParticle::Create(m_pAI[nCntAI]->GetModel(0)->GetWorldPos(), 4);
							CParticle::Create(m_pAI[nCntAI]->GetModel(0)->GetWorldPos(), 5);

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
						CManager::GetGame()->SetBlueLinkEnergy(CManager::GetGame()->GetBlueLinkEnergy() - 30);
						break;
					case 1:
						CManager::GetGame()->SetRedLinkEnergy(CManager::GetGame()->GetRedLinkEnergy() - 30);
						break;
					}
				}
			}
		}
	}
	else
	{//�}���`�v���C�̏ꍇ
		if (CManager::GetClient() != NULL)
		{//NULL�ł͂Ȃ��ꍇ
			if (CManager::GetClient()->GetPlayerIdx() == m_nPlayerIdx)
			{//�v���C���[�ԍ��ƃN���C�A���g�ԍ��������ꍇ
				if (NULL != m_pUpperMotion && NULL != m_pLowerMotion)
				{// ���[�V�����N���X���g���Ă���
					if (m_nLife > 0 && m_bDeath == false)
					{//�̗͂��O���傫�������S���Ă��Ȃ��ꍇ
						if (pScene->GetObjType() == CScene::OBJTYPE_PLAYER)
						{//�I�u�W�F�N�g�̎�ނ��v���C���[�̏ꍇ
							CPlayer *pPlayer = (CPlayer*)pScene;
							if (pPlayer->GetMechaType() == CMechaSelect::MECHATYPE_HEAVY || pPlayer->GetMechaType() == CMechaSelect::MECHATYPE_SNIPE)
							{
								if (CManager::GetGame()->GetDamageDirection()->GetDamageDirection(CDamageDirection::DIRECTION_FRONT))
								{
									m_pUpperMotion->SetMotion(CMotionManager::TYPE_DAMAGE_FRONT);	// �_���[�W���[�V�������Đ�
									m_pLowerMotion->SetMotion(CMotionManager::TYPE_DAMAGE_FRONT);	// �_���[�W���[�V�������Đ�
									m_bWince = true;
								}
								else if (CManager::GetGame()->GetDamageDirection()->GetDamageDirection(CDamageDirection::DIRECTION_BACK) ||
									CManager::GetGame()->GetDamageDirection()->GetDamageDirection(CDamageDirection::DIRECTION_RIGHT) ||
									CManager::GetGame()->GetDamageDirection()->GetDamageDirection(CDamageDirection::DIRECTION_LEFT))
								{
									m_pUpperMotion->SetMotion(CMotionManager::TYPE_DAMAGE_BACK);	// �_���[�W���[�V�������Đ�
									m_pLowerMotion->SetMotion(CMotionManager::TYPE_DAMAGE_BACK);	// �_���[�W���[�V�������Đ�
									m_bWince = true;
								}
							}
						}

						m_state = STATE_DAMAGE;				// �_���[�W���󂯂Ă����Ԃɂ���

						m_nLife -= nDamage;					// �̗͂̌��Z

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
											{//NULL�ł͂Ȃ��ꍇ
												m_nKillPlayerIdx = pPlayer->GetPlayerIdx();								//�L���v���C���[�̔ԍ���ݒu����
												CManager::GetGame()->SetKillIdx(nCntKill, pPlayer->GetPlayerIdx());		//�L���v���C���[�̔ԍ���ݒu����
												CManager::GetGame()->SetDeathIdx(nCntKill, m_nPlayerIdx);				//�f�X�v���C���[�̔ԍ���ݒu����
												CManager::GetGame()->SetPlayerType(0, CGame::TYPE_PLAYER);				//�v���C���[�̎�ނ�ݒu����
												CManager::GetGame()->SetPlayerType(1, CGame::TYPE_PLAYER);				//�v���C���[�̎�ނ�ݒu����
												CManager::GetGame()->SetLog(nCntKill, true);							//���O�̐ݒu����
											}
										}
										else if (pScene->GetObjType() == CScene::OBJTYPE_AI)
										{//�I�u�W�F�N�g�̎�ނ�AI�̏ꍇ
											CAIMecha *pAIMecha = (CAIMecha*)pScene;
											if (pAIMecha != NULL)
											{//NULL�ł͂Ȃ��ꍇ
												m_nKillPlayerIdx = pAIMecha->GetPlayer()->GetPlayerIdx();							//�L���v���C���[�̔ԍ���ݒu����
												CManager::GetGame()->SetKillIdx(nCntKill, pAIMecha->GetPlayer()->GetPlayerIdx());	//�L���v���C���[�̔ԍ���ݒu����
												CManager::GetGame()->SetDeathIdx(nCntKill, m_nPlayerIdx);							//�f�X�v���C���[�̔ԍ���ݒu����
												if (pAIMecha->GetMechaType() == CAIMecha::MECHATYPE_DRONE)
												{//�I�u�W�F�N�g�̎�ނ��h���[���̏ꍇ
													CManager::GetGame()->SetPlayerType(0, CGame::TYPE_DROWN);						//�v���C���[�̎�ނ�ݒu����
													CManager::GetGame()->SetPlayerType(1, CGame::TYPE_PLAYER);						//�v���C���[�̎�ނ�ݒu����
													CManager::GetGame()->SetLog(nCntKill, true);									//���O�̐ݒu����
												}
												else if (pAIMecha->GetMechaType() == CAIMecha::MECHATYPE_WORKER)
												{//�I�u�W�F�N�g�̎�ނ����[�J�[�̏ꍇ
													CManager::GetGame()->SetPlayerType(0, CGame::TYPE_WORKER);						//�v���C���[�̎�ނ�ݒu����
													CManager::GetGame()->SetPlayerType(1, CGame::TYPE_PLAYER);						//�v���C���[�̎�ނ�ݒu����
													CManager::GetGame()->SetLog(nCntKill, true);									//���O�̐ݒu����
												}
											}
										}
										break;
									}
								}
							}

							for (int nCntAI = 0; nCntAI < AI_MAX; nCntAI++)
							{
								for (int nCntParts = 0; nCntParts < m_pAI[nCntAI]->GetNumParts(); nCntParts++)
								{
									//AI��\�������Ȃ�
									m_pAI[nCntAI]->GetModel(nCntParts)->SetDisp(false);
								}

								//�p�[�e�B�N���𐶐�
								CParticle::Create(m_pAI[nCntAI]->GetModel(0)->GetWorldPos(), 4);
								CParticle::Create(m_pAI[nCntAI]->GetModel(0)->GetWorldPos(), 5);

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
								CManager::GetGame()->SetBlueLinkEnergy(CManager::GetGame()->GetBlueLinkEnergy() - 30);
								break;
							case 1:
								CManager::GetGame()->SetRedLinkEnergy(CManager::GetGame()->GetRedLinkEnergy() - 30);
								break;
							}
						}
					}
				}
			}
			if (CManager::GetClient()->GetPlayerIdx() == 0 && m_bConnect == false)
			{//CPU�̏ꍇ
				if (NULL != m_pUpperMotion && NULL != m_pLowerMotion)
				{// ���[�V�����N���X���g���Ă���
					if (m_nLife > 0 && m_bDeath == false)
					{//�̗͂��O���傫�������S���Ă��Ȃ��ꍇ
						if (pScene->GetObjType() == CScene::OBJTYPE_PLAYER)
						{//�I�u�W�F�N�g�̎�ނ��v���C���[�̏ꍇ
							CPlayer *pPlayer = (CPlayer*)pScene;
							if (pPlayer->GetMechaType() == CMechaSelect::MECHATYPE_HEAVY || pPlayer->GetMechaType() == CMechaSelect::MECHATYPE_SNIPE)
							{
								if (CManager::GetGame()->GetDamageDirection()->GetDamageDirection(CDamageDirection::DIRECTION_FRONT))
								{
									m_pUpperMotion->SetMotion(CMotionManager::TYPE_DAMAGE_FRONT);	// �_���[�W���[�V�������Đ�
									m_pLowerMotion->SetMotion(CMotionManager::TYPE_DAMAGE_FRONT);	// �_���[�W���[�V�������Đ�
									m_bWince = true;
								}
								else if (CManager::GetGame()->GetDamageDirection()->GetDamageDirection(CDamageDirection::DIRECTION_BACK) ||
									CManager::GetGame()->GetDamageDirection()->GetDamageDirection(CDamageDirection::DIRECTION_RIGHT) ||
									CManager::GetGame()->GetDamageDirection()->GetDamageDirection(CDamageDirection::DIRECTION_LEFT))
								{
									m_pUpperMotion->SetMotion(CMotionManager::TYPE_DAMAGE_BACK);	// �_���[�W���[�V�������Đ�
									m_pLowerMotion->SetMotion(CMotionManager::TYPE_DAMAGE_BACK);	// �_���[�W���[�V�������Đ�
									m_bWince = true;
								}
							}
						}

						m_state = STATE_DAMAGE;				// �_���[�W���󂯂Ă����Ԃɂ���

						m_nLife -= nDamage;					// �̗͂̌��Z

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
											{//NULL�ł͂Ȃ��ꍇ
												m_nKillPlayerIdx = pPlayer->GetPlayerIdx();								//�L���v���C���[�̔ԍ���ݒu����
												CManager::GetGame()->SetKillIdx(nCntKill, pPlayer->GetPlayerIdx());		//�L���v���C���[�̔ԍ���ݒu����
												CManager::GetGame()->SetDeathIdx(nCntKill, m_nPlayerIdx);				//�f�X�v���C���[�̔ԍ���ݒu����
												CManager::GetGame()->SetPlayerType(0, CGame::TYPE_PLAYER);				//�v���C���[�̎�ނ�ݒu����
												CManager::GetGame()->SetPlayerType(1, CGame::TYPE_PLAYER);				//�v���C���[�̎�ނ�ݒu����
												CManager::GetGame()->SetLog(nCntKill, true);							//���O�̐ݒu����
											}
										}
										else if (pScene->GetObjType() == CScene::OBJTYPE_AI)
										{//�I�u�W�F�N�g�̎�ނ�AI�̏ꍇ
											CAIMecha *pAIMecha = (CAIMecha*)pScene;
											if (pAIMecha != NULL)
											{//NULL�ł͂Ȃ��ꍇ
												m_nKillPlayerIdx = pAIMecha->GetPlayer()->GetPlayerIdx();							//�L���v���C���[�̔ԍ���ݒu����
												CManager::GetGame()->SetKillIdx(nCntKill, pAIMecha->GetPlayer()->GetPlayerIdx());	//�L���v���C���[�̔ԍ���ݒu����
												CManager::GetGame()->SetDeathIdx(nCntKill, m_nPlayerIdx);							//�f�X�v���C���[�̔ԍ���ݒu����
												if (pAIMecha->GetMechaType() == CAIMecha::MECHATYPE_DRONE)
												{//�I�u�W�F�N�g�̎�ނ��h���[���̏ꍇ
													CManager::GetGame()->SetPlayerType(0, CGame::TYPE_DROWN);						//�v���C���[�̎�ނ�ݒu����
													CManager::GetGame()->SetPlayerType(1, CGame::TYPE_PLAYER);						//�v���C���[�̎�ނ�ݒu����
													CManager::GetGame()->SetLog(nCntKill, true);									//���O�̐ݒu����
												}
												else if (pAIMecha->GetMechaType() == CAIMecha::MECHATYPE_WORKER)
												{//�I�u�W�F�N�g�̎�ނ����[�J�[�̏ꍇ
													CManager::GetGame()->SetPlayerType(0, CGame::TYPE_WORKER);						//�v���C���[�̎�ނ�ݒu����
													CManager::GetGame()->SetPlayerType(1, CGame::TYPE_PLAYER);						//�v���C���[�̎�ނ�ݒu����
													CManager::GetGame()->SetLog(nCntKill, true);									//���O�̐ݒu����
												}
											}
										}
										break;
									}
								}
							}

							for (int nCntAI = 0; nCntAI < AI_MAX; nCntAI++)
							{
								for (int nCntParts = 0; nCntParts < m_pAI[nCntAI]->GetNumParts(); nCntParts++)
								{
									//AI��\�������Ȃ�
									m_pAI[nCntAI]->GetModel(nCntParts)->SetDisp(false);
								}

								//�p�[�e�B�N���𐶐�
								CParticle::Create(m_pAI[nCntAI]->GetModel(0)->GetWorldPos(), 4);
								CParticle::Create(m_pAI[nCntAI]->GetModel(0)->GetWorldPos(), 5);

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
								CManager::GetGame()->SetBlueLinkEnergy(CManager::GetGame()->GetBlueLinkEnergy() - 30);
								break;
							case 1:
								CManager::GetGame()->SetRedLinkEnergy(CManager::GetGame()->GetRedLinkEnergy() - 30);
								break;
							}
						}
					}
				}
			}
		}
	}

	if (CMenu::GetMode() == CMenu::MODE_TUTORIAL)
	{//�V���O���v���C�̏ꍇ
		if (NULL != m_pUpperMotion && NULL != m_pLowerMotion)
		{// ���[�V�����N���X���g���Ă���
			if (m_nLife > 0 && m_bDeath == false)
			{//�̗͂��O���傫�������S���Ă��Ȃ��ꍇ

				m_state = STATE_DAMAGE;								// �_���[�W���󂯂Ă����Ԃɂ���

				m_nLife -= nDamage;									//�̗͂̌��Z

				if (0 >= m_nLife)
				{//�̗͂��O�ȉ��̏ꍇ
					m_nLife = 0;		//�̗͂��O�ɂ���
					m_bDeath = true;	//���S��Ԃɂ���

					if (m_bDeath == true && CManager::GetMode() == CManager::MODE_TUTORIAL)
					{//���S���Ă���ꍇ
						CSound *pSound = CManager::GetSound();
						pSound->PlaySound(CSound::SOUND_LABEL_EXPLOSION);

						//�p�[�e�B�N���𐶐�
						CParticle::Create(m_pModel[0]->GetWorldPos(), 4);
						CParticle::Create(m_pModel[0]->GetWorldPos(), 5);

						for (int nCntModel = 0; nCntModel < m_nNumParts; nCntModel++)
						{//�\�����Ȃ�
							m_pModel[nCntModel]->SetDisp(false);
						}

					}
				}
			}
		}
	}

}

//=========================================
// �Ђ�ݏ�Ԃ̎擾
//=========================================
bool CPlayer::GetWince(void)
{
	if (m_pUpperMotion->GetMotion() != CMotionManager::TYPE_DAMAGE_FRONT && m_pUpperMotion->GetMotion() != CMotionManager::TYPE_DAMAGE_BACK)
	{
		return false;
	}

	return true;
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
void CPlayer::Reload(bool bReload)
{
	if(bReload == true)
	{
		m_nCntReRoad++;		// �J�E���^�[���Z

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

		if (m_nCntReRoad % m_nReload == 0)
		{	// �ݒ肵���b���と���S���Q�[�W�j�����e����
			m_nRemBullet = m_nCapacity;			// �e����

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

			m_bReload = false;
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
		if (m_bConnect == true)
		{
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
		}
		else
		{
			m_nCntCPURespawn++;
			if (m_nCntCPURespawn % 300 == 0)
			{
				respawn = RESPAWN_SELECT;		// ���X�|�[���I����Ԃɐݒ�
				m_nCntCPURespawn = 0;
			}
		}
		break;
	}

	m_Respawn = respawn;

	//CDebugProc::Print("m_nDisTime : %d\n", m_nDisTime);
	//CDebugProc::Print("m_nRespawnTimer : %d\n", m_nRespawnTimer);
}

//=========================================
// ���X�|�[���I������
//=========================================
void CPlayer::SelectRespawn(void)
{
	if (m_bConnect == true)
	{
		if (m_pUITexSelectRes[0] == NULL || m_pUITexSelectRes[1] == NULL || m_pUITexSelectRes[2] == NULL || m_pUITexSelectRes[3] == NULL || m_pCursor == NULL)
		{	// UI�̐���
			m_pUITexSelectRes[0] = CUI_TEXTURE::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), SCREEN_WIDTH, SCREEN_HEIGHT, CUI_TEXTURE::UIFLAME_NONE);	// ���n
			m_pUITexSelectRes[1] = CUI_TEXTURE::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, 360.0f, 0.0f), 650.0f, 370.0f, CUI_TEXTURE::UIFLAME_MAP);	// �}�b�v
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

		//���X�|�[���ʒu�̃A�C�R���̐���
		CreateRespawnPosIcon();

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

			for (int nCntButton = 0; nCntButton < SELECTRESPAWN_BOTTON; nCntButton++)
			{
				if (m_pUIRespawnPosIcon[nCntButton] != NULL)
				{
					if (nSelect == nCntButton)
					{
						m_pUIRespawnPosIcon[nCntButton]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
					}
					else
					{
						m_pUIRespawnPosIcon[nCntButton]->SetColor(D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f));
					}
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

				m_pos = CManager::GetGame()->GetRespawnPos(m_nTeam, m_point);

				if (m_bConnect == false)
				{
					// �J�n���_�̃m�[�h�̏�����
					float fMinLength = 100000, fLength = 100000;	// �����n

					// �J�n���_�̃m�[�h�̏�����
					for (int nCntNode = 0; nCntNode < m_pNodeData->GetLoadData().nodeMax; nCntNode++)
					{// �m�[�h�̐��������
					 // ���������߂�
						fLength = (m_pNodeData->GetLoadData().pos[nCntNode].x - m_pos.x) *
							(m_pNodeData->GetLoadData().pos[nCntNode].x - m_pos.x) +
							(m_pNodeData->GetLoadData().pos[nCntNode].z - m_pos.z) *
							(m_pNodeData->GetLoadData().pos[nCntNode].z - m_pos.z);

						if (fMinLength > fLength)
						{// �����̍ŏ��l�����߂�
							fMinLength = fLength;
							m_nStartNode = nCntNode;
							m_nEndNode = nCntNode;
						}
					}

					// �o�H�T��
					CPlayer::RootSearch();
				}

				// �ʏ��Ԃɖ߂�
				m_Respawn = RESPAWN_NONE;

				// ���C�t�̐ݒ�
				m_nLife = m_nLifeMax;

				//���S���Ă��Ȃ��悤�ɂ���
				m_bDeath = false;

				//AI�̏���������
				m_pAI[0]->SetPos(m_pos + D3DXVECTOR3(0.0f, 70.0f, 0.0f));
				m_pAI[1]->SetPos(m_pos);

				for (int nCntAI = 0; nCntAI < AI_MAX; nCntAI++)
				{
					m_pAI[nCntAI]->SetLife(m_pAI[nCntAI]->GetLifeMax());
					m_pAI[nCntAI]->SetDeath(false);
					// �`�悷��
					for (int nCntParts = 0; nCntParts < m_pAI[nCntAI]->GetNumParts(); nCntParts++)
					{
						m_pAI[nCntAI]->GetModel(nCntParts)->SetDisp(true);
					}
				}
				// �`�悷��
				for (int nCntParts = 0; nCntParts < m_nNumParts; nCntParts++)
				{
					m_pModel[nCntParts]->SetDisp(true);
				}

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

				for (int nCnt = 0; nCnt < SELECTRESPAWN_BOTTON; nCnt++)
				{
					if (m_pUIRespawnPosIcon[nCnt] != NULL)
					{	// �e�N�X�`���̔j��
						m_pUIRespawnPosIcon[nCnt]->Uninit();
						m_pUIRespawnPosIcon[nCnt] = NULL;
					}
				}

				if (m_pCursor != NULL)
				{	// �J�[�\���̔j��
					m_pCursor->Uninit();
					m_pCursor = NULL;
				}
			}	// �{�^�������ꂽ��
		}	// �������ꂽ
	}
	else
	{

		m_point = (POINT)(rand() % POINT_MAX);

		m_pos = CManager::GetGame()->GetRespawnPos(m_nTeam, m_point);

		if (m_bConnect == false)
		{
			// �J�n���_�̃m�[�h�̏�����
			float fMinLength = 100000, fLength = 100000;	// �����n

															// �J�n���_�̃m�[�h�̏�����
			for (int nCntNode = 0; nCntNode < m_pNodeData->GetLoadData().nodeMax; nCntNode++)
			{// �m�[�h�̐��������
			 // ���������߂�
				fLength = (m_pNodeData->GetLoadData().pos[nCntNode].x - m_pos.x) *
					(m_pNodeData->GetLoadData().pos[nCntNode].x - m_pos.x) +
					(m_pNodeData->GetLoadData().pos[nCntNode].z - m_pos.z) *
					(m_pNodeData->GetLoadData().pos[nCntNode].z - m_pos.z);

				if (fMinLength > fLength)
				{// �����̍ŏ��l�����߂�
					fMinLength = fLength;
					m_nStartNode = nCntNode;
					m_nEndNode = nCntNode;
				}
			}

			// �o�H�T��
			CPlayer::RootSearch();
		}

		// �ʏ��Ԃɖ߂�
		m_Respawn = RESPAWN_NONE;

		// ���C�t�̐ݒ�
		m_nLife = m_nLifeMax;

		//���S���Ă��Ȃ��悤�ɂ���
		m_bDeath = false;

		//AI�̏���������
		m_pAI[0]->SetPos(m_pos + D3DXVECTOR3(0.0f, 70.0f, 0.0f));
		m_pAI[1]->SetPos(m_pos);

		for (int nCntAI = 0; nCntAI < AI_MAX; nCntAI++)
		{
			m_pAI[nCntAI]->SetLife(m_pAI[nCntAI]->GetLifeMax());
			m_pAI[nCntAI]->SetDeath(false);
			// �`�悷��
			for (int nCntParts = 0; nCntParts < m_pAI[nCntAI]->GetNumParts(); nCntParts++)
			{
				m_pAI[nCntAI]->GetModel(nCntParts)->SetDisp(true);
			}
		}
		// �`�悷��
		for (int nCntParts = 0; nCntParts < m_nNumParts; nCntParts++)
		{
			m_pModel[nCntParts]->SetDisp(true);
		}
	}
	//CDebugProc::Print("���X�|�[���I��");
}
//=============================================================================
//	�`���b�g�{�^���̐���
//=============================================================================
void CPlayer::CreateRadioChatButton(void)
{
	if (m_pUIRadioBotton[0] == NULL && m_pUIRadioBotton[1] == NULL && m_pUIRadioBotton[2] == NULL && m_pUIRadioBotton[3] == NULL
		&& m_pUIRadioBotton[4] == NULL && m_pUIRadioBotton[5] == NULL && m_pUIRadioBotton[6] == NULL && m_pUIRadioBotton[7] == NULL && m_pCursor == NULL)
	{	// �{�^���ƃJ�[�\���̐���
		m_pUIRadioBotton[0] = CButton2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 + 100.0f, 180.0f, 0.0f), RADIOCHAT_BOTTON_WIDTH, RADIOCHAT_BOTTON_HEIGHT);
		m_pUIRadioBotton[0]->BindTexture(CTexture::GetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_RADIOCHAT)));
		m_pUIRadioBotton[0]->SetTex(0, 1, RADIOCHAT_BOTTON_PATTERN);

		m_pUIRadioBotton[1] = CButton2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 + 200.0f, 280.0f, 0.0f), RADIOCHAT_BOTTON_WIDTH, RADIOCHAT_BOTTON_HEIGHT);
		m_pUIRadioBotton[1]->BindTexture(CTexture::GetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_RADIOCHAT)));
		m_pUIRadioBotton[1]->SetTex(1, 1, RADIOCHAT_BOTTON_PATTERN);

		m_pUIRadioBotton[2] = CButton2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 + 200.0f, 470.0f, 0.0f), RADIOCHAT_BOTTON_WIDTH, RADIOCHAT_BOTTON_HEIGHT);
		m_pUIRadioBotton[2]->BindTexture(CTexture::GetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_RADIOCHAT)));
		m_pUIRadioBotton[2]->SetTex(2, 1, RADIOCHAT_BOTTON_PATTERN);

		m_pUIRadioBotton[3] = CButton2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 + 100.0f, 570.0f, 0.0f), RADIOCHAT_BOTTON_WIDTH, RADIOCHAT_BOTTON_HEIGHT);
		m_pUIRadioBotton[3]->BindTexture(CTexture::GetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_RADIOCHAT)));
		m_pUIRadioBotton[3]->SetTex(3, 1, RADIOCHAT_BOTTON_PATTERN);

		m_pUIRadioBotton[4] = CButton2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 - 97.0f, 180.0f, 0.0f), RADIOCHAT_BOTTON_WIDTH, RADIOCHAT_BOTTON_HEIGHT);
		m_pUIRadioBotton[4]->BindTexture(CTexture::GetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_RADIOCHAT)));
		m_pUIRadioBotton[4]->SetTex(7, 1, RADIOCHAT_BOTTON_PATTERN);

		m_pUIRadioBotton[5] = CButton2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 - 200.0f, 280.0f, 0.0f), RADIOCHAT_BOTTON_WIDTH, RADIOCHAT_BOTTON_HEIGHT);
		m_pUIRadioBotton[5]->BindTexture(CTexture::GetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_RADIOCHAT)));
		m_pUIRadioBotton[5]->SetTex(6, 1, RADIOCHAT_BOTTON_PATTERN);

		m_pUIRadioBotton[6] = CButton2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 - 200.0f, 470.0f, 0.0f), RADIOCHAT_BOTTON_WIDTH, RADIOCHAT_BOTTON_HEIGHT);
		m_pUIRadioBotton[6]->BindTexture(CTexture::GetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_RADIOCHAT)));
		m_pUIRadioBotton[6]->SetTex(5, 1, RADIOCHAT_BOTTON_PATTERN);

		m_pUIRadioBotton[7] = CButton2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 - 97.0f, 570.0f, 0.0f), RADIOCHAT_BOTTON_WIDTH, RADIOCHAT_BOTTON_HEIGHT);
		m_pUIRadioBotton[7]->BindTexture(CTexture::GetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_RADIOCHAT)));
		m_pUIRadioBotton[7]->SetTex(4, 1, RADIOCHAT_BOTTON_PATTERN);

		m_pCursor = CMouseCursor2D::Create();		// �J�[�\��
	}

	if (m_pUIButtonOption == NULL)
	{	// �I�v�V�����{�^������
		m_pUIButtonOption = CButton2D::Create(D3DXVECTOR3(970.0f, 530.0f, 0.0f), 100.0f, 120.0f);
		m_pUIButtonOption->BindTexture(CTexture::GetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_OPTION_ICON)));
	}
}

//=============================================================================
//	�`���b�g�{�^���̔j��
//=============================================================================
void CPlayer::UninitRadioChatButton(void)
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

	if (m_pUIButtonOption != NULL)
	{	// �I�v�V�����{�^���j��
		m_pUIButtonOption->Uninit();
		m_pUIButtonOption = NULL;
	}
}
//=============================================================================
//	���W�I�`���b�g�{�^����������
//=============================================================================
void CPlayer::ChatBotton(void)
{
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();	// �L�[�{�[�h�̓��͂��擾
	CDirectInput *pDirectInput = CManager::GetDirectInput();	//DirectInput�̎擾
	CDirectInput::GamePad *DirectInputStick = pDirectInput->GetgamePadStick();

	if (pKeyboard->GetTrigger(DIK_C) || pDirectInput->GetGamePadTrigger(1))
	{
		m_nRadioChat = (m_nRadioChat + 1) % 2;

		if (m_nRadioChat == 1)
		{
			m_bChatBotton = true;		// �{�^���\����
			CreateRadioChatButton();	// �`���b�g�{�^���̐���
		}
		else
		{
			m_bChatBotton = false;		// �{�^����\����
			UninitRadioChatButton();	// �`���b�g�{�^���̔j��
		}
	}

	if (m_pUIRadioBotton[0] != NULL && m_pUIRadioBotton[1] != NULL && m_pUIRadioBotton[2] != NULL && m_pUIRadioBotton[3] != NULL
		&& m_pUIRadioBotton[4] != NULL && m_pUIRadioBotton[5] != NULL && m_pUIRadioBotton[6] != NULL && m_pUIRadioBotton[7] != NULL && m_pCursor != NULL && m_pUIButtonOption != NULL)
	{	// �{�^���ƃJ�[�\�����������ꂽ
		m_bChat = false;
		int nSelect = -1;

		// �I�v�V�����{�^���̐ݒ�
		if (m_pUIButtonOption->InRangeMenu(m_pCursor->GetMousePosition()))
		{// �͈͓����`�F�b�N
			if (m_pUIButtonOption->ClickRelease())
			{// �N���b�N���ꂽ
				m_bOption = true;				// �I�v�V�����ݒ蒆
				m_bChatBotton = false;		// �{�^����\��
			}
		}

		// �`���b�g�{�^���̔���
		if (m_bOption == false)
		{	// �I�v�V�����ݒ肶��Ȃ���
			for (int nCntButton = 0; nCntButton < RADIOCHAT_BOTTON; nCntButton++)
			{
				if (m_pUIRadioBotton[nCntButton]->InRangeMenu(m_pCursor->GetMousePosition()))
				{// �͈͓����`�F�b�N
					if (m_pUIRadioBotton[nCntButton]->ClickRelease())
					{// �N���b�N���ꂽ
						m_bChat = true;
						m_radiochat = (RADIOCHAT)nCntButton;
						break;
					}
					nSelect = nCntButton;
				}
			}
		}
	}
}

//=============================================================================
//	�{�^����������āA���b�Z�[�W�\��
//=============================================================================
void CPlayer::ChatMess(bool bChat)
{
	if (m_pUITexRadio == NULL)
	{
		m_bChatBotton = false;		// �{�^����\����

		m_pUITexRadio = CUI_TEXTURE::Create(D3DXVECTOR3(1280.0f, 550.0f, 0.0f), RADIOCHAT_MESS_WIDTH, RADIOCHAT_MESS_HEIGHT, CUI_TEXTURE::UIFLAME_RADIOCHAT_MESS);
		m_pUITexRadio->SetTex(m_radiochat, 1, RADIOCHAT_BOTTON_PATTERN);
		m_pUITexRadio->SetObjType(OBJTYPE_MESS);
	}
	if (m_pUITexRadio != NULL)
	{

		UninitRadioChatButton();
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
			m_nRadioChat = 0;				// �؂�ւ����Z�b�g
		}
	}
}

//=============================================================================
//	���Ԃ̃��b�Z�[�W��\�����鏈��
//=============================================================================
void CPlayer::AllyChatMess(void)
{
	if (m_pUITexAllyRadio == NULL)
	{//NULL�̏ꍇ
		m_pUITexAllyRadio = CUI_TEXTURE::Create(D3DXVECTOR3(1280.0f, 550.0f, 0.0f), RADIOCHAT_MESS_WIDTH, RADIOCHAT_MESS_HEIGHT, CUI_TEXTURE::UIFLAME_RADIOCHAT_MESS);
		m_pUITexAllyRadio->SetTex(m_allyRadiochat, 1, RADIOCHAT_BOTTON_PATTERN);
		m_pUITexAllyRadio->SetObjType(OBJTYPE_MESS);
	}

	if (m_pUITexAllyRadio != NULL)
	{//NULL�ł͂Ȃ��ꍇ
		bool bMove = false;		// �~�܂������ǂ���
		D3DXVECTOR3 texPos = m_pUITexAllyRadio->GetPos();		// ���݂̈ʒu���擾
		D3DXCOLOR texCol = m_pUITexAllyRadio->GetColor();		// ���݂̐F���擾

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
			m_nAllyTimer++;		// �J�E���^�[���Z
			if (m_nAllyTimer % RADIOCHAT_DISPLAY_TIME == 0)
			{	// 5�b�o������
				m_nAllyTimer = 0;
				m_bAllyCol = true;
			}
		}

		if (m_bAllyCol == true)
		{
			texCol.a -= RADIOCHAT_COL;
		}

		m_pUITexAllyRadio->SetPos(texPos);			// �ʒu���X�V
		m_pUITexAllyRadio->SetColor(texCol);		// �F���X�V

		if (texCol.a <= 0.0f)
		{	// ���S�ɐF������
			if (m_pUITexAllyRadio != NULL)
			{	// ���b�Z�[�W�e�N�X�`���̔j��
				m_pUITexAllyRadio->Uninit();
				m_pUITexAllyRadio = NULL;
			}
			m_bAllyChat = false;					// �`���b�g���Ă��Ȃ�
			bMove = false;
			m_bAllyCol = false;
		}
	}
}

//=============================================================================
//	�I�v�V�����̔j��
//=============================================================================
void CPlayer::UninitOption(void)
{
	for (int nCnt = 0; nCnt < OPTION_SELECT; nCnt++)
	{
		if (m_pUIButtonSelect[nCnt] != NULL)
		{	// ���ڂ̔j��
			m_pUIButtonSelect[nCnt]->Uninit();
			m_pUIButtonSelect[nCnt] = NULL;
		}
	}

	if (m_pUITexOption != NULL)
	{	// �t���[���̔j��
		m_pUITexOption->Uninit();
		m_pUITexOption = NULL;
	}

	if (m_pCursor != NULL)
	{	// �J�[�\���̔j��
		m_pCursor->Uninit();
		m_pCursor = NULL;
	}

	if (m_pUIButtonBack != NULL)
	{	// �~�{�^���̔j��
		m_pUIButtonBack->Uninit();
		m_pUIButtonBack = NULL;
	}
}

//=============================================================================
//	�I�v�V�����ݒ菈��
//=============================================================================
void CPlayer::Option(bool bOption)
{
	CCamera *pCamera = NULL;

	if (bOption == true)
	{
		if (m_pUITexOption == NULL && m_pUIButtonSelect[0] == NULL && m_pUIButtonSelect[1] == NULL
			&& m_pUIButtonSelect[2] == NULL && m_pUIButtonSelect[3] == NULL && m_pUIButtonSelect[4] == NULL && m_pUIButtonBack == NULL)
		{	// �}�E�X�ݒ�t���[���A�g�O���X�C�b�`����
			m_pUITexOption = CUI_TEXTURE::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), 450.0f, 200.0f, CUI_TEXTURE::UIFLAME_OPTION_FLAME);

			for (int nCnt = 0; nCnt < OPTION_SELECT; nCnt++)
			{	// �g�O���X�C�b�`�̐���
				m_pUIButtonSelect[nCnt] = CButton2D::Create(D3DXVECTOR3(460.0f + (nCnt * 92.0f), SCREEN_HEIGHT / 2 + 30.0f, 0.0f), 30.0f, 30.0f);
				m_pUIButtonSelect[nCnt]->BindTexture(CTexture::GetTexture(CTexture::TEXTURE_OPTION_TOGGlE));
				m_pUIButtonSelect[nCnt]->SetTex(0, 1, 2);		// �ŏ��͍��e�N�X�`��
			}

			// �~�{�^��
			m_pUIButtonBack = CButton2D::Create(D3DXVECTOR3(820.0f, 300.0f, 0.0f), 65.0f, 40.0f);
			m_pUIButtonBack->BindTexture(CTexture::GetTexture(CTexture::TEXTURE_OPTION_BATSU));
		}

		// �g�p���Ȃ��e�N�X�`���͔j������
		for (int nCnt = 0; nCnt < RADIOCHAT_BOTTON; nCnt++)
		{
			if (m_pUIRadioBotton[nCnt] != NULL)
			{	// ���W�I�`���b�g�{�^���̔j��
				m_pUIRadioBotton[nCnt]->Uninit();
				m_pUIRadioBotton[nCnt] = NULL;
			}
		}

		if (m_pUIButtonOption != NULL)
		{	// �I�v�V�����{�^���̔j��
			m_pUIButtonOption->Uninit();
			m_pUIButtonOption = NULL;
		}

		if (m_pUITexOption != NULL && m_pUIButtonSelect[0] != NULL&& m_pUIButtonSelect[1] != NULL
			&& m_pUIButtonSelect[2] != NULL && m_pUIButtonSelect[3] != NULL && m_pUIButtonSelect[4] != NULL)
		{	// �}�E�X�ݒ�t���[���A�g�O���X�C�b�`�������ꂽ
			bool bOptionSelect = false;		// �����I������Ă��Ȃ����

											// �J�������x�ݒ�{�^��
			for (int nCntButton = 0; nCntButton < OPTION_SELECT; nCntButton++)
			{
				if (m_pUIButtonSelect[nCntButton]->InRangeMenu(m_pCursor->GetMousePosition()))
				{// �͈͓����`�F�b�N
					if (m_pUIButtonSelect[nCntButton]->ClickRelease())
					{// �N���b�N���ꂽ
						bOptionSelect = true;
						m_nSelectOption = nCntButton;			// �N���b�N���ꂽ���
						SetSelectOption(m_nSelectOption);	// �I�����ڂ̐ݒ�
						break;
					}
				}

				if (nCntButton == m_nSelectOption)
				{	// �I���������
					m_pUIButtonSelect[nCntButton]->SetTex(1, 1, 2);
					m_pUIButtonSelect[nCntButton]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				}
				else
				{	// ����ȊO
					m_pUIButtonSelect[nCntButton]->SetTex(0, 1, 2);
					m_pUIButtonSelect[nCntButton]->SetColor(D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f));
				}
			}

			// �~�{�^��
			if (m_pUIButtonBack->InRangeMenu(m_pCursor->GetMousePosition()))
			{	// �͈͓����`�F�b�N
				if (m_pUIButtonBack->ClickRelease())
				{	// �N���b�N���ꂽ
					UninitOption();			// �I�v�V�����̔j��
					m_bOption = false;		// �I�v�V�����ݒ�OFF
					m_nRadioChat = 0;		// �؂�ւ����Z�b�g

				}
			}
		}
	}
}

//=============================================================================
//	�퓬�p�e�L�X�g�ǂݍ���
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
	else if (CMechaSelect::MECHATYPE_SNIPE == m_mecha) { pBattleFileName = SNIPE_BATTLE_FILE; }

	// �t�@�C�����J��
	pFile = fopen(pBattleFileName, "r");

	if (pFile != NULL)
	{
		sscanf(&acLine[0], "%s", &acData);				// �����f�[�^�ɃR�s�[

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
//	�T��
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

				//CDebugProc::Print("CPU�x�N�g���P�F%.2f %.2f %.2f", m_SearchVec_0.x, m_SearchVec_0.y, m_SearchVec_0.z);
				//CDebugProc::Print("CPU�x�N�g���Q�F%.2f %.2f %.2f", m_SearchVec_1.x, m_SearchVec_1.y, m_SearchVec_1.z);
				//CDebugProc::Print("CPU�x�N�g���R�F%.2f %.2f %.2f", m_SearchVec_2.x, m_SearchVec_2.y, m_SearchVec_2.z);

				//CDebugProc::Print("fCrossProduct0�F%.2f", fCrossProduct0);
				//CDebugProc::Print("fCrossProduct1�F%.2f", fCrossProduct1);
				//CDebugProc::Print("fCrossProduct2�F%.2f", fCrossProduct2);

				if ((fCrossProduct0 > 0.0f && fCrossProduct1 > 0.0f && fCrossProduct2 > 0.0f) || (fCrossProduct0 < 0.0f && fCrossProduct1 < 0.0f && fCrossProduct2 < 0.0f))
				{// �O�p�`�̓����ɓG������

					// ���������G�v���C���[�̏����擾
					m_pFindPlayer = pPlayer;

					if (m_nCountCollect >= COLLECTIONDATA_MAX)
					{// ���W�f�[�^���ő�܂ŏW�܂����ꍇ
						m_bCollectSwitch = true;// ���ϒl�̊���o�����@��؂�ւ���
						m_nCountCollect = 0;// �J�E���g������������
					}
					m_collectionPos[m_nCountCollect] = m_pFindPlayer->GetPos();	// �G�v���C���[�̈ʒu�����擾
					m_nCountCollect++;	// �J�E���g��i�߂�

					// �s���̔z�u
					m_PinPos = m_pFindPlayer->GetPos();
					m_bPin = true;

					bFind = true;
					return bFind;
				}
				else
				{
					// �������Ă���v���C���[�̏����폜����
					m_pFindPlayer = NULL;
					m_bPin = false;
					bFind = false;
				}

				//CDebugProc::Print("bFind = %d", bFind);
			}
		}
		// ���̃I�u�W�F�N�g������
		pScene = pSceneNext;
	}

	return bFind;
}

//=============================================================================
//	�퓬
//=============================================================================
void CPlayer::Battle(void)
{
	CScene *pScene = CScene::GetSceneTop(FIND_FIND_CHARACTER_PRIORITY);
	CScene *pSceneNext = NULL;
	D3DXVECTOR3 dispertion; // �u��

	while (pScene != NULL)
	{// NULL�ɂȂ�܂Ń��[�v
		pSceneNext = pScene->GetSceneNext();
		CScene::OBJTYPE objType = pScene->GetObjType();

		if (CScene::OBJTYPE_PLAYER == objType)
		{// �v���C���[�I�u�W�F�N�g�̂Ƃ�
			CPlayer *pPlayer = (CPlayer*)pScene;
			int nTeam = pPlayer->GetTeam();

			if (m_nTeam != nTeam)
			{// �`�[�����Ⴄ�Ƃ�
				if (rand() % 20 == 0)
				{
					// �e����
					CpuShoot();
				}
			}
		}

		 // ���̃I�u�W�F�N�g������
		pScene = pSceneNext;
	}
}

//=============================================================================
//	�ړ�
//=============================================================================
void CPlayer::BattleMovent(void)
{
	bool bKey = false;	// �{�^�������t���O
	float fDirMove;

	if (m_pFindPlayer != NULL)
	{// �G�v���C���[�𔭌����Ă���ꍇ
		// ���������G�v���C���[�̕����ֈړ�����
		fDirMove = atan2f(m_pFindPlayer->GetPos().x - m_pos.x, m_pFindPlayer->GetPos().z - m_pos.z);

		// ���������G�v���C���[�̕���������
		m_rotDest.y = atan2f(m_pFindPlayer->GetPos().x - m_pos.x, m_pFindPlayer->GetPos().z - m_pos.z);
		bKey = true;
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
//	�e���ˏ���
//=============================================================================
void CPlayer::CpuShoot(void)
{
	D3DXVECTOR3 dispertion;	// �e�̃u��

	// �e�̔��ˊԊu
	m_nCntShoot = (m_nCntShoot + 1) % 7;

	if (m_nRemBullet > 0)
	{// �e���c���Ă���ꍇ
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
			CBulletPlayer::Create(posCanon, m_pAngle[nCntShoots * 2], m_pAngleV[nCntShoots * 2], m_nAttack, m_nTeam, this,m_fBulletSpeed);

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

			// �e�̔���
			CBulletPlayer::Create(posCanon, m_pAngle[nCntShoots * 2 + 1], m_pAngleV[nCntShoots * 2 + 1], m_nAttack, m_nTeam, this,m_fBulletSpeed);
		}

		// �e�����炷
		m_nRemBullet--;
	}
	else if (m_nRemBullet <= 0)
	{// �e���Ȃ��Ȃ����ꍇ
		m_bReload = true;
	}
}

//=============================================================================
//	�V���O�����s���֌W�X�V����
//=============================================================================
void CPlayer::PinUpdateSingle()
{
	CInputMouse *pMouse = CManager::GetInputMouse(); // �}�E�X�̓��͂��擾

	//===================================
	// �����̃s���z�u
	//===================================
	if (pMouse->GetTrigger(CInputMouse::DIMS_BUTTON_2) == true || pMouse->GetMouseAxisZ() >= 120.0f || pMouse->GetMouseAxisZ() <= -120.0f)
	{// �z�C�[���N���b�Nor���[��
		// �J�����̊p�x�ƒ����_���擾
		D3DXVECTOR3 rotCamera = CManager::GetCamera()->GetRot();
		D3DXVECTOR3 posR = CManager::GetCamera()->GetPosR();

		// ���e�B�N���i�ړI�̈ʒu�j�̎擾
		D3DXVECTOR3 posReticle = m_pReticle->GetPos();

		// �ˏo���̈ʒu�̎擾
		D3DXMATRIX mtxCanon = m_pModel[2]->GetMtxWorld();
		D3DXVECTOR3 posCanon = D3DXVECTOR3(mtxCanon._41, mtxCanon._42, mtxCanon._43) + D3DXVECTOR3(sinf(rotCamera.y) * 30.0f, cosf(rotCamera.x) * 30.0f, cosf(rotCamera.y) * 30.0f);

		// ���������̊p�x�̌v�Z
		m_pAngle[0] = atan2f(posReticle.x - posCanon.x, posReticle.z - posCanon.z);
		m_pAngleV[0] = rotCamera.x;

		if (m_pPinBullet != NULL)
		{// �O��̒e�̔j��
			m_pPinBullet->Uninit();
			m_pPinBullet = NULL;
		}

		// �e�̐���
		m_pPinBullet = m_pPinBullet->Create(posCanon, m_pAngle[0], m_pAngleV[0], m_nTeam, this);
	}

	if (m_pPinBullet != NULL)
	{// ����p�̒e��NULL����Ȃ��Ƃ�
		if (m_pPinBullet->BulletCollision())
		{// �e�����������Ƃ�
		 // �s�����ėp�e�̔���`�F�b�N
			m_PinPos = m_pPinBullet->GetHitPos();
			m_pPinBullet->Uninit();
			m_pPinBullet = NULL;
		}

		if (m_PinPos != D3DXVECTOR3(0.0f, 0.0f, 0.0f))
		{// �e�����������Ƃ�
			if (m_pPin == NULL)
			{// �s������������Ă��Ȃ��ꍇ
			 // �s���̔z�u
				m_pPin = m_pPin->Create(D3DXVECTOR3(m_PinPos.x, 30.0f, m_PinPos.z));
				m_bPin = true;
			}
			else
			{// �s������������Ă���ꍇ
			 // �s���̈ʒu�X�V
				m_pPin->GetPos() = D3DXVECTOR3(m_PinPos.x, 30.0f, m_PinPos.z);
			}
			m_nPinLife = PIN_LIFE;
		}
	}

	if (m_bPin)
	{// �s�����g�p����Ă���ꍇ
	 // �s���̍X�V
		m_nPinLife--;

		if (m_nPinLife <= 0)
		{// ��莞�Ԍo�߂Ńs�����폜
			m_PinPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			m_bPin = false;
			m_pPin->Uninit();
			m_pPin = NULL;
		}
	}

	//===================================
	// AI�̃s���z�u
	//===================================
	CScene *pAIScene = CScene::GetSceneTop(AI_PRIORITY);
	CScene *pAISceneNext = NULL;
	int nCntAIMax = 0;

	while (pAIScene != NULL)
	{// NULL�ɂȂ�܂Ń��[�v
		pAISceneNext = pAIScene->GetSceneNext();
		CScene::OBJTYPE objType = pAIScene->GetObjType();

		if (objType == CScene::OBJTYPE_AI)
		{// AI�I�u�W�F�N�g�̂Ƃ�
			CAIMecha *pAI = (CAIMecha*)pAIScene;

			if (m_nTeam == pAI->GetTeam() && pAI->GetPlayer() == this)
			{// ������AI�Ƃ�
				// AI�̃s�������擾����
				m_AIPinPos[nCntAIMax] = pAI->GetPinPos();
				m_bAIPin[nCntAIMax] = pAI->GetPinUse();

				if (m_pAIPin[nCntAIMax] != NULL)
				{// �s������������Ă���ꍇ
					if (m_nAIPinLife[nCntAIMax] <= 0)
					{// ��莞�Ԍo�߂Ńs�����폜
						pAI->GetPinUse() = false;
						m_bAIPin[nCntAIMax] = false;
						m_pAIPin[nCntAIMax]->Uninit();
						m_pAIPin[nCntAIMax] = NULL;
					}
				}
				nCntAIMax++;
			}
		}

		// ���̃I�u�W�F�N�g������
		pAIScene = pAISceneNext;
	}

	for (int nCntAI = 0; nCntAI < AI_MAX; nCntAI++)
	{// AI�@�̐��������
		if (m_bAIPin[nCntAI])
		{// AI�̃s�����g���Ă���ꍇ
			if (m_pAIPin[nCntAI] == NULL)
			{// �s������������Ă��Ȃ��ꍇ
			 // �s���̔z�u
				m_nAIPinLife[nCntAI] = PIN_LIFE;
				m_pAIPin[nCntAI] = m_pAIPin[nCntAI]->Create(m_AIPinPos[nCntAI] + D3DXVECTOR3(0.0f, 70.0f, 0.0f));
				m_pAIPin[nCntAI]->GetTexture() = CTexture::GetTexture(CTexture::TEXTURE_ENEMY_PIN);
				m_bAIPin[nCntAI] = true;
			}
			else
			{// �s������������Ă���ꍇ
			 // �s���̍X�V
				m_nAIPinLife[nCntAI]--;
				m_pAIPin[nCntAI]->GetPos() = m_AIPinPos[nCntAI] + D3DXVECTOR3(0.0f, 70.0f, 0.0f);
			}
		}
	}

	//===================================
	// �����v���C���[�̃s���z�u
	//===================================
	CScene *pAllyPlayerScene = CScene::GetSceneTop(PLAYER_PRIORITY);
	CScene *pAllyPlayerSceneNext = NULL;
	D3DXVECTOR3 AllyPlayerPinPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	while (pAllyPlayerScene != NULL)
	{// NULL�ɂȂ�܂Ń��[�v
		pAllyPlayerSceneNext = pAllyPlayerScene->GetSceneNext();
		CScene::OBJTYPE objType = pAllyPlayerScene->GetObjType();

		if (objType == CScene::OBJTYPE_PLAYER)
		{// �v���C���[�I�u�W�F�N�g�̂Ƃ�
			CPlayer *pPlayer = (CPlayer*)pAllyPlayerScene;
			if (m_nTeam == pPlayer->GetTeam() && pPlayer != this)
			{// �`�[���������Ŏ������g����Ȃ��Ƃ�

			 // �����̃s�������擾����
				m_bAllyPin = pPlayer->GetPinUse();
				AllyPlayerPinPos = pPlayer->GetPinPos();
				m_AllyPosPinPos = pPlayer->GetPos();

				if (m_bAllyPin)
				{// �����̃s�����g���Ă���ꍇ
					if (m_pAllyPin != NULL)
					{// �s������������Ă���ꍇ
						if (m_nPinLife <= 0)
						{// ��莞�Ԍo�߂Ńs�����폜
							pPlayer->GetPinUse() = false;
							m_bAllyPin = false;
							m_pAllyPin->Uninit();
							m_pAllyPin = NULL;
						}
					}
				}
			}
		}

		// ���̃I�u�W�F�N�g������
		pAllyPlayerScene = pAllyPlayerSceneNext;
	}

	if (m_bAllyPin)
	{// �����̃s�����g���Ă���ꍇ
		if (m_pAllyPin == NULL)
		{// �����̃s������������Ă��Ȃ��ꍇ
		 // �s���̔z�u
			m_nAllyPinLife = PIN_LIFE;
			m_pAllyPin = m_pAllyPin->Create(AllyPlayerPinPos + D3DXVECTOR3(0.0f, 70.0f, 0.0f));
			m_pAllyPin->GetTexture() = CTexture::GetTexture(CTexture::TEXTURE_ENEMY_PIN);
		}
		else
		{// �����̃s������������Ă���ꍇ
		 // �s���̍X�V
			m_nPinLife--;
			m_pAllyPin->GetPos() = AllyPlayerPinPos + D3DXVECTOR3(0.0f, 70.0f, 0.0f);
		}
	}

	if (m_pAllyPosPin == NULL)
	{// �����̈ʒu�s������������Ă��Ȃ��ꍇ
	 // �s���̔z�u
		m_pAllyPosPin = m_pAllyPosPin->Create(m_AllyPosPinPos + D3DXVECTOR3(0.0f, 70.0f, 0.0f));
		m_pAllyPosPin->GetTexture() = CTexture::GetTexture(CTexture::TEXTURE_FRIEND_PIN);
	}
	else
	{// �����̈ʒu�s������������Ă���ꍇ
	 // �s���̍X�V
		m_pAllyPosPin->GetPos() = m_AllyPosPinPos + D3DXVECTOR3(0.0f, 70.0f, 0.0f);
	}

	//===================================
	// ����AI�̃s���z�u
	//===================================
	CScene *pAllyAIScene = CScene::GetSceneTop(AI_PRIORITY);
	CScene *pAllyAISceneNext = NULL;
	int nCntAllyAIMax = 0;

	while (pAllyAIScene != NULL)
	{// NULL�ɂȂ�܂Ń��[�v
		pAllyAISceneNext = pAllyAIScene->GetSceneNext();
		CScene::OBJTYPE objType = pAllyAIScene->GetObjType();

		if (objType == CScene::OBJTYPE_AI)
		{// AI�I�u�W�F�N�g�̂Ƃ�
			CAIMecha *pAllyAI = (CAIMecha*)pAllyAIScene;

			if (m_nTeam == pAllyAI->GetTeam() && pAllyAI->GetPlayer() != this)
			{// �`�[���������Ŏ�����AI����Ȃ��Ƃ�
			 // ���Ԃ̃s�������擾����
				m_AllyAIPinPos[nCntAllyAIMax] = pAllyAI->GetPinPos();
				m_bAllyAIPin[nCntAllyAIMax] = pAllyAI->GetPinUse();

				if (m_pAllyAIPin[nCntAllyAIMax] != NULL)
				{// �s������������Ă���ꍇ
					if (m_nAllyAIPinLife[nCntAllyAIMax] <= 0)
					{// ��莞�Ԍo�߂Ńs�����폜
						pAllyAI->GetPinUse() = false;
						m_bAllyAIPin[nCntAllyAIMax] = false;
						m_pAllyAIPin[nCntAllyAIMax]->Uninit();
						m_pAllyAIPin[nCntAllyAIMax] = NULL;
					}
				}
				nCntAllyAIMax++;
			}
		}

		// ���̃I�u�W�F�N�g������
		pAllyAIScene = pAllyAISceneNext;
	}

	for (int nCntAllyAI = 0; nCntAllyAI < AI_MAX; nCntAllyAI++)
	{// ����AI�@�̐��������
		if (m_bAllyAIPin[nCntAllyAI])
		{// ����AI�̃s�����g���Ă���ꍇ
			if (m_pAllyAIPin[nCntAllyAI] == NULL)
			{// �s������������Ă��Ȃ��ꍇ
			 // �s���̔z�u
				m_nAllyAIPinLife[nCntAllyAI] = PIN_LIFE;
				m_pAllyAIPin[nCntAllyAI] = m_pAllyAIPin[nCntAllyAI]->Create(m_AllyAIPinPos[nCntAllyAI] + D3DXVECTOR3(0.0f, 70.0f, 0.0f));
				m_pAllyAIPin[nCntAllyAI]->GetTexture() = CTexture::GetTexture(CTexture::TEXTURE_ENEMY_PIN);
			}
			else
			{// �s������������Ă���ꍇ
			 // �s���̍X�V
				m_nAllyAIPinLife[nCntAllyAI]--;
				m_pAllyAIPin[nCntAllyAI]->GetPos() = m_AllyAIPinPos[nCntAllyAI] + D3DXVECTOR3(0.0f, 70.0f, 0.0f);
			}
		}
	}
}

//=============================================================================
//	�}���`���s���֌W�X�V����
//=============================================================================
void CPlayer::PinUpdateMulti()
{
	CInputMouse *pMouse = CManager::GetInputMouse();	// �}�E�X�̓��͂��擾

	//===================================
	// �����̃s���z�u
	//===================================
	if (pMouse->GetTrigger(CInputMouse::DIMS_BUTTON_2) == true || pMouse->GetMouseAxisZ() >= 120.0f || pMouse->GetMouseAxisZ() <= -120.0f)
	{// �z�C�[���N���b�Nor���[��
		// �J�����̊p�x�ƒ����_���擾
		D3DXVECTOR3 rotCamera = CManager::GetCamera()->GetRot();
		D3DXVECTOR3 posR = CManager::GetCamera()->GetPosR();

		// ���e�B�N���i�ړI�̈ʒu�j�̎擾
		D3DXVECTOR3 posReticle = m_pReticle->GetPos();

		// �ˏo���̈ʒu�̎擾
		D3DXMATRIX mtxCanon = m_pModel[2]->GetMtxWorld();
		D3DXVECTOR3 posCanon = D3DXVECTOR3(mtxCanon._41, mtxCanon._42, mtxCanon._43) + D3DXVECTOR3(sinf(rotCamera.y) * 30.0f, cosf(rotCamera.x) * 30.0f, cosf(rotCamera.y) * 30.0f);

		// ���������̊p�x�̌v�Z
		m_pAngle[0] = atan2f(posReticle.x - posCanon.x, posReticle.z - posCanon.z);
		m_pAngleV[0] = rotCamera.x;

		if (m_pPinBullet != NULL)
		{// �O��̒e�̔j��
			m_pPinBullet->Uninit();
			m_pPinBullet = NULL;
		}

		// �e�̐���
		m_pPinBullet = m_pPinBullet->Create(posCanon, m_pAngle[0], m_pAngleV[0], m_nTeam, this);
	}

	if (m_pPinBullet != NULL)
	{// ����p�̒e��NULL����Ȃ��Ƃ�
		if (m_pPinBullet->BulletCollision())
		{// �e�����������Ƃ�
			// �s�����ėp�e�̔���`�F�b�N
			m_PinPos = m_pPinBullet->GetHitPos();
			m_pPinBullet->Uninit();
			m_pPinBullet = NULL;
		}

		if (m_PinPos != D3DXVECTOR3(0.0f, 0.0f, 0.0f))
		{// �e�����������Ƃ�
			if (m_pPin == NULL)
			{// �s������������Ă��Ȃ��ꍇ
				// �s���̔z�u
				m_pPin = m_pPin->Create(D3DXVECTOR3(m_PinPos.x, 30.0f, m_PinPos.z));
				m_bPin = true;
			}
			else
			{// �s������������Ă���ꍇ
				// �s���̈ʒu�X�V
				m_pPin->GetPos() = D3DXVECTOR3(m_PinPos.x, 30.0f, m_PinPos.z);
			}
			m_nPinLife = PIN_LIFE;
		}
	}

	if (m_bPin)
	{// �s�����g�p����Ă���ꍇ
	 // �s���̍X�V
		m_nPinLife--;

		if (m_nPinLife <= 0)
		{// ��莞�Ԍo�߂Ńs�����폜
			m_PinPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			m_bPin = false;
			m_pPin->Uninit();
			m_pPin = NULL;
		}
	}

	//===================================
	// ������AI�̃s���z�u
	//===================================
	CScene *pAIScene = CScene::GetSceneTop(AI_PRIORITY);
	CScene *pAISceneNext = NULL;
	int nCntAIMax = 0;

	while (pAIScene != NULL)
	{// NULL�ɂȂ�܂Ń��[�v
		pAISceneNext = pAIScene->GetSceneNext();
		CScene::OBJTYPE objType = pAIScene->GetObjType();

		if (objType == CScene::OBJTYPE_AI)
		{// AI�I�u�W�F�N�g�̂Ƃ�
			CAIMecha *pAI = (CAIMecha*)pAIScene;

			if (m_nTeam == pAI->GetTeam() && pAI->GetPlayer() == this)
			{// ������AI�̎�
			 // ���Ԃ̃s�������擾����
				m_AIPinPos[nCntAIMax] = pAI->GetPinPos();
				m_bAIPin[nCntAIMax] = pAI->GetPinUse();

				if (m_pAIPin[nCntAIMax] != NULL)
				{// �s������������Ă���ꍇ
					if (m_nAIPinLife[nCntAIMax] <= 0)
					{// ��莞�Ԍo�߂Ńs�����폜
						pAI->GetPinUse() = false;
						m_bAIPin[nCntAIMax] = false;
						m_pAIPin[nCntAIMax]->Uninit();
						m_pAIPin[nCntAIMax] = NULL;
					}
				}
				nCntAIMax++;
			}
		}

		// ���̃I�u�W�F�N�g������
		pAIScene = pAISceneNext;
	}

	for (int nCntAI = 0; nCntAI < AI_MAX; nCntAI++)
	{// AI�@�̐��������
		if (m_bAIPin[nCntAI])
		{// AI�̃s�����g���Ă���ꍇ
			if (m_pAIPin[nCntAI] == NULL)
			{// �s������������Ă��Ȃ��ꍇ
			 // �s���̔z�u
				m_nAIPinLife[nCntAI] = PIN_LIFE;
				m_pAIPin[nCntAI] = m_pAIPin[nCntAI]->Create(m_AIPinPos[nCntAI] + D3DXVECTOR3(0.0f, 70.0f, 0.0f));
				m_pAIPin[nCntAI]->GetTexture() = CTexture::GetTexture(CTexture::TEXTURE_ENEMY_PIN);
				m_bAIPin[nCntAI] = true;
			}
			else
			{// �s������������Ă���ꍇ
			 // �s���̍X�V
				m_nAIPinLife[nCntAI]--;
				m_pAIPin[nCntAI]->GetPos() = m_AIPinPos[nCntAI] + D3DXVECTOR3(0.0f, 70.0f, 0.0f);
			}
		}
	}

	//===================================
	// �����v���C���[�̃s���z�u
	//===================================
	if (m_bAllyPin)
	{// ���Ԃ̃s�����g���Ă���ꍇ
		if (m_pAllyPin == NULL)
		{// �s������������Ă��Ȃ��ꍇ
		 // �s���̔z�u
			m_pAllyPin = m_pAllyPin->Create(D3DXVECTOR3(m_AllyPinPos.x, 30.0f, m_AllyPinPos.z));
			m_pAllyPin->GetTexture() = CTexture::GetTexture(CTexture::TEXTURE_GOAL_PIN);
		}
		else
		{// �s������������Ă���ꍇ
		 // �s���̍X�V
			m_nAllyPinLife--;
			m_pAllyPin->GetPos() = D3DXVECTOR3(m_AllyPinPos.x, 30.0f, m_AllyPinPos.z);

			if (m_nAllyPinLife <= 0)
			{// ��莞�Ԍo�߂Ńs�����폜
				m_bAllyPin = false;
				m_pAllyPin->Uninit();
				m_pAllyPin = NULL;
			}
		}
	}

	//===================================
	// ����AI�̃s���z�u
	//===================================
	for (int nCntAllyAI = 0; nCntAllyAI < AI_MAX; nCntAllyAI++)
	{// ����AI�@�̐��������
		if (m_bAllyAIPin[nCntAllyAI])
		{// ����AI�̃s�����g���Ă���ꍇ
			if (m_pAllyAIPin[nCntAllyAI] == NULL)
			{// �s������������Ă��Ȃ��ꍇ
			 // �s���̔z�u
				m_pAllyAIPin[nCntAllyAI] = m_pAllyAIPin[nCntAllyAI]->Create(m_AllyAIPinPos[nCntAllyAI] + D3DXVECTOR3(0.0f, 70.0f, 0.0f));
				m_pAllyAIPin[nCntAllyAI]->GetTexture() = CTexture::GetTexture(CTexture::TEXTURE_ENEMY_PIN);
			}
			else
			{// �s������������Ă���ꍇ
			 // �s���̍X�V
				m_nAllyAIPinLife[nCntAllyAI]--;
				m_pAllyAIPin[nCntAllyAI]->GetPos() = m_AllyAIPinPos[nCntAllyAI] + D3DXVECTOR3(0.0f, 70.0f, 0.0f);

				if (m_nAllyAIPinLife[nCntAllyAI] <= 0)
				{// ��莞�Ԍo�߂Ńs�����폜
					m_bAllyAIPin[nCntAllyAI] = false;
					m_pAllyAIPin[nCntAllyAI]->Uninit();
					m_pAllyAIPin[nCntAllyAI] = NULL;
				}
			}
		}
	}

	//===================================
	// �����̈ʒu�s���z�u
	//===================================
	CScene *pAllyPlayerScene = CScene::GetSceneTop(PLAYER_PRIORITY);
	CScene *pAllyPlayerSceneNext = NULL;
	D3DXVECTOR3 AllyPlayerPinPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	while (pAllyPlayerScene != NULL)
	{// NULL�ɂȂ�܂Ń��[�v
		pAllyPlayerSceneNext = pAllyPlayerScene->GetSceneNext();
		CScene::OBJTYPE objType = pAllyPlayerScene->GetObjType();

		if (objType == CScene::OBJTYPE_PLAYER)
		{// �v���C���[�I�u�W�F�N�g�̂Ƃ�
			CPlayer *pPlayer = (CPlayer*)pAllyPlayerScene;
			if (m_nTeam == pPlayer->GetTeam() && pPlayer != this)
			{// �`�[���������Ŏ������g����Ȃ��Ƃ�
				m_AllyPosPinPos = pPlayer->GetPos();
			}
		}

		// ���̃I�u�W�F�N�g������
		pAllyPlayerScene = pAllyPlayerSceneNext;
	}

	if (m_pAllyPosPin == NULL)
	{// �����̃s������������Ă��Ȃ��ꍇ
	 // �s���̔z�u
		m_pAllyPosPin = m_pAllyPosPin->Create(m_AllyPosPinPos + D3DXVECTOR3(0.0f, 70.0f, 0.0f));
		m_pAllyPosPin->GetTexture() = CTexture::GetTexture(CTexture::TEXTURE_FRIEND_PIN);
	}
	else
	{// �����̃s������������Ă���ꍇ
	 // �s���̍X�V
		m_pAllyPosPin->GetPos() = m_AllyPosPinPos + D3DXVECTOR3(0.0f, 70.0f, 0.0f);
	}
}

//=============================================================================
//	AI�X�V����
//=============================================================================
void CPlayer::AIUpdate(void)
{
	//CDebugProc::Print("========�v���C���[========\n");
	//CDebugProc::Print("�v���C���[�̈ʒu x : %.1f / z : %.1f\n", m_pos.x, m_pos.z);
	//CDebugProc::Print("�J�n�n�_ : %d\n", m_nStartNode);
	//CDebugProc::Print("�ړI�n : %d\n", m_nEndNode);
	//CDebugProc::Print("�x�e����: %d\n", m_nBreaktime);
	//CDebugProc::Print("�f�[�^���W�J�E���^�[ : %d\n", m_nCollectionTimer);
	//CDebugProc::Print("���݂̈ړ���: %d\n", m_nPoint);
	//CDebugProc::Print("�ڕW�܂ł̈ړ���: %d\n", m_nCountPoint);
	//CDebugProc::Print("�S�[��������: %d\n", m_nGoalCount);
	//CDebugProc::Print("\n");

	// �O��̖ڕW�p�x�̕ۑ�
	m_rotDestOld = m_rotDest;

	for (int nCntSearch = 0; nCntSearch < MAX_SEARCH; nCntSearch++)
	{// �T���̍X�V����
		m_pSearch[nCntSearch]->Update();
	}

	// �O��̔�����Ԃ̎擾
	m_bFindOld = m_bFind;

	// ������Ԃ̎擾
	m_bFind = Distance();

	// ���ݒn�̍X�V
	if (m_bFindOld != m_bFind)
	{// �G�����������ꍇ�@�܂��́@�G�𔭌������ꍇ
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
	}

	// �s���̐ݒ�
	if (m_bFind == true)
	{// �G�𔭌����Ă���ꍇ
		//�퓬���̈ړ�����
		BattleMovent();

		//�퓬�̏���
		Battle();
	}
	else
	{
		// �����ړ�����
		CPlayer::AutoMove();
	}
	//CDebugProc::Print("m_bFind: %d", m_bFind);

	// �����[�h����
	if (m_bReload == true)
	{// �����[�h��Ԃ̏ꍇ
		m_nCntReRoad++;		// �J�E���^�[���Z

		if (m_nCntReRoad % m_nReload == 0)
		{// �ݒ莞�Ԍo�߂Ń����[�h������������
			m_nRemBullet = m_nCapacity;	// �e�𑕓U
			m_bReload = false;			// �����[�h������������
		}
	}

	//===========================================//
	//               ���W�f�[�^�Q��              //
	//===========================================//
	float fMinLength = 100000, fLength = 100000;	// �����n
	D3DXVECTOR3 total = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ���W�����f�[�^�̍��v�l

	if (!m_bCollectSwitch)
	{// 1�T��
		for (int nCntCollect = 0; nCntCollect < m_nCountCollect + 1; nCntCollect++)
		{// ���W�����f�[�^�̐��������
			total += m_collectionPos[nCntCollect];	// ���W�f�[�^�����v����
		}
	}
	else
	{// 2�T�ڈȍ~
		for (int nCntCollect = 0; nCntCollect < COLLECTIONDATA_MAX; nCntCollect++)
		{// ���W�ł���f�[�^�̍ő吔�������
			total += m_collectionPos[nCntCollect];	// ���W�f�[�^�����v����
		}
	}

	// ���W�f�[�^�̕��ϒl�����
	if (!m_bCollectSwitch)
	{// 1�T�ڂ̏ꍇ
		m_totalCollectPos = total / (float)(m_nCountCollect + 1);
	}
	else
	{// 2�T�ڈȍ~
		m_totalCollectPos = total / (float)COLLECTIONDATA_MAX;
	}

	// ���ϒl�ɍł��߂��m�[�h����������
	for (int nCntNode = 0; nCntNode < m_pNodeData->GetLoadData().nodeMax; nCntNode++)
	{// �m�[�h�̐��������
		// ���������߂�
		fLength =
			(m_pNodeData->GetLoadData().pos[nCntNode].x - m_totalCollectPos.x) *
			(m_pNodeData->GetLoadData().pos[nCntNode].x - m_totalCollectPos.x) +
			(m_pNodeData->GetLoadData().pos[nCntNode].z - m_totalCollectPos.z) *
			(m_pNodeData->GetLoadData().pos[nCntNode].z - m_totalCollectPos.z);

		if (fMinLength > fLength)
		{// �����̍ŏ��l�����߂�
			fMinLength = fLength;
			m_nNearTotalCollectNumber = nCntNode;
		}
	}

	if (m_rotDest.y > D3DX_PI) { m_rotDest.y -= D3DX_PI * 2.0f; }
	if (m_rotDest.y < -D3DX_PI) { m_rotDest.y += D3DX_PI * 2.0f; }

	if (m_fRotDestUpper.y > D3DX_PI) { m_fRotDestUpper.y -= D3DX_PI * 2.0f; }
	if (m_fRotDestUpper.y < -D3DX_PI) { m_fRotDestUpper.y += D3DX_PI * 2.0f; }

	if (m_rotDest.y - m_fRotDestUpper.y > D3DX_PI || m_rotDest.y - m_fRotDestUpper.y < -D3DX_PI)
	{// ������1���ȏ゠��ꍇ
		m_fRotDestUpper.y -= (m_rotDest.y - m_fRotDestUpper.y) * (m_fSpeed * 0.03f);
	}
	else
	{
		// �ڕW�����֌���
		m_fRotDestUpper.y += (m_rotDest.y - m_fRotDestUpper.y) * (m_fSpeed * 0.03f);
	}
}

//=============================================================================
//	�����ړ�����
//=============================================================================
void CPlayer::AutoMove()
{
	CMotionManager::TYPE type = CMotionManager::TYPE_NEUTRAL;	// ���[�V�����̎��
	bool bMove = false;	// �{�^�������t���O
	float fVigilanceRot[CONNECT_MAX];

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

		if (!m_bFind)
		{// �G��������
			m_rotDest.y = atan2f(m_posDest.x - m_pos.x, m_posDest.z - m_pos.z);
		}
	}
	else if (m_nBreaktime < 0)
	{// �ړ���
		bMove = true;
		m_nBreaktime = PLAYER_BREAKTIME;

		if (m_nPoint == m_nCountPoint)
		{// �ŏI�ڕW�n�_�ɓ��������玟�̖ړI�n��T��
			m_nBreaktime = PLAYER_FINALPOINT_BREAKTIME;
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

		for (int nCntConnect = 0; nCntConnect < m_pNodeData->GetLoadData().connectNum[m_nStartNode]; nCntConnect++)
		{// �q�����Ă�m�[�h�̐��������
			fVigilanceRot[nCntConnect] = atan2f(m_pNodeData->GetLoadData().pos[m_pNodeData->GetLoadData().connectIndex[m_nStartNode][nCntConnect]].x - m_pos.x, m_pNodeData->GetLoadData().pos[m_pNodeData->GetLoadData().connectIndex[m_nStartNode][nCntConnect]].z - m_pos.z) + D3DX_PI;
		}

		if (!m_bFind && (m_nBreaktime == PLAYER_FINALPOINT_BREAKTIME - 1 || m_nBreaktime % (PLAYER_FINALPOINT_BREAKTIME / m_pNodeData->GetLoadData().connectNum[m_nStartNode]) == 0))
		{// �G���������Ɍq�����Ă�m�[�h�̕������ׂĂ�����
			m_rotDest.y = fVigilanceRot[m_nVigilanceCount] + D3DX_PI;

			m_nVigilanceCount++;

			if (m_nVigilanceCount >= m_pNodeData->GetLoadData().connectNum[m_nStartNode])
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

	// �ړI�̐ݒ�
	if (m_nGoalCount % DATA_REFERENCE_TIME == 0)
	{// ����I�Ɏ��W�����f�[�^�Ɋ�Â����ړ����s��
		m_nNewEndNode = m_nNearTotalCollectNumber;
	}
	else
	{// �V�K�ړI�n���w��n�_���烉���_���Ō��肷��
		do
		{// �����n�_�������ꍇ�͂���1�x���߂�
			nMovePoint = m_nMovePoint[rand() % RANDOM_MOVE_POINT];
		} while (m_nNewEndNode == nMovePoint);
		m_nNewEndNode = nMovePoint;
	}

	for (int nCntNode = 0; nCntNode < m_pNodeData->GetLoadData().nodeMax; nCntNode++)
	{// �m�[�h�̐��������
		if (m_pNodeData->GetLoadData().pos[nCntNode].x + POS_ACCEPTABLE > m_pos.x
			&& m_pNodeData->GetLoadData().pos[nCntNode].x - POS_ACCEPTABLE < m_pos.x
			&& m_pNodeData->GetLoadData().pos[nCntNode].z + POS_ACCEPTABLE > m_pos.z
			&& m_pNodeData->GetLoadData().pos[nCntNode].z - POS_ACCEPTABLE < m_pos.z)
		{// �v���C���[�̈ʒu�����e�͈͓�
			m_nStartNode = m_nEndNode;	// �O��̖ړI�n���J�n�n�_�Ƃ��ēo�^
		}
	}

	for (int nCntNode = 0; nCntNode < m_pNodeData->GetLoadData().nodeMax; nCntNode++)
	{// �m�[�h�̐��������
		if (m_pNodeData->GetLoadData().pos[nCntNode].x + POS_ACCEPTABLE > m_pos.x
			&& m_pNodeData->GetLoadData().pos[nCntNode].x - POS_ACCEPTABLE < m_pos.x
			&& m_pNodeData->GetLoadData().pos[nCntNode].z + POS_ACCEPTABLE > m_pos.z
			&& m_pNodeData->GetLoadData().pos[nCntNode].z - POS_ACCEPTABLE < m_pos.z)
		{// �v���C���[�̈ʒu���ړI�n�̋��e�͈͓�
			if (m_pNodeData->GetLoadData().pos[nCntNode] != m_pNodeData->GetLoadData().pos[m_nNewEndNode])
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
	for (int nCntNode = 0; nCntNode < m_pNodeData->GetLoadData().nodeMax; nCntNode++, nCntWeight++)
	{// �m�[�h�̐��������
		weight[nCntWeight] = sqrt((m_pNodeData->GetLoadData().pos[m_nStartNode].x - m_pNodeData->GetLoadData().pos[nCntNode].x) * (m_pNodeData->GetLoadData().pos[m_nStartNode].x - m_pNodeData->GetLoadData().pos[nCntNode].x) + (m_pNodeData->GetLoadData().pos[m_nStartNode].z - m_pNodeData->GetLoadData().pos[nCntNode].z) * (m_pNodeData->GetLoadData().pos[m_nStartNode].z - m_pNodeData->GetLoadData().pos[nCntNode].z));
	}

	//======= �G�b�W�ǉ� =========================================================================
	for (int nCntNode = 0; nCntNode < m_pNodeData->GetLoadData().nodeMax; nCntNode++)
	{// �m�[�h�̐��������
		for (int nCntConnect = 0; nCntConnect < m_pNodeData->GetLoadData().connectNum[nCntNode]; nCntConnect++)
		{// �q�����Ă�m�[�h�̐��������
			CPlayer::AddEdge(nCntNode, m_pNodeData->GetLoadData().connectIndex[nCntNode][nCntConnect], weight[nCntNode], node);
		}
	}

	//======= �ŒZ�o�H�𒲂ׂ� =========================================================================
	CPlayer::Dijkstra(m_pNodeData->GetLoadData().nodeMax, m_nStartNode, m_nEndNode, node);

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
	for (int nCntNodeMax = 0; nCntNodeMax < m_pNodeData->GetLoadData().nodeMax; )
	{// �m�[�h�̐��������
		for (int nCntNode = path.size() - 1; nCntNode >= 0; nCntNode--, nCntNodeMax++)
		{
			m_waypoint[nCntNodeMax] = m_pNodeData->GetLoadData().pos[path[nCntNode]];
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
// ���X�|�[���ʒu�̃A�C�R���𐶐�
//=============================================================================
void CPlayer::CreateRespawnPosIcon(void)
{
	//�X�|�[���ʒu��UI����
	switch (m_nTeam)
	{//�`�[���ɂ���Đ�������
	case 0:
		if (m_pUIRespawnPosIcon[0] == NULL && m_pUIRespawnPosIcon[1] == NULL && m_pUIRespawnPosIcon[2] == NULL && m_pUIRespawnPosIcon[3] == NULL)
		{
			m_pUIRespawnPosIcon[0] = CUI_TEXTURE::Create(D3DXVECTOR3(355.0f, 210.0f, 0.0f), 30.0f, 30.0f, CUI_TEXTURE::UI_FLAME_RESPAWNPOS);
			m_pUIRespawnPosIcon[0]->BindTexture(CTexture::GetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_SELECT_RESPAWN)));
			m_pUIRespawnPosIcon[0]->SetTex(0, 1, 4);
			m_pUIRespawnPosIcon[0]->SetColor(D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f));

			m_pUIRespawnPosIcon[1] = CUI_TEXTURE::Create(D3DXVECTOR3(355.0f, 330.0f, 0.0f), 30.0f, 30.0f, CUI_TEXTURE::UI_FLAME_RESPAWNPOS);
			m_pUIRespawnPosIcon[1]->BindTexture(CTexture::GetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_SELECT_RESPAWN)));
			m_pUIRespawnPosIcon[1]->SetTex(1, 1, 4);
			m_pUIRespawnPosIcon[1]->SetColor(D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f));

			m_pUIRespawnPosIcon[2] = CUI_TEXTURE::Create(D3DXVECTOR3(355.0f, 390.0f, 0.0f), 30.0f, 30.0f, CUI_TEXTURE::UI_FLAME_RESPAWNPOS);
			m_pUIRespawnPosIcon[2]->BindTexture(CTexture::GetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_SELECT_RESPAWN)));
			m_pUIRespawnPosIcon[2]->SetTex(2, 1, 4);
			m_pUIRespawnPosIcon[2]->SetColor(D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f));

			m_pUIRespawnPosIcon[3] = CUI_TEXTURE::Create(D3DXVECTOR3(355.0f, 485.0f, 0.0f), 30.0f, 30.0f, CUI_TEXTURE::UI_FLAME_RESPAWNPOS);
			m_pUIRespawnPosIcon[3]->BindTexture(CTexture::GetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_SELECT_RESPAWN)));
			m_pUIRespawnPosIcon[3]->SetTex(3, 1, 4);
			m_pUIRespawnPosIcon[3]->SetColor(D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f));
		}
		break;
	case 1:
		if (m_pUIRespawnPosIcon[0] == NULL && m_pUIRespawnPosIcon[1] == NULL && m_pUIRespawnPosIcon[2] == NULL && m_pUIRespawnPosIcon[3] == NULL)
		{
			m_pUIRespawnPosIcon[0] = CUI_TEXTURE::Create(D3DXVECTOR3(925.0f, 210.0f, 0.0f), 30.0f, 30.0f, CUI_TEXTURE::UI_FLAME_RESPAWNPOS);
			m_pUIRespawnPosIcon[0]->BindTexture(CTexture::GetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_SELECT_RESPAWN)));
			m_pUIRespawnPosIcon[0]->SetTex(0, 1, 4);
			m_pUIRespawnPosIcon[0]->SetColor(D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f));

			m_pUIRespawnPosIcon[1] = CUI_TEXTURE::Create(D3DXVECTOR3(925.0f, 330.0f, 0.0f), 30.0f, 30.0f, CUI_TEXTURE::UI_FLAME_RESPAWNPOS);
			m_pUIRespawnPosIcon[1]->BindTexture(CTexture::GetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_SELECT_RESPAWN)));
			m_pUIRespawnPosIcon[1]->SetTex(1, 1, 4);
			m_pUIRespawnPosIcon[1]->SetColor(D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f));

			m_pUIRespawnPosIcon[2] = CUI_TEXTURE::Create(D3DXVECTOR3(925.0f, 390.0f, 0.0f), 30.0f, 30.0f, CUI_TEXTURE::UI_FLAME_RESPAWNPOS);
			m_pUIRespawnPosIcon[2]->BindTexture(CTexture::GetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_SELECT_RESPAWN)));
			m_pUIRespawnPosIcon[2]->SetTex(2, 1, 4);
			m_pUIRespawnPosIcon[2]->SetColor(D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f));

			m_pUIRespawnPosIcon[3] = CUI_TEXTURE::Create(D3DXVECTOR3(925.0f, 510.0f, 0.0f), 30.0f, 30.0f, CUI_TEXTURE::UI_FLAME_RESPAWNPOS);
			m_pUIRespawnPosIcon[3]->BindTexture(CTexture::GetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_SELECT_RESPAWN)));
			m_pUIRespawnPosIcon[3]->SetTex(3, 1, 4);
			m_pUIRespawnPosIcon[3]->SetColor(D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f));
		}
		break;
	}
}