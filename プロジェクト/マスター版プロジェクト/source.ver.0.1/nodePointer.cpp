//=============================================================================
//
// �m�[�h�|�C���^�[���� [mouseCursor.cpp]
// Author : Komatsu Keisuke
//
//=============================================================================
#include "nodePointer.h"
#include "manager.h"
#include "renderer.h"
#include "debugproc.h"
#include "input.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define POINT_SIZE_X			(18.0f)		// ���n�_�p�|���S���̑傫��X
#define POINT_SIZE_Z			(17.0f)		// ���n�_�p�|���S���̑傫��Y

//=============================================================================
// �ÓI�����o�ϐ�
//=============================================================================
D3DXVECTOR3 CNodePointer::m_SearchPos = {};
int CNodePointer::m_nNodeMax = 0;

//=============================================================================
// ��������
//=============================================================================
CNodePointer *CNodePointer::Create(D3DXVECTOR3 pos)
{
	CNodePointer *pNodePointer = NULL;

	if (pNodePointer == NULL)
	{
		pNodePointer = new CNodePointer;

		if (pNodePointer != NULL)
		{
			pNodePointer->m_nNumber = m_nNodeMax;
			pNodePointer->m_pos = pos;
			pNodePointer->Init();
		}
	}

	return pNodePointer;
}

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CNodePointer::CNodePointer(int nPriority, CScene::OBJTYPE objType) : CScene3D(nPriority, objType){}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CNodePointer::~CNodePointer(){}

//=============================================================================
// ����������
//=============================================================================
HRESULT CNodePointer::Init()
{	
	CScene3D::Init();

	// �����ݒ�
	m_size = D3DXVECTOR3(POINT_SIZE_X, 0.0f, POINT_SIZE_Z);
	m_color = D3DXCOLOR(1.0f, 0.5f, 0.8f, 0.5f);

	for (int nCntConnect = 0; nCntConnect <CONNECT_MAX; nCntConnect++)
	{// �ڑ��\�ő吔�����
		m_nConnect[nCntConnect] = 0;
	}
	m_nConnectMax = 0;

	// �m�[�h�|�C���^�[�̐ݒ�
	CScene3D::SetPos(m_pos);
	CScene3D::SetSize(m_size);
	CScene3D::SetColor(m_color);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CNodePointer::Uninit(void)
{
	CScene3D::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CNodePointer::Update(void)
{
	//CDebugProc::Print("�m�[�h�̈ʒu x : %.1f / y : %.1f / z : %.1f\n", m_pos.x, m_pos.y, m_pos.z);
	//CDebugProc::Print("No. : %d\n", m_nNumber);
	//CDebugProc::Print("�ڑ��m�[�h�� : %d\n", m_nConnectMax);
	//for (int nCntConnect = 0; nCntConnect < m_nConnectMax; nCntConnect++)
	//{
	//	CDebugProc::Print("�ڑ���ԍ�[%d] : %d\n", nCntConnect, m_nConnect[nCntConnect]);
	//}
	//CDebugProc::Print("\n");

	CScene3D::SetSize(m_size);
	CScene3D::SetColor(m_color);
}

//=============================================================================
// �`�揈��
//=============================================================================
void CNodePointer::Draw(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// ���C�e�B���OOFF
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// Z�e�X�gOFF
	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);

	CScene3D::Draw();

	// Z�e�X�gON
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);

	// ���C�e�B���OON
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}