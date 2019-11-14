//=============================================================================
//
// テクスチャ読み込み処理 [texture.h]
// Author : Takuto Ishida
//
//=============================================================================
#include "texture.h"
#include "renderer.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 CTexture::m_pTexture[CTexture::TEXTURE_MAX] = {};

//=========================================
// コンストラクタ
//=========================================
CTexture::CTexture()
{
}

//=========================================
// デストラクタ
//=========================================
CTexture::~CTexture()
{
}

//=========================================
// テクスチャの読み込み
//=========================================
void CTexture::Load(void)
{
	// デバイスの取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	const char *pTextureName[TEXTURE_MAX] =
	{// テクスチャ名
		{ "data/TEXTURE/UI/number.png" },									// ナンバー
		{ "data/TEXTURE/press_any.png" },									// プレスボタン
		{ "data/TEXTURE/UI/bg001.png" },									// Bg00から変更
		{ "data/TEXTURE/UI/bg002.png" },									// BG01から変更
		{ "data/TEXTURE/UI/weapon_ui.png" },							// 武器フレーム
		{ "data/TEXTURE/UI/frontLine.png" },								// タイトル
		{ "data/TEXTURE/UI/press_any.png" },								// プレスボタン
		{ "data/TEXTURE/UI/pleyer_frame.png" },						// プレイヤーフレーム
		{ "data/TEXTURE/UI/change.png" },									// パート切り替え
		{ "data/TEXTURE/strategy/player_1_frame.png" },			// 1Pの小隊情報フレーム
		{ "data/TEXTURE/strategy/player_2_frame.png" },			// 2Pの小隊情報フレーム
		{ "data/TEXTURE/strategy/frame_white.png" },				// ただのフレーム（ストラテジーパートで使用）
		{ "data/TEXTURE/strategy/strategy_bg.png" },					// ストラテジーパートの背景
		{ "data/TEXTURE/UI/upper_ui.png" },								// BLUE、RED、タイムUI
		{ "data/TEXTURE/UI/part_change.png" },							// アクション、ストラテジー
		{ "data/TEXTURE/UI/ai_mecha_frame.png" },					// ドローン、ワーカー
		{ "data/TEXTURE/UI/cursor.png" },									// マウスカーソル
		{ "data/TEXTURE/strategy/worker_button.png" },			// ワーカー
		{ "data/TEXTURE/strategy/drone_button.png" },				// ドローン
		{ "data/TEXTURE/UI/reticle.png" },									// レティクル
		{ "data/TEXTURE/mechaSelect/main_frame.png" },			// メインメニュー、メカセレクトロゴ
		{ "data/TEXTURE/mechaSelect/aptitude_range.jpg" },		// 距離表示（機体選択画面で使用）
		{ "data/TEXTURE/mechaSelect/mecha_inst.png" },			// 機体説明（機体選択画面で使用）
		{ "data/TEXTURE/mechaSelect/mecha_type.png" },			// 機体の種類（機体選択画面で使用）
		{ "data/TEXTURE/menu/menu_disp.png" },						// モード説明ロゴ（メニュー画面で使用）
		{ "data/TEXTURE/menu/menu_frame.png" },					// モードフレーム（メニュー画面で使用）
		{ "data/TEXTURE/menu/menu_inst.png" },						// モードの概要（メニュー選択画面で使用）
		{ "data/TEXTURE/matching/matching_bg.jpg" },				// マッチング画面の背景（マッチング画面で使用）
		{ "data/TEXTURE/matching/matching_bg_000.png" },		// マッチングBG_00（マッチング画面で使用）
		{ "data/TEXTURE/matching/matching_bg_001.png" },		// マッチングBG_01（マッチング画面で使用）
		{ "data/TEXTURE/matching/VS.png" },								// VSロゴ（マッチング画面で使用）
		{ "data/TEXTURE/matching/player_number.png" },			// プレイヤーナンバー（マッチング画面で使用）
		{ "data/TEXTURE/matching/blue_mecha_list.png" },		// BLUEチームメカリスト（マッチング画面で使用）
		{ "data/TEXTURE/matching/red_mecha_list.png" },			// REDチームメカリスト（マッチング画面で使用）
		{ "data/TEXTURE/respawn/start_death.png" },					// 戦闘開始、行動不能ロゴ（アクションパートで使用）
		{ "data/TEXTURE/respawn/tile_pattern.png" },					// タイルアニメーション（アクションパートで使用）
		{ "data/TEXTURE/respawn/respawn_sequence.png" },	// 復帰ロゴ（アクションパートで使用）
		{ "data/TEXTURE/respawn/map_b.jpg" },							// リスポーンマップ（アクションパートで使用）
		{ "data/TEXTURE/respawn/respawn_frame.jpg" },			// リスポーンフレーム（アクションパートで使用）
		{ "data/TEXTURE/respawn/respawn_title.jpg" },				// リスポーンタイトル（アクションパートで使用）
		{ "data/TEXTURE/respawn/select_respawn.png" },			// リスポーン選択（アクションパートで使用）
		{ "data/TEXTURE/UI/reload.png" },									// リロードロゴ（アクションパートで使用）
		{ "data/TEXTURE/mechaSelect/mecha_select.png" },		// 機体画像（機体選択画面で使用）
		{ "data/TEXTURE/bullet.png" },						// 弾
	};

	for (int nCntTexture = 0; nCntTexture < TEXTURE_MAX; nCntTexture++)
	{// テクスチャの生成
		D3DXCreateTextureFromFile(pDevice, pTextureName[nCntTexture], &m_pTexture[nCntTexture]);
	}
}

//=========================================
// 読み込んだテクスチャの解放
//=========================================
void CTexture::Unload(void)
{
	for (int nCntTexture = 0; nCntTexture < TEXTURE_MAX; nCntTexture++)
	{// テクスチャ数分ループ
		if (NULL != m_pTexture[nCntTexture])
		{// テクスチャの破棄
			m_pTexture[nCntTexture]->Release();
			m_pTexture[nCntTexture] = NULL;
		}
	}
}
