//=============================================================================
//
// ���C�g���� [light.cpp]
// Author : Takuto Ishida
//
//=============================================================================
#include "light.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "debugProc.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************************************
D3DLIGHT9 CLight::m_alight[MAX_LIGHT] = {};

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CLight::CLight()
{
	for (int nCntDirect = 0; nCntDirect < MAX_LIGHT; nCntDirect++)
	{
		m_direct[nCntDirect] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
	m_nSelectLight = 0;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CLight::~CLight()
{
}

//=============================================================================
// ���C�g�̏���������
//=============================================================================
void CLight::Init(void)
{
	// �f�o�C�X�̎擾
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	for (int nCntLight = 0; nCntLight < MAX_LIGHT; nCntLight++)
	{
		// ���C�g���N���A����
		ZeroMemory(&m_alight[nCntLight], sizeof(D3DLIGHT9));

		// ���C�g�̎�ނ�ݒ�
		m_alight[nCntLight].Type = D3DLIGHT_DIRECTIONAL;
	}

	// ���C�g�̊g�U����ݒ�
	m_alight[0].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_alight[1].Diffuse = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
	m_alight[2].Diffuse = D3DXCOLOR(0.45f, 0.45f, 0.45f, 1.0f);

	// ���C�g�̕����̐ݒ�
	m_direct[0] = D3DXVECTOR3(0.81f, -0.9f, 0.20f);
	D3DXVec3Normalize(&m_direct[0], &m_direct[0]);	// ���K������
	m_alight[0].Direction = m_direct[0];

	// ���C�g�̕����̐ݒ�
	m_direct[1] = D3DXVECTOR3(-0.58f, 0.88f, -0.44f);
	D3DXVec3Normalize(&m_direct[1], &m_direct[1]);	// ���K������
	m_alight[1].Direction = m_direct[1];

	// ���C�g�̕����̐ݒ�
	m_direct[2] = D3DXVECTOR3(0.89f, -0.11f, 0.44f);
	D3DXVec3Normalize(&m_direct[2], &m_direct[2]);	// ���K������
	m_alight[2].Direction = m_direct[2];

	for (int nCntLight = 0; nCntLight < MAX_LIGHT; nCntLight++)
	{
		// ���C�g��ݒ肷��
		pDevice->SetLight(nCntLight, &m_alight[nCntLight]);

		// ���C�g��L���ɂ���
		pDevice->LightEnable(nCntLight, TRUE);
	}
}

//=============================================================================
// ���C�g�̏I������
//=============================================================================
void CLight::Uninit(void)
{
}

//=============================================================================
// ���C�g�̍X�V����
//=============================================================================
void CLight::Update(void)
{
#ifdef _DEBUG
	// �f�o�C�X�̎擾
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();	// �L�[�{�[�h�̎擾

	if (pKeyboard->GetRepeat(DIK_T))
	{// �I�����グ��
		m_nSelectLight = (1 + m_nSelectLight) % MAX_LIGHT;
	}
	if (pKeyboard->GetRepeat(DIK_G))
	{// �I����������
		m_nSelectLight = (1 + (MAX_LIGHT - 1)) % MAX_LIGHT;
	}

	// X
	if (pKeyboard->GetRepeat(DIK_Y))
	{// �グ��
		m_fAngle[m_nSelectLight] += D3DX_PI * 0.01f;
		if (m_fAngle[m_nSelectLight] > D3DX_PI) { m_fAngle[m_nSelectLight] = -D3DX_PI; }

		m_direct[m_nSelectLight].x += sinf(D3DX_PI * m_fAngle[m_nSelectLight]);
		m_direct[m_nSelectLight].y += cosf(D3DX_PI * m_fAngle[m_nSelectLight]);
	}
	if (pKeyboard->GetRepeat(DIK_H))
	{// ������
		m_fAngle[m_nSelectLight] -= D3DX_PI * 0.01f;
		if (m_fAngle[m_nSelectLight] < -D3DX_PI) { m_fAngle[m_nSelectLight] = D3DX_PI; }

		m_direct[m_nSelectLight].x += sinf(D3DX_PI * m_fAngle[m_nSelectLight]);
		m_direct[m_nSelectLight].y += cosf(D3DX_PI * m_fAngle[m_nSelectLight]);
	}

	// Z
	if (pKeyboard->GetRepeat(DIK_U))
	{// �グ��
		m_fAngle[m_nSelectLight] += D3DX_PI * 0.01f;
		if (m_fAngle[m_nSelectLight] > D3DX_PI) { m_fAngle[m_nSelectLight] = -D3DX_PI; }

		m_direct[m_nSelectLight].z += sinf(D3DX_PI * m_fAngle[m_nSelectLight]);
		m_direct[m_nSelectLight].y += cosf(D3DX_PI * m_fAngle[m_nSelectLight]);
	}
	if (pKeyboard->GetRepeat(DIK_J))
	{// ������
		m_fAngle[m_nSelectLight] -= D3DX_PI * 0.01f;
		if (m_fAngle[m_nSelectLight] < -D3DX_PI) { m_fAngle[m_nSelectLight] = D3DX_PI; }

		m_direct[m_nSelectLight].z += sinf(D3DX_PI * m_fAngle[m_nSelectLight]);
		m_direct[m_nSelectLight].y += cosf(D3DX_PI * m_fAngle[m_nSelectLight]);
	}

	D3DXVec3Normalize(&m_direct[m_nSelectLight], &m_direct[m_nSelectLight]);	// ���K������
	m_alight[m_nSelectLight].Direction = m_direct[m_nSelectLight];

	// ���C�g��ݒ肷��
	pDevice->SetLight(m_nSelectLight, &m_alight[m_nSelectLight]);

	// ���C�g��L���ɂ���
	pDevice->LightEnable(m_nSelectLight, TRUE);

	CDebugProc::Print("\n���C�g%d", m_nSelectLight);
	CDebugProc::Print("���C�gX�F%.2f", m_direct[m_nSelectLight].x);
	CDebugProc::Print("���C�gY�F%.2f", m_direct[m_nSelectLight].y);
	CDebugProc::Print("���C�gZ�F%.2f\n", m_direct[m_nSelectLight].z);
#endif
}
