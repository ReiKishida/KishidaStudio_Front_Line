//=============================================================================
//
// ライト処理 [light.cpp]
// Author : Takuto Ishida
//
//=============================================================================
#include "light.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "debugProc.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************
D3DLIGHT9 CLight::m_alight[MAX_LIGHT] = {};

//=============================================================================
// コンストラクタ
//=============================================================================
CLight::CLight()
{
	for (int nCntDirect = 0; nCntDirect < MAX_LIGHT; nCntDirect++)
	{
		m_direct[nCntDirect] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
	m_nSelectLight = 0;
}

//=============================================================================
// デストラクタ
//=============================================================================
CLight::~CLight()
{
}

//=============================================================================
// ライトの初期化処理
//=============================================================================
void CLight::Init(void)
{
	// デバイスの取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	for (int nCntLight = 0; nCntLight < MAX_LIGHT; nCntLight++)
	{
		// ライトをクリアする
		ZeroMemory(&m_alight[nCntLight], sizeof(D3DLIGHT9));

		// ライトの種類を設定
		m_alight[nCntLight].Type = D3DLIGHT_DIRECTIONAL;
	}

	// ライトの拡散光を設定
	m_alight[0].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_alight[1].Diffuse = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
	m_alight[2].Diffuse = D3DXCOLOR(0.45f, 0.45f, 0.45f, 1.0f);

	// ライトの方向の設定
	m_direct[0] = D3DXVECTOR3(0.81f, -0.9f, 0.20f);
	D3DXVec3Normalize(&m_direct[0], &m_direct[0]);	// 正規化する
	m_alight[0].Direction = m_direct[0];

	// ライトの方向の設定
	m_direct[1] = D3DXVECTOR3(-0.58f, 0.88f, -0.44f);
	D3DXVec3Normalize(&m_direct[1], &m_direct[1]);	// 正規化する
	m_alight[1].Direction = m_direct[1];

	// ライトの方向の設定
	m_direct[2] = D3DXVECTOR3(0.89f, -0.11f, 0.44f);
	D3DXVec3Normalize(&m_direct[2], &m_direct[2]);	// 正規化する
	m_alight[2].Direction = m_direct[2];

	for (int nCntLight = 0; nCntLight < MAX_LIGHT; nCntLight++)
	{
		// ライトを設定する
		pDevice->SetLight(nCntLight, &m_alight[nCntLight]);

		// ライトを有効にする
		pDevice->LightEnable(nCntLight, TRUE);
	}
}

//=============================================================================
// ライトの終了処理
//=============================================================================
void CLight::Uninit(void)
{
}

//=============================================================================
// ライトの更新処理
//=============================================================================
void CLight::Update(void)
{
#ifdef _DEBUG
	// デバイスの取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();	// キーボードの取得

	if (pKeyboard->GetRepeat(DIK_T))
	{// 選択を上げる
		m_nSelectLight = (1 + m_nSelectLight) % MAX_LIGHT;
	}
	if (pKeyboard->GetRepeat(DIK_G))
	{// 選択を下げる
		m_nSelectLight = (1 + (MAX_LIGHT - 1)) % MAX_LIGHT;
	}

	// X
	if (pKeyboard->GetRepeat(DIK_Y))
	{// 上げる
		m_fAngle[m_nSelectLight] += D3DX_PI * 0.01f;
		if (m_fAngle[m_nSelectLight] > D3DX_PI) { m_fAngle[m_nSelectLight] = -D3DX_PI; }

		m_direct[m_nSelectLight].x += sinf(D3DX_PI * m_fAngle[m_nSelectLight]);
		m_direct[m_nSelectLight].y += cosf(D3DX_PI * m_fAngle[m_nSelectLight]);
	}
	if (pKeyboard->GetRepeat(DIK_H))
	{// 下げる
		m_fAngle[m_nSelectLight] -= D3DX_PI * 0.01f;
		if (m_fAngle[m_nSelectLight] < -D3DX_PI) { m_fAngle[m_nSelectLight] = D3DX_PI; }

		m_direct[m_nSelectLight].x += sinf(D3DX_PI * m_fAngle[m_nSelectLight]);
		m_direct[m_nSelectLight].y += cosf(D3DX_PI * m_fAngle[m_nSelectLight]);
	}

	// Z
	if (pKeyboard->GetRepeat(DIK_U))
	{// 上げる
		m_fAngle[m_nSelectLight] += D3DX_PI * 0.01f;
		if (m_fAngle[m_nSelectLight] > D3DX_PI) { m_fAngle[m_nSelectLight] = -D3DX_PI; }

		m_direct[m_nSelectLight].z += sinf(D3DX_PI * m_fAngle[m_nSelectLight]);
		m_direct[m_nSelectLight].y += cosf(D3DX_PI * m_fAngle[m_nSelectLight]);
	}
	if (pKeyboard->GetRepeat(DIK_J))
	{// 下げる
		m_fAngle[m_nSelectLight] -= D3DX_PI * 0.01f;
		if (m_fAngle[m_nSelectLight] < -D3DX_PI) { m_fAngle[m_nSelectLight] = D3DX_PI; }

		m_direct[m_nSelectLight].z += sinf(D3DX_PI * m_fAngle[m_nSelectLight]);
		m_direct[m_nSelectLight].y += cosf(D3DX_PI * m_fAngle[m_nSelectLight]);
	}

	D3DXVec3Normalize(&m_direct[m_nSelectLight], &m_direct[m_nSelectLight]);	// 正規化する
	m_alight[m_nSelectLight].Direction = m_direct[m_nSelectLight];

	// ライトを設定する
	pDevice->SetLight(m_nSelectLight, &m_alight[m_nSelectLight]);

	// ライトを有効にする
	pDevice->LightEnable(m_nSelectLight, TRUE);

	CDebugProc::Print("\nライト%d", m_nSelectLight);
	CDebugProc::Print("ライトX：%.2f", m_direct[m_nSelectLight].x);
	CDebugProc::Print("ライトY：%.2f", m_direct[m_nSelectLight].y);
	CDebugProc::Print("ライトZ：%.2f\n", m_direct[m_nSelectLight].z);
#endif
}
