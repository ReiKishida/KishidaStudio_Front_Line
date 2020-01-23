//=============================================================================
//
// ピン立て処理 [pin.h]
// Author : Komatsu Keisuke
//
//=============================================================================
#ifndef _PIN_H_
#define _PIN_H_

#include "main.h"
#include "scene3DBill.h"

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
#define PIN_PRIORITY		(7)		// 処理の優先番号

//=============================================================================
// クラス定義
//=============================================================================
class CPin : public CScene3DBill
{
public:
	CPin(int nPriority = PIN_PRIORITY, CScene::OBJTYPE objType = CScene::OBJTYPE_PIN);
	~CPin();

	static CPin *Create(D3DXVECTOR3 pos);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	D3DXVECTOR3 &GetPos(void) { return m_pos; }
	LPDIRECT3DTEXTURE9 &GetTexture(void) { return m_pTexture; };

private:
	LPDIRECT3DTEXTURE9 m_pTexture;	// テクスチャ
	D3DXVECTOR3	m_pos;				// 位置
	D3DXVECTOR3 m_size;				// サイズ
};
#endif
