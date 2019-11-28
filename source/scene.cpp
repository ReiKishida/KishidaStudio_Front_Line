//=============================================================================
//
// オブジェクト処理 [scene.cpp]
// Author : Takuto Ishida
//
//=============================================================================
#include "scene.h"
#include "debugProc.h"

//==================================
// 静的メンバ変数宣言
//==================================
CScene *CScene::m_apTop[NUM_PRIORITY] = {};	// 先頭オブジェクト
CScene *CScene::m_apCur[NUM_PRIORITY] = {};	// 現在のオブジェクト（最後尾）

//==================================
// 死亡フラグを立てる
//==================================
void CScene::Release(void)
{
	// 死亡フラグを立てる
	m_bDeath = true;
}

//==================================
// オブジェクトの解放処理
//==================================
void CScene::ReleaseDeath(void)
{
	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{// 優先順位分まわす
		CScene *pScene = m_apTop[nCntPriority];	// 先頭から始める

		while (pScene != NULL)
		{// NULLでない限り回す
			CScene *pSceneNext = pScene->m_pNext;	// 更新内で削除された時のために値を保持
			if (pScene->m_bDeath)
			{// 死亡フラグチェック
				if (pScene->m_pPrev != NULL)
				{// 前のオブジェクトに次のオブジェクトのポインタを渡す
					pScene->m_pPrev->m_pNext = pScene->m_pNext;	// 前のオブジェクトのm_pNextに次のオブジェクトのポインタを入れる
				}

				if (pScene->m_pNext != NULL)
				{// 次のオブジェクトに前のオブジェクトのポインタを渡す
					pScene->m_pNext->m_pPrev = pScene->m_pPrev;
				}

				if (pScene == m_apTop[nCntPriority])
				{// thisが先頭だったとき
					m_apTop[nCntPriority] = pScene->m_pNext;	// 次のポインタを入れる
				}
				if (pScene == m_apCur[nCntPriority])
				{// thisが最後尾だったとき
					m_apCur[nCntPriority] = pScene->m_pPrev;	// 前のポインタを入れる
				}

				// メモリの開放
				delete pScene;
			}
			// 次のオブジェクトを入れる
			pScene = pSceneNext;
		}
	}
}

//==================================
// 全てのポリゴンの解放処理
//==================================
void CScene::ReleaseAll(void)
{
	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{// 優先順位分まわす
		CScene *pScene = m_apTop[nCntPriority];	// 先頭から始める

		while (pScene != NULL)
		{// オブジェクトを先頭から消していく
			CScene *pSceneNext = pScene->m_pNext;	// 更新内で削除された時のために値を保持
			if (!pScene->m_bDeath) { pScene->Uninit(); }	// 終了処理
			pScene = pSceneNext;					// 次のオブジェクトを入れる
		}
	}

	// 削除する
	ReleaseDeath();
}

//==================================
// 画面遷移時の破棄
//==================================
void CScene::ModeUninit(void)
{
	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{// 優先順位分まわす
		CScene *pScene = m_apTop[nCntPriority];	// 先頭から始める

		while (pScene != NULL)
		{// オブジェクトを先頭から消していく
			CScene *pSceneNext = pScene->m_pNext;	// 更新内で削除された時のために値を保持

			if (OBJTYPE_FADE != pScene->m_objType)
			{// フェード以外の死亡フラグを立てる
				pScene->Uninit();					// 終了処理
			}

			pScene = pSceneNext;					// 次のオブジェクトを入れる
		}
	}

	// 削除する
	ReleaseDeath();
}

//==================================
// UIの破棄
//==================================
void CScene::UIUninit(void)
{
	CScene *pScene = m_apTop[6];	// 先頭から始める

	while (pScene != NULL)
	{// オブジェクトを先頭から消していく
		CScene *pSceneNext = pScene->m_pNext;	// 更新内で削除された時のために値を保持

		if (OBJTYPE_UI_TEXTURE == pScene->m_objType || OBJTYPE_UI_NUMBER == pScene->m_objType)
		{// フェード以外の死亡フラグを立てる
			pScene->Uninit();		// 終了処理
		}

		pScene = pSceneNext;					// 次のオブジェクトを入れる
	}

	// 削除する
	ReleaseDeath();
}

//==================================
// 全てのオブジェクトの更新処理
//==================================
void CScene::UpdateAll(void)
{
	int nCntPriorities[NUM_PRIORITY] = {};

	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{// 優先順位分まわす
		CScene *pScene = m_apTop[nCntPriority];	// 先頭から始める

		while (pScene != NULL)
		{// NULLでない限り回す
			nCntPriorities[nCntPriority]++;			// デバッグ用



			CScene *pSceneNext = pScene->m_pNext;			// 更新内で削除された時のために値を保持
			if (!pScene->m_bDeath) { pScene->Update(); }	// 更新処理
			pScene = pSceneNext;							// 次のオブジェクトを入れる
		}
	}

	// 死亡フラグチェック
	ReleaseDeath();
}

//==================================
// 全てのオブジェクトの描画処理
//==================================
void CScene::DrawAll(void)
{
	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{// 優先順位分まわす
		CScene *pScene = m_apTop[nCntPriority];	// 先頭から始める

		while (pScene != NULL)
		{// NULLでない限り回す
			CScene *pSceneNext = pScene->m_pNext;		// 更新内で削除された時のために値を保持
			if (!pScene->m_bDeath) { pScene->Draw(); }	// 描画処理
			pScene = pSceneNext;						// 次のオブジェクトを入れる
		}
	}
}

//==================================
// ストラテジー画面のオブジェクトの描画処理
//==================================
void CScene::DrawStrategy(void)
{
	CScene *pScene = m_apTop[6];	// 先頭から始める

	while (pScene != NULL)
	{// NULLでない限り回す
		CScene *pSceneNext = pScene->m_pNext;		// 更新内で削除された時のために値を保持

		if (OBJTYPE_BUTTON == pScene->m_objType || OBJTYPE_BUTTONLINE == pScene->m_objType)
		{// ボタンとボタンをつなぐ線
			if (!pScene->m_bDeath) { pScene->Draw(); }	// 描画処理
		}

		pScene = pSceneNext;						// 次のオブジェクトを入れる
	}
}

//=========================================
// コンストラクタ
//=========================================
CScene::CScene(int nPriority, OBJTYPE objType)
{
	if (m_apTop[nPriority] == NULL)
	{// 先頭を入れる
		m_apTop[nPriority] = this;
	}

	if (this->m_pPrev == m_apTop[nPriority])
	{// トップの次のオブジェクトにポインタを渡す
		m_apTop[nPriority]->m_pNext = this;
	}

	m_pPrev = m_apCur[nPriority];	// ひとつ前のポインタを入れる
	if (m_pPrev != NULL)
	{// 前のオブジェクトが空でないとき
		m_pPrev->m_pNext = this;	// 前のオブジェクトの次のオブジェクトに設定
	}
	m_pNext = NULL;					// 次のポインタにNULLを入れる
	m_objType = objType;			// オブジェクトの種類に初期値を入れる
	m_apCur[nPriority] = this;		// カレント（最後尾）を更新
	m_bDeath = false;				// 死亡フラグを偽にする
	m_nPriority = nPriority;		// 優先順位
}

//=========================================
// デストラクタ
//=========================================
CScene::~CScene()
{
}

//=========================================
// 優先順位の変更
//=========================================
void CScene::SwapPriority(int nPriority)
{
	CScene *pScene = m_apTop[nPriority];	// 先頭から始める

	while (pScene != NULL)
	{// NULLでない限り回す
		CScene *pSceneNext = pScene->m_pNext;	// 更新内で削除された時のために値を保持

		if (pSceneNext == NULL)
		{// 優先順位の入れ替え
			if (m_pPrev != NULL)
			{// 前のオブジェクトに次のオブジェクトのポインタを渡す
				m_pPrev->m_pNext = m_pNext;	// 前のオブジェクトのm_pNextに次のオブジェクトのポインタを入れる
			}

			if (m_pNext != NULL)
			{// 次のオブジェクトに前のオブジェクトのポインタを渡す
				m_pNext->m_pPrev = m_pPrev;
			}

			if (m_apTop[m_nPriority] == this)
			{// 先頭だったとき
				m_apTop[m_nPriority] = m_pNext;
			}
			if (m_apCur[m_nPriority] == this)
			{// 最後尾だったとき
				m_apCur[m_nPriority] = m_pPrev;
			}

			if (m_apTop[nPriority] == NULL)
			{// 先頭がNULLだったとき
				m_apTop[nPriority] = this;
				m_apCur[nPriority] = this;
			}

			m_pPrev = m_apCur[nPriority];	// ひとつ前のポインタを入れる
			if (m_pPrev != NULL)
			{// 前のオブジェクトが空でないとき
				m_pPrev->m_pNext = this;	// 前のオブジェクトの次のオブジェクトに設定
			}

			if (m_pPrev == m_apTop[nPriority])
			{// トップの次のオブジェクトにポインタを渡す
				m_apTop[nPriority]->m_pNext = this;
			}

			pScene->m_pNext = this;		// 自分をいれる
										//m_pPrev = pScene;			// 前を入れる
			m_pNext = NULL;				// 次を空にする
			m_apCur[nPriority] = this;	// 最後尾を自分にする
			m_nPriority = nPriority;	// 優先順位を変える
			return;
		}

		pScene = pSceneNext;			// 次のオブジェクトを入れる
	}

	if (pScene == NULL)
	{// 全てNULLだったときとき
		if (m_apTop[m_nPriority] == this)
		{// 先頭だったとき
			m_apTop[m_nPriority] = m_pNext;
		}
		if (m_apCur[m_nPriority] == this)
		{// 最後尾だったとき
			m_apCur[m_nPriority] = m_pPrev;
		}

		pScene = this;				// 自分をいれる
		this->m_pPrev = NULL;		// 前を空にする
		this->m_pNext = NULL;		// 次を空にする
		m_apTop[nPriority] = this;	// 先頭を自分にする
		m_apCur[nPriority] = this;	// 最後尾を自分にする
		m_nPriority = nPriority;	// 優先順位を変える
	}
}