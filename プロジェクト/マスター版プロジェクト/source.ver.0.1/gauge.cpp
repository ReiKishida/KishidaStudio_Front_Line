//=============================================================================
//
// �Q�[�W���� [gauge.h]
// Author : Ishida Takuto
//
//=============================================================================
#include "gauge.h"
#include "logo.h"
#include "scene3DBill.h"
#include "manager.h"
#include "renderer.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================

//=============================================================================
// ��������
//=============================================================================
CGauge2D* CGauge2D::Create(int nNumBar, D3DXVECTOR3 pos, float fInit, float fMax, float fWidth, float fHeight, int nStartReduce)
{
	CGauge2D *pGauge = NULL;

	pGauge = new CGauge2D;	// ���������m��

	if (pGauge != NULL)
	{// �������m�ې���
		pGauge->m_pos = pos;					// �o�[�̈ʒu
		pGauge->m_nNumBar = nNumBar;			// �o�[�̖{��
		pGauge->m_fGauge = fInit;				// �����l
		pGauge->m_fMax = fMax;					// �ő�l
		pGauge->m_nStartReduce = nStartReduce;	// ���炵�n�߂�܂ł̎���
		pGauge->m_nCurGauge = nNumBar - 1;
		pGauge->m_pGauge = new CLogo*[nNumBar];
		for (int nCntGauge = 0; nCntGauge < nNumBar; nCntGauge++)
		{// �Q�[�W�o�[�̐���
			pGauge->m_pGauge[nCntGauge] = CLogo::Create(pos, fWidth, fHeight);		// �o�[�̐���
		}
	}

	// ����������
	pGauge->Init();

	return pGauge;
}

//=========================================
// �e�N�X�`�������蓖�Ă�
//=========================================
void CGauge2D::BindTexture(LPDIRECT3DTEXTURE9 pTexture, int nGauge)
{
	if (nGauge < m_nNumBar)
	{// �o�[�̖{���𒴂��Ȃ��悤�ɂ���
		m_pGauge[nGauge]->BindTexture(pTexture);
	}
}

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CGauge2D::CGauge2D(int nPriority, CScene::OBJTYPE objType) : CScene(nPriority, objType)
{
	m_pGauge = NULL;
	m_nCurGauge = 0;
	m_bDisp = true;

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CGauge2D::~CGauge2D()
{
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CGauge2D::Init(void)
{
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CGauge2D::Uninit(void)
{
	for (int nCntGauge = 0; nCntGauge < m_nNumBar; nCntGauge++)
	{
		if (NULL != m_pGauge[nCntGauge])
		{// 2D�|���S���̔j��
			m_pGauge[nCntGauge]->Uninit();
			m_pGauge[nCntGauge] = NULL;
		}
	}

	if (NULL != m_pGauge)
	{// �������̊J��
		delete m_pGauge;
		m_pGauge = NULL;
	}

	m_nNumBar = 0;
	// ������j��
	CScene::Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CGauge2D::Update(void)
{
	if (m_nNumBar >= 3)
	{// �o�[�̖{�����R�{�ȏ�̂Ƃ�
		const float fWidth = m_pGauge[0]->GetWidth();	// �����̒���

		float fLife = m_pGauge[m_nCurGauge - 1]->GetWidth();				// �o�[�̒������擾
		float fGaugeView = fLife * (1.0f / (fWidth * 0.01f));	// ���ɂ���
		float fPercent = m_fGauge * (1.0f / (m_fMax * 0.01f));	// �l��%�ɂ���

		if (m_nStartReduce > m_nCntReduce)
		{// �J�E���g����
			m_nCntReduce++;
		}

		if (m_nStartReduce <= m_nCntReduce)
		{// ��莞�Ԍo��
			if (fPercent < fGaugeView)
			{// �Q�[�W�����݂̒l�Ɠ����ɂȂ�܂Ō��炷
				fLife -= fWidth * 0.01f;			// �Q�[�W�̒�����1�������炷
				m_pGauge[m_nCurGauge - 1]->SetWidth(fLife);		// �o�[�̒�����ݒ�
			}
			else if (fPercent > fGaugeView)
			{// �Q�[�W�����݂̒l�Ɠ����ɂȂ�܂ő��₷
				fLife += fWidth * 0.01f;			// �Q�[�W�̒�����1�������₷
				m_pGauge[m_nCurGauge - 1]->SetWidth(fLife);		// �o�[�̒�����ݒ�
			}
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CGauge2D::Draw(void)
{
}

//=============================================================================
// �l�̐ݒ�
//=============================================================================
void CGauge2D::SetValue(float fGauge)
{
	m_fGauge = fGauge;
	float fBarWidth;		// �o�[�̒����̒l
	const float fWidth = m_pGauge[0]->GetWidth();			// �����̒���

	if (m_fGauge > 0)
	{// 0���傫���Ƃ�
		fBarWidth = fWidth / (m_fMax / m_fGauge);	// �o�[�̌��̒�����100%�Ƃ��đ���������
	}
	else
	{// ���C�t��0�̂Ƃ�
		fBarWidth = 0;
	}

	if (m_fGauge >= m_fMax)
	{// �ő�l�𒴂�����߂�
		m_fGauge = m_fMax;
		fBarWidth = fWidth;
	}

	// ���_���̎擾
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = m_pGauge[m_nCurGauge]->GetVtxBuff();

	// ���_����ݒ�
	VERTEX_2D *pVtx;	// ���_���̃|�C���^

						// ���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	float fGaugeWidth = m_pGauge[m_nCurGauge]->GetWidth();
	D3DXVECTOR3 pos = m_pGauge[m_nCurGauge]->GetPos();

	// �e�N�X�`�����W���X�V
	pVtx[0].pos.x = pos.x + (-fGaugeWidth * 0.5f);
	pVtx[1].pos.x = pos.x + (-fGaugeWidth * 0.5f + fBarWidth);
	pVtx[2].pos.x = pos.x + (-fGaugeWidth * 0.5f);
	pVtx[3].pos.x = pos.x + (-fGaugeWidth * 0.5f + fBarWidth);

	// ���_�o�b�t�@���A�����b�N
	pVtxBuff->Unlock();

	// ���_�o�b�t�@��ݒ�
	m_pGauge[m_nCurGauge]->SetVtxBuff(pVtxBuff);

	// �o�[�̒������X�V
	//m_pGauge[m_nCurGauge]->SetWidth(fBarWidth);

	if (fGauge > 0 && m_nNumBar > 2)
	{// �񕜂����Ƃ����̃o�[�����₷
		m_pGauge[m_nCurGauge - 1]->SetWidth(fBarWidth);		// �o�[�̒������X�V
	}

	m_nCntReduce = 0;						// �o�[�����炵�n�߂邽�߂̃J�E���^��������
}

//=============================================================================
// �Q�[�W�̑���
//=============================================================================
void CGauge2D::AddSubtract(float fValue)
{
	m_fGauge += fValue;		// ���C�t�𑝌�������
	float fBarWidth;		// �o�[�̒����̒l
	const float fWidth = m_pGauge[0]->GetWidth();			// �����̒���

	if (m_fGauge > 0)
	{// 0���傫���Ƃ�
		fBarWidth = fWidth / (m_fMax / m_fGauge);	// �o�[�̌��̒�����100%�Ƃ��đ���������
	}
	else
	{// 0�̂Ƃ�
		fBarWidth = 0;
	}

	if (m_fGauge >= m_fMax)
	{// �ő�l�𒴂�����߂�
		m_fGauge = m_fMax;
		fBarWidth = fWidth;
	}

	// ���_���̎擾
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = m_pGauge[m_nCurGauge]->GetVtxBuff();

	// ���_����ݒ�
	VERTEX_2D *pVtx;	// ���_���̃|�C���^

						// ���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	float fGaugeWidth = m_pGauge[m_nCurGauge]->GetWidth();
	D3DXVECTOR3 pos = m_pGauge[m_nCurGauge]->GetPos();

	// �e�N�X�`�����W���X�V
	pVtx[0].pos.x = pos.x + (-fGaugeWidth * 0.5f);
	pVtx[1].pos.x = pos.x + (-fGaugeWidth * 0.5f + fBarWidth);
	pVtx[2].pos.x = pos.x + (-fGaugeWidth * 0.5f);
	pVtx[3].pos.x = pos.x + (-fGaugeWidth * 0.5f + fBarWidth);

	// ���_�o�b�t�@���A�����b�N
	pVtxBuff->Unlock();

	// ���_�o�b�t�@��ݒ�
	m_pGauge[m_nCurGauge]->SetVtxBuff(pVtxBuff);

	//// �o�[�̒������X�V
	//m_pGauge[m_nCurGauge]->SetWidth(fBarWidth);

	if (fValue > 0 && m_nNumBar > 2)
	{// �񕜂����Ƃ����̃o�[�����₷
		m_pGauge[m_nCurGauge - 1]->SetWidth(fBarWidth);			// �o�[�̒������X�V
	}

	m_nCntReduce = 0;	// �_���[�W�o�[�����炵�n�߂邽�߂̃J�E���^��������
}

//=============================================================================
// �F�̐ݒ�
//=============================================================================
void CGauge2D::SetColor(D3DXCOLOR col, int nBar)
{
	if (m_nNumBar > nBar)
	{// �g���Ă���{���܂�
		m_pGauge[nBar]->SetColor(col);
	}
}

//=============================================================================
// �F�̎擾
//=============================================================================
D3DXCOLOR CGauge2D::GetColor(int nBar)
{
	if (m_nNumBar > nBar)
	{// �g���Ă���{���܂�
		return m_pGauge[nBar]->GetColor();
	}

	// �g���Ă��Ȃ��{�����w�肳�ꂽ�Ƃ�
	return D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
}

//=============================================================================
// �\���̏���
//=============================================================================
void CGauge2D::SetDisp(bool bDisp)
{
	m_bDisp = bDisp;

	for (int nCntGauge = 0; nCntGauge < m_nNumBar; nCntGauge++)
	{
		if (NULL != m_pGauge[nCntGauge])
		{// 2D�|���S���̔j��
			m_pGauge[nCntGauge]->SetDisp(m_bDisp);
		}
	}

}

/*==============================================*/
/*				 3D�Q�[�W�N���X					*/
/*==============================================*/
//=============================================================================
// ��������
//=============================================================================
CGauge3D* CGauge3D::Create(int nNumBar, D3DXVECTOR3 offset, float fInit, float fMax, float fWidth, float fHeight, D3DXVECTOR3 *pPosParent, int nStartReduce)
{
	CGauge3D *pGauge = NULL;

	pGauge = new CGauge3D;	// ���������m��

	if (pGauge != NULL)
	{// �������m�ې���
		pGauge->m_offset = offset;				// �o�[�̃I�t�Z�b�g
		pGauge->m_nNumBar = nNumBar;			// �o�[�̖{��
		pGauge->m_fGauge = fInit;				// �����l
		pGauge->m_fMax = fMax;					// �ő�l
		pGauge->m_nStartReduce = nStartReduce;	// ���炵�n�߂�܂ł̎���
		pGauge->m_pPosParent = pPosParent;		// �e�̈ʒu

		pGauge->m_pGauge = new CScene3DBill*[nNumBar];
		for (int nCntGauge = 0; nCntGauge < nNumBar; nCntGauge++)
		{// �Q�[�W�o�[�̐���
			pGauge->m_pGauge[nCntGauge] = CScene3DBill::Create();		// �o�[�̐���
			pGauge->m_pGauge[nCntGauge]->SetPos(offset + *pPosParent);					// �ʒu�̐ݒ�
			pGauge->m_pGauge[nCntGauge]->SetSize(D3DXVECTOR3(fWidth, fHeight, 0.0f));	// �T�C�Y�̐ݒ�
		}

		// ����������
		pGauge->Init();
	}

	return pGauge;
}

//=========================================
// �e�N�X�`�������蓖�Ă�
//=========================================
void CGauge3D::BindTexture(LPDIRECT3DTEXTURE9 pTexture, int nGauge)
{
	if (nGauge < m_nNumBar)
	{// �o�[�̖{���𒴂��Ȃ��悤�ɂ���
		m_pGauge[nGauge]->BindTexture(pTexture);
	}
}

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CGauge3D::CGauge3D(int nPriority, CScene::OBJTYPE objType) : CScene(nPriority, objType)
{
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CGauge3D::~CGauge3D()
{
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CGauge3D::Init(void)
{
	for (int nCntGauge = 0; nCntGauge < m_nNumBar; nCntGauge++)
	{// Z�o�b�t�@�̐ݒ�
		m_pGauge[nCntGauge]->SetZBuffer(true, D3DCMP_ALWAYS);
		m_pGauge[nCntGauge]->SetLighting(false);
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CGauge3D::Uninit(void)
{
	if (NULL != m_pGauge)
	{// �g���Ă���
		for (int nCntGauge = 0; nCntGauge < m_nNumBar; nCntGauge++)
		{// �o�[�̖{�������[�v
			if (NULL != m_pGauge[nCntGauge])
			{// �g���Ă�����̂�j��
				m_pGauge[nCntGauge]->Uninit();
				m_pGauge[nCntGauge] = NULL;
			}
		}

		// �j��
		delete m_pGauge;
		m_pGauge = NULL;
	}

	// ������j��
	CScene::Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CGauge3D::Update(void)
{
	if (m_pGauge)
	{// �g���Ă���
		if (m_nNumBar >= 3)
		{// �o�[�̖{�����R�{�ȏ�̂Ƃ�
			const float fWidth = m_pGauge[0]->GetWidth();	// �����̒���
			int nBar = m_nNumBar - 2;						// ���炷�o�[�̔ԍ�

			float fLife = m_pGauge[nBar]->GetWidth();				// �o�[�̒������擾
			float fGaugeView = fLife * (1.0f / (fWidth * 0.01f));	// �o�[�̒��������ɂ���
			float fPercent = m_fGauge * (1.0f / (m_fMax * 0.01f));	// �l�����ɂ���

			if (m_nStartReduce > m_nCntReduce)
			{// �J�E���g����
				m_nCntReduce++;
			}

			if (m_nStartReduce <= m_nCntReduce)
			{// ��莞�Ԍo��
				if (fPercent < fGaugeView && fLife > -fWidth)
				{// �Q�[�W�����݂̒l�Ɠ����ɂȂ�܂Ō��炷
					fLife -= fWidth * 0.01f;				// �Q�[�W�̒�����1�������炷
					m_pGauge[nBar]->SetWidth(fLife);		// �o�[�̒�����ݒ�
				}
				else if (fPercent > fGaugeView && fLife < fWidth)
				{// �Q�[�W�����݂̒l�Ɠ����ɂȂ�܂ő��₷
					fLife += fWidth * 0.01f;			// �Q�[�W�̒�����1�������₷
					m_pGauge[nBar]->SetWidth(fLife);		// �o�[�̒�����ݒ�
				}
			}
		}

		for (int nCntBar = 0; nCntBar < m_nNumBar; nCntBar++)
		{// �ʒu�̐ݒ�
			m_pGauge[nCntBar]->SetPos(m_offset + *m_pPosParent);
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CGauge3D::Draw(void)
{

}

//=============================================================================
// �Q�[�W�̑���
//=============================================================================
const void CGauge3D::AddSubtract(float fValue)
{
	m_fGauge += fValue;		// ���C�t�𑝌�������
	float fLife;			// �o�[�̒����̒l
	const float fWidth = m_pGauge[0]->GetWidth();			// �����̒���

	if (m_fGauge > 0)
	{// ���C�t��0���傫���Ƃ�
		fLife = fWidth / (m_fMax / m_fGauge);	// �o�[�̌��̒�����100%�Ƃ��ă��C�t�𑝌�������
	}
	else
	{// ���C�t��0�̂Ƃ�
		fLife = 0;
	}

	if (m_fGauge >= m_fMax)
	{// �ő�l�𒴂�����߂�
		m_fGauge = m_fMax;
		fLife = fWidth;
	}

	// �o�[�̒������X�V
	m_pGauge[m_nNumBar - 1]->SetWidth(fLife);

	D3DXVECTOR3 pos = m_pGauge[m_nNumBar - 1]->GetPos();

	if (fValue > 0 && m_nNumBar > 2)
	{// �񕜂����Ƃ����̃o�[�����₷
		m_pGauge[m_nNumBar - 2]->SetWidth(fLife);			// �o�[�̒������X�V
	}

	m_nCntReduce = 0;						// �_���[�W�o�[�����炵�n�߂邽�߂̃J�E���^��������
}

//=============================================================================
// �F�̐ݒ�
//=============================================================================
const void CGauge3D::SetColor(D3DXCOLOR col, int nBar)
{
	if (m_nNumBar > nBar)
	{// �g���Ă���{���܂�
		m_pGauge[nBar]->SetColor(col);
	}
}

//=============================================================================
// �F�̎擾
//=============================================================================
D3DXCOLOR CGauge3D::GetColor(int nBar)
{
	if (m_nNumBar > nBar)
	{// �g���Ă���{���܂�
		return m_pGauge[nBar]->GetColor();
	}

	// �g���Ă��Ȃ��{�����w�肳�ꂽ�Ƃ�
	return D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
}
