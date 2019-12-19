//=============================================================================
//
//	�퓬�n�̏��� [search.h]
// Author : Takuto Ishida
//
//=============================================================================
#ifndef _SEARCH_H_
#define _SEARCH_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �O���錾
//*****************************************************************************

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CSearch
{
public:
	CSearch();
	~CSearch();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CSearch *Create(D3DXMATRIX *pParent = NULL);

	D3DXVECTOR3 GetPos(void) { return m_pos; };					// �ʒu�̎擾
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; };		// �ʒu�̐ݒ�

	D3DXVECTOR3 GetRot(void) { return m_rot; };					// �p�x�̎擾
	 void SetRot(D3DXVECTOR3 rot);

	D3DXMATRIX &GetMtxWorld(void) { return m_mtxWorld; };					// ���[���h�}�g���b�N�X�̎擾
	void SetMtxWorld(D3DXMATRIX mtxWorld) { m_mtxWorld = mtxWorld; };	// ���[���h�}�g���b�N�X�̐ݒ�


	CSearch *GetParent(void) { return m_pParent; };					// �e�̎擾
	void SetParent(CSearch *parent) { m_pParent = parent; };	// �e�̐ݒ�

	bool Distance(void);	//�T��
	static D3DXVECTOR3 Sub_Vector(const D3DXVECTOR3& Pos0, const D3DXVECTOR3& Pos1);		//�x�N�g���̌v�Z
	static D3DXVECTOR3 Cross_Product(const D3DXVECTOR3& VecA, const D3DXVECTOR3 & VecB);	//�O�όv�Z

protected:

private:
	D3DXMATRIX			m_mtxWorld;			// ���[���h�}�g���b�N�X
	D3DXMATRIX			*m_pMtxParent;			// �e�̃��[���h�}�g���b�N�X
	CSearch				*m_pParent;			// �e�̃��f��
	D3DXVECTOR3			m_pos;				// �ʒu
	D3DXVECTOR3			m_rot;				// ��]


	D3DXVECTOR3 m_SearchVec_0;				//�F���p�x�N�g���P
	D3DXVECTOR3 m_SearchVec_1;				//�F���p�x�N�g���Q
	D3DXVECTOR3 m_SearchVec_2;				//�F���p�x�N�g���Q
	D3DXVECTOR3 m_SearchVec_P;				//�F���p�x�N�g���v���C���[

};
#endif