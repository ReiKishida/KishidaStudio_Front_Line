//=============================================================================
//
// エフェクト処理 [effect.cpp]
// Author : Ishida Takuto
//
//=============================================================================
#include "effect.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//==================================
// マクロ定義
//==================================

//==================================
// 静的メンバ変数宣言
//==================================

//==================================
// 生成処理
//==================================
CEffect* CEffect::Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fRange, int nLife)
{
	CEffect *pEffect = NULL;

	pEffect = new CEffect;		// メモリを確保

	if (NULL != pEffect)
	{// メモリが確保されているか確認
		pEffect->Init(pos, col, fRange, nLife);	// 初期化
	}
	return pEffect;
}

//=========================================
// コンストラクタ
//=========================================
CEffect::CEffect(int nPriority, CScene::OBJTYPE objType) : CScene3DBill(nPriority, objType)
{
	m_nLife = 0;
}

//=========================================
// デストラクタ
//=========================================
CEffect::~CEffect()
{
}

//=========================================
// 初期化処理
//=========================================
HRESULT CEffect::Init(D3DXVECTOR3 pos, D3DXCOLOR col, float fRange, int nLife)
{
	CScene3DBill::Init();

	// 位置の設定
	CScene3DBill::SetPos(pos);

	// 色の設定
	CScene3DBill::SetColor(col);

	// 大きさの設定
	CScene3DBill::SetSize(D3DXVECTOR3(fRange, fRange, 0.0f));
	m_nLife = nLife;

	CScene3DBill::BindTexture(CTexture::GetTexture(CTexture::TEXTURE_SHADOW));
	CScene3DBill::SetAddDraw(true);
	CScene3DBill::SetLighting(false);

	return S_OK;
}

//=========================================
// 終了処理
//=========================================
void CEffect::Uninit(void)
{
	CScene3DBill::Uninit();
}

//=========================================
// 更新処理
//=========================================
void CEffect::Update(void)
{
	m_nLife--;
	if (m_nLife <= 0)
	{// ライフがなくなったとき
		Uninit();		// 終了処理
	}
}

//=========================================
// 描画処理
//=========================================
void CEffect::Draw(void)
{
	// 描画処理
	CScene3DBill::Draw();
}
