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

#include "server.h"
#include "serverfunction.h"

#include "collisionSet.h"
#include "scene3D.h"
#include "menu.h"

#include "particle.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define FIELD_MODEL_NAME	"data/MODEL/map_UV_bill.x"
#define SKY_MODEL_NAME		"data/MODEL/sky_dome.x"
#define NUMTEX_UV_X		(1)
#define NUMTEX_UV_Y		(3)
#define GAME_INI			"data/TEXT/game.ini"

#define GAME_SCRIPT			"SCRIPT"
#define GAME_END_SCRIPT		"END_SCRIPT"
#define GAME_BLUE			"BLUE"
#define GAME_BLUE_END		"BLUE_END"
#define GAME_RED			"RED"
#define GAME_RED_END		"RED_END"
#define GAME_SPAWN_A		"A"
#define GAME_SPAWN_B		"B"
#define GAME_SPAWN_C		"C"
#define GAME_SPAWN_D		"D"

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
CGame::STATE CGame::m_state = CGame::STATE_NONE;
int CGame::m_nCurStage = 0;
CPlayer *CGame::m_pPlayer[MAX_PLAYER_CONNECT] = {};
CMechaSelect::MECHATYPE CGame::m_aMechaType[MAX_PLAYER_CONNECT] = { CMechaSelect::MECHATYPE_EMPTY,CMechaSelect::MECHATYPE_EMPTY ,CMechaSelect::MECHATYPE_EMPTY ,CMechaSelect::MECHATYPE_EMPTY };

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
	m_pSky = NULL;
	m_pMouse = NULL;

	m_nBlueLinkEnergy = 0;
	m_nRedLinkEnergy = 0;

	for (int nCntConnect = 0; nCntConnect < MAX_PLAYER_CONNECT; nCntConnect++)
	{
		m_bConnect[nCntConnect] = false;
	}

	for (int nCntTeam = 0; nCntTeam < NUM_TEAM; nCntTeam++)
	{
		for (int nCntRespawn = 0; nCntRespawn < NUM_RESPAWN_POS; nCntRespawn++)
		{
			m_aRespawnPos[nCntTeam][nCntRespawn] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		}
	}
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
	m_nBlueLinkEnergy = 100;
	m_nRedLinkEnergy = 100;

	//���X�|�[���ʒu�̓ǂݍ���
	LoadRespawnPos();

	// �ǂݍ���
	CMotionManager::Load();

	m_pField = CModel::Create();
	m_pField->SetModel(FIELD_MODEL_NAME);
	m_pField->SetPos(D3DXVECTOR3(-2.0f, 0.0f, 17.0f));

	m_pSky = CModel::Create();
	m_pSky->SetModel(SKY_MODEL_NAME);

	// �v���C���[�̐���
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER_CONNECT; nCntPlayer++)
	{
		int nTeam = 0;

		if (nCntPlayer == 0 || nCntPlayer == 1)
		{
			nTeam = 0;
		}
		else if (nCntPlayer == 2 || nCntPlayer == 3)
		{
			nTeam = 1;
		}

		if (CMenu::GetMode() == CMenu::MODE_MULTI)
		{
			bool bConnect = false;
			CClient *pClient = CManager::GetClient();
			if (m_aMechaType[nCntPlayer] == -1)
			{
				m_aMechaType[nCntPlayer] = (CMechaSelect::MECHATYPE)(rand() % CMechaSelect::MECHATYPE_MAX);
				m_pPlayer[nCntPlayer] = CPlayer::Create(nCntPlayer, m_aMechaType[nCntPlayer], m_aRespawnPos[nTeam][nCntPlayer], m_bConnect[nCntPlayer]);
			}
			else
			{
				m_bConnect[nCntPlayer] = true;
				m_pPlayer[nCntPlayer] = CPlayer::Create(nCntPlayer, m_aMechaType[nCntPlayer], m_aRespawnPos[nTeam][nCntPlayer], m_bConnect[nCntPlayer]);

			}

			//if (pClient != NULL)
			//{
			//	CMechaSelect::MECHATYPE type = (CMechaSelect::MECHATYPE)pClient->GetMechaType(nCntPlayer);
			//	if (type == -1)
			//	{
			//		type = (CMechaSelect::MECHATYPE)(rand() % CMechaSelect::MECHATYPE_MAX);
			//		m_pPlayer[nCntPlayer] = CPlayer::Create(nCntPlayer, type,m_aRespawnPos[nTeam][nCntPlayer], m_bConnect[nCntPlayer]);
			//	}
			//	else
			//	{
			//		m_bConnect[nCntPlayer] = true;
			//		m_pPlayer[nCntPlayer] = CPlayer::Create(nCntPlayer, type, m_aRespawnPos[nTeam][nCntPlayer], m_bConnect[nCntPlayer]);

			//	}
			//}
		}
		else
		{
			if (nCntPlayer == 0)
			{
				m_aMechaType[nCntPlayer] = CMechaSelect::GetMechaType();
				m_pPlayer[nCntPlayer] = CPlayer::Create(nCntPlayer, m_aMechaType[nCntPlayer], m_aRespawnPos[nTeam][nCntPlayer], true);
			}
			else
			{
				m_aMechaType[nCntPlayer] = (CMechaSelect::MECHATYPE)(rand() % CMechaSelect::MECHATYPE_MAX);
				m_pPlayer[nCntPlayer] = CPlayer::Create(nCntPlayer, m_aMechaType[nCntPlayer], m_aRespawnPos[nTeam][nCntPlayer], false);
			}
		}
	}

	// �e�̓����蔻��N���X�̐���
	CBulletCollision::Create();

	// �}�b�v�̓����蔻��̓ǂݍ���
	CCollision::Load();

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

	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER_CONNECT; nCntPlayer++)
	{
		if (NULL != m_pPlayer[nCntPlayer])
		{// �v���C���[�N���X�̔j��
			m_pPlayer[nCntPlayer]->Uninit();
			m_pPlayer[nCntPlayer] = NULL;
		}
	}

	if (NULL != m_pButtonManager)
	{// �{�^���Ǘ��N���X�̔j��
		m_pButtonManager->Uninit();
		m_pButtonManager = NULL;
	}

	if (NULL != m_pField)
	{// �n�ʂ̔j��
		m_pField->Uninit();
		delete m_pField;
		m_pField = NULL;
	}

	if (NULL != m_pSky)
	{// �X�J�C�h�[���̔j��
		m_pSky->Uninit();
		delete m_pSky;
		m_pSky = NULL;
	}

	if (NULL != m_pMouseCursor)
	{// �}�E�X�J�[�\���̔j��
		m_pMouseCursor->Uninit();
		m_pMouseCursor = NULL;
	}

	if (NULL != m_pMouse)
	{// �}�E�X�J�[�\���̔j��
		m_pMouse->Uninit();
		m_pMouse = NULL;
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

	if (CMenu::GetMode() == CMenu::MODE_MULTI)
	{
		if (CManager::GetClient() != NULL)
		{
			if (CFade::GetFade() == CFade::FADE_NONE)
			{
				if (CManager::GetClient()->GetGameMode() == CClient::GAME_MODE_PLAYER)
				{
					//�K�v�ȏ����������ޏ���
					PrintData();
				}
				//����ǂݎ�鏈��
				ReadMessage();

				if (m_pPlayer[CManager::GetClient()->GetPlayerIdx()]->GetRespawn() == CPlayer::RESPAWN_NONE)
				{
					//�p�[�g�̐؂�ւ�����
					SwicthPart();
				}
			}
		}
	}
	else
	{
		if (m_pPlayer[0]->GetRespawn() == CPlayer::RESPAWN_NONE)
		{
			//�p�[�g�̐؂�ւ�����
			SwicthPart();
		}
	}

#ifdef _DEBUG
	if (pKeyboard->GetTrigger(DIK_O))
	{
		CFade::Create(CManager::MODE_MENU);
	}
#endif

	if(NULL != m_pMouse)
	CDebugProc::Print("�}�E�X�J�[�\���F%.2f %.2f", m_pMouse->GetPos().x, m_pMouse->GetPos().z);

	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER_CONNECT; nCntPlayer++)
	{
		if (m_pPlayer[nCntPlayer]->GetDeath() == true)
		{
			//m_state = STATE_END;
		}
	}

	if (CMenu::GetMode() == CMenu::MODE_MULTI)
	{
		if (CManager::GetClient() != NULL)
		{
			if (CManager::GetClient()->GetPlayerIdx() == 0)
			{
				if (m_nBlueLinkEnergy <= 0)
				{
					m_state = STATE_END;
					CResult::SetTeamWin(CResult::TEAM_WIN_RED);
				}
				else if (m_nRedLinkEnergy <= 0)
				{
					m_state = STATE_END;
					CResult::SetTeamWin(CResult::TEAM_WIN_BLUE);
				}

			}
		}
	}
	else if (CMenu::GetMode() == CMenu::MODE_SINGLE)
	{
		if (m_nBlueLinkEnergy <= 0)
		{
			m_state = STATE_END;
			CResult::SetTeamWin(CResult::TEAM_WIN_RED);
		}
		else if (m_nRedLinkEnergy <= 0)
		{
			m_state = STATE_END;
			CResult::SetTeamWin(CResult::TEAM_WIN_BLUE);
		}

	}
	// �t�F�[�h�̎擾
	CFade::FADE fade = CFade::GetFade();

	if (fade == CFade::FADE_NONE)
	{// �t�F�[�h���̓|�[�Y�ɂł��Ȃ�����
		if (pKeyboard->GetTrigger(DIK_RETURN))
		{
			//CFade::Create(CManager::MODE_RESULT);
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

		if (!m_bPause)
		{// �|�[�Y�����Ƃ�
			if (m_pPause != NULL)
			{// �|�[�Y�łȂ��Ȃ�����j��
				m_pPause->Uninit();
				m_pPause = NULL;
				//pSound->PlaySound(CSound::SOUND_LABEL_PAUSEOFF);
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
			CFade::Create(CManager::MODE_RESULT);
		}
		break;
	}

	CDebugProc::Print("�Q�[��");
	CDebugProc::Print("�@��F%d %d %d %d", m_aMechaType[0], m_aMechaType[1], m_aMechaType[2], m_aMechaType[3]);
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

	// �f�o�C�X�̎擾
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	// ���C�e�B���OOFF
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	if (NULL != m_pSky)
	{// �X�J�C�h�[���̕`��
		m_pSky->Draw();
	}

	// ���C�e�B���OON
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
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

//=============================================================================
// �p�[�g�̐؂�ւ�����
//=============================================================================
void CGame::SwicthPart(void)
{
	CInputKeyboard	*pKeyboard = CManager::GetInputKeyboard();	// �L�[�{�[�h�̎擾
	CXInput			*pXInput = CManager::GetXInput();			// XInput�̎擾
	CInputMouse *pMouse = CManager::GetInputMouse();
	CDirectInput *pDirectInput = CManager::GetDirectInput();	//DirectInput�̎擾
	CDirectInput::GamePad *DirectInputStick = pDirectInput->GetgamePadStick();

	if (pKeyboard->GetTrigger(DIK_Z) || pDirectInput->GetGamePadTrigger(0))
	{// �p�[�g�؂�ւ�
		m_part = (PART)((m_part + 1) % PART_MAX);

		CScene::UIUninit();

		if (PART_STRATEGY == m_part)
		{
			CreateStrategyUI();
		}
		else
		{
			CreateActionUI();
		}
	}
}

//=============================================================================
// �A�N�V�����p�[�g��UI��������
//=============================================================================
void CGame::CreateActionUI(void)
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

	if (NULL != m_pMouse)
	{
		//m_pMouse->Uninit();
		//m_pMouse = NULL;
		m_pMouse->SetDisp(false);
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

//=============================================================================
// �X�g���e�W�[�p�[�g��UI��������
//=============================================================================
void CGame::CreateStrategyUI(void)
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
	//****************************************
	CUI_NUMBER::Create(D3DXVECTOR3(630.0f, 60.0f, 0.0f), 120.0f, 80.0f, 55.0f, CUI_NUMBER::UI_NUMTYPE_BLUE, 1, NUMTEX_UV_X, NUMTEX_UV_Y);							// BLUE�`�[���`�P�b�g
	CUI_NUMBER::Create(D3DXVECTOR3(920.0f, 60.0f, 0.0f), 120.0f, 80.0f, 55.0f, CUI_NUMBER::UI_NUMTYPE_RED, 2, NUMTEX_UV_X, NUMTEX_UV_Y);							// RED�`�[���`�P�b�g

	if (NULL == m_pButtonManager)
	{// �{�^���Ǘ��N���X�̐���
		m_pButtonManager = CButtonManagerStrategy::Create();
	}

	if (NULL == m_pMouseCursor)
	{// �}�E�X�J�[�\���̐���
		m_pMouseCursor = CMouseCursor2D::Create();
	}

	if (NULL == m_pMouse)
	{
		m_pMouse = CMouseCursor::Create();
	}

	if (NULL != m_pMouse)
	{
		m_pMouse->SetDisp(true);
	}
}

//=============================================================================
// �K�v�ȏ����������ޏ���
//=============================================================================
void CGame::PrintData(void)
{
	//�N���C�A���g�̎擾
	CClient *pClient = CManager::GetClient();
#if 1
	if (pClient != NULL && pClient->GetConnect() == true)
	{//NULL�ł͂Ȃ��ꍇ
		if (m_pPlayer[pClient->GetPlayerIdx()] != NULL)
		{//NULL�ł͂Ȃ��ꍇ
			pClient->Printf(SERVER_PLAYER_DATA);

			//�v���C���[�ԍ�����������
			pClient->Printf("%d", CManager::GetClient()->GetPlayerIdx());
			pClient->Printf(" ");

			//�`�[���̔ԍ�����������
			pClient->Printf("%d", CManager::GetClient()->GetTeam());
			pClient->Printf(" ");

			//�v���C���[��x,y���̈ʒu����������
			pClient->Printf("%.1f %.1f", m_pPlayer[pClient->GetPlayerIdx()]->GetPos().x, m_pPlayer[pClient->GetPlayerIdx()]->GetPos().z);
			pClient->Printf(" ");

			//�v���C���[��y���̌�������������
			pClient->Printf("%.1f", m_pPlayer[pClient->GetPlayerIdx()]->GetRot().y);
			pClient->Printf(" ");

			//���f��0�Ԃ�y���̌�������������
			pClient->Printf("%.1f", m_pPlayer[pClient->GetPlayerIdx()]->GetModel(0)->GetRot().y);
			pClient->Printf(" ");

			//���f��1�Ԃ̌�������������
			pClient->Printf("%.1f %.1f %.1f", m_pPlayer[pClient->GetPlayerIdx()]->GetModel(1)->GetRot().x, m_pPlayer[pClient->GetPlayerIdx()]->GetModel(1)->GetRot().y, m_pPlayer[pClient->GetPlayerIdx()]->GetModel(1)->GetRot().z);
			pClient->Printf(" ");

			//�J�����̌�������������
			pClient->Printf("%.1f %.1f %.1f", CManager::GetCamera()->GetRot().x, CManager::GetCamera()->GetRot().y, CManager::GetCamera()->GetRot().z);
			pClient->Printf(" ");

			//���S���Ă��邩�ǂ���
			if (m_pPlayer[pClient->GetPlayerIdx()]->GetDeath() == true)
			{
				pClient->Printf("1");
				pClient->Printf(" ");
			}
			else
			{
				pClient->Printf("0");
				pClient->Printf(" ");
			}

			//�e�𔭎˂��Ă��邩�ǂ�������������
			if (m_pPlayer[pClient->GetPlayerIdx()]->GetShoot() == true)
			{//���˂���Ă���ꍇ

				pClient->Printf("1");
				pClient->Printf(" ");

				//�e�̓������ː�����������
				pClient->Printf("%d", m_pPlayer[pClient->GetPlayerIdx()]->GetNumShoot());
				pClient->Printf(" ");

				pClient->Printf("%d", m_pPlayer[pClient->GetPlayerIdx()]->GetAttack());
				pClient->Printf(" ");

				//�����p�x�̏�����������
				for (int nCntShoot = 0; nCntShoot < m_pPlayer[pClient->GetPlayerIdx()]->GetNumShoot() * 2; nCntShoot++)
				{
					pClient->Printf("%.2f %.2f", m_pPlayer[pClient->GetPlayerIdx()]->GetAngle(nCntShoot), m_pPlayer[pClient->GetPlayerIdx()]->GetAngleV(nCntShoot));
					pClient->Printf(" ");

				}

				//���˂��Ă��Ȃ���Ԃɖ߂�
				m_pPlayer[pClient->GetPlayerIdx()]->SetShoot(false);
			}
			else
			{
				pClient->Printf("0");
				pClient->Printf(" ");
			}

			////�`���b�g�����Ă��邩�ǂ�������������
			//if (m_pPlayer[pClient->GetPlayerIdx()]->GetChat() == true)
			//{//�`���b�g�����Ă���ꍇ

			//	pClient->Printf("1");
			//	pClient->Printf(" ");

			//	//�`���b�g�̔ԍ�����������
			//	pClient->Printf("%d", m_pPlayer[pClient->GetPlayerIdx()]->GetChat());
			//	pClient->Printf(" ");

			//	//�`���b�g���Ă��Ȃ���Ԃɖ߂�
			//	m_pPlayer[pClient->GetPlayerIdx()]->SetChat(false);
			//}
			//else
			//{
			//	pClient->Printf("0");
			//	pClient->Printf(" ");
			//}

			if (pClient->GetPlayerIdx() == 0)
			{//�z�X�g�̏ꍇ
			 if (m_state == STATE_END)
			 {
			 	pClient->Printf("1");
				pClient->Printf(" ");
				pClient->Printf("%d",CResult::GetTeamWin());
				pClient->Printf(" ");
			 }
			 else
			 {
			 	pClient->Printf("0");
				pClient->Printf(" ");

				pClient->Printf("%d", m_nBlueLinkEnergy);
				pClient->Printf(" ");
				pClient->Printf("%d", m_nRedLinkEnergy);
				pClient->Printf(" ");

			 }
			 //CPU�̃f�[�^�����������ޏ���
			 //PrintCPUData();

			}
		}
	}
#endif
}

//=============================================================================
// CPU�̃f�[�^�����������ޏ���
//=============================================================================
void CGame::PrintCPUData(void)
{
	CClient *pClient = CManager::GetClient();
	if (pClient != NULL)
	{
		for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER_CONNECT; nCntPlayer++)
		{
			if (pClient->GetPlayerIdx() != nCntPlayer)
			{
				if (m_bConnect[nCntPlayer] == false)
				{
					//�v���C���[�ԍ�����������
					pClient->Printf("%d", nCntPlayer);
					pClient->Printf(" ");

					//�`�[���̔ԍ�����������
					pClient->Printf("%d", m_pPlayer[nCntPlayer]->GetTeam());
					pClient->Printf(" ");

					//�v���C���[��x,y���̈ʒu����������
					pClient->Printf("%.1f %.1f", m_pPlayer[nCntPlayer]->GetPos().x, m_pPlayer[nCntPlayer]->GetPos().z);
					pClient->Printf(" ");

					//�v���C���[��y���̌�������������
					pClient->Printf("%.1f", m_pPlayer[nCntPlayer]->GetRot().y);
					pClient->Printf(" ");

					//���S���Ă��邩�ǂ���
					if (m_pPlayer[nCntPlayer]->GetDeath() == true)
					{
						pClient->Printf("1");
						pClient->Printf(" ");
					}
					else
					{
						pClient->Printf("0");
						pClient->Printf(" ");
					}

					//�e�𔭎˂��Ă��邩�ǂ�������������
					if (m_pPlayer[nCntPlayer]->GetShoot() == true)
					{//���˂���Ă���ꍇ

						pClient->Printf("1");
						pClient->Printf(" ");

						//�e�̓������ː�����������
						pClient->Printf("%d", m_pPlayer[nCntPlayer]->GetNumShoot());
						pClient->Printf(" ");

						pClient->Printf("%d", m_pPlayer[nCntPlayer]->GetAttack());
						pClient->Printf(" ");

						//�����p�x�̏�����������
						for (int nCntShoot = 0; nCntShoot < m_pPlayer[nCntPlayer]->GetNumShoot() * 2; nCntShoot++)
						{
							pClient->Printf("%.2f %.2f", m_pPlayer[nCntPlayer]->GetAngle(nCntShoot), m_pPlayer[nCntPlayer]->GetAngleV(nCntShoot));
							pClient->Printf(" ");

						}

						//���˂��Ă��Ȃ���Ԃɖ߂�
						m_pPlayer[nCntPlayer]->SetShoot(false);
					}
					else
					{
						pClient->Printf("0");
						pClient->Printf(" ");
					}
				}
			}
		}
	}
}
//=============================================================================
// ����ǂݎ�鏈��
//=============================================================================
void CGame::ReadMessage(void)
{
	CClient *pClient = CManager::GetClient();			//�N���C�A���g�̃|�C���^���
	char *pStr = pClient->GetReceiveData();				//�T�[�o�[����󂯎�������b�Z�[�W���

	if (pClient != NULL && pClient->GetConnect() == true)
	{
		//���o������
		pStr = CServerFunction::HeadPutout(pStr, "");

		if (CServerFunction::Memcmp(pStr, SERVER_CONNECT_DATA) == 0)
		{//�ڑ������������Ă���ꍇ
			pStr += strlen(SERVER_CONNECT_DATA);								//���o��

																				//�ڑ����̓ǂݎ�菈��
			pStr = ReadConnectData(pStr);
		}
		if (CServerFunction::Memcmp(pStr, SERVER_PLAYER_START) == 0)
		{//�v���C���[�̊J�n�������Ă���ꍇ
			pStr += strlen(SERVER_PLAYER_START);							//���o��
			pStr += strlen(" ");

			for (int nCntClient = 0; nCntClient < pClient->GetNumConnect() - 1; nCntClient++)
			{
				if (CServerFunction::Memcmp(pStr, SERVER_PLAYER_DATA) == 0)
				{
					pStr += strlen(SERVER_PLAYER_DATA);
					pStr = ReadPlayerData(pStr);
				}
			}
		}
	}
}

//=============================================================================
// �ڑ��̏���ǂݎ�鏈��
//=============================================================================
char *CGame::ReadConnectData(char *pStr)
{
	//�N���C�A���g�̎擾
	CClient *pClient = CManager::GetClient();
	if (pClient != NULL)
	{
		if (pClient->GetConnect() == true)
		{
			int nWord = 0;		//�����̓��o���p

								//�ڑ������̐ݒu����
			pClient->SetNumConnect(CServerFunction::ReadInt(pStr, ""));
			nWord = CServerFunction::PopString(pStr, "");					//�������J�E���g
			pStr += nWord;													//���o��

																			//�ŏ��̔ԍ��̐ݒu����
			pClient->SetMinIdx(CServerFunction::ReadInt(pStr, ""));
			nWord = CServerFunction::PopString(pStr, "");					//�������J�E���g
			pStr += nWord;													//���o��

																			//�ő�̔ԍ��̐ݒu����
			pClient->SetMaxIdx(CServerFunction::ReadInt(pStr, ""));
			nWord = CServerFunction::PopString(pStr, "");					//�������J�E���g
			pStr += nWord;													//���o��
		}
	}
	return pStr;
}


//=============================================================================
// �v���C���[�̏���ǂݎ�鏈��
//=============================================================================
char *CGame::ReadPlayerData(char *pStr)
{
	D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);									//�ʒu
	D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 modelRotDown = D3DXVECTOR3(0.0f, 0.0f, 0.0f), modelRotUp = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				//���f���̏㔼�g�Ɖ����g�̌���
	D3DXVECTOR3 cameraRot;								//�J�����̌���
	int nPlayerIdx = 0;									//�v���C���[�̔ԍ�
	int nNumConnect = 0;								//�ڑ�����
	int nWord = 0;										//�����̓��o���p
	int nCntClient = 0;									//�N���C�A���g�̃J�E���^�[
	int nTeam = 0;
	float *pAngle = NULL;
	float *pAngleV = NULL;
	bool bShoot = false;
	int nNumShoot = 0;
	int nAttack = 0;
	bool bChat = false;
	//CPlayer::RADIOCHAT radioChat = CPlayer::RADIOCHAT_OK;
	int nBlueLinkEnergy = 0;
	int nRedLinkEnergy = 0;
	int nState = 0;

	//�N���C�A���g�̎擾
	CClient *pClient = CManager::GetClient();
	if (pClient != NULL)
	{
		if (pClient->GetConnect() == true)
		{
			//�v���C���[�ԍ�����
			nPlayerIdx = CServerFunction::ReadInt(pStr, "");
			nWord = CServerFunction::PopString(pStr, "");		//�������J�E���g
			pStr += nWord;										//���o��

																//�`�[�����̑��
			nTeam = CServerFunction::ReadInt(pStr, "");
			nWord = CServerFunction::PopString(pStr, "");
			pStr += nWord;

			//�v���C���[�̈ʒu����
			pos.x = CServerFunction::ReadFloat(pStr, "");
			nWord = CServerFunction::PopString(pStr, "");
			pStr += nWord;
			pos.z = CServerFunction::ReadFloat(pStr, "");
			nWord = CServerFunction::PopString(pStr, "");
			pStr += nWord;

			//�v���C���[�̌�������
			rot.y = CServerFunction::ReadFloat(pStr, "");
			nWord = CServerFunction::PopString(pStr, "");
			pStr += nWord;

			//���f��0�ł̌�������
			modelRotDown.y = CServerFunction::ReadFloat(pStr, "");
			nWord = CServerFunction::PopString(pStr, "");
			pStr += nWord;

			//���f��1�Ԃ̌�������
			modelRotUp.x = CServerFunction::ReadFloat(pStr, "");
			nWord = CServerFunction::PopString(pStr, "");
			pStr += nWord;
			modelRotUp.y = CServerFunction::ReadFloat(pStr, "");
			nWord = CServerFunction::PopString(pStr, "");
			pStr += nWord;
			modelRotUp.z = CServerFunction::ReadFloat(pStr, "");
			nWord = CServerFunction::PopString(pStr, "");
			pStr += nWord;


			//�J�����̌�������
			cameraRot.x = CServerFunction::ReadFloat(pStr, "");
			nWord = CServerFunction::PopString(pStr, "");
			pStr += nWord;
			cameraRot.y = CServerFunction::ReadFloat(pStr, "");
			nWord = CServerFunction::PopString(pStr, "");
			pStr += nWord;
			cameraRot.z = CServerFunction::ReadFloat(pStr, "");
			nWord = CServerFunction::PopString(pStr, "");
			pStr += nWord;

			//���S���Ă��邩���Ă��邩�ǂ�������
			bool bDeath = CServerFunction::ReadBool(pStr, "");
			nWord = CServerFunction::PopString(pStr, "");
			pStr += nWord;

			//�e�𔭎˂��Ă��邩�ǂ�������
			bShoot = CServerFunction::ReadBool(pStr, "");
			nWord = CServerFunction::PopString(pStr, "");
			pStr += nWord;

			if (bShoot == true)
			{
				nNumShoot = CServerFunction::ReadInt(pStr, "");
				nWord = CServerFunction::PopString(pStr, "");
				pStr += nWord;

				pAngle = new float[nNumShoot * 2];
				pAngleV = new float[nNumShoot * 2];

				nAttack = CServerFunction::ReadInt(pStr, "");
				nWord = CServerFunction::PopString(pStr, "");
				pStr += nWord;

				for (int nCntShoot = 0; nCntShoot < nNumShoot * 2; nCntShoot++)
				{
					pAngle[nCntShoot] = CServerFunction::ReadFloat(pStr, "");
					nWord = CServerFunction::PopString(pStr, "");
					pStr += nWord;

					pAngleV[nCntShoot] = CServerFunction::ReadFloat(pStr, "");
					nWord = CServerFunction::PopString(pStr, "");
					pStr += nWord;

				}
			}

			////�`���b�g�����Ă��邩�ǂ�������
			//bChat = CServerFunction::ReadBool(pStr, "");
			//nWord = CServerFunction::PopString(pStr, "");
			//pStr += nWord;

			if (bChat == true)
			{
				//radioChat = (CPlayer::RADIOCHAT)CServerFunction::ReadInt(pStr, "");
				//nWord = CServerFunction::PopString(pStr, "");
				//pStr += nWord;
			}

			if (nPlayerIdx == 0)
			{//�z�X�g�̏ꍇ

			 nState = CServerFunction::ReadInt(pStr, "");
			 nWord = CServerFunction::PopString(pStr, "");
			 pStr += nWord;

			 if (nState == 1)
			 {
				 CResult::SetTeamWin((CResult::TEAM_WIN)CServerFunction::ReadInt(pStr, ""));
			 }
			 else
			 {
				 m_nBlueLinkEnergy = CServerFunction::ReadInt(pStr, "");
				 nWord = CServerFunction::PopString(pStr, "");
				 pStr += nWord;

				 m_nRedLinkEnergy = CServerFunction::ReadInt(pStr, "");
				 nWord = CServerFunction::PopString(pStr, "");
				 pStr += nWord;
			 }
			 //pStr = ReadCPUData(pStr);

			}
			if (bDeath == true)
			{
				m_pPlayer[nPlayerIdx]->GetDeath() = true;
			}

			if (nPlayerIdx != pClient->GetPlayerIdx())
			{//�v���C���[�̔ԍ����N���C�A���g�ԍ��ƈႤ�ꍇ
				if (m_pPlayer[nPlayerIdx] != NULL)
				{
					//�`�[�������̐ݒu����
					//m_pPlayer[nPlayerIdx]->SetTeam(nTeam);

					SetPlayerData(nPlayerIdx, pos, rot, modelRotUp, modelRotDown, cameraRot);

					if (bShoot == true)
					{//�e�𔭎˂��Ă�int��ꍇ
						CreatePlayerBullet(nPlayerIdx, nNumShoot, nAttack, cameraRot, pAngle, pAngleV);
					}

					if (bChat == true)
					{
						//SetChatData(nPlayerIdx, (int)radioChat);
					}
					if (nState == 1)
					{
						m_state = STATE_END;
					}
				}
			}
		}
	}


	if (pAngle != NULL)
	{
		delete[] pAngle;
		pAngle = NULL;
	}
	if (pAngleV != NULL)
	{
		delete[] pAngleV;
		pAngleV = NULL;
	}

	return pStr;
}
//=============================================================================
// CPU�v���C���[����ǂݎ�鏈��
//=============================================================================
char *CGame::ReadCPUData(char *pStr)
{
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER_CONNECT; nCntPlayer++)
	{
		if (m_bConnect[nCntPlayer] == false)
		{
			D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);									//�ʒu
			D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			D3DXVECTOR3 modelRotDown = D3DXVECTOR3(0.0f, 0.0f, 0.0f), modelRotUp = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				//���f���̏㔼�g�Ɖ����g�̌���
			D3DXVECTOR3 cameraRot;								//�J�����̌���
			int nPlayerIdx = 0;									//�v���C���[�̔ԍ�
			int nNumConnect = 0;								//�ڑ�����
			int nWord = 0;										//�����̓��o���p
			int nCntClient = 0;									//�N���C�A���g�̃J�E���^�[
			int nTeam = 0;
			float *pAngle = NULL;
			float *pAngleV = NULL;
			bool bShoot = false;
			int nNumShoot = 0;
			int nAttack = 0;

			//�ԍ�����
			nPlayerIdx = CServerFunction::ReadInt(pStr, "");
			nWord = CServerFunction::PopString(pStr, "");		//�������J�E���g
			pStr += nWord;										//���o��

																//�`�[�����̑��
			nTeam = CServerFunction::ReadInt(pStr, "");
			nWord = CServerFunction::PopString(pStr, "");
			pStr += nWord;

			//�v���C���[�̈ʒu����
			pos.x = CServerFunction::ReadFloat(pStr, "");
			nWord = CServerFunction::PopString(pStr, "");
			pStr += nWord;
			pos.z = CServerFunction::ReadFloat(pStr, "");
			nWord = CServerFunction::PopString(pStr, "");
			pStr += nWord;

			//�v���C���[�̌�������
			rot.y = CServerFunction::ReadFloat(pStr, "");
			nWord = CServerFunction::PopString(pStr, "");
			pStr += nWord;

			//���S���Ă��邩�ǂ�������
			bool bDeath = CServerFunction::ReadBool(pStr, "");
			nWord = CServerFunction::PopString(pStr, "");
			pStr += nWord;

			//�e�𔭎˂��Ă��邩�ǂ�������
			bShoot = CServerFunction::ReadBool(pStr, "");
			nWord = CServerFunction::PopString(pStr, "");
			pStr += nWord;

			if (bShoot == true)
			{
				nNumShoot = CServerFunction::ReadInt(pStr, "");
				nWord = CServerFunction::PopString(pStr, "");
				pStr += nWord;

				pAngle = new float[nNumShoot * 2];
				pAngleV = new float[nNumShoot * 2];

				nAttack = CServerFunction::ReadInt(pStr, "");
				nWord = CServerFunction::PopString(pStr, "");
				pStr += nWord;

				for (int nCntShoot = 0; nCntShoot < nNumShoot * 2; nCntShoot++)
				{
					pAngle[nCntShoot] = CServerFunction::ReadFloat(pStr, "");
					nWord = CServerFunction::PopString(pStr, "");
					pStr += nWord;

					pAngleV[nCntShoot] = CServerFunction::ReadFloat(pStr, "");
					nWord = CServerFunction::PopString(pStr, "");
					pStr += nWord;

				}
			}

			SetCPUData(nPlayerIdx, pos, rot);
			if (bShoot == true)
			{
				CreateCPUBullet(nPlayerIdx, nNumShoot, nAttack, cameraRot, pAngle, pAngleV);
			}


		}
	}
	return pStr;
}

//=============================================================================
// �v���C���[�̏��̐ݒu����
//=============================================================================
void CGame::SetPlayerData(int nPlayerIdx, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 modelRotUp, D3DXVECTOR3 modelRotDown, D3DXVECTOR3 cameraRot)
{
	//�v���C���[�̈ʒu�̐ݒu����
	m_pPlayer[nPlayerIdx]->SetPos(pos);

	float fDiffRot;
	float fAngle = D3DX_PI + cameraRot.y;
	float fRotDest = m_pPlayer[nPlayerIdx]->GetRotDest();
	// �ړI�̊p�x�̒���
	if (fRotDest > D3DX_PI) { fRotDest -= D3DX_PI * 2.0f; }
	if (fRotDest < -D3DX_PI) { fRotDest += D3DX_PI * 2.0f; }

	// �ړI�̊p�x�ւ̍���
	fDiffRot = fRotDest - rot.y;
	if (fDiffRot > D3DX_PI) { fDiffRot -= D3DX_PI * 2.0f; }
	if (fDiffRot < -D3DX_PI) { fDiffRot += D3DX_PI * 2.0f; }

	// �p�x�̍X�V
	rot.y += fDiffRot * 0.05f;
	if (rot.y > D3DX_PI) { rot.y -= D3DX_PI * 2.0f; }
	if (rot.y < -D3DX_PI) { rot.y += D3DX_PI * 2.0f; }

	float fCameraAngle = fAngle - rot.y;

	// �����̒���
	if (fCameraAngle > D3DX_PI) { fCameraAngle -= D3DX_PI * 2.0f; }
	if (fCameraAngle < -D3DX_PI) { fCameraAngle += D3DX_PI * 2.0f; }

	if (fRotDest <= D3DX_PI * 0.5f && fRotDest >= D3DX_PI * -0.5f)
	{// �����g�̓�����i�s�����ɍ��킹��
		m_pPlayer[nPlayerIdx]->GetModel(0)->SetRot(D3DXVECTOR3(modelRotDown.x, rot.y + fCameraAngle, modelRotDown.z));
		m_pPlayer[nPlayerIdx]->GetModel(1)->SetRot(D3DXVECTOR3(-cameraRot.x + (D3DX_PI * 0.5f), fCameraAngle - fAngle, modelRotUp.z));
	}
	else
	{// �΂ߌ������̂Ƃ�
		m_pPlayer[nPlayerIdx]->GetModel(1)->SetRot(D3DXVECTOR3(-cameraRot.x + (D3DX_PI * 0.5f), fCameraAngle - (fAngle - D3DX_PI), modelRotUp.z));
	}

	//�v���C���[�̌����̐ݒu����
	m_pPlayer[nPlayerIdx]->SetRot(rot);

	//�v���C���[�̖ړI�̊p�x�̐ݒu����
	m_pPlayer[nPlayerIdx]->SetRotDest(fRotDest);
}

//=============================================================================
// �v���C���[�̒e�̐�������
//=============================================================================
void CGame::CreatePlayerBullet(int nPlayerIdx, int nNumShoot, int nAttack, D3DXVECTOR3 cameraRot, float *pAngle, float *pAngleV)
{
	for (int nCntShoot = 0; nCntShoot < nNumShoot; nCntShoot++)
	{
		// �e�̐���
		D3DXMATRIX mtxCanon = m_pPlayer[nPlayerIdx]->GetModel(2)->GetMtxWorld();
		D3DXVECTOR3 posCanon = D3DXVECTOR3(mtxCanon._41, mtxCanon._42, mtxCanon._43) + D3DXVECTOR3(sinf(cameraRot.y) * 30.0f, cosf(cameraRot.x) * 30.0f, cosf(cameraRot.y) * 30.0f);
		CBulletPlayer::Create(posCanon, pAngle[nCntShoot * 2], pAngleV[nCntShoot * 2], nAttack, m_pPlayer[nPlayerIdx]->GetTeam());
		mtxCanon = m_pPlayer[nPlayerIdx]->GetModel(3)->GetMtxWorld();
		posCanon = D3DXVECTOR3(mtxCanon._41, mtxCanon._42, mtxCanon._43) + D3DXVECTOR3(sinf(cameraRot.y) * 30.0f, cosf(cameraRot.x) * 30.0f, cosf(cameraRot.y) * 30.0f);
		CBulletPlayer::Create(posCanon, pAngle[nCntShoot * 2 + 1], pAngleV[nCntShoot * 2 + 1], nAttack, m_pPlayer[nPlayerIdx]->GetTeam());

		//�e�𔭎˂��Ă��邩�ǂ����̐ݒu����
		m_pPlayer[nPlayerIdx]->SetShoot(false);
	}
}

//=============================================================================
// �`���b�g���̐ݒu����
//=============================================================================
void CGame::SetChatData(int nPlayerIdx, int radioChat)
{

	//switch (nPlayerIdx)
	//{
	//case 0:
	//	m_pPlayer[1]->SetRadioChat((CPlayer::RADIOCHAT)radioChat);
	//	m_pPlayer[1]->SetAllyChat(true);
	//	break;
	//case 1:
	//	m_pPlayer[0]->SetRadioChat((CPlayer::RADIOCHAT)radioChat);
	//	m_pPlayer[0]->SetAllyChat(true);
	//	break;
	//case 2:
	//	m_pPlayer[3]->SetRadioChat((CPlayer::RADIOCHAT)radioChat);
	//	m_pPlayer[3]->SetAllyChat(true);
	//	break;
	//case 3:
	//	m_pPlayer[2]->SetRadioChat((CPlayer::RADIOCHAT)radioChat);
	//	m_pPlayer[2]->SetAllyChat(true);
	//	break;
	//}
}

//=============================================================================
// CPU�v���C���[�̏��ݒu����
//=============================================================================
void CGame::SetCPUData(int nPlayerIdx, D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	//�v���C���[�̈ʒu�̐ݒu����
	m_pPlayer[nPlayerIdx]->SetPos(pos);
	//�v���C���[�̌����̐ݒu����
	m_pPlayer[nPlayerIdx]->SetRot(rot);
}

//=============================================================================
// CPU�v���C���[�̒e�̐�������
//=============================================================================
void CGame::CreateCPUBullet(int nPlayerIdx, int nNumShoot, int nAttack, D3DXVECTOR3 cameraRot, float *pAngle, float *pAngleV)
{
	for (int nCntShoot = 0; nCntShoot < nNumShoot; nCntShoot++)
	{
		// �e�̐���
		D3DXMATRIX mtxCanon = m_pPlayer[nPlayerIdx]->GetModel(2)->GetMtxWorld();
		D3DXVECTOR3 posCanon = D3DXVECTOR3(mtxCanon._41, mtxCanon._42, mtxCanon._43) + D3DXVECTOR3(sinf(cameraRot.y) * 30.0f, cosf(cameraRot.x) * 30.0f, cosf(cameraRot.y) * 30.0f);
		CBulletPlayer::Create(posCanon, pAngle[nCntShoot * 2], pAngleV[nCntShoot * 2], nAttack, m_pPlayer[nPlayerIdx]->GetTeam());
		mtxCanon = m_pPlayer[nPlayerIdx]->GetModel(3)->GetMtxWorld();
		posCanon = D3DXVECTOR3(mtxCanon._41, mtxCanon._42, mtxCanon._43) + D3DXVECTOR3(sinf(cameraRot.y) * 30.0f, cosf(cameraRot.x) * 30.0f, cosf(cameraRot.y) * 30.0f);
		CBulletPlayer::Create(posCanon, pAngle[nCntShoot * 2 + 1], pAngleV[nCntShoot * 2 + 1], nAttack, m_pPlayer[nPlayerIdx]->GetTeam());

		//�e�𔭎˂��Ă��邩�ǂ����̐ݒu����
		m_pPlayer[nPlayerIdx]->SetShoot(false);
	}
}

//=============================================================================
// ���X�|�[���ʒu�̓ǂݍ���
//=============================================================================
void CGame::LoadRespawnPos(void)
{
	FILE *pFile;	//�t�@�C���̃|�C���^

					//�t�@�C���̓ǂݍ���
	pFile = fopen(GAME_INI, "r");

	if (pFile == NULL)
	{//�t�@�C�����ǂݍ��߂Ă��Ȃ��ꍇ
		return;
	}
	else if (pFile != NULL)
	{//�t�@�C�����ǂݍ��߂��ꍇ
		char aStr[128] = "\0";

		while (1)
		{//���[�v
			fscanf(pFile, "%s", aStr);
			if (strcmp(aStr, GAME_SCRIPT) == 0)
			{//�X�N���v�g�̊J�n�������Ă���ꍇ
				while (strcmp(aStr, GAME_END_SCRIPT) != 0)
				{//�X�N���v�g�̏I���������܂Ń��[�v
					fscanf(pFile, "%s", aStr);
					if (strcmp(aStr, GAME_BLUE) == 0)
					{//�T�[�o�[��IP�A�h���X�������Ă���ꍇ
						while (strcmp(aStr, GAME_BLUE_END) != 0)
						{
							fscanf(pFile, "%s", aStr);

							if (strcmp(aStr, GAME_SPAWN_A) == 0)
							{
								fscanf(pFile, " = %f %f %f", &m_aRespawnPos[0][0].x, &m_aRespawnPos[0][0].y, &m_aRespawnPos[0][0].z);
							}
							if (strcmp(aStr, GAME_SPAWN_B) == 0)
							{
								fscanf(pFile, " = %f %f %f", &m_aRespawnPos[0][1].x, &m_aRespawnPos[0][1].y, &m_aRespawnPos[0][1].z);
							}
							if (strcmp(aStr, GAME_SPAWN_C) == 0)
							{
								fscanf(pFile, " = %f %f %f", &m_aRespawnPos[0][2].x, &m_aRespawnPos[0][2].y, &m_aRespawnPos[0][2].z);
							}
							if (strcmp(aStr, GAME_SPAWN_D) == 0)
							{
								fscanf(pFile, " = %f %f %f", &m_aRespawnPos[0][3].x, &m_aRespawnPos[0][3].y, &m_aRespawnPos[0][3].z);
							}
						}
					}
					if (strcmp(aStr, GAME_RED) == 0)
					{//�T�[�o�[��IP�A�h���X�������Ă���ꍇ
						while (strcmp(aStr, GAME_RED_END) != 0)
						{
							fscanf(pFile, "%s", aStr);

							if (strcmp(aStr, GAME_SPAWN_A) == 0)
							{
								fscanf(pFile, " = %f %f %f", &m_aRespawnPos[1][0].x, &m_aRespawnPos[1][0].y, &m_aRespawnPos[1][0].z);
							}
							if (strcmp(aStr, GAME_SPAWN_B) == 0)
							{
								fscanf(pFile, " = %f %f %f", &m_aRespawnPos[1][1].x, &m_aRespawnPos[1][1].y, &m_aRespawnPos[1][1].z);
							}
							if (strcmp(aStr, GAME_SPAWN_C) == 0)
							{
								fscanf(pFile, " = %f %f %f", &m_aRespawnPos[1][2].x, &m_aRespawnPos[1][2].y, &m_aRespawnPos[1][2].z);
							}
							if (strcmp(aStr, GAME_SPAWN_D) == 0)
							{
								fscanf(pFile, " = %f %f %f", &m_aRespawnPos[1][3].x, &m_aRespawnPos[1][3].y, &m_aRespawnPos[1][3].z);
							}
						}
					}

				}
				break;
			}
		}
	}

}
