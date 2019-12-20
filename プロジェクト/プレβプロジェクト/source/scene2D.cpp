//=============================================================================
//
// 2D�I�u�W�F�N�g���� [scene.cpp]
// Author : Takuto Ishida
//
//=============================================================================
#include "scene2D.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"

//==================================
// �}�N����`
//==================================

//==================================
// �ÓI�����o�ϐ��錾
//==================================

//=========================================
// ��������
//=========================================
CScene2D *CScene2D::Create(void)
{
	CScene2D *pScene2D = new CScene2D;

	if (pScene2D != NULL)
	{// �������m�ې���
		pScene2D->Init();
	}

	return pScene2D;
}

//=========================================
// �e�N�X�`�������蓖�Ă�
//=========================================
void CScene2D::BindTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
}

//=========================================
// �R���X�g���N�^
//=========================================
CScene2D::CScene2D(int nPriority, CScene::OBJTYPE objType) : CScene(nPriority, objType)
{
	m_pTexture = NULL;		// �e�N�X�`���ւ̃|�C���^
	m_pVtxBuff = NULL;		// ���_�o�b�t�@�ւ̃|�C���^

	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �|���S���̈ʒu
	m_fSizeX = 0.0f;
	m_fSizeY = 0.0f;
	m_bDisp = true;
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	for (int nCntVtx = 0; nCntVtx < 4; nCntVtx++)
	{
		m_aPosVtx[nCntVtx] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
}

//=========================================
// �f�X�g���N�^
//=========================================
CScene2D::~CScene2D()
{
}

//=========================================
// ����������
//=========================================
HRESULT CScene2D::Init(void)
{
	// �f�o�C�X�̎擾
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// ���_����ݒ�
	VERTEX_2D *pVtx;	// ���_���̃|�C���^

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W
	pVtx[0].pos = D3DXVECTOR3(-m_fSizeX, -m_fSizeY, 0.0f) + m_pos;
	pVtx[1].pos = D3DXVECTOR3(m_fSizeX, -m_fSizeY, 0.0f) + m_pos;
	pVtx[2].pos = D3DXVECTOR3(-m_fSizeX, m_fSizeY, 0.0f) + m_pos;
	pVtx[3].pos = D3DXVECTOR3(m_fSizeX, m_fSizeY, 0.0f) + m_pos;

	// 1.0f�ŌŒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

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
void CScene2D::Uninit(void)
{
	// ���_�o�b�t�@�̔j��
	if (NULL != m_pVtxBuff)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	// �����̔j��
	CScene::Release();
}

//=========================================
// �X�V����
//=========================================
void CScene2D::Update(void)
{
}

//=========================================
// �`�揈��
//=========================================
void CScene2D::Draw(void)
{
	if (!m_bDisp) { return; }

	// �f�o�C�X�̎擾
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	// ���e�X�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, m_pTexture);

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	// ���e�X�g�̐ݒ��߂�
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
}

//=========================================
// ��]�̐ݒ�
//=========================================
void CScene2D::SetRot(float fRotate)
{
	D3DXMATRIX PosMatrix, RotateMatrix;

	// ���_����ݒ�
	VERTEX_2D *pVtx;		// ���_���̃|�C���^

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// �|���S���̒��S�����_�ֈړ�������
	MoveMatrix(-m_pos.x, -m_pos.y);

	// �s��̏������i�P�ʍs�񐶐��j
	D3DXMatrixIdentity(&PosMatrix);
	D3DXMatrixIdentity(&RotateMatrix);

	// ��]�s��Ɋp�x��ݒ�
	D3DXMatrixRotationZ(&RotateMatrix, fRotate);

	// ��]����
	for (int nCntVertex = 0; nCntVertex < 4; nCntVertex++)
	{
		// ���݂̒��_���W���i�[
		D3DXMatrixTranslation(&PosMatrix, m_aPosVtx[nCntVertex].x, m_aPosVtx[nCntVertex].y, m_aPosVtx[nCntVertex].z);

		// ���Z
		PosMatrix *= RotateMatrix;

		// ���ʂ�߂�
		pVtx[nCntVertex].pos.x = PosMatrix._41;
		pVtx[nCntVertex].pos.y = PosMatrix._42;
		pVtx[nCntVertex].pos.z = PosMatrix._43;
	}

	// �|���S�������̈ʒu�֖߂�
	MoveMatrix(m_pos.x, m_pos.y);

	//���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}

//=========================================
// �s����g�����ړ�
//=========================================
void CScene2D::MoveMatrix(float fPosX, float fPosY)
{
	D3DXMATRIX PosMatrix, MoveMatrix;

	// �s��̏������i�P�ʍs�񐶐��j
	D3DXMatrixIdentity(&PosMatrix);
	D3DXMatrixIdentity(&MoveMatrix);

	// �ړ��ʐݒ�
	D3DXMatrixTranslation(&MoveMatrix, fPosX, fPosY, 0.0f);

	// ���_����ݒ�
	VERTEX_2D *pVtx;		// ���_���̃|�C���^

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// �ړ�����
	for (int nCntVertex = 0; nCntVertex < 4; nCntVertex++)
	{
		// ���݂̒��_���W���i�[
		D3DXMatrixTranslation(&PosMatrix, pVtx[nCntVertex].pos.x, pVtx[nCntVertex].pos.y, pVtx[nCntVertex].pos.z);

		// ���Z
		PosMatrix *= MoveMatrix;

		// ���ʂ�߂�
		pVtx[nCntVertex].pos.x = PosMatrix._41;
		pVtx[nCntVertex].pos.y = PosMatrix._42;
		pVtx[nCntVertex].pos.z = PosMatrix._43;
	}

	//���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}

//=========================================
// ���_���W�̐ݒ�
//=========================================
void CScene2D::SetPosVtx(D3DXVECTOR3 *pPosVtx)
{
	// ���_����ݒ�
	VERTEX_2D *pVtx;		// ���_���̃|�C���^

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ�
	pVtx[0].pos = pPosVtx[0];
	pVtx[1].pos = pPosVtx[1];
	pVtx[2].pos = pPosVtx[2];
	pVtx[3].pos = pPosVtx[3];

	m_aPosVtx[0] = pPosVtx[0];
	m_aPosVtx[1] = pPosVtx[1];
	m_aPosVtx[2] = pPosVtx[2];
	m_aPosVtx[3] = pPosVtx[3];

	//���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}

//=========================================
// �����蔻��
//=========================================
bool CScene2D::Collision(D3DXVECTOR3 pos, float fRange)
{
	bool bHit = false;

	// �����XY�͈̔�
	float fLength = powf(fRange, 2);

	float fLengthX = pos.x - m_pos.x;	// X�̍�
	float fLengthY = pos.y - m_pos.y;	// Y�̍�

	float fLengthTotal = powf(fLengthX, 2) + powf(fLengthY, 2);		// XY�̍��̓��

	if (fLength >= fLengthTotal)
	{// �͈̓`�F�b�N
		bHit = true;	// �����L���ɂ���
	}

	return bHit;	// ���茋�ʂ�Ԃ�
}

//=========================================
// �ʒu�̐ݒ�
//=========================================
void CScene2D::SetPos(D3DXVECTOR3 pos)
{
	m_pos = pos;

	// ���_����ݒ�
	VERTEX_2D *pVtx;		// ���_���̃|�C���^

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_�̍��W
	pVtx[0].pos = D3DXVECTOR3(-m_fSizeX, -m_fSizeY, 0.0f) * 0.5f + m_pos;
	pVtx[1].pos = D3DXVECTOR3(m_fSizeX, -m_fSizeY, 0.0f) * 0.5f + m_pos;
	pVtx[2].pos = D3DXVECTOR3(-m_fSizeX, m_fSizeY, 0.0f) * 0.5f + m_pos;
	pVtx[3].pos = D3DXVECTOR3(m_fSizeX, m_fSizeY, 0.0f) * 0.5f + m_pos;

	m_aPosVtx[0] = pVtx[0].pos;
	m_aPosVtx[1] = pVtx[1].pos;
	m_aPosVtx[2] = pVtx[2].pos;
	m_aPosVtx[3] = pVtx[3].pos;

	//���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}

//=========================================
// �T�C�Y�̐ݒ�
//=========================================
void CScene2D::SetSize(float fSizeX, float fSizeY)
{
	// �T�C�Y�̒l������
	m_fSizeX = fSizeX;
	m_fSizeY = fSizeY;

	// ���_����ݒ�
	VERTEX_2D *pVtx;		// ���_���̃|�C���^

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_�̍��W
	pVtx[0].pos = D3DXVECTOR3(-m_fSizeX, -m_fSizeY, 0.0f) * 0.5f + m_pos;
	pVtx[1].pos = D3DXVECTOR3(m_fSizeX, -m_fSizeY, 0.0f) * 0.5f + m_pos;
	pVtx[2].pos = D3DXVECTOR3(-m_fSizeX, m_fSizeY, 0.0f) * 0.5f + m_pos;
	pVtx[3].pos = D3DXVECTOR3(m_fSizeX, m_fSizeY, 0.0f) * 0.5f + m_pos;

	m_aPosVtx[0] = pVtx[0].pos;
	m_aPosVtx[1] = pVtx[1].pos;
	m_aPosVtx[2] = pVtx[2].pos;
	m_aPosVtx[3] = pVtx[3].pos;

	//���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}

//=========================================
// ���̐ݒ�
//=========================================
void CScene2D::SetWidth(float fWidth)
{
	// �T�C�Y�̒l������
	m_fSizeX = fWidth;

	// ���_����ݒ�
	VERTEX_2D *pVtx;		// ���_���̃|�C���^

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_�̍��W
	pVtx[0].pos.x = m_pos.x - (m_fSizeX * 0.5f);
	pVtx[1].pos.x = m_pos.x + (m_fSizeX * 0.5f);
	pVtx[2].pos.x = m_pos.x - (m_fSizeX * 0.5f);
	pVtx[3].pos.x = m_pos.x + (m_fSizeX * 0.5f);

	m_aPosVtx[0] = pVtx[0].pos;
	m_aPosVtx[1] = pVtx[1].pos;
	m_aPosVtx[2] = pVtx[2].pos;
	m_aPosVtx[3] = pVtx[3].pos;

	//���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}

//=========================================
// �����̐ݒ�
//=========================================
void CScene2D::SetHeight(float fHeight)
{
	// �T�C�Y�̒l������
	m_fSizeY = fHeight;

	// ���_����ݒ�
	VERTEX_2D *pVtx;		// ���_���̃|�C���^

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_�̍��W
	pVtx[0].pos.y = m_pos.y - (m_fSizeY * 0.5f);
	pVtx[1].pos.y = m_pos.y - (m_fSizeY * 0.5f);
	pVtx[2].pos.y = m_pos.y + (m_fSizeY * 0.5f);
	pVtx[3].pos.y = m_pos.y + (m_fSizeY * 0.5f);

	m_aPosVtx[0] = pVtx[0].pos;
	m_aPosVtx[1] = pVtx[1].pos;
	m_aPosVtx[2] = pVtx[2].pos;
	m_aPosVtx[3] = pVtx[3].pos;

	//���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}

//=========================================
// �F�̐ݒ�
//=========================================
void CScene2D::SetColor(D3DXCOLOR col)
{
	// �F�̐ݒ�
	m_col = col;

	// ���_����ݒ�
	VERTEX_2D *pVtx;		// ���_���̃|�C���^

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
void CScene2D::AddColor(D3DXCOLOR col)
{
	// �F�̐ݒ�
	m_col += col;

	// ���_����ݒ�
	VERTEX_2D *pVtx;		// ���_���̃|�C���^

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

//=============================================================================
// �_�ŏ���
//=============================================================================
void CScene2D::Flashing(int nFlame)
{
	// �w�肵���t���[����
	if (m_nCntFlash < nFlame)
	{// �����ɂ���
		m_col.a -= 1.0f / nFlame;
	}
	else
	{// �s�����ɂ���
		m_col.a += 1.0f / nFlame;
	}

	m_nCntFlash = (1 + m_nCntFlash) % (nFlame * 2);		// �J�E���g��i�߂�

	if (m_col.a <= 0)
	{// �A���t�@�l���O�ȉ��̂Ƃ�
		m_col.a = 0;
		m_nCntFlash = nFlame;
	}
	else if (m_col.a >= 1)
	{// �A���t�@�l���P�ȏ�̂Ƃ�
		m_col.a = 1;
		m_nCntFlash = 0;
	}

	// ���_����ݒ�
	VERTEX_2D *pVtx;		// ���_���̃|�C���^

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

//=============================================================================
// �w�i����������
//=============================================================================
void CScene2D::SetBgMove(int nCntBg_X, int nCntBg_Y, float fScrollSpeed)
{
	VERTEX_2D * pVtx;			//���_���ւ̃|�C���^

								//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//�e�N�X�`���̍��W�ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f + (nCntBg_X * (fScrollSpeed + 1)), 0.0f + (nCntBg_Y * (fScrollSpeed + 1)));
	pVtx[1].tex = D3DXVECTOR2(1.0f + (nCntBg_X * (fScrollSpeed + 1)), 0.0f + (nCntBg_Y * (fScrollSpeed + 1)));
	pVtx[2].tex = D3DXVECTOR2(0.0f + (nCntBg_X * (fScrollSpeed + 1)), 1.0f + (nCntBg_Y * (fScrollSpeed + 1)));
	pVtx[3].tex = D3DXVECTOR2(1.0f + (nCntBg_X * (fScrollSpeed + 1)), 1.0f + (nCntBg_Y * (fScrollSpeed + 1)));

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//=============================================================================
// �����������̉��Ԗڂ��g�����E���������E�����������F�e�N�X�`����UV�ݒ�
//=============================================================================
void CScene2D::SetTex(int nUV, int nUV_X, int nUV_Y)
{
	VERTEX_2D * pVtx;			//���_���ւ̃|�C���^

								//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//�e�N�X�`���̍��W�ݒ�
	pVtx[0].tex = D3DXVECTOR2((nUV % nUV_X) * (1.0f / nUV_X), (nUV / nUV_X) * (1.0f / nUV_Y));
	pVtx[1].tex = D3DXVECTOR2((nUV % nUV_X) * (1.0f / nUV_X) + (1.0f / nUV_X), (nUV / nUV_X) * (1.0f / nUV_Y));
	pVtx[2].tex = D3DXVECTOR2((nUV % nUV_X) * (1.0f / nUV_X), (nUV / nUV_X) * (1.0f / nUV_Y) + (1.0f / nUV_Y));
	pVtx[3].tex = D3DXVECTOR2((nUV % nUV_X) * (1.0f / nUV_X) + (1.0f / nUV_X), (nUV / nUV_X) * (1.0f / nUV_Y) + (1.0f / nUV_Y));

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//=============================================================================
// ���������e�N�X�`�����X�N���[�����鏈��
//=============================================================================
void CScene2D::SetTexMove(int nUV, int nUV_X, int nUV_Y, int nCntBg_X, int nCntBg_Y, float fScrollSpeed)
{
	VERTEX_2D * pVtx;			//���_���ւ̃|�C���^

								//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//�e�N�X�`���̍��W�ݒ�
	pVtx[0].tex = D3DXVECTOR2((nUV % nUV_X) * (1.0f / nUV_X) + (nCntBg_X * (fScrollSpeed + 1)), (nUV / nUV_X) * (1.0f / nUV_Y) + (nCntBg_Y * (fScrollSpeed + 1)));
	pVtx[1].tex = D3DXVECTOR2((nUV % nUV_X) * (1.0f / nUV_X) + (1.0f / nUV_X) + (nCntBg_X * (fScrollSpeed + 1)), (nUV / nUV_X) * (1.0f / nUV_Y) + (nCntBg_Y * (fScrollSpeed + 1)));
	pVtx[2].tex = D3DXVECTOR2((nUV % nUV_X) * (1.0f / nUV_X) + (nCntBg_X * (fScrollSpeed + 1)), (nUV / nUV_X) * (1.0f / nUV_Y) + (1.0f / nUV_Y) + (nCntBg_Y * (fScrollSpeed + 1)));
	pVtx[3].tex = D3DXVECTOR2((nUV % nUV_X) * (1.0f / nUV_X) + (1.0f / nUV_X) + (nCntBg_X * (fScrollSpeed + 1)), (nUV / nUV_X) * (1.0f / nUV_Y) + (1.0f / nUV_Y) + (nCntBg_Y * (fScrollSpeed + 1)));

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}
