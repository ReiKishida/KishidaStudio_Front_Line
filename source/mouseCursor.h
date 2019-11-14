//=============================================================================
//
// �}�E�X�J�[�\������ [mouseCursor.h]
// Author : Komatsu Keisuke
//
//=============================================================================
#ifndef _WALL_H_
#define _WALL_H_

#include "scene3D.h"
#include "scene2D.h"

class CInputMouse;
//=============================================================================
// �}�N����`
//=============================================================================
#define MOUSE_PRIORITY		(7)		// �����̗D��ԍ�

//=============================================================================
// �N���X��`
//=============================================================================
class CMouseCursor : public CScene3D
{
public:
	CMouseCursor(int nPriority = MOUSE_PRIORITY, CScene::OBJTYPE objType = CScene::OBJTYPE_MOUSE);
	~CMouseCursor();

	static CMouseCursor *Create();

	HRESULT Init();
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void Input(CInputMouse *pMouse);
	void Move(CInputMouse *pMouse);

	static void SetMousePosition(D3DXVECTOR3 pos) { m_pos = pos; };
	static D3DXVECTOR3 GetMousePosition(void) { return m_pos; };

private:
	static D3DXVECTOR3	m_pos;		// �ʒu
	D3DXVECTOR3	m_cursorPos;		// �J�[�\���̈ʒu
};

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 2D�}�E�X�J�[�\���̃N���X
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class CMouseCursor2D : public CScene2D
{
public:
	CMouseCursor2D(int nPriority = MOUSE_PRIORITY, CScene::OBJTYPE objType = CScene::OBJTYPE_MOUSE);
	~CMouseCursor2D();

	static CMouseCursor2D *Create(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void Move(CInputMouse *pMouse);

	static void SetMousePosition(D3DXVECTOR3 pos) { m_pos = pos; };
	static D3DXVECTOR3 GetMousePosition(void) { return m_pos; };

private:
	static D3DXVECTOR3	m_pos;		// �ʒu
};

#endif
