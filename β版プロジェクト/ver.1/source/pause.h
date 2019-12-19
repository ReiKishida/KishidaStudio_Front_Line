//=============================================================================
//
// �|�[�Y���j���[���� [pause.h]
// Author : Takuto Ishida
//
//=============================================================================
#ifndef _PAUSE_H_
#define _PAUSE_H_

#include "scene.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CScene2D;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CPause : public CScene
{
public:
	//*****************************************************************************
	// �|�[�Y���j���[
	//*****************************************************************************
	typedef enum
	{
		TYPE_CONTINUE = 0,
		TYPE_RETRY,
		TYPE_QUIT,
		TYPE_MAX
	}TYPE;

	CPause(int nPriority = 6, CScene::OBJTYPE objType = CScene::OBJTYPE_PAUSE);
	~CPause();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CPause* Create(void);

	static HRESULT Load(void);				// �e�N�X�`����ǂݍ���
	static void Unload(void);				// �ǂݍ��񂾃e�N�X�`�������

private:
	void Select(void);

	static LPDIRECT3DTEXTURE9 m_pTexture[TYPE_MAX];	// ���L�e�N�X�`���ւ̃|�C���^

	CScene2D *m_apScene2D[TYPE_MAX];	// ����
	int m_nNumSelect;					// �I�𒆂̍���
};

#endif