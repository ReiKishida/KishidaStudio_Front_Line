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
#define PLAYER_PRIORITY		(4)				// �����̗D��ԍ�
#define PLAYER_LIFE				(100)
#define TEAM_BLUE				(100)
#define TEAM_RED				(100)
#define MAX_UITEX				(10)		// UI�e�N�X�`������
#define PLAYER_UI_NUM		(2)				// ����UI�̖���
#define PLAYER_BOTTON		(4)				// ���X�|�[�����{�^���̐�

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

	void Reload(void);			// �����[�h����
	void Respawn(RESPAWN respawn);		// ���C�t��0�ɂȂ������̏���
	void SelectRespawn(void);

	CPlayer(int nPriority = PLAYER_PRIORITY, CScene::OBJTYPE objType = CScene::OBJTYPE_PLAYER);
	~CPlayer();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CPlayer* Create(int nPlayerIdx, CMechaSelect::MECHATYPE mecha,D3DXVECTOR3 pos);

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

	float GetAngle(int nIdx) { return m_pAngle[nIdx]; };
	float GetAngleV(int nIdx) { return m_pAngleV[nIdx]; };

	int GetNumShoot(void) { return m_nNumShoot; };
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
	float			m_fSpeed;			// ���x
	int				m_nLife;			// �̗�
	int				m_nDamageTime;		// �_���[�W���󂯂����̍d������
	CScene3DBill	*m_pReticle;		// ���e�B�N��
	int				m_nRemBullet, m_nPlayerLife;
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
	float m_nDisTime;				// ���S�\������
	float *m_pAngle;
	float *m_pAngleV;

	int				m_nPlayerIdx;		// �v���C���[�ԍ�
	D3DXVECTOR3		m_posOld;			// �ߋ��̈ʒu
	bool			m_bShoot;			// �e�𔭎˂��Ă��邩�ǂ���

	int				m_nCapacity;		// ���e��
	int				m_nAttack;			// �U����
	int				m_nNumShoot;		// �������ː�
	int				m_nDispertion;		// �΂��
	int				m_nReload;			// �����[�h����
	CMechaSelect::MECHATYPE	m_mecha;	// �@�̂̎��

};

#endif