//=============================================================================
//
// �G�t�F�N�g���� [effect.cpp]
// Author : Ishida Takuto
//
//=============================================================================
#include "effect.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//==================================
// �}�N����`
//==================================

//==================================
// �ÓI�����o�ϐ��錾
//==================================

//==================================
// ��������
//==================================
CEffect* CEffect::Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fRange, int nLife)
{
	CEffect *pEffect = NULL;

	pEffect = new CEffect;		// ���������m��

	if (NULL != pEffect)
	{// ���������m�ۂ���Ă��邩�m�F
		pEffect->Init(pos, col, fRange, nLife);	// ������
	}
	return pEffect;
}

//=========================================
// �R���X�g���N�^
//=========================================
CEffect::CEffect(int nPriority, CScene::OBJTYPE objType) : CScene3DBill(nPriority, objType)
{
	m_nLife = 0;
}

//=========================================
// �f�X�g���N�^
//=========================================
CEffect::~CEffect()
{
}

//=========================================
// ����������
//=========================================
HRESULT CEffect::Init(D3DXVECTOR3 pos, D3DXCOLOR col, float fRange, int nLife)
{
	CScene3DBill::Init();

	// �ʒu�̐ݒ�
	CScene3DBill::SetPos(pos);

	// �F�̐ݒ�
	CScene3DBill::SetColor(col);

	// �傫���̐ݒ�
	CScene3DBill::SetSize(D3DXVECTOR3(fRange, fRange, 0.0f));
	m_nLife = nLife;

	CScene3DBill::BindTexture(CTexture::GetTexture(CTexture::TEXTURE_SHADOW));
	CScene3DBill::SetAddDraw(true);
	CScene3DBill::SetLighting(false);

	return S_OK;
}

//=========================================
// �I������
//=========================================
void CEffect::Uninit(void)
{
	CScene3DBill::Uninit();
}

//=========================================
// �X�V����
//=========================================
void CEffect::Update(void)
{
	m_nLife--;
	if (m_nLife <= 0)
	{// ���C�t���Ȃ��Ȃ����Ƃ�
		Uninit();		// �I������
	}
}

//=========================================
// �`�揈��
//=========================================
void CEffect::Draw(void)
{
	// �`�揈��
	CScene3DBill::Draw();
}
