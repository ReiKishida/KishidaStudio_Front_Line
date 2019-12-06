////=============================================================================
////
//// �G���� [enemy.h]
//// Author : Takuto Ishida
////
////=============================================================================
//#ifndef _ENEMY_H_
//#define _ENEMY_H_
//
//#include "scene.h"
//#include "motion.h"
//#include <vector>
//#include <math.h>
//
////*****************************************************************************
//// �}�N����`
////*****************************************************************************
//#define ENEMY_PRIORITY	(4)
//
//#define NODE_MAX	(256)		// �m�[�h�̍ő吔
////*****************************************************************************
//// �O���錾
////*****************************************************************************
//class CModel;
//class CShadow;
//class CLife3D;
//class CScene3D;
//
////*****************************************************************************
//// �N���X��`
////*****************************************************************************
//class CEnemy : public CScene
//{
//public:
//	// =============================================================
//	// �_�C�N�X�g���@�ɂ�郋�[�g�T��
//	// =============================================================
//	typedef struct
//	{
//		std::vector<int> to;		// �ǂ̃m�[�h�ƂȂ����Ă��邩
//		std::vector<float> cost;	// �G�b�W�̃R�X�g
//
//		// �_�C�N�X�g���@�̂��߂ɕK�v�ȏ��
//		bool done;		// �m��m�[�h���ǂ���
//		float minCost;	// �X�^�[�g�m�[�h����̍ŏ��R�X�g
//		int from;		// �ǂ̃m�[�h���痈����
//	}Node;
//
//	typedef struct
//	{
//		int nodeMax;							// �m�[�h�̑���
//		int edgeMax;							// �G�b�W�̑���
//		int	index[NODE_MAX];					// �����̃m�[�h�ԍ�
//		int connectNum[NODE_MAX];				// �ڑ��m�[�h��
//		int connectIndex[NODE_MAX][NODE_MAX];	// �ڑ��m�[�h�ԍ�
//		D3DXVECTOR3 pos[NODE_MAX];				// �e�m�[�h�̈ʒu
//	}NodeState;
//	// =============================================================
//
//	CEnemy(int nPriority = ENEMY_PRIORITY, CScene::OBJTYPE objType = CScene::OBJTYPE_ENEMY);
//	~CEnemy();
//	HRESULT Init(void);
//	void Uninit(void);
//	void Update(void);
//	void Draw(void);
//
//	static CEnemy* Create(D3DXVECTOR3 pos, int nType);
//
//	static HRESULT Load(void);				// �e�N�X�`����ǂݍ��ށimanager�ɂ����j
//	static void Unload(void);				// �ǂݍ��񂾃e�N�X�`�������
//
//	int GetType(void) { return m_nType; };
//
//	float GetAttackRange(void) { return m_fAttackRange; };
//	void SetAttackRange(float fAttackRange) { m_fAttackRange = fAttackRange; };
//
//	D3DXVECTOR3 GetPos(void) { return m_pos; };
//	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; };
//
//	void Damage(int nDamage, int nMotion);		// �U�����󂯂��Ƃ��̏���
//	bool Collision(D3DXVECTOR3 pos, float fRange);	// �����蔻��
//
//	// =============================================================
//	// �_�C�N�X�g���@�ɂ�郋�[�g�T��
//	// =============================================================
//	static void SetSearchPos(D3DXVECTOR3 pos) { m_searchPos = pos; };
//	static bool GetPartSwitch(void) { return m_bPartSwitch; };
//
//	void AIUpdate(void);
//	void NodeSearch(bool node);
//	void RootSearch(void);
//	void RallyRootSearch(void);
//	void AutoMove(void);
//	void AddEdge(int first, int second, float weight, Node *node);
//	void Dijkstra(int nodeMax, int start, int end, Node *node);
//	void FileLoad(char* pFileName);
//	// =============================================================
//
//private:
//	typedef struct
//	{// �t�@�C���ǂݍ��ݍ\����
//		int nIdx;			// ���f���ԍ�
//		int nParent;		// �e�ԍ�
//		D3DXVECTOR3 pos;	// �ʒu
//		D3DXVECTOR3 rot;	// ����
//	}DATA;
//
//	void GetModelData(CModel **&pModel);
//
//	static int		*m_pNumParts;				// �p�[�c��
//	static char		***m_pModelName;			// ���f����
//	static DATA		**m_pFileData;				// �t�@�C���ǂݎ��
//	static int		m_nNumType;					// ��ނ̐�
//
//	D3DXMATRIX		m_mtxWorld;					// ���[���h�}�g���b�N�X
//	D3DXVECTOR3		m_pos;						// �ʒu
//	D3DXVECTOR3		m_rot;						// ��]
//	float			m_fRotDest;					// �ړI�̊p�x
//	D3DXVECTOR3		m_move;						// �ړ���
//	int				m_nCntDamage[ATTACK_MOTION];	// �_���[�W�̃J�E���^
//	int				m_nCntBullet;				// �e�����܂ł̃J�E���^
//	int				m_nType;					// �G�̎��
//	CShadow			*m_pShadow;					// �e�̃|�C���^�ϐ�
//	CModel			**m_pModel;					// ���f���N���X�̃|�C���^�ϐ�
//	CMotion			*m_pMotion;					// ���[�V�����N���X�̃|�C���^�ϐ�
//	float			m_fWalkRange;				// ���G�͈�
//	float			m_fAttackRange;				// �U��������͈�
//	int				m_nIntervalAttack;			// �U���Ԋu
//	bool			m_bAttack;					// �U���t���O
//	CLife3D			*m_pLife;					// 3D�̗̓N���X�̃|�C���^�ϐ�
//	bool			m_bDeath;					// ���S�t���O
//	float			m_fAlpha;					// �����x
//	bool			m_bDamage[ATTACK_MOTION];	// �_���[�W���
//
//	// =============================================================
//	// �_�C�N�X�g���@�ɂ�郋�[�g�T��
//	// =============================================================
//	NodeState m_RootData;					// �}�b�v���
//
//	// �p�[�g�֌W
//	static bool m_bPartSwitch;				// �A�N�V�����ƃX�g���e�W�[�̐؂�ւ�
//
//	// �m�[�h�֌W
//	Node m_node[NODE_MAX][NODE_MAX];
//	int m_nStartNode;						// �J�n�m�[�h�ԍ�
//	int m_nEndNode;							// �I���m�[�h�ԍ�
//	int m_nNodeOld;							// �O��̃m�[�h�ԍ�
//
//	// �����[�֌W
//	int m_nRallyEndNode[NODE_MAX];			// �����[���̏I���m�[�h�ԍ�[num]
//	bool m_bRally;							// �����[��Ԃ�
//	int m_nRallyCount;						// �N���b�N��
//	int m_nRallyCountOld;					// �O��̃N���b�N��
//
//	// ���[�g�����֌W
//	static	D3DXVECTOR3	m_searchPos;		// �N���b�N���ʒu
//
//	// �����ړ��֌W
//	D3DXVECTOR3 m_waypoint[NODE_MAX];		// ���Ԓn�_
//	D3DXVECTOR3	m_posDest;					// �ڕW�ʒu
//	int m_nBreaktime;						// �x�e����
//	int m_nCountPoint;						// �ڕW�܂ł̈ړ���
//	int m_nPoint;							// ���݂̈ړ���
//	bool m_bGoal;							// �ړI�n�ɓ���������
//
//	// �|�C���^�֌W
//	static CScene3D *m_pScene3D[NODE_MAX];	// 3D�|���S���ւ̃|�C���^
//	// =============================================================
//
//};
//
//#endif