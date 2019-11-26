//=============================================================================
//
// ボタン処理 [button.h]
// Author : Takuto Ishida
//
//=============================================================================
#ifndef _BUTTON_H_
#define _BUTTON_H_

#include "scene3D.h"
#include "scene2D.h"
#include "line.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define BUTTON_PRIORITY			(6)		// 処理の優先番号
#define BUTTON_MANAGER_PRIORITY	(6)		// 処理の優先番号

//*****************************************************************************
// 前方宣言
//*****************************************************************************

//*****************************************************************************
// クラス定義
//*****************************************************************************
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 2Dボタンのクラス
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class CButton2D : public CScene2D
{
public:
	CButton2D(int nPriority = BUTTON_PRIORITY, CScene::OBJTYPE objType = CScene::OBJTYPE_BUTTON);
	~CButton2D();
	HRESULT Init(D3DXVECTOR3 pos, float fWidth, float fHeight);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CButton2D* Create(D3DXVECTOR3 pos, float fWidth, float fDepth);

	bool InRange(D3DXVECTOR3 pos);
	bool GetInRange(void) { return m_bInRange; }

	bool Click(void);
	bool ClickRelease(void);

	bool GetSwitch(void) { return m_bSwitch; }
	void SetSwitch(bool bSwitch) { m_bSwitch = bSwitch; }

private:
	bool m_bSwitch;			// 起動しているかどうか
	bool m_bInRange;		// 範囲内かどうか
	bool m_bRelease;		// リリースの判定に必要
	D3DXVECTOR2 m_size;		// 大きさ
};

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 3Dボタンのクラス
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class CButton3D : public CScene3D
{
public:
	CButton3D(int nPriority = BUTTON_PRIORITY, CScene::OBJTYPE objType = CScene::OBJTYPE_BUTTON);
	~CButton3D();
	HRESULT Init(D3DXVECTOR3 pos, float fWidth, float fDepth);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CButton3D* Create(D3DXVECTOR3 pos, float fWidth, float fDepth);

	bool InRange(D3DXVECTOR3 pos);
	bool GetInRange(void) { return m_bInRange; }

	bool Click(void);
	bool ClickRelease(void);

	bool GetSwitch(void) { return m_bSwitch; }
	void SetSwitch(bool bSwitch) { m_bSwitch = bSwitch; }

	bool GetDisp(void) { return m_bDisp; };
	void SetDisp(bool bDisp) { m_bDisp = bDisp; };

private:
	bool m_bSwitch;			// 起動しているかどうか
	bool m_bInRange;		// 範囲内かどうか
	bool m_bRelease;		// リリースの判定に必要
	D3DXVECTOR2 m_size;		// 大きさ
	bool m_bDisp;			// 表示・非表示
};

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ボタンをつなぐ線のクラス
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class CButtonLine : public CScene2D
{
public:
	CButtonLine(int nPriority = BUTTON_PRIORITY, CScene::OBJTYPE objType = CScene::OBJTYPE_BUTTONLINE);
	~CButtonLine();
	HRESULT Init(D3DXVECTOR3 start);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CButtonLine *Create(D3DXVECTOR3 start);

	bool GetBoolLink(void) { return m_bLink; };
	void SetBoolLink(bool bLink) { m_bLink = bLink; };

	void Link(D3DXVECTOR3 end);

private:
	D3DXVECTOR3 m_start;		// 開始位置
	bool m_bLink;				// 接続したかどうか
};

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ボタンの管理クラス
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class CButtonManagerStrategy : public CScene
{
public:
	CButtonManagerStrategy(int nPriority = BUTTON_MANAGER_PRIORITY, CScene::OBJTYPE objType = CScene::OBJTYPE_BUTTON_MANAGER);
	~CButtonManagerStrategy();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CButtonManagerStrategy *Create(void);

	void ButtonUninit(int nLogic);

	int GetSelectAIType(void) { return m_nSelectAIType; }
	int GetSelectLogic(int nHierarchy) { return m_aSelectLogic[nHierarchy]; }

	bool &GetSelectFinish(void) { return m_bFinish; }

private:
	void AITypeSelect(void);
	void FirstHierarchy(void);
	void SecondHierarchy(void);
	void ThirdHierarchy(void);
	void FourthHierarchy(void);

	CButton2D *m_pAIType[2];		// ドローンかワーカーどちらか
	CButton2D **m_pFirst;			// ロジックツリーの１階層目のボタン
	CButton2D **m_pSecond;			// ロジックツリーの２階層目のボタン
	CButton2D **m_pThird;			// ロジックツリーの３階層目のボタン
	CButton2D **m_pFourth;			// ロジックツリーの４階層目のボタン

	CButtonLine *m_pLine[4];		// ボタン同士をつなぐ線のクラス

	int m_aNumLogic[4];				// ロジックツリーの階層ごとのボタンの数

	int m_nSelectAIType;			// ドローンかワーカーの選択
	int m_aSelectLogic[4];			// 階層ごとの選択ボタン

	CScene2D *m_apSelectIcon[4];	// 選択した項目を表示

	bool	m_bFinish;				// 選択が終了した

#ifdef _DEBUG
	bool	m_bDisp;
#endif

};

#endif