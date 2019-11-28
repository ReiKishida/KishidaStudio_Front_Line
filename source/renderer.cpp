//=============================================================================
//
// レンダリング処理 [renderer.cpp]
// Author : Takuto Ishida
//
//=============================================================================
#include "renderer.h"
#include "scene.h"
#include "camera.h"
#include "debugProc.h"
#include "game.h"
#include "model.h"
#include "mouseCursor.h"
#include "button.h"

//==================================
// 静的メンバ変数宣言
//==================================

//=========================================
// コンストラクタ
//=========================================
CRenderer::CRenderer()
{
	m_pD3D = NULL;			// Direct3Dオブジェクト
	m_pD3DDevice = NULL;	// Deviceオブジェクト(描画に必要)
	m_hWnd = NULL;
	m_bWindowMode = true;

#ifdef _DEBUG
	m_pFont = NULL;			// フォントへのポインタ
#endif

}

//=========================================
// デストラクタ
//=========================================
CRenderer::~CRenderer()
{
}

//=========================================
// 初期化処理
//=========================================
HRESULT CRenderer::Init(HINSTANCE hInstance, HWND hWnd, bool bWindow)
{
	D3DPRESENT_PARAMETERS d3dpp;
	D3DDISPLAYMODE d3ddm;
	m_hWnd = hWnd;
	m_bWindowMode = bWindow;

	// Direct3Dオブジェクトの作成
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (m_pD3D == NULL)
	{
		return E_FAIL;
	}

	// 現在のディスプレイモードを取得
	if (FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));								// ワークをゼロクリア
	d3dpp.BackBufferCount = 1;							// バックバッファの数
	d3dpp.BackBufferWidth = SCREEN_WIDTH;				// ゲーム画面サイズ(幅)
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;				// ゲーム画面サイズ(高さ)
	d3dpp.BackBufferFormat = d3ddm.Format;				// カラーモードの指定
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;		// 映像信号に同期してフリップする
	d3dpp.EnableAutoDepthStencil = TRUE;				// デプスバッファ（Ｚバッファ）とステンシルバッファを作成
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;		// デプスバッファとして16bitを使う
	d3dpp.Windowed = m_bWindowMode;						// ウィンドウモード
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;		// リフレッシュレート
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;	// インターバル

	// デバイスの生成
	// ディスプレイアダプタを表すためのデバイスを作成
	// 描画と頂点処理をハードウェアで行なう
	if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		m_hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp, &m_pD3DDevice)))
	{
		// 上記の設定が失敗したら
		// 描画をハードウェアで行い、頂点処理はCPUで行なう
		if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			m_hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp, &m_pD3DDevice)))
		{
			// 上記の設定が失敗したら
			// 描画と頂点処理をCPUで行なう
			if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF, m_hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp, &m_pD3DDevice)))
			{
				// 生成失敗
				return E_FAIL;
			}
		}
	}

	// レンダーステートの設定
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				// 裏面をカリング
	m_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);						// Zバッファを使用
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// αブレンドを行う
	m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);						// ライティングモード有効
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// αソースカラーの指定
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// αデスティネーションカラーの指定

	// サンプラーステートの設定
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// テクスチャアドレッシング方法(U値)を設定
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// テクスチャアドレッシング方法(V値)を設定
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// テクスチャ縮小フィルタモードを設定
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// テクスチャ拡大フィルタモードを設定

	// テクスチャステージステートの設定
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);	// アルファブレンディング処理を設定
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);	// 最初のアルファ引数
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);	// ２番目のアルファ引数

#ifdef _DEBUG
	// デバッグ情報表示用フォントの生成
	D3DXCreateFont(m_pD3DDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &m_pFont);
#endif

	return S_OK;
}

//=========================================
// 終了処理
//=========================================
void CRenderer::Uninit(void)
{
#ifdef _DEBUG
	// デバッグ情報表示用フォントの破棄
	if (m_pFont != NULL)
	{
		m_pFont->Release();
		m_pFont = NULL;
	}
#endif

	// デバイスの破棄
	if (m_pD3DDevice != NULL)
	{
		m_pD3DDevice->Release();
		m_pD3DDevice = NULL;
	}

	// Direct3Dオブジェクトの破棄
	if (m_pD3D != NULL)
	{
		m_pD3D->Release();
		m_pD3D = NULL;
	}
}

//=========================================
// 更新処理
//=========================================
void CRenderer::Update(void)
{
	CScene::UpdateAll();
}

//=========================================
// 描画処理
//=========================================
void CRenderer::Draw(void)
{
	// バックバッファ＆Ｚバッファのクリア
	m_pD3DDevice->Clear(0,
		NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL),
		D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), 1.0f, 0);

	if (SUCCEEDED(m_pD3DDevice->BeginScene()))
	{// Direct3Dによる描画の開始
		CCamera *pCamera = NULL;			// カメラの情報を格納
		pCamera = CManager::GetCamera();	// カメラの情報を取得

		if (NULL != CManager::GetGame())
		{
			if (CGame::PART_ACTION == CManager::GetGame()->GetPart())
			{
				if (NULL != pCamera)
				{// ヌルチェック
					pCamera->Set(0);	// カメラのセット

					// 全てのオブジェクトの描画
					CScene::DrawAll();
				}
			}
			else if (CGame::PART_STRATEGY == CManager::GetGame()->GetPart())
			{// ストラテジー画面のみ
				pCamera->Set(0);	// カメラのセット

				CManager::GetGame()->GetMouse()->SetDisp(false);

				// 全てのオブジェクトの描画
				CScene::DrawAll();

				CManager::GetGame()->GetMouse()->SetDisp(true);

				pCamera->Set(1);

				m_pD3DDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
				m_pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
				m_pD3DDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_GREATEREQUAL);

				CManager::GetGame()->GetField()->Draw();

				pCamera->Set(0);	// カメラのセット
				CScene::DrawStrategy();
				CManager::GetGame()->GetMouseCursor()->Draw();

				pCamera->Set(1);
				CManager::GetGame()->GetButtonManager()->MarkerDraw();
				CManager::GetGame()->GetMouse()->Draw();

				m_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
				m_pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
				m_pD3DDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
			}
		}
		else
		{
			// 全てのオブジェクトの描画
			CScene::DrawAll();
		}

#ifdef _DEBUG
		pCamera->Set(0);	// カメラのセット
		// デバッグ表示
		CDebugProc::Draw();
#endif

		// Direct3Dによる描画の終了
		m_pD3DDevice->EndScene();
	}

	// バックバッファとフロントバッファの入れ替え
	m_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}
