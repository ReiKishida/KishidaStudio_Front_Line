//=============================================================================
//
// ロゴ処理 [logo.h]
// Author : Takuto Ishida
//
//=============================================================================
#ifndef _LOGO_H_
#define _LOGO_H_

#include "scene2D.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define LOGO_PRIORITY	(5)		// 処理の優先順位

//*****************************************************************************
// 前方宣言
//*****************************************************************************

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CLogo : public CScene2D
{
public:
	CLogo(int nPriority = LOGO_PRIORITY, CScene::OBJTYPE objType = CScene::OBJTYPE_LOGO);
	~CLogo();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CLogo* Create(D3DXVECTOR3 pos, float fWidth, float fHeight);	// 生成

private:
};

/*========================*/
/*   入力待ち表示クラス   */
/*========================*/
class CLogoPressButton : public CLogo
{
public:
	CLogoPressButton(int nPriority = LOGO_PRIORITY, CScene::OBJTYPE objType = CScene::OBJTYPE_LOGO);
	~CLogoPressButton();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CLogoPressButton* Create(D3DXVECTOR3 pos, float fWidth, float fHeight);		// 生成

private:
	LPDIRECT3DTEXTURE9 m_pTexture;	// テクスチャへのポインタ
	int m_nCntFlash;				// 点滅用カウンタ
};

/*========================*/
/*   操作説明表示クラス   */
/*========================*/
class CLogoTutorial : public CLogo
{
public:
	CLogoTutorial(int nPriority = LOGO_PRIORITY, CScene::OBJTYPE objType = CScene::OBJTYPE_LOGO);
	~CLogoTutorial();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CLogoTutorial* Create(D3DXVECTOR3 pos, float fWidth, float fHeight);		// 生成

private:
	LPDIRECT3DTEXTURE9 m_pTexture;	// テクスチャへのポインタ
};

#endif