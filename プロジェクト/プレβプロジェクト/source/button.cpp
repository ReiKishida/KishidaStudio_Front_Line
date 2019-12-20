//=============================================================================
//
// ボタン処理 [button.cpp]
// Author : Takuto Ishida
//
//=============================================================================
#include "button.h"
#include "manager.h"
#include "renderer.h"
#include "debugProc.h"
#include "input.h"
#include "texture.h"
#include "game.h"
#include "mouseCursor.h"
#include "fade.h"
#include "camera.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define BUTTON_UNSELECTED_COLOR		(D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f))		// 非選択時の色
#define BUTTON_LOGIC_WIDTH			(80.0f)									// ロジックツリーボタンの幅
#define BUTTON_LOGIC_HEIGHT			(80.0f)									// ロジックツリーボタンの高さ
#define BUTTON_LINE_THICK			(2.5f)									// 線の太さ

#define BUTTON_NUM_FIRST_ICON		(2)
#define BUTTON_NUM_SECOND_ICON		(2)
#define BUTTON_NUM_THIRD_ICON		(2)
#define BUTTON_NUM_FORTH_ICON		(2)

//==================================
// 静的メンバ変数宣言
//==================================

/****************************************************************/
/*						2Dボタンのクラス						*/
/****************************************************************/
//==================================
// 生成処理
//==================================
CButton2D* CButton2D::Create(D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	CButton2D *pButton;

	pButton = new CButton2D;	// メモリを確保

	if (NULL != pButton)
	{// メモリ確保成功
		pButton->Init(pos, fWidth, fHeight);	// 初期化処理
	}

	return pButton;
}

//=========================================
// コンストラクタ
//=========================================
CButton2D::CButton2D(int nPriority, CScene::OBJTYPE objType) : CScene2D(nPriority, objType)
{
	m_bInRange = false;
	m_bSwitch = false;
	m_bRelease = false;
	m_size = D3DXVECTOR2(0.0f, 0.0f);
}

//=========================================
// デストラクタ
//=========================================
CButton2D::~CButton2D()
{
}

//=========================================
// 初期化処理
//=========================================
HRESULT CButton2D::Init(D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	// ポリゴンの設定
	CScene2D::Init();
	CScene2D::SetPos(pos);
	CScene2D::SetSize(fWidth, fHeight);
	CScene2D::SetColor(BUTTON_UNSELECTED_COLOR);
	m_size = D3DXVECTOR2(fWidth, fHeight);
	return S_OK;
}

//=========================================
// 終了処理
//=========================================
void CButton2D::Uninit(void)
{
	CScene2D::Uninit();
}

//=========================================
// 更新処理
//=========================================
void CButton2D::Update(void)
{
}

//=========================================
// 描画処理
//=========================================
void CButton2D::Draw(void)
{
	CScene2D::Draw();
}

//=========================================
// 範囲内かチェック
//=========================================
bool CButton2D::InRange(D3DXVECTOR3 _pos)
{
	if (CFade::GetFade() != CFade::FADE_NONE) { return false; }

	D3DXVECTOR3 pos = CScene2D::GetPos();

	if (pos.x - (m_size.x * 0.5f) <= _pos.x && pos.x + (m_size.x * 0.5f) >= _pos.x)
	{// X軸が範囲内
		if (pos.y - (m_size.y * 0.5f) <= _pos.y && pos.y + (m_size.y * 0.5f) >= _pos.y)
		{// Y軸が範囲内
			m_bInRange = true;
			CScene2D::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			return m_bInRange;
		}
	}

	if (m_bRelease)
	{// クリックされたが起動せずに範囲外に出た
		CScene2D::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		CScene2D::SetSize(m_size.x, m_size.y);
	}

	// 起動していない
	if (!m_bSwitch) { CScene2D::SetColor(BUTTON_UNSELECTED_COLOR); }

	// フラグを負にする
	m_bInRange = false;
	m_bRelease = false;
	return m_bInRange;
}

//=========================================
// 範囲内かチェック（メニュー、機体選択で使用）
//=========================================
bool CButton2D::InRangeMenu(D3DXVECTOR3 _pos)
{
	if (CFade::GetFade() != CFade::FADE_NONE) { return false; }

	D3DXVECTOR3 pos = CScene2D::GetPos();

	if (pos.x - (m_size.x * 0.4f) <= _pos.x && pos.x + (m_size.x * 0.4f) >= _pos.x)
	{// X軸が範囲内
		if (pos.y - (m_size.y * 0.4f) <= _pos.y && pos.y + (m_size.y * 0.4f) >= _pos.y)
		{// Y軸が範囲内
			m_bInRange = true;
			CScene2D::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			return m_bInRange;
		}
	}

	if (m_bRelease)
	{// クリックされたが起動せずに範囲外に出た
		CScene2D::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		CScene2D::SetSize(m_size.x, m_size.y);
	}

	// 起動していない
	if (!m_bSwitch) { CScene2D::SetColor(BUTTON_UNSELECTED_COLOR); }

	// フラグを負にする
	m_bInRange = false;
	m_bRelease = false;
	return m_bInRange;
}

//=========================================
// 選択の設定
//=========================================
bool CButton2D::Click(void)
{
	if (!m_bInRange) { return false; }	// 範囲外だったら抜ける

	CInputMouse *pMouse = CManager::GetInputMouse();	// マウスの入力を取得

	if (pMouse->GetTrigger(CInputMouse::DIMS_BUTTON_0))
	{// 左クリックされた
		m_bSwitch = m_bSwitch ? false : true;
		return true;
	}

	return false;
}

//=========================================
// 選択の設定
//=========================================
bool CButton2D::ClickRelease(void)
{
	if (!m_bInRange) { return false; }	// 範囲外だったら抜ける

	CInputMouse *pMouse = CManager::GetInputMouse();	// マウスの入力を取得

	if (pMouse->GetTrigger(CInputMouse::DIMS_BUTTON_0))
	{// 左クリックされた
		m_bRelease = true;
		CScene2D::SetSize(CScene2D::GetWidth() * 0.9f, CScene2D::GetHeight() * 0.9f);
	}

	if (m_bRelease)
	{// ボタンが押されている間
		CScene2D::SetColor(D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f));
	}

	if (pMouse->GetRelease(CInputMouse::DIMS_BUTTON_0) && m_bRelease)
	{// 左クリックを離した
		m_bSwitch = m_bSwitch ? false : true;
		CScene2D::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		CScene2D::SetSize(m_size.x, m_size.y);
		m_bRelease = false;
		return true;
	}

	return false;
}

/****************************************************************/
/*						3Dボタンのクラス						*/
/****************************************************************/
//==================================
// 生成処理
//==================================
CButton3D* CButton3D::Create(D3DXVECTOR3 pos, float fWidth, float fDepth)
{
	CButton3D *pButton;

	pButton = new CButton3D;	// メモリを確保

	if (NULL != pButton)
	{// メモリ確保成功
		pButton->Init(pos, fWidth, fDepth);	// 初期化処理
	}

	return pButton;
}

//=========================================
// コンストラクタ
//=========================================
CButton3D::CButton3D(int nPriority, CScene::OBJTYPE objType) : CScene3D(nPriority, objType)
{
	m_bInRange = false;
	m_bSwitch = false;
	m_bRelease = false;
	m_size = D3DXVECTOR2(0.0f, 0.0f);
	m_bDisp = true;
}

//=========================================
// デストラクタ
//=========================================
CButton3D::~CButton3D()
{
}

//=========================================
// 初期化処理
//=========================================
HRESULT CButton3D::Init(D3DXVECTOR3 pos, float fWidth, float fDepth)
{
	// ポリゴンの設定
	CScene3D::Init();
	CScene3D::SetPos(pos);
	CScene3D::SetSize(D3DXVECTOR3(fWidth,0.0f,fDepth));
	CScene3D::SetColor(BUTTON_UNSELECTED_COLOR);
	m_size = D3DXVECTOR2(fWidth, fDepth);
	return S_OK;
}

//=========================================
// 終了処理
//=========================================
void CButton3D::Uninit(void)
{
	CScene3D::Uninit();
}

//=========================================
// 更新処理
//=========================================
void CButton3D::Update(void)
{
}

//=========================================
// 描画処理
//=========================================
void CButton3D::Draw(void)
{
	// 非表示の場合描画しない
	if (!m_bDisp) { return; }

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// ライティングOFF
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// ZテストOFF
	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);

	CScene3D::Draw();

	// ZテストON
	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

	// ライティングON
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//=========================================
// 範囲内かチェック
//=========================================
bool CButton3D::InRange(D3DXVECTOR3 _pos)
{
	D3DXVECTOR3 pos = CScene3D::GetPos();

	if (pos.x - m_size.x <= _pos.x && pos.x + m_size.x >= _pos.x)
	{// x軸が範囲内
		if (pos.z - m_size.y <= _pos.z && pos.z + m_size.y >= _pos.z)
		{// Z軸が範囲内
			m_bInRange = true;
			CScene3D::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			return m_bInRange;
		}
	}

	if (m_bRelease)
	{// クリックされたが起動せずに範囲外に出た
		CScene3D::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		CScene3D::SetSize(D3DXVECTOR3(m_size.x, 0.0f, m_size.y));
	}

	// 起動していない
	if (!m_bSwitch) { CScene3D::SetColor(BUTTON_UNSELECTED_COLOR); }

	// フラグを負にする
	m_bInRange = false;
	m_bRelease = false;
	return m_bInRange;
}

//=========================================
// 選択の設定
//=========================================
bool CButton3D::Click(void)
{
	if (!m_bInRange) { return false; }	// 範囲外だったら抜ける

	CInputMouse *pMouse = CManager::GetInputMouse();	// マウスの入力を取得

	if (pMouse->GetTrigger(CInputMouse::DIMS_BUTTON_0))
	{// 左クリックされた
		m_bSwitch = m_bSwitch ? false : true;
		return true;
	}

	return false;
}

//=========================================
// 選択の設定
//=========================================
bool CButton3D::ClickRelease(void)
{
	if (!m_bInRange) { return false; }	// 範囲外だったら抜ける

	CInputMouse *pMouse = CManager::GetInputMouse();	// マウスの入力を取得

	if (pMouse->GetTrigger(CInputMouse::DIMS_BUTTON_0))
	{// 左クリックされた
		m_bRelease = true;
		CScene3D::SetSize(CScene3D::GetSize() * 0.9f);
	}

	if (m_bRelease)
	{// ボタンが押されている間
		CScene3D::SetColor(D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f));
	}

	if (pMouse->GetRelease(CInputMouse::DIMS_BUTTON_0) && m_bRelease)
	{// 左クリックを離した
		m_bSwitch = m_bSwitch ? false : true;
		CScene3D::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		CScene3D::SetSize(D3DXVECTOR3(m_size.x, 0.0f, m_size.y));
		m_bRelease = false;
		return true;
	}

	return false;
}

/****************************************************************/
/*						ボタンの線のクラス						*/
/****************************************************************/
//==================================
// 生成処理
//==================================
CButtonLine* CButtonLine::Create(D3DXVECTOR3 start)
{
	CButtonLine *pButtonLine;

	pButtonLine = new CButtonLine;	// メモリを確保

	if (NULL != pButtonLine)
	{// メモリ確保成功
		pButtonLine->Init(start);	// 初期化処理
	}

	return pButtonLine;
}

//=========================================
// コンストラクタ
//=========================================
CButtonLine::CButtonLine(int nPriority, CScene::OBJTYPE objType) : CScene2D(nPriority, objType)
{
	m_bLink = false;
}

//=========================================
// デストラクタ
//=========================================
CButtonLine::~CButtonLine()
{
}

//=========================================
// 初期化処理
//=========================================
HRESULT CButtonLine::Init(D3DXVECTOR3 start)
{
	// ポリゴンの設定
	CScene2D::Init();

	// 位置の設定
	CScene2D::SetPos(start);

	m_start = start;

	return S_OK;
}

//=========================================
// 終了処理
//=========================================
void CButtonLine::Uninit(void)
{
	CScene2D::Uninit();
}

//=========================================
// 更新処理
//=========================================
void CButtonLine::Update(void)
{
	// 接続しているときは抜ける
	if (m_bLink) { return; }

	// マウスカーソルの位置を取得
	D3DXVECTOR3 cursor = CManager::GetGame()->GetMouseCursor()->GetPos();

	// カーソル位置への角度を計算
	float fAngle = atan2f(cursor.x - m_start.x, cursor.y - m_start.y);

	// 開始位置からカーソル位置への距離を計算
	float fLength = sqrtf(powf(cursor.x - m_start.x, 2.0f) + powf(cursor.y - m_start.y, 2.0f));

	D3DXVECTOR3 posVtx[4];
	posVtx[0] = D3DXVECTOR3(-BUTTON_LINE_THICK, 0.0f, 0.0f);
	posVtx[1] = D3DXVECTOR3(BUTTON_LINE_THICK, 0.0f, 0.0f);
	posVtx[2] = D3DXVECTOR3(-BUTTON_LINE_THICK, fLength, 0.0f);
	posVtx[3] = D3DXVECTOR3(BUTTON_LINE_THICK, fLength, 0.0f);

	// 頂点座標の設定
	CScene2D::SetPosVtx(posVtx);

	// 角度の設定
	CScene2D::SetRot(-fAngle);
}

//=========================================
// 描画処理
//=========================================
void CButtonLine::Draw(void)
{
	CScene2D::Draw();
}

//=========================================
// 接続したとき
//=========================================
void CButtonLine::Link(D3DXVECTOR3 end)
{
	m_bLink = true;

	// カーソル位置への角度を計算
	float fAngle = atan2f(end.x - m_start.x, end.y - m_start.y);

	// 開始位置からカーソル位置への距離を計算
	float fLength = sqrtf(powf(end.x - m_start.x, 2.0f) + powf(end.y - m_start.y, 2.0f));

	D3DXVECTOR3 posVtx[4];
	posVtx[0] = D3DXVECTOR3(-BUTTON_LINE_THICK, 0.0f, 0.0f);
	posVtx[1] = D3DXVECTOR3(BUTTON_LINE_THICK, 0.0f, 0.0f);
	posVtx[2] = D3DXVECTOR3(-BUTTON_LINE_THICK, fLength, 0.0f);
	posVtx[3] = D3DXVECTOR3(BUTTON_LINE_THICK, fLength, 0.0f);

	// 頂点座標の設定
	CScene2D::SetPosVtx(posVtx);

	// 角度の設定
	CScene2D::SetRot(-fAngle);
}

/****************************************************************/
/*						ボタン管理クラス						*/
/****************************************************************/
//==================================
// 生成処理
//==================================
CButtonManagerStrategy* CButtonManagerStrategy::Create(void)
{
	CButtonManagerStrategy *pButton;

	pButton = new CButtonManagerStrategy;	// メモリを確保

	if (NULL != pButton)
	{// メモリ確保成功
		pButton->Init();	// 初期化処理
	}

	return pButton;
}

//=========================================
// コンストラクタ
//=========================================
CButtonManagerStrategy::CButtonManagerStrategy(int nPriority, CScene::OBJTYPE objType) : CScene(nPriority, objType)
{
	for (int nCntAI = 0; nCntAI < 2; nCntAI++)
	{
		m_pAIType[nCntAI] = NULL;
	}
	m_pFirst = NULL;
	m_pSecond = NULL;
	m_pThird = NULL;
	m_pFourth = NULL;
	m_nSelectAIType = -1;
	m_bFinish = false;
	m_bSelect = true;
	m_pMarker = NULL;
	m_nSetNumMarker = 0;

	for (int nCntHierarchy = 0; nCntHierarchy < 4; nCntHierarchy++)
	{
		m_aNumLogic[nCntHierarchy] = 0;
		m_aSelectLogic[nCntHierarchy] = -1;
		m_pLine[nCntHierarchy] = NULL;
		m_apSelectIcon[nCntHierarchy] = NULL;
	}

	m_bDisp = false;
}

//=========================================
// デストラクタ
//=========================================
CButtonManagerStrategy::~CButtonManagerStrategy()
{
}

//=========================================
// 初期化処理
//=========================================
HRESULT CButtonManagerStrategy::Init(void)
{
	// ドローンとウォーカーのボタンの生成
	m_pAIType[0] = CButton2D::Create(D3DXVECTOR3(405.0f, 370.0f, 0.0f), 150.0f, 150.0f);
	m_pAIType[1] = CButton2D::Create(D3DXVECTOR3(405.0f, 570.0f, 0.0f), 150.0f, 150.0f);

	m_pAIType[0]->SetColor(BUTTON_UNSELECTED_COLOR);
	m_pAIType[1]->SetColor(BUTTON_UNSELECTED_COLOR);

	m_pAIType[0]->BindTexture(CTexture::GetTexture(CTexture::TEXTURE_DRONE_BUTTON));
	m_pAIType[1]->BindTexture(CTexture::GetTexture(CTexture::TEXTURE_WORKER_BUTTON));

	return S_OK;
}

//=========================================
// 終了処理
//=========================================
void CButtonManagerStrategy::Uninit(void)
{
	if (m_pFirst != NULL)
	{// １番目の階層の破棄
		for (int nCntLogic = 0; nCntLogic < m_aNumLogic[0]; nCntLogic++)
		{// 使われている数分ループ
			if (m_pFirst[nCntLogic] != NULL)
			{// 破棄
				m_pFirst[nCntLogic]->Uninit();
				m_pFirst[nCntLogic] = NULL;
			}
		}

		// 破棄
		delete[] m_pFirst;
		m_pFirst = NULL;
	}

	if (m_pSecond != NULL)
	{// ２番目の階層の破棄
		for (int nCntLogic = 0; nCntLogic < m_aNumLogic[1]; nCntLogic++)
		{// 使われている数分ループ
			if (m_pSecond[nCntLogic] != NULL)
			{// 破棄
				m_pSecond[nCntLogic]->Uninit();
				m_pSecond[nCntLogic] = NULL;
			}
		}

		// 破棄
		delete[] m_pSecond;
		m_pSecond = NULL;
	}

	if (m_pThird != NULL)
	{// ３番目の階層の破棄
		for (int nCntLogic = 0; nCntLogic < m_aNumLogic[2]; nCntLogic++)
		{// 使われている数分ループ
			if (m_pThird[nCntLogic] != NULL)
			{// 破棄
				m_pThird[nCntLogic]->Uninit();
				m_pThird[nCntLogic] = NULL;
			}
		}

		// 破棄
		delete[] m_pThird;
		m_pThird = NULL;
	}

	if (m_pFourth != NULL)
	{// ４番目の階層の破棄
		for (int nCntLogic = 0; nCntLogic < m_aNumLogic[3]; nCntLogic++)
		{// 使われている数分ループ
			if (m_pFourth[nCntLogic] != NULL)
			{// 破棄
				m_pFourth[nCntLogic]->Uninit();
				m_pFourth[nCntLogic] = NULL;
			}
		}

		// 破棄
		delete[] m_pFourth;
		m_pFourth = NULL;
	}

	for (int nCntAI = 0; nCntAI < 2; nCntAI++)
	{// AI機体の種類分ループ
		if (m_pAIType[nCntAI] != NULL)
		{// ボタンの破棄
			m_pAIType[nCntAI]->Uninit();
			m_pAIType[nCntAI] = NULL;
		}
	}

	for (int nCntHierarchy = 0; nCntHierarchy < 4; nCntHierarchy++)
	{// 階層分ループ
		if (m_pLine[nCntHierarchy] != NULL)
		{// 線の破棄
			m_pLine[nCntHierarchy]->Uninit();
			m_pLine[nCntHierarchy] = NULL;
		}

		if (m_apSelectIcon[nCntHierarchy] != NULL)
		{// 選択した項目の破棄
			m_apSelectIcon[nCntHierarchy]->Uninit();
			m_apSelectIcon[nCntHierarchy] = NULL;
		}
	}

	if (m_pMarker != NULL)
	{
		int nNumMarker;		// マーカーの数
		if (2 == m_aSelectLogic[2]) { nNumMarker = 4; }
		else { nNumMarker = 1; }

		for (int nCntMarker = 0; nCntMarker < nNumMarker; nCntMarker++)
		{// マーカーの数分ループ
			if (m_pMarker[nCntMarker] != NULL)
			{// マーカーの破棄
				m_pMarker[nCntMarker]->Uninit();
				m_pMarker[nCntMarker] = NULL;
			}
		}

		// マーカーの破棄
		delete[] m_pMarker;
		m_pMarker = NULL;
	}

	// 自分を破棄
	CScene::Release();
}

//=========================================
// 更新処理
//=========================================
void CButtonManagerStrategy::Update(void)
{
	if (m_bSelect)
	{
		// ドローンかウォーカーの選択処理
		AITypeSelect();

		if (NULL != m_pFirst)
		{// １階層目の処理
			FirstHierarchy();
		}

		if (NULL != m_pSecond)
		{// ２階層目の処理
			SecondHierarchy();
		}

		if (NULL != m_pThird)
		{// ３階層目の処理
			ThirdHierarchy();
		}

		if (NULL != m_pFourth)
		{// ４階層目の処理
			FourthHierarchy();
		}
	}

	if (m_bFinish && NULL == m_pMarker)
	{// マーカーの生成
		int nNumMarker = 0;		// マーカーの数
		if (2 == m_aSelectLogic[2]) { nNumMarker = 4; }
		else { nNumMarker = 1; }

		// マーカーを必要な分生成
		m_pMarker = new CScene3D*[nNumMarker];
		for (int nCntMarker = 0; nCntMarker < nNumMarker; nCntMarker++)
		{
			m_pMarker[nCntMarker] = CScene3D::Create();
			m_pMarker[nCntMarker]->SetBoolLighting(false);
		}
	}

	if (NULL != m_pMarker)
	{// マーカーの設置
		int nNumMarker;		// マーカーの数
		if (2 == m_aSelectLogic[2]) { nNumMarker = 4; }
		else { nNumMarker = 1; }

		for (int nCntMarker = 0; nCntMarker < nNumMarker; nCntMarker++)
		{// ズーム倍率に応じてサイズ変更
			m_pMarker[nCntMarker]->SetSize(D3DXVECTOR3(40.0f, 0.0f, 40.0f) / CManager::GetCamera()->GetZoom());
		}

		CInputMouse *pMouse = CManager::GetInputMouse();

		if (pMouse->GetTrigger(CInputMouse::DIMS_BUTTON_0))
		{// 左クリックされた

			if (nNumMarker > m_nSetNumMarker)
			{// マーカーを置く
				m_pMarker[m_nSetNumMarker]->SetPos(CManager::GetGame()->GetMouse()->GetPos());

				m_nSetNumMarker++;
			}
			else
			{
				m_pMarker[m_nSetNumMarker - 1]->SetPos(CManager::GetGame()->GetMouse()->GetPos());
			}
		}
	}

	CDebugProc::Print("選択ボタン：%d %d %d %d", m_aSelectLogic[0], m_aSelectLogic[1], m_aSelectLogic[2], m_aSelectLogic[3]);
}

//=========================================
// 描画処理
//=========================================
void CButtonManagerStrategy::Draw(void)
{
}

//=========================================
// マーカーの描画処理
//=========================================
void CButtonManagerStrategy::MarkerDraw(void)
{
	for (int nCntMarker = 0; nCntMarker < m_nSetNumMarker; nCntMarker++)
	{
		m_pMarker[nCntMarker]->Draw();
	}
}

//=========================================
// ドローンかウォーカーの選択処理
//=========================================
void CButtonManagerStrategy::AITypeSelect(void)
{
	for (int nCntAIType = 0; nCntAIType < 2; nCntAIType++)
	{// ドローンとウォーカーを両方チェック
		if (m_pAIType[nCntAIType]->InRange(CManager::GetGame()->GetMouseCursor()->GetPos()))
		{// 範囲内かチェック
			if (m_pAIType[nCntAIType]->ClickRelease())
			{// クリックされた
				ButtonUninit(4);	// ボタンの破棄

				if (m_pAIType[nCntAIType]->GetSwitch())
				{// 起動時
					m_pAIType[(nCntAIType + 1) % 2]->SetSwitch(false);

					m_nSelectAIType = nCntAIType;

					m_aNumLogic[0] = 2;		// １階層目のボタンの数
					m_pFirst = new CButton2D*[m_aNumLogic[0]];
					for (int nCntButton = 0; nCntButton < m_aNumLogic[0]; nCntButton++)
					{// ボタンの生成
						float posY = (480.0f + (nCntButton * BUTTON_LOGIC_HEIGHT * (5 - m_aNumLogic[0]))) - (BUTTON_LOGIC_HEIGHT * (5 - m_aNumLogic[0]) * (m_aNumLogic[0] / 2.0f)) + BUTTON_LOGIC_HEIGHT * (5 - m_aNumLogic[0]) / 2.0f;
						m_pFirst[nCntButton] = CButton2D::Create(D3DXVECTOR3(580.0f, posY, 0.0f), BUTTON_LOGIC_WIDTH, BUTTON_LOGIC_HEIGHT);
						m_pFirst[nCntButton]->BindTexture(CTexture::GetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_STRATEGY_ICON)));
					}

					m_pFirst[0]->SetTex(0, 1, 17);
					m_pFirst[1]->SetTex(10, 1, 17);

					// 線の生成
					if (NULL == m_pLine[0]) { m_pLine[0] = CButtonLine::Create(m_pAIType[nCntAIType]->GetPos() + D3DXVECTOR3(m_pAIType[m_nSelectAIType]->GetWidth() * 0.5f, 0.0f, 0.0f)); }
				}
				else
				{// 停止時
					m_nSelectAIType = -1;
				}
			}
		}
	}
}

//=========================================
// １階層目の処理
//=========================================
void CButtonManagerStrategy::FirstHierarchy(void)
{
	for (int nCntFirst = 0; nCntFirst < m_aNumLogic[0]; nCntFirst++)
	{
		if (m_pFirst[nCntFirst]->InRange(CManager::GetGame()->GetMouseCursor()->GetPos()))
		{// 範囲内かチェック
			if (m_pFirst[nCntFirst]->ClickRelease())
			{// クリックされた
				ButtonUninit(3);	// ボタンの破棄

				if (m_pFirst[nCntFirst]->GetSwitch())
				{// 起動時
					m_aSelectLogic[0] = nCntFirst;
					m_pLine[0]->Link(m_pFirst[nCntFirst]->GetPos() + D3DXVECTOR3(-m_pFirst[nCntFirst]->GetWidth() * 0.5f, 0.0f, 0.0f));

					if (0 == nCntFirst)
					{// 0番目のボタン
						m_aNumLogic[1] = 2;		// ２階層目のボタンの数
						m_pSecond = new CButton2D*[m_aNumLogic[1]];
						for (int nCntButton = 0; nCntButton < m_aNumLogic[1]; nCntButton++)
						{// ボタンの生成
							float posY = (480.0f + (nCntButton * BUTTON_LOGIC_HEIGHT * (5 - m_aNumLogic[1]))) - (BUTTON_LOGIC_HEIGHT * (5 - m_aNumLogic[1]) * (m_aNumLogic[1] / 2.0f)) + BUTTON_LOGIC_HEIGHT * (5 - m_aNumLogic[1]) / 2.0f;
							m_pSecond[nCntButton] = CButton2D::Create(D3DXVECTOR3(786.0f, posY, 0.0f), BUTTON_LOGIC_WIDTH, BUTTON_LOGIC_HEIGHT);
							m_pSecond[nCntButton]->BindTexture(CTexture::GetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_STRATEGY_ICON)));
							m_pSecond[nCntButton]->SetTex(nCntButton + 1, 1, 17);
						}
					}
					else
					{// 1番目のボタン
						m_aNumLogic[1] = 2;		// ２階層目のボタンの数
						m_pSecond = new CButton2D*[m_aNumLogic[1]];
						for (int nCntButton = 0; nCntButton < m_aNumLogic[1]; nCntButton++)
						{// ボタンの生成
							float posY = (480.0f + (nCntButton * BUTTON_LOGIC_HEIGHT * (5 - m_aNumLogic[1]))) - (BUTTON_LOGIC_HEIGHT * (5 - m_aNumLogic[1]) * (m_aNumLogic[1] / 2.0f)) + BUTTON_LOGIC_HEIGHT * (5 - m_aNumLogic[1]) / 2.0f;
							m_pSecond[nCntButton] = CButton2D::Create(D3DXVECTOR3(890.0f, posY, 0.0f), BUTTON_LOGIC_WIDTH, BUTTON_LOGIC_HEIGHT);
							m_pSecond[nCntButton]->BindTexture(CTexture::GetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_STRATEGY_ICON)));
							m_pSecond[nCntButton]->SetTex(nCntButton + 11, 1, 17);
						}
					}

					// 線の生成
					if (NULL == m_pLine[1]) { m_pLine[1] = CButtonLine::Create(m_pFirst[m_aSelectLogic[0]]->GetPos() + D3DXVECTOR3(m_pFirst[m_aSelectLogic[0]]->GetWidth() * 0.5f, 0.0f, 0.0f)); }

					if(NULL == m_apSelectIcon[0])
					{// 選択した項目の生成
						m_apSelectIcon[0] = CScene2D::Create();
						m_apSelectIcon[0]->SwapPriority(6);
						m_apSelectIcon[0]->SetPos(D3DXVECTOR3(m_pFirst[0]->GetPos().x, 190.0f, 0.0f));
						m_apSelectIcon[0]->SetSize(BUTTON_LOGIC_WIDTH, BUTTON_LOGIC_HEIGHT);
						m_apSelectIcon[0]->BindTexture(CTexture::GetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_STRATEGY_ICON)));
						m_apSelectIcon[0]->SetTex(m_aSelectLogic[0] * 10, 1, 17);
					}
					else
					{
						m_apSelectIcon[0]->SetTex(m_aSelectLogic[0] * 10, 1, 17);
					}
				}
				else
				{// 停止時
					m_aSelectLogic[0] = -1;
				}
			}
		}
	}

	for (int nCntFirst = 0; nCntFirst < m_aNumLogic[0]; nCntFirst++)
	{// １階層目のボタンの数分ループ
		if (nCntFirst != m_aSelectLogic[0])
		{// 選択中以外のボタンを停止状態にする
			m_pFirst[nCntFirst]->SetSwitch(false);
		}
	}
}

//=========================================
// ２階層目の処理
//=========================================
void CButtonManagerStrategy::SecondHierarchy(void)
{
	for (int nCntSecond = 0; nCntSecond < m_aNumLogic[1]; nCntSecond++)
	{
		if (m_pSecond[nCntSecond]->InRange(CManager::GetGame()->GetMouseCursor()->GetPos()))
		{// 範囲内かチェック
			if (m_pSecond[nCntSecond]->ClickRelease())
			{// クリックされた
				ButtonUninit(2);	// ボタンの破棄

				if (m_pSecond[nCntSecond]->GetSwitch())
				{// 起動時
					m_aSelectLogic[1] = nCntSecond;
					m_pLine[1]->Link(m_pSecond[nCntSecond]->GetPos() + D3DXVECTOR3(-m_pSecond[nCntSecond]->GetWidth() * 0.5f, 0.0f, 0.0f));

					if (0 == m_aSelectLogic[0])
					{// 移動ロジックツリー
						if (0 == nCntSecond)
						{// 0番目のボタン
							m_aNumLogic[2] = 3;		// ３階層目のボタンの数
							m_pThird = new CButton2D*[m_aNumLogic[2]];
							for (int nCntButton = 0; nCntButton < m_aNumLogic[2]; nCntButton++)
							{// ボタンの生成
								float posY = (480.0f + (nCntButton * BUTTON_LOGIC_HEIGHT * (5 - m_aNumLogic[2]))) - (BUTTON_LOGIC_HEIGHT * (5 - m_aNumLogic[2]) * (m_aNumLogic[2] / 2.0f)) + BUTTON_LOGIC_HEIGHT * (5 - m_aNumLogic[2]) / 2.0f;
								m_pThird[nCntButton] = CButton2D::Create(D3DXVECTOR3(992.0f, posY, 0.0f), BUTTON_LOGIC_WIDTH, BUTTON_LOGIC_HEIGHT);
								m_pThird[nCntButton]->BindTexture(CTexture::GetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_STRATEGY_ICON)));
								m_pThird[nCntButton]->SetTex(nCntButton + 3, 1, 17);
							}
						}
						else
						{// 1番目のボタン
							m_aNumLogic[2] = 2;		// ３階層目のボタンの数
							m_pThird = new CButton2D*[m_aNumLogic[2]];
							for (int nCntButton = 0; nCntButton < m_aNumLogic[2]; nCntButton++)
							{// ボタンの生成
								float posY = (480.0f + (nCntButton * BUTTON_LOGIC_HEIGHT * (5 - m_aNumLogic[2]))) - (BUTTON_LOGIC_HEIGHT * (5 - m_aNumLogic[2]) * (m_aNumLogic[2] / 2.0f)) + BUTTON_LOGIC_HEIGHT * (5 - m_aNumLogic[2]) / 2.0f;
								m_pThird[nCntButton] = CButton2D::Create(D3DXVECTOR3(992.0f, posY, 0.0f), BUTTON_LOGIC_WIDTH, BUTTON_LOGIC_HEIGHT);
								m_pThird[nCntButton]->BindTexture(CTexture::GetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_STRATEGY_ICON)));
								m_pThird[nCntButton]->SetTex(nCntButton + 6, 1, 17);
							}
						}
					}
					else if (1 == m_aSelectLogic[0])
					{// 待機ロジックツリー
						m_aNumLogic[2] = 2;		// ３階層目のボタンの数
						m_pThird = new CButton2D*[m_aNumLogic[2]];
						for (int nCntButton = 0; nCntButton < m_aNumLogic[2]; nCntButton++)
						{// ボタンの生成
							float posY = (480.0f + (nCntButton * BUTTON_LOGIC_HEIGHT * (5 - m_aNumLogic[2]))) - (BUTTON_LOGIC_HEIGHT * (5 - m_aNumLogic[2]) * (m_aNumLogic[2] / 2.0f)) + BUTTON_LOGIC_HEIGHT * (5 - m_aNumLogic[2]) / 2.0f;
							m_pThird[nCntButton] = CButton2D::Create(D3DXVECTOR3(1200.0f, posY, 0.0f), BUTTON_LOGIC_WIDTH, BUTTON_LOGIC_HEIGHT);
							m_pThird[nCntButton]->BindTexture(CTexture::GetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_STRATEGY_ICON)));
							m_pThird[nCntButton]->SetTex(nCntButton + 13 + (m_aSelectLogic[1] * 2), 1, 17);
						}
					}

					// 線の生成
					if (NULL == m_pLine[2]) { m_pLine[2] = CButtonLine::Create(m_pSecond[m_aSelectLogic[1]]->GetPos() + D3DXVECTOR3(m_pSecond[m_aSelectLogic[1]]->GetWidth() * 0.5f, 0.0f, 0.0f)); }

					if (NULL == m_apSelectIcon[1])
					{// 選択した項目の生成
						m_apSelectIcon[1] = CScene2D::Create();
						m_apSelectIcon[1]->SwapPriority(6);
						m_apSelectIcon[1]->SetPos(D3DXVECTOR3(m_pSecond[0]->GetPos().x, 190.0f, 0.0f));
						m_apSelectIcon[1]->SetSize(BUTTON_LOGIC_WIDTH, BUTTON_LOGIC_HEIGHT);
						m_apSelectIcon[1]->BindTexture(CTexture::GetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_STRATEGY_ICON)));
						m_apSelectIcon[1]->SetTex(m_aSelectLogic[1] + (m_aSelectLogic[0] * 10) + 1, 1, 17);
					}
					else
					{
						m_apSelectIcon[1]->SetTex(m_aSelectLogic[1] + (m_aSelectLogic[0] * 10) + 1, 1, 17);
					}
				}
				else
				{// 停止時
					m_aSelectLogic[1] = -1;
				}
			}
		}
	}

	for (int nCntSecond = 0; nCntSecond < m_aNumLogic[1]; nCntSecond++)
	{// ２階層目のボタンの数分ループ
		if (nCntSecond != m_aSelectLogic[1])
		{// 選択中以外のボタンを停止状態にする
			m_pSecond[nCntSecond]->SetSwitch(false);
		}
	}
}

//=========================================
// ３階層目の処理
//=========================================
void CButtonManagerStrategy::ThirdHierarchy(void)
{
	if (-1 == m_aSelectLogic[1]) { return; }	// 一つ前の階層が選択されていない場合

	for (int nCntThird = 0; nCntThird < m_aNumLogic[2]; nCntThird++)
	{
		if (m_pThird[nCntThird]->InRange(CManager::GetGame()->GetMouseCursor()->GetPos()))
		{// 範囲内かチェック
			if (m_pThird[nCntThird]->ClickRelease())
			{// クリックされた
				ButtonUninit(1);	// ボタンの破棄

				if (m_pThird[nCntThird]->GetSwitch())
				{// 起動時
					m_aSelectLogic[2] = nCntThird;
					m_pLine[2]->Link(m_pThird[nCntThird]->GetPos() + D3DXVECTOR3(-m_pThird[nCntThird]->GetWidth() * 0.5f, 0.0f, 0.0f));

					if (NULL == m_apSelectIcon[2])
					{// 選択した項目の生成
						m_apSelectIcon[2] = CScene2D::Create();
						m_apSelectIcon[2]->SwapPriority(6);
						m_apSelectIcon[2]->SetPos(D3DXVECTOR3(m_pThird[0]->GetPos().x, 190.0f, 0.0f));
						m_apSelectIcon[2]->SetSize(BUTTON_LOGIC_WIDTH, BUTTON_LOGIC_HEIGHT);
						m_apSelectIcon[2]->BindTexture(CTexture::GetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_STRATEGY_ICON)));
						if (0 == m_aSelectLogic[0])
						{// 移動
							m_apSelectIcon[2]->SetTex(m_aSelectLogic[2] + (3 * (m_aSelectLogic[1] + 1)), 1, 17);
						}
						else
						{// 待機
							m_apSelectIcon[2]->SetTex(m_aSelectLogic[2] + 13 + (m_aSelectLogic[1] * 2), 1, 17);
						}
					}
					else
					{
						m_apSelectIcon[2]->SetTex(m_aSelectLogic[2] + (m_aSelectLogic[0] * 10) + (3 * (m_aSelectLogic[1] + 1)), 1, 17);
					}

					if (0 == m_aSelectLogic[0])
					{// 移動ロジックツリー
						m_aNumLogic[3] = 2;		// ４階層目のボタンの数
						m_pFourth = new CButton2D*[m_aNumLogic[3]];
						for (int nCntButton = 0; nCntButton < m_aNumLogic[3]; nCntButton++)
						{// ボタンの生成
							float posY = (480.0f + (nCntButton * BUTTON_LOGIC_HEIGHT * (5 - m_aNumLogic[3]))) - (BUTTON_LOGIC_HEIGHT * (5 - m_aNumLogic[3]) * (m_aNumLogic[3] / 2.0f)) + BUTTON_LOGIC_HEIGHT * (5 - m_aNumLogic[3]) / 2.0f;
							m_pFourth[nCntButton] = CButton2D::Create(D3DXVECTOR3(1200.0f, posY, 0.0f), BUTTON_LOGIC_WIDTH, BUTTON_LOGIC_HEIGHT);
							m_pFourth[nCntButton]->BindTexture(CTexture::GetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_STRATEGY_ICON)));
							m_pFourth[nCntButton]->SetTex(nCntButton + 8, 1, 17);
						}

						// 線の生成
						if (NULL == m_pLine[3]) { m_pLine[3] = CButtonLine::Create(m_pThird[m_aSelectLogic[2]]->GetPos() + D3DXVECTOR3(m_pThird[m_aSelectLogic[2]]->GetWidth() * 0.5f, 0.0f, 0.0f)); }
					}
					else
					{
						m_bFinish = true;
						m_bSelect = false;
						ButtonUninit(4);	// ボタンの破棄
					}
				}
				else
				{// 停止時
					m_aSelectLogic[2] = -1;
				}
			}
		}
	}

	for (int nCntThird = 0; nCntThird < m_aNumLogic[2]; nCntThird++)
	{// ３階層目のボタンの数分ループ
		if (nCntThird != m_aSelectLogic[2])
		{// 選択中以外のボタンを停止状態にする
			m_pThird[nCntThird]->SetSwitch(false);
		}
	}
}

//=========================================
// ４階層目の処理
//=========================================
void CButtonManagerStrategy::FourthHierarchy(void)
{
	if (-1 == m_aSelectLogic[2]) { return; }	// 一つ前の階層が選択されていない場合

	for (int nCntFourth = 0; nCntFourth < m_aNumLogic[3]; nCntFourth++)
	{
		if (m_pFourth[nCntFourth]->InRange(CManager::GetGame()->GetMouseCursor()->GetPos()))
		{// 範囲内かチェック
			if (m_pFourth[nCntFourth]->ClickRelease())
			{// クリックされた
				m_aSelectLogic[3] = nCntFourth;
				m_pLine[3]->Link(m_pFourth[nCntFourth]->GetPos() + D3DXVECTOR3(-m_pFourth[nCntFourth]->GetWidth() * 0.5f, 0.0f, 0.0f));

				if (NULL == m_apSelectIcon[3])
				{// 選択した項目の生成
					m_apSelectIcon[3] = CScene2D::Create();
					m_apSelectIcon[3]->SwapPriority(6);
					m_apSelectIcon[3]->SetPos(D3DXVECTOR3(m_pFourth[0]->GetPos().x, 190.0f, 0.0f));
					m_apSelectIcon[3]->SetSize(BUTTON_LOGIC_WIDTH, BUTTON_LOGIC_HEIGHT);
					m_apSelectIcon[3]->BindTexture(CTexture::GetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_STRATEGY_ICON)));
					m_apSelectIcon[3]->SetTex(m_aSelectLogic[3] + 8, 1, 17);
					m_bFinish = true;
					m_bSelect = false;
					ButtonUninit(4);	// ボタンの破棄
				}
			}
		}
	}

	for (int nCntFourth = 0; nCntFourth < m_aNumLogic[3]; nCntFourth++)
	{// ４階層目のボタンの数分ループ
		if (nCntFourth != m_aSelectLogic[3])
		{// 選択中以外のボタンを停止状態にする
			m_pFourth[nCntFourth]->SetSwitch(false);
		}
	}
}

//=========================================
// マーカーの設置
//=========================================
void CButtonManagerStrategy::Marker(void)
{

}

//=========================================
// ボタンの削除
//=========================================
void CButtonManagerStrategy::ButtonUninit(int nLogic)
{
	if (m_pFirst != NULL && 4 == nLogic)
	{// １番目の階層の破棄
		for (int nCntLogic = 0; nCntLogic < m_aNumLogic[0]; nCntLogic++)
		{// 使われている数分ループ
			if (m_pFirst[nCntLogic] != NULL)
			{// 破棄
				m_pFirst[nCntLogic]->Uninit();
				m_pFirst[nCntLogic] = NULL;
			}
		}

		// 破棄
		delete[] m_pFirst;
		m_pFirst = NULL;
	}

	if (m_pSecond != NULL && 3 <= nLogic)
	{// ２番目の階層の破棄
		for (int nCntLogic = 0; nCntLogic < m_aNumLogic[1]; nCntLogic++)
		{// 使われている数分ループ
			if (m_pSecond[nCntLogic] != NULL)
			{// 破棄
				m_pSecond[nCntLogic]->Uninit();
				m_pSecond[nCntLogic] = NULL;
			}
		}

		// 破棄
		delete[] m_pSecond;
		m_pSecond = NULL;
	}

	if (m_pThird != NULL && 2 <= nLogic)
	{// ３番目の階層の破棄
		for (int nCntLogic = 0; nCntLogic < m_aNumLogic[2]; nCntLogic++)
		{// 使われている数分ループ
			if (m_pThird[nCntLogic] != NULL)
			{// 破棄
				m_pThird[nCntLogic]->Uninit();
				m_pThird[nCntLogic] = NULL;
			}
		}

		// 破棄
		delete[] m_pThird;
		m_pThird = NULL;
	}

	if (m_pFourth != NULL && 1 <= nLogic)
	{// ４番目の階層の破棄
		for (int nCntLogic = 0; nCntLogic < m_aNumLogic[3]; nCntLogic++)
		{// 使われている数分ループ
			if (m_pFourth[nCntLogic] != NULL)
			{// 破棄
				m_pFourth[nCntLogic]->Uninit();
				m_pFourth[nCntLogic] = NULL;
			}
		}

		// 破棄
		delete[] m_pFourth;
		m_pFourth = NULL;
	}

	for (int nCntButton = 4 - nLogic; nCntButton < 4; nCntButton++)
	{// ボタンの数を初期化
		m_aNumLogic[nCntButton] = 0;
		if (!m_bFinish)
		{
			m_aSelectLogic[nCntButton] = -1;
		}

		if (1 <= nCntButton && m_pLine[nCntButton - 1] != NULL)
		{// つながっている線を離す
			m_pLine[nCntButton - 1]->SetBoolLink(false);
		}

		if (m_pLine[nCntButton] != NULL)
		{// 線の破棄
			m_pLine[nCntButton]->Uninit();
			m_pLine[nCntButton] = NULL;
		}

		if (!m_bFinish)
		{// 選択が終わってないとき
			if (m_apSelectIcon[nCntButton] != NULL)
			{// 選択した項目の破棄
				m_apSelectIcon[nCntButton]->Uninit();
				m_apSelectIcon[nCntButton] = NULL;
			}
		}
	}
}