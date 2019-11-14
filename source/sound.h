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
		SOUND_LABEL_TITLE = 0,		// タイトル
		SOUND_LABEL_GAME,			// ゲーム
		SOUND_LABEL_CLEAR,			// ゲームクリア
		SOUND_LABEL_PAUSEON,		// ポーズon音
		SOUND_LABEL_PAUSEOFF,		// ポーズoff音
		SOUND_LABEL_DECIDE,			// 決定音
		SOUND_LABEL_CURSOR,			// カーソル移動音
		SOUND_LABEL_HIT,			// ヒット音
		SOUND_LABEL_DAMAGE,			// ダメージ音
		SOUND_LABEL_JUMP,			// ジャンプ音
		SOUND_LABEL_SLIP,			// スリップ音
		SOUND_LABEL_ENEMYDEATH,		// 敵死亡音
		SOUND_LABEL_SWING,			// 素振り音
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
		{ "data/BGM/title.wav", -1 },			// タイトル
		{ "data/BGM/game.wav", -1 },			// ゲーム
		{ "data/BGM/game_clear.wav", -1 },		// ゲームクリア
		{ "data/SE/pause_on.wav", 0 },			// ポーズon音
		{ "data/SE/pause_off.wav", 0 },			// ポーズoff音
		{ "data/SE/decide000.wav", 0 },			// 決定音
		{ "data/SE/cursor.wav", 0 },			// カーソル移動音
		{ "data/SE/hit.wav", 0 },				// ヒット音
		{ "data/SE/damage.wav", 0 },			// ダメージ音
		{ "data/SE/jump.wav", 0 },				// ジャンプ音
		{ "data/SE/slip.wav", -1 },				// スリップ音
		{ "data/SE/enemy_death.wav", 0 },		// 敵死亡音
		{ "data/SE/punch-swing.wav", 0 },		// 素振り音
	};
};

#endif
