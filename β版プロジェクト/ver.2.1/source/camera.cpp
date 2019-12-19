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
#include "tutorial.h"
#include "server.h"
#include "menu.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define CAMERA_LENGTH		(-100.0f)		// �J�����̋���
#define CAMERA_WHOLE_LENGTH		(-4500.0f)		// �J�������S�̂����n���̋���

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
	m_fZoom = 0.0f;
	m_fCameraSpeed = 0.0f;
	m_nCntStarting = 0;
	m_stageCamera = STAGE_CAMERA_STRAIGHT;
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
	m_fZoom = 1.0f;
	m_fCameraSpeed = 0.0042f;		// �J�����̏������x�ݒ�
	m_nCntStarting = 0;
	m_stageCamera = STAGE_CAMERA_STRAIGHT;

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
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();	// �L�[�{�[�h�̓��͂��擾
	CInputMouse *pInputMouse = CManager::GetInputMouse();	// �}�E�X�̓��͏����擾

	if (CManager::GetMode() == CManager::MODE_RESULT)
	{
		m_posV = D3DXVECTOR3(78.9f, 40.0f, -212.4f);
		m_posR = D3DXVECTOR3(-700.0f, 0.0f, 500.0f);
	}

	if (CManager::GetMode() == CManager::MODE_GAME)
	{//���[�h���Q�[���̏ꍇ
		if (CManager::GetGame()->GetGameState() == CGame::STATE_STARTING)
		{//�Q�[���̏�Ԃ��J�n�O�̏ꍇ
			if (m_nCntStarting == 0)
			{
				m_posV = D3DXVECTOR3(-2500.0f, 50.0f, 0.0f);
				m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			}
			if (m_nCntStarting == 200)
			{//200�ɃJ�E���^�[���B�����ꍇ
				m_stageCamera = STAGE_CAMERA_TURN;	//��]��Ԃɂ���
				m_posV = D3DXVECTOR3(0.0f, 400.0f, -0.0f);
				m_posR = D3DXVECTOR3(-110.0f, 500.0f, -1000.0f);
			}
			if (m_nCntStarting == 500)
			{//500�ɃJ�E���^�[���B�����ꍇ
				m_stageCamera = STAGE_CAMERA_OVERLOOK;	//�����낵��Ԃɂ���
				m_posV = D3DXVECTOR3(1500.0f, 50.0f, 0.0f);
				m_posR = D3DXVECTOR3(-0.0f, 50.0f, -0.0f);
				//m_posV = D3DXVECTOR3(2500.0f, 800.0f, 0.0f);
				//m_posR = D3DXVECTOR3(-0.0f, -50.0f, -0.0f);
				//m_rot.y = D3DX_PI * 0.5f;

			}
			if (m_nCntStarting == 900)
			{
				m_stageCamera = STAGE_CAMERA_STOP;	//��~��Ԃɂ���

				//�Q�[�����J�n�O�t�F�[�h�A�E�g�ɂ���
				CManager::GetGame()->SetGameState(CGame::STATE_STARTING_FADE_OUT);
			}

			m_nCntStarting++;	//�J�E���^�[�̉��Z

			//�J�����ʂ̏�������
			switch (m_stageCamera)
			{
			case STAGE_CAMERA_STRAIGHT:
				m_posV.x += 2.5f;
				m_posR.x += 2.5f;
				break;

			case STAGE_CAMERA_TURN:
				m_rot.y -= 0.005f;
				if (m_rot.y >= D3DX_PI)
				{
					m_rot.y = -D3DX_PI;
				}
				m_posV.x = m_posR.x - sinf(m_rot.y + (D3DX_PI * 0.25f)) * - 750.0f;
				m_posV.z = m_posR.z - cosf(m_rot.y + (D3DX_PI * 0.25f)) * -750.0f;
				break;

			case STAGE_CAMERA_OVERLOOK:
				m_posV.x += 5.0f;
				m_posV.y += 2.5f;
				m_posR.y -= 0.25f;
				/*m_rot.y -= 0.005f;
				if (m_rot.y >= D3DX_PI)
				{
					m_rot.y = -D3DX_PI;
				}
				m_posV.x = m_posR.x - sinf(m_rot.y + (D3DX_PI * 0.5f)) * -2000.0f;
				m_posV.z = m_posR.z - cosf(m_rot.y + (D3DX_PI * 0.5f)) * -2000.0f;*/
				break;
			}
		}

		if (CManager::GetGame()->GetGameState() == CGame::STATE_NORMAL || CManager::GetGame()->GetGameState() == CGame::STATE_STARTING_FADE_IN)
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
				bool bChat = pPlayer->GetChatBotton();
				bool bOption = pPlayer->GetOption();							// �I�v�V������
				int nSelectButton = pPlayer->GetSelectOption();			// �J�������x�̎擾

				switch (nSelectButton)
				{
				case 0:		// �Œᑬ
					m_fCameraSpeed = 0.0021f;
					break;

				case 1:		// �ᑬ
					m_fCameraSpeed = 0.0032f;
					break;

				case 2:		// ����
					m_fCameraSpeed = 0.0042f;
					break;

				case 3:		// ����
					m_fCameraSpeed = 0.0052f;
					break;

				case 4:		// �ō���
					m_fCameraSpeed = 0.0062f;
					break;
				}

				if (pPlayer->GetRespawn() == CPlayer::RESPAWN_NONE && bChat == false && bOption == false && !pPlayer->GetWince())
				{
					// �}�E�X���W�̑O��Ƃ̍��������߂�
					float fDiffX = (float)pInputMouse->GetDiffPointX();
					float fDiffY = (float)pInputMouse->GetDiffPointY();

					// ������ς���
					m_rot.y += fDiffX * m_fCameraSpeed;	// ������
					m_rot.x += fDiffY * m_fCameraSpeed;	// �c����
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
				float moveX = (float)pInputMouse->GetDiffPointX();
				float moveY = (float)pInputMouse->GetDiffPointY();

				// �}�b�v�̃X�N���[��
				if (pInputMouse->GetPress(CInputMouse::DIMS_BUTTON_1))
				{// ��CTRL�ƍ��N���b�N�ŃX�N���[��
					m_posVDest.x -= (moveX / m_fZoom) * 2.0f;
					m_posVDest.z += (moveY / m_fZoom) * 2.0f;

					m_posRDest.x -= (moveX / m_fZoom) * 2.0f;
					m_posRDest.z += (moveY / m_fZoom) * 2.0f;
				}

				// �z�C�[���Ŋg��E�k��
				if (pInputMouse->GetMouseAxisZ() >= 120.0f && m_fZoom < 3.0f)
				{// �g��
					m_fZoom++;
				}
				else if (pInputMouse->GetMouseAxisZ() <= -120.0f && m_fZoom > 1.0f)
				{// �k��
					m_fZoom--;
				}

				// �����_�̍X�V
				m_posR.x = m_posRDest.x - sinf(D3DX_PI) * 0.1f;
				m_posR.y = m_posRDest.y + 100.0f;
				m_posR.z = m_posRDest.z - cosf(D3DX_PI) * 0.1f;

				// ���_�̍X�V
				m_posV.x = m_posVDest.x + sinf(D3DX_PI) * -0.01f;
				m_posV.y = m_posVDest.y + 500.0f;
				m_posV.z = m_posVDest.z + cosf(D3DX_PI) * -0.01f;
			}
		}
	}
	else if (CManager::GetMode() == CManager::MODE_TUTORIAL)
	{
		if (CManager::GetTutorial()->GetPart() == CTutorial::PART_ACTION)
		{// �A�N�V�����p�[�g
			CPlayer *pPlayer = NULL;

			// �v���C���[�̎擾
			pPlayer = CManager::GetTutorial()->GetPlayer(0);

			D3DXVECTOR3 pos = pPlayer->GetPos();
			float fModelHeight = pPlayer->GetVtxMax().y;
			bool bChat = pPlayer->GetChatBotton();
			bool bOption = pPlayer->GetOption();							// �I�v�V������
			int nSelectButton = pPlayer->GetSelectOption();			// �J�������x�̎擾

			switch (nSelectButton)
			{
			case 0:		// �Œᑬ
				m_fCameraSpeed = 0.0021f;
				break;

			case 1:		// �ᑬ
				m_fCameraSpeed = 0.0032f;
				break;

			case 2:		// ����
				m_fCameraSpeed = 0.0042f;
				break;

			case 3:		// ����
				m_fCameraSpeed = 0.0052f;
				break;

			case 4:		// �ō���
				m_fCameraSpeed = 0.0062f;
				break;
			}

			if (pPlayer->GetRespawn() == CPlayer::RESPAWN_NONE && bChat == false && bOption == false && !pPlayer->GetWince())
			{
				// �}�E�X���W�̑O��Ƃ̍��������߂�
				float fDiffX = (float)pInputMouse->GetDiffPointX();
				float fDiffY = (float)pInputMouse->GetDiffPointY();

				// ������ς���
				m_rot.y += fDiffX * m_fCameraSpeed;	// ������
				m_rot.x += fDiffY * m_fCameraSpeed;	// �c����
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
			float moveX = (float)pInputMouse->GetDiffPointX();
			float moveY = (float)pInputMouse->GetDiffPointY();

			// �}�b�v�̃X�N���[��
			if (pInputMouse->GetPress(CInputMouse::DIMS_BUTTON_1))
			{// ��CTRL�ƍ��N���b�N�ŃX�N���[��
				m_posVDest.x -= (moveX / m_fZoom) * 2.0f;
				m_posVDest.z += (moveY / m_fZoom) * 2.0f;

				m_posRDest.x -= (moveX / m_fZoom) * 2.0f;
				m_posRDest.z += (moveY / m_fZoom) * 2.0f;
			}

			// �z�C�[���Ŋg��E�k��
			if (pInputMouse->GetMouseAxisZ() >= 120.0f && m_fZoom < 3.0f)
			{// �g��
				m_fZoom++;
			}
			else if (pInputMouse->GetMouseAxisZ() <= -120.0f && m_fZoom > 1.0f)
			{// �k��
				m_fZoom--;
			}

			// �����_�̍X�V
			m_posR.x = m_posRDest.x - sinf(D3DX_PI) * 0.1f;
			m_posR.y = m_posRDest.y + 100.0f;
			m_posR.z = m_posRDest.z - cosf(D3DX_PI) * 0.1f;

			// ���_�̍X�V
			m_posV.x = m_posVDest.x + sinf(D3DX_PI) * -0.01f;
			m_posV.y = m_posVDest.y + 500.0f;
			m_posV.z = m_posVDest.z + cosf(D3DX_PI) * -0.01f;
		}
	}
	CDebugProc::Print("���_%.2f %.2f %.2f", m_posV.x, m_posV.y, m_posV.z);

	CDebugProc::Print("�����_%.2f %.2f %.2f", m_posR.x, m_posR.y, m_posR.z);

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
	if (NULL != CManager::GetGame())
	{// �v���W�F�N�V�����}�g���b�N�X���쐬
		if (CManager::GetGame()->GetPart() == CGame::PART_ACTION)
		{// �A�N�V�����p�[�g
			D3DXMatrixPerspectiveFovLH(&m_mtxProjection,
				D3DXToRadian(45.0f),							// ��p
				(float)m_viewport.Width / (float)m_viewport.Height,		// �A�X�y�N�g��
				0.1f,											// �J�����͈̔́i��O�j
				12000.0f);										// �J�����͈̔́i���s�j
		}
		else if (CManager::GetGame()->GetPart() == CGame::PART_STRATEGY)
		{// �X�g���e�W�[�p�[�g
			if (0 == nIdxCamera)
			{
				D3DXMatrixOrthoLH(&m_mtxProjection,
					(float)m_viewport.Width / m_fZoom * PARALLEL_PROJECTION_SIZE,
					(float)m_viewport.Height / m_fZoom * PARALLEL_PROJECTION_SIZE,
					1.0f,
					1.0f);
			}
			else
			{
				D3DXMatrixOrthoLH(&m_mtxProjection,
					(float)m_viewport.Width / m_fZoom * PARALLEL_PROJECTION_SIZE,
					(float)m_viewport.Height / m_fZoom * PARALLEL_PROJECTION_SIZE,
					10.0f,
					8000.0f);
			}
		}
	}
	else if (NULL != CManager::GetTutorial())
	{// �v���W�F�N�V�����}�g���b�N�X���쐬
		if (CManager::GetTutorial()->GetPart() == CGame::PART_ACTION)
		{// �A�N�V�����p�[�g
			D3DXMatrixPerspectiveFovLH(&m_mtxProjection,
				D3DXToRadian(45.0f),							// ��p
				(float)m_viewport.Width / (float)m_viewport.Height,		// �A�X�y�N�g��
				0.1f,											// �J�����͈̔́i��O�j
				8000.0f);										// �J�����͈̔́i���s�j
		}
		else if (CManager::GetTutorial()->GetPart() == CGame::PART_STRATEGY)
		{// �X�g���e�W�[�p�[�g
			if (0 == nIdxCamera)
			{
				D3DXMatrixOrthoLH(&m_mtxProjection,
					(float)m_viewport.Width / m_fZoom * PARALLEL_PROJECTION_SIZE,
					(float)m_viewport.Height / m_fZoom * PARALLEL_PROJECTION_SIZE,
					1.0f,
					1.0f);
			}
			else
			{
				D3DXMatrixOrthoLH(&m_mtxProjection,
					(float)m_viewport.Width / m_fZoom * 2.5f,
					(float)m_viewport.Height / m_fZoom * 2.5f,
					10.0f,
					8000.0f);
			}
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