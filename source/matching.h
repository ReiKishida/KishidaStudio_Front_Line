//=============================================================================
//
// マッチング画面処理 [matching.h]
// Author : Ayaka Hakozaki
//
//=============================================================================
#ifndef _MATCHING_H_
#define _MATCHING_H_

#include "scene.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MATCHING_PRIORITY				(7)		// 優先番号
#define MATCHING_UITEX						(8)

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CUI_TEXTURE;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CMatching : public CScene
{
public:
	CMatching(int nPriority = MATCHING_PRIORITY, CScene::OBJTYPE objType = CScene::OBJTYPE_MATCHING);
	~CMatching();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	CUI_TEXTURE		*m_pUITex[MATCHING_UITEX];		// UIテクスチャポインタ
	int						m_nCntBgMove;			// スクロール
};

#endif