//=============================================================================
//
// UI_Texture処理 [UI_Texture.h]
// Author : Ayaka Hakozaki
//
//=============================================================================
#ifndef _UI_TEXTURE_H_
#define _UI_TEXTURE_H_

#include "scene2D.h"
#include "scene3D.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define UI_PRIORITY	(6)		// 処理の優先順位

//*****************************************************************************
// 前方宣言
//*****************************************************************************

//*****************************************************************************
// 2Dテクスチャクラス定義
//*****************************************************************************
class CUI_TEXTURE : public CScene2D
{
public:
	typedef enum
	{	// フレームの種類
		UIFLAME_NONE,						// フレームなし
		UIFLAME_WEAPON,					// 武器
		UIFLAME_PLAYER_HP,				// プレイヤーの体力
		UIFLAME_DRONE,						// ドローン
		UIFLAME_WORKER,					// AI01
		UIFLAME_TEAM_BLUE,				// BLUEチーム
		UIFLAME_TEAM_RED,				// REDチーム
		UIFLAME_TIMER,						// タイマーフレーム
		UIFLAME_ACTION_PART,			// アクションパート切り替え
		UIFLAME_STRATEGY_PART,		// ストラテジーパート切り替え
		UIFLAME_BG_00,						// Bg00
		UIFLAME_BG_01,						// Bg01
		UIFLAME_FLONTLINE,				// タイトル（フロントライン）
		UIFLAME_PRESSBOTTON,			// プレスボタン
		UIFLAME_CHANGE,					// パート切り替え
		UIFLAME_1P_INFO,					// 1Pの小隊情報（ストラテジーパートで使用）
		UIFLAME_2P_INFO,					// 2Pの小隊情報（ストラテジーパートで使用）
		UIFLAME_FLAME_WHITE,			// ただの白フレーム（ストラテジーパートで使用）
		UIFLAME_FLAME_BLUE,			// ただの青フレーム（ストラテジーパートで使用）
		UIFLAME_STRATEGY_BG,			// ストラテジーパートの背景（ストラテジーパートで使用）
		UIFLAME_RELOAD,					// リロードロゴ（アクションパートで使用）
		UIFLAME_MATCHING_BG,			// マッチング背景（マッチング画面で使用）
		UIFLAME_MATCHING_BG_00,	// マッチングBG_00（マッチング画面で使用）
		UIFLAME_MATCHING_BG_01,	// マッチングBG_01（マッチング画面で使用）
		UIFLAME_VS,								// VSロゴ（マッチング画面で使用）
		UIFLAME_PLAYER_NUM,			// プレイヤーナンバー（マッチング画面で使用）
		UIFLAME_BLUE_MECHA_LIST,	// BLUEチームの機体リスト（マッチング画面で使用）
		UIFLAME_RED_MECHA_LIST,	// REDチームの機体リスト（マッチング画面で使用）
		UIFLAME_START,						// 戦闘開始ロゴ（アクションパートで使用）
		UIFLAME_DEATH,						// 行動不能ロゴ（アクションパートで使用）
		UIFLAME_TILE_PATTERN,			// タイルアニメーション（アクションパートで使用）
		UIFLAME_RESPAWN,					// 復帰ロゴ（アクションパートで使用）
		UIFLAME_MAP,							// リスポーンマップ（アクションパートで使用）
		UIFLAME_RESPAWN_FLAME,	// リスポーンフレーム（アクションパートで使用）
		UIFLAME_TITLE,							// リスポーンタイトル（アクションパートで使用）
		UIFLAME_SELECT_RESPAWN,	// リスポーン選択（アクションパートで使用）
		UIFLAME_MAX							// タイプの最大数
	} UI_TEXTYPE;

	CUI_TEXTURE(int nPriority = UI_PRIORITY, CScene::OBJTYPE objType = CScene::OBJTYPE_UI_TEXTURE);
	~CUI_TEXTURE();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CUI_TEXTURE *Create(D3DXVECTOR3 pos, float fWidth, float fHeight, UI_TEXTYPE UITexType);		// 生成

private:
	D3DXVECTOR3	m_pos;						// 位置
	D3DXCOLOR		m_col;						// カラー
	UI_TEXTYPE		m_UITexType;			// UIの種類を管理
	int						m_nCntBgMove;		// 背景スクロール
	int						m_nFlash;				// 点滅フレーム数
	int						m_nCntAnim, m_nPatternAnim;			// アニメーションのカウンター
};

//*****************************************************************************
// 3Dテクスチャクラス定義
//*****************************************************************************
class CUI_TEXTURE3D : public CScene3D
{
public:
	typedef enum
	{
		UI3DTEX_NONE,
		UI3DTEX_ACTION,
		UI3DTEX_STRATEGY,
		UI3DTEX_MAX
	} UI3DTEX;

	CUI_TEXTURE3D(int nPriority = UI_PRIORITY, CScene::OBJTYPE objType = CScene::OBJTYPE_UI_TEXTURE3D);
	~CUI_TEXTURE3D();
	static CUI_TEXTURE3D *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, UI3DTEX UI3DTex);		// 生成
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	D3DXVECTOR3	m_pos;					// 位置
	D3DXVECTOR3	m_size;				// サイズ
	D3DXCOLOR		m_col;					// カラー
	UI3DTEX				m_UI3dTex;			// 3DUIのタイプ管理
};

#endif