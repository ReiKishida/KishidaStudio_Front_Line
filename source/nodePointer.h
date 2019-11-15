//=============================================================================
//
// ノードポインタ―処理 [mouseCursor.h]
// Author : Komatsu Keisuke
//
//=============================================================================
#ifndef _NODEPOINTER_H_
#define _NODEPOINTER_H_

#include "main.h"
#include "scene3D.h"

class CScene3D;

//=============================================================================
// マクロ定義
//=============================================================================
#define NODE_PRIORITY		(6)

//=============================================================================
// クラス定義
//=============================================================================
class CNodePointer : public CScene3D
{
public:
	CNodePointer(int nPriority = NODE_PRIORITY, CScene::OBJTYPE objType = CScene::OBJTYPE_NODE);
	~CNodePointer();
	
	static CNodePointer *Create(D3DXVECTOR3 pos);
	
	HRESULT Init();
	void Uninit(void);
	void Update(void);
	void Draw(void);

	D3DXVECTOR3 &GetPos(void) { return m_pos; };
	D3DXVECTOR3 &GetSize(void) { return m_size; };
	D3DXCOLOR &GetColor(void) { return m_color; };
	int &GetMyNumber(void) { return m_nNumber; };
	int &GetConnect(int	value) { return m_nConnect[value]; };
	int &GetConnectMax(void) { return m_nConnectMax; };

	static int &GetNodeMax(void) { return m_nNodeMax; };
	static D3DXVECTOR3 &GetSearchPos(void) { return m_SearchPos; };

private:
	D3DXVECTOR3	m_pos;				// 位置
	D3DXVECTOR3 m_size;				// サイズ
	D3DXCOLOR m_color;				// 色
	int m_nNumber;					// 自分の番号
	int m_nConnect[CONNECT_MAX];	// 接続先の番号
	int m_nConnectMax;				// 全体の接続数

	static int m_nNodeMax;			// 全ノードの数
	static D3DXVECTOR3 m_SearchPos;	// 検索用位置
};
#endif
