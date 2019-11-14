//=============================================================================
//
// ���U���g���� [result.cpp]
// Author : Takuto Ishida
//
//=============================================================================
#include "result.h"
#include "fade.h"
#include "input.h"
#include "title.h"
#include "manager.h"
#include "debugProc.h"
#include "number.h"
#include "logo.h"
#include "sound.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define RESULT_TIME				(420)			// �^�C�g����ʂ܂ł̎���
#define RESULT_NUMBER_SIZE		(200.0f)		// �����̑傫��
#define RESULT_LOGO_WIDTH		(600.0f)		// ���S�̕�
#define RESULT_LOGO_HEIGHT		(150.0f)		// ���S�̍���

#define RESULT_PRESS_WIDTH		(500.0f)		// ���͑҂��\���̕�
#define RESULT_PRESS_HEIGHT		(100.0f)		// ���͑҂��\���̍���
#define RESULT_BEFORE_FLASH		(60)			// �{�^������͂���O�̓_��
#define RESULT_AFTER_FLASH		(5)				// �{�^������͂�����̓_��

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
int	CResult::m_nScore = 0;

//=========================================
// �R���X�g���N�^
//=========================================
CResult::CResult()
{
	m_nCntResult = 0;
	m_nDigit = 0;
	m_nFlash = 0;
	m_pLogo = NULL;
	m_pPressButton = NULL;
	m_pNumber = NULL;
	m_pScore = NULL;
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
	// �I�u�W�F�N�g�^�C�v�̐ݒ�
	CScene::SetObjType(CScene::OBJTYPE_RESULT);

	// ���[�h�̎擾
	CManager::MODE mode = CManager::GetMode();

	// ���S�𐶐�
	m_pLogo = CLogo::Create(D3DXVECTOR3(SCREEN_WIDTH / 2,100.0f,0.0f), RESULT_LOGO_WIDTH, RESULT_LOGO_HEIGHT);
	m_pLogo->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));	// �F�̐ݒ�

	// ���͑҂��\���𐶐�
	m_pPressButton = CLogoPressButton::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, 600.0f, 0.0f), RESULT_PRESS_WIDTH, RESULT_PRESS_HEIGHT);
	m_pPressButton->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));	// �F�̐ݒ�

	// ���͑҂��\���̓_�ő��x
	m_nFlash = RESULT_BEFORE_FLASH;

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CResult::Uninit(void)
{
	if (NULL != m_pLogo)
	{// ���S�̔j��
		m_pLogo->Uninit();
		m_pLogo = NULL;
	}

	for (int nCntNumber = 0; nCntNumber < m_nDigit; nCntNumber++)
	{// ���������[�v
		if (NULL != m_pNumber[nCntNumber])
		{// �����̔j��
			m_pNumber[nCntNumber]->Uninit();
			m_pNumber[nCntNumber] = NULL;

		}
	}

	if (NULL != m_pNumber)
	{// �����̔j��
		delete m_pNumber;
		m_pNumber = NULL;
	}

	if (NULL != m_pPressButton)
	{// �|���S���̔j��
		m_pPressButton->Uninit();
		m_pPressButton = NULL;
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
	// ���U���g�̌o�ߎ��Ԃ��J�E���g
	m_nCntResult++;

	// �_��
	m_pPressButton->Flashing(m_nFlash);

	// ���͂̏����擾
	CInputKeyboard	*pKeyboard = CManager::GetInputKeyboard();	// �L�[�{�[�h�̎擾
	CXInput			*pXInput = CManager::GetXInput();			// XInput�̎擾
	CSound			*pSound = CManager::GetSound();				// �T�E���h�̎擾


	if (pKeyboard->GetAnyKey() || pXInput->GetAnyButton(0))
	{// ��ʑJ��
		if (CFade::GetFade() == CFade::FADE_NONE)
		{// �t�F�[�h���Ȃ��Ƃ�
			CFade::Create(CManager::MODE_TITLE);
			//pSound->PlaySound(CSound::SOUND_LABEL_DECIDE);

			// �_�ł��鑬�x���グ��
			m_nFlash = RESULT_AFTER_FLASH;
		}
	}

	if (m_nCntResult >= RESULT_TIME)
	{// ��ʑJ��
		CFade::Create(CManager::MODE_TITLE);
	}

	CDebugProc::Print("���U���g");

}

//=============================================================================
// �`�揈��
//=============================================================================
void CResult::Draw(void)
{
}
