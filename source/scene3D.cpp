//=============================================================================
//
// 3D�I�u�W�F�N�g���� [scene3D.cpp]
// Author : Ishida Takuto
//
//=============================================================================
#include "scene3D.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "debugProc.h"

//==================================
// �}�N����`
//==================================

//==================================
// �ÓI�����o�ϐ��錾
//==================================

//=========================================
// ��������
//=========================================
CScene3D *CScene3D::Create(void)
{
	CScene3D *pScene3D = new CScene3D;

	if (NULL != pScene3D)
	{// �������m�ې���
		pScene3D->Init();
	}

	return pScene3D;
}

//=========================================
// �e�N�X�`�������蓖�Ă�
//=========================================
void CScene3D::BindTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
}

//=========================================
// �R���X�g���N�^
//=========================================
CScene3D::CScene3D(int nPriority, CScene::OBJTYPE objType) : CScene(nPriority, objType)
{
	m_pTexture = NULL;		// �e�N�X�`���ւ̃|�C���^
	m_pVtxBuff = NULL;		// ���_�o�b�t�@�ւ̃|�C���^
	m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �|���S���̈ʒu
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �|���S���̌���
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_pMtxParent = NULL;
	m_blendOp = D3DBLENDOP_FORCE_DWORD;
	m_bLighting = true;
	m_bZtest = true;
}

//=========================================
// �f�X�g���N�^
//=========================================
CScene3D::~CScene3D()
{
}

//=========================================
// ����������
//=========================================
HRESULT CScene3D::Init(void)
{
	// �f�o�C�X�̎擾
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	// �F�̐ݒ�
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// ���_����ݒ�
	VERTEX_3D *pVtx;	// ���_���̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W
	pVtx[0].pos = D3DXVECTOR3(-m_size.x, m_size.y, m_size.z);
	pVtx[1].pos = D3DXVECTOR3(m_size.x, m_size.y, m_size.z);
	pVtx[2].pos = D3DXVECTOR3(-m_size.x, -m_size.y, -m_size.z);
	pVtx[3].pos = D3DXVECTOR3(m_size.x, -m_size.y, -m_size.z);

	pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

	// ���_�J���[
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	// �e�N�X�`�����W
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// ���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=========================================
// �I������
//=========================================
void CScene3D::Uninit(void)
{
	if (NULL != m_pVtxBuff)
	{// ���_�o�b�t�@�̔j��
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	// ������j��
	CScene::Release();
}

//=========================================
// �X�V����
//=========================================
void CScene3D::Update(void)
{
}

//=========================================
// �`�揈��
//=========================================
void CScene3D::Draw(void)
{
	// �f�o�C�X�̎擾
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	if (!m_bLighting)
	{// ���C�e�B���OOFF
		pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	}

	if (!m_bZtest)
	{// Z�e�X�gOFF
		pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	}

	// ���e�X�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	if (D3DBLENDOP_FORCE_DWORD != m_blendOp)
	{// ���u�����f�B���O�̐ݒ�
		pDevice->SetRenderState(D3DRS_BLENDOP, m_blendOp);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	}

	D3DXMATRIX mtxRot, mtxTrans;	// ���[���h�}�g���b�N�X�̕ۑ��p

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	if (m_pMtxParent)
	{// �e�̃}�g���b�N�X�Ɗ|�����킹��
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, m_pMtxParent);
	}

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, m_pTexture);

	// �`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	if (D3DBLENDOP_FORCE_DWORD != m_blendOp)
	{// ���u�����f�B���O�����ɖ߂�
		pDevice->SetRenderState(D3DRS_BLENDOP, m_blendOp);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	}

	// ���e�X�g�̐ݒ��߂�
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	if (!m_bZtest)
	{// Z�e�X�gON
		pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	}

	if (!m_bLighting)
	{// ���C�e�B���OON
		pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	}
}

//=========================================
// �ʒu�̐ݒ�
//=========================================
bool CScene3D::Collision(D3DXVECTOR3 pos, float fRadius)
{
	// �����XYZ�͈̔�
	float fLength = powf(fRadius, 2);

	float fLengthX = pos.x - m_pos.x;	// X�̍�
	float fLengthY = pos.y - m_pos.y;	// Y�̍�
	float fLengthZ = pos.z - m_pos.z;	// Z�̍�

	float fLengthTotal = powf(fLengthX, 2) + powf(fLengthY, 2) + powf(fLengthZ, 2);		// XYZ�̍��̓��

	if (fLength >= fLengthTotal)
	{// �͈̓`�F�b�N
		return true;	// �����L���ɂ���
	}

	return false;
}

//=========================================
// �ʒu�̐ݒ�
//=========================================
void CScene3D::SetPos(D3DXVECTOR3 pos)
{
	m_pos = pos;	// �ʒu�̐ݒ�

	//// ���_����ݒ�
	//VERTEX_3D *pVtx;	// ���_���̃|�C���^

	//// ���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	//m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//// ���_���W
	//pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_size.x, m_pos.y + m_size.y, m_pos.z + m_size.z);
	//pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_size.x, m_pos.y + m_size.y, m_pos.z + m_size.z);
	//pVtx[2].pos = D3DXVECTOR3(m_pos.x - m_size.x, m_pos.y - m_size.y, m_pos.z - m_size.z);
	//pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_size.x, m_pos.y - m_size.y, m_pos.z - m_size.z);

	//// ���_�o�b�t�@���A�����b�N
	//m_pVtxBuff->Unlock();
}

//=========================================
// �T�C�Y�̐ݒ�
//=========================================
void CScene3D::SetSize(D3DXVECTOR3 size)
{
	m_size = size;

	// ���_����ݒ�
	VERTEX_3D *pVtx;	// ���_���̃|�C���^

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos = D3DXVECTOR3(-m_size.x, m_size.y, m_size.z);
	pVtx[1].pos = D3DXVECTOR3(m_size.x, m_size.y, m_size.z);
	pVtx[2].pos = D3DXVECTOR3(-m_size.x, -m_size.y, -m_size.z);
	pVtx[3].pos = D3DXVECTOR3(m_size.x, -m_size.y, -m_size.z);

	//���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}

//=========================================
// ���̐ݒ�
//=========================================
void CScene3D::SetWidth(float fWidth)
{
	m_size.x = fWidth;

	// ���_����ݒ�
	VERTEX_3D *pVtx;	// ���_���̃|�C���^

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos.x = -m_size.x;
	pVtx[1].pos.x = m_size.x;
	pVtx[2].pos.x = -m_size.x;
	pVtx[3].pos.x = m_size.x;

	//���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}

//=========================================
// �����̐ݒ�
//=========================================
void CScene3D::SetHeight(float fHeight)
{
	m_size.y = fHeight;

	// ���_����ݒ�
	VERTEX_3D *pVtx;	// ���_���̃|�C���^

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos.y = m_size.y;
	pVtx[1].pos.y = m_size.y;
	pVtx[2].pos.y = -m_size.y;
	pVtx[3].pos.y = -m_size.y;

	//���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}

//=========================================
// ���s�̐ݒ�
//=========================================
void CScene3D::SetDepth(float fDepth)
{
	m_size.z = fDepth;

	// ���_����ݒ�
	VERTEX_3D *pVtx;	// ���_���̃|�C���^

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos.z = m_size.z;
	pVtx[1].pos.z = m_size.z;
	pVtx[2].pos.z = -m_size.z;
	pVtx[3].pos.z = -m_size.z;

	//���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}

//=========================================
// �F�̐ݒ�
//=========================================
void CScene3D::SetColor(D3DXCOLOR col)
{
	m_col = col;

	// ���_����ݒ�
	VERTEX_3D *pVtx;		// ���_���̃|�C���^

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//�e�N�X�`�����W
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	//���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}

//=========================================
// �F�̉��Z
//=========================================
void CScene3D::AddColor(D3DXCOLOR col)
{
	m_col += col;

	// ���_����ݒ�
	VERTEX_3D *pVtx;		// ���_���̃|�C���^

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//�e�N�X�`�����W
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	//���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}

//=========================================
// �e�N�X�`�����W�̐ݒ�
//=========================================
void CScene3D::SetTexPos(D3DXVECTOR2 *texPos)
{
	// ���_����ݒ�
	VERTEX_3D *pVtx;		// ���_���̃|�C���^

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//�e�N�X�`�����W
	pVtx[0].tex = texPos[0];
	pVtx[1].tex = texPos[1];
	pVtx[2].tex = texPos[2];
	pVtx[3].tex = texPos[3];

	//���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}

//=========================================
// �e�N�X�`�����W�𓮂���
//=========================================
void CScene3D::MoveTexPos(D3DXVECTOR2 speed)
{
	// ���_����ݒ�
	VERTEX_3D *pVtx;		// ���_���̃|�C���^

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//�e�N�X�`�����W
	pVtx[0].tex += speed;
	pVtx[1].tex += speed;
	pVtx[2].tex += speed;
	pVtx[3].tex += speed;

	//���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}
