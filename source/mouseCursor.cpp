//=============================================================================
//
// マウスカーソル処理 [mouseCursor.cpp]
// Author : Komatsu Keisuke
//
//=============================================================================
#include "mouseCursor.h"
#include "manager.h"
#include "renderer.h"
#include "debugproc.h"
#include "input.h"
#include "enemy.h"
#include "texture.h"
#include "camera.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define MOUSE_WIDTH		(30.0f)			// 幅
#define MOUSE_HEIGHT	(30.0f)			// 高さ

//=============================================================================
// 静的メンバ変数
//=============================================================================
D3DXVECTOR3 CMouseCursor::m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
D3DXVECTOR3 CMouseCursor2D::m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

//=============================================================================
// 生成処理
//=============================================================================
CMouseCursor  *CMouseCursor::Create()
{
	CMouseCursor *pMouseCursor = NULL;

	if (pMouseCursor == NULL)
	{
		pMouseCursor = new CMouseCursor;

		if (pMouseCursor != NULL)
		{
			pMouseCursor->Init();
		}
	}

	return pMouseCursor;
}

//=============================================================================
// コンストラクタ
//=============================================================================
CMouseCursor::CMouseCursor(int nPriority, CScene::OBJTYPE objType) : CScene3D(nPriority, objType){}

//=============================================================================
// デストラクタ
//=============================================================================
CMouseCursor::~CMouseCursor(){}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CMouseCursor::Init()
{
	CScene3D::Init();

	CScene3D::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	CScene3D::BindTexture(CTexture::GetTexture(CTexture::TEXTURE_MOUSECURSOR));

	m_cursorPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// 頂点情報の取得
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = CScene3D::GetVtxBuff();

	// 頂点情報を設定
	VERTEX_3D *pVtx;

	//頂点バッファをロックし、頂点データへのポインタを取得
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(MOUSE_WIDTH, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, -MOUSE_HEIGHT);
	pVtx[3].pos = D3DXVECTOR3(MOUSE_WIDTH, 0.0f, -MOUSE_HEIGHT);

	//頂点バッファをアンロック
	pVtxBuff->Unlock();

	// 頂点情報の設定
	CScene3D::SetVtxBuff(pVtxBuff);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CMouseCursor::Uninit(void)
{
	CScene3D::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CMouseCursor::Update(void)
{
	CInputMouse *pMouse = CManager::GetInputMouse();	// マウスの入力を取得

	CDebugProc::Print("=====マウス用3Dポリゴン======\n");
	CDebugProc::Print(" ポリゴンの座標\n");
	CDebugProc::Print("  x : %.1f\n  y : %.1f\n  z : %.1f\n", m_pos.x, m_pos.y, m_pos.z);
	CDebugProc::Print(" カーソルの座標\n");
	CDebugProc::Print("  x : %.1f\n  y : %.1f\n  z : %.1f\n", m_cursorPos.x, m_cursorPos.y, m_cursorPos.z);
	CDebugProc::Print("============================\n\n");

	if (CEnemy::GetPartSwitch() == true)
	{// ストラテジーパート時
		// 入力時処理
		Input(pMouse);
	}

	// 移動時処理
	Move(pMouse);
}

//=============================================================================
// 描画処理
//=============================================================================
void CMouseCursor::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// ライティングOFF
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// ZテストOFF
	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

	CScene3D::Draw();

	// ZテストON
	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

	// ライティングON
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//=============================================================================
// 入力処理
//=============================================================================
void CMouseCursor::Input(CInputMouse *pMouse)
{
	if (pMouse->GetTrigger(CInputMouse::DIMS_BUTTON_0) == true)
	{// 左クリック
		// 目的地設定
		CEnemy::SetSearchPos(m_pos);
	}
	if (pMouse->GetTrigger(CInputMouse::DIMS_BUTTON_1) == true)
	{// 右クリック
	}
}

//=============================================================================
// 移動処理
//=============================================================================
void CMouseCursor::Move(CInputMouse *pMouse)
{
	D3DXVECTOR3 move;	// 移動量

	// ウィンドウの矩形を取得
	RECT rect;
	GetWindowRect(CManager::GetRenderer()->GetHWnd(), &rect);

	// マウスカーソルに合わせて動かす
	move.x = ((float)(pMouse->GetPoint().x - rect.left) - ((rect.right - rect.left) / 2.0f));
	move.y = 0.0f;
	move.z = -((float)(pMouse->GetPoint().y - rect.top) - ((rect.bottom - rect.top) / 2.0f));

	// 移動量の代入
	m_pos = move;
	m_cursorPos.x = move.x;
	m_cursorPos.z = move.z;

	// 位置の設定
	CScene3D::SetPos(m_pos);
}

/****************************************************************/
/*					3Dマウスカーソルのクラス					*/
/****************************************************************/
//=============================================================================
// 生成処理
//=============================================================================
CMouseCursor2D  *CMouseCursor2D::Create()
{
	CMouseCursor2D *pMouseCursor = NULL;

	if (pMouseCursor == NULL)
	{
		pMouseCursor = new CMouseCursor2D;

		if (pMouseCursor != NULL)
		{
			pMouseCursor->Init();
		}
	}

	return pMouseCursor;
}

//=============================================================================
// コンストラクタ
//=============================================================================
CMouseCursor2D::CMouseCursor2D(int nPriority, CScene::OBJTYPE objType) : CScene2D(nPriority, objType) {}

//=============================================================================
// デストラクタ
//=============================================================================
CMouseCursor2D::~CMouseCursor2D() {}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CMouseCursor2D::Init()
{
	CScene2D::Init();

	CScene2D::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	CScene2D::BindTexture(CTexture::GetTexture(CTexture::TEXTURE_MOUSECURSOR));

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CMouseCursor2D::Uninit(void)
{
	CScene2D::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CMouseCursor2D::Update(void)
{
	CInputMouse *pMouse = CManager::GetInputMouse();	// マウスの入力を取得

	// 移動時処理
	Move(pMouse);
}

//=============================================================================
// 描画処理
//=============================================================================
void CMouseCursor2D::Draw(void)
{
	CScene2D::Draw();
}

//=============================================================================
// 移動処理
//=============================================================================
void CMouseCursor2D::Move(CInputMouse *pMouse)
{
	// ウィンドウの矩形を取得
	RECT rect;
	GetWindowRect(CManager::GetRenderer()->GetHWnd(), &rect);

	// さらにサイズ変更を考慮して、現在のサイズで補正（枠サイズ等あるので厳密ではない）
	m_pos.x = float(pMouse->GetPoint().x - rect.left) - 5.0f;
	m_pos.y = float(pMouse->GetPoint().y - rect.top) - 30.0f;

	// 位置の設定
	CScene2D::SetPos(m_pos);

	// 頂点情報の取得
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = CScene2D::GetVtxBuff();

	// 頂点情報を設定
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点データへのポインタを取得
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos = m_pos + D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = m_pos + D3DXVECTOR3(MOUSE_WIDTH, 0.0f, 0.0f);
	pVtx[2].pos = m_pos + D3DXVECTOR3(0.0f, MOUSE_HEIGHT, 0.0f);
	pVtx[3].pos = m_pos + D3DXVECTOR3(MOUSE_WIDTH, MOUSE_HEIGHT, 0.0f);

	//頂点バッファをアンロック
	pVtxBuff->Unlock();

	// 頂点情報の設定
	CScene2D::SetVtxBuff(pVtxBuff);
}