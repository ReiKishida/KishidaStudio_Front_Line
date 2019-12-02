//=============================================================================
//
// UI_Texture���� [UI_Texture.cpp]
// Author : Ayaka Hakozaki
//
//=============================================================================
#include "UI_Texture.h"
#include "texture.h"
#include "number.h"
#include "input.h"
#include "manager.h"
#include "debugProc.h"
#include "fade.h"
#include "UI_Number.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define UI_TEX_SCROLLSPEED		(0.005f)	// �w�i�̃X�N���[���X�s�[�h
#define UI_TEX_FLASH					(40)			// �_�Ńt���[����
#define UI_TEX_RELOAD_TIME		(60 * 2)	// �����[�h����
#define UI_TEX_ANIM_SPEED		(10)			// �^�C���A�j���[�V����
#define UI_TEX_ANIM_PATTERN	(8)			// �^�C���̃p�^�[����

//*****************************************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************************************

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CUI_TEXTURE::CUI_TEXTURE(int nPriority, CScene::OBJTYPE objType) : CScene2D(nPriority, objType)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_texPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_colRadioMess = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	m_nCntBgMove = 0;
	m_nFlash = 0;
	m_nCntAnim = 0;
	m_nPatternAnim = 0;
	m_nCntReload = 0;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CUI_TEXTURE::~CUI_TEXTURE()
{
}

//=============================================================================
// ��������
//=============================================================================
CUI_TEXTURE *CUI_TEXTURE::Create(D3DXVECTOR3 pos, float fWidth, float fHeight, UI_TEXTYPE UITexType)
{
	CUI_TEXTURE *pUITex = NULL;	// UI�N���X�̃|�C���^�ϐ�

	pUITex = new CUI_TEXTURE;		// �������m��

	if (NULL != pUITex)
	{	// �������m�ۂł���
		pUITex->m_UITexType = UITexType;	// �t���[���^�C�v�̐ݒ�
		pUITex->Init();									// ����������
		pUITex->SetPos(pos);						// �ʒu�̐ݒ�
		pUITex->SetSize(fWidth, fHeight);		// �T�C�Y�̐ݒ�
	}

	return pUITex;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CUI_TEXTURE::Init(void)
{
	// ����������
	CScene2D::Init();

	//****************************************
	// �e�N�X�`���E�F�E�e�^�C�v�������̐ݒ�
	//****************************************
	switch (m_UITexType)
	{
	case UIFLAME_NONE:
		CScene2D::SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.8f));
		break;

		//****************************************
		// �^�C�g��
		//****************************************
	case UIFLAME_BG_00:
		CScene2D::BindTexture(CTexture::GetTexture(CTexture::TEXTURE_BG_00));
		CScene2D::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		break;

	case UIFLAME_BG_01:
		CScene2D::BindTexture(CTexture::GetTexture(CTexture::TEXTURE_BG_01));
		CScene2D::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		break;

	case UIFLAME_FLONTLINE:
		CScene2D::BindTexture(CTexture::GetTexture(CTexture::TEXTURE_FLONTLINE));
		CScene2D::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		break;

	case UIFLAME_PRESSBOTTON:
		CScene2D::BindTexture(CTexture::GetTexture(CTexture::TEXTURE_PRESSBOTTON));
		CScene2D::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		break;

		//****************************************
		// �}�b�`���O���
		//****************************************
	case UIFLAME_MATCHING_BG:
		CScene2D::BindTexture(CTexture::GetTexture(CTexture::TEXTURE_MATCHING_BG));
		CScene2D::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		break;

	case UIFLAME_MATCHING_BG_00:
		CScene2D::BindTexture(CTexture::GetTexture(CTexture::TEXTURE_MATCHING_BG_00));
		CScene2D::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		break;

	case UIFLAME_MATCHING_BG_01:
		CScene2D::BindTexture(CTexture::GetTexture(CTexture::TEXTURE_MATCHING_BG_01));
		CScene2D::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		break;

	case UIFLAME_VS:
		CScene2D::BindTexture(CTexture::GetTexture(CTexture::TEXTURE_VS));
		CScene2D::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		break;

	case UIFLAME_PLAYER_NUM:
		CScene2D::BindTexture(CTexture::GetTexture(CTexture::TEXTURE_PLAYER_NUM));
		CScene2D::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		break;

	case UIFLAME_BLUE_MECHA_LIST:
		CScene2D::BindTexture(CTexture::GetTexture(CTexture::TEXTURE_BLUE_MECHA_LIST));
		CScene2D::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		break;

	case UIFLAME_RED_MECHA_LIST:
		CScene2D::BindTexture(CTexture::GetTexture(CTexture::TEXTURE_RED_MECHA_LIST));
		CScene2D::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		break;

		//****************************************
		// �A�N�V�����E�X�g���e�W�[����
		//****************************************
	case UIFLAME_TEAM_BLUE:
		CScene2D::BindTexture(CTexture::GetTexture(CTexture::TEXTURE_UPPER_UI));
		CScene2D::SetTex(0,1,3);
		CScene2D::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		break;

	case UIFLAME_TEAM_RED:
		CScene2D::BindTexture(CTexture::GetTexture(CTexture::TEXTURE_UPPER_UI));
		CScene2D::SetTex(1, 1, 3);
		CScene2D::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		break;

	case UIFLAME_PLAYER_HP:
		CScene2D::BindTexture(CTexture::GetTexture(CTexture::TEXTURE_PLAYER_FLAME));
		CScene2D::SetTex(0,1,4);
		CScene2D::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		break;

	case UIFLAME_DRONE:
		CScene2D::BindTexture(CTexture::GetTexture(CTexture::TEXTURE_AI_MECHA_FLAME));
		CScene2D::SetTex(0, 1, 2);
		CScene2D::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		break;

	case UIFLAME_WORKER:
		CScene2D::BindTexture(CTexture::GetTexture(CTexture::TEXTURE_AI_MECHA_FLAME));
		CScene2D::SetTex(1, 1, 2);
		CScene2D::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		break;

	case UIFLAME_WEAPON:
		CScene2D::BindTexture(CTexture::GetTexture(CTexture::TEXTURE_WEAPON_FLAME));
		CScene2D::SetTex(0, 1, 4);
		CScene2D::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		break;

	case UIFLAME_TIMER:
		CScene2D::BindTexture(CTexture::GetTexture(CTexture::TEXTURE_UPPER_UI));
		CScene2D::SetTex(2, 1, 3);
		CScene2D::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		break;

	case UIFLAME_CHANGE:
		m_nFlash = UI_TEX_FLASH;
		CScene2D::BindTexture(CTexture::GetTexture(CTexture::TEXTURE_CHANGE));
		CScene2D::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		break;

	case UIFLAME_ACTION_PART:
		CScene2D::BindTexture(CTexture::GetTexture(CTexture::TEXTURE_PART_CHANGE));
		CScene2D::SetTex(0, 1, 2);
		CScene2D::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		break;

	case UIFLAME_STRATEGY_PART:
		CScene2D::BindTexture(CTexture::GetTexture(CTexture::TEXTURE_PART_CHANGE));
		CScene2D::SetTex(1, 1, 2);
		CScene2D::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		break;

		//****************************************
		// �A�N�V�����p�[�g
		//****************************************
	case UIFLAME_RELOAD:
		CScene2D::BindTexture(CTexture::GetTexture(CTexture::TEXTURE_RELOAD));
		CScene2D::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		CScene2D::SetObjType(OBJTYPE_NONE);
		break;

	case UIFLAME_START:
		CScene2D::BindTexture(CTexture::GetTexture(CTexture::TEXTURE_START_DEATH));
		CScene2D::SetTex(0, 1, 2);
		CScene2D::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		break;

	case UIFLAME_DEATH:
		CScene2D::BindTexture(CTexture::GetTexture(CTexture::TEXTURE_START_DEATH));
		CScene2D::SetTex(1, 1, 2);
		CScene2D::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		break;

	case UIFLAME_TILE_PATTERN:
		CScene2D::BindTexture(CTexture::GetTexture(CTexture::TEXTURE_TILE_PATTERN));
		CScene2D::SetTex(0, 1, UI_TEX_ANIM_PATTERN);
		CScene2D::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		break;

	case UIFLAME_RESPAWN:
		CScene2D::BindTexture(CTexture::GetTexture(CTexture::TEXTURE_RESPAWN));
		CScene2D::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		break;

	case UIFLAME_MAP:
		CScene2D::BindTexture(CTexture::GetTexture(CTexture::TEXTURE_MAP));
		CScene2D::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		break;

	case UIFLAME_RESPAWN_FLAME:
		CScene2D::BindTexture(CTexture::GetTexture(CTexture::TEXTURE_RESPAWN_FLAME));
		CScene2D::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		break;

	case UIFLAME_TITLE:
		CScene2D::BindTexture(CTexture::GetTexture(CTexture::TEXTURE_TITLE));
		CScene2D::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		break;

	case UIFLAME_SELECT_RESPAWN:
		CScene2D::BindTexture(CTexture::GetTexture(CTexture::TEXTURE_SELECT_RESPAWN));
		CScene2D::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		break;

	case UIFLAME_RADIOCHAT:
		CScene2D::BindTexture(CTexture::GetTexture(CTexture::TEXTURE_RADIOCHAT));
		CScene2D::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		break;

	case UIFLAME_RADIOCHAT_MESS:
		CScene2D::BindTexture(CTexture::GetTexture(CTexture::TEXTURE_RADIOCHAT_MESS));
		m_colRadioMess = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		break;

	case UIFLAME_TEAM_WIN:
		CScene2D::BindTexture(CTexture::GetTexture(CTexture::TEXTURE_TEAM_WIN));
		CScene2D::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		break;

		//****************************************
		// �X�g���e�W�[�p�[�g
		//****************************************
	case UIFLAME_1P_INFO:
		CScene2D::BindTexture(CTexture::GetTexture(CTexture::TEXTURE_1P_INFO));
		CScene2D::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		break;

	case UIFLAME_2P_INFO:
		CScene2D::BindTexture(CTexture::GetTexture(CTexture::TEXTURE_2P_INFO));
		CScene2D::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		break;

	case UIFLAME_FLAME_WHITE:
		CScene2D::BindTexture(CTexture::GetTexture(CTexture::TEXTURE_FLAME));
		CScene2D::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		break;

	case UIFLAME_FLAME_BLUE:
		CScene2D::BindTexture(CTexture::GetTexture(CTexture::TEXTURE_FLAME));
		CScene2D::SetColor(D3DXCOLOR(0.0f, 0.4f, 1.0f, 1.0f));
		break;

	case UIFLAME_STRATEGY_BG:
		CScene2D::BindTexture(CTexture::GetTexture(CTexture::TEXTURE_STRATEGY_BG));
		CScene2D::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f));
		break;
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CUI_TEXTURE::Uninit(void)
{
	m_nCntAnim = 0;
	m_nPatternAnim = 0;

	// �I�u�W�F�N�g��j��
	CScene2D::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CUI_TEXTURE::Update(void)
{
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();	// �L�[�{�[�h�̓��͂��擾

	// �w�i�X�N���[���J�E���^�[
	m_nCntBgMove++;

	//****************************************
	// �e�^�C�v�̏���
	//****************************************
	switch (m_UITexType)
	{
	case UIFLAME_BG_00:
		CScene2D::SetBgMove(m_nCntBgMove, 0, UI_TEX_SCROLLSPEED);		// -m_nCntBgMove�Ŕ��Ό����Ɉړ�
		break;

	case UIFLAME_BG_01:
		CScene2D::SetBgMove(-m_nCntBgMove, 0, UI_TEX_SCROLLSPEED);		// -m_nCntBgMove�Ŕ��Ό����Ɉړ�
		break;

	case UIFLAME_CHANGE:
		CScene2D::Flashing(m_nFlash);		// �_��
		break;

	case UIFLAME_TILE_PATTERN:
		m_nCntAnim++;
		if (m_nCntAnim % UI_TEX_ANIM_SPEED == 0)
		{
			m_nPatternAnim++;
			CScene2D::SetTex(m_nPatternAnim, 1, UI_TEX_ANIM_PATTERN);
		}
		if (m_nPatternAnim > (UI_TEX_ANIM_PATTERN - 1))
		{
			CScene2D::SetTex((UI_TEX_ANIM_PATTERN - 1), 1, UI_TEX_ANIM_PATTERN);
		}
		break;
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CUI_TEXTURE::Draw(void)
{
	CScene2D::Draw();	// �`�揈��
}

//****************************************
// 3D�e�N�X�`���N���X
//****************************************
//=============================================================================
// �R���X�g���N�^
//=============================================================================
CUI_TEXTURE3D::CUI_TEXTURE3D(int nPriority, CScene::OBJTYPE objType) : CScene3D(nPriority, objType)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_UI3dTex = UI3DTEX_NONE;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CUI_TEXTURE3D::~CUI_TEXTURE3D()
{
}

//=============================================================================
// ����
//=============================================================================
CUI_TEXTURE3D *CUI_TEXTURE3D::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, UI3DTEX UI3DTex)
{
	CUI_TEXTURE3D *pUITex3D = NULL;

	pUITex3D = new CUI_TEXTURE3D;

	if (pUITex3D != NULL)
	{
		pUITex3D->m_UI3dTex = UI3DTex;
		pUITex3D->m_pos = pos;
		pUITex3D->m_size = size;
		pUITex3D->Init();
		pUITex3D->SetPos(pos);		// �ʒu�̐ݒ�
		pUITex3D->SetSize(size);		// �T�C�Y
	}

	return pUITex3D;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CUI_TEXTURE3D::Init(void)
{
	CScene3D::Init();		// ������

	//switch (m_UI3dTex)
	//{
	//case UI3DTEX_ACTION:
	//	CScene3D::BindTexture(CTexture::GetTexture(CTexture::TEXTURE_ACTION));
	//	CScene3D::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	//	break;

	//case UI3DTEX_STRATEGY:
	//	CScene3D::BindTexture(CTexture::GetTexture(CTexture::TEXTURE_STRATEGY));
	//	CScene3D::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	//	break;
	//}
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CUI_TEXTURE3D::Uninit(void)
{
	CScene3D::Uninit();	// �I��
}

//=============================================================================
// �X�V����
//=============================================================================
void CUI_TEXTURE3D::Update(void)
{
#ifdef _DEBUG
	CDebugProc::Print("pos : %.0f, %.0f, %.0f\n", m_pos.x, m_pos.y, m_pos.z);
	CDebugProc::Print("size : %.0f, %.0f, %.0f\n", m_size.x, m_size.y, m_size.z);
#endif
}

//=============================================================================
// �`�揈��
//=============================================================================
void CUI_TEXTURE3D::Draw(void)
{
	CScene3D::Draw();	// �`�揈��
}