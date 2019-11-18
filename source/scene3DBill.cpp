//=============================================================================
//
// 3D�r���{�[�h�I�u�W�F�N�g���� [scene3DBill.h]
// Author : Ishida Takuto
//
//=============================================================================
#include "scene3DBill.h"
#include "renderer.h"
#include "manager.h"

//==================================
// �}�N����`
//==================================

//==================================
// �ÓI�����o�ϐ��錾
//==================================

//=========================================
// ��������
//=========================================
CScene3DBill *CScene3DBill::Create(void)
{
	CScene3DBill *pScene3DBill = new CScene3DBill;

	if (NULL != pScene3DBill)
	{// �������m�ې���
		pScene3DBill->Init();
	}

	return pScene3DBill;
}

//=========================================
// �e�N�X�`�������蓖�Ă�
//=========================================
void CScene3DBill::BindTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
}

//=========================================
// �R���X�g���N�^
//=========================================
CScene3DBill::CScene3DBill(int nPriority, CScene::OBJTYPE objType) : CScene(nPriority, objType)
{
	m_pTexture = NULL;		// �e�N�X�`���ւ̃|�C���^
	m_pVtxBuff = NULL;		// ���_�o�b�t�@�ւ̃|�C���^
	m_pMtxParent = NULL;	// �e�̃��[���h�}�g���b�N�X
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ʒu
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �O��̈ʒu
	m_fWidth = 0.0f;							// ��
	m_fHeight = 0.0f;							// ����
	m_cmpFunc = D3DCMP_LESSEQUAL;				// Z�o�b�t�@�̐ݒ�
	m_bZBuffer = false;							// Z�o�b�t�@�̐ݒ�����邩�ǂ���
	m_bLighting = true;							// ���C�e�B���O�̗L��
	m_bDisp = true;
}

//=========================================
// �f�X�g���N�^
//=========================================
CScene3DBill::~CScene3DBill()
{
}

//=========================================
// �|���S���̏���������
//=========================================
HRESULT CScene3DBill::Init(void)
{
	// �f�o�C�X�̎擾
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

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

	pVtx[0].pos = D3DXVECTOR3(-m_fWidth, m_fHeight, 0.0f) * 0.5f;
	pVtx[1].pos = D3DXVECTOR3(m_fWidth, m_fHeight, 0.0f) * 0.5f;
	pVtx[2].pos = D3DXVECTOR3(-m_fWidth, -m_fHeight, 0.0f) * 0.5f;
	pVtx[3].pos = D3DXVECTOR3(m_fWidth, -m_fHeight, 0.0f) * 0.5f;

	// �@��
	pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

	// ���_�J���[
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

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
// �|���S���̏I������
//=========================================
void CScene3DBill::Uninit(void)
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
// �|���S���̍X�V����
//=========================================
void CScene3DBill::Update(void)
{
}

//=========================================
// �|���S���̕`�揈��
//=========================================
void CScene3DBill::Draw(void)
{
	if (!m_bDisp) { return; }

	// �f�o�C�X�̎擾
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	if (m_bZBuffer)
	{// Z�o�b�t�@�̐ݒ�
		pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
		pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	}

	// ���e�X�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	if (!m_bLighting)
	{// ���C�e�B���O�𖳌��ɂ���
		pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	}

	D3DXMATRIX mtxTrans, mtxView;	// ���[���h�}�g���b�N�X�̕ۑ��p

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// ��]�𔽉f
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);
	m_mtxWorld._11 = mtxView._11;
	m_mtxWorld._12 = mtxView._21;
	m_mtxWorld._13 = mtxView._31;
	m_mtxWorld._21 = mtxView._12;
	m_mtxWorld._22 = mtxView._22;
	m_mtxWorld._23 = mtxView._32;
	m_mtxWorld._31 = mtxView._13;
	m_mtxWorld._32 = mtxView._23;
	m_mtxWorld._33 = mtxView._33;

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

	if (!m_bLighting)
	{// ���C�e�B���O��L���ɂ���
		pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	}

	if (m_bZBuffer)
	{// Z�o�b�t�@�̐ݒ��߂�
		pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
		pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	}

	// ���e�X�g�̐ݒ��߂�
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
}

//=========================================
// �����蔻��
//=========================================
bool CScene3DBill::Collision(D3DXVECTOR3 pos, float fRadius)
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
// �F�̐ݒ�
//=========================================
void CScene3DBill::SetColor(D3DXCOLOR col)
{
	// �F�̐ݒ�
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
void CScene3DBill::AddColor(D3DXCOLOR col)
{
	// �F�̉��Z
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
// �T�C�Y�̐ݒ�
//=========================================
void CScene3DBill::SetSize(D3DXVECTOR3 size)
{
	// �T�C�Y�̐ݒ�
	m_fWidth = size.x;
	m_fHeight = size.y;

	// ���_����ݒ�
	VERTEX_3D *pVtx;	// ���_���̃|�C���^

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos = D3DXVECTOR3(-m_fWidth, m_fHeight, size.z) * 0.5f;
	pVtx[1].pos = D3DXVECTOR3(m_fWidth, m_fHeight, size.z) * 0.5f;
	pVtx[2].pos = D3DXVECTOR3(-m_fWidth, -m_fHeight, size.z) * 0.5f;
	pVtx[3].pos = D3DXVECTOR3(m_fWidth, -m_fHeight, size.z) * 0.5f;

	//���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}

//=========================================
// ���̐ݒ�
//=========================================
void CScene3DBill::SetWidth(float fWidth)
{
	m_fWidth = fWidth;

	// ���_����ݒ�
	VERTEX_3D *pVtx;	// ���_���̃|�C���^

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos.x = -m_fWidth * 0.5f;
	pVtx[1].pos.x = m_fWidth * 0.5f;
	pVtx[2].pos.x = -m_fWidth * 0.5f;
	pVtx[3].pos.x = m_fWidth * 0.5f;

	//���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}

//=========================================
// �����̐ݒ�
//=========================================
void CScene3DBill::SetHeight(float fHeight)
{
	m_fHeight = fHeight;

	// ���_����ݒ�
	VERTEX_3D *pVtx;	// ���_���̃|�C���^

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos.y = m_fHeight * 0.5f;
	pVtx[1].pos.y = m_fHeight * 0.5f;
	pVtx[2].pos.y = -m_fHeight * 0.5f;
	pVtx[3].pos.y = -m_fHeight * 0.5f;

	//���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}

//=========================================
// Z�o�b�t�@�̐ݒ�
//=========================================
void CScene3DBill::SetZBuffer(D3DCMPFUNC cmpFunc)
{
	m_cmpFunc = cmpFunc;

	if (!m_bZBuffer)
	{// Z�o�b�t�@�̐ݒ������
		m_bZBuffer = true;
	}
}