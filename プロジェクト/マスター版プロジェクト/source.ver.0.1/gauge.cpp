//=============================================================================
//
// ゲージ処理 [gauge.h]
// Author : Ishida Takuto
//
//=============================================================================
#include "gauge.h"
#include "logo.h"
#include "scene3DBill.h"
#include "manager.h"
#include "renderer.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================

//=============================================================================
// 生成処理
//=============================================================================
CGauge2D* CGauge2D::Create(int nNumBar, D3DXVECTOR3 pos, float fInit, float fMax, float fWidth, float fHeight, int nStartReduce)
{
	CGauge2D *pGauge = NULL;

	pGauge = new CGauge2D;	// メモリを確保

	if (pGauge != NULL)
	{// メモリ確保成功
		pGauge->m_pos = pos;					// バーの位置
		pGauge->m_nNumBar = nNumBar;			// バーの本数
		pGauge->m_fGauge = fInit;				// 初期値
		pGauge->m_fMax = fMax;					// 最大値
		pGauge->m_nStartReduce = nStartReduce;	// 減らし始めるまでの時間
		pGauge->m_nCurGauge = nNumBar - 1;
		pGauge->m_pGauge = new CLogo*[nNumBar];
		for (int nCntGauge = 0; nCntGauge < nNumBar; nCntGauge++)
		{// ゲージバーの生成
			pGauge->m_pGauge[nCntGauge] = CLogo::Create(pos, fWidth, fHeight);		// バーの生成
		}
	}

	// 初期化処理
	pGauge->Init();

	return pGauge;
}

//=========================================
// テクスチャを割り当てる
//=========================================
void CGauge2D::BindTexture(LPDIRECT3DTEXTURE9 pTexture, int nGauge)
{
	if (nGauge < m_nNumBar)
	{// バーの本数を超えないようにする
		m_pGauge[nGauge]->BindTexture(pTexture);
	}
}

//=============================================================================
// コンストラクタ
//=============================================================================
CGauge2D::CGauge2D(int nPriority, CScene::OBJTYPE objType) : CScene(nPriority, objType)
{
	m_pGauge = NULL;
	m_nCurGauge = 0;
	m_bDisp = true;

}

//=============================================================================
// デストラクタ
//=============================================================================
CGauge2D::~CGauge2D()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CGauge2D::Init(void)
{
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CGauge2D::Uninit(void)
{
	for (int nCntGauge = 0; nCntGauge < m_nNumBar; nCntGauge++)
	{
		if (NULL != m_pGauge[nCntGauge])
		{// 2Dポリゴンの破棄
			m_pGauge[nCntGauge]->Uninit();
			m_pGauge[nCntGauge] = NULL;
		}
	}

	if (NULL != m_pGauge)
	{// メモリの開放
		delete m_pGauge;
		m_pGauge = NULL;
	}

	m_nNumBar = 0;
	// 自分を破棄
	CScene::Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CGauge2D::Update(void)
{
	if (m_nNumBar >= 3)
	{// バーの本数が３本以上のとき
		const float fWidth = m_pGauge[0]->GetWidth();	// 横幅の長さ

		float fLife = m_pGauge[m_nCurGauge - 1]->GetWidth();				// バーの長さを取得
		float fGaugeView = fLife * (1.0f / (fWidth * 0.01f));	// ％にする
		float fPercent = m_fGauge * (1.0f / (m_fMax * 0.01f));	// 値を%にする

		if (m_nStartReduce > m_nCntReduce)
		{// カウントする
			m_nCntReduce++;
		}

		if (m_nStartReduce <= m_nCntReduce)
		{// 一定時間経過
			if (fPercent < fGaugeView)
			{// ゲージが現在の値と同じになるまで減らす
				fLife -= fWidth * 0.01f;			// ゲージの長さを1％ずつ減らす
				m_pGauge[m_nCurGauge - 1]->SetWidth(fLife);		// バーの長さを設定
			}
			else if (fPercent > fGaugeView)
			{// ゲージが現在の値と同じになるまで増やす
				fLife += fWidth * 0.01f;			// ゲージの長さを1％ずつ増やす
				m_pGauge[m_nCurGauge - 1]->SetWidth(fLife);		// バーの長さを設定
			}
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CGauge2D::Draw(void)
{
}

//=============================================================================
// 値の設定
//=============================================================================
void CGauge2D::SetValue(float fGauge)
{
	m_fGauge = fGauge;
	float fBarWidth;		// バーの長さの値
	const float fWidth = m_pGauge[0]->GetWidth();			// 横幅の長さ

	if (m_fGauge > 0)
	{// 0より大きいとき
		fBarWidth = fWidth / (m_fMax / m_fGauge);	// バーの元の長さを100%として増減させる
	}
	else
	{// ライフが0のとき
		fBarWidth = 0;
	}

	if (m_fGauge >= m_fMax)
	{// 最大値を超えたら戻す
		m_fGauge = m_fMax;
		fBarWidth = fWidth;
	}

	// 頂点情報の取得
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = m_pGauge[m_nCurGauge]->GetVtxBuff();

	// 頂点情報を設定
	VERTEX_2D *pVtx;	// 頂点情報のポインタ

						// 頂点バッファをロックし、頂点データへのポインタを取得
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	float fGaugeWidth = m_pGauge[m_nCurGauge]->GetWidth();
	D3DXVECTOR3 pos = m_pGauge[m_nCurGauge]->GetPos();

	// テクスチャ座標を更新
	pVtx[0].pos.x = pos.x + (-fGaugeWidth * 0.5f);
	pVtx[1].pos.x = pos.x + (-fGaugeWidth * 0.5f + fBarWidth);
	pVtx[2].pos.x = pos.x + (-fGaugeWidth * 0.5f);
	pVtx[3].pos.x = pos.x + (-fGaugeWidth * 0.5f + fBarWidth);

	// 頂点バッファをアンロック
	pVtxBuff->Unlock();

	// 頂点バッファを設定
	m_pGauge[m_nCurGauge]->SetVtxBuff(pVtxBuff);

	// バーの長さを更新
	//m_pGauge[m_nCurGauge]->SetWidth(fBarWidth);

	if (fGauge > 0 && m_nNumBar > 2)
	{// 回復したとき後ろのバーも増やす
		m_pGauge[m_nCurGauge - 1]->SetWidth(fBarWidth);		// バーの長さを更新
	}

	m_nCntReduce = 0;						// バーを減らし始めるためのカウンタを初期化
}

//=============================================================================
// ゲージの増減
//=============================================================================
void CGauge2D::AddSubtract(float fValue)
{
	m_fGauge += fValue;		// ライフを増減させる
	float fBarWidth;		// バーの長さの値
	const float fWidth = m_pGauge[0]->GetWidth();			// 横幅の長さ

	if (m_fGauge > 0)
	{// 0より大きいとき
		fBarWidth = fWidth / (m_fMax / m_fGauge);	// バーの元の長さを100%として増減させる
	}
	else
	{// 0のとき
		fBarWidth = 0;
	}

	if (m_fGauge >= m_fMax)
	{// 最大値を超えたら戻す
		m_fGauge = m_fMax;
		fBarWidth = fWidth;
	}

	// 頂点情報の取得
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = m_pGauge[m_nCurGauge]->GetVtxBuff();

	// 頂点情報を設定
	VERTEX_2D *pVtx;	// 頂点情報のポインタ

						// 頂点バッファをロックし、頂点データへのポインタを取得
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	float fGaugeWidth = m_pGauge[m_nCurGauge]->GetWidth();
	D3DXVECTOR3 pos = m_pGauge[m_nCurGauge]->GetPos();

	// テクスチャ座標を更新
	pVtx[0].pos.x = pos.x + (-fGaugeWidth * 0.5f);
	pVtx[1].pos.x = pos.x + (-fGaugeWidth * 0.5f + fBarWidth);
	pVtx[2].pos.x = pos.x + (-fGaugeWidth * 0.5f);
	pVtx[3].pos.x = pos.x + (-fGaugeWidth * 0.5f + fBarWidth);

	// 頂点バッファをアンロック
	pVtxBuff->Unlock();

	// 頂点バッファを設定
	m_pGauge[m_nCurGauge]->SetVtxBuff(pVtxBuff);

	//// バーの長さを更新
	//m_pGauge[m_nCurGauge]->SetWidth(fBarWidth);

	if (fValue > 0 && m_nNumBar > 2)
	{// 回復したとき後ろのバーも増やす
		m_pGauge[m_nCurGauge - 1]->SetWidth(fBarWidth);			// バーの長さを更新
	}

	m_nCntReduce = 0;	// ダメージバーを減らし始めるためのカウンタを初期化
}

//=============================================================================
// 色の設定
//=============================================================================
void CGauge2D::SetColor(D3DXCOLOR col, int nBar)
{
	if (m_nNumBar > nBar)
	{// 使われている本数まで
		m_pGauge[nBar]->SetColor(col);
	}
}

//=============================================================================
// 色の取得
//=============================================================================
D3DXCOLOR CGauge2D::GetColor(int nBar)
{
	if (m_nNumBar > nBar)
	{// 使われている本数まで
		return m_pGauge[nBar]->GetColor();
	}

	// 使われていない本数が指定されたとき
	return D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
}

//=============================================================================
// 表示の処理
//=============================================================================
void CGauge2D::SetDisp(bool bDisp)
{
	m_bDisp = bDisp;

	for (int nCntGauge = 0; nCntGauge < m_nNumBar; nCntGauge++)
	{
		if (NULL != m_pGauge[nCntGauge])
		{// 2Dポリゴンの破棄
			m_pGauge[nCntGauge]->SetDisp(m_bDisp);
		}
	}

}

/*==============================================*/
/*				 3Dゲージクラス					*/
/*==============================================*/
//=============================================================================
// 生成処理
//=============================================================================
CGauge3D* CGauge3D::Create(int nNumBar, D3DXVECTOR3 offset, float fInit, float fMax, float fWidth, float fHeight, D3DXVECTOR3 *pPosParent, int nStartReduce)
{
	CGauge3D *pGauge = NULL;

	pGauge = new CGauge3D;	// メモリを確保

	if (pGauge != NULL)
	{// メモリ確保成功
		pGauge->m_offset = offset;				// バーのオフセット
		pGauge->m_nNumBar = nNumBar;			// バーの本数
		pGauge->m_fGauge = fInit;				// 初期値
		pGauge->m_fMax = fMax;					// 最大値
		pGauge->m_nStartReduce = nStartReduce;	// 減らし始めるまでの時間
		pGauge->m_pPosParent = pPosParent;		// 親の位置

		pGauge->m_pGauge = new CScene3DBill*[nNumBar];
		for (int nCntGauge = 0; nCntGauge < nNumBar; nCntGauge++)
		{// ゲージバーの生成
			pGauge->m_pGauge[nCntGauge] = CScene3DBill::Create();		// バーの生成
			pGauge->m_pGauge[nCntGauge]->SetPos(offset + *pPosParent);					// 位置の設定
			pGauge->m_pGauge[nCntGauge]->SetSize(D3DXVECTOR3(fWidth, fHeight, 0.0f));	// サイズの設定
		}

		// 初期化処理
		pGauge->Init();
	}

	return pGauge;
}

//=========================================
// テクスチャを割り当てる
//=========================================
void CGauge3D::BindTexture(LPDIRECT3DTEXTURE9 pTexture, int nGauge)
{
	if (nGauge < m_nNumBar)
	{// バーの本数を超えないようにする
		m_pGauge[nGauge]->BindTexture(pTexture);
	}
}

//=============================================================================
// コンストラクタ
//=============================================================================
CGauge3D::CGauge3D(int nPriority, CScene::OBJTYPE objType) : CScene(nPriority, objType)
{
}

//=============================================================================
// デストラクタ
//=============================================================================
CGauge3D::~CGauge3D()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CGauge3D::Init(void)
{
	for (int nCntGauge = 0; nCntGauge < m_nNumBar; nCntGauge++)
	{// Zバッファの設定
		m_pGauge[nCntGauge]->SetZBuffer(true, D3DCMP_ALWAYS);
		m_pGauge[nCntGauge]->SetLighting(false);
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CGauge3D::Uninit(void)
{
	if (NULL != m_pGauge)
	{// 使われている
		for (int nCntGauge = 0; nCntGauge < m_nNumBar; nCntGauge++)
		{// バーの本数分ループ
			if (NULL != m_pGauge[nCntGauge])
			{// 使われているものを破棄
				m_pGauge[nCntGauge]->Uninit();
				m_pGauge[nCntGauge] = NULL;
			}
		}

		// 破棄
		delete m_pGauge;
		m_pGauge = NULL;
	}

	// 自分を破棄
	CScene::Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CGauge3D::Update(void)
{
	if (m_pGauge)
	{// 使われている
		if (m_nNumBar >= 3)
		{// バーの本数が３本以上のとき
			const float fWidth = m_pGauge[0]->GetWidth();	// 横幅の長さ
			int nBar = m_nNumBar - 2;						// 減らすバーの番号

			float fLife = m_pGauge[nBar]->GetWidth();				// バーの長さを取得
			float fGaugeView = fLife * (1.0f / (fWidth * 0.01f));	// バーの長さを％にする
			float fPercent = m_fGauge * (1.0f / (m_fMax * 0.01f));	// 値を％にする

			if (m_nStartReduce > m_nCntReduce)
			{// カウントする
				m_nCntReduce++;
			}

			if (m_nStartReduce <= m_nCntReduce)
			{// 一定時間経過
				if (fPercent < fGaugeView && fLife > -fWidth)
				{// ゲージが現在の値と同じになるまで減らす
					fLife -= fWidth * 0.01f;				// ゲージの長さを1％ずつ減らす
					m_pGauge[nBar]->SetWidth(fLife);		// バーの長さを設定
				}
				else if (fPercent > fGaugeView && fLife < fWidth)
				{// ゲージが現在の値と同じになるまで増やす
					fLife += fWidth * 0.01f;			// ゲージの長さを1％ずつ増やす
					m_pGauge[nBar]->SetWidth(fLife);		// バーの長さを設定
				}
			}
		}

		for (int nCntBar = 0; nCntBar < m_nNumBar; nCntBar++)
		{// 位置の設定
			m_pGauge[nCntBar]->SetPos(m_offset + *m_pPosParent);
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CGauge3D::Draw(void)
{

}

//=============================================================================
// ゲージの増減
//=============================================================================
const void CGauge3D::AddSubtract(float fValue)
{
	m_fGauge += fValue;		// ライフを増減させる
	float fLife;			// バーの長さの値
	const float fWidth = m_pGauge[0]->GetWidth();			// 横幅の長さ

	if (m_fGauge > 0)
	{// ライフが0より大きいとき
		fLife = fWidth / (m_fMax / m_fGauge);	// バーの元の長さを100%としてライフを増減させる
	}
	else
	{// ライフが0のとき
		fLife = 0;
	}

	if (m_fGauge >= m_fMax)
	{// 最大値を超えたら戻す
		m_fGauge = m_fMax;
		fLife = fWidth;
	}

	// バーの長さを更新
	m_pGauge[m_nNumBar - 1]->SetWidth(fLife);

	D3DXVECTOR3 pos = m_pGauge[m_nNumBar - 1]->GetPos();

	if (fValue > 0 && m_nNumBar > 2)
	{// 回復したとき後ろのバーも増やす
		m_pGauge[m_nNumBar - 2]->SetWidth(fLife);			// バーの長さを更新
	}

	m_nCntReduce = 0;						// ダメージバーを減らし始めるためのカウンタを初期化
}

//=============================================================================
// 色の設定
//=============================================================================
const void CGauge3D::SetColor(D3DXCOLOR col, int nBar)
{
	if (m_nNumBar > nBar)
	{// 使われている本数まで
		m_pGauge[nBar]->SetColor(col);
	}
}

//=============================================================================
// 色の取得
//=============================================================================
D3DXCOLOR CGauge3D::GetColor(int nBar)
{
	if (m_nNumBar > nBar)
	{// 使われている本数まで
		return m_pGauge[nBar]->GetColor();
	}

	// 使われていない本数が指定されたとき
	return D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
}
