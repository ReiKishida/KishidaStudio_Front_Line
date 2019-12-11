//=============================================================================
//
// �`���[�g���A������ [tutorial.h]
// Author : Takuto Ishida & Rei Kishida
//
//=============================================================================
#ifndef _TUTORIAL_H_
#define _TUTORIAL_H_

#include "scene.h"
#include "server.h"
#include "mechaSelect.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define GAME_PRIORITY		(6)				// �D��ԍ�
#define GRAVITY				(0.5f)			// �d��
#define NUMTEX_UV_X			(1)
#define NUMTEX_UV_Y			(3)
#define NUM_TEAM			(2)
#define NUM_RESPAWN_POS		(4)
#define MAX_PLAYER_TUTORIAL (2)
//*****************************************************************************
// �O���錾
//*****************************************************************************
class CPause;
class CTimer;
class CPlayer;
class CButtonManagerStrategy;
class CMouseCursor2D;
class CGauge2D;
class CModel;
class CScene3D;
class CMouseCursor;
class CUI_TEXTURE;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CTutorial : public CScene
{
public:
	typedef enum
	{
		TUTORIALSTATE_NONE = 0,
		TUTORIALSTATE_NORMAL,
		TUTORIALSTATE_CLEAR,
		TUTORIALSTATE_MAX
	}TUTORIALSTATE;

	typedef enum
	{
		CLEARSTATE_NONE = 0,
		CLEARSTATE_TRUE,
		CLEARSTATE_MAX
	}CLEARSTATE;

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

	CTutorial(int nPriority = GAME_PRIORITY, CScene::OBJTYPE objType = CScene::OBJTYPE_GAME);
	~CTutorial();
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

	CMouseCursor2D *GetMouseCursor(void) { return m_pMouseCursor; }

	CModel *GetField(void) { return m_pField; };

	CMouseCursor *GetMouse(void) { return m_pMouse; }

	CButtonManagerStrategy *GetButtonManager(void) { return m_pButtonManager; };

	void SetBlueLinkEnergy(int nBlueLinkEnergy) { m_nBlueLinkEnergy = nBlueLinkEnergy; };
	int GetBlueLinkEnergy(void) { return m_nBlueLinkEnergy; };

	void SetRedLinkEnergy(int nRedLinkEnergy) { m_nRedLinkEnergy = nRedLinkEnergy; };
	int GetRedLinkEnergy(void) { return m_nRedLinkEnergy; };

	D3DXVECTOR3 GetRespawnPos(int nTeam, int nPoint) { return m_aRespawnPos[nTeam][nPoint]; };

	static void SetMechaType(int nPlayerIdx, CMechaSelect::MECHATYPE type) { m_aMechaType[nPlayerIdx] = type; };
	static CMechaSelect::MECHATYPE GetMechaType(int nPlayerIdx) { return m_aMechaType[nPlayerIdx]; };

private:
	void TutorialBasicMovement(void);
	void TutorialAction(void);
	void TutorialStrategy(void);


	void SetTexTutorial(int nType, int nSteps);
	void LoadRespawnPos(void);

	void PrintData(void);
	void PrintCPUData(void);
	void ReadMessage(void);
	char *ReadConnectData(char *pStr);
	char *ReadPlayerData(char *pStr);
	char *ReadCPUData(char *pStr);

	void SetPlayerData(int nPlayerIdx, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 modelRotUp, D3DXVECTOR3 modelRotDown, D3DXVECTOR3 cameraRot);
	void CreatePlayerBullet(int nPlayerIdx, int nNumShoot, int nAttack, D3DXVECTOR3 cameraRot, float *pAngle, float *pAngleV);
	void SetChatData(int nPlayerIdx, int radioChat);
	void SetCPUData(int nPlayerIdx, D3DXVECTOR3 pos, D3DXVECTOR3 rot);
	void CreateCPUBullet(int nPlayerIdx, int nNumShoot, int nAttack, D3DXVECTOR3 cameraRot, float *pAngle, float *pAngleV);

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
	static CPlayer *m_pPlayer[MAX_PLAYER_TUTORIAL];					// �v���C���[�N���X�̃|�C���^�ϐ�
	CButtonManagerStrategy *m_pButtonManager;	// �{�^���̊Ǘ��N���X�̃|�C���^�ϐ�
	CMouseCursor2D *m_pMouseCursor;				// �}�E�X�J�[�\��
	PART m_part;								// ���݂̃p�[�g
	CModel *m_pField;							// �n�ʂ̃��f��
	CModel *m_pSky;								// �X�J�C�h�[��
	bool m_bConnect[MAX_PLAYER_TUTORIAL];

	CMouseCursor *m_pMouse;

	int m_nBlueLinkEnergy;
	int m_nRedLinkEnergy;
	D3DXVECTOR3 m_aRespawnPos[NUM_TEAM][NUM_RESPAWN_POS];

	bool m_bPlayerDeath[MAX_PLAYER_TUTORIAL];
	bool m_bAIDeath[2][MAX_PLAYER_TUTORIAL];

	static CMechaSelect::MECHATYPE m_aMechaType[MAX_PLAYER_TUTORIAL];

	static int m_nType;	//�`���[�g���A���̎��

	static int m_nSteps;	// �i�s�x
	static int m_nStepsMax;	// �i�s�x�̍ő吔
	static int m_nInstMax;	// �����̍ő吔


	//�`���[�g���A���Ŏg���e�N�X�`���N���X
	CUI_TEXTURE *m_apSteps[3];	//UI�e�N�X�`���N���X�̃|�C���^(�i�s�x)
	CUI_TEXTURE *m_pMission;	//UI�e�N�X�`���N���X�̃|�C���^(�ڕW)
	CUI_TEXTURE *m_pInst;		//UI�e�N�X�`���N���X�̃|�C���^(����)

	//��{����̃N���A���
	CLEARSTATE m_R_CameraState;	//�E�������Ƃ��̃N���A���
	CLEARSTATE m_L_CameraState;	//���������Ƃ��̃N���A���
	CLEARSTATE m_U_CameraState;	//��������Ƃ��̃N���A���
	CLEARSTATE m_D_CameraState;	//���������Ƃ��̃N���A���
	CLEARSTATE m_Move_State;	//�ړ��̃N���A���

	//�A�N�V�����p�[�g�̃N���A���
	CLEARSTATE m_AttackState;	//�U���̃N���A���
	CLEARSTATE m_ReloadState;	//�����[�h�̃N���A���

	//�`���[�g���A���̏��
	static TUTORIALSTATE m_TutorialState;	//�`���[�g���A���̏��
	CUI_TEXTURE *m_Succes;	//�`���[�g���A���̊����̃e�N�X�`��

	int m_nDisp;		//�N���A��\�����鎞��


};

#endif