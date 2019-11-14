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

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CCamera : public CScene
{
public:
	CCamera();
	~CCamera();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Set(void);

	D3DXMATRIX GetProjection(void) { return m_mtxProjection; };
	D3DXMATRIX GetView(void) { return m_mtxView; };
	float GetRotY(void) { return m_rot.y; };
	D3DXVECTOR3 GetRot(void) { return m_rot; };

	D3DXVECTOR3 GetPosV(void) { return m_posV; };
	void SetPosV(D3DXVECTOR3 posV) { m_posV = posV; };

	D3DXVECTOR3 GetPosR(void) { return m_posR; };
	void SetPosR(D3DXVECTOR3 posR) { m_posR = posR; };

	float GetLength(void) { return m_fLength; };
	void SetLength(float fLength) { m_fLength = fLength; };

	D3DXVECTOR3 CalcScreenToWorld(float fScreenX, float fScreenY);

private:
	D3DXVECTOR3 m_posV;				// 視点
	D3DXVECTOR3 m_posR;				// 注視点
	D3DXVECTOR3 m_posVDest;			// 目的の視点
	D3DXVECTOR3 m_posRDest;			// 目的の注視点
	D3DXVECTOR3 m_vecU;				// 上方向ベクトル
	D3DXMATRIX	m_mtxProjection;	// プロジェクションマトリックス	（テレビ）
	D3DXMATRIX	m_mtxView;			// ビューマトリックス			（カメラマン）
	D3DXVECTOR3 m_rot;				// 向き
	D3DXVECTOR3 m_rotDest;			// 目的の向き
	float		m_fLength;			// 注視点との距離
	float		m_fAngle;			// 角度
	float		m_fLookHeight;		// カメラの高さ
};

#endif
