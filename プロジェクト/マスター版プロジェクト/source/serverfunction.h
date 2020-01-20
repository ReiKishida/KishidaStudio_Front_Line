//=============================================================================
//
// サーバーの情報を読み取る際の関数[serverfunction.h]
// Auther:Jukiya Hayakawa
//
//=============================================================================
#ifndef _SERVERFUNCTION_H_
#define _SERVERFUNCTION_H_

//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "main.h"
#include <stdio.h>

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define SERVER_CONNECT_DATA			"CONNECT_DATA = "			//接続している総数
#define SERVER_CLIENT_IDX			"CLIENT_IDX = "				//クライアントの番号
#define SERVER_NOW_MODE				"NOW_MODE = "				//現在のモード
#define SERVER_CHARACTER_START		"CHARACTER_START"			//キャラ情報の開始
#define SERVER_PLAYER_START			"PLAYER_START"				//プレイヤーの開始
#define SERVER_PLAYER_IDX			"PLAYER_IDX = "				//プレイヤーの番号
#define SERVER_PLAYER_POS			"PLAYER_POS = "				//プレイヤーの位置
#define SERVER_PLAYER_ROT_UP		"PLAYER_ROT_UP = "			//プレイヤーの上半身の向き
#define SERVER_PLAYER_ROT_DOWN		"PLAYER_ROT_DOWN = "		//プレイヤーの下半身の向き
#define SERVER_PLAYER_MOTION_UP		"PLAYER_MOTION_UP = "		//プレイヤーの上半身のモーション
#define SERVER_PLAYER_MOTION_DOWN	"PLAYER_MOTION_DOWN = "		//プレイヤーの下半身のモーション
#define SERVER_PLAYER_END			"PLAYER_END"				//プレイヤーの終了
#define SERVER_CHARACTER_END		"CHARACTER_END"				//キャラ情報の終了
#define SERVER_SCRIPT				"SCRIPT"					//スクリプトの開始
#define SERVER_IP					"SERVER_IP"					//サーバーのIPアドレス情報
#define SERVER_TEAM					"TEAM"						//チームの情報
#define SERVER_END_SCRIPT			"END_SCRIPT"				//スクリプトの終了
#define SERVER_CLIENT_DELETE		"CLIENT_DELETE = "			//クライアントの削除
#define SERVER_GAME_MODE			"GAME_MODE"					//ゲームモードの状態
#define SERVER_IDX					"IDX = "					//番号の情報
#define SERVER_MODE_MATCHING		"MODE_MATCHING"				//マッチングモード
#define SERVER_MODE_GAME			"MODE_GAME"					//ゲームモード
#define SERVER_MODE_RESULT			"MODE_RESULT"				//リザルトモード
#define SERVER_PLAYER_DATA			"PLAYER_DATA = "			//プレイヤー情報
#define SERVER_AI_PLAYER_DATA		"AI_PLAYER_DATA = "			//AIプレイヤー情報
#define SERVER_FADE_OUT				"FADE_OUT"					//フェードアウトしている状態
#define SERVER_FADE_NONE			"FADE_NONE"					//フェードしていない状態
#define SERVER_CONNECT_STATE		"CONNECT_STATE = "			//接続状況

//*****************************************************************************
// クラスの定義
//*****************************************************************************
class CServerFunction
{
public:    // 誰からもアクセス可能
	CServerFunction();
	~CServerFunction();

	static char *ReadLine(FILE *pFile, char *pDst);
	static char *GetLineTop(char *pScr);
	static char *HeadPutout(char *pDest, char *pHead);
	static char *ReadString(char *pScr, char *pDest, char *pHead);
	static int Memcmp(char *pScr, char *pCmp);
	static int PopString(char *pScr, char *pDst);
	static int ReadInt(char *pScr, char *pHead);
	static float ReadFloat(char *pScr, char *pHead);
	static bool ReadBool(char *pScr, char *pHead);
	static D3DXVECTOR3 ReadVector3(char *pScr, char *pHead);
};

#endif