//=============================================================================
//
// �}�b�`���O��ʏ��� [matching.cpp]
// Author : Ayaka Hakozaki & Jukiya Hayakawa
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
#define WIDTH					(130.0f)		// ��ʒ[����ǂ̂��炢����Ă��邩

#define UP_POS_Y					((SCREEN_HEIGHT / 2) - PLAYER_NUM_HEIGHT)		// 1P��3P��POS_X�Ŏg�p
#define RIGHT_POS_X					(SCREEN_WIDTH - WIDTH)							// 3P��4P��POS_X�Ŏg�p
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
	m_nCntFade = 0;
	m_bFade = false;

	for (int nCntUITex = 0; nCntUITex < MATCHING_UITEX; nCntUITex++)
	{
		m_pUITex[nCntUITex] = NULL;
	}

	for (int nCntPlayerUI = 0; nCntPlayerUI < MATCHING_UI_PLAYER; nCntPlayerUI++)
	{
		m_pPlayerUI[nCntPlayerUI] = NULL;
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
	m_nCntFade = 0;		//�t�F�[�h�̃J�E���^�[
	m_bFade = false;	//�t�F�[�h�����邩�ǂ���
	m_nNumBlue = 0;		//�`�[���̑���
	m_nNumRed = 0;		//�ԃ`�[���̑���

	for (int nCntConnect = 0; nCntConnect < MAX_PLAYER_CONNECT; nCntConnect++)
	{
		m_bConnect[nCntConnect] = false;
	}

	if (CMenu::GetMode() == CMenu::MODE_MULTI)
	{//�}���`�v���C�̏ꍇ
		//�N���C�A���g�̐���
		CManager::CreateClient();
	}

	//UI�e�N�X�`���̐�������
	CreateUITex();

	//�v���C���[UI�̐�������
	CreatePlayerUI();

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

	//�v���C���[UI�̔j��
	for (int nCntPlayerUI = 0; nCntPlayerUI < MATCHING_UI_PLAYER; nCntPlayerUI++)
	{
		if (m_pPlayerUI[nCntPlayerUI] != NULL)
		{
			m_pPlayerUI[nCntPlayerUI]->Uninit();
			m_pPlayerUI[nCntPlayerUI] = NULL;
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
	CClient *pClient = CManager::GetClient();
	if (pClient != NULL)
	{
		if (pClient->GetConnect() == true)
		{
			//m_bConnect[pClient->GetPlayerIdx()] = true;
		}
	}
	//�K�v�ȏ����������ޏ���
	PrintData();

	//����ǂݎ�鏈��
	ReadMessage();

	//UI�̃X�N���[������
	ScrollUI();

	//�t�F�[�h�����邩�̊m�F����
	CheckFade();

	CDebugProc::Print("�}�b�`���O���");
}

//=============================================================================
// �`�揈��
//=============================================================================
void CMatching::Draw(void)
{
}

//=============================================================================
// UI�e�N�X�`���̐�������
//=============================================================================
void CMatching::CreateUITex(void)
{
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

	// ���S
	CUI_TEXTURE::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), 150.0f, 150.0f, CUI_TEXTURE::UIFLAME_VS);
}

//=============================================================================
// �v���C���[UI�̏���
//=============================================================================
void CMatching::CreatePlayerUI(void)
{
	//�v���C���[0�Ԃ�UI
	m_pPlayerUI[0] = CScene2D::Create();
	m_pPlayerUI[0]->SetPos(D3DXVECTOR3((SCREEN_WIDTH / 2) - 300.0f, 150.0f, 0.0f));
	m_pPlayerUI[0]->SetSize(250.0f, 250.0f);
	m_pPlayerUI[0]->BindTexture(CTexture::GetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_MECHA_SELECT)));
	m_pPlayerUI[0]->SwapPriority(7);

	//�v���C���[�Ԃ�UI
	m_pPlayerUI[1] = CScene2D::Create();
	m_pPlayerUI[1]->SetPos(D3DXVECTOR3((SCREEN_WIDTH / 2) - 300.0f, SCREEN_HEIGHT - 250.0f, 0.0f));
	m_pPlayerUI[1]->SetSize(250.0f, 250.0f);
	m_pPlayerUI[1]->BindTexture(CTexture::GetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_MECHA_SELECT)));
	m_pPlayerUI[1]->SwapPriority(7);

	//�v���C���[2�Ԃ�UI
	m_pPlayerUI[2] = CScene2D::Create();
	m_pPlayerUI[2]->SetPos(D3DXVECTOR3((SCREEN_WIDTH / 2) + 300.0f, 150.0f, 0.0f));
	m_pPlayerUI[2]->SetSize(250.0f, 250.0f);
	m_pPlayerUI[2]->BindTexture(CTexture::GetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_MECHA_SELECT)));
	m_pPlayerUI[2]->SwapPriority(7);

	//�v���C���[3�Ԃ�UI
	m_pPlayerUI[3] = CScene2D::Create();
	m_pPlayerUI[3]->SetPos(D3DXVECTOR3((SCREEN_WIDTH / 2) + 300.0f, SCREEN_HEIGHT - 250.0f, 0.0f));
	m_pPlayerUI[3]->SetSize(250.0f, 250.0f);
	m_pPlayerUI[3]->BindTexture(CTexture::GetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_MECHA_SELECT)));
	m_pPlayerUI[3]->SwapPriority(7);
}

//=============================================================================
// UI�̃X�N���[������
//=============================================================================
void CMatching::ScrollUI(void)
{
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
			//�@�̂̎�ޏ�����
			pClient->SetMechaType(pClient->GetPlayerIdx(), CMechaSelect::GetMechaType());

			//�v���C���[�ԍ�����������
			pClient->Printf("%d", pClient->GetPlayerIdx());
			pClient->Printf(" ");

			//�@�̂̎�ޏ�����������
			pClient->Printf("%d", pClient->GetMechaType(pClient->GetPlayerIdx()));
			pClient->Printf(" ");

			if (pClient->GetPlayerIdx() == 0)
			{
				//�t�F�[�h�̂��Ă��邩�ǂ�����������
				switch (m_bFade)
				{
				case true:
					pClient->Printf("1");
					break;
				case false:
					pClient->Printf("0");
					break;
				}
			}
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
		if (pClient->GetConnect() == true)
		{
			char *pStr = pClient->GetReceiveData();
			int nWord = 0;
			int nPlayerIdx = 0;
			int nMechatype = 0;
			int nFade = 0;

			//���o������
			pStr = CServerFunction::HeadPutout(pStr, "");

			if (CServerFunction::Memcmp(pStr, SERVER_CONNECT_DATA) == 0)
			{//�ڑ������������Ă���ꍇ
				pStr += strlen(SERVER_CONNECT_DATA);								//���o��
				pClient->SetNumConnect(CServerFunction::ReadInt(pStr, ""));		//�����̐ݒu����
				nWord = CServerFunction::PopString(pStr, "");					//�������J�E���g
				pStr += nWord;													//���o��

				pClient->SetMinIdx(CServerFunction::ReadInt(pStr, ""));
				nWord = CServerFunction::PopString(pStr, "");					//�������J�E���g
				pStr += nWord;													//���o��

				pClient->SetMaxIdx(CServerFunction::ReadInt(pStr, ""));
				nWord = CServerFunction::PopString(pStr, "");					//�������J�E���g
				pStr += nWord;													//���o��

				m_nNumBlue = CServerFunction::ReadInt(pStr, "");
				nWord = CServerFunction::PopString(pStr, "");					//�������J�E���g
				pStr += nWord;													//���o��

				m_nNumRed = CServerFunction::ReadInt(pStr, "");
				nWord = CServerFunction::PopString(pStr, "");					//�������J�E���g
				pStr += nWord;													//���o��

				for (int nCntPlayerConnect = 0; nCntPlayerConnect < MAX_PLAYER_CONNECT; nCntPlayerConnect++)
				{
					m_bConnect[nCntPlayerConnect] = CServerFunction::ReadBool(pStr, "");
					nWord = CServerFunction::PopString(pStr, "");					//�������J�E���g
					pStr += nWord;													//���o��
				}
			}
			if (CServerFunction::Memcmp(pStr, SERVER_PLAYER_START) == 0)
			{//�v���C���[�̊J�n�������Ă���ꍇ
				pStr += strlen(SERVER_PLAYER_START);

				for (int nCntClient = pClient->GetMinIdx(); nCntClient < pClient->GetMaxIdx(); nCntClient++)
				{
					//�ԍ�����
					nPlayerIdx = CServerFunction::ReadInt(pStr, "");
					nWord = CServerFunction::PopString(pStr, "");		//�������J�E���g
					pStr += nWord;										//���o��

					//���J�̎�ނ���												//�ԍ�����
					nMechatype = CServerFunction::ReadInt(pStr, "");
					nWord = CServerFunction::PopString(pStr, "");		//�������J�E���g
					pStr += nWord;										//���o��

					if (nPlayerIdx == 0)
					{
						//�t�F�[�h������																		//���J�̎�ނ���												//�ԍ�����
						nFade = CServerFunction::ReadInt(pStr, "");
						nWord = CServerFunction::PopString(pStr, "");		//�������J�E���g
						pStr += nWord;										//���o��
					}

					//
					if (nFade == 1)
					{
						m_bFade = true;
					}

					if (nPlayerIdx != pClient->GetPlayerIdx())
					{
						pClient->SetMechaType(nPlayerIdx, nMechatype);
					}
				}
			}
			CDebugProc::Print("��ށF%d %d %d %d", pClient->GetMechaType(0), pClient->GetMechaType(1), pClient->GetMechaType(2), pClient->GetMechaType(3));
			CDebugProc::Print("�ŏ��F%d", pClient->GetMinIdx());
			CDebugProc::Print("�ő�F%d", pClient->GetMaxIdx());
		}
		for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER_CONNECT; nCntPlayer++)
		{//
			if (m_bConnect[nCntPlayer] == true)
			{
				m_pPlayerUI[nCntPlayer]->SetTex(pClient->GetMechaType(nCntPlayer) + 1, 1, 5);
			}
			else if (m_bConnect[nCntPlayer] == false)
			{
				m_pPlayerUI[nCntPlayer]->SetTex(0, 1, 5);
			}
		}

	}
}

//=============================================================================
// �t�F�[�h�����邩�m�F���鏈��
//=============================================================================
void CMatching::CheckFade(void)
{
	//�N���C�A���g�̎擾
	CClient *pClient = CManager::GetClient();

	// ���͂̏����擾
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();
	CXInput *pXInput = CManager::GetXInput();

	// �T�E���h�̎擾
	CSound *pSound = CManager::GetSound();

	if (pClient != NULL)
	{
		//if (pClient->GetNumConnect() >= 2 /*MAX_CONNECT*/ &&
		//	m_bFade == false)
		//if(m_nNumBlue >= 1 && m_nNumRed >= 1 && m_bFade == false)
		//{//�ڑ�����Ă��鑍������萔�ȏ�̏ꍇ���t�F�[�h�����Ă��Ȃ��ꍇ
		//	//if (pKeyboard->GetAnyKey() || pXInput->GetAnyButton(0))
		//	if(pKeyboard->GetTrigger(DIK_RETURN) ==true)
		//	{//�{�^���������ꂽ�ꍇ
		//		m_bFade = true;	//�t�F�[�h�������Ԃɂ���
		//	}
		//}
		if (m_nNumBlue == 2 && m_nNumRed == 2 && m_bFade == false)
		{
			if (pClient->GetPlayerIdx() == 0)
			{
				m_bFade = true;
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
		}
	}

}