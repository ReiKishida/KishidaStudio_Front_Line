//=============================================================================
//
// サーバー処理 [server.cpp]
// Author :　Jukiya Hayakawa
//
//=============================================================================
#include "server.h"
#include "debugproc.h"
#include "serverfunction.h"
#include <stdio.h>
#include "manager.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define SERVER_INI ("data\\TEXT\\server.ini")

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================


//=============================================================================
// コンストラクタ
//=============================================================================
CServer::CServer()
{

}

//=============================================================================
// デストラクタ
//=============================================================================
CServer::~CServer()
{

}

//=============================================================================
// 生成処理
//=============================================================================
CServer *CServer::Create()
{
	CServer *pServer = NULL;		//サーバーのポインタ

	if (pServer == NULL)
	{//NULLの場合
		pServer = new CServer;	//動的確保
		if (pServer != NULL)
		{//NULLではない場合
			pServer->Init();		//初期化処理
		}
	}

	return pServer;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CServer::Init(void)
{
	m_bConnected = false;
	memset(m_aSendMessage, 0, MAX_SERVER_DATA);
	memset(m_aReceiveMessage, 0, MAX_SERVER_DATA);

	//ソケットの作成
	m_sockServer = socket(AF_INET, SOCK_STREAM, 0);

	//ソケットの設定
	m_addrServer.sin_family = AF_INET;
	m_addrServer.sin_port = htons(12345);
	m_addrServer.sin_addr.S_un.S_addr = INADDR_ANY;

	//ソケットの名称を付与
	bind(m_sockServer, (struct sockaddr*)&m_addrServer, sizeof(m_addrServer));

	//クライアントからの接続要求を待っている状態にする
	listen(m_sockServer, 5);
	m_bConnected = false;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CServer::Uninit(void)
{
	//ソケット（サーバー）の開放
	closesocket(m_sockServer);

	//ソケット（クライアント）の開放
	closesocket(m_sockClient);
}

//=============================================================================
// 更新処理
//=============================================================================
void CServer::Update(void)
{
	if (m_bConnected != true)
	{
		m_nLengthAddClient = sizeof(m_addrClient);
		m_sockClient = accept(m_sockServer, (struct sockaddr*)&m_addrClient, &m_nLengthAddClient);
		//クライアントからの接続要求を受け付ける
		if (m_sockClient == INVALID_SOCKET)
		{
			return;
		}
		memset(m_aSendMessage, 0, sizeof(m_aSendMessage));
		m_bConnected = true;
	}
	else
	{
		//クライアントへデータ（メッセージ）送信
		send(m_sockClient, "HELLOONLINE!", 14, 0);
		memset(m_aReceiveMessage, 0, sizeof(m_aReceiveMessage));
		m_nLengthAddClient = recv(m_sockClient, &m_aReceiveMessage[0], MAX_SERVER_DATA, 0);
		CDebugProc::Print("受信バイト[%d]\n", m_nLengthAddClient);
	}
}

//=============================================================================
// サーバーの初期化処理
//=============================================================================
HRESULT CServer::Startup(void)
{
	WSADATA wsaData;		//winsockの情報

							//winsockの初期化
	WSAStartup(MAKEWORD(2, 0), &wsaData);

	return S_OK;
}

//=============================================================================
// サーバーのクリーン処理
//=============================================================================
void CServer::Cleanup(void)
{
	//winsockの終了
	WSACleanup();
}

//=============================================================================
//
// クライアント処理
//
//=============================================================================

//=============================================================================
// コンストラクタ
//=============================================================================
CClient::CClient()
{

}

//=============================================================================
// デストラクタ
//=============================================================================
CClient::~CClient()
{

}

//=============================================================================
// 生成処理
//=============================================================================
CClient *CClient::Create()
{
	CClient *pClient = NULL;		//サーバーのポインタ

	if (pClient == NULL)
	{//NULLの場合
		pClient = new CClient;	//動的確保
		if (pClient != NULL)
		{//NULLではない場合
			pClient->Init();		//初期化処理
		}
	}

	return pClient;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CClient::Init(void)
{
	for (int nCntConnect = 0; nCntConnect < MAX_CONNECT; nCntConnect++)
	{
		m_nMechaType[nCntConnect] =  -1;
	}

	m_nPlayerIdx = 0;
	memset(m_aSendMessage, 0, MAX_SERVER_DATA);			//サーバーに送信するメッセージ
	memset(m_aData, 0, MAX_SERVER_DATA);				//受信データ
	memset(m_aReceiveMessage, 0, MAX_SERVER_DATA);		//サーバーに受信するメッセージ
	m_nClientIdx = 0;									//クライアント番号
	m_nNumConnect = 0;									//接続している総数
	m_threadHandle = 0;									//スレッド情報
	m_team = 0;											//チーム情報
	m_gameMode = GAME_MODE_PLAYER;
	m_nMinIdx = 0;										//最小の番号
	m_nMaxIdx = 0;										//最大の番号
														//サーバーシステム情報を読み込み
	LoadServerSystem();

	m_threadHandle = (HANDLE)_beginthreadex(NULL, 0, WaitConnectHandle, this, 0, NULL);
	return S_OK;
}

//=============================================================================
// 終了処理
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

		//サーバーからデータ（メッセージ）を受信
		memset(m_aReceiveMessage, 0, MAX_SERVER_DATA);
		m_nLengthData = recv(m_sockClient, &m_aReceiveMessage[0], MAX_SERVER_DATA, 0);
	}

	//スレッドの破棄
	if (m_threadHandle != 0)
	{
		CloseHandle(m_threadHandle);
	}
	//ソケット（クライアント）の開放
	closesocket(m_sockClient);
}

//=============================================================================
// 更新処理
//=============================================================================
void CClient::Update(void)
{
	if (m_bConnected != true)
	{//接続していない場合
		CDebugProc::Print("接続されていません。");
	}
	else
	{//接続している場合
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
		//サーバーへデータ（メッセージ）を送信
		CDebugProc::Print("送ったメッセージ：%s\n", m_aSendMessage);
		send(m_sockClient, m_aSendMessage, strlen(m_aSendMessage), 0);

		//サーバーからデータ（メッセージ）を受信
		memset(m_aReceiveMessage, 0, MAX_SERVER_DATA);
		m_nLengthData = recv(m_sockClient, &m_aReceiveMessage[0], MAX_SERVER_DATA, 0);

		if (m_nLengthData > 1)
		{
			CDebugProc::Print("メッセージ：[%s]\n", &m_aReceiveMessage);
			CDebugProc::Print("データ数[%d]", m_nLengthData);

		}
		CDebugProc::Print("クライアント番号%d\n", m_nClientIdx);
	}
	memset(m_aData, 0, MAX_SERVER_DATA);

	switch (m_team)
	{
	case 0:
		CDebugProc::Print("青");
		break;

	case 1:
		CDebugProc::Print("赤");
		break;
	}
}

//=============================================================================
// 接続している総数の設置処理
//=============================================================================
void CClient::SetNumConnect(int nNumConnect)
{
	m_nNumConnect = nNumConnect;
}

//=============================================================================
// 接続総数の取得処理
//=============================================================================
int CClient::GetNumConnect(void)
{
	return m_nNumConnect;
}

//=============================================================================
// クライアント番号の取得処理
//=============================================================================
int CClient::GetClientIdx(void)
{
	return m_nClientIdx;
}

//=============================================================================
// サーバーから受け取った情報の取得処理
//=============================================================================
char *CClient::GetReceiveData(void)
{
	return m_aReceiveMessage;
}

//=============================================================================
// チーム情報の取得処理
//=============================================================================
int CClient::GetTeam(void)
{
	return m_team;
}

//=============================================================================
// ゲームのモード情報取得処理
//=============================================================================
CClient::GAME_MODE CClient::GetGameMode(void)
{
	return m_gameMode;
}

//=============================================================================
// プレイヤー番号の取得処理
//=============================================================================
int CClient::GetPlayerIdx(void)
{
	return m_nPlayerIdx;
}

//=============================================================================
// メカの種類の設置処理
//=============================================================================
void CClient::SetMechaType(int nPlayerIdx, int nMechaType)
{
	m_nMechaType[nPlayerIdx] = nMechaType;
}

//=============================================================================
// メカの種類の取得処理
//=============================================================================
int CClient::GetMechaType(int nPlayerIdx)
{
	return m_nMechaType[nPlayerIdx];
}

//=============================================================================
// 接続しているかどうかの取得処理
//=============================================================================
bool CClient::GetConnect(void)
{
	return m_bConnected;
}

//=============================================================================
// 最小の番号の設置処理
//=============================================================================
void CClient::SetMinIdx(int nMinIdx)
{
	m_nMinIdx = nMinIdx;
}

//=============================================================================
// 最小の番号の取得処理
//=============================================================================
int CClient::GetMinIdx(void)
{
	return m_nMinIdx;
}

//=============================================================================
// 最大の番号の設置処理
//=============================================================================
void CClient::SetMaxIdx(int nMaxIdx)
{
	m_nMaxIdx = nMaxIdx;
}

//=============================================================================
// 最大の番号の取得処理
//=============================================================================
int CClient::GetMaxIdx(void)
{
	return	m_nMaxIdx;
}

//=============================================================================
// データの受け渡し処理
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
// クライアント番号の設置処理
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
// 番号の設置処理
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
// スレッドの呼び出し処理
//=============================================================================
unsigned _stdcall CClient::WaitConnectHandle(void *pClient)
{
	reinterpret_cast<CClient*>(pClient)->WaitConnect();
	return 0;
}

//=============================================================================
// スレッドの呼び出し処理
//=============================================================================
void CClient::WaitConnect(void)
{
	m_sockClient = socket(AF_INET, SOCK_STREAM, 0);

	//ソケットの設定
	m_addrServer.sin_family = AF_INET;
	m_addrServer.sin_port = htons(12345);
	m_addrServer.sin_addr.S_un.S_addr = inet_addr(m_serverIP);	//127.0.0.1 = ローカル・ループバッグ・アドレス（初期）

																//サーバー接続
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
// サーバーのIPアドレスを読み込む
//=============================================================================
void CClient::LoadServerSystem(void)
{
	FILE *pFile;	//ファイルのポインタ

					//ファイルの読み込み
	pFile = fopen(SERVER_INI, "r");

	if (pFile == NULL)
	{//ファイルが読み込めていない場合
		return;
	}
	else if (pFile != NULL)
	{//ファイルが読み込めた場合
		char aStr[128] = "\0";

		while (1)
		{//ループ
			fscanf(pFile, "%s", aStr);
			if (strcmp(aStr, SERVER_SCRIPT) == 0)
			{//スクリプトの開始を示している場合
				while (strcmp(aStr, SERVER_END_SCRIPT) != 0)
				{//スクリプトの終了を示すまでループ
					fscanf(pFile, "%s", aStr);
					if (strcmp(aStr, SERVER_IP) == 0)
					{//サーバーのIPアドレスを示している場合
						fscanf(pFile, " = %s", &aStr);
						strcpy(m_serverIP, aStr);
					}
					if (strcmp(aStr, SERVER_GAME_MODE) == 0)
					{//ゲームのモード情報を示している場合
						fscanf(pFile, " = %d", &m_gameMode);
					}
					if (strcmp(aStr, SERVER_TEAM) == 0)
					{//チームを示している場合
						fscanf(pFile, " = %d", &m_team);
					}
				}
				break;
			}
		}
	}
}
