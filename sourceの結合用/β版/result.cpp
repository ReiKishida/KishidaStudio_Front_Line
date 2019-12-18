//=============================================================================
//
// ���U���g���� [result.cpp]
// Author : Ayaka Hakozaki
//
//=============================================================================
#include "result.h"
#include "fade.h"
#include "input.h"
#include "title.h"
#include "manager.h"
#include "debugProc.h"
#include "number.h"
#include "sound.h"
#include "UI_Texture.h"
#include "camera.h"
#include "model.h"
#include "player.h"
#include "mechaSelect.h"
#include "motion.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define RESULT_PRESS_WIDTH		(500.0f)		// ���͑҂��\���̕�
#define RESULT_PRESS_HEIGHT		(100.0f)		// ���͑҂��\���̍���

#define RESULT_LOGO_WIDTH			(450.0f)		// ���S�̕�
#define RESULT_LOGO_HEIGHT		(150.0f)		// ���S�̍���

#define TEAM_WIN_LOGO					(2)				// ���S�̐�
#define RESULT_FLASH						(40)				// �{�^������͂���O�̓_��
#define FIELD_MODEL_NAME			"data/MODEL/map_UV_bill.x"

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
CResult::TEAM_WIN CResult::m_teamWin = TEAM_WIN_BLUE;
CPlayer *CResult::m_pPlayer[MAX_PLAYER_CONNECT] = {};

//=========================================
// �R���X�g���N�^
//=========================================
CResult::CResult(int nPriority, CScene::OBJTYPE objType) : CScene(nPriority, objType)
{
	for (int nCnt = 0; nCnt < RESULT_UI_TEX; nCnt++)
	{
		m_pUITex[nCnt] = NULL;
	}

	for (int nCntType = 0; nCntType < MAX_PLAYER_CONNECT; nCntType++)
	{
		m_mechaType[nCntType] = CMechaSelect::MECHATYPE_EMPTY;
	}

	m_pField = NULL;
	m_nCntFlash = 0;
}

//=========================================
// �f�X�g���N�^
//=========================================
CResult::~CResult()
{
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CResult::Init(void)
{
	CMotionManager::Load();

	// �I�u�W�F�N�g�^�C�v�̐ݒ�
	//CScene::SetObjType(CScene::OBJTYPE_RESULT);

	// ���[�h�̎擾
	CManager::MODE mode = CManager::GetMode();

	// �t�B�[���h�̐���
	m_pField = CModel::Create();
	m_pField->SetModel(FIELD_MODEL_NAME);

	// �_��
	m_nCntFlash = RESULT_FLASH;

	// �v���X�{�^������
	if (m_pUITex[0] == NULL)
	{
		m_pUITex[0] = CUI_TEXTURE::Create(D3DXVECTOR3(1000.0f, 650.0f, 0.0f), RESULT_PRESS_WIDTH, RESULT_PRESS_HEIGHT, CUI_TEXTURE::UIFLAME_PRESSBOTTON);		// �v���X�{�^��
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CResult::Uninit(void)
{
	CMotionManager::Unload();

	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER_CONNECT; nCntPlayer++)
	{
		if (m_pPlayer[nCntPlayer] != NULL)
		{
			m_pPlayer[nCntPlayer]->Uninit();
			m_pPlayer[nCntPlayer] = NULL;
		}
	}

	for (int nCnt = 0; nCnt < RESULT_UI_TEX; nCnt++)
	{
		if (m_pUITex[nCnt] != NULL)
		{	// UI�e�N�X�`���̔j��
			m_pUITex[nCnt]->Uninit();
			m_pUITex[nCnt] = NULL;
		}
	}

	if (m_pField != NULL)
	{	// �t�B�[���h�̔j��
		m_pField->Uninit();
		delete m_pField;
		m_pField = NULL;
	}

	// �N���C�A���g�̔j��
	CManager::ReleaseClient();

	// ������j��
	CScene::Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CResult::Update(void)
{
	// ���͂̏����擾
	CInputKeyboard	*pKeyboard = CManager::GetInputKeyboard();	// �L�[�{�[�h�̎擾
	CXInput			*pXInput = CManager::GetXInput();			// XInput�̎擾
	CSound			*pSound = CManager::GetSound();				// �T�E���h�̎擾

	// �v���X�{�^���_��
	if (m_pUITex[0] != NULL)
	{
		m_pUITex[0]->Flashing(m_nCntFlash);
	}

	switch (m_teamWin)
	{
	case TEAM_WIN_BLUE:
		if (m_pUITex[1] == NULL)
		{
			m_pUITex[1] = CUI_TEXTURE::Create(D3DXVECTOR3(250.0f, 100.0f, 0.0f), RESULT_LOGO_WIDTH, RESULT_LOGO_HEIGHT, CUI_TEXTURE::UIFLAME_TEAM_WIN);		// �`�[�����S
			m_pUITex[1]->SetTex(0, 1, TEAM_WIN_LOGO);

			if (m_pPlayer[0] == NULL && m_pPlayer[1] == NULL && m_pPlayer[2] == NULL && m_pPlayer[3] == NULL)
			{
				// BLUE����O
				m_pPlayer[0] = CPlayer::Create(0, CGame::GetMechaType(0), D3DXVECTOR3(23.0f, 0.0f, -89.0f), true);
				m_pPlayer[0]->SetRot(D3DXVECTOR3(0.0f, 100.0f, 0.0f));
				m_pPlayer[1] = CPlayer::Create(1, CGame::GetMechaType(1), D3DXVECTOR3(-15.0f, 0.0f, -139.0f), true);
				m_pPlayer[1]->SetRot(D3DXVECTOR3(0.0f, 100.0f, 0.0f));

				// RED����
				m_pPlayer[2] = CPlayer::Create(2, CGame::GetMechaType(2), D3DXVECTOR3(-133.0f, 0.0f, -160.0f), true);
				m_pPlayer[2]->SetRot(D3DXVECTOR3(0.0f, -200.0f, 0.0f));
				m_pPlayer[3] = CPlayer::Create(3, CGame::GetMechaType(3), D3DXVECTOR3(-133.0f, 0.0f, -115.0f), true);
				m_pPlayer[3]->SetRot(D3DXVECTOR3(0.0f, -200.0f, 0.0f));
			}
		}
		break;

	case TEAM_WIN_RED:
		if (m_pUITex[1] == NULL)
		{
			m_pUITex[1] = CUI_TEXTURE::Create(D3DXVECTOR3(250.0f, 100.0f, 0.0f), RESULT_LOGO_WIDTH, RESULT_LOGO_HEIGHT, CUI_TEXTURE::UIFLAME_TEAM_WIN);		// �`�[�����S
			m_pUITex[1]->SetTex(1, 1, TEAM_WIN_LOGO);
		}

		if (m_pPlayer[0] == NULL && m_pPlayer[1] == NULL && m_pPlayer[2] == NULL && m_pPlayer[3] == NULL)
		{
			// BLUE����
			m_pPlayer[0] = CPlayer::Create(0, CGame::GetMechaType(0), D3DXVECTOR3(-133.0f, 0.0f, -160.0f), true);
			m_pPlayer[0]->SetRot(D3DXVECTOR3(0.0f, -200.0f, 0.0f));
			m_pPlayer[1] = CPlayer::Create(1, CGame::GetMechaType(1), D3DXVECTOR3(-133.0f, 0.0f, -115.0f), true);
			m_pPlayer[1]->SetRot(D3DXVECTOR3(0.0f, -200.0f, 0.0f));

			// RED����O
			m_pPlayer[2] = CPlayer::Create(2, CGame::GetMechaType(2), D3DXVECTOR3(23.0f, 0.0f, -89.0f), true);
			m_pPlayer[2]->SetRot(D3DXVECTOR3(0.0f, 100.0f, 0.0f));
			m_pPlayer[3] = CPlayer::Create(3, CGame::GetMechaType(3), D3DXVECTOR3(-15.0f, 0.0f, -139.0f), true);
			m_pPlayer[3]->SetRot(D3DXVECTOR3(0.0f, 100.0f, 0.0f));
		}
		break;
	}

	if (pKeyboard->GetTrigger(DIK_RETURN))
	{// ��ʑJ��
		if (CFade::GetFade() == CFade::FADE_NONE)
		{// �t�F�[�h���Ȃ��Ƃ�
			CFade::Create(CManager::MODE_TITLE);
			//pSound->PlaySound(CSound::SOUND_LABEL_DECIDE);
		}
	}

	CDebugProc::Print("���U���g");
}

//=============================================================================
// �`�揈��
//=============================================================================
void CResult::Draw(void)
{
	if (m_pField != NULL)
	{	// �t�B�[���h�̕`��
		m_pField->Draw();
	}
}
