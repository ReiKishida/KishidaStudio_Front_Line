//=============================================================================
//
// ���J�I������ [mechaSelect.cpp]
// Author : Ayaka Hakozaki
//
//=============================================================================
#include "mechaSelect.h"
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

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MENU_LOGO_WIDTH		(450.0f)	// ���j���[���S�̕�
#define MENU_LOGO_HEIGHT	(120.0f)	// ���j���[���S�̍���
#define MENU_LOGO_POS		(D3DXVECTOR3(MENU_LOGO_WIDTH / 2.0f + 20.0f, MENU_LOGO_HEIGHT / 2.0f + 20.0f, 0.0f))			// ���j���[���S�̈ʒu

#define MENU_SELECT_WIDTH	(630.0f)	// �I�����ڕ\���̕�
#define MENU_SELECT_HEIGHT	(130.0f)	// �I�����ڕ\���̍���
#define MENU_SELECT_POS		(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f + (MENU_SELECT_HEIGHT / 2.0f + 20.0f), 0.0f))		// �I�����ڕ\���̈ʒu

#define MENU_BUTTON_WIDTH	(350.0f)	// �{�^���̕�
#define MENU_BUTTON_HEIGHT	(130.0f)	// �{�^���̍���

#define BOTTON_INTERVAL			(40.0f)		// �{�^���ƃ{�^���̊Ԋu

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
CMechaSelect::MECHATYPE CMechaSelect::m_mechaType = CMechaSelect::MECHATYPE_SHOOTER;

//=========================================
// �R���X�g���N�^
//=========================================
CMechaSelect::CMechaSelect(int nPriority, CScene::OBJTYPE objType) : CScene(nPriority, objType)
{
	m_pBg = NULL;
	m_pCursor = NULL;

	for (int nCntButton = 0; nCntButton < MECHASEL_NUM_BUTTON; nCntButton++)
	{
		m_apButtonUI[nCntButton] = NULL;
	}

	for (int nCntUI = 0; nCntUI < MECHASEL_NUM_UI; nCntUI++)
	{
		m_apUI[nCntUI] = NULL;
	}
}

//=========================================
// �f�X�g���N�^
//=========================================
CMechaSelect::~CMechaSelect()
{
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CMechaSelect::Init(void)
{
	// �w�i�̐���
	m_pBg = CBg::Create();

	// ���j���[���S
	m_apUI[0] = CScene2D::Create();
	m_apUI[0]->SetPos(MENU_LOGO_POS);
	m_apUI[0]->SetSize(MENU_LOGO_WIDTH, MENU_LOGO_HEIGHT);
	m_apUI[0]->BindTexture(CTexture::GetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_LOGO)));

	// �I�𒆂̋@�̐���
	m_apUI[1] = CScene2D::Create();
	m_apUI[1]->SetPos(D3DXVECTOR3(340.0f, 630.0f, 0.0f));
	m_apUI[1]->SetSize(MENU_SELECT_WIDTH, MENU_SELECT_HEIGHT);
	m_apUI[1]->BindTexture(CTexture::GetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_MECHASEL_INST)));

	// �I�𒆂̋@�̕\��
	m_apUI[2] = CScene2D::Create();
	m_apUI[2]->SetPos(D3DXVECTOR3(300.0f, 350.0f, 0.0f));
	m_apUI[2]->SetSize(400.0f, 400.0f);
	m_apUI[2]->BindTexture(CTexture::GetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_AI_MECHA_FLAME)));

	// �K�������\��
	m_apUI[3] = CScene2D::Create();
	m_apUI[3]->SetPos(D3DXVECTOR3(800.0f, SCREEN_HEIGHT / 2, 0.0f));
	m_apUI[3]->SetSize(80.0f, 660.0f);
	m_apUI[3]->BindTexture(CTexture::GetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_ADAPT_RANGE)));

	// �{�^���i�@�̂̃^�C�v�j�̐���
	for (int nCntButton = 0; nCntButton < MECHASEL_NUM_BUTTON; nCntButton++)
	{
		if (m_apButtonUI[nCntButton] == NULL)
		{
			m_apButtonUI[nCntButton] = CButton2D::Create(D3DXVECTOR3(1070.0f, 100.0f + (nCntButton * (MENU_BUTTON_HEIGHT + BOTTON_INTERVAL)), 0.0f), MENU_BUTTON_WIDTH, MENU_BUTTON_HEIGHT);
			m_apButtonUI[nCntButton]->BindTexture(CTexture::GetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_MECHATYPE)));
			m_apButtonUI[nCntButton]->SetTex(nCntButton, 1, 4);
		}
	}

	// �J�[�\���̐���
	m_pCursor = CMouseCursor2D::Create();

	//���J�I���̏�����
	m_mechaType = MECHATYPE_LIGHT;
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CMechaSelect::Uninit(void)
{
	for (int nCntButton = 0; nCntButton < MECHASEL_NUM_BUTTON; nCntButton++)
	{
		if (NULL != m_apButtonUI[nCntButton])
		{// �{�^��UI�̔j��
			m_apButtonUI[nCntButton]->Uninit();
			m_apButtonUI[nCntButton] = NULL;
		}
	}

	for (int nCntUI = 0; nCntUI < MECHASEL_NUM_UI; nCntUI++)
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
void CMechaSelect::Update(void)
{
	// ���͂̏����擾
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();
	CXInput *pXInput = CManager::GetXInput();

	// �T�E���h�̎擾
	CSound *pSound = CManager::GetSound();

	bool bButtonSwitch = false;
	int nSelect = -1;
	//MECHATYPE mechaType = MECHATYPE_LIGHT;

	for (int nCntButton = 0; nCntButton < MECHASEL_NUM_BUTTON; nCntButton++)
	{// �{�^���̔���
		if (m_apButtonUI[nCntButton]->InRange(m_pCursor->GetMousePosition()))
		{// �͈͓����`�F�b�N
			if (m_apButtonUI[nCntButton]->ClickRelease())
			{// �N���b�N���ꂽ
				bButtonSwitch = true;
				//mechaType = (MECHATYPE)nCntButton;
				m_mechaType = (MECHATYPE)nCntButton;
				break;
			}

			nSelect = nCntButton;
		}
	}

	//**********************************************
	// �I������Ă��Ȃ��Ƃ��I�����ڂ��\���ɂ���
	//**********************************************
	// �I�𒆂̋@�̐���
	if (-1 == nSelect) { m_apUI[1]->SetDisp(false); }
	else{ m_apUI[1]->SetDisp(true); }

	switch (nSelect)
	{
	case 0:		// �^�C�vA
		m_apUI[1]->SetTex(0, 1, 4);
		break;

	case 1:		// �^�C�vB
		m_apUI[1]->SetTex(1, 1, 4);
		break;

	case 2:		// �^�C�vC
		m_apUI[1]->SetTex(2, 1, 4);
		break;

	case 3:		// �^�C�vD
		m_apUI[1]->SetTex(3, 1, 4);
		break;
	}

	// �I�𒆂̋@�̕\��
	if (-1 == nSelect) { m_apUI[2]->SetDisp(false); }
	else { m_apUI[2]->SetDisp(true); }

	switch (nSelect)
	{
	case 0:		// �^�C�vA
		m_apUI[2]->SetTex(0, 1, 2);
		break;

	case 1:		// �^�C�vB
		m_apUI[2]->SetTex(1, 1, 2);
		break;

	case 2:		// �^�C�vC
		m_apUI[2]->SetTex(0, 1, 2);
		break;

	case 3:		// �^�C�vD
		m_apUI[2]->SetTex(1, 1, 2);
		break;
	}

	if (bButtonSwitch)
	{// ��ʑJ��
		if (CFade::GetFade() == CFade::FADE_NONE)
		{// �t�F�[�h���Ȃ��Ƃ�
			CManager::MODE mode;

			if (CMenu::GetMode() == CMenu::MODE_MULTI)
			{
				mode = CManager::MODE_MATCHING;
			}
			else
			{
				mode = CManager::MODE_GAME;
			}

			switch (m_mechaType)
			{
			case MECHATYPE_LIGHT:
				CFade::Create(mode);
				break;

			case MECHATYPE_SHOOTER:
				CFade::Create(mode);
				break;

			case MECHATYPE_HEAVY:
				CFade::Create(mode);
				break;

			case MECHATYPE_ASSULT:
				CFade::Create(mode);
				break;
			}
		}
	}

	CDebugProc::Print("���J�I�����");
}

//=============================================================================
// �`�揈��
//=============================================================================
void CMechaSelect::Draw(void)
{
}