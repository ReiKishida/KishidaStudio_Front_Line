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
#include "server.h"
#include "menu.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define CAMERA_LENGTH		(-100.0f)		// �J�����̋���

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
	m_viewport.Height = SCREEN_HEIGHT;
	m_viewport.Width = SCREEN_WIDTH;
	m_viewport.MaxZ = 1.0f;
	m_viewport.MinZ = 0.0f;
	m_viewport.X = 0;
	m_viewport.Y = 0;
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
	m_rot = D3DXVECTOR3(D3DX_PI * 0.5f, 0.0f, 0.0f);
	m_fLength = m_posV.z;
	m_fLookHeight = 25.0f;

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
			CPlayer *pPlayer = NULL;

			// �v���C���[�̎擾
			if (CMenu::GetMode() == CMenu::MODE_MULTI)
			{// �}���`
				if (CManager::GetClient() != NULL && CManager::GetClient()->GetConnect() == true)
				{
					pPlayer = CManager::GetGame()->GetPlayer(CManager::GetClient()->GetPlayerIdx());
				}
				else
				{
					pPlayer = CManager::GetGame()->GetPlayer(0);
				}
			}
			else
			{// �V���O��
				pPlayer = CManager::GetGame()->GetPlayer(0);
			}

			D3DXVECTOR3 pos = pPlayer->GetPos();
			float fModelHeight = pPlayer->GetVtxMax().y;

			if (pPlayer->GetRespawn() == CPlayer::RESPAWN_NONE)
			{
				// �}�E�X�̓��͏����擾
				CInputMouse *pInputMouse = CManager::GetInputMouse();

				// �}�E�X���W�̑O��Ƃ̍��������߂�
				float fDiffX = (float)pInputMouse->GetDiffPointX();
				float fDiffY = (float)pInputMouse->GetDiffPointY();

				// ������ς���
				m_rot.y += fDiffX * 0.0062f;	// ������
				m_rot.x += fDiffY * 0.0062f;	// �c����
			}

			// �����̒���
			if (m_rot.y > D3DX_PI) { m_rot.y -= D3DX_PI * 2.0f; }
			if (m_rot.y < -D3DX_PI) { m_rot.y += D3DX_PI * 2.0f; }

			// �����̒���
			if (m_rot.x > D3DX_PI) { m_rot.x -= D3DX_PI * 2.0f; }
			if (m_rot.x < -D3DX_PI) { m_rot.x += D3DX_PI * 2.0f; }

			// x���̉�]�̐���
			if (D3DX_PI * 0.3f > m_rot.x)
			{// �㑤
				m_rot.x = D3DX_PI * 0.3f;
			}
			else if (D3DX_PI * 0.55f < m_rot.x)
			{// ����
				m_rot.x = D3DX_PI * 0.55f;
			}

			// �����_
			m_posR.x = pos.x - sinf(m_rot.y) * m_fLength;
			m_posR.z = pos.z - cosf(m_rot.y) * m_fLength;
			m_posR.y = pos.y - cosf(m_rot.x) * m_fLength + fModelHeight + m_fLookHeight;

			// ���_
			m_posV.x = pos.x + sinf(m_rot.y) * m_fLength;
			m_posV.z = pos.z + cosf(m_rot.y) * m_fLength;
			m_posV.y = pos.y + cosf(m_rot.x) * m_fLength + fModelHeight + m_fLookHeight;
		}
		else
		{// �X�g���e�W�[�p�[�g
			m_posV = D3DXVECTOR3(0.0f, 500.0f, -0.01f);
			m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			//m_rot.y = D3DX_PI;
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
void CCamera::Set(int nIdxCamera)
{
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();		// �f�o�C�X�̎擾

											// �v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxProjection);

	switch (nIdxCamera)
	{
	case 0:
		m_viewport.Width = SCREEN_WIDTH;
		m_viewport.Height = SCREEN_HEIGHT;
		m_viewport.MaxZ = 1.0f;
		m_viewport.MinZ = 0.0f;
		m_viewport.X = 0;
		m_viewport.Y = 0;
		break;

	case 1:
		m_viewport.Width = 750;
		m_viewport.Height = 450;
		m_viewport.MaxZ = 1.0f;
		m_viewport.MinZ = 0.0f;
		m_viewport.X = SCREEN_WIDTH - 760;
		m_viewport.Y = SCREEN_HEIGHT - 470;
		break;
	}

	// �r���[�|�[�g�̐ݒ�
	pDevice->SetViewport(&m_viewport);

	CGame *pGame = CManager::GetGame();
	if (NULL != pGame)
	{// �v���W�F�N�V�����}�g���b�N�X���쐬
		if (pGame->GetPart() == CGame::PART_ACTION)
		{// �A�N�V�����p�[�g
			D3DXMatrixPerspectiveFovLH(&m_mtxProjection,
				D3DXToRadian(45.0f),							// ��p
				(float)m_viewport.Width / (float)m_viewport.Height,		// �A�X�y�N�g��
				10.0f,											// �J�����͈̔́i��O�j
				8000.0f);										// �J�����͈̔́i���s�j
		}
		else if (pGame->GetPart() == CGame::PART_STRATEGY)
		{// �X�g���e�W�[�p�[�g
			D3DXMatrixOrthoLH(&m_mtxProjection, (float)m_viewport.Width, (float)m_viewport.Height, 10.0f, 8000.0f);
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

//=============================================================================
// �X�N���[�����W�����[���h���W�ɕϊ�
//=============================================================================
D3DXVECTOR3 CCamera::CalcScreenToWorld(int nScreenX, int nScreenY, float fZ)
{
	// �e�s��̋t�s����Z�o
	D3DXMATRIX InvView, InvPrj, VP, InvViewport;
	D3DXMatrixInverse(&InvView, NULL, &m_mtxView);
	D3DXMatrixInverse(&InvPrj, NULL, &m_mtxProjection);
	D3DXMatrixIdentity(&VP);
	VP._11 = 750.0f / 2.0f + (SCREEN_WIDTH - 760); VP._22 = -450.0f / 2.0f + (SCREEN_HEIGHT - 470);
	VP._41 = 750.0f / 2.0f + (SCREEN_WIDTH - 760); VP._42 = 450.0f / 2.0f + (SCREEN_HEIGHT - 470);
	D3DXMatrixInverse(&InvViewport, NULL, &VP);

	// �t�ϊ�
	D3DXMATRIX tmp = InvViewport * InvPrj * InvView;
	D3DXVECTOR3 out;
	D3DXVec3TransformCoord(&out, &D3DXVECTOR3((float)nScreenX, (float)nScreenY, fZ), &tmp);

	return out;
}

//=============================================================================
// XZ���ʂƃX�N���[�����W�̌�_�Z�o�֐�
//=============================================================================
D3DXVECTOR3 CCamera::CalcScreenToXZ(int nScreenX, int nScreenY)
{
	D3DXVECTOR3 nearpos;
	D3DXVECTOR3 farpos;
	D3DXVECTOR3 ray;
	D3DXVECTOR3 out;
	nearpos = CalcScreenToWorld(nScreenX, nScreenY, 0.0f);
	farpos = CalcScreenToWorld(nScreenX, nScreenY, 1.0f);
	ray = farpos - nearpos;
	D3DXVec3Normalize(&ray, &ray);

	if (ray.y <= 0)
	{// ����_
		float Lray = D3DXVec3Dot(&ray, &D3DXVECTOR3(0, 1, 0));
		float LP0 = D3DXVec3Dot(&(-nearpos), &D3DXVECTOR3(0, 1, 0));
		out = nearpos + (LP0 / Lray)*ray;
	}
	else
	{// �������Ȃ�����
		out = farpos;
	}

	return D3DXVECTOR3(out.x, out.y, -out.z);
}

//=============================================================================
// �r���[�|�[�g�̎擾
//=============================================================================
D3DVIEWPORT9 CCamera::GetViewport(int nIdxCamera)
{
	D3DVIEWPORT9 viewport;
	switch (nIdxCamera)
	{
	case 0:
		viewport.Width = SCREEN_WIDTH;
		viewport.Height = SCREEN_HEIGHT;
		viewport.MaxZ = 1.0f;
		viewport.MinZ = 0.0f;
		viewport.X = 0;
		viewport.Y = 0;
		break;

	case 1:
		viewport.Width = 750;
		viewport.Height = 450;
		viewport.MaxZ = 1.0f;
		viewport.MinZ = 0.0f;
		viewport.X = SCREEN_WIDTH - 760;
		viewport.Y = SCREEN_HEIGHT - 470;
		break;
	}

	return viewport;
}

void CCamera::GetInfo(D3DVIEWPORT9 *pViewport, D3DXMATRIX *pMtxProjection, D3DXMATRIX *pMtxView)
{
	m_viewport.Width = 750;
	m_viewport.Height = 450;
	m_viewport.MaxZ = 1.0f;
	m_viewport.MinZ = 0.0f;
	m_viewport.X = SCREEN_WIDTH - 760;
	m_viewport.Y = SCREEN_HEIGHT - 470;

	*pViewport = m_viewport;

	// �v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxProjection);
	D3DXMatrixOrthoLH(&m_mtxProjection, (float)m_viewport.Width, (float)m_viewport.Height, 10.0f, 8000.0f);

	*pMtxProjection = m_mtxProjection;

	// �r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxView);

	// �r���[�}�g���b�N�X���쐬
	D3DXMatrixLookAtLH(&m_mtxView,
		&m_posV,
		&m_posR,
		&m_vecU);

	*pMtxView = m_mtxView;
}