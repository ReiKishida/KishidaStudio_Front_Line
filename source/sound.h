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
		SOUND_LABEL_TITLE = 0,		// �^�C�g��
		SOUND_LABEL_GAME,			// �Q�[��
		SOUND_LABEL_CLEAR,			// �Q�[���N���A
		SOUND_LABEL_PAUSEON,		// �|�[�Yon��
		SOUND_LABEL_PAUSEOFF,		// �|�[�Yoff��
		SOUND_LABEL_DECIDE,			// ���艹
		SOUND_LABEL_CURSOR,			// �J�[�\���ړ���
		SOUND_LABEL_HIT,			// �q�b�g��
		SOUND_LABEL_DAMAGE,			// �_���[�W��
		SOUND_LABEL_JUMP,			// �W�����v��
		SOUND_LABEL_SLIP,			// �X���b�v��
		SOUND_LABEL_ENEMYDEATH,		// �G���S��
		SOUND_LABEL_SWING,			// �f�U�艹
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
		{ "data/BGM/title.wav", -1 },			// �^�C�g��
		{ "data/BGM/game.wav", -1 },			// �Q�[��
		{ "data/BGM/game_clear.wav", -1 },		// �Q�[���N���A
		{ "data/SE/pause_on.wav", 0 },			// �|�[�Yon��
		{ "data/SE/pause_off.wav", 0 },			// �|�[�Yoff��
		{ "data/SE/decide000.wav", 0 },			// ���艹
		{ "data/SE/cursor.wav", 0 },			// �J�[�\���ړ���
		{ "data/SE/hit.wav", 0 },				// �q�b�g��
		{ "data/SE/damage.wav", 0 },			// �_���[�W��
		{ "data/SE/jump.wav", 0 },				// �W�����v��
		{ "data/SE/slip.wav", -1 },				// �X���b�v��
		{ "data/SE/enemy_death.wav", 0 },		// �G���S��
		{ "data/SE/punch-swing.wav", 0 },		// �f�U�艹
	};
};

#endif
