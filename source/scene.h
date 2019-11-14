//=============================================================================
//
// �I�u�W�F�N�g���� [scene.h]
// Author : Takuto Ishida
//
//=============================================================================
#ifndef _SCENE_H_
#define _SCENE_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_OBJECT		(128)		// �|���S���̍ő吔
#define NUM_PRIORITY	(8)			// �D�揇�ʂ̐�

//=====================================
// �N���X��`
//=====================================
class CScene
{
public:
	typedef enum
	{
		OBJTYPE_NONE = 0,			// �����l
		OBJTYPE_TITLE,				// �^�C�g��
		OBJTYPE_MENU,				// ���j���[
		OBJTYPE_MECHASELECT,		// �@�̑I��
		OBJTYPE_MATCHING,			// �}�b�`���O
		OBJTYPE_TUTORIAL,			// �`���[�g���A��
		OBJTYPE_GAME,				// �Q�[��
		OBJTYPE_RESULT,				// ���U���g
		OBJTYPE_RANKING,			// �����L���O
		OBJTYPE_FADE,				// �t�F�[�h
		OBJTYPE_PLAYER,				// �v���C���[
		OBJTYPE_ENEMY,				// �G
		OBJTYPE_MODEL,				// ���f��
		OBJTYPE_BG,					// �w�i
		OBJTYPE_FIELD,				// �t�B�[���h
		OBJTYPE_BULLET,				// �e
		OBJTYPE_BULLET_COLLISION,	// �e�̓����蔻��
		OBJTYPE_SHADOW,				// �e
		OBJTYPE_ORBIT,				// �O��
		OBJTYPE_GAUGE,				// �Q�[�W
		OBJTYPE_LIFE,				// ���C�t
		OBJTYPE_PAUSE,				// �|�[�Y
		OBJTYPE_STAGE,				// �X�e�[�W
		OBJTYPE_MOTION,				// ���[�V����
		OBJTYPE_MOTION_MANAGER,		// ���[�V�����Ǘ�
		OBJTYPE_LOGO,				// ���S
		OBJTYPE_SETMANAGER,			// �z�u���Ǘ�
		OBJTYPE_MODELOBJECT,		// �z�u��
		OBJTYPE_TIME,				// �^�C��
		OBJTYPE_PARTICLE,			// �p�[�e�B�N��
		OBJTYPE_NUMBER,				// ����
		OBJTYPE_MOUSE,				// �}�E�X�J�[�\��
		OBJTYPE_BUTTON,				// �{�^��
		OBJTYPE_BUTTONLINE,			// �{�^�����Ȃ���
		OBJTYPE_BUTTON_MANAGER,		// �{�^���̊Ǘ�
		OBJTYPE_ITEM,				// �A�C�e��
		OBJTYPE_UI_NUMBER,
		OBJTYPE_UI_TEXTURE,
		OBJTYPE_UI_TEXTURE3D,
		OBJTYPE_COLLISION,
		OBJTYPE_MAX					// �I�u�W�F�N�g�̎�ނ̍ő吔
	}OBJTYPE;

	CScene(int nPriority = 3, OBJTYPE objType = OBJTYPE_NONE);
	virtual ~CScene();
	virtual HRESULT Init(void) = 0;
	virtual void Uninit(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;

	static void ReleaseAll(void);
	static void UpdateAll(void);
	static void DrawAll(void);
	static void ReleaseDeath(void);

	static void ModeUninit(void);
	static void StageUninit(void);
	static void UIUninit(void);

	void SwapPriority(int nPriority);		// �D�揇�ʂ̕ύX

	void SetObjType(OBJTYPE objType) { m_objType = objType; }
	OBJTYPE GetObjType(void) { return m_objType; }

	static CScene *GetSceneTop(int nPriority) { return m_apTop[nPriority]; }
	CScene *GetSceneNext(void) { return m_pNext; }

protected:
	void Release(void);

private:
	static CScene *m_apTop[NUM_PRIORITY];		// �擪�̃I�u�W�F�N�g�ւ̃|�C���^
	static CScene *m_apCur[NUM_PRIORITY];		// ���݂̃I�u�W�F�N�g�ւ̃|�C���^�i�Ō���j
	CScene *m_pPrev;			// �O�̃I�u�W�F�N�g�ւ̃|�C���^
	CScene *m_pNext;			// ���̃I�u�W�F�N�g�ւ̃|�C���^
	int m_nPriority;			// �D�揇��
	OBJTYPE m_objType;			// �I�u�W�F�N�g�̎��
	bool m_bDeath;				// ���S�t���O
};

#endif