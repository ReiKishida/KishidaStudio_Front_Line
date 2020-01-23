//=============================================================================
//
// �s�����ď��� [pin.h]
// Author : Komatsu Keisuke
//
//=============================================================================
#ifndef _PIN_H_
#define _PIN_H_

#include "main.h"
#include "scene3DBill.h"

//=============================================================================
// �O���錾
//=============================================================================
class CInputKeyboard;
class CInputMouse;
class CCamera;
class CNodePointer;
class CNodeDataFiler;

//=============================================================================
// �}�N����`
//=============================================================================
#define PIN_PRIORITY		(7)		// �����̗D��ԍ�

//=============================================================================
// �N���X��`
//=============================================================================
class CPin : public CScene3DBill
{
public:
	CPin(int nPriority = PIN_PRIORITY, CScene::OBJTYPE objType = CScene::OBJTYPE_PIN);
	~CPin();

	static CPin *Create(D3DXVECTOR3 pos);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	D3DXVECTOR3 &GetPos(void) { return m_pos; }
	LPDIRECT3DTEXTURE9 &GetTexture(void) { return m_pTexture; };

private:
	LPDIRECT3DTEXTURE9 m_pTexture;	// �e�N�X�`��
	D3DXVECTOR3	m_pos;				// �ʒu
	D3DXVECTOR3 m_size;				// �T�C�Y
};
#endif
