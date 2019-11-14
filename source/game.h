//=============================================================================
//
// �Q�[������ [game.h]
// Author : Takuto Ishida
//
//=============================================================================
#ifndef _GAME_H_
#define _GAME_H_

#include "scene.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define GAME_PRIORITY		(6)				// �D��ԍ�
#define GRAVITY				(0.5f)			// �d��

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CPause;
class CTimer;
class CPlayer;
class CButtonManagerStrategy;
class CMouseCursor;
class CGauge2D;
class CModel;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CGame : public CScene
{
public:
	typedef enum
	{
		STATE_NONE = 0,	// �������Ă��Ȃ����
		STATE_NORMAL,	// �ʏ���
		STATE_END,		// �Q�[���I�[�o�[
		STATE_CLEAR,	// �Q�[���N���A
		STATE_MAX
	}STATE;

	typedef enum
	{// �p�[�g�̎��
		PART_ACTION = 0,
		PART_STRATEGY,
		PART_MAX,
	}PART;

	CGame(int nPriority = GAME_PRIORITY, CScene::OBJTYPE objType = CScene::OBJTYPE_GAME);
	~CGame();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static const void SetGameState(STATE state);
	STATE GetGameState(void) { return m_state; };

	const void SetPauseBool(bool bPause) { m_bPause = bPause; };
	bool GetPauseBool(void) { return m_bPause; };

	CPause *GetPause(void) { return m_pPause; };

	static int GetStage(void) { return m_nCurStage; };
	static void SetStage(int stage);

	static CPlayer *GetPlayer(void) { return m_pPlayer; };

	PART GetPart(void) { return m_part; };

private:
	CPause *m_pPause;							// �|�[�Y�N���X�̃|�C���^�ϐ�
	static STATE m_state;						// �Q�[���̏��
	bool m_bPause;								// �|�[�YON/OFF�̐ؑ�
	int m_modeCounter;							// �J�ڂ���܂ł̃J�E���^
	int	m_nCntEnemyAppear;						// �G���o��������J�E���^
	CTimer *m_pTimer;							// �^�C�}�[�N���X�̃|�C���^�ϐ�
	static int m_nCurStage;						// ���݂̃X�e�[�W
	static CPlayer *m_pPlayer;					// �v���C���[�N���X�̃|�C���^�ϐ�
	CButtonManagerStrategy *m_pButtonManager;	// �{�^���̊Ǘ��N���X�̃|�C���^�ϐ�
	CMouseCursor *m_pMouseCursor;				// �}�E�X�J�[�\��
	PART m_part;								// ���݂̃p�[�g
	CModel *m_pField;							// �n�ʂ̃��f��
};

#endif