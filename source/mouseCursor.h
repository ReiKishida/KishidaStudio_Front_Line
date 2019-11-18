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

	void SaveLoad(CInputKeyboard *pKeyboard);
	HRESULT FileSave(void);
	void FileLoad(char* pFileName);

	D3DXVECTOR3 GetPos(void) { return m_pos; }

private:
	NODE_LOAD_STATE m_LoadNodeData;		// ロードしたマップ情報
	NODE_SAVE_STATE m_SaveNodeData;		// セーブするマップ情報

	LPDIRECT3DTEXTURE9 m_pTexture;	// テクスチャへのポインタ
	D3DXVECTOR3	m_pos;				// 位置
	D3DXVECTOR3 m_size;				// サイズ

	int m_nNodeCounter;				// 配置したノードの数
	int m_nConnectNodeCounter;		// 配置した接続先ノードの数
	int m_nSelectNode;				// 選択している接続元ノードの番号
	int m_nSelectConnectNode;		// 選択している接続先ノードの番号
	int m_nMassageCount;			// メッセージを表示する時間

	bool m_bConnectMode;			// 接続設定状態か
	bool m_bConnectSet;				// 接続元設定状態か
	bool m_bConnectNumSet;			// 接続先設定状態か
	bool m_bSaveMassage;			// セーブ完了メッセージのONOFF
	bool m_bErrorMassage;			// エラーメッセージのONOFF

									// 接続元ノードの情報	(コメント用)
	D3DXVECTOR3	m_SelectNodePos;			// 位置
	int	m_SelectNodeNumber;					// 番号
	int	m_SelectNodeConnectMax;				// 接続数
	int	m_SelectNodeConnect[CONNECT_MAX];	// 接続先

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
