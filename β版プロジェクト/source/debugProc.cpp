//=============================================================================
//
// �f�o�b�O���\������ [debugProc.cpp]
// Author : Takuto Ishida
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "debugProc.h"
#include "renderer.h"
#include "manager.h"

#include <stdio.h>
#include <stdarg.h>

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//=========================================
// �ÓI�����o�ϐ�
//=========================================
LPD3DXFONT CDebugProc::m_pFont[MAX_FONT] = {};		// �t�H���g�̎��
char CDebugProc::m_aStr[MAX_DEBUG_TEXT] = {};		// �\�����镶����
bool CDebugProc::m_bDisp = true;					// �\�����邩�ǂ���

//=============================================================================
// ������i�[����
//=============================================================================
void CDebugProc::Print(char *fmt, ...)
{
	char aSource[MAX_DEBUG_TEXT];

	// �ϒ��������g���ĕ�����ɂ���
	va_list arg;
	va_start(arg, fmt);
	vsprintf(aSource, fmt, arg);
	va_end(arg);

	// �������S�p�ɂ���
	char aStr[MAX_DEBUG_TEXT];
	LCMapString(0x0411, LCMAP_FULLWIDTH, aSource, -1, aStr, MAX_DEBUG_TEXT);

	int nMaxLength = strlen(m_aStr);	// ���܂ł̕�����̒������擾
	int nLength = strlen(aStr);			// ����̕�����̒������擾

	if (nMaxLength + nLength < MAX_DEBUG_TEXT)
	{// ������̒������I�[�o�[���Ȃ��悤�ɂ���
		if (strcmp(&aStr[nLength - 1], "\n") != 0)
		{// ���s���Ȃ��ꍇ�����
			strcat(&aStr[nLength], "\n");	// ���s��ǉ�
		}

		// �������ǉ�
		strcat(m_aStr, aStr);
	}
}

//=============================================================================
// ������`�揈��
//=============================================================================
void CDebugProc::Draw(void)
{
	RECT rect = { 11,11,SCREEN_WIDTH,SCREEN_HEIGHT };	// �\��������W

	if (m_bDisp)
	{// �\������
		m_pFont[0]->DrawText(					// �e������
			NULL,
			&m_aStr[0],							// �\�����镶����
			-1,									//�\���T�C�Y(-1�őS��)
			&rect,								//�\���͈�
			DT_LEFT | DT_NOCLIP,				//���l�߂ŗ��[����
			D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));	//�F

		// �ʒu�����炷
		rect.right -= 1;
		rect.left -= 1;

		m_pFont[0]->DrawText(					// �����̕\��
			NULL,
			&m_aStr[0],							// �\�����镶����
			-1,									//�\���T�C�Y(-1�őS��)
			&rect,								//�\���͈�
			DT_LEFT | DT_NOCLIP,				//���l�߂ŗ��[����
			D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));	//�F
	}

	// ���������ɂ���
	strcpy(m_aStr, "");
}

//=============================================================================
// �\���E��\���̐؂�ւ�
//=============================================================================
void CDebugProc::DebugDisp(bool bDisp)
{
	m_bDisp = bDisp;
}

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CDebugProc::CDebugProc()
{
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CDebugProc::~CDebugProc()
{
}

//=============================================================================
// ����������
//=============================================================================
void CDebugProc::Init(void)
{
	// �f�o�C�X�̎擾
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	// ������t�H���g�̏�����
	D3DXCreateFont(pDevice,
		18,                     // ��������
		7,                     // ������
		FW_NORMAL,              // �t�H���g�X�^�C��(����)
		NULL,                   // �~�b�v�}�b�v���f���̐�
		FALSE,                  // �Α̂ɂ��邩�ǂ���
		DEFAULT_CHARSET,		// �����Z�b�g(����)
		OUT_DEFAULT_PRECIS,
		PROOF_QUALITY,
		FIXED_PITCH | FF_MODERN,
		"JK�S�V�b�NM",               //�t�H���g�̎��"JK�S�V�b�NM"
		&m_pFont[0]);
}

//=============================================================================
// �I������
//=============================================================================
void CDebugProc::Uninit(void)
{
	for (int nCntFont = 0; nCntFont < MAX_FONT; nCntFont++)
	{
		if (NULL != m_pFont[nCntFont])
		{// �t�H���g�̊J��
			m_pFont[nCntFont]->Release();
			m_pFont[nCntFont] = NULL;
		}
	}
}
