//=============================================================================
//
// �v���C���[���� [player.h]
// Author : Takuto Ishida
//
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "scene.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define PLAYER_PRIORITY		(4)	// �����̗D��ԍ�
#define COLLECTIONDATA_MAX	(12)// ���W����f�[�^�̍ő�
#define RANDOM_MOVE_POINT	(9)	// �����_���ړ��̒n�_��

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CModel;
class CMotion;
class CShadow;
class CLife2D;
class CScene3DBill;
class CScene3D;
class CEnemy;
class CInputMouse;
class CFile;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CPlayer : public CScene
{
public:
	typedef struct
	{// �L�[�v�f
		float posX;
		float posY;
		float posZ;
		float rotX;
		float rotY;
		float rotZ;
	}KEY;

	typedef struct
	{// �L�[���
		int nFrame;		// �t���[��
		KEY *pKey;	// �L�[�v�f
	}KEY_INFO;

	typedef struct
	{// �W�����v����^�C�~���O����
		D3DXVECTOR3		pos;		// ����p�̈ʒu
		float			fAngle;		// �n�ʂ̊p�x
		bool			bJump;		// �W�����v�{�^�������^�C�~���O
		CScene3DBill	*pIcon;		// �W�����v�^�C�~���O�ɏo���A�C�R��
	}JUDG_JUMP;

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
	// =============================================================

	CPlayer(int nPriority = PLAYER_PRIORITY, CScene::OBJTYPE objType = CScene::OBJTYPE_PLAYER);
	~CPlayer();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CPlayer* Create(void);

	D3DXVECTOR3 GetPos(void) { return m_pos; };					// �ʒu�̎擾
	const void SetPos(D3DXVECTOR3 pos) { m_pos = pos; };		// �ʒu�̐ݒ�

	D3DXVECTOR3 GetRot(void) { return m_rot; };					// �p�x�̎擾
	const void SetRot(D3DXVECTOR3 rot) { m_rot = rot; };		// �p�x�̐ݒ�

	D3DXVECTOR3 GetMove(void) { return m_move; };				// �ړ��ʂ̎擾
	const void SetMove(D3DXVECTOR3 move) { m_move = move; };	// �ړ��ʂ̐ݒ�

	bool GetSlipFlag(void) { return m_bSlip; };					// ������Ԃ̎擾
	void SetSlipFlag(bool bSlip) { m_bSlip = bSlip; };			// ������Ԃ̐ݒ�

	D3DXMATRIX GetMtxWorld(void) { return m_mtxWorld; };					// ���[���h�}�g���b�N�X�̎擾
	const void SetMtxWorld(D3DXMATRIX mtxWorld) { m_mtxWorld = mtxWorld; };	// ���[���h�}�g���b�N�X�̐ݒ�

	bool Collision(D3DXVECTOR3 pos, float fRange);

	void Damage(int nDamage);

	// =============================================================
	// AI�֌W
	// =============================================================
	void AIUpdate(CInputMouse *pMouse);	// AI�̍X�V
	void AutoMove(void);	// �����ړ�
	void NodeSearch(void);	// �v���C���[���W����m�[�h����
	void RootSearch(void);	// �ŒZ�o�H����
	void AddEdge(int first, int second, float weight, Node *node);	// �G�b�W�̒ǉ�
	void Dijkstra(int nodeMax, int start, int end, Node *node);	// �o�H�T��
	
	CPlayer *GetPlayer(void) { return m_pPlayer; };
	static void SetSearchPos(D3DXVECTOR3 pos) { m_searchPos = pos; };
	// =============================================================

private:
	void Movement(void);
	void Jump(float fAngleOld);
	void Tilt(void);

	D3DXMATRIX		m_mtxWorld;			// ���[���h�}�g���b�N�X
	D3DXVECTOR3		m_pos;				// �ʒu
	D3DXVECTOR3		m_rot;				// ��]
	D3DXVECTOR3		m_move;				// �ړ���
	float			m_rotDest;			// �ړI�̊p�x
	int				m_nCntState;		// �X�e�[�g�̃J�E���^
	float			m_fSpeed;			// ���x
	float			m_fSlipSpeed;		// ���鑬�x
	float			m_fAngleSlip;		// �������
	float			m_fJumpSpeed;		// �W�����v���̑��x
	bool			m_bJump;			// �W�����v�t���O
	bool			m_bSlip;			// �����Ă邩�ǂ���
	CModel			**m_pModel;			// ���f���N���X�̃|�C���^�ϐ�
	int				m_nNumParts;		// �p�[�c��
	int				m_nCurMotion;		// ���݂̃��[�V����
	CMotion			*m_pMotion;			// ���[�V�����N���X�̃|�C���^�ϐ�
	CShadow			*m_pShadow;			// �e�̃|�C���^�ϐ�
	CLife2D			*m_pLife;			// ���C�t�N���X�̃|�C���^�ϐ�
	bool			m_bDamage;			// �_���[�W���󂯂����ǂ���
	bool			m_bMove;			// �����邩�ǂ���
	int				m_nCntParticle;		// �p�[�e�B�N�����o���܂ł̃J�E���^
	float			m_fAngle;			// �n�ʂƌ����̌�_�̊p�x
	bool			m_bDash;			// �W�����v���ɋ󒆃_�b�V�����g�������ǂ���
	bool			m_bStageFade;		// �X�e�[�W�J�ڒ�
	bool			m_bJumpKey;			// �W�����v�{�^�������t���O
	JUDG_JUMP		m_judgJump;			// �W�����v�^�C�~���O����̍\����

	// =============================================================
	// AI�֌W
	// =============================================================
	D3DXVECTOR3 m_waypoint[NODEPOINT_MAX];	// ���Ԓn�_
	D3DXVECTOR3 m_collectionPos[ENEMY_MAX][COLLECTIONDATA_MAX];	// ���W�����f�[�^
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

	CFile *m_pNodeData;							// �}�b�v���ւ̃|�C���^
	static CPlayer *m_pPlayer;					// �v���C���[�N���X�ւ̃|�C���^
	static CScene3D *m_pScene3D[NODEPOINT_MAX];	// 3D�|���S���ւ̃|�C���^
	static CEnemy *m_pEnemy[ENEMY_MAX];			// �G�l�~�[�ւ̃|�C���^
};

#endif