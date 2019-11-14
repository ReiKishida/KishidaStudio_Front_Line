//=============================================================================
//
// オブジェクト処理 [scene.h]
// Author : Takuto Ishida
//
//=============================================================================
#ifndef _SCENE_H_
#define _SCENE_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_OBJECT		(128)		// ポリゴンの最大数
#define NUM_PRIORITY	(8)			// 優先順位の数

//=====================================
// クラス定義
//=====================================
class CScene
{
public:
	typedef enum
	{
		OBJTYPE_NONE = 0,			// 初期値
		OBJTYPE_TITLE,				// タイトル
		OBJTYPE_MENU,				// メニュー
		OBJTYPE_MECHASELECT,		// 機体選択
		OBJTYPE_MATCHING,			// マッチング
		OBJTYPE_TUTORIAL,			// チュートリアル
		OBJTYPE_GAME,				// ゲーム
		OBJTYPE_RESULT,				// リザルト
		OBJTYPE_RANKING,			// ランキング
		OBJTYPE_FADE,				// フェード
		OBJTYPE_PLAYER,				// プレイヤー
		OBJTYPE_ENEMY,				// 敵
		OBJTYPE_MODEL,				// モデル
		OBJTYPE_BG,					// 背景
		OBJTYPE_FIELD,				// フィールド
		OBJTYPE_BULLET,				// 弾
		OBJTYPE_BULLET_COLLISION,	// 弾の当たり判定
		OBJTYPE_SHADOW,				// 影
		OBJTYPE_ORBIT,				// 軌跡
		OBJTYPE_GAUGE,				// ゲージ
		OBJTYPE_LIFE,				// ライフ
		OBJTYPE_PAUSE,				// ポーズ
		OBJTYPE_STAGE,				// ステージ
		OBJTYPE_MOTION,				// モーション
		OBJTYPE_MOTION_MANAGER,		// モーション管理
		OBJTYPE_LOGO,				// ロゴ
		OBJTYPE_SETMANAGER,			// 配置物管理
		OBJTYPE_MODELOBJECT,		// 配置物
		OBJTYPE_TIME,				// タイム
		OBJTYPE_PARTICLE,			// パーティクル
		OBJTYPE_NUMBER,				// 数字
		OBJTYPE_MOUSE,				// マウスカーソル
		OBJTYPE_BUTTON,				// ボタン
		OBJTYPE_BUTTONLINE,			// ボタンをつなぐ線
		OBJTYPE_BUTTON_MANAGER,		// ボタンの管理
		OBJTYPE_ITEM,				// アイテム
		OBJTYPE_UI_NUMBER,
		OBJTYPE_UI_TEXTURE,
		OBJTYPE_UI_TEXTURE3D,
		OBJTYPE_COLLISION,
		OBJTYPE_MAX					// オブジェクトの種類の最大数
	}OBJTYPE;

	CScene(int nPriority = 3, OBJTYPE objType = OBJTYPE_NONE);
	virtual ~CScene();
	virtual HRESULT Init(void) = 0;
	virtual void Uninit(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;

	static void ReleaseAll(void);
	static void UpdateAll(void);
	static void DrawAll(void);
	static void ReleaseDeath(void);

	static void ModeUninit(void);
	static void StageUninit(void);
	static void UIUninit(void);

	void SwapPriority(int nPriority);		// 優先順位の変更

	void SetObjType(OBJTYPE objType) { m_objType = objType; }
	OBJTYPE GetObjType(void) { return m_objType; }

	static CScene *GetSceneTop(int nPriority) { return m_apTop[nPriority]; }
	CScene *GetSceneNext(void) { return m_pNext; }

protected:
	void Release(void);

private:
	static CScene *m_apTop[NUM_PRIORITY];		// 先頭のオブジェクトへのポインタ
	static CScene *m_apCur[NUM_PRIORITY];		// 現在のオブジェクトへのポインタ（最後尾）
	CScene *m_pPrev;			// 前のオブジェクトへのポインタ
	CScene *m_pNext;			// 次のオブジェクトへのポインタ
	int m_nPriority;			// 優先順位
	OBJTYPE m_objType;			// オブジェクトの種類
	bool m_bDeath;				// 死亡フラグ
};

#endif