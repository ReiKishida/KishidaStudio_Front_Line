//=============================================================================
//
// �J�������� [camera.cpp]
// Author : Takuto Ishida
//
//=============================================================================
#include "camera.h"
#include "manager.h"
#include "input.h"
#include "renderer.h"
#include "debugProc.h"
#include "player.h"
#include "game.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define CAMERA_MOVE_HEIGHT	(100.0f)		// �v���C���[�Ƃ̍��ō����𓮂���
#define CAMERA_HEIGHT_SPEED	(0.1f)			// �����𓮂������x
#define CAMERA_ZOOM_LENGTH	(-500.0f)		// �Y�[�������Ƃ��̋���
#define CAMERA_LENGTH		(-100.0f)		// �J�����̋���
#define CAMERA_ZOOM			(5.0f)			// �Y�[�����鑬�x

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CCamera::CCamera()
{
	m_posV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posVDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posRDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vecU = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fLength = 0.0f;
	m_fLookHeight = 0.0f;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CCamera::~CCamera()
{
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CCamera::Init(void)
{
	m_posV = D3DXVECTOR3(0.0f, 0.0f, CAMERA_LENGTH);
	m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fLength = m_posV.z;
	m_fLookHeight = 60.0f;

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CCamera::Uninit(void)
{
}

//=============================================================================
// �X�V����
//=============================================================================
void CCamera::Update(void)
{
	if (CManager::GetMode() == CManager::MODE_GAME)
	{
		if (CManager::GetGame()->GetPart() == CGame::PART_ACTION)
		{// �A�N�V�����p�[�g
			// �}�E�X�̓��͏����擾
			CInputMouse *pInputMouse = CManager::GetInputMouse();

			// �}�E�X���W�̑O��Ƃ̍��������߂�
			float fDiffX = (float)pInputMouse->GetDiffPointX();
			float fDiffY = (float)pInputMouse->GetDiffPointY();

			// �v���C���[�̎擾
			CPlayer *pPlayer = CManager::GetGame()->GetPlayer();
			D3DXVECTOR3 pos = pPlayer->GetPos();

			// ������ς���
			m_rot.y += fDiffX * 0.01f;	// ������
			m_rot.x += fDiffY * 0.01f;	// �c����

			// �����̒���
			if (m_rot.y > D3DX_PI) { m_rot.y -= D3DX_PI * 2.0f; }
			if (m_rot.y < -D3DX_PI) { m_rot.y += D3DX_PI * 2.0f; }

			// �����̒���
			if (m_rot.x > D3DX_PI) { m_rot.x -= D3DX_PI * 2.0f; }
			if (m_rot.x < -D3DX_PI) { m_rot.x += D3DX_PI * 2.0f; }

			// x���̉�]�̐���
			if (D3DX_PI * 0.4f > m_rot.x)
			{// �㑤
				m_rot.x = D3DX_PI * 0.4f;
			}
			else if (D3DX_PI * 0.55f < m_rot.x)
			{// ����
				m_rot.x = D3DX_PI * 0.55f;
			}

			// �����_
			m_posR.x = pos.x - sinf(m_rot.y) * m_fLength;
			m_posR.z = pos.z - cosf(m_rot.y) * m_fLength;
			m_posR.y = pos.y - cosf(m_rot.x) * m_fLength + m_fLookHeight;

			// ���_
			m_posV.x = pos.x + sinf(m_rot.y) * m_fLength;
			m_posV.z = pos.z + cosf(m_rot.y) * m_fLength;
			m_posV.y = pos.y + cosf(m_rot.x) * m_fLength + m_fLookHeight;
		}
		else
		{// �X�g���e�W�[�p�[�g
			m_posV = D3DXVECTOR3(0.0f, 1000.0f, -100.0f);
			m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			m_rot.y = D3DX_PI;
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CCamera::Draw(void)
{
}

//=============================================================================
// �J�����̐ݒ菈��
//=============================================================================
void CCamera::Set(void)
{
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();		// �f�o�C�X�̎擾

	// �v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxProjection);

	CGame *pGame = CManager::GetGame();
	if (NULL != pGame)
	{// �v���W�F�N�V�����}�g���b�N�X���쐬
		if (pGame->GetPart() == CGame::PART_ACTION)
		{// �A�N�V�����p�[�g
			D3DXMatrixPerspectiveFovLH(&m_mtxProjection,
				D3DXToRadian(45.0f),							// ��p
				(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,		// �A�X�y�N�g��
				10.0f,											// �J�����͈̔́i��O�j
				8000.0f);										// �J�����͈̔́i���s�j
		}
		else if(pGame->GetPart() == CGame::PART_STRATEGY)
		{// �X�g���e�W�[�p�[�g
			D3DXMatrixOrthoLH(&m_mtxProjection, (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT, 10.0f, 8000.0f);
		}
	}
	else
	{// �v���W�F�N�V�����}�g���b�N�X���쐬
		D3DXMatrixPerspectiveFovLH(&m_mtxProjection,
			D3DXToRadian(45.0f),							// ��p
			(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,		// �A�X�y�N�g��
			10.0f,											// �J�����͈̔́i��O�j
			8000.0f);										// �J�����͈̔́i���s�j
	}
	// �v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);

	// �r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxView);

	// �r���[�}�g���b�N�X���쐬
	D3DXMatrixLookAtLH(&m_mtxView,
		&m_posV,
		&m_posR,
		&m_vecU);

	// �r���[�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);
}
