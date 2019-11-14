//=============================================================================
//
// �e���� [bullet.cpp]
// Author : Takuto Ishida
//
//=============================================================================
#include "bullet.h"
#include "manager.h"
#include "renderer.h"
#include "enemy.h"
#include "player.h"
#include "game.h"
#include "texture.h"
#include "camera.h"
#include "server.h"

//==================================
// �}�N����`
//==================================
#define BULLET_SPEED	(15.0f)	// �ړ��̑��x
#define BULLET_LIFE		(80)	// ����

//==================================
// �ÓI�����o�ϐ��錾
//==================================

//=========================================
// �R���X�g���N�^
//=========================================
CBullet::CBullet(int nPriority, CScene::OBJTYPE objType) : CScene3DBill(nPriority, objType)
{
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nLife = 0;
}

//=========================================
// �f�X�g���N�^
//=========================================
CBullet::~CBullet()
{
}

//=========================================
// ����������
//=========================================
HRESULT CBullet::Init(D3DXVECTOR3 pos)
{
	CScene3DBill::Init();

	// �e�N�X�`���̐ݒ�
	//BindTexture(CTexture::GetTexture(CTexture::TEXTURE_BULLET));

	// �|���S���̈ʒu��ݒ�
	CScene3DBill::SetPos(pos);

	return S_OK;
}

//=========================================
// �X�V����
//=========================================
void CBullet::Update(void)
{
	m_nLife--;		// �����̃J�E���g

	D3DXVECTOR3 pos = CScene3DBill::GetPos();	// �ʒu�̎擾
	pos += m_move;

	if (m_nLife <= 0)
	{// �����ŏ���
		Uninit();
	}
	else
	{// �ʒu�̍X�V
		CScene3DBill::SetPos(pos);
	}
}

/************************************************************/
/*						�����蔻��							*/
/************************************************************/
//==================================
// ��������
//==================================
CBulletCollision *CBulletCollision::Create(void)
{
	CBulletCollision *pBulletCollision = NULL;

	pBulletCollision = new CBulletCollision;	// ���������m��

	if (NULL != pBulletCollision)
	{// �������m�ې���
		pBulletCollision->Init();
	}

	return pBulletCollision;
}

//=========================================
// �R���X�g���N�^
//=========================================
CBulletCollision::CBulletCollision(int nPriority, CScene::OBJTYPE objType) : CScene(nPriority, objType)
{
}

//=========================================
// �f�X�g���N�^
//=========================================
CBulletCollision::~CBulletCollision()
{
}

//=========================================
// ����������
//=========================================
HRESULT CBulletCollision::Init(void)
{
	return S_OK;
}

//=========================================
// �I������
//=========================================
void CBulletCollision::Uninit(void)
{
	// �����̔j��
	CScene::Release();
}

//=========================================
// �X�V����
//=========================================
void CBulletCollision::Update(void)
{
	// �e��T��
	CScene *pSceneBullet = CScene::GetSceneTop(BULLET_PRIORITY);
	CScene *pSceneNextBullet = NULL;
	while (pSceneBullet != NULL)
	{// NULL�ɂȂ�܂Ń��[�v
		pSceneNextBullet = pSceneBullet->GetSceneNext();
		CScene::OBJTYPE objTypeBullet = pSceneBullet->GetObjType();

		if (objTypeBullet == CScene::OBJTYPE_BULLET)
		{// �e�������Ƃ�
			CBullet* pBullet = (CBullet*)pSceneBullet;

			// �e�̓����蔻��
			pBullet->BulletCollision();
		}

		// ���̃I�u�W�F�N�g������
		pSceneBullet = pSceneNextBullet;
	}
}

//=========================================
// �`�揈��
//=========================================
void CBulletCollision::Draw(void)
{
}

/************************************************************/
/*						�v���C���[�̒e						*/
/************************************************************/
//==================================
// ��������
//==================================
CBulletPlayer* CBulletPlayer::Create(D3DXVECTOR3 pos, float fAngle, float fAngleVertical, int nDamage)
{
	CBulletPlayer *pBullet = NULL;

	pBullet = new CBulletPlayer;	// ���������m��

	if (NULL != pBullet)
	{// �������m�ې���
		pBullet->Init(pos, fAngle, fAngleVertical, nDamage);
	}

	return pBullet;
}

//=========================================
// �R���X�g���N�^
//=========================================
CBulletPlayer::CBulletPlayer()
{
}

//=========================================
// �f�X�g���N�^
//=========================================
CBulletPlayer::~CBulletPlayer()
{
}

//=========================================
// ����������
//=========================================
HRESULT CBulletPlayer::Init(D3DXVECTOR3 pos, float fAngle, float fAngleVertical, int nDamage)
{
	CBullet::Init(pos);										// �ʒu�̐ݒ�
	CBullet::SetLighting(false);

	CBullet::SetMove(D3DXVECTOR3(sinf(fAngle) * BULLET_SPEED, cosf(fAngleVertical) * BULLET_SPEED, cosf(fAngle) * BULLET_SPEED));
	CBullet::SetLife(BULLET_LIFE);
	CBullet::SetSize(D3DXVECTOR3(3.0f, 3.0f, 0.0f));							// �傫���̐ݒ�
	CBullet::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));	// �F�̐ݒ�
	CBullet::SetDamage(nDamage);
	//CBullet::BindTexture(CTexture::GetTexture(CTexture::TEXTURE_BULLET));

	return S_OK;
}

//=========================================
// �I������
//=========================================
void CBulletPlayer::Uninit(void)
{
	CScene3DBill::Uninit();
}

//=========================================
// �X�V����
//=========================================
void CBulletPlayer::Update(void)
{
	// �e�̍X�V����
	CBullet::Update();
}

//=========================================
// �`�揈��
//=========================================
void CBulletPlayer::Draw(void)
{
	CScene3DBill::Draw();
}

//=========================================
// �v���C���[�̒e�̔���
//=========================================
bool CBulletPlayer::BulletCollision(void)
{
	// �G��T��
	CScene *pSceneEnemy = CScene::GetSceneTop(ENEMY_PRIORITY);
	CScene *pSceneNextEnemy = NULL;
	while (pSceneEnemy != NULL)
	{// NULL�ɂȂ�܂Ń��[�v
		pSceneNextEnemy = pSceneEnemy->GetSceneNext();
		CScene::OBJTYPE objTypeEnemy = pSceneEnemy->GetObjType();

		if (objTypeEnemy == CScene::OBJTYPE_ENEMY)
		{// �G�������Ƃ�
			CEnemy *pEnemy = (CEnemy*)pSceneEnemy;	// �L���X�g���ēG�N���X�̕ϐ��Ƃ��Ďg��

			if (CScene3DBill::Collision(pEnemy->GetPos(), 50.0f))
			{// �ڐG���Ă���
				pEnemy->Uninit();
				CGame::SetGameState(CGame::STATE_END);
				Uninit();
				return true;		// �ڐG�����̂�true��Ԃ�
			}
		}

		// ���̃I�u�W�F�N�g������
		pSceneEnemy = pSceneNextEnemy;
	}

	return false;	// �ڐG���ĂȂ��̂�false��Ԃ�
}

/************************************************************/
/*							�G�̒e							*/
/************************************************************/
//==================================
// ��������
//==================================
CBulletEnemy* CBulletEnemy::Create(D3DXVECTOR3 pos, TYPE type, float fDestAngle)
{
	CBulletEnemy *pBullet = NULL;

	pBullet = new CBulletEnemy;	// ���������m��

	if (NULL != pBullet)
	{// �������m�ې���
		pBullet->m_type = type;
		pBullet->m_fAngle = fDestAngle;
		pBullet->Init(pos);
	}

	return pBullet;
}

//=========================================
// �R���X�g���N�^
//=========================================
CBulletEnemy::CBulletEnemy()
{
	m_nCntAngle = 0;
	m_fAngle = 0;
}

//=========================================
// �f�X�g���N�^
//=========================================
CBulletEnemy::~CBulletEnemy()
{
}

//=========================================
// ����������
//=========================================
HRESULT CBulletEnemy::Init(D3DXVECTOR3 pos)
{
	CBullet::Init(pos);

	switch (m_type)
	{
	case TYPE_NORMAL:
		CBullet::SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
		CBullet::SetLife(BULLET_LIFE);
		CBullet::SetSize(D3DXVECTOR3(5.0f, 5.0f, 5.0f));			// �傫���̐ݒ�
		CBullet::SetDamage(1);
		break;

	default:
		break;
	}

	CBullet::SetMove(D3DXVECTOR3(sinf(m_fAngle) * BULLET_SPEED, cosf(m_fAngle) * BULLET_SPEED, 0.0f));
	return S_OK;
}

//=========================================
// �I������
//=========================================
void CBulletEnemy::Uninit(void)
{
	D3DXVECTOR3 pos = CScene3DBill::GetPos();	// �ʒu�̎擾

	CScene3DBill::Uninit();
}

//=========================================
// �X�V����
//=========================================
void CBulletEnemy::Update(void)
{
	// �e�̍X�V����
	CBullet::Update();
}

//=========================================
// �`�揈��
//=========================================
void CBulletEnemy::Draw(void)
{
	CScene3DBill::Draw();
}

//=========================================
// �G�̒e�̔���
//=========================================
bool CBulletEnemy::BulletCollision(void)
{
	// �v���C���[��T��
	CPlayer *pPlayer = CGame::GetPlayer(CManager::GetClient()->GetClientIdx());	// �L���X�g���ēG�N���X�̕ϐ��Ƃ��Ďg��

	if (Collision(pPlayer->GetPos(), pPlayer->GetVtxMax().x))
	{// �ڐG���Ă���
		pPlayer->Damage(GetDamage());	// �_���[�W��^����
		return true;					// �ڐG�����̂�true��Ԃ�
	}

	return false;	// �ڐG���ĂȂ��̂�false��Ԃ�
}
