//=============================================================================
//
// ピン立て処理 [pin.cpp]
// Author : Komatsu Keisuke
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "pin.h"
#include "manager.h"
#include "renderer.h"
#include "debugproc.h"
#include "input.h"
#include "texture.h"
#include "player.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define MOUSE_WIDTH		(20.0f)		// ピンテクスチャの幅
#define MOUSE_HEIGHT	(30.0f)		// ピンテクスチャの高さ

//=============================================================================
// 静的メンバ変数
//=============================================================================

//=============================================================================
// 生成処理
//=============================================================================
CPin  *CPin::Create(D3DXVECTOR3 pos)
{
	CPin *pPin = NULL;

	if (pPin == NULL)
	{
		pPin = new CPin;

		if (pPin != NULL)
		{// メモリ確保成功
			pPin->m_pos = pos;
			pPin->Init();
		}
	}

	return pPin;
}

//=============================================================================
// コンストラクタ
//=============================================================================
CPin::CPin(int nPriority, CScene::OBJTYPE objType) : CScene3DBill(nPriority, objType) {}

//=============================================================================
// デストラクタ
//=============================================================================
CPin::~CPin() {}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CPin::Init()
{
	m_size = D3DXVECTOR3(MOUSE_WIDTH, MOUSE_HEIGHT, 0.0f);
	m_pTexture = CTexture::GetTexture(CTexture::TEXTURE_GOAL_PIN);
	CScene3DBill::Init();

	// ピンの初期設定
	CScene3DBill::SetPos(m_pos);
	CScene3DBill::SetSize(m_size);
	CScene3DBill::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	CScene3DBill::BindTexture(m_pTexture);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CPin::Uninit(void)
{
	// ピンの破棄
	CScene3DBill::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CPin::Update(void)
{
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();	// キーボードの入力を取得
	CInputMouse *pMouse = CManager::GetInputMouse();			// マウスの入力を取得

	// 情報更新
	CScene3DBill::BindTexture(m_pTexture);
	CScene3DBill::SetPos(m_pos);
}

//=============================================================================
// 描画処理
//=============================================================================
void CPin::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// ライティングOFF
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// ZテストOFF
	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);

	// ピンの描画
	CScene3DBill::Draw();

	// ZテストON
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);

	// ライティングON
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}