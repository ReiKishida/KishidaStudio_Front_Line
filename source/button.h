//=============================================================================
//
// �{�^������ [button.h]
// Author : Takuto Ishida
//
//=============================================================================
#ifndef _BUTTON_H_
#define _BUTTON_H_

#include "scene3D.h"
#include "scene2D.h"
#include "line.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define BUTTON_PRIORITY			(6)		// �����̗D��ԍ�
#define BUTTON_MANAGER_PRIORITY	(6)		// �����̗D��ԍ�

//*****************************************************************************
// �O���錾
//*****************************************************************************

//*****************************************************************************
// �N���X��`
//*****************************************************************************
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 2D�{�^���̃N���X
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class CButton2D : public CScene2D
{
public:
	CButton2D(int nPriority = BUTTON_PRIORITY, CScene::OBJTYPE objType = CScene::OBJTYPE_BUTTON);
	~CButton2D();
	HRESULT Init(D3DXVECTOR3 pos, float fWidth, float fHeight);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CButton2D* Create(D3DXVECTOR3 pos, float fWidth, float fDepth);

	bool InRange(D3DXVECTOR3 pos);
	bool GetInRange(void) { return m_bInRange; }

	bool Click(void);
	bool ClickRelease(void);

	bool GetSwitch(void) { return m_bSwitch; }
	void SetSwitch(bool bSwitch) { m_bSwitch = bSwitch; }

private:
	bool m_bSwitch;			// �N�����Ă��邩�ǂ���
	bool m_bInRange;		// �͈͓����ǂ���
	bool m_bRelease;		// �����[�X�̔���ɕK�v
	D3DXVECTOR2 m_size;		// �傫��
};

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 3D�{�^���̃N���X
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class CButton3D : public CScene3D
{
public:
	CButton3D(int nPriority = BUTTON_PRIORITY, CScene::OBJTYPE objType = CScene::OBJTYPE_BUTTON);
	~CButton3D();
	HRESULT Init(D3DXVECTOR3 pos, float fWidth, float fDepth);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CButton3D* Create(D3DXVECTOR3 pos, float fWidth, float fDepth);

	bool InRange(D3DXVECTOR3 pos);
	bool GetInRange(void) { return m_bInRange; }

	bool Click(void);
	bool ClickRelease(void);

	bool GetSwitch(void) { return m_bSwitch; }
	void SetSwitch(bool bSwitch) { m_bSwitch = bSwitch; }

	bool GetDisp(void) { return m_bDisp; };
	void SetDisp(bool bDisp) { m_bDisp = bDisp; };

private:
	bool m_bSwitch;			// �N�����Ă��邩�ǂ���
	bool m_bInRange;		// �͈͓����ǂ���
	bool m_bRelease;		// �����[�X�̔���ɕK�v
	D3DXVECTOR2 m_size;		// �傫��
	bool m_bDisp;			// �\���E��\��
};

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �{�^�����Ȃ����̃N���X
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class CButtonLine : public CScene2D
{
public:
	CButtonLine(int nPriority = BUTTON_PRIORITY, CScene::OBJTYPE objType = CScene::OBJTYPE_BUTTONLINE);
	~CButtonLine();
	HRESULT Init(D3DXVECTOR3 start);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CButtonLine *Create(D3DXVECTOR3 start);

	bool GetBoolLink(void) { return m_bLink; };
	void SetBoolLink(bool bLink) { m_bLink = bLink; };

	void Link(D3DXVECTOR3 end);

private:
	D3DXVECTOR3 m_start;		// �J�n�ʒu
	bool m_bLink;				// �ڑ��������ǂ���
};

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �{�^���̊Ǘ��N���X
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class CButtonManagerStrategy : public CScene
{
public:
	CButtonManagerStrategy(int nPriority = BUTTON_MANAGER_PRIORITY, CScene::OBJTYPE objType = CScene::OBJTYPE_BUTTON_MANAGER);
	~CButtonManagerStrategy();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CButtonManagerStrategy *Create(void);

	void ButtonUninit(int nLogic);

	int GetSelectAIType(void) { return m_nSelectAIType; }
	int GetSelectLogic(int nHierarchy) { return m_aSelectLogic[nHierarchy]; }

	bool &GetSelectFinish(void) { return m_bFinish; }

private:
	void AITypeSelect(void);
	void FirstHierarchy(void);
	void SecondHierarchy(void);
	void ThirdHierarchy(void);
	void FourthHierarchy(void);

	CButton2D *m_pAIType[2];		// �h���[�������[�J�[�ǂ��炩
	CButton2D **m_pFirst;			// ���W�b�N�c���[�̂P�K�w�ڂ̃{�^��
	CButton2D **m_pSecond;			// ���W�b�N�c���[�̂Q�K�w�ڂ̃{�^��
	CButton2D **m_pThird;			// ���W�b�N�c���[�̂R�K�w�ڂ̃{�^��
	CButton2D **m_pFourth;			// ���W�b�N�c���[�̂S�K�w�ڂ̃{�^��

	CButtonLine *m_pLine[4];		// �{�^�����m���Ȃ����̃N���X

	int m_aNumLogic[4];				// ���W�b�N�c���[�̊K�w���Ƃ̃{�^���̐�

	int m_nSelectAIType;			// �h���[�������[�J�[�̑I��
	int m_aSelectLogic[4];			// �K�w���Ƃ̑I���{�^��

	CScene2D *m_apSelectIcon[4];	// �I���������ڂ�\��

	bool	m_bFinish;				// �I�����I������

#ifdef _DEBUG
	bool	m_bDisp;
#endif

};

#endif