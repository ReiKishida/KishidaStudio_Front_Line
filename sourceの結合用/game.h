//=============================================================================
//
// �Q�[������ [game.h]
// Author : Takuto Ishida
//
//=============================================================================
#ifndef _GAME_H_
#define _GAME_H_

#include "scene.h"
#include "server.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define GAME_PRIORITY		(6)				// �D��ԍ�
#define GRAVITY				(0.5f)			// �d��
#define NUMTEX_UV_X			(1)
#define NUMTEX_UV_Y			(3)

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

	static CPlayer *GetPlayer(int nPlayerIdx) { return m_pPlayer[nPlayerIdx]; };

	PART GetPart(void) { return m_part; };

	CMouseCursor *GetMouseCursor(void) { return m_pMouseCursor; }

private:
	void PrintData(void);
	void ReadMessage(void);
	char *ReadConnectData(char *pStr);
	char *ReadPlayerData(char *pStr);
	void SetPlayerData(int nPlayerIdx, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 modelRotUp, D3DXVECTOR3 modelRotDown, D3DXVECTOR3 cameraRot);
	void CreatePlayerBullet(int nPlayerIdx,int nNumShoot,int nAttack, D3DXVECTOR3 cameraRot, float *pAngle, float *pAngleV);
	void SetChatData(int nPlayerIdx, int radioChat);

	void CreateActionUI(void);
	void CreateStrategyUI(void);
	void SwicthPart(void);

	CPause *m_pPause;							// �|�[�Y�N���X�̃|�C���^�ϐ�
	static STATE m_state;						// �Q�[���̏��
	bool m_bPause;								// �|�[�YON/OFF�̐ؑ�
	int m_modeCounter;							// �J�ڂ���܂ł̃J�E���^
	int	m_nCntEnemyAppear;						// �G���o��������J�E���^
	CTimer *m_pTimer;							// �^�C�}�[�N���X�̃|�C���^�ϐ�
	static int m_nCurStage;						// ���݂̃X�e�[�W
	static CPlayer *m_pPlayer[MAX_CONNECT];					// �v���C���[�N���X�̃|�C���^�ϐ�
	CButtonManagerStrategy *m_pButtonManager;	// �{�^���̊Ǘ��N���X�̃|�C���^�ϐ�
	CMouseCursor *m_pMouseCursor;				// �}�E�X�J�[�\��
	PART m_part;								// ���݂̃p�[�g
	CModel *m_pField;							// �n�ʂ̃��f��
	CModel *m_pSky;								// �X�J�C�h�[��
};

#endif