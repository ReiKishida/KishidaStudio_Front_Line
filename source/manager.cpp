//=============================================================================
//
// �}�l�[�W���[���� [manager.cpp]
// Author : Takuto Ishida
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "input.h"
#include "camera.h"
#include "light.h"
#include "debugProc.h"
#include "title.h"
#include "menu.h"
#include "tutorial.h"
#include "game.h"
#include "result.h"
#include "pause.h"
#include "particle.h"
#include "meshField.h"
#include "sound.h"
#include "texture.h"
#include "mechaSelect.h"
#include <stdio.h>

//=========================================
// �}�N����`
//=========================================
#define SYSTEM_FILE	"data/TEXT/system.ini"		// �n�ʂ̏��e�L�X�g

//=========================================
// �ÓI�����o�ϐ�
//=========================================
CRenderer			*CManager::m_pRenderer = NULL;			// �����_���[�N���X�̃|�C���^�ϐ�
CInputKeyboard		*CManager::m_pInputKeyboard = NULL;		// �C���v�b�g�N���X�̃|�C���^�ϐ�
CInputMouse			*CManager::m_pInputMouse = NULL;		// �}�E�X�N���X�̃|�C���^�ϐ�
CDirectInput		*CManager::m_pDirectInput = NULL;		// DirectInput�N���X�^�̃|�C���^
CXInput				*CManager::m_pXInput = NULL;			// XInput�N���X�^�̃|�C���^
CCamera				*CManager::m_pCamera = NULL;			// �J�����N���X�̃|�C���^�ϐ�
CLight				*CManager::m_pLight = NULL;				// ���C�g�N���X�̃|�C���^�ϐ�
CDebugProc			*CManager::m_pDebugProc = NULL;			// �f�o�b�O�����N���X�̃|�C���^�ϐ�
CSound				*CManager::m_pSound = NULL;				// �T�E���h�N���X�̃|�C���^�ϐ�
CTitle				*CManager::m_pTitle = NULL;				// �^�C�g���N���X�̃|�C���^�ϐ�
CMenu				*CManager::m_pMenu = NULL;				// ���j���[�N���X�̃|�C���^�ϐ�
CMechaSelect		*CManager::m_pMechaSelect = NULL;
CTutorial			*CManager::m_pTutorial = NULL;			// �`���[�g���A���N���X�̃|�C���^�ϐ�
CGame				*CManager::m_pGame = NULL;				// �Q�[���N���X�̃|�C���^�ϐ�
CResult				*CManager::m_pResult = NULL;			// ���U���g�N���X�̃|�C���^�ϐ�
CRanking			*CManager::m_pRanking = NULL;			// �����L���O�N���X�̃|�C���^�ϐ�
CManager::MODE		CManager::m_mode = CManager::MODE_GAME;

int  CManager::m_nNumStage = 0;

//=========================================
// �R���X�g���N�^
//=========================================
CManager::CManager()
{
}

//=========================================
// �f�X�g���N�^
//=========================================
CManager::~CManager()
{
}

//=========================================
// ����������
//=========================================
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, bool bWindow)
{
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// �����_�����O�N���X�̐���
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	if (NULL == m_pRenderer)
	{// ����������
		m_pRenderer = new CRenderer;	// �������m��

		if (NULL != m_pRenderer)
		{// ���������m�ۂł���
			if (FAILED(m_pRenderer->Init(hInstance, hWnd, bWindow)))	//FALSE�Ńt���X�N���[��
			{// ����������
				return -1;
			}
		}
		else
		{// �������m�ۂɎ��s
			MessageBox(0, "�����_�����O�N���X�̃��������m�ۂł��܂���ł���", "���s", MB_OK);
		}
	}
	else
	{// ��łȂ������Ƃ�
		MessageBox(0, "�����_�����O�N���X�ɉ��������Ă��܂�", "���s", MB_OK);
	}

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// �C���v�b�g(�L�[�{�[�h)�N���X�̐���
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	if (NULL == m_pInputKeyboard)
	{// ����������
		m_pInputKeyboard = new CInputKeyboard;	// �L�[�{�[�h�̓��͗p

		if (NULL != m_pInputKeyboard)
		{// �������m��
			if (FAILED(m_pInputKeyboard->Init(hInstance, hWnd)))
			{// ����������
				return -1;
			}
		}
		else
		{// �������m�ۂɎ��s
			MessageBox(0, "�C���v�b�g(�L�[�{�[�h)�N���X�̃��������m�ۂł��܂���ł���", "���s", MB_OK);
		}
	}
	else
	{// ��łȂ������Ƃ�
		MessageBox(0, "�C���v�b�g(�L�[�{�[�h)�N���X�ɉ��������Ă��܂�", "���s", MB_OK);
	}


	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// �C���v�b�g(�L�[�{�[�h)�N���X�̐���
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	if (NULL == m_pInputMouse)
	{// ����������
		m_pInputMouse = new CInputMouse;	// �}�E�X�̓��͗p

		if (NULL != m_pInputMouse)
		{// �������m��
			if (FAILED(m_pInputMouse->Init(hInstance, hWnd)))
			{// ����������
				return -1;
			}
		}
		else
		{// �������m�ۂɎ��s
			MessageBox(0, "�C���v�b�g(�}�E�X)�N���X�̃��������m�ۂł��܂���ł���", "���s", MB_OK);
		}
	}
	else
	{// ��łȂ������Ƃ�
		MessageBox(0, "�C���v�b�g(�}�E�X)�N���X�ɉ��������Ă��܂�", "���s", MB_OK);
	}

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// �C���v�b�g(DirectInput)�N���X�̐���
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	if (NULL == m_pDirectInput)
	{// ����������
		m_pDirectInput = new CDirectInput;	// �L�[�{�[�h�̓��͗p

		if (NULL != m_pDirectInput)
		{// �������m��
			if (FAILED(m_pDirectInput->Init(hInstance, hWnd)))
			{// ����������
				return -1;
			}
		}
		else
		{// �������m�ۂɎ��s
			MessageBox(0, "�C���v�b�g(DirectInput)�N���X�̃��������m�ۂł��܂���ł���", "���s", MB_OK);
		}
	}

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// �C���v�b�g(XInput)�N���X�̐���
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	if (NULL == m_pXInput)
	{// ����������
		m_pXInput = new CXInput;	// �L�[�{�[�h�̓��͗p

		if (NULL != m_pXInput)
		{// �������m��
			if (FAILED(m_pXInput->Init()))
			{// ����������
				return -1;
			}
		}
		else
		{// �������m�ۂɎ��s
			MessageBox(0, "�C���v�b�g(XInput)�N���X�̃��������m�ۂł��܂���ł���", "���s", MB_OK);
		}
	}
	else
	{// ��łȂ������Ƃ�
		MessageBox(0, "�C���v�b�g(XInput)�N���X�ɉ��������Ă��܂�", "���s", MB_OK);
	}

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// �J�����N���X�̐���
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	if (NULL == m_pCamera)
	{// �k���`�F�b�N
		m_pCamera = new CCamera;	// �������m��

		if (NULL != m_pCamera)
		{// ���������m�ۂł������ǂ���
			m_pCamera->Init();
		}
		else
		{// �������m�ۂɎ��s
			MessageBox(0, "�J�����N���X�̃��������m�ۂł��܂���ł���", "���s", MB_OK);
		}
	}
	else
	{// NULL�łȂ������Ƃ�
		MessageBox(0, "�J�����N���X�ɉ��������Ă��܂�", "���s", MB_OK);
	}

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// ���C�g�N���X�̐���
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	if (NULL == m_pLight)
	{// �k���`�F�b�N
		m_pLight = new CLight;	// �������m��

		if (NULL != m_pLight)
		{// ���������m�ۂł������ǂ���
			m_pLight->Init();
		}
		else
		{// �������m�ۂɎ��s
			MessageBox(0, "���C�g�N���X�̃��������m�ۂł��܂���ł���", "���s", MB_OK);
		}
	}
	else
	{// NULL�łȂ������Ƃ�
		MessageBox(0, "���C�g�N���X�ɉ��������Ă��܂�", "���s", MB_OK);
	}

#ifdef _DEBUG
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// �f�o�b�O�����N���X�̐���
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	if (NULL == m_pDebugProc)
	{// �k���`�F�b�N
		m_pDebugProc = new CDebugProc;	// �������m��

		if (NULL != m_pDebugProc)
		{// ���������m�ۂł������ǂ���
			m_pDebugProc->Init();
		}
		else
		{// �������m�ۂɎ��s
			MessageBox(0, "�f�o�b�O�����N���X�̃��������m�ۂł��܂���ł���", "���s", MB_OK);
		}
	}
	else
	{// NULL�łȂ������Ƃ�
		MessageBox(0, "�f�o�b�O�����N���X�ɉ��������Ă��܂�", "���s", MB_OK);
	}
#endif

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// �T�E���h�N���X�̐���
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	if (NULL == m_pSound)
	{// �k���`�F�b�N
		m_pSound = new CSound;	// �������m��

		if (NULL != m_pSound)
		{// ���������m�ۂł������ǂ���
			m_pSound->Init(hWnd);
		}
		else
		{// �������m�ۂɎ��s
			MessageBox(0, "�T�E���h�N���X�̃��������m�ۂł��܂���ł���", "���s", MB_OK);
		}
	}
	else
	{// NULL�łȂ������Ƃ�
		MessageBox(0, "�T�E���h�N���X�ɉ��������Ă��܂�", "���s", MB_OK);
	}

	// �e�N�X�`���̓ǂݍ���
	CTexture::Load();

	// ���݂̃��[�h��ݒ�
	SetMode(m_mode);

	// �t�@�C������f�[�^��ǂݎ��
	CParData::Load();

	// �f�o�b�O�����̕\��
	m_bDebugProc = true;

	return S_OK;
}

//=========================================
// �I������
//=========================================
void CManager::Uninit(void)
{
	// �f�[�^�̔j��
	CParData::Unload();

	CScene::ReleaseAll();	// �S�ẴI�u�W�F�N�g�̔j��

	CTexture::Unload();

	if (NULL != m_pRenderer)
	{// �����_�����O�N���X�̔j��
		m_pRenderer->Uninit();
		delete m_pRenderer;
		m_pRenderer = NULL;
	}

	if (NULL != m_pInputKeyboard)
	{// �C���v�b�g(�L�[�{�[�h)�N���X�̔j��
		m_pInputKeyboard->Uninit();
		delete m_pInputKeyboard;
		m_pInputKeyboard = NULL;
	}

	if (NULL != m_pInputMouse)
	{// �C���v�b�g(�}�E�X)�N���X�̔j��
		m_pInputMouse->Uninit();
		delete m_pInputMouse;
		m_pInputMouse = NULL;
	}

	if (NULL != m_pDirectInput)
	{// �C���v�b�g(DirectInput)�N���X�̔j��
		m_pDirectInput->Uninit();
		delete m_pDirectInput;
		m_pDirectInput = NULL;
	}

	if (NULL != m_pXInput)
	{// �C���v�b�g(XInput)�N���X�̔j��
		m_pXInput->Uninit();
		delete m_pXInput;
		m_pXInput = NULL;
	}

	if (NULL != m_pCamera)
	{// �J�����N���X�̔j��
		m_pCamera->Uninit();
		delete m_pCamera;
		m_pCamera = NULL;
	}

	if (NULL != m_pLight)
	{// ���C�g�N���X�̔j��
		m_pLight->Uninit();
		delete m_pLight;
		m_pLight = NULL;
	}

#ifdef _DEBUG
	if (NULL != m_pDebugProc)
	{// �f�o�b�O�����N���X�̔j��
		m_pDebugProc->Uninit();
		delete m_pDebugProc;
		m_pDebugProc = NULL;
	}
#endif

	if (NULL != m_pSound)
	{// �T�E���h�N���X�̔j��
		m_pSound->Uninit();
		delete m_pSound;
		m_pSound = NULL;
	}
}

//=========================================
// �X�V����
//=========================================
void CManager::Update(void)
{
	m_pInputKeyboard->Update();		// �C���v�b�g�N���X�̍X�V
	m_pInputMouse->Update();
	m_pDirectInput->Update();
	m_pXInput->Update();

	if (m_pGame != NULL)
	{// �Q�[����
		if (!m_pGame->GetPauseBool())
		{// �|�[�Y���łȂ��Ƃ�
			m_pRenderer->Update();			// �����_���[�N���X�̍X�V
			m_pLight->Update();				// ���C�g�N���X�̍X�V
		}
		else
		{// �|�[�Y��
			CPause *pPause = m_pGame->GetPause();
			if (pPause != NULL)
			{
				pPause->Update();
			}

			m_pGame->Update();
		}
	}
	else
	{// �Q�[���ȊO
		m_pRenderer->Update();			// �����_���[�N���X�̍X�V
		m_pLight->Update();				// ���C�g�N���X�̍X�V
	}

#ifdef _DEBUG
	if (m_pInputKeyboard->GetTrigger(DIK_F1))
	{
		m_bDebugProc = m_bDebugProc ? false : true;
		m_pDebugProc->DebugDisp(m_bDebugProc);
	}
#endif
}

//=========================================
// �`�揈��
//=========================================
void CManager::Draw(void)
{
	m_pRenderer->Draw();	// �`��
}

//=============================================================================
// ���[�h�̏I���E����������
//=============================================================================
const void CManager::SetMode(MODE mode)
{
	// �t�F�[�h�ȊO�̃I�u�W�F�N�g��j��
	CScene::ModeUninit();

	switch (m_mode)
	{
	case MODE_TITLE:
		if (m_pTitle != NULL)
		{// �^�C�g��
			m_pTitle = NULL;
		}
		break;

	case MODE_MENU:
		if (m_pMenu != NULL)
		{// ���j���[
			m_pMenu = NULL;
		}
		break;

	case MODE_MECHASELECT:
		if (m_pMechaSelect != NULL)
		{// ���j���[
			m_pMechaSelect = NULL;
		}
		break;

	case MODE_TUTORIAL:
		if (m_pTutorial != NULL)
		{// �`���[�g���A��
			//m_pSound->StopSound(CSound::SOUND_LABEL_TITLE);
			m_pTutorial = NULL;
		}
		break;

	case MODE_GAME:
		if (m_pGame != NULL)
		{// �Q�[��
			//m_pSound->StopSound(CSound::SOUND_LABEL_GAME);
			m_pGame = NULL;
		}
		break;

	case MODE_RESULT:
		if (m_pResult != NULL)
		{// �N���A
			m_pResult = NULL;
		}
		break;
	}

	m_mode = mode;

	//�V�������(���[�h)�̏���������
	switch (mode)
	{
	case MODE_TITLE:
		if (m_pTitle == NULL)
		{// �^�C�g��
			//m_pSound->PlaySound(CSound::SOUND_LABEL_TITLE);
			m_pCamera->Init();
			m_pTitle = new CTitle;
			m_pTitle->Init();
		}
		break;

	case MODE_MENU:
		if (m_pMenu == NULL)
		{// ���j���[
			m_pMenu = new CMenu;
			m_pMenu->Init();
		}
		break;

	case MODE_MECHASELECT:
		if (m_pMechaSelect == NULL)
		{// ���j���[
			m_pMechaSelect = new CMechaSelect;
			m_pMechaSelect->Init();
		}
		break;

	case MODE_TUTORIAL:
		if (m_pTutorial == NULL)
		{// �^�C�g��
			m_pTutorial = new CTutorial;
			m_pTutorial->Init();
		}
		break;

	case MODE_GAME:
		if (m_pGame == NULL)
		{// �Q�[��
			//m_pSound->PlaySound(CSound::SOUND_LABEL_GAME);
			m_pCamera->Init();
			m_pGame = new CGame;
			m_pGame->Init();
		}
		break;

	case MODE_RESULT:
		if (m_pResult == NULL)
		{// �N���A
			//m_pSound->PlaySound(CSound::SOUND_LABEL_CLEAR);
			m_pResult = new CResult;
			m_pResult->Init();
		}
		break;
	}
}