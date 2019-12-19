//=============================================================================
//
// ポーズメニュー処理 [pause.h]
// Author : Takuto Ishida
//
//=============================================================================
#ifndef _PAUSE_H_
#define _PAUSE_H_

#include "scene.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CScene2D;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CPause : public CScene
{
public:
	//*****************************************************************************
	// ポーズメニュー
	//*****************************************************************************
	typedef enum
	{
		TYPE_CONTINUE = 0,
		TYPE_RETRY,
		TYPE_QUIT,
		TYPE_MAX
	}TYPE;

	CPause(int nPriority = 6, CScene::OBJTYPE objType = CScene::OBJTYPE_PAUSE);
	~CPause();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CPause* Create(void);

	static HRESULT Load(void);				// テクスチャを読み込む
	static void Unload(void);				// 読み込んだテクスチャを解放

private:
	void Select(void);

	static LPDIRECT3DTEXTURE9 m_pTexture[TYPE_MAX];	// 共有テクスチャへのポインタ

	CScene2D *m_apScene2D[TYPE_MAX];	// 項目
	int m_nNumSelect;					// 選択中の項目
};

#endif