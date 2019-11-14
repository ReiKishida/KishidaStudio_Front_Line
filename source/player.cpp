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

//==================================
// �}�N����`
//==================================
#define MODEL_FILE					"data/TEXT/chaser/model_chaser.txt"
#define PLAYER_DAMAGE_TIME			(60)		// �_���[�W���󂯂����̖��G����
#define PLAYER_DAMAGE_MOVE			(40)		// �_���[�W���󂯂Ă��瓮����悤�ɂȂ�܂ł̎���
#define PLAYER_RETICLE_LENGTH		(2500.0f)	// ���e�B�N���̋���
#define PLAYER_BULLET_DISPERTION	(40)		// �e�̂΂��
#define RELOAD_TIME					(60 * 2)

//==================================
// �ÓI�����o�ϐ��錾
//==================================

//==================================
// ��������
//==================================
CPlayer* CPlayer::Create(void)
{
	CPlayer *pPlayer;

	pPlayer = new CPlayer;	// ���������m��

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
	m_fRotDest = 0.0f;
	m_fSpeed = 0.0f;
	m_fCameraAngle = 0.0f;
	m_pModel = NULL;
	m_nNumParts = 0;
	m_pUpperMotion = NULL;
	m_pLowerMotion = NULL;
	m_pReticle = NULL;
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
		m_pUpperMotion = CMotion::Create(0, 6, m_pModel);
		m_pUpperMotion->SetMotion(CMotionManager::TYPE_NEUTRAL);	// �j���[�g�������[�V������ݒ�
	}

	if (NULL == m_pLowerMotion)
	{// ���[�V�����N���X�̐���
		m_pLowerMotion = CMotion::Create(0, m_nNumParts, m_pModel, 5);
		m_pLowerMotion->SetMotion(CMotionManager::TYPE_NEUTRAL);	// �j���[�g�������[�V������ݒ�
	}

	if (NULL == m_pShadow)
	{// �e�̐���
		m_pShadow = CShadow::Create(&m_pos);
	}

	if (NULL == m_pReticle)
	{// ���e�B�N���̐���
		m_pReticle = CScene3DBill::Create();
		m_pReticle->BindTexture(CTexture::GetTexture(CTexture::TEXTURE_RETICLE));
		m_pReticle->SetSize(D3DXVECTOR3(50.0f, 50.0f, 0.0f));
		m_pReticle->SetLighting(false);
		m_pReticle->SetZBuffer(D3DCMP_ALWAYS);
		m_pReticle->SwapPriority(6);
	}

	m_fSpeed = 5.0f;
	m_nRemBullet = 5;
	m_pUINum->SetRemainBullet(m_nRemBullet);

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

	// �I�u�W�F�N�g�̔j��
	CScene::Release();
}

//=========================================
// �X�V����
//=========================================
void CPlayer::Update(void)
{
	if (CManager::GetGame()->GetPart() == CGame::PART_ACTION)
	{// �A�N�V�����p�[�g
		CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();	// �L�[�{�[�h�̓��͂��擾
		CXInput *pXInput = CManager::GetXInput();					// XInput�̓��͂��擾

		// �ړ��̏���
		Movement();

		// �e������
		Shoot();

		// �p�x�̍X�V
		Angle();

		// �d��
		//m_move.y -= GRAVITY;

		// �n�ʃI�u�W�F�N�g��T��
		CScene *pScene = CScene::GetSceneTop(MESHFIELD_PRIORITY);
		while (pScene != NULL)
		{// NULL�ɂȂ�܂Ń��[�v
			CScene *pSceneNext = pScene->GetSceneNext();
			CScene::OBJTYPE objType = pScene->GetObjType();

			if (objType == CScene::OBJTYPE_FIELD)
			{// �n�ʂ������Ƃ�
				CMeshField *pMeshField = (CMeshField*)pScene;

				float fHeight = pMeshField->GetHeight(m_pos);		// �n�ʂ̍����̎擾
				m_pos.y = fHeight;
				break;
			}

			// ���̃I�u�W�F�N�g������
			pScene = pSceneNext;
		}

		D3DXVECTOR3 rotCamera = CManager::GetCamera()->GetRot();
		D3DXVECTOR3 posR = CManager::GetCamera()->GetPosR();

		m_pReticle->SetPos(D3DXVECTOR3(sinf(rotCamera.y) * PLAYER_RETICLE_LENGTH, cosf(rotCamera.x) * PLAYER_RETICLE_LENGTH, cosf(rotCamera.y) * PLAYER_RETICLE_LENGTH) + D3DXVECTOR3(m_pos.x, 0.0f, m_pos.z));

		for (int nCntModel = 0; nCntModel < m_nNumParts; nCntModel++)
		{// �p�[�c���f���̍X�V
			m_pModel[nCntModel]->Update();
		}
		CDebugProc::Print("�ʒu�F%.2f %.2f %.2f", m_pos.x, m_pos.y, m_pos.z);
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
		//if ()
		//{// �_���[�W���󂯂Ă�����
		//	m_pModel[nCntModel]->AddColor(D3DXCOLOR(0.7f, 0.0f, 0.0f, -0.5f));
		//}

		// ���f���̕`�揈��
		m_pModel[nCntModel]->Draw();

		//if (m_bDamage)
		//{// �_���[�W���󂯂Ă�����
		//	m_pModel[nCntModel]->AddColor(D3DXCOLOR(-0.7f, 0.0f, 0.0f, 0.5f));
		//}
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
			m_pUpperMotion->SetMotion(CMotionManager::TYPE_WALK);
			m_pLowerMotion->SetMotion(CMotionManager::TYPE_WALK);
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

	m_pos += m_move;
}

//=========================================
// �e��������
//=========================================
void CPlayer::Shoot(void)
{
	CInputMouse *pMouse = CManager::GetInputMouse();	// �}�E�X�̓��͂��擾
	CXInput *pXInput = CManager::GetXInput();			// XInput�̓��͂��擾

	if (pMouse->GetTrigger(CInputMouse::DIMS_BUTTON_0) && m_nRemBullet > 0)
	{
		// �J�����̊p�x�ƒ����_���擾
		D3DXVECTOR3 rotCamera = CManager::GetCamera()->GetRot();
		D3DXVECTOR3 posR = CManager::GetCamera()->GetPosR();

		// ���e�B�N���i�ړI�̈ʒu�j�̎擾
		D3DXVECTOR3 posReticle = m_pReticle->GetPos();
		D3DXVECTOR3 dispertion = D3DXVECTOR3((float)(PLAYER_BULLET_DISPERTION - rand() % (PLAYER_BULLET_DISPERTION * 2)), 0.0f, (float)(PLAYER_BULLET_DISPERTION - rand() % (PLAYER_BULLET_DISPERTION * 2)));
		posReticle += dispertion;

		// �ˏo���̈ʒu�̎擾
		D3DXMATRIX mtxCanon = m_pModel[3]->GetMtxWorld();
		D3DXVECTOR3 posCanon = D3DXVECTOR3(mtxCanon._41, mtxCanon._42, mtxCanon._43) + D3DXVECTOR3(sinf(rotCamera.y) * 30.0f, cosf(rotCamera.x) * 30.0f, cosf(rotCamera.y) * 30.0f);

		// ���������̊p�x�̌v�Z
		float fAngle = atan2f(posReticle.x - posCanon.x, posReticle.z - posCanon.z);
		float fAngleV = rotCamera.x + (float)(PLAYER_BULLET_DISPERTION - (rand() % PLAYER_BULLET_DISPERTION * 2)) * 0.0005f;

		// �e�̐���
		CBulletPlayer::Create(posCanon, fAngle, fAngleV, CBulletPlayer::TYPE_NORMAL);

		// ���e�B�N���i�ړI�̈ʒu�j�̎擾
		posReticle = m_pReticle->GetPos();
		dispertion = D3DXVECTOR3((float)(PLAYER_BULLET_DISPERTION - rand() % (PLAYER_BULLET_DISPERTION * 2)), 0.0f, (float)(PLAYER_BULLET_DISPERTION - rand() % (PLAYER_BULLET_DISPERTION * 2)));
		posReticle += dispertion;

		// �ˏo���̈ʒu�̎擾
		mtxCanon = m_pModel[5]->GetMtxWorld();
		posCanon = D3DXVECTOR3(mtxCanon._41, mtxCanon._42, mtxCanon._43) + D3DXVECTOR3(sinf(rotCamera.y) * 30.0f, cosf(rotCamera.x) * 30.0f, cosf(rotCamera.y) * 30.0f);

		// ���������̊p�x�̌v�Z
		fAngle = atan2f(posReticle.x - posCanon.x, posReticle.z - posCanon.z);
		fAngleV = rotCamera.x + (float)(PLAYER_BULLET_DISPERTION - (rand() % PLAYER_BULLET_DISPERTION * 2)) * 0.0005f;

		// �e�̐���
		CBulletPlayer::Create(posCanon, fAngle, fAngleV, CBulletPlayer::TYPE_NORMAL);
		m_nRemBullet--;
	}

	Reload();

	m_pUINum->SetRemainBullet(m_nRemBullet);
	//if (m_nRemBullet == 0) { m_nRemBullet = 3; }
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
	//else
	//{// �����
	//	if (m_fRotDest <= D3DX_PI * 0.5f && m_fRotDest >= D3DX_PI * -0.5f)
	//	{// �����g�̓�����i�s�����ɍ��킹��
	//		D3DXVECTOR3 rot = m_pModel[0]->GetRot();
	//		m_pModel[0]->SetRot(D3DXVECTOR3(rot.x, m_fCameraAngle - m_rot.y, rot.z));
	//		rot = m_pModel[1]->GetRot();
	//		m_pModel[1]->SetRot(D3DXVECTOR3(-rotCamera.x + (D3DX_PI * 0.5f), m_fCameraAngle - fAngle, rot.z));
	//	}
	//	else
	//	{// �΂ߌ������̂Ƃ�
	//		D3DXVECTOR3 rot = m_pModel[1]->GetRot();
	//		m_pModel[1]->SetRot(D3DXVECTOR3(-rotCamera.x + (D3DX_PI * 0.5f), m_fCameraAngle - (fAngle - D3DX_PI), rot.z));
	//	}
	//	CDebugProc::Print("�㔼�g�p�x:%.2f", m_pModel[1]->GetRot().y);

	//}
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
			CSound *pSound = CManager::GetSound();				// �T�E���h�̎擾
			pSound->PlaySound(CSound::SOUND_LABEL_DAMAGE);		// �_���[�W�����Đ�
		}
	}
}

//=========================================
// ���_�̍ő�l���擾
//=========================================
D3DXVECTOR3 CPlayer::GetVtxMax(void)
{
	if (NULL != m_pModel) { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }

	D3DXVECTOR3 vtxMax = D3DXVECTOR3(-10000.0f, -10000.0f, -10000.0f);
	for (int nCntParts = 0; nCntParts < m_nNumParts; nCntParts++)
	{// �S�p�[�c������
		D3DXVECTOR3 vtx = m_pModel[nCntParts]->GetVtxMax();

		if (vtx.x > vtxMax.x) { vtxMax.x = vtx.x; }
		if (vtx.y > vtxMax.y) { vtxMax.y = vtx.y; }
		if (vtx.z > vtxMax.z) { vtxMax.z = vtx.z; }
	}

	return vtxMax;
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
			m_pUITex[0] = CUI_TEXTURE::Create(D3DXVECTOR3(1100.0f, 650.0f, 0.0f), 200.0f, 80.0f, CUI_TEXTURE::UIFLAME_REROAD);
		}
		if (m_pUITex[1] == NULL)
		{	// ��ʒ���
			m_pUITex[1] = CUI_TEXTURE::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), 200.0f, 80.0f, CUI_TEXTURE::UIFLAME_REROAD);
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
			//m_pGauge->BindTexture(CTexture::GetTexture(CTexture::TEXTURE_GAUGE), 0);		// ���̒���
			//m_pGauge->BindTexture(CTexture::GetTexture(CTexture::TEXTURE_GAUGE1), 1);	//  ���݂̗̑�
		}

		// �Q�[�W����
		m_pGauge->AddSubtract(100.0f / RELOAD_TIME);		// 100�� / �b��

															//*******************************************
															// �ݒ肵���b���と���S���Q�[�W�j�����e����
															//*******************************************
		if (m_nCntReRoad % RELOAD_TIME == 0)
		{
			// �e����
			m_nRemBullet = PLAYER_BULLET;

			if (m_pGauge != NULL)
			{	// �Q�[�W�̔j��
				m_pGauge->Uninit();
				m_pGauge = NULL;
			}

			for (int nCnt = 0; nCnt < MAX_UITEX; nCnt++)
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
