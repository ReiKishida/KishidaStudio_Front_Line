//=============================================================================
//
// �I�u�W�F�N�g���� [scene2D.h]
// Author : Takuto Ishida
//
//=============================================================================
#ifndef _SCENE2D_H_
#define _SCENE2D_H_

#include "scene.h"

//====================================================================
// �N���X��`
//====================================================================
class CScene2D : public CScene
{
public:
	CScene2D(int nPriority = 3, CScene::OBJTYPE objType = CScene::OBJTYPE_NONE);
	~CScene2D();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CScene2D *Create(void);							// ����

	bool Collision(D3DXVECTOR3 pos, float vtxMaxX);			// �����蔻��

	D3DXVECTOR3 GetPos(void) { return m_pos; };				// �ʒu�̎擾
	void SetPos(D3DXVECTOR3 pos);						// �ʒu�̐ݒ�

	void SetSize(float fSizeX, float fSizeY);			// �T�C�Y�̐ݒ�

	float GetHeight(void) { return m_fSizeY; };				// �����̎擾
	void SetHeight(float fHeight);					// �����̐ݒ�

	float GetWidth(void) { return m_fSizeX; };				// ���̎擾
	void SetWidth(float fWidth);						// ���̐ݒ�

	D3DXCOLOR GetColor(void) { return m_col; };				// �F�̎擾
	void SetColor(D3DXCOLOR col);						// �F�̐ݒ�
	void AddColor(D3DXCOLOR col);						// �F�̉��Z

	void BindTexture(LPDIRECT3DTEXTURE9 pTexture);			// �e�N�X�`���̐ݒ�

	LPDIRECT3DVERTEXBUFFER9 GetVtxBuff(void) { return m_pVtxBuff; };
	void SetVtxBuff(LPDIRECT3DVERTEXBUFFER9 vtxBuff) { m_pVtxBuff = vtxBuff; };

	bool GetDisp(void) { return m_bDisp; }
	void SetDisp(bool bDisp) { m_bDisp = bDisp; }

	void Flashing(int nFlame);		// �_��
	void SetBgMove(int nCntBg_X, int nCntBg_Y, float fScrollSpeed);	// �w�i�������i���j
	void SetTex(int nUV, int nUV_X, int nUV_Y);

protected:

private:
	LPDIRECT3DTEXTURE9		m_pTexture;		// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// ���_�o�b�t�@�ւ̃|�C���^

	D3DXVECTOR3				m_pos;			// �|���S���̈ʒu
	float					m_fSizeX;		// X�̑傫��
	float					m_fSizeY;		// Y�̑傫��
	D3DXCOLOR				m_col;			// �F
	int						m_nCntFlash;	// �_�ŗp�J�E���^
	bool					m_bDisp;		// �\������\����
};

#endif