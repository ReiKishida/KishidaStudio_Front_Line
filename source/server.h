//=============================================================================
//
// サーバー処理 [server.h]
// Author :　Jukiya Hayakawa
//
//=============================================================================
#ifndef _SERVER_H_
#define _SERVER_H_

#include "main.h"
#include "debugProc.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_SERVER_DATA (2048)
#define MAX_CONNECT (4)

//*****************************************************************************
// クラス定義(サーバー)
//*****************************************************************************
class CServer
{
public:	//誰からもアクセス可能
	//メンバ関数
	CServer();
	~CServer();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

	//静的メンバ関数
	static CServer *Create(void);
	static HRESULT Startup(void);
	static void Cleanup(void);

private: //自分だけがアクセス可能
	struct sockaddr_in m_addrServer;				//ソケットアドレス（サーバー）の情報
	struct sockaddr_in m_addrClient;				//ソケットアドレス（クライアント）の情報
	SOCKET m_sockServer;							//ソケット（サーバー用）
	SOCKET m_sockClient;							//ソケット（クライアント用）
	int m_nLengthAddClient;							//ソケットアドレス（クライアント）のサイズ
	bool m_bConnected;								//接続されているかどうか
	char m_aSendMessage[MAX_SERVER_DATA];				//クライアントに送信するデータ
	char m_aReceiveMessage[MAX_SERVER_DATA];			//クライアントから受け取るデータ
};

//*****************************************************************************
// クラス定義(クライアント)
//*****************************************************************************
class CClient
{
public:	//誰からもアクセス可能
	//============================
	//		構造体の定義
	//============================
	typedef enum
	{
		GAME_MODE_PLAYER = 0,
		GAME_MODE_WATCHING,
		GAME_MODE_MAX,
	}GAME_MODE;

	//メンバ関数
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
	//静的メンバ関数
	static CClient *Create(void);

private: //自分だけがアクセス可能

	//メンバ関数
	void SetClientIdx(void);
	void SetIdx(void);
	void LoadServerSystem(void);

	//静的メンバ関数
	static unsigned _stdcall WaitConnectHandle(void *);

	void WaitConnect(void);

	//メンバ変数
	struct sockaddr_in m_addrServer;				//ソケットアドレス（サーバー）の情報
	SOCKET m_sockClient;							//ソケット（クライアント用）
	char m_aData[MAX_SERVER_DATA];					//受信データ
	int m_nLengthData;								//受信データサイズ
	bool m_bConnected;								//接続したかどうか
	char m_aSendMessage[MAX_SERVER_DATA];			//サーバーへ送信するメッセージ
	char m_aReceiveMessage[MAX_SERVER_DATA];		//サーバー受信するメッセージ
	int	m_nClientIdx;								//クライアント番号
	int m_nNumConnect;								//接続している総数
	char m_serverIP[128];							//IPアドレス情報
	int m_team;										//チーム情報
	HANDLE m_threadHandle;							//スレッド情報
	GAME_MODE m_gameMode;							//ゲームのモード情報
	int m_nPlayerIdx;								//プレイヤーの番号
	int m_nMechaType[MAX_CONNECT];					//メカの種類
};

#endif // _SERVER_H_
