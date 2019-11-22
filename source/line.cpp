//=============================================================================
//
// �����蔻��̃��C������ [line.cpp]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#include "line.h"
#include "manager.h"
#include "renderer.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//=============================================================================
// ��������
//=============================================================================
CLine *CLine::Create(D3DXVECTOR3 start, D3DXVECTOR3 end, D3DXCOLOR col)
{
	CLine *pLine = NULL;

	pLine = new CLine;	// ���������m��

	if (pLine != NULL)
	{// �������m�ې���
		pLine->m_start = start;	// �n�_
		pLine->m_end = end;		// �I�_
		pLine->m_col = col;		// �F
		pLine->Init();			// ����������
	}

	return pLine;
}

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CLine::CLine()
{
	m_start = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_end = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_pVtxBuff = NULL;
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CLine::~CLine()
{
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CLine::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// �f�o�C�X�̎擾

	// ���_���̍쐬
	MakeVertex(pDevice);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CLine::Uninit(void)
{
	// ���_�o�b�t�@�̊J��
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	// ������j��
	CScene::Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CLine::Update(void)
{
}

//=============================================================================
// �`�揈��
//=============================================================================
void CLine::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// �f�o�C�X�̎擾
	D3DXMATRIX	mtxTrans;				// �v�Z�p�}�g���b�N�X

	// ���C�g����
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, 0.0f, 0.0f, 0.0f);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, NULL);

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_LINELIST, 0, 1);

	// ���C�g�L��
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//=============================================================================
// �n�_�ƏI�_�̍��W�̐ݒ�
//=============================================================================
void CLine::SetPoint(D3DXVECTOR3 start, D3DXVECTOR3 end)
{
	// �I�_�̍��W�̐ݒ�
	m_end = end;
	m_start = start;

	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_�̍��W
	pVtx[0].pos = m_start;
	pVtx[1].pos = m_end;

	// ���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}

//=============================================================================
// �F�̐ݒ�
//=============================================================================
void CLine::SetColor(D3DXCOLOR col)
{
	// �F�̐ݒ�
	m_col = col;

	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_�̍��W
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;

	// ���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}

//=============================================================================
// ���_���̍쐬
//=============================================================================
void CLine::MakeVertex(LPDIRECT3DDEVICE9 pDevice)
{
	// ���_�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 2
		,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_�̍��W
	pVtx[0].pos = m_start;
	pVtx[1].pos = m_end;

	// �@���̐ݒ�
	pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

	// ���_�J���[
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;

	//�e�N�X�`�����W
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 1.0f);

	// ���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}

/****************************************************************/
/*						2D���C���̃N���X						*/
/****************************************************************/
//=============================================================================
// ��������
//=============================================================================
CLine2D *CLine2D::Create(D3DXVECTOR3 start, D3DXVECTOR3 end, D3DXCOLOR col)
{
	CLine2D *pLine = NULL;

	pLine = new CLine2D;	// ���������m��

	if (pLine != NULL)
	{// �������m�ې���
		pLine->m_start = start;	// �n�_
		pLine->m_end = end;		// �I�_
		pLine->m_col = col;		// �F
		pLine->Init();			// ����������
	}

	return pLine;
}

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CLine2D::CLine2D(int nPriority, CScene::OBJTYPE objType) : CScene(nPriority, objType)
{
	m_start = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_end = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_pVtxBuff = NULL;
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CLine2D::~CLine2D()
{
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CLine2D::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// �f�o�C�X�̎擾

	// ���_���̍쐬
	MakeVertex(pDevice);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CLine2D::Uninit(void)
{
	// ���_�o�b�t�@�̊J��
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	// ������j��
	CScene::Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CLine2D::Update(void)
{
	m_rot.z += 1.0f;
}

//=============================================================================
// �`�揈��
//=============================================================================
void CLine2D::Draw(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, NULL);

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_LINELIST, 0, 1);
}

//=============================================================================
// �n�_�ƏI�_�̍��W�̐ݒ�
//=============================================================================
void CLine2D::SetPoint(D3DXVECTOR3 start, D3DXVECTOR3 end)
{
	// �I�_�̍��W�̐ݒ�
	m_end = end;
	m_start = start;

	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_�̍��W
	pVtx[0].pos = m_start;
	pVtx[1].pos = m_end;

	// ���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}

//=============================================================================
// �n�_�̐ݒ�
//=============================================================================
void CLine2D::SetStart(D3DXVECTOR3 start)
{
	m_start = start;

	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_�̍��W
	pVtx[0].pos = m_start;

	// ���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}

//=============================================================================
// �I�_�̐ݒ�
//=============================================================================
void CLine2D::SetEnd(D3DXVECTOR3 end)
{
	m_end = end;

	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_�̍��W
	pVtx[1].pos = m_end;

	// ���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}

//=============================================================================
// �F�̐ݒ�
//=============================================================================
void CLine2D::SetColor(D3DXCOLOR col)
{
	// �F�̐ݒ�
	m_col = col;

	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_�̍��W
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;

	// ���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}

//=============================================================================
// ���_���̍쐬
//=============================================================================
void CLine2D::MakeVertex(LPDIRECT3DDEVICE9 pDevice)
{
	// ���_�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 2,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_�̍��W
	pVtx[0].pos = m_start;
	pVtx[1].pos = m_end;

	// �@���̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;

	// ���_�J���[
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;

	//�e�N�X�`�����W
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 1.0f);

	// ���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}

//=========================================
// �ڑ������Ƃ�
//=========================================
void CLine2D::Link(D3DXVECTOR3 end)
{
	// ���_����ݒ�
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos = m_start;
	pVtx[1].pos = m_end;

	//���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}