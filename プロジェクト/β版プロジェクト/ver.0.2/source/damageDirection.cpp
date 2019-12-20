//=============================================================================
//
// 攻撃を受けた方向表示処理 [damageDirection.h]
// Author : Takuto Ishida
//
//=============================================================================
#include "damageDirection.h"
#include "scene2D.h"
#include "texture.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define DAMAGEDIRECTION_UPPER_WIDTH		(550.0f)			// 上下の幅
#define DAMAGEDIRECTION_UPPER_HEIGHT	(100.0f)			// 上下の高さ
#define DAMAGEDIRECTION_SIDE_WIDTH		(200.0f)			// 左右の幅
#define DAMAGEDIRECTION_SIDE_HEIGHT		(700.0f)			// 左右の高さ

#define DAMAGEDIRECTION_SIZE_FRONT_BACK	(D3DXVECTOR2(DAMAGEDIRECTION_UPPER_WIDTH, DAMAGEDIRECTION_UPPER_HEIGHT))	// 前方向の表示位置
#define DAMAGEDIRECTION_SIZE_RIGHT_LEFT	(D3DXVECTOR2(DAMAGEDIRECTION_SIDE_WIDTH, DAMAGEDIRECTION_SIDE_HEIGHT))		// 右方向の表示位置

#define DAMAGEDIRECTION_POS_FRONT	(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, 125.0f, 0.0f))					// 前方向の表示位置
#define DAMAGEDIRECTION_POS_BACK	(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT - (DAMAGEDIRECTION_SIZE_FRONT_BACK.y * 0.5f), 0.0f))	// 後方向の表示位置
#define DAMAGEDIRECTION_POS_RIGHT	(D3DXVECTOR3(SCREEN_WIDTH - (DAMAGEDIRECTION_SIZE_RIGHT_LEFT.x * 0.75f), SCREEN_HEIGHT / 2.0f, 0.0f))	// 右方向の表示位置
#define DAMAGEDIRECTION_POS_LEFT	(D3DXVECTOR3(DAMAGEDIRECTION_SIZE_RIGHT_LEFT.x * 0.75f, SCREEN_HEIGHT / 2.0f, 0.0f))					// 左方向の表示位置

//==================================
// 静的メンバ変数宣言
//==================================

/****************************************************************/
/*						2Dボタンのクラス						*/
/****************************************************************/
//==================================
// 生成処理
//==================================
CDamageDirection* CDamageDirection::Create(void)
{
	CDamageDirection *pDamageDirection;

	pDamageDirection = new CDamageDirection;	// メモリを確保

	if (NULL != pDamageDirection)
	{// メモリ確保成功
		pDamageDirection->Init();	// 初期化処理
	}

	return pDamageDirection;
}

//=========================================
// コンストラクタ
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
// デストラクタ
//=========================================
CDamageDirection::~CDamageDirection()
{
}

//=========================================
// 初期化処理
//=========================================
HRESULT CDamageDirection::Init(void)
{
	return S_OK;
}

//=========================================
// 終了処理
//=========================================
void CDamageDirection::Uninit(void)
{
	for (int nCntDirection = 0; nCntDirection < DIRECTION_MAX; nCntDirection++)
	{
		if (NULL != m_pPolygon[nCntDirection])
		{// ポリゴンの破棄
			m_pPolygon[nCntDirection]->Uninit();
			m_pPolygon[nCntDirection] = NULL;
		}
	}

	// 自分を破棄
	CScene::Release();
}

//=========================================
// 更新処理
//=========================================
void CDamageDirection::Update(void)
{
	for (int nCntDirection = 0; nCntDirection < DIRECTION_MAX; nCntDirection++)
	{
		if (0 < m_nCntLife[nCntDirection])
		{// 生成されているポリゴンの表示時間をカウント
			m_nCntLife[nCntDirection]--;

			if (NULL != m_pPolygon[nCntDirection] && 0 >= m_nCntLife[nCntDirection])
			{// 表示時間を過ぎたポリゴンの破棄
				m_pPolygon[nCntDirection]->Uninit();
				m_pPolygon[nCntDirection] = NULL;
				m_nCntLife[nCntDirection] = 0;
			}
		}

		// 当たったフラグを戻す
		m_bHitDirection[nCntDirection] = false;
	}
}

//=========================================
// 描画処理
//=========================================
void CDamageDirection::Draw(void)
{
}

//=========================================
// ポリゴン生成処理
//=========================================
void CDamageDirection::CreatePolygon(D3DXVECTOR3 dirSelf, D3DXVECTOR3 dirObj)
{
	// ベクトルのなす角を求める
	float fAngle = (dirSelf.x * dirObj.x) + (dirSelf.z * dirObj.z);
	fAngle = fAngle / (sqrtf((dirSelf.x * dirSelf.x) + (dirSelf.z * dirSelf.z)) * sqrtf((dirObj.x * dirObj.x) + (dirObj.z * dirObj.z)));
	fAngle = acosf(fAngle);

	// 前後
	if (fAngle >= D3DX_PI * 0.75f || fAngle <= D3DX_PI * -0.75f)
	{// 前側
		if (NULL == m_pPolygon[DIRECTION_FRONT])
		{
			m_pPolygon[DIRECTION_FRONT] = CScene2D::Create();
			m_pPolygon[DIRECTION_FRONT]->BindTexture(CTexture::GetTexture(CTexture::TEXTURE_HIT_UPPER));
			m_pPolygon[DIRECTION_FRONT]->SetTex(1, 1, 2);
			m_pPolygon[DIRECTION_FRONT]->SetPos(DAMAGEDIRECTION_POS_FRONT);
			m_pPolygon[DIRECTION_FRONT]->SetSize(DAMAGEDIRECTION_SIZE_FRONT_BACK.x, DAMAGEDIRECTION_SIZE_FRONT_BACK.y);

			// 表示時間の設定
			m_nCntLife[DIRECTION_FRONT] = 90;
		}

		// 当たったフラグをたてる
		m_bHitDirection[DIRECTION_FRONT] = true;
	}
	else if(fAngle <= D3DX_PI * 0.25f && fAngle >= D3DX_PI * -0.25f)
	{// 後ろ側
		if (NULL == m_pPolygon[DIRECTION_BACK])
		{
			m_pPolygon[DIRECTION_BACK] = CScene2D::Create();
			m_pPolygon[DIRECTION_BACK]->BindTexture(CTexture::GetTexture(CTexture::TEXTURE_HIT_UPPER));
			m_pPolygon[DIRECTION_BACK]->SetTex(0, 1, 2);
			m_pPolygon[DIRECTION_BACK]->SetPos(DAMAGEDIRECTION_POS_BACK);
			m_pPolygon[DIRECTION_BACK]->SetSize(DAMAGEDIRECTION_SIZE_FRONT_BACK.x, DAMAGEDIRECTION_SIZE_FRONT_BACK.y);

			// 表示時間の設定
			m_nCntLife[DIRECTION_BACK] = 90;
		}

		// 当たったフラグをたてる
		m_bHitDirection[DIRECTION_BACK] = true;
	}

	if (fAngle >= D3DX_PI * 0.25f && fAngle <= D3DX_PI * 0.75f)
	{// 左右
		if ((dirSelf.z * dirObj.x) - (dirSelf.x * dirObj.z) <= 0)
		{// 右側
			if (NULL == m_pPolygon[DIRECTION_RIGHT])
			{
				m_pPolygon[DIRECTION_RIGHT] = CScene2D::Create();
				m_pPolygon[DIRECTION_RIGHT]->BindTexture(CTexture::GetTexture(CTexture::TEXTURE_HIT_SIDE));
				m_pPolygon[DIRECTION_RIGHT]->SetTex(1, 1, 2);
				m_pPolygon[DIRECTION_RIGHT]->SetPos(DAMAGEDIRECTION_POS_RIGHT);
				m_pPolygon[DIRECTION_RIGHT]->SetSize(DAMAGEDIRECTION_SIZE_RIGHT_LEFT.x, DAMAGEDIRECTION_SIZE_RIGHT_LEFT.y);

				// 表示時間の設定
				m_nCntLife[DIRECTION_RIGHT] = 90;
			}

			// 当たったフラグをたてる
			m_bHitDirection[DIRECTION_RIGHT] = true;
		}
		else
		{// 左側
			if (NULL == m_pPolygon[DIRECTION_LEFT])
			{
				m_pPolygon[DIRECTION_LEFT] = CScene2D::Create();
				m_pPolygon[DIRECTION_LEFT]->BindTexture(CTexture::GetTexture(CTexture::TEXTURE_HIT_SIDE));
				m_pPolygon[DIRECTION_LEFT]->SetTex(0, 1, 2);
				m_pPolygon[DIRECTION_LEFT]->SetPos(DAMAGEDIRECTION_POS_LEFT);
				m_pPolygon[DIRECTION_LEFT]->SetSize(DAMAGEDIRECTION_SIZE_RIGHT_LEFT.x, DAMAGEDIRECTION_SIZE_RIGHT_LEFT.y);

				// 表示時間の設定
				m_nCntLife[DIRECTION_LEFT] = 90;
			}

			// 当たったフラグをたてる
			m_bHitDirection[DIRECTION_LEFT] = true;
		}
	}
}
