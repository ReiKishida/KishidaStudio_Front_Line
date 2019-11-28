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

class CInputKeyboard;
class CInputMouse;
class CCamera;
class CNodePointer;

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
	typedef struct
	{
		int nodeMax;									// ノードの総数
		int	index[NODEPOINT_MAX];						// 自分のノード番号
		int connectNum[NODEPOINT_MAX];					// 接続ノード数
		int connectIndex[NODEPOINT_MAX][CONNECT_MAX];	// 接続ノード番号
		D3DXVECTOR3 pos[NODEPOINT_MAX];					// 各ノードの位置
	}NODE_LOAD_STATE;

	typedef struct
	{
		int nodeMax;			// ノードの総数
		int	index;				// 自分のノード番号
		int connectNum;			// 接続ノード数
		int *connectIndex;		// 接続ノード番号
		D3DXVECTOR3 pos;		// 各ノードの位置
	}NODE_SAVE_STATE;

	CMouseCursor(int nPriority = MOUSE_PRIORITY, CScene::OBJTYPE objType = CScene::OBJTYPE_MOUSE);
	~CMouseCursor();

	static CMouseCursor *Create();

	HRESULT Init();
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void Input(CInputKeyboard *pKeyboard, CInputMouse *pMouse);
	void Move(CInputMouse *pMouse);

	void FileLoad(char* pFileName);

	D3DXVECTOR3 GetPos(void) { return m_pos; }
	D3DXVECTOR3 &Getsetpos(void) { return m_setpos; }

private:
	NODE_LOAD_STATE m_LoadNodeData;		// ロードしたマップ情報
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
