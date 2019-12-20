//=============================================================================
//
// カメラ処理 [camera.h]
// Author : Takuto Ishida
//
//=============================================================================
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "scene.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define PARALLEL_PROJECTION_SIZE	(6.0f)

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CCamera : public CScene
{
public:
	typedef enum
	{
		STAGE_CAMERA_STRAIGHT = 0,
		STAGE_CAMERA_TURN,
		STAGE_CAMERA_OVERLOOK,
		STAGE_CAMERA_STOP,

		STAGE_CAMERA_MAX,
	}STAGE_CAMERA;

	CCamera();
	~CCamera();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Set(int nIdxCamera);

	D3DXMATRIX GetProjection(void) { return m_mtxProjection; };
	D3DXMATRIX GetView(void) { return m_mtxView; };
	float GetRotY(void) { return m_rot.y; };

	D3DXVECTOR3 GetRot(void) { return m_rot; };
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }

	D3DXVECTOR3 GetPosV(void) { return m_posV; };
	void SetPosV(D3DXVECTOR3 posV) { m_posV = posV; };

	D3DXVECTOR3 GetPosR(void) { return m_posR; };
	void SetPosR(D3DXVECTOR3 posR) { m_posR = posR; };

	float GetLength(void) { return m_fLength; };
	void SetLength(float fLength) { m_fLength = fLength; };

	D3DXVECTOR3 CalcScreenToWorld(int nScreenX, int nScreenY, float fZ);
	D3DXVECTOR3 CalcScreenToXZ(int nScreenX, int nScreenY);

	D3DVIEWPORT9 GetViewport(int nIdxCamera);

	void GetInfo(D3DVIEWPORT9 *pViewport, D3DXMATRIX *pMtxProjection, D3DXMATRIX *pMtxView);
	float GetZoom(void) { return m_fZoom; };

	STAGE_CAMERA GetStageCamera(void) { return m_stageCamera; };

private:
	D3DXVECTOR3		m_posV;								// 視点
	D3DXVECTOR3		m_posR;								// 注視点
	D3DXVECTOR3		m_posVDest;							// 目的の視点
	D3DXVECTOR3		m_posRDest;							// 目的の注視点
	D3DXVECTOR3		m_vecU;								// 上方向ベクトル
	D3DXMATRIX		m_mtxProjection;					// プロジェクションマトリックス	（テレビ）
	D3DXMATRIX		m_mtxView;							// ビューマトリックス			（カメラマン）
	D3DXVECTOR3		m_rot;								// 向き
	D3DXVECTOR3		m_rotDest;							// 目的の向き
	float			m_fLength;							// 注視点との距離
	float			m_fAngle;							// 角度
	float			m_fLookHeight;						// カメラの高さ
	float			m_fZoom;							// ズームの倍率
	D3DVIEWPORT9	m_viewport;							// ビューポート
	float			m_fCameraSpeed;
	int				m_nCntStarting;						//開始前のカウンター
	STAGE_CAMERA	m_stageCamera;
};

#endif
