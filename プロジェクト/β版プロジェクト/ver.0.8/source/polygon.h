//=============================================================================
//
// ポリゴン処理 [polygon.h]
// Author : Ishida Takuto
//
//=============================================================================
#ifndef _POLYGON_H_
#define _POLYGON_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// クラス定義
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

	static HRESULT Load(void);				// テクスチャを読み込む
	static void Unload(void);				// 読み込んだテクスチャを解放

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
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;					// 頂点バッファへのポインタ

	D3DXVECTOR3 m_pos;									// 座標
	D3DXCOLOR m_col;									// 色
	float m_fWidth;										// ポリゴンのＸ軸
	float m_fHeight;									// ポリゴンのＹ軸
	int	m_nTexType;										// テクスチャ番号
	int m_nType;										// 作り方の種類
	int m_nCntColor;									// 色のカウンタ
};

#endif