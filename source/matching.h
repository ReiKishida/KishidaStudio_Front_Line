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
#define MATCHING_UI_PLAYER				(4)
//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CUI_TEXTURE;
class CScene2D;

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
	void PrintData(void);
	void ReadMessage(void);

	CUI_TEXTURE		*m_pUITex[MATCHING_UITEX];		// UIテクスチャポインタ
	CScene2D		*m_pMatchingPlayerUI[MATCHING_UI_PLAYER];
	int						m_nCntBgMove;			// スクロール
	int m_nCntFade;
	bool m_bFade;
};

#endif