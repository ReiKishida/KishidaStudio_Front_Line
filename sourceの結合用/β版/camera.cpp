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
#include "tutorial.h"
#include "server.h"
#include "menu.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define CAMERA_LENGTH		(-85.0f)		// カメラの距離
#define CAMERA_WHOLE_LENGTH		(-4500.0f)		// カメラが全体を見渡すの距離

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
	m_viewport.Height = SCREEN_HEIGHT;
	m_viewport.Width = SCREEN_WIDTH;
	m_viewport.MaxZ = 1.0f;
	m_viewport.MinZ = 0.0f;
	m_viewport.X = 0;
	m_viewport.Y = 0;
	m_fZoom = 0.0f;
	m_fCameraSpeed = 0.0f;
	m_nCntStarting = 0;
	m_stageCamera = STAGE_CAMERA_STRAIGHT;
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
	m_rot = D3DXVECTOR3(D3DX_PI * 0.5f, 0.0f, 0.0f);
	m_fLength = m_posV.z;
	m_fLookHeight = 25.0f;
	m_fZoom = 1.0f;
	m_fCameraSpeed = 0.0042f;		// カメラの初期速度設定
	m_nCntStarting = 0;
	m_stageCamera = STAGE_CAMERA_STRAIGHT;

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
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();	// キーボードの入力を取得
	CInputMouse *pInputMouse = CManager::GetInputMouse();	// マウスの入力情報を取得

	if (CManager::GetMode() == CManager::MODE_RESULT)
	{
		m_posV = D3DXVECTOR3(78.9f, 40.0f, -212.4f);
		m_posR = D3DXVECTOR3(-700.0f, 0.0f, 500.0f);
	}

	if (CManager::GetMode() == CManager::MODE_GAME)
	{//モードがゲームの場合
		if (CManager::GetGame()->GetGameState() == CGame::STATE_STARTING)
		{//ゲームの状態が開始前の場合
			if (m_nCntStarting == 0)
			{
				m_posV = D3DXVECTOR3(-2500.0f, 50.0f, 0.0f);
				m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			}
			if (m_nCntStarting == 200)
			{//200にカウンターが達した場合
				m_stageCamera = STAGE_CAMERA_TURN;	//回転状態にする
				m_posV = D3DXVECTOR3(0.0f, 400.0f, -0.0f);
				m_posR = D3DXVECTOR3(-110.0f, 500.0f, -1000.0f);
			}
			if (m_nCntStarting == 500)
			{//500にカウンターが達した場合
				m_stageCamera = STAGE_CAMERA_OVERLOOK;	//見下ろし状態にする
				m_posV = D3DXVECTOR3(1500.0f, 50.0f, 0.0f);
				m_posR = D3DXVECTOR3(-0.0f, 50.0f, -0.0f);
				//m_posV = D3DXVECTOR3(2500.0f, 800.0f, 0.0f);
				//m_posR = D3DXVECTOR3(-0.0f, -50.0f, -0.0f);
				//m_rot.y = D3DX_PI * 0.5f;

			}
			if (m_nCntStarting == 900)
			{
				m_stageCamera = STAGE_CAMERA_STOP;	//停止状態にする

				//ゲームを開始前フェードアウトにする
				CManager::GetGame()->SetGameState(CGame::STATE_STARTING_FADE_OUT);
			}

			m_nCntStarting++;	//カウンターの加算

			//カメラ別の処理分け
			switch (m_stageCamera)
			{
			case STAGE_CAMERA_STRAIGHT:
				m_posV.x += 2.5f;
				m_posR.x += 2.5f;
				break;

			case STAGE_CAMERA_TURN:
				m_rot.y -= 0.005f;
				if (m_rot.y >= D3DX_PI)
				{
					m_rot.y = -D3DX_PI;
				}
				m_posV.x = m_posR.x - sinf(m_rot.y + (D3DX_PI * 0.25f)) * - 750.0f;
				m_posV.z = m_posR.z - cosf(m_rot.y + (D3DX_PI * 0.25f)) * -750.0f;
				break;

			case STAGE_CAMERA_OVERLOOK:
				m_posV.x += 5.0f;
				m_posV.y += 2.5f;
				m_posR.y -= 0.25f;
				/*m_rot.y -= 0.005f;
				if (m_rot.y >= D3DX_PI)
				{
					m_rot.y = -D3DX_PI;
				}
				m_posV.x = m_posR.x - sinf(m_rot.y + (D3DX_PI * 0.5f)) * -2000.0f;
				m_posV.z = m_posR.z - cosf(m_rot.y + (D3DX_PI * 0.5f)) * -2000.0f;*/
				break;
			}
		}

		if (CManager::GetGame()->GetGameState() == CGame::STATE_NORMAL || CManager::GetGame()->GetGameState() == CGame::STATE_STARTING_FADE_IN)
		{
			if (CManager::GetGame()->GetPart() == CGame::PART_ACTION)
			{// アクションパート
				CPlayer *pPlayer = NULL;

				// プレイヤーの取得
				if (CMenu::GetMode() == CMenu::MODE_MULTI)
				{// マルチ
					if (CManager::GetClient() != NULL && CManager::GetClient()->GetConnect() == true)
					{
						pPlayer = CManager::GetGame()->GetPlayer(CManager::GetClient()->GetPlayerIdx());
					}
					else
					{
						pPlayer = CManager::GetGame()->GetPlayer(0);
					}
				}
				else
				{// シングル
					pPlayer = CManager::GetGame()->GetPlayer(0);
				}

				D3DXVECTOR3 pos = pPlayer->GetPos();
				float fModelHeight = pPlayer->GetVtxMax().y;
				bool bChat = pPlayer->GetChatBotton();
				bool bOption = pPlayer->GetOption();							// オプション中
				int nSelectButton = pPlayer->GetSelectOption();			// カメラ速度の取得

				switch (nSelectButton)
				{
				case 0:		// 最低速
					m_fCameraSpeed = 0.0021f;
					break;

				case 1:		// 低速
					m_fCameraSpeed = 0.0032f;
					break;

				case 2:		// 普通
					m_fCameraSpeed = 0.0042f;
					break;

				case 3:		// 高速
					m_fCameraSpeed = 0.0052f;
					break;

				case 4:		// 最高速
					m_fCameraSpeed = 0.0062f;
					break;
				}

				if (pPlayer->GetRespawn() == CPlayer::RESPAWN_NONE && bChat == false && bOption == false && !pPlayer->GetWince())
				{
					// マウス座標の前回との差分を求める
					float fDiffX = (float)pInputMouse->GetDiffPointX();
					float fDiffY = (float)pInputMouse->GetDiffPointY();

					// 向きを変える
					m_rot.y += fDiffX * m_fCameraSpeed;	// 横方向
					m_rot.x += fDiffY * m_fCameraSpeed;	// 縦方向
				}

				// 差分の調節
				if (m_rot.y > D3DX_PI) { m_rot.y -= D3DX_PI * 2.0f; }
				if (m_rot.y < -D3DX_PI) { m_rot.y += D3DX_PI * 2.0f; }

				// 差分の調節
				if (m_rot.x > D3DX_PI) { m_rot.x -= D3DX_PI * 2.0f; }
				if (m_rot.x < -D3DX_PI) { m_rot.x += D3DX_PI * 2.0f; }

				// x軸の回転の制御
				if (D3DX_PI * 0.3f > m_rot.x)
				{// 上側
					m_rot.x = D3DX_PI * 0.3f;
				}
				else if (D3DX_PI * 0.55f < m_rot.x)
				{// 下側
					m_rot.x = D3DX_PI * 0.55f;
				}

				// 注視点
				m_posR.x = pos.x - sinf(m_rot.y) * m_fLength;
				m_posR.z = pos.z - cosf(m_rot.y) * m_fLength;
				m_posR.y = pos.y - cosf(m_rot.x) * m_fLength + fModelHeight + m_fLookHeight;

				// 視点
				m_posV.x = pos.x + sinf(m_rot.y) * m_fLength;
				m_posV.z = pos.z + cosf(m_rot.y) * m_fLength;
				m_posV.y = pos.y + cosf(m_rot.x) * m_fLength + fModelHeight + m_fLookHeight;
			}
			else
			{// ストラテジーパート
				float moveX = (float)pInputMouse->GetDiffPointX();
				float moveY = (float)pInputMouse->GetDiffPointY();

				// マップのスクロール
				if (pInputMouse->GetPress(CInputMouse::DIMS_BUTTON_1))
				{// 左CTRLと左クリックでスクロール
					m_posVDest.x -= (moveX / m_fZoom) * 2.0f;
					m_posVDest.z += (moveY / m_fZoom) * 2.0f;

					m_posRDest.x -= (moveX / m_fZoom) * 2.0f;
					m_posRDest.z += (moveY / m_fZoom) * 2.0f;
				}

				// ホイールで拡大・縮小
				if (pInputMouse->GetMouseAxisZ() >= 120.0f && m_fZoom < 3.0f)
				{// 拡大
					m_fZoom++;
				}
				else if (pInputMouse->GetMouseAxisZ() <= -120.0f && m_fZoom > 1.0f)
				{// 縮小
					m_fZoom--;
				}

				// 注視点の更新
				m_posR.x = m_posRDest.x - sinf(D3DX_PI) * 0.1f;
				m_posR.y = m_posRDest.y + 100.0f;
				m_posR.z = m_posRDest.z - cosf(D3DX_PI) * 0.1f;

				// 視点の更新
				m_posV.x = m_posVDest.x + sinf(D3DX_PI) * -0.01f;
				m_posV.y = m_posVDest.y + 500.0f;
				m_posV.z = m_posVDest.z + cosf(D3DX_PI) * -0.01f;
			}
		}
	}
	else if (CManager::GetMode() == CManager::MODE_TUTORIAL)
	{
		if (CManager::GetTutorial()->GetPart() == CTutorial::PART_ACTION)
		{// アクションパート
			CPlayer *pPlayer = NULL;

			// プレイヤーの取得
			pPlayer = CManager::GetTutorial()->GetPlayer(0);

			D3DXVECTOR3 pos = pPlayer->GetPos();
			float fModelHeight = pPlayer->GetVtxMax().y;
			bool bChat = pPlayer->GetChatBotton();
			bool bOption = pPlayer->GetOption();							// オプション中
			int nSelectButton = pPlayer->GetSelectOption();			// カメラ速度の取得

			switch (nSelectButton)
			{
			case 0:		// 最低速
				m_fCameraSpeed = 0.0021f;
				break;

			case 1:		// 低速
				m_fCameraSpeed = 0.0032f;
				break;

			case 2:		// 普通
				m_fCameraSpeed = 0.0042f;
				break;

			case 3:		// 高速
				m_fCameraSpeed = 0.0052f;
				break;

			case 4:		// 最高速
				m_fCameraSpeed = 0.0062f;
				break;
			}

			if (pPlayer->GetRespawn() == CPlayer::RESPAWN_NONE && bChat == false && bOption == false && !pPlayer->GetWince())
			{
				// マウス座標の前回との差分を求める
				float fDiffX = (float)pInputMouse->GetDiffPointX();
				float fDiffY = (float)pInputMouse->GetDiffPointY();

				// 向きを変える
				m_rot.y += fDiffX * m_fCameraSpeed;	// 横方向
				m_rot.x += fDiffY * m_fCameraSpeed;	// 縦方向
			}

			// 差分の調節
			if (m_rot.y > D3DX_PI) { m_rot.y -= D3DX_PI * 2.0f; }
			if (m_rot.y < -D3DX_PI) { m_rot.y += D3DX_PI * 2.0f; }

			// 差分の調節
			if (m_rot.x > D3DX_PI) { m_rot.x -= D3DX_PI * 2.0f; }
			if (m_rot.x < -D3DX_PI) { m_rot.x += D3DX_PI * 2.0f; }

			// x軸の回転の制御
			if (D3DX_PI * 0.3f > m_rot.x)
			{// 上側
				m_rot.x = D3DX_PI * 0.3f;
			}
			else if (D3DX_PI * 0.55f < m_rot.x)
			{// 下側
				m_rot.x = D3DX_PI * 0.55f;
			}

			// 注視点
			m_posR.x = pos.x - sinf(m_rot.y) * m_fLength;
			m_posR.z = pos.z - cosf(m_rot.y) * m_fLength;
			m_posR.y = pos.y - cosf(m_rot.x) * m_fLength + fModelHeight + m_fLookHeight;

			// 視点
			m_posV.x = pos.x + sinf(m_rot.y) * m_fLength;
			m_posV.z = pos.z + cosf(m_rot.y) * m_fLength;
			m_posV.y = pos.y + cosf(m_rot.x) * m_fLength + fModelHeight + m_fLookHeight;
		}
		else
		{// ストラテジーパート
			float moveX = (float)pInputMouse->GetDiffPointX();
			float moveY = (float)pInputMouse->GetDiffPointY();

			// マップのスクロール
			if (pInputMouse->GetPress(CInputMouse::DIMS_BUTTON_1))
			{// 左CTRLと左クリックでスクロール
				m_posVDest.x -= (moveX / m_fZoom) * 2.0f;
				m_posVDest.z += (moveY / m_fZoom) * 2.0f;

				m_posRDest.x -= (moveX / m_fZoom) * 2.0f;
				m_posRDest.z += (moveY / m_fZoom) * 2.0f;
			}

			// ホイールで拡大・縮小
			if (pInputMouse->GetMouseAxisZ() >= 120.0f && m_fZoom < 3.0f)
			{// 拡大
				m_fZoom++;
			}
			else if (pInputMouse->GetMouseAxisZ() <= -120.0f && m_fZoom > 1.0f)
			{// 縮小
				m_fZoom--;
			}

			// 注視点の更新
			m_posR.x = m_posRDest.x - sinf(D3DX_PI) * 0.1f;
			m_posR.y = m_posRDest.y + 100.0f;
			m_posR.z = m_posRDest.z - cosf(D3DX_PI) * 0.1f;

			// 視点の更新
			m_posV.x = m_posVDest.x + sinf(D3DX_PI) * -0.01f;
			m_posV.y = m_posVDest.y + 500.0f;
			m_posV.z = m_posVDest.z + cosf(D3DX_PI) * -0.01f;
		}
	}
	CDebugProc::Print("視点%.2f %.2f %.2f", m_posV.x, m_posV.y, m_posV.z);

	CDebugProc::Print("注視点%.2f %.2f %.2f", m_posR.x, m_posR.y, m_posR.z);

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
void CCamera::Set(int nIdxCamera)
{
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();		// デバイスの取得

	// プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxProjection);

	switch (nIdxCamera)
	{
	case 0:
		m_viewport.Width = SCREEN_WIDTH;
		m_viewport.Height = SCREEN_HEIGHT;
		m_viewport.MaxZ = 1.0f;
		m_viewport.MinZ = 0.0f;
		m_viewport.X = 0;
		m_viewport.Y = 0;
		break;

	case 1:
		m_viewport.Width = 750;
		m_viewport.Height = 450;
		m_viewport.MaxZ = 1.0f;
		m_viewport.MinZ = 0.0f;
		m_viewport.X = SCREEN_WIDTH - 760;
		m_viewport.Y = SCREEN_HEIGHT - 470;
		break;
	}

	// ビューポートの設定
	pDevice->SetViewport(&m_viewport);
	if (NULL != CManager::GetGame())
	{// プロジェクションマトリックスを作成
		if (CManager::GetGame()->GetPart() == CGame::PART_ACTION)
		{// アクションパート
			D3DXMatrixPerspectiveFovLH(&m_mtxProjection,
				D3DXToRadian(45.0f),							// 画角
				(float)m_viewport.Width / (float)m_viewport.Height,		// アスペクト比
				0.1f,											// カメラの範囲（手前）
				12000.0f);										// カメラの範囲（奥行）
		}
		else if (CManager::GetGame()->GetPart() == CGame::PART_STRATEGY)
		{// ストラテジーパート
			if (0 == nIdxCamera)
			{
				D3DXMatrixPerspectiveFovLH(&m_mtxProjection,
					D3DXToRadian(45.0f),							// 画角
					(float)m_viewport.Width / (float)m_viewport.Height,		// アスペクト比
					0.1f,											// カメラの範囲（手前）
					1.0f);										// カメラの範囲（奥行）
			}
			else
			{
				D3DXMatrixOrthoLH(&m_mtxProjection,
					(float)m_viewport.Width / m_fZoom * PARALLEL_PROJECTION_SIZE,
					(float)m_viewport.Height / m_fZoom * PARALLEL_PROJECTION_SIZE,
					10.0f,
					8000.0f);
			}
		}
	}
	else if (NULL != CManager::GetTutorial())
	{// プロジェクションマトリックスを作成
		if (CManager::GetTutorial()->GetPart() == CGame::PART_ACTION)
		{// アクションパート
			D3DXMatrixPerspectiveFovLH(&m_mtxProjection,
				D3DXToRadian(45.0f),							// 画角
				(float)m_viewport.Width / (float)m_viewport.Height,		// アスペクト比
				0.1f,											// カメラの範囲（手前）
				8000.0f);										// カメラの範囲（奥行）
		}
		else if (CManager::GetTutorial()->GetPart() == CGame::PART_STRATEGY)
		{// ストラテジーパート
			if (0 == nIdxCamera)
			{
				D3DXMatrixOrthoLH(&m_mtxProjection,
					(float)m_viewport.Width / m_fZoom * PARALLEL_PROJECTION_SIZE,
					(float)m_viewport.Height / m_fZoom * PARALLEL_PROJECTION_SIZE,
					1.0f,
					1.0f);
			}
			else
			{
				D3DXMatrixOrthoLH(&m_mtxProjection,
					(float)m_viewport.Width / m_fZoom * 2.5f,
					(float)m_viewport.Height / m_fZoom * 2.5f,
					10.0f,
					8000.0f);
			}
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

//=============================================================================
// スクリーン座標をワールド座標に変換
//=============================================================================
D3DXVECTOR3 CCamera::CalcScreenToWorld(int nScreenX, int nScreenY, float fZ)
{
	// 各行列の逆行列を算出
	D3DXMATRIX InvView, InvPrj, VP, InvViewport;
	D3DXMatrixInverse(&InvView, NULL, &m_mtxView);
	D3DXMatrixInverse(&InvPrj, NULL, &m_mtxProjection);
	D3DXMatrixIdentity(&VP);
	VP._11 = 750.0f / 2.0f + (SCREEN_WIDTH - 760); VP._22 = -450.0f / 2.0f + (SCREEN_HEIGHT - 470);
	VP._41 = 750.0f / 2.0f + (SCREEN_WIDTH - 760); VP._42 = 450.0f / 2.0f + (SCREEN_HEIGHT - 470);
	D3DXMatrixInverse(&InvViewport, NULL, &VP);

	// 逆変換
	D3DXMATRIX tmp = InvViewport * InvPrj * InvView;
	D3DXVECTOR3 out;
	D3DXVec3TransformCoord(&out, &D3DXVECTOR3((float)nScreenX, (float)nScreenY, fZ), &tmp);

	return out;
}

//=============================================================================
// XZ平面とスクリーン座標の交点算出関数
//=============================================================================
D3DXVECTOR3 CCamera::CalcScreenToXZ(int nScreenX, int nScreenY)
{
	D3DXVECTOR3 nearpos;
	D3DXVECTOR3 farpos;
	D3DXVECTOR3 ray;
	D3DXVECTOR3 out;
	nearpos = CalcScreenToWorld(nScreenX, nScreenY, 0.0f);
	farpos = CalcScreenToWorld(nScreenX, nScreenY, 1.0f);
	ray = farpos - nearpos;
	D3DXVec3Normalize(&ray, &ray);

	if (ray.y <= 0)
	{// 床交点
		float Lray = D3DXVec3Dot(&ray, &D3DXVECTOR3(0, 1, 0));
		float LP0 = D3DXVec3Dot(&(-nearpos), &D3DXVECTOR3(0, 1, 0));
		out = nearpos + (LP0 / Lray)*ray;
	}
	else
	{// 交差しなかった
		out = farpos;
	}

	return D3DXVECTOR3(out.x, out.y, -out.z);
}

//=============================================================================
// ビューポートの取得
//=============================================================================
D3DVIEWPORT9 CCamera::GetViewport(int nIdxCamera)
{
	D3DVIEWPORT9 viewport;
	switch (nIdxCamera)
	{
	case 0:
		viewport.Width = SCREEN_WIDTH;
		viewport.Height = SCREEN_HEIGHT;
		viewport.MaxZ = 1.0f;
		viewport.MinZ = 0.0f;
		viewport.X = 0;
		viewport.Y = 0;
		break;

	case 1:
		viewport.Width = 750;
		viewport.Height = 450;
		viewport.MaxZ = 1.0f;
		viewport.MinZ = 0.0f;
		viewport.X = SCREEN_WIDTH - 760;
		viewport.Y = SCREEN_HEIGHT - 470;
		break;
	}

	return viewport;
}

void CCamera::GetInfo(D3DVIEWPORT9 *pViewport, D3DXMATRIX *pMtxProjection, D3DXMATRIX *pMtxView)
{
	m_viewport.Width = 750;
	m_viewport.Height = 450;
	m_viewport.MaxZ = 1.0f;
	m_viewport.MinZ = 0.0f;
	m_viewport.X = SCREEN_WIDTH - 760;
	m_viewport.Y = SCREEN_HEIGHT - 470;

	*pViewport = m_viewport;

	// プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxProjection);
	D3DXMatrixOrthoLH(&m_mtxProjection, (float)m_viewport.Width, (float)m_viewport.Height, 10.0f, 8000.0f);

	*pMtxProjection = m_mtxProjection;

	// ビューマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxView);

	// ビューマトリックスを作成
	D3DXMatrixLookAtLH(&m_mtxView,
		&m_posV,
		&m_posR,
		&m_vecU);

	*pMtxView = m_mtxView;
}