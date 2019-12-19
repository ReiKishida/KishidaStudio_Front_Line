//=============================================================================
//
// �s�����ď��� [pin.cpp]
// Author : Komatsu Keisuke
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "pin.h"
#include "manager.h"
#include "renderer.h"
#include "debugproc.h"
#include "input.h"
#include "texture.h"
#include "player.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define MOUSE_WIDTH		(20.0f)		// �s���e�N�X�`���̕�
#define MOUSE_HEIGHT	(30.0f)		// �s���e�N�X�`���̍���

//=============================================================================
// �ÓI�����o�ϐ�
//=============================================================================

//=============================================================================
// ��������
//=============================================================================
CPin  *CPin::Create(D3DXVECTOR3 pos)
{
	CPin *pPin = NULL;

	if (pPin == NULL)
	{
		pPin = new CPin;

		if (pPin != NULL)
		{// �������m�ې���
			pPin->m_pos = pos;
			pPin->Init();
		}
	}

	return pPin;
}

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CPin::CPin(int nPriority, CScene::OBJTYPE objType) : CScene3DBill(nPriority, objType) {}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CPin::~CPin() {}

//=============================================================================
// ����������
//=============================================================================
HRESULT CPin::Init()
{
	m_size = D3DXVECTOR3(MOUSE_WIDTH, MOUSE_HEIGHT, 0.0f);
	m_pTexture = CTexture::GetTexture(CTexture::TEXTURE_GOAL_PIN);
	CScene3DBill::Init();

	// �s���̏����ݒ�
	CScene3DBill::SetPos(m_pos);
	CScene3DBill::SetSize(m_size);
	CScene3DBill::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	CScene3DBill::BindTexture(m_pTexture);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CPin::Uninit(void)
{
	// �s���̔j��
	CScene3DBill::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CPin::Update(void)
{
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();	// �L�[�{�[�h�̓��͂��擾
	CInputMouse *pMouse = CManager::GetInputMouse();			// �}�E�X�̓��͂��擾

	// ���X�V
	CScene3DBill::BindTexture(m_pTexture);
	CScene3DBill::SetPos(m_pos);
}

//=============================================================================
// �`�揈��
//=============================================================================
void CPin::Draw(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// ���C�e�B���OOFF
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// Z�e�X�gOFF
	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);

	// �s���̕`��
	CScene3DBill::Draw();

	// Z�e�X�gON
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);

	// ���C�e�B���OON
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}