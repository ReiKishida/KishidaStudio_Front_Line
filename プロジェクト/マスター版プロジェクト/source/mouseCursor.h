//=============================================================================
//
// マウスカーソル処理 [mouseCursor.h]
// Author : Komatsu Keisuke
//
//=============================================================================
#ifndef _MOUSECURSOR_H_
#define _MOUSECURSOR_H_

#include "main.h"
#include "scene3D.h"
#include "scene2D.h"

//=============================================================================
// 前方宣言
//=============================================================================
class CInputKeyboard;
class CInputMouse;
class CCamera;
class CNodePointer;
class CNodeDataFiler;

//=============================================================================
// マクロ定義
//=============================================================================
#define MOUSE_PRIORITY		(7)		// 処理の優先番号

//=============================================================================
// 3Dマウスカーソルのクラス
// Author : Komatsu Keisuke
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

	D3DXVECTOR3 GetPos(void) { return m_pos; }
	D3DXVECTOR3 &Getsetpos(void) { return m_setpos; }

private:
	void Mouse(CInputMouse *pMouse);
	void File(CInputKeyboard *pKeyboard);

	CNodeDataFiler *m_pNodeData;	// マップ情報へのポインタ
	LPDIRECT3DTEXTURE9 m_pTexture;	// テクスチャへのポインタ
	D3DXVECTOR3	m_pos;				// 位置
	D3DXVECTOR3	m_setpos;			// 地点登録用位置
	D3DXVECTOR3 m_size;				// サイズ
	int m_nNodeCounter;				// 配置したノードの数

	static CCamera *m_pCamera;		// カメラへのポインタ
	static CNodePointer *m_pNodePointer[NODEPOINT_MAX];	// ノードポリゴンへのポインタ
};

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 2Dマウスカーソルのクラス
// Author : Takuto Ishida
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
