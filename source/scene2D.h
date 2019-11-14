//=============================================================================
//
// オブジェクト処理 [scene2D.h]
// Author : Takuto Ishida
//
//=============================================================================
#ifndef _SCENE2D_H_
#define _SCENE2D_H_

#include "scene.h"

//====================================================================
// クラス定義
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

	static CScene2D *Create(void);							// 生成

	bool Collision(D3DXVECTOR3 pos, float vtxMaxX);			// 当たり判定

	D3DXVECTOR3 GetPos(void) { return m_pos; };				// 位置の取得
	void SetPos(D3DXVECTOR3 pos);						// 位置の設定

	void SetSize(float fSizeX, float fSizeY);			// サイズの設定

	float GetHeight(void) { return m_fSizeY; };				// 高さの取得
	void SetHeight(float fHeight);					// 高さの設定

	float GetWidth(void) { return m_fSizeX; };				// 幅の取得
	void SetWidth(float fWidth);						// 幅の設定

	D3DXCOLOR GetColor(void) { return m_col; };				// 色の取得
	void SetColor(D3DXCOLOR col);						// 色の設定
	void AddColor(D3DXCOLOR col);						// 色の加算

	void BindTexture(LPDIRECT3DTEXTURE9 pTexture);			// テクスチャの設定

	LPDIRECT3DVERTEXBUFFER9 GetVtxBuff(void) { return m_pVtxBuff; };
	void SetVtxBuff(LPDIRECT3DVERTEXBUFFER9 vtxBuff) { m_pVtxBuff = vtxBuff; };

	bool GetDisp(void) { return m_bDisp; }
	void SetDisp(bool bDisp) { m_bDisp = bDisp; }

	void Flashing(int nFlame);		// 点滅
	void SetBgMove(int nCntBg_X, int nCntBg_Y, float fScrollSpeed);	// 背景動かす（横）
	void SetTex(int nUV, int nUV_X, int nUV_Y);

protected:

private:
	LPDIRECT3DTEXTURE9		m_pTexture;		// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// 頂点バッファへのポインタ

	D3DXVECTOR3				m_pos;			// ポリゴンの位置
	float					m_fSizeX;		// Xの大きさ
	float					m_fSizeY;		// Yの大きさ
	D3DXCOLOR				m_col;			// 色
	int						m_nCntFlash;	// 点滅用カウンタ
	bool					m_bDisp;		// 表示か非表示か
};

#endif