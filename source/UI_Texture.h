//=============================================================================
//
// UI_Texture���� [UI_Texture.h]
// Author : Ayaka Hakozaki
//
//=============================================================================
#ifndef _UI_TEXTURE_H_
#define _UI_TEXTURE_H_

#include "scene2D.h"
#include "scene3D.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define UI_PRIORITY	(6)		// �����̗D�揇��

//*****************************************************************************
// �O���錾
//*****************************************************************************

//*****************************************************************************
// 2D�e�N�X�`���N���X��`
//*****************************************************************************
class CUI_TEXTURE : public CScene2D
{
public:
	typedef enum
	{	// �t���[���̎��
		UIFLAME_NONE,						// �t���[���Ȃ�
		UIFLAME_WEAPON,					// ����
		UIFLAME_PLAYER_HP,				// �v���C���[�̗̑�
		UIFLAME_DRONE,						// �h���[��
		UIFLAME_WORKER,					// AI01
		UIFLAME_TEAM_BLUE,				// BLUE�`�[��
		UIFLAME_TEAM_RED,				// RED�`�[��
		UIFLAME_TIMER,						// �^�C�}�[�t���[��
		UIFLAME_ACTION_PART,			// �A�N�V�����p�[�g�؂�ւ�
		UIFLAME_STRATEGY_PART,		// �X�g���e�W�[�p�[�g�؂�ւ�
		UIFLAME_BG_00,						// Bg00
		UIFLAME_BG_01,						// Bg01
		UIFLAME_FLONTLINE,				// �^�C�g���i�t�����g���C���j
		UIFLAME_PRESSBOTTON,			// �v���X�{�^��
		UIFLAME_CHANGE,					// �p�[�g�؂�ւ�
		UIFLAME_1P_INFO,					// 1P�̏������i�X�g���e�W�[�p�[�g�Ŏg�p�j
		UIFLAME_2P_INFO,					// 2P�̏������i�X�g���e�W�[�p�[�g�Ŏg�p�j
		UIFLAME_FLAME_WHITE,			// �����̔��t���[���i�X�g���e�W�[�p�[�g�Ŏg�p�j
		UIFLAME_FLAME_BLUE,			// �����̐t���[���i�X�g���e�W�[�p�[�g�Ŏg�p�j
		UIFLAME_STRATEGY_BG,			// �X�g���e�W�[�p�[�g�̔w�i�i�X�g���e�W�[�p�[�g�Ŏg�p�j
		UIFLAME_RELOAD,					// �����[�h���S�i�A�N�V�����p�[�g�Ŏg�p�j
		UIFLAME_MATCHING_BG,			// �}�b�`���O�w�i�i�}�b�`���O��ʂŎg�p�j
		UIFLAME_MATCHING_BG_00,	// �}�b�`���OBG_00�i�}�b�`���O��ʂŎg�p�j
		UIFLAME_MATCHING_BG_01,	// �}�b�`���OBG_01�i�}�b�`���O��ʂŎg�p�j
		UIFLAME_VS,								// VS���S�i�}�b�`���O��ʂŎg�p�j
		UIFLAME_PLAYER_NUM,			// �v���C���[�i���o�[�i�}�b�`���O��ʂŎg�p�j
		UIFLAME_BLUE_MECHA_LIST,	// BLUE�`�[���̋@�̃��X�g�i�}�b�`���O��ʂŎg�p�j
		UIFLAME_RED_MECHA_LIST,	// RED�`�[���̋@�̃��X�g�i�}�b�`���O��ʂŎg�p�j
		UIFLAME_START,						// �퓬�J�n���S�i�A�N�V�����p�[�g�Ŏg�p�j
		UIFLAME_DEATH,						// �s���s�\���S�i�A�N�V�����p�[�g�Ŏg�p�j
		UIFLAME_TILE_PATTERN,			// �^�C���A�j���[�V�����i�A�N�V�����p�[�g�Ŏg�p�j
		UIFLAME_RESPAWN,					// ���A���S�i�A�N�V�����p�[�g�Ŏg�p�j
		UIFLAME_MAP,							// ���X�|�[���}�b�v�i�A�N�V�����p�[�g�Ŏg�p�j
		UIFLAME_RESPAWN_FLAME,	// ���X�|�[���t���[���i�A�N�V�����p�[�g�Ŏg�p�j
		UIFLAME_TITLE,							// ���X�|�[���^�C�g���i�A�N�V�����p�[�g�Ŏg�p�j
		UIFLAME_SELECT_RESPAWN,	// ���X�|�[���I���i�A�N�V�����p�[�g�Ŏg�p�j
		UIFLAME_MAX							// �^�C�v�̍ő吔
	} UI_TEXTYPE;

	CUI_TEXTURE(int nPriority = UI_PRIORITY, CScene::OBJTYPE objType = CScene::OBJTYPE_UI_TEXTURE);
	~CUI_TEXTURE();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CUI_TEXTURE *Create(D3DXVECTOR3 pos, float fWidth, float fHeight, UI_TEXTYPE UITexType);		// ����

private:
	D3DXVECTOR3	m_pos;						// �ʒu
	D3DXCOLOR		m_col;						// �J���[
	UI_TEXTYPE		m_UITexType;			// UI�̎�ނ��Ǘ�
	int						m_nCntBgMove;		// �w�i�X�N���[��
	int						m_nFlash;				// �_�Ńt���[����
	int						m_nCntAnim, m_nPatternAnim;			// �A�j���[�V�����̃J�E���^�[
};

//*****************************************************************************
// 3D�e�N�X�`���N���X��`
//*****************************************************************************
class CUI_TEXTURE3D : public CScene3D
{
public:
	typedef enum
	{
		UI3DTEX_NONE,
		UI3DTEX_ACTION,
		UI3DTEX_STRATEGY,
		UI3DTEX_MAX
	} UI3DTEX;

	CUI_TEXTURE3D(int nPriority = UI_PRIORITY, CScene::OBJTYPE objType = CScene::OBJTYPE_UI_TEXTURE3D);
	~CUI_TEXTURE3D();
	static CUI_TEXTURE3D *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, UI3DTEX UI3DTex);		// ����
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	D3DXVECTOR3	m_pos;					// �ʒu
	D3DXVECTOR3	m_size;				// �T�C�Y
	D3DXCOLOR		m_col;					// �J���[
	UI3DTEX				m_UI3dTex;			// 3DUI�̃^�C�v�Ǘ�
};

#endif