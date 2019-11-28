//=============================================================================
//
// �T�E���h���� [sound.h]
// Author : Takuto Ishida
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �O���錾
//*****************************************************************************

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CSound
{
public:
	//*************************************
	// �T�E���h�t�@�C��
	//*************************************
	typedef enum
	{
		SOUND_LABEL_TITLE_MENU = 0,		// �^�C�g��
		SOUND_LABEL_MATCHING,			// �}�b�`���O
		SOUND_LABEL_GAME,				// �Q�[��
		//SOUND_LABEL_RESULT,				// ���U���g
		SOUND_LABEL_DECIDE,				// ���艹
		SOUND_LABEL_MACHINEGUN,			// �}�V���K��
		SOUND_LABEL_RIFLE,				// ���C�t��
		SOUND_LABEL_SHOTGUN,			// �V���b�g�K��
		SOUND_LABEL_SNIPE,				// �X�i�C�p�[���C�t��
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
	// �p�����[�^�\���̒�`
	//*****************************************************************************
	typedef struct
	{
		char *pFilename;	// �t�@�C����
		int nCntLoop;		// ���[�v�J�E���g(-1�Ń��[�v�Đ�)
	} SOUNDPARAM;

	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

	IXAudio2 *m_pXAudio2;								// XAudio2�I�u�W�F�N�g�ւ̃|�C���^
	IXAudio2MasteringVoice *m_pMasteringVoice;			// �}�X�^�[�{�C�X�ւ̃|�C���^
	IXAudio2SourceVoice *m_apSourceVoice[SOUND_LABEL_MAX];	// �\�[�X�{�C�X�ւ̃|�C���^
	BYTE *m_apDataAudio[SOUND_LABEL_MAX];					// �I�[�f�B�I�f�[�^�ւ̃|�C���^
	DWORD m_aSizeAudio[SOUND_LABEL_MAX];					// �I�[�f�B�I�f�[�^�T�C�Y

															// �e���f�ނ̃p�����[�^
	SOUNDPARAM m_aSoundParam[SOUND_LABEL_MAX] =
	{
		{ "data/BGM/title_menu.wav", -1 },		// �^�C�g���A���j���[
		{ "data/BGM/matching.wav", -1 },		// �}�b�`���O
		{ "data/BGM/game.wav", -1 },			// �Q�[��
												//{ "data/BGM/result.wav", -1 },			// ���U���g
		{ "data/SE/decide.wav", 0 },			// ����
		{ "data/SE/machine-gun.wav", 0 },		// �}�V���K��
		{ "data/SE/rifle.wav", 0 },				// ���C�t��
		{ "data/SE/shotgun.wav", 0 },			// �V���b�g�K��
		{ "data/SE/snipe.wav", 0 },				// �X�i�C�p�[���C�t��
	};
};

#endif
