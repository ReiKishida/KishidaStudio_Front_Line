//=============================================================================
//
// �e�N�X�`���ǂݍ��ݏ��� [texture.h]
// Author : Takuto Ishida
//
//=============================================================================
#ifndef _TEXTURE_H_
#define _TEXTURE_H_

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
class CTexture
{	// �e�N�X�`���Ǘ��N���X
public:
	typedef enum
	{
		TEXTURE_NUMBER = 0,				// ����
		TEXTURE_PRESS_BUTTON,			// ���͑҂�
		TEXTURE_BG_00,							// �w�i�P
		TEXTURE_BG_01,							// �w�i�Q
		TEXTURE_WEAPON_FLAME,			// ����̃t���[��
		TEXTURE_FLONTLINE,					// �^�C�g��
		TEXTURE_PRESSBOTTON,			// �v���X�{�^��
		TEXTURE_PLAYER_FLAME,			// �v���C���[�t���[��
		TEXTURE_CHANGE,						// �p�[�g�؂�ւ�
		TEXTURE_1P_INFO,						// 1P�̏������t���[��
		TEXTURE_2P_INFO,						// 2P�̏������t���[��
		TEXTURE_FLAME,							// �����̃t���[���i�X�g���e�W�[�p�[�g�Ŏg�p�j
		TEXTURE_STRATEGY_BG,				// �X�g���e�W�[�p�[�g�̔w�i
		TEXTURE_UPPER_UI,						// BLUE�ARED�A�^�C��UI
		TEXTURE_PART_CHANGE,			// �A�N�V�����A�X�g���e�W�[
		TEXTURE_AI_MECHA_FLAME,		// �h���[���A���[�J�[
		TEXTURE_MOUSECURSOR,			// �}�E�X�J�[�\��
		TEXTURE_WORKER_BUTTON,		// �E�H�[�J�[�̃A�C�R��
		TEXTURE_DRONE_BUTTON,			// �h���[���̃A�C�R��
		TEXTURE_RETICLE,						// ���e�B�N��
		TEXTURE_LOGO,							// ���C�����j���[�A���J�Z���N�g���S
		TEXTURE_ADAPT_RANGE,				// �����\���i�@�̑I����ʂŎg�p�j
		TEXTURE_MECHASEL_INST,			// �@�̐����i�@�̑I����ʂŎg�p�j
		TEXTURE_MECHATYPE,				// �@�̂̎�ށi�@�̑I����ʂŎg�p�j
		TEXTURE_MENU_DISP,					// ���[�h�������S�i���j���[��ʂŎg�p�j
		TEXTURE_MENU_FLAME,				// ���[�h�t���[���i���j���[��ʂŎg�p�j
		TEXTURE_MENU_INST,					// ���[�h�̊T�v�i���j���[�I����ʂŎg�p�j
		TEXTURE_MATCHING_BG,			// �}�b�`���O��ʂ̔w�i�i�}�b�`���O��ʂŎg�p�j
		TEXTURE_MATCHING_BG_00,		// �}�b�`���OBG_00�i�}�b�`���O��ʂŎg�p�j
		TEXTURE_MATCHING_BG_01,		// �}�b�`���OBG_01�i�}�b�`���O��ʂŎg�p�j
		TEXTURE_VS,									// VS���S�i�}�b�`���O��ʂŎg�p�j
		TEXTURE_PLAYER_NUM,				// �v���C���[�i���o�[�i�}�b�`���O��ʂŎg�p�j
		TEXTURE_BLUE_MECHA_LIST,		// BLUE�`�[�����J���X�g�i�}�b�`���O��ʂŎg�p�j
		TEXTURE_RED_MECHA_LIST,		// RED�`�[�����J���X�g�i�}�b�`���O��ʂŎg�p�j
		TEXTURE_START_DEATH,				// �퓬�J�n�A�s���s�\���S�i�A�N�V�����p�[�g�Ŏg�p�j
		TEXTURE_TILE_PATTERN,				// �^�C���A�j���[�V�����i�A�N�V�����p�[�g�Ŏg�p�j
		TEXTURE_RESPAWN,						// ���A���S�i�A�N�V�����p�[�g�Ŏg�p�j
		TEXTURE_MAP,								// ���X�|�[���}�b�v�i�A�N�V�����p�[�g�Ŏg�p�j
		TEXTURE_RESPAWN_FLAME,		// ���X�|�[���t���[���i�A�N�V�����p�[�g�Ŏg�p�j
		TEXTURE_TITLE,							// ���X�|�[���^�C�g���i�A�N�V�����p�[�g�Ŏg�p�j
		TEXTURE_SELECT_RESPAWN,		// ���X�|�[���I���i�A�N�V�����p�[�g�Ŏg�p�j
		TEXTURE_RELOAD,						// �����[�h���S�i�A�N�V�����p�[�g�Ŏg�p�j
		TEXTURE_MECHA_SELECT,			// �@�̉摜�i�@�̑I����ʂŎg�p�j
		TEXTURE_MAX
	}TEXTURE;

	CTexture();
	~CTexture();

	static void Load(void);
	static void Unload(void);

	static LPDIRECT3DTEXTURE9 GetTexture(TEXTURE texture) { return m_pTexture[texture]; }	// �e�N�X�`���̎擾

private:
	static LPDIRECT3DTEXTURE9 m_pTexture[TEXTURE_MAX];	// �e�N�X�`��
};

#endif