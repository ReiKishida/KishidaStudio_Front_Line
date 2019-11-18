//=============================================================================
//
// ���[�V�����Ǘ����� [motion.h]
// Author : Takuto Ishida
//
//=============================================================================
#ifndef _MOTION_H_
#define _MOTION_H_

#include "scene.h"

#include <stdio.h>

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_FILE		(6)		// �t�@�C����
#define MOTION_PRIORITY	(0)		// �����̗D�揇��
#define ATTACK_MOTION	(CMotionManager::TYPE_MAX - CMotionManager::TYPE_ATTACK01)	// �U�����[�V������

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CModel;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CMotionManager : public CScene
{// ���[�V�����Ǘ��N���X
public:
	typedef enum
	{
		TYPE_NEUTRAL = 0,	// �j���[�g����
		TYPE_WALK,			// ����
		TYPE_BACK,			// �o�b�N����

		TYPE_DAMAGE,		// �_���[�W
		TYPE_SLIP,			// ����
		TYPE_DEATH,			// ���S
		TYPE_ATTACK01,		// �U���P
		TYPE_ATTACK02,		// �U���Q
		TYPE_ATTACK03,		// �U���R
		TYPE_MAX
	}TYPE;

	typedef struct
	{// �L�[�v�f
		int nIdxModel;		// ������������郂�f���ԍ�
		float fPosX;		// X���W
		float fPosY;		// Y���W
		float fPosZ;		// Z���W
		float fRadius;		// ���a
		int nStartFrame;	// �J�n�t���[��
		int nEndFrame;		// �I���t���[��
	}COLLISION;

	typedef struct
	{// �L�[�v�f
		int nIdxModel;		// ���f���ԍ�
		float fPosX;		// X���W
		float fPosY;		// Y���W
		float fPosZ;		// Z���W
		int nStartFrame;	// �J�n�t���[��
		int nEndFrame;		// �I���t���[��
	}ORBIT;

	typedef struct
	{// �L�[�v�f
		float posX;
		float posY;
		float posZ;
		float rotX;
		float rotY;
		float rotZ;
	}KEY;

	CMotionManager(int nPriority = MOTION_PRIORITY, CScene::OBJTYPE objType = CScene::OBJTYPE_MOTION_MANAGER);
	~CMotionManager();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CMotionManager *Create(int nFile);

	static HRESULT Load(void);				// �e�N�X�`����ǂݍ��ށimanager�ɂ����j
	static void Unload(void);				// �ǂݍ��񂾃e�N�X�`�������

	int GetNumKey(int nMotion) { return m_pNumKey[m_nFile][nMotion]; };
	int *GetFlame(int nMotion) { return m_pFrame[m_nFile][nMotion]; };
	bool GetLoop(int nMotion) { return m_pLoop[m_nFile][nMotion]; };
	CMotionManager::KEY **GetKey(int nMotion) { return m_pKey[m_nFile][nMotion]; };
	CMotionManager::COLLISION *GetCollision(int nMotion) { return m_pCollision[m_nFile][nMotion]; };
	CMotionManager::ORBIT *GetOrbit(int nMotion) { return m_pOrbit[m_nFile][nMotion]; };
	int GetType(int nMotion) { return m_pType[m_nFile][nMotion]; };
	void SetFile(int nFile) { m_nFile = nFile; };

	int GetNumMotion(void) { return m_nNumMotion[m_nFile]; };

private:
	static int m_nParts[MAX_FILE];				// �p�[�c��
	static int *m_pNumKey[MAX_FILE];			// �L�[��
	static int **m_pFrame[MAX_FILE];			// �t���[��
	static KEY ***m_pKey[MAX_FILE];				// �p�[�c���Ƃ̃L�[�v�f
	static bool *m_pLoop[MAX_FILE];				// ���[�v���邩�ǂ���
	static int *m_pType[MAX_FILE];				// ���[�V�����̎��
	static COLLISION **m_pCollision[MAX_FILE];	// �����蔻��
	static ORBIT **m_pOrbit[MAX_FILE];			// �O��
	static int m_nNumMotion[MAX_FILE];			// ���[�V������
	int m_nFile;								// �t�@�C���̎w��
};

/*========================*/
/*  ���[�V�����Đ��N���X  */
/*========================*/
class CMotion : public CScene
{
public:
	CMotion(int nPriority = MOTION_PRIORITY, CScene::OBJTYPE objType = CScene::OBJTYPE_MOTION);
	~CMotion();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CMotion *Create(int nFile, int nParts, CModel **pModel, int nFootIdx = 0);			// ���[�V�������̐���

	int GetMotion(void) { return m_nCurMotion; };
	void SetMotion(CMotionManager::TYPE type);

	int GetType(void) { return m_nType; };

	bool Collision(D3DXVECTOR3 pos);

	int GetCurKey(void)		{ return m_nCurKey; };			// ���݂̃L�[�̎擾
	int GetCurFlame(void)	{ return m_nCntFlame; };		// ���݂̃t���[�����擾
	int GetNumKey(void)		{ return m_nKey; };				// �L�[���̎擾
	int GetAttackIdx(void);									// �U������̂��郂�f��

	int GetIdxLowerBody(void) { return m_nFootIdx; };		// �����g�ȉ��̔ԍ��̎擾
	void SetIdxLowerBody(int nIdx) { m_nFootIdx = nIdx; };	// �����g�ȉ��̔ԍ��̐ݒ�

private:
	void Motion(void);							// ���[�V�����̍Đ�

	CModel **m_pModel;							// ���f���N���X�̃|�C���^�ϐ�
	int	m_nCurKey;								// ���݂̃L�[
	int *m_pFrame;								// �t���[��
	CMotionManager::KEY **m_pKey;				// �p�[�c���Ƃ̃L�[�v�f
	int m_nKey;									// �L�[��
	int m_nParts;								// �p�[�c��
	bool m_bLoop;								// ���[�v���邩�ǂ���
	int m_nCntFlame;							// �t���[���J�E���g
	D3DXVECTOR3 m_posInit;						// ���S���f���̏����ʒu
	int m_nCurMotion;							// ���݂̃��[�V����
	int m_nType;								// ���[�V�����̎��
	CMotionManager::COLLISION *m_pCollision;	// �����蔻��̃|�C���^
	CMotionManager::ORBIT *m_pOrbit;			// �O��
	CMotionManager *m_pMotionManager;			// ���[�V�����Ǘ��N���X�̃|�C���^
	int m_nCntTotalFlame;						// �����t���[����
	bool m_bInit;								// �ŏ��̈ʒu�ɂȂ������ǂ���
	int m_nFootIdx;								// �����g�ȉ��̔ԍ�
};

#endif