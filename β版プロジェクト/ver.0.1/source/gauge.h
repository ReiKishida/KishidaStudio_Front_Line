//=============================================================================
//
// �Q�[�W���� [gauge.h]
// Author : Ishida Takuto
//
//=============================================================================
#ifndef _GAUGE_H_
#define _GAUGE_H_

#include "scene.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define GAUGE_PRIORITY		(5)		// �����̗D��ԍ�

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CLogo;
class CScene3DBill;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CGauge2D : public CScene
{
public:
	CGauge2D(int nPriority = GAUGE_PRIORITY, CScene::OBJTYPE objType = CScene::OBJTYPE_GAUGE);
	~CGauge2D();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CGauge2D* Create(int nNumBar, D3DXVECTOR3 pos, float fInit, float fMax, float fWidth, float fHeight, int nStartReduce = 60);

	void BindTexture(LPDIRECT3DTEXTURE9 pTexture, int nGauge);	// �e�N�X�`���̐ݒ�

	void AddSubtract(float fValue);		// �l�𑝌�������
	float GetValue(void) { return m_fGauge; };		// �l�̎擾
	void SetValue(float fGauge);					// �l�̐ݒ�

	void SetColor(D3DXCOLOR col, int nBar);
	D3DXCOLOR GetColor(int nBar);

	void SetCurGauge(int nCurGauge) { m_nCurGauge = nCurGauge; };

	float GetMaxValue(void) { return m_fMax; }
	void SetMaxValue(float fMaxValue) { m_fMax = fMaxValue; }

	int GetNumBar(void) { return m_nNumBar; }

private:
	CLogo **m_pGauge;		// 2D�|���S���N���X�̃|�C���^�ϐ�
	float m_fGauge;			// ���݂̃Q�[�W�̒l
	int m_nCntReduce;		// �o�[�����炵�n�߂�܂ł̃J�E���^
	D3DXVECTOR3 m_pos;		// �ʒu
	int m_nNumBar;			// �o�[�̐�
	float m_fMax;			// �ő�l
	int m_nStartReduce;		// ���炵�n�߂�܂ł̎���
	int m_nCurGauge;		// ���݂̒l�������o�[�̔ԍ�
};

/*========================*/
/*		3D�Q�[�W�N���X	  */
/*========================*/
class CGauge3D : public CScene
{
public:
	CGauge3D(int nPriority = GAUGE_PRIORITY, CScene::OBJTYPE objType = CScene::OBJTYPE_GAUGE);
	~CGauge3D();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CGauge3D* Create(int nNumBar, D3DXVECTOR3 pos, float fInit, float fMax, float fWidth, float fHeight, D3DXVECTOR3 *posParent, int nStartReduce = 90);

	void BindTexture(LPDIRECT3DTEXTURE9 pTexture, int nGauge);	// �e�N�X�`���̐ݒ�

	const void AddSubtract(float fValue);				// �l�𑝌�������
	float Get(void) { return m_fGauge; };				// �l�̎擾
	void Set(float fGauge) { m_fGauge = fGauge; };		// �l�̐ݒ�

	const void SetColor(D3DXCOLOR col, int nBar);
	D3DXCOLOR GetColor(int nBar);

private:
	CScene3DBill **m_pGauge;	// 3D�|���S���N���X�̃|�C���^�ϐ�
	float m_fGauge;				// ���݂̃Q�[�W�̒l
	int m_nCntReduce;			// �o�[�����炵�n�߂�܂ł̃J�E���^
	D3DXVECTOR3 m_offset;		// �I�t�Z�b�g
	D3DXCOLOR *m_pCol;			// �F
	int m_nNumBar;				// �o�[�̐�
	float m_fMax;				// �ő�l
	int m_nStartReduce;			// ���炵�n�߂�܂ł̎���
	D3DXVECTOR3 *m_pPosParent;	// �e�̈ʒu
};

#endif
