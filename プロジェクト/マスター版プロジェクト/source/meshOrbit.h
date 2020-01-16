//=============================================================================
//
// �O�Տ��� [meshOrbit.h]
// Author : Takuto Ishida
//
//=============================================================================
#ifndef _MESHORBIT_H_
#define _MESHORBIT_H_

#include "scene.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define FIELD_TEXTURE	(1)		// �e�N�X�`����
#define MAX_ORBIT_VERTEX	(128)		// �e�N�X�`����

//*****************************************************************************
// �O���錾
//*****************************************************************************

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CMeshOrbit : public CScene
{
public:
	CMeshOrbit(int nPriority = 5, CScene::OBJTYPE objType = CScene::OBJTYPE_ORBIT);
	~CMeshOrbit();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CMeshOrbit* Create(D3DXVECTOR3 offset, int nLife, D3DXMATRIX *pMtx);

	static HRESULT Load(void);				// �e�N�X�`����ǂݍ���
	static void Unload(void);				// �ǂݍ��񂾃e�N�X�`�������

private:
	static LPDIRECT3DTEXTURE9 m_pTexture[FIELD_TEXTURE];	// ���L�e�N�X�`���ւ̃|�C���^

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;				// ���_�o�b�t�@�ւ̃|�C���^
	D3DXMATRIX				m_mtxWorld;				// ���[���h�}�g���b�N�X
	D3DXMATRIX				*m_pMtxParent;			// �e�̃}�g���b�N�X�ւ̃|�C���^
	D3DXVECTOR3				m_aPosVertex[MAX_ORBIT_VERTEX];		// �Z�o�������_���W
	D3DXVECTOR3				m_aOffset[2];						// ���[�̃I�t�Z�b�g

	D3DXCOLOR				m_col;					// �F
	int						m_nLife;				// ����
};

#endif
