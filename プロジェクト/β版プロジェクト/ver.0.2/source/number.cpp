//=============================================================================
//
// �������� [number.cpp]
// Author : Takuto Ishida
//
//=============================================================================
#include "number.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "UI_Number.h"

//==================================
// �}�N����`
//==================================

//==================================
// �ÓI�����o�ϐ��錾
//==================================

//==================================
// ��������
//==================================
CNumber* CNumber::Create(D3DXVECTOR3 pos, float fWidth, float fHeight, CUI_NUMBER::UI_NUMTYPE UINumType, int nUV, int nUV_X, int nUV_Y)
{
	CNumber *pNumber = NULL;

	pNumber = new CNumber;	// ���������m��

	if (pNumber)
	{// �������m�ې���
		pNumber->m_TeamType = UINumType;
		pNumber->m_nUV = nUV;
		pNumber->m_nUV_X = nUV_X;
		pNumber->m_nUV_Y = nUV_Y;
		pNumber->Init(pos, fWidth, fHeight);	// ����������
	}

	return pNumber;
}

//=========================================
// �R���X�g���N�^
//=========================================
CNumber::CNumber(int nPriority, CScene::OBJTYPE objType) : CScene2D(nPriority, objType)
{
	m_nNumber = 0;
	m_UINum = NULL;
	m_TeamType = m_UINum->UI_NUMTYPE_NONE;		// �^�C�v�ݒ肵�Ȃ�
	m_nUV = 0;
	m_nUV_X = 0;
	m_nUV_Y = 0;
}

//=========================================
// �f�X�g���N�^
//=========================================
CNumber::~CNumber()
{
}

//=========================================
// ����������
//=========================================
HRESULT CNumber::Init(D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	// ����������
	CScene2D::Init();

	if (m_TeamType == CUI_NUMBER::UI_NUMTYPE_BLUE)
	{
		CScene2D::BindTexture(CTexture::GetTexture(CTexture::TEXTURE_NUMBER));
		CScene2D::SetTex(m_nUV, m_nUV_X, m_nUV_Y);
	}
	else if (m_TeamType == CUI_NUMBER::UI_NUMTYPE_RED)
	{
		CScene2D::BindTexture(CTexture::GetTexture(CTexture::TEXTURE_NUMBER));
		CScene2D::SetTex(m_nUV, m_nUV_X, m_nUV_Y);
	}
	else
	{
		CScene2D::BindTexture(CTexture::GetTexture(CTexture::TEXTURE_NUMBER));
		CScene2D::SetTex(m_nUV, m_nUV_X, m_nUV_Y);
	}

	// �ʒu�̐ݒ�
	CScene2D::SetPos(pos);

	// �T�C�Y�̐ݒ�
	CScene2D::SetSize(fWidth, fHeight);

	return S_OK;
}

//=========================================
// �I������
//=========================================
void CNumber::Uninit(void)
{
	// �I������
	CScene2D::Uninit();
}

//=========================================
// �X�V����
//=========================================
void CNumber::Update(void)
{
}

//=========================================
// �`�揈��
//=========================================
void CNumber::Draw(void)
{
	// �`�揈��
	CScene2D::Draw();
}

//=========================================
// �����̐ݒ�
//=========================================
const void CNumber::SetNumber(int nNumber)
{
	// �l�̕ۑ�
	m_nNumber = nNumber;

	// ���_���̎擾
	LPDIRECT3DVERTEXBUFFER9 vtxBuff = CScene2D::GetVtxBuff();

	// ���_����ݒ�
	VERTEX_2D *pVtx;	// ���_���̃|�C���^

						// ���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	vtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// �e�N�X�`�����W
	pVtx[0].tex = D3DXVECTOR2(0.0f + m_nNumber * 0.1f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.1f + m_nNumber * 0.1f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f + m_nNumber * 0.1f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.1f + m_nNumber * 0.1f, 1.0f);

	// ���_�o�b�t�@���A�����b�N
	vtxBuff->Unlock();
}

//=========================================
// ������UV�̐ݒ�
//=========================================
const void CNumber::SetTexNumber(int nNumber)
{
	// �l�̕ۑ�
	m_nNumber = nNumber;

	// ���_���̎擾
	LPDIRECT3DVERTEXBUFFER9 vtxBuff = CScene2D::GetVtxBuff();

	// ���_����ݒ�
	VERTEX_2D *pVtx;	// ���_���̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	vtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// �e�N�X�`�����W
	pVtx[0].tex = D3DXVECTOR2(0.0f + m_nNumber * 0.1f, (m_nUV / m_nUV_X) * (1.0f / m_nUV_Y));
	pVtx[1].tex = D3DXVECTOR2(0.1f + m_nNumber * 0.1f, (m_nUV / m_nUV_X) * (1.0f / m_nUV_Y));
	pVtx[2].tex = D3DXVECTOR2(0.0f + m_nNumber * 0.1f, (m_nUV / m_nUV_X) * (1.0f / m_nUV_Y) + (1.0f / m_nUV_Y));
	pVtx[3].tex = D3DXVECTOR2(0.1f + m_nNumber * 0.1f, (m_nUV / m_nUV_X) * (1.0f / m_nUV_Y) + (1.0f / m_nUV_Y));

	// ���_�o�b�t�@���A�����b�N
	vtxBuff->Unlock();
}

//=========================================
// �T�C�Y�̎擾
//=========================================
void CNumber::GetSize(float *pWidth, float *pHeight)
{
	if (pWidth)
	{// NULL�łȂ��Ƃ��l������
		*pWidth = CScene2D::GetWidth();
	}

	if (pHeight)
	{// NULL�łȂ��Ƃ��l������
		*pHeight = CScene2D::GetHeight();
	}
}
