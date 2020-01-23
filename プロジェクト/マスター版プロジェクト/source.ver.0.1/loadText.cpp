//=============================================================================
//
// �e�L�X�g�ǂݍ��ݏ��� [loadText.cpp]
// Author : Takuto Ishida
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "loadText.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************

//=========================================
// �R���X�g���N�^
//=========================================
CLoadText::CLoadText()
{
}

//=========================================
// �f�X�g���N�^
//=========================================
CLoadText::~CLoadText()
{
}

//=========================================
// �P�s���ǂݍ���
//=========================================
char *CLoadText::ReadLine(FILE *pFile, char *pDest)
{
	// ��������P�s���擾
	fgets(pDest, 256, pFile);

	while (1)
	{
		if (memcmp(pDest, "#", 1) == 0)
		{// �擪���u#�v�̂Ƃ�
		 // ��������P�s���擾
			fgets(pDest, 256, pFile);
			//break;
		}
		else if (memcmp(pDest, "\n", 1) == 0)
		{// �擪���u\n�v�̂Ƃ�
			pDest += 1;
			break;
		}
		else if (memcmp(pDest, "\t", 1) == 0)
		{// �擪���u\t�v�̂Ƃ�
			pDest += 1;
		}
		else if (memcmp(pDest, " ", 1) == 0)
		{// �擪���u �v�̂Ƃ�
			pDest += 1;
		}
		else
		{// �L���ȍs
			break;
		}
	}

	return pDest;
}

//=========================================
// ������𔲂��o��
//=========================================
int CLoadText::PopString(char *pSrc, char *pDest)
{
	int nCntString = 0;
	char *pStr = NULL;
	strcpy(pDest, pSrc);	// ����������o��

	while (1)
	{
		nCntString++;
		pSrc += 1;				// ���o��

		if (memcmp(pSrc, " ", 1) == 0)
		{// �X�y�[�X�������Ƃ�
			pStr = new char[nCntString + 1];
			strncpy(pStr, pDest, nCntString);
			break;
		}
	}

	strncpy(pDest, pStr, nCntString);	// ����������o��
	nCntString++;			// �X�y�[�X����i�߂�
	pSrc += 1;				// ���o��

	delete[] pStr;
	return nCntString;
}
