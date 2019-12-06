//=============================================================================
//
// �`���[�g���A������ [tutorial.cpp]
// Author : Takuto Ishida
//
//=============================================================================
#include "tutorial.h"
#include "renderer.h"
#include "fade.h"
#include "input.h"
#include "manager.h"
#include "debugProc.h"
#include "logo.h"
#include "sound.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TUTORIAL_PRESSANY_POS		(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, 650.0f, 0.0f))	// ���͑҂��\���̈ʒu
#define TUTORIAL_PRESSANY_WIDTH		(500.0f)	// ���͑҂��̕�
#define TUTORIAL_PRESSANY_HEIGHT	(100.0f)	// ���͑҂��̍���
#define TUTORIAL_BEFORE_FLASH		(60)		// �{�^������͂���O�̓_��
#define TUTORIAL_AFTER_FLASH		(5)			// �{�^������͂�����̓_��

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************

//=========================================
// �R���X�g���N�^
//=========================================
CTutorial::CTutorial()
{
	m_pTexture = NULL;
}

//=========================================
// �f�X�g���N�^
//=========================================
CTutorial::~CTutorial()
{
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CTutorial::Init(void)
{
	// �I�u�W�F�N�g�^�C�v�̐ݒ�
	CScene::SetObjType(CScene::OBJTYPE_TUTORIAL);

	// �ꖇ�G�̐���
	//m_pPolygon = CScene2D::Create();
	//m_pPolygon->SetSize(SCREEN_WIDTH, SCREEN_HEIGHT);

	// ���S�̐���
	m_pPressButton = CLogoPressButton::Create(TUTORIAL_PRESSANY_POS, TUTORIAL_PRESSANY_WIDTH, TUTORIAL_PRESSANY_HEIGHT);

	// �F�̐ݒ�
	m_pPressButton->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));

	// �f�o�C�X�̎擾
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	// �_�ł���Ԋu
	m_nFlash = TUTORIAL_BEFORE_FLASH;

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CTutorial::Uninit(void)
{
	if (NULL != m_pPressButton)
	{// ���S�̔j��
		m_pPressButton->Uninit();
		m_pPressButton = NULL;
	}

	if (NULL != m_pPolygon)
	{// �ꖇ�G�̔j��
		m_pPolygon->Uninit();
		m_pPolygon = NULL;
	}

	// �����̔j��
	CScene::Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CTutorial::Update(void)
{
	// ���͂̏����擾
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();
	CXInput *pXInput = CManager::GetXInput();

	// �T�E���h�̎擾
	CSound *pSound = CManager::GetSound();

	m_pPressButton->Flashing(m_nFlash);	// ���͑҂��e�N�X�`����_��

	if (pKeyboard->GetAnyKey() || pXInput->GetAnyButton(0))
	{// ��ʑJ��
		if (CFade::GetFade() == CFade::FADE_NONE)
		{// �t�F�[�h���Ȃ��Ƃ�
			CFade::Create(CManager::MODE_GAME);
			//pSound->PlaySound(CSound::SOUND_LABEL_DECIDE);

			// �_�ł��鑬�x���グ��
			m_nFlash = TUTORIAL_AFTER_FLASH;
		}
	}

	CDebugProc::Print("�`���[�g���A��");
}

//=============================================================================
// �`�揈��
//=============================================================================
void CTutorial::Draw(void)
{
}
