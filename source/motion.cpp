//=============================================================================
//
// モーション処理 [motion.cpp]
// Author : Takuto Ishida
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "motion.h"
#include "renderer.h"
#include "manager.h"
#include "debugProc.h"
#include "model.h"
#include "meshOrbit.h"
#include "particle.h"
#include "loadText.h"

//==================================
// マクロ定義
//==================================

//==================================
// 静的メンバ変数宣言
//==================================
int **CMotionManager::m_pFrame[MAX_FILE] = {};								// フレーム
CMotionManager::KEY ***CMotionManager::m_pKey[MAX_FILE] = {};				// パーツごとのキー要素
bool *CMotionManager::m_pLoop[MAX_FILE] = {};								// ループするかどうか
int *CMotionManager::m_pNumKey[MAX_FILE] = {};								// キー数
int *CMotionManager::m_pType[MAX_FILE] = {};								// モーションの種類
CMotionManager::COLLISION **CMotionManager::m_pCollision[MAX_FILE] = {};	// 当たり判定
CMotionManager::ORBIT **CMotionManager::m_pOrbit[MAX_FILE] = {};			// 軌跡
int CMotionManager::m_nParts[MAX_FILE] = {};								// パーツ数
int CMotionManager::m_nNumMotion[MAX_FILE] = {};							// モーション数

//=========================================
// 生成処理
//=========================================
CMotionManager *CMotionManager::Create(int nFile)
{
	// メモリ確保
	CMotionManager *pMotionManager = new CMotionManager;

	if (pMotionManager != NULL)
	{// メモリ確保成功
		pMotionManager->m_nFile = nFile;
		pMotionManager->Init();
	}

	return pMotionManager;
}

//=========================================
// モデルの読み込み
//=========================================
HRESULT CMotionManager::Load(void)
{
	FILE *pFile;
	int nCntMotion = 0;
	int nCntKey = 0;
	int nCntParts = 0;
	int nNumParts;
	int nCntFile = 0;
	char *pFileName[MAX_FILE] =
	{
		{ "data/TEXT/PLAYER/assult/motion_assult.txt" },
		{ "data/TEXT/PLAYER/light/motion_light.txt" },
		{ "data/TEXT/PLAYER/heavy/motion_heavy.txt" },
		{ "data/TEXT/PLAYER/snipe/motion_snipe.txt" },

		{ "data/TEXT/AI/drone/motion_drone.txt" },

		{ "data/TEXT/ENEMY/motion.txt" },
	};

	for (nCntFile = 0; nCntFile < MAX_FILE; nCntFile++)
	{
		// ファイルを開く
		pFile = fopen(pFileName[nCntFile], "r");

		if (NULL != pFile)
		{// ファイルがあった
			char *pStrCur;		// 文字列の先頭へのポインタ
			char aLine[256];	// 文字列読み込み用
			char aStr[256];		// 文字列抜き出し用

			strcpy(aStr, "");

			while (memcmp(aStr, "END_SCRIPT", strlen("END_SCRIPT")) != 0)
			{
				pStrCur = CLoadText::ReadLine(pFile, &aLine[0]);	// 文字列の先頭を読み込み
				strcpy(aStr, pStrCur);					// 文字列を取り出す

				if (memcmp(aStr, "NUM_MODEL = ", strlen("NUM_MODEL = ")) == 0)
				{// パーツ数
					pStrCur += strlen("NUM_MODEL = ");	// 頭出し
					strcpy(aStr, pStrCur);				// 文字列の取り出し

					nNumParts = atoi(aStr);
				}
				else if (memcmp(aStr, "NUM_MOTION = ", strlen("NUM_MOTION = ")) == 0)
				{// モーション数
					pStrCur += strlen("NUM_MOTION = ");	// 頭出し
					strcpy(aStr, pStrCur);				// 文字列の取り出し

					m_nNumMotion[nCntFile] = atoi(aStr);

					// メモリ確保
					m_pNumKey[nCntFile] = new int[m_nNumMotion[nCntFile]];
					m_pLoop[nCntFile] = new bool[m_nNumMotion[nCntFile]];
					m_pType[nCntFile] = new int[m_nNumMotion[nCntFile]];
					m_pFrame[nCntFile] = new int*[m_nNumMotion[nCntFile]];
					m_pKey[nCntFile] = new KEY**[m_nNumMotion[nCntFile]];
					m_pCollision[nCntFile] = new COLLISION*[m_nNumMotion[nCntFile]];
					m_pOrbit[nCntFile] = new ORBIT*[m_nNumMotion[nCntFile]];
				}
				else if (memcmp(aStr, "MOTIONSET", strlen("MOTIONSET")) == 0)
				{// キャラクター情報の設定
					m_pCollision[nCntFile][nCntMotion] = NULL;
					m_pOrbit[nCntFile][nCntMotion] = NULL;

					while (memcmp(aStr, "END_MOTIONSET", strlen("END_MOTIONSET")) != 0)
					{// 終了メッセージまでループ
						pStrCur = CLoadText::ReadLine(pFile, &aLine[0]);	// 文字列の先頭を読み込み
						strcpy(aStr, pStrCur);					// 文字列を取り出す

						if (memcmp(aStr, "LOOP = ", strlen("LOOP = ")) == 0)
						{// ループの有無
							pStrCur += strlen("LOOP = ");			// 頭出し
							strcpy(aStr, pStrCur);					// 文字列の取り出す

							if (atoi(aStr) != 0)
							{// 正のとき
								m_pLoop[nCntFile][nCntMotion] = true;
							}
							else
							{// 負のとき
								m_pLoop[nCntFile][nCntMotion] = false;
							}
						}
						else if (memcmp(aStr, "NUM_KEY = ", strlen("NUM_KEY = ")) == 0)
						{// キー数
							pStrCur += strlen("NUM_KEY = ");	// 頭出し
							strcpy(aStr, pStrCur);				// 文字列の取り出す

							m_pNumKey[nCntFile][nCntMotion] = atoi(aStr);	// 整数型に直す

							m_pFrame[nCntFile][nCntMotion] = new int[m_pNumKey[nCntFile][nCntMotion]];	// 必要なフレーム分メモリ確保
							m_pKey[nCntFile][nCntMotion] = new KEY*[m_pNumKey[nCntFile][nCntMotion]];	// キー数分メモリ確保
						}
						else if (memcmp(aStr, "COLLISION = ", strlen("COLLISION = ")) == 0)
						{// 当たり判定
							pStrCur += strlen("COLLISION = ");	// 頭出し
							strcpy(aStr, pStrCur);				// 文字列の取り出す

							// メモリ確保
							m_pCollision[nCntFile][nCntMotion] = new COLLISION;

							// モデル番号
							pStrCur += CLoadText::PopString(&pStrCur[0], &aStr[0]);
							m_pCollision[nCntFile][nCntMotion]->nIdxModel = atoi(&aStr[0]);	// 整数型に直す

							// Ｘ軸
							pStrCur += CLoadText::PopString(&pStrCur[0], &aStr[0]);
							m_pCollision[nCntFile][nCntMotion]->fPosX = (float)atof(&aStr[0]);	// 小数型に直す

							// Ｙ軸
							pStrCur += CLoadText::PopString(&pStrCur[0], &aStr[0]);
							m_pCollision[nCntFile][nCntMotion]->fPosY = (float)atof(&aStr[0]);	// 小数型に直す

							// Ｚ軸
							pStrCur += CLoadText::PopString(&pStrCur[0], &aStr[0]);
							m_pCollision[nCntFile][nCntMotion]->fPosZ = (float)atof(&aStr[0]);	// 小数型に直す

							// 半径
							pStrCur += CLoadText::PopString(&pStrCur[0], &aStr[0]);
							m_pCollision[nCntFile][nCntMotion]->fRadius = (float)atof(&aStr[0]);	// 小数型に直す

							// 開始フレーム
							pStrCur += CLoadText::PopString(&pStrCur[0], &aStr[0]);
							m_pCollision[nCntFile][nCntMotion]->nStartFrame = atoi(&aStr[0]);	// 整数型に直す

							// 終了フレーム
							pStrCur += CLoadText::PopString(&pStrCur[0], &aStr[0]);
							m_pCollision[nCntFile][nCntMotion]->nEndFrame = atoi(&aStr[0]);	// 整数型に直す
						}
						else if (memcmp(aStr, "ORBIT = ", strlen("ORBIT = ")) == 0)
						{// 軌跡
							pStrCur += strlen("ORBIT = ");		// 頭出し
							strcpy(aStr, pStrCur);				// 文字列の取り出す

							// メモリ確保
							m_pOrbit[nCntFile][nCntMotion] = new ORBIT;

							if (NULL != m_pOrbit[nCntFile][nCntMotion])
							{
								// モデル番号
								pStrCur += CLoadText::PopString(&pStrCur[0], &aStr[0]);
								m_pOrbit[nCntFile][nCntMotion]->nIdxModel = atoi(&aStr[0]);		// 整数型に直す

								// Ｘ軸
								pStrCur += CLoadText::PopString(&pStrCur[0], &aStr[0]);
								m_pOrbit[nCntFile][nCntMotion]->fPosX = (float)atof(&aStr[0]);	// 小数型に直す

								// Ｙ軸
								pStrCur += CLoadText::PopString(&pStrCur[0], &aStr[0]);
								m_pOrbit[nCntFile][nCntMotion]->fPosY = (float)atof(&aStr[0]);	// 小数型に直す

								// Ｚ軸
								pStrCur += CLoadText::PopString(&pStrCur[0], &aStr[0]);
								m_pOrbit[nCntFile][nCntMotion]->fPosZ = (float)atof(&aStr[0]);	// 小数型に直す

								// 開始フレーム
								pStrCur += CLoadText::PopString(&pStrCur[0], &aStr[0]);
								m_pOrbit[nCntFile][nCntMotion]->nStartFrame = atoi(&aStr[0]);	// 整数型に直す

								// 終了フレーム
								pStrCur += CLoadText::PopString(&pStrCur[0], &aStr[0]);
								m_pOrbit[nCntFile][nCntMotion]->nEndFrame = atoi(&aStr[0]);		// 整数型に直す
							}
						}
						else if (memcmp(aStr, "KEYSET", strlen("KEYSET")) == 0)
						{// パーツの設定
							m_pKey[nCntFile][nCntMotion][nCntKey] = new KEY[nNumParts];

							while (memcmp(aStr, "END_KEYSET", strlen("END_KEYSET")) != 0)
							{// 終了メッセージまでループ
								pStrCur = CLoadText::ReadLine(pFile, &aLine[0]);			// 文字列の先頭を読み込み
								strcpy(aStr, pStrCur);		// 文字列を取り出す

								if (memcmp(aStr, "FRAME = ", strlen("FRAME = ")) == 0)
								{// フレーム数
									pStrCur += strlen("FRAME = ");				// 頭出し

									strcpy(aStr, pStrCur);						// 文字列を取り出す
									m_pFrame[nCntFile][nCntMotion][nCntKey] = atoi(aStr);	// 整数型に直す
								}
								else if (memcmp(aStr, "KEY", strlen("KEY")) == 0)
								{// キー情報
									while (memcmp(aStr, "END_KEY", strlen("END_KEY")) != 0)
									{
										pStrCur = CLoadText::ReadLine(pFile, &aLine[0]);			// 文字列の先頭を読み込み
										strcpy(aStr, pStrCur);		// 文字列を取り出す

										if (memcmp(aStr, "POS = ", strlen("POS = ")) == 0)
										{// 位置
											pStrCur += strlen("POS = ");			// 頭出し

											// Ｘ軸
											pStrCur += CLoadText::PopString(&pStrCur[0], &aStr[0]);
											m_pKey[nCntFile][nCntMotion][nCntKey][nCntParts].posX = (float)atof(&aStr[0]);	// 整数型に直す

											// Ｙ軸
											pStrCur += CLoadText::PopString(&pStrCur[0], &aStr[0]);
											m_pKey[nCntFile][nCntMotion][nCntKey][nCntParts].posY = (float)atof(&aStr[0]);	// 整数型に直す

											// Ｚ軸
											pStrCur += CLoadText::PopString(&pStrCur[0], &aStr[0]);
											m_pKey[nCntFile][nCntMotion][nCntKey][nCntParts].posZ = (float)atof(&aStr[0]);	// 整数型に直す
										}
										else if (memcmp(aStr, "ROT = ", strlen("ROT = ")) == 0)
										{// 向き
											pStrCur += strlen("ROT = ");			// 頭出し

											// Ｘ軸
											pStrCur += CLoadText::PopString(&pStrCur[0], &aStr[0]);
											m_pKey[nCntFile][nCntMotion][nCntKey][nCntParts].rotX = (float)atof(&aStr[0]);	// 整数型に直す

											// Ｙ軸
											pStrCur += CLoadText::PopString(&pStrCur[0], &aStr[0]);
											m_pKey[nCntFile][nCntMotion][nCntKey][nCntParts].rotY = (float)atof(&aStr[0]);	// 整数型に直す

											// Ｚ軸
											pStrCur += CLoadText::PopString(&pStrCur[0], &aStr[0]);
											m_pKey[nCntFile][nCntMotion][nCntKey][nCntParts].rotZ = (float)atof(&aStr[0]);	// 整数型に直す
										}
									}
									nCntParts++;	// パーツ数のカウント
								}
							}
							nCntKey++;	// キーのカウント
							nCntParts = 0;
						}
					}
					m_pType[nCntFile][nCntMotion] = nCntMotion;
					nCntMotion++;	// モーション数のカウント
					nCntKey = 0;
					nCntParts = 0;

					if (m_nNumMotion[nCntFile] <= nCntMotion)
					{// モーション数が超えたら抜ける
						break;
					}
				}
			}
			fclose(pFile);	// ファイルを閉じる
			nCntMotion = 0;	// モーション数のカウント
			nCntKey = 0;
			nCntParts = 0;
		}
		else
		{// ファイルがないとき
			MessageBox(0, "ファイルがありません。", "motion.txt", MB_YESNO);
		}
	}

	return S_OK;
}

//=========================================
// 読み込んだ情報の解放
//=========================================
void CMotionManager::Unload(void)
{
	for (int nCntFile = 0; nCntFile < MAX_FILE; nCntFile++)
	{
		for (int nCntMotion = 0; nCntMotion < m_nNumMotion[nCntFile]; nCntMotion++)
		{
			if (NULL != m_pKey[nCntFile])
			{// キー情報の破棄
				for (int nCnt = 0; nCnt < m_pNumKey[nCntFile][nCntMotion]; nCnt++)
				{
					if (NULL != m_pKey[nCntFile][nCntMotion][nCnt])
					{
						delete m_pKey[nCntFile][nCntMotion][nCnt];
						m_pKey[nCntFile][nCntMotion][nCnt] = NULL;
					}
				}

				if (NULL != m_pKey[nCntFile][nCntMotion])
				{
					delete m_pKey[nCntFile][nCntMotion];
					m_pKey[nCntFile][nCntMotion] = NULL;
				}
			}

			if (NULL != m_pFrame[nCntFile])
			{// キーの破棄
				delete m_pFrame[nCntFile][nCntMotion];
				m_pFrame[nCntFile][nCntMotion] = NULL;
			}

			if (NULL != m_pCollision[nCntFile])
			{// 当たり判定の破棄
				delete m_pCollision[nCntFile][nCntMotion];
				m_pCollision[nCntFile][nCntMotion] = NULL;
			}

			if (NULL != m_pOrbit[nCntFile])
			{// 軌跡の破棄
				delete m_pOrbit[nCntFile][nCntMotion];
				m_pOrbit[nCntFile][nCntMotion] = NULL;
			}
		}

		if (NULL != m_pCollision[nCntFile])
		{// 当たり判定の破棄
			delete m_pCollision[nCntFile];
			m_pCollision[nCntFile] = NULL;
		}

		if (NULL != m_pOrbit[nCntFile])
		{// 軌跡の破棄
			delete m_pOrbit[nCntFile];
			m_pOrbit[nCntFile] = NULL;
		}

		if (NULL != m_pLoop[nCntFile])
		{// ループ情報の破棄
			delete m_pLoop[nCntFile];
			m_pLoop[nCntFile] = NULL;
		}

		if (NULL != m_pNumKey[nCntFile])
		{// キー数の破棄
			delete m_pNumKey[nCntFile];
			m_pNumKey[nCntFile] = NULL;
		}

		if (NULL != m_pKey[nCntFile])
		{// キーの破棄
			delete m_pKey[nCntFile];
			m_pKey[nCntFile] = NULL;
		}

		if (NULL != m_pFrame[nCntFile])
		{// フレームの破棄
			delete m_pFrame[nCntFile];
			m_pFrame[nCntFile] = NULL;
		}

		if (NULL != m_pType[nCntFile])
		{// 種類の破棄
			delete m_pType[nCntFile];
			m_pType[nCntFile] = NULL;
		}
	}
}

//=========================================
// コンストラクタ
//=========================================
CMotionManager::CMotionManager(int nPriority, CScene::OBJTYPE objType) : CScene(nPriority, objType)
{
	m_nFile = 0;
}

//=========================================
// デストラクタ
//=========================================
CMotionManager::~CMotionManager()
{
}

//=========================================
// 初期化処理
//=========================================
HRESULT CMotionManager::Init()
{
	return S_OK;
}

//=========================================
// 終了処理
//=========================================
void CMotionManager::Uninit(void)
{
	// 自分を破棄
	CScene::Release();
}

//=========================================
// 更新処理
//=========================================
void CMotionManager::Update(void)
{
}

//=========================================
// 描画処理
//=========================================
void CMotionManager::Draw(void)
{
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
// モーション再生クラス
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//=========================================
// モーションの再生
//=========================================
void CMotion::SetMotion(CMotionManager::TYPE type)
{
	if (m_nCurMotion != type && m_pMotionManager->GetNumMotion() > type)
	{
		int nNumMotion = m_pMotionManager->GetNumMotion();
		for (int nCntMotion = 0; nCntMotion < nNumMotion; nCntMotion++)
		{// モーション数分ループ
			if (m_pMotionManager->GetType(nCntMotion) == type)
			{// モーションの種類が一致
				m_nType = type;												// モーションの種類を取得
				m_bLoop = m_pMotionManager->GetLoop(nCntMotion);			// ループの有無を取得
				m_nKey = m_pMotionManager->GetNumKey(nCntMotion);			// キー数を取得
				m_pKey = m_pMotionManager->GetKey(nCntMotion);				// キー情報を取得
				m_pFrame = m_pMotionManager->GetFlame(nCntMotion);			// フレーム数を取得
				m_pCollision = m_pMotionManager->GetCollision(nCntMotion);	// 当たり判定
				m_pOrbit = m_pMotionManager->GetOrbit(nCntMotion);			// 軌跡
				m_nCurMotion = type;										// モーション番号を記憶

				m_nCurKey = 0;									// 現在のキーをリセット
				m_nCntFlame = 0;								// フレーム数をリセット
				m_nCntTotalFlame = 0;
				m_bInit = false;

				break;
			}
		}
	}
}

//=========================================
// モーションクラスの生成
//=========================================
CMotion *CMotion::Create(int nFile, int nParts, CModel **pModel, int nFootIdx)
{
	CMotion *pMotion = NULL;

	if (pMotion == NULL)
	{// 空のとき
		pMotion = new CMotion;

		if (pMotion != NULL)
		{// メモリ確保成功
			pMotion->m_nParts = nParts;			// パーツ数
			pMotion->m_pModel = pModel;			// モデルクラスのポインタ変数
			pMotion->m_nFootIdx = nFootIdx;		// 下半身以下の番号
			pMotion->m_pMotionManager = CMotionManager::Create(nFile);	// モーション管理クラスの生成
			pMotion->Init();
		}
	}

	return pMotion;
}

//=========================================
// コンストラクタ
//=========================================
CMotion::CMotion(int nPriority, CScene::OBJTYPE objType) : CScene(nPriority, objType)
{
	m_pFrame = NULL;
	m_pKey = NULL;
	m_posInit = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_bLoop = false;
	m_nCntFlame = 0;
	m_nCurKey = 0;
	m_nKey = 0;
	m_nParts = 0;
	m_pModel = NULL;
	m_nCurMotion = 0;
	m_nType = CMotionManager::TYPE_NEUTRAL;
	m_pMotionManager = NULL;
	m_nCntTotalFlame = 0;	// 総合フレーム数
	m_bInit = true;
	m_pOrbit = NULL;
	m_pCollision = NULL;
	m_nFootIdx = 0;
}

//=========================================
// デストラクタ
//=========================================
CMotion::~CMotion()
{
}

//=========================================
// 初期化処理
//=========================================
HRESULT CMotion::Init()
{
	m_posInit = m_pModel[0]->GetPos();				// 中心モデルの初期位置の取得
	m_nCurMotion = -1;								// 現在のモーションを指定しない
	SetMotion(CMotionManager::TYPE_NEUTRAL);		// ニュートラルモーションを設定
	return S_OK;
}

//=========================================
// 終了処理
//=========================================
void CMotion::Uninit(void)
{
	if (m_pMotionManager != NULL)
	{// モーション管理を破棄
		m_pMotionManager->Uninit();
	}

	// 死亡フラグを立てる
	CScene::Release();
}

//=========================================
// 更新処理
//=========================================
void CMotion::Update(void)
{
	// モーション
	Motion();
}

//=========================================
// 描画処理
//=========================================
void CMotion::Draw(void)
{
}

//=========================================
// モーション
//=========================================
void CMotion::Motion(void)
{
	CMotionManager::KEY *pKey;
	float fRateMotion;
	float fDiffMotion;
	D3DXVECTOR3 rotModel;		// 計算用向き
	D3DXVECTOR3 posModel;		// 計算用位置

	if (!m_bInit)
	{// 次のモーションへつなげる
		m_nCntFlame = m_pFrame[m_nCurKey] / 2;
		m_bInit = true;
	}

	//差分の計算
	fRateMotion = (m_pFrame[m_nCurKey] - m_nCntFlame) * 1.0f;

	//アニメーション
	for (int nCntParts = m_nFootIdx; nCntParts < m_nParts; nCntParts++)
	{
		if (m_pModel[nCntParts] != NULL)
		{// モデルが使用されている
			pKey = &m_pKey[m_nCurKey][nCntParts];

			// モデルの向きの取得
			rotModel = m_pModel[nCntParts]->GetRot();

			/*----------------------------------------------*/
			/*						X軸						*/
			/*----------------------------------------------*/
			// 角度の調節
			if (pKey->rotX > D3DX_PI) { pKey->rotX -= D3DX_PI * 2.0f; }
			if (pKey->rotX < -D3DX_PI) { pKey->rotX += D3DX_PI * 2.0f; }

			// 差分を求める
			fDiffMotion = pKey->rotX - rotModel.x;

			// 差分の調節
			if (fDiffMotion > D3DX_PI) { fDiffMotion -= D3DX_PI * 2.0f; }
			if (fDiffMotion < -D3DX_PI) { fDiffMotion += D3DX_PI * 2.0f; }

			// 現在の向きの設定
			rotModel.x += fDiffMotion / fRateMotion;

			// X目的の位置の調整
			if (rotModel.x > D3DX_PI) { rotModel.x -= D3DX_PI * 2.0f; }
			if (rotModel.x < -D3DX_PI) { rotModel.x += D3DX_PI * 2.0f; }

			/*----------------------------------------------*/
			/*						Y軸						*/
			/*----------------------------------------------*/
			// 角度の調節
			if (pKey->rotY > D3DX_PI) { pKey->rotY -= D3DX_PI * 2.0f; }
			if (pKey->rotY < -D3DX_PI) { pKey->rotY += D3DX_PI * 2.0f; }

			// 差分を求める
			fDiffMotion = pKey->rotY - rotModel.y;

			// 差分の調節
			if (fDiffMotion > D3DX_PI) { fDiffMotion -= D3DX_PI * 2.0f; }
			if (fDiffMotion < -D3DX_PI) { fDiffMotion += D3DX_PI * 2.0f; }

			// 現在の向きの設定
			rotModel.y += fDiffMotion / fRateMotion;

			// Y目的の位置の調整
			if (rotModel.y > D3DX_PI) { rotModel.y -= D3DX_PI * 2.0f; }
			if (rotModel.y < -D3DX_PI) { rotModel.y += D3DX_PI * 2.0f; }

			/*----------------------------------------------*/
			/*						Z軸						*/
			/*----------------------------------------------*/
			// 角度の調節
			if (pKey->rotZ > D3DX_PI) { pKey->rotZ -= D3DX_PI * 2.0f; }
			if (pKey->rotZ < -D3DX_PI) { pKey->rotZ += D3DX_PI * 2.0f; }

			// 差分を求める
			fDiffMotion = pKey->rotZ - rotModel.z;

			// 差分の調節
			if (fDiffMotion > D3DX_PI) { fDiffMotion -= D3DX_PI * 2.0f; }
			if (fDiffMotion < -D3DX_PI) { fDiffMotion += D3DX_PI * 2.0f; }

			// 現在の向きの設定
			rotModel.z += fDiffMotion / fRateMotion;

			// Z目的の位置の調整
			if (rotModel.z > D3DX_PI) { rotModel.z -= D3DX_PI * 2.0f; }
			else if (rotModel.z < -D3DX_PI) { rotModel.z += D3DX_PI * 2.0f; }

			// 向きの設定
			m_pModel[nCntParts]->SetRot(rotModel);
		}
	}

	/*================================================*/
	/*						位置					  */
	/*================================================*/
	posModel = m_pModel[0]->GetPos();
	pKey = &m_pKey[m_nCurKey][0];

	/*		X軸		*/
	fDiffMotion = pKey->posX - posModel.x + m_posInit.x;	// 差分を求める
	posModel.x += fDiffMotion / fRateMotion;				// 現在の向きの設定

	/*		Y軸		*/
	fDiffMotion = pKey->posY - posModel.y + m_posInit.y;	// 差分を求める
	posModel.y += fDiffMotion / fRateMotion;				// 現在の向きの設定

	/*		Z軸		*/
	fDiffMotion = pKey->posZ - posModel.z + m_posInit.z;	// 差分を求める
	posModel.z += fDiffMotion / fRateMotion;				// 現在の向きの設定

	// 位置の設定
	m_pModel[0]->SetPos(posModel);

	// 軌跡
	if (NULL != m_pOrbit && m_nCntTotalFlame == m_pOrbit->nStartFrame)
	{// 使われているとき && 開始フレーム
		CMeshOrbit::Create(D3DXVECTOR3(m_pOrbit->fPosX, m_pOrbit->fPosY, m_pOrbit->fPosZ), m_pOrbit->nEndFrame - m_pOrbit->nStartFrame, &m_pModel[m_pOrbit->nIdxModel]->GetMtxWorld());
	}

	// フレームカウント
	m_nCntFlame++;
	m_nCntTotalFlame++;

	if (m_nCntFlame % m_pFrame[m_nCurKey] == 0)
	{// キーを進める
		m_nCurKey = (1 + m_nCurKey) % m_nKey;
		m_nCntFlame = 0;

		if (m_nCurKey == 0)
		{// フレームを0に戻す
			//if (CMotionManager::TYPE_JUMP != m_nCurMotion)
			{// ジャンプでないとき
				m_nCntTotalFlame = 0;

				if (!m_bLoop)
				{// ループしない
					SetMotion(CMotionManager::TYPE_NEUTRAL);
				}
			}
		}
	}
}

//=========================================
// 攻撃当たり判定
//=========================================
bool CMotion::Collision(D3DXVECTOR3 pos)
{
	bool bHit = false;

	if (m_pCollision)
	{// 使われている
		if (m_nCntTotalFlame >= m_pCollision->nStartFrame && m_nCntTotalFlame <= m_pCollision->nEndFrame)
		{
			// 相手のXYの範囲
			float fLength = powf(m_pCollision->fRadius, 2);

			float fLengthX = pos.x - m_pModel[m_pCollision->nIdxModel]->GetMtxWorld()._41;	// Xの差
			float fLengthY = pos.y - m_pModel[m_pCollision->nIdxModel]->GetMtxWorld()._42;	// Yの差
			float fLengthZ = pos.z - m_pModel[m_pCollision->nIdxModel]->GetMtxWorld()._43;	// Zの差

			float fLengthTotal = powf(fLengthX, 2) + powf(fLengthY, 2) + powf(fLengthZ, 2);						// XYZの差の二乗

			if (fLength >= fLengthTotal)
			{// 範囲内
				bHit = true;
			}
		}
	}

	return bHit;
}

//=========================================
// 攻撃当たり判定
//=========================================
int CMotion::GetAttackIdx(void)
{
	int nIdxModel = -1;

	if (m_pCollision)
	{// 判定がある
		nIdxModel = m_pCollision->nIdxModel;
	}

	return nIdxModel;
}