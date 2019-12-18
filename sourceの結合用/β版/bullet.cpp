//=============================================================================
//
// 弾処理 [bullet.cpp]
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
// マクロ定義
//==================================
#define BULLET_SPEED	(15.0f)	// 移動の速度
#define BULLET_LIFE		(80)	// 寿命

//==================================
// 静的メンバ変数宣言
//==================================

//=========================================
// コンストラクタ
//=========================================
CBullet::CBullet(int nPriority, CScene::OBJTYPE objType) : CScene3DBill(nPriority, objType)
{
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nLife = 0;
}

//=========================================
// デストラクタ
//=========================================
CBullet::~CBullet()
{
}

//=========================================
// 初期化処理
//=========================================
HRESULT CBullet::Init(D3DXVECTOR3 pos)
{
	CScene3DBill::Init();

	// ポリゴンの位置を設定
	CScene3DBill::SetPos(pos);

	return S_OK;
}

//=========================================
// 更新処理
//=========================================
void CBullet::Update(void)
{
	m_nLife--;		// 寿命のカウント

	D3DXVECTOR3 pos = CScene3DBill::GetPos();	// 位置の取得
	pos += m_move;

	if (m_nLife <= 0)
	{// 寿命で消す
		Uninit();
	}
	else
	{// 位置の更新
		CScene3DBill::SetPos(pos);
	}
}

/************************************************************/
/*						当たり判定							*/
/************************************************************/
//==================================
// 生成処理
//==================================
CBulletCollision *CBulletCollision::Create(void)
{
	CBulletCollision *pBulletCollision = NULL;

	pBulletCollision = new CBulletCollision;	// メモリを確保

	if (NULL != pBulletCollision)
	{// メモリ確保成功
		pBulletCollision->Init();
	}

	return pBulletCollision;
}

//=========================================
// コンストラクタ
//=========================================
CBulletCollision::CBulletCollision(int nPriority, CScene::OBJTYPE objType) : CScene(nPriority, objType)
{
}

//=========================================
// デストラクタ
//=========================================
CBulletCollision::~CBulletCollision()
{
}

//=========================================
// 初期化処理
//=========================================
HRESULT CBulletCollision::Init(void)
{
	return S_OK;
}

//=========================================
// 終了処理
//=========================================
void CBulletCollision::Uninit(void)
{
	// 自分の破棄
	CScene::Release();
}

//=========================================
// 更新処理
//=========================================
void CBulletCollision::Update(void)
{
	// 弾を探す
	CScene *pSceneBullet = CScene::GetSceneTop(BULLET_PRIORITY);
	CScene *pSceneNextBullet = NULL;
	while (pSceneBullet != NULL)
	{// NULLになるまでループ
		pSceneNextBullet = pSceneBullet->GetSceneNext();
		CScene::OBJTYPE objTypeBullet = pSceneBullet->GetObjType();

		if (objTypeBullet == CScene::OBJTYPE_BULLET)
		{// 弾だったとき
			CBullet* pBullet = (CBullet*)pSceneBullet;

			// 弾の当たり判定
			pBullet->BulletCollision();
		}

		// 次のオブジェクトを見る
		pSceneBullet = pSceneNextBullet;
	}
}

//=========================================
// 描画処理
//=========================================
void CBulletCollision::Draw(void)
{
}

/************************************************************/
/*						プレイヤーの弾						*/
/************************************************************/
//==================================
// 生成処理
//==================================
CBulletPlayer* CBulletPlayer::Create(D3DXVECTOR3 pos, float fAngle, float fAngleVertical, int nDamage, int nTeam, CScene *pScene)
{
	CBulletPlayer *pBullet = NULL;

	pBullet = new CBulletPlayer;	// メモリを確保

	if (NULL != pBullet)
	{// メモリ確保成功
		pBullet->m_nTeam = nTeam;
		pBullet->m_pScene = pScene;
		pBullet->Init(pos, fAngle, fAngleVertical, nDamage);
	}

	return pBullet;
}

//=========================================
// コンストラクタ
//=========================================
CBulletPlayer::CBulletPlayer()
{
}

//=========================================
// デストラクタ
//=========================================
CBulletPlayer::~CBulletPlayer()
{
}

//=========================================
// 初期化処理
//=========================================
HRESULT CBulletPlayer::Init(D3DXVECTOR3 pos, float fAngle, float fAngleVertical, int nDamage)
{
	CBullet::Init(pos);										// 位置の設定
	CBullet::SetLighting(false);

	CBullet::SetMove(D3DXVECTOR3(sinf(fAngle) * BULLET_SPEED, cosf(fAngleVertical) * BULLET_SPEED, cosf(fAngle) * BULLET_SPEED));
	CBullet::SetLife(BULLET_LIFE);
	CBullet::SetSize(D3DXVECTOR3(3.0f, 3.0f, 0.0f));							// 大きさの設定
	CBullet::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));	// 色の設定
	CBullet::SetDamage(nDamage);
	CBullet::BindTexture(CTexture::GetTexture(CTexture::TEXTURE_BULLET));

	return S_OK;
}

//=========================================
// 終了処理
//=========================================
void CBulletPlayer::Uninit(void)
{
	m_pScene = NULL;
	CScene3DBill::Uninit();
}

//=========================================
// 更新処理
//=========================================
void CBulletPlayer::Update(void)
{
	// 弾の更新処理
	CBullet::Update();
}

//=========================================
// 描画処理
//=========================================
void CBulletPlayer::Draw(void)
{
	CScene3DBill::Draw();
}

//=========================================
// プレイヤーの弾の判定
//=========================================
bool CBulletPlayer::BulletCollision(void)
{
	// 敵を探す
	CScene *pScene = CScene::GetSceneTop(PLAYER_PRIORITY);
	CScene *pSceneNext = NULL;
	while (pScene != NULL)
	{// NULLになるまでループ
		pSceneNext = pScene->GetSceneNext();
		CScene::OBJTYPE objType = pScene->GetObjType();

		if (CScene::OBJTYPE_PLAYER == objType)
		{// プレイヤー
			CPlayer *pPlayer = (CPlayer*)pScene;
			int nTeam = pPlayer->GetTeam();
			if (m_nTeam != nTeam)
			{
				if (CScene3DBill::Collision(pPlayer->GetPos(), 50.0f) && pPlayer->GetDeath() ==false)
				{// 接触している
					if (CMenu::GetMode() == CMenu::MODE_SINGLE)
					{//シングルプレイの場合
						if (pPlayer->GetPlayerIdx() == 0)
						{//プレイヤー番号が０の場合
							float fAngle = CManager::GetCamera()->GetRot().y;
							D3DXVECTOR3 dir = D3DXVECTOR3(sinf(fAngle), 0.0f, cosf(fAngle));
							CManager::GetGame()->GetDamageDirection()->CreatePolygon(dir, CBullet::GetMove());
						}
					}
					else if (CMenu::GetMode() == CMenu::MODE_MULTI)
					{//マルチプレイの場合
						if (CManager::GetClient()->GetPlayerIdx() == pPlayer->GetPlayerIdx() && pPlayer->GetDeath() == false)
						{//プレイヤー番号が一致する場合　
							float fAngle = CManager::GetCamera()->GetRot().y;
							D3DXVECTOR3 dir = D3DXVECTOR3(sinf(fAngle), 0.0f, cosf(fAngle));
							CManager::GetGame()->GetDamageDirection()->CreatePolygon(dir, CBullet::GetMove());
						}
					}

					pPlayer->Damage(CBullet::GetDamage(), m_pScene);
					//pPlayer->Damage(CBullet::GetDamage(), ->GetPlayerIdx());

					CParticle::Create(CBullet::GetPos(), 1);

					Uninit();
					return true;		// 接触したのでtrueを返す
				}
			}
		}
		else if (CScene::OBJTYPE_AI == objType)
		{// AI機体
			CAIMecha *pAI = (CAIMecha*)pScene;
			int nTeam = pAI->GetTeam();
			if (m_nTeam != nTeam)
			{
				if (CScene3DBill::Collision(pAI->GetPos(), 50.0f) && pAI->GetDeath() == false)
				{// 接触している
					pAI->Damage(CBullet::GetDamage(), m_pScene);
					//pAI->Damage(CBullet::GetDamage(),m_pPlayer->GetPlayerIdx());
					CParticle::Create(CBullet::GetPos(), 1);
					Uninit();
					return true;		// 接触したのでtrueを返す
				}
			}
		}

		// 次のオブジェクトを見る
		pScene = pSceneNext;
	}

	// マップの当たり判定
	D3DXVECTOR3 lengthMax = D3DXVECTOR3(CScene3DBill::GetWidth(), CScene3DBill::GetWidth(), CScene3DBill::GetWidth()) * 2.0f;
	D3DXVECTOR3 lengthMin = D3DXVECTOR3(-CScene3DBill::GetWidth(), -CScene3DBill::GetWidth(), -CScene3DBill::GetWidth());

	if (CCollision::Collision(&GetPos(), GetPosOld(), lengthMax, lengthMin))
	{// 衝突している
		CParticle::Create(CBullet::GetPos(), 1);
		Uninit();
	}

	return false;	// 接触してないのでfalseを返す
}

/************************************************************/
/*							敵の弾							*/
/************************************************************/
//==================================
// 生成処理
//==================================
CBulletEnemy* CBulletEnemy::Create(D3DXVECTOR3 pos, TYPE type, float fDestAngle)
{
	CBulletEnemy *pBullet = NULL;

	pBullet = new CBulletEnemy;	// メモリを確保

	if (NULL != pBullet)
	{// メモリ確保成功
		pBullet->m_type = type;
		pBullet->m_fAngle = fDestAngle;
		pBullet->Init(pos);
	}

	return pBullet;
}

//=========================================
// コンストラクタ
//=========================================
CBulletEnemy::CBulletEnemy()
{
	m_nCntAngle = 0;
	m_fAngle = 0;
}

//=========================================
// デストラクタ
//=========================================
CBulletEnemy::~CBulletEnemy()
{
}

//=========================================
// 初期化処理
//=========================================
HRESULT CBulletEnemy::Init(D3DXVECTOR3 pos)
{
	CBullet::Init(pos);

	switch (m_type)
	{
	case TYPE_NORMAL:
		CBullet::SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
		CBullet::SetLife(BULLET_LIFE);
		CBullet::SetSize(D3DXVECTOR3(5.0f, 5.0f, 5.0f));			// 大きさの設定
		CBullet::SetDamage(1);
		break;

	default:
		break;
	}

	CBullet::SetMove(D3DXVECTOR3(sinf(m_fAngle) * BULLET_SPEED, cosf(m_fAngle) * BULLET_SPEED, 0.0f));
	return S_OK;
}

//=========================================
// 終了処理
//=========================================
void CBulletEnemy::Uninit(void)
{
	D3DXVECTOR3 pos = CScene3DBill::GetPos();	// 位置の取得

	CScene3DBill::Uninit();
}

//=========================================
// 更新処理
//=========================================
void CBulletEnemy::Update(void)
{
	// 弾の更新処理
	CBullet::Update();
}

//=========================================
// 描画処理
//=========================================
void CBulletEnemy::Draw(void)
{
	CScene3DBill::Draw();
}

//=========================================
// 敵の弾の判定
//=========================================
bool CBulletEnemy::BulletCollision(void)
{
	// プレイヤーを探す
	CPlayer *pPlayer = CGame::GetPlayer(CManager::GetClient()->GetClientIdx());	// キャストして敵クラスの変数として使う

	if (Collision(pPlayer->GetPos(), pPlayer->GetVtxMax().x))
	{// 接触している
		pPlayer->Damage(GetDamage(), this);	// ダメージを与える
		return true;					// 接触したのでtrueを返す
	}

	return false;	// 接触してないのでfalseを返す
}

/************************************************************/
/*						ピン立て用の弾						*/
/************************************************************/
//==================================
// 生成処理
//==================================
CBulletPin* CBulletPin::Create(D3DXVECTOR3 pos, float fAngle, float fAngleVertical, int nTeam, CScene *pScene)
{
	CBulletPin *pBullet = NULL;

	pBullet = new CBulletPin;	// メモリを確保

	if (NULL != pBullet)
	{// メモリ確保成功
		pBullet->m_pScene = pScene;
		pBullet->m_nTeam = nTeam;
		pBullet->Init(pos, fAngle, fAngleVertical);
	}

	return pBullet;
}

//=========================================
// コンストラクタ
//=========================================
CBulletPin::CBulletPin(){}

//=========================================
// デストラクタ
//=========================================
CBulletPin::~CBulletPin(){}

//=========================================
// 初期化処理
//=========================================
HRESULT CBulletPin::Init(D3DXVECTOR3 pos, float fAngle, float fAngleVertical)
{
	float fSpeed = 15.0f;	// 移動速度
	float fSize = 3.0f;		// 大きさ
	int nLife = 1000;		// 体力

	CBullet::Init(pos);										// 位置の設定
	CBullet::SetLighting(false);

	CBullet::SetMove(D3DXVECTOR3(sinf(fAngle) * fSpeed, cosf(fAngleVertical) * fSpeed, cosf(fAngle) * fSpeed));
	CBullet::SetLife(nLife);
	CBullet::SetSize(D3DXVECTOR3(fSize, fSize, 0.0f));		// 大きさの設定
	CBullet::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));	// 色の設定
	CBullet::BindTexture(CTexture::GetTexture(CTexture::TEXTURE_BULLET));

	return S_OK;
}

//=========================================
// 終了処理
//=========================================
void CBulletPin::Uninit(void)
{
	m_pScene = NULL;
	CScene3DBill::Uninit();
}

//=========================================
// 更新処理
//=========================================
void CBulletPin::Update(void)
{
	// 弾の更新処理
	CBullet::Update();
}

//=========================================
// 描画処理
//=========================================
void CBulletPin::Draw(void)
{
	CScene3DBill::Draw();
}

//=========================================
// 弾の判定
//=========================================
bool CBulletPin::BulletCollision(void)
{
	// 敵を探す
	CScene *pScene = CScene::GetSceneTop(PLAYER_PRIORITY);
	CScene *pSceneNext = NULL;
	while (pScene != NULL)
	{// NULLになるまでループ
		pSceneNext = pScene->GetSceneNext();
		CScene::OBJTYPE objType = pScene->GetObjType();

		if (objType == CScene::OBJTYPE_PLAYER)
		{// プレイヤー
			CPlayer *pPlayer = (CPlayer*)pScene;
			int nTeam = pPlayer->GetTeam();
			if (m_nTeam != nTeam)
			{// 同じチームじゃないとき
				if (CScene3DBill::Collision(pPlayer->GetPos(), 50.0f) && pPlayer->GetDeath() == false)
				{// 接触している
					m_HitPos = pPlayer->GetPos(); // 接触地点を登録
					return true;		// 接触したのでtrueを返す
				}
			}
		}
		else if (objType == CScene::OBJTYPE_AI)
		{// AI機体
			CAIMecha *pAI = (CAIMecha*)pScene;
			int nTeam = pAI->GetTeam();
			if (m_nTeam != nTeam)
			{// 同じチームじゃないとき
				if (CScene3DBill::Collision(pAI->GetPos(), 50.0f) && pAI->GetDeath() == false)
				{// 接触している
					m_HitPos = pAI->GetPos(); // 接触地点を登録
					return true;		// 接触したのでtrueを返す
				}
			}
		}

		// 次のオブジェクトを見る
		pScene = pSceneNext;
	}

	// マップの当たり判定
	D3DXVECTOR3 lengthMax = D3DXVECTOR3(CScene3DBill::GetWidth(), CScene3DBill::GetWidth(), CScene3DBill::GetWidth()) * 2.0f;
	D3DXVECTOR3 lengthMin = D3DXVECTOR3(-CScene3DBill::GetWidth(), -CScene3DBill::GetWidth(), -CScene3DBill::GetWidth());

	if (CCollision::Collision(&GetPos(), GetPosOld(), lengthMax, lengthMin) || CBullet::GetPos().y <= 0.0f)
	{// 建物か地面に衝突している
		m_HitPos = CBullet::GetPos(); // 接触地点を登録
		return true;	// 接触しているのでtrueを返す
	}

	return false;	// 接触してないのでfalseを返す
}
