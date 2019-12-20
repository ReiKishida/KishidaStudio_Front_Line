//=============================================================================
//
// �U�����󂯂������\������ [damageDirection.h]
// Author : Takuto Ishida
//
//=============================================================================
#include "damageDirection.h"
#include "scene2D.h"
#include "texture.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define DAMAGEDIRECTION_UPPER_WIDTH		(550.0f)			// �㉺�̕�
#define DAMAGEDIRECTION_UPPER_HEIGHT	(100.0f)			// �㉺�̍���
#define DAMAGEDIRECTION_SIDE_WIDTH		(200.0f)			// ���E�̕�
#define DAMAGEDIRECTION_SIDE_HEIGHT		(700.0f)			// ���E�̍���

#define DAMAGEDIRECTION_SIZE_FRONT_BACK	(D3DXVECTOR2(DAMAGEDIRECTION_UPPER_WIDTH, DAMAGEDIRECTION_UPPER_HEIGHT))	// �O�����̕\���ʒu
#define DAMAGEDIRECTION_SIZE_RIGHT_LEFT	(D3DXVECTOR2(DAMAGEDIRECTION_SIDE_WIDTH, DAMAGEDIRECTION_SIDE_HEIGHT))		// �E�����̕\���ʒu

#define DAMAGEDIRECTION_POS_FRONT	(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, 125.0f, 0.0f))					// �O�����̕\���ʒu
#define DAMAGEDIRECTION_POS_BACK	(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT - (DAMAGEDIRECTION_SIZE_FRONT_BACK.y * 0.5f), 0.0f))	// ������̕\���ʒu
#define DAMAGEDIRECTION_POS_RIGHT	(D3DXVECTOR3(SCREEN_WIDTH - (DAMAGEDIRECTION_SIZE_RIGHT_LEFT.x * 0.75f), SCREEN_HEIGHT / 2.0f, 0.0f))	// �E�����̕\���ʒu
#define DAMAGEDIRECTION_POS_LEFT	(D3DXVECTOR3(DAMAGEDIRECTION_SIZE_RIGHT_LEFT.x * 0.75f, SCREEN_HEIGHT / 2.0f, 0.0f))					// �������̕\���ʒu

//==================================
// �ÓI�����o�ϐ��錾
//==================================

/****************************************************************/
/*						2D�{�^���̃N���X						*/
/****************************************************************/
//==================================
// ��������
//==================================
CDamageDirection* CDamageDirection::Create(void)
{
	CDamageDirection *pDamageDirection;

	pDamageDirection = new CDamageDirection;	// ���������m��

	if (NULL != pDamageDirection)
	{// �������m�ې���
		pDamageDirection->Init();	// ����������
	}

	return pDamageDirection;
}

//=========================================
// �R���X�g���N�^
//=========================================
CDamageDirection::CDamageDirection(int nPriority, CScene::OBJTYPE objType) : CScene(nPriority, objType)
{
	for (int nCntDirection = 0; nCntDirection < DIRECTION_MAX; nCntDirection++)
	{
		m_nCntLife[nCntDirection] = 0;
		m_pPolygon[nCntDirection] = NULL;
		m_bHitDirection[nCntDirection] = false;
	}
}

//=========================================
// �f�X�g���N�^
//=========================================
CDamageDirection::~CDamageDirection()
{
}

//=========================================
// ����������
//=========================================
HRESULT CDamageDirection::Init(void)
{
	return S_OK;
}

//=========================================
// �I������
//=========================================
void CDamageDirection::Uninit(void)
{
	for (int nCntDirection = 0; nCntDirection < DIRECTION_MAX; nCntDirection++)
	{
		if (NULL != m_pPolygon[nCntDirection])
		{// �|���S���̔j��
			m_pPolygon[nCntDirection]->Uninit();
			m_pPolygon[nCntDirection] = NULL;
		}
	}

	// ������j��
	CScene::Release();
}

//=========================================
// �X�V����
//=========================================
void CDamageDirection::Update(void)
{
	for (int nCntDirection = 0; nCntDirection < DIRECTION_MAX; nCntDirection++)
	{
		if (0 < m_nCntLife[nCntDirection])
		{// ��������Ă���|���S���̕\�����Ԃ��J�E���g
			m_nCntLife[nCntDirection]--;

			if (NULL != m_pPolygon[nCntDirection] && 0 >= m_nCntLife[nCntDirection])
			{// �\�����Ԃ��߂����|���S���̔j��
				m_pPolygon[nCntDirection]->Uninit();
				m_pPolygon[nCntDirection] = NULL;
				m_nCntLife[nCntDirection] = 0;
			}
		}

		// ���������t���O��߂�
		m_bHitDirection[nCntDirection] = false;
	}
}

//=========================================
// �`�揈��
//=========================================
void CDamageDirection::Draw(void)
{
}

//=========================================
// �|���S����������
//=========================================
void CDamageDirection::CreatePolygon(D3DXVECTOR3 dirSelf, D3DXVECTOR3 dirObj)
{
	// �x�N�g���̂Ȃ��p�����߂�
	float fAngle = (dirSelf.x * dirObj.x) + (dirSelf.z * dirObj.z);
	fAngle = fAngle / (sqrtf((dirSelf.x * dirSelf.x) + (dirSelf.z * dirSelf.z)) * sqrtf((dirObj.x * dirObj.x) + (dirObj.z * dirObj.z)));
	fAngle = acosf(fAngle);

	// �O��
	if (fAngle >= D3DX_PI * 0.75f || fAngle <= D3DX_PI * -0.75f)
	{// �O��
		if (NULL == m_pPolygon[DIRECTION_FRONT])
		{
			m_pPolygon[DIRECTION_FRONT] = CScene2D::Create();
			m_pPolygon[DIRECTION_FRONT]->BindTexture(CTexture::GetTexture(CTexture::TEXTURE_HIT_UPPER));
			m_pPolygon[DIRECTION_FRONT]->SetTex(1, 1, 2);
			m_pPolygon[DIRECTION_FRONT]->SetPos(DAMAGEDIRECTION_POS_FRONT);
			m_pPolygon[DIRECTION_FRONT]->SetSize(DAMAGEDIRECTION_SIZE_FRONT_BACK.x, DAMAGEDIRECTION_SIZE_FRONT_BACK.y);

			// �\�����Ԃ̐ݒ�
			m_nCntLife[DIRECTION_FRONT] = 90;
		}

		// ���������t���O�����Ă�
		m_bHitDirection[DIRECTION_FRONT] = true;
	}
	else if(fAngle <= D3DX_PI * 0.25f && fAngle >= D3DX_PI * -0.25f)
	{// ��둤
		if (NULL == m_pPolygon[DIRECTION_BACK])
		{
			m_pPolygon[DIRECTION_BACK] = CScene2D::Create();
			m_pPolygon[DIRECTION_BACK]->BindTexture(CTexture::GetTexture(CTexture::TEXTURE_HIT_UPPER));
			m_pPolygon[DIRECTION_BACK]->SetTex(0, 1, 2);
			m_pPolygon[DIRECTION_BACK]->SetPos(DAMAGEDIRECTION_POS_BACK);
			m_pPolygon[DIRECTION_BACK]->SetSize(DAMAGEDIRECTION_SIZE_FRONT_BACK.x, DAMAGEDIRECTION_SIZE_FRONT_BACK.y);

			// �\�����Ԃ̐ݒ�
			m_nCntLife[DIRECTION_BACK] = 90;
		}

		// ���������t���O�����Ă�
		m_bHitDirection[DIRECTION_BACK] = true;
	}

	if (fAngle >= D3DX_PI * 0.25f && fAngle <= D3DX_PI * 0.75f)
	{// ���E
		if ((dirSelf.z * dirObj.x) - (dirSelf.x * dirObj.z) <= 0)
		{// �E��
			if (NULL == m_pPolygon[DIRECTION_RIGHT])
			{
				m_pPolygon[DIRECTION_RIGHT] = CScene2D::Create();
				m_pPolygon[DIRECTION_RIGHT]->BindTexture(CTexture::GetTexture(CTexture::TEXTURE_HIT_SIDE));
				m_pPolygon[DIRECTION_RIGHT]->SetTex(1, 1, 2);
				m_pPolygon[DIRECTION_RIGHT]->SetPos(DAMAGEDIRECTION_POS_RIGHT);
				m_pPolygon[DIRECTION_RIGHT]->SetSize(DAMAGEDIRECTION_SIZE_RIGHT_LEFT.x, DAMAGEDIRECTION_SIZE_RIGHT_LEFT.y);

				// �\�����Ԃ̐ݒ�
				m_nCntLife[DIRECTION_RIGHT] = 90;
			}

			// ���������t���O�����Ă�
			m_bHitDirection[DIRECTION_RIGHT] = true;
		}
		else
		{// ����
			if (NULL == m_pPolygon[DIRECTION_LEFT])
			{
				m_pPolygon[DIRECTION_LEFT] = CScene2D::Create();
				m_pPolygon[DIRECTION_LEFT]->BindTexture(CTexture::GetTexture(CTexture::TEXTURE_HIT_SIDE));
				m_pPolygon[DIRECTION_LEFT]->SetTex(0, 1, 2);
				m_pPolygon[DIRECTION_LEFT]->SetPos(DAMAGEDIRECTION_POS_LEFT);
				m_pPolygon[DIRECTION_LEFT]->SetSize(DAMAGEDIRECTION_SIZE_RIGHT_LEFT.x, DAMAGEDIRECTION_SIZE_RIGHT_LEFT.y);

				// �\�����Ԃ̐ݒ�
				m_nCntLife[DIRECTION_LEFT] = 90;
			}

			// ���������t���O�����Ă�
			m_bHitDirection[DIRECTION_LEFT] = true;
		}
	}
}
