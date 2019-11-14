//=============================================================================
//
// �Q�[������ [game.cpp]
// Author : Takuto Ishida
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "game.h"
#include "camera.h"
#include "renderer.h"
#include "fade.h"
#include "input.h"
#include "manager.h"
#include "scene.h"
#include "camera.h"
#include "debugProc.h"
#include "result.h"
#include "sound.h"
#include "life.h"
#include "modelSet.h"
#include "player.h"
#include "meshField.h"
#include "motion.h"
#include "pause.h"
#include "meshField.h"
#include "number.h"
#include "bullet.h"
#include "button.h"
#include "enemy.h"
#include "mouseCursor.h"
#include "UI_Number.h"
#include "UI_Texture.h"
#include "texture.h"
#include <stdio.h>

#include "model.h"
#include "gauge.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define FIELD_MODEL_NAME	"data/MODEL/map_UV_bill.x"
#define NUMTEX_UV_X		(1)
#define NUMTEX_UV_Y		(3)

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
CGame::STATE CGame::m_state = CGame::STATE_NONE;
int CGame::m_nCurStage = 0;
CPlayer *CGame::m_pPlayer = NULL;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CGame::CGame(int nPriority, CScene::OBJTYPE objType) : CScene(nPriority, objType)
{
	m_modeCounter = 0;
	m_pMouseCursor = NULL;
	m_pPause = NULL;
	m_bPause = false;
	m_pTimer = NULL;
	m_pButtonManager = NULL;
	m_part = PART_ACTION;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CGame::~CGame()
{
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CGame::Init(void)
{
	// �ǂݍ���
	CMotionManager::Load();

	m_pField = CModel::Create();
	m_pField->SetModel(FIELD_MODEL_NAME);

	// �v���C���[�̐���
	m_pPlayer = CPlayer::Create();

	CBulletCollision::Create();

	CEnemy::Load();
	CEnemy::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);

	//****************************************
	// 2DUI�����i�t���[���j
	//****************************************
	// ����t���[��
	CUI_TEXTURE::Create(D3DXVECTOR3(1100.0f, 650.0f, 0.0f), 350.0f, 120.0f, CUI_TEXTURE::UIFLAME_WEAPON);		// ����

																												// �v���C���[�̗̓t���[��
	CUI_TEXTURE::Create(D3DXVECTOR3(180.0f, 650.0f, 0.0f), 350.0f, 125.0f, CUI_TEXTURE::UIFLAME_PLAYER_HP);	// ����

																											// AI�`�P�b�g���t���[��
	CUI_TEXTURE::Create(D3DXVECTOR3(125.0f, 530.0f, 0.0f), 230.0f, 100.0f, CUI_TEXTURE::UIFLAME_DRONE);		// AI01
	CUI_TEXTURE::Create(D3DXVECTOR3(125.0f, 420.0f, 0.0f), 230.0f, 100.0f, CUI_TEXTURE::UIFLAME_WORKER);	// AI02

																											// �`�[���̃`�P�b�g���t���[��
	CUI_TEXTURE::Create(D3DXVECTOR3(345.0f, 60.0f, 0.0f), 120.0f, 50.0f, CUI_TEXTURE::UIFLAME_TEAM_BLUE);	// BLUE
	CUI_TEXTURE::Create(D3DXVECTOR3(935.0f, 60.0f, 0.0f), 120.0f, 50.0f, CUI_TEXTURE::UIFLAME_TEAM_RED);	// RED

																											// �^�C�}�[�t���[��
	CUI_TEXTURE::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, 30.0f, 0.0f), 100.0f, 35.0f, CUI_TEXTURE::UIFLAME_TIMER);	// �^�C�����S

																												// �p�[�g�؂�ւ��e�N�X�`��
	CUI_TEXTURE::Create(D3DXVECTOR3(125.0f, 50.0f, 0.0f), 200.0f, 90.0f, CUI_TEXTURE::UIFLAME_STRATEGY_PART);	// �X�g���e�W�[�p�[�g
	CUI_TEXTURE::Create(D3DXVECTOR3(165.0f, 90.0f, 0.0f), 200.0f, 90.0f, CUI_TEXTURE::UIFLAME_ACTION_PART);		// �A�N�V�����p�[�g

																												// �p�[�g�؂�ւ��i�`�F���W�j
	CUI_TEXTURE::Create(D3DXVECTOR3(50.0f, 135.0f, 0.0f), 100.0f, 110.0f, CUI_TEXTURE::UIFLAME_CHANGE);

	//****************************************
	// UI�����i�����j
	//****************************************
	CUI_NUMBER::Create(D3DXVECTOR3(970.0f, 660.0f, 0.0f), 150.0f, 90.0f, 60.0f, CUI_NUMBER::UI_NUMTYPE_REMAINBULLET, 0, NUMTEX_UV_X, NUMTEX_UV_Y);		// �c�e
	CUI_NUMBER::Create(D3DXVECTOR3(195.0f, 650.0f, 0.0f), 170.0f, 110.0f, 70.0f, CUI_NUMBER::UI_NUMTYPE_PLAYER_HP, 0, NUMTEX_UV_X, NUMTEX_UV_Y);				// �v���C���[���C�t
	CUI_NUMBER::Create(D3DXVECTOR3(430.0f, 60.0f, 0.0f), 120.0f, 80.0f, 55.0f, CUI_NUMBER::UI_NUMTYPE_BLUE, 1, NUMTEX_UV_X, NUMTEX_UV_Y);							// BLUE�`�[���`�P�b�g
	CUI_NUMBER::Create(D3DXVECTOR3(720.0f, 60.0f, 0.0f), 120.0f, 80.0f, 55.0f, CUI_NUMBER::UI_NUMTYPE_RED, 2, NUMTEX_UV_X, NUMTEX_UV_Y);							// RED�`�[���`�P�b�g

																																									//CUI_NUMBER::Create(D3DXVECTOR3(200.0f, 260.0f, 0.0f), 150.0f, 75.0f, 40.0f, CUI_NUMBER::UI_NUMTYPE_PLAYER_HP);			// �v���C���[���C�t
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CGame::Uninit(void)
{
	// �f�[�^�̔j��
	CMotionManager::Unload();
	CModelSetManager::Unload();
	CEnemy::Unload();

	if (NULL != m_pPlayer)
	{// �v���C���[�N���X�̔j��
		m_pPlayer->Uninit();
		m_pPlayer = NULL;
	}

	if (NULL != m_pField)
	{// �n�ʂ̔j��
		m_pField->Uninit();
		delete m_pField;
		m_pField = NULL;
	}

	// �I�u�W�F�N�g��j��
	CScene::Release();
}

//=============================================================================
// �Q�[���X�V����
//=============================================================================
void CGame::Update(void)
{
	CInputKeyboard	*pKeyboard = CManager::GetInputKeyboard();	// �L�[�{�[�h�̎擾
	CXInput			*pXInput = CManager::GetXInput();			// XInput�̎擾
	CSound *pSound = CManager::GetSound();						// �T�E���h�̎擾
	CInputMouse *pMouse = CManager::GetInputMouse();
	CDirectInput *pDirectInput = CManager::GetDirectInput();	//DirectInput�̎擾
	CDirectInput::GamePad *DirectInputStick = pDirectInput->GetgamePadStick();

#ifdef _DEBUG
	if (pKeyboard->GetTrigger(DIK_O))
	{
		CFade::Create(CManager::MODE_MENU);
	}
#endif

	if (pKeyboard->GetTrigger(DIK_Z) || pDirectInput->GetGamePadTrigger(0))
	{// �p�[�g�؂�ւ�
		m_part = (PART)((m_part + 1) % PART_MAX);

		CScene::UIUninit();
		if (PART_STRATEGY == m_part)
		{
			//****************************************
			// 2DUI�����i�t���[���j
			//****************************************
			// �w�i
			CUI_TEXTURE::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), SCREEN_WIDTH, SCREEN_HEIGHT, CUI_TEXTURE::UIFLAME_STRATEGY_BG);	// �w�i

																																						// �`�[���̃`�P�b�g���t���[��
			CUI_TEXTURE::Create(D3DXVECTOR3(495.0f, 60.0f, 0.0f), 120.0f, 50.0f, CUI_TEXTURE::UIFLAME_TEAM_BLUE);	// BLUE
			CUI_TEXTURE::Create(D3DXVECTOR3(1085.0f, 60.0f, 0.0f), 120.0f, 50.0f, CUI_TEXTURE::UIFLAME_TEAM_RED);	// RED

																													// �^�C�}�[�t���[��
			CUI_TEXTURE::Create(D3DXVECTOR3(790, 30.0f, 0.0f), 100.0f, 35.0f, CUI_TEXTURE::UIFLAME_TIMER);	// �^�C�����S

																											// �p�[�g�؂�ւ��e�N�X�`��
			CUI_TEXTURE::Create(D3DXVECTOR3(125.0f, 50.0f, 0.0f), 200.0f, 90.0f, CUI_TEXTURE::UIFLAME_ACTION_PART);	// �A�N�V�����p�[�g
			CUI_TEXTURE::Create(D3DXVECTOR3(165.0f, 90.0f, 0.0f), 200.0f, 90.0f, CUI_TEXTURE::UIFLAME_STRATEGY_PART);		// �X�g���e�W�[�p�[�g

																															// �p�[�g�؂�ւ��i�`�F���W�j
			CUI_TEXTURE::Create(D3DXVECTOR3(50.0f, 135.0f, 0.0f), 100.0f, 110.0f, CUI_TEXTURE::UIFLAME_CHANGE);

			// �t���[��
			CUI_TEXTURE::Create(D3DXVECTOR3(785.0f, 420.0f, 0.0f), 990.0f, 590.0f, CUI_TEXTURE::UIFLAME_FLAME_BLUE);	// ��g
			CUI_TEXTURE::Create(D3DXVECTOR3(790.0f, 190.0f, 0.0f), 970.0f, 100.0f, CUI_TEXTURE::UIFLAME_FLAME_WHITE);	// ���W�b�N���C��
			CUI_TEXTURE::Create(D3DXVECTOR3(405.0f, 470.0f, 0.0f), 200.0f, 450.0f, CUI_TEXTURE::UIFLAME_FLAME_WHITE);	// AI�\��
			CUI_TEXTURE::Create(D3DXVECTOR3(895.0f, 470.0f, 0.0f), 760.0f, 450.0f, CUI_TEXTURE::UIFLAME_FLAME_WHITE);	// ���W�b�N����

																														//****************************************
																														// 1P�������
																														//****************************************
			CUI_TEXTURE::Create(D3DXVECTOR3(150.0f, 325.0f, 0.0f), 280.0f, 260.0f, CUI_TEXTURE::UIFLAME_1P_INFO);

			// �v���C���[�̗̓t���[��
			CUI_TEXTURE::Create(D3DXVECTOR3(150.0f, 260.0f, 0.0f), 255.0f, 80.0f, CUI_TEXTURE::UIFLAME_PLAYER_HP);

			// AI�`�P�b�g���t���[��
			CUI_TEXTURE::Create(D3DXVECTOR3(150.0f, 335.0f, 0.0f), 255.0f, 80.0f, CUI_TEXTURE::UIFLAME_WORKER);		// ���[�J�[
			CUI_TEXTURE::Create(D3DXVECTOR3(150.0f, 410.0f, 0.0f), 255.0f, 80.0f, CUI_TEXTURE::UIFLAME_DRONE);		// �h���[��

																													//****************************************
																													// 2P�������
																													//****************************************
			CUI_TEXTURE::Create(D3DXVECTOR3(150.0f, 590.0f, 0.0f), 280.0f, 260.0f, CUI_TEXTURE::UIFLAME_2P_INFO);

			// �v���C���[�̗̓t���[��
			CUI_TEXTURE::Create(D3DXVECTOR3(150.0f, 525.0f, 0.0f), 255.0f, 80.0f, CUI_TEXTURE::UIFLAME_PLAYER_HP);

			// AI�`�P�b�g���t���[��
			CUI_TEXTURE::Create(D3DXVECTOR3(150.0f, 600.0f, 0.0f), 255.0f, 80.0f, CUI_TEXTURE::UIFLAME_WORKER);		// ���[�J�[
			CUI_TEXTURE::Create(D3DXVECTOR3(150.0f, 675.0f, 0.0f), 255.0f, 80.0f, CUI_TEXTURE::UIFLAME_DRONE);		// �h���[��

																													//****************************************
																													// UI�����i�����j
			//																										//****************************************
			CUI_NUMBER::Create(D3DXVECTOR3(630.0f, 60.0f, 0.0f), 120.0f, 80.0f, 55.0f, CUI_NUMBER::UI_NUMTYPE_BLUE, 1, NUMTEX_UV_X, NUMTEX_UV_Y);							// BLUE�`�[���`�P�b�g
			CUI_NUMBER::Create(D3DXVECTOR3(920.0f, 60.0f, 0.0f), 120.0f, 80.0f, 55.0f, CUI_NUMBER::UI_NUMTYPE_RED, 2, NUMTEX_UV_X, NUMTEX_UV_Y);							// RED�`�[���`�P�b�g

			if (NULL == m_pButtonManager)
			{// �{�^���Ǘ��N���X�̐���
				m_pButtonManager = CButtonManagerStrategy::Create();
			}

			if (NULL == m_pMouseCursor)
			{// �}�E�X�J�[�\���̐���
				m_pMouseCursor = CMouseCursor::Create();
			}
		}
		else
		{
			if (NULL != m_pButtonManager)
			{// �{�^���Ǘ��N���X�̔j��
				m_pButtonManager->Uninit();
				m_pButtonManager = NULL;
			}

			if (NULL != m_pMouseCursor)
			{// �}�E�X�J�[�\���̔j��
				m_pMouseCursor->Uninit();
				m_pMouseCursor = NULL;
			}

			//****************************************
			// 2DUI�����i�t���[���j
			//****************************************
			// ����t���[��
			CUI_TEXTURE::Create(D3DXVECTOR3(1100.0f, 650.0f, 0.0f), 350.0f, 120.0f, CUI_TEXTURE::UIFLAME_WEAPON);		// ����

																														// �v���C���[�̗̓t���[��
			CUI_TEXTURE::Create(D3DXVECTOR3(180.0f, 650.0f, 0.0f), 350.0f, 125.0f, CUI_TEXTURE::UIFLAME_PLAYER_HP);	// ����

																													// AI�`�P�b�g���t���[��
			CUI_TEXTURE::Create(D3DXVECTOR3(125.0f, 530.0f, 0.0f), 230.0f, 100.0f, CUI_TEXTURE::UIFLAME_DRONE);		// AI01
			CUI_TEXTURE::Create(D3DXVECTOR3(125.0f, 420.0f, 0.0f), 230.0f, 100.0f, CUI_TEXTURE::UIFLAME_WORKER);	// AI02

																													// �`�[���̃`�P�b�g���t���[��
			CUI_TEXTURE::Create(D3DXVECTOR3(345.0f, 60.0f, 0.0f), 120.0f, 50.0f, CUI_TEXTURE::UIFLAME_TEAM_BLUE);	// BLUE
			CUI_TEXTURE::Create(D3DXVECTOR3(935.0f, 60.0f, 0.0f), 120.0f, 50.0f, CUI_TEXTURE::UIFLAME_TEAM_RED);	// RED

																													// �^�C�}�[�t���[��
			CUI_TEXTURE::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, 30.0f, 0.0f), 100.0f, 35.0f, CUI_TEXTURE::UIFLAME_TIMER);	// �^�C�����S

																														// �p�[�g�؂�ւ��e�N�X�`��
			CUI_TEXTURE::Create(D3DXVECTOR3(125.0f, 50.0f, 0.0f), 200.0f, 90.0f, CUI_TEXTURE::UIFLAME_STRATEGY_PART);	// �X�g���e�W�[�p�[�g
			CUI_TEXTURE::Create(D3DXVECTOR3(165.0f, 90.0f, 0.0f), 200.0f, 90.0f, CUI_TEXTURE::UIFLAME_ACTION_PART);		// �A�N�V�����p�[�g

																														// �p�[�g�؂�ւ��i�`�F���W�j
			CUI_TEXTURE::Create(D3DXVECTOR3(50.0f, 135.0f, 0.0f), 100.0f, 110.0f, CUI_TEXTURE::UIFLAME_CHANGE);

			//****************************************
			// UI�����i�����j
			//****************************************
			CUI_NUMBER::Create(D3DXVECTOR3(970.0f, 660.0f, 0.0f), 150.0f, 90.0f, 60.0f, CUI_NUMBER::UI_NUMTYPE_REMAINBULLET, 0, NUMTEX_UV_X, NUMTEX_UV_Y);		// �c�e
			CUI_NUMBER::Create(D3DXVECTOR3(195.0f, 650.0f, 0.0f), 170.0f, 110.0f, 70.0f, CUI_NUMBER::UI_NUMTYPE_PLAYER_HP, 0, NUMTEX_UV_X, NUMTEX_UV_Y);				// �v���C���[���C�t
			CUI_NUMBER::Create(D3DXVECTOR3(430.0f, 60.0f, 0.0f), 120.0f, 80.0f, 55.0f, CUI_NUMBER::UI_NUMTYPE_BLUE, 1, NUMTEX_UV_X, NUMTEX_UV_Y);							// BLUE�`�[���`�P�b�g
			CUI_NUMBER::Create(D3DXVECTOR3(720.0f, 60.0f, 0.0f), 120.0f, 80.0f, 55.0f, CUI_NUMBER::UI_NUMTYPE_RED, 2, NUMTEX_UV_X, NUMTEX_UV_Y);							// RED�`�[���`�P�b�g

		}
	}

	// �t�F�[�h�̎擾
	CFade::FADE fade = CFade::GetFade();

	if (fade == CFade::FADE_NONE)
	{// �t�F�[�h���̓|�[�Y�ɂł��Ȃ�����
		if (pKeyboard->GetTrigger(DIK_RETURN))
		{
			CFade::Create(CManager::MODE_RESULT);
		}

		if (pKeyboard->GetTrigger(DIK_P) || pXInput->GetTrigger(0, CXInput::XIJS_BUTTON_4))
		{// �|�[�YON/OFF
			m_bPause = m_bPause ? false : true;

			if (m_bPause)
			{// �|�[�Y�ɂȂ����Ƃ�
				if (m_pPause == NULL)
				{
					//pSound->PlaySound(CSound::SOUND_LABEL_PAUSEON);
					m_pPause = CPause::Create();	// �|�[�Y�N���X�𐶐�
				}
			}
		}

		if(!m_bPause)
		{// �|�[�Y�����Ƃ�
			if (m_pPause != NULL)
			{// �|�[�Y�łȂ��Ȃ�����j��
				m_pPause->Uninit();
				m_pPause = NULL;
				pSound->PlaySound(CSound::SOUND_LABEL_PAUSEOFF);
			}
		}
	}

	switch (m_state)
	{
	case STATE_NORMAL:
		break;

	case STATE_END:
		m_modeCounter++;
		if (m_modeCounter >= 60)
		{
			m_state = STATE_NONE;

			// ���(���[�h)�̐ݒ�
			//CFade::Create(CManager::MODE_RESULT);
		}
		break;
	}

	CDebugProc::Print("�Q�[��");
}

//=============================================================================
// �Q�[���`�揈��
//=============================================================================
void CGame::Draw(void)
{
	if (NULL != m_pField)
	{// �n�ʂ̕`��
		m_pField->Draw();
	}
}

//=============================================================================
// �Q�[���̏�Ԃ̐ݒ�
//=============================================================================
const void CGame::SetGameState(STATE state)
{
	m_state = state;
}

//=============================================================================
// �X�e�[�W�̐ݒ�
//=============================================================================
void CGame::SetStage(int nStage)
{
	// ���݂̃X�e�[�W��ݒ�
	m_nCurStage = nStage;
}
