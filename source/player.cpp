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

//==================================
// �}�N����`
//==================================
#define ASSULT_FILE					"data/TEXT/PLAYER/assult/model_assult.txt"
#define LIGHT_FILE					"data/TEXT/PLAYER/light/model_light.txt"
#define HEAVY_FILE					"data/TEXT/PLAYER/heavy/model_heavy.txt"
#define SNIPE_FILE					"data/TEXT/PLAYER/snipe/model_snipe.txt"

#define PLAYER_DAMAGE_TIME			(60)		// �_���[�W���󂯂����̖��G����
#define PLAYER_DAMAGE_MOVE			(40)		// �_���[�W���󂯂Ă��瓮����悤�ɂȂ�܂ł̎���
#define PLAYER_RETICLE_LENGTH		(2500.0f)	// ���e�B�N���̋���
#define ANIM_SPEED							(10)		// �^�C���A�j���[�V����
#define ANIM_PATTERN						(8)		// �^�C���̃p�^�[����
#define PLAYER_BOTTON_WIDTH	(75.0f)	// ���X�|�[���{�^���̉���
#define PLAYER_BOTTON_HEIGHT	(75.0f)	// ���X�|�[���{�^���̏c��
#define PLAYER_BOTTON_INT			(40.0f)	// �{�^���ƃ{�^���̊Ԋu
#define PLAYER_UI_HEIGHT				(630.0f)

//==================================
// �ÓI�����o�ϐ��錾
//==================================

//==================================
// ��������
//==================================
CPlayer* CPlayer::Create(int nPlayerIdx, CMechaSelect::MECHATYPE mecha, D3DXVECTOR3 pos)
{
	CPlayer *pPlayer;

	pPlayer = new CPlayer;	// ���������m��

	if (pPlayer != NULL)
	{// �������m�ې���
		pPlayer->SetPlayerIdx(nPlayerIdx);
		pPlayer->m_mecha = mecha;
		pPlayer->m_pos = pos;
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

	for (int nCnt = 0; nCnt < PLAYER_UI_NUM; nCnt++)
	{
		m_pUINum[nCnt] = NULL;
	}

	for (int nCnt = 0; nCnt < PLAYER_BOTTON; nCnt++)
	{
		m_apButtonUI[nCnt] = NULL;
	}

	for (int nCnt = 0; nCnt < MAX_UITEX; nCnt++)
	{// �|�C���^��3�ڂ���g�p���Ă��邽�߁A�����l��2
		m_pUITex[nCnt] = NULL;
	}

	// �J�E���^�[�̏�����
	m_nCntAnim = 0;
	m_nPatternAnim = 0;
	m_nCntShoot = 0;
	m_bShootButton = false;
}

//=========================================
// �f�X�g���N�^
//=========================================
CPlayer::~CPlayer()
{
}

//=========================================
// ����������
//=========================================
HRESULT CPlayer::Init(void)
{
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
				m_pReticle->SetZBuffer(D3DCMP_ALWAYS);
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
			m_pReticle->SetZBuffer(D3DCMP_ALWAYS);
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
	else if(m_nPlayerIdx == 2 || m_nPlayerIdx == 3)
	{
		for (int nCntModel = 0; nCntModel < m_nNumParts; nCntModel++)
		{
			m_pModel[nCntModel]->SetColor(D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
		}

		m_nTeam = 1;

		// �J�����̌����̐ݒ�
		CManager::GetCamera()->SetRot(D3DXVECTOR3(D3DX_PI * 0.5f, D3DX_PI * -0.5f, 0.0f));
	}

	CAIMecha::Create(this, CAIMecha::MECHATYPE_DRONE, D3DXVECTOR3(0.0f, 70.0f, 0.0f));

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

	// �I�u�W�F�N�g�̔j��
	CScene::Release();
}

//=========================================
// �X�V����
//=========================================
void CPlayer::Update(void)
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
	else if(CMenu::GetMode() == CMenu::MODE_SINGLE)
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

				// ���C�t�̐ݒ�
				m_pUINum[1]->SetPlayerLife(m_nLife);
			}
		}

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

	if (pMouse->GetPress(CInputMouse::DIMS_BUTTON_0) && m_nRemBullet > 0)
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
	Reload();

	// �c�e�̐ݒ�
	m_pUINum[0]->SetRemainBullet(m_nRemBullet);
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

	D3DXVECTOR3 rot = m_pModel[1]->GetRot();

	//if(m_fCameraAngle + (D3DX_PI * 0.25f) < rot.y || m_fCameraAngle + (D3DX_PI * -0.25f) > rot.y)
	{// ����O�ɂȂ���
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
// �n�ʂ̋N���ɏ�鏈��
//=========================================
void CPlayer::FieldWalk(void)
{
	// �n�ʂ̃I�u�W�F�N�g��T��
	CScene *pScene = CScene::GetSceneTop(MESHFIELD_PRIORITY);
	CScene *pSceneNext = NULL;
	while (NULL != pScene)
	{// NULL�܂ł܂킷
		pSceneNext = pScene->GetSceneNext();				// ���̃I�u�W�F�N�g��ۊ�
		CScene::OBJTYPE objType = pScene->GetObjType();		// �I�u�W�F�N�g�̎�ނ��擾

		if (objType == CScene::OBJTYPE_FIELD)
		{// �n�ʂ������Ƃ�

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
// �����[�h����
//=========================================
void CPlayer::Reload(void)
{
	//****************************************
	// �e��0�[�������[�h���S�A�Q�[�W�\��
	//****************************************
	if (m_nRemBullet <= 0)
	{
		m_nCntReRoad++;		// �J�E���^�[���Z

							//****************************************
							// NULL�`�F�b�N����̃����[�h���S����
							//****************************************
		if (m_pUITex[0] == NULL)
		{	// �e�̂Ƃ���
			m_pUITex[0] = CUI_TEXTURE::Create(D3DXVECTOR3(1100.0f, 650.0f, 0.0f), 200.0f, 80.0f, CUI_TEXTURE::UIFLAME_RELOAD);
		}
		if (m_pUITex[1] == NULL)
		{	// ��ʒ���
			m_pUITex[1] = CUI_TEXTURE::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), 200.0f, 80.0f, CUI_TEXTURE::UIFLAME_RELOAD);
		}

		// �����[�h���S�_��
		m_pUITex[0]->Flashing(30);
		m_pUITex[1]->Flashing(30);

		//****************************************
		//�Q�[�W�̐���
		//****************************************
		if (m_pGauge == NULL)
		{
			m_pGauge = CGauge2D::Create(2, D3DXVECTOR3(SCREEN_WIDTH / 2, 400.0f, 0.0f), 0.0f, 100.0f, 300.0f, 30.0f);
			m_pGauge->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), 0);	// ���̒���
			m_pGauge->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 1);	// ���݂̗̑�
		}

		// �Q�[�W����
		m_pGauge->AddSubtract(100.0f / m_nReload);		// 100�� / �b��

															//*******************************************
															// �ݒ肵���b���と���S���Q�[�W�j�����e����
															//*******************************************
		if (m_nCntReRoad % m_nReload == 0)
		{
			// �e����
			m_nRemBullet = m_nCapacity;

			if (m_pGauge != NULL)
			{	// �Q�[�W�̔j��
				m_pGauge->Uninit();
				m_pGauge = NULL;
			}

			for (int nCnt = 0; nCnt < 2; nCnt++)
			{	// �����[�h���S�̔j��
				if (m_pUITex[nCnt] != NULL)
				{
					m_pUITex[nCnt]->Uninit();
					m_pUITex[nCnt] = NULL;
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
		if (m_pUITex[2] == NULL || m_pUITex[3] == NULL)
		{	// ��������
			m_pUITex[2] = CUI_TEXTURE::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), 500.0f, 260.0f, CUI_TEXTURE::UIFLAME_TILE_PATTERN);	// �^�C��
			m_pUITex[2]->SetTex(0, 1, 8);		// �����^�C���p�^�[��
			m_pUITex[3] = CUI_TEXTURE::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), 350.0f, 150.0f, CUI_TEXTURE::UIFLAME_START);	// �퓬�J�n���S
		}
		if (m_pUITex[2] != NULL || m_pUITex[3] != NULL)
		{	// �������ꂽ
			m_nCntAnim++;		// �A�j���[�V����
			m_nTimer++;			// ���S��������܂ł̃^�C�}�[
			if ((m_nCntAnim % ANIM_SPEED) == 0)
			{	// �A�j���[�V��������
				m_nPatternAnim++;		// �A�j���[�V�����p�^�[����
				m_pUITex[2]->SetTex(m_nPatternAnim, 1, ANIM_PATTERN);

				if (m_nPatternAnim > (ANIM_PATTERN - 1))
				{	// 8�p�^�[���ڂ܂ł�����A�����I��8�p�^�[���ڂɂ���
					m_pUITex[2]->SetTex((ANIM_PATTERN - 1), 1, ANIM_PATTERN);

					if (m_nTimer % 60 == 0)
					{	// ���S��������܂ł̎���
						m_nDisTime--;
					}
					if (m_nDisTime <= 0)
					{	// 0�ɂȂ���
						for (int nCnt = 2; nCnt < MAX_UITEX; nCnt++)
						{	// �|�C���^��3�ڂ���g�p���Ă��邽�߁A�����l��2
							if (m_pUITex[nCnt] != NULL)
							{	// UI�e�N�X�`���̔j��
								m_pUITex[nCnt]->Uninit();
								m_pUITex[nCnt] = NULL;
							}
						}
						respawn = RESPAWN_NONE;		// �ʏ���
						m_nCntAnim = 0;
						m_nPatternAnim = 0;
					}	// 0�ɂȂ�����
				}	// �p�^�[�����ő�܂ł���
			}	// �A�j���[�V�����̏���
		}	// �������ꂽ
		break;

	case RESPAWN_DEATH:
		if (m_pUITex[2] == NULL || m_pUITex[3] == NULL || m_pUITex[4] == NULL || m_pUINum[2] == NULL)
		{	// NULL�̎��A�������ŏ��̃e�N�X�`����ݒ�
			m_pUITex[2] = CUI_TEXTURE::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), 500.0f, 260.0f, CUI_TEXTURE::UIFLAME_TILE_PATTERN);	// �^�C��
			m_pUITex[2]->SetTex(0, 1, 8);		// �����^�C���p�^�[��
			m_pUITex[3] = CUI_TEXTURE::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), 350.0f, 150.0f, CUI_TEXTURE::UIFLAME_DEATH);	// �s���s�\���S
			m_pUITex[4] = CUI_TEXTURE::Create(D3DXVECTOR3(560.0f, 460.0f, 0.0f), 200.0f, 30.0f, CUI_TEXTURE::UIFLAME_RESPAWN);	// ������A
			m_pUINum[2] = CUI_NUMBER::Create(D3DXVECTOR3(750.0f, 460.0f, 0.0f), 60.0f, 60.0f, 30.0f, CUI_NUMBER::UI_NUMTYPE_CNTRESPAWN, 0, NUMTEX_UV_X, NUMTEX_UV_Y);	// ������A�J�E���^�[
		}
		if (m_pUITex[2] != NULL || m_pUITex[3] != NULL || m_pUITex[4] != NULL || m_pUINum[2] != NULL)
		{	// �������Ă��鎞
			m_nCntAnim++;
			if ((m_nCntAnim % ANIM_SPEED) == 0)
			{	// �A�j���[�V��������
				m_nPatternAnim++;
				m_pUITex[2]->SetTex(m_nPatternAnim, 1, ANIM_PATTERN);

				if (m_nPatternAnim > (ANIM_PATTERN - 1))
				{	// 8�p�^�[���ڂ܂ł�����A�����I��8�p�^�[���ڂɂ���
					m_pUITex[2]->SetTex((ANIM_PATTERN - 1), 1, ANIM_PATTERN);
				}
			}

			// CUI_NUMBER����A������A�J�E���^�[�擾
			int nRespawn = m_pUINum[2]->GetRespawn();
			if (nRespawn <= 0)
			{	// �J�E���^�[0�ȉ��ɂȂ�����
				respawn = RESPAWN_SELECT;		// ���X�|�[���I����Ԃɐݒ�

				for (int nCnt = 2; nCnt < MAX_UITEX; nCnt++)
				{	// �|�C���^��3�ڂ���g�p���Ă��邽�߁A�����l��2
					if (m_pUITex[nCnt] != NULL)
					{	// UI�e�N�X�`���̔j��
						m_pUITex[nCnt]->Uninit();
						m_pUITex[nCnt] = NULL;
					}
				}

				if (m_pUINum[2] != NULL)
				{	// ������A�J�E���^�[�̔j��
					m_pUINum[2]->Uninit();
					m_pUINum[2] = NULL;
				}

				// �J�E���^�[�̏�����
				m_nCntAnim = 0;
				m_nPatternAnim = 0;
			}
		}
		break;
	}

	m_Respawn = respawn;

#ifdef _DEBUG
	CDebugProc::Print("m_nCntAnim : %d\n", m_nCntAnim);
	CDebugProc::Print("m_nPatternAnim : %d\n", m_nPatternAnim);
	CDebugProc::Print("m_nDisTime : %d\n", m_nDisTime);
	CDebugProc::Print("m_nTimer : %d\n", m_nTimer);
#endif
}

//=========================================
// ���X�|�[���I������
//=========================================
void CPlayer::SelectRespawn(void)
{
	if (m_pUITex[5] == NULL || m_pUITex[6] == NULL || m_pUITex[7] == NULL || m_pUITex[5] == NULL || m_pCursor == NULL)
	{	// UI�̐���
		m_pUITex[5] = CUI_TEXTURE::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), SCREEN_WIDTH, SCREEN_HEIGHT, CUI_TEXTURE::UIFLAME_NONE);	// ���n
		m_pUITex[6] = CUI_TEXTURE::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, 360.0f, 0.0f), 500.0f, 370.0f, CUI_TEXTURE::UIFLAME_MAP);	// �}�b�v
		m_pUITex[7] = CUI_TEXTURE::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, 120.0f, 0.0f), 400.0f, 80.0f, CUI_TEXTURE::UIFLAME_TITLE);	// ���X�|�[���^�C�g��
		m_pUITex[8] = CUI_TEXTURE::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, PLAYER_UI_HEIGHT, 0.0f), 500.0f, 100.0f, CUI_TEXTURE::UIFLAME_RESPAWN_FLAME);		// �t���[��
		m_pCursor = CMouseCursor2D::Create();		// �J�[�\��
	}

	for (int nCnt = 0; nCnt < PLAYER_BOTTON; nCnt++)
	{
		if (m_apButtonUI[nCnt] == NULL)
		{	// �{�^���̐���
			m_apButtonUI[nCnt] = CButton2D::Create(D3DXVECTOR3(470.0f + (nCnt *(PLAYER_BOTTON_WIDTH + PLAYER_BOTTON_INT)), PLAYER_UI_HEIGHT, 0.0f),
				PLAYER_BOTTON_WIDTH, PLAYER_BOTTON_HEIGHT);  	// ���X�|�[���{�^���̉���
			m_apButtonUI[nCnt]->BindTexture(CTexture::GetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_SELECT_RESPAWN)));
			m_apButtonUI[nCnt]->SetTex(nCnt, 1, 4);
		}
	}

	if (m_apButtonUI[0] != NULL || m_apButtonUI[1] != NULL || m_apButtonUI[2] != NULL || m_apButtonUI[3] != NULL)
	{	// ��������Ă�����
		bool bBottonSwitch = false;
		int nSelect = -1;
		POINT point = POINT_A;

		// �{�^���̔���
		for (int nCntButton = 0; nCntButton < PLAYER_BOTTON; nCntButton++)
		{
			if (m_apButtonUI[nCntButton]->InRange(m_pCursor->GetMousePosition()))
			{// �͈͓����`�F�b�N
				if (m_apButtonUI[nCntButton]->ClickRelease())
				{// �N���b�N���ꂽ
					bBottonSwitch = true;
					point = (POINT)nCntButton;
					break;
				}
				nSelect = nCntButton;
			}
		}

		// �{�^�������ꂽ���̏���
		if (bBottonSwitch)
		{	// true�̎�
			switch (point)
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
			for (int nCnt = 0; nCnt < PLAYER_BOTTON; nCnt++)
			{
				if (m_apButtonUI[nCnt] != NULL)
				{	// �{�^���̔j��
					m_apButtonUI[nCnt]->Uninit();
					m_apButtonUI[nCnt] = NULL;
				}
			}

			for (int nCnt = 5; nCnt < MAX_UITEX; nCnt++)
			{
				if (m_pUITex[nCnt] != NULL)
				{	// �|�C���^��6�ڂ���g�p���Ă��邽�߁A�����l��5
					m_pUITex[nCnt]->Uninit();
					m_pUITex[nCnt] = NULL;
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