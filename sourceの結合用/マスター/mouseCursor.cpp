//=============================================================================
//
// マウスカーソル処理 [mouseCursor.cpp]
// Author : Komatsu Keisuke
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "mouseCursor.h"
#include "manager.h"
#include "renderer.h"
#include "debugproc.h"
#include "input.h"
#include "camera.h"
#include "game.h"
#include "player.h"
#include "nodePointer.h"
#include "scene2D.h"
#include "texture.h"
#include "AI.h"
#include "menu.h"
#include "nodeDataFiler.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define	LOAD_FILENAME	("data/TEXT/NODE_DATA/NodeData.txt")	// 読み込むファイルのパス
//#define	LOAD_FILENAME	("data/TEXT/NODE_DATA/NodeDataTutorial.txt")	// 読み込むファイルのパス
#define MOUSE_WIDTH		(30.0f)		// マウステクスチャの幅
#define MOUSE_HEIGHT	(30.0f)		// マウステクスチャの高さ
#define MOUSE_WIDTH_T	(15.0f)		// マウステクスチャの幅
#define MOUSE_HEIGHT_T	(15.0f)		// マウステクスチャの高さ

//=============================================================================
// 静的メンバ変数
//=============================================================================
CCamera *CMouseCursor::m_pCamera = NULL;
CNodePointer *CMouseCursor::m_pNodePointer[NODEPOINT_MAX] = {};
D3DXVECTOR3 CMouseCursor2D::m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

/*==============================================================*/
/*					3Dマウスカーソルのクラス					*/
/*                  Author : Komatsu Keisuke                    */
/*==============================================================*/
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
		{// メモリ確保成功
			pMouseCursor->Init();
		}
	}

	return pMouseCursor;
}

//=============================================================================
// コンストラクタ
//=============================================================================
CMouseCursor::CMouseCursor(int nPriority, CScene::OBJTYPE objType) : CScene3D(nPriority, objType) {}

//=============================================================================
// デストラクタ
//=============================================================================
CMouseCursor::~CMouseCursor() {}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CMouseCursor::Init()
{
	CScene3D::Init();

	// マップ情報の取得
	if (CManager::GetGame() != NULL)
	{
		m_pNodeData = CGame::GetNodeFiler();
		m_size = D3DXVECTOR3(MOUSE_WIDTH, 0.0f, MOUSE_HEIGHT) * 3.0f;
	}
	else if (CManager::GetTutorial() != NULL)
	{
		m_pNodeData = CTutorial::GetNodeFiler();
		m_size = D3DXVECTOR3(MOUSE_WIDTH_T, 0.0f, MOUSE_HEIGHT_T) * 3.0f;

	}
	// 情報の初期化
	m_pCamera = CManager::GetCamera();
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_setpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nNodeCounter = 0;

	// マウスカーソルの設定
	CScene3D::SetSize(m_size);
	CScene3D::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	CScene3D::BindTexture(CTexture::GetTexture(CTexture::TEXTURE_MOUSECURSOR));

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CMouseCursor::Uninit(void)
{
	for (int nCntNode = 0; nCntNode < m_pNodeData->GetLoadData().nodeMax; nCntNode++)
	{// 全ノード数分回る
		if (m_pNodePointer[nCntNode] != NULL)
		{// 可視化用ポリゴンがNULLじゃない
			m_pNodePointer[nCntNode]->Uninit();
			m_pNodePointer[nCntNode] = NULL;
		}
	}

	CScene3D::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CMouseCursor::Update(void)
{
	//CDebugProc::Print("=====マウス用3Dポリゴン======\n");
	//CDebugProc::Print("3Dマップ上のマウスの座標：%.2f %.2f", m_pos.x, m_pos.z);
	//CDebugProc::Print("スクリーン上のマウス座標：%.2f %.2f", (float)pMouse->GetPoint().x, (float)pMouse->GetPoint().y);
	//CDebugProc::Print("エディター上のマウスの座標：%.2f %.2f", m_setpos.x, m_setpos.z);

	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();	// キーボードの入力を取得
	CInputMouse *pMouse = CManager::GetInputMouse();	// マウスの入力を取得

	// ゲーム内マウス処理
	CMouseCursor::Mouse(pMouse);

#ifdef _DEBUG
	// ファイル関係処理
	CMouseCursor::File(pKeyboard);
#endif
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

	CScene3D::Draw();

	// ライティングON
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//=============================================================================
// ゲーム内マウス処理
//=============================================================================
void CMouseCursor::Mouse(CInputMouse *pMouse)
{
	D3DXVECTOR3 MousePos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DVIEWPORT9 viewport;
	D3DXMATRIX matrix, proj, view;

	if (m_pCamera != NULL)
	{// カメラがNullじゃない
		// スクリーン座標をワールド座標へ変換
		MousePos = D3DXVECTOR3((float)pMouse->GetPoint().x, (float)pMouse->GetPoint().y, 0.0f);
		viewport = m_pCamera->GetViewport(1);
		D3DXMatrixIdentity(&matrix);
		m_pCamera->GetInfo(&viewport, &proj, &view);
		D3DXVec3Unproject(&MousePos, &MousePos, &viewport, &proj, &view, &matrix);

		// カーソル位置の設定
		m_pos.x = MousePos.x * (PARALLEL_PROJECTION_SIZE / m_pCamera->GetZoom());
		m_pos.y = 0.0f;
		m_pos.z = MousePos.z * (PARALLEL_PROJECTION_SIZE / m_pCamera->GetZoom());

		switch ((int)m_pCamera->GetZoom())
		{
		case 1:
			m_pos.x -= m_pCamera->GetPosR().x * ((PARALLEL_PROJECTION_SIZE) - 1.0f);
			m_pos.z -= m_pCamera->GetPosR().z * ((PARALLEL_PROJECTION_SIZE) - 1.0f);
			break;
		case 2:
			m_pos.x -= m_pCamera->GetPosR().x * ((PARALLEL_PROJECTION_SIZE / 2.0f) - 1.0f);
			m_pos.z -= m_pCamera->GetPosR().z * ((PARALLEL_PROJECTION_SIZE / 2.0f) - 1.0f);
			break;
		case 3:
			m_pos.x -= m_pCamera->GetPosR().x * ((PARALLEL_PROJECTION_SIZE / 3.0f) - 1.0f);
			m_pos.z -= m_pCamera->GetPosR().z * ((PARALLEL_PROJECTION_SIZE / 3.0f) - 1.0f);
			break;
		}

		// 位置・サイズの設定
		CScene3D::SetMousePos(m_pos);
		CScene3D::SetSize(((m_size * 1.1f) / m_pCamera->GetZoom()));
	}

	if (NULL != CManager::GetGame())
	{
		if (CManager::GetGame()->GetPart() == CGame::PART_STRATEGY)
		{// ストラテジーパート時のみポジションを設定
			m_setpos = D3DXVECTOR3(((int)(m_pos.x / 38)) * 38.0f, 0.0f, ((int)(m_pos.z / 36)) * 36.0f);
		}
	}
	else if (NULL != CManager::GetTutorial())
	{
		if (CManager::GetTutorial()->GetPart() == CGame::PART_STRATEGY)
		{// ストラテジーパート時のみポジションを設定
			m_setpos = D3DXVECTOR3(((int)(m_pos.x / 38)) * 38.0f, 0.0f, ((int)(m_pos.z / 36)) * 36.0f);
		}
	}
}

//=============================================================================
// ファイル関係処理
//=============================================================================
void CMouseCursor::File(CInputKeyboard *pKeyboard)
{
	bool bSet = false;

	if (pKeyboard->GetTrigger(DIK_F11) == true)
	{// F11キーでファイルをロード
		// 前回のデータの削除
		for (int nCntNode = 0; nCntNode < m_pNodeData->GetLoadData().nodeMax; nCntNode++)
		{// 全ノード数分回る
			if (m_pNodePointer[nCntNode] != NULL)
			{// データが入っている場合
				m_nNodeCounter = 0;
				m_pNodePointer[nCntNode]->GetNodeMax() = 0;
				m_pNodePointer[nCntNode]->GetConnectMax() = 0;
				m_pNodePointer[nCntNode]->Uninit();
				m_pNodePointer[nCntNode] = NULL;
			}
		}

		// ファイルロ―ド
		m_pNodeData->FileLoad(LOAD_FILENAME);

		// ロードしたデータをもとにノードを配置
		for (int nCntNode = 0; nCntNode < m_pNodeData->GetLoadData().nodeMax; nCntNode++)
		{// 全ノード数分回る
			if (m_pNodePointer[nCntNode] == NULL)
			{// ノード可視化用ポリゴンがNULLじゃない
				m_pNodePointer[nCntNode] = m_pNodePointer[nCntNode]->Create(m_pNodeData->GetLoadData().pos[nCntNode]);
				m_pNodePointer[nCntNode]->GetNodeMax()++;
				m_pNodeData->GetLoadData().index[nCntNode] = m_pNodePointer[nCntNode]->GetMyNumber();

				for (int nCntConnectNode = 0; nCntConnectNode < m_pNodeData->GetLoadData().connectNum[nCntNode]; nCntConnectNode++)
				{// 接続ノード数分回る
					m_pNodePointer[nCntNode]->GetConnectMax()++;
					m_pNodePointer[nCntNode]->GetConnect(nCntConnectNode) = m_pNodeData->GetLoadData().connectIndex[nCntNode][nCntConnectNode];
				}

				m_nNodeCounter = m_pNodePointer[nCntNode]->GetNodeMax();
			}
		}
	}

	if (pKeyboard->GetTrigger(DIK_F9) == true)
	{// 9キーで	可視化用ポリゴンを破棄
		for (int nCntNode = 0; nCntNode < m_pNodeData->GetLoadData().nodeMax; nCntNode++)
		{// 全ノード数分回る
			if (m_pNodePointer[nCntNode] != NULL)
			{// 可視化用ポリゴンがNULLじゃない
				m_pNodePointer[nCntNode]->Uninit();
				m_pNodePointer[nCntNode] = NULL;
			}
		}
	}
}

/****************************************************************/
/*					2Dマウスカーソルのクラス					*/
/*                  Author : Takuto Ishida                      */
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
	m_pos.x = float(pMouse->GetPoint().x - rect.left) - (MOUSE_WIDTH / 2.0f);
	m_pos.y = float(pMouse->GetPoint().y - rect.top) - (MOUSE_HEIGHT / 2.0f);

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