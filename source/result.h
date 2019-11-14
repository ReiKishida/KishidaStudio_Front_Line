//=============================================================================
//
// ���U���g���� [result.h]
// Author : Takuto Ishida
//
//=============================================================================
#ifndef _RESULT_H_
#define _RESULT_H_

#include "scene.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CNumber;
class CLogo;
class CLogoPressButton;
class CScore;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CResult : public CScene
{
public:
	CResult();
	~CResult();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static void SetScore(int nScore) { m_nScore = nScore; };

private:
	static int	m_nScore;		// �X�R�A
	int			m_nDigit;		// ����
	int			m_nCntResult;	// ���U���g�̎���
	int			m_nFlash;		// �_�ł����鑬�x

	CNumber				**m_pNumber;			// �����N���X�̃|�C���^�ϐ�
	CLogo				*m_pLogo;				// ���S�N���X�̃|�C���^�ϐ�
	CLogoPressButton	*m_pPressButton;		// ���͑҂��\���N���X�̃|�C���^�ϐ�
	CScore				*m_pScore;				// �X�R�A�N���X�̃|�C���^�ϐ�
};

#endif