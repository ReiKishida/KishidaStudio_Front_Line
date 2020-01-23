//=============================================================================
//
// �`���[�g���A�����j���[���� [tutorialmenu.cpp]
// Author : Takuto Ishida & Ayaka Hakozaki &Rei Kishida
//
//=============================================================================
#include "tutorialmenu.h"
#include "fade.h"
#include "input.h"
#include "debugProc.h"
#include "sound.h"
#include "texture.h"
#include "bg.h"
#include "mouseCursor.h"
#include "button.h"
#include "UI_Texture.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MENU_LOGO_WIDTH		(420.0f)	// ���j���[���S�̕�1
#define MENU_LOGO_HEIGHT	(90.0f)	// ���j���[���S�̍���
#define MENU_LOGO_POS		(D3DXVECTOR3(MENU_LOGO_WIDTH / 2.0f + 10.0f, MENU_LOGO_HEIGHT / 2.0f + 10.0f, 0.0f))			// ���j���[���S�̈ʒu

#define MENU_MESSAGE_WIDTH		(1280.0f)	// ���b�Z�[�W�\���̕�
#define MENU_MESSAGE_HEIGHT	(70.0f)	// ���b�Z�[�W�\���̍���

#define MENU_BUTTON_WIDTH	(250.0f)	// �{�^���̕�
#define MENU_BUTTON_HEIGHT	(220.0f)	// �{�^���̍���

#define BACK_BUTTON_WIDTH	(150.0f)	// �{�^���̕�
#define BACK_BUTTON_HEIGHT	(150.0f)	// �{�^���̍���


#define MESS_SCROLL_SPEED		(0.003f)	// �X�N���[���X�s�[�h

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
CTutorialMenu::TYPE CTutorialMenu::m_Type = CTutorialMenu::TYPE_NONE;

//=========================================
// �R���X�g���N�^
//=========================================
CTutorialMenu::CTutorialMenu(int nPriority, CScene::OBJTYPE objType) : CScene(nPriority, objType)
{
	m_pBg = NULL;
	m_pCursor = NULL;
	m_nCntBgMove = 0;
	m_apUITexMess = NULL;

	for (int nCntButton = 0; nCntButton < TUTORIALMENU_NUM_BUTTON; nCntButton++)
	{
		m_apButtonUI[nCntButton] = NULL;
	}

	for (int nCntUI = 0; nCntUI < TUTORIALMENU_NUM_UI; nCntUI++)
	{
		m_apUI[nCntUI] = NULL;
	}

	m_pBackButton = NULL;
}

//=========================================
// �f�X�g���N�^
//=========================================
CTutorialMenu::~CTutorialMenu()
{
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CTutorialMenu::Init(void)
{
	// �w�i�̐���
	m_pBg = CBg::Create();

	// ���j���[���S
	if (m_apUI[0] == NULL && m_apUI[1] == NULL)
	{
		m_apUI[0] = CScene2D::Create();
		m_apUI[0]->SetPos(MENU_LOGO_POS);
		m_apUI[0]->SetSize(MENU_LOGO_WIDTH, MENU_LOGO_HEIGHT);
		m_apUI[0]->BindTexture(CTexture::GetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_LOGO)));
		m_apUI[0]->SetTex(2, 1, TUTO_TEX_MENU_LOGO);

		// �C���t�H���[�V����
		m_apUI[1] = CScene2D::Create();
		m_apUI[1]->SetPos(D3DXVECTOR3(SCREEN_WIDTH / 2, 650.0f, 0.0f));
		m_apUI[1]->SetSize(SCREEN_WIDTH, 140.0f);
		m_apUI[1]->BindTexture(CTexture::GetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_MENU_INFOMETION)));
	}

	// ���j���[���b�Z�[�W
	if (m_apUITexMess == NULL)
	{
		m_apUITexMess = CUI_TEXTURE::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, 685.0f, 0.0f), MENU_MESSAGE_WIDTH, MENU_MESSAGE_HEIGHT, CUI_TEXTURE::UIFLAME_TUTORIALMENU_MESS);
	}

	// �{�^���̐���
	if (m_apButtonUI[0] == NULL && m_apButtonUI[1] == NULL && m_apButtonUI[2] == NULL )
	{
		m_apButtonUI[0] = CButton2D::Create(D3DXVECTOR3(SCREEN_WIDTH /2 + 300.0f, SCREEN_HEIGHT /2 - 140.0f, 0.0f), MENU_BUTTON_WIDTH, MENU_BUTTON_HEIGHT);				// �x�[�V�b�N���[�u�����g
		m_apButtonUI[0]->BindTexture(CTexture::GetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_TUTORIALMENU_FRAME)));
		m_apButtonUI[0]->SetTex(0, 1, TUTO_TEX_MENU_FLAME);

		m_apButtonUI[1] = CButton2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 + 500.0f, SCREEN_HEIGHT / 2, 0.0f), MENU_BUTTON_WIDTH, MENU_BUTTON_HEIGHT);				// �A�N�V�����p�[�g
		m_apButtonUI[1]->BindTexture(CTexture::GetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_TUTORIALMENU_FRAME)));
		m_apButtonUI[1]->SetTex(1, 1, TUTO_TEX_MENU_FLAME);

		m_apButtonUI[2] = CButton2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 + 300.0f, SCREEN_HEIGHT / 2 + 140, 0.0f), MENU_BUTTON_WIDTH, MENU_BUTTON_HEIGHT);				// �X�g���e�W�[
		m_apButtonUI[2]->BindTexture(CTexture::GetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_TUTORIALMENU_FRAME)));
		m_apButtonUI[2]->SetTex(2, 1, TUTO_TEX_MENU_FLAME);

	}
	if (m_pBackButton == NULL)
	{
		m_pBackButton = CButton2D::Create(D3DXVECTOR3(1200.0f, 80.0f, 0.0f), BACK_BUTTON_WIDTH, BACK_BUTTON_HEIGHT);				// �߂�{�^��
		m_pBackButton->BindTexture(CTexture::GetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_BACK)));											// �e�N�X�`���f��
	}

	// �J�[�\���̐���
	if (m_pCursor == NULL)
	{
		m_pCursor = CMouseCursor2D::Create();
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CTutorialMenu::Uninit(void)
{
	for (int nCntButton = 0; nCntButton < TUTORIALMENU_NUM_BUTTON; nCntButton++)
	{
		if (NULL != m_apButtonUI[nCntButton])
		{// �{�^��UI�̔j��
			m_apButtonUI[nCntButton]->Uninit();
			m_apButtonUI[nCntButton] = NULL;
		}
	}

	for (int nCntUI = 0; nCntUI < TUTORIALMENU_NUM_UI; nCntUI++)
	{
		if (NULL != m_apUI[nCntUI])
		{// UI�̔j��
			m_apUI[nCntUI]->Uninit();
			m_apUI[nCntUI] = NULL;
		}
	}

	if (m_apUITexMess != NULL)
	{	// ���b�Z�[�W�̔j��
		m_apUITexMess->Uninit();
		m_apUITexMess = NULL;
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


	if (m_pBackButton != NULL)
	{// ���b�Z�[�W�̔j��
		m_pBackButton->Uninit();
		m_pBackButton = NULL;
	}


	// �����̔j��
	CScene::Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CTutorialMenu::Update(void)
{
	// ���͂̏����擾
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();
	CXInput *pXInput = CManager::GetXInput();

	// �T�E���h�̎擾
	CSound *pSound = CManager::GetSound();

	bool bButtonSwitch = false;
	int nSelect = -1;

	if (m_pBackButton->InRange(m_pCursor->GetMousePosition()))
	{ // �͈͓����`�F�b�N
		//���b�Z�[�W�e�N�X�`���̕ϓ�
		//m_apUITexMess->SetTexMove(3, 1, TUTO_TEX_MENU_MESS, m_nCntBgMove, 0, MESS_SCROLL_SPEED);
		nSelect = 3;
		if (m_pBackButton->ClickRelease())
		{ // �N���b�N���ꂽ
			pSound->PlaySound(CSound::SOUND_LABEL_DECIDE);
			if (CFade::GetFade() == CFade::FADE_NONE)
			{// �t�F�[�h���Ȃ��Ƃ�
				CFade::Create(CManager::MODE_MENU);
			}
		}
	}

	for (int nCntButton = 0; nCntButton < TUTORIALMENU_NUM_BUTTON; nCntButton++)
	{// �{�^���̔���
		if (m_apButtonUI[nCntButton]->InRange(m_pCursor->GetMousePosition()))
		{// �͈͓����`�F�b�N
			if (m_apButtonUI[nCntButton]->ClickRelease())
			{// �N���b�N���ꂽ
				pSound->PlaySound(CSound::SOUND_LABEL_DECIDE);
				bButtonSwitch = true;
				m_Type = (TYPE)nCntButton;
				break;
			}

			nSelect = nCntButton;
		}
	}

	// �X�N���[���J�E���^�[
	m_nCntBgMove++;

	//**********************************************
	// ���[�h�������S�\��
	//**********************************************
	// �I������Ă��Ȃ��Ƃ��I�����ڂ��\���ɂ���
	if (-1 == nSelect) { m_apUITexMess->SetDisp(false); }
	else { m_apUITexMess->SetDisp(true); }

	switch (nSelect)
	{
	case 0:		// ��{����
		m_apUITexMess->SetTexMove(0, 1, TUTO_TEX_MENU_MESS, m_nCntBgMove, 0, MESS_SCROLL_SPEED);
		break;

	case 1:		//�@�A�N�V�����p�[�g
		m_apUITexMess->SetTexMove(1, 1, TUTO_TEX_MENU_MESS, m_nCntBgMove, 0, MESS_SCROLL_SPEED);
		break;

	case 2:		// �X�g���e�W�[
		m_apUITexMess->SetTexMove(2, 1, TUTO_TEX_MENU_MESS, m_nCntBgMove, 0, MESS_SCROLL_SPEED);
		break;
	case 3:		// �߂�
		m_apUITexMess->SetTexMove(3, 1, TUTO_TEX_MENU_MESS, m_nCntBgMove, 0, MESS_SCROLL_SPEED);
		break;
	}

	//**********************************************
	// �{�^�������ꂽ
	//**********************************************
	if (bButtonSwitch)
	{// ��ʑJ��
		if (CFade::GetFade() == CFade::FADE_NONE)
		{// �t�F�[�h���Ȃ��Ƃ�
			CManager::MODE mode;

			mode = CManager::MODE_TUTORIAL;

			switch (m_Type)
			{
			case TYPE_MOVEMENT:
				CFade::Create(mode);
				break;

			case TYPE_ACTION:
				CFade::Create(mode);
				break;

			case TYPE_STRATEGY:
				CFade::Create(mode);
				break;
			}
		}
	}

	CDebugProc::Print("�`���[�g���A�����j���[");
}

//=============================================================================
// �`�揈��
//=============================================================================
void CTutorialMenu::Draw(void)
{
}