//=============================================================================
//
// UI_Number���� [UI_Number.h]
// Author : Ayaka Hakozaki
//
//=============================================================================
#ifndef _UI_NUMBER_H_
#define _UI_NUMBER_H_

#include "scene2D.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define UI_NUMBER_PRIORITY	(6)		// �����̗D�揇��

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CNumber;
class CGauge2D;
class CUI_TEXTURE;

//*****************************************************************************
// ����UI�N���X
//*****************************************************************************
class CUI_NUMBER : public  CScene2D
{
public:
	typedef enum
	{
		UI_NUMTYPE_NONE,								// �����ݒ肵�Ȃ�
		UI_NUMTYPE_REMAINBULLET,			// �c�e
		UI_NUMTYPE_PLAYER_HP,					// �v���C���[�̗̑�
		UI_NUMTYPE_ALLY_HP,						// �����̗̑�
		UI_NUMTYPE_DRONE_HP,					// �h���[���̗̑�
		UI_NUMTYPE_WORKER_HP,					// ���[�J�[�̗̑�
		UI_NUMTYPE_ALLY_DRONE_HP,			// �����h���[���̗̑�
		UI_NUMTYPE_ALLY_WORKER_HP,		// �������[�J�[�̗̑�
		UI_NUMTYPE_BLUE,								// BLUE�`�[���̃`�P�b�g��
		UI_NUMTYPE_RED,								// RED�`�[���̃`�P�b�g��
		UI_NUMTYPE_CNTRESPAWN,				// ������A�J�E���g
		UINUMTYPE_MAX									// �^�C�v�̍ő吔
	} UI_NUMTYPE;

	CUI_NUMBER(int nPriority = UI_NUMBER_PRIORITY, CScene::OBJTYPE objType = CScene::OBJTYPE_UI_NUMBER);
	~CUI_NUMBER();
	static CUI_NUMBER *Create(D3DXVECTOR3 pos, float fWidth, float fHeight, float fInt, UI_NUMTYPE UINumType, int nUV, int nUV_X, int nUV_Y);		// ����
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//****************************************
	// �e�ݒ�
	//****************************************
	void SetNumColor(int nNum, int nDefNum);										// �l�ɂ���ĐF�ς�鏈��
	void SetNum(int nCalcNum);																// �������X�V���鏈��

	UI_NUMTYPE GetNumType(void) { return m_UINumType; }				// UI�^�C�v�̎擾
	int GetRespawn(void) { return m_nCntRespawn; }								// ������A�J�E���^�[�̎擾
	void SetTime(int nTime) { m_nTimer = nTime; }								// �^�C�}�[�̐ݒ�
	int GetDigits(void) { return m_nDigits; }												// ���݂̐����̌����擾
	void SetDigits(int nDigits) { m_nDigits = nDigits; }							// ���̐ݒ�

																					// public�ϐ�
	UI_NUMTYPE		m_UINumType;		// ����UI�̃^�C�v�Ǘ�

private:
	int						m_nRemBullet;							// �擾���Ă����c�e�̊Ǘ�
	int						m_nPlayerLife;							// �v���C���[�̃��C�t
	int						m_nCntRespawn;						// ������A�J�E���^�[
	int						m_nDroneLife, m_nWorkerLife;	// AI�̃��C�t
	D3DXVECTOR3	m_pos;											// �ʒu
	D3DXCOLOR		m_col;											// �J���[
	float						m_fWidth, m_fHeight;					// ���A����
	float						 m_fInt;										// �����̊Ԋu
	CNumber				**m_apNumber;							// �i���o�[�ϐ��ւ̃|�C���^
	int						m_nDigits;									// ����
	int						m_nDiff;										// ����
	int						m_nUV, m_nUV_X, m_nUV_Y;		// �e�N�X�`��UV
	int						m_nTimer;									// �J�E���^�[
	CGauge2D			*m_pGaugeBlue;							// BLUE�`�[���Q�[�W
	CGauge2D			*m_pGaugeRed;							// RED�`�[���Q�[�W
	int						m_nDamege, m_nInitGauge;		// �_���[�W�ʕۑ��A�Q�[�W�l�ۑ�
	CUI_TEXTURE		*m_pUITex;									// �Q�[�W�t���[��
	int						m_nCntMove;								// �X�N���[���J�E���^�[
};

#endif