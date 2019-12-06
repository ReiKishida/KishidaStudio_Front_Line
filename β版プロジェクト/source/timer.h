//=============================================================================
//
// �^�C�}�[���� [timer.h]
// Author : Takuto Ishida
//
//=============================================================================
#ifndef _TIMER_H_
#define _TIMER_H_

#include "scene.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TIMER_PRIORITY	(6)		// �����̗D�揇��

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CNumber;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CTimer : public CScene
{
public:
	CTimer(int nPriority = TIMER_PRIORITY, CScene::OBJTYPE objType = CScene::OBJTYPE_TIME);
	~CTimer();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CTimer* Create(D3DXVECTOR3 pos, int nTime);

	static int GetTime(void) { return m_nTimer; };
	static void SetTime(int nTime) { m_nTimer = nTime; }

private:
	D3DXVECTOR3		m_pos;				// �ʒu
	CNumber			**m_pNumber;		// �i���o�[�ϐ��ւ̃|�C���^
	static int		m_nTimer;			// ���݂̎���
	int				m_nCntSecond;		// �b�̌v�Z�ɕK�v
	int				m_nDigits;			// ����
};

#endif
