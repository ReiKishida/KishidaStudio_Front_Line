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
#include "enemy.h"
#include "player.h"
#include "nodePointer.h"
#include "scene2D.h"
#include "texture.h"
#include "AI.h"
#include "menu.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define	LOAD_FILENAME	("data/TEXT/NODE_DATA/NodeData.txt")	// 読み込むファイルのパス
#define MOUSE_WIDTH		(30.0f)		// マウステクスチャの幅
#define MOUSE_HEIGHT	(30.0f)		// マウステクスチャの高さ

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
		{
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

	// マップ情報の初期化
	m_LoadNodeData = {};

	// 情報の初期化
	m_pCamera = CManager::GetCamera();
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_setpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR3(MOUSE_WIDTH, 0.0f, MOUSE_HEIGHT) * 3.0f;
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
	for (int nCntNode = 0; nCntNode < m_LoadNodeData.nodeMax; nCntNode++)
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
			m_pos.x -= m_pCamera->GetPosR().x * 5.0f;
			m_pos.z -= m_pCamera->GetPosR().z * 5.0f;
			break;
		case 2:
			m_pos.x -= m_pCamera->GetPosR().x * 2.0f;
			m_pos.z -= m_pCamera->GetPosR().z * 2.0f;
			break;
		case 3:
			m_pos.x -= m_pCamera->GetPosR().x;
			m_pos.z -= m_pCamera->GetPosR().z;
			break;
		}

		// 位置・サイズの設定
		CScene3D::SetMousePos(m_pos);
		CScene3D::SetSize((m_size / m_pCamera->GetZoom()));
	}

	if (CManager::GetGame()->GetPart() == CGame::PART_STRATEGY)
	{// ストラテジーパート時のみポジションを設定
		m_setpos = D3DXVECTOR3(((int)(m_pos.x / 38)) * 38.0f, 0.0f, ((int)(m_pos.z / 36)) * 36.0f);
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
		for (int nCntNode = 0; nCntNode < m_LoadNodeData.nodeMax; nCntNode++)
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
		CMouseCursor::FileLoad(LOAD_FILENAME);

		// ロードしたデータをもとにノードを配置
		for (int nCntNode = 0; nCntNode < m_LoadNodeData.nodeMax; nCntNode++)
		{// 全ノード数分回る
			if (m_pNodePointer[nCntNode] == NULL)
			{// ノード可視化用ポリゴンがNULLじゃない
				m_pNodePointer[nCntNode] = m_pNodePointer[nCntNode]->Create(m_LoadNodeData.pos[nCntNode]);
				m_pNodePointer[nCntNode]->GetNodeMax()++;
				m_LoadNodeData.index[nCntNode] = m_pNodePointer[nCntNode]->GetMyNumber();

				for (int nCntConnectNode = 0; nCntConnectNode < m_LoadNodeData.connectNum[nCntNode]; nCntConnectNode++)
				{// 接続ノード数分回る
					m_pNodePointer[nCntNode]->GetConnectMax()++;
					m_pNodePointer[nCntNode]->GetConnect(nCntConnectNode) = m_LoadNodeData.connectIndex[nCntNode][nCntConnectNode];
				}

				m_nNodeCounter = m_pNodePointer[nCntNode]->GetNodeMax();
			}
		}
	}

	if (pKeyboard->GetTrigger(DIK_F9) == true)
	{// 9キーで	可視化用ポリゴンを破棄
		for (int nCntNode = 0; nCntNode < m_LoadNodeData.nodeMax; nCntNode++)
		{// 全ノード数分回る
			if (m_pNodePointer[nCntNode] != NULL)
			{// 可視化用ポリゴンがNULLじゃない
				m_pNodePointer[nCntNode]->Uninit();
				m_pNodePointer[nCntNode] = NULL;
			}
		}
	}
}

//=============================================================================
// ファイルの読み込み
//=============================================================================
void CMouseCursor::FileLoad(char* pFileName)
{
	FILE* pFile = NULL;		// ファイルポインタ
	char ReadText[256];		// 読み込んだ文字列を入れておく
	char HeadText[256];		// 比較用
	char DustBox[256];		// 使用しないものを入れておく
	int nCount = 0;
	int nCntIndex = 0;

	// 一時データベース
	std::vector<NODE_LOAD_STATE> LoadState; LoadState.clear();

	// 初期化
	NODE_LOAD_STATE OneState = {};

	// ファイルオープン
	pFile = fopen(pFileName, "r");

	if (pFile != NULL)
	{// ファイルが開かれていれば
		while (strcmp(HeadText, "START_LOAD") != 0)
		{// "START_LOAD" が読み込まれるまで繰り返し文字列を読み取る
			fgets(ReadText, sizeof(ReadText), pFile);
			sscanf(ReadText, "%s", &HeadText);
		}
		if (strcmp(HeadText, "START_LOAD") == 0)
		{// "START_LOAD" が読み取れた場合、処理開始
			while (strcmp(HeadText, "END_LOAD") != 0)
			{// "END_LOAD" が読み込まれるまで繰り返し文字列を読み取る
				fgets(ReadText, sizeof(ReadText), pFile);
				sscanf(ReadText, "%s", &HeadText);

				if (strcmp(HeadText, "\n") == 0)
				{// 文字列の先頭が [\n](改行) の場合処理しない

				}
				else if (strcmp(HeadText, "START_DATA") == 0)
				{// "START_DATA" が読み取れた場合
					nCount = 0;
					while (strcmp(HeadText, "END_DATA") != 0)
					{// "END_DATA" が読み込まれるまで繰り返し文字列を読み取る
						fgets(ReadText, sizeof(ReadText), pFile);
						sscanf(ReadText, "%s", &HeadText);

						if (strcmp(HeadText, "\n") == 0)
						{// 文字列の先頭が [\n](改行) の場合処理しない

						}
						else if (strcmp(HeadText, "NODESET") == 0)
						{// "NODESET" が読み取れた場合
							while (strcmp(HeadText, "END_NODESET") != 0)
							{// "END_NODESET" が読み込まれるまで繰り返し文字列を読み取る
								fgets(ReadText, sizeof(ReadText), pFile);
								sscanf(ReadText, "%s", &HeadText);

								if (strcmp(HeadText, "\n") == 0)
								{// 文字列の先頭が [\n](改行) の場合処理しない

								}
								else if (strcmp(HeadText, "NODE_POS") == 0)	// ノードの位置
								{
									sscanf(ReadText, "%s %c %f %f %f",
										&DustBox, &DustBox,
										&OneState.pos[nCount].x,
										&OneState.pos[nCount].y,
										&OneState.pos[nCount].z);
								}
								else if (strcmp(HeadText, "CONNECT_NUM") == 0)	// 接続ノードの数
								{
									sscanf(ReadText, "%s %c %d",
										&DustBox, &DustBox,
										&OneState.connectNum[nCount]);
								}
								else if (strcmp(HeadText, "CONNECT_INDEX") == 0)	// 接続ノードの番号
								{
									sscanf(ReadText, "%s %c %d",
										&DustBox, &DustBox,
										&OneState.connectIndex[nCount][nCntIndex]);
									nCntIndex++;
								}
							}

							OneState.index[nCount] = nCount;
							nCntIndex = 0;
							nCount++;
						}
					}
					OneState.nodeMax = nCount; // ノードの総数

											   // 一つのデータを読み込んだ後,一時データベースに格納
					LoadState.emplace_back(OneState);
				}
			}
		}

		// ファイルクローズ
		if (pFile != NULL)
		{
			fclose(pFile);
			pFile = NULL;
		}
	}

	m_LoadNodeData = OneState;	// データの代入
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