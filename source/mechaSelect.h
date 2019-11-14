//=============================================================================
//
// ���J�I������ [mechaSelect.h]
// Author : Ayaka Hakozaki
//
//=============================================================================
#ifndef _MECHASELECT_H_
#define _MECHASELECT_H_

#include "scene.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MECHASEL_PRIORITY				(7)		// �D��ԍ�
#define MECHASEL_NUM_BUTTON		(4)		// �{�^���̐�
#define MECHASEL_NUM_UI					(4)		// UI�̐�

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CBg;
class CButton2D;
class CMouseCursor2D;
class CScene2D;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CMechaSelect : public CScene
{
public:
	typedef enum
	{
		MECHATYPE_LIGHT = 0,		// �y�ʌ^
		MECHATYPE_SHOOTER,		// �x���^
		MECHATYPE_HEAVY,				// �d�ʌ^
		MECHATYPE_ASSULT,			// ���P�^
		MECHATYPE_MAX					// �ő吔
	} MECHATYPE;

	CMechaSelect(int nPriority = MECHASEL_PRIORITY, CScene::OBJTYPE objType = CScene::OBJTYPE_MECHASELECT);
	~CMechaSelect();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// �ϐ�
	MECHATYPE m_mechaType;			// �^�C�v�����ϐ�

private:
	CBg *m_pBg;		// �w�i�̃|�C���^�ϐ�
	CButton2D *m_apButtonUI[MECHASEL_NUM_BUTTON];	// �{�^���N���X�̃|�C���^�ϐ�
	CMouseCursor2D *m_pCursor;					// �J�[�\���N���X�̃|�C���^�ϐ�
	CScene2D *m_apUI[MECHASEL_NUM_UI];				// UI�̃|���S��
};

#endif