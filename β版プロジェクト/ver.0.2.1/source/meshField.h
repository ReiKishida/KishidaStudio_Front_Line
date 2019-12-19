//=============================================================================
//
// ���b�V���t�B�[���h���� [meshField.h]
// Author : Takuto Ishida
//
//=============================================================================
#ifndef _MESHFIELD_H_
#define _MESHFIELD_H_

#include "scene3DIndex.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MESHFIELD_PRIORITY	(3)		// ���b�V���t�B�[���h�̗D��ԍ�

//*****************************************************************************
// �O���錾
//*****************************************************************************

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CMeshField : public CScene3DIndex
{
public:
	CMeshField(int nPriority = MESHFIELD_PRIORITY, CScene::OBJTYPE objType = CScene::OBJTYPE_FIELD);
	~CMeshField();
	HRESULT Init(int nField);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CMeshField* Create(D3DXVECTOR3 pos, int nField);

	float GetHeight(D3DXVECTOR3 pos);

	D3DXVECTOR3 GetNor(D3DXVECTOR3 pos);

	float GetAngle(D3DXVECTOR3 pos, float fAngle);
	float GetSlipAngle(D3DXVECTOR3 pos);
	bool OnField(D3DXVECTOR3 pos, float fMaxHeight);
	void SetRipples(D3DXVECTOR3 pos, float fRange, float fHeight);

private:
	void LoadVtx(void);			// ���_���̓ǂݍ���
	void LoadInfo(int nField);	// �n�ʂ̏����擾

	D3DXVECTOR3 m_pos;	// �ʒu
	int m_nType;		// �e�N�X�`���̎��
	int m_nWire;		// ���C���[�t���[����\�����邩
	float m_fRipples;	// �g��

	char m_aFileName[128];	// ���_���̃o�C�i���t�@�C����
};

#endif
