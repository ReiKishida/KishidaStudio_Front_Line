//=============================================================================
//
// �^�C�g������ [title.cpp]
// Author : Takuto Ishida
//
//=============================================================================
#include "title.h"
#include "renderer.h"
#include "fade.h"
#include "input.h"
#include "manager.h"
#include "debugProc.h"
#include "logo.h"
#include "sound.h"
#include "UI_Texture.h"		// �ǉ�
#include "texture.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TITLE_TIME		(1200)		// �����L���O��ʂ܂ł̎���

#define TITLE_LOGO_POS		(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, 250.0f, 0.0f))	// �^�C�g�����S�̈ʒu
#define TITLE_LOGO_WIDTH	(700.0f)	// �^�C�g�����S�̕�
#define TITLE_LOGO_HEIGHT	(300.0f)	// �^�C�g�����S�̍���

#define TITLE_PRESSANY_POS		(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, 600.0f, 0.0f))	// ���͑҂��\���̈ʒu
#define TITLE_PRESSANY_WIDTH	(500.0f)	// ���͑҂��̕�
#define TITLE_PRESSANY_HEIGHT	(100.0f)	// ���͑҂��̍���
#define TITLE_BEFORE_FLASH		(60)		// �{�^������͂���O�̓_��
#define TITLE_AFTER_FLASH		(5)			// �{�^������͂�����̓_��

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
CUI_TEXTURE *CTitle::m_pUI[TITLE_UI] = {};

//=========================================
// �R���X�g���N�^
//=========================================
CTitle::CTitle(int nPriority, CScene::OBJTYPE objType) : CScene(nPriority, objType)
{
	m_nFlash = 0;
	m_pBg = NULL;
	m_pUI[TITLE_UI] = {};
}

//=========================================
// �f�X�g���N�^
//=========================================
CTitle::~CTitle()
{
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CTitle::Init(void)
{
	// �_�ł���Ԋu
	m_nFlash = TITLE_BEFORE_FLASH;

	//CTexture::Load();

	// Bg�X�N���[��
	CUI_TEXTURE::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), 1280.0f, 720.0f, CUI_TEXTURE::UIFLAME_BG_00);	// bg01
	CUI_TEXTURE::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), 1280.0f, 720.0f, CUI_TEXTURE::UIFLAME_BG_01);	// bg02

	// UI�\��
	m_pUI[0] = CUI_TEXTURE::Create(TITLE_LOGO_POS, TITLE_LOGO_WIDTH, TITLE_LOGO_HEIGHT, CUI_TEXTURE::UIFLAME_FLONTLINE);		// �t�����g���C��
	m_pUI[1] = CUI_TEXTURE::Create(TITLE_PRESSANY_POS, TITLE_PRESSANY_WIDTH, TITLE_PRESSANY_HEIGHT, CUI_TEXTURE::UIFLAME_PRESSBOTTON);		// �v���X�{�^��

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CTitle::Uninit(void)
{
	for (int nCntUI = 0; nCntUI < TITLE_UI; nCntUI++)
	{	// UI�̔j��
		if (m_pUI[nCntUI] != NULL)
		{
			m_pUI[nCntUI]->Uninit();
			m_pUI[nCntUI] = NULL;
		}
	}

	//if (NULL != m_pPressButton)
	//{// ���S�̔j��
	//	m_pPressButton->Uninit();
	//	m_pPressButton = NULL;
	//}

	// �����̔j��
	CScene::Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CTitle::Update(void)
{
	// ���͂̏����擾
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();
	CXInput *pXInput = CManager::GetXInput();
	CInputMouse *pMouse = CManager::GetInputMouse();

	// �T�E���h�̎擾
	CSound *pSound = CManager::GetSound();

	// �v���X�{�^�������A�_�ł���
	m_pUI[1]->Flashing(m_nFlash);

	if (pKeyboard->GetAnyKey() || pXInput->GetAnyButton(0) || pMouse->GetTrigger(CInputMouse::DIMS_BUTTON_0))
	{// ��ʑJ��
		if (CFade::GetFade() == CFade::FADE_NONE)
		{// �t�F�[�h���Ȃ��Ƃ�
			CFade::Create(CManager::MODE_MENU);
			pSound->PlaySound(CSound::SOUND_LABEL_DECIDE);
		}
	}

	CDebugProc::Print("�^�C�g��");
}

//=============================================================================
// �`�揈��
//=============================================================================
void CTitle::Draw(void)
{
}