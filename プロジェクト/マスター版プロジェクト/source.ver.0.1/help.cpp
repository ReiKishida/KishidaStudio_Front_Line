//=============================================================================
//
// �w���v���� [help.cpp]
// Author : Ayaka Hakozaki
//
//=============================================================================
#include "help.h"
#include "fade.h"
#include "input.h"
#include "debugProc.h"
#include "sound.h"
#include "texture.h"
#include "bg.h"
#include "mouseCursor.h"
#include "button.h"
#include "server.h"
#include "menu.h"
#include "UI_Texture.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define HELP_LOGO_WIDTH			(420.0f)	// �w���v���S�̕�
#define HELP_LOGO_HEIGHT		(90.0f)		// �w���v���S�̍���
#define HELP_LOGO_POS		(D3DXVECTOR3(100.0f, HELP_LOGO_HEIGHT / 2.0f + 10.0f, 0.0f))			// �w���v���S�̈ʒu

#define HELP_BUTTON_WIDTH	(270.0f)	// �p�[�g�I���{�^���̕�
#define HELP_BUTTON_HEIGHT	(230.0f)	// �p�[�g�I���{�^���̍���

#define HELP_INST_WIDTH			(300.0f)	// ��������̕�
#define HELP_INST_HEIGHT			(380.0f)	// ��������̍���

#define HELP_OPERATION_SIZE	(520.0f)	// ����摜�̃T�C�Y
#define HELP_BACK_SIZE				(150.0f)	// back�{�^���̑傫��

#define HELP_LENGTH_WIDTH		(160.0f)	// �C���^�[�t�F�C�X���S��
#define HELP_LENGTH_HEIGHT	(70.0f)		// �C���^�[�t�F�C�X���S����

#define TEX_HELP_FLAME			(4)			// �C���^�[�t�F�C�X�摜

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************

//=========================================
// �R���X�g���N�^
//=========================================
CHelp::CHelp(int nPriority, CScene::OBJTYPE objType) : CScene(nPriority, objType)
{
	m_pBg = NULL;
	m_pCursor = NULL;
	m_pButtonUIback = NULL;

	for (int nCntButton = 0; nCntButton < HELP_NUM_BUTTON; nCntButton++)
	{
		m_apButtonUI[nCntButton] = NULL;
	}

	for (int nCntUI = 0; nCntUI < HELP_NUM_UI; nCntUI++)
	{
		m_apUI[nCntUI] = NULL;
	}

	for (int nCntInst = 0; nCntInst < HELP_INST; nCntInst++)
	{
		m_apUIInst[nCntInst] = NULL;
	}
}

//=========================================
// �f�X�g���N�^
//=========================================
CHelp::~CHelp()
{
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CHelp::Init(void)
{
	// ����
	HelpCreate();

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CHelp::Uninit(void)
{
	for (int nCntButton = 0; nCntButton < HELP_NUM_BUTTON; nCntButton++)
	{
		if (NULL != m_apButtonUI[nCntButton])
		{// �{�^��UI�̔j��
			m_apButtonUI[nCntButton]->Uninit();
			m_apButtonUI[nCntButton] = NULL;
		}
	}

	for (int nCntUI = 0; nCntUI < HELP_NUM_UI; nCntUI++)
	{
		if (NULL != m_apUI[nCntUI])
		{// UI�̔j��
			m_apUI[nCntUI]->Uninit();
			m_apUI[nCntUI] = NULL;
		}
	}

	for (int nCntInst = 0; nCntInst < HELP_INST; nCntInst++)
	{
		if (m_apUIInst[nCntInst] != NULL)
		{
			m_apUIInst[nCntInst]->Uninit();
			m_apUIInst[nCntInst] = NULL;
		}
	}

	if (m_pButtonUIback != NULL)
	{	// back�{�^���̔j��
		m_pButtonUIback->Uninit();
		m_pButtonUIback = NULL;
	}

	if (NULL != m_pBg)
	{// �w�i�̔j��
		m_pBg->Uninit();
		m_pBg = NULL;
	}

	if (NULL != m_pCursor)
	{// �J�[�\���̔j��
		m_pCursor->Uninit();
		m_pCursor = NULL;
	}

	// �����̔j��
	CScene::Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CHelp::Update(void)
{
	// ���͂̏����擾
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();
	CXInput *pXInput = CManager::GetXInput();

	// �T�E���h�̎擾
	CSound *pSound = CManager::GetSound();

	int nSelKeyboard = -1;
	int nSelGamepad = -1;

	// �{�^���̔���i�p�[�g���Ƃ̑���摜�j
	for (int nCntButton = 0; nCntButton < HELP_NUM_BUTTON; nCntButton++)
	{
		if (m_apButtonUI[nCntButton]->InRangeMenu(m_pCursor->GetMousePosition()))
		{// �͈͓����`�F�b�N
			if (nCntButton == 0 || nCntButton == 1)
			{	// �L�[�{�[�h
				nSelKeyboard = nCntButton;
			}
			else
			{	// �Q�[���p�b�h
				nSelGamepad = nCntButton;
			}
		}
	}

	// �{�^���̔���iback�{�^���j
	bool bBackButton = false;

	if (m_pButtonUIback->InRangeMenu(m_pCursor->GetMousePosition()))
	{	// �͈͓����`�F�b�N
		if (m_pButtonUIback->ClickRelease())
		{	// �N���b�N���ꂽ
			bBackButton = true;
		}
	}

	if (bBackButton == true)
	{	// �N���b�N���ꂽ��A���j���[��ʂ֖߂�
		pSound->PlaySound(CSound::SOUND_LABEL_DECIDE);

		CFade::Create(CManager::MODE_MENU);
	}

	//**********************************************
	// �I�𒆂̑���摜
	//**********************************************
	if (nSelKeyboard == -1) { m_apUIInst[0]->SetDisp(false); }
	else { m_apUIInst[0]->SetDisp(true); }

	if (nSelGamepad == -1) { m_apUIInst[1]->SetDisp(false); }
	else { m_apUIInst[1]->SetDisp(true); }

	switch (nSelKeyboard)
	{
	case 0:
		m_apUIInst[0]->SetTex(0, 1, 2);
		break;

	case 1:
		m_apUIInst[0]->SetTex(1, 1, 2);
		break;
	}

	switch (nSelGamepad)
	{
	case 2:
		m_apUIInst[1]->SetTex(0, 1, 2);
		break;

	case 3:
		m_apUIInst[1]->SetTex(1, 1, 2);
		break;
	}

	CDebugProc::Print("�w���v");
}

//=============================================================================
// �`�揈��
//=============================================================================
void CHelp::Draw(void)
{
}

//=============================================================================
// ����
//=============================================================================
void CHelp::HelpCreate(void)
{
	// �w�i�̐���
	if (m_pBg == NULL)
	{
		m_pBg = CBg::Create();
	}

	// UI�̐���
	if (m_apUI[0] == NULL && m_apUI[1] == NULL && m_apUI[2] == NULL)
	{
		// �w���v���S
		m_apUI[0] = CScene2D::Create();
		m_apUI[0]->SetPos(HELP_LOGO_POS);
		m_apUI[0]->SetSize(HELP_LOGO_WIDTH, HELP_LOGO_HEIGHT);
		m_apUI[0]->BindTexture(CTexture::GetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_LOGO)));
		m_apUI[0]->SetTex(3, 1, TEX_MENU_LOGO);

		// gamepad
		m_apUI[1] = CScene2D::Create();
		m_apUI[1]->SetPos(D3DXVECTOR3(720.0f, 640.0f, 0.0f));
		m_apUI[1]->SetSize(HELP_LENGTH_WIDTH, HELP_LENGTH_HEIGHT);
		m_apUI[1]->BindTexture(CTexture::GetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_INTERFACE)));
		m_apUI[1]->SetTex(1, 1, 2);

		// keyboard
		m_apUI[2] = CScene2D::Create();
		m_apUI[2]->SetPos(D3DXVECTOR3(1140.0f, 330.0f, 0.0f));
		m_apUI[2]->SetSize(HELP_LENGTH_WIDTH, HELP_LENGTH_HEIGHT);
		m_apUI[2]->BindTexture(CTexture::GetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_INTERFACE)));
		m_apUI[2]->SetTex(0, 1, 2);
	}

	// �I�𒆂̑���摜
	if (m_apUIInst[0] == NULL && m_apUIInst[1] == NULL)
	{
		m_apUIInst[0] = CScene2D::Create();			// keyborad
		m_apUIInst[0]->SetPos(D3DXVECTOR3(HELP_INST_WIDTH, HELP_INST_HEIGHT, 0.0f));
		m_apUIInst[0]->SetSize(HELP_OPERATION_SIZE, HELP_OPERATION_SIZE);
		m_apUIInst[0]->BindTexture(CTexture::GetTexture(CTexture::TEXTURE_KEYBOARD_INST));

		m_apUIInst[1] = CScene2D::Create();			// gamepad
		m_apUIInst[1]->SetPos(D3DXVECTOR3(HELP_INST_WIDTH, HELP_INST_HEIGHT, 0.0f));
		m_apUIInst[1]->SetSize(HELP_OPERATION_SIZE, HELP_OPERATION_SIZE);
		m_apUIInst[1]->BindTexture(CTexture::GetTexture(CTexture::TEXTURE_GAMEPAD_INST));
	}

	// �{�^���̐���
	if (m_apButtonUI[0] == NULL && m_apButtonUI[1] == NULL&& m_apButtonUI[2] == NULL&& m_apButtonUI[3] == NULL)
	{
		m_apButtonUI[0] = CButton2D::Create(D3DXVECTOR3(935.0f, 375.0f, 0.0f), HELP_BUTTON_WIDTH, HELP_BUTTON_HEIGHT);		// keyboard : action
		m_apButtonUI[0]->BindTexture(CTexture::GetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_HELP_FLAME)));
		m_apButtonUI[0]->SetTex(0, 1, TEX_HELP_FLAME);

		m_apButtonUI[1] = CButton2D::Create(D3DXVECTOR3(1140.0f, 485.0f, 0.0f), HELP_BUTTON_WIDTH, HELP_BUTTON_HEIGHT);		// keyboard : strategy
		m_apButtonUI[1]->BindTexture(CTexture::GetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_HELP_FLAME)));
		m_apButtonUI[1]->SetTex(1, 1, TEX_HELP_FLAME);

		m_apButtonUI[2] = CButton2D::Create(D3DXVECTOR3(730.0f, 485.0f, 0.0f), HELP_BUTTON_WIDTH, HELP_BUTTON_HEIGHT);		// gamepad : action
		m_apButtonUI[2]->BindTexture(CTexture::GetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_HELP_FLAME)));
		m_apButtonUI[2]->SetTex(2, 1, TEX_HELP_FLAME);

		m_apButtonUI[3] = CButton2D::Create(D3DXVECTOR3(935.0f, 590.0f, 0.0f), HELP_BUTTON_WIDTH, HELP_BUTTON_HEIGHT);		// gamepad : strategy
		m_apButtonUI[3]->BindTexture(CTexture::GetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_HELP_FLAME)));
		m_apButtonUI[3]->SetTex(3, 1, TEX_HELP_FLAME);
	}

	// back�{�^���̐���
	if (m_pButtonUIback == NULL)
	{
		m_pButtonUIback = CButton2D::Create(D3DXVECTOR3(1200.0f, 80.0f, 0.0f), HELP_BACK_SIZE, HELP_BACK_SIZE);		// back�{�^��
		m_pButtonUIback->BindTexture(CTexture::GetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_BACK)));
	}

	// �J�[�\���̐���
	if (m_pCursor == NULL)
	{
		m_pCursor = CMouseCursor2D::Create();
	}
}