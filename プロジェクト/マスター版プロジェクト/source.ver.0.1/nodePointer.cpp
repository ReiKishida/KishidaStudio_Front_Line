//=============================================================================
//
// ノードポインター処理 [mouseCursor.cpp]
// Author : Komatsu Keisuke
//
//=============================================================================
#include "nodePointer.h"
#include "manager.h"
#include "renderer.h"
#include "debugproc.h"
#include "input.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define POINT_SIZE_X			(18.0f)		// 仮地点用ポリゴンの大きさX
#define POINT_SIZE_Z			(17.0f)		// 仮地点用ポリゴンの大きさY

//=============================================================================
// 静的メンバ変数
//=============================================================================
D3DXVECTOR3 CNodePointer::m_SearchPos = {};
int CNodePointer::m_nNodeMax = 0;

//=============================================================================
// 生成処理
//=============================================================================
CNodePointer *CNodePointer::Create(D3DXVECTOR3 pos)
{
	CNodePointer *pNodePointer = NULL;

	if (pNodePointer == NULL)
	{
		pNodePointer = new CNodePointer;

		if (pNodePointer != NULL)
		{
			pNodePointer->m_nNumber = m_nNodeMax;
			pNodePointer->m_pos = pos;
			pNodePointer->Init();
		}
	}

	return pNodePointer;
}

//=============================================================================
// コンストラクタ
//=============================================================================
CNodePointer::CNodePointer(int nPriority, CScene::OBJTYPE objType) : CScene3D(nPriority, objType){}

//=============================================================================
// デストラクタ
//=============================================================================
CNodePointer::~CNodePointer(){}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CNodePointer::Init()
{	
	CScene3D::Init();

	// 初期設定
	m_size = D3DXVECTOR3(POINT_SIZE_X, 0.0f, POINT_SIZE_Z);
	m_color = D3DXCOLOR(1.0f, 0.5f, 0.8f, 0.5f);

	for (int nCntConnect = 0; nCntConnect <CONNECT_MAX; nCntConnect++)
	{// 接続可能最大数分回る
		m_nConnect[nCntConnect] = 0;
	}
	m_nConnectMax = 0;

	// ノードポインターの設定
	CScene3D::SetPos(m_pos);
	CScene3D::SetSize(m_size);
	CScene3D::SetColor(m_color);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CNodePointer::Uninit(void)
{
	CScene3D::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CNodePointer::Update(void)
{
	//CDebugProc::Print("ノードの位置 x : %.1f / y : %.1f / z : %.1f\n", m_pos.x, m_pos.y, m_pos.z);
	//CDebugProc::Print("No. : %d\n", m_nNumber);
	//CDebugProc::Print("接続ノード数 : %d\n", m_nConnectMax);
	//for (int nCntConnect = 0; nCntConnect < m_nConnectMax; nCntConnect++)
	//{
	//	CDebugProc::Print("接続先番号[%d] : %d\n", nCntConnect, m_nConnect[nCntConnect]);
	//}
	//CDebugProc::Print("\n");

	CScene3D::SetSize(m_size);
	CScene3D::SetColor(m_color);
}

//=============================================================================
// 描画処理
//=============================================================================
void CNodePointer::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// ライティングOFF
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// ZテストOFF
	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);

	CScene3D::Draw();

	// ZテストON
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);

	// ライティングON
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}