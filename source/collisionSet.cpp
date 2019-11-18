//=============================================================================
//
// メッシュフィールド処理 [meshField.cpp]
// Author : Ishida Takuto
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "collisionSet.h"
#include "manager.h"
#include "renderer.h"
#include "line.h"
#include "scene.h"
#include "debugProc.h"
#include <stdio.h>

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define COLLISION_FILENAME		"data/TEXT/FIELD/collision.txt"

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================

//=============================================================================
// 読み込み処理
//=============================================================================
void CCollision::Load(void)
{
	D3DXVECTOR3 pos, size;

	// モデルオブジェクトを探す
	CScene *pScene = CScene::GetSceneTop(COLLISION_PRIORITY);

	while (pScene != NULL)
	{// NULLになるまでループ
		CScene *pSceneNext = pScene->GetSceneNext();
		CScene::OBJTYPE objType = pScene->GetObjType();

		if (CScene::OBJTYPE_COLLISION == objType)
		{// 当たり判定の場合
			CCollision *pCollision = (CCollision*)pScene;
			pCollision->Uninit();
		}

		// 次のオブジェクトを見る
		pScene = pSceneNext;
	}

	// ファイルを開く
	FILE *pFile = fopen(COLLISION_FILENAME, "r");

	if (NULL != pFile)
	{// ファイルがあった
		char aStr[128];

		while (1)
		{// スクリプトを読み終わるまでループ
			fscanf(pFile, "%s", aStr);

			if (strcmp(aStr, "SCRIPT") == 0)
			{// スクリプト開始
				while (strcmp(aStr, "END_SCRIPT") != 0)
				{// スクリプト終了メッセージまでループ
					fscanf(pFile, "%s", aStr);

					if (strcmp(aStr, "COLLISIONSET") == 0)
					{// モデル数
						while (strcmp(aStr, "END_COLLISIONSET") != 0)
						{// 終了メッセージまでループ
							fscanf(pFile, "%s", aStr);
							if (strcmp(aStr, "POS") == 0)
							{// 位置
								fscanf(pFile, " = %f %f %f", &pos.x, &pos.y, &pos.z);
							}
							else if (strcmp(aStr, "SIZE") == 0)
							{// 大きさ
								fscanf(pFile, " = %f %f %f", &size.x, &size.y, &size.z);
							}
						}

						// 当たり判定の生成
						CCollision::Create(pos, size.x, size.y, size.z);
					}
				}
				break;
			}
		}
		fclose(pFile);	// ファイルを閉じる
	}
	else
	{// ファイルがないとき
		MessageBox(0, "テキストファイルがありません。", "ENEMY/model.txt", MB_YESNO);
	}
}

//==================================
// 生成処理
//==================================
CCollision* CCollision::Create(D3DXVECTOR3 pos, float fWidth, float fHeight, float fDepth)
{
	CCollision *pCollision = NULL;

	pCollision = new CCollision;	// メモリを確保

	if (NULL != pCollision)
	{// メモリ確保成功
		pCollision->m_pos = pos;
		pCollision->m_fWidth = fWidth;
		pCollision->m_fHeight = fHeight;
		pCollision->m_fDepth = fDepth;
		pCollision->Init();
	}

	return pCollision;
}

//=========================================
// コンストラクタ
//=========================================
CCollision::CCollision(int nPriority, CScene::OBJTYPE objType) : CScene(nPriority, objType)
{
	m_fWidth = 0;
	m_fHeight = 0;
	m_fDepth = 0;
	m_pLine = NULL;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//=========================================
// デストラクタ
//=========================================
CCollision::~CCollision()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CCollision::Init(void)
{
	// 辺の数分メモリ確保
	m_pLine = new CLine*[SIDE_MAX];

	/*----------------------------------*/
	/*			ラインの生成			*/
	/*----------------------------------*/
	// 上辺
	m_pLine[SIDE_UPPER_FRONT] = CLine::Create(m_pos + D3DXVECTOR3(0.0f, m_fHeight, m_fDepth), m_pos + D3DXVECTOR3(m_fWidth, m_fHeight, m_fDepth), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	m_pLine[SIDE_UPPER_RIGHT] = CLine::Create(m_pos + D3DXVECTOR3(m_fWidth, m_fHeight, m_fDepth), m_pos + D3DXVECTOR3(m_fWidth, m_fHeight, 0.0f), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	m_pLine[SIDE_UPPER_BEHIND] = CLine::Create(m_pos + D3DXVECTOR3(m_fWidth, m_fHeight, 0.0f), m_pos + D3DXVECTOR3(0.0f, m_fHeight, 0.0f), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	m_pLine[SIDE_UPPER_LEFT] = CLine::Create(m_pos + D3DXVECTOR3(0.0f, m_fHeight, 0.0f), m_pos + D3DXVECTOR3(0.0f, m_fHeight, m_fDepth), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));

	// 中辺
	m_pLine[SIDE_MIDDLE_LEFT_FRONT] = CLine::Create(m_pos + D3DXVECTOR3(0.0f, m_fHeight, m_fDepth), m_pos + D3DXVECTOR3(0.0f, 0.0f, m_fDepth), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	m_pLine[SIDE_MIDDLE_RIGHT_FRONT] = CLine::Create(m_pos + D3DXVECTOR3(m_fWidth, m_fHeight, m_fDepth), m_pos + D3DXVECTOR3(m_fWidth, 0.0f, m_fDepth), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	m_pLine[SIDE_MIDDLE_RIGHT_BEHIND] = CLine::Create(m_pos + D3DXVECTOR3(m_fWidth, m_fHeight, 0.0f), m_pos + D3DXVECTOR3(m_fWidth, 0.0f, 0.0f), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	m_pLine[SIDE_MIDDLE_LEFT_BEHIND] = CLine::Create(m_pos + D3DXVECTOR3(0.0f, m_fHeight, 0.0f), m_pos + D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));

	// 底辺
	m_pLine[SIDE_LOWER_FRONT] = CLine::Create(m_pos + D3DXVECTOR3(0.0f, 0.0f, m_fDepth), m_pos + D3DXVECTOR3(m_fWidth, 0.0f, m_fDepth), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	m_pLine[SIDE_LOWER_RIGHT] = CLine::Create(m_pos + D3DXVECTOR3(m_fWidth, 0.0f, m_fDepth), m_pos + D3DXVECTOR3(m_fWidth, 0.0f, 0.0f), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	m_pLine[SIDE_LOWER_BEHIND] = CLine::Create(m_pos + D3DXVECTOR3(m_fWidth, 0.0f, 0.0f), m_pos + D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	m_pLine[SIDE_LOWER_LEFT] = CLine::Create(m_pos + D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_pos + D3DXVECTOR3(0.0f, 0.0f, m_fDepth), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CCollision::Uninit(void)
{
	if (NULL != m_pLine)
	{
		for (int nCntLine = 0; nCntLine < SIDE_MAX; nCntLine++)
		{// 破棄
			m_pLine[nCntLine]->Uninit();
			m_pLine[nCntLine] = NULL;
		}

		// メモリの開放
		delete m_pLine;
		m_pLine = NULL;
	}

	// 自分を破棄
	CScene::Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CCollision::Update(void)
{
}

//=============================================================================
// 描画処理
//=============================================================================
void CCollision::Draw(void)
{
}

//=============================================================================
// 当たり判定
//=============================================================================
bool CCollision::Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 posOld, D3DXVECTOR3 vtxMax, D3DXVECTOR3 vtxMin)
{
	bool bHit = false;
	// モデルオブジェクトを探す
	CScene *pScene = CScene::GetSceneTop(COLLISION_PRIORITY);

	while (pScene != NULL)
	{// NULLになるまでループ
		CScene *pSceneNext = pScene->GetSceneNext();
		CScene::OBJTYPE objType = pScene->GetObjType();

		if (CScene::OBJTYPE_COLLISION == objType)
		{// 当たり判定の場合
			CCollision *pCollision = (CCollision*)pScene;
			if (pCollision->CollisionBox(pPos, posOld, vtxMax, vtxMin))
			{// ボックスコリジョンに衝突している
				bHit = true;
			}
		}

		// 次のオブジェクトを見る
		pScene = pSceneNext;
	}

	return bHit;
}

//=============================================================================
// ボックス型の当たり判定
//=============================================================================
bool CCollision::CollisionBox(D3DXVECTOR3 *pPos, D3DXVECTOR3 posOld, D3DXVECTOR3 vtxMax, D3DXVECTOR3 vtxMin)
{
	bool bHitX = false, bHitY = false, bHitZ = false;
	D3DXVECTOR3 modelVtxMax, modelVtxMin;
	modelVtxMax = *pPos + vtxMax;
	modelVtxMin = *pPos + vtxMin;

	D3DXVECTOR3 vecA, vecB, vecC;

	/********************************/
	/*			Xの外積計算			*/
	/********************************/
	// 最小値側の判定
	vecA = m_pLine[SIDE_LOWER_LEFT]->GetVector();
	vecB = modelVtxMax - m_pLine[SIDE_LOWER_LEFT]->GetStart();
	vecC = modelVtxMin - m_pLine[SIDE_LOWER_LEFT]->GetStart();

	if ((vecA.z * vecB.x) - (vecA.x * vecB.z) > 0 || (vecA.z * vecC.x) - (vecA.x * vecC.z) > 0)
	{// 範囲内
	 // 最大数側の判定
		vecA = m_pLine[SIDE_LOWER_RIGHT]->GetVector();
		vecB = modelVtxMin - m_pLine[SIDE_LOWER_RIGHT]->GetStart();
		vecC = modelVtxMax - m_pLine[SIDE_LOWER_RIGHT]->GetStart();

		if ((vecA.z * vecB.x) - (vecA.x * vecB.z) > 0 || (vecA.z * vecC.x) - (vecA.x * vecC.z) > 0)
		{// 範囲内
			bHitX = true;
		}
		else { return false; }
	}
	else { return false; }

	/********************************/
	/*			Zの外積計算			*/
	/********************************/
	// 最小値側の判定
	vecA = m_pLine[SIDE_LOWER_BEHIND]->GetVector();
	vecB = modelVtxMax - m_pLine[SIDE_LOWER_BEHIND]->GetStart();
	vecC = modelVtxMin - m_pLine[SIDE_LOWER_BEHIND]->GetStart();

	if ((vecA.z * vecB.x) - (vecA.x * vecB.z) > 0 || (vecA.z * vecC.x) - (vecA.x * vecC.z) > 0)
	{// 範囲内
	 // 最大数側の判定
		vecA = m_pLine[SIDE_LOWER_FRONT]->GetVector();
		vecB = modelVtxMin - m_pLine[SIDE_LOWER_FRONT]->GetStart();
		vecC = modelVtxMax - m_pLine[SIDE_LOWER_FRONT]->GetStart();

		if ((vecA.z * vecB.x) - (vecA.x * vecB.z) > 0 || (vecA.z * vecC.x) - (vecA.x * vecC.z) > 0)
		{// 範囲内
			bHitZ = true;
		}
		else { return false; }
	}
	else { return false; }

	if (bHitX && bHitZ)
	{// 範囲内に入っている
		if (pPos->x - vtxMax.x <= m_fWidth + m_pos.x && posOld.x - (vtxMax.x - 1.0f) >= m_fWidth + m_pos.x)
		{// 右側から衝突
			pPos->x = vtxMax.x + m_fWidth + m_pos.x;
		}
		else if (pPos->x + vtxMax.z > m_pos.x && posOld.x + (vtxMax.z - 1.0f) <= m_pos.x)
		{// 左側から衝突
			pPos->x = m_pos.x - vtxMax.z;
		}

		if (pPos->z - vtxMax.x <= m_fDepth + m_pos.z && posOld.z - (vtxMax.x - 1.0f) >= m_fDepth + m_pos.z)
		{// 前側から衝突
			pPos->z = vtxMax.x + m_fDepth + m_pos.z;
		}
		else if (pPos->z + vtxMax.z >= m_pos.z && posOld.z + (vtxMax.z - 1.0f) <= m_pos.z)
		{// 後側から衝突
			pPos->z = m_pos.z - vtxMax.z;
		}

		return true;
	}

	return false;
}

//=============================================================================
// 位置の設定
//=============================================================================
void CCollision::SetPos(D3DXVECTOR3 pos)
{
	// 位置の設定
	m_pos = pos;

	// 上辺
	m_pLine[SIDE_UPPER_FRONT]->SetPoint(m_pos + D3DXVECTOR3(0.0f, m_fHeight, m_fDepth), m_pos + D3DXVECTOR3(m_fWidth, m_fHeight, m_fDepth));
	m_pLine[SIDE_UPPER_RIGHT]->SetPoint(m_pos + D3DXVECTOR3(m_fWidth, m_fHeight, m_fDepth), m_pos + D3DXVECTOR3(m_fWidth, m_fHeight, 0.0f));
	m_pLine[SIDE_UPPER_BEHIND]->SetPoint(m_pos + D3DXVECTOR3(m_fWidth, m_fHeight, 0.0f), m_pos + D3DXVECTOR3(0.0f, m_fHeight, 0.0f));
	m_pLine[SIDE_UPPER_LEFT]->SetPoint(m_pos + D3DXVECTOR3(0.0f, m_fHeight, 0.0f), m_pos + D3DXVECTOR3(0.0f, m_fHeight, m_fDepth));

	// 中辺
	m_pLine[SIDE_MIDDLE_LEFT_FRONT]->SetPoint(m_pos + D3DXVECTOR3(0.0f, m_fHeight, m_fDepth), m_pos + D3DXVECTOR3(0.0f, 0.0f, m_fDepth));
	m_pLine[SIDE_MIDDLE_RIGHT_FRONT]->SetPoint(m_pos + D3DXVECTOR3(m_fWidth, m_fHeight, m_fDepth), m_pos + D3DXVECTOR3(m_fWidth, 0.0f, m_fDepth));
	m_pLine[SIDE_MIDDLE_RIGHT_BEHIND]->SetPoint(m_pos + D3DXVECTOR3(m_fWidth, m_fHeight, 0.0f), m_pos + D3DXVECTOR3(m_fWidth, 0.0f, 0.0f));
	m_pLine[SIDE_MIDDLE_LEFT_BEHIND]->SetPoint(m_pos + D3DXVECTOR3(0.0f, m_fHeight, 0.0f), m_pos + D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	// 底辺
	m_pLine[SIDE_LOWER_FRONT]->SetPoint(m_pos + D3DXVECTOR3(0.0f, 0.0f, m_fDepth), m_pos + D3DXVECTOR3(m_fWidth, 0.0f, m_fDepth));
	m_pLine[SIDE_LOWER_RIGHT]->SetPoint(m_pos + D3DXVECTOR3(m_fWidth, 0.0f, m_fDepth), m_pos + D3DXVECTOR3(m_fWidth, 0.0f, 0.0f));
	m_pLine[SIDE_LOWER_BEHIND]->SetPoint(m_pos + D3DXVECTOR3(m_fWidth, 0.0f, 0.0f), m_pos + D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_pLine[SIDE_LOWER_LEFT]->SetPoint(m_pos + D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_pos + D3DXVECTOR3(0.0f, 0.0f, m_fDepth));
}

//=============================================================================
// サイズの設定
//=============================================================================
void CCollision::SetSize(D3DXVECTOR3 size)
{
	// サイズの設定
	m_fWidth = size.x;
	m_fHeight = size.y;
	m_fDepth = size.z;

	// 上辺
	m_pLine[SIDE_UPPER_FRONT]->SetPoint(m_pos + D3DXVECTOR3(0.0f, m_fHeight, m_fDepth), m_pos + D3DXVECTOR3(m_fWidth, m_fHeight, m_fDepth));
	m_pLine[SIDE_UPPER_RIGHT]->SetPoint(m_pos + D3DXVECTOR3(m_fWidth, m_fHeight, m_fDepth), m_pos + D3DXVECTOR3(m_fWidth, m_fHeight, 0.0f));
	m_pLine[SIDE_UPPER_BEHIND]->SetPoint(m_pos + D3DXVECTOR3(m_fWidth, m_fHeight, 0.0f), m_pos + D3DXVECTOR3(0.0f, m_fHeight, 0.0f));
	m_pLine[SIDE_UPPER_LEFT]->SetPoint(m_pos + D3DXVECTOR3(0.0f, m_fHeight, 0.0f), m_pos + D3DXVECTOR3(0.0f, m_fHeight, m_fDepth));

	// 中辺
	m_pLine[SIDE_MIDDLE_LEFT_FRONT]->SetPoint(m_pos + D3DXVECTOR3(0.0f, m_fHeight, m_fDepth), m_pos + D3DXVECTOR3(0.0f, 0.0f, m_fDepth));
	m_pLine[SIDE_MIDDLE_RIGHT_FRONT]->SetPoint(m_pos + D3DXVECTOR3(m_fWidth, m_fHeight, m_fDepth), m_pos + D3DXVECTOR3(m_fWidth, 0.0f, m_fDepth));
	m_pLine[SIDE_MIDDLE_RIGHT_BEHIND]->SetPoint(m_pos + D3DXVECTOR3(m_fWidth, m_fHeight, 0.0f), m_pos + D3DXVECTOR3(m_fWidth, 0.0f, 0.0f));
m_pLine[SIDE_MIDDLE_LEFT_BEHIND]->SetPoint(m_pos + D3DXVECTOR3(0.0f, m_fHeight, 0.0f), m_pos + D3DXVECTOR3(0.0f, 0.0f, 0.0f));

// 底辺
m_pLine[SIDE_LOWER_FRONT]->SetPoint(m_pos + D3DXVECTOR3(0.0f, 0.0f, m_fDepth), m_pos + D3DXVECTOR3(m_fWidth, 0.0f, m_fDepth));
m_pLine[SIDE_LOWER_RIGHT]->SetPoint(m_pos + D3DXVECTOR3(m_fWidth, 0.0f, m_fDepth), m_pos + D3DXVECTOR3(m_fWidth, 0.0f, 0.0f));
m_pLine[SIDE_LOWER_BEHIND]->SetPoint(m_pos + D3DXVECTOR3(m_fWidth, 0.0f, 0.0f), m_pos + D3DXVECTOR3(0.0f, 0.0f, 0.0f));
m_pLine[SIDE_LOWER_LEFT]->SetPoint(m_pos + D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_pos + D3DXVECTOR3(0.0f, 0.0f, m_fDepth));
}
