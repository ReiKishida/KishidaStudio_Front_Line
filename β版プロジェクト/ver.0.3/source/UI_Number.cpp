//=============================================================================
//
// UI_Number���� [UI_Number.cpp]
// Author : Ayaka Hakozaki
//
//=============================================================================
#include "UI_Texture.h"
#include "texture.h"
#include "number.h"
#include "input.h"
#include "manager.h"
#include "debugProc.h"
#include "fade.h"
#include "UI_Number.h"
#include "game.h"
#include "player.h"
#include "menu.h"
#include "gauge.h"
#include "AI.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define UI_POS	(D3DXVECTOR3((m_pos.x - (nCntDigits * m_fInt)) - m_fWidth / 2, m_pos.y, 0.0f))

//*****************************************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************************************

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CUI_NUMBER::CUI_NUMBER(int nPriority, CScene::OBJTYPE objType) : CScene2D(nPriority, objType)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_fWidth = 0.0f;
	m_fHeight = 0.0f;
	m_fInt = 0.0f;
	m_apNumber = NULL;
	m_nUV = 0;
	m_nUV_X = 0;
	m_nUV_Y = 0;
	m_nDigits = 0;
	m_nTimer = 0;
	m_pGaugeBlue = NULL;
	m_pGaugeRed = NULL;
	m_nDamege = 0;
	m_nInitGauge = 0;
	m_nDiff = 0;
	m_pUITex = NULL;
	m_nCntMove = 0;
	m_nPlayerLife = 0;
	m_nDroneLife = 0;
	m_nWorkerLife = 0;
	m_nRemBullet = 0;
	m_nCntRespawn = 0;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CUI_NUMBER::~CUI_NUMBER()
{
}

//=============================================================================
// ��������
//=============================================================================
CUI_NUMBER *CUI_NUMBER::Create(D3DXVECTOR3 pos, float fWidth, float fHeight, float fInt, UI_NUMTYPE UINumType, int nUV, int nUV_X, int nUV_Y)
{
	CUI_NUMBER *pUINum = NULL;	// UI�N���X�̃|�C���^�ϐ�

	pUINum = new CUI_NUMBER;		// �������m��

	if (NULL != pUINum)
	{	// �������m�ۂł���
		pUINum->m_UINumType = UINumType;	// �^�C�v
		pUINum->m_pos = pos;								// �ʒu
		pUINum->m_fWidth = fWidth;					// ��
		pUINum->m_fHeight = fHeight;					// ����
		pUINum->m_fInt = fInt;								// �����̊Ԋu
		pUINum->m_nUV = nUV;							// (0 : W, 1 : B, 2 : R)
		pUINum->m_nUV_X = nUV_X;					// �i���o�[�e�N�X�`��UV_X
		pUINum->m_nUV_Y = nUV_Y;					// �i���o�[�e�N�X�`��UV_Y
		pUINum->Init();											// ����������
	}

	return pUINum;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CUI_NUMBER::Init(void)
{
	// Scene2D�̏�����
	CScene2D::Init();

	// �ʒu�̐ݒ�
	SetPos(m_pos);

	//****************************************
	// �^�C�v�ʏ���
	//****************************************
	switch (m_UINumType)
	{
	case UI_NUMTYPE_NONE:
		break;

	case UI_NUMTYPE_REMAINBULLET:
		if (CMenu::GetMode() == CMenu::MODE_MULTI)
		{
			if (CManager::GetClient() != NULL)
			{
				m_nRemBullet = CManager::GetGame()->GetPlayer(CManager::GetClient()->GetPlayerIdx())->GetRemainBullet();
			}
		}
		else if (CMenu::GetMode() == CMenu::MODE_SINGLE)
		{	// �v���C���[����e���擾
			m_nRemBullet = CManager::GetGame()->GetPlayer(0)->GetBulletCapacity();
		}
		break;

	case UI_NUMTYPE_PLAYER_HP:
		if (CMenu::GetMode() == CMenu::MODE_MULTI)
		{
			if (CManager::GetClient() != NULL)
			{
				m_nPlayerLife = CManager::GetGame()->GetPlayer(CManager::GetClient()->GetPlayerIdx())->GetLife();
			}
		}
		else if (CMenu::GetMode() == CMenu::MODE_SINGLE)
		{	// �v���C���[���烉�C�t���擾
			m_nPlayerLife = CManager::GetGame()->GetPlayer(0)->GetLife();
		}
		break;

	case UI_NUMTYPE_ALLY_HP:		// �`�[������
		if (CMenu::GetMode() == CMenu::MODE_MULTI)
		{
			if (CManager::GetClient() != NULL)
			{
				int nPlayerIdx = 0;
				switch (CManager::GetClient()->GetPlayerIdx())
				{
				case 0:
					nPlayerIdx = 1;
					break;
				case 1:
					nPlayerIdx = 0;
					break;
				case 2:
					nPlayerIdx = 3;
					break;
				case 3:
					nPlayerIdx = 2;
					break;
				}
				m_nPlayerLife = CManager::GetGame()->GetPlayer(nPlayerIdx)->GetLife();
			}
		}
		else if (CMenu::GetMode() == CMenu::MODE_SINGLE)
		{	// �v���C���[���烉�C�t���擾
			m_nPlayerLife = CManager::GetGame()->GetPlayer(1)->GetLife();
		}
		break;

	case UI_NUMTYPE_DRONE_HP:		// �����̃h���[��
		if (CMenu::GetMode() == CMenu::MODE_MULTI)
		{
			if (CManager::GetClient() != NULL)
			{	// �S�v���C���[����A���C�t���擾
				m_nDroneLife = CManager::GetGame()->GetPlayer(CManager::GetClient()->GetPlayerIdx())->GetMyAI(0)->GetLife();
			}
		}
		else if (CMenu::GetMode() == CMenu::MODE_SINGLE)
		{	// �v���C���[���烉�C�t���擾
			m_nDroneLife = CManager::GetGame()->GetPlayer(0)->GetMyAI(0)->GetLife();		// 1P
		}
		break;

	case UI_NUMTYPE_WORKER_HP:		// �����̃��[�J�[
		if (CMenu::GetMode() == CMenu::MODE_MULTI)
		{
			if (CManager::GetClient() != NULL)
			{	// �S�v���C���[����A���C�t���擾
				m_nWorkerLife = CManager::GetGame()->GetPlayer(CManager::GetClient()->GetPlayerIdx())->GetMyAI(1)->GetLife();
			}
		}
		if (CMenu::GetMode() == CMenu::MODE_SINGLE)
		{	// �v���C���[���烉�C�t���擾
			m_nWorkerLife = CManager::GetGame()->GetPlayer(0)->GetMyAI(1)->GetLife();	// 1P
		}
		break;

	case UI_NUMTYPE_ALLY_DRONE_HP:		// ���`�[�������̃h���[��
		if (CMenu::GetMode() == CMenu::MODE_MULTI)
		{
			if (CManager::GetClient() != NULL)
			{
				int nPlayerIdx = 0;
				switch (CManager::GetClient()->GetPlayerIdx())
				{
				case 0:
					nPlayerIdx = 1;
					break;

				case 1:
					nPlayerIdx = 0;
					break;

				case 2:
					nPlayerIdx = 3;
					break;

				case 3:
					nPlayerIdx = 2;
					break;
				}
				m_nDroneLife = CManager::GetGame()->GetPlayer(nPlayerIdx)->GetMyAI(0)->GetLife();
			}
		}
		else if (CMenu::GetMode() == CMenu::MODE_SINGLE)
		{	// �v���C���[���烉�C�t���擾
			m_nDroneLife = CManager::GetGame()->GetPlayer(1)->GetMyAI(0)->GetLife();		// 2P
		}
		break;

	case UI_NUMTYPE_ALLY_WORKER_HP:		// ���`�[�������̃��[�J�[
		if (CMenu::GetMode() == CMenu::MODE_MULTI)
		{
			if (CManager::GetClient() != NULL)
			{
				int nPlayerIdx = 0;
				switch (CManager::GetClient()->GetPlayerIdx())
				{
				case 0:
					nPlayerIdx = 1;
					break;

				case 1:
					nPlayerIdx = 0;
					break;

				case 2:
					nPlayerIdx = 3;
					break;

				case 3:
					nPlayerIdx = 2;
					break;
				}
				m_nWorkerLife = CManager::GetGame()->GetPlayer(nPlayerIdx)->GetMyAI(1)->GetLife();
			}
		}
		else if (CMenu::GetMode() == CMenu::MODE_SINGLE)
		{	// �v���C���[���烉�C�t���擾
			m_nWorkerLife = CManager::GetGame()->GetPlayer(1)->GetMyAI(1)->GetLife();	// 2P
		}
		break;

	case UI_NUMTYPE_CNTRESPAWN:
		m_nCntRespawn = 5;		// �J�E���g�����l
		break;

	case UI_NUMTYPE_BLUE:
		m_nInitGauge = MAX_LINKENERGY;
		if (m_pGaugeBlue == NULL)
		{
			m_pGaugeBlue = CGauge2D::Create(2, m_pos, 0.0f, (float)m_nInitGauge, m_fWidth, m_fHeight);
			m_nInitGauge = CManager::GetGame()->GetBlueLinkEnergy();		// �Q�[������BLUE�`�[���`�P�b�g�擾
			m_pGaugeBlue->AddSubtract((float)m_nInitGauge);			// �Q�[�W�̑���
			m_pGaugeBlue->SetValue((float)m_nInitGauge);			// �Q�[�W�̒l�ݒ�

		}
		if (m_pGaugeBlue != NULL)
		{
			m_pGaugeBlue->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), 0);	// ���̒���
			m_pGaugeBlue->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 1);	// ���݂̗̑�
			m_pGaugeBlue->BindTexture(CTexture::GetTexture(CTexture::TEXTURE_LINK_BLUE), 1);
		}
		if (m_pUITex == NULL)
		{
			m_pUITex = CUI_TEXTURE::Create(m_pos, m_fWidth, m_fHeight, CUI_TEXTURE::UIFLAME_LINK_FLAME);
		}
		break;

	case UI_NUMTYPE_RED:
		m_nInitGauge = MAX_LINKENERGY;

		if (m_pGaugeRed == NULL)
		{
			m_pGaugeRed = CGauge2D::Create(2, m_pos, 0.0f, (float)m_nInitGauge, m_fWidth, m_fHeight);
			m_pGaugeRed->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), 0);	// ���̒���
			m_pGaugeRed->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 1);	// ���݂̗̑�
			m_pGaugeRed->BindTexture(CTexture::GetTexture(CTexture::TEXTURE_LINK_RED), 1);
			m_nInitGauge = CManager::GetGame()->GetRedLinkEnergy();		// �Q�[������BLUE�`�[���`�P�b�g�擾
			m_pGaugeRed->AddSubtract((float)m_nInitGauge);			// �Q�[�W�̑���
			m_pGaugeRed->SetValue((float)m_nInitGauge);			// �Q�[�W�̒l�ݒ�
		}
		if (m_pUITex == NULL)
		{
			m_pUITex = CUI_TEXTURE::Create(m_pos, m_fWidth, m_fHeight, CUI_TEXTURE::UIFLAME_LINK_FLAME);
		}
		break;
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CUI_NUMBER::Uninit(void)
{
	if (m_pUITex != NULL)
	{	// �����N�Q�[�W�̔j��
		m_pUITex->Uninit();
		m_pUITex = NULL;
	}

	if (m_pGaugeBlue != NULL)
	{	// BLUE�Q�[�W
		m_pGaugeBlue->Uninit();
		m_pGaugeBlue = NULL;
	}

	if (m_pGaugeRed != NULL)
	{	// RED�Q�[�W
		m_pGaugeRed->Uninit();
		m_pGaugeRed = NULL;
	}

	if (NULL != m_apNumber)
	{// �g���Ă���ꍇ�j������
		for (int nCntScore = 0; nCntScore < m_nDigits; nCntScore++)
		{// ���������[�v
			if (NULL != m_apNumber[nCntScore])
			{// �g���Ă���ꍇ�j������
				m_apNumber[nCntScore]->Uninit();
				m_apNumber[nCntScore] = NULL;
			}
		}

		delete[] m_apNumber;
		m_apNumber = NULL;
	}

	// Scene2D�̔j��
	CScene2D::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CUI_NUMBER::Update(void)
{
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();	// �L�[�{�[�h�̓��͂��擾
	m_UINumType = CUI_NUMBER::GetNumType();							// ���݂̃^�C�v���擾

																	// �J�E���^�[���Z
	m_nCntMove++;

	//****************************************
	// �^�C�v�ʏ���
	//****************************************
	switch (m_UINumType)
	{
	case UI_NUMTYPE_REMAINBULLET:
		if (CMenu::GetMode() == CMenu::MODE_MULTI)
		{
			if (CManager::GetClient() != NULL)
			{
				if (CManager::GetClient()->GetConnect() == true)
				{
					m_nRemBullet = CGame::GetPlayer(CManager::GetClient()->GetPlayerIdx())->GetRemainBullet();			// �v���C���[���猻�݂̒e�̒l���擾
					SetNumColor(m_nRemBullet, CGame::GetPlayer(CManager::GetClient()->GetPlayerIdx())->GetBulletCapacity());				// �l�ɂ���ĐF�ω�����

					if (m_nRemBullet >= 0)
					{	// �e���c���Ă�����X�V
						SetNum(m_nRemBullet);
					}
				}
			}
			else
			{
				m_nRemBullet = CGame::GetPlayer(0)->GetRemainBullet();			// �v���C���[���猻�݂̒e�̒l���擾
				SetNumColor(m_nRemBullet, CGame::GetPlayer(0)->GetBulletCapacity());				// �l�ɂ���ĐF�ω�����

				if (m_nRemBullet >= 0)
				{	// �e���c���Ă�����X�V
					SetNum(m_nRemBullet);
				}
			}
		}
		else if (CMenu::GetMode() == CMenu::MODE_SINGLE)
		{
			m_nRemBullet = CGame::GetPlayer(0)->GetRemainBullet();									// �v���C���[���猻�݂̒e�̒l���擾
			SetNumColor(m_nRemBullet, CGame::GetPlayer(0)->GetBulletCapacity());			// �l�ɂ���ĐF�ω�����

			if (m_nRemBullet >= 0)
			{	// �e���c���Ă�����X�V
				SetNum(m_nRemBullet);
			}
		}
		break;

	case UI_NUMTYPE_PLAYER_HP:
		if (CMenu::GetMode() == CMenu::MODE_MULTI)
		{
			if (CManager::GetClient() != NULL)
			{
				if (CManager::GetClient()->GetConnect() == true)
				{
					m_nPlayerLife = CGame::GetPlayer(CManager::GetClient()->GetPlayerIdx())->GetLife();
					SetNumColor(m_nPlayerLife, CGame::GetPlayer(CManager::GetClient()->GetPlayerIdx())->GetLifeMax());

					if (m_nPlayerLife >= 0)
					{	// 0�ȏ�̎��A�����X�V
						SetNum(m_nPlayerLife);
					}
				}
			}
			else
			{
				m_nPlayerLife = CGame::GetPlayer(CManager::GetClient()->GetPlayerIdx())->GetLife();
				SetNumColor(m_nPlayerLife, CGame::GetPlayer(0)->GetLifeMax());

				if (m_nPlayerLife >= 0)
				{	// 0�ȏ�̎��A�����X�V
					SetNum(m_nPlayerLife);
				}
			}
		}
		else if (CMenu::GetMode() == CMenu::MODE_SINGLE)
		{
			m_nPlayerLife = CGame::GetPlayer(0)->GetLife();
			SetNumColor(m_nPlayerLife, CGame::GetPlayer(0)->GetLifeMax());

			if (m_nPlayerLife >= 0)
			{	// 0�ȏ�̎��A�����X�V
				SetNum(m_nPlayerLife);
			}
		}
		break;

	case UI_NUMTYPE_ALLY_HP:
		if (CMenu::GetMode() == CMenu::MODE_MULTI)
		{
			if (CManager::GetClient() != NULL)
			{
				if (CManager::GetClient()->GetConnect() == true)
				{
					int nPlayerIdx = 0;
					switch (CManager::GetClient()->GetPlayerIdx())
					{
					case 0:
						nPlayerIdx = 1;
						break;
					case 1:
						nPlayerIdx = 0;
						break;
					case 2:
						nPlayerIdx = 3;
						break;
					case 3:
						nPlayerIdx = 2;
						break;
					}

					m_nPlayerLife = CManager::GetGame()->GetPlayer(nPlayerIdx)->GetLife();	// �����̃��C�t���擾
					SetNumColor(m_nPlayerLife, CGame::GetPlayer(CManager::GetClient()->GetPlayerIdx())->GetLifeMax());

					if (m_nPlayerLife >= 0)
					{	// 0�ȏ�̎��A�����X�V
						SetNum(m_nPlayerLife);
					}
				}
			}
			else
			{
				int nPlayerIdx = 0;
				switch (CManager::GetClient()->GetPlayerIdx())
				{
				case 0:
					nPlayerIdx = 1;
					break;
				case 1:
					nPlayerIdx = 0;
					break;
				case 2:
					nPlayerIdx = 3;
					break;
				case 3:
					nPlayerIdx = 2;
					break;
				}

				m_nPlayerLife = CManager::GetGame()->GetPlayer(nPlayerIdx)->GetLife();	// �����̃��C�t���擾
				SetNumColor(m_nPlayerLife, CGame::GetPlayer(1)->GetLifeMax());

				if (m_nPlayerLife >= 0)
				{	// 0�ȏ�̎��A�����X�V
					SetNum(m_nPlayerLife);
				}
			}
		}
		else if (CMenu::GetMode() == CMenu::MODE_SINGLE)
		{
			m_nPlayerLife = CGame::GetPlayer(1)->GetLife();
			SetNumColor(m_nPlayerLife, CGame::GetPlayer(1)->GetLifeMax());

			if (m_nPlayerLife >= 0)
			{	// 0�ȏ�̎��A�����X�V
				SetNum(m_nPlayerLife);
			}
		}
		break;

	case UI_NUMTYPE_CNTRESPAWN:
		m_nTimer++;
		if (m_nTimer % 60 == 0)
		{
			m_nCntRespawn--;
		}

		if (m_nCntRespawn >= 0)
		{	// �J�E���^�[0�ȏ�
			SetNum(m_nCntRespawn);
		}

		SetTime(m_nTimer);
		break;

	case UI_NUMTYPE_BLUE:
		if (m_pGaugeBlue != NULL)
		{
			m_nDiff = m_nInitGauge - CManager::GetGame()->GetBlueLinkEnergy();		// �_���[�W�ʎ擾

			if (m_nDiff > 0)
			{
				m_nInitGauge--;
				m_nDiff--;
			}

			m_pGaugeBlue->AddSubtract((float)m_nInitGauge);	// �Q�[�W�̑���
			m_pGaugeBlue->SetValue((float)m_nInitGauge);			// �Q�[�W�̒l�ݒ�
		}
		break;

	case UI_NUMTYPE_RED:
		if (m_pGaugeRed != NULL)
		{
			m_nDiff = m_nInitGauge - CManager::GetGame()->GetRedLinkEnergy();		// �_���[�W�ʎ擾

			if (m_nDiff > 0)
			{
				m_nInitGauge--;
				m_nDiff--;
			}

			m_pGaugeRed->AddSubtract((float)m_nInitGauge);		// �Q�[�W�̑���
			m_pGaugeRed->SetValue((float)m_nInitGauge);			// �Q�[�W�̒l�ݒ�
		}
		break;

	case UI_NUMTYPE_DRONE_HP:
		if (CMenu::GetMode() == CMenu::MODE_MULTI)
		{
			if (CManager::GetClient() != NULL)
			{
				if (CManager::GetClient()->GetConnect() == true)
				{
					m_nDroneLife = CGame::GetPlayer(CManager::GetClient()->GetPlayerIdx())->GetMyAI(0)->GetLife();
					SetNumColor(m_nDroneLife, CGame::GetPlayer(CManager::GetClient()->GetPlayerIdx())->GetMyAI(0)->GetLifeMax());

					if (m_nDroneLife >= 0)
					{	// 0�ȏ�̎��A�����X�V
						SetNum(m_nDroneLife);
					}
				}
			}
			else
			{
				m_nDroneLife = CGame::GetPlayer(CManager::GetClient()->GetPlayerIdx())->GetMyAI(0)->GetLife();
				SetNumColor(m_nDroneLife, CGame::GetPlayer(0)->GetLifeMax());

				if (m_nDroneLife >= 0)
				{	// 0�ȏ�̎��A�����X�V
					SetNum(m_nDroneLife);
				}
			}
		}
		else if (CMenu::GetMode() == CMenu::MODE_SINGLE)
		{
			m_nDroneLife = CManager::GetGame()->GetPlayer(0)->GetMyAI(0)->GetLife();
			SetNumColor(m_nDroneLife, CGame::GetPlayer(0)->GetMyAI(0)->GetLifeMax());

			if (m_nDroneLife >= 0)
			{	// 0�ȏ�̎��A�����X�V
				SetNum(m_nDroneLife);
			}
		}
		break;

	case UI_NUMTYPE_WORKER_HP:
		if (CMenu::GetMode() == CMenu::MODE_MULTI)
		{
			if (CManager::GetClient() != NULL)
			{
				if (CManager::GetClient()->GetConnect() == true)
				{
					m_nWorkerLife = CGame::GetPlayer(CManager::GetClient()->GetPlayerIdx())->GetMyAI(1)->GetLife();
					SetNumColor(m_nWorkerLife, CGame::GetPlayer(CManager::GetClient()->GetPlayerIdx())->GetMyAI(1)->GetLifeMax());

					if (m_nWorkerLife >= 0)
					{	// 0�ȏ�̎��A�����X�V
						SetNum(m_nWorkerLife);
					}
				}
			}
			else
			{
				m_nWorkerLife = CGame::GetPlayer(CManager::GetClient()->GetPlayerIdx())->GetMyAI(1)->GetLife();
				SetNumColor(m_nWorkerLife, CGame::GetPlayer(0)->GetLifeMax());

				if (m_nWorkerLife >= 0)
				{	// 0�ȏ�̎��A�����X�V
					SetNum(m_nWorkerLife);
				}
			}
		}
		else if (CMenu::GetMode() == CMenu::MODE_SINGLE)
		{
			m_nWorkerLife = CManager::GetGame()->GetPlayer(0)->GetMyAI(1)->GetLife();
			SetNumColor(m_nWorkerLife, CGame::GetPlayer(0)->GetMyAI(1)->GetLifeMax());

			if (m_nWorkerLife >= 0)
			{	// 0�ȏ�̎��A�����X�V
				SetNum(m_nWorkerLife);
			}
		}
		break;

	case UI_NUMTYPE_ALLY_DRONE_HP:
		if (CMenu::GetMode() == CMenu::MODE_MULTI)
		{
			if (CManager::GetClient() != NULL)
			{
				if (CManager::GetClient()->GetConnect() == true)
				{
					int nPlayerIdx = 0;
					switch (CManager::GetClient()->GetPlayerIdx())
					{
					case 0:
						nPlayerIdx = 1;
						break;
					case 1:
						nPlayerIdx = 0;
						break;
					case 2:
						nPlayerIdx = 3;
						break;
					case 3:
						nPlayerIdx = 2;
						break;
					}

					m_nDroneLife = CManager::GetGame()->GetPlayer(nPlayerIdx)->GetMyAI(0)->GetLife();	// �����h���[�����C�t���擾
					SetNumColor(m_nDroneLife, CGame::GetPlayer(CManager::GetClient()->GetPlayerIdx())->GetMyAI(0)->GetLifeMax());

					if (m_nDroneLife >= 0)
					{	// 0�ȏ�̎��A�����X�V
						SetNum(m_nDroneLife);
					}
				}
			}
			else
			{
				int nPlayerIdx = 0;
				switch (CManager::GetClient()->GetPlayerIdx())
				{
				case 0:
					nPlayerIdx = 1;
					break;
				case 1:
					nPlayerIdx = 0;
					break;
				case 2:
					nPlayerIdx = 3;
					break;
				case 3:
					nPlayerIdx = 2;
					break;
				}

				m_nDroneLife = CManager::GetGame()->GetPlayer(nPlayerIdx)->GetMyAI(0)->GetLife();	// �����h���[�����C�t���擾
				SetNumColor(m_nDroneLife, CGame::GetPlayer(0)->GetLifeMax());

				if (m_nDroneLife >= 0)
				{	// 0�ȏ�̎��A�����X�V
					SetNum(m_nDroneLife);
				}
			}
		}
		else if (CMenu::GetMode() == CMenu::MODE_SINGLE)
		{
			m_nDroneLife = CManager::GetGame()->GetPlayer(1)->GetMyAI(0)->GetLife();
			SetNumColor(m_nDroneLife, CGame::GetPlayer(1)->GetMyAI(0)->GetLifeMax());

			if (m_nDroneLife >= 0)
			{	// 0�ȏ�̎��A�����X�V
				SetNum(m_nDroneLife);
			}
		}
		break;

	case UI_NUMTYPE_ALLY_WORKER_HP:
		if (CMenu::GetMode() == CMenu::MODE_MULTI)
		{
			if (CManager::GetClient() != NULL)
			{
				if (CManager::GetClient()->GetConnect() == true)
				{
					int nPlayerIdx = 0;
					switch (CManager::GetClient()->GetPlayerIdx())
					{
					case 0:
						nPlayerIdx = 1;
						break;
					case 1:
						nPlayerIdx = 0;
						break;
					case 2:
						nPlayerIdx = 3;
						break;
					case 3:
						nPlayerIdx = 2;
						break;
					}

					m_nWorkerLife = CManager::GetGame()->GetPlayer(nPlayerIdx)->GetMyAI(1)->GetLife();	// �������[�J�[���C�t���擾
					SetNumColor(m_nWorkerLife, CGame::GetPlayer(CManager::GetClient()->GetPlayerIdx())->GetMyAI(1)->GetLifeMax());

					if (m_nWorkerLife >= 0)
					{	// 0�ȏ�̎��A�����X�V
						SetNum(m_nWorkerLife);
					}
				}
			}
			else
			{
				int nPlayerIdx = 0;
				switch (CManager::GetClient()->GetPlayerIdx())
				{
				case 0:
					nPlayerIdx = 1;
					break;
				case 1:
					nPlayerIdx = 0;
					break;
				case 2:
					nPlayerIdx = 3;
					break;
				case 3:
					nPlayerIdx = 2;
					break;
				}

				m_nWorkerLife = CManager::GetGame()->GetPlayer(nPlayerIdx)->GetMyAI(1)->GetLife();	// �������[�J�[���C�t���擾
				SetNumColor(m_nWorkerLife, CGame::GetPlayer(CManager::GetClient()->GetPlayerIdx())->GetMyAI(1)->GetLifeMax());

				if (m_nWorkerLife >= 0)
				{	// 0�ȏ�̎��A�����X�V
					SetNum(m_nWorkerLife);
				}
			}
		}
		else if (CMenu::GetMode() == CMenu::MODE_SINGLE)
		{
			m_nWorkerLife = CManager::GetGame()->GetPlayer(1)->GetMyAI(1)->GetLife();
			SetNumColor(m_nWorkerLife, CGame::GetPlayer(1)->GetMyAI(1)->GetLifeMax());

			if (m_nWorkerLife >= 0)
			{	// 0�ȏ�̎��A�����X�V
				SetNum(m_nWorkerLife);
			}
		}
		break;
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CUI_NUMBER::Draw(void)
{
	// Scene2D�̕`��
	CScene2D::Draw();
}

//=============================================================================
// ���݂̒l�E�����l�F�擾���̒l����v�Z���A����ɂ���ĐF�ς�鏈��
//=============================================================================
void CUI_NUMBER::SetNumColor(int nNum, int nDefNum)
{
	// ��{�A��
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	if (nNum <= nDefNum / 2)
	{	// �����ɂȂ����物�F
		m_col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
	}

	if (nNum <= nDefNum / 4)
	{	// 4/1�ɂȂ������
		m_col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	}
}

//=============================================================================
// ���݂̒l�E�����l�E�F�F�擾���Ă���������\�����čX�V���鏈��
//=============================================================================
void CUI_NUMBER::SetNum(int nCalcNum)
{
	//****************************************
	//���݂̒l�̕\���ݒ�
	//****************************************
	int nDigits = (int)log10f((float)nCalcNum) + 1;
	if (nDigits <= 0) { nDigits = 1; }		// ������0�ȉ��̂Ƃ�

	if (nDigits != m_nDigits)
	{// �������ς����
		for (int nCntScore = 0; nCntScore < m_nDigits; nCntScore++)
		{// ���������[�v
			if (NULL != m_apNumber[nCntScore])
			{// �g���Ă���ꍇ�j������
				m_apNumber[nCntScore]->Uninit();
				m_apNumber[nCntScore] = NULL;
			}
		}

		if (NULL != m_apNumber)
		{// �g���Ă���ꍇ�j������
			delete[] m_apNumber;
			m_apNumber = NULL;
		}

		// �����̐ݒ�i�擾���Ă������ɏ㏑���j
		m_nDigits = nDigits;

		// �������m��
		m_apNumber = new CNumber*[m_nDigits];

		if (m_apNumber)
		{// �������m�ۂł���
			for (int nCntDigits = 0; nCntDigits < m_nDigits; nCntDigits++)
			{// ���������[�v
				m_apNumber[nCntDigits] = CNumber::Create(UI_POS, m_fWidth, m_fHeight, m_UINumType, m_nUV, m_nUV_X, m_nUV_Y);
			}
		}
	}

	// �����̐ݒ�
	int aNumber;
	for (int nCntDigits = 0; nCntDigits < m_nDigits; nCntDigits++)
	{// ���̏�����������ݒ�
		aNumber = nCalcNum % (int)powf(10.0f, (float)(nCntDigits + 1)) / (int)powf(10.0f, (float)nCntDigits);	// �����̌v�Z
		m_apNumber[nCntDigits]->SetTexNumber(aNumber);		// �����̐ݒ�
		m_apNumber[nCntDigits]->SetColor(m_col);
	}
}