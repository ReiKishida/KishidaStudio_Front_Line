//=============================================================================
//
// �p�[�e�B�N������ [particle.cpp]
// Author : Ishida Takuto
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "particle.h"
#include "renderer.h"
#include "manager.h"
#include "effect.h"
#include <stdio.h>

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define PARTICLE_FILENAME	"data/TEXT/EFFECT/particle.txt"				// �p�[�e�B�N���f�[�^
#define TEXTURE_FILENAME	"data/TEXT/EFFECT/particle_texture.txt"		// �e�N�X�`����

//*****************************************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************************************
LPDIRECT3DTEXTURE9 *CParData::m_pTexture = NULL;	// �e�N�X�`���̏��
int CParData::m_nNumTexture = 0;					// �e�N�X�`���̐�
CParData::PAR_DATA **CParData::m_pParticleData = NULL;	// �t�@�C������ǂݎ�����f�[�^
int CParData::m_nNumParticle = 0;					// �p�[�e�B�N����

//=========================================
// �R���X�g���N�^
//=========================================
CParticle::CParticle(int nPriority, CScene::OBJTYPE objType) : CScene(nPriority, objType)
{
	m_nType = 0;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//=========================================
// �f�X�g���N�^
//=========================================
CParticle::~CParticle()
{
}

//==================================
// ��������
//==================================
CParticle *CParticle::Create(D3DXVECTOR3 pos, int nType)
{
	CParticle *pParticle;

	pParticle = new CParEmitter;	// ���������m��
	pParticle->m_pos = pos;
	pParticle->m_nType = nType;
	pParticle->Init();

	return pParticle;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CParticle::Init(void)
{
	// �I�u�W�F�N�g�^�C�v�̐ݒ�
	CScene::SetObjType(CScene::OBJTYPE_PARTICLE);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CParticle::Uninit(void)
{
	Release();
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//		�p�[�e�B�N���̃G�~�b�^
//
//=========================================
// �R���X�g���N�^
//=========================================
CParEmitter::CParEmitter()
{
	m_nCntParticle = 0;
	m_nAppear = 0;
	m_nCntAngle = 0;

	m_nLife = 0;
	m_nType = 0;
	m_nAppear = 0;
	m_nLap = 0;
	m_bLoop = false;
}

//=========================================
// �f�X�g���N�^
//=========================================
CParEmitter::~CParEmitter()
{
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CParEmitter::Init(void)
{
	CParticle::Init();
	Set(m_pos, m_nType);	// �G�~�b�^�̐ݒ�

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CParEmitter::Uninit(void)
{
	CParticle::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CParEmitter::Update(void)
{
	bool bDelete = false;
	m_nLife--;	// �G�~�b�^�̕\�����Ԃ��J�E���g

	if (m_nLife <= 0 && m_bLoop == false)
	{// ��莞�Ԍo�߂ŃG�~�b�^������
		bDelete = true;
	}

	// �o��������Ԋu�̃J�E���g
	m_nCntParticle = (m_nCntParticle + 1) % m_nAppear;

	if (m_nCntParticle == 0)
	{// �o��������
		CParticlePolygon::Create(m_pos, m_nType, m_nTexture, m_nCntAngle);

		m_nCntAngle = (m_nCntAngle + 1) % m_nLap;
	}

	if (true == bDelete)
	{// ����
		Uninit();
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CParEmitter::Draw(void)
{
}

//=============================================================================
// �G�~�b�^�̐ݒ�
//=============================================================================
void CParEmitter::Set(D3DXVECTOR3 pos, int nType)
{
	m_pos = pos;
	m_nType = nType;
	m_nAppear = CParData::GetAppear(m_nType);
	m_nLife = CParData::GetLife(m_nType);
	m_nTexture = CParData::GetTextureIdx(m_nType);
	m_nLap = CParData::GetLap(m_nType);
}

//=============================================================================
// �G�~�b�^�̈ʒu�̐ݒ�
//=============================================================================
void CParEmitter::SetPosParticleEmitter(int nIdx, D3DXVECTOR3 pos)
{
	// �ʒu���X�V
	m_pos = pos;
}

//=============================================================================
// �G�~�b�^�̍폜
//=============================================================================
void CParEmitter::DestroyParticle(int nIdx)
{
	// ����
	CParEmitter::Uninit();
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//		�p�[�e�B�N���̃|���S��
//
//==================================
// ��������
//==================================
CParticlePolygon *CParticlePolygon::Create(D3DXVECTOR3 pos, int nType, int nTexture, int nAngle)
{
	CParticlePolygon *pParticlePlygon;

	pParticlePlygon = new CParticlePolygon;	// ���������m��

	if (NULL != pParticlePlygon)
	{
		pParticlePlygon->m_nType = nType;
		pParticlePlygon->m_nCntAngle = nAngle;
		pParticlePlygon->Init(pos);
		pParticlePlygon->BindTexture(CParData::GetTexture(nTexture));
	}

	return pParticlePlygon;
}

//=========================================
// �R���X�g���N�^
//=========================================
CParticlePolygon::CParticlePolygon(int nPriority) : CScene3DBill(nPriority)
{
	m_bRotVec = false;
	m_bDraw = false;
	m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	m_fAngle = 0.0f;
	m_fBouncing = 0.0f;
	m_fCntRot = 0.0f;
	m_fGravity = 0.0f;
	m_fLength = 0.0f;
	m_fRadius = 0.0f;
	m_fRotAngle = 0.0f;
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nCntAngle = 0;
	m_nLife = 0;
	m_nType = 0;
	m_nLifeMax = 0;
}

//=========================================
// �f�X�g���N�^
//=========================================
CParticlePolygon::~CParticlePolygon()
{
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CParticlePolygon::Init(D3DXVECTOR3 pos)
{
	// �I�u�W�F�N�g�^�C�v�̐ݒ�
	CScene::SetObjType(CScene::OBJTYPE_PARTICLE);

	// ����������
	CScene3DBill::Init();
	CScene3DBill::SetPos(pos);

	int nLap = CParData::GetLap(m_nType);				// ����̐�
	int *nLife = CParData::GetParLife(m_nType);
	int *nRadius = CParData::GetRadius(m_nType);
	int *nWidth = CParData::GetWidth(m_nType);
	int *nHeight = CParData::GetHeight(m_nType);
	int *nDepth = CParData::GetDepth(m_nType);
	float fSpread = CParData::GetSpread(m_nType);
	D3DXCOLOR col = CParData::GetColor(m_nType);
	bool bDraw = CParData::GetDraw(m_nType);
	float fGravity = CParData::GetGravity(m_nType);

	m_fAngle = 1.0f / (nLap / 2);
	if (0 != nHeight[1] && 0 != nHeight[0])
	{// Y���ړ��ʂ������O�ȊO
		m_move = D3DXVECTOR3(sinf(D3DX_PI * (0.0f + (m_fAngle * m_nCntAngle))) * ((nWidth[0] + (rand() % (1 + nWidth[1]))) - ((rand() % 10) * 0.1f)) * fSpread,
			((rand() % nHeight[1]) + (rand() % nHeight[0])) * fSpread,
			cosf(D3DX_PI * (0.0f + (m_fAngle * m_nCntAngle))) * ((nDepth[0] + (rand() % (1 + nDepth[1]))) - ((rand() % 10) * 0.1f)) * fSpread);
	}
	else
	{// Y���̈ړ��ʂ��O�̏ꍇ
		m_move = D3DXVECTOR3(sinf(D3DX_PI * (0.0f + (m_fAngle * m_nCntAngle))) * ((nWidth[0] + (rand() % (1 + nWidth[1]))) - ((rand() % 10) * 0.1f)) * fSpread,
			(nHeight[1] + nHeight[0]) * fSpread,
			cosf(D3DX_PI * (0.0f + (m_fAngle * m_nCntAngle))) * ((nDepth[0] + (rand() % (1 + nDepth[1]))) - ((rand() % 10) * 0.1f)) * fSpread);
	}

	if(1 == m_nType)
	{
		m_col = D3DXCOLOR(col.r, (rand() % 3 + 4) * 0.1f, (rand() % 2 + 1) * 0.1f, 1.0f);
	}
	else { m_col = col; }

	CScene3DBill::SetColor(m_col);
	m_fRadius = nRadius[0] + (rand() % (1 + (nRadius[1] - nRadius[0])) - 1) + ((rand() % 10) * 0.1f);
	m_nLife = nLife[0] + (rand() % (1 + (nLife[1] - nLife[0])) - 1);
	m_nLifeMax = m_nLife;
	m_fGravity = fGravity;
	m_bDraw = bDraw;

	// ���_�̊p�x
	m_fRotAngle = atan2f(pos.x - (pos.x + m_fRadius), pos.y - (pos.y + m_fRadius));

	// ���S���璸�_�܂ł̋���
	m_fLength = sqrtf((m_fRadius * m_fRadius) + (m_fRadius * m_fRadius));

	// ��]�������_���������Ƃ��̉�]����
	if (rand() % 2 == 0)
	{// �E���
		m_bRotVec = true;
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CParticlePolygon::Uninit(void)
{
	CScene3DBill::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CParticlePolygon::Update(void)
{
	m_nLife--;		// �\�����Ԃ��J�E���g

	if (m_nLife <= 0)
	{// �\�����Ԃ�0�ȉ��ɂȂ���
		Uninit();	// �I������
	}
	else
	{
		D3DXVECTOR3 pos = CScene3DBill::GetPos();
		D3DXCOLOR colFlame = CParData::GetFlameColor(m_nType);
		float fFlameRadius = CParData::GetFlameRadius(m_nType);
		bool bBouncing = CParData::GetBouncing(m_nType);
		int nRotType = CParData::GetRotType(m_nType);
		float fRotSpeed = CParData::GetRotSpeed(m_nType);
		m_move.y -= m_fGravity;		// �d�͂�������

		m_fBouncing = pos.y;		// �l���L��

		pos += m_move;				// �ʒu�̍X�V
		m_col += colFlame;			// �F�̍X�V
		m_fRadius += fFlameRadius;	// �傫���̍X�V

		if (pos.y <= 0 && bBouncing == true)
		{// �o�E���h
			m_move.y += (m_fBouncing - pos.y) * 2.0f;
		}

		if (nRotType == 1)
		{// ���v���
			m_fCntRot -= fRotSpeed;			// ��]�̃J�E���^
		}
		else if (nRotType == 2)
		{// �����v���
			m_fCntRot += fRotSpeed;			// ��]�̃J�E���^
		}
		else if (nRotType == 3)
		{// �����_��
			if (m_bRotVec)
			{// ���v���
				m_fCntRot += fRotSpeed;		// ��]�̃J�E���^
			}
			else
			{// �����v���
				m_fCntRot -= fRotSpeed;		// ��]�̃J�E���^
			}
		}

		// ���_�o�b�t�@�̎擾
		LPDIRECT3DVERTEXBUFFER9 pVtxBuff = CScene3DBill::GetVtxBuff();

		// ���_����ݒ�
		VERTEX_3D *pVtx;	// ���_���̃|�C���^

		// ���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
		pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		//���_���W�̐ݒ�
		pVtx[0].pos.x = sinf(m_fRotAngle + m_fCntRot) * -(m_fLength + m_fRadius);
		pVtx[0].pos.y = cosf(m_fRotAngle + m_fCntRot) * (m_fLength + m_fRadius);
		pVtx[1].pos.x = sinf(m_fRotAngle - m_fCntRot) * (m_fLength + m_fRadius);
		pVtx[1].pos.y = cosf(m_fRotAngle - m_fCntRot) * (m_fLength + m_fRadius);
		pVtx[2].pos.x = sinf(m_fRotAngle - m_fCntRot) * -(m_fLength + m_fRadius);
		pVtx[2].pos.y = cosf(m_fRotAngle - m_fCntRot) * -(m_fLength + m_fRadius);
		pVtx[3].pos.x = sinf(m_fRotAngle + m_fCntRot) * (m_fLength + m_fRadius);
		pVtx[3].pos.y = cosf(m_fRotAngle + m_fCntRot) * -(m_fLength + m_fRadius);

		// ���_�o�b�t�@���A�����b�N
		pVtxBuff->Unlock();

		// ���_�o�b�t�@�̐ݒ�
		CScene3DBill::SetVtxBuff(pVtxBuff);

		//���_�J���[
		CScene3DBill::SetColor(m_col);

		CScene3DBill::SetPos(pos);

		if (1 == m_nType)
		{
			CEffect::Create(pos, m_col, m_fLength + m_fRadius, m_nLifeMax / 3);
		}
	}
}

//=============================================================================
// �|���S���`�揈��
//=============================================================================
void CParticlePolygon::Draw(void)
{
	// �f�o�C�X�̎擾
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	// ���Ȃ��悤�ɂ���
	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

	if (m_bDraw == true)
	{ // ���u�����f�B���O�����Z�����ɐݒ�
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	}
	else
	{// ���u�����f�B���O�����ɖ߂�
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	}

	// ���C�e�B���O�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// �`��
	CScene3DBill::Draw();

	// ���C�e�B���O��L���ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// ���u�����f�B���O�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// Z�o�b�t�@�����ɖ߂�
	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//		�p�[�e�B�N���̃f�[�^
//
//=========================================
// �R���X�g���N�^
//=========================================
CParData::CParData()
{
}

//=========================================
// �f�X�g���N�^
//=========================================
CParData::~CParData()
{
}

//=============================================================================
// �t�@�C������f�[�^���擾
//=============================================================================
HRESULT CParData::Load(void)
{
	FILE *pFile;
	int nCntEffect = 0;
	char aStr[128];

	// �t�@�C�����J��
	pFile = fopen(PARTICLE_FILENAME, "r");

	if (NULL != pFile)
	{
		while (strcmp(aStr, "END_SCRIPT") != 0)
		{// �I�����b�Z�[�W�܂Ń��[�v
			fscanf(pFile, "%s", aStr);

			if (strcmp(aStr, "NUM_PARTICLE") == 0)
			{// �p�[�e�B�N����
				fscanf(pFile, " = %d", &m_nNumParticle);
				m_pParticleData = new CParData::PAR_DATA*[m_nNumParticle];
			}
			if (strcmp(aStr, "PARTICLESET") == 0)
			{// �G�~�b�^�̏��
				m_pParticleData[nCntEffect] = new CParData::PAR_DATA;
				while (strcmp(aStr, "END_PARTICLESET") != 0)
				{// �I�����b�Z�[�W�܂Ń��[�v
					fscanf(pFile, "%s", &aStr);

					if (strcmp(aStr, "MAXMOVE") == 0)
					{// �ړ��ʂ̍ő�
						fscanf(pFile, " = %d %d %d", &m_pParticleData[nCntEffect]->nWidth[1], &m_pParticleData[nCntEffect]->nHeight[1], &m_pParticleData[nCntEffect]->nDepth[1]);
					}
					else if (strcmp(aStr, "MINMOVE") == 0)
					{// �ړ��ʂ̍ŏ�
						fscanf(pFile, " = %d %d %d", &m_pParticleData[nCntEffect]->nWidth[0], &m_pParticleData[nCntEffect]->nHeight[0], &m_pParticleData[nCntEffect]->nDepth[0]);
					}
					else if (strcmp(aStr, "INITCOL") == 0)
					{// �����J���[
						fscanf(pFile, " = %f %f %f %f", &m_pParticleData[nCntEffect]->col.r, &m_pParticleData[nCntEffect]->col.g, &m_pParticleData[nCntEffect]->col.b, &m_pParticleData[nCntEffect]->col.a);
					}
					else if (strcmp(aStr, "FLAMECOL") == 0)
					{// �t���[���ɂ��J���[�̕ω�
						fscanf(pFile, " = %f %f %f %f", &m_pParticleData[nCntEffect]->flameCol.r, &m_pParticleData[nCntEffect]->flameCol.g, &m_pParticleData[nCntEffect]->flameCol.b, &m_pParticleData[nCntEffect]->flameCol.a);
					}
					else if (strcmp(aStr, "INITSIZE") == 0)
					{// �����T�C�Y
						fscanf(pFile, " = %d %d", &m_pParticleData[nCntEffect]->nRadius[0], &m_pParticleData[nCntEffect]->nRadius[1]);
					}
					else if (strcmp(aStr, "FLAMESIZE") == 0)
					{// �t���[���ɂ��T�C�Y�̕ω�
						fscanf(pFile, " = %f", &m_pParticleData[nCntEffect]->fFlameRadius);
					}
					else if (strcmp(aStr, "EMITTERLIFE") == 0)
					{// �\������(����)
						fscanf(pFile, " = %d", &m_pParticleData[nCntEffect]->nLife);
					}
					else if (strcmp(aStr, "PARLIFE") == 0)
					{// �p�[�e�B�N���̕\������
						fscanf(pFile, " = %d %d", &m_pParticleData[nCntEffect]->nParLife[0], &m_pParticleData[nCntEffect]->nParLife[1]);
					}
					else if (strcmp(aStr, "APPEAR") == 0)
					{// �o��Ԋu
						fscanf(pFile, " = %d", &m_pParticleData[nCntEffect]->nAppear);
					}
					else if (strcmp(aStr, "SPREAD") == 0)
					{// �L����
						fscanf(pFile, " = %f", &m_pParticleData[nCntEffect]->fSpread);
					}
					else if (strcmp(aStr, "LAP") == 0)
					{// 1���ɏo����
						fscanf(pFile, " = %d", &m_pParticleData[nCntEffect]->nLap);
					}
					else if (strcmp(aStr, "GRAVITY") == 0)
					{// �d��
						fscanf(pFile, " = %f", &m_pParticleData[nCntEffect]->fGravity);
					}
					else if (strcmp(aStr, "ROTPATTERN") == 0)
					{// ��]�̃p�^�[��
						fscanf(pFile, " = %d", &m_pParticleData[nCntEffect]->nRot);
					}
					else if (strcmp(aStr, "ROTSPEED") == 0)
					{// ��]�̑��x
						fscanf(pFile, " = %f", &m_pParticleData[nCntEffect]->fRotSpeed);
					}
					else if (strcmp(aStr, "LOOP") == 0)
					{// ���[�v���邩�ǂ���
						int nBool;
						fscanf(pFile, " = %d", &nBool);
						if (0 == nBool) { m_pParticleData[nCntEffect]->bLoop = false; }
						else { m_pParticleData[nCntEffect]->bLoop = true; }
					}
					else if (strcmp(aStr, "BOUND") == 0)
					{// �o�E���h���邩�ǂ���
						int nBool;
						fscanf(pFile, " = %d", &nBool);
						if (0 == nBool) { m_pParticleData[nCntEffect]->bBouncing = false; }
						else { m_pParticleData[nCntEffect]->bBouncing = true; }
					}
					else if (strcmp(aStr, "TEXTURE") == 0)
					{// �e�N�X�`���̎��
						fscanf(pFile, " = %d", &m_pParticleData[nCntEffect]->nTexture);
					}
					else if (strcmp(aStr, "DRAW") == 0)
					{// ���Z�����̗L��
						int nBool;
						fscanf(pFile, " = %d", &nBool);
						if (0 == nBool) { m_pParticleData[nCntEffect]->bDraw = false; }
						else{ m_pParticleData[nCntEffect]->bDraw = true; }
					}
				}
				nCntEffect++;	// �G�~�b�^�ԍ��̃J�E���g

				if (nCntEffect >= m_nNumParticle)
				{// �w�肵�����𒴂���
					break;
				}
			}
		}

		// �t�@�C�������
		fclose(pFile);
	}
	else
	{// ���s
		MessageBox(0, "�t�@�C�����J���܂���ł���", "particle.txt", MB_OK);
	}

	// �f�o�C�X�̎擾
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	char *pFileName = NULL;		// �e�N�X�`���̖��O
	int nCntTexture;

	// �t�@�C�����J��
	pFile = fopen(TEXTURE_FILENAME, "r");

	if (NULL != pFile)
	{
		// �e�N�X�`����
		fscanf(pFile, "%d", &m_nNumTexture);

		// �������m��
		m_pTexture = new LPDIRECT3DTEXTURE9[m_nNumTexture];
		pFileName = new char[128];

		for (nCntTexture = 0; nCntTexture < m_nNumTexture; nCntTexture++)
		{// �e�N�X�`���������[�v
			fscanf(pFile, "%s\n", &pFileName[0]);	// �e�N�X�`����

			// �e�N�X�`���̓ǂݍ���
			D3DXCreateTextureFromFile(pDevice, &pFileName[0], &m_pTexture[nCntTexture]);
		}

		// �j��
		delete[] pFileName;

		fclose(pFile);
	}
	else
	{// ���s
		MessageBox(0, "�t�@�C�����J���܂���ł���", "particle_texture.txt", MB_OK);
	}

	return S_OK;
}

//=============================================================================
// �f�[�^�̊J��
//=============================================================================
void CParData::Unload(void)
{
	for (int nCntParticle = 0; nCntParticle < m_nNumParticle; nCntParticle++)
	{// �ǂݎ�����f�[�^��j��
		if (NULL != m_pParticleData[nCntParticle])
		{// �������̊J��
			delete m_pParticleData[nCntParticle];
			m_pParticleData[nCntParticle] = NULL;
		}

		if (NULL != m_pTexture[nCntParticle])
		{// �e�N�X�`���̊J��
			m_pTexture[nCntParticle]->Release();
			m_pTexture[nCntParticle] = NULL;
		}
	}

	if (NULL != m_pParticleData)
	{// �������̊J��
		delete[] m_pParticleData;
		m_pParticleData = NULL;
	}

	if (NULL != m_pTexture)
	{// �e�N�X�`���̔j��
		delete[] m_pTexture;
		m_pTexture = NULL;
	}

	// �l�������l�ɂ���
	m_nNumTexture = 0;
	m_nNumParticle = 0;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CParData::Init(void)
{
	for (int nCntParticle = 0; nCntParticle < m_nNumParticle; nCntParticle++)
	{// �\���̂̃f�[�^�ɏ����l������
		for (int nCntArray = 0; nCntArray < 2; nCntArray++)
		{
			m_pParticleData[nCntParticle]->nWidth[nCntArray] = 0;
			m_pParticleData[nCntParticle]->nHeight[nCntArray] = 0;
			m_pParticleData[nCntParticle]->nDepth[nCntArray] = 0;
			m_pParticleData[nCntParticle]->nRadius[nCntArray] = 0;
			m_pParticleData[nCntParticle]->nParLife[nCntArray] = 0;
		}
		m_pParticleData[nCntParticle]->col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		m_pParticleData[nCntParticle]->flameCol = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		m_pParticleData[nCntParticle]->fFlameRadius = 0.0f;
		m_pParticleData[nCntParticle]->nLife = 0;
		m_pParticleData[nCntParticle]->nTexture = 0;
		m_pParticleData[nCntParticle]->fSpread = 0.0f;
		m_pParticleData[nCntParticle]->nAppear = 0;
		m_pParticleData[nCntParticle]->fGravity = 0.0f;
		m_pParticleData[nCntParticle]->nLap = 0;
		m_pParticleData[nCntParticle]->nRot = 0;
		m_pParticleData[nCntParticle]->fRotSpeed = 0.0f;
		m_pParticleData[nCntParticle]->bLoop = false;
		m_pParticleData[nCntParticle]->bBouncing = false;
		m_pParticleData[nCntParticle]->bDraw = false;
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CParData::Uninit(void)
{

}
