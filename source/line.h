//=============================================================================
//
// �����蔻��̃��C������ [line.h]
// Author : Ishida Takuto
//
//=============================================================================
#ifndef _LINE_H_
#define _LINE_H_

#include "scene.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define LINE_PRIORITY	(6)		// �����̗D�揇��

//*****************************************************************************
// �O���錾
//*****************************************************************************

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CLine : public CScene
{
public:
	CLine();
	~CLine();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CLine *Create(D3DXVECTOR3 start, D3DXVECTOR3 end, D3DXCOLOR col);

	void MakeVertex(LPDIRECT3DDEVICE9 pDevice);

	void SetPoint(D3DXVECTOR3 start, D3DXVECTOR3 end);

	void SetColor(D3DXCOLOR col);

	D3DXVECTOR3 GetStart(void) { return m_start; };
	void SetStart(D3DXVECTOR3 start) { m_start = start; };

	D3DXVECTOR3 GetEnd(void) { return m_end; };
	void SetEnd(D3DXVECTOR3 end) { m_end = end; };

	D3DXVECTOR3 GetVector(void) { return m_end - m_start; };

private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// ���_�o�b�t�@�ւ̃|�C���^
	D3DXMATRIX m_mtxWorld;
	D3DXVECTOR3 m_start;
	D3DXVECTOR3 m_end;
	D3DXCOLOR m_col;
};

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 2D�̃��C���̃N���X
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class CLine2D : public CScene
{
public:
	CLine2D(int nPriority = LINE_PRIORITY, CScene::OBJTYPE objType = CScene::OBJTYPE_LINE);
	~CLine2D();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CLine2D *Create(D3DXVECTOR3 start, D3DXVECTOR3 end, D3DXCOLOR col);

	void MakeVertex(LPDIRECT3DDEVICE9 pDevice);
	void Link(D3DXVECTOR3 end);

	void SetPoint(D3DXVECTOR3 start, D3DXVECTOR3 end);

	D3DXCOLOR GetColor(void) { return m_col; };
	void SetColor(D3DXCOLOR col);

	D3DXVECTOR3 GetStart(void) { return m_start; };
	void SetStart(D3DXVECTOR3 start);

	D3DXVECTOR3 GetEnd(void) { return m_end; };
	void SetEnd(D3DXVECTOR3 end);

	D3DXVECTOR3 GetVector(void) { return m_end - m_start; };

private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// ���_�o�b�t�@�ւ̃|�C���^
	D3DXMATRIX m_mtxWorld;
	D3DXVECTOR3 m_rot;
	D3DXVECTOR3 m_start;
	D3DXVECTOR3 m_end;
	D3DXCOLOR m_col;
};


#endif
