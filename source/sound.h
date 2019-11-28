//=============================================================================
//
// サウンド処理 [sound.h]
// Author : Takuto Ishida
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// 前方宣言
//*****************************************************************************

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CSound
{
public:
	//*************************************
	// サウンドファイル
	//*************************************
	typedef enum
	{
		SOUND_LABEL_TITLE_MENU = 0,		// タイトル
		SOUND_LABEL_MATCHING,			// マッチング
		SOUND_LABEL_GAME,				// ゲーム
		//SOUND_LABEL_RESULT,				// リザルト
		SOUND_LABEL_DECIDE,				// 決定音
		SOUND_LABEL_MACHINEGUN,			// マシンガン
		SOUND_LABEL_RIFLE,				// ライフル
		SOUND_LABEL_SHOTGUN,			// ショットガン
		SOUND_LABEL_SNIPE,				// スナイパーライフル
		SOUND_LABEL_MAX,
	}SOUND_LABEL;

	CSound();
	~CSound();
	HRESULT Init(HWND hWnd);
	void Uninit(void);
	HRESULT PlaySound(SOUND_LABEL label);
	void StopSound(SOUND_LABEL label);
	void StopSound(void);

private:
	//*****************************************************************************
	// パラメータ構造体定義
	//*****************************************************************************
	typedef struct
	{
		char *pFilename;	// ファイル名
		int nCntLoop;		// ループカウント(-1でループ再生)
	} SOUNDPARAM;

	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

	IXAudio2 *m_pXAudio2;								// XAudio2オブジェクトへのポインタ
	IXAudio2MasteringVoice *m_pMasteringVoice;			// マスターボイスへのポインタ
	IXAudio2SourceVoice *m_apSourceVoice[SOUND_LABEL_MAX];	// ソースボイスへのポインタ
	BYTE *m_apDataAudio[SOUND_LABEL_MAX];					// オーディオデータへのポインタ
	DWORD m_aSizeAudio[SOUND_LABEL_MAX];					// オーディオデータサイズ

															// 各音素材のパラメータ
	SOUNDPARAM m_aSoundParam[SOUND_LABEL_MAX] =
	{
		{ "data/BGM/title_menu.wav", -1 },		// タイトル、メニュー
		{ "data/BGM/matching.wav", -1 },		// マッチング
		{ "data/BGM/game.wav", -1 },			// ゲーム
												//{ "data/BGM/result.wav", -1 },			// リザルト
		{ "data/SE/decide.wav", 0 },			// 決定
		{ "data/SE/machine-gun.wav", 0 },		// マシンガン
		{ "data/SE/rifle.wav", 0 },				// ライフル
		{ "data/SE/shotgun.wav", 0 },			// ショットガン
		{ "data/SE/snipe.wav", 0 },				// スナイパーライフル
	};
};

#endif
