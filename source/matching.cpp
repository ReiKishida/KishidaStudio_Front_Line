//=============================================================================
//
// �}�b�`���O��ʏ��� [matching.cpp]
// Author : Ayaka Hakozaki
//
//=============================================================================
#include "matching.h"
#include "fade.h"
#include "input.h"
#include "debugProc.h"
#include "sound.h"
#include "texture.h"
#include "UI_Texture.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define PLAYER_NUM_WIDTH		(150.0f)		// �v���C���[�i���o�[���S�̕�
#define PLAYER_NUM_HEIGHT		(70.0f)			// �v���C���[�i���o�[���S�̍���
#define WIDTH								(130.0f)		// ��ʒ[����ǂ̂��炢����Ă��邩

#define UP_POS_Y							((SCREEN_HEIGHT / 2) - PLAYER_NUM_HEIGHT)		// 1P��3P��POS_X�Ŏg�p
#define RIGHT_POS_X					(SCREEN_WIDTH - WIDTH)											// 3P��4P��POS_X�Ŏg�p
#define DOWN_POS_Y					((SCREEN_HEIGHT - PLAYER_NUM_HEIGHT))			// 2P��4P��POS_Y�Ŏg�p

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************

//=========================================
// �R���X�g���N�^
//=========================================
CMatching::CMatching(int nPriority, CScene::OBJTYPE objType) : CScene(nPriority, objType)
{
	m_nCntBgMove = 0;
	m_pUITex[0] = NULL;
	m_pUITex[1] = NULL;
	m_pUITex[2] = NULL;
	m_pUITex[3] = NULL;
}

//=========================================
// �f�X�g���N�^
//=========================================
CMatching::~CMatching()
{
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CMatching::Init(void)
{
	// �w�i���n
	CUI_TEXTURE::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), SCREEN_WIDTH , SCREEN_HEIGHT, CUI_TEXTURE::UIFLAME_MATCHING_BG);

	// BLUE�`�[���w�i�X�N���[��
	m_pUITex[0] = CUI_TEXTURE::Create(D3DXVECTOR3(SCREEN_WIDTH / 4, SCREEN_HEIGHT / 2, 0.0f), SCREEN_WIDTH / 2, SCREEN_HEIGHT, CUI_TEXTURE::UIFLAME_MATCHING_BG_00);
	m_pUITex[1] = CUI_TEXTURE::Create(D3DXVECTOR3(SCREEN_WIDTH / 4, SCREEN_HEIGHT / 2, 0.0f), SCREEN_WIDTH / 2, SCREEN_HEIGHT, CUI_TEXTURE::UIFLAME_MATCHING_BG_01);

	// RED�`�[���w�i�X�N���[��
	m_pUITex[2] = CUI_TEXTURE::Create(D3DXVECTOR3(SCREEN_WIDTH - (SCREEN_WIDTH / 4), SCREEN_HEIGHT / 2, 0.0f), SCREEN_WIDTH / 2, SCREEN_HEIGHT, CUI_TEXTURE::UIFLAME_MATCHING_BG_00);
	m_pUITex[3] = CUI_TEXTURE::Create(D3DXVECTOR3(SCREEN_WIDTH - (SCREEN_WIDTH / 4), SCREEN_HEIGHT / 2, 0.0f), SCREEN_WIDTH / 2, SCREEN_HEIGHT, CUI_TEXTURE::UIFLAME_MATCHING_BG_01);

	// 1P���S
	m_pUITex[4] = CUI_TEXTURE::Create(D3DXVECTOR3(WIDTH, UP_POS_Y, 0.0f), PLAYER_NUM_WIDTH, PLAYER_NUM_HEIGHT, CUI_TEXTURE::UIFLAME_PLAYER_NUM);
	m_pUITex[4]->SetTex(0, 1, 4);

	// 2P���S
	m_pUITex[5] = CUI_TEXTURE::Create(D3DXVECTOR3(WIDTH, DOWN_POS_Y, 0.0f), PLAYER_NUM_WIDTH, PLAYER_NUM_HEIGHT, CUI_TEXTURE::UIFLAME_PLAYER_NUM);
	m_pUITex[5]->SetTex(1, 1, 4);

	// 3P���S
	m_pUITex[6] = CUI_TEXTURE::Create(D3DXVECTOR3(RIGHT_POS_X, UP_POS_Y, 0.0f), PLAYER_NUM_WIDTH, PLAYER_NUM_HEIGHT, CUI_TEXTURE::UIFLAME_PLAYER_NUM);
	m_pUITex[6]->SetTex(2, 1, 4);

	// 4P���S
	m_pUITex[7] = CUI_TEXTURE::Create(D3DXVECTOR3(RIGHT_POS_X, DOWN_POS_Y, 0.0f), PLAYER_NUM_WIDTH, PLAYER_NUM_HEIGHT, CUI_TEXTURE::UIFLAME_PLAYER_NUM);
	m_pUITex[7]->SetTex(3, 1, 4);

	// ���S
	CUI_TEXTURE::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), 150.0f, 150.0f, CUI_TEXTURE::UIFLAME_VS);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CMatching::Uninit(void)
{
	// UI�̔j���i�w�i�X�N���[���j
	for (int nCntUITex = 0; nCntUITex < MATCHING_UITEX; nCntUITex++)
	{
		if (m_pUITex[nCntUITex] != NULL)
		{
			m_pUITex[nCntUITex]->Uninit();
			m_pUITex[nCntUITex] = NULL;
		}
	}

	// �����̔j��
	CScene::Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CMatching::Update(void)
{
	// ���͂̏����擾
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();
	CXInput *pXInput = CManager::GetXInput();

	// �T�E���h�̎擾
	CSound *pSound = CManager::GetSound();

	// �J�E���^�[���Z
	m_nCntBgMove++;

	//****************************************
	//  BLUE�`�[���F�F�ƃX�N���[������
	//****************************************
	m_pUITex[0]->SetColor(D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
	m_pUITex[0]->SetBgMove(0, -m_nCntBgMove, 0.0025f);				// ��

	m_pUITex[1]->SetColor(D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
	m_pUITex[1]->SetBgMove(0, m_nCntBgMove, 0.0025f);				// ��

	//****************************************
	//  RED�`�[���F�F�ƃX�N���[������
	//****************************************
	m_pUITex[2]->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	m_pUITex[2]->SetBgMove(0, -m_nCntBgMove, 0.0020f);			// ��

	m_pUITex[3]->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	m_pUITex[3]->SetBgMove(0, m_nCntBgMove, 0.0020f);			// ��

	//****************************************
	// ��ʑJ��
	//****************************************
	if (pKeyboard->GetAnyKey() || pXInput->GetAnyButton(0))
	{
		if (CFade::GetFade() == CFade::FADE_NONE)
		{// �t�F�[�h���Ȃ��Ƃ�
			CFade::Create(CManager::MODE_GAME);
			//pSound->PlaySound(CSound::SOUND_LABEL_DECIDE);
		}
	}

	CDebugProc::Print("�}�b�`���O���");
}

//=============================================================================
// �`�揈��
//=============================================================================
void CMatching::Draw(void)
{
}