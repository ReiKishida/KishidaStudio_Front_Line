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
#define MENU_LOGO_WIDTH				(420.0f)	// �@�̑I�����S�̕�
#define MENU_LOGO_HEIGHT				(90.0f)		// �@�̑I�����S�̍���
#define MENU_LOGO_POS		(D3DXVECTOR3(MENU_LOGO_WIDTH / 2.0f + 10.0f, MENU_LOGO_HEIGHT / 2.0f + 10.0f, 0.0f))			// ���j���[���S�̈ʒu

#define MENU_SELECT_WIDTH			(670.0f)	// �I�����ڕ\���̕�
#define MENU_SELECT_HEIGHT			(180.0f)	// �I�����ڕ\���̍���

#define MECHASEL_BUTTON_WIDTH	(270.0f)	// �@�̑I���{�^���̕�
#define MECHASEL_BUTTON_HEIGHT	(230.0f)	// �@�̑I���{�^���̍���

#define MECHASEL_MECHA_SIZE		(520.0f)	// �@�̉摜�̃T�C�Y
#define MECHASEL_BACK_SIZE			(150.0f)	// back�{�^���̑傫��

#define MECHASEL_LENGTH_WIDTH	(150.0f)	// �򋗗����S��
#define MECHASEL_LENGTH_HEIGHT	(70.0f)		// �򋗗����S����

#define TEX_MECHA_TYPE					(4)			// �@�̂̎��

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
CMechaSelect::MECHATYPE CMechaSelect::m_mechaType = CMechaSelect::MECHATYPE_ASSULT;

//=========================================
// �R���X�g���N�^
//=========================================
CMechaSelect::CMechaSelect(int nPriority, CScene::OBJTYPE objType) : CScene(nPriority, objType)
{
	m_pBg = NULL;
	m_pCursor = NULL;
	m_pButtonUIback = NULL;

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
	// ����
	MechaSelectCreate();

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
void CMechaSelect::Update(void)
{
	//if (CFade::GetFade() == CFade::FADE_NONE)
	//{// �t�F�[�h���Ȃ��Ƃ�
	//	m_mechaType = (CMechaSelect::MECHATYPE)(rand() % CMechaSelect::MECHATYPE_MAX);

	//	CFade::Create(CManager::MODE_MATCHING);
	//}

	// ���͂̏����擾
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();
	CXInput *pXInput = CManager::GetXInput();

	// �T�E���h�̎擾
	CSound *pSound = CManager::GetSound();

	// �{�^���̔���i�@�̑I���j
	bool bButtonSwitch = false;
	int nSelect = -1;

	for (int nCntButton = 0; nCntButton < MECHASEL_NUM_BUTTON; nCntButton++)
	{// �{�^���̔���
		if (m_apButtonUI[nCntButton]->InRangeMenu(m_pCursor->GetMousePosition()))
		{// �͈͓����`�F�b�N
			if (m_apButtonUI[nCntButton]->ClickRelease())
			{// �N���b�N���ꂽ
				bButtonSwitch = true;
				m_mechaType = (MECHATYPE)nCntButton;
				break;
			}
			nSelect = nCntButton;
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
		CFade::Create(CManager::MODE_MENU);
	}

	//**********************************************
	// �I�𒆂̋@�̕\��
	//**********************************************
	if (-1 == nSelect) { m_apUI[1]->SetDisp(false); }
	else { m_apUI[1]->SetDisp(true); }

	// �I�΂�Ă���@�̂ɁA�@�̉摜���蓖��
	m_apUI[1]->SetTex(nSelect + 1, 1, TEX_MECHA_TYPE + 1);

	//**********************************************
	// �I�𒆂̋@�̐���
	//**********************************************
	if (-1 == nSelect) { m_apUI[2]->SetDisp(false); }
	else { m_apUI[2]->SetDisp(true); }

	// �I�΂�Ă���@�̂ɁA�����摜���蓖��
	m_apUI[2]->SetTex(nSelect, 1, TEX_MECHA_TYPE);

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

			case MECHATYPE_SNIPE:
				CFade::Create(mode);
				break;

			case MECHATYPE_HEAVY:
				CFade::Create(mode);
				break;

			case MECHATYPE_ASSULT:
				CFade::Create(mode);
				break;
			}
			pSound->PlaySound(CSound::SOUND_LABEL_DECIDE);
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

//=============================================================================
// ����
//=============================================================================
void CMechaSelect::MechaSelectCreate(void)
{
	// �w�i�̐���
	if (m_pBg == NULL)
	{
		m_pBg = CBg::Create();
	}

	if (m_apUI[0] == NULL && m_apUI[1] == NULL && m_apUI[2] == NULL && m_apUI[3] == NULL && m_apUI[4] == NULL)
	{
		// �@�̑I�����S
		m_apUI[0] = CScene2D::Create();
		m_apUI[0]->SetPos(MENU_LOGO_POS);
		m_apUI[0]->SetSize(MENU_LOGO_WIDTH, MENU_LOGO_HEIGHT);
		m_apUI[0]->BindTexture(CTexture::GetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_LOGO)));
		m_apUI[0]->SetTex(1, 1, TEX_MENU_LOGO);

		// �I�𒆂̋@�̕\��
		m_apUI[1] = CScene2D::Create();
		m_apUI[1]->SetPos(D3DXVECTOR3(300.0f, 380.0f, 0.0f));
		m_apUI[1]->SetSize(MECHASEL_MECHA_SIZE, MECHASEL_MECHA_SIZE);
		m_apUI[1]->BindTexture(CTexture::GetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_MECHA_SELECT)));

		// �I�𒆂̋@�̐���
		m_apUI[2] = CScene2D::Create();
		m_apUI[2]->SetPos(D3DXVECTOR3(340.0f, 620.0f, 0.0f));
		m_apUI[2]->SetSize(MENU_SELECT_WIDTH, MENU_SELECT_HEIGHT);
		m_apUI[2]->BindTexture(CTexture::GetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_MECHASEL_INST)));

		// long
		m_apUI[3] = CScene2D::Create();
		m_apUI[3]->SetPos(D3DXVECTOR3(740.0f, 640.0f, 0.0f));
		m_apUI[3]->SetSize(MECHASEL_LENGTH_WIDTH, MECHASEL_LENGTH_HEIGHT);
		m_apUI[3]->BindTexture(CTexture::GetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_RANGE)));
		m_apUI[3]->SetTex(0, 1, 2);

		// short
		m_apUI[4] = CScene2D::Create();
		m_apUI[4]->SetPos(D3DXVECTOR3(1140.0f, 330.0f, 0.0f));
		m_apUI[4]->SetSize(MECHASEL_LENGTH_WIDTH, MECHASEL_LENGTH_HEIGHT);
		m_apUI[4]->BindTexture(CTexture::GetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_RANGE)));
		m_apUI[4]->SetTex(1, 1, 2);
	}

	// �{�^���i�@�̂̃^�C�v�j�̐���
	if (m_apButtonUI[0] == NULL && m_apButtonUI[1] == NULL&& m_apButtonUI[2] == NULL&& m_apButtonUI[3] == NULL)
	{
		m_apButtonUI[0] = CButton2D::Create(D3DXVECTOR3(935.0f, 375.0f, 0.0f), MECHASEL_BUTTON_WIDTH, MECHASEL_BUTTON_HEIGHT);		// assult
		m_apButtonUI[0]->BindTexture(CTexture::GetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_MECHATYPE)));
		m_apButtonUI[0]->SetTex(0, 1, TEX_MECHA_TYPE);

		m_apButtonUI[1] = CButton2D::Create(D3DXVECTOR3(1140.0f, 485.0f, 0.0f), MECHASEL_BUTTON_WIDTH, MECHASEL_BUTTON_HEIGHT);		// light
		m_apButtonUI[1]->BindTexture(CTexture::GetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_MECHATYPE)));
		m_apButtonUI[1]->SetTex(1, 1, TEX_MECHA_TYPE);

		m_apButtonUI[2] = CButton2D::Create(D3DXVECTOR3(730.0f, 485.0f, 0.0f), MECHASEL_BUTTON_WIDTH, MECHASEL_BUTTON_HEIGHT);		// heavy
		m_apButtonUI[2]->BindTexture(CTexture::GetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_MECHATYPE)));
		m_apButtonUI[2]->SetTex(2, 1, TEX_MECHA_TYPE);

		m_apButtonUI[3] = CButton2D::Create(D3DXVECTOR3(935.0f, 590.0f, 0.0f), MECHASEL_BUTTON_WIDTH, MECHASEL_BUTTON_HEIGHT);		// snip
		m_apButtonUI[3]->BindTexture(CTexture::GetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_MECHATYPE)));
		m_apButtonUI[3]->SetTex(3, 1, TEX_MECHA_TYPE);
	}

	// back�{�^���̐���
	if (m_pButtonUIback == NULL)
	{
		m_pButtonUIback = CButton2D::Create(D3DXVECTOR3(1200.0f, 80.0f, 0.0f), MECHASEL_BACK_SIZE, MECHASEL_BACK_SIZE);		// back�{�^��
		m_pButtonUIback->BindTexture(CTexture::GetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_BACK)));
	}

	// �J�[�\���̐���
	if (m_pCursor == NULL)
	{
		m_pCursor = CMouseCursor2D::Create();
	}
}