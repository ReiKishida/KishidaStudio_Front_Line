//=============================================================================
//
// レンダリング処理 [renderer.h]
// Author : Takuto Ishida
//
//=============================================================================
#ifndef _RENDERER_H_
#define _RENDERER_H_

#include "main.h"
#include "manager.h"

//=====================================
// 前方宣言
//=====================================

//=====================================
// クラス定義
//=====================================
class CRenderer : public CManager
{
public:
	CRenderer();
	~CRenderer();
	HRESULT Init(HINSTANCE hInstance, HWND hWnd, bool bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	LPDIRECT3DDEVICE9 GetDevice(void) { return m_pD3DDevice; };	// デバイスの取得
	HWND GetHWnd(void) { return m_hWnd; }
	bool GetWindowMode(void) { return m_bWindowMode; };

private:
	LPDIRECT3D9				m_pD3D;			// Direct3Dオブジェクト
	LPDIRECT3DDEVICE9		m_pD3DDevice;	// Deviceオブジェクト(描画に必要)
	HWND					m_hWnd;			// ウィンドウハンドル
	bool					m_bWindowMode;	// ウィンドウかフルスクリーンどちらか

#ifdef _DEBUG
	LPD3DXFONT				m_pFont;		// フォントへのポインタ
#endif
};

#endif