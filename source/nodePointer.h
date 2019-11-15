//=============================================================================
//
// �m�[�h�|�C���^�\���� [mouseCursor.h]
// Author : Komatsu Keisuke
//
//=============================================================================
#ifndef _NODEPOINTER_H_
#define _NODEPOINTER_H_

#include "main.h"
#include "scene3D.h"

class CScene3D;

//=============================================================================
// �}�N����`
//=============================================================================
#define NODE_PRIORITY		(6)

//=============================================================================
// �N���X��`
//=============================================================================
class CNodePointer : public CScene3D
{
public:
	CNodePointer(int nPriority = NODE_PRIORITY, CScene::OBJTYPE objType = CScene::OBJTYPE_NODE);
	~CNodePointer();
	
	static CNodePointer *Create(D3DXVECTOR3 pos);
	
	HRESULT Init();
	void Uninit(void);
	void Update(void);
	void Draw(void);

	D3DXVECTOR3 &GetPos(void) { return m_pos; };
	D3DXVECTOR3 &GetSize(void) { return m_size; };
	D3DXCOLOR &GetColor(void) { return m_color; };
	int &GetMyNumber(void) { return m_nNumber; };
	int &GetConnect(int	value) { return m_nConnect[value]; };
	int &GetConnectMax(void) { return m_nConnectMax; };

	static int &GetNodeMax(void) { return m_nNodeMax; };
	static D3DXVECTOR3 &GetSearchPos(void) { return m_SearchPos; };

private:
	D3DXVECTOR3	m_pos;				// �ʒu
	D3DXVECTOR3 m_size;				// �T�C�Y
	D3DXCOLOR m_color;				// �F
	int m_nNumber;					// �����̔ԍ�
	int m_nConnect[CONNECT_MAX];	// �ڑ���̔ԍ�
	int m_nConnectMax;				// �S�̂̐ڑ���

	static int m_nNodeMax;			// �S�m�[�h�̐�
	static D3DXVECTOR3 m_SearchPos;	// �����p�ʒu
};
#endif
