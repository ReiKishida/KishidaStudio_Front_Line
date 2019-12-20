//=============================================================================
//
// �C���f�b�N�X3D�I�u�W�F�N�g���� [scene3DIndex.h]
// Author : Takuto Ishida
//
//=============================================================================
#ifndef _SCENE3DINDEX_H_
#define _SCENE3DINDEX_H_

#include "scene.h"

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CScene3DIndex : public CScene
{
public:
	typedef enum
	{
		TYPE_FIELD = 0,
		TYPE_CYLINDER,
		TYPE_MAX
	}TYPE;

	CScene3DIndex(int nPriority = 3, OBJTYPE objType = OBJTYPE_FIELD);
	~CScene3DIndex();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void MakeVertex(void);				// ���_�̍쐬
	void SetVtx(void);					// ���_���̐ݒ�
	void SetNor(VERTEX_3D *pVtx);		// �@���̌v�Z
	void SetInfo(float fWidth, float fHeight, float fDepth, int nMeshX, int nMeshY, int nMeshZ);
	void GetInfo(float *pWidth, float *pHeight, float *pDepth, int *pMeshX, int *pMeshY, int *pMeshZ);

	D3DXVECTOR3 GetPos(void) { return m_pos; };
	const void SetPos(D3DXVECTOR3 pos) { m_pos = pos; };

	D3DXVECTOR3 GetRot(void) { return m_rot; };
	const void SetRot(D3DXVECTOR3 rot) { m_rot = rot; };

	void BindTexture(LPDIRECT3DTEXTURE9 pTexture);

	// ���_�o�b�t�@
	LPDIRECT3DVERTEXBUFFER9 GetVtxBuff(void) { return m_pVtxBuff; };
	const void SetVtxBuff(LPDIRECT3DVERTEXBUFFER9 vtxBuff) { m_pVtxBuff = vtxBuff; };

	// �C���f�b�N�X�o�b�t�@
	LPDIRECT3DINDEXBUFFER9 GetIdxBuff(void) { return m_pIdxBuff; };
	const void SetIdxBuff(LPDIRECT3DINDEXBUFFER9 IdxBuff) { m_pIdxBuff = IdxBuff; };

	int GetNumVtx(void) { return m_nNumVtx; };
	int GetNumPolygon(void) { return m_nNumPolygon; };

	TYPE GetType(void) { return m_type; };
	void SetType(TYPE type) { m_type = type; };

	D3DXMATRIX GetMtxWorld(void) { return m_mtxWorld; };
	void SetMtxWorld(D3DXMATRIX mtxWorld) { m_mtxWorld = mtxWorld; };

	int GetNumVtxX(void) { return m_nMeshX; };
	int GetNumVtxY(void) { return m_nMeshY; };
	int GetNumVtxZ(void) { return m_nMeshZ; };

protected:

private:
	LPDIRECT3DTEXTURE9		m_pTexture;				// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;				// ���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DINDEXBUFFER9	m_pIdxBuff;				// �C���f�b�N�X�o�b�t�@�ւ̃|�C���^
	D3DXMATRIX				m_mtxWorld;				// ���[���h�}�g���b�N�X

	D3DXVECTOR3				m_pos;					// �|���S���̈ʒu
	D3DXVECTOR3				m_rot;					// ��]
	int						m_nNumVtx;				// ���_��
	int						m_nNumPolygon;			// �|���S����
	TYPE					m_type;					// �����̎��
	float					m_fLengthX;				// ���_��X���̋���
	float					m_fLengthY;				// ���_��Y���̋���
	float					m_fLengthZ;				// ���_��Z���̋���
	int						m_nMeshX;				// X���̕�����
	int						m_nMeshY;				// Y���̕�����
	int						m_nMeshZ;				// Z���̕�����
};

#endif