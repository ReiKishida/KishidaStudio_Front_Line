//=============================================================================
//
// �m�[�h�f�[�^�Ǘ����� [file.cpp]
// Author : Komatsu Keisuke
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "nodeDatafiler.h"
#include "input.h"
#include "debugProc.h"
#include "nodePointer.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define MASSAGE_DISPLAY_TIME	(180)	// ���b�Z�[�W�̕\������(�t���[��)

//=============================================================================
// �ÓI�����o�ϐ�
//=============================================================================

//=============================================================================
// ��������
//=============================================================================
CNodeDataFiler* CNodeDataFiler::Create(void)
{
	CNodeDataFiler *pNodeFiler = NULL;

	if (pNodeFiler == NULL)
	{
		pNodeFiler = new CNodeDataFiler;	// ���������m��

		if (pNodeFiler != NULL)
		{// �������m�ې���
			pNodeFiler->Init();
		}
	}

	return pNodeFiler;
}

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CNodeDataFiler::CNodeDataFiler(int nPriority, CScene::OBJTYPE objType) : CScene(nPriority, objType)
{
	m_LoadNodeData = {};
	m_SaveNodeData = {};

	m_nMassageCount = MASSAGE_DISPLAY_TIME;
	m_bSaveMassage = false;
	m_bLoadMassage = false;
	m_bErrorMassage = false;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CNodeDataFiler::~CNodeDataFiler(){}

//=============================================================================
// �X�V����
//=============================================================================
void CNodeDataFiler::Update()
{
	if (m_bErrorMassage == true)
	{// �G���[���b�Z�[�W�̕\��
		CDebugProc::Print("====================================\n");
		CDebugProc::Print("= �t�@�C���ɏ������߂܂���ł��� =\n");
		CDebugProc::Print("====================================\n\n");
	}
	else if (m_bSaveMassage == true)
	{// �Z�[�u�������b�Z�[�W�̕\��
		CDebugProc::Print("===================\n");
		CDebugProc::Print("= �Z�[�u���튮�� =\n");
		CDebugProc::Print("===================\n\n");
	}
	else if (m_bLoadMassage == true)
	{// ���[�h�������b�Z�[�W�̕\��
		CDebugProc::Print("===================\n");
		CDebugProc::Print("= ���[�h���튮�� =\n");
		CDebugProc::Print("===================\n\n");
	}

	if (m_bSaveMassage == true || m_bLoadMassage == true || m_bErrorMassage == true)
	{// ���b�Z�[�W�\�����Ԃ̌���
		m_nMassageCount--;

		if (m_nMassageCount <= 0)
		{// ���b�Z�[�W�̔�\��
			m_bSaveMassage = false;
			m_bLoadMassage = false;
			m_bErrorMassage = false;
			m_nMassageCount = MASSAGE_DISPLAY_TIME;
		}
	}
}

//=============================================================================
// ���[�g�T���p�t�@�C���̏����o��
//=============================================================================
HRESULT CNodeDataFiler::FileSave(char *pFileName)
{
	FILE* pFile = NULL;		// �t�@�C���|�C���^
	int nCntSaveState = 0;

	//�G�̊�{�����i�[
	std::vector<NODE_SAVE_STATE> SaveState;

	//�f�[�^�̕ۑ�
	CScene* pScene = NULL;
	pScene = CScene::GetSceneTop(NODE_PRIORITY);

	//�t�@�C���I�[�v��
	pFile = fopen(pFileName, "w");

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
		fprintf(pFile, "START_DATA");

		if (pScene != NULL)
		{
			do
			{
				CScene* pSceneNext = pScene->GetSceneNext();
				if (pScene->GetObjType() == CScene::OBJTYPE_NODE)
				{
					//���ʃf�[�^�̕ۑ�
					CNodePointer* pNodePointer = (CNodePointer*)pScene;

					fprintf(pFile, "\n");
					fprintf(pFile, "	NODESET // �|�C���g[%d]\n", pNodePointer->GetMyNumber());
					fprintf(pFile, "		NODE_POS = %.1f %.1f %.1f  // �ʒu\n", pNodePointer->GetPos().x, pNodePointer->GetPos().y, pNodePointer->GetPos().z);
					fprintf(pFile, "		CONNECT_NUM = %d  // �ڑ��m�[�h��\n", pNodePointer->GetConnectMax());

					for (int nCntConnectNode = 0; nCntConnectNode < pNodePointer->GetConnectMax(); nCntConnectNode++)
					{// �q�����Ă�m�[�h�̐��������
						fprintf(pFile, "		CONNECT_INDEX = %d	// �ڑ��m�[�h��� [%d��]\n", pNodePointer->GetConnect(nCntConnectNode), nCntConnectNode + 1);
					}
					fprintf(pFile, "	END_NODESET\n");
				}
				pScene = pSceneNext;
				nCntSaveState++;

			} while (pScene != NULL);

			fprintf(pFile, "END_DATA\n");
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
		m_nMassageCount = MASSAGE_DISPLAY_TIME;
		return S_FALSE;
	}

	m_bSaveMassage = true;
	m_nMassageCount = MASSAGE_DISPLAY_TIME;
	return S_OK;
}

//=============================================================================
// ���[�g�T���p�t�@�C���̓ǂݍ���
//=============================================================================
void CNodeDataFiler::FileLoad(char *pFileName)
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
	m_bLoadMassage = true;
	m_nMassageCount = MASSAGE_DISPLAY_TIME;
}