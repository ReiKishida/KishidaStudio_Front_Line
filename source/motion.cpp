//=============================================================================
//
// ���[�V�������� [motion.cpp]
// Author : Takuto Ishida
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "motion.h"
#include "renderer.h"
#include "manager.h"
#include "debugProc.h"
#include "model.h"
#include "meshOrbit.h"
#include "particle.h"
#include "loadText.h"

//==================================
// �}�N����`
//==================================

//==================================
// �ÓI�����o�ϐ��錾
//==================================
int **CMotionManager::m_pFrame[MAX_FILE] = {};								// �t���[��
CMotionManager::KEY ***CMotionManager::m_pKey[MAX_FILE] = {};				// �p�[�c���Ƃ̃L�[�v�f
bool *CMotionManager::m_pLoop[MAX_FILE] = {};								// ���[�v���邩�ǂ���
int *CMotionManager::m_pNumKey[MAX_FILE] = {};								// �L�[��
int *CMotionManager::m_pType[MAX_FILE] = {};								// ���[�V�����̎��
CMotionManager::COLLISION **CMotionManager::m_pCollision[MAX_FILE] = {};	// �����蔻��
CMotionManager::ORBIT **CMotionManager::m_pOrbit[MAX_FILE] = {};			// �O��
int CMotionManager::m_nParts[MAX_FILE] = {};								// �p�[�c��
int CMotionManager::m_nNumMotion[MAX_FILE] = {};							// ���[�V������

//=========================================
// ��������
//=========================================
CMotionManager *CMotionManager::Create(int nFile)
{
	// �������m��
	CMotionManager *pMotionManager = new CMotionManager;

	if (pMotionManager != NULL)
	{// �������m�ې���
		pMotionManager->m_nFile = nFile;
		pMotionManager->Init();
	}

	return pMotionManager;
}

//=========================================
// ���f���̓ǂݍ���
//=========================================
HRESULT CMotionManager::Load(void)
{
	FILE *pFile;
	int nCntMotion = 0;
	int nCntKey = 0;
	int nCntParts = 0;
	int nNumParts;
	int nCntFile = 0;
	char *pFileName[MAX_FILE] =
	{
		{ "data/TEXT/PLAYER/assult/motion_assult.txt" },
		{ "data/TEXT/PLAYER/light/motion_light.txt" },
		{ "data/TEXT/PLAYER/heavy/motion_heavy.txt" },
		{ "data/TEXT/PLAYER/snipe/motion_snipe.txt" },

		{ "data/TEXT/AI/drone/motion_drone.txt" },

		{ "data/TEXT/ENEMY/motion.txt" },
	};

	for (nCntFile = 0; nCntFile < MAX_FILE; nCntFile++)
	{
		// �t�@�C�����J��
		pFile = fopen(pFileName[nCntFile], "r");

		if (NULL != pFile)
		{// �t�@�C����������
			char *pStrCur;		// ������̐擪�ւ̃|�C���^
			char aLine[256];	// ������ǂݍ��ݗp
			char aStr[256];		// �����񔲂��o���p

			strcpy(aStr, "");

			while (memcmp(aStr, "END_SCRIPT", strlen("END_SCRIPT")) != 0)
			{
				pStrCur = CLoadText::ReadLine(pFile, &aLine[0]);	// ������̐擪��ǂݍ���
				strcpy(aStr, pStrCur);					// ����������o��

				if (memcmp(aStr, "NUM_MODEL = ", strlen("NUM_MODEL = ")) == 0)
				{// �p�[�c��
					pStrCur += strlen("NUM_MODEL = ");	// ���o��
					strcpy(aStr, pStrCur);				// ������̎��o��

					nNumParts = atoi(aStr);
				}
				else if (memcmp(aStr, "NUM_MOTION = ", strlen("NUM_MOTION = ")) == 0)
				{// ���[�V������
					pStrCur += strlen("NUM_MOTION = ");	// ���o��
					strcpy(aStr, pStrCur);				// ������̎��o��

					m_nNumMotion[nCntFile] = atoi(aStr);

					// �������m��
					m_pNumKey[nCntFile] = new int[m_nNumMotion[nCntFile]];
					m_pLoop[nCntFile] = new bool[m_nNumMotion[nCntFile]];
					m_pType[nCntFile] = new int[m_nNumMotion[nCntFile]];
					m_pFrame[nCntFile] = new int*[m_nNumMotion[nCntFile]];
					m_pKey[nCntFile] = new KEY**[m_nNumMotion[nCntFile]];
					m_pCollision[nCntFile] = new COLLISION*[m_nNumMotion[nCntFile]];
					m_pOrbit[nCntFile] = new ORBIT*[m_nNumMotion[nCntFile]];
				}
				else if (memcmp(aStr, "MOTIONSET", strlen("MOTIONSET")) == 0)
				{// �L�����N�^�[���̐ݒ�
					m_pCollision[nCntFile][nCntMotion] = NULL;
					m_pOrbit[nCntFile][nCntMotion] = NULL;

					while (memcmp(aStr, "END_MOTIONSET", strlen("END_MOTIONSET")) != 0)
					{// �I�����b�Z�[�W�܂Ń��[�v
						pStrCur = CLoadText::ReadLine(pFile, &aLine[0]);	// ������̐擪��ǂݍ���
						strcpy(aStr, pStrCur);					// ����������o��

						if (memcmp(aStr, "LOOP = ", strlen("LOOP = ")) == 0)
						{// ���[�v�̗L��
							pStrCur += strlen("LOOP = ");			// ���o��
							strcpy(aStr, pStrCur);					// ������̎��o��

							if (atoi(aStr) != 0)
							{// ���̂Ƃ�
								m_pLoop[nCntFile][nCntMotion] = true;
							}
							else
							{// ���̂Ƃ�
								m_pLoop[nCntFile][nCntMotion] = false;
							}
						}
						else if (memcmp(aStr, "NUM_KEY = ", strlen("NUM_KEY = ")) == 0)
						{// �L�[��
							pStrCur += strlen("NUM_KEY = ");	// ���o��
							strcpy(aStr, pStrCur);				// ������̎��o��

							m_pNumKey[nCntFile][nCntMotion] = atoi(aStr);	// �����^�ɒ���

							m_pFrame[nCntFile][nCntMotion] = new int[m_pNumKey[nCntFile][nCntMotion]];	// �K�v�ȃt���[�����������m��
							m_pKey[nCntFile][nCntMotion] = new KEY*[m_pNumKey[nCntFile][nCntMotion]];	// �L�[�����������m��
						}
						else if (memcmp(aStr, "COLLISION = ", strlen("COLLISION = ")) == 0)
						{// �����蔻��
							pStrCur += strlen("COLLISION = ");	// ���o��
							strcpy(aStr, pStrCur);				// ������̎��o��

							// �������m��
							m_pCollision[nCntFile][nCntMotion] = new COLLISION;

							// ���f���ԍ�
							pStrCur += CLoadText::PopString(&pStrCur[0], &aStr[0]);
							m_pCollision[nCntFile][nCntMotion]->nIdxModel = atoi(&aStr[0]);	// �����^�ɒ���

							// �w��
							pStrCur += CLoadText::PopString(&pStrCur[0], &aStr[0]);
							m_pCollision[nCntFile][nCntMotion]->fPosX = (float)atof(&aStr[0]);	// �����^�ɒ���

							// �x��
							pStrCur += CLoadText::PopString(&pStrCur[0], &aStr[0]);
							m_pCollision[nCntFile][nCntMotion]->fPosY = (float)atof(&aStr[0]);	// �����^�ɒ���

							// �y��
							pStrCur += CLoadText::PopString(&pStrCur[0], &aStr[0]);
							m_pCollision[nCntFile][nCntMotion]->fPosZ = (float)atof(&aStr[0]);	// �����^�ɒ���

							// ���a
							pStrCur += CLoadText::PopString(&pStrCur[0], &aStr[0]);
							m_pCollision[nCntFile][nCntMotion]->fRadius = (float)atof(&aStr[0]);	// �����^�ɒ���

							// �J�n�t���[��
							pStrCur += CLoadText::PopString(&pStrCur[0], &aStr[0]);
							m_pCollision[nCntFile][nCntMotion]->nStartFrame = atoi(&aStr[0]);	// �����^�ɒ���

							// �I���t���[��
							pStrCur += CLoadText::PopString(&pStrCur[0], &aStr[0]);
							m_pCollision[nCntFile][nCntMotion]->nEndFrame = atoi(&aStr[0]);	// �����^�ɒ���
						}
						else if (memcmp(aStr, "ORBIT = ", strlen("ORBIT = ")) == 0)
						{// �O��
							pStrCur += strlen("ORBIT = ");		// ���o��
							strcpy(aStr, pStrCur);				// ������̎��o��

							// �������m��
							m_pOrbit[nCntFile][nCntMotion] = new ORBIT;

							if (NULL != m_pOrbit[nCntFile][nCntMotion])
							{
								// ���f���ԍ�
								pStrCur += CLoadText::PopString(&pStrCur[0], &aStr[0]);
								m_pOrbit[nCntFile][nCntMotion]->nIdxModel = atoi(&aStr[0]);		// �����^�ɒ���

								// �w��
								pStrCur += CLoadText::PopString(&pStrCur[0], &aStr[0]);
								m_pOrbit[nCntFile][nCntMotion]->fPosX = (float)atof(&aStr[0]);	// �����^�ɒ���

								// �x��
								pStrCur += CLoadText::PopString(&pStrCur[0], &aStr[0]);
								m_pOrbit[nCntFile][nCntMotion]->fPosY = (float)atof(&aStr[0]);	// �����^�ɒ���

								// �y��
								pStrCur += CLoadText::PopString(&pStrCur[0], &aStr[0]);
								m_pOrbit[nCntFile][nCntMotion]->fPosZ = (float)atof(&aStr[0]);	// �����^�ɒ���

								// �J�n�t���[��
								pStrCur += CLoadText::PopString(&pStrCur[0], &aStr[0]);
								m_pOrbit[nCntFile][nCntMotion]->nStartFrame = atoi(&aStr[0]);	// �����^�ɒ���

								// �I���t���[��
								pStrCur += CLoadText::PopString(&pStrCur[0], &aStr[0]);
								m_pOrbit[nCntFile][nCntMotion]->nEndFrame = atoi(&aStr[0]);		// �����^�ɒ���
							}
						}
						else if (memcmp(aStr, "KEYSET", strlen("KEYSET")) == 0)
						{// �p�[�c�̐ݒ�
							m_pKey[nCntFile][nCntMotion][nCntKey] = new KEY[nNumParts];

							while (memcmp(aStr, "END_KEYSET", strlen("END_KEYSET")) != 0)
							{// �I�����b�Z�[�W�܂Ń��[�v
								pStrCur = CLoadText::ReadLine(pFile, &aLine[0]);			// ������̐擪��ǂݍ���
								strcpy(aStr, pStrCur);		// ����������o��

								if (memcmp(aStr, "FRAME = ", strlen("FRAME = ")) == 0)
								{// �t���[����
									pStrCur += strlen("FRAME = ");				// ���o��

									strcpy(aStr, pStrCur);						// ����������o��
									m_pFrame[nCntFile][nCntMotion][nCntKey] = atoi(aStr);	// �����^�ɒ���
								}
								else if (memcmp(aStr, "KEY", strlen("KEY")) == 0)
								{// �L�[���
									while (memcmp(aStr, "END_KEY", strlen("END_KEY")) != 0)
									{
										pStrCur = CLoadText::ReadLine(pFile, &aLine[0]);			// ������̐擪��ǂݍ���
										strcpy(aStr, pStrCur);		// ����������o��

										if (memcmp(aStr, "POS = ", strlen("POS = ")) == 0)
										{// �ʒu
											pStrCur += strlen("POS = ");			// ���o��

											// �w��
											pStrCur += CLoadText::PopString(&pStrCur[0], &aStr[0]);
											m_pKey[nCntFile][nCntMotion][nCntKey][nCntParts].posX = (float)atof(&aStr[0]);	// �����^�ɒ���

											// �x��
											pStrCur += CLoadText::PopString(&pStrCur[0], &aStr[0]);
											m_pKey[nCntFile][nCntMotion][nCntKey][nCntParts].posY = (float)atof(&aStr[0]);	// �����^�ɒ���

											// �y��
											pStrCur += CLoadText::PopString(&pStrCur[0], &aStr[0]);
											m_pKey[nCntFile][nCntMotion][nCntKey][nCntParts].posZ = (float)atof(&aStr[0]);	// �����^�ɒ���
										}
										else if (memcmp(aStr, "ROT = ", strlen("ROT = ")) == 0)
										{// ����
											pStrCur += strlen("ROT = ");			// ���o��

											// �w��
											pStrCur += CLoadText::PopString(&pStrCur[0], &aStr[0]);
											m_pKey[nCntFile][nCntMotion][nCntKey][nCntParts].rotX = (float)atof(&aStr[0]);	// �����^�ɒ���

											// �x��
											pStrCur += CLoadText::PopString(&pStrCur[0], &aStr[0]);
											m_pKey[nCntFile][nCntMotion][nCntKey][nCntParts].rotY = (float)atof(&aStr[0]);	// �����^�ɒ���

											// �y��
											pStrCur += CLoadText::PopString(&pStrCur[0], &aStr[0]);
											m_pKey[nCntFile][nCntMotion][nCntKey][nCntParts].rotZ = (float)atof(&aStr[0]);	// �����^�ɒ���
										}
									}
									nCntParts++;	// �p�[�c���̃J�E���g
								}
							}
							nCntKey++;	// �L�[�̃J�E���g
							nCntParts = 0;
						}
					}
					m_pType[nCntFile][nCntMotion] = nCntMotion;
					nCntMotion++;	// ���[�V�������̃J�E���g
					nCntKey = 0;
					nCntParts = 0;

					if (m_nNumMotion[nCntFile] <= nCntMotion)
					{// ���[�V���������������甲����
						break;
					}
				}
			}
			fclose(pFile);	// �t�@�C�������
			nCntMotion = 0;	// ���[�V�������̃J�E���g
			nCntKey = 0;
			nCntParts = 0;
		}
		else
		{// �t�@�C�����Ȃ��Ƃ�
			MessageBox(0, "�t�@�C��������܂���B", "motion.txt", MB_YESNO);
		}
	}

	return S_OK;
}

//=========================================
// �ǂݍ��񂾏��̉��
//=========================================
void CMotionManager::Unload(void)
{
	for (int nCntFile = 0; nCntFile < MAX_FILE; nCntFile++)
	{
		for (int nCntMotion = 0; nCntMotion < m_nNumMotion[nCntFile]; nCntMotion++)
		{
			if (NULL != m_pKey[nCntFile])
			{// �L�[���̔j��
				for (int nCnt = 0; nCnt < m_pNumKey[nCntFile][nCntMotion]; nCnt++)
				{
					if (NULL != m_pKey[nCntFile][nCntMotion][nCnt])
					{
						delete m_pKey[nCntFile][nCntMotion][nCnt];
						m_pKey[nCntFile][nCntMotion][nCnt] = NULL;
					}
				}

				if (NULL != m_pKey[nCntFile][nCntMotion])
				{
					delete m_pKey[nCntFile][nCntMotion];
					m_pKey[nCntFile][nCntMotion] = NULL;
				}
			}

			if (NULL != m_pFrame[nCntFile])
			{// �L�[�̔j��
				delete m_pFrame[nCntFile][nCntMotion];
				m_pFrame[nCntFile][nCntMotion] = NULL;
			}

			if (NULL != m_pCollision[nCntFile])
			{// �����蔻��̔j��
				delete m_pCollision[nCntFile][nCntMotion];
				m_pCollision[nCntFile][nCntMotion] = NULL;
			}

			if (NULL != m_pOrbit[nCntFile])
			{// �O�Ղ̔j��
				delete m_pOrbit[nCntFile][nCntMotion];
				m_pOrbit[nCntFile][nCntMotion] = NULL;
			}
		}

		if (NULL != m_pCollision[nCntFile])
		{// �����蔻��̔j��
			delete m_pCollision[nCntFile];
			m_pCollision[nCntFile] = NULL;
		}

		if (NULL != m_pOrbit[nCntFile])
		{// �O�Ղ̔j��
			delete m_pOrbit[nCntFile];
			m_pOrbit[nCntFile] = NULL;
		}

		if (NULL != m_pLoop[nCntFile])
		{// ���[�v���̔j��
			delete m_pLoop[nCntFile];
			m_pLoop[nCntFile] = NULL;
		}

		if (NULL != m_pNumKey[nCntFile])
		{// �L�[���̔j��
			delete m_pNumKey[nCntFile];
			m_pNumKey[nCntFile] = NULL;
		}

		if (NULL != m_pKey[nCntFile])
		{// �L�[�̔j��
			delete m_pKey[nCntFile];
			m_pKey[nCntFile] = NULL;
		}

		if (NULL != m_pFrame[nCntFile])
		{// �t���[���̔j��
			delete m_pFrame[nCntFile];
			m_pFrame[nCntFile] = NULL;
		}

		if (NULL != m_pType[nCntFile])
		{// ��ނ̔j��
			delete m_pType[nCntFile];
			m_pType[nCntFile] = NULL;
		}
	}
}

//=========================================
// �R���X�g���N�^
//=========================================
CMotionManager::CMotionManager(int nPriority, CScene::OBJTYPE objType) : CScene(nPriority, objType)
{
	m_nFile = 0;
}

//=========================================
// �f�X�g���N�^
//=========================================
CMotionManager::~CMotionManager()
{
}

//=========================================
// ����������
//=========================================
HRESULT CMotionManager::Init()
{
	return S_OK;
}

//=========================================
// �I������
//=========================================
void CMotionManager::Uninit(void)
{
	// ������j��
	CScene::Release();
}

//=========================================
// �X�V����
//=========================================
void CMotionManager::Update(void)
{
}

//=========================================
// �`�揈��
//=========================================
void CMotionManager::Draw(void)
{
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
// ���[�V�����Đ��N���X
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//=========================================
// ���[�V�����̍Đ�
//=========================================
void CMotion::SetMotion(CMotionManager::TYPE type)
{
	if (m_nCurMotion != type && m_pMotionManager->GetNumMotion() > type)
	{
		int nNumMotion = m_pMotionManager->GetNumMotion();
		for (int nCntMotion = 0; nCntMotion < nNumMotion; nCntMotion++)
		{// ���[�V�����������[�v
			if (m_pMotionManager->GetType(nCntMotion) == type)
			{// ���[�V�����̎�ނ���v
				m_nType = type;												// ���[�V�����̎�ނ��擾
				m_bLoop = m_pMotionManager->GetLoop(nCntMotion);			// ���[�v�̗L�����擾
				m_nKey = m_pMotionManager->GetNumKey(nCntMotion);			// �L�[�����擾
				m_pKey = m_pMotionManager->GetKey(nCntMotion);				// �L�[�����擾
				m_pFrame = m_pMotionManager->GetFlame(nCntMotion);			// �t���[�������擾
				m_pCollision = m_pMotionManager->GetCollision(nCntMotion);	// �����蔻��
				m_pOrbit = m_pMotionManager->GetOrbit(nCntMotion);			// �O��
				m_nCurMotion = type;										// ���[�V�����ԍ����L��

				m_nCurKey = 0;									// ���݂̃L�[�����Z�b�g
				m_nCntFlame = 0;								// �t���[���������Z�b�g
				m_nCntTotalFlame = 0;
				m_bInit = false;

				break;
			}
		}
	}
}

//=========================================
// ���[�V�����N���X�̐���
//=========================================
CMotion *CMotion::Create(int nFile, int nParts, CModel **pModel, int nFootIdx)
{
	CMotion *pMotion = NULL;

	if (pMotion == NULL)
	{// ��̂Ƃ�
		pMotion = new CMotion;

		if (pMotion != NULL)
		{// �������m�ې���
			pMotion->m_nParts = nParts;			// �p�[�c��
			pMotion->m_pModel = pModel;			// ���f���N���X�̃|�C���^�ϐ�
			pMotion->m_nFootIdx = nFootIdx;		// �����g�ȉ��̔ԍ�
			pMotion->m_pMotionManager = CMotionManager::Create(nFile);	// ���[�V�����Ǘ��N���X�̐���
			pMotion->Init();
		}
	}

	return pMotion;
}

//=========================================
// �R���X�g���N�^
//=========================================
CMotion::CMotion(int nPriority, CScene::OBJTYPE objType) : CScene(nPriority, objType)
{
	m_pFrame = NULL;
	m_pKey = NULL;
	m_posInit = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_bLoop = false;
	m_nCntFlame = 0;
	m_nCurKey = 0;
	m_nKey = 0;
	m_nParts = 0;
	m_pModel = NULL;
	m_nCurMotion = 0;
	m_nType = CMotionManager::TYPE_NEUTRAL;
	m_pMotionManager = NULL;
	m_nCntTotalFlame = 0;	// �����t���[����
	m_bInit = true;
	m_pOrbit = NULL;
	m_pCollision = NULL;
	m_nFootIdx = 0;
}

//=========================================
// �f�X�g���N�^
//=========================================
CMotion::~CMotion()
{
}

//=========================================
// ����������
//=========================================
HRESULT CMotion::Init()
{
	m_posInit = m_pModel[0]->GetPos();				// ���S���f���̏����ʒu�̎擾
	m_nCurMotion = -1;								// ���݂̃��[�V�������w�肵�Ȃ�
	SetMotion(CMotionManager::TYPE_NEUTRAL);		// �j���[�g�������[�V������ݒ�
	return S_OK;
}

//=========================================
// �I������
//=========================================
void CMotion::Uninit(void)
{
	if (m_pMotionManager != NULL)
	{// ���[�V�����Ǘ���j��
		m_pMotionManager->Uninit();
	}

	// ���S�t���O�𗧂Ă�
	CScene::Release();
}

//=========================================
// �X�V����
//=========================================
void CMotion::Update(void)
{
	// ���[�V����
	Motion();
}

//=========================================
// �`�揈��
//=========================================
void CMotion::Draw(void)
{
}

//=========================================
// ���[�V����
//=========================================
void CMotion::Motion(void)
{
	CMotionManager::KEY *pKey;
	float fRateMotion;
	float fDiffMotion;
	D3DXVECTOR3 rotModel;		// �v�Z�p����
	D3DXVECTOR3 posModel;		// �v�Z�p�ʒu

	if (!m_bInit)
	{// ���̃��[�V�����ւȂ���
		m_nCntFlame = m_pFrame[m_nCurKey] / 2;
		m_bInit = true;
	}

	//�����̌v�Z
	fRateMotion = (m_pFrame[m_nCurKey] - m_nCntFlame) * 1.0f;

	//�A�j���[�V����
	for (int nCntParts = m_nFootIdx; nCntParts < m_nParts; nCntParts++)
	{
		if (m_pModel[nCntParts] != NULL)
		{// ���f�����g�p����Ă���
			pKey = &m_pKey[m_nCurKey][nCntParts];

			// ���f���̌����̎擾
			rotModel = m_pModel[nCntParts]->GetRot();

			/*----------------------------------------------*/
			/*						X��						*/
			/*----------------------------------------------*/
			// �p�x�̒���
			if (pKey->rotX > D3DX_PI) { pKey->rotX -= D3DX_PI * 2.0f; }
			if (pKey->rotX < -D3DX_PI) { pKey->rotX += D3DX_PI * 2.0f; }

			// ���������߂�
			fDiffMotion = pKey->rotX - rotModel.x;

			// �����̒���
			if (fDiffMotion > D3DX_PI) { fDiffMotion -= D3DX_PI * 2.0f; }
			if (fDiffMotion < -D3DX_PI) { fDiffMotion += D3DX_PI * 2.0f; }

			// ���݂̌����̐ݒ�
			rotModel.x += fDiffMotion / fRateMotion;

			// X�ړI�̈ʒu�̒���
			if (rotModel.x > D3DX_PI) { rotModel.x -= D3DX_PI * 2.0f; }
			if (rotModel.x < -D3DX_PI) { rotModel.x += D3DX_PI * 2.0f; }

			/*----------------------------------------------*/
			/*						Y��						*/
			/*----------------------------------------------*/
			// �p�x�̒���
			if (pKey->rotY > D3DX_PI) { pKey->rotY -= D3DX_PI * 2.0f; }
			if (pKey->rotY < -D3DX_PI) { pKey->rotY += D3DX_PI * 2.0f; }

			// ���������߂�
			fDiffMotion = pKey->rotY - rotModel.y;

			// �����̒���
			if (fDiffMotion > D3DX_PI) { fDiffMotion -= D3DX_PI * 2.0f; }
			if (fDiffMotion < -D3DX_PI) { fDiffMotion += D3DX_PI * 2.0f; }

			// ���݂̌����̐ݒ�
			rotModel.y += fDiffMotion / fRateMotion;

			// Y�ړI�̈ʒu�̒���
			if (rotModel.y > D3DX_PI) { rotModel.y -= D3DX_PI * 2.0f; }
			if (rotModel.y < -D3DX_PI) { rotModel.y += D3DX_PI * 2.0f; }

			/*----------------------------------------------*/
			/*						Z��						*/
			/*----------------------------------------------*/
			// �p�x�̒���
			if (pKey->rotZ > D3DX_PI) { pKey->rotZ -= D3DX_PI * 2.0f; }
			if (pKey->rotZ < -D3DX_PI) { pKey->rotZ += D3DX_PI * 2.0f; }

			// ���������߂�
			fDiffMotion = pKey->rotZ - rotModel.z;

			// �����̒���
			if (fDiffMotion > D3DX_PI) { fDiffMotion -= D3DX_PI * 2.0f; }
			if (fDiffMotion < -D3DX_PI) { fDiffMotion += D3DX_PI * 2.0f; }

			// ���݂̌����̐ݒ�
			rotModel.z += fDiffMotion / fRateMotion;

			// Z�ړI�̈ʒu�̒���
			if (rotModel.z > D3DX_PI) { rotModel.z -= D3DX_PI * 2.0f; }
			else if (rotModel.z < -D3DX_PI) { rotModel.z += D3DX_PI * 2.0f; }

			// �����̐ݒ�
			m_pModel[nCntParts]->SetRot(rotModel);
		}
	}

	/*================================================*/
	/*						�ʒu					  */
	/*================================================*/
	posModel = m_pModel[0]->GetPos();
	pKey = &m_pKey[m_nCurKey][0];

	/*		X��		*/
	fDiffMotion = pKey->posX - posModel.x + m_posInit.x;	// ���������߂�
	posModel.x += fDiffMotion / fRateMotion;				// ���݂̌����̐ݒ�

	/*		Y��		*/
	fDiffMotion = pKey->posY - posModel.y + m_posInit.y;	// ���������߂�
	posModel.y += fDiffMotion / fRateMotion;				// ���݂̌����̐ݒ�

	/*		Z��		*/
	fDiffMotion = pKey->posZ - posModel.z + m_posInit.z;	// ���������߂�
	posModel.z += fDiffMotion / fRateMotion;				// ���݂̌����̐ݒ�

	// �ʒu�̐ݒ�
	m_pModel[0]->SetPos(posModel);

	// �O��
	if (NULL != m_pOrbit && m_nCntTotalFlame == m_pOrbit->nStartFrame)
	{// �g���Ă���Ƃ� && �J�n�t���[��
		CMeshOrbit::Create(D3DXVECTOR3(m_pOrbit->fPosX, m_pOrbit->fPosY, m_pOrbit->fPosZ), m_pOrbit->nEndFrame - m_pOrbit->nStartFrame, &m_pModel[m_pOrbit->nIdxModel]->GetMtxWorld());
	}

	// �t���[���J�E���g
	m_nCntFlame++;
	m_nCntTotalFlame++;

	if (m_nCntFlame % m_pFrame[m_nCurKey] == 0)
	{// �L�[��i�߂�
		m_nCurKey = (1 + m_nCurKey) % m_nKey;
		m_nCntFlame = 0;

		if (m_nCurKey == 0)
		{// �t���[����0�ɖ߂�
			//if (CMotionManager::TYPE_JUMP != m_nCurMotion)
			{// �W�����v�łȂ��Ƃ�
				m_nCntTotalFlame = 0;

				if (!m_bLoop)
				{// ���[�v���Ȃ�
					SetMotion(CMotionManager::TYPE_NEUTRAL);
				}
			}
		}
	}
}

//=========================================
// �U�������蔻��
//=========================================
bool CMotion::Collision(D3DXVECTOR3 pos)
{
	bool bHit = false;

	if (m_pCollision)
	{// �g���Ă���
		if (m_nCntTotalFlame >= m_pCollision->nStartFrame && m_nCntTotalFlame <= m_pCollision->nEndFrame)
		{
			// �����XY�͈̔�
			float fLength = powf(m_pCollision->fRadius, 2);

			float fLengthX = pos.x - m_pModel[m_pCollision->nIdxModel]->GetMtxWorld()._41;	// X�̍�
			float fLengthY = pos.y - m_pModel[m_pCollision->nIdxModel]->GetMtxWorld()._42;	// Y�̍�
			float fLengthZ = pos.z - m_pModel[m_pCollision->nIdxModel]->GetMtxWorld()._43;	// Z�̍�

			float fLengthTotal = powf(fLengthX, 2) + powf(fLengthY, 2) + powf(fLengthZ, 2);						// XYZ�̍��̓��

			if (fLength >= fLengthTotal)
			{// �͈͓�
				bHit = true;
			}
		}
	}

	return bHit;
}

//=========================================
// �U�������蔻��
//=========================================
int CMotion::GetAttackIdx(void)
{
	int nIdxModel = -1;

	if (m_pCollision)
	{// ���肪����
		nIdxModel = m_pCollision->nIdxModel;
	}

	return nIdxModel;
}