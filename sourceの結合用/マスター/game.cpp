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

#include "shadow.h"

#include "particle.h"

#include "AI.h"

#include "damageDirection.h"
#include "nodeDataFiler.h"

#include "matching.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define FIELD_MODEL_NAME	"data/MODEL/map_no_floor.x"
#define FLOOR_MODEL_NAME	"data/MODEL/floor.x"
#define SKY_MODEL_NAME		"data/MODEL/skydome.x"
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
CDamageDirection *CGame::m_pDamageDirection = NULL;
CNodeDataFiler *CGame::m_pNodeFiler = NULL;			// �}�b�v�f�[�^�N���X�̃|�C���^�ϐ�

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
	m_pField = NULL;
	m_nBlueLinkEnergy = 0;
	m_nRedLinkEnergy = 0;
	m_pFadeBG = NULL;
	m_pEndBG = NULL;
	m_pEndResultLogo = NULL;
	m_pEndMyTeamLogo = NULL;
	m_pFloor = NULL;

	for (int nCntConnect = 0; nCntConnect < MAX_PLAYER_CONNECT; nCntConnect++)
	{
		//m_bConnect[nCntConnect] = false;
		m_bPlayerDeath[nCntConnect] = false;
	}

	for (int nCntTeam = 0; nCntTeam < NUM_TEAM; nCntTeam++)
	{
		m_pEndScore[nCntTeam] = NULL;
		m_pEndTeamEmbrem[nCntTeam] = NULL;

		for (int nCntRespawn = 0; nCntRespawn < NUM_RESPAWN_POS; nCntRespawn++)
		{
			m_aRespawnPos[nCntTeam][nCntRespawn] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		}
	}

	for (int nCntConnect = 0; nCntConnect < 2; nCntConnect++)
	{
		for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER_CONNECT; nCntPlayer++)
		{
			m_bAIDeath[nCntConnect][nCntPlayer] = false;
		}
	}

	for (int nCntKill = 0; nCntKill < NUM_KILL_LOG; nCntKill++)
	{
		m_apKillLogBase[nCntKill] = NULL;
		m_bLog[nCntKill] = false;
		m_nKillIdx[nCntKill] = 0;
		m_nDeathIdx[nCntKill] = 0;
		m_nCntDrawLog[nCntKill] = 0;

		for (int nCntPlayer = 0; nCntPlayer < NUM_KILL_LOG_PLAYER; nCntPlayer++)
		{
			m_apKillLogPlayerIcon[nCntKill][nCntPlayer] = NULL;
			m_apKillLogPlayerIdx[nCntKill][nCntPlayer] = NULL;
		}
	}

	for (int nCntPlayer = 0; nCntPlayer < NUM_KILL_LOG_PLAYER; nCntPlayer++)
	{
		m_playerType[nCntPlayer] = TYPE_PLAYER;

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
	m_nBlueLinkEnergy = MAX_LINKENERGY;
	m_nRedLinkEnergy = MAX_LINKENERGY;

	//���X�|�[���ʒu�̓ǂݍ���
	LoadRespawnPos();

	// �ǂݍ���
	CMotionManager::Load();

	m_pFloor = CModel::Create();
	m_pFloor->SetModel(FLOOR_MODEL_NAME, false);
	m_pFloor->SetPos(D3DXVECTOR3(-15.0f, 0.0f, 30.0f));

	m_pField = CModel::Create();
	m_pField->SetModel(FIELD_MODEL_NAME);
	m_pField->SetPos(D3DXVECTOR3(-15.0f, 0.0f, 30.0f));
	m_pField->SetShader();

	m_pSky = CModel::Create();
	m_pSky->SetModel(SKY_MODEL_NAME, false);
	m_pSky->SetPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	//=====================================================================================
	// �}�b�v�f�[�^�t�@�C���[�̐���
	//=====================================================================================
	if (m_pNodeFiler == NULL)
	{// NULL�`�F�b�N
		m_pNodeFiler = CNodeDataFiler::Create();
	}

	// �v���C���[�̐���
	CreatePlayer();

	//CShadow::Create();

	// �e�̓����蔻��N���X�̐���
	CBulletCollision::Create();

	// �}�b�v�̓����蔻��̓ǂݍ���
	CCollision::Load();

	////****************************************
	//// 2DUI�����i�t���[���j
	////****************************************
	//// ����t���[��
	//CUI_TEXTURE::Create(D3DXVECTOR3(1100.0f, 650.0f, 0.0f), 350.0f, 120.0f, CUI_TEXTURE::UIFLAME_WEAPON);		// ����

	//																											// �v���C���[�̗̓t���[��
	//CUI_TEXTURE::Create(D3DXVECTOR3(180.0f, 650.0f, 0.0f), 350.0f, 125.0f, CUI_TEXTURE::UIFLAME_PLAYER_HP);	// ����

	//																										// AI�`�P�b�g���t���[��
	//CUI_TEXTURE::Create(D3DXVECTOR3(125.0f, 530.0f, 0.0f), 230.0f, 100.0f, CUI_TEXTURE::UIFLAME_DRONE);		// AI01
	//CUI_TEXTURE::Create(D3DXVECTOR3(125.0f, 420.0f, 0.0f), 230.0f, 100.0f, CUI_TEXTURE::UIFLAME_WORKER);	// AI02

	//																										// �`�[���̃`�P�b�g���t���[��
	//CUI_TEXTURE::Create(D3DXVECTOR3(80.0f, 40.0f, 0.0f), 120.0f, 50.0f, CUI_TEXTURE::UIFLAME_TEAM_BLUE);	// BLUE
	//CUI_TEXTURE::Create(D3DXVECTOR3(80.0f, 100.0f, 0.0f), 120.0f, 50.0f, CUI_TEXTURE::UIFLAME_TEAM_RED);	// RED

	//// �p�[�g�؂�ւ��e�N�X�`��
	////CUI_TEXTURE::Create(D3DXVECTOR3(125.0f, 50.0f, 0.0f), 200.0f, 90.0f, CUI_TEXTURE::UIFLAME_STRATEGY_PART);	// �X�g���e�W�[�p�[�g
	////CUI_TEXTURE::Create(D3DXVECTOR3(165.0f, 90.0f, 0.0f), 200.0f, 90.0f, CUI_TEXTURE::UIFLAME_ACTION_PART);		// �A�N�V�����p�[�g

	//// �p�[�g�؂�ւ��i�`�F���W�j
	////CUI_TEXTURE::Create(D3DXVECTOR3(50.0f, 135.0f, 0.0f), 100.0f, 110.0f, CUI_TEXTURE::UIFLAME_CHANGE);

	////****************************************
	//// UI�����i�����j
	////****************************************
	//CUI_NUMBER::Create(D3DXVECTOR3(370.0f, 40.0f, 0.0f), 430.0f, 30.0f, 0.0f, CUI_NUMBER::UI_NUMTYPE_BLUE, 0, NUMTEX_UV_X, NUMTEX_UV_Y);							// BLUE�`�[���`�P�b�g
	//CUI_NUMBER::Create(D3DXVECTOR3(370.0f, 100.0f, 0.0f), 430.0f, 30.0f, 0.0f, CUI_NUMBER::UI_NUMTYPE_RED, 0, NUMTEX_UV_X, NUMTEX_UV_Y);							// RED�`�[���`�P�b�g
	//CUI_NUMBER::Create(D3DXVECTOR3(970.0f, 660.0f, 0.0f), 150.0f, 90.0f, 60.0f, CUI_NUMBER::UI_NUMTYPE_REMAINBULLET, 0, NUMTEX_UV_X, NUMTEX_UV_Y);		// �c�e
	//CUI_NUMBER::Create(D3DXVECTOR3(110.0f, 530.0f, 0.0f), 130.0f, 70.0f, 50.0f, CUI_NUMBER::UI_NUMTYPE_DRONE_HP, 0, NUMTEX_UV_X, NUMTEX_UV_Y);				// �h���[�����C�t
	//CUI_NUMBER::Create(D3DXVECTOR3(110.0f, 420.0f, 0.0f), 130.0f, 70.0f, 50.0f, CUI_NUMBER::UI_NUMTYPE_WORKER_HP, 0, NUMTEX_UV_X, NUMTEX_UV_Y);			// ���[�J�[���C�t
	//CUI_NUMBER::Create(D3DXVECTOR3(195.0f, 650.0f, 0.0f), 170.0f, 110.0f, 70.0f, CUI_NUMBER::UI_NUMTYPE_PLAYER_HP, 0, NUMTEX_UV_X, NUMTEX_UV_Y);			// �v���C���[���C�t

	if (NULL == m_pDamageDirection)
	{
		m_pDamageDirection = CDamageDirection::Create();
	}

	m_state = STATE_STARTING;
	CManager::GetCamera()->Init();
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
	//CEnemy::Unload();

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

	if (NULL != m_pFloor)
	{// �n�ʂ̔j��
		m_pFloor->Uninit();
		delete m_pFloor;
		m_pFloor = NULL;
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

	if (NULL != m_pDamageDirection)
	{// �U�����󂯂������\���̔j��
		m_pDamageDirection->Uninit();
		m_pDamageDirection = NULL;
	}

	if (m_pNodeFiler != NULL)
	{// �}�b�v�f�[�^�t�@�C���[�̔j��
		m_pNodeFiler->Uninit();
		m_pNodeFiler = NULL;
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
			if (CManager::GetClient()->GetGameMode() == CClient::GAME_MODE_PLAYER)
			{
				/*if (CManager::GetClient()->GetPlayerIdx() == 0)
				{
					if (m_state == STATE_NORMAL)
					{
						m_state = STATE_END;
					}
				}*/

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

	if (NULL != m_pMouse)
		CDebugProc::Print("�}�E�X�J�[�\���F%.2f %.2f", m_pMouse->GetPos().x, m_pMouse->GetPos().z);

	// �t�F�[�h�̎擾
	CFade::FADE fade = CFade::GetFade();

#ifdef _DEBUG
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
			}
		}
	}
#endif // !_DEBUG

	//�L�����O�̐�������
	CreateKillLog();

	//�L�����O�̍X�V����
	UpdateKillLog();

	//��ԕʂ̏���
	switch (m_state)
	{
	case STATE_STARTING:
		break;
	case STATE_STARTING_FADE_OUT:
		StartingFadeOutUpdate();
		break;
	case STATE_STARTING_FADE_IN:
		StartingFadeInUpdate();
		break;
	case STATE_NORMAL:
		NomalUpdate();
		break;
	case STATE_END:
		EndUpdate();
	break;
	case STATE_END_FADE_OUT:
		EndFadeOutUpdate();
		break;
	case STATE_FADE:
		FadeUpdate();
		break;
	}

	CDebugProc::Print("�Q�[��");
}

//=============================================================================
// �Q�[���`�揈��
//=============================================================================
void CGame::Draw(void)
{
	if (NULL != m_pFloor)
	{// �n�ʂ̕`��
		m_pFloor->Draw();
	}

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
// �J�n�O�̃t�F�[�h�A�E�g�̍X�V����
//=============================================================================
void CGame::StartingFadeOutUpdate(void)
{
	if (m_pFadeBG == NULL)
	{//NULL�̏ꍇ
		//�w�i�̐���
		m_pFadeBG = CUI_TEXTURE::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), SCREEN_WIDTH, SCREEN_HEIGHT, CUI_TEXTURE::UIFLAME_NONE);
		m_pFadeBG->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));
	}
	else
	{//NULL�ł͂Ȃ��ꍇ
		//�F���擾�������x�����Z
		D3DXCOLOR col = m_pFadeBG->GetColor();
		col.a += 0.01f;

		//�F�̐ݒu����
		m_pFadeBG->SetColor(col);

		if (col.a >= 1.0f)
		{//�����x���P�ȏ�̏ꍇ
			//�J�n�O�̃t�F�[�h�C���ɂ���
			m_state = STATE_STARTING_FADE_IN;
		}
	}
}

//=============================================================================
// �J�n�O�̃t�F�[�h�C���̍X�V����
//=============================================================================
void CGame::StartingFadeInUpdate(void)
{
	if (m_pFadeBG != NULL)
	{//NULL�ł͂Ȃ��ꍇ
	 //�F���擾�������x�����Z
		D3DXCOLOR col = m_pFadeBG->GetColor();
		col.a -= 0.01f;

		//�F�̐ݒu����
		m_pFadeBG->SetColor(col);
		if (col.a <= 0.0f)
		{//�����x���O�ȉ��̏ꍇ
		 //�ʏ��Ԃɂ���
			m_state = STATE_NORMAL;

			//�w�i�̊J��
			if (m_pFadeBG != NULL)
			{
				m_pFadeBG->Uninit();
				m_pFadeBG = NULL;
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
			CUI_TEXTURE::Create(D3DXVECTOR3(80.0f, 40.0f, 0.0f), 120.0f, 50.0f, CUI_TEXTURE::UIFLAME_TEAM_BLUE);	// BLUE
			CUI_TEXTURE::Create(D3DXVECTOR3(80.0f, 100.0f, 0.0f), 120.0f, 50.0f, CUI_TEXTURE::UIFLAME_TEAM_RED);	// RED

																													//****************************************
																													// UI�����i�����j
																													//****************************************
			CUI_NUMBER::Create(D3DXVECTOR3(370.0f, 40.0f, 0.0f), 430.0f, 30.0f, 0.0f, CUI_NUMBER::UI_NUMTYPE_BLUE, 0, NUMTEX_UV_X, NUMTEX_UV_Y);							// BLUE�`�[���`�P�b�g
			CUI_NUMBER::Create(D3DXVECTOR3(370.0f, 100.0f, 0.0f), 430.0f, 30.0f, 0.0f, CUI_NUMBER::UI_NUMTYPE_RED, 0, NUMTEX_UV_X, NUMTEX_UV_Y);							// RED�`�[���`�P�b�g
			CUI_NUMBER::Create(D3DXVECTOR3(1120.0f, 660.0f, 0.0f), 150.0f, 90.0f, 60.0f, CUI_NUMBER::UI_NUMTYPE_REMAINBULLET, 0, NUMTEX_UV_X, NUMTEX_UV_Y);		// �c�e
			CUI_NUMBER::Create(D3DXVECTOR3(260.0f, 530.0f, 0.0f), 100.0f, 60.0f, 40.0f, CUI_NUMBER::UI_NUMTYPE_DRONE_HP, 0, NUMTEX_UV_X, NUMTEX_UV_Y);				// �h���[�����C�t
			CUI_NUMBER::Create(D3DXVECTOR3(260.0f, 420.0f, 0.0f), 100.0f, 60.0f, 40.0f, CUI_NUMBER::UI_NUMTYPE_WORKER_HP, 0, NUMTEX_UV_X, NUMTEX_UV_Y);			// ���[�J�[���C�t
			CUI_NUMBER::Create(D3DXVECTOR3(385.0f, 650.0f, 0.0f), 150.0f, 100.0f, 60.0f, CUI_NUMBER::UI_NUMTYPE_PLAYER_HP, 0, NUMTEX_UV_X, NUMTEX_UV_Y);			// �v���C���[���C�t
		}
	}
}

//=============================================================================
// �ʏ�̍X�V����
//=============================================================================
void CGame::NomalUpdate (void)
{
	if (CMenu::GetMode() == CMenu::MODE_MULTI)
	{//�}���`�v���C�̏ꍇ
		if (CManager::GetClient() != NULL)
		{//NULL�ł͂Ȃ��ꍇ
			if (CManager::GetClient()->GetPlayerIdx() == 0)
			{//�v���C���[�ԍ����O�̏ꍇ
				if (m_nBlueLinkEnergy <= 0)
				{//�u���[�`�[���̃����N�G�l���M�[���O�ȉ��̏ꍇ
					//�I����Ԃɂ���
					m_state = STATE_END;
					//���b�h�`�[���̏����ɂ���
					CResult::SetTeamWin(CResult::TEAM_WIN_RED);
				}
				else if (m_nRedLinkEnergy <= 0)
				{//���b�h�`�[���̃����N�G�l���M�[��0�ȉ��̏ꍇ
					//�I����Ԃɂ���
					m_state = STATE_END;
					//�u���[�`�[���̏����ɂ���
					CResult::SetTeamWin(CResult::TEAM_WIN_BLUE);
				}
			}
		}
	}
	else if (CMenu::GetMode() == CMenu::MODE_SINGLE)
	{//�V���O���v���C�̏ꍇ
		if (m_nBlueLinkEnergy <= 0)
		{//�u���[�`�[���̃����N�G�l���M�[���O�ȉ��̏ꍇ
			//�I����Ԃɂ���
			m_state = STATE_END;
			//���b�h�`�[���̏����ɂ���
			CResult::SetTeamWin(CResult::TEAM_WIN_RED);
		}
		else if (m_nRedLinkEnergy <= 0)
		{//���b�h�`�[���̃����N�G�l���M�[��0�ȉ��̏ꍇ
			//�I����Ԃɂ���
			m_state = STATE_END;
			//�u���[�`�[���̏����ɂ���
			CResult::SetTeamWin(CResult::TEAM_WIN_BLUE);
		}
	}
	CDebugProc::Print("�ڑ��� ");
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER_CONNECT; nCntPlayer++)
	{
		switch (CMatching::GetConnect(nCntPlayer))
		{
		case true:
			CDebugProc::Print("1");
			break;
		case false:
			CDebugProc::Print("0");
			break;
		}
	}
}

//=============================================================================
// �I���̍X�V����
//=============================================================================
void CGame::EndUpdate(void)
{
	//UI�̔j��
	CScene::UIUninit();

	//�o�O�΍��p�Ƀv���C���[�t�h�̔j������
	if (CMenu::MODE_MULTI == CMenu::GetMode())
	{
		if (CManager::GetClient() != NULL)
		{
			m_pPlayer[CManager::GetClient()->GetPlayerIdx()]->ReleasePlayerUI();
		}
	}
	else if (CMenu::MODE_SINGLE == CMenu::GetMode())
	{
		m_pPlayer[0]->ReleasePlayerUI();
	}

	if (m_pEndBG == NULL)
	{//NULL�̏ꍇ
		//�w�i�̐���
		m_pEndBG = CUI_TEXTURE::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), SCREEN_WIDTH, SCREEN_HEIGHT, CUI_TEXTURE::UIFLAME_NONE);

		if (CResult::GetTeamWin() == CResult::TEAM_WIN_BLUE)
		{//�u���[�`�[���̏����̏ꍇ
			//�w�i�̐F��ɂ���
			m_pEndBG->SetColor(D3DXCOLOR(0.0f, 0.0f, 1.0f, 0.0f));
		}
		else if (CResult::GetTeamWin() == CResult::TEAM_WIN_RED)
		{//���b�h�`�[���̏����̏ꍇ
			//�w�i�̐F��Ԃɂ���
			m_pEndBG->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.0f));
		}
		//�I���̃t�F�[�h�A�E�g�ɂ���
		m_state = STATE_END_FADE_OUT;
	}
}

//=============================================================================
// �I���̃t�F�[�h�A�E�g�̍X�V����
//=============================================================================
void CGame::EndFadeOutUpdate(void)
{
	if (m_pEndBG != NULL)
	{//NULL�ł͂Ȃ��ꍇ
		//�F���擾�������x�����Z
		D3DXCOLOR col = m_pEndBG->GetColor();
		col.a += 0.01f;

		//�F�̐ݒu����
		m_pEndBG->SetColor(col);

		if (col.a >= 0.5f)
		{//�����x��0.5�ȏ�̏ꍇ
			//�t�F�[�h��Ԃɂ���
			m_state = STATE_FADE;

			//���ʕ\���ɕK�v��UI�𐶐�

			if (m_pEndScore[0] == NULL)
			{//NULL�̏ꍇ
				//�u���[�`�[���̃X�R�A�𐶐�
				m_pEndScore[0] = CUI_NUMBER::Create(D3DXVECTOR3(515.0f, 500.0f, 0.0f), 150.0f, 150.0f, 75.0f, CUI_NUMBER::UI_NUMTYPE_NONE, 0, 1, 3);
				//m_pEndScore[0]->SetNumDisPlay(m_nBlueLinkEnergy, CUI_NUMBER::UI_NUMTYPE_NONE);
				m_pEndScore[0]->SetNum(m_nBlueLinkEnergy);
			}
			if (m_pEndScore[1] == NULL)
			{//NULL�̏ꍇ
				//���b�h�`�[���̃X�R�A�𐶐�
				m_pEndScore[1] = CUI_NUMBER::Create(D3DXVECTOR3(950.0f, 500.0f, 0.0f), 150.0f, 150.0f, 75.0f, CUI_NUMBER::UI_NUMTYPE_NONE, 0, 1, 3);
				//m_pEndScore[1]->SetNumDisPlay(m_nRedLinkEnergy, CUI_NUMBER::UI_NUMTYPE_NONE);
				m_pEndScore[1]->SetNum(m_nRedLinkEnergy);
			}

			//�`�[���G���u�����𐶐�
			if (m_pEndTeamEmbrem[0] == NULL) { m_pEndTeamEmbrem[0] = CUI_TEXTURE::Create(D3DXVECTOR3(180.0f, 500.0f, 0.0f), 200.0f, 200.0f, CUI_TEXTURE::UIFLAME_RESULT_BLUE_EMBREM); }
			if (m_pEndTeamEmbrem[1] == NULL) { m_pEndTeamEmbrem[1] = CUI_TEXTURE::Create(D3DXVECTOR3(1130.0f, 500.0f, 0.0f), 200.0f, 200.0f, CUI_TEXTURE::UIFLAME_RESULT_RED_EMBREM); }


			if (CMenu::GetMode() == CMenu::MODE_SINGLE)
			{//�V���O���v���C�̏ꍇ
				if (m_pEndResultLogo == NULL)
				{//NULL�̏ꍇ
					//���s�̃��S�𐶐�
					if (CResult::GetTeamWin() == CResult::TEAM_WIN_BLUE) { m_pEndResultLogo = CUI_TEXTURE::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, (SCREEN_HEIGHT / 2) - 150.0f, 0.0f), SCREEN_WIDTH / 2, 200.0f, CUI_TEXTURE::UIFLAME_RESULT_VICTORY); }
					else if (CResult::GetTeamWin() == CResult::TEAM_WIN_RED) { m_pEndResultLogo = CUI_TEXTURE::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, (SCREEN_HEIGHT / 2) - 150.0f, 0.0f), SCREEN_WIDTH / 2, 200.0f, CUI_TEXTURE::UIFLAME_RESULT_DEFEAT); }
				}
				//�����̃`�[�����������S�𐶐�
				m_pEndMyTeamLogo = CUI_TEXTURE::Create(D3DXVECTOR3(160.0f, 650.0f, 0.0f), 150.0f, 50.0f, CUI_TEXTURE::UIFLAME_RESULT_MYTEAMLOGO);
			}
			else if (CMenu::GetMode() == CMenu::MODE_MULTI)
			{//�}���`�v���C�̏ꍇ
				if (CManager::GetClient() != NULL)
				{//NULL�ł͂Ȃ��ꍇ
					if (CManager::GetClient()->GetTeam() == 0)
					{//�`�[����0�̏ꍇ
						if (m_pEndResultLogo == NULL)
						{//NULL�̏ꍇ
							//���s�̃��S�𐶐�
							if (CResult::GetTeamWin() == CResult::TEAM_WIN_BLUE) { m_pEndResultLogo = CUI_TEXTURE::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, (SCREEN_HEIGHT / 2) - 150.0f, 0.0f), SCREEN_WIDTH / 2, 200.0f, CUI_TEXTURE::UIFLAME_RESULT_VICTORY); }
							else if (CResult::GetTeamWin() == CResult::TEAM_WIN_RED) { m_pEndResultLogo = CUI_TEXTURE::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, (SCREEN_HEIGHT / 2) - 150.0f, 0.0f), SCREEN_WIDTH / 2, 200.0f, CUI_TEXTURE::UIFLAME_RESULT_DEFEAT); }
						}
						//�����̃`�[�����������S�𐶐�
						m_pEndMyTeamLogo = CUI_TEXTURE::Create(D3DXVECTOR3(160.0f, 650.0f, 0.0f), 150.0f, 50.0f, CUI_TEXTURE::UIFLAME_RESULT_MYTEAMLOGO);
					}
					else if (CManager::GetClient()->GetTeam() == 1)
					{//�`�[����1�̏ꍇ
						if (m_pEndResultLogo == NULL)
						{//NULL�̏ꍇ
							//���s�̃��S�𐶐�
							if (CResult::GetTeamWin() == CResult::TEAM_WIN_BLUE) { m_pEndResultLogo = CUI_TEXTURE::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, (SCREEN_HEIGHT / 2) - 150.0f, 0.0f), SCREEN_WIDTH / 2, 200.0f, CUI_TEXTURE::UIFLAME_RESULT_DEFEAT); }
							else if (CResult::GetTeamWin() == CResult::TEAM_WIN_RED) { m_pEndResultLogo = CUI_TEXTURE::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, (SCREEN_HEIGHT / 2) - 150.0f, 0.0f), SCREEN_WIDTH / 2, 200.0f, CUI_TEXTURE::UIFLAME_RESULT_VICTORY); }
						}
						//�����̃`�[�����������S�𐶐�
						m_pEndMyTeamLogo = CUI_TEXTURE::Create(D3DXVECTOR3(1130.0f, 650.0f, 0.0f), 150.0f, 50.0f, CUI_TEXTURE::UIFLAME_RESULT_MYTEAMLOGO);
					}
				}
			}
		}
	}
}

//=============================================================================
// �t�F�[�h�̍X�V����
//=============================================================================
void CGame::FadeUpdate(void)
{
	//�J�ڂ���܂ł̃J�E���^�[�����Z
	m_modeCounter++;
	if (m_modeCounter >= 200)
	{//�펀����܂ł̃J�E���^�[��200�ȏ�̏ꍇ

		//�w�i�̊J��
		if (m_pEndBG != NULL)
		{
			m_pEndBG->Uninit();
			m_pEndBG = NULL;

		}

		//���s�̃��S�̊J��
		if (m_pEndResultLogo != NULL)
		{
			m_pEndResultLogo->Uninit();
			m_pEndResultLogo = NULL;
		}

		//�����̃`�[�����������S�̊J��
		if (m_pEndMyTeamLogo != NULL)
		{
			m_pEndMyTeamLogo->Uninit();
			m_pEndMyTeamLogo = NULL;
		}

		for (int nCntResult = 0; nCntResult < NUM_TEAM; nCntResult++)
		{
			//�X�R�A�̔j��
			if (m_pEndScore[nCntResult] != NULL)
			{
				m_pEndScore[nCntResult]->Uninit();
				m_pEndScore[nCntResult] = NULL;
			}

			//�`�[���G���u�����̔j��
			if (m_pEndTeamEmbrem[nCntResult] != NULL)
			{
				m_pEndTeamEmbrem[nCntResult]->Uninit();
				m_pEndTeamEmbrem[nCntResult] = NULL;
			}
		}

		//�������Ă��Ȃ���Ԃɂ���
		m_state = STATE_NONE;

		// ���(���[�h)�̐ݒ�
		CFade::Create(CManager::MODE_RESULT);
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

	if (m_state == STATE_NORMAL)
	{
		if (pKeyboard->GetTrigger(DIK_Z) || pDirectInput->GetGamePadTrigger(0))
		{// �p�[�g�؂�ւ�
			m_part = (PART)((m_part + 1) % PART_MAX);

			CScene::UIUninit();

			if (PART_STRATEGY == m_part)
			{
				//�o�O�΍��p�Ƀv���C���[�t�h�̔j������
				if (CMenu::MODE_MULTI == CMenu::GetMode())
				{
					if (CManager::GetClient() != NULL)
					{
						m_pPlayer[CManager::GetClient()->GetPlayerIdx()]->ReleasePlayerUI();
					}
				}
				else if (CMenu::MODE_SINGLE == CMenu::GetMode())
				{
					m_pPlayer[0]->ReleasePlayerUI();
				}

				CreateStrategyUI();
			}
			else
			{
				CreateActionUI();
			}
		}
	}
	else if (m_state == STATE_END)
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
		m_part = PART_ACTION;

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
	CUI_TEXTURE::Create(D3DXVECTOR3(80.0f, 40.0f, 0.0f), 120.0f, 50.0f, CUI_TEXTURE::UIFLAME_TEAM_BLUE);	// BLUE
	CUI_TEXTURE::Create(D3DXVECTOR3(80.0f, 100.0f, 0.0f), 120.0f, 50.0f, CUI_TEXTURE::UIFLAME_TEAM_RED);	// RED

																											//****************************************
																											// UI�����i�����j
																											//****************************************
	CUI_NUMBER::Create(D3DXVECTOR3(370.0f, 40.0f, 0.0f), 430.0f, 30.0f, 0.0f, CUI_NUMBER::UI_NUMTYPE_BLUE, 0, NUMTEX_UV_X, NUMTEX_UV_Y);							// BLUE�`�[���`�P�b�g
	CUI_NUMBER::Create(D3DXVECTOR3(370.0f, 100.0f, 0.0f), 430.0f, 30.0f, 0.0f, CUI_NUMBER::UI_NUMTYPE_RED, 0, NUMTEX_UV_X, NUMTEX_UV_Y);							// RED�`�[���`�P�b�g
	CUI_NUMBER::Create(D3DXVECTOR3(1120.0f, 660.0f, 0.0f), 150.0f, 90.0f, 60.0f, CUI_NUMBER::UI_NUMTYPE_REMAINBULLET, 0, NUMTEX_UV_X, NUMTEX_UV_Y);		// �c�e
	CUI_NUMBER::Create(D3DXVECTOR3(260.0f, 530.0f, 0.0f), 100.0f, 60.0f, 40.0f, CUI_NUMBER::UI_NUMTYPE_DRONE_HP, 0, NUMTEX_UV_X, NUMTEX_UV_Y);				// �h���[�����C�t
	CUI_NUMBER::Create(D3DXVECTOR3(260.0f, 420.0f, 0.0f), 100.0f, 60.0f, 40.0f, CUI_NUMBER::UI_NUMTYPE_WORKER_HP, 0, NUMTEX_UV_X, NUMTEX_UV_Y);			// ���[�J�[���C�t
	CUI_NUMBER::Create(D3DXVECTOR3(385.0f, 650.0f, 0.0f), 150.0f, 100.0f, 60.0f, CUI_NUMBER::UI_NUMTYPE_PLAYER_HP, 0, NUMTEX_UV_X, NUMTEX_UV_Y);			// �v���C���[���C�t

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

																																				// �t���[��
	CUI_TEXTURE::Create(D3DXVECTOR3(785.0f, 420.0f, 0.0f), 990.0f, 590.0f, CUI_TEXTURE::UIFLAME_FLAME_BLUE);	// ��g
	CUI_TEXTURE::Create(D3DXVECTOR3(790.0f, 190.0f, 0.0f), 970.0f, 100.0f, CUI_TEXTURE::UIFLAME_FLAME_WHITE);	// ���W�b�N���C��
	CUI_TEXTURE::Create(D3DXVECTOR3(405.0f, 470.0f, 0.0f), 200.0f, 450.0f, CUI_TEXTURE::UIFLAME_FLAME_WHITE);	// AI�\��
	CUI_TEXTURE::Create(D3DXVECTOR3(895.0f, 470.0f, 0.0f), 760.0f, 450.0f, CUI_TEXTURE::UIFLAME_FLAME_WHITE);	// ���W�b�N����

																												// �`�[���̃`�P�b�g���t���[��
	CUI_TEXTURE::Create(D3DXVECTOR3(80.0f, 40.0f, 0.0f), 120.0f, 50.0f, CUI_TEXTURE::UIFLAME_TEAM_BLUE);	// BLUE
	CUI_TEXTURE::Create(D3DXVECTOR3(80.0f, 100.0f, 0.0f), 120.0f, 50.0f, CUI_TEXTURE::UIFLAME_TEAM_RED);	// RED

																											//****************************************
																											// 1P�������
																											//****************************************
	CUI_TEXTURE::Create(D3DXVECTOR3(150.0f, 325.0f, 0.0f), 280.0f, 260.0f, CUI_TEXTURE::UIFLAME_1P_INFO);

	// �v���C���[�̗̓t���[��
	CUI_TEXTURE::Create(D3DXVECTOR3(150.0f, 260.0f, 0.0f), 255.0f, 80.0f, CUI_TEXTURE::UIFLAME_PLAYER_HP);

	// AI�`�P�b�g���t���[��
	CUI_TEXTURE::Create(D3DXVECTOR3(150.0f, 335.0f, 0.0f), 255.0f, 80.0f, CUI_TEXTURE::UIFLAME_WORKER);		// ���[�J�[
	CUI_TEXTURE::Create(D3DXVECTOR3(150.0f, 410.0f, 0.0f), 255.0f, 80.0f, CUI_TEXTURE::UIFLAME_DRONE);		// �h���[��

																											// �e���C�t�̐�������
	CUI_NUMBER::Create(D3DXVECTOR3(305.0f, 260.0f, 0.0f), 115.0f, 70.0f, 45.0f, CUI_NUMBER::UI_NUMTYPE_PLAYER_HP, 0, NUMTEX_UV_X, NUMTEX_UV_Y);			// 1P�v���C���[
	CUI_NUMBER::Create(D3DXVECTOR3(305.0f, 335.0f, 0.0f), 115.0f, 70.0f, 45.0f, CUI_NUMBER::UI_NUMTYPE_WORKER_HP, 0, NUMTEX_UV_X, NUMTEX_UV_Y);		// 1P���[�J�[
	CUI_NUMBER::Create(D3DXVECTOR3(305.0f, 410.0f, 0.0f), 115.0f, 70.0f, 45.0f, CUI_NUMBER::UI_NUMTYPE_DRONE_HP, 0, NUMTEX_UV_X, NUMTEX_UV_Y);			// 1P�h���[��

																																						//****************************************
																																						// 2P�������
																																						//****************************************
	CUI_TEXTURE::Create(D3DXVECTOR3(150.0f, 590.0f, 0.0f), 280.0f, 260.0f, CUI_TEXTURE::UIFLAME_2P_INFO);

	// �v���C���[�̗̓t���[��
	CUI_TEXTURE::Create(D3DXVECTOR3(150.0f, 525.0f, 0.0f), 255.0f, 80.0f, CUI_TEXTURE::UIFLAME_ALLY_HP);

	// AI�`�P�b�g���t���[��
	CUI_TEXTURE::Create(D3DXVECTOR3(150.0f, 600.0f, 0.0f), 255.0f, 80.0f, CUI_TEXTURE::UIFLAME_WORKER);		// ���[�J�[
	CUI_TEXTURE::Create(D3DXVECTOR3(150.0f, 675.0f, 0.0f), 255.0f, 80.0f, CUI_TEXTURE::UIFLAME_DRONE);		// �h���[��

																											// �e���C�t�̐�������
	CUI_NUMBER::Create(D3DXVECTOR3(305.0f, 525.0f, 0.0f), 115.0f, 70.0f, 45.0f, CUI_NUMBER::UI_NUMTYPE_ALLY_HP, 0, NUMTEX_UV_X, NUMTEX_UV_Y);				// 2P�v���C���[
	CUI_NUMBER::Create(D3DXVECTOR3(305.0f, 600.0f, 0.0f), 115.0f, 70.0f, 45.0f, CUI_NUMBER::UI_NUMTYPE_ALLY_WORKER_HP, 0, NUMTEX_UV_X, NUMTEX_UV_Y);		// 2P���[�J�[
	CUI_NUMBER::Create(D3DXVECTOR3(305.0f, 675.0f, 0.0f), 115.0f, 70.0f, 45.0f, CUI_NUMBER::UI_NUMTYPE_ALLY_DRONE_HP, 0, NUMTEX_UV_X, NUMTEX_UV_Y);		// 2P�h���[��

																																						// �X�g���e�W�[�������
	CUI_TEXTURE::Create(D3DXVECTOR3(940.0f, 70.0f, 0.0f), 640.0f, 80.0f, CUI_TEXTURE::UIFLAME_STRATEGY_INST);

	//****************************************
	// UI�����i�����j
	//****************************************
	CUI_NUMBER::Create(D3DXVECTOR3(370.0f, 40.0f, 0.0f), 430.0f, 30.0f, 0.0f, CUI_NUMBER::UI_NUMTYPE_BLUE, 0, NUMTEX_UV_X, NUMTEX_UV_Y);							// BLUE�`�[���`�P�b�g
	CUI_NUMBER::Create(D3DXVECTOR3(370.0f, 100.0f, 0.0f), 430.0f, 30.0f, 0.0f, CUI_NUMBER::UI_NUMTYPE_RED, 0, NUMTEX_UV_X, NUMTEX_UV_Y);							// RED�`�[���`�P�b�g

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
// �v���C���[�̐�������
//=============================================================================
void CGame::CreatePlayer(void)
{
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER_CONNECT; nCntPlayer++)
	{
		int nTeam = 0;	//�`�[�����
		D3DXVECTOR3 respawnPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		if (nCntPlayer == 0 || nCntPlayer == 1)
		{//�v���C���[�ԍ����O���P�̏ꍇ
			nTeam = 0;	//�`�[��
		}
		else if (nCntPlayer == 2 || nCntPlayer == 3)
		{//�v���C���[�ԍ����Q���R�̏ꍇ
			nTeam = 1;	//�ԃ`�[��
		}

		switch (nCntPlayer)
		{
		case 0:
			respawnPos = m_aRespawnPos[nTeam][1];
			break;
		case 1:
			respawnPos = m_aRespawnPos[nTeam][2];
			break;
		case 2:
			respawnPos = m_aRespawnPos[nTeam][1];
			break;
		case 3:
			respawnPos = m_aRespawnPos[nTeam][2];
			break;
		}

		if (CMenu::GetMode() == CMenu::MODE_MULTI)
		{//�}���`�v���C�̏ꍇ
			bool bConnect = false;	//�ڑ����Ă��邩�ǂ���

			//�N���C�A���g�̎擾
			CClient *pClient = CManager::GetClient();

			//if (m_aMechaType[nCntPlayer] == -1)
			//{//�@�̔ԍ���-1�̏ꍇ
			// //�����_���ŋ@�̂����߂�
			//	m_aMechaType[nCntPlayer] = (CMechaSelect::MECHATYPE)(rand() % CMechaSelect::MECHATYPE_MAX);
			//	m_pPlayer[nCntPlayer] = CPlayer::Create(nCntPlayer, m_aMechaType[nCntPlayer], respawnPos, m_bConnect[nCntPlayer]);
			//}
			//else
			//{//����ȊO�̏ꍇ
			//	m_bConnect[nCntPlayer] = true;	//�ڑ����Ă����Ԃɂ���
			//	m_pPlayer[nCntPlayer] = CPlayer::Create(nCntPlayer, m_aMechaType[nCntPlayer], respawnPos, m_bConnect[nCntPlayer]);

			//}

			if (CMatching::GetConnect(nCntPlayer) == false)
			{//�}�b�`���O���_�Őڑ�����Ă��Ȃ������ꍇ
				m_pPlayer[nCntPlayer] = CPlayer::Create(nCntPlayer, m_aMechaType[nCntPlayer], respawnPos, false);
			}
			else if (CMatching::GetConnect(nCntPlayer) == true)
			{
				m_pPlayer[nCntPlayer] = CPlayer::Create(nCntPlayer, m_aMechaType[nCntPlayer], respawnPos, true);
			}
		}
		else
		{//�V���O���v���C�̏ꍇ
			if (nCntPlayer == 0)
			{//�v���C���[�ԍ����O�Ԃ̏ꍇ
				m_aMechaType[nCntPlayer] = CMechaSelect::GetMechaType();
				m_pPlayer[nCntPlayer] = CPlayer::Create(nCntPlayer, m_aMechaType[nCntPlayer], respawnPos, true);
			}
			else
			{//����ȊO�̏ꍇ
				m_aMechaType[nCntPlayer] = (CMechaSelect::MECHATYPE)(rand() % CMechaSelect::MECHATYPE_MAX);
				m_pPlayer[nCntPlayer] = CPlayer::Create(nCntPlayer, m_aMechaType[nCntPlayer], respawnPos, false);
			}
		}
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
			pClient->Printf("%.2f %.2f %.2f", CManager::GetCamera()->GetRot().x, CManager::GetCamera()->GetRot().y, CManager::GetCamera()->GetRot().z);
			pClient->Printf(" ");

			//���S���Ă��邩�ǂ���
			if (m_pPlayer[pClient->GetPlayerIdx()]->GetDeath() == true)
			{
				//���S���Ă��邱�Ƃ���������
				pClient->Printf("1");
				pClient->Printf(" ");

				//�L���v���C���[�̔ԍ�����������
				pClient->Printf("%d", m_pPlayer[pClient->GetPlayerIdx()]->GetKillPlayerIdx());
				pClient->Printf(" ");

				//�L���v���C���[�̎�ނ���������
				pClient->Printf("%d", (int)m_playerType[0]);
				pClient->Printf(" ");

			}
			else
			{
				//���S���Ă��Ȃ����Ƃ���������
				pClient->Printf("0");
				pClient->Printf(" ");

				//�̗͂̕\��
				pClient->Printf("%d", m_pPlayer[pClient->GetPlayerIdx()]->GetLife());
				pClient->Printf(" ");
			}

			//�e�𔭎˂��Ă��邩�ǂ�������������
			if (m_pPlayer[pClient->GetPlayerIdx()]->GetShoot() == true)
			{//���˂���Ă���ꍇ
			 //���˂��Ă��������������
				pClient->Printf("1");
				pClient->Printf(" ");

				//�e�̓������ː�����������
				pClient->Printf("%d", m_pPlayer[pClient->GetPlayerIdx()]->GetNumShoot());
				pClient->Printf(" ");

				//�U���͂���������
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
				//���˂��Ă��Ȃ�������������
				pClient->Printf("0");
				pClient->Printf(" ");
			}

			//�`���b�g�����Ă��邩�ǂ�������������
			if (m_pPlayer[pClient->GetPlayerIdx()]->GetChat() == true)
			{//�`���b�g�����Ă���ꍇ
			 //�`���b�g�����Ă��邱�Ƃ���������
				pClient->Printf("1");
				pClient->Printf(" ");

				//�`���b�g�̔ԍ�����������
				pClient->Printf("%d", m_pPlayer[pClient->GetPlayerIdx()]->GetRadioChat());
				pClient->Printf(" ");
			}
			else
			{
				//�`���b�g�����Ă��Ȃ����Ƃ���������
				pClient->Printf("0");
				pClient->Printf(" ");
			}

			for (int nCntAI = 0; nCntAI < AI_MAX; nCntAI++)
			{
				//AI�̈ʒu����������
				pClient->Printf("%.1f %.1f %.1f", m_pPlayer[pClient->GetPlayerIdx()]->GetMyAI(nCntAI)->GetPos().x, m_pPlayer[pClient->GetPlayerIdx()]->GetMyAI(nCntAI)->GetPos().y, m_pPlayer[pClient->GetPlayerIdx()]->GetMyAI(nCntAI)->GetPos().z);
				pClient->Printf(" ");

				//AI�̌�������������
				pClient->Printf("%.1f", m_pPlayer[pClient->GetPlayerIdx()]->GetMyAI(nCntAI)->GetRot().y);
				pClient->Printf(" ");

				//AI�̎��S���Ă��邩�ǂ�������������
				if (m_pPlayer[pClient->GetPlayerIdx()]->GetMyAI(nCntAI)->GetDeath() == true)
				{//���S���Ă���ꍇ
				 //���S���Ă��������������
					pClient->Printf("1");
					pClient->Printf(" ");

					//�L���v���C���[�̔ԍ�����������
					pClient->Printf("%d", m_pPlayer[pClient->GetPlayerIdx()]->GetMyAI(nCntAI)->GetKillPlayerIdx());
					pClient->Printf(" ");

					//�L���v���C���[�̎�ނ���������
					pClient->Printf("%d", (int)m_playerType[0]);
					pClient->Printf(" ");

				}
				else
				{
					//���S���Ă��Ȃ����Ƃ���������
					pClient->Printf("0");
					pClient->Printf(" ");

					//�̗͂̕\��
					pClient->Printf("%d", m_pPlayer[pClient->GetPlayerIdx()]->GetMyAI(nCntAI)->GetLife());
					pClient->Printf(" ");

				}

				//�e�𔭎˂��Ă��邩�ǂ�������������
				if (m_pPlayer[pClient->GetPlayerIdx()]->GetMyAI(nCntAI)->GetShoot() == true)
				{//���˂���Ă���ꍇ
					//���˂��Ă��������������
					pClient->Printf("1");
					pClient->Printf(" ");

					//�U���͂���������
					pClient->Printf("%d", m_pPlayer[pClient->GetPlayerIdx()]->GetMyAI(nCntAI)->GetAttack());
					pClient->Printf(" ");

					//�����p�x�̏�����������
					pClient->Printf("%.2f %.2f", m_pPlayer[pClient->GetPlayerIdx()]->GetMyAI(nCntAI)->GetAngle(), m_pPlayer[pClient->GetPlayerIdx()]->GetMyAI(nCntAI)->GetAngleV());
					pClient->Printf(" ");

					//���˂��Ă��Ȃ���Ԃɖ߂�
					m_pPlayer[pClient->GetPlayerIdx()]->GetMyAI(nCntAI)->SetShoot(false);
				}
				else
				{
					//���˂��Ă��Ȃ�������������
					pClient->Printf("0");
					pClient->Printf(" ");
				}

			}

			// �s�����g�p���Ă��邩�ǂ�������������
			if (m_pPlayer[pClient->GetPlayerIdx()]->GetPinUse() == true)
			{// �s�����g�p���Ă���ꍇ
				pClient->Printf("1");
				pClient->Printf(" ");

				// �ʒu����������
				pClient->Printf("%.1f %.1f %.1f", m_pPlayer[pClient->GetPlayerIdx()]->GetPinPos().x, m_pPlayer[pClient->GetPlayerIdx()]->GetPinPos().y, m_pPlayer[pClient->GetPlayerIdx()]->GetPinPos().z);
				pClient->Printf(" ");

				//// �\�����Ԃ���������
				//pClient->Printf("%d", m_pPlayer[pClient->GetPlayerIdx()]->GetPinLife());
				//pClient->Printf(" ");
			}
			else
			{// �s�����g�p���Ă��Ȃ��ꍇ
				pClient->Printf("0");
				pClient->Printf(" ");
			}

			for (int nCntAI = 0; nCntAI < AI_MAX; nCntAI++)
			{// AI�̐��������
			 // AI���s�����g�p���Ă��邩�ǂ�������������
				if (m_pPlayer[pClient->GetPlayerIdx()]->GetAIPinUse(nCntAI) == true)
				{// AI���s�����g�p���Ă���ꍇ
					pClient->Printf("1");
					pClient->Printf(" ");

					// �ʒu����������
					pClient->Printf("%.1f %.1f %.1f", m_pPlayer[pClient->GetPlayerIdx()]->GetAIPinPos(nCntAI).x, m_pPlayer[pClient->GetPlayerIdx()]->GetAIPinPos(nCntAI).y, m_pPlayer[pClient->GetPlayerIdx()]->GetAIPinPos(nCntAI).z);
					pClient->Printf(" ");
				}
				else
				{// �s�����g�p���Ă��Ȃ��ꍇ
					pClient->Printf("0");
					pClient->Printf(" ");
				}
			}

#if 0
			//AI�̈ʒu����������
			pClient->Printf("%.1f %.1f %.1f", m_pPlayer[pClient->GetPlayerIdx()]->GetMyAI(0)->GetPos().x, m_pPlayer[pClient->GetPlayerIdx()]->GetMyAI(0)->GetPos().y, m_pPlayer[pClient->GetPlayerIdx()]->GetMyAI(0)->GetPos().z);
			pClient->Printf(" ");

			//AI�̌�������������
			pClient->Printf("%.1f", m_pPlayer[pClient->GetPlayerIdx()]->GetMyAI(0)->GetRot().y);
			pClient->Printf(" ");

			//AI�̎��S���Ă��邩�ǂ�������������
			if (m_pPlayer[pClient->GetPlayerIdx()]->GetMyAI(0)->GetDeath() == true)
			{//���S���Ă���ꍇ
			 //���S���Ă��������������
				pClient->Printf("1");
				pClient->Printf(" ");

				//�L���v���C���[�̔ԍ�����������
				pClient->Printf("%d", m_pPlayer[pClient->GetPlayerIdx()]->GetMyAI(0)->GetKillPlayerIdx());
				pClient->Printf(" ");

				//�L���v���C���[�̎�ނ���������
				pClient->Printf("%d", (int)m_playerType[0]);
				pClient->Printf(" ");

			}
			else
			{
				//���S���Ă��Ȃ����Ƃ���������
				pClient->Printf("0");
				pClient->Printf(" ");
			}
#endif
			if (pClient->GetPlayerIdx() == 0)
			{//�z�X�g�̏ꍇ
				if (m_state == STATE_END)
				{//�Q�[���I�[�o�[�̏ꍇ
				 //�Q�[���I������������
					pClient->Printf("1");
					pClient->Printf(" ");

					//�������`�[���̏�����������
					pClient->Printf("%d", CResult::GetTeamWin());
					pClient->Printf(" ");
				}
				else
				{
					pClient->Printf("0");
					pClient->Printf(" ");
				}
				//CPU�̃f�[�^�����������ޏ���
				PrintCPUData();
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
				if (CMatching::GetConnect(nCntPlayer) == false)
				{
					//AI�v���C���[������������
					pClient->Printf(SERVER_AI_PLAYER_DATA);

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

					//���f��0�Ԃ�y���̌�������������
					pClient->Printf("%.1f", m_pPlayer[nCntPlayer]->GetModel(0)->GetRot().y);
					pClient->Printf(" ");

					//���f��1�Ԃ̌�������������
					pClient->Printf("%.1f %.1f %.1f", m_pPlayer[nCntPlayer]->GetModel(1)->GetRot().x, m_pPlayer[nCntPlayer]->GetModel(1)->GetRot().y, m_pPlayer[nCntPlayer]->GetModel(1)->GetRot().z);
					pClient->Printf(" ");

					//�㔼�g�̂̌�������������
					pClient->Printf("%.2f %.2f %.2f", m_pPlayer[nCntPlayer]->GetDestUpper().x, m_pPlayer[nCntPlayer]->GetDestUpper().y, m_pPlayer[nCntPlayer]->GetDestUpper().z);
					pClient->Printf(" ");

					//���S���Ă��邩�ǂ���
					if (m_pPlayer[nCntPlayer]->GetDeath() == true)
					{
						//���S���Ă��邱�Ƃ���������
						pClient->Printf("1");
						pClient->Printf(" ");

						//�L���v���C���[�̔ԍ�����������
						pClient->Printf("%d", m_pPlayer[nCntPlayer]->GetKillPlayerIdx());
						pClient->Printf(" ");

						//�L���v���C���[�̎�ނ���������
						pClient->Printf("%d", (int)m_playerType[0]);
						pClient->Printf(" ");

					}
					else
					{
						//���S���Ă��Ȃ����Ƃ���������
						pClient->Printf("0");
						pClient->Printf(" ");

						//�̗͂���������
						pClient->Printf("%d", m_pPlayer[nCntPlayer]->GetLife());
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

						//�U���͂���������
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


					for (int nCntAI = 0; nCntAI < AI_MAX; nCntAI++)
					{
						//AI�̈ʒu����������
						pClient->Printf("%.1f %.1f %.1f", m_pPlayer[nCntPlayer]->GetMyAI(nCntAI)->GetPos().x, m_pPlayer[nCntPlayer]->GetMyAI(nCntAI)->GetPos().y, m_pPlayer[nCntPlayer]->GetMyAI(nCntAI)->GetPos().z);
						pClient->Printf(" ");

						//AI�̌�������������
						pClient->Printf("%.1f", m_pPlayer[nCntPlayer]->GetMyAI(nCntAI)->GetRot().y);
						pClient->Printf(" ");

						//AI�̎��S���Ă��邩�ǂ�������������
						if (m_pPlayer[nCntPlayer]->GetMyAI(nCntAI)->GetDeath() == true)
						{//���S���Ă���ꍇ
						 //���S���Ă��������������
							pClient->Printf("1");
							pClient->Printf(" ");

							//�L���v���C���[�̔ԍ�����������
							pClient->Printf("%d", m_pPlayer[nCntPlayer]->GetMyAI(nCntAI)->GetKillPlayerIdx());
							pClient->Printf(" ");

							//�L���v���C���[�̎�ނ���������
							pClient->Printf("%d", (int)m_playerType[0]);
							pClient->Printf(" ");

						}
						else
						{
							//���S���Ă��Ȃ����Ƃ���������
							pClient->Printf("0");
							pClient->Printf(" ");

							//�̗͂̕\��
							pClient->Printf("%d", m_pPlayer[nCntPlayer]->GetMyAI(nCntAI)->GetLife());
							pClient->Printf(" ");
						}

						//�e�𔭎˂��Ă��邩�ǂ�������������
						if (m_pPlayer[nCntPlayer]->GetMyAI(nCntAI)->GetShoot() == true)
						{//���˂���Ă���ꍇ
						 //���˂��Ă��������������
							pClient->Printf("1");
							pClient->Printf(" ");

							//�U���͂���������
							pClient->Printf("%d", m_pPlayer[nCntPlayer]->GetMyAI(nCntAI)->GetAttack());
							pClient->Printf(" ");

							//�����p�x�̏�����������
							pClient->Printf("%.2f %.2f", m_pPlayer[nCntPlayer]->GetMyAI(nCntAI)->GetAngle(), m_pPlayer[nCntPlayer]->GetMyAI(nCntAI)->GetAngleV());
							pClient->Printf(" ");

							//���˂��Ă��Ȃ���Ԃɖ߂�
							m_pPlayer[nCntPlayer]->GetMyAI(nCntAI)->SetShoot(false);
						}
						else
						{
							//���˂��Ă��Ȃ�������������
							pClient->Printf("0");
							pClient->Printf(" ");
						}
					}

					// �s�����g�p���Ă��邩�ǂ�������������
					if (m_pPlayer[nCntPlayer]->GetPinUse() == true)
					{// �s�����g�p���Ă���ꍇ
						pClient->Printf("1");
						pClient->Printf(" ");

						// �ʒu����������
						pClient->Printf("%.1f %.1f %.1f", m_pPlayer[nCntPlayer]->GetPinPos().x, m_pPlayer[nCntPlayer]->GetPinPos().y, m_pPlayer[nCntPlayer]->GetPinPos().z);
						pClient->Printf(" ");
					}
					else
					{// �s�����g�p���Ă��Ȃ��ꍇ
						pClient->Printf("0");
						pClient->Printf(" ");
					}

					for (int nCntAI = 0; nCntAI < AI_MAX; nCntAI++)
					{// AI�̐��������
					 // AI���s�����g�p���Ă��邩�ǂ�������������
						if (m_pPlayer[nCntPlayer]->GetAIPinUse(nCntAI) == true)
						{// AI���s�����g�p���Ă���ꍇ
							pClient->Printf("1");
							pClient->Printf(" ");

							// �ʒu����������
							pClient->Printf("%.1f %.1f %.1f", m_pPlayer[nCntPlayer]->GetAIPinPos(nCntAI).x, m_pPlayer[nCntPlayer]->GetAIPinPos(nCntAI).y, m_pPlayer[nCntPlayer]->GetAIPinPos(nCntAI).z);
							pClient->Printf(" ");
						}
						else
						{// �s�����g�p���Ă��Ȃ��ꍇ
							pClient->Printf("0");
							pClient->Printf(" ");
						}
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
			pStr += strlen(SERVER_CONNECT_DATA);			//���o��

															//�ڑ����̓ǂݎ�菈��
			pStr = ReadConnectData(pStr);
		}
		if (CServerFunction::Memcmp(pStr, SERVER_PLAYER_START) == 0)
		{//�v���C���[�̊J�n�������Ă���ꍇ
			pStr += strlen(SERVER_PLAYER_START);			//���o��
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
	D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				//�ʒu
	D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				//����
	D3DXVECTOR3 modelRotDown = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//���f���̉����g�̌���
	D3DXVECTOR3	modelRotUp = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//���f���̏㔼�g�̌���
	D3DXVECTOR3 cameraRot;											//�J�����̌���
	int nPlayerIdx = 0;												//�v���C���[�̔ԍ�
	int nNumConnect = 0;											//�ڑ�����
	int nWord = 0;													//�����̓��o���p
	int nCntClient = 0;												//�N���C�A���g�̃J�E���^�[
	int nTeam = 0;													//�`�[�����
	float *pAngle = NULL;											//�����p�x
	float *pAngleV = NULL;											//�����p�x
	bool bShoot = false;											//�e�𔭎˂��Ă��邩�ǂ���
	int nNumShoot = 0;												//�������ː�
	int nAttack = 0;												//�U����
	bool bChat = false;												//�`���b�g���Ă��邩�ǂ���
	CPlayer::RADIOCHAT radioChat = CPlayer::RADIOCHAT_OK;			//���W�I�`���b�g
	int nBlueLinkEnergy = 0;										//�u���[�`�[���̃����N�G�l���M�[
	int nRedLinkEnergy = 0;											//���b�h�`�[���̃����N�G�l���M�[
	int nState = 0;													//��ԏ��
	int nLife = 0;													//�̗͏��
	int nKillPlayerIdx = 0;											//�L���v���C���[�̔ԍ�
	TYPE playerType = TYPE_PLAYER;									//�L���v���C���[�̎��

	bool bPinUse = false;											// �s�����g�p���Ă��邩�ǂ���
	D3DXVECTOR3 pinPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// �s���̈ʒu
	bool bAIPinUse[AI_MAX] = { false, false };						// AI���s�����g�p���Ă��邩�ǂ���
	D3DXVECTOR3 AIPinPos[AI_MAX] = { D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f) };// AI�̃s���̈ʒu

	D3DXVECTOR3 AIPos[AI_MAX] = {D3DXVECTOR3(0.0f,0.0f,0.0f),D3DXVECTOR3(0.0f,0.0f,0.0f)};		//AI�̈ʒu
	D3DXVECTOR3 AIRot[AI_MAX] = { D3DXVECTOR3(0.0f,0.0f,0.0f) ,D3DXVECTOR3(0.0f,0.0f,0.0f) };	//AI�̌���
	bool bAIDeath[AI_MAX] = {false ,false};														//AI������ł��邩�ǂ���
	int nAIKillPlayerIdx[AI_MAX] = {0,0};														//AI�L���v���C���[�̔ԍ�
	TYPE AIPlayerType[AI_MAX] = {TYPE_PLAYER,TYPE_PLAYER};										//AI�L���v���C���[�̎��
	bool bAIShoot[AI_MAX] = { false,false };													//AI�e���˂������ǂ���
	float fAIAngle[AI_MAX] = { 0.0f,0.0f };														//AI�p�x
	float fAIAngleV[AI_MAX] = { 0.0f,0.0f };													//AI�p�x
	int nAIAttack[AI_MAX] = { 0, 0 };															//AI�U����
	int nAILife[AI_MAX] = { 0,0 };																//AI�̗�

#if 0
	D3DXVECTOR3 AIPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				//AI�̈ʒu
	D3DXVECTOR3 AIRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				//AI�̌���
	bool bAIDeath = false;											//AI������ł��邩�ǂ���
	int nAIKillPlayerIdx = 0;										//AI�L���v���C���[�̔ԍ�
	TYPE AIPlayerType = TYPE_PLAYER;								//AI�L���v���C���[�̎��
#endif
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

			//���f��0�Ԃ̌�������
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

			if (bDeath == true)
			{//���S���Ă����ꍇ
			 //�L���v���C���[�̔ԍ�����
				nKillPlayerIdx = CServerFunction::ReadInt(pStr, "");
				nWord = CServerFunction::PopString(pStr, "");
				pStr += nWord;

				//�L���v���C���[�̎�ނ���
				playerType = (TYPE)CServerFunction::ReadInt(pStr, "");
				nWord = CServerFunction::PopString(pStr, "");
				pStr += nWord;
			}
			else if (bDeath == false)
			{//���S���Ă��Ȃ��ꍇ
			 //�̗͂̏�����
				nLife = CServerFunction::ReadInt(pStr, "");
				nWord = CServerFunction::PopString(pStr, "");
				pStr += nWord;
			}

			//�e�𔭎˂��Ă��邩�ǂ�������
			bShoot = CServerFunction::ReadBool(pStr, "");
			nWord = CServerFunction::PopString(pStr, "");
			pStr += nWord;

			if (bShoot == true)
			{//�e�𔭎˂��Ă���ꍇ
			 //�e�̓������ː�������
				nNumShoot = CServerFunction::ReadInt(pStr, "");
				nWord = CServerFunction::PopString(pStr, "");
				pStr += nWord;

				//�����p�x�𓯎����ː������I�m��
				pAngle = new float[nNumShoot * 2];
				pAngleV = new float[nNumShoot * 2];

				//�U���͂���
				nAttack = CServerFunction::ReadInt(pStr, "");
				nWord = CServerFunction::PopString(pStr, "");
				pStr += nWord;

				//�����p�x����
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

			//�`���b�g�����Ă��邩�ǂ�������
			bChat = CServerFunction::ReadBool(pStr, "");
			nWord = CServerFunction::PopString(pStr, "");
			pStr += nWord;

			if (bChat == true)
			{//�`���b�g�����Ă���ꍇ
				radioChat = (CPlayer::RADIOCHAT)CServerFunction::ReadInt(pStr, "");
				nWord = CServerFunction::PopString(pStr, "");
				pStr += nWord;
			}

			for (int nCntAI = 0; nCntAI < AI_MAX; nCntAI++)
			{
				//AI�̈ʒu����
				AIPos[nCntAI].x = CServerFunction::ReadFloat(pStr, "");
				nWord = CServerFunction::PopString(pStr, "");
				pStr += nWord;
				AIPos[nCntAI].y = CServerFunction::ReadFloat(pStr, "");
				nWord = CServerFunction::PopString(pStr, "");
				pStr += nWord;
				AIPos[nCntAI].z = CServerFunction::ReadFloat(pStr, "");
				nWord = CServerFunction::PopString(pStr, "");
				pStr += nWord;

				//AI�̌�������
				AIRot[nCntAI].y = CServerFunction::ReadFloat(pStr, "");
				nWord = CServerFunction::PopString(pStr, "");
				pStr += nWord;

				//AI�̎��S���Ă��邩�ǂ�������
				bAIDeath[nCntAI] = CServerFunction::ReadBool(pStr, "");
				nWord = CServerFunction::PopString(pStr, "");
				pStr += nWord;

				if (bAIDeath[nCntAI] == true)
				{
					//�L���v���C���[�̔ԍ�����
					nAIKillPlayerIdx[nCntAI] = CServerFunction::ReadInt(pStr, "");
					nWord = CServerFunction::PopString(pStr, "");
					pStr += nWord;

					//�L���v���C���[�̎�ނ���
					AIPlayerType[nCntAI] = (TYPE)CServerFunction::ReadInt(pStr, "");
					nWord = CServerFunction::PopString(pStr, "");
					pStr += nWord;
				}
				else
				{
					nAILife[nCntAI] = CServerFunction::ReadInt(pStr, "");
					nWord = CServerFunction::PopString(pStr, "");
					pStr += nWord;
				}

				//�e�𔭎˂��Ă��邩�ǂ�������
				bAIShoot[nCntAI] = CServerFunction::ReadBool(pStr, "");
				nWord = CServerFunction::PopString(pStr, "");
				pStr += nWord;

				if (bAIShoot[nCntAI] == true)
				{//�e�𔭎˂��Ă���ꍇ
					//�U���͂���
					nAIAttack[nCntAI] = CServerFunction::ReadInt(pStr, "");
					nWord = CServerFunction::PopString(pStr, "");
					pStr += nWord;

					//�����p�x�̑��
					fAIAngle[nCntAI] = CServerFunction::ReadFloat(pStr, "");
					nWord = CServerFunction::PopString(pStr, "");
					pStr += nWord;

					fAIAngleV[nCntAI] = CServerFunction::ReadFloat(pStr, "");
					nWord = CServerFunction::PopString(pStr, "");
					pStr += nWord;
				}

			}

			//�s�����g�p���Ă邩�ǂ�������
			bPinUse = CServerFunction::ReadBool(pStr, "");
			nWord = CServerFunction::PopString(pStr, "");
			pStr += nWord;

			if (bPinUse == true)
			{// �s�����g�p���Ă���ꍇ
			 // �s���̈ʒu����
				pinPos.x = CServerFunction::ReadFloat(pStr, "");
				nWord = CServerFunction::PopString(pStr, "");
				pStr += nWord;
				pinPos.y = CServerFunction::ReadFloat(pStr, "");
				nWord = CServerFunction::PopString(pStr, "");
				pStr += nWord;
				pinPos.z = CServerFunction::ReadFloat(pStr, "");
				nWord = CServerFunction::PopString(pStr, "");
				pStr += nWord;
			}

			for (int nCntAI = 0; nCntAI < AI_MAX; nCntAI++)
			{// AI�̐��������
			 // AI���s�����g�p���Ă邩�ǂ�������
				bAIPinUse[nCntAI] = CServerFunction::ReadBool(pStr, "");
				nWord = CServerFunction::PopString(pStr, "");
				pStr += nWord;

				if (bAIPinUse[nCntAI] == true)
				{// AI���s�����g�p���Ă���ꍇ
				 // AI�̃s���̈ʒu����
					AIPinPos[nCntAI].x = CServerFunction::ReadFloat(pStr, "");
					nWord = CServerFunction::PopString(pStr, "");
					pStr += nWord;
					AIPinPos[nCntAI].y = CServerFunction::ReadFloat(pStr, "");
					nWord = CServerFunction::PopString(pStr, "");
					pStr += nWord;
					AIPinPos[nCntAI].z = CServerFunction::ReadFloat(pStr, "");
					nWord = CServerFunction::PopString(pStr, "");
					pStr += nWord;
				}
			}

			if (nPlayerIdx == 0)
			{//�z�X�g�̏ꍇ
			 //���݂̏�Ԃ���
				nState = CServerFunction::ReadInt(pStr, "");
				nWord = CServerFunction::PopString(pStr, "");
				pStr += nWord;

				if (nState == 1)
				{//�I�����
					CResult::SetTeamWin((CResult::TEAM_WIN)CServerFunction::ReadInt(pStr, ""));
				}
				//CPU���̓ǂݎ��
				pStr = ReadCPUData(pStr);
			}

			if (bDeath == true)
			{//���S���Ă����ꍇ
				m_pPlayer[nPlayerIdx]->GetDeath() = true;	//���S������
				if (m_bPlayerDeath[nPlayerIdx] == false)
				{//
					m_bPlayerDeath[nPlayerIdx] = true;		//

					if (m_bPlayerDeath[nPlayerIdx] == true)
					{//���S���Ă���ꍇ
					 //�p�[�e�B�N���̐���
						CParticle::Create(m_pPlayer[nPlayerIdx]->GetModel(0)->GetWorldPos(), 4);
						CParticle::Create(m_pPlayer[nPlayerIdx]->GetModel(0)->GetWorldPos(), 5);

						for (int nCntModel = 0; nCntModel < m_pPlayer[nPlayerIdx]->GetNumParts(); nCntModel++)
						{
							//�`�悵�Ȃ��悤�ɂ���
							m_pPlayer[nPlayerIdx]->GetModel(nCntModel)->SetDisp(false);
						}

						for (int nCntAI = 0; nCntAI < AI_MAX; nCntAI++)
						{
							for (int nCntParts = 0; nCntParts < m_pPlayer[nPlayerIdx]->GetMyAI(nCntAI)->GetNumParts(); nCntParts++)
							{
								//AI��\�������Ȃ�
								m_pPlayer[nPlayerIdx]->GetMyAI(nCntAI)->GetModel(nCntParts)->SetDisp(false);
							}

							//�p�[�e�B�N���𐶐�
							CParticle::Create(m_pPlayer[nPlayerIdx]->GetMyAI(nCntAI)->GetModel(0)->GetWorldPos(), 4);
							CParticle::Create(m_pPlayer[nPlayerIdx]->GetMyAI(nCntAI)->GetModel(0)->GetWorldPos(), 5);

						}

						//�`�[���ʂ̏���
						switch (nTeam)
						{
						case 0:
							m_nBlueLinkEnergy -= 30;	//�u���[�`�[���̃����N�G�l���M�[���Z
							break;
						case 1:
							m_nRedLinkEnergy -= 30;		//���b�h�`�[���̃����N�G�l���M�[���Z
							break;
						}

						//�L�����O�̐ݒu����
						for (int nCntKill = 0; nCntKill < NUM_KILL_LOG; nCntKill++)
						{
							if (CManager::GetGame()->GetLog(nCntKill) == false)
							{
								CManager::GetGame()->SetKillIdx(nCntKill, nKillPlayerIdx);
								CManager::GetGame()->SetDeathIdx(nCntKill, nPlayerIdx);
								CManager::GetGame()->SetPlayerType(0, playerType);
								CManager::GetGame()->SetPlayerType(1, TYPE_PLAYER);
								CManager::GetGame()->SetLog(nCntKill, true);
								break;
							}
						}
					}
				}
			}
			else
			{//�v���C���[
				if (m_bPlayerDeath[nPlayerIdx] == true)
				{
					m_bPlayerDeath[nPlayerIdx] = false;
					m_pPlayer[nPlayerIdx]->GetDeath() = false;
					for (int nCntModel = 0; nCntModel < m_pPlayer[nPlayerIdx]->GetNumParts(); nCntModel++)
					{
						m_pPlayer[nPlayerIdx]->GetModel(nCntModel)->SetDisp(true);
					}

					for (int nCntAI = 0; nCntAI < AI_MAX; nCntAI++)
					{
						for (int nCntParts = 0; nCntParts < m_pPlayer[nPlayerIdx]->GetMyAI(nCntAI)->GetNumParts(); nCntParts++)
						{
							//AI��\��������
							m_pPlayer[nPlayerIdx]->GetMyAI(nCntAI)->GetModel(nCntParts)->SetDisp(true);
						}
					}
				}
			}

			for (int nCntAI = 0; nCntAI < AI_MAX; nCntAI++)
			{

				if (bAIDeath[nCntAI] == true)
				{//AI�����S���Ă���ꍇ
					m_pPlayer[nPlayerIdx]->GetMyAI(nCntAI)->SetDeath(bAIDeath[nCntAI]);	//AI�̎��S�ݒu����
					if (m_bAIDeath[nCntAI][nPlayerIdx] == false)
					{//
						m_bAIDeath[nCntAI][nPlayerIdx] = true;
						if (m_bAIDeath[nCntAI][nPlayerIdx] == true)
						{//
						 //�p�[�e�B�N���𐶐�
							CParticle::Create(m_pPlayer[nPlayerIdx]->GetMyAI(nCntAI)->GetModel(0)->GetWorldPos(), 4);
							CParticle::Create(m_pPlayer[nPlayerIdx]->GetMyAI(nCntAI)->GetModel(0)->GetWorldPos(), 5);
							for (int nCntModel = 0; nCntModel < m_pPlayer[nPlayerIdx]->GetMyAI(nCntAI)->GetNumParts(); nCntModel++)
							{//�\�����Ȃ�
								m_pPlayer[nPlayerIdx]->GetMyAI(nCntAI)->GetModel(nCntModel)->SetDisp(false);
							}

							//�`�[���ʏ���
							switch (nTeam)
							{
							case 0:
								m_nBlueLinkEnergy -= 10;	//�u���[�`�[���̃����N�G�l���M�[���Z
								break;
							case 1:
								m_nRedLinkEnergy -= 10;		//���b�h�`�[���̃����N�G�l���M�[���Z
								break;
							}

							//�L�����O�̐ݒu����
							for (int nCntKill = 0; nCntKill < NUM_KILL_LOG; nCntKill++)
							{
								if (CManager::GetGame()->GetLog(nCntKill) == false)
								{
									CManager::GetGame()->SetKillIdx(nCntKill, nAIKillPlayerIdx[nCntAI]);
									CManager::GetGame()->SetDeathIdx(nCntKill, nPlayerIdx);
									CManager::GetGame()->SetPlayerType(0, AIPlayerType[nCntAI]);
									if (m_pPlayer[nPlayerIdx]->GetMyAI(nCntAI)->GetMechaType() == CAIMecha::MECHATYPE_DRONE)
									{
										CManager::GetGame()->SetPlayerType(1, TYPE_DROWN);
									}
									else if (m_pPlayer[nPlayerIdx]->GetMyAI(nCntAI)->GetMechaType() == CAIMecha::MECHATYPE_WORKER)
									{
										CManager::GetGame()->SetPlayerType(1, TYPE_WORKER);
									}
									CManager::GetGame()->SetLog(nCntKill, true);
									break;
								}
							}
						}
					}
				}
				else
				{
					if (m_bAIDeath[nCntAI][nPlayerIdx] == true)
					{
						m_bAIDeath[nCntAI][nPlayerIdx] = false;
						m_pPlayer[nPlayerIdx]->GetMyAI(nCntAI)->SetDeath(bAIDeath[nCntAI]);

						//�\������
						for (int nCntModel = 0; nCntModel < m_pPlayer[nPlayerIdx]->GetMyAI(nCntAI)->GetNumParts(); nCntModel++)
						{
							m_pPlayer[nPlayerIdx]->GetMyAI(nCntAI)->GetModel(nCntModel)->SetDisp(true);
						}
					}
				}
			}
			if (nPlayerIdx != pClient->GetPlayerIdx())
			{//�v���C���[�̔ԍ����N���C�A���g�ԍ��ƈႤ�ꍇ
				if (m_pPlayer[nPlayerIdx] != NULL)
				{
					//�`�[�������̐ݒu����
					//m_pPlayer[nPlayerIdx]->SetTeam(nTeam);

					SetPlayerData(nPlayerIdx, pos, rot, modelRotUp, modelRotDown, cameraRot);

					//�̗͂̐ݒu����
					m_pPlayer[nPlayerIdx]->SetLife(nLife);

					for (int nCntAI = 0; nCntAI < AI_MAX; nCntAI++)
					{
						//�ʒu�̐ݒu����
						m_pPlayer[nPlayerIdx]->GetMyAI(nCntAI)->SetPos(AIPos[nCntAI]);

						//�����̐ݒu����
						m_pPlayer[nPlayerIdx]->GetMyAI(nCntAI)->SetRot(AIRot[nCntAI]);

						//�̗͂̐ݒu����
						m_pPlayer[nPlayerIdx]->GetMyAI(nCntAI)->SetLife(nAILife[nCntAI]);

						if (bAIShoot[nCntAI] == true)
						{
							D3DXMATRIX mtxCanon = m_pPlayer[nPlayerIdx]->GetMyAI(nCntAI)->GetMtxWorld();

							D3DXVECTOR3 posCanon = D3DXVECTOR3(mtxCanon._41, mtxCanon._42 - 8.0f, mtxCanon._43);
							// �e�̐���
							CBulletPlayer::Create(posCanon, fAIAngle[nCntAI], fAIAngleV[nCntAI], nAIAttack[nCntAI], nTeam, m_pPlayer[nPlayerIdx]->GetMyAI(nCntAI), m_pPlayer[nPlayerIdx]->GetBulletSpeed(), m_pPlayer[nPlayerIdx]->GetBulletLife());

							m_pPlayer[nPlayerIdx]->GetMyAI(nCntAI)->SetShoot(false);
						}
					}

					if (bShoot == true)
					{//�e�𔭎˂��Ă���ꍇ
						CreatePlayerBullet(nPlayerIdx, nNumShoot, nAttack, cameraRot, pAngle, pAngleV);
					}

					if (bChat == true && m_pPlayer[pClient->GetPlayerIdx()]->GetTeam() == nTeam)
					{//�`���b�g���Ă��Ă������`�[���̏ꍇ
						SetChatData(nPlayerIdx, (int)radioChat);
					}

					// �s���̍X�V
					SetPinData(nPlayerIdx, pinPos,bPinUse);

					for (int nCntAI = 0; nCntAI < AI_MAX; nCntAI++)
					{// AI�̐��������
						// AI�s���̍X�V
						SetAIPinData(nPlayerIdx, nCntAI, AIPinPos[nCntAI], bAIPinUse[nCntAI]);
					}

					if (nState == 1)
					{//�I�����Ă���ꍇ
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
		if (CMatching::GetConnect(nCntPlayer) == false)
		{
			D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				//�ʒu
			D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				//����
			D3DXVECTOR3 modelRotDown = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//���f���̉����g�̌���
			D3DXVECTOR3	modelRotUp = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//���f���̏㔼�g�̌���
			D3DXVECTOR3 rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			int nPlayerIdx = 0;												//�v���C���[�̔ԍ�
			int nNumConnect = 0;											//�ڑ�����
			int nWord = 0;													//�����̓��o���p
			int nCntClient = 0;												//�N���C�A���g�̃J�E���^�[
			int nTeam = 0;													//�`�[�����
			float *pAngle = NULL;											//�����p�x
			float *pAngleV = NULL;											//�����p�x
			bool bShoot = false;											//�e�𔭎˂��Ă��邩�ǂ���
			int nNumShoot = 0;												//�������ː�
			int nAttack = 0;												//�U����
			int nLife = 0;													//�̗�
			int nKillPlayerIdx = 0;											//�L���v���C���[�̔ԍ�
			TYPE playerType = TYPE_PLAYER;									//�L���v���C���[�̎��

			bool bPinUse = false;											// �s�����g�p���Ă��邩�ǂ���
			D3DXVECTOR3 pinPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// �s���̈ʒu
			bool bAIPinUse[AI_MAX] = { false, false };						// AI���s�����g�p���Ă��邩�ǂ���
			D3DXVECTOR3 AIPinPos[AI_MAX] = { D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f) };// AI�̃s���̈ʒu

			D3DXVECTOR3 AIPos[AI_MAX] = { D3DXVECTOR3(0.0f,0.0f,0.0f),D3DXVECTOR3(0.0f,0.0f,0.0f) };		//AI�̈ʒu
			D3DXVECTOR3 AIRot[AI_MAX] = { D3DXVECTOR3(0.0f,0.0f,0.0f) ,D3DXVECTOR3(0.0f,0.0f,0.0f) };	//AI�̌���
			bool bAIDeath[AI_MAX] = { false ,false };														//AI������ł��邩�ǂ���
			int nAIKillPlayerIdx[AI_MAX] = { 0,0 };														//AI�L���v���C���[�̔ԍ�
			TYPE AIPlayerType[AI_MAX] = { TYPE_PLAYER,TYPE_PLAYER };										//AI�L���v���C���[�̎��
			bool bAIShoot[AI_MAX] = { false,false };													//AI�e���˂������ǂ���
			float fAIAngle[AI_MAX] = { 0.0f,0.0f };														//AI�p�x
			float fAIAngleV[AI_MAX] = { 0.0f,0.0f };													//AI�p�x
			int nAIAttack[AI_MAX] = { 0, 0 };															//AI�U����
			int nAILife[AI_MAX] = { 0,0 };																//AI�̗�

			if (CServerFunction::Memcmp(pStr, SERVER_AI_PLAYER_DATA) == 0)
			{
				pStr += strlen(SERVER_AI_PLAYER_DATA);

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

				//���f��0�Ԃ̌�������
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

				//���f��1�Ԃ̌�������
				rotDest.x = CServerFunction::ReadFloat(pStr, "");
				nWord = CServerFunction::PopString(pStr, "");
				pStr += nWord;
				rotDest.y = CServerFunction::ReadFloat(pStr, "");
				nWord = CServerFunction::PopString(pStr, "");
				pStr += nWord;
				rotDest.z = CServerFunction::ReadFloat(pStr, "");
				nWord = CServerFunction::PopString(pStr, "");
				pStr += nWord;

				//���S���Ă��邩�ǂ�������
				bool bDeath = CServerFunction::ReadBool(pStr, "");
				nWord = CServerFunction::PopString(pStr, "");
				pStr += nWord;

				if (bDeath == false)
				{//���S���Ă��Ȃ��ꍇ
				 //�̗͂̑��
					nLife = CServerFunction::ReadInt(pStr, "");
					nWord = CServerFunction::PopString(pStr, "");
					pStr += nWord;
				}
				else
				{
					nKillPlayerIdx = CServerFunction::ReadInt(pStr, "");
					nWord = CServerFunction::PopString(pStr, "");
					pStr += nWord;

					//�L���v���C���[�̎�ނ���
					playerType = (TYPE)CServerFunction::ReadInt(pStr, "");
					nWord = CServerFunction::PopString(pStr, "");
					pStr += nWord;
				}

				//�e�𔭎˂��Ă��邩�ǂ�������
				bShoot = CServerFunction::ReadBool(pStr, "");
				nWord = CServerFunction::PopString(pStr, "");
				pStr += nWord;

				if (bShoot == true)
				{//�e�𔭎˂��Ă���ꍇ
				 //�e�̓������ː�����
					nNumShoot = CServerFunction::ReadInt(pStr, "");
					nWord = CServerFunction::PopString(pStr, "");
					pStr += nWord;

					//�����p�x�̓��I�m��
					pAngle = new float[nNumShoot * 2];
					pAngleV = new float[nNumShoot * 2];

					//�U���͂���
					nAttack = CServerFunction::ReadInt(pStr, "");
					nWord = CServerFunction::PopString(pStr, "");
					pStr += nWord;

					//�����p�x����
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

				for (int nCntAI = 0; nCntAI < AI_MAX; nCntAI++)
				{
					//AI�̈ʒu����
					AIPos[nCntAI].x = CServerFunction::ReadFloat(pStr, "");
					nWord = CServerFunction::PopString(pStr, "");
					pStr += nWord;
					AIPos[nCntAI].y = CServerFunction::ReadFloat(pStr, "");
					nWord = CServerFunction::PopString(pStr, "");
					pStr += nWord;
					AIPos[nCntAI].z = CServerFunction::ReadFloat(pStr, "");
					nWord = CServerFunction::PopString(pStr, "");
					pStr += nWord;

					//AI�̌�������
					AIRot[nCntAI].y = CServerFunction::ReadFloat(pStr, "");
					nWord = CServerFunction::PopString(pStr, "");
					pStr += nWord;

					//AI�̎��S���Ă��邩�ǂ�������
					bAIDeath[nCntAI] = CServerFunction::ReadBool(pStr, "");
					nWord = CServerFunction::PopString(pStr, "");
					pStr += nWord;

					if (bAIDeath[nCntAI] == true)
					{
						//�L���v���C���[�̔ԍ�����
						nAIKillPlayerIdx[nCntAI] = CServerFunction::ReadInt(pStr, "");
						nWord = CServerFunction::PopString(pStr, "");
						pStr += nWord;

						//�L���v���C���[�̎�ނ���
						AIPlayerType[nCntAI] = (TYPE)CServerFunction::ReadInt(pStr, "");
						nWord = CServerFunction::PopString(pStr, "");
						pStr += nWord;
					}
					else
					{
						nAILife[nCntAI] = CServerFunction::ReadInt(pStr, "");
						nWord = CServerFunction::PopString(pStr, "");
						pStr += nWord;
					}

					//�e�𔭎˂��Ă��邩�ǂ�������
					bAIShoot[nCntAI] = CServerFunction::ReadBool(pStr, "");
					nWord = CServerFunction::PopString(pStr, "");
					pStr += nWord;

					if (bAIShoot[nCntAI] == true)
					{//�e�𔭎˂��Ă���ꍇ
					 //�U���͂���
						nAIAttack[nCntAI] = CServerFunction::ReadInt(pStr, "");
						nWord = CServerFunction::PopString(pStr, "");
						pStr += nWord;

						//�����p�x�̑��
						fAIAngle[nCntAI] = CServerFunction::ReadFloat(pStr, "");
						nWord = CServerFunction::PopString(pStr, "");
						pStr += nWord;

						fAIAngleV[nCntAI] = CServerFunction::ReadFloat(pStr, "");
						nWord = CServerFunction::PopString(pStr, "");
						pStr += nWord;
					}
				}

				//�s�����g�p���Ă邩�ǂ�������
				bPinUse = CServerFunction::ReadBool(pStr, "");
				nWord = CServerFunction::PopString(pStr, "");
				pStr += nWord;

				if (bPinUse == true)
				{// �s�����g�p���Ă���ꍇ
				 // �s���̈ʒu����
					pinPos.x = CServerFunction::ReadFloat(pStr, "");
					nWord = CServerFunction::PopString(pStr, "");
					pStr += nWord;
					pinPos.y = CServerFunction::ReadFloat(pStr, "");
					nWord = CServerFunction::PopString(pStr, "");
					pStr += nWord;
					pinPos.z = CServerFunction::ReadFloat(pStr, "");
					nWord = CServerFunction::PopString(pStr, "");
					pStr += nWord;
				}

				for (int nCntAI = 0; nCntAI < AI_MAX; nCntAI++)
				{// AI�̐��������
				 // AI���s�����g�p���Ă邩�ǂ�������
					bAIPinUse[nCntAI] = CServerFunction::ReadBool(pStr, "");
					nWord = CServerFunction::PopString(pStr, "");
					pStr += nWord;

					if (bAIPinUse[nCntAI] == true)
					{// AI���s�����g�p���Ă���ꍇ
					 // AI�̃s���̈ʒu����
						AIPinPos[nCntAI].x = CServerFunction::ReadFloat(pStr, "");
						nWord = CServerFunction::PopString(pStr, "");
						pStr += nWord;
						AIPinPos[nCntAI].y = CServerFunction::ReadFloat(pStr, "");
						nWord = CServerFunction::PopString(pStr, "");
						pStr += nWord;
						AIPinPos[nCntAI].z = CServerFunction::ReadFloat(pStr, "");
						nWord = CServerFunction::PopString(pStr, "");
						pStr += nWord;
					}
				}

				if (bDeath == true)
				{//���S���Ă���ꍇ
					m_pPlayer[nPlayerIdx]->GetDeath() = true;
					if (m_bPlayerDeath[nPlayerIdx] == false)
					{
						m_bPlayerDeath[nPlayerIdx] = true;
						if (m_bPlayerDeath[nPlayerIdx] == true)
						{
							//�p�[�e�B�N���̐���
							CParticle::Create(m_pPlayer[nPlayerIdx]->GetModel(0)->GetWorldPos(), 4);
							CParticle::Create(m_pPlayer[nPlayerIdx]->GetModel(0)->GetWorldPos(), 5);

							for (int nCntModel = 0; nCntModel < m_pPlayer[nPlayerIdx]->GetNumParts(); nCntModel++)
							{
								//�`�悵�Ȃ��悤�ɂ���
								m_pPlayer[nPlayerIdx]->GetModel(nCntModel)->SetDisp(false);
							}

							for (int nCntAI = 0; nCntAI < AI_MAX; nCntAI++)
							{
								for (int nCntParts = 0; nCntParts < m_pPlayer[nPlayerIdx]->GetMyAI(nCntAI)->GetNumParts(); nCntParts++)
								{
									//AI��\�������Ȃ�
									m_pPlayer[nPlayerIdx]->GetMyAI(nCntAI)->GetModel(nCntParts)->SetDisp(false);
								}

								//�p�[�e�B�N���𐶐�
								CParticle::Create(m_pPlayer[nPlayerIdx]->GetMyAI(nCntAI)->GetModel(0)->GetWorldPos(), 4);
								CParticle::Create(m_pPlayer[nPlayerIdx]->GetMyAI(nCntAI)->GetModel(0)->GetWorldPos(), 5);

							}

							switch (nTeam)
							{
							case 0:
								m_nBlueLinkEnergy -= 30;	//�u���[�`�[���̃����N�G�l���M�[���Z
								break;
							case 1:
								m_nRedLinkEnergy -= 30;		//���b�h�`�[���̃����N�G�l���M�[���Z
								break;
							}

							//�L�����O�̐ݒu����
							for (int nCntKill = 0; nCntKill < NUM_KILL_LOG; nCntKill++)
							{
								if (CManager::GetGame()->GetLog(nCntKill) == false)
								{
									CManager::GetGame()->SetKillIdx(nCntKill, nKillPlayerIdx);
									CManager::GetGame()->SetDeathIdx(nCntKill, nPlayerIdx);
									CManager::GetGame()->SetPlayerType(0, playerType);
									CManager::GetGame()->SetPlayerType(1, TYPE_PLAYER);
									CManager::GetGame()->SetLog(nCntKill, true);
									break;
								}
							}
						}
					}
				}
				else
				{//���S���Ă��Ȃ��ꍇ
					m_bPlayerDeath[nPlayerIdx] = false;
				}

				for (int nCntAI = 0; nCntAI < AI_MAX; nCntAI++)
				{

					if (bAIDeath[nCntAI] == true)
					{//AI�����S���Ă���ꍇ
						m_pPlayer[nPlayerIdx]->GetMyAI(nCntAI)->SetDeath(bAIDeath[nCntAI]);	//AI�̎��S�ݒu����
						if (m_bAIDeath[nCntAI][nPlayerIdx] == false)
						{//
							m_bAIDeath[nCntAI][nPlayerIdx] = true;
							if (m_bAIDeath[nCntAI][nPlayerIdx] == true)
							{//
							 //�p�[�e�B�N���𐶐�
								CParticle::Create(m_pPlayer[nPlayerIdx]->GetMyAI(nCntAI)->GetModel(0)->GetWorldPos(), 4);
								CParticle::Create(m_pPlayer[nPlayerIdx]->GetMyAI(nCntAI)->GetModel(0)->GetWorldPos(), 5);
								for (int nCntModel = 0; nCntModel < m_pPlayer[nPlayerIdx]->GetMyAI(nCntAI)->GetNumParts(); nCntModel++)
								{//�\�����Ȃ�
									m_pPlayer[nPlayerIdx]->GetMyAI(nCntAI)->GetModel(nCntModel)->SetDisp(false);
								}

								//�`�[���ʏ���
								switch (nTeam)
								{
								case 0:
									m_nBlueLinkEnergy -= 10;	//�u���[�`�[���̃����N�G�l���M�[���Z
									break;
								case 1:
									m_nRedLinkEnergy -= 10;		//���b�h�`�[���̃����N�G�l���M�[���Z
									break;
								}

								//�L�����O�̐ݒu����
								for (int nCntKill = 0; nCntKill < NUM_KILL_LOG; nCntKill++)
								{
									if (CManager::GetGame()->GetLog(nCntKill) == false)
									{
										CManager::GetGame()->SetKillIdx(nCntKill, nAIKillPlayerIdx[nCntAI]);
										CManager::GetGame()->SetDeathIdx(nCntKill, nPlayerIdx);
										CManager::GetGame()->SetPlayerType(0, AIPlayerType[nCntAI]);
										if (m_pPlayer[nPlayerIdx]->GetMyAI(nCntAI)->GetMechaType() == CAIMecha::MECHATYPE_DRONE)
										{
											CManager::GetGame()->SetPlayerType(1, TYPE_DROWN);
										}
										else if (m_pPlayer[nPlayerIdx]->GetMyAI(nCntAI)->GetMechaType() == CAIMecha::MECHATYPE_WORKER)
										{
											CManager::GetGame()->SetPlayerType(1, TYPE_WORKER);
										}
										CManager::GetGame()->SetLog(nCntKill, true);
										break;
									}
								}
							}
						}
					}
					else
					{
						if (m_bAIDeath[nCntAI][nPlayerIdx] == true)
						{
							m_bAIDeath[nCntAI][nPlayerIdx] = false;
							m_pPlayer[nPlayerIdx]->GetMyAI(nCntAI)->SetDeath(bAIDeath[nCntAI]);

							//�\������
							for (int nCntModel = 0; nCntModel < m_pPlayer[nPlayerIdx]->GetMyAI(nCntAI)->GetNumParts(); nCntModel++)
							{
								m_pPlayer[nPlayerIdx]->GetMyAI(nCntAI)->GetModel(nCntModel)->SetDisp(true);
							}
						}
					}
				}

				//CPU�v���C���[�̏���ݒu����
				SetCPUData(nPlayerIdx, pos, rot);

				// �s���̍X�V
				SetPinData(nPlayerIdx, pinPos, bPinUse);

				for (int nCntAI = 0; nCntAI < AI_MAX; nCntAI++)
				{// AI�̐��������
				 // AI�s���̍X�V
					SetAIPinData(nPlayerIdx, nCntAI, AIPinPos[nCntAI], bAIPinUse[nCntAI]);
				}

				//// �p�x�����߂�
				//D3DXVECTOR3 rotCamera = m_pPlayer[nPlayerIdx]->GetDestUpper();
				//float fDiffRot;
				//float fAngle = D3DX_PI + rotCamera.y;
				//float fRotDest = m_pPlayer[nPlayerIdx]->GetRotDest();

				//// �ړI�̊p�x�̒���
				//if (fRotDest > D3DX_PI) { fRotDest -= D3DX_PI * 2.0f; }
				//if (fRotDest < -D3DX_PI) { fRotDest += D3DX_PI * 2.0f; }

				//// �ړI�̊p�x�ւ̍���
				//fDiffRot = fRotDest - rot.y;
				//if (fDiffRot > D3DX_PI) { fDiffRot -= D3DX_PI * 2.0f; }
				//if (fDiffRot < -D3DX_PI) { fDiffRot += D3DX_PI * 2.0f; }

				//// �p�x�̍X�V
				//rot.y += fDiffRot * 0.05f;
				//if (rot.y > D3DX_PI) { rot.y -= D3DX_PI * 2.0f; }
				//if (rot.y < -D3DX_PI) { rot.y += D3DX_PI * 2.0f; }

				//float cameraAngle = fAngle - rot.y;

				//// �����̒���
				//if (cameraAngle > D3DX_PI) { cameraAngle -= D3DX_PI * 2.0f; }
				//if (cameraAngle < -D3DX_PI) { cameraAngle += D3DX_PI * 2.0f; }

				//if (fRotDest <= D3DX_PI * 0.5f && fRotDest >= D3DX_PI * -0.5f)
				//{// �����g�̓�����i�s�����ɍ��킹��
				//	D3DXVECTOR3 modelRot =m_pPlayer[nPlayerIdx]->GetModel(0)->GetRot();
				//	m_pPlayer[nPlayerIdx]->GetModel(0)->SetRot(D3DXVECTOR3(modelRot.x, rot.y + cameraAngle, modelRot.z));
				//	modelRot = m_pPlayer[nPlayerIdx]->GetModel(0)->GetRot();
				//	modelRot = D3DXVECTOR3(-rotCamera.x + (D3DX_PI * 0.5f), cameraAngle - fAngle, modelRot.z);
				//	m_pPlayer[nPlayerIdx]->GetModel(0)->SetRot(modelRot);
				//}
				//else
				//{// �΂ߌ������̂Ƃ�
				//	D3DXVECTOR3 modelRot = m_pPlayer[nPlayerIdx]->GetModel(1)->GetRot();
				//	modelRot = D3DXVECTOR3(-rotCamera.x + (D3DX_PI * 0.5f), cameraAngle - (fAngle - D3DX_PI), modelRot.z);
				//	m_pPlayer[nPlayerIdx]->GetModel(1)->SetRot(modelRot);
				//}

				float fDiffRot;
				float fAngle = D3DX_PI + rotDest.y;
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
					m_pPlayer[nPlayerIdx]->GetModel(1)->SetRot(D3DXVECTOR3(-rotDest.x + (D3DX_PI * 0.5f), fCameraAngle - fAngle, modelRotUp.z));
				}
				else
				{// �΂ߌ������̂Ƃ�
					m_pPlayer[nPlayerIdx]->GetModel(1)->SetRot(D3DXVECTOR3(-rotDest.x + (D3DX_PI * 0.5f), fCameraAngle - (fAngle - D3DX_PI), modelRotUp.z));
				}

				//�v���C���[�̌����̐ݒu����
				m_pPlayer[nPlayerIdx]->SetRot(rot);

				m_pPlayer[nPlayerIdx]->SetDestUpper(rotDest);

				//�v���C���[�̖ړI�̊p�x�̐ݒu����
				//m_pPlayer[nPlayerIdx]->SetRotDest(fRotDest);

				//�̗͂̐ݒu����
				m_pPlayer[nPlayerIdx]->SetLife(nLife);

				if (bShoot == true)
				{//CPU�v���C���[�̒e��������
					CreateCPUBullet(nPlayerIdx, nNumShoot, nAttack, rotDest, pAngle, pAngleV);
				}

				for (int nCntAI = 0; nCntAI < AI_MAX; nCntAI++)
				{
					m_pPlayer[nPlayerIdx]->GetMyAI(nCntAI)->SetPos(AIPos[nCntAI]);
					m_pPlayer[nPlayerIdx]->GetMyAI(nCntAI)->SetRot(AIRot[nCntAI]);
					m_pPlayer[nPlayerIdx]->GetMyAI(nCntAI)->SetLife(nAILife[nCntAI]);
					if (bAIShoot[nCntAI] == true)
					{
						D3DXMATRIX mtxCanon = m_pPlayer[nPlayerIdx]->GetMyAI(nCntAI)->GetMtxWorld();

						D3DXVECTOR3 posCanon = D3DXVECTOR3(mtxCanon._41, mtxCanon._42 - 8.0f, mtxCanon._43);
						// �e�̐���
						CBulletPlayer::Create(posCanon, fAIAngle[nCntAI], fAIAngleV[nCntAI], nAIAttack[nCntAI], nTeam, m_pPlayer[nPlayerIdx]->GetMyAI(nCntAI), m_pPlayer[nPlayerIdx]->GetBulletSpeed(), m_pPlayer[nPlayerIdx]->GetBulletLife());

						m_pPlayer[nPlayerIdx]->GetMyAI(nCntAI)->SetShoot(false);
					}
				}


			}

			//�����p�x�̔j��
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
		CBulletPlayer::Create(posCanon, pAngle[nCntShoot * 2], pAngleV[nCntShoot * 2], nAttack, m_pPlayer[nPlayerIdx]->GetTeam(), m_pPlayer[nPlayerIdx], m_pPlayer[nPlayerIdx]->GetBulletSpeed(), m_pPlayer[nPlayerIdx]->GetBulletLife());
		mtxCanon = m_pPlayer[nPlayerIdx]->GetModel(3)->GetMtxWorld();
		posCanon = D3DXVECTOR3(mtxCanon._41, mtxCanon._42, mtxCanon._43) + D3DXVECTOR3(sinf(cameraRot.y) * 30.0f, cosf(cameraRot.x) * 30.0f, cosf(cameraRot.y) * 30.0f);
		CBulletPlayer::Create(posCanon, pAngle[nCntShoot * 2 + 1], pAngleV[nCntShoot * 2 + 1], nAttack, m_pPlayer[nPlayerIdx]->GetTeam(), m_pPlayer[nPlayerIdx], m_pPlayer[nPlayerIdx]->GetBulletSpeed(), m_pPlayer[nPlayerIdx]->GetBulletLife());

		//�e�𔭎˂��Ă��邩�ǂ����̐ݒu����
		m_pPlayer[nPlayerIdx]->SetShoot(false);
	}
}

//=============================================================================
// �`���b�g���̐ݒu����
//=============================================================================
void CGame::SetChatData(int nPlayerIdx, int radioChat)
{
	//�v���C���[�ԍ��ʂŏ�������
	switch (nPlayerIdx)
	{
	case 0:
		if (m_pPlayer[1]->GetAllyChat() == false)
		{
			m_pPlayer[1]->SetAllyRadioChat((CPlayer::RADIOCHAT)radioChat);
			m_pPlayer[1]->SetAllyChat(true);
		}
		break;
	case 1:
		if (m_pPlayer[0]->GetAllyChat() == false)
		{
			m_pPlayer[0]->SetAllyRadioChat((CPlayer::RADIOCHAT)radioChat);
			m_pPlayer[0]->SetAllyChat(true);
		}
		break;
	case 2:
		if (m_pPlayer[3]->GetAllyChat() == false)
		{
			m_pPlayer[3]->SetAllyRadioChat((CPlayer::RADIOCHAT)radioChat);
			m_pPlayer[3]->SetAllyChat(true);
		}
		break;
	case 3:
		if (m_pPlayer[2]->GetAllyChat() == false)
		{
			m_pPlayer[2]->SetAllyRadioChat((CPlayer::RADIOCHAT)radioChat);
			m_pPlayer[2]->SetAllyChat(true);
		}
		break;
	}
}

//=============================================================================
// �s�����̐ݒu����
//=============================================================================
void CGame::SetPinData(int nPlayerIdx, D3DXVECTOR3 pinPos, bool use)
{
	// �v���C���[�ԍ��ʂŏ�������
	switch (nPlayerIdx)
	{
	case 0:
		// �v���C���[0
		m_pPlayer[1]->GetAllyPinPos() = pinPos;
		m_pPlayer[1]->GetAllyPinUse() = use;
		break;
	case 1:
		// �v���C���[1
		m_pPlayer[0]->GetAllyPinPos() = pinPos;
		m_pPlayer[0]->GetAllyPinUse() = use;
		break;
	case 2:
		// �v���C���[2
		m_pPlayer[3]->GetAllyPinPos() = pinPos;
		m_pPlayer[3]->GetAllyPinUse() = use;
		break;
	case 3:
		// �v���C���[3
		m_pPlayer[2]->GetAllyPinPos() = pinPos;
		m_pPlayer[2]->GetAllyPinUse() = use;
		break;
	}
}

//=============================================================================
// �s�����̐ݒu����
//=============================================================================
void CGame::SetAIPinData(int nPlayerIdx, int AIInd, D3DXVECTOR3 AIPinPos, bool use)
{
	// �v���C���[�ԍ��ʂŏ�������
	switch (nPlayerIdx)
	{
	case 0:
		// �v���C���[0
		m_pPlayer[1]->GetAllyAIPinPos(AIInd) = AIPinPos;
		m_pPlayer[1]->GetAllyAIPinUse(AIInd) = use;
		break;
	case 1:
		// �v���C���[1
		m_pPlayer[0]->GetAllyAIPinPos(AIInd) = AIPinPos;
		m_pPlayer[0]->GetAllyAIPinUse(AIInd) = use;
		break;
	case 2:
		// �v���C���[2
		m_pPlayer[3]->GetAllyAIPinPos(AIInd) = AIPinPos;
		m_pPlayer[3]->GetAllyAIPinUse(AIInd) = use;
		break;
	case 3:
		// �v���C���[3
		m_pPlayer[2]->GetAllyAIPinPos(AIInd) = AIPinPos;
		m_pPlayer[2]->GetAllyAIPinUse(AIInd) = use;
		break;
	}
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
		CBulletPlayer::Create(posCanon, pAngle[nCntShoot * 2], pAngleV[nCntShoot * 2], nAttack, m_pPlayer[nPlayerIdx]->GetTeam(), m_pPlayer[nPlayerIdx], m_pPlayer[nPlayerIdx]->GetBulletSpeed(), m_pPlayer[nPlayerIdx]->GetBulletLife());
		mtxCanon = m_pPlayer[nPlayerIdx]->GetModel(3)->GetMtxWorld();
		posCanon = D3DXVECTOR3(mtxCanon._41, mtxCanon._42, mtxCanon._43) + D3DXVECTOR3(sinf(cameraRot.y) * 30.0f, cosf(cameraRot.x) * 30.0f, cosf(cameraRot.y) * 30.0f);
		CBulletPlayer::Create(posCanon, pAngle[nCntShoot * 2 + 1], pAngleV[nCntShoot * 2 + 1], nAttack, m_pPlayer[nPlayerIdx]->GetTeam(), m_pPlayer[nPlayerIdx], m_pPlayer[nPlayerIdx]->GetBulletSpeed(), m_pPlayer[nPlayerIdx]->GetBulletLife());

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

//=============================================================================
// �L�����O�̐�������
//=============================================================================
void CGame::CreateKillLog(void)
{
	//if (m_apKillLogBase[0] == NULL)
	//{
	//	m_apKillLogBase[0] = CUI_TEXTURE::Create(D3DXVECTOR3(1125.0f, 30.0f, 0.0f), 250.0f, 50.0f, CUI_TEXTURE::UIFLAME_KILL_LOG_BG);
	//	m_apKillLogBase[0]->SetTex(0, 1, 2);

	//	m_apKillLogPlayerIcon[0][0] = CUI_TEXTURE::Create(D3DXVECTOR3(1035.0f, 30.0f, 0.0f), 45.0f, 45.0f, CUI_TEXTURE::UIFLAME_KILL_LOG_PLAYERICON);
	//	m_apKillLogPlayerIcon[0][0]->SetTex(m_aMechaType[m_nKillIdx[0]], 1, 6);
	//	m_apKillLogPlayerIcon[0][1] = CUI_TEXTURE::Create(D3DXVECTOR3(1215.0f, 30.0f, 0.0f), 45.0f, 45.0f, CUI_TEXTURE::UIFLAME_KILL_LOG_PLAYERICON);
	//	m_apKillLogPlayerIcon[0][1]->SetTex(m_aMechaType[m_nDeathIdx[0]], 1, 6);

	//	m_apKillLogPlayerIdx[0][0] = CUI_TEXTURE::Create(D3DXVECTOR3(1070.0f, 40.0f, 0.0f), 35.0f, 35.0f, CUI_TEXTURE::UIFLAME_KILL_LOG_PLAYERIDX);
	//	m_apKillLogPlayerIdx[0][0]->SetTex(m_nKillIdx[0], 1, 4);

	//	m_apKillLogPlayerIdx[0][1] = CUI_TEXTURE::Create(D3DXVECTOR3(1175.0f, 40.0f, 0.0f), 35.0f, 35.0f, CUI_TEXTURE::UIFLAME_KILL_LOG_PLAYERIDX);
	//	m_apKillLogPlayerIdx[0][1]->SetTex(m_nDeathIdx[0], 1, 4);

	//}

	for (int nCntLog = 0; nCntLog < NUM_KILL_LOG; nCntLog++)
	{
		if (m_bLog[nCntLog] == true && m_apKillLogBase[nCntLog] == NULL && m_apKillLogPlayerIcon[nCntLog][0] == NULL && m_apKillLogPlayerIcon[nCntLog][1] == NULL && m_apKillLogPlayerIdx[nCntLog][0] == NULL && m_apKillLogPlayerIdx[nCntLog][1] == NULL)
		{//���O���g�p����Ă��銎�Ώۂ̂t�h���S��NULL�̏ꍇ
		 //UI�̐�������
			m_apKillLogBase[nCntLog] = CUI_TEXTURE::Create(D3DXVECTOR3(1125.0f, 30.0f, 0.0f), 250.0f, 50.0f, CUI_TEXTURE::UIFLAME_KILL_LOG_BG);
			m_apKillLogBase[nCntLog]->SetObjType(OBJTYPE_KILLLOG);
			if (m_nKillIdx[nCntLog] == 0 || m_nKillIdx[nCntLog] == 1)
			{
				m_apKillLogBase[nCntLog]->SetTex(0, 1, 2);
			}
			if (m_nKillIdx[nCntLog] == 2 || m_nKillIdx[nCntLog] == 3)
			{
				m_apKillLogBase[nCntLog]->SetTex(1, 1, 2);
			}

			m_apKillLogPlayerIcon[nCntLog][0] = CUI_TEXTURE::Create(D3DXVECTOR3(1035.0f, 30.0f, 0.0f), 45.0f, 45.0f, CUI_TEXTURE::UIFLAME_KILL_LOG_PLAYERICON);
			m_apKillLogPlayerIcon[nCntLog][0]->SetObjType(OBJTYPE_KILLLOG);

			m_apKillLogPlayerIcon[nCntLog][1] = CUI_TEXTURE::Create(D3DXVECTOR3(1215.0f, 30.0f, 0.0f), 45.0f, 45.0f, CUI_TEXTURE::UIFLAME_KILL_LOG_PLAYERICON);
			m_apKillLogPlayerIcon[nCntLog][1]->SetObjType(OBJTYPE_KILLLOG);

			if (m_playerType[0] == TYPE_PLAYER)
			{
				m_apKillLogPlayerIcon[nCntLog][0]->SetTex(m_aMechaType[m_nKillIdx[nCntLog]], 1, 6);
			}
			else if (m_playerType[0] == TYPE_DROWN)
			{
				m_apKillLogPlayerIcon[nCntLog][0]->SetTex(4, 1, 6);
			}
			else if (m_playerType[0] == TYPE_WORKER)
			{
				m_apKillLogPlayerIcon[nCntLog][0]->SetTex(5, 1, 6);
			}

			if (m_playerType[1] == TYPE_PLAYER)
			{
				m_apKillLogPlayerIcon[nCntLog][1]->SetTex(m_aMechaType[m_nDeathIdx[nCntLog]], 1, 6);
			}
			else if (m_playerType[1] == TYPE_DROWN)
			{
				m_apKillLogPlayerIcon[nCntLog][1]->SetTex(4, 1, 6);
			}
			else if (m_playerType[1] == TYPE_WORKER)
			{
				m_apKillLogPlayerIcon[nCntLog][1]->SetTex(5, 1, 6);
			}

			m_apKillLogPlayerIdx[nCntLog][0] = CUI_TEXTURE::Create(D3DXVECTOR3(1070.0f, 40.0f, 0.0f), 35.0f, 35.0f, CUI_TEXTURE::UIFLAME_KILL_LOG_PLAYERIDX);
			m_apKillLogPlayerIdx[nCntLog][0]->SetObjType(OBJTYPE_KILLLOG);
			m_apKillLogPlayerIdx[nCntLog][0]->SetTex(m_nKillIdx[nCntLog], 1, 4);

			m_apKillLogPlayerIdx[nCntLog][1] = CUI_TEXTURE::Create(D3DXVECTOR3(1175.0f, 40.0f, 0.0f), 35.0f, 35.0f, CUI_TEXTURE::UIFLAME_KILL_LOG_PLAYERIDX);
			m_apKillLogPlayerIdx[nCntLog][1]->SetObjType(OBJTYPE_KILLLOG);
			m_apKillLogPlayerIdx[nCntLog][1]->SetTex(m_nDeathIdx[nCntLog], 1, 4);

			//m_apKillLogBase[nCntLog] = CUI_TEXTURE::Create(D3DXVECTOR3(1000.0f, 100.0f, 0.0f), 300.0f, 100.0f, CUI_TEXTURE::UIFLAME_KILL_LOG_BG);
			//m_apKillLogPlayerIcon[nCntLog][0] = CUI_TEXTURE::Create(D3DXVECTOR3(1000.0f, 100.0f, 0.0f), 150.0f, 100.0f, CUI_TEXTURE::UIFLAME_KILL_LOG_BG);
			if (nCntLog >= 1)
			{//�J�E���^�[���P�ȏ�̏ꍇ
				for (int nCntDown = 0; nCntDown < nCntLog; nCntDown++)
				{
					if (nCntDown != nCntLog)
					{//���O�J�E���^�[�ƃ��O��������J�E���^�[���Ⴄ�ꍇ
					 //UI��������
						m_apKillLogBase[nCntDown]->SetPos(D3DXVECTOR3(m_apKillLogBase[nCntDown]->GetPos().x, m_apKillLogBase[nCntDown]->GetPos().y + 50.0f, m_apKillLogBase[nCntDown]->GetPos().z));
						m_apKillLogPlayerIcon[nCntDown][0]->SetPos(D3DXVECTOR3(m_apKillLogPlayerIcon[nCntDown][0]->GetPos().x, m_apKillLogPlayerIcon[nCntDown][0]->GetPos().y + 50.0f, m_apKillLogPlayerIcon[nCntDown][0]->GetPos().z));
						m_apKillLogPlayerIcon[nCntDown][1]->SetPos(D3DXVECTOR3(m_apKillLogPlayerIcon[nCntDown][1]->GetPos().x, m_apKillLogPlayerIcon[nCntDown][1]->GetPos().y + 50.0f, m_apKillLogPlayerIcon[nCntDown][1]->GetPos().z));
						m_apKillLogPlayerIdx[nCntDown][0]->SetPos(D3DXVECTOR3(m_apKillLogPlayerIdx[nCntDown][0]->GetPos().x, m_apKillLogPlayerIdx[nCntDown][0]->GetPos().y + 50.0f, m_apKillLogPlayerIdx[nCntDown][0]->GetPos().z));
						m_apKillLogPlayerIdx[nCntDown][1]->SetPos(D3DXVECTOR3(m_apKillLogPlayerIdx[nCntDown][1]->GetPos().x, m_apKillLogPlayerIdx[nCntDown][1]->GetPos().y + 50.0f, m_apKillLogPlayerIdx[nCntDown][1]->GetPos().z));
					}
				}
			}
		}
	}

}


//=============================================================================
// �L�����O�̍X�V����
//=============================================================================
void CGame::UpdateKillLog(void)
{
	for (int nCntLog = 0; nCntLog < NUM_KILL_LOG; nCntLog++)
	{
		if (m_bLog[nCntLog] == true)
		{//���O���g�p����Ă��銎�Ώۂ̂t�h���S��NULL�ł͂Ȃ��ꍇ
			m_nCntDrawLog[nCntLog]++;	//���O�̕`��J�E���^�[�����Z
			if (m_nCntDrawLog[nCntLog] >= 300)
			{//���O�̕`��J�E���^�[��300�ȏ�̏ꍇ
				//�F���擾

				D3DXCOLOR col = m_apKillLogBase[nCntLog]->GetColor();
				col.a -= 0.05f;	//�F�̌��Z

				//UI�̐F��ݒu����
				if (m_apKillLogBase[nCntLog] != NULL) { m_apKillLogBase[nCntLog]->SetColor(col); }
				if (m_apKillLogPlayerIcon[nCntLog][0] != NULL) {m_apKillLogPlayerIcon[nCntLog][0]->SetColor(col);}
				if (m_apKillLogPlayerIcon[nCntLog][1] != NULL) {m_apKillLogPlayerIcon[nCntLog][1]->SetColor(col);}
				if(m_apKillLogPlayerIdx[nCntLog][0] != NULL){ m_apKillLogPlayerIdx[nCntLog][0]->SetColor(col); }
				if(m_apKillLogPlayerIdx[nCntLog][1] != NULL){ m_apKillLogPlayerIdx[nCntLog][1]->SetColor(col); }

				if (col.a <= 0.0f)
				{//�����x��0�ȉ��̏ꍇ
					m_nCntDrawLog[nCntLog] = 0;	//���O�̕`��J�E���^�|��������
					m_bLog[nCntLog] = false;	//���O���g�p���Ă��Ȃ���Ԃɂ���

					//���O�̊J������
					ReleaseKillLog(nCntLog);
				}
			}
		}
	}
}

//=============================================================================
// �L�����O�̊J������
//=============================================================================
void CGame::ReleaseKillLog(int nIdx)
{
	if (m_apKillLogBase[nIdx] != NULL)
	{
		m_apKillLogBase[nIdx]->Uninit();
		m_apKillLogBase[nIdx] = NULL;
	}
	for (int nCntPlayer = 0; nCntPlayer < NUM_KILL_LOG_PLAYER; nCntPlayer++)
	{
		if (m_apKillLogPlayerIcon[nIdx][nCntPlayer] != NULL)
		{
			m_apKillLogPlayerIcon[nIdx][nCntPlayer]->Uninit();
			m_apKillLogPlayerIcon[nIdx][nCntPlayer] = NULL;

		}
		if (m_apKillLogPlayerIdx[nIdx][nCntPlayer] != NULL)
		{
			m_apKillLogPlayerIdx[nIdx][nCntPlayer]->Uninit();
			m_apKillLogPlayerIdx[nIdx][nCntPlayer] = NULL;

		}

	}
}