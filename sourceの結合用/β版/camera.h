//=============================================================================
//
// �J�������� [camera.h]
// Author : Takuto Ishida
//
//=============================================================================
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "scene.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define PARALLEL_PROJECTION_SIZE	(6.0f)

//*****************************************************************************
// �N���X��`
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
	D3DXVECTOR3		m_posV;								// ���_
	D3DXVECTOR3		m_posR;								// �����_
	D3DXVECTOR3		m_posVDest;							// �ړI�̎��_
	D3DXVECTOR3		m_posRDest;							// �ړI�̒����_
	D3DXVECTOR3		m_vecU;								// ������x�N�g��
	D3DXMATRIX		m_mtxProjection;					// �v���W�F�N�V�����}�g���b�N�X	�i�e���r�j
	D3DXMATRIX		m_mtxView;							// �r���[�}�g���b�N�X			�i�J�����}���j
	D3DXVECTOR3		m_rot;								// ����
	D3DXVECTOR3		m_rotDest;							// �ړI�̌���
	float			m_fLength;							// �����_�Ƃ̋���
	float			m_fAngle;							// �p�x
	float			m_fLookHeight;						// �J�����̍���
	float			m_fZoom;							// �Y�[���̔{��
	D3DVIEWPORT9	m_viewport;							// �r���[�|�[�g
	float			m_fCameraSpeed;
	int				m_nCntStarting;						//�J�n�O�̃J�E���^�[
	STAGE_CAMERA	m_stageCamera;
};

#endif
