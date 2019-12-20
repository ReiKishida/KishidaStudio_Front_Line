//=============================================================================
//
// �e���� [shadow.h]
// Author : Takuto Ishida
//
//=============================================================================
#ifndef _SHADOW_H_
#define _SHADOW_H_

#include "scene.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define SHADOW_PRIORITY		(6)

//*****************************************************************************
// �O���錾
//*****************************************************************************

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CShadow : public CScene
{
public:
	CShadow(int nPriority = SHADOW_PRIORITY, CScene::OBJTYPE objType = CScene::OBJTYPE_SHADOW);
	~CShadow();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CShadow *Create(CScene *pParent);

	void Set(D3DXMATRIX mtxWorld, LPD3DXMESH mesh, int nNumMat, D3DXVECTOR3 _pos, float fHeight = 1.0f);

	static bool RangeShadow(D3DXVECTOR3 pos, CScene *pObject, D3DXVECTOR3 vtxDiff);

private:
	D3DXMATRIX	m_mtxShadow;		// �V���h�E�}�g���b�N�X
	D3DXPLANE	m_planeField;		// �n�ʂ̕���
	CScene		*m_pParent;			// �I�u�W�F�N�g�̃|�C���^
	D3DXVECTOR3	m_ObjPos;			// �I�u�W�F�N�g�̈ʒu
};

#endif
