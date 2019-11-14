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

//*****************************************************************************
// �N���X��`
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
	D3DXVECTOR3 m_posV;				// ���_
	D3DXVECTOR3 m_posR;				// �����_
	D3DXVECTOR3 m_posVDest;			// �ړI�̎��_
	D3DXVECTOR3 m_posRDest;			// �ړI�̒����_
	D3DXVECTOR3 m_vecU;				// ������x�N�g��
	D3DXMATRIX	m_mtxProjection;	// �v���W�F�N�V�����}�g���b�N�X	�i�e���r�j
	D3DXMATRIX	m_mtxView;			// �r���[�}�g���b�N�X			�i�J�����}���j
	D3DXVECTOR3 m_rot;				// ����
	D3DXVECTOR3 m_rotDest;			// �ړI�̌���
	float		m_fLength;			// �����_�Ƃ̋���
	float		m_fAngle;			// �p�x
	float		m_fLookHeight;		// �J�����̍���
};

#endif
