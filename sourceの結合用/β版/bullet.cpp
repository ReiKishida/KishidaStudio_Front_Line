//=============================================================================
//
// �e���� [bullet.cpp]
// Author : Takuto Ishida
//
//=============================================================================
#include "bullet.h"
#include "manager.h"
#include "renderer.h"
#include "player.h"
#include "game.h"
#include "texture.h"
#include "camera.h"
#include "server.h"
#include "collisionSet.h"
#include "particle.h"
#include "AI.h"

#include "damageDirection.h"
#include "model.h"

#include "menu.h"

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
CBulletPlayer* CBulletPlayer::Create(D3DXVECTOR3 pos, float fAngle, float fAngleVertical, int nDamage, int nTeam, CScene *pScene)
{
	CBulletPlayer *pBullet = NULL;

	pBullet = new CBulletPlayer;	// ���������m��

	if (NULL != pBullet)
	{// �������m�ې���
		pBullet->m_nTeam = nTeam;
		pBullet->m_pScene = pScene;
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
	CBullet::BindTexture(CTexture::GetTexture(CTexture::TEXTURE_BULLET));

	return S_OK;
}

//=========================================
// �I������
//=========================================
void CBulletPlayer::Uninit(void)
{
	m_pScene = NULL;
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
	CScene *pScene = CScene::GetSceneTop(PLAYER_PRIORITY);
	CScene *pSceneNext = NULL;
	while (pScene != NULL)
	{// NULL�ɂȂ�܂Ń��[�v
		pSceneNext = pScene->GetSceneNext();
		CScene::OBJTYPE objType = pScene->GetObjType();

		if (CScene::OBJTYPE_PLAYER == objType)
		{// �v���C���[
			CPlayer *pPlayer = (CPlayer*)pScene;
			int nTeam = pPlayer->GetTeam();
			if (m_nTeam != nTeam)
			{
				if (CScene3DBill::Collision(pPlayer->GetPos(), 50.0f) && pPlayer->GetDeath() ==false)
				{// �ڐG���Ă���
					if (CMenu::GetMode() == CMenu::MODE_SINGLE)
					{//�V���O���v���C�̏ꍇ
						if (pPlayer->GetPlayerIdx() == 0)
						{//�v���C���[�ԍ����O�̏ꍇ
							float fAngle = CManager::GetCamera()->GetRot().y;
							D3DXVECTOR3 dir = D3DXVECTOR3(sinf(fAngle), 0.0f, cosf(fAngle));
							CManager::GetGame()->GetDamageDirection()->CreatePolygon(dir, CBullet::GetMove());
						}
					}
					else if (CMenu::GetMode() == CMenu::MODE_MULTI)
					{//�}���`�v���C�̏ꍇ
						if (CManager::GetClient()->GetPlayerIdx() == pPlayer->GetPlayerIdx() && pPlayer->GetDeath() == false)
						{//�v���C���[�ԍ�����v����ꍇ�@
							float fAngle = CManager::GetCamera()->GetRot().y;
							D3DXVECTOR3 dir = D3DXVECTOR3(sinf(fAngle), 0.0f, cosf(fAngle));
							CManager::GetGame()->GetDamageDirection()->CreatePolygon(dir, CBullet::GetMove());
						}
					}

					pPlayer->Damage(CBullet::GetDamage(), m_pScene);
					//pPlayer->Damage(CBullet::GetDamage(), ->GetPlayerIdx());

					CParticle::Create(CBullet::GetPos(), 1);

					Uninit();
					return true;		// �ڐG�����̂�true��Ԃ�
				}
			}
		}
		else if (CScene::OBJTYPE_AI == objType)
		{// AI�@��
			CAIMecha *pAI = (CAIMecha*)pScene;
			int nTeam = pAI->GetTeam();
			if (m_nTeam != nTeam)
			{
				if (CScene3DBill::Collision(pAI->GetPos(), 50.0f) && pAI->GetDeath() == false)
				{// �ڐG���Ă���
					pAI->Damage(CBullet::GetDamage(), m_pScene);
					//pAI->Damage(CBullet::GetDamage(),m_pPlayer->GetPlayerIdx());
					CParticle::Create(CBullet::GetPos(), 1);
					Uninit();
					return true;		// �ڐG�����̂�true��Ԃ�
				}
			}
		}

		// ���̃I�u�W�F�N�g������
		pScene = pSceneNext;
	}

	// �}�b�v�̓����蔻��
	D3DXVECTOR3 lengthMax = D3DXVECTOR3(CScene3DBill::GetWidth(), CScene3DBill::GetWidth(), CScene3DBill::GetWidth()) * 2.0f;
	D3DXVECTOR3 lengthMin = D3DXVECTOR3(-CScene3DBill::GetWidth(), -CScene3DBill::GetWidth(), -CScene3DBill::GetWidth());

	if (CCollision::Collision(&GetPos(), GetPosOld(), lengthMax, lengthMin))
	{// �Փ˂��Ă���
		CParticle::Create(CBullet::GetPos(), 1);
		Uninit();
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
		pPlayer->Damage(GetDamage(), this);	// �_���[�W��^����
		return true;					// �ڐG�����̂�true��Ԃ�
	}

	return false;	// �ڐG���ĂȂ��̂�false��Ԃ�
}

/************************************************************/
/*						�s�����ėp�̒e						*/
/************************************************************/
//==================================
// ��������
//==================================
CBulletPin* CBulletPin::Create(D3DXVECTOR3 pos, float fAngle, float fAngleVertical, int nTeam, CScene *pScene)
{
	CBulletPin *pBullet = NULL;

	pBullet = new CBulletPin;	// ���������m��

	if (NULL != pBullet)
	{// �������m�ې���
		pBullet->m_pScene = pScene;
		pBullet->m_nTeam = nTeam;
		pBullet->Init(pos, fAngle, fAngleVertical);
	}

	return pBullet;
}

//=========================================
// �R���X�g���N�^
//=========================================
CBulletPin::CBulletPin(){}

//=========================================
// �f�X�g���N�^
//=========================================
CBulletPin::~CBulletPin(){}

//=========================================
// ����������
//=========================================
HRESULT CBulletPin::Init(D3DXVECTOR3 pos, float fAngle, float fAngleVertical)
{
	float fSpeed = 15.0f;	// �ړ����x
	float fSize = 3.0f;		// �傫��
	int nLife = 1000;		// �̗�

	CBullet::Init(pos);										// �ʒu�̐ݒ�
	CBullet::SetLighting(false);

	CBullet::SetMove(D3DXVECTOR3(sinf(fAngle) * fSpeed, cosf(fAngleVertical) * fSpeed, cosf(fAngle) * fSpeed));
	CBullet::SetLife(nLife);
	CBullet::SetSize(D3DXVECTOR3(fSize, fSize, 0.0f));		// �傫���̐ݒ�
	CBullet::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));	// �F�̐ݒ�
	CBullet::BindTexture(CTexture::GetTexture(CTexture::TEXTURE_BULLET));

	return S_OK;
}

//=========================================
// �I������
//=========================================
void CBulletPin::Uninit(void)
{
	m_pScene = NULL;
	CScene3DBill::Uninit();
}

//=========================================
// �X�V����
//=========================================
void CBulletPin::Update(void)
{
	// �e�̍X�V����
	CBullet::Update();
}

//=========================================
// �`�揈��
//=========================================
void CBulletPin::Draw(void)
{
	CScene3DBill::Draw();
}

//=========================================
// �e�̔���
//=========================================
bool CBulletPin::BulletCollision(void)
{
	// �G��T��
	CScene *pScene = CScene::GetSceneTop(PLAYER_PRIORITY);
	CScene *pSceneNext = NULL;
	while (pScene != NULL)
	{// NULL�ɂȂ�܂Ń��[�v
		pSceneNext = pScene->GetSceneNext();
		CScene::OBJTYPE objType = pScene->GetObjType();

		if (objType == CScene::OBJTYPE_PLAYER)
		{// �v���C���[
			CPlayer *pPlayer = (CPlayer*)pScene;
			int nTeam = pPlayer->GetTeam();
			if (m_nTeam != nTeam)
			{// �����`�[������Ȃ��Ƃ�
				if (CScene3DBill::Collision(pPlayer->GetPos(), 50.0f) && pPlayer->GetDeath() == false)
				{// �ڐG���Ă���
					m_HitPos = pPlayer->GetPos(); // �ڐG�n�_��o�^
					return true;		// �ڐG�����̂�true��Ԃ�
				}
			}
		}
		else if (objType == CScene::OBJTYPE_AI)
		{// AI�@��
			CAIMecha *pAI = (CAIMecha*)pScene;
			int nTeam = pAI->GetTeam();
			if (m_nTeam != nTeam)
			{// �����`�[������Ȃ��Ƃ�
				if (CScene3DBill::Collision(pAI->GetPos(), 50.0f) && pAI->GetDeath() == false)
				{// �ڐG���Ă���
					m_HitPos = pAI->GetPos(); // �ڐG�n�_��o�^
					return true;		// �ڐG�����̂�true��Ԃ�
				}
			}
		}

		// ���̃I�u�W�F�N�g������
		pScene = pSceneNext;
	}

	// �}�b�v�̓����蔻��
	D3DXVECTOR3 lengthMax = D3DXVECTOR3(CScene3DBill::GetWidth(), CScene3DBill::GetWidth(), CScene3DBill::GetWidth()) * 2.0f;
	D3DXVECTOR3 lengthMin = D3DXVECTOR3(-CScene3DBill::GetWidth(), -CScene3DBill::GetWidth(), -CScene3DBill::GetWidth());

	if (CCollision::Collision(&GetPos(), GetPosOld(), lengthMax, lengthMin) || CBullet::GetPos().y <= 0.0f)
	{// �������n�ʂɏՓ˂��Ă���
		m_HitPos = CBullet::GetPos(); // �ڐG�n�_��o�^
		return true;	// �ڐG���Ă���̂�true��Ԃ�
	}

	return false;	// �ڐG���ĂȂ��̂�false��Ԃ�
}
