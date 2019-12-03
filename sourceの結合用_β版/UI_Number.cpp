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

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define UI_POS	(D3DXVECTOR3((m_pos.x - (nCntDigits * m_fInt)) + (m_fWidth * (m_nDigits / 2.0f)) - m_fWidth / 2, m_pos.y, 0.0f))

//*****************************************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************************************
int CUI_NUMBER::m_nRemBullet = 0;
int CUI_NUMBER::m_nPlayerLife = 0;
int CUI_NUMBER::m_nTeamBlue = 0;
int CUI_NUMBER::m_nTeamRed = 0;
int CUI_NUMBER::m_nCntRespawn = 0;

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
	m_nDecrease = 0;
	m_pGaugeBlue = NULL;
	m_pGaugeRed = NULL;
	m_nDamege = 0;
	m_nInitGauge = 0;
	m_nNum = 0;
	m_nDiff = 0;
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
		GetRemainBullet();								// �v���C���[����c�e�̎擾
		SetNumDisPlay(m_nRemBullet, m_UINumType);		// �c�e��\��
		break;

	case UI_NUMTYPE_PLAYER_HP:
		GetPlayerLife();									// �v���C���[���烉�C�t�擾
		SetNumDisPlay(m_nPlayerLife, m_UINumType);		// ���C�t��ݒ�
		break;

	case UI_NUMTYPE_CNTRESPAWN:
		m_nCntRespawn = 5;		// �J�E���g�����l
		SetNumDisPlay(m_nCntRespawn, m_UINumType);		// �`�P�b�g�ݒ�
		break;

	case UI_NUMTYPE_BLUE:
		m_nInitGauge = CManager::GetGame()->GetBlueLinkEnergy();		// �Q�[������BLUE�`�[���`�P�b�g�擾
		if (m_pGaugeBlue == NULL)
		{
			m_pGaugeBlue = CGauge2D::Create(2, m_pos, 0.0f, (float)m_nInitGauge, m_fWidth, m_fHeight);
		}
		if (m_pGaugeBlue != NULL)
		{
			m_pGaugeBlue->SetColor(D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f), 0);	// ���̒���
			m_pGaugeBlue->SetColor(D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f), 1);	// ���݂̗̑�
		}

		break;

	case UI_NUMTYPE_RED:
		m_nInitGauge = CManager::GetGame()->GetRedLinkEnergy();		// �Q�[������RED�`�[���`�P�b�g�擾
		if (m_pGaugeRed == NULL)
		{
			m_pGaugeRed = CGauge2D::Create(2, m_pos, 0.0f, (float)m_nInitGauge, m_fWidth, m_fHeight);
			m_pGaugeRed->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), 0);	// ���̒���
			m_pGaugeRed->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 1);	// ���݂̗̑�
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
					SetNumColor(m_nRemBullet, CGame::GetPlayer(CManager::GetClient()->GetPlayerIdx())->GetBulletCapacity());				// �l�ɂ���ĐF�ω�����
					SetNum(m_nRemBullet, CGame::GetPlayer(CManager::GetClient()->GetPlayerIdx())->GetBulletCapacity(), m_col);
				}
			}
			else
			{
				SetNumColor(m_nRemBullet, CGame::GetPlayer(0)->GetBulletCapacity());				// �l�ɂ���ĐF�ω�����
				SetNum(m_nRemBullet, CGame::GetPlayer(0)->GetBulletCapacity(), m_col);
			}
		}
		else if (CMenu::GetMode() == CMenu::MODE_SINGLE)
		{
			SetNumColor(m_nRemBullet, CGame::GetPlayer(0)->GetBulletCapacity());				// �l�ɂ���ĐF�ω�����
			SetNum(m_nRemBullet, CGame::GetPlayer(0)->GetBulletCapacity(), m_col);
		}
		break;

	case UI_NUMTYPE_PLAYER_HP:
		if (CMenu::GetMode() == CMenu::MODE_MULTI)
		{
			if (CManager::GetClient() != NULL)
			{
				if (CManager::GetClient()->GetConnect() == true)
				{

					SetNumColor(m_nPlayerLife, CGame::GetPlayer(CManager::GetClient()->GetPlayerIdx())->GetLifeMax());

					if (m_nPlayerLife >= 0)
					{	// 0�ȏ�̎��A�����X�V
						SetNum(m_nPlayerLife, CGame::GetPlayer(CManager::GetClient()->GetPlayerIdx())->GetLifeMax(), m_col);
					}
				}
			}
			else
			{
				SetNumColor(m_nPlayerLife, CGame::GetPlayer(0)->GetLifeMax());

				if (m_nPlayerLife >= 0)
				{	// 0�ȏ�̎��A�����X�V
					SetNum(m_nPlayerLife, CGame::GetPlayer(0)->GetLifeMax(), m_col);
				}
			}
		}
		else if (CMenu::GetMode() == CMenu::MODE_SINGLE)
		{
			SetNumColor(m_nPlayerLife, CGame::GetPlayer(0)->GetLifeMax());

			if (m_nPlayerLife >= 0)
			{	// 0�ȏ�̎��A�����X�V
				SetNum(m_nPlayerLife, CGame::GetPlayer(0)->GetLifeMax(), m_col);
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
			SetNum(m_nCntRespawn, 5, m_col);
		}
		else
		{	// �J�E���^�[0�ɂȂ���
			CGame::SetGameState(CGame::STATE_END);
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
	}

	// �e�����̐ݒ�
	SetPlayerLife(m_nPlayerLife);		// ���C�t
	SetTeamBlue(m_nTeamBlue);		// BLUE�`�[��
	SetTeamRed(m_nTeamRed);			// RED�`�[��
	SetRespawn(m_nCntRespawn);		// ������A�J�E���^�[
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

	if (CManager::GetGame()->GetPart() == CGame::PART_STRATEGY)
	{
		if (UI_NUMTYPE_REMAINBULLET == m_UINumType || UI_NUMTYPE_PLAYER_HP == m_UINumType)
		{
			m_col = D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f);
		}
	}

	if (nNum <= nDefNum / 2)
	{	// �����ɂȂ����物�F
		if (CManager::GetGame()->GetPart() == CGame::PART_STRATEGY)
		{// �X�g���e�W�[�p�[�g
			if (UI_NUMTYPE_REMAINBULLET == m_UINumType || UI_NUMTYPE_PLAYER_HP == m_UINumType)
			{
				m_col = D3DXCOLOR(0.1f, 0.1f, 0.0f, 1.0f);
			}
		}
		else
		{// �A�N�V�����p�[�g
			m_col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
		}
	}

	if (nNum <= nDefNum / 4)
	{	// 4/1�ɂȂ������
		if (CManager::GetGame()->GetPart() == CGame::PART_STRATEGY)
		{// �X�g���e�W�[�p�[�g
			if (UI_NUMTYPE_REMAINBULLET == m_UINumType || UI_NUMTYPE_PLAYER_HP == m_UINumType)
			{
				m_col = D3DXCOLOR(0.1f, 0.0f, 0.0f, 1.0f);
			}
		}
		else
		{// �A�N�V�����p�[�g
			m_col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		}
	}
}

//=============================================================================
// �擾���Ă����l�E�^�C�v�̎擾�F�擾���Ă���������\�����鏈��
//=============================================================================
void CUI_NUMBER::SetNumDisPlay(int nGetValue, UI_NUMTYPE UINumType)
{
	//****************************************
	//�����̕\�������ݒ�
	//****************************************
	int m_nDigits = (int)log10f((float)nGetValue) + 1;
	if (m_nDigits <= 0) { m_nDigits = 1; }			// ������0�ȉ��̂Ƃ�

	m_apNumber = new CNumber*[m_nDigits];			//������ ���������m��

	if (m_apNumber)
	{// �������m�ې���
		for (int nCntDigits = 0; nCntDigits < m_nDigits; nCntDigits++)
		{// ���������[�v
			m_apNumber[nCntDigits] = CNumber::Create(UI_POS, m_fWidth, m_fHeight, m_UINumType, m_nUV, m_nUV_X, m_nUV_Y);

			// �����̐ݒ�
			int aNumber = nGetValue % (int)powf(10.0f, (float)(nCntDigits + 1)) / (int)powf(10.0f, (float)nCntDigits);
			m_apNumber[nCntDigits]->SetTexNumber(aNumber);

			// �F�̐ݒ�
			m_apNumber[nCntDigits]->SetColor(m_col);
		}
	}

	// ���̐ݒ�
	SetDigits(m_nDigits);
}

//=============================================================================
// ���݂̒l�E�����l�E�F�F�擾���Ă���������\�����čX�V���鏈��
//=============================================================================
void CUI_NUMBER::SetNum(int nCalcNum, int nDefNum, D3DXCOLOR col)
{
	m_nDigits = GetDigits();			// SetNumDisPlay�Őݒ肵�������擾

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