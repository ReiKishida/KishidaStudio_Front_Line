//=============================================================================
//
// 3D���f���I�u�W�F�N�g���� [sceneX.h]
// Author : Ishida Takuto
//
//=============================================================================
#ifndef _SCENEX_H_
#define _SCENEX_H_

#include "scene.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �O���錾
//*****************************************************************************

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CSceneX : public CScene
{
public:
	CSceneX(int nPriority = 4, CScene::OBJTYPE objType = CScene::OBJTYPE_NONE);
	~CSceneX();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	const void BindInfo(LPDIRECT3DTEXTURE9 pTexture, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat);	// ���f�����̎󂯓n��

	D3DXVECTOR3 GetPos(void) { return m_pos; };
	const void SetPos(D3DXVECTOR3 pos) { m_pos = pos; };

	const void SetColor(D3DXCOLOR col);
	const void AddColor(D3DXCOLOR col);

	D3DXVECTOR3 GetMaxVtx(void) { return m_vtxMax; };
	D3DXVECTOR3 GetMinVtx(void) { return m_vtxMin; };

	D3DXVECTOR3 GetRot(void) { return m_rot; };
	const void SetRot(D3DXVECTOR3 rot) { m_rot = rot; };

	int CollisionBox(D3DXVECTOR3 *pos, D3DXVECTOR3 posOld, D3DXVECTOR3 rot);

	D3DXMATRIX GetMtxWorld(void) { return m_mtxWorld; };
	virtual D3DXMATRIX SetWorldMatrix(D3DXMATRIX *m_mtxWorld);

	LPD3DXMESH GetMesh(void) { return m_pMesh; };

protected:

private:
	LPDIRECT3DTEXTURE9			m_pTexture;			// �e�N�X�`���ւ̃|�C���^
	LPD3DXMESH					m_pMesh;			// ���b�V�����ւ̃|�C���^
	LPD3DXBUFFER				m_pBuffMat;			// �}�e���A�����ւ̃|�C���^
	DWORD						m_nNumMat;			// �}�e���A�����̐�

	D3DXMATERIAL				*m_pMat;			// �}�e���A���f�[�^�ւ̃|�C���^

	D3DXMATRIX					m_mtxWorld;			// ���[���h�}�g���b�N�X
	D3DXVECTOR3					m_vtxMin, m_vtxMax;	// ���f���̍ŏ��l�A�ő�l

	D3DXVECTOR3					m_pos;				// �ʒu
	D3DXVECTOR3					m_rot;				// ��]
};

#endif