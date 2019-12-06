//=============================================================================
//
// デバッグ情報表示処理 [debugProc.cpp]
// Author : Takuto Ishida
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "debugProc.h"
#include "renderer.h"
#include "manager.h"

#include <stdio.h>
#include <stdarg.h>

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//=========================================
// 静的メンバ変数
//=========================================
LPD3DXFONT CDebugProc::m_pFont[MAX_FONT] = {};		// フォントの種類
char CDebugProc::m_aStr[MAX_DEBUG_TEXT] = {};		// 表示する文字列
bool CDebugProc::m_bDisp = true;					// 表示するかどうか

//=============================================================================
// 文字列格納処理
//=============================================================================
void CDebugProc::Print(char *fmt, ...)
{
	char aSource[MAX_DEBUG_TEXT];

	// 可変長引数を使って文字列にする
	va_list arg;
	va_start(arg, fmt);
	vsprintf(aSource, fmt, arg);
	va_end(arg);

	// 文字列を全角にする
	char aStr[MAX_DEBUG_TEXT];
	LCMapString(0x0411, LCMAP_FULLWIDTH, aSource, -1, aStr, MAX_DEBUG_TEXT);

	int nMaxLength = strlen(m_aStr);	// 今までの文字列の長さを取得
	int nLength = strlen(aStr);			// 今回の文字列の長さを取得

	if (nMaxLength + nLength < MAX_DEBUG_TEXT)
	{// 文字列の長さがオーバーしないようにする
		if (strcmp(&aStr[nLength - 1], "\n") != 0)
		{// 改行がない場合入れる
			strcat(&aStr[nLength], "\n");	// 改行を追加
		}

		// 文字列を追加
		strcat(m_aStr, aStr);
	}
}

//=============================================================================
// 文字列描画処理
//=============================================================================
void CDebugProc::Draw(void)
{
	RECT rect = { 11,11,SCREEN_WIDTH,SCREEN_HEIGHT };	// 表示する座標

	if (m_bDisp)
	{// 表示する
		m_pFont[0]->DrawText(					// 影をつける
			NULL,
			&m_aStr[0],							// 表示する文字列
			-1,									//表示サイズ(-1で全部)
			&rect,								//表示範囲
			DT_LEFT | DT_NOCLIP,				//左詰めで両端揃え
			D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));	//色

		// 位置をずらす
		rect.right -= 1;
		rect.left -= 1;

		m_pFont[0]->DrawText(					// 文字の表示
			NULL,
			&m_aStr[0],							// 表示する文字列
			-1,									//表示サイズ(-1で全部)
			&rect,								//表示範囲
			DT_LEFT | DT_NOCLIP,				//左詰めで両端揃え
			D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));	//色
	}

	// 文字列を空にする
	strcpy(m_aStr, "");
}

//=============================================================================
// 表示・非表示の切り替え
//=============================================================================
void CDebugProc::DebugDisp(bool bDisp)
{
	m_bDisp = bDisp;
}

//=============================================================================
// コンストラクタ
//=============================================================================
CDebugProc::CDebugProc()
{
}

//=============================================================================
// デストラクタ
//=============================================================================
CDebugProc::~CDebugProc()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
void CDebugProc::Init(void)
{
	// デバイスの取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	// 文字列フォントの初期化
	D3DXCreateFont(pDevice,
		18,                     // 文字高さ
		7,                     // 文字幅
		FW_NORMAL,              // フォントスタイル(太さ)
		NULL,                   // ミップマップモデルの数
		FALSE,                  // 斜体にするかどうか
		DEFAULT_CHARSET,		// 文字セット(言語)
		OUT_DEFAULT_PRECIS,
		PROOF_QUALITY,
		FIXED_PITCH | FF_MODERN,
		"JKゴシックM",               //フォントの種類"JKゴシックM"
		&m_pFont[0]);
}

//=============================================================================
// 終了処理
//=============================================================================
void CDebugProc::Uninit(void)
{
	for (int nCntFont = 0; nCntFont < MAX_FONT; nCntFont++)
	{
		if (NULL != m_pFont[nCntFont])
		{// フォントの開放
			m_pFont[nCntFont]->Release();
			m_pFont[nCntFont] = NULL;
		}
	}
}
