//=============================================================================
//
// AI���� [AI.h]
// Author : Takuto Ishida
//
//=============================================================================
#ifndef _AI_H_
#define _AI_H_

#include "main.h"
#include "scene.h"
#include "game.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define AI_PRIORITY		(4)		// �����̗D��ԍ�
#define RALLYPOINT_MAX	(4)		// �����[�|�C���g�̍ő�ݒu��

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CModel;
class CMotion;
class CPlayer;
class CButtonManagerStrategy;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CAIMecha : public CScene
{
public:
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
		int nodeMax;								// �m�[�h�̑���
		int	index[NODEPOINT_MAX];					// �����̃m�[�h�ԍ�
		int connectNum[NODEPOINT_MAX];				// �ڑ��m�[�h��
		int connectIndex[NODEPOINT_MAX][CONNECT_MAX];	// �ڑ��m�[�h�ԍ�
		D3DXVECTOR3 pos[NODEPOINT_MAX];				// �e�m�[�h�̈ʒu
	}NodeState;

	typedef enum
	{
		AI_ACTION_NONE = 0,		// �Ȃ�
		AI_ACTION_MOVE,			// �ړ�
		AI_ACTION_WAIT,			// �ҋ@
		AI_ACTION_DISPATCH,		// �h��
		AI_ACTION_FOLLOW,		// �Ǐ]
		AI_ACTION_FOCUS_GOAL,	// �ڕW�d��
		AI_ACTION_RALLY,		// �����[�|�C���g
		AI_ACTION_ROUND_TRIP,	// ����
		AI_ACTION_FOLLOW_LONG,	// �������Ǐ]
		AI_ACTION_FOLLOW_SHORT,	// �Z�����Ǐ]
		AI_ACTION_ATTACK,		// �U��
		AI_ACTION_BATTLE,		// ����
		AI_ACTION_AVOIDANCE,	// ���
		AI_ACTION_PURSUIT,		// �ǐ�
		AI_ACTION_NOT_PURSUIT,	// ��ǐ�
		AI_ACTION_DISTURBANCE,	// ����
		AI_ACTION_WITHDRAWAL,	// �P��
		AI_ACTION_MAX,			// �ő�l
	}AI_ACTION;
	// =============================================================

	typedef enum
	{// ���
		STATE_APPEAR = 0,	// �o��
		STATE_NORMAL,		// �ʏ�
		STATE_DAMAGE,		// �_���[�W
		STATE_DEATH,		// ���S
		STATE_MAX
	}STATE;

	typedef enum
	{// �@�̂̎��
		MECHATYPE_WALKER = 0,
		MECHATYPE_DRONE,
		MECHATYPE_MAX
	}MECHATYPE;

	CAIMecha(int nPriority = AI_PRIORITY, CScene::OBJTYPE objType = CScene::OBJTYPE_AI);
	~CAIMecha();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CAIMecha* Create(CPlayer *pPlayer, MECHATYPE mecha, D3DXVECTOR3 pos);

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

	void Damage(int nDamage);

	CModel *GetModel(int nIdx) { return m_pModel[nIdx]; };

	MECHATYPE GetMechaType(void) { return m_mechaType; }

	int GetTeam(void) { return m_nTeam; };
	void SetDeath(bool bDeath) { m_bDeath = bDeath; };
	bool GetDeath(void) { return m_bDeath; };
	// =============================================================
	// �_�C�N�X�g���@�ɂ�郋�[�g�T��
	// =============================================================
	void AIUpdate(void);			// AI�̍X�V
	void Follow(void);				// �Ǐ]����
	void NodeSearch(bool node);		// �}�E�X���W����m�[�h����
	void AutoMove(void);			// �����ړ�
	void RootSearch(void);			// �ŒZ�o�H����
	void RallyRootSearch(void);		// �����[�|�C���g�ł̍ŒZ�o�H����
	void PatrolRootSearch(void);	// �����p�̍ŒZ�o�H����
	void Cancel(void);				// ���f
	void AddEdge(int first, int second, float weight, Node *node);	// �G�b�W�̒ǉ�
	void Dijkstra(int nodeMax, int start, int end, Node *node);		// �o�H�T��
	void FileLoad(char* pFileName);	// �}�b�v�f�[�^�t�@�C���̓ǂݍ���
									// =============================================================

private:
	D3DXMATRIX		m_mtxWorld;			// ���[���h�}�g���b�N�X
	D3DXVECTOR3		m_pos;				// �ʒu
	D3DXVECTOR3		m_rot;				// ��]
	D3DXVECTOR3		m_move;				// �ړ���
	D3DXVECTOR3		m_vtxMax;			// ���_���W�̍ő�l
	D3DXVECTOR3		m_vtxMin;			// ���_���W�̍ŏ��l
	CModel			**m_pModel;			// ���f���N���X�̃|�C���^�ϐ�
	CMotion			*m_pMotion;			// ���[�V�����N���X�̃|�C���^�ϐ�
	int				m_nCntState;		// �X�e�[�g�̃J�E���^
	int				m_nNumParts;		// �p�[�c��
	int				m_nCurMotion;		// ���݂̃��[�V����
	STATE			m_state;			// ���
	MECHATYPE		m_mechaType;		// �@�̂̎��
	CPlayer			*m_pPlayer;			// �v���C���[�̃|�C���^�ϐ�
	float			m_fSpeed;			// �ړ���
	int				m_nLife;			// ���݂̑ϋv��
	int				m_nLifeMax;			// �ő�ϋv��
	int				m_nCapacity;		// ���e��
	int				m_nAttack;			// �U����
	int				m_nNumShoot;		// ���ː�
	int				m_nTeam;			// �`�[��

										// =============================================================
										// �_�C�N�X�g���@�ɂ�郋�[�g�T��
										// =============================================================
	NodeState		m_NodeData;								// �}�b�v���

															// �p�[�g�֌W
	CGame::PART		m_bPartSwitch;							// �p�[�g���
	CGame::PART		m_bPartSwitchOld;						// �O��̃p�[�g���

															// �m�[�h�֌W
	int				m_nStartNode;							// �J�n�m�[�h�ԍ�
	int				m_nEndNode;								// �I���m�[�h�ԍ�
	int				m_nNodeOld;								// �O��̃m�[�h�ԍ�

															// �����[�֌W
	Node			m_node[NODEPOINT_MAX][NODEPOINT_MAX];	// �����[���̃m�[�h�̏��
	int				m_nRallyEndNode[NODEPOINT_MAX];			// �����[���̏I���m�[�h�ԍ�
	int				m_nRallyCount;							// �N���b�N��
	int				m_nRallyCountOld;						// �O��̃N���b�N��

															// �p�g���[���֌W
	bool			m_bPatrol;								// �����ړ���Ԃ�
	int				m_nPatrolStartNode;						// �p�g���[�����̊J�n�m�[�h�ԍ�
	D3DXVECTOR3		m_patrolWaypoint[NODEPOINT_MAX];		// �p�g���[�����̒��Ԓn�_
	int				m_nCountPatrolPoint;					// �p�g���[�����̖ڕW�܂ł̈ړ���

															// �����ړ��֌W
	D3DXVECTOR3		m_waypoint[NODEPOINT_MAX];				// ���Ԓn�_
	D3DXVECTOR3		m_posDest;								// �ڕW�ʒu
	D3DXVECTOR3		m_rotDest;								// �ڕW�ʒu
	int				m_nBreaktime;							// �x�e����
	int				m_nCountPoint;							// �ڕW�܂ł̈ړ���
	int				m_nPoint;								// ���݂̈ړ���
	bool			m_bGoal;								// �ړI�n�ɓ���������

															// ���W�b�N�c���[�֌W�̏��
	int				m_LogicTree[4];							// AI�ւ̎w���̏��
	AI_ACTION		m_AIAction[4];							// AI�̍s��

	bool			m_bDeath;								//���S���Ă��邩�ǂ���
};

#endif