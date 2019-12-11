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
	 // UI
		{ "data/TEXTURE/UI/number.png" },									// ナンバー
		{ "data/TEXTURE/UI/bg001.png" },									// Bg00から変更
		{ "data/TEXTURE/UI/bg002.png" },									// BG01から変更
		{ "data/TEXTURE/UI/weapon_ui.png" },							// 武器フレーム
		{ "data/TEXTURE/UI/frontLine.png" },								// タイトル
		{ "data/TEXTURE/UI/press_any.png" },								// プレスボタン
		{ "data/TEXTURE/UI/pleyer_frame.png" },						// プレイヤーフレーム
		{ "data/TEXTURE/UI/change.png" },									// パート切り替え
		{ "data/TEXTURE/UI/upper_ui.png" },								// BLUE、RED、タイムUI
		{ "data/TEXTURE/UI/part_change.png" },							// アクション、ストラテジー
		{ "data/TEXTURE/UI/ai_mecha_frame.png" },					// ドローン、ワーカー
		{ "data/TEXTURE/UI/cursor.png" },									// マウスカーソル
		{ "data/TEXTURE/UI/reticle.png" },									// レティクル
		{ "data/TEXTURE/UI/reload.png" },									// リロードロゴ（アクションパートで使用）
		{ "data/TEXTURE/UI/team_win.png" },								// 勝利チームのロゴ
		{ "data/TEXTURE/UI/linkenergy_blue.png" },					// BLUEリンクゲージ
		{ "data/TEXTURE/UI/linkenergy_red.png" },						// REDリンクゲージ
		{ "data/TEXTURE/UI/linkenergy_frame.png" },					// リンクゲージフレーム
		{ "data/TEXTURE/UI/danger_upper.png" },						// 当たった方向上下
		{ "data/TEXTURE/UI/danger_side.png" },						// 当たった方向左右

		// TEXTURE
		{ "data/TEXTURE/press_any.png" },									// プレスボタン
		{ "data/TEXTURE/bullet.png" },											// 弾

		// strategy
		{ "data/TEXTURE/strategy/player_1_frame.png" },			// 1Pの小隊情報フレーム
		{ "data/TEXTURE/strategy/player_2_frame.png" },			// 2Pの小隊情報フレーム
		{ "data/TEXTURE/strategy/frame_white.png" },				// ただのフレーム（ストラテジーパートで使用）
		{ "data/TEXTURE/strategy/strategy_bg.png" },					// ストラテジーパートの背景
		{ "data/TEXTURE/strategy/worker_button.png" },			// ワーカー
		{ "data/TEXTURE/strategy/drone_button.png" },				// ドローン
		{ "data/TEXTURE/strategy/strategy_icon.png" },				// ストラテジーのアイコン

		// mechaSelect
		{ "data/TEXTURE/mechaSelect/meun_title.png" },			// メインメニュー、メカセレクトロゴ
		{ "data/TEXTURE/mechaSelect/aptitude_range.jpg" },		// 距離表示（機体選択画面で使用）
		{ "data/TEXTURE/mechaSelect/mecha_inst.png" },			// 機体説明（機体選択画面で使用）
		{ "data/TEXTURE/mechaSelect/mecha_type.png" },			// 機体の種類（機体選択画面で使用）
		{ "data/TEXTURE/mechaSelect/mecha_select.png" },		// 機体画像（機体選択画面で使用）
		{ "data/TEXTURE/mechaSelect/back.png" },					// 前の画面へ戻る
		{ "data/TEXTURE/mechaSelect/mecha_range.png" },		// 飛距離（long, shortロゴ）

		// menu
		{ "data/TEXTURE/menu/menu_frame.png" },					// モードフレーム（メニュー画面で使用）
		{ "data/TEXTURE/menu/menu_inst.png" },						// モードの概要（メニュー選択画面で使用）
		{ "data/TEXTURE/menu/infomation.png" },						// インフォメーション

		// matching
		{ "data/TEXTURE/matching/matching_bg.jpg" },				// マッチング画面の背景（マッチング画面で使用）
		{ "data/TEXTURE/matching/matching_bg_000.png" },		// マッチングBG_00（マッチング画面で使用）
		{ "data/TEXTURE/matching/matching_bg_001.png" },		// マッチングBG_01（マッチング画面で使用）
		{ "data/TEXTURE/matching/VS.png" },								// VSロゴ（マッチング画面で使用）
		{ "data/TEXTURE/matching/player_number.png" },			// プレイヤーナンバー（マッチング画面で使用）
		{ "data/TEXTURE/matching/blue_mecha_list.png" },		// BLUEチームメカリスト（マッチング画面で使用）
		{ "data/TEXTURE/matching/red_mecha_list.png" },			// REDチームメカリスト（マッチング画面で使用）

		// respawn
		{ "data/TEXTURE/respawn/start_death.png" },					// 戦闘開始、行動不能ロゴ（アクションパートで使用）
		{ "data/TEXTURE/respawn/tile_pattern.png" },					// タイルアニメーション（アクションパートで使用）
		{ "data/TEXTURE/respawn/respawn_sequence.png" },		// 復帰ロゴ（アクションパートで使用）
		{ "data/TEXTURE/respawn/respawn_map.png" },				// リスポーンマップ（アクションパートで使用）
		{ "data/TEXTURE/respawn/respawn_frame.jpg" },			// リスポーンフレーム（アクションパートで使用）
		{ "data/TEXTURE/respawn/respawn_title.jpg" },				// リスポーンタイトル（アクションパートで使用）
		{ "data/TEXTURE/respawn/select_respawn.png" },			// リスポーン選択（アクションパートで使用）

		// radiochat
		{ "data/TEXTURE/radiochat/radio_chat.png" },						// ラジオチャット（アクションパートで使用）
		{ "data/TEXTURE/radiochat/radio_chat_message.png" },		// ラジオチャットメッセージ

		// option
		{ "data/TEXTURE/option/option_icon.png" },						// オプションアイコン
		{ "data/TEXTURE/option/option_frame.png" },						// オプションフレーム
		{ "data/TEXTURE/option/batsu.png" },									// バツボタン
		{ "data/TEXTURE/option/toggle.png" },									// トグルスイッチ

		// tutorialmenu
		{ "data/TEXTURE/tutorialmenu/tutorial_frame.png" },			// チュートリアルメニュー
		{ "data/TEXTURE/tutorialmenu/tutorialmenu_inst.png" },		// チュートリアルメニューの説明文

		//tutorial
		{ "data/TEXTURE/tutorial/tutorial_logo.png" },						//チュートリアルの現在の項目
		{ "data/TEXTURE/tutorial/tutorial_step.png" },						//チュートリアルの項目数
		{ "data/TEXTURE/tutorial/tutorial_mission_basic.png" },				//チュートリアルの課題(基本操作)
		{ "data/TEXTURE/tutorial/tutorial_mission_Action.png" },			//チュートリアルの課題(アクションパート)
		{ "data/TEXTURE/tutorial/tutorial_mission_Strategy.png" },			//チュートリアルの課題(ストラテジーパート)
		{ "data/TEXTURE/tutorial/basic_move.png" },							//操作の方法(基本操作)
		{ "data/TEXTURE/tutorial/action_part.png" },						//操作の方法(アクションパート)
		{ "data/TEXTURE/tutorial/strategy_part.png" },						//操作の方法(ストラテジーパート)
		{ "data/TEXTURE/tutorial/Success.png" },							//チュートリアル成功！

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
