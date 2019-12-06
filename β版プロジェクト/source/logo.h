//=============================================================================
//
// ���S���� [logo.h]
// Author : Takuto Ishida
//
//=============================================================================
#ifndef _LOGO_H_
#define _LOGO_H_

#include "scene2D.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define LOGO_PRIORITY	(5)		// �����̗D�揇��

//*****************************************************************************
// �O���錾
//*****************************************************************************

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CLogo : public CScene2D
{
public:
	CLogo(int nPriority = LOGO_PRIORITY, CScene::OBJTYPE objType = CScene::OBJTYPE_LOGO);
	~CLogo();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CLogo* Create(D3DXVECTOR3 pos, float fWidth, float fHeight);	// ����

private:
};

/*========================*/
/*   ���͑҂��\���N���X   */
/*========================*/
class CLogoPressButton : public CLogo
{
public:
	CLogoPressButton(int nPriority = LOGO_PRIORITY, CScene::OBJTYPE objType = CScene::OBJTYPE_LOGO);
	~CLogoPressButton();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CLogoPressButton* Create(D3DXVECTOR3 pos, float fWidth, float fHeight);		// ����

private:
	LPDIRECT3DTEXTURE9 m_pTexture;	// �e�N�X�`���ւ̃|�C���^
	int m_nCntFlash;				// �_�ŗp�J�E���^
};

/*========================*/
/*   ��������\���N���X   */
/*========================*/
class CLogoTutorial : public CLogo
{
public:
	CLogoTutorial(int nPriority = LOGO_PRIORITY, CScene::OBJTYPE objType = CScene::OBJTYPE_LOGO);
	~CLogoTutorial();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CLogoTutorial* Create(D3DXVECTOR3 pos, float fWidth, float fHeight);		// ����

private:
	LPDIRECT3DTEXTURE9 m_pTexture;	// �e�N�X�`���ւ̃|�C���^
};

#endif