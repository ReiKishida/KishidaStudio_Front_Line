//=============================================================================
//
// �w�i���� [bg.cpp]
// Author : Takuto Ishida
//
//=============================================================================
#include "bg.h"
#include "scene2D.h"
#include "texture.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define BG_SCROLL_SPEED		(0.005f)		// �X�N���[���̑��x

//==================================
// �ÓI�����o�ϐ��錾
//==================================

//==================================
// ��������
//==================================
CBg* CBg::Create(void)
{
	CBg *pBg;
	pBg = new CBg;	// ���������m��

	if (pBg != NULL)
	{// �������m�ې���
		pBg->Init();	// ����������
	}

	return pBg;
}

//=========================================
// �R���X�g���N�^
//=========================================
CBg::CBg(int nPriority, CScene::OBJTYPE objType) : CScene(nPriority, objType)
{
	for (int nCntBG = 0; nCntBG < MAX_BG; nCntBG++)
	{
		m_pPolygon = NULL;
	}
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
}

//=========================================
// �f�X�g���N�^
//=========================================
CBg::~CBg()
{
}

//=========================================
// ����������
//=========================================
HRESULT CBg::Init()
{
	m_pPolygon = new CScene2D*[MAX_BG];
	for (int nCntBG = 0; nCntBG < MAX_BG; nCntBG++)
	{// �w�i�|���S���̐���
		m_pPolygon[nCntBG] = CScene2D::Create();
		m_pPolygon[nCntBG]->SetPos(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f));
		m_pPolygon[nCntBG]->SetSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	}

	m_pPolygon[0]->BindTexture(CTexture::GetTexture(CTexture::TEXTURE_BG_00));
	m_pPolygon[1]->BindTexture(CTexture::GetTexture(CTexture::TEXTURE_BG_01));

	return S_OK;
}

//=========================================
// �I������
//=========================================
void CBg::Uninit(void)
{
	if (m_pPolygon != NULL)
	{
		for (int nCntBG = 0; nCntBG < MAX_BG; nCntBG++)
		{// �|���S���̔j��
			if (m_pPolygon[nCntBG] != NULL)
			{
				m_pPolygon[nCntBG]->Uninit();
				m_pPolygon[nCntBG] = NULL;
			}
		}

		delete[] m_pPolygon;
		m_pPolygon = NULL;
	}

	// ������j��
	CScene::Release();
}

//=========================================
// �X�V����
//=========================================
void CBg::Update(void)
{
	LPDIRECT3DVERTEXBUFFER9 vtxBuff;
	for (int nCntBG = 0; nCntBG < MAX_BG; nCntBG++)
	{
		// ���_���̎擾
		vtxBuff = m_pPolygon[nCntBG]->GetVtxBuff();

		// ���_����ݒ�
		VERTEX_2D *pVtx;	// ���_���̃|�C���^

		// ���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
		vtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		if (0 == nCntBG)
		{//�e�N�X�`���̍��W�ݒ�
			pVtx[0].tex.x += BG_SCROLL_SPEED;
			pVtx[1].tex.x += BG_SCROLL_SPEED;
			pVtx[2].tex.x += BG_SCROLL_SPEED;
			pVtx[3].tex.x += BG_SCROLL_SPEED;
		}
		else
		{//�e�N�X�`���̍��W�ݒ�
			pVtx[0].tex.x -= BG_SCROLL_SPEED;
			pVtx[1].tex.x -= BG_SCROLL_SPEED;
			pVtx[2].tex.x -= BG_SCROLL_SPEED;
			pVtx[3].tex.x -= BG_SCROLL_SPEED;
		}

		// ���_�o�b�t�@���A�����b�N
		vtxBuff->Unlock();

		// ���_�o�b�t�@��ݒ�
		m_pPolygon[nCntBG]->SetVtxBuff(vtxBuff);
	}
}

//=========================================
// �`�揈��
//=========================================
void CBg::Draw(void)
{
}

//=========================================
// �F�̐ݒ�
//=========================================
void CBg::SetColor(D3DXCOLOR col)
{
	m_col = col;

	for (int nCntBG = 0; nCntBG < MAX_BG; nCntBG++)
	{// �F�̐ݒ�
		m_pPolygon[nCntBG]->SetColor(m_col);
	}
}