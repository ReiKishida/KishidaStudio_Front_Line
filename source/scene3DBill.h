//=============================================================================
//
// 3D�r���{�[�h�I�u�W�F�N�g���� [scene3DBill.h]
// Author : Ishida Takuto
//
//=============================================================================
#ifndef _SCENE3DBILL_H_
#define _SCENE3DBILL_H_

#include "scene.h"

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CScene3DBill : public CScene
{
public:
	CScene3DBill(int nPriority = 3, CScene::OBJTYPE objType = CScene::OBJTYPE_NONE);
	~CScene3DBill();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CScene3DBill *Create(void);

	D3DXVECTOR3 GetPos(void) { return m_pos; };
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; };

	D3DXVECTOR3 GetSize(void) { return D3DXVECTOR3(m_fWidth, m_fHeight, 0.0f); };
	void SetSize(D3DXVECTOR3 size);

	float GetWidth(void) { return m_fWidth; };
	void SetWidth(float fWidth);

	float GetHeight(void) { return m_fHeight; };
	void SetHeight(float fHeight);

	void SetColor(D3DXCOLOR col);
	void AddColor(D3DXCOLOR col);
	D3DXCOLOR GetColor(void) { return m_col; };

	void BindTexture(LPDIRECT3DTEXTURE9 pTexture);

	LPDIRECT3DVERTEXBUFFER9 GetVtxBuff(void) { return m_pVtxBuff; };
	void SetVtxBuff(LPDIRECT3DVERTEXBUFFER9 vtxBuff) { m_pVtxBuff = vtxBuff; };

	void SetParent(D3DXMATRIX *pParent) { m_pMtxParent = pParent; };

	void SetZBuffer(D3DCMPFUNC cmpFunc);

	void SetLighting(bool bLighting) { m_bLighting = bLighting; };

	bool GetDisp(void) { return m_bDisp; }
	void SetDisp(bool bDisp) { m_bDisp = bDisp; }

	bool Collision(D3DXVECTOR3 pos, float fRadius);

protected:

private:
	LPDIRECT3DTEXTURE9		m_pTexture;			// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;			// ���_�o�b�t�@�ւ̃|�C���^
	D3DXMATRIX				m_mtxWorld;			// ���[���h�}�g���b�N�X
	D3DXMATRIX				*m_pMtxParent;		// �e�̃}�g���b�N�X

	D3DXVECTOR3				m_pos;				// �|���S���̈ʒu
	D3DXVECTOR3				m_rot;				// ��]
	float					m_fWidth;			// ���_��X���̋���
	float					m_fHeight;			// ���_��Y���̋���
	D3DXCOLOR				m_col;				// �F
	D3DCMPFUNC				m_cmpFunc;			// Z�o�b�t�@�̐ݒ�p
	bool					m_bZBuffer;			// Z�o�b�t�@�̐ݒ�����邩�ǂ���
	bool					m_bLighting;		// ���C�g���L����������
	bool					m_bDisp;			// �`�悷�邩�ǂ���
};

#endif