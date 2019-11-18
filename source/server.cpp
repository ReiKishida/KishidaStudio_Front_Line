//=============================================================================
//
// �T�[�o�[���� [server.cpp]
// Author :�@Jukiya Hayakawa
//
//=============================================================================
#include "server.h"
#include "debugproc.h"
#include "serverfunction.h"
#include <stdio.h>
#include "manager.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define SERVER_INI ("data\\TEXT\\server.ini")

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================


//=============================================================================
// �R���X�g���N�^
//=============================================================================
CServer::CServer()
{

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CServer::~CServer()
{

}

//=============================================================================
// ��������
//=============================================================================
CServer *CServer::Create()
{
	CServer *pServer = NULL;		//�T�[�o�[�̃|�C���^

	if (pServer == NULL)
	{//NULL�̏ꍇ
		pServer = new CServer;	//���I�m��
		if (pServer != NULL)
		{//NULL�ł͂Ȃ��ꍇ
			pServer->Init();		//����������
		}
	}

	return pServer;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CServer::Init(void)
{
	m_bConnected = false;
	memset(m_aSendMessage, 0, MAX_SERVER_DATA);
	memset(m_aReceiveMessage, 0, MAX_SERVER_DATA);

	//�\�P�b�g�̍쐬
	m_sockServer = socket(AF_INET, SOCK_STREAM, 0);

	//�\�P�b�g�̐ݒ�
	m_addrServer.sin_family = AF_INET;
	m_addrServer.sin_port = htons(12345);
	m_addrServer.sin_addr.S_un.S_addr = INADDR_ANY;

	//�\�P�b�g�̖��̂�t�^
	bind(m_sockServer, (struct sockaddr*)&m_addrServer, sizeof(m_addrServer));

	//�N���C�A���g����̐ڑ��v����҂��Ă����Ԃɂ���
	listen(m_sockServer, 5);
	m_bConnected = false;

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CServer::Uninit(void)
{
	//�\�P�b�g�i�T�[�o�[�j�̊J��
	closesocket(m_sockServer);

	//�\�P�b�g�i�N���C�A���g�j�̊J��
	closesocket(m_sockClient);
}

//=============================================================================
// �X�V����
//=============================================================================
void CServer::Update(void)
{
	if (m_bConnected != true)
	{
		m_nLengthAddClient = sizeof(m_addrClient);
		m_sockClient = accept(m_sockServer, (struct sockaddr*)&m_addrClient, &m_nLengthAddClient);
		//�N���C�A���g����̐ڑ��v�����󂯕t����
		if (m_sockClient == INVALID_SOCKET)
		{
			return;
		}
		memset(m_aSendMessage, 0, sizeof(m_aSendMessage));
		m_bConnected = true;
	}
	else
	{
		//�N���C�A���g�փf�[�^�i���b�Z�[�W�j���M
		send(m_sockClient, "HELLOONLINE!", 14, 0);
		memset(m_aReceiveMessage, 0, sizeof(m_aReceiveMessage));
		m_nLengthAddClient = recv(m_sockClient, &m_aReceiveMessage[0], MAX_SERVER_DATA, 0);
		CDebugProc::Print("��M�o�C�g[%d]\n", m_nLengthAddClient);
	}
}

//=============================================================================
// �T�[�o�[�̏���������
//=============================================================================
HRESULT CServer::Startup(void)
{
	WSADATA wsaData;		//winsock�̏��

							//winsock�̏�����
	WSAStartup(MAKEWORD(2, 0), &wsaData);

	return S_OK;
}

//=============================================================================
// �T�[�o�[�̃N���[������
//=============================================================================
void CServer::Cleanup(void)
{
	//winsock�̏I��
	WSACleanup();
}

//=============================================================================
//
// �N���C�A���g����
//
//=============================================================================

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CClient::CClient()
{

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CClient::~CClient()
{

}

//=============================================================================
// ��������
//=============================================================================
CClient *CClient::Create()
{
	CClient *pClient = NULL;		//�T�[�o�[�̃|�C���^

	if (pClient == NULL)
	{//NULL�̏ꍇ
		pClient = new CClient;	//���I�m��
		if (pClient != NULL)
		{//NULL�ł͂Ȃ��ꍇ
			pClient->Init();		//����������
		}
	}

	return pClient;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CClient::Init(void)
{
	for (int nCntConnect = 0; nCntConnect < MAX_CONNECT; nCntConnect++)
	{
		m_nMechaType[nCntConnect] =  -1;
	}

	m_nPlayerIdx = 0;
	memset(m_aSendMessage, 0, MAX_SERVER_DATA);			//�T�[�o�[�ɑ��M���郁�b�Z�[�W
	memset(m_aData, 0, MAX_SERVER_DATA);				//��M�f�[�^
	memset(m_aReceiveMessage, 0, MAX_SERVER_DATA);		//�T�[�o�[�Ɏ�M���郁�b�Z�[�W
	m_nClientIdx = 0;									//�N���C�A���g�ԍ�
	m_nNumConnect = 0;									//�ڑ����Ă��鑍��
	m_threadHandle = 0;									//�X���b�h���
	m_team = 0;											//�`�[�����
	m_gameMode = GAME_MODE_PLAYER;
	m_nMinIdx = 0;										//�ŏ��̔ԍ�
	m_nMaxIdx = 0;										//�ő�̔ԍ�
														//�T�[�o�[�V�X�e������ǂݍ���
	LoadServerSystem();

	m_threadHandle = (HANDLE)_beginthreadex(NULL, 0, WaitConnectHandle, this, 0, NULL);
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CClient::Uninit(void)
{
	char aStr[10];

	if (m_bConnected == true)
	{
		memset(m_aSendMessage, 0, sizeof(m_aSendMessage));
		strcpy(m_aSendMessage, SERVER_CLIENT_DELETE);
		sprintf(aStr, "%d", m_nPlayerIdx);
		strcat(m_aSendMessage, aStr);
		send(m_sockClient, m_aSendMessage, strlen(m_aSendMessage), 0);
		memset(m_aSendMessage, 0, MAX_SERVER_DATA);

		//�T�[�o�[����f�[�^�i���b�Z�[�W�j����M
		memset(m_aReceiveMessage, 0, MAX_SERVER_DATA);
		m_nLengthData = recv(m_sockClient, &m_aReceiveMessage[0], MAX_SERVER_DATA, 0);
	}

	//�X���b�h�̔j��
	if (m_threadHandle != 0)
	{
		CloseHandle(m_threadHandle);
	}
	//�\�P�b�g�i�N���C�A���g�j�̊J��
	closesocket(m_sockClient);
}

//=============================================================================
// �X�V����
//=============================================================================
void CClient::Update(void)
{
	if (m_bConnected != true)
	{//�ڑ����Ă��Ȃ��ꍇ
		CDebugProc::Print("�ڑ�����Ă��܂���B");
	}
	else
	{//�ڑ����Ă���ꍇ
		memset(m_aSendMessage, 0, sizeof(m_aSendMessage));
		if (CManager::GetMode() == CManager::MODE_MATCHING)
		{
			strcpy(m_aSendMessage, SERVER_MODE_MATCHING);
			strcat(m_aSendMessage, " ");
		}
		else if (CManager::GetMode() == CManager::MODE_GAME)
		{
			strcpy(m_aSendMessage, SERVER_MODE_GAME);
			strcat(m_aSendMessage, " ");
		}
		else if (CManager::GetMode() == CManager::MODE_RESULT)
		{
			strcpy(m_aSendMessage,SERVER_MODE_RESULT);
			strcat(m_aSendMessage, " ");
		}
		strcat(m_aSendMessage, m_aData);
		//�T�[�o�[�փf�[�^�i���b�Z�[�W�j�𑗐M
		CDebugProc::Print("���������b�Z�[�W�F%s\n", m_aSendMessage);
		send(m_sockClient, m_aSendMessage, strlen(m_aSendMessage), 0);

		//�T�[�o�[����f�[�^�i���b�Z�[�W�j����M
		memset(m_aReceiveMessage, 0, MAX_SERVER_DATA);
		m_nLengthData = recv(m_sockClient, &m_aReceiveMessage[0], MAX_SERVER_DATA, 0);

		if (m_nLengthData > 1)
		{
			CDebugProc::Print("���b�Z�[�W�F[%s]\n", &m_aReceiveMessage);
			CDebugProc::Print("�f�[�^��[%d]", m_nLengthData);

		}
		CDebugProc::Print("�N���C�A���g�ԍ�%d\n", m_nClientIdx);
	}
	memset(m_aData, 0, MAX_SERVER_DATA);

	switch (m_team)
	{
	case 0:
		CDebugProc::Print("��");
		break;

	case 1:
		CDebugProc::Print("��");
		break;
	}
}

//=============================================================================
// �ڑ����Ă��鑍���̐ݒu����
//=============================================================================
void CClient::SetNumConnect(int nNumConnect)
{
	m_nNumConnect = nNumConnect;
}

//=============================================================================
// �ڑ������̎擾����
//=============================================================================
int CClient::GetNumConnect(void)
{
	return m_nNumConnect;
}

//=============================================================================
// �N���C�A���g�ԍ��̎擾����
//=============================================================================
int CClient::GetClientIdx(void)
{
	return m_nClientIdx;
}

//=============================================================================
// �T�[�o�[����󂯎�������̎擾����
//=============================================================================
char *CClient::GetReceiveData(void)
{
	return m_aReceiveMessage;
}

//=============================================================================
// �`�[�����̎擾����
//=============================================================================
int CClient::GetTeam(void)
{
	return m_team;
}

//=============================================================================
// �Q�[���̃��[�h���擾����
//=============================================================================
CClient::GAME_MODE CClient::GetGameMode(void)
{
	return m_gameMode;
}

//=============================================================================
// �v���C���[�ԍ��̎擾����
//=============================================================================
int CClient::GetPlayerIdx(void)
{
	return m_nPlayerIdx;
}

//=============================================================================
// ���J�̎�ނ̐ݒu����
//=============================================================================
void CClient::SetMechaType(int nPlayerIdx, int nMechaType)
{
	m_nMechaType[nPlayerIdx] = nMechaType;
}

//=============================================================================
// ���J�̎�ނ̎擾����
//=============================================================================
int CClient::GetMechaType(int nPlayerIdx)
{
	return m_nMechaType[nPlayerIdx];
}

//=============================================================================
// �ڑ����Ă��邩�ǂ����̎擾����
//=============================================================================
bool CClient::GetConnect(void)
{
	return m_bConnected;
}

//=============================================================================
// �ŏ��̔ԍ��̐ݒu����
//=============================================================================
void CClient::SetMinIdx(int nMinIdx)
{
	m_nMinIdx = nMinIdx;
}

//=============================================================================
// �ŏ��̔ԍ��̎擾����
//=============================================================================
int CClient::GetMinIdx(void)
{
	return m_nMinIdx;
}

//=============================================================================
// �ő�̔ԍ��̐ݒu����
//=============================================================================
void CClient::SetMaxIdx(int nMaxIdx)
{
	m_nMaxIdx = nMaxIdx;
}

//=============================================================================
// �ő�̔ԍ��̎擾����
//=============================================================================
int CClient::GetMaxIdx(void)
{
	return	m_nMaxIdx;
}

//=============================================================================
// �f�[�^�̎󂯓n������
//=============================================================================
void CClient::Printf(char *fmt, ...)
{
	char aStr[MAX_SERVER_DATA];
	//aStr[0] = '\0';
	va_list ap;

	va_start(ap, fmt);
	vsprintf(aStr, fmt, ap);
	va_end(ap);
	strcat(m_aData, aStr);
}

//=============================================================================
// �N���C�A���g�ԍ��̐ݒu����
//=============================================================================
void CClient::SetClientIdx(void)
{
	m_nClientIdx = 0;

	memset(m_aSendMessage, 0, sizeof(m_aSendMessage));
	strcpy(m_aSendMessage, SERVER_CLIENT_IDX);
	send(m_sockClient, m_aSendMessage, strlen(m_aSendMessage), 0);

	memset(m_aReceiveMessage, 0, MAX_SERVER_DATA);
	m_nLengthData = recv(m_sockClient, &m_aReceiveMessage[0], MAX_SERVER_DATA, 0);

	m_nClientIdx = CServerFunction::ReadInt(m_aReceiveMessage, SERVER_CLIENT_IDX);

}

//=============================================================================
// �ԍ��̐ݒu����
//=============================================================================
void CClient::SetIdx(void)
{
	char aStr[10];

	m_nClientIdx = 0;
	m_nPlayerIdx = 0;

	memset(m_aSendMessage, 0, sizeof(m_aSendMessage));
	strcpy(m_aSendMessage, SERVER_IDX);
	sprintf(aStr, "%d", m_team);
	strcat(m_aSendMessage, aStr);
	send(m_sockClient, m_aSendMessage, strlen(m_aSendMessage), 0);

	memset(m_aReceiveMessage, 0, MAX_SERVER_DATA);
	m_nLengthData = recv(m_sockClient, &m_aReceiveMessage[0], MAX_SERVER_DATA, 0);

	char *pStr = m_aReceiveMessage;
	int nWord = 0;
	if (CServerFunction::Memcmp(pStr, SERVER_CLIENT_IDX) == 0)
	{
		pStr += strlen(SERVER_CLIENT_IDX);
		m_nClientIdx = CServerFunction::ReadInt(pStr, "");
		nWord = CServerFunction::PopString(pStr, "");
		pStr += nWord;
		if (CServerFunction::Memcmp(pStr, SERVER_PLAYER_IDX) == 0)
		{
			pStr += strlen(SERVER_PLAYER_IDX);
			m_nPlayerIdx = CServerFunction::ReadInt(pStr, "");
			nWord = CServerFunction::PopString(pStr, "");
			pStr += nWord;
		}
	}

	//m_nClientIdx = CServerFunction::ReadInt(m_aReceiveMessage, SERVER_CLIENT_IDX);

}

//=============================================================================
// �X���b�h�̌Ăяo������
//=============================================================================
unsigned _stdcall CClient::WaitConnectHandle(void *pClient)
{
	reinterpret_cast<CClient*>(pClient)->WaitConnect();
	return 0;
}

//=============================================================================
// �X���b�h�̌Ăяo������
//=============================================================================
void CClient::WaitConnect(void)
{
	m_sockClient = socket(AF_INET, SOCK_STREAM, 0);

	//�\�P�b�g�̐ݒ�
	m_addrServer.sin_family = AF_INET;
	m_addrServer.sin_port = htons(12345);
	m_addrServer.sin_addr.S_un.S_addr = inet_addr(m_serverIP);	//127.0.0.1 = ���[�J���E���[�v�o�b�O�E�A�h���X�i�����j

																//�T�[�o�[�ڑ�
	if (connect(m_sockClient, (struct sockaddr*)&m_addrServer, sizeof(m_addrServer)) == SOCKET_ERROR)
	{
		m_bConnected = false;
	}
	else
	{
		m_nLengthData = 0;
		//SetClientIdx();
		SetIdx();
		m_bConnected = true;
	}

}

//=============================================================================
// �T�[�o�[��IP�A�h���X��ǂݍ���
//=============================================================================
void CClient::LoadServerSystem(void)
{
	FILE *pFile;	//�t�@�C���̃|�C���^

					//�t�@�C���̓ǂݍ���
	pFile = fopen(SERVER_INI, "r");

	if (pFile == NULL)
	{//�t�@�C�����ǂݍ��߂Ă��Ȃ��ꍇ
		return;
	}
	else if (pFile != NULL)
	{//�t�@�C�����ǂݍ��߂��ꍇ
		char aStr[128] = "\0";

		while (1)
		{//���[�v
			fscanf(pFile, "%s", aStr);
			if (strcmp(aStr, SERVER_SCRIPT) == 0)
			{//�X�N���v�g�̊J�n�������Ă���ꍇ
				while (strcmp(aStr, SERVER_END_SCRIPT) != 0)
				{//�X�N���v�g�̏I���������܂Ń��[�v
					fscanf(pFile, "%s", aStr);
					if (strcmp(aStr, SERVER_IP) == 0)
					{//�T�[�o�[��IP�A�h���X�������Ă���ꍇ
						fscanf(pFile, " = %s", &aStr);
						strcpy(m_serverIP, aStr);
					}
					if (strcmp(aStr, SERVER_GAME_MODE) == 0)
					{//�Q�[���̃��[�h���������Ă���ꍇ
						fscanf(pFile, " = %d", &m_gameMode);
					}
					if (strcmp(aStr, SERVER_TEAM) == 0)
					{//�`�[���������Ă���ꍇ
						fscanf(pFile, " = %d", &m_team);
					}
				}
				break;
			}
		}
	}
}
