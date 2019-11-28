//=============================================================================
//
// �}�E�X�J�[�\������ [mouseCursor.h]
// Author : Komatsu Keisuke
//
//=============================================================================
#ifndef _MOUSECURSOR_H_
#define _MOUSECURSOR_H_

#include "main.h"
#include "scene3D.h"
#include "scene2D.h"

class CInputKeyboard;
class CInputMouse;
class CCamera;
class CNodePointer;

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
	typedef struct
	{
		int nodeMax;									// �m�[�h�̑���
		int	index[NODEPOINT_MAX];						// �����̃m�[�h�ԍ�
		int connectNum[NODEPOINT_MAX];					// �ڑ��m�[�h��
		int connectIndex[NODEPOINT_MAX][CONNECT_MAX];	// �ڑ��m�[�h�ԍ�
		D3DXVECTOR3 pos[NODEPOINT_MAX];					// �e�m�[�h�̈ʒu
	}NODE_LOAD_STATE;

	typedef struct
	{
		int nodeMax;			// �m�[�h�̑���
		int	index;				// �����̃m�[�h�ԍ�
		int connectNum;			// �ڑ��m�[�h��
		int *connectIndex;		// �ڑ��m�[�h�ԍ�
		D3DXVECTOR3 pos;		// �e�m�[�h�̈ʒu
	}NODE_SAVE_STATE;

	CMouseCursor(int nPriority = MOUSE_PRIORITY, CScene::OBJTYPE objType = CScene::OBJTYPE_MOUSE);
	~CMouseCursor();

	static CMouseCursor *Create();

	HRESULT Init();
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void Input(CInputKeyboard *pKeyboard, CInputMouse *pMouse);
	void Move(CInputMouse *pMouse);

	void FileLoad(char* pFileName);

	D3DXVECTOR3 GetPos(void) { return m_pos; }
	D3DXVECTOR3 &Getsetpos(void) { return m_setpos; }

private:
	NODE_LOAD_STATE m_LoadNodeData;		// ���[�h�����}�b�v���
	LPDIRECT3DTEXTURE9 m_pTexture;	// �e�N�X�`���ւ̃|�C���^
	D3DXVECTOR3	m_pos;				// �ʒu
	D3DXVECTOR3	m_setpos;			// �n�_�o�^�p�ʒu
	D3DXVECTOR3 m_size;				// �T�C�Y
	int m_nNodeCounter;				// �z�u�����m�[�h�̐�

	static CCamera *m_pCamera;		// �J�����ւ̃|�C���^
	static CNodePointer *m_pNodePointer[NODEPOINT_MAX];	// �m�[�h�|���S���ւ̃|�C���^
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
