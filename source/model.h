//=============================================================================
//
// �e�q�֌W���� [model.h]
// Author : Takuto Ishida
//
//=============================================================================
#ifndef _MODEL_H_
#define _MODEL_H_

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
class CModel
{
public:
	CModel();
	~CModel();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CModel *Create(D3DXMATRIX *pParent = NULL);

	D3DXVECTOR3 GetPos(void) { return m_pos; };					// �ʒu�̎擾
	const void SetPos(D3DXVECTOR3 pos) { m_pos = pos; };		// �ʒu�̐ݒ�

	D3DXVECTOR3 GetRot(void) { return m_rot; };					// �p�x�̎擾
	const void SetRot(D3DXVECTOR3 rot) { m_rot = rot; };		// �p�x�̐ݒ�

	D3DXMATRIX &GetMtxWorld(void) { return m_mtxWorld; };					// ���[���h�}�g���b�N�X�̎擾
	const void SetMtxWorld(D3DXMATRIX mtxWorld) { m_mtxWorld = mtxWorld; };	// ���[���h�}�g���b�N�X�̐ݒ�

	int GetModel(void) { return m_nIdxModel; };						// �ʒu�̎擾
	void SetModel(char *pModelName);							// ���f���̐ݒ�

	CModel *GetParent(void) { return m_pParent; };					// �e�̎擾
	void SetParent(CModel *parent) { m_pParent = parent; };	// �e�̐ݒ�

	D3DXVECTOR3 GetVtxMax(void) { return m_vtxMax; };				// ���_���W�̍ő�
	D3DXVECTOR3 GetVtxMin(void) { return m_vtxMin; };				// ���_���W�̍ŏ�

	LPD3DXMESH GetMesh(void) { return m_pMesh; };					// ���b�V�����̎擾

	void SetColor(D3DXCOLOR col);									// �F�̐ݒ�
	void AddColor(D3DXCOLOR col);

	bool Collision(D3DXVECTOR3 pos, float fRange);					// �����蔻��

protected:

private:
	LPDIRECT3DTEXTURE9	*m_pTexture;	// �e�N�X�`���ւ̃|�C���^
	LPD3DXMESH			m_pMesh;		// ���b�V�����ւ̃|�C���^
	LPD3DXBUFFER		m_pBuffMat;		// �}�e���A�����ւ̃|�C���^
	DWORD				m_nNumMat;		// �}�e���A�����̐�

	D3DXVECTOR3			m_vtxMin, m_vtxMax;	// ���f���̍ŏ��l�A�ő�l

	D3DXMATRIX			m_mtxWorld;			// ���[���h�}�g���b�N�X
	D3DXMATRIX			*m_pMtxParent;			// �e�̃��[���h�}�g���b�N�X
	CModel				*m_pParent;			// �e�̃��f��
	D3DXVECTOR3			m_pos;				// �ʒu
	D3DXVECTOR3			m_rot;				// ��]
	int					m_nIdxModel;		// �g�����f��
};
#endif