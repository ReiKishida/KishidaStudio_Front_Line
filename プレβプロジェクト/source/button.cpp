//=============================================================================
//
// �{�^������ [button.cpp]
// Author : Takuto Ishida
//
//=============================================================================
#include "button.h"
#include "manager.h"
#include "renderer.h"
#include "debugProc.h"
#include "input.h"
#include "texture.h"
#include "game.h"
#include "mouseCursor.h"
#include "fade.h"
#include "camera.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define BUTTON_UNSELECTED_COLOR		(D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f))		// ��I�����̐F
#define BUTTON_LOGIC_WIDTH			(80.0f)									// ���W�b�N�c���[�{�^���̕�
#define BUTTON_LOGIC_HEIGHT			(80.0f)									// ���W�b�N�c���[�{�^���̍���
#define BUTTON_LINE_THICK			(2.5f)									// ���̑���

#define BUTTON_NUM_FIRST_ICON		(2)
#define BUTTON_NUM_SECOND_ICON		(2)
#define BUTTON_NUM_THIRD_ICON		(2)
#define BUTTON_NUM_FORTH_ICON		(2)

//==================================
// �ÓI�����o�ϐ��錾
//==================================

/****************************************************************/
/*						2D�{�^���̃N���X						*/
/****************************************************************/
//==================================
// ��������
//==================================
CButton2D* CButton2D::Create(D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	CButton2D *pButton;

	pButton = new CButton2D;	// ���������m��

	if (NULL != pButton)
	{// �������m�ې���
		pButton->Init(pos, fWidth, fHeight);	// ����������
	}

	return pButton;
}

//=========================================
// �R���X�g���N�^
//=========================================
CButton2D::CButton2D(int nPriority, CScene::OBJTYPE objType) : CScene2D(nPriority, objType)
{
	m_bInRange = false;
	m_bSwitch = false;
	m_bRelease = false;
	m_size = D3DXVECTOR2(0.0f, 0.0f);
}

//=========================================
// �f�X�g���N�^
//=========================================
CButton2D::~CButton2D()
{
}

//=========================================
// ����������
//=========================================
HRESULT CButton2D::Init(D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	// �|���S���̐ݒ�
	CScene2D::Init();
	CScene2D::SetPos(pos);
	CScene2D::SetSize(fWidth, fHeight);
	CScene2D::SetColor(BUTTON_UNSELECTED_COLOR);
	m_size = D3DXVECTOR2(fWidth, fHeight);
	return S_OK;
}

//=========================================
// �I������
//=========================================
void CButton2D::Uninit(void)
{
	CScene2D::Uninit();
}

//=========================================
// �X�V����
//=========================================
void CButton2D::Update(void)
{
}

//=========================================
// �`�揈��
//=========================================
void CButton2D::Draw(void)
{
	CScene2D::Draw();
}

//=========================================
// �͈͓����`�F�b�N
//=========================================
bool CButton2D::InRange(D3DXVECTOR3 _pos)
{
	if (CFade::GetFade() != CFade::FADE_NONE) { return false; }

	D3DXVECTOR3 pos = CScene2D::GetPos();

	if (pos.x - (m_size.x * 0.5f) <= _pos.x && pos.x + (m_size.x * 0.5f) >= _pos.x)
	{// X�����͈͓�
		if (pos.y - (m_size.y * 0.5f) <= _pos.y && pos.y + (m_size.y * 0.5f) >= _pos.y)
		{// Y�����͈͓�
			m_bInRange = true;
			CScene2D::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			return m_bInRange;
		}
	}

	if (m_bRelease)
	{// �N���b�N���ꂽ���N�������ɔ͈͊O�ɏo��
		CScene2D::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		CScene2D::SetSize(m_size.x, m_size.y);
	}

	// �N�����Ă��Ȃ�
	if (!m_bSwitch) { CScene2D::SetColor(BUTTON_UNSELECTED_COLOR); }

	// �t���O�𕉂ɂ���
	m_bInRange = false;
	m_bRelease = false;
	return m_bInRange;
}

//=========================================
// �͈͓����`�F�b�N�i���j���[�A�@�̑I���Ŏg�p�j
//=========================================
bool CButton2D::InRangeMenu(D3DXVECTOR3 _pos)
{
	if (CFade::GetFade() != CFade::FADE_NONE) { return false; }

	D3DXVECTOR3 pos = CScene2D::GetPos();

	if (pos.x - (m_size.x * 0.4f) <= _pos.x && pos.x + (m_size.x * 0.4f) >= _pos.x)
	{// X�����͈͓�
		if (pos.y - (m_size.y * 0.4f) <= _pos.y && pos.y + (m_size.y * 0.4f) >= _pos.y)
		{// Y�����͈͓�
			m_bInRange = true;
			CScene2D::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			return m_bInRange;
		}
	}

	if (m_bRelease)
	{// �N���b�N���ꂽ���N�������ɔ͈͊O�ɏo��
		CScene2D::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		CScene2D::SetSize(m_size.x, m_size.y);
	}

	// �N�����Ă��Ȃ�
	if (!m_bSwitch) { CScene2D::SetColor(BUTTON_UNSELECTED_COLOR); }

	// �t���O�𕉂ɂ���
	m_bInRange = false;
	m_bRelease = false;
	return m_bInRange;
}

//=========================================
// �I���̐ݒ�
//=========================================
bool CButton2D::Click(void)
{
	if (!m_bInRange) { return false; }	// �͈͊O�������甲����

	CInputMouse *pMouse = CManager::GetInputMouse();	// �}�E�X�̓��͂��擾

	if (pMouse->GetTrigger(CInputMouse::DIMS_BUTTON_0))
	{// ���N���b�N���ꂽ
		m_bSwitch = m_bSwitch ? false : true;
		return true;
	}

	return false;
}

//=========================================
// �I���̐ݒ�
//=========================================
bool CButton2D::ClickRelease(void)
{
	if (!m_bInRange) { return false; }	// �͈͊O�������甲����

	CInputMouse *pMouse = CManager::GetInputMouse();	// �}�E�X�̓��͂��擾

	if (pMouse->GetTrigger(CInputMouse::DIMS_BUTTON_0))
	{// ���N���b�N���ꂽ
		m_bRelease = true;
		CScene2D::SetSize(CScene2D::GetWidth() * 0.9f, CScene2D::GetHeight() * 0.9f);
	}

	if (m_bRelease)
	{// �{�^����������Ă����
		CScene2D::SetColor(D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f));
	}

	if (pMouse->GetRelease(CInputMouse::DIMS_BUTTON_0) && m_bRelease)
	{// ���N���b�N�𗣂���
		m_bSwitch = m_bSwitch ? false : true;
		CScene2D::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		CScene2D::SetSize(m_size.x, m_size.y);
		m_bRelease = false;
		return true;
	}

	return false;
}

/****************************************************************/
/*						3D�{�^���̃N���X						*/
/****************************************************************/
//==================================
// ��������
//==================================
CButton3D* CButton3D::Create(D3DXVECTOR3 pos, float fWidth, float fDepth)
{
	CButton3D *pButton;

	pButton = new CButton3D;	// ���������m��

	if (NULL != pButton)
	{// �������m�ې���
		pButton->Init(pos, fWidth, fDepth);	// ����������
	}

	return pButton;
}

//=========================================
// �R���X�g���N�^
//=========================================
CButton3D::CButton3D(int nPriority, CScene::OBJTYPE objType) : CScene3D(nPriority, objType)
{
	m_bInRange = false;
	m_bSwitch = false;
	m_bRelease = false;
	m_size = D3DXVECTOR2(0.0f, 0.0f);
	m_bDisp = true;
}

//=========================================
// �f�X�g���N�^
//=========================================
CButton3D::~CButton3D()
{
}

//=========================================
// ����������
//=========================================
HRESULT CButton3D::Init(D3DXVECTOR3 pos, float fWidth, float fDepth)
{
	// �|���S���̐ݒ�
	CScene3D::Init();
	CScene3D::SetPos(pos);
	CScene3D::SetSize(D3DXVECTOR3(fWidth,0.0f,fDepth));
	CScene3D::SetColor(BUTTON_UNSELECTED_COLOR);
	m_size = D3DXVECTOR2(fWidth, fDepth);
	return S_OK;
}

//=========================================
// �I������
//=========================================
void CButton3D::Uninit(void)
{
	CScene3D::Uninit();
}

//=========================================
// �X�V����
//=========================================
void CButton3D::Update(void)
{
}

//=========================================
// �`�揈��
//=========================================
void CButton3D::Draw(void)
{
	// ��\���̏ꍇ�`�悵�Ȃ�
	if (!m_bDisp) { return; }

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// ���C�e�B���OOFF
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// Z�e�X�gOFF
	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);

	CScene3D::Draw();

	// Z�e�X�gON
	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

	// ���C�e�B���OON
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//=========================================
// �͈͓����`�F�b�N
//=========================================
bool CButton3D::InRange(D3DXVECTOR3 _pos)
{
	D3DXVECTOR3 pos = CScene3D::GetPos();

	if (pos.x - m_size.x <= _pos.x && pos.x + m_size.x >= _pos.x)
	{// x�����͈͓�
		if (pos.z - m_size.y <= _pos.z && pos.z + m_size.y >= _pos.z)
		{// Z�����͈͓�
			m_bInRange = true;
			CScene3D::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			return m_bInRange;
		}
	}

	if (m_bRelease)
	{// �N���b�N���ꂽ���N�������ɔ͈͊O�ɏo��
		CScene3D::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		CScene3D::SetSize(D3DXVECTOR3(m_size.x, 0.0f, m_size.y));
	}

	// �N�����Ă��Ȃ�
	if (!m_bSwitch) { CScene3D::SetColor(BUTTON_UNSELECTED_COLOR); }

	// �t���O�𕉂ɂ���
	m_bInRange = false;
	m_bRelease = false;
	return m_bInRange;
}

//=========================================
// �I���̐ݒ�
//=========================================
bool CButton3D::Click(void)
{
	if (!m_bInRange) { return false; }	// �͈͊O�������甲����

	CInputMouse *pMouse = CManager::GetInputMouse();	// �}�E�X�̓��͂��擾

	if (pMouse->GetTrigger(CInputMouse::DIMS_BUTTON_0))
	{// ���N���b�N���ꂽ
		m_bSwitch = m_bSwitch ? false : true;
		return true;
	}

	return false;
}

//=========================================
// �I���̐ݒ�
//=========================================
bool CButton3D::ClickRelease(void)
{
	if (!m_bInRange) { return false; }	// �͈͊O�������甲����

	CInputMouse *pMouse = CManager::GetInputMouse();	// �}�E�X�̓��͂��擾

	if (pMouse->GetTrigger(CInputMouse::DIMS_BUTTON_0))
	{// ���N���b�N���ꂽ
		m_bRelease = true;
		CScene3D::SetSize(CScene3D::GetSize() * 0.9f);
	}

	if (m_bRelease)
	{// �{�^����������Ă����
		CScene3D::SetColor(D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f));
	}

	if (pMouse->GetRelease(CInputMouse::DIMS_BUTTON_0) && m_bRelease)
	{// ���N���b�N�𗣂���
		m_bSwitch = m_bSwitch ? false : true;
		CScene3D::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		CScene3D::SetSize(D3DXVECTOR3(m_size.x, 0.0f, m_size.y));
		m_bRelease = false;
		return true;
	}

	return false;
}

/****************************************************************/
/*						�{�^���̐��̃N���X						*/
/****************************************************************/
//==================================
// ��������
//==================================
CButtonLine* CButtonLine::Create(D3DXVECTOR3 start)
{
	CButtonLine *pButtonLine;

	pButtonLine = new CButtonLine;	// ���������m��

	if (NULL != pButtonLine)
	{// �������m�ې���
		pButtonLine->Init(start);	// ����������
	}

	return pButtonLine;
}

//=========================================
// �R���X�g���N�^
//=========================================
CButtonLine::CButtonLine(int nPriority, CScene::OBJTYPE objType) : CScene2D(nPriority, objType)
{
	m_bLink = false;
}

//=========================================
// �f�X�g���N�^
//=========================================
CButtonLine::~CButtonLine()
{
}

//=========================================
// ����������
//=========================================
HRESULT CButtonLine::Init(D3DXVECTOR3 start)
{
	// �|���S���̐ݒ�
	CScene2D::Init();

	// �ʒu�̐ݒ�
	CScene2D::SetPos(start);

	m_start = start;

	return S_OK;
}

//=========================================
// �I������
//=========================================
void CButtonLine::Uninit(void)
{
	CScene2D::Uninit();
}

//=========================================
// �X�V����
//=========================================
void CButtonLine::Update(void)
{
	// �ڑ����Ă���Ƃ��͔�����
	if (m_bLink) { return; }

	// �}�E�X�J�[�\���̈ʒu���擾
	D3DXVECTOR3 cursor = CManager::GetGame()->GetMouseCursor()->GetPos();

	// �J�[�\���ʒu�ւ̊p�x���v�Z
	float fAngle = atan2f(cursor.x - m_start.x, cursor.y - m_start.y);

	// �J�n�ʒu����J�[�\���ʒu�ւ̋������v�Z
	float fLength = sqrtf(powf(cursor.x - m_start.x, 2.0f) + powf(cursor.y - m_start.y, 2.0f));

	D3DXVECTOR3 posVtx[4];
	posVtx[0] = D3DXVECTOR3(-BUTTON_LINE_THICK, 0.0f, 0.0f);
	posVtx[1] = D3DXVECTOR3(BUTTON_LINE_THICK, 0.0f, 0.0f);
	posVtx[2] = D3DXVECTOR3(-BUTTON_LINE_THICK, fLength, 0.0f);
	posVtx[3] = D3DXVECTOR3(BUTTON_LINE_THICK, fLength, 0.0f);

	// ���_���W�̐ݒ�
	CScene2D::SetPosVtx(posVtx);

	// �p�x�̐ݒ�
	CScene2D::SetRot(-fAngle);
}

//=========================================
// �`�揈��
//=========================================
void CButtonLine::Draw(void)
{
	CScene2D::Draw();
}

//=========================================
// �ڑ������Ƃ�
//=========================================
void CButtonLine::Link(D3DXVECTOR3 end)
{
	m_bLink = true;

	// �J�[�\���ʒu�ւ̊p�x���v�Z
	float fAngle = atan2f(end.x - m_start.x, end.y - m_start.y);

	// �J�n�ʒu����J�[�\���ʒu�ւ̋������v�Z
	float fLength = sqrtf(powf(end.x - m_start.x, 2.0f) + powf(end.y - m_start.y, 2.0f));

	D3DXVECTOR3 posVtx[4];
	posVtx[0] = D3DXVECTOR3(-BUTTON_LINE_THICK, 0.0f, 0.0f);
	posVtx[1] = D3DXVECTOR3(BUTTON_LINE_THICK, 0.0f, 0.0f);
	posVtx[2] = D3DXVECTOR3(-BUTTON_LINE_THICK, fLength, 0.0f);
	posVtx[3] = D3DXVECTOR3(BUTTON_LINE_THICK, fLength, 0.0f);

	// ���_���W�̐ݒ�
	CScene2D::SetPosVtx(posVtx);

	// �p�x�̐ݒ�
	CScene2D::SetRot(-fAngle);
}

/****************************************************************/
/*						�{�^���Ǘ��N���X						*/
/****************************************************************/
//==================================
// ��������
//==================================
CButtonManagerStrategy* CButtonManagerStrategy::Create(void)
{
	CButtonManagerStrategy *pButton;

	pButton = new CButtonManagerStrategy;	// ���������m��

	if (NULL != pButton)
	{// �������m�ې���
		pButton->Init();	// ����������
	}

	return pButton;
}

//=========================================
// �R���X�g���N�^
//=========================================
CButtonManagerStrategy::CButtonManagerStrategy(int nPriority, CScene::OBJTYPE objType) : CScene(nPriority, objType)
{
	for (int nCntAI = 0; nCntAI < 2; nCntAI++)
	{
		m_pAIType[nCntAI] = NULL;
	}
	m_pFirst = NULL;
	m_pSecond = NULL;
	m_pThird = NULL;
	m_pFourth = NULL;
	m_nSelectAIType = -1;
	m_bFinish = false;
	m_bSelect = true;
	m_pMarker = NULL;
	m_nSetNumMarker = 0;

	for (int nCntHierarchy = 0; nCntHierarchy < 4; nCntHierarchy++)
	{
		m_aNumLogic[nCntHierarchy] = 0;
		m_aSelectLogic[nCntHierarchy] = -1;
		m_pLine[nCntHierarchy] = NULL;
		m_apSelectIcon[nCntHierarchy] = NULL;
	}

	m_bDisp = false;
}

//=========================================
// �f�X�g���N�^
//=========================================
CButtonManagerStrategy::~CButtonManagerStrategy()
{
}

//=========================================
// ����������
//=========================================
HRESULT CButtonManagerStrategy::Init(void)
{
	// �h���[���ƃE�H�[�J�[�̃{�^���̐���
	m_pAIType[0] = CButton2D::Create(D3DXVECTOR3(405.0f, 370.0f, 0.0f), 150.0f, 150.0f);
	m_pAIType[1] = CButton2D::Create(D3DXVECTOR3(405.0f, 570.0f, 0.0f), 150.0f, 150.0f);

	m_pAIType[0]->SetColor(BUTTON_UNSELECTED_COLOR);
	m_pAIType[1]->SetColor(BUTTON_UNSELECTED_COLOR);

	m_pAIType[0]->BindTexture(CTexture::GetTexture(CTexture::TEXTURE_DRONE_BUTTON));
	m_pAIType[1]->BindTexture(CTexture::GetTexture(CTexture::TEXTURE_WORKER_BUTTON));

	return S_OK;
}

//=========================================
// �I������
//=========================================
void CButtonManagerStrategy::Uninit(void)
{
	if (m_pFirst != NULL)
	{// �P�Ԗڂ̊K�w�̔j��
		for (int nCntLogic = 0; nCntLogic < m_aNumLogic[0]; nCntLogic++)
		{// �g���Ă��鐔�����[�v
			if (m_pFirst[nCntLogic] != NULL)
			{// �j��
				m_pFirst[nCntLogic]->Uninit();
				m_pFirst[nCntLogic] = NULL;
			}
		}

		// �j��
		delete[] m_pFirst;
		m_pFirst = NULL;
	}

	if (m_pSecond != NULL)
	{// �Q�Ԗڂ̊K�w�̔j��
		for (int nCntLogic = 0; nCntLogic < m_aNumLogic[1]; nCntLogic++)
		{// �g���Ă��鐔�����[�v
			if (m_pSecond[nCntLogic] != NULL)
			{// �j��
				m_pSecond[nCntLogic]->Uninit();
				m_pSecond[nCntLogic] = NULL;
			}
		}

		// �j��
		delete[] m_pSecond;
		m_pSecond = NULL;
	}

	if (m_pThird != NULL)
	{// �R�Ԗڂ̊K�w�̔j��
		for (int nCntLogic = 0; nCntLogic < m_aNumLogic[2]; nCntLogic++)
		{// �g���Ă��鐔�����[�v
			if (m_pThird[nCntLogic] != NULL)
			{// �j��
				m_pThird[nCntLogic]->Uninit();
				m_pThird[nCntLogic] = NULL;
			}
		}

		// �j��
		delete[] m_pThird;
		m_pThird = NULL;
	}

	if (m_pFourth != NULL)
	{// �S�Ԗڂ̊K�w�̔j��
		for (int nCntLogic = 0; nCntLogic < m_aNumLogic[3]; nCntLogic++)
		{// �g���Ă��鐔�����[�v
			if (m_pFourth[nCntLogic] != NULL)
			{// �j��
				m_pFourth[nCntLogic]->Uninit();
				m_pFourth[nCntLogic] = NULL;
			}
		}

		// �j��
		delete[] m_pFourth;
		m_pFourth = NULL;
	}

	for (int nCntAI = 0; nCntAI < 2; nCntAI++)
	{// AI�@�̂̎�ޕ����[�v
		if (m_pAIType[nCntAI] != NULL)
		{// �{�^���̔j��
			m_pAIType[nCntAI]->Uninit();
			m_pAIType[nCntAI] = NULL;
		}
	}

	for (int nCntHierarchy = 0; nCntHierarchy < 4; nCntHierarchy++)
	{// �K�w�����[�v
		if (m_pLine[nCntHierarchy] != NULL)
		{// ���̔j��
			m_pLine[nCntHierarchy]->Uninit();
			m_pLine[nCntHierarchy] = NULL;
		}

		if (m_apSelectIcon[nCntHierarchy] != NULL)
		{// �I���������ڂ̔j��
			m_apSelectIcon[nCntHierarchy]->Uninit();
			m_apSelectIcon[nCntHierarchy] = NULL;
		}
	}

	if (m_pMarker != NULL)
	{
		int nNumMarker;		// �}�[�J�[�̐�
		if (2 == m_aSelectLogic[2]) { nNumMarker = 4; }
		else { nNumMarker = 1; }

		for (int nCntMarker = 0; nCntMarker < nNumMarker; nCntMarker++)
		{// �}�[�J�[�̐������[�v
			if (m_pMarker[nCntMarker] != NULL)
			{// �}�[�J�[�̔j��
				m_pMarker[nCntMarker]->Uninit();
				m_pMarker[nCntMarker] = NULL;
			}
		}

		// �}�[�J�[�̔j��
		delete[] m_pMarker;
		m_pMarker = NULL;
	}

	// ������j��
	CScene::Release();
}

//=========================================
// �X�V����
//=========================================
void CButtonManagerStrategy::Update(void)
{
	if (m_bSelect)
	{
		// �h���[�����E�H�[�J�[�̑I������
		AITypeSelect();

		if (NULL != m_pFirst)
		{// �P�K�w�ڂ̏���
			FirstHierarchy();
		}

		if (NULL != m_pSecond)
		{// �Q�K�w�ڂ̏���
			SecondHierarchy();
		}

		if (NULL != m_pThird)
		{// �R�K�w�ڂ̏���
			ThirdHierarchy();
		}

		if (NULL != m_pFourth)
		{// �S�K�w�ڂ̏���
			FourthHierarchy();
		}
	}

	if (m_bFinish && NULL == m_pMarker)
	{// �}�[�J�[�̐���
		int nNumMarker = 0;		// �}�[�J�[�̐�
		if (2 == m_aSelectLogic[2]) { nNumMarker = 4; }
		else { nNumMarker = 1; }

		// �}�[�J�[��K�v�ȕ�����
		m_pMarker = new CScene3D*[nNumMarker];
		for (int nCntMarker = 0; nCntMarker < nNumMarker; nCntMarker++)
		{
			m_pMarker[nCntMarker] = CScene3D::Create();
			m_pMarker[nCntMarker]->SetBoolLighting(false);
		}
	}

	if (NULL != m_pMarker)
	{// �}�[�J�[�̐ݒu
		int nNumMarker;		// �}�[�J�[�̐�
		if (2 == m_aSelectLogic[2]) { nNumMarker = 4; }
		else { nNumMarker = 1; }

		for (int nCntMarker = 0; nCntMarker < nNumMarker; nCntMarker++)
		{// �Y�[���{���ɉ����ăT�C�Y�ύX
			m_pMarker[nCntMarker]->SetSize(D3DXVECTOR3(40.0f, 0.0f, 40.0f) / CManager::GetCamera()->GetZoom());
		}

		CInputMouse *pMouse = CManager::GetInputMouse();

		if (pMouse->GetTrigger(CInputMouse::DIMS_BUTTON_0))
		{// ���N���b�N���ꂽ

			if (nNumMarker > m_nSetNumMarker)
			{// �}�[�J�[��u��
				m_pMarker[m_nSetNumMarker]->SetPos(CManager::GetGame()->GetMouse()->GetPos());

				m_nSetNumMarker++;
			}
			else
			{
				m_pMarker[m_nSetNumMarker - 1]->SetPos(CManager::GetGame()->GetMouse()->GetPos());
			}
		}
	}

	CDebugProc::Print("�I���{�^���F%d %d %d %d", m_aSelectLogic[0], m_aSelectLogic[1], m_aSelectLogic[2], m_aSelectLogic[3]);
}

//=========================================
// �`�揈��
//=========================================
void CButtonManagerStrategy::Draw(void)
{
}

//=========================================
// �}�[�J�[�̕`�揈��
//=========================================
void CButtonManagerStrategy::MarkerDraw(void)
{
	for (int nCntMarker = 0; nCntMarker < m_nSetNumMarker; nCntMarker++)
	{
		m_pMarker[nCntMarker]->Draw();
	}
}

//=========================================
// �h���[�����E�H�[�J�[�̑I������
//=========================================
void CButtonManagerStrategy::AITypeSelect(void)
{
	for (int nCntAIType = 0; nCntAIType < 2; nCntAIType++)
	{// �h���[���ƃE�H�[�J�[�𗼕��`�F�b�N
		if (m_pAIType[nCntAIType]->InRange(CManager::GetGame()->GetMouseCursor()->GetPos()))
		{// �͈͓����`�F�b�N
			if (m_pAIType[nCntAIType]->ClickRelease())
			{// �N���b�N���ꂽ
				ButtonUninit(4);	// �{�^���̔j��

				if (m_pAIType[nCntAIType]->GetSwitch())
				{// �N����
					m_pAIType[(nCntAIType + 1) % 2]->SetSwitch(false);

					m_nSelectAIType = nCntAIType;

					m_aNumLogic[0] = 2;		// �P�K�w�ڂ̃{�^���̐�
					m_pFirst = new CButton2D*[m_aNumLogic[0]];
					for (int nCntButton = 0; nCntButton < m_aNumLogic[0]; nCntButton++)
					{// �{�^���̐���
						float posY = (480.0f + (nCntButton * BUTTON_LOGIC_HEIGHT * (5 - m_aNumLogic[0]))) - (BUTTON_LOGIC_HEIGHT * (5 - m_aNumLogic[0]) * (m_aNumLogic[0] / 2.0f)) + BUTTON_LOGIC_HEIGHT * (5 - m_aNumLogic[0]) / 2.0f;
						m_pFirst[nCntButton] = CButton2D::Create(D3DXVECTOR3(580.0f, posY, 0.0f), BUTTON_LOGIC_WIDTH, BUTTON_LOGIC_HEIGHT);
						m_pFirst[nCntButton]->BindTexture(CTexture::GetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_STRATEGY_ICON)));
					}

					m_pFirst[0]->SetTex(0, 1, 17);
					m_pFirst[1]->SetTex(10, 1, 17);

					// ���̐���
					if (NULL == m_pLine[0]) { m_pLine[0] = CButtonLine::Create(m_pAIType[nCntAIType]->GetPos() + D3DXVECTOR3(m_pAIType[m_nSelectAIType]->GetWidth() * 0.5f, 0.0f, 0.0f)); }
				}
				else
				{// ��~��
					m_nSelectAIType = -1;
				}
			}
		}
	}
}

//=========================================
// �P�K�w�ڂ̏���
//=========================================
void CButtonManagerStrategy::FirstHierarchy(void)
{
	for (int nCntFirst = 0; nCntFirst < m_aNumLogic[0]; nCntFirst++)
	{
		if (m_pFirst[nCntFirst]->InRange(CManager::GetGame()->GetMouseCursor()->GetPos()))
		{// �͈͓����`�F�b�N
			if (m_pFirst[nCntFirst]->ClickRelease())
			{// �N���b�N���ꂽ
				ButtonUninit(3);	// �{�^���̔j��

				if (m_pFirst[nCntFirst]->GetSwitch())
				{// �N����
					m_aSelectLogic[0] = nCntFirst;
					m_pLine[0]->Link(m_pFirst[nCntFirst]->GetPos() + D3DXVECTOR3(-m_pFirst[nCntFirst]->GetWidth() * 0.5f, 0.0f, 0.0f));

					if (0 == nCntFirst)
					{// 0�Ԗڂ̃{�^��
						m_aNumLogic[1] = 2;		// �Q�K�w�ڂ̃{�^���̐�
						m_pSecond = new CButton2D*[m_aNumLogic[1]];
						for (int nCntButton = 0; nCntButton < m_aNumLogic[1]; nCntButton++)
						{// �{�^���̐���
							float posY = (480.0f + (nCntButton * BUTTON_LOGIC_HEIGHT * (5 - m_aNumLogic[1]))) - (BUTTON_LOGIC_HEIGHT * (5 - m_aNumLogic[1]) * (m_aNumLogic[1] / 2.0f)) + BUTTON_LOGIC_HEIGHT * (5 - m_aNumLogic[1]) / 2.0f;
							m_pSecond[nCntButton] = CButton2D::Create(D3DXVECTOR3(786.0f, posY, 0.0f), BUTTON_LOGIC_WIDTH, BUTTON_LOGIC_HEIGHT);
							m_pSecond[nCntButton]->BindTexture(CTexture::GetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_STRATEGY_ICON)));
							m_pSecond[nCntButton]->SetTex(nCntButton + 1, 1, 17);
						}
					}
					else
					{// 1�Ԗڂ̃{�^��
						m_aNumLogic[1] = 2;		// �Q�K�w�ڂ̃{�^���̐�
						m_pSecond = new CButton2D*[m_aNumLogic[1]];
						for (int nCntButton = 0; nCntButton < m_aNumLogic[1]; nCntButton++)
						{// �{�^���̐���
							float posY = (480.0f + (nCntButton * BUTTON_LOGIC_HEIGHT * (5 - m_aNumLogic[1]))) - (BUTTON_LOGIC_HEIGHT * (5 - m_aNumLogic[1]) * (m_aNumLogic[1] / 2.0f)) + BUTTON_LOGIC_HEIGHT * (5 - m_aNumLogic[1]) / 2.0f;
							m_pSecond[nCntButton] = CButton2D::Create(D3DXVECTOR3(890.0f, posY, 0.0f), BUTTON_LOGIC_WIDTH, BUTTON_LOGIC_HEIGHT);
							m_pSecond[nCntButton]->BindTexture(CTexture::GetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_STRATEGY_ICON)));
							m_pSecond[nCntButton]->SetTex(nCntButton + 11, 1, 17);
						}
					}

					// ���̐���
					if (NULL == m_pLine[1]) { m_pLine[1] = CButtonLine::Create(m_pFirst[m_aSelectLogic[0]]->GetPos() + D3DXVECTOR3(m_pFirst[m_aSelectLogic[0]]->GetWidth() * 0.5f, 0.0f, 0.0f)); }

					if(NULL == m_apSelectIcon[0])
					{// �I���������ڂ̐���
						m_apSelectIcon[0] = CScene2D::Create();
						m_apSelectIcon[0]->SwapPriority(6);
						m_apSelectIcon[0]->SetPos(D3DXVECTOR3(m_pFirst[0]->GetPos().x, 190.0f, 0.0f));
						m_apSelectIcon[0]->SetSize(BUTTON_LOGIC_WIDTH, BUTTON_LOGIC_HEIGHT);
						m_apSelectIcon[0]->BindTexture(CTexture::GetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_STRATEGY_ICON)));
						m_apSelectIcon[0]->SetTex(m_aSelectLogic[0] * 10, 1, 17);
					}
					else
					{
						m_apSelectIcon[0]->SetTex(m_aSelectLogic[0] * 10, 1, 17);
					}
				}
				else
				{// ��~��
					m_aSelectLogic[0] = -1;
				}
			}
		}
	}

	for (int nCntFirst = 0; nCntFirst < m_aNumLogic[0]; nCntFirst++)
	{// �P�K�w�ڂ̃{�^���̐������[�v
		if (nCntFirst != m_aSelectLogic[0])
		{// �I�𒆈ȊO�̃{�^�����~��Ԃɂ���
			m_pFirst[nCntFirst]->SetSwitch(false);
		}
	}
}

//=========================================
// �Q�K�w�ڂ̏���
//=========================================
void CButtonManagerStrategy::SecondHierarchy(void)
{
	for (int nCntSecond = 0; nCntSecond < m_aNumLogic[1]; nCntSecond++)
	{
		if (m_pSecond[nCntSecond]->InRange(CManager::GetGame()->GetMouseCursor()->GetPos()))
		{// �͈͓����`�F�b�N
			if (m_pSecond[nCntSecond]->ClickRelease())
			{// �N���b�N���ꂽ
				ButtonUninit(2);	// �{�^���̔j��

				if (m_pSecond[nCntSecond]->GetSwitch())
				{// �N����
					m_aSelectLogic[1] = nCntSecond;
					m_pLine[1]->Link(m_pSecond[nCntSecond]->GetPos() + D3DXVECTOR3(-m_pSecond[nCntSecond]->GetWidth() * 0.5f, 0.0f, 0.0f));

					if (0 == m_aSelectLogic[0])
					{// �ړ����W�b�N�c���[
						if (0 == nCntSecond)
						{// 0�Ԗڂ̃{�^��
							m_aNumLogic[2] = 3;		// �R�K�w�ڂ̃{�^���̐�
							m_pThird = new CButton2D*[m_aNumLogic[2]];
							for (int nCntButton = 0; nCntButton < m_aNumLogic[2]; nCntButton++)
							{// �{�^���̐���
								float posY = (480.0f + (nCntButton * BUTTON_LOGIC_HEIGHT * (5 - m_aNumLogic[2]))) - (BUTTON_LOGIC_HEIGHT * (5 - m_aNumLogic[2]) * (m_aNumLogic[2] / 2.0f)) + BUTTON_LOGIC_HEIGHT * (5 - m_aNumLogic[2]) / 2.0f;
								m_pThird[nCntButton] = CButton2D::Create(D3DXVECTOR3(992.0f, posY, 0.0f), BUTTON_LOGIC_WIDTH, BUTTON_LOGIC_HEIGHT);
								m_pThird[nCntButton]->BindTexture(CTexture::GetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_STRATEGY_ICON)));
								m_pThird[nCntButton]->SetTex(nCntButton + 3, 1, 17);
							}
						}
						else
						{// 1�Ԗڂ̃{�^��
							m_aNumLogic[2] = 2;		// �R�K�w�ڂ̃{�^���̐�
							m_pThird = new CButton2D*[m_aNumLogic[2]];
							for (int nCntButton = 0; nCntButton < m_aNumLogic[2]; nCntButton++)
							{// �{�^���̐���
								float posY = (480.0f + (nCntButton * BUTTON_LOGIC_HEIGHT * (5 - m_aNumLogic[2]))) - (BUTTON_LOGIC_HEIGHT * (5 - m_aNumLogic[2]) * (m_aNumLogic[2] / 2.0f)) + BUTTON_LOGIC_HEIGHT * (5 - m_aNumLogic[2]) / 2.0f;
								m_pThird[nCntButton] = CButton2D::Create(D3DXVECTOR3(992.0f, posY, 0.0f), BUTTON_LOGIC_WIDTH, BUTTON_LOGIC_HEIGHT);
								m_pThird[nCntButton]->BindTexture(CTexture::GetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_STRATEGY_ICON)));
								m_pThird[nCntButton]->SetTex(nCntButton + 6, 1, 17);
							}
						}
					}
					else if (1 == m_aSelectLogic[0])
					{// �ҋ@���W�b�N�c���[
						m_aNumLogic[2] = 2;		// �R�K�w�ڂ̃{�^���̐�
						m_pThird = new CButton2D*[m_aNumLogic[2]];
						for (int nCntButton = 0; nCntButton < m_aNumLogic[2]; nCntButton++)
						{// �{�^���̐���
							float posY = (480.0f + (nCntButton * BUTTON_LOGIC_HEIGHT * (5 - m_aNumLogic[2]))) - (BUTTON_LOGIC_HEIGHT * (5 - m_aNumLogic[2]) * (m_aNumLogic[2] / 2.0f)) + BUTTON_LOGIC_HEIGHT * (5 - m_aNumLogic[2]) / 2.0f;
							m_pThird[nCntButton] = CButton2D::Create(D3DXVECTOR3(1200.0f, posY, 0.0f), BUTTON_LOGIC_WIDTH, BUTTON_LOGIC_HEIGHT);
							m_pThird[nCntButton]->BindTexture(CTexture::GetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_STRATEGY_ICON)));
							m_pThird[nCntButton]->SetTex(nCntButton + 13 + (m_aSelectLogic[1] * 2), 1, 17);
						}
					}

					// ���̐���
					if (NULL == m_pLine[2]) { m_pLine[2] = CButtonLine::Create(m_pSecond[m_aSelectLogic[1]]->GetPos() + D3DXVECTOR3(m_pSecond[m_aSelectLogic[1]]->GetWidth() * 0.5f, 0.0f, 0.0f)); }

					if (NULL == m_apSelectIcon[1])
					{// �I���������ڂ̐���
						m_apSelectIcon[1] = CScene2D::Create();
						m_apSelectIcon[1]->SwapPriority(6);
						m_apSelectIcon[1]->SetPos(D3DXVECTOR3(m_pSecond[0]->GetPos().x, 190.0f, 0.0f));
						m_apSelectIcon[1]->SetSize(BUTTON_LOGIC_WIDTH, BUTTON_LOGIC_HEIGHT);
						m_apSelectIcon[1]->BindTexture(CTexture::GetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_STRATEGY_ICON)));
						m_apSelectIcon[1]->SetTex(m_aSelectLogic[1] + (m_aSelectLogic[0] * 10) + 1, 1, 17);
					}
					else
					{
						m_apSelectIcon[1]->SetTex(m_aSelectLogic[1] + (m_aSelectLogic[0] * 10) + 1, 1, 17);
					}
				}
				else
				{// ��~��
					m_aSelectLogic[1] = -1;
				}
			}
		}
	}

	for (int nCntSecond = 0; nCntSecond < m_aNumLogic[1]; nCntSecond++)
	{// �Q�K�w�ڂ̃{�^���̐������[�v
		if (nCntSecond != m_aSelectLogic[1])
		{// �I�𒆈ȊO�̃{�^�����~��Ԃɂ���
			m_pSecond[nCntSecond]->SetSwitch(false);
		}
	}
}

//=========================================
// �R�K�w�ڂ̏���
//=========================================
void CButtonManagerStrategy::ThirdHierarchy(void)
{
	if (-1 == m_aSelectLogic[1]) { return; }	// ��O�̊K�w���I������Ă��Ȃ��ꍇ

	for (int nCntThird = 0; nCntThird < m_aNumLogic[2]; nCntThird++)
	{
		if (m_pThird[nCntThird]->InRange(CManager::GetGame()->GetMouseCursor()->GetPos()))
		{// �͈͓����`�F�b�N
			if (m_pThird[nCntThird]->ClickRelease())
			{// �N���b�N���ꂽ
				ButtonUninit(1);	// �{�^���̔j��

				if (m_pThird[nCntThird]->GetSwitch())
				{// �N����
					m_aSelectLogic[2] = nCntThird;
					m_pLine[2]->Link(m_pThird[nCntThird]->GetPos() + D3DXVECTOR3(-m_pThird[nCntThird]->GetWidth() * 0.5f, 0.0f, 0.0f));

					if (NULL == m_apSelectIcon[2])
					{// �I���������ڂ̐���
						m_apSelectIcon[2] = CScene2D::Create();
						m_apSelectIcon[2]->SwapPriority(6);
						m_apSelectIcon[2]->SetPos(D3DXVECTOR3(m_pThird[0]->GetPos().x, 190.0f, 0.0f));
						m_apSelectIcon[2]->SetSize(BUTTON_LOGIC_WIDTH, BUTTON_LOGIC_HEIGHT);
						m_apSelectIcon[2]->BindTexture(CTexture::GetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_STRATEGY_ICON)));
						if (0 == m_aSelectLogic[0])
						{// �ړ�
							m_apSelectIcon[2]->SetTex(m_aSelectLogic[2] + (3 * (m_aSelectLogic[1] + 1)), 1, 17);
						}
						else
						{// �ҋ@
							m_apSelectIcon[2]->SetTex(m_aSelectLogic[2] + 13 + (m_aSelectLogic[1] * 2), 1, 17);
						}
					}
					else
					{
						m_apSelectIcon[2]->SetTex(m_aSelectLogic[2] + (m_aSelectLogic[0] * 10) + (3 * (m_aSelectLogic[1] + 1)), 1, 17);
					}

					if (0 == m_aSelectLogic[0])
					{// �ړ����W�b�N�c���[
						m_aNumLogic[3] = 2;		// �S�K�w�ڂ̃{�^���̐�
						m_pFourth = new CButton2D*[m_aNumLogic[3]];
						for (int nCntButton = 0; nCntButton < m_aNumLogic[3]; nCntButton++)
						{// �{�^���̐���
							float posY = (480.0f + (nCntButton * BUTTON_LOGIC_HEIGHT * (5 - m_aNumLogic[3]))) - (BUTTON_LOGIC_HEIGHT * (5 - m_aNumLogic[3]) * (m_aNumLogic[3] / 2.0f)) + BUTTON_LOGIC_HEIGHT * (5 - m_aNumLogic[3]) / 2.0f;
							m_pFourth[nCntButton] = CButton2D::Create(D3DXVECTOR3(1200.0f, posY, 0.0f), BUTTON_LOGIC_WIDTH, BUTTON_LOGIC_HEIGHT);
							m_pFourth[nCntButton]->BindTexture(CTexture::GetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_STRATEGY_ICON)));
							m_pFourth[nCntButton]->SetTex(nCntButton + 8, 1, 17);
						}

						// ���̐���
						if (NULL == m_pLine[3]) { m_pLine[3] = CButtonLine::Create(m_pThird[m_aSelectLogic[2]]->GetPos() + D3DXVECTOR3(m_pThird[m_aSelectLogic[2]]->GetWidth() * 0.5f, 0.0f, 0.0f)); }
					}
					else
					{
						m_bFinish = true;
						m_bSelect = false;
						ButtonUninit(4);	// �{�^���̔j��
					}
				}
				else
				{// ��~��
					m_aSelectLogic[2] = -1;
				}
			}
		}
	}

	for (int nCntThird = 0; nCntThird < m_aNumLogic[2]; nCntThird++)
	{// �R�K�w�ڂ̃{�^���̐������[�v
		if (nCntThird != m_aSelectLogic[2])
		{// �I�𒆈ȊO�̃{�^�����~��Ԃɂ���
			m_pThird[nCntThird]->SetSwitch(false);
		}
	}
}

//=========================================
// �S�K�w�ڂ̏���
//=========================================
void CButtonManagerStrategy::FourthHierarchy(void)
{
	if (-1 == m_aSelectLogic[2]) { return; }	// ��O�̊K�w���I������Ă��Ȃ��ꍇ

	for (int nCntFourth = 0; nCntFourth < m_aNumLogic[3]; nCntFourth++)
	{
		if (m_pFourth[nCntFourth]->InRange(CManager::GetGame()->GetMouseCursor()->GetPos()))
		{// �͈͓����`�F�b�N
			if (m_pFourth[nCntFourth]->ClickRelease())
			{// �N���b�N���ꂽ
				m_aSelectLogic[3] = nCntFourth;
				m_pLine[3]->Link(m_pFourth[nCntFourth]->GetPos() + D3DXVECTOR3(-m_pFourth[nCntFourth]->GetWidth() * 0.5f, 0.0f, 0.0f));

				if (NULL == m_apSelectIcon[3])
				{// �I���������ڂ̐���
					m_apSelectIcon[3] = CScene2D::Create();
					m_apSelectIcon[3]->SwapPriority(6);
					m_apSelectIcon[3]->SetPos(D3DXVECTOR3(m_pFourth[0]->GetPos().x, 190.0f, 0.0f));
					m_apSelectIcon[3]->SetSize(BUTTON_LOGIC_WIDTH, BUTTON_LOGIC_HEIGHT);
					m_apSelectIcon[3]->BindTexture(CTexture::GetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_STRATEGY_ICON)));
					m_apSelectIcon[3]->SetTex(m_aSelectLogic[3] + 8, 1, 17);
					m_bFinish = true;
					m_bSelect = false;
					ButtonUninit(4);	// �{�^���̔j��
				}
			}
		}
	}

	for (int nCntFourth = 0; nCntFourth < m_aNumLogic[3]; nCntFourth++)
	{// �S�K�w�ڂ̃{�^���̐������[�v
		if (nCntFourth != m_aSelectLogic[3])
		{// �I�𒆈ȊO�̃{�^�����~��Ԃɂ���
			m_pFourth[nCntFourth]->SetSwitch(false);
		}
	}
}

//=========================================
// �}�[�J�[�̐ݒu
//=========================================
void CButtonManagerStrategy::Marker(void)
{

}

//=========================================
// �{�^���̍폜
//=========================================
void CButtonManagerStrategy::ButtonUninit(int nLogic)
{
	if (m_pFirst != NULL && 4 == nLogic)
	{// �P�Ԗڂ̊K�w�̔j��
		for (int nCntLogic = 0; nCntLogic < m_aNumLogic[0]; nCntLogic++)
		{// �g���Ă��鐔�����[�v
			if (m_pFirst[nCntLogic] != NULL)
			{// �j��
				m_pFirst[nCntLogic]->Uninit();
				m_pFirst[nCntLogic] = NULL;
			}
		}

		// �j��
		delete[] m_pFirst;
		m_pFirst = NULL;
	}

	if (m_pSecond != NULL && 3 <= nLogic)
	{// �Q�Ԗڂ̊K�w�̔j��
		for (int nCntLogic = 0; nCntLogic < m_aNumLogic[1]; nCntLogic++)
		{// �g���Ă��鐔�����[�v
			if (m_pSecond[nCntLogic] != NULL)
			{// �j��
				m_pSecond[nCntLogic]->Uninit();
				m_pSecond[nCntLogic] = NULL;
			}
		}

		// �j��
		delete[] m_pSecond;
		m_pSecond = NULL;
	}

	if (m_pThird != NULL && 2 <= nLogic)
	{// �R�Ԗڂ̊K�w�̔j��
		for (int nCntLogic = 0; nCntLogic < m_aNumLogic[2]; nCntLogic++)
		{// �g���Ă��鐔�����[�v
			if (m_pThird[nCntLogic] != NULL)
			{// �j��
				m_pThird[nCntLogic]->Uninit();
				m_pThird[nCntLogic] = NULL;
			}
		}

		// �j��
		delete[] m_pThird;
		m_pThird = NULL;
	}

	if (m_pFourth != NULL && 1 <= nLogic)
	{// �S�Ԗڂ̊K�w�̔j��
		for (int nCntLogic = 0; nCntLogic < m_aNumLogic[3]; nCntLogic++)
		{// �g���Ă��鐔�����[�v
			if (m_pFourth[nCntLogic] != NULL)
			{// �j��
				m_pFourth[nCntLogic]->Uninit();
				m_pFourth[nCntLogic] = NULL;
			}
		}

		// �j��
		delete[] m_pFourth;
		m_pFourth = NULL;
	}

	for (int nCntButton = 4 - nLogic; nCntButton < 4; nCntButton++)
	{// �{�^���̐���������
		m_aNumLogic[nCntButton] = 0;
		if (!m_bFinish)
		{
			m_aSelectLogic[nCntButton] = -1;
		}

		if (1 <= nCntButton && m_pLine[nCntButton - 1] != NULL)
		{// �Ȃ����Ă�����𗣂�
			m_pLine[nCntButton - 1]->SetBoolLink(false);
		}

		if (m_pLine[nCntButton] != NULL)
		{// ���̔j��
			m_pLine[nCntButton]->Uninit();
			m_pLine[nCntButton] = NULL;
		}

		if (!m_bFinish)
		{// �I�����I����ĂȂ��Ƃ�
			if (m_apSelectIcon[nCntButton] != NULL)
			{// �I���������ڂ̔j��
				m_apSelectIcon[nCntButton]->Uninit();
				m_apSelectIcon[nCntButton] = NULL;
			}
		}
	}
}