//=============================================================================
//
// AI処理 [AI.cpp]
// Author : Takuto Ishida
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "AI.h"
#include "player.h"
#include "manager.h"
#include "renderer.h"
#include "debugProc.h"
#include "model.h"
#include "motion.h"
#include "game.h"
#include "bullet.h"
#include "server.h"
#include "collisionSet.h"
#include <stdio.h>

//==================================
// マクロ定義
//==================================
#define WALKER_FILE					"data/TEXT/AI/walker/model_walker.txt"
#define DRONE_FILE					"data/TEXT/AI/drone/model_drone.txt"

//==================================
// 静的メンバ変数宣言
//==================================

//==================================
// 生成処理
//==================================
CAIMecha* CAIMecha::Create(CPlayer *pPlayer, MECHATYPE mecha, D3DXVECTOR3 pos)
{
	CAIMecha *pAI;

	pAI = new CAIMecha;	// メモリを確保

	if (pAI != NULL)
	{// メモリ確保成功
		pAI->m_mechaType = mecha;
		pAI->m_pos = pos;
		pAI->m_pPlayer = pPlayer;
		pAI->Init();
	}

	return pAI;
}

//=========================================
// コンストラクタ
//=========================================
CAIMecha::CAIMecha(int nPriority, CScene::OBJTYPE objType) : CScene(nPriority, objType)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nCntState = 0;
	m_pModel = NULL;
	m_pMotion = NULL;
	m_nNumParts = 0;
	m_vtxMax = D3DXVECTOR3(-1000.0f, -1000.0f, -1000.0f);
	m_vtxMin = D3DXVECTOR3(1000.0f, 1000.0f, 1000.0f);
	m_nLife = 0;
	m_fSpeed = 0.0f;
	m_nAttack = 0;
	m_mechaType = MECHATYPE_WALKER;
	m_nCapacity = 0;
	m_nCurMotion = 0;
	m_nLifeMax = 0;
	m_nNumShoot = 0;
	m_pPlayer = NULL;
	m_nTeam = 0;
}

//=========================================
// デストラクタ
//=========================================
CAIMecha::~CAIMecha()
{
}

//=========================================
// 初期化処理
//=========================================
HRESULT CAIMecha::Init(void)
{
	// デバイスの取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	FILE *pFile;
	int nCntParts = 0;		// パーツ数のカウンタ
	int nCntModel = 0;
	float fX, fY, fZ;
	int nId;
	int nParent;
	char **pModelName = NULL;
	char *pFileName = "";

	if (MECHATYPE_WALKER == m_mechaType) { pFileName = WALKER_FILE; }
	else if (MECHATYPE_DRONE == m_mechaType) { pFileName = DRONE_FILE; }

	// ファイルを開く
	pFile = fopen(pFileName, "r");

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

					if (strcmp(aStr, "NUM_MODEL") == 0)
					{// モデル数
						int nNumModel;
						fscanf(pFile, " = %d", &nNumModel);

						pModelName = new char*[nNumModel];		// メモリ確保
					}
					if (strcmp(aStr, "MODEL_FILENAME") == 0)
					{// キャラクター情報
						fscanf(pFile, " = %s", &aStr);

						pModelName[nCntModel] = new char[64];		// メモリ確保
						strcpy(pModelName[nCntModel], aStr);
						nCntModel++;
					}
					if (strcmp(aStr, "CHARACTERSET") == 0)
					{// キャラクター情報
						while (strcmp(aStr, "END_CHARACTERSET") != 0)
						{// キャラクター情報終了メッセージまでループ
							fscanf(pFile, "%s", aStr);

							if (strcmp(aStr, "NUM_PARTS") == 0)
							{// パーツ数
								fscanf(pFile, " = %d", &m_nNumParts);
								m_pModel = new CModel*[m_nNumParts];

								for (int nCntModelParts = 0; nCntModelParts < m_nNumParts; nCntModelParts++)
								{// パーツ数分ループ
									m_pModel[nCntModelParts] = CModel::Create(&m_mtxWorld);
								}
							}
							else if (strcmp(aStr, "CAPACITY") == 0)
							{// 装弾数
								fscanf(pFile, " = %d", &m_nCapacity);
							}
							else if (strcmp(aStr, "ATTACK") == 0)
							{// 攻撃力
								fscanf(pFile, " = %d", &m_nAttack);
							}
							else if (strcmp(aStr, "SHOOTS") == 0)
							{// 発射数
								fscanf(pFile, " = %d", &m_nNumShoot);
							}
							else if (strcmp(aStr, "LIFE") == 0)
							{// 耐久力
								fscanf(pFile, " = %d", &m_nLifeMax);
							}
							else if (strcmp(aStr, "SPEED") == 0)
							{// 移動量
								fscanf(pFile, " = %f", &m_fSpeed);
							}
							else if (strcmp(aStr, "PARTSSET") == 0)
							{// パーツの設定
								while (strcmp(aStr, "END_PARTSSET") != 0)
								{// パーツの設定終了メッセージまでループ
									if (m_pModel[nCntParts] != NULL)
									{// メモリ確保成功
										fscanf(pFile, "%s", &aStr);

										if (strcmp(aStr, "INDEX") == 0)
										{// モデル番号
											fscanf(pFile, " = %d", &nId);

											// 使うモデルを指定
											m_pModel[nCntParts]->SetModel(pModelName[nId]);
											m_pModel[nCntParts]->Init();
										}
										else if (strcmp(aStr, "PARENT") == 0)
										{// 親を決める
											fscanf(pFile, " = %d", &nParent);

											if (nParent >= 0)
											{// 親のポインタを設定
												m_pModel[nCntParts]->SetParent(m_pModel[nParent]);
											}
											else
											{// 親がいない場合
												m_pModel[nCntParts]->SetParent(NULL);
											}
										}
										else if (strcmp(aStr, "POS") == 0)
										{// 位置の設定
											fscanf(pFile, " = %f %f %f", &fX, &fY, &fZ);

											// 位置の設定
											m_pModel[nCntParts]->SetPos(D3DXVECTOR3(fX, fY, fZ));
										}
										else if (strcmp(aStr, "ROT") == 0)
										{// 向きの設定
											fscanf(pFile, " = %f %f %f", &fX, &fY, &fZ);

											// 向きの設定
											m_pModel[nCntParts]->SetRot(D3DXVECTOR3(fX, fY, fZ));
										}
									}
								}
								nCntParts++;
							}
						}
					}
				}
				break;
			}
		}
		fclose(pFile);	// ファイルを閉じる
	}
	else
	{// ファイルがないとき
		MessageBox(0, "ファイルがありません", "プレイヤーモデルテキスト", MB_YESNO);
	}

	if (NULL != pModelName)
	{// メモリの開放
		for (int nCnt = 0; nCnt < m_nNumParts; nCnt++)
		{// メモリの開放
			delete[] pModelName[nCnt];
		}

		// メモリの開放
		delete[] pModelName;
	}

	if (NULL == m_pMotion)
	{// モーションクラスの生成
		m_pMotion = CMotion::Create(m_mechaType, m_nNumParts, m_pModel);
		m_pMotion->SetMotion(CMotionManager::TYPE_NEUTRAL);	// ニュートラルモーションを設定
	}

	// 頂点座標の最小値と最大値を求める
	D3DXVECTOR3 pos, posMax, posMin;
	D3DXVECTOR3 posMaxVtx, posMinVtx;
	posMax = m_vtxMax;
	posMin = m_vtxMin;
	for (int nCntModel = 0; nCntModel < m_nNumParts; nCntModel++)
	{
		pos = m_pModel[nCntModel]->GetPos();
		posMaxVtx = m_pModel[nCntModel]->GetVtxMax() + pos;
		posMinVtx = m_pModel[nCntModel]->GetVtxMin() + pos;

		// Xの値の比較
		if (posMin.x > posMinVtx.x)
		{// 最小値と比較
			posMin.x = posMinVtx.x;	// 最小値より小さければ代入
		}

		if (posMax.x < posMaxVtx.x)
		{// 最大値と比較
			posMax.x = posMaxVtx.x;	// 最大値より大きければ代入
		}

		// Yの値の比較
		if (posMin.y > posMinVtx.y)
		{// 最小値と比較
			posMin.y = posMinVtx.y;	// 最小値より小さければ代入
		}

		if (posMax.y < posMaxVtx.y)
		{// 最大値と比較
			posMax.y = posMaxVtx.y;	// 最大値より大きければ代入
		}

		// Zの値の比較
		if (posMin.z > posMinVtx.z)
		{// 最小値と比較
			posMin.z = posMinVtx.z;	// 最小値より小さければ代入
		}

		if (posMax.z < posMaxVtx.z)
		{// 最大値と比較
			posMax.z = posMaxVtx.z;	// 最大値より大きければ代入
		}
	}

	// 全パーツの頂点座標の最小値・最大値
	m_vtxMin.x = posMin.x;
	m_vtxMin.y = posMin.y;
	m_vtxMin.z = posMin.z;
	m_vtxMax.x = posMax.x;
	m_vtxMax.y = posMax.y;
	m_vtxMax.z = posMax.z;

	m_nLife = m_nLifeMax;	// ライフの初期値

	if (m_pPlayer->GetTeam() == 0 || m_pPlayer->GetTeam() == 1)
	{
		for (int nCntModel = 0; nCntModel < m_nNumParts; nCntModel++)
		{// チームごとの色に設定
			m_pModel[nCntModel]->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
		}

		// チームの設定
		m_nTeam = 0;
	}
	else if (m_pPlayer->GetTeam() == 2 || m_pPlayer->GetTeam() == 3)
	{
		for (int nCntModel = 0; nCntModel < m_nNumParts; nCntModel++)
		{// チームごとの色に設定
			m_pModel[nCntModel]->SetColor(D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
		}

		// チームの設定
		m_nTeam = 1;
	}

	return S_OK;
}

//=========================================
// 終了処理
//=========================================
void CAIMecha::Uninit(void)
{
	if (NULL != m_pMotion)
	{// モーションクラスの破棄
		m_pMotion->Uninit();
		m_pMotion = NULL;
	}

	if (NULL != m_pModel)
	{
		for (int nCntModel = 0; nCntModel < m_nNumParts; nCntModel++)
		{// モデルの破棄
			if (NULL != m_pModel[nCntModel])
			{
				m_pModel[nCntModel]->Uninit();
				delete m_pModel[nCntModel];
				m_pModel[nCntModel] = NULL;
			}
		}

		// メモリの開放
		delete[] m_pModel;
		m_pModel = NULL;
	}

	// オブジェクトの破棄
	CScene::Release();
}

//=========================================
// 更新処理
//=========================================
void CAIMecha::Update(void)
{
	if (NULL != m_pMotion && MECHATYPE_WALKER == m_mechaType)
	{// モーション
		m_pMotion->SetMotion(CMotionManager::TYPE_NEUTRAL);
	}

	if (MECHATYPE_DRONE == m_mechaType)
	{// ドローンのプロペラを回す
		D3DXVECTOR3 rot = m_pModel[1]->GetRot();
		rot.y += 2.0f;
		m_pModel[1]->SetRot(rot);
	}
}

//=========================================
// 描画処理
//=========================================
void CAIMecha::Draw(void)
{
	// デバイスの取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;	// 計算用マトリックス

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 移動を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	for (int nCntModel = 0; nCntModel < m_nNumParts; nCntModel++)
	{// モデルの描画処理
		m_pModel[nCntModel]->Draw();
	}
}
