//=============================================================================
//
// AI���� [AI.h]
// Author : Komatsu Keisuek
//
//=============================================================================
#ifndef _AI_H_
#define _AI_H_

#include "main.h"
#include "scene.h"
#include "game.h"
#include "player.h"
#include "tutorial.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define AI_PRIORITY		(4)		// �����̗D��ԍ�
#define RALLYPOINT_MAX	(4)		// �����[�|�C���g�̍ő�ݒu��

//=============================================================================
// �O���錾
//=============================================================================
class CInputMouse;
class CModel;
class CMotion;
class CPlayer;
class CButtonManagerStrategy;
class CNodeDataFiler;
class CSearch;
class CPin;
class CScene3D;

//=============================================================================
// �N���X��`
//=============================================================================
class CAIMecha : public CScene
{
public:
	typedef struct
	{// ���[�g�T���p
		std::vector<int> to;		// �ǂ̃m�[�h�ƂȂ����Ă��邩
		std::vector<float> cost;	// �G�b�W�̃R�X�g

									// �_�C�N�X�g���@�̂��߂ɕK�v�ȏ��
		bool done;		// �m��m�[�h���ǂ���
		float minCost;	// �X�^�[�g�m�[�h����̍ŏ��R�X�g
		int from;		// �ǂ̃m�[�h���痈����
	}Node;

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
		MECHATYPE_DRONE = 0,
		MECHATYPE_WORKER,
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

	void Damage(int nDamage, CScene *pScene);

	CModel *GetModel(int nIdx) { return m_pModel[nIdx]; };

	MECHATYPE GetMechaType(void) { return m_mechaType; }

	int GetTeam(void) { return m_nTeam; };

	void SetDeath(bool bDeath) { m_bDeath = bDeath; };
	bool GetDeath(void) { return m_bDeath; };

	CPlayer *GetPlayer(void) { return m_pPlayer; };

	int GetKillPlayerIdx(void) { return m_nKillPlayerIdx; };

	int GetNumParts(void) { return m_nNumParts; };

	int GetLifeMax(void) { return m_nLifeMax; };
	void SetLife(int nLife) { m_nLife = nLife; };
	int GetLife(void) { return m_nLife; };

	D3DXVECTOR3 GetPosCanon(void) { return m_posCanon; };
	float GetAngle(void) { return m_fAngle; };
	float GetAngleV(void) { return m_fAngleV; };
	void SetShoot(bool bShoot) { m_bShoot = bShoot; };
	bool GetShoot(void) { return m_bShoot; };
	int GetAttack(void) { return m_nAttack; };

	// �s��
	D3DXVECTOR3 &GetPinPos(void) { return m_PinPos; };
	bool &GetPinUse(void) { return m_bPin; };

	float GetBulletSpeed(void) { return m_fBulletSpeed; };

	CScene3D *GetAIIcon(void) { return m_pAIIcon; };
private:
	// =============================================================
	// �ړ��nAI�̊֐�
	// =============================================================
	void AIUpdate(void);					// AI�̍X�V
	void AIActionSet(CInputMouse *pMouse);	// AI�s���̐ݒ�
	void Attack(void);						// �U���֌W����
	void AutoMove(void);					// �����ړ�
	void Follow(void);						// �Ǐ]����
	void NodeSearch(bool node);				// �}�E�X���W����m�[�h����
	void RootSearch(void);					// �ŒZ�o�H����
	void RallyRootSearch(void);				// �����[�|�C���g�ł̍ŒZ�o�H����
	void PatrolRootSearch(void);			// �����p�̍ŒZ�o�H����
	void Cancel(void);						// ���f
	void AddEdge(int first, int second, float weight, Node *node);	// �G�b�W�̒ǉ�
	void Dijkstra(int nodeMax, int start, int end, Node *node);		// �o�H�T��

																	// =============================================================
																	// �퓬�nAI�̊֐�
																	// =============================================================

																	// =============================================================
																	// ��{�nAI�̕ϐ�
																	// =============================================================
	D3DXMATRIX		m_mtxWorld;			// ���[���h�}�g���b�N�X
	D3DXVECTOR3		m_pos;				// �ʒu
	D3DXVECTOR3		m_posOld;			// �O��ʒu
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
	CPlayer			*m_pEnemyPlayer[ENEMY_PLAYER_MAX];	// �G�v���C���[�̃|�C���^�ϐ�
	float			m_fSpeed;			// �ړ���
	int				m_nLife;			// ���݂̑ϋv��
	int				m_nLifeMax;			// �ő�ϋv��
	int				m_nCapacity;		// ���e��
	int				m_nAttack;			// �U����
	int				m_nNumShoot;		// ���ː�
	float			m_fBulletSpeed;		// �e��
	int				m_nTeam;			// �`�[��
	bool			m_bDeath;			// ���S���Ă��邩�ǂ���
	int				m_nKillPlayerIdx;

	// �s��
	D3DXVECTOR3		m_PinPos;			// ���������G�̈ʒu
	bool			m_bPin;				// �s���𗧂ĂĂ��邩

										// =============================================================
										// �ړ��nAI�̕ϐ�
										// =============================================================
	CNodeDataFiler *m_pNodeData;							// �}�b�v���ւ̃|�C���^

															// �p�[�g�֌W
	CGame::PART		m_bPartSwitch;							// �p�[�g���
	CGame::PART		m_bPartSwitchOld;						// �O��̃p�[�g���

	CTutorial::PART		m_bPartSwitch_T;					// �`���[�g���A���p�p�[�g���
	CTutorial::PART		m_bPartSwitchOld_T;					// �`���[�g���A���p�O��̃p�[�g���
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

															// =============================================================
															// �퓬�n��AI�ϐ�
															// =============================================================
	bool m_bFind;	// �����̏��
	D3DXVECTOR3 m_posCanon;									//�ˏo���̈ʒu
	float m_fAngle;											//���E�U�������̏��
	float m_fAngleV;										//�㉺�U�������̏��
	bool m_bShoot;											//���˂��Ă��邩�ǂ���

	CScene3D *m_pAIIcon;
};

#endif