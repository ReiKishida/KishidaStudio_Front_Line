//=============================================================================
//
// �}�E�X�J�[�\������ [mouseCursor.cpp]
// Author : Komatsu Keisuke
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "mouseCursor.h"
#include "manager.h"
#include "renderer.h"
#include "debugproc.h"
#include "input.h"
#include "camera.h"
#include "game.h"
#include "enemy.h"
#include "player.h"
#include "nodePointer.h"
#include "scene2D.h"
#include "texture.h"
#include "AI.h"
#include "menu.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define	LOAD_FILENAME	("data/TEXT/NODE_DATA/NodeData.txt")	// �ǂݍ��ރt�@�C���̃p�X
#define MOUSE_WIDTH		(30.0f)		// �}�E�X�e�N�X�`���̕�
#define MOUSE_HEIGHT	(30.0f)		// �}�E�X�e�N�X�`���̍���

//=============================================================================
// �ÓI�����o�ϐ�
//=============================================================================
CCamera *CMouseCursor::m_pCamera = NULL;
CNodePointer *CMouseCursor::m_pNodePointer[NODEPOINT_MAX] = {};
D3DXVECTOR3 CMouseCursor2D::m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

/*==============================================================*/
/*					3D�}�E�X�J�[�\���̃N���X					*/
/*                  Author : Komatsu Keisuke                    */
/*==============================================================*/
//=============================================================================
// ��������
//=============================================================================
CMouseCursor  *CMouseCursor::Create()
{
	CMouseCursor *pMouseCursor = NULL;

	if (pMouseCursor == NULL)
	{
		pMouseCursor = new CMouseCursor;

		if (pMouseCursor != NULL)
		{
			pMouseCursor->Init();
		}
	}

	return pMouseCursor;
}

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CMouseCursor::CMouseCursor(int nPriority, CScene::OBJTYPE objType) : CScene3D(nPriority, objType) {}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CMouseCursor::~CMouseCursor() {}

//=============================================================================
// ����������
//=============================================================================
HRESULT CMouseCursor::Init()
{
	CScene3D::Init();

	// �}�b�v���̏�����
	m_LoadNodeData = {};

	// ���̏�����
	m_pCamera = CManager::GetCamera();
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_setpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR3(MOUSE_WIDTH, 0.0f, MOUSE_HEIGHT) * 3.0f;
	m_nNodeCounter = 0;

	// �}�E�X�J�[�\���̐ݒ�
	CScene3D::SetSize(m_size);
	CScene3D::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	CScene3D::BindTexture(CTexture::GetTexture(CTexture::TEXTURE_MOUSECURSOR));

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CMouseCursor::Uninit(void)
{
	for (int nCntNode = 0; nCntNode < m_LoadNodeData.nodeMax; nCntNode++)
	{// �S�m�[�h�������
		if (m_pNodePointer[nCntNode] != NULL)
		{// �����p�|���S����NULL����Ȃ�
			m_pNodePointer[nCntNode]->Uninit();
			m_pNodePointer[nCntNode] = NULL;
		}
	}

	CScene3D::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CMouseCursor::Update(void)
{
	//CDebugProc::Print("=====�}�E�X�p3D�|���S��======\n");
	//CDebugProc::Print("3D�}�b�v��̃}�E�X�̍��W�F%.2f %.2f", m_pos.x, m_pos.z);
	//CDebugProc::Print("�X�N���[����̃}�E�X���W�F%.2f %.2f", (float)pMouse->GetPoint().x, (float)pMouse->GetPoint().y);
	//CDebugProc::Print("�G�f�B�^�[��̃}�E�X�̍��W�F%.2f %.2f", m_setpos.x, m_setpos.z);

	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();	// �L�[�{�[�h�̓��͂��擾
	CInputMouse *pMouse = CManager::GetInputMouse();	// �}�E�X�̓��͂��擾

	// �Q�[�����}�E�X����
	CMouseCursor::Mouse(pMouse);

#ifdef _DEBUG
	// �t�@�C���֌W����
	CMouseCursor::File(pKeyboard);
#endif
}

//=============================================================================
// �`�揈��
//=============================================================================
void CMouseCursor::Draw(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// ���C�e�B���OOFF
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	CScene3D::Draw();

	// ���C�e�B���OON
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//=============================================================================
// �Q�[�����}�E�X����
//=============================================================================
void CMouseCursor::Mouse(CInputMouse *pMouse)
{
	D3DXVECTOR3 MousePos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DVIEWPORT9 viewport;
	D3DXMATRIX matrix, proj, view;

	if (m_pCamera != NULL)
	{// �J������Null����Ȃ�
		// �X�N���[�����W�����[���h���W�֕ϊ�
		MousePos = D3DXVECTOR3((float)pMouse->GetPoint().x, (float)pMouse->GetPoint().y, 0.0f);
		viewport = m_pCamera->GetViewport(1);
		D3DXMatrixIdentity(&matrix);
		m_pCamera->GetInfo(&viewport, &proj, &view);
		D3DXVec3Unproject(&MousePos, &MousePos, &viewport, &proj, &view, &matrix);

		// �J�[�\���ʒu�̐ݒ�
		m_pos.x = MousePos.x * (PARALLEL_PROJECTION_SIZE / m_pCamera->GetZoom());
		m_pos.y = 0.0f;
		m_pos.z = MousePos.z * (PARALLEL_PROJECTION_SIZE / m_pCamera->GetZoom());

		switch ((int)m_pCamera->GetZoom())
		{
		case 1:
			m_pos.x -= m_pCamera->GetPosR().x * 5.0f;
			m_pos.z -= m_pCamera->GetPosR().z * 5.0f;
			break;
		case 2:
			m_pos.x -= m_pCamera->GetPosR().x * 2.0f;
			m_pos.z -= m_pCamera->GetPosR().z * 2.0f;
			break;
		case 3:
			m_pos.x -= m_pCamera->GetPosR().x;
			m_pos.z -= m_pCamera->GetPosR().z;
			break;
		}

		// �ʒu�E�T�C�Y�̐ݒ�
		CScene3D::SetMousePos(m_pos);
		CScene3D::SetSize((m_size / m_pCamera->GetZoom()));
	}

	if (CManager::GetGame()->GetPart() == CGame::PART_STRATEGY)
	{// �X�g���e�W�[�p�[�g���̂݃|�W�V������ݒ�
		m_setpos = D3DXVECTOR3(((int)(m_pos.x / 38)) * 38.0f, 0.0f, ((int)(m_pos.z / 36)) * 36.0f);
	}
}

//=============================================================================
// �t�@�C���֌W����
//=============================================================================
void CMouseCursor::File(CInputKeyboard *pKeyboard)
{
	bool bSet = false;

	if (pKeyboard->GetTrigger(DIK_F11) == true)
	{// F11�L�[�Ńt�@�C�������[�h
		// �O��̃f�[�^�̍폜
		for (int nCntNode = 0; nCntNode < m_LoadNodeData.nodeMax; nCntNode++)
		{// �S�m�[�h�������
			if (m_pNodePointer[nCntNode] != NULL)
			{// �f�[�^�������Ă���ꍇ
				m_nNodeCounter = 0;
				m_pNodePointer[nCntNode]->GetNodeMax() = 0;
				m_pNodePointer[nCntNode]->GetConnectMax() = 0;
				m_pNodePointer[nCntNode]->Uninit();
				m_pNodePointer[nCntNode] = NULL;
			}
		}

		// �t�@�C�����\�h
		CMouseCursor::FileLoad(LOAD_FILENAME);

		// ���[�h�����f�[�^�����ƂɃm�[�h��z�u
		for (int nCntNode = 0; nCntNode < m_LoadNodeData.nodeMax; nCntNode++)
		{// �S�m�[�h�������
			if (m_pNodePointer[nCntNode] == NULL)
			{// �m�[�h�����p�|���S����NULL����Ȃ�
				m_pNodePointer[nCntNode] = m_pNodePointer[nCntNode]->Create(m_LoadNodeData.pos[nCntNode]);
				m_pNodePointer[nCntNode]->GetNodeMax()++;
				m_LoadNodeData.index[nCntNode] = m_pNodePointer[nCntNode]->GetMyNumber();

				for (int nCntConnectNode = 0; nCntConnectNode < m_LoadNodeData.connectNum[nCntNode]; nCntConnectNode++)
				{// �ڑ��m�[�h�������
					m_pNodePointer[nCntNode]->GetConnectMax()++;
					m_pNodePointer[nCntNode]->GetConnect(nCntConnectNode) = m_LoadNodeData.connectIndex[nCntNode][nCntConnectNode];
				}

				m_nNodeCounter = m_pNodePointer[nCntNode]->GetNodeMax();
			}
		}
	}

	if (pKeyboard->GetTrigger(DIK_F9) == true)
	{// 9�L�[��	�����p�|���S����j��
		for (int nCntNode = 0; nCntNode < m_LoadNodeData.nodeMax; nCntNode++)
		{// �S�m�[�h�������
			if (m_pNodePointer[nCntNode] != NULL)
			{// �����p�|���S����NULL����Ȃ�
				m_pNodePointer[nCntNode]->Uninit();
				m_pNodePointer[nCntNode] = NULL;
			}
		}
	}
}

//=============================================================================
// �t�@�C���̓ǂݍ���
//=============================================================================
void CMouseCursor::FileLoad(char* pFileName)
{
	FILE* pFile = NULL;		// �t�@�C���|�C���^
	char ReadText[256];		// �ǂݍ��񂾕���������Ă���
	char HeadText[256];		// ��r�p
	char DustBox[256];		// �g�p���Ȃ����̂����Ă���
	int nCount = 0;
	int nCntIndex = 0;

	// �ꎞ�f�[�^�x�[�X
	std::vector<NODE_LOAD_STATE> LoadState; LoadState.clear();

	// ������
	NODE_LOAD_STATE OneState = {};

	// �t�@�C���I�[�v��
	pFile = fopen(pFileName, "r");

	if (pFile != NULL)
	{// �t�@�C�����J����Ă����
		while (strcmp(HeadText, "START_LOAD") != 0)
		{// "START_LOAD" ���ǂݍ��܂��܂ŌJ��Ԃ��������ǂݎ��
			fgets(ReadText, sizeof(ReadText), pFile);
			sscanf(ReadText, "%s", &HeadText);
		}
		if (strcmp(HeadText, "START_LOAD") == 0)
		{// "START_LOAD" ���ǂݎ�ꂽ�ꍇ�A�����J�n
			while (strcmp(HeadText, "END_LOAD") != 0)
			{// "END_LOAD" ���ǂݍ��܂��܂ŌJ��Ԃ��������ǂݎ��
				fgets(ReadText, sizeof(ReadText), pFile);
				sscanf(ReadText, "%s", &HeadText);

				if (strcmp(HeadText, "\n") == 0)
				{// ������̐擪�� [\n](���s) �̏ꍇ�������Ȃ�

				}
				else if (strcmp(HeadText, "START_DATA") == 0)
				{// "START_DATA" ���ǂݎ�ꂽ�ꍇ
					nCount = 0;
					while (strcmp(HeadText, "END_DATA") != 0)
					{// "END_DATA" ���ǂݍ��܂��܂ŌJ��Ԃ��������ǂݎ��
						fgets(ReadText, sizeof(ReadText), pFile);
						sscanf(ReadText, "%s", &HeadText);

						if (strcmp(HeadText, "\n") == 0)
						{// ������̐擪�� [\n](���s) �̏ꍇ�������Ȃ�

						}
						else if (strcmp(HeadText, "NODESET") == 0)
						{// "NODESET" ���ǂݎ�ꂽ�ꍇ
							while (strcmp(HeadText, "END_NODESET") != 0)
							{// "END_NODESET" ���ǂݍ��܂��܂ŌJ��Ԃ��������ǂݎ��
								fgets(ReadText, sizeof(ReadText), pFile);
								sscanf(ReadText, "%s", &HeadText);

								if (strcmp(HeadText, "\n") == 0)
								{// ������̐擪�� [\n](���s) �̏ꍇ�������Ȃ�

								}
								else if (strcmp(HeadText, "NODE_POS") == 0)	// �m�[�h�̈ʒu
								{
									sscanf(ReadText, "%s %c %f %f %f",
										&DustBox, &DustBox,
										&OneState.pos[nCount].x,
										&OneState.pos[nCount].y,
										&OneState.pos[nCount].z);
								}
								else if (strcmp(HeadText, "CONNECT_NUM") == 0)	// �ڑ��m�[�h�̐�
								{
									sscanf(ReadText, "%s %c %d",
										&DustBox, &DustBox,
										&OneState.connectNum[nCount]);
								}
								else if (strcmp(HeadText, "CONNECT_INDEX") == 0)	// �ڑ��m�[�h�̔ԍ�
								{
									sscanf(ReadText, "%s %c %d",
										&DustBox, &DustBox,
										&OneState.connectIndex[nCount][nCntIndex]);
									nCntIndex++;
								}
							}

							OneState.index[nCount] = nCount;
							nCntIndex = 0;
							nCount++;
						}
					}
					OneState.nodeMax = nCount; // �m�[�h�̑���

											   // ��̃f�[�^��ǂݍ��񂾌�,�ꎞ�f�[�^�x�[�X�Ɋi�[
					LoadState.emplace_back(OneState);
				}
			}
		}

		// �t�@�C���N���[�Y
		if (pFile != NULL)
		{
			fclose(pFile);
			pFile = NULL;
		}
	}

	m_LoadNodeData = OneState;	// �f�[�^�̑��
}

/****************************************************************/
/*					2D�}�E�X�J�[�\���̃N���X					*/
/*                  Author : Takuto Ishida                      */
/****************************************************************/
//=============================================================================
// ��������
//=============================================================================
CMouseCursor2D  *CMouseCursor2D::Create()
{
	CMouseCursor2D *pMouseCursor = NULL;

	if (pMouseCursor == NULL)
	{
		pMouseCursor = new CMouseCursor2D;

		if (pMouseCursor != NULL)
		{
			pMouseCursor->Init();
		}
	}

	return pMouseCursor;
}

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CMouseCursor2D::CMouseCursor2D(int nPriority, CScene::OBJTYPE objType) : CScene2D(nPriority, objType) {}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CMouseCursor2D::~CMouseCursor2D() {}

//=============================================================================
// ����������
//=============================================================================
HRESULT CMouseCursor2D::Init()
{
	CScene2D::Init();

	CScene2D::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	CScene2D::BindTexture(CTexture::GetTexture(CTexture::TEXTURE_MOUSECURSOR));

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CMouseCursor2D::Uninit(void)
{
	CScene2D::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CMouseCursor2D::Update(void)
{
	CInputMouse *pMouse = CManager::GetInputMouse();	// �}�E�X�̓��͂��擾

	// �ړ�������
	Move(pMouse);
}

//=============================================================================
// �`�揈��
//=============================================================================
void CMouseCursor2D::Draw(void)
{
	CScene2D::Draw();
}

//=============================================================================
// �ړ�����
//=============================================================================
void CMouseCursor2D::Move(CInputMouse *pMouse)
{
	// �E�B���h�E�̋�`���擾
	RECT rect;
	GetWindowRect(CManager::GetRenderer()->GetHWnd(), &rect);

	// ����ɃT�C�Y�ύX���l�����āA���݂̃T�C�Y�ŕ␳�i�g�T�C�Y������̂Ō����ł͂Ȃ��j
	m_pos.x = float(pMouse->GetPoint().x - rect.left) - (MOUSE_WIDTH / 2.0f);
	m_pos.y = float(pMouse->GetPoint().y - rect.top) - (MOUSE_HEIGHT / 2.0f);

	// �ʒu�̐ݒ�
	CScene2D::SetPos(m_pos);

	// ���_���̎擾
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = CScene2D::GetVtxBuff();

	// ���_����ݒ�
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos = m_pos + D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = m_pos + D3DXVECTOR3(MOUSE_WIDTH, 0.0f, 0.0f);
	pVtx[2].pos = m_pos + D3DXVECTOR3(0.0f, MOUSE_HEIGHT, 0.0f);
	pVtx[3].pos = m_pos + D3DXVECTOR3(MOUSE_WIDTH, MOUSE_HEIGHT, 0.0f);

	//���_�o�b�t�@���A�����b�N
	pVtxBuff->Unlock();

	// ���_���̐ݒ�
	CScene2D::SetVtxBuff(pVtxBuff);
}