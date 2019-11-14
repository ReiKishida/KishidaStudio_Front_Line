//=============================================================================
//
// �T�[�o�[���� [server.h]
// Author :�@Jukiya Hayakawa
//
//=============================================================================
#ifndef _SERVER_H_
#define _SERVER_H_

#include "main.h"
#include "debugProc.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_SERVER_DATA (2048)
#define MAX_CONNECT (4)

//*****************************************************************************
// �N���X��`(�T�[�o�[)
//*****************************************************************************
class CServer
{
public:	//�N������A�N�Z�X�\
	//�����o�֐�
	CServer();
	~CServer();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

	//�ÓI�����o�֐�
	static CServer *Create(void);
	static HRESULT Startup(void);
	static void Cleanup(void);

private: //�����������A�N�Z�X�\
	struct sockaddr_in m_addrServer;				//�\�P�b�g�A�h���X�i�T�[�o�[�j�̏��
	struct sockaddr_in m_addrClient;				//�\�P�b�g�A�h���X�i�N���C�A���g�j�̏��
	SOCKET m_sockServer;							//�\�P�b�g�i�T�[�o�[�p�j
	SOCKET m_sockClient;							//�\�P�b�g�i�N���C�A���g�p�j
	int m_nLengthAddClient;							//�\�P�b�g�A�h���X�i�N���C�A���g�j�̃T�C�Y
	bool m_bConnected;								//�ڑ�����Ă��邩�ǂ���
	char m_aSendMessage[MAX_SERVER_DATA];				//�N���C�A���g�ɑ��M����f�[�^
	char m_aReceiveMessage[MAX_SERVER_DATA];			//�N���C�A���g����󂯎��f�[�^
};

//*****************************************************************************
// �N���X��`(�N���C�A���g)
//*****************************************************************************
class CClient
{
public:	//�N������A�N�Z�X�\
	//============================
	//		�\���̂̒�`
	//============================
	typedef enum
	{
		GAME_MODE_PLAYER = 0,
		GAME_MODE_WATCHING,
		GAME_MODE_MAX,
	}GAME_MODE;

	//�����o�֐�
	CClient();
	~CClient();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Printf(char *fmt, ...);
	void SetNumConnect(int nNumConnect);
	int GetNumConnect(void);
	int GetClientIdx(void);
	char *GetReceiveData(void);
	int GetTeam(void);
	GAME_MODE GetGameMode(void);
	int GetPlayerIdx(void);

	void SetMechaType(int nPlayerIdx,int nMechaType);
	int GetMechaType(int nPlayerIdx);

	bool GetConnect(void) { return m_bConnected; };
	//�ÓI�����o�֐�
	static CClient *Create(void);

private: //�����������A�N�Z�X�\

	//�����o�֐�
	void SetClientIdx(void);
	void SetIdx(void);
	void LoadServerSystem(void);

	//�ÓI�����o�֐�
	static unsigned _stdcall WaitConnectHandle(void *);

	void WaitConnect(void);

	//�����o�ϐ�
	struct sockaddr_in m_addrServer;				//�\�P�b�g�A�h���X�i�T�[�o�[�j�̏��
	SOCKET m_sockClient;							//�\�P�b�g�i�N���C�A���g�p�j
	char m_aData[MAX_SERVER_DATA];					//��M�f�[�^
	int m_nLengthData;								//��M�f�[�^�T�C�Y
	bool m_bConnected;								//�ڑ��������ǂ���
	char m_aSendMessage[MAX_SERVER_DATA];			//�T�[�o�[�֑��M���郁�b�Z�[�W
	char m_aReceiveMessage[MAX_SERVER_DATA];		//�T�[�o�[��M���郁�b�Z�[�W
	int	m_nClientIdx;								//�N���C�A���g�ԍ�
	int m_nNumConnect;								//�ڑ����Ă��鑍��
	char m_serverIP[128];							//IP�A�h���X���
	int m_team;										//�`�[�����
	HANDLE m_threadHandle;							//�X���b�h���
	GAME_MODE m_gameMode;							//�Q�[���̃��[�h���
	int m_nPlayerIdx;								//�v���C���[�̔ԍ�
	int m_nMechaType[MAX_CONNECT];					//���J�̎��
};

#endif // _SERVER_H_
