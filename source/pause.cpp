//=============================================================================
//
// ポーズメニュー処理 [pause.cpp]
// Author : Takuto Ishida
//
//=============================================================================
#include "pause.h"
#include "scene.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
#include "game.h"
#include "manager.h"
#include "game.h"
#include "renderer.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define PAUSE_TEXTURE00	"data/TEXTURE/pause000.png"		// テクスチャへのパス
#define PAUSE_TEXTURE01	"data/TEXTURE/pause001.png"		// テクスチャへのパス
#define PAUSE_TEXTURE02	"data/TEXTURE/pause002.png"		// テクスチャへのパス

#define PMENU_SIZE_X	(200.0f)		// メニューの大きさ(X)
#define PMENU_SIZE_Y	(45.0f)			// メニューの大きさ(Y)

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9 CPause::m_pTexture[TYPE_MAX] = {};	// 共有テクスチャへのポインタ

//=============================================================================
// テクスチャの読み込み
//=============================================================================
HRESULT CPause::Load(void)
{
	// デバイスの取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, PAUSE_TEXTURE00, &m_pTexture[TYPE_CONTINUE]);
	D3DXCreateTextureFromFile(pDevice, PAUSE_TEXTURE01, &m_pTexture[TYPE_RETRY]);
	D3DXCreateTextureFromFile(pDevice, PAUSE_TEXTURE02, &m_pTexture[TYPE_QUIT]);

	return S_OK;
}

//=============================================================================
// 読み込んだテクスチャの解放
//=============================================================================
void CPause::Unload(void)
{
	for (int nCntTex = 0; nCntTex < TYPE_MAX; nCntTex++)
	{
		if (NULL != m_pTexture[nCntTex])
		{// テクスチャの破棄
			m_pTexture[nCntTex]->Release();
			m_pTexture[nCntTex] = NULL;
		}
	}
}

//=============================================================================
// 生成処理
//=============================================================================
CPause* CPause::Create(void)
{
	CPause *pPause = NULL;	// ポーズクラスのポインタ変数

	pPause = new CPause;	// メモリを確保

	if (NULL != pPause)
	{// メモリを確保できたとき
		pPause->Init();		// 初期化処理
	}

	return pPause;
}

//=============================================================================
// コンストラクタ
//=============================================================================
CPause::CPause(int nPriority, CScene::OBJTYPE objType) : CScene(nPriority, objType)
{
	for (int nCntPolygon = 0; nCntPolygon < TYPE_MAX; nCntPolygon++)
	{// 2Dポリゴン
		m_apScene2D[nCntPolygon] = NULL;
	}

	m_nNumSelect = 0;	// 現在の選択肢
}

//=============================================================================
// デストラクタ
//=============================================================================
CPause::~CPause()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CPause::Init(void)
{
	// ポーズメニューの選択肢
	m_apScene2D[TYPE_CONTINUE] = CScene2D::Create();
	m_apScene2D[TYPE_RETRY] = CScene2D::Create();
	m_apScene2D[TYPE_QUIT] = CScene2D::Create();

	// テクスチャの設定
	m_apScene2D[TYPE_CONTINUE]->BindTexture(m_pTexture[TYPE_CONTINUE]);
	m_apScene2D[TYPE_RETRY]->BindTexture(m_pTexture[TYPE_RETRY]);
	m_apScene2D[TYPE_QUIT]->BindTexture(m_pTexture[TYPE_QUIT]);

	// 最初の選択カーソルは一番上に合わせる
	m_nNumSelect = TYPE_CONTINUE;

	for (int nCntPolygon = 0; nCntPolygon < TYPE_MAX; nCntPolygon++)
	{// 大きさの設定
		m_apScene2D[nCntPolygon]->SetSize(PMENU_SIZE_X, PMENU_SIZE_Y);
	}

	// 頂点座標
	m_apScene2D[TYPE_CONTINUE]->SetPos(D3DXVECTOR3(SCREEN_WIDTH / 2, 200.0f, 0.0f));
	m_apScene2D[TYPE_RETRY]->SetPos(D3DXVECTOR3(SCREEN_WIDTH / 2, 350.0f, 0.0f));
	m_apScene2D[TYPE_QUIT]->SetPos(D3DXVECTOR3(SCREEN_WIDTH / 2, 500.0f, 0.0f));

	// 頂点カラー
	m_apScene2D[TYPE_CONTINUE]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	m_apScene2D[TYPE_RETRY]->SetColor(D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f));
	m_apScene2D[TYPE_QUIT]->SetColor(D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f));

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CPause::Uninit(void)
{
	int nCntMenu;

	for (nCntMenu = 0; nCntMenu < TYPE_MAX; nCntMenu++)
	{// ポリゴンの破棄
		if (m_apScene2D[nCntMenu] != NULL)
		{
			m_apScene2D[nCntMenu]->Uninit();
			m_apScene2D[nCntMenu] = NULL;
		}
	}

	// オブジェクトを破棄
	CScene::Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CPause::Update(void)
{
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();	// キーボードの取得
	CXInput *pXInput = CManager::GetXInput();					// ジョイパッドの取得
	CSound *pSound = CManager::GetSound();						// サウンドの情報を取得

	// 選択処理
	Select();

	if (pXInput->GetTrigger(0, CXInput::XIJS_BUTTON_4) || pKeyboard->GetTrigger(DIK_P))
	{// ポーズを閉じる
		m_nNumSelect = TYPE_CONTINUE;
	}

	if (pXInput->GetRepeat(0, CXInput::XIJS_BUTTON_16) || pXInput->GetRepeat(0, CXInput::XIJS_BUTTON_0) || pKeyboard->GetRepeat(DIK_W) || pKeyboard->GetRepeat(DIK_UP))
	{// 選択状態を１つ上げる
		pSound->PlaySound(CSound::SOUND_LABEL_CURSOR);
		m_nNumSelect = (m_nNumSelect + (TYPE_MAX - 1)) % TYPE_MAX;

		for (int nCntPolygon = 0; nCntPolygon < TYPE_MAX; nCntPolygon++)
		{// 選択中と他で色を分ける
			if (nCntPolygon == m_nNumSelect)
			{// 選択中
				m_apScene2D[nCntPolygon]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			}
			else
			{// 他
				m_apScene2D[nCntPolygon]->SetColor(D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f));
			}
		}
	}

	if (pXInput->GetRepeat(0, CXInput::XIJS_BUTTON_17) || pXInput->GetRepeat(0, CXInput::XIJS_BUTTON_1) || pKeyboard->GetRepeat(DIK_S) || pKeyboard->GetRepeat(DIK_DOWN))
	{
		pSound->PlaySound(CSound::SOUND_LABEL_CURSOR);
		m_nNumSelect = (m_nNumSelect + 1) % TYPE_MAX;	// 選択状態を１つ下げる

		for (int nCntPolygon = 0; nCntPolygon < TYPE_MAX; nCntPolygon++)
		{// 選択中と他で色を分ける
			if (nCntPolygon == m_nNumSelect)
			{// 選択中
				m_apScene2D[nCntPolygon]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			}
			else
			{// 他
				m_apScene2D[nCntPolygon]->SetColor(D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f));
			}
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CPause::Draw(void)
{
}

//=============================================================================
// 選択処理
//=============================================================================
void CPause::Select(void)
{
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();	// キーボードの取得
	CXInput *pXInput = CManager::GetXInput();					// ジョイパッドの取得

	if (pXInput->GetTrigger(0, CXInput::XIJS_BUTTON_11) || pKeyboard->GetTrigger(DIK_RETURN))
	{
		// ポーズ画面から抜ける
		CManager::GetGame()->SetPauseBool(false);

		// 選択中の項目
		switch (m_nNumSelect)
		{
		case TYPE_CONTINUE:
			break;

		case TYPE_RETRY:
				// 画面遷移
				CFade::Create(CManager::MODE_GAME);

			break;

		case TYPE_QUIT:
				// 画面遷移
				CFade::Create(CManager::MODE_TITLE);

			break;
		}
	}
}

