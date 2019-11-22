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
#include "stage.h"
#include "modelSet.h"
#include "fade.h"
#include "life.h"
#include "particle.h"
#include "sound.h"
#include "scene3D.h"
#include "modelSet.h"
#include "file.h"

//==================================
// �}�N����`
//==================================
#define MODEL_FILE				"data/TEXT/PLAYER/model.txt"
#define JUDG_ICON_TEXTURE		"data/TEXTURE/judg_icon.png"
#define MOVE_SPEED				(3.0f)		// �ړ��̑��x
#define PLAYER_SLIP_ANGLE		(0.8f)		// ����悤�ɂȂ�p�x
#define JUMP_SPEED				(4.0f)		// �W�����v�̑��x
#define SLIP_ENABLE_SPEED		(1.0f)		// ����悤�ɂȂ鑬�x
#define PLAYER_LIFE				(100)		// �̗�
#define	PLAYER_LIFE_WIDTH		(500.0f)	// �̗̓o�[�̕�
#define PLAYER_LIFE_HEIGHT		(50.0f)		// �̗̓o�[�̍���
#define PLAYER_DAMAGE_TIME		(60)		// �_���[�W���󂯂����̖��G����
#define PLAYER_DAMAGE_MOVE		(40)		// �_���[�W���󂯂Ă��瓮����悤�ɂȂ�܂ł̎���
#define PLAYER_CLIMB_SPEED		(1.1f)		// �o��Ƃ��̑��x�̌�����
#define PLAYER_SLIP_SPEED		(1.7f)		// ���鑬�x
#define PLAYER_SLIP_JUMP		(0.75f)		// ������уW�����v���x�␳
#define PLAYER_JUMP_DASH		(25.0f)		// �W�����v���̋󒆃_�b�V���̑��x�␳
#define PLAYER_POS_JUMP_JUDGE	(50.0f)		// �W�����v����p�̃v���C���[����̋���

// =============================================================
// AI�֌W
// =============================================================
#define	LOAD_FILENAME		("data/TEXT/NODE_DATA/NodeData.txt")	// �}�b�v�f�[�^��ǂݍ��ރt�@�C���̖��O
#define POINT_SIZE_X		(18.0f)		// ���n�_�p�|���S���̑傫��X
#define POINT_SIZE_Z		(17.0f)		// ���n�_�p�|���S���̑傫��Y
#define MOVE_ACCEPTABLE		(20.0f)		// �ړ����̌덷�̋��e�͈�
#define POS_ACCEPTABLE		(30.0f)		// �������̌덷�̋��e�͈�
#define MOUSE_ACCEPTABLE	(20.0f)		// �}�E�X�̌덷�̋��e�͈�
#define COLLECT_TIME		(5)			// �f�[�^�̎��W���s���Ԋu(�b)
#define DATA_REFERENCE_TIME	(5)			// �f�[�^�̎Q�Ƃ��s���Ԋu(��)
#define PLAYER_BREAKTIME	(1)			// �x�e����(�t���[��)
#define PLAYER_FINALPOINT_BREAKTIME	(120)	// �ŏI�n�_�̋x�e����(�t���[��)

//==================================
// �ÓI�����o�ϐ��錾
//==================================
D3DXVECTOR3 CPlayer::m_searchPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
CPlayer *CPlayer::m_pPlayer = NULL;
CEnemy *CPlayer::m_pEnemy[ENEMY_MAX] = {};
CScene3D *CPlayer::m_pScene3D[NODEPOINT_MAX] = {};

//==================================
// ��������
//==================================
CPlayer* CPlayer::Create(void)
{
	CPlayer *pPlayer;

	pPlayer = new CPlayer;	// ���������m��
	m_pPlayer = pPlayer;

	if (pPlayer != NULL)
	{// �������m�ې���
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
	m_rotDest = 0.0f;
	m_fSpeed = 0.0f;
	m_fSlipSpeed = 0.0f;
	m_bJump = false;
	m_bSlip = false;
	m_fJumpSpeed = 0.0f;
	m_pModel = NULL;
	m_nNumParts = 0;
	m_fAngleSlip = 0.0f;
	m_pMotion = NULL;
	m_pLife = NULL;
	m_bDamage = false;
	m_bMove = true;
	m_nCntParticle = 0;
	m_bDash = false;
	m_bJumpKey = false;
	m_judgJump.bJump = false;
	m_judgJump.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_judgJump.pIcon = NULL;
	m_judgJump.fAngle = 0.0f;
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
	float fX, fY, fZ;
	int nId;
	int nParent;
	char **pModelName = NULL;

	// �t�@�C�����J��
	pFile = fopen(MODEL_FILE, "r");

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
		MessageBox(0, "�t�@�C��������܂���", "player/model.txt", MB_YESNO);
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

	// �|���S���̈ʒu��ݒ�
	m_fSpeed = MOVE_SPEED;	// ���x�̐ݒ�

	if (NULL == m_pMotion)
	{// ���[�V�����N���X�̐���
		m_pMotion = CMotion::Create(0, m_nNumParts, m_pModel);
		m_pMotion->SetMotion(CMotionManager::TYPE_NEUTRAL);	// �j���[�g�������[�V������ݒ�
	}

	if (NULL == m_pShadow)
	{// �e�̐���
		m_pShadow = CShadow::Create(&m_pos);
	}

	if (NULL == m_pLife && CManager::MODE_GAME == CManager::GetMode())
	{// �̗͂̐���
		//m_pLife = CLife2D::Create(PLAYER_LIFE, D3DXVECTOR3(10.0f, 0.0f, 0.0f), PLAYER_LIFE_WIDTH, PLAYER_LIFE_HEIGHT);
	}

	m_judgJump.pIcon = CScene3DBill::Create();
	m_judgJump.pIcon->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
	m_judgJump.pIcon->SetSize(D3DXVECTOR3(20.0f, 20.0f, 0.0f));
	m_judgJump.pIcon->SetLighting(false);

	// �e�N�X�`���̐���
	LPDIRECT3DTEXTURE9 pTexture;
	D3DXCreateTextureFromFile(pDevice, JUDG_ICON_TEXTURE, &pTexture);

	m_judgJump.pIcon->BindTexture(pTexture);

	// ���l�̏�����==============================================================================
	m_pNodeData = CGame::GetFile();
	m_posDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nPoint = 0;
	m_nCountPoint = 0;
	m_nCountCollect = 0;
	m_nBreaktime = 0;
	m_nGoalCount = 0;
	m_nCollectionTimer = COLLECT_TIME * 60;
	m_bGoal = false;
	m_bPartSwitch = false;
	m_bCollectSwitch = false;

	for (int nCntEnemy = 0; nCntEnemy < ENEMY_MAX; nCntEnemy++)
	{// �G�l�~�[�̍ő�l�����
		for (int nCntCollect = 0; nCntCollect < COLLECTIONDATA_MAX; nCntCollect++)
		{// ���W�f�[�^�̍ő�l�����
			m_collectionPos[nCntEnemy][nCntCollect] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		}
	}

	// �}�b�v�֌W==============================================================================
	m_pNodeData->FileLoad();	// �t�@�C���ǂݍ���

	// �J�n���_�̃m�[�h�̏�����
	for (int nCntNode = 0; nCntNode < m_pNodeData->GetLoadData().nodeMax; nCntNode++)
	{// �m�[�h�̐��������
		if (m_pNodeData->GetLoadData().pos[nCntNode].x + POS_ACCEPTABLE > m_pos.x
			&& m_pNodeData->GetLoadData().pos[nCntNode].x - POS_ACCEPTABLE < m_pos.x
			&& m_pNodeData->GetLoadData().pos[nCntNode].z + POS_ACCEPTABLE > m_pos.z
			&& m_pNodeData->GetLoadData().pos[nCntNode].z - POS_ACCEPTABLE < m_pos.z)
		{// �덷��+-10�ȓ�
			m_nStartNode = nCntNode;
		}
	}
	m_nEndNode = m_nMovePoint[rand() % 7];

	//// �n�_�����p
	//for (int nCntNode = 0; nCntNode < m_pNodeData->GetLoadData().nodeMax; nCntNode++)
	//{// �S�m�[�h�̐��������
	//	if (m_pScene3D[nCntNode] == NULL)
	//	{// 3D�|���S���̐���
	//		m_pScene3D[nCntNode] = m_pScene3D[nCntNode]->Create();
	//		if (m_pScene3D[nCntNode] != NULL)
	//		{// 3D�|���S���̐ݒ�
	//			m_pNodeData->GetLoadData().pos[nCntNode].y += 3.0f;
	//			m_pScene3D[nCntNode]->SetPos(m_pNodeData->GetLoadData().pos[nCntNode]);
	//			m_pScene3D[nCntNode]->SetWidth(POINT_SIZE_X);
	//			m_pScene3D[nCntNode]->SetDepth(POINT_SIZE_Z);
	//			m_pScene3D[nCntNode]->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	//		}
	//	}
	//}

	return S_OK;
}

//=========================================
// �I������
//=========================================
void CPlayer::Uninit(void)
{
	if (m_pScene3D != NULL)
	{// ���ݒu�n�_�̔j��
		for (int nCntNode = 0; nCntNode < NODEPOINT_MAX; nCntNode++)
		{
			if (m_pScene3D[nCntNode] != NULL)
			{
				m_pScene3D[nCntNode]->Uninit();
				m_pScene3D[nCntNode] = NULL;
			}
		}
	}

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

	if (NULL != m_pLife)
	{// ���C�t�̔j��
		m_pLife->Uninit();
		m_pLife = NULL;
	}

	if (NULL != m_judgJump.pIcon)
	{// �W�����v�^�C�~���O����p�A�C�R���̔j��
		m_judgJump.pIcon->Uninit();
	}

	// �I�u�W�F�N�g�̔j��
	CScene::Release();
}

//=========================================
// �X�V����
//=========================================
void CPlayer::Update(void)
{
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();	// �L�[�{�[�h�̓��͂��擾
	CInputMouse *pMouse = CManager::GetInputMouse();	// �}�E�X�̓��͂��擾
	D3DXVECTOR3 posOld = m_pos;	// �ʒu�̕ۑ�
	bool bJumpOld = m_bJump;	// �W�����v�t���O�̋L��

	// ���x������
	m_fSpeed = MOVE_SPEED;

	// AI�֌W�̍X�V
	CPlayer::AIUpdate(pMouse);

	// �ړ��̏���
	//CPlayer::Movement();

	// �d��
	m_move.y -= GRAVITY;

	for (int nCntModel = 0; nCntModel < m_nNumParts; nCntModel++)
	{// �p�[�c���f���̍X�V
		m_pModel[nCntModel]->Update();
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
	{// �p�[�c���f���̕`��
		if (m_bDamage)
		{// �_���[�W���󂯂Ă�����
			m_pModel[nCntModel]->AddColor(D3DXCOLOR(0.7f, 0.0f, 0.0f, -0.5f));
		}

		// ���f���̕`�揈��
		m_pModel[nCntModel]->Draw();

		if (m_bDamage)
		{// �_���[�W���󂯂Ă�����
			m_pModel[nCntModel]->AddColor(D3DXCOLOR(-0.7f, 0.0f, 0.0f, 0.5f));
		}
	}
}

//=========================================
// �ړ��̏���
//=========================================
void CPlayer::Movement(void)
{
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();	// �L�[�{�[�h�̓��͂��擾
	bool bMoveKey = false;			// �ړ��L�[�����t���O

	// �J�����̊p�x���擾
	float rotCamera = CManager::GetCamera()->GetRotY();

	if (pKeyboard->GetPress(DIK_W) == true)
	{// ��ړ�
		if (pKeyboard->GetPress(DIK_D) == true)
		{// �E��ړ�
			bMoveKey = true;
			m_rotDest = (D3DX_PI * 0.25f + rotCamera) - m_rot.y;
			m_move.x -= sinf(D3DX_PI * 0.25f + rotCamera) * m_fSpeed;
			m_move.z -= cosf(D3DX_PI * 0.25f + rotCamera) * m_fSpeed;
		}
		else if (pKeyboard->GetPress(DIK_A) == true)
		{// ����ړ�
			bMoveKey = true;
			m_rotDest = (D3DX_PI * -0.25f + rotCamera) - m_rot.y;
			m_move.x += sinf(D3DX_PI * 0.75f + rotCamera) * m_fSpeed;
			m_move.z += cosf(D3DX_PI * 0.75f + rotCamera) * m_fSpeed;
		}
		else
		{// ��̂�
			bMoveKey = true;
			m_rotDest = (D3DX_PI * 0.0f + rotCamera) - m_rot.y;
			m_move.x -= sinf(D3DX_PI * 0.0f + rotCamera) * m_fSpeed;
			m_move.z -= cosf(D3DX_PI * 0.0f + rotCamera) * m_fSpeed;
		}
	}
	else if (pKeyboard->GetPress(DIK_S) == true)
	{// ���ړ�
		if (pKeyboard->GetPress(DIK_D) == true)
		{// �E���ړ�
			bMoveKey = true;
			m_rotDest = (D3DX_PI * 0.75f + rotCamera) - m_rot.y;
			m_move.x -= sinf(D3DX_PI * 0.75f + rotCamera) * m_fSpeed;
			m_move.z -= cosf(D3DX_PI * 0.75f + rotCamera) * m_fSpeed;
		}
		else if (pKeyboard->GetPress(DIK_A) == true)
		{// �����ړ�
			bMoveKey = true;
			m_rotDest = (D3DX_PI * -0.75f + rotCamera) - m_rot.y;
			m_move.x += sinf(D3DX_PI * 0.25f + rotCamera) * m_fSpeed;
			m_move.z += cosf(D3DX_PI * 0.25f + rotCamera) * m_fSpeed;
		}
		else
		{// ���̂�
			bMoveKey = true;
			m_rotDest = (D3DX_PI * 1.0f + rotCamera) - m_rot.y;
			m_move.x += sinf(D3DX_PI * 0.0f + rotCamera) * m_fSpeed;
			m_move.z += cosf(D3DX_PI * 0.0f + rotCamera) * m_fSpeed;
		}
	}
	else if (pKeyboard->GetPress(DIK_D) == true)
	{// �E�ړ�
		bMoveKey = true;
		m_rotDest = (D3DX_PI * 0.5f + rotCamera) - m_rot.y;
		m_move.x -= sinf(D3DX_PI * 0.5f + rotCamera) * m_fSpeed;
		m_move.z -= cosf(D3DX_PI * 0.5f + rotCamera) * m_fSpeed;
	}
	else if (pKeyboard->GetPress(DIK_A) == true)
	{// ���ړ�
		bMoveKey = true;
		m_rotDest = (D3DX_PI * -0.5f + rotCamera) - m_rot.y;
		m_move.x += sinf(D3DX_PI * 0.5f + rotCamera) * m_fSpeed;
		m_move.z += cosf(D3DX_PI * 0.5f + rotCamera) * m_fSpeed;
	}

	if (m_pMotion)
	{// ���[�V�����N���X���g���Ă���
		if (bMoveKey && !m_bJump)
		{// �ړ����[�V����
			m_pMotion->SetMotion(CMotionManager::TYPE_WALK);
		}
		else if (!bMoveKey && (CMotionManager::TYPE_WALK == m_pMotion->GetType() || CMotionManager::TYPE_SLIP == m_pMotion->GetType() && !m_bSlip))
		{// �j���[�g�������[�V����
			m_pMotion->SetMotion(CMotionManager::TYPE_NEUTRAL);
		}
	}

	// �ڕW�̕����̏C��
	if (m_rotDest < -D3DX_PI)
	{
		m_rotDest += D3DX_PI * 2.0f;
	}
	if (m_rotDest > D3DX_PI)
	{
		m_rotDest -= D3DX_PI * 2.0f;
	}

	// �ړ��Ɖ�]�̐ݒ�
	m_pos += m_move;
	m_rot.y += m_rotDest * 0.1f;

	// ����
	m_move.x += (0.0f - m_move.x) * 0.2f;
	m_move.z += (0.0f - m_move.z) * 0.2f;

	// �����̏C��
	if (m_rot.y < -D3DX_PI)
	{
		m_rot.y += D3DX_PI * 2.0f;
	}
	if (m_rot.y > D3DX_PI)
	{
		m_rot.y -= D3DX_PI * 2.0f;
	}

	// �ڕW�̕����̏�����
	m_rotDest = 0.0f;

	float fHeight = 0.0f;	// �N���̍��������߂�
	CScene *pScene = NULL;	// �I�u�W�F�N�g�Ǘ��N���X�̃|�C���^�ϐ�

	// �n�ʃI�u�W�F�N�g��T��
	pScene = CScene::GetSceneTop(MESHFIELD_PRIORITY);

	while (pScene != NULL)
	{// NULL�ɂȂ�܂Ń��[�v
		CScene *pSceneNext = pScene->GetSceneNext();
		CScene::OBJTYPE objType = pScene->GetObjType();

		if (objType == CScene::OBJTYPE_FIELD)
		{// �n�ʂ������Ƃ�
			CMeshField *pMeshField = (CMeshField*)pScene;	// �n�ʂ̃|�C���^���擾

			fHeight = pMeshField->GetHeight(m_pos);			// �n�ʂ̍����̎擾
		}

		// ���̃I�u�W�F�N�g������
		pScene = pSceneNext;
	}

	if (m_pos.y <= fHeight)
	{// �n�ʈȉ��ɂȂ����Ƃ�
		m_pos.y = fHeight;	// ���������W�ɓ����
		m_move.y = 0.0f;	// Y���̈ړ��ʂ𖳂���
	}
}

//=========================================
// �W�����v�̏���
//=========================================
void CPlayer::Jump(float fAngleOld)
{
	if (m_fAngle < 0.0f && fAngleOld >= 0.0f && !m_bJump && m_bSlip)
	{// ����Ă��Ēn�ʂɂ��Ȃ��Ȃ����Ƃ�
		m_move.y = fAngleOld * m_fSlipSpeed * PLAYER_SLIP_JUMP;
		if (m_move.y > 0.0f)
		{// ��ɔ�񂾎��W�����v��Ԃɂ���
			m_bJump = true;
			m_bDash = false;
			m_judgJump.bJump = false;

			CSound *pSound = CManager::GetSound();		// �T�E���h�̎擾
			pSound->PlaySound(CSound::SOUND_LABEL_JUMP);
		}
		m_move.x = sinf(m_rotDest) * m_fSlipSpeed * PLAYER_SLIP_JUMP;
		m_pMotion->SetMotion(CMotionManager::TYPE_JUMP);
	}
}

//=========================================
// �X�΂̏���
//=========================================
void CPlayer::Tilt(void)
{
	CSound *pSound = CManager::GetSound();			// �T�E���h�̎擾
	float fCosTheta = 0.0f;							// �v�Z����
	D3DXVECTOR3 nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �@��

														// �n�ʃI�u�W�F�N�g��T��
	CScene *pScene = CScene::GetSceneTop(MESHFIELD_PRIORITY);

	while (pScene != NULL)
	{// NULL�ɂȂ�܂Ń��[�v
		CScene *pSceneNext = pScene->GetSceneNext();
		CScene::OBJTYPE objType = pScene->GetObjType();

		if (objType == CScene::OBJTYPE_FIELD)
		{// �n�ʂ������Ƃ�
			CMeshField *pMeshField = (CMeshField*)pScene;

			m_fAngle = pMeshField->GetAngle(m_pos, m_rot.y);	// �X�΂̊p�x�̎擾
			nor = pMeshField->GetNor(m_pos);					// �n�ʂ̖@���̎擾

			if (!m_bJump)
			{// ��̉���������擾
				m_fAngleSlip = pMeshField->GetSlipAngle(m_pos);
			}
		}

		// ���̃I�u�W�F�N�g������
		pScene = pSceneNext;
	}

	CDebugProc::Print("�n�ʂƂ�cos�ƁF%.2f", m_fAngle);

	// �J�����̎擾
	CCamera *pCamera = CManager::GetCamera();

	if (!m_bSlip)
	{// �����Ă��Ȃ�
		if (!m_bJump && nor.y < PLAYER_SLIP_ANGLE && nor.y != 0.0f && 0 > m_fAngle)
		{// �W�����v���ĂȂ����p�x�����ȉ��Ŋ���
			m_bSlip = true;
			pSound->PlaySound(CSound::SOUND_LABEL_SLIP);

			if (m_fAngle < 0.0f)
			{// ���艺���Ƃ����x������
				m_fSlipSpeed = m_fSpeed;
			}
		}

		if (!m_bJump)
		{// �n��
			if (m_fAngle < 0.0f)
			{// �����Ƃ��ɑ��x���グ��
				m_fSpeed += MOVE_SPEED * (1.0f - nor.y);
			}
			else if (m_fAngle > 0.0f)
			{// ���Ƃ��ɑ��x��������
				m_fSpeed -= MOVE_SPEED * (1.0f - nor.y) * PLAYER_CLIMB_SPEED;
			}
		}
	}
	else
	{// �����Ă���
		if (!m_bJump)
		{// ����ړ��ʂ𑝂₷
			m_fSlipSpeed += PLAYER_SLIP_SPEED * (1.0f - nor.y) * -m_fAngle;

			// ���藎����Ƃ��̃p�[�e�B�N��
			CParticle::Create(m_pos, 2);

			if (m_fAngle > 0.0f)
			{// �o���Ă���Ƃ�
				m_fSlipSpeed -= PLAYER_SLIP_SPEED * m_fAngle;
			}

			m_move.x += sinf(m_fAngleSlip) * m_fSlipSpeed;

			if (0.0 <= m_rot.y && 0 < m_move.x)
			{// �ړ��ʂ��t�ɂ���
				m_move.x *= -1.0f;
			}
			else if (0.0 >= m_rot.y && 0 > m_move.x)
			{// �ړ��ʂ��t�ɂ���
				m_move.x *= -1.0f;
			}
		}
		else
		{// �W�����v��
			pSound->StopSound(CSound::SOUND_LABEL_SLIP);
		}
	}

	if (m_fSlipSpeed > -3.0f && m_fSlipSpeed < 3.0f)
	{// ���x�����ȉ��Œʏ��
		m_bSlip = false;
		pSound->StopSound(CSound::SOUND_LABEL_SLIP);
		m_fSlipSpeed = 0;
	}

	// �ړ��ʂ̃x�N�g��
	D3DXVECTOR3 vecMove = (m_pos + m_move) - m_pos;

	// �ړ��ʃx�N�g���Ɩ@����cos�Ƃ����߂�
	fCosTheta = (vecMove.x * nor.x) + (vecMove.y * nor.y) + (vecMove.z * nor.z);
	fCosTheta = fCosTheta / (sqrtf(powf(vecMove.x, 2) + powf(vecMove.y, 2) + powf(vecMove.z, 2)) * (sqrtf(powf(nor.x, 2) + powf(nor.y, 2) + powf(nor.z, 2))));

	if (isnan(fCosTheta))
	{// nan�`�F�b�N
		fCosTheta = 0.0f;
	}

	// �p�x�ɂ���ăp�[�e�B�N����ς���
	if (nor.y < PLAYER_SLIP_ANGLE && 0 < m_fAngle && (m_move.x >= 0.01f || m_move.x <= -0.01f))
	{// �����o��
		m_nCntParticle = (1 + m_nCntParticle) % 15;

		if (m_nCntParticle == 0)
		{// �p�[�e�B�N�����o��
			CParticle::Create(m_pos + D3DXVECTOR3(0.0f, 40.0f, 0.0f), 0);
		}
	}

	CDebugProc::Print("���鑬�x�F%.2f", m_fSlipSpeed);
	CDebugProc::Print("�W�����v���x�F%.2f", m_fJumpSpeed);
}

//=========================================
// �����蔻��
//=========================================
bool CPlayer::Collision(D3DXVECTOR3 pos, float fRange)
{
	bool bHit = false;

	// �����XY�͈̔�
	float fLength = powf(fRange, 2);

	float fLengthX = pos.x - m_mtxWorld._41;	// X�̍�
	float fLengthY = pos.y - m_mtxWorld._42;	// Y�̍�
	float fLengthZ = pos.z - m_mtxWorld._43;	// Z�̍�

	float fLengthTotal = powf(fLengthX, 2) + powf(fLengthY, 2) + powf(fLengthZ, 2);		// XYZ�̍��̓��

	if (fLength >= fLengthTotal)
	{// ����Ǝ����̔���
		bHit = true;	// �����L���ɂ���
	}

	return bHit;	// ���茋�ʂ�Ԃ�
}

//=========================================
// �_���[�W���󂯂��Ƃ��̏���
//=========================================
void CPlayer::Damage(int nDamage)
{
	if (m_pMotion)
	{// ���[�V�����N���X���g���Ă���
		if (NULL != m_pLife && m_pMotion->GetType() != CMotionManager::TYPE_DAMAGE && !m_bDamage)
		{// ���C�t�N���X���g���Ă���
			m_pLife->AddSubtract(nDamage);
			m_pMotion->SetMotion(CMotionManager::TYPE_DAMAGE);	// �_���[�W���[�V�������Đ�
			m_bDamage = true;									// �_���[�W���󂯂Ă����Ԃɂ���
			CSound *pSound = CManager::GetSound();				// �T�E���h�̎擾
			pSound->PlaySound(CSound::SOUND_LABEL_DAMAGE);		// �_���[�W�����Đ�

			if (0 >= m_pLife->GetLife())
			{// �̗͂�0�ȉ��ɂȂ���
				Uninit();
				CGame::SetGameState(CGame::STATE_END);
			}
		}
	}
}

//=============================================================================
//	AI�X�V����
//=============================================================================
void CPlayer::AIUpdate(CInputMouse *pMouse)
{
	//CDebugProc::Print("========�v���C���[========\n");
	//CDebugProc::Print("�v���C���[�̈ʒu x : %.1f / z : %.1f\n", m_pos.x, m_pos.z);
	CDebugProc::Print("�J�n�n�_ : %d\n", m_nStartNode);
	CDebugProc::Print("�ړI�n : %d\n", m_nEndNode);	
	//CDebugProc::Print("�x�e����: %d\n", m_nBreaktime);
	CDebugProc::Print("�f�[�^���W�J�E���^�[ : %d\n", m_nCollectionTimer);
	//CDebugProc::Print("���݂̈ړ���: %d\n", m_nPoint);
	//CDebugProc::Print("�ڕW�܂ł̈ړ���: %d\n", m_nCountPoint);
	CDebugProc::Print("�S�[��������: %d\n", m_nGoalCount);
	CDebugProc::Print("���v�l x : %.1f / z : %.1f\n", m_totalCollectPos.x, m_totalCollectPos.z);
	CDebugProc::Print("���ϒl�ɍł��߂��m�[�h[%d] x : %.1f / z : %.1f\n", m_nNearTotalCollectNumber, m_pNodeData->GetLoadData().pos[m_nNearTotalCollectNumber].x, m_pNodeData->GetLoadData().pos[m_nNearTotalCollectNumber].z);
	CDebugProc::Print("\n");

	for (int nCntEnemy = 0; nCntEnemy < CEnemy::GetEnemyMax(); nCntEnemy++)
	{// �G�l�~�[�̐������
		CDebugProc::Print("�G�l�~�[[%d]\n", nCntEnemy);
		for (int nCntCollect = 0; nCntCollect < COLLECTIONDATA_MAX; nCntCollect++)
		{// ���W�f�[�^�̍ő�l�����
			CDebugProc::Print("���W�����f�[�^[%d] x : %.1f / z : %.1f\n", nCntCollect, m_collectionPos[nCntEnemy][nCntCollect].x, m_collectionPos[nCntEnemy][nCntCollect].z);
		}
		CDebugProc::Print("\n");
	}

	float fMinLength = 100000, fLength = 100000;	// �����n
	D3DXVECTOR3 total = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ���W�����f�[�^�̍��v�l
	int nCntEnemyMax = 0;	// �G�l�~�[�������̃J�E���^
	CScene *pScene = CScene::GetSceneTop(ENEMY_PRIORITY);// �v���C�I���e�B�[�`�F�b�N
	m_nCollectionTimer--;	// �f�[�^���W�̃J�E���^��i�߂�

	// �G�l�~�[�̃|�C���^�̎擾
	while (pScene != NULL)
	{// �V�[����NULL�`�F�b�N
	 // Update��Uninit����Ă��܂��ꍇ�@Next��������\�������邩��Next�Ƀf�[�^���c���Ă���
		CScene *pSceneNext = pScene->GetSceneNext();
		if (pScene->GetDeath() == false)
		{// ���S�t���O�������Ă��Ȃ�
			if (pScene->GetObjType() == OBJTYPE_ENEMY)
			{// �^�C�v���G�l�~�[
				m_pEnemy[nCntEnemyMax] = ((CEnemy*)pScene)->GetEnemy();// �G�l�~�[�̏��̎擾
				nCntEnemyMax++;// �J�E���^��i�߂�
			}
		}
		// Next�Ɏ���Scene������
		pScene = pSceneNext;
	}

	// �f�[�^�̎��W
	if (m_nCollectionTimer <= 0)
	{// ���莞�Ԃ��ƂɃf�[�^���W
		// �ʒu�f�[�^�̎擾
		for (int nCntEnemy = 0; nCntEnemy < nCntEnemyMax; nCntEnemy++)
		{// �G�l�~�[�̐��������
			if (m_pEnemy[nCntEnemy] != NULL)
			{// �G�l�~�[��NULL�`�F�b�N
				m_collectionPos[nCntEnemy][m_nCountCollect] = m_pEnemy[nCntEnemy]->GetPos();	// �G�̈ʒu�����擾
				m_nCollectionTimer = COLLECT_TIME * 60;	// ���Ԃ�߂�
			}
		}

		// ���W�����f�[�^�����v����
		for (int nCntEnemy = 0; nCntEnemy < nCntEnemyMax; nCntEnemy++)
		{// �G�l�~�[�̐��������
			if (m_pEnemy[nCntEnemy] != NULL)
			{// �G�l�~�[��NULL�`�F�b�N
				if (!m_bCollectSwitch)
				{// 1�T��
					for (int nCntCollect = 0; nCntCollect < m_nCountCollect + 1; nCntCollect++)
					{// ���W�����f�[�^�̐��������
						total += m_collectionPos[nCntEnemy][nCntCollect];	// ���W�f�[�^�����v����
					}
				}
				else
				{// 2�T�ڈȍ~
					for (int nCntCollect = 0; nCntCollect < COLLECTIONDATA_MAX; nCntCollect++)
					{// ���W�ł���f�[�^�̍ő吔�������
						total += m_collectionPos[nCntEnemy][nCntCollect];	// ���W�f�[�^�����v����
					}
				}
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

		// �f�[�^�����W�����񐔂̃J�E���g
		if (m_nCountCollect < COLLECTIONDATA_MAX - 1)
		{// ���W�ő�ɒB���Ă��Ȃ��ꍇ
			m_nCountCollect++;	// ���W�J�E���g��i�߂�
		}
		else
		{// ���W�ő�ɒB�����ꍇ
			m_bCollectSwitch = true;// ���ϒl�̊���o�����@��؂�ւ���
			m_nCountCollect = 0;	// ���W�J�E���g���ŏ�����ɂ���
		}

		// ���ϒl�̍ł��߂��m�[�h����������
		for (int nCntNode = 0; nCntNode < m_pNodeData->GetLoadData().nodeMax; nCntNode++)
		{// �m�[�h�̐��������
		 // ���������߂�
			float fLength = (m_pNodeData->GetLoadData().pos[nCntNode].x - m_totalCollectPos.x) * (m_pNodeData->GetLoadData().pos[nCntNode].x - m_totalCollectPos.x) + (m_pNodeData->GetLoadData().pos[nCntNode].z - m_totalCollectPos.z) * (m_pNodeData->GetLoadData().pos[nCntNode].z - m_totalCollectPos.z);

			if (fMinLength > fLength)
			{// �����̍ŏ��l�����߂�
				fMinLength = fLength;
				m_nNearTotalCollectNumber = nCntNode;
			}
		}
	}

	// �����ړ�����
	CPlayer::AutoMove();
}

//=============================================================================
//	�����ړ�����
//=============================================================================
void CPlayer::AutoMove()
{
	CMotionManager::TYPE type = CMotionManager::TYPE_NEUTRAL;	// ���[�V�����̎��
	bool bMoveKey = false;	// �ړ��L�[�����t���O

	// �ڕW�n�_��ݒ�
	m_posDest = m_waypoint[m_nPoint];

	// �ړI�Ƃ̍������o��
	float fLength = (m_pos.x - m_posDest.x) * (m_pos.x - m_posDest.x) + (m_pos.z - m_posDest.z) * (m_pos.z - m_posDest.z);
	m_nBreaktime--;

	if (fLength > MOVE_ACCEPTABLE)
	{// ���������e�l���Ɏ��܂�܂ŖړI�n�Ɉړ�����
		bMoveKey = true;
		m_move.x = sinf(atan2f(m_posDest.x - m_pos.x, m_posDest.z - m_pos.z)) * m_fSpeed;
		m_move.z = cosf(atan2f(m_posDest.x - m_pos.x, m_posDest.z - m_pos.z)) * m_fSpeed;
		m_rot.y = atan2f(m_posDest.x - m_pos.x, m_posDest.z - m_pos.z) + D3DX_PI;
	}
	else if (m_nBreaktime < 0)
	{// �ړ���x�e
		bMoveKey = false;
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
		m_nPoint++;
	}
	else if(m_nBreaktime > 0)
	{// �x�e��
		m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ��~����
	}

	if (m_pMotion)
	{// �g���Ă���
		if (type != CMotionManager::TYPE_ATTACK01 && type != CMotionManager::TYPE_DAMAGE)
		{// �U�����[�V�����łȂ���
			if (m_move.x > 0.1f || m_move.x < -0.1f || m_move.z > 0.1f || m_move.z < -0.1f)
			{// �ړ����[�V������ݒ�
				m_pMotion->SetMotion(CMotionManager::TYPE_WALK);		// ���[�V�����̐ݒ�
			}
			else
			{// �j���[�g�������[�V������ݒ�
				m_pMotion->SetMotion(CMotionManager::TYPE_NEUTRAL);		// ���[�V�����̐ݒ�
			}
		}
	}

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

	//// �����ɍł��߂��G�l�~�[��T��
	//for (int nCntEnemy = 0; nCntEnemy < CEnemy::GetEnemyMax(); nCntEnemy++)
	//{// �G�l�~�[�̐��������
	//	if (m_pEnemy[nCntEnemy] != NULL)
	//	{// �G�l�~�[��NULL�`�F�b�N
	//	 // �S�ẴG�l�~�[�ƃv���C���[�Ƃ̍������o��
	//		fLength = (m_pEnemy[nCntEnemy]->GetPos().x - m_pos.x) * (m_pEnemy[nCntEnemy]->GetPos().x - m_pos.x) + (m_pEnemy[nCntEnemy]->GetPos().z - m_pos.z) * (m_pEnemy[nCntEnemy]->GetPos().z - m_pos.z);

	//		if (fMinLength > fLength)
	//		{// �����̍ŏ��l�����߂�
	//			fMinLength = fLength;
	//			nNearEnemyNumber = nCntEnemy;
	//		}
	//	}
	//}

	//for (int nCntNode = 0; nCntNode < m_pNodeData->GetLoadData().nodeMax; nCntNode++)
	//{// �m�[�h�̐��������
	//	if (m_pEnemy[nNearEnemyNumber] != NULL)
	//	{// �G�l�~�[��NULL�`�F�b�N
	//		if (m_pNodeData->GetLoadData().pos[nCntNode].x + POS_ACCEPTABLE > m_pEnemy[nNearEnemyNumber]->GetPos().x
	//			&& m_pNodeData->GetLoadData().pos[nCntNode].x - POS_ACCEPTABLE < m_pEnemy[nNearEnemyNumber]->GetPos().x
	//			&& m_pNodeData->GetLoadData().pos[nCntNode].z + POS_ACCEPTABLE > m_pEnemy[nNearEnemyNumber]->GetPos().z
	//			&& m_pNodeData->GetLoadData().pos[nCntNode].z - POS_ACCEPTABLE < m_pEnemy[nNearEnemyNumber]->GetPos().z)
	//		{// �ł��߂��G�̈ʒu�����e�͈͓�
	//			m_nEndNode = nCntNode;
	//		}
	//	}
	//}
}

//=============================================================================
//	���[�g��������
//=============================================================================
void CPlayer::RootSearch()
{
	Node node[NODEPOINT_MAX];	// �m�[�h�̏��
	float weight[NODEPOINT_MAX];	// �e�G�b�W�̃R�X�g
	int nCntWeight = 0;		// �R�X�g�̃J�E���^
	std::vector<int> path;	// �ŒZ�o�H�̏���ێ�����vector

	//======= �G�b�W�R�X�g�̎Z�o =========================================================================
	for (int nCntNode = 0; nCntNode < m_pNodeData->GetLoadData().nodeMax; nCntNode++, nCntWeight++)
	{// �m�[�h�̐��������
		weight[nCntWeight] = sqrt((m_pNodeData->GetLoadData().pos[m_nStartNode].x - m_pNodeData->GetLoadData().pos[nCntNode].x) * (m_pNodeData->GetLoadData().pos[m_nStartNode].x - m_pNodeData->GetLoadData().pos[nCntNode].x) + (m_pNodeData->GetLoadData().pos[m_nStartNode].z - m_pNodeData->GetLoadData().pos[nCntNode].z) * (m_pNodeData->GetLoadData().pos[m_nStartNode].z - m_pNodeData->GetLoadData().pos[nCntNode].z));
	}

	////======= �R�X�g�\�̏o�� =========================================================================
	//CDebugProc::Print("========�ړ��R�X�g�\========\n");
	//for (int nCntNode = 0; nCntNode < m_pNodeData->GetLoadData().nodeMax; nCntNode++)
	//{// �m�[�h�̐��������
	//	CDebugProc::Print(" No.%d : %.1f\n", nCntNode, weight[nCntNode]);
	//}
	//CDebugProc::Print("=============================\n\n");

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
	//CDebugProc::Print(" �ŒZ�o�H�́y");
	for (int nCntNode = path.size() - 1; nCntNode >= 0; nCntNode--)
	{
		//CDebugProc::Print(" %d ", path[nCntNode]);
		if (m_nCountPoint < nCntNode)
		{// �ő�l����
			// �ړI�܂ł̈ړ��񐔂��擾����
			m_nCountPoint = nCntNode;
		}
	}
	//CDebugProc::Print("�z�̏��ł��B\n\n");

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
	//// �m�[�hv�̓m�[�hu�ƂȂ����Ă����������
	//node[second].to.push_back(first);
	//// �m�[�hv�ƃm�[�hu�̃G�b�W�̏d�݂�����
	//node[second].cost.push_back(weight);
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