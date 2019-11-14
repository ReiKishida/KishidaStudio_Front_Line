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
#include "server.h"
#include "menu.h"
#include "serverfunction.h"
#include "mechaSelect.h"

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
	m_nCntFade = 0;
	m_bFade = false;

	for (int nCntPlayerUI = 0; nCntPlayerUI < MATCHING_UI_PLAYER; nCntPlayerUI++)
	{
		m_pMatchingPlayerUI[nCntPlayerUI] = NULL;
	}

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
	m_nCntFade = 0;
	m_bFade = false;
	if (CMenu::GetMode() == CMenu::MODE_MULTI)
	{
		CManager::CreateClient();
	}

	// �w�i���n
	CUI_TEXTURE::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), SCREEN_WIDTH, SCREEN_HEIGHT, CUI_TEXTURE::UIFLAME_MATCHING_BG);

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

	m_pMatchingPlayerUI[0] = CScene2D::Create();
	m_pMatchingPlayerUI[0]->SetPos(D3DXVECTOR3((SCREEN_WIDTH / 2) - 300.0f, 150.0f, 0.0f));
	m_pMatchingPlayerUI[0]->SetSize(250.0f, 250.0f);
	m_pMatchingPlayerUI[0]->BindTexture(CTexture::GetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_MECHA_SELECT)));
	m_pMatchingPlayerUI[0]->SwapPriority(7);

	m_pMatchingPlayerUI[1] = CScene2D::Create();
	m_pMatchingPlayerUI[1]->SetPos(D3DXVECTOR3((SCREEN_WIDTH / 2) - 300.0f, SCREEN_HEIGHT - 250.0f, 0.0f));
	m_pMatchingPlayerUI[1]->SetSize(250.0f, 250.0f);
	m_pMatchingPlayerUI[1]->BindTexture(CTexture::GetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_MECHA_SELECT)));
	m_pMatchingPlayerUI[1]->SwapPriority(7);

	m_pMatchingPlayerUI[2] = CScene2D::Create();
	m_pMatchingPlayerUI[2]->SetPos(D3DXVECTOR3((SCREEN_WIDTH / 2) + 300.0f, 150.0f, 0.0f));
	m_pMatchingPlayerUI[2]->SetSize(250.0f, 250.0f);
	m_pMatchingPlayerUI[2]->BindTexture(CTexture::GetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_MECHA_SELECT)));
	m_pMatchingPlayerUI[2]->SwapPriority(7);

	m_pMatchingPlayerUI[3] = CScene2D::Create();
	m_pMatchingPlayerUI[3]->SetPos(D3DXVECTOR3((SCREEN_WIDTH / 2) + 300.0f, SCREEN_HEIGHT - 250.0f, 0.0f));
	m_pMatchingPlayerUI[3]->SetSize(250.0f, 250.0f);
	m_pMatchingPlayerUI[3]->BindTexture(CTexture::GetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_MECHA_SELECT)));
	m_pMatchingPlayerUI[3]->SwapPriority(7);

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
	//�K�v�ȏ����������ޏ���
	PrintData();

	//����ǂݎ�鏈��
	ReadMessage();

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
		 //CFade::Create(CManager::MODE_GAME);
		 //pSound->PlaySound(CSound::SOUND_LABEL_DECIDE);
		}
	}

	CDebugProc::Print("�}�b�`���O���");

	CClient *pClient = CManager::GetClient();			//�N���C�A���g�̃|�C���^���

	if (pClient != NULL)
	{
		char *pStr = pClient->GetReceiveData();				//�T�[�o�[����󂯎�������b�Z�[�W���

															//���o������
		pStr = CServerFunction::HeadPutout(pStr, "");

		if (CServerFunction::Memcmp(pStr, SERVER_NUM_CONNECT) == 0)
		{//�ڑ������������Ă���ꍇ
			pStr += strlen(SERVER_NUM_CONNECT);								//���o��
			pClient->SetNumConnect(CServerFunction::ReadInt(pStr, ""));		//�����̐ݒu����
		}

		if (pClient->GetNumConnect() >= /*2*/ MAX_CONNECT)
		{
			if (m_bFade == false)
			{
				m_bFade = true;
			}
			if (CFade::GetFade() == CFade::FADE_NONE)
			{// �t�F�[�h���Ȃ��Ƃ�

			}

		}
		if (pClient->GetConnect() == true)
		{
			for (int nCntPlayer = 0; nCntPlayer < MAX_CONNECT; nCntPlayer++)
			{

				m_pMatchingPlayerUI[nCntPlayer]->SetTex(pClient->GetMechaType(nCntPlayer), 1, 4);
			}
		}

	}

	if (m_bFade == true)
	{
		m_nCntFade++;
	}
	if (m_nCntFade >= 200)
	{
		if (CFade::GetFade() == CFade::FADE_NONE)
		{// �t�F�[�h���Ȃ��Ƃ�

			CFade::Create(CManager::MODE_GAME);
			//pSound->PlaySound(CSound::SOUND_LABEL_DECIDE);
		}

	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CMatching::Draw(void)
{
}

//=============================================================================
// �K�v�ȏ����������ޏ���
//=============================================================================
void CMatching::PrintData(void)
{
	//�N���C�A���g�̎擾
	CClient *pClient = CManager::GetClient();
	if (pClient != NULL)
	{
		if (pClient->GetConnect() == true)
		{
			int nIdx = pClient->GetClientIdx();
			pClient->SetMechaType(pClient->GetPlayerIdx(), CMechaSelect::GetMechaType());
			//m_pMatchingPlayerUI[nIdx]->BindTexture(CTexture::GetTexture((CTexture::TEXTURE)CMechaSelect::GetMechaType()));
			pClient->Printf("%d", pClient->GetPlayerIdx());
			pClient->Printf(" ");
			pClient->Printf("%d", pClient->GetMechaType(pClient->GetPlayerIdx()));
			pClient->Printf(" ");
			CDebugProc::Print("�v���C���[�ԍ�%d", pClient->GetPlayerIdx());
		}
	}
}

//=============================================================================
// ����ǂݎ�鏈��
//=============================================================================
void CMatching::ReadMessage(void)
{
	//�N���C�A���g�̎擾
	CClient *pClient = CManager::GetClient();

	if (pClient != NULL)
	{
		if (pClient->GetConnect())
		{
			char *pStr = pClient->GetReceiveData();
			int nWord = 0;
			int nPlayerIdx = 0;
			int nMechatype = 0;

			//���o������
			pStr = CServerFunction::HeadPutout(pStr, "");

			if (CServerFunction::Memcmp(pStr, SERVER_NUM_CONNECT) == 0)
			{//�ڑ������������Ă���ꍇ
				pStr += strlen(SERVER_NUM_CONNECT);								//���o��
				pClient->SetNumConnect(CServerFunction::ReadInt(pStr, ""));		//�����̐ݒu����
				nWord = CServerFunction::PopString(pStr, "");					//�������J�E���g
				pStr += nWord;													//���o��
			}
			if (CServerFunction::Memcmp(pStr, SERVER_PLAYER_START) == 0)
			{//�v���C���[�̊J�n�������Ă���ꍇ
				pStr += strlen(SERVER_PLAYER_START);

				for (int nCntClient = 0; nCntClient < pClient->GetNumConnect() - 1; nCntClient++)
				{
					//�ԍ�����
					nPlayerIdx = CServerFunction::ReadInt(pStr, "");
					nWord = CServerFunction::PopString(pStr, "");		//�������J�E���g
					pStr += nWord;										//���o��

																		//���J�̎�ނ���												//�ԍ�����
					nMechatype = CServerFunction::ReadInt(pStr, "");
					nWord = CServerFunction::PopString(pStr, "");		//�������J�E���g
					pStr += nWord;										//���o��

					if (nPlayerIdx != pClient->GetPlayerIdx())
					{
						pClient->SetMechaType(nPlayerIdx, nMechatype);
					}
				}
			}
			CDebugProc::Print("��ށF%d %d %d %d", pClient->GetMechaType(0), pClient->GetMechaType(1), pClient->GetMechaType(2), pClient->GetMechaType(3));
		}
	}
}