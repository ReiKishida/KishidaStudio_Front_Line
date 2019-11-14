//=============================================================================
//
// ゲージ処理 [gauge.h]
// Author : Ishida Takuto
//
//=============================================================================
#ifndef _GAUGE_H_
#define _GAUGE_H_

#include "scene.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define GAUGE_PRIORITY		(5)		// 処理の優先番号

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CLogo;
class CScene3DBill;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CGauge2D : public CScene
{
public:
	CGauge2D(int nPriority = GAUGE_PRIORITY, CScene::OBJTYPE objType = CScene::OBJTYPE_GAUGE);
	~CGauge2D();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CGauge2D* Create(int nNumBar, D3DXVECTOR3 pos, float fInit, float fMax, float fWidth, float fHeight, int nStartReduce = 60);

	void BindTexture(LPDIRECT3DTEXTURE9 pTexture, int nGauge);	// テクスチャの設定

	void AddSubtract(float fValue);		// 値を増減させる
	float GetValue(void) { return m_fGauge; };		// 値の取得
	void SetValue(float fGauge);					// 値の設定

	void SetColor(D3DXCOLOR col, int nBar);
	D3DXCOLOR GetColor(int nBar);

	void SetCurGauge(int nCurGauge) { m_nCurGauge = nCurGauge; };

	float GetMaxValue(void) { return m_fMax; }
	void SetMaxValue(float fMaxValue) { m_fMax = fMaxValue; }

	int GetNumBar(void) { return m_nNumBar; }

private:
	CLogo **m_pGauge;		// 2Dポリゴンクラスのポインタ変数
	float m_fGauge;			// 現在のゲージの値
	int m_nCntReduce;		// バーを減らし始めるまでのカウンタ
	D3DXVECTOR3 m_pos;		// 位置
	int m_nNumBar;			// バーの数
	float m_fMax;			// 最大値
	int m_nStartReduce;		// 減らし始めるまでの時間
	int m_nCurGauge;		// 現在の値を示すバーの番号
};

/*========================*/
/*		3Dゲージクラス	  */
/*========================*/
class CGauge3D : public CScene
{
public:
	CGauge3D(int nPriority = GAUGE_PRIORITY, CScene::OBJTYPE objType = CScene::OBJTYPE_GAUGE);
	~CGauge3D();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CGauge3D* Create(int nNumBar, D3DXVECTOR3 pos, float fInit, float fMax, float fWidth, float fHeight, D3DXVECTOR3 *posParent, int nStartReduce = 90);

	void BindTexture(LPDIRECT3DTEXTURE9 pTexture, int nGauge);	// テクスチャの設定

	const void AddSubtract(float fValue);				// 値を増減させる
	float Get(void) { return m_fGauge; };				// 値の取得
	void Set(float fGauge) { m_fGauge = fGauge; };		// 値の設定

	const void SetColor(D3DXCOLOR col, int nBar);
	D3DXCOLOR GetColor(int nBar);

private:
	CScene3DBill **m_pGauge;	// 3Dポリゴンクラスのポインタ変数
	float m_fGauge;				// 現在のゲージの値
	int m_nCntReduce;			// バーを減らし始めるまでのカウンタ
	D3DXVECTOR3 m_offset;		// オフセット
	D3DXCOLOR *m_pCol;			// 色
	int m_nNumBar;				// バーの数
	float m_fMax;				// 最大値
	int m_nStartReduce;			// 減らし始めるまでの時間
	D3DXVECTOR3 *m_pPosParent;	// 親の位置
};

#endif
