//=============================================================================
//
// �e�N�X�`���ǂݍ��ݏ��� [texture.h]
// Author : Takuto Ishida
//
//=============================================================================
#include "texture.h"
#include "renderer.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 CTexture::m_pTexture[CTexture::TEXTURE_MAX] = {};

//=========================================
// �R���X�g���N�^
//=========================================
CTexture::CTexture()
{
}

//=========================================
// �f�X�g���N�^
//=========================================
CTexture::~CTexture()
{
}

//=========================================
// �e�N�X�`���̓ǂݍ���
//=========================================
void CTexture::Load(void)
{
	// �f�o�C�X�̎擾
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	const char *pTextureName[TEXTURE_MAX] =
	{// �e�N�X�`����
		{ "data/TEXTURE/UI/number.png" },									// �i���o�[
		{ "data/TEXTURE/press_any.png" },									// �v���X�{�^��
		{ "data/TEXTURE/UI/bg001.png" },									// Bg00����ύX
		{ "data/TEXTURE/UI/bg002.png" },									// BG01����ύX
		{ "data/TEXTURE/UI/weapon_ui.png" },							// ����t���[��
		{ "data/TEXTURE/UI/frontLine.png" },								// �^�C�g��
		{ "data/TEXTURE/UI/press_any.png" },								// �v���X�{�^��
		{ "data/TEXTURE/UI/pleyer_frame.png" },						// �v���C���[�t���[��
		{ "data/TEXTURE/UI/change.png" },									// �p�[�g�؂�ւ�
		{ "data/TEXTURE/strategy/player_1_frame.png" },			// 1P�̏������t���[��
		{ "data/TEXTURE/strategy/player_2_frame.png" },			// 2P�̏������t���[��
		{ "data/TEXTURE/strategy/frame_white.png" },				// �����̃t���[���i�X�g���e�W�[�p�[�g�Ŏg�p�j
		{ "data/TEXTURE/strategy/strategy_bg.png" },					// �X�g���e�W�[�p�[�g�̔w�i
		{ "data/TEXTURE/UI/upper_ui.png" },								// BLUE�ARED�A�^�C��UI
		{ "data/TEXTURE/UI/part_change.png" },							// �A�N�V�����A�X�g���e�W�[
		{ "data/TEXTURE/UI/ai_mecha_frame.png" },					// �h���[���A���[�J�[
		{ "data/TEXTURE/UI/cursor.png" },									// �}�E�X�J�[�\��
		{ "data/TEXTURE/strategy/worker_button.png" },			// ���[�J�[
		{ "data/TEXTURE/strategy/drone_button.png" },				// �h���[��
		{ "data/TEXTURE/UI/reticle.png" },									// ���e�B�N��
		{ "data/TEXTURE/mechaSelect/main_frame.png" },			// ���C�����j���[�A���J�Z���N�g���S
		{ "data/TEXTURE/mechaSelect/aptitude_range.jpg" },		// �����\���i�@�̑I����ʂŎg�p�j
		{ "data/TEXTURE/mechaSelect/mecha_inst.png" },			// �@�̐����i�@�̑I����ʂŎg�p�j
		{ "data/TEXTURE/mechaSelect/mecha_type.png" },			// �@�̂̎�ށi�@�̑I����ʂŎg�p�j
		{ "data/TEXTURE/menu/menu_disp.png" },						// ���[�h�������S�i���j���[��ʂŎg�p�j
		{ "data/TEXTURE/menu/menu_frame.png" },					// ���[�h�t���[���i���j���[��ʂŎg�p�j
		{ "data/TEXTURE/menu/menu_inst.png" },						// ���[�h�̊T�v�i���j���[�I����ʂŎg�p�j
		{ "data/TEXTURE/matching/matching_bg.jpg" },				// �}�b�`���O��ʂ̔w�i�i�}�b�`���O��ʂŎg�p�j
		{ "data/TEXTURE/matching/matching_bg_000.png" },		// �}�b�`���OBG_00�i�}�b�`���O��ʂŎg�p�j
		{ "data/TEXTURE/matching/matching_bg_001.png" },		// �}�b�`���OBG_01�i�}�b�`���O��ʂŎg�p�j
		{ "data/TEXTURE/matching/VS.png" },								// VS���S�i�}�b�`���O��ʂŎg�p�j
		{ "data/TEXTURE/matching/player_number.png" },			// �v���C���[�i���o�[�i�}�b�`���O��ʂŎg�p�j
		{ "data/TEXTURE/matching/blue_mecha_list.png" },		// BLUE�`�[�����J���X�g�i�}�b�`���O��ʂŎg�p�j
		{ "data/TEXTURE/matching/red_mecha_list.png" },			// RED�`�[�����J���X�g�i�}�b�`���O��ʂŎg�p�j
		{ "data/TEXTURE/respawn/start_death.png" },					// �퓬�J�n�A�s���s�\���S�i�A�N�V�����p�[�g�Ŏg�p�j
		{ "data/TEXTURE/respawn/tile_pattern.png" },					// �^�C���A�j���[�V�����i�A�N�V�����p�[�g�Ŏg�p�j
		{ "data/TEXTURE/respawn/respawn_sequence.png" },	// ���A���S�i�A�N�V�����p�[�g�Ŏg�p�j
		{ "data/TEXTURE/respawn/map_b.jpg" },							// ���X�|�[���}�b�v�i�A�N�V�����p�[�g�Ŏg�p�j
		{ "data/TEXTURE/respawn/respawn_frame.jpg" },			// ���X�|�[���t���[���i�A�N�V�����p�[�g�Ŏg�p�j
		{ "data/TEXTURE/respawn/respawn_title.jpg" },				// ���X�|�[���^�C�g���i�A�N�V�����p�[�g�Ŏg�p�j
		{ "data/TEXTURE/respawn/select_respawn.png" },			// ���X�|�[���I���i�A�N�V�����p�[�g�Ŏg�p�j
		{ "data/TEXTURE/UI/reload.png" },									// �����[�h���S�i�A�N�V�����p�[�g�Ŏg�p�j
		{ "data/TEXTURE/mechaSelect/mecha_select.png" },		// �@�̉摜�i�@�̑I����ʂŎg�p�j
		{ "data/TEXTURE/bullet.png" },						// �e
	};

	for (int nCntTexture = 0; nCntTexture < TEXTURE_MAX; nCntTexture++)
	{// �e�N�X�`���̐���
		D3DXCreateTextureFromFile(pDevice, pTextureName[nCntTexture], &m_pTexture[nCntTexture]);
	}
}

//=========================================
// �ǂݍ��񂾃e�N�X�`���̉��
//=========================================
void CTexture::Unload(void)
{
	for (int nCntTexture = 0; nCntTexture < TEXTURE_MAX; nCntTexture++)
	{// �e�N�X�`���������[�v
		if (NULL != m_pTexture[nCntTexture])
		{// �e�N�X�`���̔j��
			m_pTexture[nCntTexture]->Release();
			m_pTexture[nCntTexture] = NULL;
		}
	}
}
