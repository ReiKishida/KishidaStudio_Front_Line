//=============================================================================
//
// カメラ処理 [camera.cpp]
// Author : Takuto Ishida
//
//=============================================================================
#include "camera.h"
#include "manager.h"
#include "input.h"
#include "renderer.h"
#include "debugProc.h"
#include "player.h"
#include "game.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define CAMERA_MOVE_HEIGHT	(100.0f)		// プレイヤーとの差で高さを動かす
#define CAMERA_HEIGHT_SPEED	(0.1f)			// 高さを動かす速度
#define CAMERA_ZOOM_LENGTH	(-500.0f)		// ズームしたときの距離
#define CAMERA_LENGTH		(-100.0f)		// カメラの距離
#define CAMERA_ZOOM			(5.0f)			// ズームする速度

//=============================================================================
// コンストラクタ
//=============================================================================
CCamera::CCamera()
{
	m_posV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posVDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posRDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vecU = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fLength = 0.0f;
	m_fLookHeight = 0.0f;
}

//=============================================================================
// デストラクタ
//=============================================================================
CCamera::~CCamera()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CCamera::Init(void)
{
	m_posV = D3DXVECTOR3(0.0f, 0.0f, CAMERA_LENGTH);
	m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fLength = m_posV.z;
	m_fLookHeight = 60.0f;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CCamera::Uninit(void)
{
}

//=============================================================================
// 更新処理
//=============================================================================
void CCamera::Update(void)
{
	if (CManager::GetMode() == CManager::MODE_GAME)
	{
		if (CManager::GetGame()->GetPart() == CGame::PART_ACTION)
		{// アクションパート
			// マウスの入力情報を取得
			CInputMouse *pInputMouse = CManager::GetInputMouse();

			// マウス座標の前回との差分を求める
			float fDiffX = (float)pInputMouse->GetDiffPointX();
			float fDiffY = (float)pInputMouse->GetDiffPointY();

			// プレイヤーの取得
			CPlayer *pPlayer = CManager::GetGame()->GetPlayer();
			D3DXVECTOR3 pos = pPlayer->GetPos();

			// 向きを変える
			m_rot.y += fDiffX * 0.01f;	// 横方向
			m_rot.x += fDiffY * 0.01f;	// 縦方向

			// 差分の調節
			if (m_rot.y > D3DX_PI) { m_rot.y -= D3DX_PI * 2.0f; }
			if (m_rot.y < -D3DX_PI) { m_rot.y += D3DX_PI * 2.0f; }

			// 差分の調節
			if (m_rot.x > D3DX_PI) { m_rot.x -= D3DX_PI * 2.0f; }
			if (m_rot.x < -D3DX_PI) { m_rot.x += D3DX_PI * 2.0f; }

			// x軸の回転の制御
			if (D3DX_PI * 0.4f > m_rot.x)
			{// 上側
				m_rot.x = D3DX_PI * 0.4f;
			}
			else if (D3DX_PI * 0.55f < m_rot.x)
			{// 下側
				m_rot.x = D3DX_PI * 0.55f;
			}

			// 注視点
			m_posR.x = pos.x - sinf(m_rot.y) * m_fLength;
			m_posR.z = pos.z - cosf(m_rot.y) * m_fLength;
			m_posR.y = pos.y - cosf(m_rot.x) * m_fLength + m_fLookHeight;

			// 視点
			m_posV.x = pos.x + sinf(m_rot.y) * m_fLength;
			m_posV.z = pos.z + cosf(m_rot.y) * m_fLength;
			m_posV.y = pos.y + cosf(m_rot.x) * m_fLength + m_fLookHeight;
		}
		else
		{// ストラテジーパート
			m_posV = D3DXVECTOR3(0.0f, 1000.0f, -100.0f);
			m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			m_rot.y = D3DX_PI;
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CCamera::Draw(void)
{
}

//=============================================================================
// カメラの設定処理
//=============================================================================
void CCamera::Set(void)
{
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();		// デバイスの取得

	// プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxProjection);

	CGame *pGame = CManager::GetGame();
	if (NULL != pGame)
	{// プロジェクションマトリックスを作成
		if (pGame->GetPart() == CGame::PART_ACTION)
		{// アクションパート
			D3DXMatrixPerspectiveFovLH(&m_mtxProjection,
				D3DXToRadian(45.0f),							// 画角
				(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,		// アスペクト比
				10.0f,											// カメラの範囲（手前）
				8000.0f);										// カメラの範囲（奥行）
		}
		else if(pGame->GetPart() == CGame::PART_STRATEGY)
		{// ストラテジーパート
			D3DXMatrixOrthoLH(&m_mtxProjection, (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT, 10.0f, 8000.0f);
		}
	}
	else
	{// プロジェクションマトリックスを作成
		D3DXMatrixPerspectiveFovLH(&m_mtxProjection,
			D3DXToRadian(45.0f),							// 画角
			(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,		// アスペクト比
			10.0f,											// カメラの範囲（手前）
			8000.0f);										// カメラの範囲（奥行）
	}
	// プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);

	// ビューマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxView);

	// ビューマトリックスを作成
	D3DXMatrixLookAtLH(&m_mtxView,
		&m_posV,
		&m_posR,
		&m_vecU);

	// ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);
}
