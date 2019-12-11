//=============================================================================
//
// テクスチャ読み込み処理 [texture.h]
// Author : Takuto Ishida
//
//=============================================================================
#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// 前方宣言
//*****************************************************************************

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CTexture
{	// テクスチャ管理クラス
public:
	typedef enum
	{
		// UI
		TEXTURE_NUMBER = 0,				// 数字
		TEXTURE_BG_00,							// 背景１
		TEXTURE_BG_01,							// 背景２
		TEXTURE_WEAPON_FLAME,			// 武器のフレーム
		TEXTURE_FLONTLINE,					// タイトル
		TEXTURE_PRESSBOTTON,			// プレスボタン
		TEXTURE_PLAYER_FLAME,			// プレイヤーフレーム
		TEXTURE_CHANGE,						// パート切り替え
		TEXTURE_UPPER_UI,						// BLUE、RED、タイムUI
		TEXTURE_PART_CHANGE,			// アクション、ストラテジー
		TEXTURE_AI_MECHA_FLAME,		// ドローン、ワーカー
		TEXTURE_MOUSECURSOR,			// マウスカーソル
		TEXTURE_RETICLE,						// レティクル
		TEXTURE_RELOAD,						// リロードロゴ（アクションパートで使用）
		TEXTURE_TEAM_WIN,					// 勝利チームのロゴ

		// TEXTURE
		TEXTURE_PRESS_BUTTON,			// 入力待ち
		TEXTURE_BULLET,						// 弾

		// strategy
		TEXTURE_1P_INFO,						// 1Pの小隊情報フレーム
		TEXTURE_2P_INFO,						// 2Pの小隊情報フレーム
		TEXTURE_FLAME,							// ただのフレーム（ストラテジーパートで使用）
		TEXTURE_STRATEGY_BG,				// ストラテジーパートの背景
		TEXTURE_WORKER_BUTTON,		// ウォーカーのアイコン
		TEXTURE_DRONE_BUTTON,			// ドローンのアイコン
		TEXTURE_STRATEGY_ICON,			// ストラテジーのアイコン

		// nechaSelect
		TEXTURE_LOGO,							// メインメニュー、メカセレクトロゴ
		TEXTURE_ADAPT_RANGE,				// 距離表示（機体選択画面で使用）
		TEXTURE_MECHASEL_INST,			// 機体説明（機体選択画面で使用）
		TEXTURE_MECHATYPE,				// 機体の種類（機体選択画面で使用）
		TEXTURE_MECHA_SELECT,			// 機体画像（機体選択画面で使用）

		// menu
		TEXTURE_MENU_FLAME,				// モードフレーム（メニュー画面で使用）
		TEXTURE_MENU_INST,					// モードの概要（メニュー選択画面で使用）
		TEXTURE_MENU_INFOMETION,	// インフォメーション

		// matching
		TEXTURE_MATCHING_BG,			// マッチング画面の背景（マッチング画面で使用）
		TEXTURE_MATCHING_BG_00,		// マッチングBG_00（マッチング画面で使用）
		TEXTURE_MATCHING_BG_01,		// マッチングBG_01（マッチング画面で使用）
		TEXTURE_VS,									// VSロゴ（マッチング画面で使用）
		TEXTURE_PLAYER_NUM,				// プレイヤーナンバー（マッチング画面で使用）
		TEXTURE_BLUE_MECHA_LIST,		// BLUEチームメカリスト（マッチング画面で使用）
		TEXTURE_RED_MECHA_LIST,		// REDチームメカリスト（マッチング画面で使用）

		// respawn
		TEXTURE_START_DEATH,				// 戦闘開始、行動不能ロゴ（アクションパートで使用）
		TEXTURE_TILE_PATTERN,				// タイルアニメーション（アクションパートで使用）
		TEXTURE_RESPAWN,						// 復帰ロゴ（アクションパートで使用）
		TEXTURE_MAP,								// リスポーンマップ（アクションパートで使用）
		TEXTURE_RESPAWN_FLAME,		// リスポーンフレーム（アクションパートで使用）
		TEXTURE_TITLE,							// リスポーンタイトル（アクションパートで使用）
		TEXTURE_SELECT_RESPAWN,		// リスポーン選択（アクションパートで使用）

		// radiochat
		TEXTURE_RADIOCHAT,					// ラジオチャット
		TEXTURE_RADIOCHAT_MESS,		// ラジオチャットメッセージ

		// TutorialMenu
		TEXTURE_TUTORIALMENU_FRAME,			// フレーム
		TEXTURE_TUTORIALMENU_INST,			// 項目説明文

		// Back
		TEXTURE_BACK,

		// Tutorial
		TEXTURE_TUTORIAL_TITLE,					//チュートリアルの現在の項目
		TEXTURE_TUTORIAL_NUMBER,				//チュートリアルの項目数
		TEXTURE_TUTORIAL_MISSION_BASIC,			//チュートリアルの課題(基本操作)
		TEXTURE_TUTORIAL_MISSION_ACTION,		//チュートリアルの課題(アクションパート)
		TEXTURE_TUTORIAL_MISSION_STRATEGY,		//チュートリアルの課題(ストラテジーパート)
		TEXTURE_TUTORIAL_INST_BASIC,			//操作の方法(基本操作)
		TEXTURE_TUTORIAL_INST_ACTION,			//チュートリアルの課題(アクションパート)
		TEXTURE_TUTORIAL_INST_STRATEGY,			//チュートリアルの課題(ストラテジーパート)

		TEXTURE_TUTORIAL_SUCCESS,			//チュートリアル成功！

		// Help
		TEXTURE_HELP_DISP,					//操作方法
		TEXTURE_MANUAL_FLAME,				//操作方法の項目

		TEXTURE_SHADOW,

		TEXTURE_MAX
	}TEXTURE;

	CTexture();
	~CTexture();

	static void Load(void);
	static void Unload(void);

	static LPDIRECT3DTEXTURE9 GetTexture(TEXTURE texture) { return m_pTexture[texture]; }	// テクスチャの取得

private:
	static LPDIRECT3DTEXTURE9 m_pTexture[TEXTURE_MAX];	// テクスチャ
};

#endif