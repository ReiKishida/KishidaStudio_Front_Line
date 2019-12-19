//=============================================================================
//
// フェード処理 [fade.h]
// Author : Takuto Ishida
//
//=============================================================================
#ifndef _FADE_H_
#define _FADE_H_

#include "scene2D.h"
#include "manager.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CFade : public CScene2D
{
public:
	typedef enum
	{// フェードの状態
		FADE_NONE = 0,		// 何もしていない状態
		FADE_IN,			// フェードイン状態
		FADE_OUT,			// フェードアウト状態
		FADE_MAX
	} FADE;

	CFade(int nPriority = 7, CScene::OBJTYPE objType = CScene::OBJTYPE_FADE);
	~CFade();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CFade *Create(CManager::MODE modeNext);

	static FADE GetFade(void) { return m_fade; };

private:
	LPDIRECT3DTEXTURE9		m_pTexture;		// 共有テクスチャへのポインタ
	static FADE				m_fade;			// フェード状態
	static CManager::MODE	m_modeNext;		// 次のモード
	static D3DXCOLOR		m_colFade;		// フェード色
};

#endif
