//=============================================================================
//
// �|���S������ [polygon.h]
// Author : Ishida Takuto
//
//=============================================================================
#ifndef _POLYGON_H_
#define _POLYGON_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CPolygon
{
public:
	CPolygon();
	~CPolygon();
	HRESULT Init(D3DXVECTOR3 pos, float fWidth, float fHeight, int nType);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CPolygon* Create(int nType, D3DXVECTOR3 pos, float fWidth, float fHeight, int nTexType = -1);

	static HRESULT Load(void);				// �e�N�X�`����ǂݍ���
	static void Unload(void);				// �ǂݍ��񂾃e�N�X�`�������

	const void SetPos(D3DXVECTOR3 pos);
	D3DXVECTOR3 GetPos(void) { return m_pos; };

	const void SetColor(D3DXCOLOR col);
	const void AddColor(D3DXCOLOR col);
	D3DXCOLOR GetColor(void) { return m_col; };

	const void SetWidth(float fWidth);
	float GetWidth(void) { return m_fWidth; };

	const void SetHeight(float fHeight);
	float GetHeight(void) { return m_fHeight; };

	void Flashing(int nFlame);

private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;					// ���_�o�b�t�@�ւ̃|�C���^

	D3DXVECTOR3 m_pos;									// ���W
	D3DXCOLOR m_col;									// �F
	float m_fWidth;										// �|���S���̂w��
	float m_fHeight;									// �|���S���̂x��
	int	m_nTexType;										// �e�N�X�`���ԍ�
	int m_nType;										// �����̎��
	int m_nCntColor;									// �F�̃J�E���^
};

#endif