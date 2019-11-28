//=============================================================================
//
// �����_�����O���� [renderer.cpp]
// Author : Takuto Ishida
//
//=============================================================================
#include "renderer.h"
#include "scene.h"
#include "camera.h"
#include "debugProc.h"
#include "game.h"
#include "model.h"
#include "mouseCursor.h"
#include "button.h"

//==================================
// �ÓI�����o�ϐ��錾
//==================================

//=========================================
// �R���X�g���N�^
//=========================================
CRenderer::CRenderer()
{
	m_pD3D = NULL;			// Direct3D�I�u�W�F�N�g
	m_pD3DDevice = NULL;	// Device�I�u�W�F�N�g(�`��ɕK�v)
	m_hWnd = NULL;
	m_bWindowMode = true;

#ifdef _DEBUG
	m_pFont = NULL;			// �t�H���g�ւ̃|�C���^
#endif

}

//=========================================
// �f�X�g���N�^
//=========================================
CRenderer::~CRenderer()
{
}

//=========================================
// ����������
//=========================================
HRESULT CRenderer::Init(HINSTANCE hInstance, HWND hWnd, bool bWindow)
{
	D3DPRESENT_PARAMETERS d3dpp;
	D3DDISPLAYMODE d3ddm;
	m_hWnd = hWnd;
	m_bWindowMode = bWindow;

	// Direct3D�I�u�W�F�N�g�̍쐬
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (m_pD3D == NULL)
	{
		return E_FAIL;
	}

	// ���݂̃f�B�X�v���C���[�h���擾
	if (FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// �f�o�C�X�̃v���[���e�[�V�����p�����[�^�̐ݒ�
	ZeroMemory(&d3dpp, sizeof(d3dpp));								// ���[�N���[���N���A
	d3dpp.BackBufferCount = 1;							// �o�b�N�o�b�t�@�̐�
	d3dpp.BackBufferWidth = SCREEN_WIDTH;				// �Q�[����ʃT�C�Y(��)
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;				// �Q�[����ʃT�C�Y(����)
	d3dpp.BackBufferFormat = d3ddm.Format;				// �J���[���[�h�̎w��
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;		// �f���M���ɓ������ăt���b�v����
	d3dpp.EnableAutoDepthStencil = TRUE;				// �f�v�X�o�b�t�@�i�y�o�b�t�@�j�ƃX�e���V���o�b�t�@���쐬
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;		// �f�v�X�o�b�t�@�Ƃ���16bit���g��
	d3dpp.Windowed = m_bWindowMode;						// �E�B���h�E���[�h
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;		// ���t���b�V�����[�g
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;	// �C���^�[�o��

	// �f�o�C�X�̐���
	// �f�B�X�v���C�A�_�v�^��\�����߂̃f�o�C�X���쐬
	// �`��ƒ��_�������n�[�h�E�F�A�ōs�Ȃ�
	if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		m_hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp, &m_pD3DDevice)))
	{
		// ��L�̐ݒ肪���s������
		// �`����n�[�h�E�F�A�ōs���A���_������CPU�ōs�Ȃ�
		if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			m_hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp, &m_pD3DDevice)))
		{
			// ��L�̐ݒ肪���s������
			// �`��ƒ��_������CPU�ōs�Ȃ�
			if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF, m_hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp, &m_pD3DDevice)))
			{
				// �������s
				return E_FAIL;
			}
		}
	}

	// �����_�[�X�e�[�g�̐ݒ�
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				// ���ʂ��J�����O
	m_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);						// Z�o�b�t�@���g�p
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// ���u�����h���s��
	m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);						// ���C�e�B���O���[�h�L��
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// ���\�[�X�J���[�̎w��
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// ���f�X�e�B�l�[�V�����J���[�̎w��

	// �T���v���[�X�e�[�g�̐ݒ�
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// �e�N�X�`���A�h���b�V���O���@(U�l)��ݒ�
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// �e�N�X�`���A�h���b�V���O���@(V�l)��ݒ�
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// �e�N�X�`���k���t�B���^���[�h��ݒ�
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// �e�N�X�`���g��t�B���^���[�h��ݒ�

	// �e�N�X�`���X�e�[�W�X�e�[�g�̐ݒ�
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);	// �A���t�@�u�����f�B���O������ݒ�
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);	// �ŏ��̃A���t�@����
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);	// �Q�Ԗڂ̃A���t�@����

#ifdef _DEBUG
	// �f�o�b�O���\���p�t�H���g�̐���
	D3DXCreateFont(m_pD3DDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &m_pFont);
#endif

	return S_OK;
}

//=========================================
// �I������
//=========================================
void CRenderer::Uninit(void)
{
#ifdef _DEBUG
	// �f�o�b�O���\���p�t�H���g�̔j��
	if (m_pFont != NULL)
	{
		m_pFont->Release();
		m_pFont = NULL;
	}
#endif

	// �f�o�C�X�̔j��
	if (m_pD3DDevice != NULL)
	{
		m_pD3DDevice->Release();
		m_pD3DDevice = NULL;
	}

	// Direct3D�I�u�W�F�N�g�̔j��
	if (m_pD3D != NULL)
	{
		m_pD3D->Release();
		m_pD3D = NULL;
	}
}

//=========================================
// �X�V����
//=========================================
void CRenderer::Update(void)
{
	CScene::UpdateAll();
}

//=========================================
// �`�揈��
//=========================================
void CRenderer::Draw(void)
{
	// �o�b�N�o�b�t�@���y�o�b�t�@�̃N���A
	m_pD3DDevice->Clear(0,
		NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL),
		D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), 1.0f, 0);

	if (SUCCEEDED(m_pD3DDevice->BeginScene()))
	{// Direct3D�ɂ��`��̊J�n
		CCamera *pCamera = NULL;			// �J�����̏����i�[
		pCamera = CManager::GetCamera();	// �J�����̏����擾

		if (NULL != CManager::GetGame())
		{
			if (CGame::PART_ACTION == CManager::GetGame()->GetPart())
			{
				if (NULL != pCamera)
				{// �k���`�F�b�N
					pCamera->Set(0);	// �J�����̃Z�b�g

					// �S�ẴI�u�W�F�N�g�̕`��
					CScene::DrawAll();
				}
			}
			else if (CGame::PART_STRATEGY == CManager::GetGame()->GetPart())
			{// �X�g���e�W�[��ʂ̂�
				pCamera->Set(0);	// �J�����̃Z�b�g

				CManager::GetGame()->GetMouse()->SetDisp(false);

				// �S�ẴI�u�W�F�N�g�̕`��
				CScene::DrawAll();

				CManager::GetGame()->GetMouse()->SetDisp(true);

				pCamera->Set(1);

				m_pD3DDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
				m_pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
				m_pD3DDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_GREATEREQUAL);

				CManager::GetGame()->GetField()->Draw();

				pCamera->Set(0);	// �J�����̃Z�b�g
				CScene::DrawStrategy();
				CManager::GetGame()->GetMouseCursor()->Draw();

				pCamera->Set(1);
				CManager::GetGame()->GetButtonManager()->MarkerDraw();
				CManager::GetGame()->GetMouse()->Draw();

				m_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
				m_pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
				m_pD3DDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
			}
		}
		else
		{
			// �S�ẴI�u�W�F�N�g�̕`��
			CScene::DrawAll();
		}

#ifdef _DEBUG
		pCamera->Set(0);	// �J�����̃Z�b�g
		// �f�o�b�O�\��
		CDebugProc::Draw();
#endif

		// Direct3D�ɂ��`��̏I��
		m_pD3DDevice->EndScene();
	}

	// �o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
	m_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}
