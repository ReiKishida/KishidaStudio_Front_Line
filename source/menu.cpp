//=============================================================================
//
// ���j���[���� [menu.cpp]
// Author : Takuto Ishida
//
//=============================================================================
#include "menu.h"
#include "fade.h"
#include "input.h"
#include "debugProc.h"
#include "sound.h"
#include "texture.h"
#include "bg.h"
#include "mouseCursor.h"
#include "button.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MENU_LOGO_WIDTH		(450.0f)	// ���j���[���S�̕�
#define MENU_LOGO_HEIGHT	(120.0f)	// ���j���[���S�̍���
#define MENU_LOGO_POS		(D3DXVECTOR3(MENU_LOGO_WIDTH / 2.0f + 20.0f, MENU_LOGO_HEIGHT / 2.0f + 20.0f, 0.0f))			// ���j���[���S�̈ʒu

#define MENU_SELECT_WIDTH	(550.0f)	// �I�����ڕ\���̕�
#define MENU_SELECT_HEIGHT	(130.0f)	// �I�����ڕ\���̍���
#define MENU_SELECT_POS		(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f + (MENU_SELECT_HEIGHT / 2.0f + 20.0f), 0.0f))		// �I�����ڕ\���̈ʒu

#define MENU_MESSAGE_WIDTH	(1200.0f)	// ���b�Z�[�W�\���̕�
#define MENU_MESSAGE_HEIGHT	(190.0f)	// ���b�Z�[�W�\���̍���
#define MENU_MESSAGE_POS	(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT - (MENU_MESSAGE_HEIGHT / 2.0f + 10.0f), 0.0f))	// ���b�Z�[�W�\���̈ʒu

#define MENU_BUTTON_WIDTH	(380.0f)	// �{�^���̕�
#define MENU_BUTTON_HEIGHT	(170.0f)	// �{�^���̍���

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
CMenu::MODE CMenu::m_mode = CMenu::MODE_SINGLE;

//=========================================
// �R���X�g���N�^
//=========================================
CMenu::CMenu(int nPriority, CScene::OBJTYPE objType) : CScene(nPriority, objType)
{
	m_pBg = NULL;
	m_pCursor = NULL;

	for (int nCntButton = 0; nCntButton < MENU_NUM_BUTTON; nCntButton++)
	{
		m_apButtonUI[nCntButton] = NULL;
	}

	for (int nCntUI = 0; nCntUI < MENU_NUM_UI; nCntUI++)
	{
		m_apUI[nCntUI] = NULL;
	}
}

//=========================================
// �f�X�g���N�^
//=========================================
CMenu::~CMenu()
{
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CMenu::Init(void)
{
	// �w�i�̐���
	m_pBg = CBg::Create();

	// ���j���[���S
	m_apUI[0] = CScene2D::Create();
	m_apUI[0]->SetPos(MENU_LOGO_POS);
	m_apUI[0]->SetSize(MENU_LOGO_WIDTH, MENU_LOGO_HEIGHT);
	m_apUI[0]->BindTexture(CTexture::GetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_LOGO)));
	m_apUI[0]->SetTex(0, 1, 2);

	// �I�����ڕ\��
	m_apUI[1] = CScene2D::Create();
	m_apUI[1]->SetPos(MENU_SELECT_POS);
	m_apUI[1]->SetSize(MENU_SELECT_WIDTH, MENU_SELECT_HEIGHT);
	m_apUI[1]->BindTexture(CTexture::GetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_MENU_DISP)));

	// ���b�Z�[�W�\��
	m_apUI[2] = CScene2D::Create();
	m_apUI[2]->SetPos(MENU_MESSAGE_POS);
	m_apUI[2]->SetSize(MENU_MESSAGE_WIDTH, MENU_MESSAGE_HEIGHT);
	m_apUI[2]->BindTexture(CTexture::GetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_MENU_INST)));

	// �{�^���̐���
	for (int nCntButton = 0; nCntButton < MENU_NUM_BUTTON; nCntButton++)
	{
		float fPosX = SCREEN_WIDTH / 2.0f + (nCntButton * (MENU_BUTTON_WIDTH + 50.0f)) - ((MENU_BUTTON_WIDTH + 50.0f) * (MENU_NUM_BUTTON / 2.0f)) + ((MENU_BUTTON_WIDTH + 50.0f) / 2.0f);
		m_apButtonUI[nCntButton] = CButton2D::Create(D3DXVECTOR3(fPosX, SCREEN_HEIGHT / 2.0f - (MENU_BUTTON_HEIGHT / 2.0f), 0.0f), MENU_BUTTON_WIDTH, MENU_BUTTON_HEIGHT);
		m_apButtonUI[nCntButton]->BindTexture(CTexture::GetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_MENU_FLAME)));
		m_apButtonUI[nCntButton]->SetTex(nCntButton, 1, 3);
	}

	// �J�[�\���̐���
	m_pCursor = CMouseCursor2D::Create();

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CMenu::Uninit(void)
{
	for (int nCntButton = 0; nCntButton < MENU_NUM_BUTTON; nCntButton++)
	{
		if (NULL != m_apButtonUI[nCntButton])
		{// �{�^��UI�̔j��
			m_apButtonUI[nCntButton]->Uninit();
			m_apButtonUI[nCntButton] = NULL;
		}
	}

	for (int nCntUI = 0; nCntUI < MENU_NUM_UI; nCntUI++)
	{
		if (NULL != m_apUI[nCntUI])
		{// UI�̔j��
			m_apUI[nCntUI]->Uninit();
			m_apUI[nCntUI] = NULL;
		}
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
void CMenu::Update(void)
{
	// ���͂̏����擾
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();
	CXInput *pXInput = CManager::GetXInput();

	// �T�E���h�̎擾
	CSound *pSound = CManager::GetSound();

	bool bButtonSwitch = false;
	int nSelect = -1;
	//MODE mode = MODE_SINGLE;

	for (int nCntButton = 0; nCntButton < MENU_NUM_BUTTON; nCntButton++)
	{// �{�^���̔���
		if (m_apButtonUI[nCntButton]->InRange(m_pCursor->GetMousePosition()))
		{// �͈͓����`�F�b�N
			if (m_apButtonUI[nCntButton]->ClickRelease())
			{// �N���b�N���ꂽ
				bButtonSwitch = true;
				//mode = (MODE)nCntButton;
				m_mode = (MODE)nCntButton;
				break;
			}

			nSelect = nCntButton;
		}
	}

	//**********************************************
	// ���[�h�������S�\��
	//**********************************************
	// �I������Ă��Ȃ��Ƃ��I�����ڂ��\���ɂ���
	if (-1 == nSelect) { m_apUI[1]->SetDisp(false); }
	else{ m_apUI[1]->SetDisp(true); }

	switch (nSelect)
	{
	case 0:		// �V���O���v���C
		m_apUI[1]->SetTex(0, 1, 3);
		break;

	case 1:		// �}���`�v���C
		m_apUI[1]->SetTex(1, 1, 3);
		break;

	case 2:		// �^�C�g���߂�
		m_apUI[1]->SetTex(2, 1, 3);
		break;
	}

	//**********************************************
	// ���[�h�T�v�\��
	//**********************************************
	// �I������Ă��Ȃ��Ƃ��I�����ڂ��\���ɂ���
	if (-1 == nSelect) { m_apUI[2]->SetDisp(false); }
	else { m_apUI[2]->SetDisp(true); }

	switch (nSelect)
	{
	case 0:		// �V���O���v���C
		m_apUI[2]->SetTex(0, 1, 3);
		break;

	case 1:		// �}���`�v���C
		m_apUI[2]->SetTex(1, 1, 3);
		break;

	case 2:		// �^�C�g���߂�
		m_apUI[2]->SetTex(2, 1, 3);
		break;
	}

	if (bButtonSwitch)
	{// ��ʑJ��
		if (CFade::GetFade() == CFade::FADE_NONE)
		{// �t�F�[�h���Ȃ��Ƃ�
			switch(m_mode)
			{
			case MODE_SINGLE:
				CFade::Create(CManager::MODE_MECHASELECT);
				//pSound->PlaySound(CSound::SOUND_LABEL_DECIDE);
				break;

			case MODE_MULTI:
				CFade::Create(CManager::MODE_MECHASELECT);
				break;

			case MODE_QUIT:
				CFade::Create(CManager::MODE_TITLE);
				break;
			}
		}
	}

	CDebugProc::Print("���j���[");
}

//=============================================================================
// �`�揈��
//=============================================================================
void CMenu::Draw(void)
{
}