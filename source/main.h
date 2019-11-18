//=============================================================================
//
// ���C������ [main.h]
// Author : Takuto Ishida
//
//=============================================================================
#ifndef _MAIN_H_
#define _MAIN_H_

// �Q�c�|���S�����_�t�H�[�}�b�g( ���_���W[2D] / ���_�J���[ / �e�N�X�`�����W )
#define FVF_VERTEX_2D	(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)

// �R�c�|���S�����_�t�H�[�}�b�g( ���_���W[3D] / �@�� / ���_�J���[ / �e�N�X�`�����W )
#define FVF_VERTEX_3D	(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)

//*****************************************************************************
// �C���N���[�h�t�@�C��
//*****************************************************************************
#define _WINSOCK_DEPRECATED_NO_WARNINGS		//�T�[�o�[�ɕK�v�Ȍx������
#define _CRT_SECURE_NO_WARNINGS				//�x���Ώ�

#include <windows.h>
#include "d3dx9.h"						//�`�揈���ɕK�v
#define DIRECTINPUT_VERSION	(0x0800)	//�r���h���̌x���Ώ��p�}�N��
#include "dinput.h"						//���͏����ɕK�v
#include "xaudio2.h"					//�T�E���h�����ŕK�v
#include <XInput.h>                     // XInput�̏����ɕK�v

#include <process.h>					//�T�[�o�[�ɕK�v

// ���������[�N�`�F�b�N
#ifdef _DEBUG
#include <crtdbg.h>
//#define malloc(X) _malloc_dbg(X,_NORMAL_BLOCK,__FILE__,__LINE__)
#define _mallocc_dbg(X,_NOMAL_BLOCK,__FILE__,__LINE__)
#define _CRTDBG_MAP_ALLOC
#define new ::new(_NORMAL_BLOCK,__FILE__,__LINE__)
#endif
//*****************************************************************************
// ���C�u�����̃����N
//*****************************************************************************
#pragma comment(lib,"d3d9.lib")		//�`�揈���ɕK�v
#pragma comment(lib,"d3dx9.lib")	//[d3d9.lib]�̊g�����C�u����
#pragma comment(lib,"dxguid.lib")	//DirectX�R���|�[�l���g(���i)�g�p�ɕK�v
#pragma comment(lib,"winmm.lib")	//�V�X�e�������擾�ɕK�v
#pragma comment(lib,"dinput8.lib")	//���͏����ɕK�v
#pragma comment (lib, "XInput.lib")	// XInput�����ɕK�v
#pragma comment(lib,"ws2_32.lib")		//winsock�ɕK�v

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define SCREEN_WIDTH	(1280)		// �E�C���h�E�̕�
#define SCREEN_HEIGHT	(720)		// �E�C���h�E�̍���
#define SERVER_MODE (1)
#define NODEPOINT_MAX	(512)
#define CONNECT_MAX		(9)

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;	//���_���W
	float rhw;			//1.0f�ŌŒ�
	D3DCOLOR col;		//���_�J���[
	D3DXVECTOR2 tex;	//�e�N�X�`�����W
}VERTEX_2D;

// �R�c�|���S�����_�t�H�[�}�b�g�ɍ��킹���\���̂��`
typedef struct
{
	D3DXVECTOR3 pos;	// ���_���W
	D3DXVECTOR3 nor;	// �@���x�N�g��
	D3DCOLOR col;		// ���_�J���[
	D3DXVECTOR2 tex;	// �e�N�X�`�����W
} VERTEX_3D;

#endif