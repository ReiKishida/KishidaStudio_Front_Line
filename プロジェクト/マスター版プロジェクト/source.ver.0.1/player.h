//=============================================================================
//
// �v���C���[���� [player.h]
// Author : Takuto Ishida
//
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "scene.h"
#include "mechaSelect.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define PLAYER_PRIORITY		(4)		// �����̗D��ԍ�
#define TEAM_BLUE			(100)
#define TEAM_RED			(100)
#define PLAYER_UI_NUM		(2)		// ����UI�̖���
#define COLLECTIONDATA_MAX	(12)	// ���W����f�[�^�̍ő吔
#define RANDOM_MOVE_POINT	(11)	// �����_���ړ��̒n�_��
#define ENEMY_PLAYER_MAX	(2)		// �G�v���C���[�̐�
#define AI_MAX				(2)		// ��l�������AI�̐�
#define PLAYER_MAX			(2)		// �v���C���[�̐�

// �����[�h
#define RELOAD_TEX					(2)			// �����[�h�Ŏg�p����e�N�X�`����

// ���X�|�[��
#define RESPAWN_TEX					(3)			// ���X�|�[���Ŏg�p����e�N�X�`����

// ���X�|�[���ʒu�I��
#define SELECTRESPAWN_BOTTON		(4)			// ���X�|�[���ʒu�I���{�^��
#define SERECTRESPAWN_TEX			(4)			// ���X�|�[���ʒu�I�𒆂̃e�N�X�`����

// ���W�I�`���b�g����
#define RADIOCHAT_BOTTON			(8)			// ���W�I�`���b�g�{�^���̐�
#define RADIOCHAT_MESSAGE			(2)			// ���W�I�`���b�g�{�^���̐�
#define RADIOCHAT_BOTTON_PATTERN	(8)			// ���W�I�`���b�g�摜�̃p�^�[����
#define RADIOCHAT_BOTTON_WIDTH		(200.0f)	// �{�^���̕�
#define RADIOCHAT_BOTTON_HEIGHT		(200.0f)	// �{�^���̍���
#define RADIOCHAT_MESS_WIDTH		(400.0f)	// ���b�Z�[�W�̕�
#define RADIOCHAT_MESS_HEIGHT		(80.0f)		// ���b�Z�[�W�̍���

// �I�v�V����
#define OPTION_SELECT							(5)			// �J�������x�ݒ�̐�

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CModel;
class CMotion;
class CShadow;
class CScene3DBill;
class CUI_TEXTURE;
class CGauge2D;
class CUI_NUMBER;
class CMouseCursor2D;
class CButton2D;
class CInputMouse;
class CAIMecha;
class CSearch;
class CNodeDataFiler;
class CPin;
class CBulletPin;
class CScene3D;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CPlayer : public CScene
{
public:
	typedef enum
	{// ���
		STATE_APPEAR = 0,	// �o��
		STATE_NORMAL,		// �ʏ�
		STATE_DAMAGE,		// �_���[�W
		STATE_DEATH,		// ���S
		STATE_MAX
	}STATE;

	typedef enum
	{	// ���X�|�[�����̏��
		RESPAWN_NONE = 0,		// �ʏ���
		RESPAWN_START,			// �퓬�J�n
		RESPAWN_DEATH,			// �s���s�\
		RESPAWN_SELECT,			// ���X�|�[���I��
		RESPAWN_MAX
	} RESPAWN;

	typedef enum
	{	// ���X�|�[���n�_
		POINT_A = 0,
		POINT_B,
		POINT_C,
		POINT_D,
		POINT_MAX
	} POINT;

	typedef enum
	{	// ���W�I�`���b�g�̎��
		RADIOCHAT_OK = 0,		// ����
		RADIOCHAT_OPPSITE,		// ����
		RADIOCHAT_THANKS,		// ����
		RADIOCHAT_APOLOGY,		// �Ӎ�
		RADIOCHAT_NICESHOOT,	// �i�C�X�V���b�g
		RADIOCHAT_FOLLOW,		// ���Ă����I
		RADIOCHAT_RECESSION,	// ��ނ���I
		RADIOCHAT_SUPPORT,		// ���삵��I
		RADIOCHAT_MAX
	} RADIOCHAT;

	typedef struct
	{// ���[�g�T���p
		std::vector<int> to;		// �ǂ̃m�[�h�ƂȂ����Ă��邩
		std::vector<float> cost;	// �G�b�W�̃R�X�g

		// �_�C�N�X�g���@�̂��߂ɕK�v�ȏ��
		bool done;		// �m��m�[�h���ǂ���
		float minCost;	// �X�^�[�g�m�[�h����̍ŏ��R�X�g
		int from;		// �ǂ̃m�[�h���痈����
	}Node;

	CPlayer(int nPriority = PLAYER_PRIORITY, CScene::OBJTYPE objType = CScene::OBJTYPE_PLAYER);
	~CPlayer();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CPlayer* Create(int nPlayerIdx, CMechaSelect::MECHATYPE mecha, D3DXVECTOR3 pos, bool bConnect);

	D3DXVECTOR3 GetPos(void) { return m_pos; };			// �ʒu�̎擾
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; };		// �ʒu�̐ݒ�

	D3DXVECTOR3 GetRot(void) { return m_rot; };			// �p�x�̎擾
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; };		// �p�x�̐ݒ�

	D3DXVECTOR3 GetMove(void) { return m_move; };		// �ړ��ʂ̎擾
	void SetMove(D3DXVECTOR3 move) { m_move = move; };	// �ړ��ʂ̐ݒ�

	D3DXMATRIX GetMtxWorld(void) { return m_mtxWorld; };				// ���[���h�}�g���b�N�X�̎擾
	void SetMtxWorld(D3DXMATRIX mtxWorld) { m_mtxWorld = mtxWorld; };	// ���[���h�}�g���b�N�X�̐ݒ�

	D3DXVECTOR3 GetVtxMax(void) { return m_vtxMax; }
	D3DXVECTOR3 GetVtxMin(void) { return m_vtxMin; }

	void Damage(int nDamage, CScene *pScene);

	D3DXVECTOR3 CalcScreenToWorld(float fScreenX, float fScreenY);

	CModel *GetModel(int nIdx) { return m_pModel[nIdx]; };

	int GetPlayerIdx(void) { return m_nPlayerIdx; };
	void SetPlayerIdx(int nIdx) { m_nPlayerIdx = nIdx; };

	bool GetShoot(void) { return m_bShoot; };
	void SetShoot(bool bShoot) { m_bShoot = bShoot; };

	float GetRotDest(void) { return m_fRotDest; };
	void SetRotDest(float fRotDest) { m_fRotDest = fRotDest; };

	int GetBulletCapacity(void) { return m_nCapacity; }
	int GetAttack(void) { return m_nAttack; }

	RESPAWN GetRespawn(void) { return m_Respawn; }

	float GetAngle(int nIdx) { return m_pAngle[nIdx]; }
	float GetAngleV(int nIdx) { return m_pAngleV[nIdx]; }

	int GetNumShoot(void) { return m_nNumShoot; }

	int GetBulletLife(void) { return m_nBulletLife; };

	void SetTeam(int nTeam) { m_nTeam = nTeam; };
	int GetTeam(void) { return m_nTeam; };
	bool &GetDeath(void) { return m_bDeath; };

	void SetLife(int nLife) { m_nLife = nLife; };
	int GetLife(void) { return m_nLife; };
	int GetLifeMax(void) { return m_nLifeMax; }

	bool GetWince(void);

	CPlayer *GetPlayer(void) { return m_pPlayer; };
	static void SetSearchPos(D3DXVECTOR3 pos) { m_searchPos = pos; };
	CAIMecha *GetMyAI(int nIdx) { return m_pAI[nIdx]; };
	bool &GetPlayerConnect(void) { return m_bConnect; };

	bool GetReload(void) { return m_bReload; };

	CMechaSelect::MECHATYPE	GetMechaType(void) { return m_mecha; };

	//==================
	//   �s���֌W
	//==================
	// �����̃s�����
	bool &GetPinUse(void) { return m_bPin; };
	int &GetPinLife(void) { return m_nPinLife; };
	D3DXVECTOR3 &GetPinPos(void) { return m_PinPos; };
	// AI�̃s�����
	bool &GetAIPinUse(int nValue) { return m_bAIPin[nValue]; };
	D3DXVECTOR3 &GetAIPinPos(int nValue) { return m_AIPinPos[nValue]; };
	// �����̃s�����
	bool &GetAllyPinUse(void) { return m_bAllyPin; };
	D3DXVECTOR3 &GetAllyPinPos(void) { return m_AllyPinPos; };
	// ����AI�̃s�����
	bool &GetAllyAIPinUse(int nValue) { return m_bAllyAIPin[nValue]; };
	D3DXVECTOR3 &GetAllyAIPinPos(int nValue) { return m_AllyAIPinPos[nValue]; };

	float GetBulletSpeed(void) { return m_fBulletSpeed; };

	// ���W�I�`���b�g
	RADIOCHAT GetRadioChat(void) { return m_radiochat; }									// ���W�I�`���b�g���̎擾
	void SetRadioChat(RADIOCHAT radiochat) { m_radiochat = radiochat; }						// ���W�I�`���b�g�̐ݒ�
	void SetChat(bool bChat) { m_bChat = bChat; };											//�`���b�g���̐ݒu����
	bool GetChat(void) { return m_bChat; }													// �`���b�g���̎擾
	void SetAllyChat(bool bAllyChat) { m_bAllyChat = bAllyChat; }							// �����̃`���b�g���g�p���Ă��邩�ǂ����̐ݒ�
	bool GetAllyChat(void) { return m_bAllyChat; };
	void SetAllyRadioChat(RADIOCHAT allyRadioChat) { m_allyRadiochat = allyRadioChat; }		// �����̃`���b�g���̐ݒ�
	bool GetChatBotton(void) { return m_bChatBotton; }

	// �I�v�V����
	bool GetOption(void) { return m_bOption; }																// �I�v�V������Ԃ̎擾
	void SetOption(bool bOption) { m_bOption = bOption; }											// �I�v�V������Ԃ̐ݒ�
	int GetSelectOption(void) { return m_nSelectOption; }												// �I�����ꂽ�J�������x�擾
	void SetSelectOption(int nSelectOption) { m_nSelectOption = nSelectOption; }		// �I�����ꂽ�J�������x�ݒ�

	int GetKillPlayerIdx(void) { return m_nKillPlayerIdx; };
	int GetNumParts(void) { return m_nNumParts; };

	int GetRemainBullet(void) { return m_nRemBullet; };

	void SetDestUpper(D3DXVECTOR3 destUpper) { m_fRotDestUpper = destUpper; };
	D3DXVECTOR3 GetDestUpper(void) { return m_fRotDestUpper; };

	CScene3D *GetPlayerIcon(void) { return m_pPlayerIcon; };

	void ReleasePlayerUI(void);
private:
	void Movement(void);
	void Shoot(void);
	void Angle(void);
	void SetParticle(void);
	void Reload(bool bReload);			// �����[�h����
	void Respawn(RESPAWN respawn);		// ���C�t��0�ɂȂ������̏���
	void SelectRespawn(void);			// ���X�|�[���ʒu�I������
	void ChatBotton(void);				// ���W�I�`���b�g�{�^���̐���
	void ChatMess(bool bChat);			// �{�^����������āA���b�Z�[�W�\��
	void CreateRespawnPosIcon(void);
	void AllyChatMess(void);
	void Option(bool bOption);						// �I�v�V�����ݒ�
	void UninitOption(void);							// �I�v�V�����̔j��
	void CreateRadioChatButton(void);		// �`���b�g�{�^���̐���
	void UninitRadioChatButton(void);			// �`���b�g�{�^���̔j��
	void PinUpdateSingle(void);			// �V���O�����̃s���֌W�̍X�V
	void PinUpdateMulti(void);			// �}���`���̃s���֌W�̍X�V

	// �퓬�nAI�̏���
	void LoadBattleFile(void);	// �퓬�p�̃e�L�X�g�̓ǂݍ���
	bool Distance(void);		// �͈͒T��
	void Battle(void);			// �퓬
	void BattleMovent(void);	// �퓬���̈ړ�
	void CpuShoot(void);		// �e����

	// �ړ��nAI�̏���
	void AIUpdate(void);	// AI�̍X�V
	void AutoMove(void);	// �����ړ�
	void NodeSearch(void);	// �v���C���[���W����m�[�h����
	void RootSearch(void);	// �ŒZ�o�H����
	void AddEdge(int first, int second, float weight, Node *node);	// �G�b�W�̒ǉ�
	void Dijkstra(int nodeMax, int start, int end, Node *node);	// �o�H�T��

	// =============================================================
	// ��{���̕ϐ�
	// =============================================================
	D3DXMATRIX		m_mtxWorld;			// ���[���h�}�g���b�N�X
	D3DXVECTOR3		m_pos;				// �ʒu
	D3DXVECTOR3		m_rot;				// ��]
	D3DXVECTOR3		m_move;				// �ړ���
	D3DXVECTOR3		m_vtxMax;			// ���_���W�̍ő�l
	D3DXVECTOR3		m_vtxMin;			// ���_���W�̍ŏ��l
	CModel			**m_pModel;			// ���f���N���X�̃|�C���^�ϐ�
	CMotion			*m_pUpperMotion;	// �㔼�g���[�V�����N���X�̃|�C���^�ϐ�
	CMotion			*m_pLowerMotion;	// �����g���[�V�����N���X�̃|�C���^�ϐ�
	//CShadow			*m_pShadow;			// �e�̃|�C���^�ϐ�
	float			m_fRotDest;			// �ړI�̊p�x
	int				m_nCntState;		// �X�e�[�g�̃J�E���^
	int				m_nNumParts;		// �p�[�c��
	STATE			m_state;			// ���
	float			m_fCameraAngle;		// �J�����̌���
	int				m_nDamageTime;		// �_���[�W���󂯂����̍d������
	CScene3DBill	*m_pReticle;		// ���e�B�N��
	int m_nTimer;						// �^�C�}�[

	int				m_nPlayerIdx;		// �v���C���[�ԍ�
	D3DXVECTOR3		m_posOld;			// �ߋ��̈ʒu
	bool			m_bShoot;			// �e�𔭎˂��Ă��邩�ǂ���
	float			m_fBulletSpeed;		// �e��
	int				m_nBulletLife;		// �e�̎���
	int				m_nLife;			// �ϋv��

	int				m_nCapacity;		// ���e��
	int				m_nAttack;			// �U����
	int				m_nNumShoot;		// �������ː�
	int				m_nDispertion;		// �΂��
	int				m_nReload;			// �����[�h����
	int				m_nLifeMax;			// �ϋv��
	float			m_fSpeed;			// �ړ���

	CMechaSelect::MECHATYPE	m_mecha;	// �@�̂̎��
	float			*m_pAngle;
	float			*m_pAngleV;
	int				m_nTeam;
	bool			m_bDeath;
	int				m_nCntShoot;		// ���ˊԊu
	bool			m_bShootButton;		// �e�̔��˃{�^�������t���O
	bool			m_bConnect;			// �ڑ����Ă��邩�ǂ���
	bool			m_bWince;			// �Ђ��ł��邩�ǂ���

	// �s��
	CPin			*m_pPin;					// �s���̃|�C���^
	CBulletPin		*m_pPinBullet;				// �s�����Ĕ���p�̒e�̃|�C���^
	bool			m_bPin;						// �s���𗧂ĂĂ��邩
	int				m_nPinLife;					// �s���̕\������
	D3DXVECTOR3		m_PinPos;					// �s���̈ʒu
												// AI�̃s��
	CPin			*m_pAIPin[AI_MAX];			// AI�̃s���̃|�C���^
	bool			m_bAIPin[AI_MAX];			// AI���s���𗧂ĂĂ��邩
	D3DXVECTOR3		m_AIPinPos[AI_MAX];			// AI�̃s���̈ʒu
												// �����̃s��
	CPin			*m_pAllyPin;				// �����̃s���̃|�C���^
	bool			m_bAllyPin;					// �������s���𗧂ĂĂ��邩
	D3DXVECTOR3		m_AllyPinPos;				// �����̃s���̈ʒu
												// ����AI�̃s��
	CPin			*m_pAllyAIPin[AI_MAX];		// ����AI�̃s���̃|�C���^
	bool			m_bAllyAIPin[AI_MAX];		// ����AI���s���𗧂ĂĂ��邩
	D3DXVECTOR3		m_AllyAIPinPos[AI_MAX];		// ����AI�̃s���̈ʒu

	// �����̈ʒu�s��
	CPin			*m_pAllyPosPin;				// �����̈ʒu�s���̃|�C���^
	D3DXVECTOR3		m_AllyPosPinPos;			// �����̈ʒu

	int				m_nCntCPURespawn;			// CPU�v���C���[�̃��X�|�[���J�E���^�[
	// =============================================================
	// UI�֌W
	// =============================================================
	CMouseCursor2D	*m_pCursor;							// �J�[�\���N���X�̃|�C���^�ϐ��i���W�I�`���b�g�A���X�|�[���ʒu�I���j
	int				m_nDiff;							// ����
	int				m_nRemBullet;						// �c�e

	// �����[�h
	CUI_TEXTURE		*m_pUITexReload[RELOAD_TEX];		// �����[�h�Ŏg�p����e�N�X�`��
	CGauge2D		*m_pGauge;							// �Q�[�W
	int				m_nCntReRoad;
	bool			m_bReload;							// �����[�h��Ԃ��ǂ���

	// ���X�|�[��
	CUI_TEXTURE		*m_pUITexRespawn[RESPAWN_TEX];				// ���X�|�[���Ŏg�p����e�N�X�`��
	CUI_TEXTURE		*m_pUIRespawnPosIcon[SELECTRESPAWN_BOTTON];	// ���X�|�[���ʒu�̃A�C�R��
	CUI_NUMBER		*m_pUINumRespawn;							// ������A�J�E���^�[
	RESPAWN			m_Respawn;									// ���X�|�[���^�C�v�̎擾
	int				m_nRespawnTimer;							// �^�C�}�[
	float			m_nDisTime;									// ���S�\������

	// ���X�|�[���ʒu�I��
	CButton2D		*m_pUISelectResBotton[SELECTRESPAWN_BOTTON];// ���W�I�`���b�g�{�^��UI
	CUI_TEXTURE		*m_pUITexSelectRes[SERECTRESPAWN_TEX];		// ���W�I�`���b�g���b�Z�[�WUITex
	POINT			m_point;									// ���X�|�[���ʒu�̊Ǘ�

	// ���W�I�`���b�g
	CButton2D		*m_pUIRadioBotton[RADIOCHAT_BOTTON];		// ���W�I�`���b�g�{�^��UI
	CUI_TEXTURE		*m_pUITexRadio;								// ���W�I�`���b�g���b�Z�[�WUITex
	CUI_TEXTURE		*m_pUITexAllyRadio;							// �������W�I�`���b�g���b�Z�[�WUITex
	RADIOCHAT		m_radiochat;								// ���W�I�`���b�g�Ǘ�
	RADIOCHAT		m_allyRadiochat;							// �����̃`���b�g���
	bool			m_bChat;									// �`���b�g�J�n���ǂ���
	bool			m_bAllyChat;								// �����̃`���b�g
	bool			m_bCol;										// �F�̊Ǘ�
	bool			m_bAllyCol;									// �����̐F�̊Ǘ�
	bool			m_bChatBotton;								// �`���b�g�{�^�����������ǂ���
	int				m_moveSpeed;								// �e�N�X�`�������X�s�[�h
	int				m_nTexTimer;								// �e�N�X�`���\���^�C�}�[
	int				m_nAllyTimer;
	int				m_nRadioChat;														// ���W�I�`���b�g�؂�ւ�

	// �I�v�V����
	CButton2D			*m_pUIButtonOption;												// �I�v�V����UI�|�C���^
	bool						m_bOption;																// �I�v�V������Ԃ��ǂ���
	CUI_TEXTURE		*m_pUITexOption;													// �I�v�V�����ݒ�Ŏg�p����UITex
	CButton2D			*m_pUIButtonBack;													// �߂�{�^��UI�|�C���^
	CButton2D			*m_pUIButtonSelect[OPTION_SELECT];				// �J�����̑��x�I��
	int						m_nSelectOption;													//	�J�������x�̍���

	int m_nKillPlayerIdx;					//�L���v���C���[�̔ԍ�

	// =============================================================
	// �ړ��nAI�̕ϐ�
	// =============================================================
	D3DXVECTOR3 m_waypoint[NODEPOINT_MAX];	// ���Ԓn�_
	D3DXVECTOR3 m_collectionPos[COLLECTIONDATA_MAX];	// ���W�����f�[�^
	D3DXVECTOR3	m_posDest;				// �ڕW�ʒu
	D3DXVECTOR3	m_rotDest;				// �ڕW����
	D3DXVECTOR3	m_rotDestOld;			// �O��̖ڕW����
	D3DXVECTOR3 m_totalCollectPos;		// ���W�����f�[�^�̍��v�l
	int m_nBreaktime;					// �x�e����
	int m_nStartNode;					// �J�n�m�[�h�ԍ�
	int m_nEndNode;						// �I���m�[�h�ԍ�
	int m_nNewEndNode;					// �V�K�I���m�[�h�ԍ�
	int m_nCountPoint;					// �ڕW�܂ł̈ړ���
	int m_nCountCollect;				// ���W�����f�[�^�̐�
	int m_nPoint;						// ���݂̈ړ���
	int m_nNearTotalCollectNumber;		// ���ϒl�Ɉ�ԋ߂��m�[�h�̃i���o�[
	int m_nGoalCount;					// �ŏI�ړI�n�ɓ��B������
	int m_nVigilanceCount;				// �x�����̌�������̃J�E���g
	bool m_bGoal;						// �ړI�n�ɓ���������
	bool m_bPartSwitch;					// �A�N�V�����ƃX�g���e�W�[�̐؂�ւ�
	bool m_bCollectSwitch;				// ���ϒl����o�����@�̐؂�ւ�
	static	D3DXVECTOR3	m_searchPos;	// �N���b�N���ʒu
	int m_nMovePoint[RANDOM_MOVE_POINT] = { 0, 24, 47, 72, 144, 183, 188, 229, 279, 287, 316 }; // �����_���ړ��̓o�^�n�_

	CNodeDataFiler *m_pNodeData;		// �}�b�v���ւ̃|�C���^
	static CPlayer *m_pPlayer;			// �v���C���[�N���X�ւ̃|�C���^
	CAIMecha *m_pAI[AI_MAX];			// ������AI�N���X�ւ̃|�C���^

	// =============================================================
	//AI�퓬�n�̕ϐ�
	// =============================================================
	//�퓬�ɕK�v�ȕϐ�
	CSearch **m_pSearch;			// �T���̃|�C���^�N���X

	//�F���n
	CPlayer *m_pFindPlayer;			// ���������v���C���[�̃|�C���^
	float m_fSearchLength;			// �F������
	float m_fSearchAngle;			// �F���p�x
	D3DXVECTOR3 m_SearchVec_0;		// �F���p�x�N�g���P
	D3DXVECTOR3 m_SearchVec_1;		// �F���p�x�N�g���Q
	D3DXVECTOR3 m_SearchVec_2;		// �F���p�x�N�g���Q
	bool m_bFind;					// �����̏��
	bool m_bFindOld;				// �O��̔������

	//�U���n
	float	m_fRange;				// �͈�

	//�ړ��n
	D3DXVECTOR3 m_fRotDestUpper;	// �㔼�g

	CScene3D *m_pPlayerIcon;
};

#endif