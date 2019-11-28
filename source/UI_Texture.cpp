//=============================================================================
//
// UI_Texture処理 [UI_Texture.cpp]
// Author : Ayaka Hakozaki
//
//=============================================================================
#include "UI_Texture.h"
#include "texture.h"
#include "number.h"
#include "input.h"
#include "manager.h"
#include "debugProc.h"
#include "fade.h"
#include "UI_Number.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define UI_TEX_SCROLLSPEED		(0.005f)	// 背景のスクロールスピード
#define UI_TEX_FLASH					(40)			// 点滅フレーム数
#define UI_TEX_RELOAD_TIME		(60 * 2)	// リロード時間
#define UI_TEX_ANIM_SPEED		(10)			// タイルアニメーション
#define UI_TEX_ANIM_PATTERN	(8)			// タイルのパターン数

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************

//=============================================================================
// コンストラクタ
//=============================================================================
CUI_TEXTURE::CUI_TEXTURE(int nPriority, CScene::OBJTYPE objType) : CScene2D(nPriority, objType)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_texPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_colRadioMess = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	m_nCntBgMove = 0;
	m_nFlash = 0;
	m_nCntAnim = 0;
	m_nPatternAnim = 0;
	m_nCntReload = 0;
}

//=============================================================================
// デストラクタ
//=============================================================================
CUI_TEXTURE::~CUI_TEXTURE()
{
}

//=============================================================================
// 生成処理
//=============================================================================
CUI_TEXTURE *CUI_TEXTURE::Create(D3DXVECTOR3 pos, float fWidth, float fHeight, UI_TEXTYPE UITexType)
{
	CUI_TEXTURE *pUITex = NULL;	// UIクラスのポインタ変数

	pUITex = new CUI_TEXTURE;		// メモリ確保

	if (NULL != pUITex)
	{	// メモリ確保できた
		pUITex->m_UITexType = UITexType;	// フレームタイプの設定
		pUITex->Init();									// 初期化処理
		pUITex->SetPos(pos);						// 位置の設定
		pUITex->SetSize(fWidth, fHeight);		// サイズの設定
	}

	return pUITex;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CUI_TEXTURE::Init(void)
{
	// 初期化処理
	CScene2D::Init();

	//****************************************
	// テクスチャ・色・各タイプ初期化の設定
	//****************************************
	switch (m_UITexType)
	{
	case UIFLAME_NONE:
		CScene2D::SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.8f));
		break;

		//****************************************
		// タイトル
		//****************************************
	case UIFLAME_BG_00:
		CScene2D::BindTexture(CTexture::GetTexture(CTexture::TEXTURE_BG_00));
		CScene2D::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		break;

	case UIFLAME_BG_01:
		CScene2D::BindTexture(CTexture::GetTexture(CTexture::TEXTURE_BG_01));
		CScene2D::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		break;

	case UIFLAME_FLONTLINE:
		CScene2D::BindTexture(CTexture::GetTexture(CTexture::TEXTURE_FLONTLINE));
		CScene2D::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		break;

	case UIFLAME_PRESSBOTTON:
		CScene2D::BindTexture(CTexture::GetTexture(CTexture::TEXTURE_PRESSBOTTON));
		CScene2D::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		break;

		//****************************************
		// マッチング画面
		//****************************************
	case UIFLAME_MATCHING_BG:
		CScene2D::BindTexture(CTexture::GetTexture(CTexture::TEXTURE_MATCHING_BG));
		CScene2D::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		break;

	case UIFLAME_MATCHING_BG_00:
		CScene2D::BindTexture(CTexture::GetTexture(CTexture::TEXTURE_MATCHING_BG_00));
		CScene2D::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		break;

	case UIFLAME_MATCHING_BG_01:
		CScene2D::BindTexture(CTexture::GetTexture(CTexture::TEXTURE_MATCHING_BG_01));
		CScene2D::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		break;

	case UIFLAME_VS:
		CScene2D::BindTexture(CTexture::GetTexture(CTexture::TEXTURE_VS));
		CScene2D::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		break;

	case UIFLAME_PLAYER_NUM:
		CScene2D::BindTexture(CTexture::GetTexture(CTexture::TEXTURE_PLAYER_NUM));
		CScene2D::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		break;

	case UIFLAME_BLUE_MECHA_LIST:
		CScene2D::BindTexture(CTexture::GetTexture(CTexture::TEXTURE_BLUE_MECHA_LIST));
		CScene2D::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		break;

	case UIFLAME_RED_MECHA_LIST:
		CScene2D::BindTexture(CTexture::GetTexture(CTexture::TEXTURE_RED_MECHA_LIST));
		CScene2D::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		break;

		//****************************************
		// アクション・ストラテジー共通
		//****************************************
	case UIFLAME_TEAM_BLUE:
		CScene2D::BindTexture(CTexture::GetTexture(CTexture::TEXTURE_UPPER_UI));
		CScene2D::SetTex(0,1,3);
		CScene2D::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		break;

	case UIFLAME_TEAM_RED:
		CScene2D::BindTexture(CTexture::GetTexture(CTexture::TEXTURE_UPPER_UI));
		CScene2D::SetTex(1, 1, 3);
		CScene2D::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		break;

	case UIFLAME_PLAYER_HP:
		CScene2D::BindTexture(CTexture::GetTexture(CTexture::TEXTURE_PLAYER_FLAME));
		CScene2D::SetTex(0,1,4);
		CScene2D::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		break;

	case UIFLAME_DRONE:
		CScene2D::BindTexture(CTexture::GetTexture(CTexture::TEXTURE_AI_MECHA_FLAME));
		CScene2D::SetTex(0, 1, 2);
		CScene2D::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		break;

	case UIFLAME_WORKER:
		CScene2D::BindTexture(CTexture::GetTexture(CTexture::TEXTURE_AI_MECHA_FLAME));
		CScene2D::SetTex(1, 1, 2);
		CScene2D::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		break;

	case UIFLAME_WEAPON:
		CScene2D::BindTexture(CTexture::GetTexture(CTexture::TEXTURE_WEAPON_FLAME));
		CScene2D::SetTex(0, 1, 4);
		CScene2D::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		break;

	case UIFLAME_TIMER:
		CScene2D::BindTexture(CTexture::GetTexture(CTexture::TEXTURE_UPPER_UI));
		CScene2D::SetTex(2, 1, 3);
		CScene2D::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		break;

	case UIFLAME_CHANGE:
		m_nFlash = UI_TEX_FLASH;
		CScene2D::BindTexture(CTexture::GetTexture(CTexture::TEXTURE_CHANGE));
		CScene2D::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		break;

	case UIFLAME_ACTION_PART:
		CScene2D::BindTexture(CTexture::GetTexture(CTexture::TEXTURE_PART_CHANGE));
		CScene2D::SetTex(0, 1, 2);
		CScene2D::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		break;

	case UIFLAME_STRATEGY_PART:
		CScene2D::BindTexture(CTexture::GetTexture(CTexture::TEXTURE_PART_CHANGE));
		CScene2D::SetTex(1, 1, 2);
		CScene2D::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		break;

		//****************************************
		// アクションパート
		//****************************************
	case UIFLAME_RELOAD:
		CScene2D::BindTexture(CTexture::GetTexture(CTexture::TEXTURE_RELOAD));
		CScene2D::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		CScene2D::SetObjType(OBJTYPE_NONE);
		break;

	case UIFLAME_START:
		CScene2D::BindTexture(CTexture::GetTexture(CTexture::TEXTURE_START_DEATH));
		CScene2D::SetTex(0, 1, 2);
		CScene2D::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		break;

	case UIFLAME_DEATH:
		CScene2D::BindTexture(CTexture::GetTexture(CTexture::TEXTURE_START_DEATH));
		CScene2D::SetTex(1, 1, 2);
		CScene2D::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		break;

	case UIFLAME_TILE_PATTERN:
		CScene2D::BindTexture(CTexture::GetTexture(CTexture::TEXTURE_TILE_PATTERN));
		CScene2D::SetTex(0, 1, UI_TEX_ANIM_PATTERN);
		CScene2D::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		break;

	case UIFLAME_RESPAWN:
		CScene2D::BindTexture(CTexture::GetTexture(CTexture::TEXTURE_RESPAWN));
		CScene2D::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		break;

	case UIFLAME_MAP:
		CScene2D::BindTexture(CTexture::GetTexture(CTexture::TEXTURE_MAP));
		CScene2D::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		break;

	case UIFLAME_RESPAWN_FLAME:
		CScene2D::BindTexture(CTexture::GetTexture(CTexture::TEXTURE_RESPAWN_FLAME));
		CScene2D::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		break;

	case UIFLAME_TITLE:
		CScene2D::BindTexture(CTexture::GetTexture(CTexture::TEXTURE_TITLE));
		CScene2D::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		break;

	case UIFLAME_SELECT_RESPAWN:
		CScene2D::BindTexture(CTexture::GetTexture(CTexture::TEXTURE_SELECT_RESPAWN));
		CScene2D::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		break;

	case UIFLAME_RADIOCHAT:
		CScene2D::BindTexture(CTexture::GetTexture(CTexture::TEXTURE_RADIOCHAT));
		CScene2D::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		break;

	case UIFLAME_RADIOCHAT_MESS:
		CScene2D::BindTexture(CTexture::GetTexture(CTexture::TEXTURE_RADIOCHAT_MESS));
		m_colRadioMess = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		break;

	case UIFLAME_TEAM_WIN:
		CScene2D::BindTexture(CTexture::GetTexture(CTexture::TEXTURE_TEAM_WIN));
		CScene2D::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		break;

		//****************************************
		// ストラテジーパート
		//****************************************
	case UIFLAME_1P_INFO:
		CScene2D::BindTexture(CTexture::GetTexture(CTexture::TEXTURE_1P_INFO));
		CScene2D::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		break;

	case UIFLAME_2P_INFO:
		CScene2D::BindTexture(CTexture::GetTexture(CTexture::TEXTURE_2P_INFO));
		CScene2D::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		break;

	case UIFLAME_FLAME_WHITE:
		CScene2D::BindTexture(CTexture::GetTexture(CTexture::TEXTURE_FLAME));
		CScene2D::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		break;

	case UIFLAME_FLAME_BLUE:
		CScene2D::BindTexture(CTexture::GetTexture(CTexture::TEXTURE_FLAME));
		CScene2D::SetColor(D3DXCOLOR(0.0f, 0.4f, 1.0f, 1.0f));
		break;

	case UIFLAME_STRATEGY_BG:
		CScene2D::BindTexture(CTexture::GetTexture(CTexture::TEXTURE_STRATEGY_BG));
		CScene2D::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f));
		break;
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CUI_TEXTURE::Uninit(void)
{
	m_nCntAnim = 0;
	m_nPatternAnim = 0;

	// オブジェクトを破棄
	CScene2D::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CUI_TEXTURE::Update(void)
{
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();	// キーボードの入力を取得

	// 背景スクロールカウンター
	m_nCntBgMove++;

	//****************************************
	// 各タイプの処理
	//****************************************
	switch (m_UITexType)
	{
	case UIFLAME_BG_00:
		CScene2D::SetBgMove(m_nCntBgMove, 0, UI_TEX_SCROLLSPEED);		// -m_nCntBgMoveで反対向きに移動
		break;

	case UIFLAME_BG_01:
		CScene2D::SetBgMove(-m_nCntBgMove, 0, UI_TEX_SCROLLSPEED);		// -m_nCntBgMoveで反対向きに移動
		break;

	case UIFLAME_CHANGE:
		CScene2D::Flashing(m_nFlash);		// 点滅
		break;

	case UIFLAME_TILE_PATTERN:
		m_nCntAnim++;
		if (m_nCntAnim % UI_TEX_ANIM_SPEED == 0)
		{
			m_nPatternAnim++;
			CScene2D::SetTex(m_nPatternAnim, 1, UI_TEX_ANIM_PATTERN);
		}
		if (m_nPatternAnim > (UI_TEX_ANIM_PATTERN - 1))
		{
			CScene2D::SetTex((UI_TEX_ANIM_PATTERN - 1), 1, UI_TEX_ANIM_PATTERN);
		}
		break;
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CUI_TEXTURE::Draw(void)
{
	CScene2D::Draw();	// 描画処理
}

//****************************************
// 3Dテクスチャクラス
//****************************************
//=============================================================================
// コンストラクタ
//=============================================================================
CUI_TEXTURE3D::CUI_TEXTURE3D(int nPriority, CScene::OBJTYPE objType) : CScene3D(nPriority, objType)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_UI3dTex = UI3DTEX_NONE;
}

//=============================================================================
// デストラクタ
//=============================================================================
CUI_TEXTURE3D::~CUI_TEXTURE3D()
{
}

//=============================================================================
// 生成
//=============================================================================
CUI_TEXTURE3D *CUI_TEXTURE3D::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, UI3DTEX UI3DTex)
{
	CUI_TEXTURE3D *pUITex3D = NULL;

	pUITex3D = new CUI_TEXTURE3D;

	if (pUITex3D != NULL)
	{
		pUITex3D->m_UI3dTex = UI3DTex;
		pUITex3D->m_pos = pos;
		pUITex3D->m_size = size;
		pUITex3D->Init();
		pUITex3D->SetPos(pos);		// 位置の設定
		pUITex3D->SetSize(size);		// サイズ
	}

	return pUITex3D;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CUI_TEXTURE3D::Init(void)
{
	CScene3D::Init();		// 初期化

	//switch (m_UI3dTex)
	//{
	//case UI3DTEX_ACTION:
	//	CScene3D::BindTexture(CTexture::GetTexture(CTexture::TEXTURE_ACTION));
	//	CScene3D::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	//	break;

	//case UI3DTEX_STRATEGY:
	//	CScene3D::BindTexture(CTexture::GetTexture(CTexture::TEXTURE_STRATEGY));
	//	CScene3D::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	//	break;
	//}
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CUI_TEXTURE3D::Uninit(void)
{
	CScene3D::Uninit();	// 終了
}

//=============================================================================
// 更新処理
//=============================================================================
void CUI_TEXTURE3D::Update(void)
{
#ifdef _DEBUG
	CDebugProc::Print("pos : %.0f, %.0f, %.0f\n", m_pos.x, m_pos.y, m_pos.z);
	CDebugProc::Print("size : %.0f, %.0f, %.0f\n", m_size.x, m_size.y, m_size.z);
#endif
}

//=============================================================================
// 描画処理
//=============================================================================
void CUI_TEXTURE3D::Draw(void)
{
	CScene3D::Draw();	// 描画処理
}