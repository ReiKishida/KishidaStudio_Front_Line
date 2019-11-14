//=============================================================================
//
// マウスカーソル処理 [mouseCursor.h]
// Author : Komatsu Keisuke
//
//=============================================================================
#ifndef _WALL_H_
#define _WALL_H_

#include "scene3D.h"
#include "scene2D.h"

class CInputMouse;
//=============================================================================
// マクロ定義
//=============================================================================
#define MOUSE_PRIORITY		(7)		// 処理の優先番号

//=============================================================================
// クラス定義
//=============================================================================
class CMouseCursor : public CScene3D
{
public:
	CMouseCursor(int nPriority = MOUSE_PRIORITY, CScene::OBJTYPE objType = CScene::OBJTYPE_MOUSE);
	~CMouseCursor();

	static CMouseCursor *Create();

	HRESULT Init();
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void Input(CInputMouse *pMouse);
	void Move(CInputMouse *pMouse);

	static void SetMousePosition(D3DXVECTOR3 pos) { m_pos = pos; };
	static D3DXVECTOR3 GetMousePosition(void) { return m_pos; };

private:
	static D3DXVECTOR3	m_pos;		// 位置
	D3DXVECTOR3	m_cursorPos;		// カーソルの位置
};

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 2Dマウスカーソルのクラス
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class CMouseCursor2D : public CScene2D
{
public:
	CMouseCursor2D(int nPriority = MOUSE_PRIORITY, CScene::OBJTYPE objType = CScene::OBJTYPE_MOUSE);
	~CMouseCursor2D();

	static CMouseCursor2D *Create(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void Move(CInputMouse *pMouse);

	static void SetMousePosition(D3DXVECTOR3 pos) { m_pos = pos; };
	static D3DXVECTOR3 GetMousePosition(void) { return m_pos; };

private:
	static D3DXVECTOR3	m_pos;		// 位置
};

#endif
