//=============================================================================
//
// �t�F�[�h���� [fade.cpp]
// Author : Takuto Ishida
//
//=============================================================================
#include "fade.h"
#include "input.h"
#include "renderer.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
CFade::FADE		CFade::m_fade = CFade::FADE_NONE;								// �t�F�[�h���
CManager::MODE	CFade::m_modeNext = CManager::MODE_TITLE;				// ���̃��[�h
D3DXCOLOR		CFade::m_colFade = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);	// �t�F�[�h�F

//=============================================================================
// �t�F�[�h�̐���
//=============================================================================
CFade *CFade::Create(CManager::MODE modeNext)
{
	CFade *pFade = NULL;
	if (pFade == NULL && m_fade == FADE_NONE)
	{// �t�F�[�h�̏�Ԃ������Ȃ��Ƃ��̂ݐ��������
		pFade = new CFade;

		if (pFade != NULL)
		{
			pFade->Init();	// ����������

			// �l�̏�����
			m_fade = FADE_OUT;			// �t�F�[�h�A�E�g��Ԃ�
			m_modeNext = modeNext;		// ���̃V�[�����L��
		}
	}

	return pFade;
}

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CFade::CFade(int nPriority, CScene::OBJTYPE objType) : CScene2D(nPriority, objType)
{
	m_fade = FADE_NONE;
	m_colFade = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CFade::~CFade()
{
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CFade::Init(void)
{
	CScene2D::Init();

	// �ʒu�̐ݒ�
	CScene2D::SetPos(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f));

	// �傫���̐ݒ�
	CScene2D::SetSize(SCREEN_WIDTH, SCREEN_HEIGHT);

	// �F�̐ݒ�
	m_colFade = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	CScene2D::SetColor(m_colFade);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CFade::Uninit(void)
{
	CScene2D::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CFade::Update(void)
{
	bool bDelete = false;

	if (m_fade != FADE_NONE)
	{// �t�F�[�h�̏�Ԃ��������Ă��Ȃ��Ƃ��̓X���[
		if (m_fade == FADE_IN)		//�t�F�[�h�C�����
		{
			m_colFade.a -= 0.02f;	//��ʂ𓧖��ɂ��Ă���

			if (m_colFade.a <= 0.0f)
			{
				m_colFade.a = 0.0f;
				m_fade = FADE_NONE;	//�������Ă��Ȃ����
				bDelete = true;
			}
		}
		else if (m_fade == FADE_OUT)
		{
			m_colFade.a += 0.02f;	//��ʂ�s�����ɂ��Ă���

			if (m_colFade.a >= 1.0f)
			{
				m_colFade.a = 1.0f;
				m_fade = FADE_IN;	//�t�F�[�h�C����Ԃ�

				//���[�h�̐ݒ�
				CManager::SetMode(m_modeNext);
			}
		}

		// �F�̐ݒ�
		CScene2D::SetColor(m_colFade);

		if (bDelete == true)
		{// �I������t�F�[�h������
			Uninit();
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CFade::Draw(void)
{
	// �f�o�C�X�̎擾
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	// Z�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);

	CScene2D::Draw();

	// Z�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
}
