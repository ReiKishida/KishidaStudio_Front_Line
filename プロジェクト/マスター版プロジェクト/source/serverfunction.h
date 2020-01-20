//=============================================================================
//
// �T�[�o�[�̏���ǂݎ��ۂ̊֐�[serverfunction.h]
// Auther:Jukiya Hayakawa
//
//=============================================================================
#ifndef _SERVERFUNCTION_H_
#define _SERVERFUNCTION_H_

//*****************************************************************************
// �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"
#include <stdio.h>

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define SERVER_CONNECT_DATA			"CONNECT_DATA = "			//�ڑ����Ă��鑍��
#define SERVER_CLIENT_IDX			"CLIENT_IDX = "				//�N���C�A���g�̔ԍ�
#define SERVER_NOW_MODE				"NOW_MODE = "				//���݂̃��[�h
#define SERVER_CHARACTER_START		"CHARACTER_START"			//�L�������̊J�n
#define SERVER_PLAYER_START			"PLAYER_START"				//�v���C���[�̊J�n
#define SERVER_PLAYER_IDX			"PLAYER_IDX = "				//�v���C���[�̔ԍ�
#define SERVER_PLAYER_POS			"PLAYER_POS = "				//�v���C���[�̈ʒu
#define SERVER_PLAYER_ROT_UP		"PLAYER_ROT_UP = "			//�v���C���[�̏㔼�g�̌���
#define SERVER_PLAYER_ROT_DOWN		"PLAYER_ROT_DOWN = "		//�v���C���[�̉����g�̌���
#define SERVER_PLAYER_MOTION_UP		"PLAYER_MOTION_UP = "		//�v���C���[�̏㔼�g�̃��[�V����
#define SERVER_PLAYER_MOTION_DOWN	"PLAYER_MOTION_DOWN = "		//�v���C���[�̉����g�̃��[�V����
#define SERVER_PLAYER_END			"PLAYER_END"				//�v���C���[�̏I��
#define SERVER_CHARACTER_END		"CHARACTER_END"				//�L�������̏I��
#define SERVER_SCRIPT				"SCRIPT"					//�X�N���v�g�̊J�n
#define SERVER_IP					"SERVER_IP"					//�T�[�o�[��IP�A�h���X���
#define SERVER_TEAM					"TEAM"						//�`�[���̏��
#define SERVER_END_SCRIPT			"END_SCRIPT"				//�X�N���v�g�̏I��
#define SERVER_CLIENT_DELETE		"CLIENT_DELETE = "			//�N���C�A���g�̍폜
#define SERVER_GAME_MODE			"GAME_MODE"					//�Q�[�����[�h�̏��
#define SERVER_IDX					"IDX = "					//�ԍ��̏��
#define SERVER_MODE_MATCHING		"MODE_MATCHING"				//�}�b�`���O���[�h
#define SERVER_MODE_GAME			"MODE_GAME"					//�Q�[�����[�h
#define SERVER_MODE_RESULT			"MODE_RESULT"				//���U���g���[�h
#define SERVER_PLAYER_DATA			"PLAYER_DATA = "			//�v���C���[���
#define SERVER_AI_PLAYER_DATA		"AI_PLAYER_DATA = "			//AI�v���C���[���
#define SERVER_FADE_OUT				"FADE_OUT"					//�t�F�[�h�A�E�g���Ă�����
#define SERVER_FADE_NONE			"FADE_NONE"					//�t�F�[�h���Ă��Ȃ����
#define SERVER_CONNECT_STATE		"CONNECT_STATE = "			//�ڑ���

//*****************************************************************************
// �N���X�̒�`
//*****************************************************************************
class CServerFunction
{
public:    // �N������A�N�Z�X�\
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