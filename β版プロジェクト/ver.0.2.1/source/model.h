//=============================================================================
//
// �e�q�֌W���� [model.h]
// Author : Takuto Ishida
//
//=============================================================================
#ifndef _MODEL_H_
#define _MODEL_H_

#include "scene.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CShadow;

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

	static CModel *Create(CScene *pObject = NULL, D3DXMATRIX *pParent = NULL);

	D3DXVECTOR3 GetPos(void) { return m_pos; };					// �ʒu�̎擾
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; };				// �ʒu�̐ݒ�

	D3DXVECTOR3 GetRot(void) { return m_rot; };					// �p�x�̎擾
	const void SetRot(D3DXVECTOR3 rot) { m_rot = rot; };		// �p�x�̐ݒ�

	D3DXVECTOR3 GetWorldPos(void) { return D3DXVECTOR3(m_mtxWorld._41, m_mtxWorld._42, m_mtxWorld._43); };			// ���[���h���W�̎擾

	D3DXMATRIX &GetMtxWorld(void) { return m_mtxWorld; };					// ���[���h�}�g���b�N�X�̎擾
	void SetMtxWorld(D3DXMATRIX mtxWorld) { m_mtxWorld = mtxWorld; };	// ���[���h�}�g���b�N�X�̐ݒ�

	int GetModel(void) { return m_nIdxModel; };					// �ʒu�̎擾
	void SetModel(char *pModelName, bool bShadow = true);	// ���f���̐ݒ�

	CModel *GetParent(void) { return m_pParent; };					// �e�̎擾
	void SetParent(CModel *parent) { m_pParent = parent; };	// �e�̐ݒ�

	D3DXVECTOR3 GetVtxMax(void) { return m_vtxMax; };				// ���_���W�̍ő�
	D3DXVECTOR3 GetVtxMin(void) { return m_vtxMin; };				// ���_���W�̍ŏ�

	D3DXVECTOR3 GetVtxDiff(void) { return m_vtxMax - m_vtxMin; };	// ���_���W�̍ŏ��ƍő�̍�

	LPD3DXMESH GetMesh(void) { return m_pMesh; };					// ���b�V�����̎擾

	void SetColor(D3DXCOLOR col);									// �F�̐ݒ�
	void AddColor(D3DXCOLOR col);

	bool Collision(D3DXVECTOR3 pos, float fRange);					// �����蔻��

	void MatrixCalculation(void);									// �}�g���b�N�X�̌v�Z

	bool GetDisp(void) { return m_bDisp; };
	void SetDisp(bool bDisp) { m_bDisp = bDisp; };

	float GetShadowHeight(void) { return m_fShadowHeight; };
	void SetShadowHeight(float fShadowHeight) { m_fShadowHeight = fShadowHeight; };

	void SetShader(void);

protected:

private:
	LPDIRECT3DTEXTURE9	*m_pTexture;	// �e�N�X�`���ւ̃|�C���^
	LPD3DXMESH			m_pMesh;		// ���b�V�����ւ̃|�C���^
	LPD3DXBUFFER		m_pBuffMat;		// �}�e���A�����ւ̃|�C���^
	DWORD				m_nNumMat;		// �}�e���A�����̐�

	D3DXVECTOR3			m_vtxMin, m_vtxMax;	// ���f���̍ŏ��l�A�ő�l

	D3DXMATRIX			m_mtxWorld;			// ���[���h�}�g���b�N�X
	D3DXMATRIX			*m_pMtxParent;		// �e�̃��[���h�}�g���b�N�X
	CModel				*m_pParent;			// �e�̃��f��
	D3DXVECTOR3			m_pos;				// �ʒu
	D3DXVECTOR3			m_rot;				// ��]
	int					m_nIdxModel;		// �g�����f��
	bool				m_bDisp;			// �`�悷�邩�ǂ���
	CShadow				*m_pShadow;			// �e�̃|�C���^�ϐ�
	float				m_fShadowHeight;	// �e�̍���

	LPD3DXEFFECT		m_pShader;			// �V�F�[�_
	LPDIRECT3DTEXTURE9	m_rampTexture;		// �����v�e�N�X�`��

	CScene				*m_pObject;			// �������̃I�u�W�F�N�g�̃|�C���^
};
#endif