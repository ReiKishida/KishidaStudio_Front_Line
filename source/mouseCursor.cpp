//=============================================================================
//
// �}�E�X�J�[�\������ [mouseCursor.cpp]
// Author : Komatsu Keisuke
//
//=============================================================================
#include "mouseCursor.h"
#include "manager.h"
#include "renderer.h"
#include "debugproc.h"
#include "input.h"
#include "enemy.h"
#include "texture.h"
#include "camera.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define MOUSE_WIDTH		(30.0f)			// ��
#define MOUSE_HEIGHT	(30.0f)			// ����

//=============================================================================
// �ÓI�����o�ϐ�
//=============================================================================
D3DXVECTOR3 CMouseCursor::m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
D3DXVECTOR3 CMouseCursor2D::m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

//=============================================================================
// ��������
//=============================================================================
CMouseCursor  *CMouseCursor::Create()
{
	CMouseCursor *pMouseCursor = NULL;

	if (pMouseCursor == NULL)
	{
		pMouseCursor = new CMouseCursor;

		if (pMouseCursor != NULL)
		{
			pMouseCursor->Init();
		}
	}

	return pMouseCursor;
}

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CMouseCursor::CMouseCursor(int nPriority, CScene::OBJTYPE objType) : CScene3D(nPriority, objType){}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CMouseCursor::~CMouseCursor(){}

//=============================================================================
// ����������
//=============================================================================
HRESULT CMouseCursor::Init()
{
	CScene3D::Init();

	CScene3D::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	CScene3D::BindTexture(CTexture::GetTexture(CTexture::TEXTURE_MOUSECURSOR));

	m_cursorPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// ���_���̎擾
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = CScene3D::GetVtxBuff();

	// ���_����ݒ�
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(MOUSE_WIDTH, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, -MOUSE_HEIGHT);
	pVtx[3].pos = D3DXVECTOR3(MOUSE_WIDTH, 0.0f, -MOUSE_HEIGHT);

	//���_�o�b�t�@���A�����b�N
	pVtxBuff->Unlock();

	// ���_���̐ݒ�
	CScene3D::SetVtxBuff(pVtxBuff);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CMouseCursor::Uninit(void)
{
	CScene3D::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CMouseCursor::Update(void)
{
	CInputMouse *pMouse = CManager::GetInputMouse();	// �}�E�X�̓��͂��擾

	CDebugProc::Print("=====�}�E�X�p3D�|���S��======\n");
	CDebugProc::Print(" �|���S���̍��W\n");
	CDebugProc::Print("  x : %.1f\n  y : %.1f\n  z : %.1f\n", m_pos.x, m_pos.y, m_pos.z);
	CDebugProc::Print(" �J�[�\���̍��W\n");
	CDebugProc::Print("  x : %.1f\n  y : %.1f\n  z : %.1f\n", m_cursorPos.x, m_cursorPos.y, m_cursorPos.z);
	CDebugProc::Print("============================\n\n");

	if (CEnemy::GetPartSwitch() == true)
	{// �X�g���e�W�[�p�[�g��
		// ���͎�����
		Input(pMouse);
	}

	// �ړ�������
	Move(pMouse);
}

//=============================================================================
// �`�揈��
//=============================================================================
void CMouseCursor::Draw(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// ���C�e�B���OOFF
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// Z�e�X�gOFF
	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

	CScene3D::Draw();

	// Z�e�X�gON
	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

	// ���C�e�B���OON
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//=============================================================================
// ���͏���
//=============================================================================
void CMouseCursor::Input(CInputMouse *pMouse)
{
	if (pMouse->GetTrigger(CInputMouse::DIMS_BUTTON_0) == true)
	{// ���N���b�N
		// �ړI�n�ݒ�
		CEnemy::SetSearchPos(m_pos);
	}
	if (pMouse->GetTrigger(CInputMouse::DIMS_BUTTON_1) == true)
	{// �E�N���b�N
	}
}

//=============================================================================
// �ړ�����
//=============================================================================
void CMouseCursor::Move(CInputMouse *pMouse)
{
	D3DXVECTOR3 move;	// �ړ���

	// �E�B���h�E�̋�`���擾
	RECT rect;
	GetWindowRect(CManager::GetRenderer()->GetHWnd(), &rect);

	// �}�E�X�J�[�\���ɍ��킹�ē�����
	move.x = ((float)(pMouse->GetPoint().x - rect.left) - ((rect.right - rect.left) / 2.0f));
	move.y = 0.0f;
	move.z = -((float)(pMouse->GetPoint().y - rect.top) - ((rect.bottom - rect.top) / 2.0f));

	// �ړ��ʂ̑��
	m_pos = move;
	m_cursorPos.x = move.x;
	m_cursorPos.z = move.z;

	// �ʒu�̐ݒ�
	CScene3D::SetPos(m_pos);
}

/****************************************************************/
/*					3D�}�E�X�J�[�\���̃N���X					*/
/****************************************************************/
//=============================================================================
// ��������
//=============================================================================
CMouseCursor2D  *CMouseCursor2D::Create()
{
	CMouseCursor2D *pMouseCursor = NULL;

	if (pMouseCursor == NULL)
	{
		pMouseCursor = new CMouseCursor2D;

		if (pMouseCursor != NULL)
		{
			pMouseCursor->Init();
		}
	}

	return pMouseCursor;
}

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CMouseCursor2D::CMouseCursor2D(int nPriority, CScene::OBJTYPE objType) : CScene2D(nPriority, objType) {}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CMouseCursor2D::~CMouseCursor2D() {}

//=============================================================================
// ����������
//=============================================================================
HRESULT CMouseCursor2D::Init()
{
	CScene2D::Init();

	CScene2D::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	CScene2D::BindTexture(CTexture::GetTexture(CTexture::TEXTURE_MOUSECURSOR));

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CMouseCursor2D::Uninit(void)
{
	CScene2D::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CMouseCursor2D::Update(void)
{
	CInputMouse *pMouse = CManager::GetInputMouse();	// �}�E�X�̓��͂��擾

	// �ړ�������
	Move(pMouse);
}

//=============================================================================
// �`�揈��
//=============================================================================
void CMouseCursor2D::Draw(void)
{
	CScene2D::Draw();
}

//=============================================================================
// �ړ�����
//=============================================================================
void CMouseCursor2D::Move(CInputMouse *pMouse)
{
	// �E�B���h�E�̋�`���擾
	RECT rect;
	GetWindowRect(CManager::GetRenderer()->GetHWnd(), &rect);

	// ����ɃT�C�Y�ύX���l�����āA���݂̃T�C�Y�ŕ␳�i�g�T�C�Y������̂Ō����ł͂Ȃ��j
	m_pos.x = float(pMouse->GetPoint().x - rect.left) - 5.0f;
	m_pos.y = float(pMouse->GetPoint().y - rect.top) - 30.0f;

	// �ʒu�̐ݒ�
	CScene2D::SetPos(m_pos);

	// ���_���̎擾
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = CScene2D::GetVtxBuff();

	// ���_����ݒ�
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos = m_pos + D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = m_pos + D3DXVECTOR3(MOUSE_WIDTH, 0.0f, 0.0f);
	pVtx[2].pos = m_pos + D3DXVECTOR3(0.0f, MOUSE_HEIGHT, 0.0f);
	pVtx[3].pos = m_pos + D3DXVECTOR3(MOUSE_WIDTH, MOUSE_HEIGHT, 0.0f);

	//���_�o�b�t�@���A�����b�N
	pVtxBuff->Unlock();

	// ���_���̐ݒ�
	CScene2D::SetVtxBuff(pVtxBuff);
}