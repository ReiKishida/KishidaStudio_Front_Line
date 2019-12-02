//=============================================================================
//
// ���U���g���� [result.h]
// Author : Ayaka Hakozaki
//
//=============================================================================
#ifndef _RESULT_H_
#define _RESULT_H_

#include "scene.h"
#include "game.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define RESULT_UI_TEX			(2)		// �g�p����UI�e�N�X�`���̖���

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CNumber;
class CLogo;
class CLogoPressButton;
class CScore;
class CUI_TEXTURE;
class CModel;
class CPlayer;
class CMechaSelect;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CResult : public CScene
{
public:
	typedef enum
	{	// �������`�[��
		TEAM_WIN_BLUE = 0,
		TEAM_WIN_RED,
		TEAM_WIN_MAX
	} TEAM_WIN;

	CResult();
	~CResult();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static void SetTeamWin(TEAM_WIN teamWin) { m_teamWin = teamWin; }

private:
	static TEAM_WIN		m_teamWin;								// �������`�[�����擾
	static CPlayer				*m_pPlayer[MAX_PLAYER_CONNECT];
	CMechaSelect::MECHATYPE	m_mechaType[MAX_PLAYER_CONNECT];
	CModel						*m_pField;									// �n�ʂ̃��f��
	CUI_TEXTURE				*m_pUITex[RESULT_UI_TEX];	// UI�e�N�X�`���|�C���^
	int								m_nCntFlash;								// �v���X�{�^���_��
};

#endif