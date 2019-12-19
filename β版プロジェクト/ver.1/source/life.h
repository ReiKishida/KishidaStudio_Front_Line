//=============================================================================
//
// 体力処理 [life.h]
// Author : Ishida Takuto
//
//=============================================================================
#ifndef _LIFE_H_
#define _LIFE_H_

#include "scene.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define LIFE_PRIORITY		(5)		// ライフの優先番号
#define MAX_POLYGON_LIFE	(3)		// ポリゴンの最大数
#define MAX_LIFE			(100)	// ライフの上限
#define MAX_LIFETEXTURE		(4)		// テクスチャ数

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CGauge2D;
class CGauge3D;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CLifeManager : public CScene
{
public:
	CLifeManager(int nPriority = LIFE_PRIORITY, CScene::OBJTYPE objType = CScene::OBJTYPE_LIFE);
	~CLifeManager();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static HRESULT Load(void);		// データの読み込み
	static void Unload(void);		// 読み込んだデータの破棄

	LPDIRECT3DTEXTURE9 Getexture(int nTexture) { return m_pTexture[nTexture]; };

	virtual const void AddSubtract(int nValue);		// 体力を増減させる
	int GetLife(void) { return m_nLife; };			// 体力の取得
	void SetLife(int nLife) { m_nLife = nLife; };	// 体力の設定

private:
	static LPDIRECT3DTEXTURE9 m_pTexture[MAX_LIFETEXTURE];	// 共有テクスチャへのポインタ
	int m_nLife;			// 現在の体力の値(最前バー)
};

/*===========================*/
/*		2D体力クラス		 */
/*===========================*/
class CLife2D : public CLifeManager
{
public:
	CLife2D(int nPriority = LIFE_PRIORITY, CScene::OBJTYPE objType = CScene::OBJTYPE_LIFE);
	~CLife2D();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CLife2D *Create(int nLife, D3DXVECTOR3 pos, float fWidth, float fHeight);

	const void AddSubtract(int nValue);

private:
	CGauge2D *m_pGauge;		// ゲージクラスのポインタ変数
};

/*===========================*/
/*		3D体力クラス		 */
/*===========================*/
class CLife3D : public CLifeManager
{
public:
	CLife3D(int nPriority = LIFE_PRIORITY, CScene::OBJTYPE objType = CScene::OBJTYPE_LIFE);
	~CLife3D();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CLife3D *Create(int nLife, D3DXVECTOR3 pos, float fWidth, float fHeight, D3DXVECTOR3 *pPosParent);

	const void AddSubtract(int nValue);

private:
	CGauge3D *m_pGauge;		// ゲージクラスのポインタ変数
};

#endif
