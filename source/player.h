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
#define PLAYER_PRIORITY		(4)		// �����̗D��ԍ�
#define PLAYER_LIFE				(100)
#define TEAM_BLUE				(100)
#define TEAM_RED				(100)
#define PLAYER_BULLET			(100)
#define MAX_UITEX				(2)

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

	CPlayer(int nPriority = PLAYER_PRIORITY, CScene::OBJTYPE objType = CScene::OBJTYPE_PLAYER);
	~CPlayer();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CPlayer* Create(void);

	D3DXVECTOR3 GetPos(void) { return m_pos; };					// �ʒu�̎擾
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; };		// �ʒu�̐ݒ�

	D3DXVECTOR3 GetRot(void) { return m_rot; };					// �p�x�̎擾
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; };		// �p�x�̐ݒ�

	D3DXVECTOR3 GetMove(void) { return m_move; };				// �ړ��ʂ̎擾
	void SetMove(D3DXVECTOR3 move) { m_move = move; };	// �ړ��ʂ̐ݒ�

	D3DXMATRIX GetMtxWorld(void) { return m_mtxWorld; };					// ���[���h�}�g���b�N�X�̎擾
	void SetMtxWorld(D3DXMATRIX mtxWorld) { m_mtxWorld = mtxWorld; };	// ���[���h�}�g���b�N�X�̐ݒ�

	D3DXVECTOR3 GetVtxMax(void);

	void Damage(int nDamage);

	D3DXVECTOR3 CalcScreenToWorld(float fScreenX, float fScreenY);

	void Reload(void);

private:
	void Movement(void);
	void Shoot(void);
	void Angle(void);
	void FieldWalk(void);

	D3DXMATRIX		m_mtxWorld;			// ���[���h�}�g���b�N�X
	D3DXVECTOR3		m_pos;				// �ʒu
	D3DXVECTOR3		m_rot;				// ��]
	D3DXVECTOR3		m_move;				// �ړ���
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
	int				m_nRemBullet;
	int				m_nCntReRoad;
	CUI_TEXTURE *m_pUITex[MAX_UITEX];
	CUI_NUMBER	*m_pUINum;
	CGauge2D *m_pGauge;
};

#endif