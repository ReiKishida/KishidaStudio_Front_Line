//=============================================================================
//
// 敵処理 [enemy.cpp]
// Author : Takuto Ishida
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "enemy.h"
#include "scene.h"
#include "manager.h"
#include "renderer.h"
#include "model.h"
#include "motion.h"
#include "player.h"
#include "meshField.h"
#include "game.h"
#include "modelSet.h"
#include "shadow.h"
#include "life.h"
#include "sound.h"

#include <stdio.h>

#include "debugProc.h"
#include "mouseCursor.h"
#include "input.h"

//==================================
// マクロ定義
//==================================
#define ENEMY_MODEL_FILE	"data/TEXT/ENEMY/model.txt"
#define ENEMY_SPEED		(2.0f)	// 移動の速度
#define ENEMY_LIFE		(10)		// ライフ
#define ENEMY_SHOOT		(120)	// 攻撃間隔

#define ENEMY_LIFE_WIDTH	(50.0f)		// 体力バーの幅
#define ENEMY_LIFE_HEIGHT	(10.0f)		// 体力バーの高さ

#define ENEMY_WIDTH			(30.0f)		// 当たり判定の幅
#define ENEMY_DAMAGE_TIME	(30)		// ダメージ状態の時間

// モーション番号
#define ENEMY_NEUTRAL		(0)		// ニュートラル
#define ENEMY_WALK			(1)		// 歩行
#define ENEMY_JUMP			(2)		// ジャンプ
#define ENEMY_LAND			(3)		// 着地
#define ENEMY_DAMAGE		(4)		// ダメージ
#define ENEMY_ATTACK		(5)		// 攻撃

// ルート探索用
#define POINT_SIZE			(25.0f)		// 仮地点用ポリゴンの大きさ
#define PLAYER_BREAKTIME	(15)		// 休憩時間(フレーム)
#define MOUSE_ACCEPTABLE	(40.0f)		// マウスの誤差の許容範囲
#define MOVE_ACCEPTABLE		(50.0f)		// 移動の誤差の許容範囲

//==================================
// 静的メンバ変数宣言
//==================================
int				*CEnemy::m_pNumParts = {};				// パーツ数
char			***CEnemy::m_pModelName = NULL;			// モデル名
CEnemy::DATA	**CEnemy::m_pFileData = NULL;			// ファイル読み取り
int				CEnemy::m_nNumType = 0;					// 種類の数

// ルート探索用
CScene3D *CEnemy::m_pScene3D[NODE_MAX] = {};
D3DXVECTOR3 CEnemy::m_searchPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
bool CEnemy::m_bPartSwitch = false;

//==================================
// 生成処理
//==================================
CEnemy* CEnemy::Create(D3DXVECTOR3 pos, int nType)
{
	CEnemy *pEnemy;

	pEnemy = new CEnemy;	// メモリを確保

	if (pEnemy != NULL)
	{// メモリ確保成功
		pEnemy->m_pos = pos;	// 位置
		pEnemy->m_nType = nType;	// 種類
		pEnemy->Init();			// 初期化処理
	}

	return pEnemy;
}

//=========================================
// モデルの読み込み
//=========================================
HRESULT CEnemy::Load(void)
{
	// デバイスの取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	FILE *pFile;
	int nCntParts = 0;		// パーツ数のカウンタ
	int nCntModel = 0;
	m_nNumType = 1;

	// モデル数分メモリ確保
	m_pNumParts = new int[m_nNumType];
	m_pModelName = new char**[m_nNumType];
	m_pFileData = new DATA*[m_nNumType];

	for (int nCntType = 0; nCntType < m_nNumType; nCntType++)
	{
		// ファイルを開く
		pFile = fopen("data/TEXT/enemy/model.txt", "r");

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

							m_pModelName[nCntType] = new char*[nNumModel];		// メモリ確保
						}
						if (strcmp(aStr, "MODEL_FILENAME") == 0)
						{// キャラクター情報
							m_pModelName[nCntType][nCntModel] = new char[64];		// メモリ確保
							fscanf(pFile, " = %s", &m_pModelName[nCntType][nCntModel][0]);

							nCntModel++;
						}
						if (strcmp(aStr, "CHARACTERSET") == 0)
						{// キャラクター情報
							while (strcmp(aStr, "END_CHARACTERSET") != 0)
							{// キャラクター情報終了メッセージまでループ
								fscanf(pFile, "%s", aStr);

								if (strcmp(aStr, "NUM_PARTS") == 0)
								{// パーツ数
									fscanf(pFile, " = %d", &m_pNumParts[nCntType]);
									m_pFileData[nCntType] = new DATA[m_pNumParts[nCntType]];
								}
								else if (strcmp(aStr, "PARTSSET") == 0)
								{// パーツの設定
									while (strcmp(aStr, "END_PARTSSET") != 0)
									{// パーツの設定終了メッセージまでループ
										fscanf(pFile, "%s", &aStr);

										if (strcmp(aStr, "INDEX") == 0)
										{// モデル番号
											fscanf(pFile, " = %d", &m_pFileData[nCntType][nCntParts].nIdx);
										}
										else if (strcmp(aStr, "PARENT") == 0)
										{// 親を決める
											fscanf(pFile, " = %d", &m_pFileData[nCntType][nCntParts].nParent);
										}
										else if (strcmp(aStr, "POS") == 0)
										{// 位置の設定
											fscanf(pFile, " = %f %f %f", &m_pFileData[nCntType][nCntParts].pos.x, &m_pFileData[nCntType][nCntParts].pos.y, &m_pFileData[nCntType][nCntParts].pos.z);
										}
										else if (strcmp(aStr, "ROT") == 0)
										{// 向きの設定
											fscanf(pFile, " = %f %f %f", &m_pFileData[nCntType][nCntParts].rot.x, &m_pFileData[nCntType][nCntParts].rot.y, &m_pFileData[nCntType][nCntParts].rot.z);
										}
									}
									nCntParts++;	// パーツ数のカウント
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
			MessageBox(0, "テキストファイルがありません。", "ENEMY/model.txt", MB_YESNO);
		}
	}

	return S_OK;
}

//=========================================
// 読み込んだデータの解放
//=========================================
void CEnemy::Unload(void)
{
	if (NULL != m_pFileData)
	{
		for (int nCntType = 0; nCntType < m_nNumType; nCntType++)
		{// 種類分ループ
			if (NULL != m_pFileData[nCntType])
			{// メモリの開放
				delete[] m_pFileData[nCntType];
				m_pFileData[nCntType] = NULL;
			}
		}

		if (NULL != m_pFileData)
		{// メモリの開放
			delete[] m_pFileData;
			m_pFileData = NULL;
		}
	}

	if (NULL != m_pModelName)
	{// パーツ数の破棄
		for (int nCntType = 0; nCntType < m_nNumType; nCntType++)
		{// 種類分ループ
			for (int nCntParts = 0; nCntParts < m_pNumParts[nCntType]; nCntParts++)
			{
				if (NULL != m_pModelName[nCntType][nCntParts])
				{// 破棄
					delete[] m_pModelName[nCntType][nCntParts];
					m_pModelName[nCntType][nCntParts] = NULL;
				}
			}

			if (NULL != m_pModelName[nCntType])
			{// 破棄
				delete[] m_pModelName[nCntType];
				m_pModelName[nCntType] = NULL;
			}
		}
		if (NULL != m_pModelName)
		{// 破棄
			delete[] m_pModelName;
			m_pModelName = NULL;
		}
	}

	if (NULL != m_pNumParts)
	{// パーツ数の破棄
		delete[] m_pNumParts;
		m_pNumParts = NULL;
	}
}

//=========================================
// コンストラクタ
//=========================================
CEnemy::CEnemy(int nPriority, CScene::OBJTYPE objType) : CScene(nPriority, objType)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nCntBullet = 0;
	m_pShadow = NULL;
	m_fAttackRange = 0.0f;
	m_fWalkRange = 0.0f;
	m_bAttack = false;
	m_nIntervalAttack = 0;
	m_fRotDest = 0.0f;
	m_pLife = NULL;
	m_fAlpha = 0.0f;

	for (int nCntDamage = 0; nCntDamage < ATTACK_MOTION; nCntDamage++)
	{// 攻撃モーション分ループ
		m_nCntDamage[nCntDamage] = 0;
		m_bDamage[nCntDamage] = false;
	}
}

//=========================================
// デストラクタ
//=========================================
CEnemy::~CEnemy(){}

//=========================================
// 初期化処理
//=========================================
HRESULT CEnemy::Init(void)
{
	if (NULL == m_pModel)
	{// モデルクラスの生成
		m_pModel = new CModel*[m_pNumParts[m_nType]];	// パーツ分のメモリを確保

		for (int nCntModel = 0; nCntModel < m_pNumParts[m_nType]; nCntModel++)
		{// メモリの確保
			m_pModel[nCntModel] = CModel::Create(&m_mtxWorld);

			// モデル情報の受け渡し
			m_pModel[nCntModel]->SetModel(m_pModelName[m_nType][nCntModel]);

			if (m_pFileData[m_nType][nCntModel].nParent >= 0)
			{// 親のポインタを設定
				m_pModel[nCntModel]->SetParent(m_pModel[m_pFileData[m_nType][nCntModel].nParent]);
			}
			else
			{// 親がいない場合
				m_pModel[nCntModel]->SetParent(NULL);
			}

			m_pModel[nCntModel]->Init();
			m_pModel[nCntModel]->SetPos(m_pFileData[m_nType][nCntModel].pos);
			m_pModel[nCntModel]->SetRot(m_pFileData[m_nType][nCntModel].rot);
		}
	}

	if (NULL == m_pMotion)
	{// モーションクラスの生成
		m_pMotion = CMotion::Create(1, m_pNumParts[m_nType], &m_pModel[m_nType]);
		m_pMotion->SetMotion(CMotionManager::TYPE_NEUTRAL);		// ニュートラルモーションを設定
	}

	// 攻撃判定の範囲
	m_fAttackRange = 60.0f;
	m_fWalkRange = 400.0f;

	// =============================================================
	// ダイクストラ法によるルート探索
	// =============================================================
	FileLoad("data/TEXT/point.txt");	// ファイル読み込み

	// データの初期化
	m_nBreaktime = 0;
	m_nCountPoint = 0;
	m_nPoint = 0;
	m_bGoal = false;
	m_bRally = false;
	m_nRallyCount = 0;
	m_nRallyCountOld = 0;
	m_nNodeOld = 0;

	m_searchPos = m_pos;

	// 開始時点のノードの初期化
	for (int nCntNode = 0; nCntNode < m_RootData.nodeMax; nCntNode++)
	{// ノードの数だけ回る
		if (m_RootData.pos[nCntNode].x + MOUSE_ACCEPTABLE  > m_pos.x
			&& m_RootData.pos[nCntNode].x - MOUSE_ACCEPTABLE < m_pos.x
			&& m_RootData.pos[nCntNode].z + MOUSE_ACCEPTABLE > m_pos.z
			&& m_RootData.pos[nCntNode].z - MOUSE_ACCEPTABLE < m_pos.z)
		{// 誤差が+-10以内
			m_nStartNode = nCntNode;
			m_nEndNode = nCntNode;
			m_nRallyEndNode[nCntNode] = nCntNode;
		}

		// 地点系の初期化
		m_waypoint[nCntNode] = m_RootData.pos[m_nStartNode];
	}


	// 地点可視化用
	for (int nCntNode = 0; nCntNode < m_RootData.nodeMax; nCntNode++)
	{// 全ノードの数だけ回る
		if (m_pScene3D[nCntNode] == NULL)
		{// 3Dポリゴンの生成
			m_pScene3D[nCntNode] = m_pScene3D[nCntNode]->Create();
			if (m_pScene3D != NULL)
			{// 3Dポリゴンの設定
				m_RootData.pos[nCntNode].y += 0.1f;
				m_pScene3D[nCntNode]->SetPos(m_RootData.pos[nCntNode]);
				m_pScene3D[nCntNode]->SetWidth(POINT_SIZE);
				m_pScene3D[nCntNode]->SetDepth(POINT_SIZE);
				m_pScene3D[nCntNode]->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			}
		}
	}

	// =============================================================

	return S_OK;
}

//=========================================
// 終了処理
//=========================================
void CEnemy::Uninit(void)
{
	if (m_pScene3D != NULL)
	{
		for (int nCntNode = 0; nCntNode < NODE_MAX; nCntNode++)
		{
			if (m_pScene3D[nCntNode] != NULL)
			{
				m_pScene3D[nCntNode]->Uninit();
				m_pScene3D[nCntNode] = NULL;
			}
		}
	}

	if (m_pShadow != NULL)
	{// 影の破棄
		m_pShadow->Uninit();
		m_pShadow = NULL;
	}

	if (NULL != m_pLife)
	{// 体力クラスの破棄
		m_pLife->Uninit();
		m_pLife = NULL;
	}

	if (m_pMotion != NULL)
	{// モーションクラスの破棄
		m_pMotion->Uninit();
		m_pMotion = NULL;
	}

	for (int nCntType = 0; nCntType < m_nNumType; nCntType++)
	{
		for (int nCntModel = 0; nCntModel < m_pNumParts[nCntType]; nCntModel++)
		{// モデルの破棄
			if (NULL != m_pModel)
			{
				m_pModel[nCntModel]->Uninit();
				delete m_pModel[nCntModel];
				m_pModel[nCntModel] = NULL;
			}
		}
	}

	if (NULL != m_pModel)
	{// メモリの開放
		delete m_pModel;
		m_pModel = NULL;
	}

	// 自分を破棄
	CScene::Release();
}

//=========================================
// 更新処理
//=========================================
void CEnemy::Update(void)
{
	CScene *pScene = NULL;	// オブジェクト管理クラスのポインタ変数

	AIUpdate();

	// 目標の方向の修正
	if (m_fRotDest < -D3DX_PI)
	{
		m_fRotDest += D3DX_PI * 2.0f;
	}
	if (m_fRotDest > D3DX_PI)
	{
		m_fRotDest -= D3DX_PI * 2.0f;
	}

	m_pos += m_move;
	m_rot.y += m_fRotDest * 0.1f;

	m_move.x += (0.0f - m_move.x) * 0.2f;
	m_move.z += (0.0f - m_move.z) * 0.2f;

	// 方向の修正
	if (m_rot.y < -D3DX_PI)
	{
		m_rot.y += D3DX_PI * 2.0f;
	}
	if (m_rot.y > D3DX_PI)
	{
		m_rot.y -= D3DX_PI * 2.0f;
	}

	// 目標の方向の初期化
	m_fRotDest = 0.0f;

	// 重力
	m_move.y -= GRAVITY;

	float fHeight = 0.0f;	// 起伏の高さを求める

	// 地面オブジェクトを探す
	pScene = CScene::GetSceneTop(MESHFIELD_PRIORITY);

	while (pScene != NULL)
	{// NULLになるまでループ
		CScene *pSceneNext = pScene->GetSceneNext();
		CScene::OBJTYPE objType = pScene->GetObjType();

		if (objType == CScene::OBJTYPE_FIELD)
		{// 地面だったとき
			CMeshField *pMeshField = (CMeshField*)pScene;	// 地面のポインタを取得

			fHeight = pMeshField->GetHeight(m_pos);			// 地面の高さの取得
		}

		// 次のオブジェクトを見る
		pScene = pSceneNext;
	}

	if (m_pos.y <= fHeight)
	{// 地面以下になったとき
		m_pos.y = fHeight;	// 高さを座標に入れる
		m_move.y = 0.0f;	// Y軸の移動量を無くす
	}
}

//=========================================
// 描画処理
//=========================================
void CEnemy::Draw(void)
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

	for (int nCntModel = 0; nCntModel < m_pNumParts[0]; nCntModel++)
	{// パーツモデルの描画
		if (m_bDeath)
		{// 死亡状態
			m_pModel[nCntModel]->AddColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, -m_fAlpha));
		}
		for (int nCntDamage = 0; nCntDamage < ATTACK_MOTION; nCntDamage++)
		{// 攻撃モーション分ループ
			if (m_bDamage[nCntDamage])
			{// ダメージ状態
				m_pModel[nCntModel]->AddColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.0f));
				break;
			}
		}

		// モデルの描画
		m_pModel[nCntModel]->Draw();

		if (m_bDeath)
		{// 死亡状態
			m_pModel[nCntModel]->AddColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, m_fAlpha));
		}
		for (int nCntDamage = 0; nCntDamage < ATTACK_MOTION; nCntDamage++)
		{// 攻撃モーション分ループ
			if (m_bDamage[nCntDamage])
			{// ダメージ状態
				m_pModel[nCntModel]->AddColor(D3DXCOLOR(-1.0f, 0.0f, 0.0f, 0.0f));
				break;
			}
		}
	}
}

//=========================================
// 当たり判定
//=========================================
bool CEnemy::Collision(D3DXVECTOR3 pos, float fRange)
{
	bool bHit = false;

	// 相手のXYの範囲
	float fLength = powf(fRange, 2);

	float fLengthX = pos.x - m_pos.x;						// Xの差
	float fLengthY = pos.y - m_pos.y;						// Yの差
	float fLengthTotal = powf(fLengthX, 2) + powf(fLengthY, 2);		// XYの差の二乗

	if (fLength >= fLengthTotal)
	{// 相手と自分の判定
		if (fLengthX > 0)
		{// 右側にいる
			m_pos.x += fLengthX - fRange;
		}
		else if(fLengthX < 0)
		{// 左側にいる
			m_pos.x -= fabsf(fLengthX) - fRange;
		}
		bHit = true;	// 判定を有効にする
	}

	return bHit;	// 判定結果を返す
}

//=========================================
// モデル情報の受け渡し
//=========================================
void CEnemy::GetModelData(CModel**& pModel)
{
	m_pModel = pModel;
}

//=========================================
// ダメージを受けたときの処理
//=========================================
void CEnemy::Damage(int nDamage, int nMotion)
{
	if (0 < m_pLife->GetLife())
	{
		if (nMotion < ATTACK_MOTION && nMotion >= 0)
		{// 配列を越さないようにする
			if (NULL != m_pLife && !m_bDamage[nMotion])
			{// ライフクラスが使われている
				m_pLife->AddSubtract(nDamage);

				// ダメージ状態にする
				m_pMotion->SetMotion(CMotionManager::TYPE_DAMAGE);
				m_bDamage[nMotion] = true;
				CSound *pSound = CManager::GetSound();			// サウンドの取得
				//pSound->PlaySound(CSound::SOUND_LABEL_HIT);		// 攻撃ヒット音の再生

				if (0 >= m_pLife->GetLife())
				{// 体力が0以下になった
					m_bDeath = true;
					m_nCntDamage[nMotion] = 0;
					//pSound->PlaySound(CSound::SOUND_LABEL_ENEMYDEATH);	// 敵死亡音の再生
					m_move.y = 10.0f;

					if (m_pMotion)
					{// 死亡モーション
						m_pMotion->SetMotion(CMotionManager::TYPE_DEATH);
					}
				}
			}
		}
		else if (-1 == nMotion)
		{// モーションが-1のとき
			CSound *pSound = CManager::GetSound();			// サウンドの取得
			//pSound->PlaySound(CSound::SOUND_LABEL_HIT);		// 攻撃ヒット音の再生

			m_pLife->AddSubtract(nDamage);	// ライフを0にする

			if (0 >= m_pLife->GetLife())
			{// 体力が0以下になった
				m_bDeath = true;
				//pSound->PlaySound(CSound::SOUND_LABEL_ENEMYDEATH);	// 敵死亡音の再生

				m_move.y = 15.0f;

				if (m_pMotion)
				{// 死亡モーション
					m_pMotion->SetMotion(CMotionManager::TYPE_DEATH);
				}
			}
		}
	}
}

//=============================================================================
//	AI更新処理
//=============================================================================
void CEnemy::AIUpdate()
{
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();	// キーボードの入力を取得
	CInputMouse *pMouse = CManager::GetInputMouse();	// マウスの入力を取得
	int nCntRally = 0;

	// パート情報	ストラテジー：true	アクション：false
	//CDebugProc::Print(" 現在のパート : %s\n\n", m_bPartSwitch ? "ストラテジー" : "アクション");

	// ラリー情報
	//CDebugProc::Print(" ラリー状態 : %s\n\n", m_bRally ? "ON" : "OFF");
	//CDebugProc::Print(" クリック数 : %d\n", m_nRallyCount);

	// ノード情報
	//CDebugProc::Print(" ノードの数 : %d\n", m_RootData.nodeMax);
	//CDebugProc::Print(" ゴールに到着%s。\n\n", m_bGoal ? "しました" : "してません");

	if (!m_bRally)
	{// ラリー状態じゃない
		//CDebugProc::Print(" 開始地点 : %d\n", m_nStartNode);
		//CDebugProc::Print(" 終了地点 : %d\n\n", m_nEndNode);
	}
	else
	{// ラリー時
		//CDebugProc::Print(" 開始地点 : %d\n", m_nRallyEndNode[0]);
		for (nCntRally = 1; nCntRally < m_nRallyCount; nCntRally++)
		{
			//CDebugProc::Print(" 中間地点[%d] : %d\n", nCntRally, m_nRallyEndNode[nCntRally]);
		}
		//CDebugProc::Print(" 終了地点 : %d\n\n", m_nRallyEndNode[m_nRallyCount]);
	}

	// エッジ情報
	//CDebugProc::Print(" エッジの数 : %d\n", m_RootData.edgeMax);

	// エネミー情報
	//CDebugProc::Print("=======エネミーの位置========\n");
	//CDebugProc::Print("  x : %.1f\n  y : %.1f\n  z : %.1f\n", m_pos.x, m_pos.y, m_pos.z);
	//CDebugProc::Print("=============================\n\n");

	// 自動移動情報
	//CDebugProc::Print("========自動移動関係========\n");
	//CDebugProc::Print(" 自動移動目標地点\n  x : %.1f\n  y : %.1f\n  z : %.1f\n", m_posDest.x, m_posDest.y, m_posDest.z);
	////CDebugProc::Print(" 休憩時間 : %.1f\n", m_fBreaktime);
	//CDebugProc::Print(" 目標までの移動回数 : %d\n", m_nCountPoint);
	//CDebugProc::Print(" 現在の移動回数 : %d\n", m_nPoint);
	//CDebugProc::Print("=============================\n\n");

	if (pMouse->GetTrigger(CInputMouse::DIMS_BUTTON_1))
	{// 右クリック
		// Part切り替え
		m_bPartSwitch = m_bPartSwitch ? false : true;
		m_bGoal = false;
	}

	if (pMouse->GetTrigger(CInputMouse::DIMS_BUTTON_3))
	{// マウスリターン
		// ラリー状態の切り替え
		m_bRally = m_bRally ? false : true;
	}

	if (!m_bPartSwitch)
	{// アクションパート
		m_nRallyCount = 0;
		// 自動移動処理
		AutoMove();
	}
	else
	{// ストラテジーパート
		m_nPoint = 0;
		m_nCountPoint = -1;
		NodeSearch(false);

		if (!m_bRally)
		{// ラリー状態じゃない
			RootSearch();
		}
		else
		{// ラリー時
			RallyRootSearch();
		}
	}

	if (m_bGoal == true)
	{// 目標地点到着時
		for (int nCntNode = 0; nCntNode < m_RootData.nodeMax; nCntNode++)
		{// ノードの数だけ回る
			if (m_pScene3D != NULL)
			{// 3Dポリゴンの設定
				m_pScene3D[nCntNode]->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			}
		}
	}
}

//=============================================================================
//	ノード検索処理
//=============================================================================
void CEnemy::NodeSearch(bool node)
{
	CInputMouse *pMouse = CManager::GetInputMouse();	// マウスの入力を取得

	for (int nCntNode = 0; nCntNode < m_RootData.nodeMax; nCntNode++)
	{// ノードの数だけ回る
		if (m_RootData.pos[nCntNode].x + MOUSE_ACCEPTABLE > m_searchPos.x
			&& m_RootData.pos[nCntNode].x - MOUSE_ACCEPTABLE < m_searchPos.x
			&& m_RootData.pos[nCntNode].z + MOUSE_ACCEPTABLE > m_searchPos.z
			&& m_RootData.pos[nCntNode].z - MOUSE_ACCEPTABLE < m_searchPos.z)
		{// 誤差が許容範囲内
			if (m_pScene3D[nCntNode] != NULL)
			{// 3Dポリゴンの設定
				if (m_nRallyCountOld == m_nRallyCount)
				{// 置き直し
					m_pScene3D[m_nNodeOld]->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
				}

				switch (m_nRallyCount)
				{// 番号ごとに色を変える
				case 1:
					m_pScene3D[nCntNode]->SetColor(D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
					break;
				}
			}

			// 前回の情報を保存
			m_nNodeOld = nCntNode;
			m_nRallyCountOld = m_nRallyCount;

			if (node)
			{// 開始ノードを設定する
				m_nStartNode = nCntNode;
			}
			else if (!node)
			{// 終了ノードを設定する
				m_nRallyEndNode[m_nRallyCount] = nCntNode;

				if (m_nRallyCount > 0)
				{// 1回以上クリックしている
					m_nEndNode = nCntNode;
				}

				if (pMouse->GetTrigger(CInputMouse::DIMS_BUTTON_0))
				{// 左クリック時
					if (m_bRally)
					{// ラリー時
						m_nRallyCount++;
					}
					else
					{// ラリー状態じゃない
						m_nRallyCount = 1;
					}
				}
			}
		}
	}
}

//=============================================================================
//	自動移動処理
//=============================================================================
void CEnemy::AutoMove()
{
	CMotionManager::TYPE type = CMotionManager::TYPE_NEUTRAL;		// モーションの種類

	bool bMoveKey = false;	// 移動キー押下フラグ

	// 目標地点を設定
	m_posDest = m_waypoint[m_nPoint];

	// 目的との差分を出す
	float fLength = (m_pos.x - m_posDest.x) * (m_pos.x - m_posDest.x) + (m_pos.z - m_posDest.z) * (m_pos.z - m_posDest.z);
	m_nBreaktime--;

	if (fLength > MOVE_ACCEPTABLE)
	{// 差分が許容値内に収まるまで目的地に移動する
		bMoveKey = true;
		m_move.x = sinf(atan2f(m_posDest.x - m_pos.x, m_posDest.z - m_pos.z)) * ENEMY_SPEED;
		m_move.z = cosf(atan2f(m_posDest.x - m_pos.x, m_posDest.z - m_pos.z)) * ENEMY_SPEED;
		m_rot.y = atan2f(m_posDest.x - m_pos.x, m_posDest.z - m_pos.z) + D3DX_PI;
	}
	else if (m_nBreaktime < 0)
	{// 移動後休憩
		bMoveKey = false;
		m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_nBreaktime = PLAYER_BREAKTIME;

		if (m_nCountPoint != 0 && m_nCountPoint == m_nPoint)
		{// 終了ノードに到着
			m_bGoal = true;
		}
	}
	else if (m_nBreaktime == 0 && m_nCountPoint > m_nPoint)
	{// 休憩終了
		m_nPoint++;
		if (m_nCountPoint == m_nPoint)
		{// 終点を次の開始点にする
			NodeSearch(true);
		}
	}

	if (m_pMotion)
	{// 使われている
		if (type != CMotionManager::TYPE_ATTACK01 && type != CMotionManager::TYPE_DAMAGE)
		{// 攻撃モーションでない時
			if (m_move.x > 0.1f || m_move.x < -0.1f || m_move.z > 0.1f || m_move.z < -0.1f)
			{// 移動モーションを設定
				m_pMotion->SetMotion(CMotionManager::TYPE_WALK);		// モーションの設定
			}
			else
			{// ニュートラルモーションを設定
				m_pMotion->SetMotion(CMotionManager::TYPE_NEUTRAL);		// モーションの設定
			}
		}
	}

	// 位置の更新
	m_pos.x += m_move.x;
	m_pos.z += m_move.z;
}

//=============================================================================
//	ルート検索処理
//=============================================================================
void CEnemy::RootSearch()
{
	Node node[NODE_MAX];	// ノードの情報
	float weight[NODE_MAX];	// 各エッジのコスト
	int nCntWeight = 0;		// コストのカウンタ
	std::vector<int> path;	// 最短経路の情報を保持するvector

	//======= エッジコストの算出 =========================================================================
	for (int nCntNode = 0; nCntNode < m_RootData.nodeMax; nCntNode++, nCntWeight++)
	{// ノードの数だけ回る
		weight[nCntWeight] = sqrt((m_RootData.pos[m_nStartNode].x - m_RootData.pos[nCntNode].x) * (m_RootData.pos[m_nStartNode].x - m_RootData.pos[nCntNode].x) + (m_RootData.pos[m_nStartNode].z - m_RootData.pos[nCntNode].z) * (m_RootData.pos[m_nStartNode].z - m_RootData.pos[nCntNode].z));
	}

	//======= コスト表の出力 =========================================================================
	//CDebugProc::Print("========移動コスト表========\n");
	for (int nCntNode = 0; nCntNode < m_RootData.nodeMax; nCntNode++)
	{// ノードの数だけ回る
		//CDebugProc::Print(" No.%d : %.1f\n", nCntNode, weight[nCntNode]);
	}
	//CDebugProc::Print("=============================\n\n");

	//======= エッジ追加 =========================================================================
	for (int nCntNode = 0; nCntNode < m_RootData.nodeMax; nCntNode++)
	{// ノードの数だけ回る
		for (int nCntConnect = 0; nCntConnect < m_RootData.connectNum[nCntNode]; nCntConnect++)
		{// 繋がってるノードの数だけ回る
			AddEdge(nCntNode, m_RootData.connectIndex[nCntNode][nCntConnect], weight[nCntNode], node);
		}
	}

	//======= 最短経路を調べる =========================================================================
	Dijkstra(m_RootData.nodeMax, m_nStartNode, m_nEndNode, node);

	for (int nCntNode = m_nEndNode; nCntNode != m_nStartNode; nCntNode = node[nCntNode].from)
	{// 最短経路をゴールから順にスタートまでたどる
		path.push_back(nCntNode);
	}
	path.push_back(m_nStartNode);

	//======= 最短経路の出力 =========================================================================
	//CDebugProc::Print(" 最短経路は【");
	for (int nCntNode = path.size() - 1; nCntNode >= 0; nCntNode--)
	{
		//CDebugProc::Print(" %d ", path[nCntNode]);
		if (m_nCountPoint < nCntNode)
		{// 最大値を代入
			m_nCountPoint = nCntNode;
		}
	}
	//CDebugProc::Print("】の順です。\n\n");

	//======= 目標地点の設定 =========================================================================
	for (int nCntNodeMax = 0; nCntNodeMax < m_RootData.nodeMax; )
	{// ノードの数だけ回る
		for (int nCntNode = path.size() - 1; nCntNode >= 0; nCntNode--, nCntNodeMax++)
		{
			m_waypoint[nCntNodeMax] = m_RootData.pos[path[nCntNode]];
		}
	}
}

//=============================================================================
//	ラリールート検索処理
//=============================================================================
void CEnemy::RallyRootSearch()
{
	float weight[NODE_MAX];	// 各エッジのコスト
	int nCntWeight = 0;		// コストのカウンタ
	std::vector<int> path;	// 最短経路の情報を保持するvector

	//======= エッジコストの算出 =========================================================================
	for (int nCntNode = 0; nCntNode < m_RootData.nodeMax; nCntNode++, nCntWeight++)
	{// ノードの数だけ回る
		weight[nCntWeight] = sqrt(
			(m_RootData.pos[m_nRallyEndNode[m_nRallyCount - 1]].x - m_RootData.pos[nCntNode].x) *
			(m_RootData.pos[m_nRallyEndNode[m_nRallyCount - 1]].x - m_RootData.pos[nCntNode].x) +
			(m_RootData.pos[m_nRallyEndNode[m_nRallyCount - 1]].z - m_RootData.pos[nCntNode].z) *
			(m_RootData.pos[m_nRallyEndNode[m_nRallyCount - 1]].z - m_RootData.pos[nCntNode].z));
	}

	//======= コスト表の出力 =========================================================================
	//CDebugProc::Print("========移動コスト表========\n");
	for (int nCntNode = 0; nCntNode < m_RootData.nodeMax; nCntNode++)
	{// ノードの数だけ回る
		//CDebugProc::Print(" No.%d : %.1f\n", nCntNode, weight[nCntNode]);
	}
	//CDebugProc::Print("=============================\n\n");

	//======= エッジ追加 =========================================================================
	for (int nCntNode = 0; nCntNode < m_RootData.nodeMax; nCntNode++)
	{// ノードの数だけ回る
		for (int nCntConnect = 0; nCntConnect < m_RootData.connectNum[nCntNode]; nCntConnect++)
		{// 繋がってるノードの数だけ回る
			AddEdge(nCntNode, m_RootData.connectIndex[nCntNode][nCntConnect], weight[nCntNode], m_node[m_nRallyCount]);
		}
	}

	//======= 最短経路を調べる =========================================================================
	if (m_nRallyCount > 1)
	{// 2ポイント目以降
		for (int nCntRally = 1; nCntRally < m_nRallyCount; nCntRally++)
		{// 2ポイント以降の数だけ回る
			// ダイクストラ法で最短経路を求める
			Dijkstra(m_RootData.nodeMax, m_nRallyEndNode[m_nRallyCount - nCntRally], m_nRallyEndNode[m_nRallyCount - (nCntRally - 1)], m_node[m_nRallyCount - (nCntRally - 1)]);

			for (int nCntNode = m_nRallyEndNode[m_nRallyCount - (nCntRally - 1)]; nCntNode != m_nRallyEndNode[m_nRallyCount - nCntRally]; nCntNode = m_node[m_nRallyCount - (nCntRally - 1)][nCntNode].from)
			{// 最短経路をゴールから順にスタートまでたどる
				path.push_back(nCntNode);
			}
			path.push_back(m_nRallyEndNode[m_nRallyCount - nCntRally]);
		}
	}

	if (m_nRallyCount > 0)
	{// 1ポイント目
		// ダイクストラ法で最短経路を求める
		Dijkstra(m_RootData.nodeMax, m_nStartNode, m_nRallyEndNode[m_nRallyCount - (m_nRallyCount - 1)], m_node[m_nRallyCount - (m_nRallyCount - 1)]);

		for (int nCntNode = m_nRallyEndNode[m_nRallyCount - (m_nRallyCount - 1)]; nCntNode != m_nStartNode; nCntNode = m_node[m_nRallyCount - (m_nRallyCount - 1)][nCntNode].from)
		{// 最短経路をゴールから順にスタートまでたどる
			path.push_back(nCntNode);
		}
		path.push_back(m_nStartNode);
	}

	if (m_nRallyCount == 0)
	{// スタート位置
		//ダイクストラ法で最短経路を求める
		Dijkstra(m_RootData.nodeMax, m_nStartNode, m_nRallyEndNode[m_nRallyCount], m_node[m_nRallyCount]);

		for (int nCntNode = m_nRallyEndNode[m_nRallyCount]; nCntNode != m_nStartNode; nCntNode = m_node[m_nRallyCount][nCntNode].from)
		{// 最短経路をゴールから順にスタートまでたどる
			path.push_back(nCntNode);
		}
		path.push_back(m_nStartNode);
	}

	//======= 最短経路の出力 =========================================================================
	//CDebugProc::Print(" 最短経路は【");
	for (int nCntNode = path.size() - 1; nCntNode >= 0; nCntNode--)
	{
		//CDebugProc::Print(" %d ", path[nCntNode]);
		if (m_nCountPoint < nCntNode)
		{// 最大値を代入
			m_nCountPoint = nCntNode;
		}
	}
	//CDebugProc::Print("】の順です。\n\n");

	//======= 目標地点の設定 =========================================================================
	for (int nCntNodeMax = 0; nCntNodeMax < m_RootData.nodeMax; )
	{// ノードの数だけ回る
		for (int nCntNode = path.size() - 1; nCntNode >= 0; nCntNode--, nCntNodeMax++)
		{// ゴールから辿る
			m_waypoint[nCntNodeMax] = m_RootData.pos[path[nCntNode]];
		}
	}
}

//=============================================================================
// エッジ追加処理
//=============================================================================
void CEnemy::AddEdge(int first, int second, float weight, Node *node)
{
	// ノードuはノードvとつながっている情報を入れる
	node[second].to.push_back(first);
	// ノードuとノードvのエッジの重みを入れる
	node[second].cost.push_back(weight);
	//// ノードvはノードuとつながっている情報を入れる
	//node[second].to.push_back(first);
	//// ノードvとノードuのエッジの重みを入れる
	//node[second].cost.push_back(weight);
}

//=============================================================================
// ダイクストラ法
//=============================================================================
void CEnemy::Dijkstra(int nodeMax, int start, int end, Node *node)
{
	// 変数の初期化
	for (int nCntNode = 0; nCntNode < nodeMax; nCntNode++)
	{
		node[nCntNode].done = false;
		node[nCntNode].minCost = -1;
	}

	node[start].minCost = 0;	// スタートノードまでのコストは0
	while (1)
	{
		int doneNode = -1;	// 最新の確定したノード番号(-1はNULLのかわり)
		for (int nCntNode = 0; nCntNode < nodeMax; nCntNode++)
		{
			if (node[nCntNode].done == true)
			{// ノードiが確定しているときcontinue
				continue;
			}

			if (node[nCntNode].minCost < 0)
			{// ノードiまでの現時点での最小コストが不明のとき
				continue;
			}

			// 確定したノード番号が-1かノードiの現時点の最小コストが小さいとき
			// 確定ノード番号を更新する
			if (doneNode < 0 || node[nCntNode].minCost < node[doneNode].minCost)
			{
				doneNode = nCntNode;
			}
		}

		if (doneNode == -1)
		{
			break;	// すべてのノードが確定したら終了
		}

		node[doneNode].done = true;	// ノードを確定させる

		for (int nCntNodeSize = 0; nCntNodeSize < (int)node[doneNode].to.size(); nCntNodeSize++)
		{
			int to = node[doneNode].to[nCntNodeSize];
			float cost = node[doneNode].minCost + node[doneNode].cost[nCntNodeSize];

			// ノードtoはまだ訪れていないノード
			// またはノードtoへより小さいコストの経路だったら
			// ノードtoの最小コストを更新
			if (node[to].minCost < 0.0f || cost < node[to].minCost)
			{
				node[to].minCost = cost;
				node[to].from = doneNode;
			}
		}
	}
}

//=============================================================================
// ルート探索用ファイルの読み込み
//=============================================================================
void CEnemy::FileLoad(char* pFileName)
{
	FILE* pFile = NULL;		// ファイルポインタ
	char ReadText[256];		// 読み込んだ文字列を入れておく
	char HeadText[256];		// 比較用
	char DustBox[256];		// 使用しないものを入れておく
	int nCount = 0;
	int nCntIndex = 0;

	// 一時データベース
	std::vector<NodeState> LoadState; LoadState.clear();

	// 初期化
	NodeState OneState = {};

	// ファイルオープン
	pFile = fopen(pFileName, "r");

	if (pFile != NULL)
	{// ファイルが開かれていれば
		while (strcmp(HeadText, "START_LOAD") != 0)
		{// "START_LOAD" が読み込まれるまで繰り返し文字列を読み取る
			fgets(ReadText, sizeof(ReadText), pFile);
			sscanf(ReadText, "%s", &HeadText);
		}
		if (strcmp(HeadText, "START_LOAD") == 0)
		{// "START_LOAD" が読み取れた場合、処理開始
			while (strcmp(HeadText, "END_LOAD") != 0)
			{// "END_LOAD" が読み込まれるまで繰り返し文字列を読み取る
				fgets(ReadText, sizeof(ReadText), pFile);
				sscanf(ReadText, "%s", &HeadText);

				if (strcmp(HeadText, "\n") == 0)
				{// 文字列の先頭が [\n](改行) の場合処理しない

				}
				else if (strcmp(HeadText, "NUM_NODE") == 0)	// ノードの総数
				{
					sscanf(ReadText, "%s %c %d",
						&DustBox, &DustBox,
						&OneState.nodeMax);
				}
				else if (strcmp(HeadText, "NUM_EDGE") == 0)	// エッジの総数
				{
					sscanf(ReadText, "%s %c %d",
						&DustBox, &DustBox,
						&OneState.edgeMax);
				}
				else if (strcmp(HeadText, "NODE_POS") == 0)	// ノードの位置
				{
					sscanf(ReadText, "%s %c %f %f %f",
						&DustBox, &DustBox,
						&OneState.pos[nCount].x,
						&OneState.pos[nCount].y,
						&OneState.pos[nCount].z);
					nCount++;
				}
				else if (strcmp(HeadText, "START_DATA") == 0)
				{// "START_DATA" が読み取れた場合
					nCount = 0;
					while (strcmp(HeadText, "END_DATA") != 0)
					{// "END_DATA" が読み込まれるまで繰り返し文字列を読み取る
						fgets(ReadText, sizeof(ReadText), pFile);
						sscanf(ReadText, "%s", &HeadText);

						if (strcmp(HeadText, "\n") == 0)
						{// 文字列の先頭が [\n](改行) の場合処理しない

						}
						else if (strcmp(HeadText, "NODESET") == 0)
						{// "NODESET" が読み取れた場合
							while (strcmp(HeadText, "END_NODESET") != 0)
							{// "END_NODESET" が読み込まれるまで繰り返し文字列を読み取る
								fgets(ReadText, sizeof(ReadText), pFile);
								sscanf(ReadText, "%s", &HeadText);

								if (strcmp(HeadText, "\n") == 0)
								{// 文字列の先頭が [\n](改行) の場合処理しない

								}
								else if (strcmp(HeadText, "INDEX") == 0)	// ノード番号
								{
									sscanf(ReadText, "%s %c %d",
										&DustBox, &DustBox,
										&OneState.index[nCount]);
								}
								else if (strcmp(HeadText, "CONNECT_NUM") == 0)	// 接続ノードの数
								{
									sscanf(ReadText, "%s %c %d",
										&DustBox, &DustBox,
										&OneState.connectNum[nCount]);
								}
								else if (strcmp(HeadText, "CONNECT_INDEX") == 0)	// 接続ノードの番号
								{
									sscanf(ReadText, "%s %c %d",
										&DustBox, &DustBox,
										&OneState.connectIndex[nCount][nCntIndex]);
									nCntIndex++;
								}
							}
							nCntIndex = 0;
							nCount++;
						}
					}

					// 一つのデータを読み込んだ後,一時データベースに格納
					LoadState.emplace_back(OneState);
				}
			}
		}

		// ファイルクローズ
		if (pFile != NULL)
		{
			fclose(pFile);
			pFile = NULL;
		}
	}

	m_RootData = OneState;	// データの代入
}