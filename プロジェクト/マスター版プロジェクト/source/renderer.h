//=============================================================================
//
// �����_�����O���� [renderer.h]
// Author : Takuto Ishida
//
//=============================================================================
#ifndef _RENDERER_H_
#define _RENDERER_H_

#include "main.h"
#include "manager.h"

//=====================================
// �O���錾
//=====================================

//=====================================
// �N���X��`
//=====================================
class CRenderer : public CManager
{
public:
	CRenderer();
	~CRenderer();
	HRESULT Init(HINSTANCE hInstance, HWND hWnd, bool bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	LPDIRECT3DDEVICE9 GetDevice(void) { return m_pD3DDevice; };	// �f�o�C�X�̎擾
	HWND GetHWnd(void) { return m_hWnd; }
	bool GetWindowMode(void) { return m_bWindowMode; };

private:
	LPDIRECT3D9				m_pD3D;			// Direct3D�I�u�W�F�N�g
	LPDIRECT3DDEVICE9		m_pD3DDevice;	// Device�I�u�W�F�N�g(�`��ɕK�v)
	HWND					m_hWnd;			// �E�B���h�E�n���h��
	bool					m_bWindowMode;	// �E�B���h�E���t���X�N���[���ǂ��炩

#ifdef _DEBUG
	LPD3DXFONT				m_pFont;		// �t�H���g�ւ̃|�C���^
#endif
};

#endif