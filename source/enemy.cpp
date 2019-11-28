//=============================================================================
//
// �G���� [enemy.cpp]
// Author : Takuto Ishida
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "enemy.h"
#include "scene.h"
#include "manager.h"
#include "renderer.h"
#include "model.h"
#include "motion.h"
#include "player.h"
#include "meshField.h"
#include "game.h"
#include "modelSet.h"
#include "shadow.h"
#include "life.h"
#include "sound.h"

#include <stdio.h>

#include "debugProc.h"
#include "mouseCursor.h"
#include "input.h"

//==================================
// �}�N����`
//==================================
#define ENEMY_MODEL_FILE	"data/TEXT/ENEMY/model.txt"
#define ENEMY_SPEED		(2.0f)	// �ړ��̑��x
#define ENEMY_LIFE		(10)		// ���C�t
#define ENEMY_SHOOT		(120)	// �U���Ԋu

#define ENEMY_LIFE_WIDTH	(50.0f)		// �̗̓o�[�̕�
#define ENEMY_LIFE_HEIGHT	(10.0f)		// �̗̓o�[�̍���

#define ENEMY_WIDTH			(30.0f)		// �����蔻��̕�
#define ENEMY_DAMAGE_TIME	(30)		// �_���[�W��Ԃ̎���

// ���[�V�����ԍ�
#define ENEMY_NEUTRAL		(0)		// �j���[�g����
#define ENEMY_WALK			(1)		// ���s
#define ENEMY_JUMP			(2)		// �W�����v
#define ENEMY_LAND			(3)		// ���n
#define ENEMY_DAMAGE		(4)		// �_���[�W
#define ENEMY_ATTACK		(5)		// �U��

// ���[�g�T���p
#define POINT_SIZE			(25.0f)		// ���n�_�p�|���S���̑傫��
#define PLAYER_BREAKTIME	(15)		// �x�e����(�t���[��)
#define MOUSE_ACCEPTABLE	(40.0f)		// �}�E�X�̌덷�̋��e�͈�
#define MOVE_ACCEPTABLE		(50.0f)		// �ړ��̌덷�̋��e�͈�

//==================================
// �ÓI�����o�ϐ��錾
//==================================
int				*CEnemy::m_pNumParts = {};				// �p�[�c��
char			***CEnemy::m_pModelName = NULL;			// ���f����
CEnemy::DATA	**CEnemy::m_pFileData = NULL;			// �t�@�C���ǂݎ��
int				CEnemy::m_nNumType = 0;					// ��ނ̐�

// ���[�g�T���p
CScene3D *CEnemy::m_pScene3D[NODE_MAX] = {};
D3DXVECTOR3 CEnemy::m_searchPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
bool CEnemy::m_bPartSwitch = false;

//==================================
// ��������
//==================================
CEnemy* CEnemy::Create(D3DXVECTOR3 pos, int nType)
{
	CEnemy *pEnemy;

	pEnemy = new CEnemy;	// ���������m��

	if (pEnemy != NULL)
	{// �������m�ې���
		pEnemy->m_pos = pos;	// �ʒu
		pEnemy->m_nType = nType;	// ���
		pEnemy->Init();			// ����������
	}

	return pEnemy;
}

//=========================================
// ���f���̓ǂݍ���
//=========================================
HRESULT CEnemy::Load(void)
{
	// �f�o�C�X�̎擾
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	FILE *pFile;
	int nCntParts = 0;		// �p�[�c���̃J�E���^
	int nCntModel = 0;
	m_nNumType = 1;

	// ���f�������������m��
	m_pNumParts = new int[m_nNumType];
	m_pModelName = new char**[m_nNumType];
	m_pFileData = new DATA*[m_nNumType];

	for (int nCntType = 0; nCntType < m_nNumType; nCntType++)
	{
		// �t�@�C�����J��
		pFile = fopen("data/TEXT/enemy/model.txt", "r");

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

							m_pModelName[nCntType] = new char*[nNumModel];		// �������m��
						}
						if (strcmp(aStr, "MODEL_FILENAME") == 0)
						{// �L�����N�^�[���
							m_pModelName[nCntType][nCntModel] = new char[64];		// �������m��
							fscanf(pFile, " = %s", &m_pModelName[nCntType][nCntModel][0]);

							nCntModel++;
						}
						if (strcmp(aStr, "CHARACTERSET") == 0)
						{// �L�����N�^�[���
							while (strcmp(aStr, "END_CHARACTERSET") != 0)
							{// �L�����N�^�[���I�����b�Z�[�W�܂Ń��[�v
								fscanf(pFile, "%s", aStr);

								if (strcmp(aStr, "NUM_PARTS") == 0)
								{// �p�[�c��
									fscanf(pFile, " = %d", &m_pNumParts[nCntType]);
									m_pFileData[nCntType] = new DATA[m_pNumParts[nCntType]];
								}
								else if (strcmp(aStr, "PARTSSET") == 0)
								{// �p�[�c�̐ݒ�
									while (strcmp(aStr, "END_PARTSSET") != 0)
									{// �p�[�c�̐ݒ�I�����b�Z�[�W�܂Ń��[�v
										fscanf(pFile, "%s", &aStr);

										if (strcmp(aStr, "INDEX") == 0)
										{// ���f���ԍ�
											fscanf(pFile, " = %d", &m_pFileData[nCntType][nCntParts].nIdx);
										}
										else if (strcmp(aStr, "PARENT") == 0)
										{// �e�����߂�
											fscanf(pFile, " = %d", &m_pFileData[nCntType][nCntParts].nParent);
										}
										else if (strcmp(aStr, "POS") == 0)
										{// �ʒu�̐ݒ�
											fscanf(pFile, " = %f %f %f", &m_pFileData[nCntType][nCntParts].pos.x, &m_pFileData[nCntType][nCntParts].pos.y, &m_pFileData[nCntType][nCntParts].pos.z);
										}
										else if (strcmp(aStr, "ROT") == 0)
										{// �����̐ݒ�
											fscanf(pFile, " = %f %f %f", &m_pFileData[nCntType][nCntParts].rot.x, &m_pFileData[nCntType][nCntParts].rot.y, &m_pFileData[nCntType][nCntParts].rot.z);
										}
									}
									nCntParts++;	// �p�[�c���̃J�E���g
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
			MessageBox(0, "�e�L�X�g�t�@�C��������܂���B", "ENEMY/model.txt", MB_YESNO);
		}
	}

	return S_OK;
}

//=========================================
// �ǂݍ��񂾃f�[�^�̉��
//=========================================
void CEnemy::Unload(void)
{
	if (NULL != m_pFileData)
	{
		for (int nCntType = 0; nCntType < m_nNumType; nCntType++)
		{// ��ޕ����[�v
			if (NULL != m_pFileData[nCntType])
			{// �������̊J��
				delete[] m_pFileData[nCntType];
				m_pFileData[nCntType] = NULL;
			}
		}

		if (NULL != m_pFileData)
		{// �������̊J��
			delete[] m_pFileData;
			m_pFileData = NULL;
		}
	}

	if (NULL != m_pModelName)
	{// �p�[�c���̔j��
		for (int nCntType = 0; nCntType < m_nNumType; nCntType++)
		{// ��ޕ����[�v
			for (int nCntParts = 0; nCntParts < m_pNumParts[nCntType]; nCntParts++)
			{
				if (NULL != m_pModelName[nCntType][nCntParts])
				{// �j��
					delete[] m_pModelName[nCntType][nCntParts];
					m_pModelName[nCntType][nCntParts] = NULL;
				}
			}

			if (NULL != m_pModelName[nCntType])
			{// �j��
				delete[] m_pModelName[nCntType];
				m_pModelName[nCntType] = NULL;
			}
		}
		if (NULL != m_pModelName)
		{// �j��
			delete[] m_pModelName;
			m_pModelName = NULL;
		}
	}

	if (NULL != m_pNumParts)
	{// �p�[�c���̔j��
		delete[] m_pNumParts;
		m_pNumParts = NULL;
	}
}

//=========================================
// �R���X�g���N�^
//=========================================
CEnemy::CEnemy(int nPriority, CScene::OBJTYPE objType) : CScene(nPriority, objType)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nCntBullet = 0;
	m_pShadow = NULL;
	m_fAttackRange = 0.0f;
	m_fWalkRange = 0.0f;
	m_bAttack = false;
	m_nIntervalAttack = 0;
	m_fRotDest = 0.0f;
	m_pLife = NULL;
	m_fAlpha = 0.0f;

	for (int nCntDamage = 0; nCntDamage < ATTACK_MOTION; nCntDamage++)
	{// �U�����[�V���������[�v
		m_nCntDamage[nCntDamage] = 0;
		m_bDamage[nCntDamage] = false;
	}
}

//=========================================
// �f�X�g���N�^
//=========================================
CEnemy::~CEnemy(){}

//=========================================
// ����������
//=========================================
HRESULT CEnemy::Init(void)
{
	if (NULL == m_pModel)
	{// ���f���N���X�̐���
		m_pModel = new CModel*[m_pNumParts[m_nType]];	// �p�[�c���̃��������m��

		for (int nCntModel = 0; nCntModel < m_pNumParts[m_nType]; nCntModel++)
		{// �������̊m��
			m_pModel[nCntModel] = CModel::Create(&m_mtxWorld);

			// ���f�����̎󂯓n��
			m_pModel[nCntModel]->SetModel(m_pModelName[m_nType][nCntModel]);

			if (m_pFileData[m_nType][nCntModel].nParent >= 0)
			{// �e�̃|�C���^��ݒ�
				m_pModel[nCntModel]->SetParent(m_pModel[m_pFileData[m_nType][nCntModel].nParent]);
			}
			else
			{// �e�����Ȃ��ꍇ
				m_pModel[nCntModel]->SetParent(NULL);
			}

			m_pModel[nCntModel]->Init();
			m_pModel[nCntModel]->SetPos(m_pFileData[m_nType][nCntModel].pos);
			m_pModel[nCntModel]->SetRot(m_pFileData[m_nType][nCntModel].rot);
		}
	}

	if (NULL == m_pMotion)
	{// ���[�V�����N���X�̐���
		m_pMotion = CMotion::Create(1, m_pNumParts[m_nType], &m_pModel[m_nType]);
		m_pMotion->SetMotion(CMotionManager::TYPE_NEUTRAL);		// �j���[�g�������[�V������ݒ�
	}

	// �U������͈̔�
	m_fAttackRange = 60.0f;
	m_fWalkRange = 400.0f;

	// =============================================================
	// �_�C�N�X�g���@�ɂ�郋�[�g�T��
	// =============================================================
	FileLoad("data/TEXT/point.txt");	// �t�@�C���ǂݍ���

	// �f�[�^�̏�����
	m_nBreaktime = 0;
	m_nCountPoint = 0;
	m_nPoint = 0;
	m_bGoal = false;
	m_bRally = false;
	m_nRallyCount = 0;
	m_nRallyCountOld = 0;
	m_nNodeOld = 0;

	m_searchPos = m_pos;

	// �J�n���_�̃m�[�h�̏�����
	for (int nCntNode = 0; nCntNode < m_RootData.nodeMax; nCntNode++)
	{// �m�[�h�̐��������
		if (m_RootData.pos[nCntNode].x + MOUSE_ACCEPTABLE  > m_pos.x
			&& m_RootData.pos[nCntNode].x - MOUSE_ACCEPTABLE < m_pos.x
			&& m_RootData.pos[nCntNode].z + MOUSE_ACCEPTABLE > m_pos.z
			&& m_RootData.pos[nCntNode].z - MOUSE_ACCEPTABLE < m_pos.z)
		{// �덷��+-10�ȓ�
			m_nStartNode = nCntNode;
			m_nEndNode = nCntNode;
			m_nRallyEndNode[nCntNode] = nCntNode;
		}

		// �n�_�n�̏�����
		m_waypoint[nCntNode] = m_RootData.pos[m_nStartNode];
	}


	// �n�_�����p
	for (int nCntNode = 0; nCntNode < m_RootData.nodeMax; nCntNode++)
	{// �S�m�[�h�̐��������
		if (m_pScene3D[nCntNode] == NULL)
		{// 3D�|���S���̐���
			m_pScene3D[nCntNode] = m_pScene3D[nCntNode]->Create();
			if (m_pScene3D != NULL)
			{// 3D�|���S���̐ݒ�
				m_RootData.pos[nCntNode].y += 0.1f;
				m_pScene3D[nCntNode]->SetPos(m_RootData.pos[nCntNode]);
				m_pScene3D[nCntNode]->SetWidth(POINT_SIZE);
				m_pScene3D[nCntNode]->SetDepth(POINT_SIZE);
				m_pScene3D[nCntNode]->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			}
		}
	}

	// =============================================================

	return S_OK;
}

//=========================================
// �I������
//=========================================
void CEnemy::Uninit(void)
{
	if (m_pScene3D != NULL)
	{
		for (int nCntNode = 0; nCntNode < NODE_MAX; nCntNode++)
		{
			if (m_pScene3D[nCntNode] != NULL)
			{
				m_pScene3D[nCntNode]->Uninit();
				m_pScene3D[nCntNode] = NULL;
			}
		}
	}

	if (m_pShadow != NULL)
	{// �e�̔j��
		m_pShadow->Uninit();
		m_pShadow = NULL;
	}

	if (NULL != m_pLife)
	{// �̗̓N���X�̔j��
		m_pLife->Uninit();
		m_pLife = NULL;
	}

	if (m_pMotion != NULL)
	{// ���[�V�����N���X�̔j��
		m_pMotion->Uninit();
		m_pMotion = NULL;
	}

	for (int nCntType = 0; nCntType < m_nNumType; nCntType++)
	{
		for (int nCntModel = 0; nCntModel < m_pNumParts[nCntType]; nCntModel++)
		{// ���f���̔j��
			if (NULL != m_pModel)
			{
				m_pModel[nCntModel]->Uninit();
				delete m_pModel[nCntModel];
				m_pModel[nCntModel] = NULL;
			}
		}
	}

	if (NULL != m_pModel)
	{// �������̊J��
		delete m_pModel;
		m_pModel = NULL;
	}

	// ������j��
	CScene::Release();
}

//=========================================
// �X�V����
//=========================================
void CEnemy::Update(void)
{
	CScene *pScene = NULL;	// �I�u�W�F�N�g�Ǘ��N���X�̃|�C���^�ϐ�

	AIUpdate();

	// �ڕW�̕����̏C��
	if (m_fRotDest < -D3DX_PI)
	{
		m_fRotDest += D3DX_PI * 2.0f;
	}
	if (m_fRotDest > D3DX_PI)
	{
		m_fRotDest -= D3DX_PI * 2.0f;
	}

	m_pos += m_move;
	m_rot.y += m_fRotDest * 0.1f;

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
	m_fRotDest = 0.0f;

	// �d��
	m_move.y -= GRAVITY;

	float fHeight = 0.0f;	// �N���̍��������߂�

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
// �`�揈��
//=========================================
void CEnemy::Draw(void)
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

	for (int nCntModel = 0; nCntModel < m_pNumParts[0]; nCntModel++)
	{// �p�[�c���f���̕`��
		if (m_bDeath)
		{// ���S���
			m_pModel[nCntModel]->AddColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, -m_fAlpha));
		}
		for (int nCntDamage = 0; nCntDamage < ATTACK_MOTION; nCntDamage++)
		{// �U�����[�V���������[�v
			if (m_bDamage[nCntDamage])
			{// �_���[�W���
				m_pModel[nCntModel]->AddColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.0f));
				break;
			}
		}

		// ���f���̕`��
		m_pModel[nCntModel]->Draw();

		if (m_bDeath)
		{// ���S���
			m_pModel[nCntModel]->AddColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, m_fAlpha));
		}
		for (int nCntDamage = 0; nCntDamage < ATTACK_MOTION; nCntDamage++)
		{// �U�����[�V���������[�v
			if (m_bDamage[nCntDamage])
			{// �_���[�W���
				m_pModel[nCntModel]->AddColor(D3DXCOLOR(-1.0f, 0.0f, 0.0f, 0.0f));
				break;
			}
		}
	}
}

//=========================================
// �����蔻��
//=========================================
bool CEnemy::Collision(D3DXVECTOR3 pos, float fRange)
{
	bool bHit = false;

	// �����XY�͈̔�
	float fLength = powf(fRange, 2);

	float fLengthX = pos.x - m_pos.x;						// X�̍�
	float fLengthY = pos.y - m_pos.y;						// Y�̍�
	float fLengthTotal = powf(fLengthX, 2) + powf(fLengthY, 2);		// XY�̍��̓��

	if (fLength >= fLengthTotal)
	{// ����Ǝ����̔���
		if (fLengthX > 0)
		{// �E���ɂ���
			m_pos.x += fLengthX - fRange;
		}
		else if(fLengthX < 0)
		{// �����ɂ���
			m_pos.x -= fabsf(fLengthX) - fRange;
		}
		bHit = true;	// �����L���ɂ���
	}

	return bHit;	// ���茋�ʂ�Ԃ�
}

//=========================================
// ���f�����̎󂯓n��
//=========================================
void CEnemy::GetModelData(CModel**& pModel)
{
	m_pModel = pModel;
}

//=========================================
// �_���[�W���󂯂��Ƃ��̏���
//=========================================
void CEnemy::Damage(int nDamage, int nMotion)
{
	if (0 < m_pLife->GetLife())
	{
		if (nMotion < ATTACK_MOTION && nMotion >= 0)
		{// �z����z���Ȃ��悤�ɂ���
			if (NULL != m_pLife && !m_bDamage[nMotion])
			{// ���C�t�N���X���g���Ă���
				m_pLife->AddSubtract(nDamage);

				// �_���[�W��Ԃɂ���
				m_pMotion->SetMotion(CMotionManager::TYPE_DAMAGE);
				m_bDamage[nMotion] = true;
				CSound *pSound = CManager::GetSound();			// �T�E���h�̎擾
				//pSound->PlaySound(CSound::SOUND_LABEL_HIT);		// �U���q�b�g���̍Đ�

				if (0 >= m_pLife->GetLife())
				{// �̗͂�0�ȉ��ɂȂ���
					m_bDeath = true;
					m_nCntDamage[nMotion] = 0;
					//pSound->PlaySound(CSound::SOUND_LABEL_ENEMYDEATH);	// �G���S���̍Đ�
					m_move.y = 10.0f;

					if (m_pMotion)
					{// ���S���[�V����
						m_pMotion->SetMotion(CMotionManager::TYPE_DEATH);
					}
				}
			}
		}
		else if (-1 == nMotion)
		{// ���[�V������-1�̂Ƃ�
			CSound *pSound = CManager::GetSound();			// �T�E���h�̎擾
			//pSound->PlaySound(CSound::SOUND_LABEL_HIT);		// �U���q�b�g���̍Đ�

			m_pLife->AddSubtract(nDamage);	// ���C�t��0�ɂ���

			if (0 >= m_pLife->GetLife())
			{// �̗͂�0�ȉ��ɂȂ���
				m_bDeath = true;
				//pSound->PlaySound(CSound::SOUND_LABEL_ENEMYDEATH);	// �G���S���̍Đ�

				m_move.y = 15.0f;

				if (m_pMotion)
				{// ���S���[�V����
					m_pMotion->SetMotion(CMotionManager::TYPE_DEATH);
				}
			}
		}
	}
}

//=============================================================================
//	AI�X�V����
//=============================================================================
void CEnemy::AIUpdate()
{
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();	// �L�[�{�[�h�̓��͂��擾
	CInputMouse *pMouse = CManager::GetInputMouse();	// �}�E�X�̓��͂��擾
	int nCntRally = 0;

	// �p�[�g���	�X�g���e�W�[�Ftrue	�A�N�V�����Ffalse
	//CDebugProc::Print(" ���݂̃p�[�g : %s\n\n", m_bPartSwitch ? "�X�g���e�W�[" : "�A�N�V����");

	// �����[���
	//CDebugProc::Print(" �����[��� : %s\n\n", m_bRally ? "ON" : "OFF");
	//CDebugProc::Print(" �N���b�N�� : %d\n", m_nRallyCount);

	// �m�[�h���
	//CDebugProc::Print(" �m�[�h�̐� : %d\n", m_RootData.nodeMax);
	//CDebugProc::Print(" �S�[���ɓ���%s�B\n\n", m_bGoal ? "���܂���" : "���Ă܂���");

	if (!m_bRally)
	{// �����[��Ԃ���Ȃ�
		//CDebugProc::Print(" �J�n�n�_ : %d\n", m_nStartNode);
		//CDebugProc::Print(" �I���n�_ : %d\n\n", m_nEndNode);
	}
	else
	{// �����[��
		//CDebugProc::Print(" �J�n�n�_ : %d\n", m_nRallyEndNode[0]);
		for (nCntRally = 1; nCntRally < m_nRallyCount; nCntRally++)
		{
			//CDebugProc::Print(" ���Ԓn�_[%d] : %d\n", nCntRally, m_nRallyEndNode[nCntRally]);
		}
		//CDebugProc::Print(" �I���n�_ : %d\n\n", m_nRallyEndNode[m_nRallyCount]);
	}

	// �G�b�W���
	//CDebugProc::Print(" �G�b�W�̐� : %d\n", m_RootData.edgeMax);

	// �G�l�~�[���
	//CDebugProc::Print("=======�G�l�~�[�̈ʒu========\n");
	//CDebugProc::Print("  x : %.1f\n  y : %.1f\n  z : %.1f\n", m_pos.x, m_pos.y, m_pos.z);
	//CDebugProc::Print("=============================\n\n");

	// �����ړ����
	//CDebugProc::Print("========�����ړ��֌W========\n");
	//CDebugProc::Print(" �����ړ��ڕW�n�_\n  x : %.1f\n  y : %.1f\n  z : %.1f\n", m_posDest.x, m_posDest.y, m_posDest.z);
	////CDebugProc::Print(" �x�e���� : %.1f\n", m_fBreaktime);
	//CDebugProc::Print(" �ڕW�܂ł̈ړ��� : %d\n", m_nCountPoint);
	//CDebugProc::Print(" ���݂̈ړ��� : %d\n", m_nPoint);
	//CDebugProc::Print("=============================\n\n");

	if (pMouse->GetTrigger(CInputMouse::DIMS_BUTTON_1))
	{// �E�N���b�N
		// Part�؂�ւ�
		m_bPartSwitch = m_bPartSwitch ? false : true;
		m_bGoal = false;
	}

	if (pMouse->GetTrigger(CInputMouse::DIMS_BUTTON_3))
	{// �}�E�X���^�[��
		// �����[��Ԃ̐؂�ւ�
		m_bRally = m_bRally ? false : true;
	}

	if (!m_bPartSwitch)
	{// �A�N�V�����p�[�g
		m_nRallyCount = 0;
		// �����ړ�����
		AutoMove();
	}
	else
	{// �X�g���e�W�[�p�[�g
		m_nPoint = 0;
		m_nCountPoint = -1;
		NodeSearch(false);

		if (!m_bRally)
		{// �����[��Ԃ���Ȃ�
			RootSearch();
		}
		else
		{// �����[��
			RallyRootSearch();
		}
	}

	if (m_bGoal == true)
	{// �ڕW�n�_������
		for (int nCntNode = 0; nCntNode < m_RootData.nodeMax; nCntNode++)
		{// �m�[�h�̐��������
			if (m_pScene3D != NULL)
			{// 3D�|���S���̐ݒ�
				m_pScene3D[nCntNode]->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			}
		}
	}
}

//=============================================================================
//	�m�[�h��������
//=============================================================================
void CEnemy::NodeSearch(bool node)
{
	CInputMouse *pMouse = CManager::GetInputMouse();	// �}�E�X�̓��͂��擾

	for (int nCntNode = 0; nCntNode < m_RootData.nodeMax; nCntNode++)
	{// �m�[�h�̐��������
		if (m_RootData.pos[nCntNode].x + MOUSE_ACCEPTABLE > m_searchPos.x
			&& m_RootData.pos[nCntNode].x - MOUSE_ACCEPTABLE < m_searchPos.x
			&& m_RootData.pos[nCntNode].z + MOUSE_ACCEPTABLE > m_searchPos.z
			&& m_RootData.pos[nCntNode].z - MOUSE_ACCEPTABLE < m_searchPos.z)
		{// �덷�����e�͈͓�
			if (m_pScene3D[nCntNode] != NULL)
			{// 3D�|���S���̐ݒ�
				if (m_nRallyCountOld == m_nRallyCount)
				{// �u������
					m_pScene3D[m_nNodeOld]->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
				}

				switch (m_nRallyCount)
				{// �ԍ����ƂɐF��ς���
				case 1:
					m_pScene3D[nCntNode]->SetColor(D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
					break;
				}
			}

			// �O��̏���ۑ�
			m_nNodeOld = nCntNode;
			m_nRallyCountOld = m_nRallyCount;

			if (node)
			{// �J�n�m�[�h��ݒ肷��
				m_nStartNode = nCntNode;
			}
			else if (!node)
			{// �I���m�[�h��ݒ肷��
				m_nRallyEndNode[m_nRallyCount] = nCntNode;

				if (m_nRallyCount > 0)
				{// 1��ȏ�N���b�N���Ă���
					m_nEndNode = nCntNode;
				}

				if (pMouse->GetTrigger(CInputMouse::DIMS_BUTTON_0))
				{// ���N���b�N��
					if (m_bRally)
					{// �����[��
						m_nRallyCount++;
					}
					else
					{// �����[��Ԃ���Ȃ�
						m_nRallyCount = 1;
					}
				}
			}
		}
	}
}

//=============================================================================
//	�����ړ�����
//=============================================================================
void CEnemy::AutoMove()
{
	CMotionManager::TYPE type = CMotionManager::TYPE_NEUTRAL;		// ���[�V�����̎��

	bool bMoveKey = false;	// �ړ��L�[�����t���O

	// �ڕW�n�_��ݒ�
	m_posDest = m_waypoint[m_nPoint];

	// �ړI�Ƃ̍������o��
	float fLength = (m_pos.x - m_posDest.x) * (m_pos.x - m_posDest.x) + (m_pos.z - m_posDest.z) * (m_pos.z - m_posDest.z);
	m_nBreaktime--;

	if (fLength > MOVE_ACCEPTABLE)
	{// ���������e�l���Ɏ��܂�܂ŖړI�n�Ɉړ�����
		bMoveKey = true;
		m_move.x = sinf(atan2f(m_posDest.x - m_pos.x, m_posDest.z - m_pos.z)) * ENEMY_SPEED;
		m_move.z = cosf(atan2f(m_posDest.x - m_pos.x, m_posDest.z - m_pos.z)) * ENEMY_SPEED;
		m_rot.y = atan2f(m_posDest.x - m_pos.x, m_posDest.z - m_pos.z) + D3DX_PI;
	}
	else if (m_nBreaktime < 0)
	{// �ړ���x�e
		bMoveKey = false;
		m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_nBreaktime = PLAYER_BREAKTIME;

		if (m_nCountPoint != 0 && m_nCountPoint == m_nPoint)
		{// �I���m�[�h�ɓ���
			m_bGoal = true;
		}
	}
	else if (m_nBreaktime == 0 && m_nCountPoint > m_nPoint)
	{// �x�e�I��
		m_nPoint++;
		if (m_nCountPoint == m_nPoint)
		{// �I�_�����̊J�n�_�ɂ���
			NodeSearch(true);
		}
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
//	���[�g��������
//=============================================================================
void CEnemy::RootSearch()
{
	Node node[NODE_MAX];	// �m�[�h�̏��
	float weight[NODE_MAX];	// �e�G�b�W�̃R�X�g
	int nCntWeight = 0;		// �R�X�g�̃J�E���^
	std::vector<int> path;	// �ŒZ�o�H�̏���ێ�����vector

	//======= �G�b�W�R�X�g�̎Z�o =========================================================================
	for (int nCntNode = 0; nCntNode < m_RootData.nodeMax; nCntNode++, nCntWeight++)
	{// �m�[�h�̐��������
		weight[nCntWeight] = sqrt((m_RootData.pos[m_nStartNode].x - m_RootData.pos[nCntNode].x) * (m_RootData.pos[m_nStartNode].x - m_RootData.pos[nCntNode].x) + (m_RootData.pos[m_nStartNode].z - m_RootData.pos[nCntNode].z) * (m_RootData.pos[m_nStartNode].z - m_RootData.pos[nCntNode].z));
	}

	//======= �R�X�g�\�̏o�� =========================================================================
	//CDebugProc::Print("========�ړ��R�X�g�\========\n");
	for (int nCntNode = 0; nCntNode < m_RootData.nodeMax; nCntNode++)
	{// �m�[�h�̐��������
		//CDebugProc::Print(" No.%d : %.1f\n", nCntNode, weight[nCntNode]);
	}
	//CDebugProc::Print("=============================\n\n");

	//======= �G�b�W�ǉ� =========================================================================
	for (int nCntNode = 0; nCntNode < m_RootData.nodeMax; nCntNode++)
	{// �m�[�h�̐��������
		for (int nCntConnect = 0; nCntConnect < m_RootData.connectNum[nCntNode]; nCntConnect++)
		{// �q�����Ă�m�[�h�̐��������
			AddEdge(nCntNode, m_RootData.connectIndex[nCntNode][nCntConnect], weight[nCntNode], node);
		}
	}

	//======= �ŒZ�o�H�𒲂ׂ� =========================================================================
	Dijkstra(m_RootData.nodeMax, m_nStartNode, m_nEndNode, node);

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
			m_nCountPoint = nCntNode;
		}
	}
	//CDebugProc::Print("�z�̏��ł��B\n\n");

	//======= �ڕW�n�_�̐ݒ� =========================================================================
	for (int nCntNodeMax = 0; nCntNodeMax < m_RootData.nodeMax; )
	{// �m�[�h�̐��������
		for (int nCntNode = path.size() - 1; nCntNode >= 0; nCntNode--, nCntNodeMax++)
		{
			m_waypoint[nCntNodeMax] = m_RootData.pos[path[nCntNode]];
		}
	}
}

//=============================================================================
//	�����[���[�g��������
//=============================================================================
void CEnemy::RallyRootSearch()
{
	float weight[NODE_MAX];	// �e�G�b�W�̃R�X�g
	int nCntWeight = 0;		// �R�X�g�̃J�E���^
	std::vector<int> path;	// �ŒZ�o�H�̏���ێ�����vector

	//======= �G�b�W�R�X�g�̎Z�o =========================================================================
	for (int nCntNode = 0; nCntNode < m_RootData.nodeMax; nCntNode++, nCntWeight++)
	{// �m�[�h�̐��������
		weight[nCntWeight] = sqrt(
			(m_RootData.pos[m_nRallyEndNode[m_nRallyCount - 1]].x - m_RootData.pos[nCntNode].x) *
			(m_RootData.pos[m_nRallyEndNode[m_nRallyCount - 1]].x - m_RootData.pos[nCntNode].x) +
			(m_RootData.pos[m_nRallyEndNode[m_nRallyCount - 1]].z - m_RootData.pos[nCntNode].z) *
			(m_RootData.pos[m_nRallyEndNode[m_nRallyCount - 1]].z - m_RootData.pos[nCntNode].z));
	}

	//======= �R�X�g�\�̏o�� =========================================================================
	//CDebugProc::Print("========�ړ��R�X�g�\========\n");
	for (int nCntNode = 0; nCntNode < m_RootData.nodeMax; nCntNode++)
	{// �m�[�h�̐��������
		//CDebugProc::Print(" No.%d : %.1f\n", nCntNode, weight[nCntNode]);
	}
	//CDebugProc::Print("=============================\n\n");

	//======= �G�b�W�ǉ� =========================================================================
	for (int nCntNode = 0; nCntNode < m_RootData.nodeMax; nCntNode++)
	{// �m�[�h�̐��������
		for (int nCntConnect = 0; nCntConnect < m_RootData.connectNum[nCntNode]; nCntConnect++)
		{// �q�����Ă�m�[�h�̐��������
			AddEdge(nCntNode, m_RootData.connectIndex[nCntNode][nCntConnect], weight[nCntNode], m_node[m_nRallyCount]);
		}
	}

	//======= �ŒZ�o�H�𒲂ׂ� =========================================================================
	if (m_nRallyCount > 1)
	{// 2�|�C���g�ڈȍ~
		for (int nCntRally = 1; nCntRally < m_nRallyCount; nCntRally++)
		{// 2�|�C���g�ȍ~�̐��������
			// �_�C�N�X�g���@�ōŒZ�o�H�����߂�
			Dijkstra(m_RootData.nodeMax, m_nRallyEndNode[m_nRallyCount - nCntRally], m_nRallyEndNode[m_nRallyCount - (nCntRally - 1)], m_node[m_nRallyCount - (nCntRally - 1)]);

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
		Dijkstra(m_RootData.nodeMax, m_nStartNode, m_nRallyEndNode[m_nRallyCount - (m_nRallyCount - 1)], m_node[m_nRallyCount - (m_nRallyCount - 1)]);

		for (int nCntNode = m_nRallyEndNode[m_nRallyCount - (m_nRallyCount - 1)]; nCntNode != m_nStartNode; nCntNode = m_node[m_nRallyCount - (m_nRallyCount - 1)][nCntNode].from)
		{// �ŒZ�o�H���S�[�����珇�ɃX�^�[�g�܂ł��ǂ�
			path.push_back(nCntNode);
		}
		path.push_back(m_nStartNode);
	}

	if (m_nRallyCount == 0)
	{// �X�^�[�g�ʒu
		//�_�C�N�X�g���@�ōŒZ�o�H�����߂�
		Dijkstra(m_RootData.nodeMax, m_nStartNode, m_nRallyEndNode[m_nRallyCount], m_node[m_nRallyCount]);

		for (int nCntNode = m_nRallyEndNode[m_nRallyCount]; nCntNode != m_nStartNode; nCntNode = m_node[m_nRallyCount][nCntNode].from)
		{// �ŒZ�o�H���S�[�����珇�ɃX�^�[�g�܂ł��ǂ�
			path.push_back(nCntNode);
		}
		path.push_back(m_nStartNode);
	}

	//======= �ŒZ�o�H�̏o�� =========================================================================
	//CDebugProc::Print(" �ŒZ�o�H�́y");
	for (int nCntNode = path.size() - 1; nCntNode >= 0; nCntNode--)
	{
		//CDebugProc::Print(" %d ", path[nCntNode]);
		if (m_nCountPoint < nCntNode)
		{// �ő�l����
			m_nCountPoint = nCntNode;
		}
	}
	//CDebugProc::Print("�z�̏��ł��B\n\n");

	//======= �ڕW�n�_�̐ݒ� =========================================================================
	for (int nCntNodeMax = 0; nCntNodeMax < m_RootData.nodeMax; )
	{// �m�[�h�̐��������
		for (int nCntNode = path.size() - 1; nCntNode >= 0; nCntNode--, nCntNodeMax++)
		{// �S�[������H��
			m_waypoint[nCntNodeMax] = m_RootData.pos[path[nCntNode]];
		}
	}
}

//=============================================================================
// �G�b�W�ǉ�����
//=============================================================================
void CEnemy::AddEdge(int first, int second, float weight, Node *node)
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
void CEnemy::Dijkstra(int nodeMax, int start, int end, Node *node)
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
void CEnemy::FileLoad(char* pFileName)
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
				else if (strcmp(HeadText, "NUM_NODE") == 0)	// �m�[�h�̑���
				{
					sscanf(ReadText, "%s %c %d",
						&DustBox, &DustBox,
						&OneState.nodeMax);
				}
				else if (strcmp(HeadText, "NUM_EDGE") == 0)	// �G�b�W�̑���
				{
					sscanf(ReadText, "%s %c %d",
						&DustBox, &DustBox,
						&OneState.edgeMax);
				}
				else if (strcmp(HeadText, "NODE_POS") == 0)	// �m�[�h�̈ʒu
				{
					sscanf(ReadText, "%s %c %f %f %f",
						&DustBox, &DustBox,
						&OneState.pos[nCount].x,
						&OneState.pos[nCount].y,
						&OneState.pos[nCount].z);
					nCount++;
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
								else if (strcmp(HeadText, "INDEX") == 0)	// �m�[�h�ԍ�
								{
									sscanf(ReadText, "%s %c %d",
										&DustBox, &DustBox,
										&OneState.index[nCount]);
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
							nCntIndex = 0;
							nCount++;
						}
					}

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

	m_RootData = OneState;	// �f�[�^�̑��
}