//=============================================================================
//
// 当たり判定のライン処理 [line.h]
// Author : Ishida Takuto
//
//=============================================================================
#ifndef _LINE_H_
#define _LINE_H_

#include "scene.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// 前方宣言
//*****************************************************************************

//*****************************************************************************
// クラス定義
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

	D3DXVECTOR3 GetStart(void) { return m_start; }
	D3DXVECTOR3 GetEnd(void) { return m_end; }
	D3DXVECTOR3 GetVector(void) { return m_end - m_start; }

private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// 頂点バッファへのポインタ
	D3DXMATRIX m_mtxWorld;
	D3DXVECTOR3 m_start;
	D3DXVECTOR3 m_end;
	D3DXCOLOR m_col;
};

#endif
