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

//=============================================================================
// �}�N����`
//=============================================================================
#define	LOAD_FILENAME			("data/TEXT/NODE_DATA/NodeData.txt")	// �ǂݍ��ރt�@�C���̃p�X
#define CURSOR_TEXTURE			("data/TEXTURE/reticle.png")	// �J�[�\���̃e�N�X�`��
#define MASSAGE_DISPLAY_TIME	(180)							// ���b�Z�[�W�̕\������
#define MOUSE_WIDTH				(30.0f)
#define MOUSE_HEIGHT			(30.0f)

//=============================================================================
// �ÓI�����o�ϐ�
//=============================================================================
CCamera *CMouseCursor::m_pCamera = NULL;
CNodePointer *CMouseCursor::m_pNodePointer[NODEPOINT_MAX] = {};
D3DXVECTOR3 CMouseCursor2D::m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

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
	m_SaveNodeData = {};

	// �ڑ����m�[�h�̏��̏�����
	m_SelectNodePos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_SelectNodeNumber = 0;
	m_SelectNodeConnectMax = 0;
	for (int nNode = 0; nNode < CONNECT_MAX; nNode++)
	{// �ڑ��\�ő吔�����
		m_SelectNodeConnect[nNode] = 0;
	}

	m_pCamera = CManager::GetCamera();
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR3(15.0f, 0.0f, 15.0f);
	m_bConnectMode = false;
	m_bSaveMassage = false;
	m_bErrorMassage = false;
	m_bConnectSet = true;
	m_bConnectNumSet = false;
	m_nMassageCount = MASSAGE_DISPLAY_TIME;
	m_nNodeCounter = 0;
	m_nConnectNodeCounter = 0;
	m_nSelectNode = 0;
	m_nSelectConnectNode = 0;

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
	CScene3D::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CMouseCursor::Update(void)
{
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();	// �L�[�{�[�h�̓��͂��擾
	CInputMouse *pMouse = CManager::GetInputMouse();	// �}�E�X�̓��͂��擾

	//if (m_bErrorMassage == true)
	//{// �G���[���b�Z�[�W�̕\��
	//	CDebugProc::Print("====================================\n");
	//	CDebugProc::Print("= �t�@�C���ɏ������߂܂���ł��� =\n");
	//	CDebugProc::Print("====================================\n\n");
	//}
	//else if (m_bSaveMassage == true)
	//{// �Z�[�u�������b�Z�[�W�̕\��
	//	CDebugProc::Print("===================\n");
	//	CDebugProc::Print("= �Z�[�u���튮�� =\n");
	//	CDebugProc::Print("===================\n\n");
	//}

	CDebugProc::Print("=====�}�E�X�p3D�|���S��======\n");
	CDebugProc::Print("�J�[�\���̍��W\n x : %.1f y : %.1f z : %.1f\n", m_pos.x, m_pos.y, m_pos.z);
	CDebugProc::Print("============================\n");
	//CDebugProc::Print("�z�u���Ă�m�[�h�̐�: %d\n", m_nNodeCounter);

	//CDebugProc::Print("(LSHIFT)�ڑ��ݒ胂�[�h: %s\n", m_bConnectMode ? "ON" : "OFF");

	//if (m_bConnectMode == true)
	//{// �ڑ��ݒ胂�[�h�̏ꍇ
	//	CDebugProc::Print("(SPACE)�ڑ� [%s] �ݒ胂�[�h\n", m_bConnectSet ? "��" : "��");
	//	if (m_bConnectNumSet == false)
	//	{// �ڑ����ݒ胂�[�h�̏ꍇ
	//		CDebugProc::Print("(LCLICK)�ڑ�����ݒ肷��\n");
	//	}
	//	else if (m_bConnectNumSet == true)
	//	{// �ڑ���ݒ胂�[�h�̏ꍇ
	//		CDebugProc::Print("(LCLICK)�ڑ����ݒ肷��\n");
	//		CDebugProc::Print("(RCLICK)�ڑ����1�߂�\n");
	//	}

	//	CDebugProc::Print("�m�[�h�̈ʒu x : %.1f / y : %.1f / z : %.1f\n", m_SelectNodePos.x, m_SelectNodePos.y, m_SelectNodePos.z);
	//	CDebugProc::Print("No. : %d\n", m_SelectNodeNumber);
	//	CDebugProc::Print("�ڑ��m�[�h�� : %d\n", m_SelectNodeConnectMax);
	//	for (int nCntConnect = 0; nCntConnect < m_SelectNodeConnectMax; nCntConnect++)
	//	{
	//		CDebugProc::Print("�ڑ���ԍ�[%d] : %d\n", nCntConnect, m_SelectNodeConnect[nCntConnect]);
	//	}
	//	CDebugProc::Print("\n");

	//	CDebugProc::Print("�ڑ����m�[�h�ԍ� : %d\n", m_nSelectNode);
	//	CDebugProc::Print("�ڑ���m�[�h�ԍ� : %d\n", m_nSelectConnectNode);
	//}
	//else if (m_bConnectMode == false)
	//{// �m�[�h�z�u���[�h�̏ꍇ
	//	CDebugProc::Print("(LCLICK)�m�[�h��ݒ肷��\n");
	//	CDebugProc::Print("(RCLICK)�m�[�h��1�߂�\n");
	//}

	if (m_bSaveMassage == true || m_bErrorMassage == true)
	{// ���b�Z�[�W�\�����Ԃ̌���
		m_nMassageCount--;

		if (m_nMassageCount <= 0)
		{// ���b�Z�[�W�̔�\��
			m_bSaveMassage = false;
			m_bErrorMassage = false;
			m_nMassageCount = MASSAGE_DISPLAY_TIME;
		}
	}

	// ���͎�����
	CMouseCursor::Input(pKeyboard, pMouse);

	// �ړ�������
	CMouseCursor::Move(pMouse);

	// �f�[�^�̃Z�[�u���[�h�Ăяo��
	//CMouseCursor::SaveLoad(pKeyboard);
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

	// Z�e�X�gOFF
	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);

	CScene3D::Draw();

	// Z�e�X�gON
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);

	// ���C�e�B���OON
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//=============================================================================
// ���͏���
//=============================================================================
void CMouseCursor::Input(CInputKeyboard *pKeyboard, CInputMouse *pMouse)
{
	bool bSet = false;
	D3DXVECTOR3 SetPos = D3DXVECTOR3(((int)(m_pos.x / 38)) * 38.0f, 0.0f, ((int)(m_pos.z / 36)) * 36.0f);

	//if (CGame::GetCreateMode() == true)
	{// �m�[�}�����[�h��
		if (pMouse->GetTrigger(CInputMouse::DIMS_BUTTON_0) == true && CEnemy::GetPartSwitch() == true && pKeyboard->GetPress(DIK_LCONTROL) == false)
		{// �X�g���e�W�[�p�[�g���ɍ��N���b�N��AI�̖ړI�n��ݒ�
			CEnemy::SetSearchPos(SetPos);
		}
	}
	//	//else if (CGame::GetCreateMode() == false)
	//	{// �N���G�C�e�B�u���[�h��

	//if (pKeyboard->GetTrigger(DIK_LSHIFT) == true)
	//{// NUMPAD1�{�^���Őڑ��ҏW���[�h�̕ύX
	//	m_bConnectMode = m_bConnectMode ? false : true;
	//}

	//if (m_bConnectMode == true)
	//{// �ڑ��ҏW���[�h���̂�
	//	if (pKeyboard->GetTrigger(DIK_SPACE) == true)
	//	{// Space�L�[�Őڑ���,�惂�[�h�̕ύX
	//		m_bConnectSet = m_bConnectSet ? false : true;
	//		m_bConnectNumSet = !m_bConnectSet;
	//	}
	//}

	//		if (m_bConnectMode == false)
	//		{// �m�[�h�z�u�ݒ胂�[�h��
	//			if (pMouse->GetTrigger(CInputMouse::DIMS_BUTTON_0) == true && pKeyboard->GetPress(DIK_LCONTROL) == false)
	//			{//	���N���b�N�Ńm�[�h�z�u
	//			 // �ړI�n�ݒ�
	//				for (int nCntNode = 0; nCntNode < m_nNodeCounter; nCntNode++)
	//				{// �z�u���Ă�m�[�h�̐��������
	//					if (m_pNodePointer[nCntNode] != NULL)
	//					{// �S�m�[�h��NULL�`�F�b�N
	//						if (m_pNodePointer[nCntNode]->GetPos() == SetPos)
	//						{// ���łɒu���Ă���ꏊ�̏ꍇ
	//							bSet = true;
	//						}
	//					}
	//				}
	//
	//				if (m_pNodePointer[m_nNodeCounter] == NULL && bSet == false)
	//				{// �m�[�h�|���S���̒ǉ� �����ɔz�u�ς݂Ȃ�z�u�ł��Ȃ�
	//					m_pNodePointer[m_nNodeCounter] = m_pNodePointer[m_nNodeCounter]->Create(SetPos);
	//					CNodePointer::GetNodeMax()++;
	//					m_nNodeCounter++;
	//				}
	//			}
	//			else if (pMouse->GetTrigger(CInputMouse::DIMS_BUTTON_1) == true && m_nNodeCounter > 0)
	//			{// �E�N���b�N��1�߂�
	//			 // �ڑ���̐ڑ��������炷
	//				CNodePointer::GetNodeMax()--;
	//				m_nNodeCounter--;
	//
	//				if (m_pNodePointer[m_nNodeCounter] != NULL)
	//				{// �m�[�h�|���S���̍폜
	//					m_pNodePointer[m_nNodeCounter]->Uninit();
	//					m_pNodePointer[m_nNodeCounter] = NULL;
	//				}
	//			}
	//		}
	//		else if (m_bConnectMode == true)
	//		{// �m�[�h�ڑ��ݒ胂�[�h��
	//			if (m_bConnectSet == true)
	//			{// �ڑ����ݒ胂�[�h��
	//				if (pMouse->GetTrigger(CInputMouse::DIMS_BUTTON_0) == true && pKeyboard->GetPress(DIK_LCONTROL) == false)
	//				{// ���N���b�N�Őڑ����̃m�[�h��ݒ肷��
	//					for (int nCntNode = 0; nCntNode < m_nNodeCounter; nCntNode++)
	//					{// �z�u���Ă�m�[�h�̐��������
	//						if (m_pNodePointer[nCntNode] != NULL)
	//						{// �S�m�[�h��NULL�`�F�b�N
	//							if (m_pNodePointer[nCntNode]->GetPos() == SetPos)
	//							{// �I���m�[�h�̔ԍ�������
	//
	//								if (m_pNodePointer[m_nSelectNode]->GetPos() != SetPos)
	//								{// 1�ڂ���Ȃ���ΑO��̐F�����ɖ߂�
	//									m_pNodePointer[m_nSelectNode]->GetColor() = D3DXCOLOR(1.0f, 0.5f, 0.8f, 0.5f);
	//								}
	//
	//								// �I���m�[�h�̔ԍ���ڑ����Ƃ��Ď擾
	//								m_nSelectNode = m_pNodePointer[nCntNode]->GetMyNumber();
	//
	//								// �ڑ����m�[�h��I�����Ă�̂�������悤�ɐF��ς���
	//								m_pNodePointer[m_nSelectNode]->GetColor() = D3DXCOLOR(0.0f, 0.0f, 1.0f, 0.5f);
	//
	//								m_SelectNodePos = m_pNodePointer[m_nSelectNode]->GetPos();
	//								m_SelectNodeNumber = m_pNodePointer[m_nSelectNode]->GetMyNumber();
	//								m_SelectNodeConnectMax = m_pNodePointer[m_nSelectNode]->GetConnectMax();
	//								for (int nCntConnect = 0; nCntConnect < m_pNodePointer[m_nSelectNode]->GetConnectMax(); nCntConnect++)
	//								{
	//									m_SelectNodeConnect[nCntConnect] = m_pNodePointer[m_nSelectNode]->GetConnect(nCntConnect);
	//								}
	//							}
	//						}
	//					}
	//				}
	//			}
	//			else if (m_bConnectNumSet == true)
	//			{// �ڑ���ݒ胂�[�h��
	//				if (pMouse->GetTrigger(CInputMouse::DIMS_BUTTON_0) == true)
	//				{// ���N���b�N�Őڑ���̃m�[�h��ݒ肷��
	//					for (int nCntNode = 0; nCntNode < m_nNodeCounter; nCntNode++)
	//					{// �z�u���Ă�m�[�h�̐��������
	//						if (m_pNodePointer[nCntNode] != NULL)
	//						{// �S�m�[�h��NULL�`�F�b�N
	//							if (m_pNodePointer[nCntNode]->GetConnectMax() < CONNECT_MAX && m_pNodePointer[m_nSelectNode]->GetConnectMax() < CONNECT_MAX)
	//							{// �ڑ��ł���ő�ȉ��̏ꍇ�̂�
	//								if (m_pNodePointer[m_nSelectConnectNode]->GetPos() != SetPos && m_pNodePointer[m_nSelectNode]->GetPos() != SetPos)
	//								{// �����ꏊ��ڑ����̏ꍇ�͑I���ł��Ȃ�
	//									if (m_pNodePointer[nCntNode]->GetPos() == SetPos)
	//									{// �I���m�[�h�̔ԍ�������
	//
	//										if (m_pNodePointer[m_nSelectConnectNode]->GetPos() != SetPos && m_pNodePointer[m_nSelectNode]->GetPos() != SetPos)
	//										{// 1�ڂ�ڑ�������Ȃ���ΑO��̐F�����ɖ߂�
	//											m_pNodePointer[m_nSelectConnectNode]->GetColor() = D3DXCOLOR(1.0f, 0.5f, 0.8f, 0.5f);
	//										}
	//
	//										// �I���m�[�h�̔ԍ���ڑ���Ƃ��Ď擾
	//										m_nSelectConnectNode = m_pNodePointer[nCntNode]->GetMyNumber();
	//
	//										// �ڑ����m�[�h�ɐڑ���m�[�h�̔ԍ���o�^����
	//										m_pNodePointer[m_nSelectNode]->GetConnect(m_pNodePointer[m_nSelectNode]->GetConnectMax()) = m_pNodePointer[m_nSelectConnectNode]->GetMyNumber();
	//
	//										// �ڑ����̐ڑ����𑝂₷
	//										m_pNodePointer[m_nSelectNode]->GetConnectMax()++;
	//										m_nConnectNodeCounter++;
	//
	//										// �ڑ���m�[�h��I�����Ă�̂�������悤�ɐF��ς���
	//										m_pNodePointer[m_nSelectConnectNode]->GetColor() = D3DXCOLOR(0.0f, 1.0f, 1.0f, 0.5f);
	//									}
	//								}
	//							}
	//						}
	//					}
	//				}
	//
	//				if (pMouse->GetTrigger(CInputMouse::DIMS_BUTTON_1) == true && m_nConnectNodeCounter > 0 && m_nSelectConnectNode > 0)
	//				{// �ڑ����1�ȏ�ݒ肵�Ă鎞�ɁA�E�N���b�N��1�O�̐ڑ���ݒ�ɖ߂�
	//					for (int nCntNode = 0; nCntNode < m_nNodeCounter; nCntNode++)
	//					{// �z�u���Ă�m�[�h�̐��������
	//						if (m_pNodePointer[nCntNode] != NULL)
	//						{// �S�m�[�h��NULL�`�F�b�N
	//							if (m_pNodePointer[nCntNode]->GetConnectMax() > 0)
	//							{// �ڑ��ł���0��葽���ꍇ�̂�
	//								if (m_pNodePointer[m_nSelectConnectNode]->GetPos() != SetPos)
	//								{// �ڑ����̏ꍇ�͍폜�ł��Ȃ�
	//
	//								 // �ڑ����폜
	//									m_pNodePointer[nCntNode]->GetConnect(m_pNodePointer[nCntNode]->GetConnectMax()) = NULL;
	//
	//									// �ڑ���̐ڑ��������炷
	//									m_pNodePointer[nCntNode]->GetConnectMax()--;
	//									m_nConnectNodeCounter--;
	//
	//									// ���炵���̂�������悤�ɐF��ς���
	//									m_pNodePointer[nCntNode]->GetColor() = D3DXCOLOR(1.0f, 0.5f, 0.8f, 0.5f);
	//								}
	//							}
	//						}
	//					}
	//				}
	//			}
	//		}
	//	}
}

//=============================================================================
// �ړ�����
//=============================================================================
void CMouseCursor::Move(CInputMouse *pMouse)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// �f�o�C�X�̎擾
	D3DVIEWPORT9 viewport;
	D3DXMATRIX matrix;

	if (m_pCamera != NULL)
	{// �J������Null����Ȃ�
	 // �X�N���[�����W�����[���h���W�֕ϊ�
		D3DXVECTOR3 MousePos((float)pMouse->GetPoint().x, (float)pMouse->GetPoint().y, 1);
		viewport = CManager::GetCamera()->GetViewport(1);
		D3DXMatrixIdentity(&matrix);
		D3DXMATRIX proj, view;
		CManager::GetCamera()->GetInfo(&viewport, &proj, &view);
		D3DXVec3Unproject(&MousePos, &MousePos, &viewport, &m_pCamera->GetProjection(), &m_pCamera->GetView(), &matrix);

		// �J�[�\���ʒu�̐ݒ�
		m_pos.x = MousePos.x;
		m_pos.y = 0.0f;
		m_pos.z = MousePos.z;

		CDebugProc::Print("�X�N���[�����W�F%.2f %.2f", (float)pMouse->GetPoint().x, (float)pMouse->GetPoint().y);

		//// �Y�[���{���ɉ������J�[�\���̃T�C�Y�ύX
		//switch ((int)m_pCamera->GetZoom())
		//{
		//case 3:
		//	m_size = D3DXVECTOR3(3.5f, 0.0f, 3.5f);
		//	break;
		//case 2:
		//	m_size = D3DXVECTOR3(5.0f, 0.0f, 5.0f);
		//	break;
		//case 1:
		//	m_size = D3DXVECTOR3(10.0f, 0.0f, 10.0f);
		//	break;
		//}
	}

	// �ʒu�E�T�C�Y�̐ݒ�
	CScene3D::SetMousePos(m_pos);
	CScene3D::SetSize(m_size);
}

//=============================================================================
// ���[�g�T���p�t�@�C���̏����o��
//=============================================================================
HRESULT CMouseCursor::FileSave()
{
	FILE* pFile = NULL;		// �t�@�C���|�C���^
	int nCntSaveState = 0;

	//�G�̊�{�����i�[
	std::vector<NODE_SAVE_STATE> SaveState;

	//�f�[�^�̕ۑ�
	CScene* pScene = NULL;
	pScene = CScene::GetSceneTop(NODE_PRIORITY);

	//�t�@�C���I�[�v��
	pFile = fopen(LOAD_FILENAME, "w");

	if (pFile != NULL)
	{
		fprintf(pFile, "//====================================================================================\n");
		fprintf(pFile, "//																					  \n");
		fprintf(pFile, "//	�}�b�v�m�[�h�z�u�p�e�L�X�g														  \n");
		fprintf(pFile, "//	Author : Komatsu Keisuke														  \n");
		fprintf(pFile, "//																					  \n");
		fprintf(pFile, "//====================================================================================\n");
		fprintf(pFile, "START_LOAD  // ���[�h�J�n\n\n");
		fprintf(pFile, "//========================================\n");
		fprintf(pFile, "// �m�[�h�̏��\n");
		fprintf(pFile, "//========================================\n");
		fprintf(pFile, "START_DATA\n\n");

		if (pScene != NULL)
		{
			do
			{
				CScene* pSceneNext = pScene->GetSceneNext();
				if (pScene->GetObjType() == OBJTYPE_NODE)
				{
					//���ʃf�[�^�̕ۑ�
					CNodePointer* pNodePointer = (CNodePointer*)pScene;

					fprintf(pFile, "	NODESET // �|�C���g[%d]\n", pNodePointer->GetMyNumber());
					fprintf(pFile, "		NODE_POS = %.1f %.1f %.1f  // �ʒu\n", pNodePointer->GetPos().x, pNodePointer->GetPos().y, pNodePointer->GetPos().z);
					fprintf(pFile, "		CONNECT_NUM = %d  // �ڑ��m�[�h��\n", pNodePointer->GetConnectMax());

					for (int nCntConnectNode = 0; nCntConnectNode < pNodePointer->GetConnectMax(); nCntConnectNode++)
					{// �q�����Ă�m�[�h�̐��������
						fprintf(pFile, "		CONNECT_INDEX = %d  // �ڑ��m�[�h�ԍ� [%d��]\n", pNodePointer->GetConnect(nCntConnectNode), nCntConnectNode + 1);
					}
					fprintf(pFile, "	END_NODESET\n\n");
				}
				pScene = pSceneNext;
				nCntSaveState++;

			} while (pScene != NULL);

			fprintf(pFile, "END_DATA\n\n");
			fprintf(pFile, "END_LOAD  // ���[�h�I��\n");
		}

		//�t�@�C���N���[�Y
		if (pFile != NULL)
		{
			fclose(pFile);
			pFile = NULL;
		}
	}
	else
	{//�����ɓ�������G���[(���̃t�@�C�����Ȃ�or�J���Ȃ�)
		m_bErrorMassage = true;
		return S_FALSE;
	}
	return S_OK;
}

//=============================================================================
// ���[�g�T���p�t�@�C���̓ǂݍ���
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

//=============================================================================
// �t�@�C���̃Z�[�u���[�h�Ăяo��
//=============================================================================
void CMouseCursor::SaveLoad(CInputKeyboard *pKeyboard)
{
	if (pKeyboard->GetTrigger(DIK_F9) == true)
	{// F9�L�[�Ńt�@�C���ɃZ�[�u
		FileSave();
		m_bSaveMassage = true;
	}

	if (pKeyboard->GetTrigger(DIK_F10) == true)
	{// F10�L�[�Ńt�@�C�������[�h
		CMouseCursor::FileLoad(LOAD_FILENAME);

		// ���[�h�����f�[�^�����ƂɃm�[�h��z�u
		for (int nCntNode = 0; nCntNode < m_LoadNodeData.nodeMax; nCntNode++)
		{// �S�m�[�h�������
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
/****************************************************************/
/*					2D�}�E�X�J�[�\���̃N���X					*/
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
	m_pos.x = float(pMouse->GetPoint().x - rect.left) - 5.0f;
	m_pos.y = float(pMouse->GetPoint().y - rect.top) - 30.0f;

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