//=============================================================================
//
// �{�^������ [button.cpp]
// Author : Ishida Takuto
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

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define BUTTON_UNSELECTED_COLOR		(D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f))		// ��I�����̐F
#define BUTTON_LOGIC_WIDTH			(35.0f)									// ���W�b�N�c���[�{�^���̕�
#define BUTTON_LOGIC_HEIGHT			(35.0f)									// ���W�b�N�c���[�{�^���̍���
#define BUTTON_LINE_THICK			(2.5f)									// ���̑���

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
CButtonLine::CButtonLine(int nPriority, CScene::OBJTYPE objType) : CScene3D(nPriority, objType)
{
	m_start = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
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
	CScene3D::Init();

	// �ʒu�̐ݒ�
	CScene3D::SetPos(start);
	m_start = start;

	return S_OK;
}

//=========================================
// �I������
//=========================================
void CButtonLine::Uninit(void)
{
	CScene3D::Uninit();
}

//=========================================
// �X�V����
//=========================================
void CButtonLine::Update(void)
{
	if (m_bLink) { return; }	// �ڑ����Ă���Ƃ��͔�����

	D3DXVECTOR3 cursor = CManager::GetGame()->GetMouseCursor()->GetPos();

	// �J�[�\���ʒu�ւ̊p�x���v�Z
	float fAngle = atan2f(cursor.x - m_start.x, cursor.z - m_start.z);

	// �J�n�ʒu����J�[�\���ʒu�ւ̋������v�Z
	float fLength = sqrtf(powf(cursor.x - m_start.x, 2.0f) + powf(cursor.z - m_start.z, 2.0f));

	/*						*/
	/*		�����̐ݒ�		*/
	/*						*/
	// ���_���̎擾
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = CScene3D::GetVtxBuff();

	// ���_����ݒ�
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos = D3DXVECTOR3(-BUTTON_LINE_THICK, 0.0f, fLength);
	pVtx[1].pos = D3DXVECTOR3(BUTTON_LINE_THICK, 0.0f, fLength);
	pVtx[2].pos = D3DXVECTOR3(-BUTTON_LINE_THICK, 0.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(BUTTON_LINE_THICK, 0.0f, 0.0f);

	//���_�o�b�t�@���A�����b�N
	pVtxBuff->Unlock();

	// ���_���̐ݒ�
	CScene3D::SetVtxBuff(pVtxBuff);

	// �p�x�̐ݒ�
	CScene3D::SetRot(D3DXVECTOR3(0.0f, fAngle, 0.0f));
}

//=========================================
// �`�揈��
//=========================================
void CButtonLine::Draw(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// ���C�e�B���OOFF
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// Z�e�X�gOFF
	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

	CScene3D::Draw();

	// Z�e�X�gON
	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

	// ���C�e�B���OON
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//=========================================
// �ڑ������Ƃ�
//=========================================
void CButtonLine::Link(D3DXVECTOR3 end)
{
	m_bLink = true;

	// �J�[�\���ʒu�ւ̊p�x���v�Z
	float fAngle = atan2f(end.x - m_start.x, end.z - m_start.z);

	// �J�n�ʒu����J�[�\���ʒu�ւ̋������v�Z
	float fLength = sqrtf(powf(end.x - m_start.x, 2.0f) + powf(end.z - m_start.z, 2.0f));

	/*						*/
	/*		�����̐ݒ�		*/
	/*						*/
	// ���_���̎擾
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = CScene3D::GetVtxBuff();

	// ���_����ݒ�
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos = D3DXVECTOR3(-BUTTON_LINE_THICK, 0.0f, fLength);
	pVtx[1].pos = D3DXVECTOR3(BUTTON_LINE_THICK, 0.0f, fLength);
	pVtx[2].pos = D3DXVECTOR3(-BUTTON_LINE_THICK, 0.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(BUTTON_LINE_THICK, 0.0f, 0.0f);

	//���_�o�b�t�@���A�����b�N
	pVtxBuff->Unlock();

	// ���_���̐ݒ�
	CScene3D::SetVtxBuff(pVtxBuff);

	// �p�x�̐ݒ�
	CScene3D::SetRot(D3DXVECTOR3(0.0f, fAngle, 0.0f));

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

	for (int nCntHierarchy = 0; nCntHierarchy < 4; nCntHierarchy++)
	{
		m_aNumLogic[nCntHierarchy] = 0;
		m_aSelectLogic[nCntHierarchy] = -1;
		m_pLine[nCntHierarchy] = NULL;
		m_apSelectIcon[nCntHierarchy] = NULL;
	}

#ifdef _DEBUG
	m_bDisp = true;
#endif
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
	m_pAIType[0] = CButton3D::Create(D3DXVECTOR3(-235.0f, 0.0f, 0.0f), 80.0f, 80.0f);
	m_pAIType[1] = CButton3D::Create(D3DXVECTOR3(-235.0f, 0.0f, -230.0f), 80.0f, 80.0f);

	m_pAIType[0]->SetColor(BUTTON_UNSELECTED_COLOR);
	m_pAIType[1]->SetColor(BUTTON_UNSELECTED_COLOR);

	m_pAIType[0]->BindTexture(CTexture::GetTexture(CTexture::TEXTURE_WORKER_BUTTON));
	m_pAIType[1]->BindTexture(CTexture::GetTexture(CTexture::TEXTURE_DRONE_BUTTON));

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

	// ������j��
	CScene::Release();
}

//=========================================
// �X�V����
//=========================================
void CButtonManagerStrategy::Update(void)
{
#ifdef _DEBUG
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();	// �L�[�{�[�h�̓��͂��擾

	if (pKeyboard->GetTrigger(DIK_X))
	{
		m_bDisp = m_bDisp ? false : true;
	}

	if (!m_bDisp)
	{
		m_pAIType[0]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
		m_pAIType[1]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
	}
#endif

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

	CDebugProc::Print("�I���{�^���F%d %d %d %d", m_aSelectLogic[0], m_aSelectLogic[1], m_aSelectLogic[2], m_aSelectLogic[3]);
}

//=========================================
// �`�揈��
//=========================================
void CButtonManagerStrategy::Draw(void)
{
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
					m_pFirst = new CButton3D*[m_aNumLogic[0]];
					for (int nCntButton = 0; nCntButton < m_aNumLogic[0]; nCntButton++)
					{// �{�^���̐���
						float posZ = (-110.0f - (nCntButton * (BUTTON_LOGIC_HEIGHT - 5.0f) * (8 - m_aNumLogic[0]))) + ((BUTTON_LOGIC_HEIGHT - 5.0f) * (8 - m_aNumLogic[0]) * (m_aNumLogic[0] / 2.0f)) - (BUTTON_LOGIC_HEIGHT - 5.0f) * (8 - m_aNumLogic[0]) / 2.0f;
						m_pFirst[nCntButton] = CButton3D::Create(D3DXVECTOR3(-20.0f, 0.0f, posZ), BUTTON_LOGIC_WIDTH, BUTTON_LOGIC_HEIGHT);
						m_pFirst[nCntButton]->BindTexture(CTexture::GetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_NUMBER + nCntButton)));
					}

					// ���̐���
					if (NULL == m_pLine[0]) { m_pLine[0] = CButtonLine::Create(m_pAIType[m_nSelectAIType]->GetPos() + D3DXVECTOR3(m_pAIType[m_nSelectAIType]->GetWidth() - BUTTON_LINE_THICK, 0.0f, 0.0f)); }
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
					m_pLine[0]->Link(m_pFirst[nCntFirst]->GetPos() + D3DXVECTOR3(-m_pFirst[nCntFirst]->GetWidth(), 0.0f, 0.0f));

					if (0 == nCntFirst)
					{// 0�Ԗڂ̃{�^��
						m_aNumLogic[1] = 2;		// �Q�K�w�ڂ̃{�^���̐�
						m_pSecond = new CButton3D*[m_aNumLogic[1]];
						for (int nCntButton = 0; nCntButton < m_aNumLogic[1]; nCntButton++)
						{// �{�^���̐���
							float posZ = (-110.0f - (nCntButton * (BUTTON_LOGIC_HEIGHT - 5.0f) * (8 - m_aNumLogic[1]))) + ((BUTTON_LOGIC_HEIGHT - 5.0f) * (8 - m_aNumLogic[1]) * (m_aNumLogic[1] / 2.0f)) - (BUTTON_LOGIC_HEIGHT - 5.0f) * (8 - m_aNumLogic[1]) / 2.0f;
							m_pSecond[nCntButton] = CButton3D::Create(D3DXVECTOR3(160.0f, 0.0f, posZ), BUTTON_LOGIC_WIDTH, BUTTON_LOGIC_HEIGHT);
							m_pSecond[nCntButton]->BindTexture(CTexture::GetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_NUMBER + nCntButton)));
						}
					}
					else
					{// 1�Ԗڂ̃{�^��
						m_aNumLogic[1] = 2;		// �Q�K�w�ڂ̃{�^���̐�
						m_pSecond = new CButton3D*[m_aNumLogic[1]];
						for (int nCntButton = 0; nCntButton < m_aNumLogic[1]; nCntButton++)
						{// �{�^���̐���
							float posZ = (-110.0f - (nCntButton * (BUTTON_LOGIC_HEIGHT - 5.0f) * (8 - m_aNumLogic[1]))) + ((BUTTON_LOGIC_HEIGHT - 5.0f) * (8 - m_aNumLogic[1]) * (m_aNumLogic[1] / 2.0f)) - (BUTTON_LOGIC_HEIGHT - 5.0f) * (8 - m_aNumLogic[1]) / 2.0f;
							m_pSecond[nCntButton] = CButton3D::Create(D3DXVECTOR3(250.0f, 0.0f, posZ), BUTTON_LOGIC_WIDTH, BUTTON_LOGIC_HEIGHT);
							m_pSecond[nCntButton]->BindTexture(CTexture::GetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_NUMBER + nCntButton)));
						}
					}

					// ���̐���
					if (NULL == m_pLine[1]) { m_pLine[1] = CButtonLine::Create(m_pFirst[m_aSelectLogic[0]]->GetPos() + D3DXVECTOR3(m_pFirst[m_aSelectLogic[0]]->GetWidth() - BUTTON_LINE_THICK, 0.0f, 0.0f)); }

					if(NULL == m_apSelectIcon[0])
					{// �I���������ڂ̐���
						m_apSelectIcon[0] = CScene3D::Create();
						m_apSelectIcon[0]->SwapPriority(6);
						m_apSelectIcon[0]->SetPos(D3DXVECTOR3(m_pFirst[0]->GetPos().x, 0.0f, 170.0f));
						m_apSelectIcon[0]->SetSize(D3DXVECTOR3(BUTTON_LOGIC_WIDTH, 0.0f, BUTTON_LOGIC_HEIGHT));
						m_apSelectIcon[0]->SetBoolLighting(false);
						m_apSelectIcon[0]->SetBoolZtest(false);
						m_apSelectIcon[0]->BindTexture(CTexture::GetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_NUMBER + nCntFirst)));
					}
					else
					{
						m_apSelectIcon[0]->BindTexture(CTexture::GetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_NUMBER + nCntFirst)));
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
					m_pLine[1]->Link(m_pSecond[nCntSecond]->GetPos() + D3DXVECTOR3(-m_pSecond[nCntSecond]->GetWidth(), 0.0f, 0.0f));

					if (0 == m_aSelectLogic[0])
					{// �ړ����W�b�N�c���[
						if (0 == nCntSecond)
						{// 0�Ԗڂ̃{�^��
							m_aNumLogic[2] = 3;		// �R�K�w�ڂ̃{�^���̐�
							m_pThird = new CButton3D*[m_aNumLogic[2]];
							for (int nCntButton = 0; nCntButton < m_aNumLogic[2]; nCntButton++)
							{// �{�^���̐���
								float posZ = (-110.0f - (nCntButton * (BUTTON_LOGIC_HEIGHT - 5.0f) * (8 - m_aNumLogic[2]))) + ((BUTTON_LOGIC_HEIGHT - 5.0f) * (8 - m_aNumLogic[2]) * (m_aNumLogic[2] / 2.0f)) - (BUTTON_LOGIC_HEIGHT - 5.0f) * (8 - m_aNumLogic[2]) / 2.0f;
								m_pThird[nCntButton] = CButton3D::Create(D3DXVECTOR3(340.0f, 0.0f, posZ), BUTTON_LOGIC_WIDTH, BUTTON_LOGIC_HEIGHT);
								m_pThird[nCntButton]->BindTexture(CTexture::GetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_NUMBER + nCntButton)));
							}
						}
						else
						{// 1�Ԗڂ̃{�^��
							m_aNumLogic[2] = 2;		// �R�K�w�ڂ̃{�^���̐�
							m_pThird = new CButton3D*[m_aNumLogic[2]];
							for (int nCntButton = 0; nCntButton < m_aNumLogic[2]; nCntButton++)
							{// �{�^���̐���
								float posZ = (-110.0f - (nCntButton * (BUTTON_LOGIC_HEIGHT - 5.0f) * (8 - m_aNumLogic[2]))) + ((BUTTON_LOGIC_HEIGHT - 5.0f) * (8 - m_aNumLogic[2]) * (m_aNumLogic[2] / 2.0f)) - (BUTTON_LOGIC_HEIGHT - 5.0f) * (8 - m_aNumLogic[2]) / 2.0f;
								m_pThird[nCntButton] = CButton3D::Create(D3DXVECTOR3(340.0f, 0.0f, posZ), BUTTON_LOGIC_WIDTH, BUTTON_LOGIC_HEIGHT);
								m_pThird[nCntButton]->BindTexture(CTexture::GetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_NUMBER + nCntButton)));
							}
						}
					}
					else if (1 == m_aSelectLogic[0])
					{// �ҋ@���W�b�N�c���[
						m_aNumLogic[2] = 2;		// �R�K�w�ڂ̃{�^���̐�
						m_pThird = new CButton3D*[m_aNumLogic[2]];
						for (int nCntButton = 0; nCntButton < m_aNumLogic[2]; nCntButton++)
						{// �{�^���̐���
							float posZ = (-110.0f - (nCntButton * (BUTTON_LOGIC_HEIGHT - 5.0f) * (8 - m_aNumLogic[2]))) + ((BUTTON_LOGIC_HEIGHT - 5.0f) * (8 - m_aNumLogic[2]) * (m_aNumLogic[2] / 2.0f)) - (BUTTON_LOGIC_HEIGHT - 5.0f) * (8 - m_aNumLogic[2]) / 2.0f;
							m_pThird[nCntButton] = CButton3D::Create(D3DXVECTOR3(520.0f, 0.0f, posZ), BUTTON_LOGIC_WIDTH, BUTTON_LOGIC_HEIGHT);
							m_pThird[nCntButton]->BindTexture(CTexture::GetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_NUMBER + nCntButton)));
						}
					}

					// ���̐���
					if (NULL == m_pLine[2]) { m_pLine[2] = CButtonLine::Create(m_pSecond[m_aSelectLogic[1]]->GetPos() + D3DXVECTOR3(m_pSecond[m_aSelectLogic[1]]->GetWidth() - BUTTON_LINE_THICK, 0.0f, 0.0f)); }

					if (NULL == m_apSelectIcon[1])
					{// �I���������ڂ̐���
						m_apSelectIcon[1] = CScene3D::Create();
						m_apSelectIcon[1]->SwapPriority(6);
						m_apSelectIcon[1]->SetPos(D3DXVECTOR3(m_pSecond[0]->GetPos().x, 0.0f, 170.0f));
						m_apSelectIcon[1]->SetSize(D3DXVECTOR3(BUTTON_LOGIC_WIDTH, 0.0f, BUTTON_LOGIC_HEIGHT));
						m_apSelectIcon[1]->SetBoolLighting(false);
						m_apSelectIcon[1]->SetBoolZtest(false);
						m_apSelectIcon[1]->BindTexture(CTexture::GetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_NUMBER + nCntSecond)));
					}
					else
					{
						m_apSelectIcon[1]->BindTexture(CTexture::GetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_NUMBER + nCntSecond)));
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
					m_pLine[2]->Link(m_pThird[nCntThird]->GetPos() + D3DXVECTOR3(-m_pThird[nCntThird]->GetWidth(), 0.0f, 0.0f));

					if (0 == m_aSelectLogic[0])
					{// �ړ����W�b�N�c���[
						m_aNumLogic[3] = 2;		// �S�K�w�ڂ̃{�^���̐�
						m_pFourth = new CButton3D*[m_aNumLogic[3]];
						for (int nCntButton = 0; nCntButton < m_aNumLogic[3]; nCntButton++)
						{// �{�^���̐���
							float posZ = (-110.0f - (nCntButton * (BUTTON_LOGIC_HEIGHT - 5.0f) * (8 - m_aNumLogic[3]))) + ((BUTTON_LOGIC_HEIGHT - 5.0f) * (8 - m_aNumLogic[3]) * (m_aNumLogic[3] / 2.0f)) - (BUTTON_LOGIC_HEIGHT - 5.0f) * (8 - m_aNumLogic[3]) / 2.0f;
							m_pFourth[nCntButton] = CButton3D::Create(D3DXVECTOR3(520.0f, 0.0f, posZ), BUTTON_LOGIC_WIDTH, BUTTON_LOGIC_HEIGHT);
							m_pFourth[nCntButton]->BindTexture(CTexture::GetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_NUMBER + nCntButton)));
						}

						// ���̐���
						if (NULL == m_pLine[3]) { m_pLine[3] = CButtonLine::Create(m_pThird[m_aSelectLogic[2]]->GetPos() + D3DXVECTOR3(m_pThird[m_aSelectLogic[2]]->GetWidth() - BUTTON_LINE_THICK, 0.0f, 0.0f)); }
					}

					if (NULL == m_apSelectIcon[2])
					{// �I���������ڂ̐���
						m_apSelectIcon[2] = CScene3D::Create();
						m_apSelectIcon[2]->SwapPriority(6);
						m_apSelectIcon[2]->SetPos(D3DXVECTOR3(m_pThird[0]->GetPos().x, 0.0f, 170.0f));
						m_apSelectIcon[2]->SetSize(D3DXVECTOR3(BUTTON_LOGIC_WIDTH, 0.0f, BUTTON_LOGIC_HEIGHT));
						m_apSelectIcon[2]->SetBoolLighting(false);
						m_apSelectIcon[2]->SetBoolZtest(false);
						m_apSelectIcon[2]->BindTexture(CTexture::GetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_NUMBER + nCntThird)));
					}
					else
					{
						m_apSelectIcon[2]->BindTexture(CTexture::GetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_NUMBER + nCntThird)));
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
				m_pLine[3]->Link(m_pFourth[nCntFourth]->GetPos() + D3DXVECTOR3(-m_pFourth[nCntFourth]->GetWidth(), 0.0f, 0.0f));

				if (NULL == m_apSelectIcon[3])
				{// �I���������ڂ̐���
					m_apSelectIcon[3] = CScene3D::Create();
					m_apSelectIcon[3]->SwapPriority(6);
					m_apSelectIcon[3]->SetPos(D3DXVECTOR3(m_pFourth[0]->GetPos().x, 0.0f, 170.0f));
					m_apSelectIcon[3]->SetSize(D3DXVECTOR3(BUTTON_LOGIC_WIDTH, 0.0f, BUTTON_LOGIC_HEIGHT));
					m_apSelectIcon[3]->SetBoolLighting(false);
					m_apSelectIcon[3]->SetBoolZtest(false);
					m_apSelectIcon[3]->BindTexture(CTexture::GetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_NUMBER + nCntFourth)));
				}
				else
				{
					m_apSelectIcon[3]->BindTexture(CTexture::GetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_NUMBER + nCntFourth)));
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
		m_aSelectLogic[nCntButton] = -1;

		if (m_pLine[nCntButton] != NULL)
		{// ���̔j��
			m_pLine[nCntButton]->Uninit();
			m_pLine[nCntButton] = NULL;
		}

		if (m_apSelectIcon[nCntButton] != NULL)
		{// �I���������ڂ̔j��
			m_apSelectIcon[nCntButton]->Uninit();
			m_apSelectIcon[nCntButton] = NULL;
		}
	}
}