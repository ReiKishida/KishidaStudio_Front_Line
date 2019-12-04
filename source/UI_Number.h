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

//*****************************************************************************
// ����UI�N���X
//*****************************************************************************
class CUI_NUMBER : public  CScene2D
{
public:
	typedef enum
	{
		UI_NUMTYPE_NONE,						// �����ݒ肵�Ȃ�
		UI_NUMTYPE_REMAINBULLET,	// �c�e
		UI_NUMTYPE_PLAYER_HP,			// �v���C���[�̗̑�
		UI_NUMTYPE_AI00_HP,					// �h���[���̗̑�
		UI_NUMTYPE_AI01_HP,					// ���[�J�[�̗̑�
		UI_NUMTYPE_BLUE,						// BLUE�`�[���̃`�P�b�g��
		UI_NUMTYPE_RED,						// RED�`�[���̃`�P�b�g��
		UI_NUMTYPE_CNTRESPAWN,		// ������A�J�E���g
		UINUMTYPE_MAX							// �^�C�v�̍ő吔
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
	void SetNumDisPlay(int nGetValue, UI_NUMTYPE UINumType);		// ������\�����邾���̏���
	void SetNum(int nCalcNum, int nDefNum, D3DXCOLOR col);			// �������X�V���鏈��
	void SetTime(int nTime) { m_nTimer = nTime; }								// �^�C�}�[�̐ݒ�
	int GetTime(void) { return m_nTimer; }												// �^�C�}�[�̎擾

																						//****************************************
																						// �e�^�C�v�̎擾�Ɛݒ�
																						//****************************************
																						// �^�C�v
	void SetNumType(UI_NUMTYPE UINumType) { m_UINumType = UINumType; };
	UI_NUMTYPE GetNumType(void) { return m_UINumType; }

	// ��
	int GetDigits(void) { return m_nDigits; }
	void SetDigits(int nDigits) { m_nDigits = nDigits; }

	// �c�e
	int GetRemainBullet(void) { return m_nRemBullet; };
	void SetRemainBullet(int nRemBullet) { m_nRemBullet = nRemBullet; }

	// �v���C���[�̗�
	int GetPlayerLife(void) { return m_nPlayerLife; }
	void SetPlayerLife(int nPlayerLife) { m_nPlayerLife = nPlayerLife; }

	// BLUE�`�P�b�g
	int GetTeamBlue(void) { return m_nTeamBlue; }
	void SetTeamBlue(int nTeamBlue) { m_nTeamBlue = nTeamBlue; }

	// RED�`�P�b�g
	int GetTeamRed(void) { return m_nTeamRed; }
	void SetTeamRed(int nTeamRed) { m_nTeamRed = nTeamRed; }

	// ������A�J�E���^�[
	int GetRespawn(void) { return m_nCntRespawn; }
	void SetRespawn(int nCntRespawn) { m_nCntRespawn = nCntRespawn; }

	//****************************************
	// public�ϐ�
	//****************************************
	UI_NUMTYPE		m_UINumType;		// ����UI�̃^�C�v�Ǘ�

private:
	static int				m_nRemBullet;							// �擾���Ă����c�e�̊Ǘ�
	static int				m_nPlayerLife;							// �v���C���[�̃��C�t
	static int				m_nTeamBlue, m_nTeamRed;		// BLUE�`�[���ARED�`�[��
	static int				m_nCntRespawn;						// ������A�J�E���^�[
	D3DXVECTOR3	m_pos;											// �ʒu
	D3DXCOLOR		m_col;											// �J���[
	float						m_fWidth, m_fHeight;					// ���A����
	float						 m_fInt;										// �����̊Ԋu
	CNumber				**m_apNumber;							// �i���o�[�ϐ��ւ̃|�C���^
	int						m_nDigits;									// ����
	int						m_nDiff;										// ����
	int						m_nUV, m_nUV_X, m_nUV_Y;		// �e�N�X�`��UV
	int						m_nTimer;									// �J�E���^�[
	int						m_nDecrease;
	CGauge2D			*m_pGaugeBlue;							// BLUE�`�[���Q�[�W
	CGauge2D			*m_pGaugeRed;							// RED�`�[���Q�[�W
	int						m_nDamege, m_nInitGauge;		// �_���[�W�ʕۑ��A�Q�[�W�l�ۑ�
	int						m_nNum;										// �Q�[�W�̒l�v�Z�p�A�_���[�W��
};

#endif