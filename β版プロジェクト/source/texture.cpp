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
	 // UI
		{ "data/TEXTURE/UI/number.png" },									// �i���o�[
		{ "data/TEXTURE/UI/bg001.png" },									// Bg00����ύX
		{ "data/TEXTURE/UI/bg002.png" },									// BG01����ύX
		{ "data/TEXTURE/UI/weapon_ui.png" },							// ����t���[��
		{ "data/TEXTURE/UI/frontLine.png" },								// �^�C�g��
		{ "data/TEXTURE/UI/press_any.png" },								// �v���X�{�^��
		{ "data/TEXTURE/UI/pleyer_frame.png" },						// �v���C���[�t���[��
		{ "data/TEXTURE/UI/change.png" },									// �p�[�g�؂�ւ�
		{ "data/TEXTURE/UI/upper_ui.png" },								// BLUE�ARED�A�^�C��UI
		{ "data/TEXTURE/UI/part_change.png" },							// �A�N�V�����A�X�g���e�W�[
		{ "data/TEXTURE/UI/ai_mecha_frame.png" },					// �h���[���A���[�J�[
		{ "data/TEXTURE/UI/cursor.png" },									// �}�E�X�J�[�\��
		{ "data/TEXTURE/UI/reticle.png" },									// ���e�B�N��
		{ "data/TEXTURE/UI/reload.png" },									// �����[�h���S�i�A�N�V�����p�[�g�Ŏg�p�j
		{ "data/TEXTURE/UI/team_win.png" },								// �����`�[���̃��S
		{ "data/TEXTURE/UI/linkenergy_blue.png" },					// BLUE�����N�Q�[�W
		{ "data/TEXTURE/UI/linkenergy_red.png" },						// RED�����N�Q�[�W
		{ "data/TEXTURE/UI/linkenergy_frame.png" },					// �����N�Q�[�W�t���[��
		{ "data/TEXTURE/UI/danger_upper.png" },						// �������������㉺
		{ "data/TEXTURE/UI/danger_side.png" },						// ���������������E

		// TEXTURE
		{ "data/TEXTURE/press_any.png" },									// �v���X�{�^��
		{ "data/TEXTURE/bullet.png" },											// �e

		// strategy
		{ "data/TEXTURE/strategy/player_1_frame.png" },			// 1P�̏������t���[��
		{ "data/TEXTURE/strategy/player_2_frame.png" },			// 2P�̏������t���[��
		{ "data/TEXTURE/strategy/frame_white.png" },				// �����̃t���[���i�X�g���e�W�[�p�[�g�Ŏg�p�j
		{ "data/TEXTURE/strategy/strategy_bg.png" },					// �X�g���e�W�[�p�[�g�̔w�i
		{ "data/TEXTURE/strategy/worker_button.png" },			// ���[�J�[
		{ "data/TEXTURE/strategy/drone_button.png" },				// �h���[��
		{ "data/TEXTURE/strategy/strategy_icon.png" },				// �X�g���e�W�[�̃A�C�R��

		// mechaSelect
		{ "data/TEXTURE/mechaSelect/meun_title.png" },			// ���C�����j���[�A���J�Z���N�g���S
		{ "data/TEXTURE/mechaSelect/aptitude_range.jpg" },		// �����\���i�@�̑I����ʂŎg�p�j
		{ "data/TEXTURE/mechaSelect/mecha_inst.png" },			// �@�̐����i�@�̑I����ʂŎg�p�j
		{ "data/TEXTURE/mechaSelect/mecha_type.png" },			// �@�̂̎�ށi�@�̑I����ʂŎg�p�j
		{ "data/TEXTURE/mechaSelect/mecha_select.png" },		// �@�̉摜�i�@�̑I����ʂŎg�p�j
		{ "data/TEXTURE/mechaSelect/back.png" },					// �O�̉�ʂ֖߂�
		{ "data/TEXTURE/mechaSelect/mecha_range.png" },		// �򋗗��ilong, short���S�j

		// menu
		{ "data/TEXTURE/menu/menu_frame.png" },					// ���[�h�t���[���i���j���[��ʂŎg�p�j
		{ "data/TEXTURE/menu/menu_inst.png" },						// ���[�h�̊T�v�i���j���[�I����ʂŎg�p�j
		{ "data/TEXTURE/menu/infomation.png" },						// �C���t�H���[�V����

		// matching
		{ "data/TEXTURE/matching/matching_bg.jpg" },				// �}�b�`���O��ʂ̔w�i�i�}�b�`���O��ʂŎg�p�j
		{ "data/TEXTURE/matching/matching_bg_000.png" },		// �}�b�`���OBG_00�i�}�b�`���O��ʂŎg�p�j
		{ "data/TEXTURE/matching/matching_bg_001.png" },		// �}�b�`���OBG_01�i�}�b�`���O��ʂŎg�p�j
		{ "data/TEXTURE/matching/VS.png" },								// VS���S�i�}�b�`���O��ʂŎg�p�j
		{ "data/TEXTURE/matching/player_number.png" },			// �v���C���[�i���o�[�i�}�b�`���O��ʂŎg�p�j
		{ "data/TEXTURE/matching/blue_mecha_list.png" },		// BLUE�`�[�����J���X�g�i�}�b�`���O��ʂŎg�p�j
		{ "data/TEXTURE/matching/red_mecha_list.png" },			// RED�`�[�����J���X�g�i�}�b�`���O��ʂŎg�p�j

		// respawn
		{ "data/TEXTURE/respawn/start_death.png" },					// �퓬�J�n�A�s���s�\���S�i�A�N�V�����p�[�g�Ŏg�p�j
		{ "data/TEXTURE/respawn/tile_pattern.png" },					// �^�C���A�j���[�V�����i�A�N�V�����p�[�g�Ŏg�p�j
		{ "data/TEXTURE/respawn/respawn_sequence.png" },		// ���A���S�i�A�N�V�����p�[�g�Ŏg�p�j
		{ "data/TEXTURE/respawn/respawn_map.png" },				// ���X�|�[���}�b�v�i�A�N�V�����p�[�g�Ŏg�p�j
		{ "data/TEXTURE/respawn/respawn_frame.jpg" },			// ���X�|�[���t���[���i�A�N�V�����p�[�g�Ŏg�p�j
		{ "data/TEXTURE/respawn/respawn_title.jpg" },				// ���X�|�[���^�C�g���i�A�N�V�����p�[�g�Ŏg�p�j
		{ "data/TEXTURE/respawn/select_respawn.png" },			// ���X�|�[���I���i�A�N�V�����p�[�g�Ŏg�p�j

		// radiochat
		{ "data/TEXTURE/radiochat/radio_chat.png" },						// ���W�I�`���b�g�i�A�N�V�����p�[�g�Ŏg�p�j
		{ "data/TEXTURE/radiochat/radio_chat_message.png" },		// ���W�I�`���b�g���b�Z�[�W

		// option
		{ "data/TEXTURE/option/option_icon.png" },						// �I�v�V�����A�C�R��
		{ "data/TEXTURE/option/option_frame.png" },						// �I�v�V�����t���[��
		{ "data/TEXTURE/option/batsu.png" },									// �o�c�{�^��
		{ "data/TEXTURE/option/toggle.png" },									// �g�O���X�C�b�`

		// tutorialmenu
		{ "data/TEXTURE/tutorialmenu/tutorial_frame.png" },			// �`���[�g���A�����j���[
		{ "data/TEXTURE/tutorialmenu/tutorialmenu_inst.png" },		// �`���[�g���A�����j���[�̐�����

		//tutorial
		{ "data/TEXTURE/tutorial/tutorial_logo.png" },						//�`���[�g���A���̌��݂̍���
		{ "data/TEXTURE/tutorial/tutorial_step.png" },						//�`���[�g���A���̍��ڐ�
		{ "data/TEXTURE/tutorial/tutorial_mission_basic.png" },				//�`���[�g���A���̉ۑ�(��{����)
		{ "data/TEXTURE/tutorial/tutorial_mission_Action.png" },			//�`���[�g���A���̉ۑ�(�A�N�V�����p�[�g)
		{ "data/TEXTURE/tutorial/tutorial_mission_Strategy.png" },			//�`���[�g���A���̉ۑ�(�X�g���e�W�[�p�[�g)
		{ "data/TEXTURE/tutorial/basic_move.png" },							//����̕��@(��{����)
		{ "data/TEXTURE/tutorial/action_part.png" },						//����̕��@(�A�N�V�����p�[�g)
		{ "data/TEXTURE/tutorial/strategy_part.png" },						//����̕��@(�X�g���e�W�[�p�[�g)
		{ "data/TEXTURE/tutorial/Success.png" },							//�`���[�g���A�������I

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
