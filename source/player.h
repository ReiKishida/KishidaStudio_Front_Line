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
#define TEAM_BLUE				(100)
#define TEAM_RED				(100)
#define MAX_UITEX				(10)		// UI�e�N�X�`������
#define PLAYER_UI_NUM		(2)		// ����UI�̖���
#define PLAYER_BOTTON		(4)		// ���X�|�[�����{�^���̐�
#define COLLECTIONDATA_MAX	(12)// ���W����f�[�^�̍ő�
#define RANDOM_MOVE_POINT	(9)	// �����_���ړ��̒n�_��
#define ENEMY_PLAYER_MAX	(2)	// �G�v���C���[�̐�
#define NODE_MAX	(256)		// �m�[�h�̍ő吔

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
		RESPAWN_START,				// �퓬�J�n
		RESPAWN_DEATH,				// �s���s�\
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

	// =============================================================
	// �_�C�N�X�g���@�ɂ�郋�[�g�T��
	// =============================================================
	typedef struct
	{
		std::vector<int> to;		// �ǂ̃m�[�h�ƂȂ����Ă��邩
		std::vector<float> cost;	// �G�b�W�̃R�X�g

									// �_�C�N�X�g���@�̂��߂ɕK�v�ȏ��
		bool done;		// �m��m�[�h���ǂ���
		float minCost;	// �X�^�[�g�m�[�h����̍ŏ��R�X�g
		int from;		// �ǂ̃m�[�h���痈����
	}Node;

	typedef struct
	{
		int nodeMax;							// �m�[�h�̑���
		int edgeMax;							// �G�b�W�̑���
		int	index[NODE_MAX];					// �����̃m�[�h�ԍ�
		int connectNum[NODE_MAX];				// �ڑ��m�[�h��
		int connectIndex[NODE_MAX][NODE_MAX];	// �ڑ��m�[�h�ԍ�
		D3DXVECTOR3 pos[NODE_MAX];				// �e�m�[�h�̈ʒu
	}NodeState;

	// =============================================================

	void Reload(void);			// �����[�h����
	void Respawn(RESPAWN respawn);		// ���C�t��0�ɂȂ������̏���
	void SelectRespawn(void);

	CPlayer(int nPriority = PLAYER_PRIORITY, CScene::OBJTYPE objType = CScene::OBJTYPE_PLAYER);
	~CPlayer();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CPlayer* Create(int nPlayerIdx, CMechaSelect::MECHATYPE mecha, D3DXVECTOR3 pos, bool bConnect);

	D3DXVECTOR3 GetPos(void) { return m_pos; };					// �ʒu�̎擾
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; };		// �ʒu�̐ݒ�

	D3DXVECTOR3 GetRot(void) { return m_rot; };					// �p�x�̎擾
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; };		// �p�x�̐ݒ�

	D3DXVECTOR3 GetMove(void) { return m_move; };				// �ړ��ʂ̎擾
	void SetMove(D3DXVECTOR3 move) { m_move = move; };	// �ړ��ʂ̐ݒ�

	D3DXMATRIX GetMtxWorld(void) { return m_mtxWorld; };					// ���[���h�}�g���b�N�X�̎擾
	void SetMtxWorld(D3DXMATRIX mtxWorld) { m_mtxWorld = mtxWorld; };	// ���[���h�}�g���b�N�X�̐ݒ�

	D3DXVECTOR3 GetVtxMax(void) { return m_vtxMax; }
	D3DXVECTOR3 GetVtxMin(void) { return m_vtxMin; }

	void Damage(int nDamage);

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

	void SetTeam(int nTeam) { m_nTeam = nTeam; };
	int GetTeam(void) { return m_nTeam; };
	bool &GetDeath(void) { return m_bDeath; };

	int GetLifeMax(void) { return m_nLifeMax; }

	// =============================================================
	// AI�֌W
	// =============================================================
	void AIUpdate(void);	// AI�̍X�V
	void AutoMove(void);	// �����ړ�
	void NodeSearch(void);	// �v���C���[���W����m�[�h����
	void RootSearch(void);	// �ŒZ�o�H����
	void AddEdge(int first, int second, float weight, Node *node);	// �G�b�W�̒ǉ�
	void Dijkstra(int nodeMax, int start, int end, Node *node);	// �o�H�T��
	void FileLoad(char* pFileName);

	CPlayer *GetPlayer(void) { return m_pPlayer; };
	static void SetSearchPos(D3DXVECTOR3 pos) { m_searchPos = pos; };
	// =============================================================

private:
	void Movement(void);
	void Shoot(void);
	void Angle(void);
	void FieldWalk(void);

	D3DXMATRIX		m_mtxWorld;			// ���[���h�}�g���b�N�X
	D3DXVECTOR3		m_pos;				// �ʒu
	D3DXVECTOR3		m_rot;				// ��]
	D3DXVECTOR3		m_move;				// �ړ���
	D3DXVECTOR3		m_vtxMax;			// ���_���W�̍ő�l
	D3DXVECTOR3		m_vtxMin;			// ���_���W�̍ŏ��l
	CModel			**m_pModel;			// ���f���N���X�̃|�C���^�ϐ�
	CMotion			*m_pUpperMotion;	// �㔼�g���[�V�����N���X�̃|�C���^�ϐ�
	CMotion			*m_pLowerMotion;	// �����g���[�V�����N���X�̃|�C���^�ϐ�
	CShadow			*m_pShadow;			// �e�̃|�C���^�ϐ�
	float			m_fRotDest;			// �ړI�̊p�x
	int				m_nCntState;		// �X�e�[�g�̃J�E���^
	int				m_nNumParts;		// �p�[�c��
	int				m_nCurMotion;		// ���݂̃��[�V����
	STATE			m_state;			// ���
	float			m_fCameraAngle;		// �J�����̌���
	int				m_nDamageTime;		// �_���[�W���󂯂����̍d������
	CScene3DBill	*m_pReticle;		// ���e�B�N��
	int				m_nRemBullet;
	int				m_nCntReRoad;
	CUI_TEXTURE *m_pUITex[MAX_UITEX];
	CUI_NUMBER	*m_pUINum[PLAYER_UI_NUM];
	CGauge2D *m_pGauge;
	int m_nDiff;
	int m_nCntAnim, m_nPatternAnim;		// �A�j���[�V����
	RESPAWN m_Respawn;		// ���X�|�[���^�C�v�̎擾
	CButton2D *m_apButtonUI[PLAYER_BOTTON];	// �{�^���N���X�̃|�C���^�ϐ�
	CMouseCursor2D *m_pCursor;								// �J�[�\���N���X�̃|�C���^�ϐ�
	int m_nTimer;					// �^�C�}�[
	float m_nDisTime;			// ���S�\������

	int				m_nPlayerIdx;		// �v���C���[�ԍ�
	D3DXVECTOR3		m_posOld;			// �ߋ��̈ʒu
	bool			m_bShoot;			// �e�𔭎˂��Ă��邩�ǂ���
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

	// =============================================================
	// AI�֌W
	// =============================================================
	D3DXVECTOR3 m_waypoint[NODEPOINT_MAX];	// ���Ԓn�_
	D3DXVECTOR3 m_collectionPos[ENEMY_PLAYER_MAX][COLLECTIONDATA_MAX];	// ���W�����f�[�^
	D3DXVECTOR3	m_posDest;				// �ڕW�ʒu
	D3DXVECTOR3 m_totalCollectPos;		// ���W�����f�[�^�̍��v�l
	int m_nCollectionTimer;				// ���W���鎞��
	int m_nBreaktime;					// �x�e����
	int m_nStartNode;					// �J�n�m�[�h�ԍ�
	int m_nEndNode;						// �I���m�[�h�ԍ�
	int m_nNewEndNode;					// �V�K�I���m�[�h�ԍ�
	int m_nCountPoint;					// �ڕW�܂ł̈ړ���
	int m_nCountCollect;				// ���W�����f�[�^�̐�
	int m_nPoint;						// ���݂̈ړ���
	int m_nNearTotalCollectNumber;		// ���ϒl�Ɉ�ԋ߂��m�[�h�̃i���o�[
	int m_nGoalCount;					// �ŏI�ړI�n�ɓ��B������
	bool m_bGoal;						// �ړI�n�ɓ���������
	bool m_bPartSwitch;					// �A�N�V�����ƃX�g���e�W�[�̐؂�ւ�
	bool m_bCollectSwitch;				// ���ϒl����o�����@�̐؂�ւ�
	static	D3DXVECTOR3	m_searchPos;	// �N���b�N���ʒu

	int m_nMovePoint[RANDOM_MOVE_POINT] = { 26, 49, 73, 92, 115, 143, 171, 176, 202 };	// �����_���ړ��̓o�^�n�_

	NodeState m_NodeData;						// �}�b�v���ւ̃|�C���^
	static CPlayer *m_pPlayer;					// �v���C���[�N���X�ւ̃|�C���^
	//static CEnemy *m_pEnemy[ENEMY_PLAYER_MAX];			// �G�l�~�[�ւ̃|�C���^
};

#endif