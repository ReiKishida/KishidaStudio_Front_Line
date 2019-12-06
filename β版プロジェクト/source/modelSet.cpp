//=============================================================================
//
// 3Dモデルオブジェクト処理 [modelSet.h]
// Author : Ishida Takuto
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "modelSet.h"
#include "input.h"
#include "manager.h"
#include "renderer.h"
#include "debugProc.h"
#include "meshField.h"
#include "scene3D.h"
#include "shadow.h"
#include "particle.h"
#include "player.h"
#include "camera.h"

#include <stdio.h>

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MODEL_TEXT			"data/TEXT/MODEL/model_object.txt"		// オブジェクトの配置情報テキスト
#define WEED_MOVE			(1.0f)

#define LOG_SLIP_ANGLE		(0.8f)		// 滑るようになる角度
#define LOG_CLIMB_SPEED		(1.5f)		// 登るときの速度の減衰量
#define LOG_SLIP_SPEED		(4.0f)		// 滑る速度
#define LOG_SLIP_JUMP		(1.0f)		// 吹っ飛びジャンプ速度補正

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9	*CModelSetManager::m_pTexture[NUM_MODELINFO] = {};	// テクスチャ情報
LPD3DXMESH			*CModelSetManager::m_pMesh[NUM_MODELINFO] = {};		// メッシュ情報
LPD3DXBUFFER		*CModelSetManager::m_pBuffMat[NUM_MODELINFO] = {};	// マテリアル情報
DWORD				*CModelSetManager::m_pNumMat[NUM_MODELINFO] = {};		// マテリアル数
int					CModelSetManager::m_nNumType[TYPE_MAX] = {};		// モデルの種類
int					CModelSetManager::m_nNumAllModel = 0;			// モデルの総数
char				**CModelSetManager::m_apModelName[TYPE_MAX] = {};		// モデルファイル名
D3DXVECTOR3			CModelSetManager::m_StartPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
D3DXVECTOR3			CModelSetManager::m_GoalPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

//=============================================================================
// モデル情報の読み込み
//=============================================================================
HRESULT CModelSetManager::Load(int nStage)
{
	// デバイスの取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	FILE *pFile;		// ファイルのポインタ変数
	char aStr[128];		// 取得する文字列
	int nCntModel[TYPE_MAX] = {};	// モデルのカウント
	char *pFileName[1] =
	{
		"data/TEXT/MODEL/first_stage.txt",
	};

	// ファイルを開く
	pFile = fopen(pFileName[nStage], "r");

	if (NULL != pFile)
	{// ファイルがあった
		while (strcmp(aStr, "END_SCRIPT") != 0)
		{// 終了メッセージまでループ
			fscanf(pFile, "%s", &aStr[0]);

			if (strcmp(aStr, "TYPE_MODEL") == 0)
			{// オブジェクトの種類がモデルのとき
				fscanf(pFile, " = %d", &m_nNumType[TYPE_MODEL]);

				// モデルファイル名保存変数のメモリ確保
				m_apModelName[TYPE_MODEL] = new char*[m_nNumType[TYPE_MODEL]];

				// モデル情報のメモリ確保
				NumObjectType(TYPE_MODEL);
			}
			if (strcmp(aStr, "TYPE_START") == 0)
			{// オブジェクトの種類がスタート地点のとき
				fscanf(pFile, " = %d", &m_nNumType[TYPE_START]);

				// モデルファイル名保存変数のメモリ確保
				m_apModelName[TYPE_START] = new char*[m_nNumType[TYPE_START]];
			}
			if (strcmp(aStr, "TYPE_ENEMY") == 0)
			{// オブジェクトの種類が敵のとき
				fscanf(pFile, " = %d", &m_nNumType[TYPE_ENEMY]);

				// モデルファイル名保存変数のメモリ確保
				m_apModelName[TYPE_ENEMY] = new char*[m_nNumType[TYPE_ENEMY]];
			}
			if (strcmp(aStr, "TYPE_ITEM") == 0)
			{// オブジェクトの種類がアイテムのとき
				fscanf(pFile, " = %d", &m_nNumType[TYPE_ITEM]);

				// モデルファイル名保存変数のメモリ確保
				m_apModelName[TYPE_ITEM] = new char*[m_nNumType[TYPE_ITEM]];

				// モデル情報のメモリ確保
				NumObjectType(TYPE_ITEM);
			}
			if (strcmp(aStr, "LOAD") == 0)
			{// 読み込むモデル名
				while (strcmp(aStr, "END_LOAD") != 0)
				{// 終了メッセージまでループ
					fscanf(pFile, "%s", &aStr[0]);

					if (strcmp(aStr, "OBJECT_TYPE") == 0)
					{// 読み込むオブジェクトの種類
						fscanf(pFile, " = %s", &aStr[0]);
						TYPE type = TYPE_MODEL;

						// 各オブジェクトの数をカウント
						if (strcmp(aStr, "TYPE_MODEL") == 0) { type = TYPE_MODEL; }
						if (strcmp(aStr, "TYPE_START") == 0) { type = TYPE_START; }
						if (strcmp(aStr, "TYPE_ENEMY") == 0) { type = TYPE_ENEMY; }
						if (strcmp(aStr, "TYPE_ITEM") == 0) { type = TYPE_ITEM; }

						while (1)
						{// ループさせる
							fscanf(pFile, "%s", &aStr[0]);

							if (strcmp(aStr, "MODEL_FILENAME") == 0)
							{// 読み込むモデルファイル
								fscanf(pFile, " = %s", &aStr[0]);

								// 必要な分メモリ確保
								m_apModelName[type][nCntModel[type]] = new char[64];
								strcpy(m_apModelName[type][nCntModel[type]], aStr);

								if (TYPE_MODEL == type || TYPE_ITEM == type)
								{// モデル情報の作成
									CreateModelInfo(type, pDevice, &aStr[0], nCntModel[type]);
								}

								nCntModel[type]++;	// オブジェクト数のカウント
								break;
							}
						}
					}
				}
			}
			if (strcmp(aStr, "MODELSET") == 0)
			{// モデル配置情報
				CModelSetManager::TYPE type = CModelSetManager::TYPE_MODEL;
				int nType = 0;	// モデル番号
				D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 位置
				D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 向き

				while (strcmp(aStr, "END_MODELSET") != 0)
				{// 終了メッセージまでループ
					fscanf(pFile, "%s", &aStr[0]);

					if (strcmp(aStr, "OBJ") == 0)
					{// オブジェクトの種類
						fscanf(pFile, " = %s", &aStr[0]);

						if (strcmp(aStr, "TYPE_MODEL") == 0)
						{// 障害物
							type = TYPE_MODEL;
						}
						else if (strcmp(aStr, "TYPE_START") == 0)
						{// スタート地点
							type = TYPE_START;
						}
						else if (strcmp(aStr, "TYPE_ENEMY") == 0)
						{// 敵
							type = TYPE_ENEMY;
						}
						else if (strcmp(aStr, "TYPE_ITEM") == 0)
						{// アイテム
							type = TYPE_ITEM;
						}
					}
					if (strcmp(aStr, "TYPE") == 0)
					{// モデル番号
						fscanf(pFile, " = %d", &nType);
					}
					if (strcmp(aStr, "POS") == 0)
					{// 位置
						fscanf(pFile, " = %f %f %f", &pos.x, &pos.y, &pos.z);

						if (TYPE_START == type)
						{// スタートとゴール
							if (0 == nType)
							{// スタート地点
								m_StartPos = pos;
							}
							else if (1 == nType)
							{// ゴール地点
								m_GoalPos = pos;
							}
						}
					}
					if (strcmp(aStr, "ROT") == 0)
					{// 向き
						fscanf(pFile, " = %f %f %f", &rot.x, &rot.y, &rot.z);
					}
				}

				if (type == TYPE_MODEL)
				{// オブジェクトの生成
					CModelObject::Create(pos, rot, type, nType);
				}
				else if (type == TYPE_ENEMY && CManager::MODE_GAME == CManager::GetMode())
				{// 敵の生成

				}
			}
		}

		// ファイルを閉じる
		fclose(pFile);
	}
	else
	{// ファイルが開けない
		MessageBox(0, "ファイルがありません！", pFileName[nStage], MB_OK);
	}

	return S_OK;
}

//=============================================================================
// オブジェクトの種類ごとの総数分メモリ確保
//=============================================================================
void CModelSetManager::NumObjectType(TYPE type)
{
	m_pTexture[type] = new LPDIRECT3DTEXTURE9[m_nNumType[type]];
	m_pMesh[type] = new LPD3DXMESH[m_nNumType[type]];
	m_pBuffMat[type] = new LPD3DXBUFFER[m_nNumType[type]];
	m_pNumMat[type] = new DWORD[m_nNumType[type]];
}

//=============================================================================
// モデル情報の作成
//=============================================================================
void CModelSetManager::CreateModelInfo(TYPE type, LPDIRECT3DDEVICE9 pDevice, char *pModelFileName, int nCntModel)
{
	D3DXMATERIAL *pMat;				// マテリアルデータへのポインタ

	// Xファイルの読み込み
	D3DXLoadMeshFromX(pModelFileName,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_pBuffMat[type][nCntModel],
		NULL,
		&m_pNumMat[type][nCntModel],
		&m_pMesh[type][nCntModel]);

	// NULLを入れておく
	m_pTexture[type][nCntModel] = NULL;

	// テクスチャの作成
	pMat = (D3DXMATERIAL*)m_pBuffMat[type][nCntModel]->GetBufferPointer();
	D3DXCreateTextureFromFile(pDevice, pMat->pTextureFilename, &m_pTexture[type][nCntModel]);
}

//=============================================================================
// 読み込んだ情報の解放
//=============================================================================
void CModelSetManager::Unload(void)
{
	// 敵のモデル情報の破棄

	for (int nCntType = 0; nCntType < NUM_MODELINFO; nCntType++)
	{// オブジェクトの種類分ループ
		if (NULL != m_pTexture[nCntType])
		{
			for (int nCntNumType = 0; nCntNumType < m_nNumType[nCntType]; nCntNumType++)
			{
				if (NULL != m_pTexture[nCntType][nCntNumType])
				{// テクスチャの破棄
					m_pTexture[nCntType][nCntNumType]->Release();
					m_pTexture[nCntType][nCntNumType] = NULL;
				}

				if (NULL != m_pMesh[nCntType][nCntNumType])
				{// メッシュ情報の破棄
					m_pMesh[nCntType][nCntNumType]->Release();
					m_pMesh[nCntType][nCntNumType] = NULL;
				}

				if (NULL != m_pBuffMat[nCntType][nCntNumType])
				{// マテリアル情報の破棄
					m_pBuffMat[nCntType][nCntNumType]->Release();
					m_pBuffMat[nCntType][nCntNumType] = NULL;
				}

				if (NULL != m_pNumMat[nCntType][nCntNumType])
				{// マテリアルの数を破棄
					m_pNumMat[nCntType][nCntNumType] = NULL;
				}
			}
		}

		if (NULL != m_pTexture[nCntType])
		{// テクスチャの破棄
			delete[] m_pTexture[nCntType];
			m_pTexture[nCntType] = NULL;
		}

		if (NULL != m_pMesh[nCntType])
		{// メッシュ情報の破棄
			delete[] m_pMesh[nCntType];
			m_pMesh[nCntType] = NULL;
		}

		if (NULL != m_pBuffMat[nCntType])
		{// マテリアル情報の破棄
			delete[] m_pBuffMat[nCntType];
			m_pBuffMat[nCntType] = NULL;
		}

		if (NULL != m_pNumMat[nCntType])
		{// マテリアル数の破棄
			delete[] m_pNumMat[nCntType];
			m_pNumMat[nCntType] = NULL;
		}
	}

	for (int nCntType = 0; nCntType < TYPE_MAX; nCntType++)
	{// オブジェクトの種類分ループ
		if (NULL != m_apModelName[nCntType])
		{
			for (int nCntModel = 0; nCntModel < m_nNumType[nCntType]; nCntModel++)
			{
				if (NULL != m_apModelName[nCntType][nCntModel])
				{// メモリの開放
					delete[] m_apModelName[nCntType][nCntModel];
					m_apModelName[nCntType][nCntModel] = NULL;
				}
			}

			if (NULL != m_apModelName[nCntType])
			{// メモリの開放
				delete[] m_apModelName[nCntType];
				m_apModelName[nCntType] = NULL;
			}
		}
	}

	// モデル数の初期化
	m_nNumAllModel = 0;
}

//=============================================================================
// 生成処理
//=============================================================================
CModelSetManager *CModelSetManager::Create(void)
{
	CModelSetManager *pModelObjectManager = NULL;

	// メモリ確保
	pModelObjectManager = new CModelSetManager;

	if (pModelObjectManager != NULL)
	{// メモリ確保成功
		pModelObjectManager->Init();
	}

	return pModelObjectManager;
}

//=============================================================================
// コンストラクタ
//=============================================================================
CModelSetManager::CModelSetManager(int nPriority, CScene::OBJTYPE objType) : CScene(nPriority, objType)
{
}

//=============================================================================
// デストラクタ
//=============================================================================
CModelSetManager::~CModelSetManager()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CModelSetManager::Init(void)
{
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CModelSetManager::Uninit(void)
{
	// 自分を破棄
	CScene::Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CModelSetManager::Update(void)
{
}

//=============================================================================
// 描画処理
//=============================================================================
void CModelSetManager::Draw(void)
{
}

/*==================================================================================*/
/*																					*/
/*								モデルオブジェクトクラス							*/
/*																					*/
/*==================================================================================*/
//=============================================================================
// 生成処理
//=============================================================================
CModelObject* CModelObject::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, CModelSetManager::TYPE type, int nType)
{
	CModelObject *pObject = NULL;

	pObject = new CModelObject;	// メモリを確保

	if (pObject != NULL)
	{// メモリ確保成功
		pObject->Init(pos, rot, type, nType);

		// モデル数の設定
		CModelSetManager::AddNumModel(1);
	}

	return pObject;
}

//=============================================================================
// コンストラクタ
//=============================================================================
CModelObject::CModelObject(int nPriority, CScene::OBJTYPE objType) : CSceneX(nPriority, objType)
{
	m_nType = 0;
	m_fShadowRange = 0.0f;
}

//=============================================================================
// デストラクタ
//=============================================================================
CModelObject::~CModelObject()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CModelObject::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, CModelSetManager::TYPE type, int nType)
{
	// モデル情報の受け渡し
	CSceneX::BindInfo(CModelSetManager::GetTexture(type, nType), CModelSetManager::GetMesh(type, nType), CModelSetManager::GetBuffMat(type, nType), CModelSetManager::GetNumMat(type, nType));

	// 初期化処理
	CSceneX::Init();

	// 位置の設定
	CSceneX::SetPos(pos);

	// 向きの設定
	CSceneX::SetRot(rot);

	// 種類の保存
	m_nType = nType;

	D3DXVECTOR3 vtxMax = CSceneX::GetMaxVtx();						// 頂点座標の最大値を取得
	m_fShadowRange = sqrtf(powf(vtxMax.x, 2) + powf(vtxMax.z, 2));	// 半径を求める

	// 地面オブジェクトを探す
	CScene *pScene = CScene::GetSceneTop(MESHFIELD_PRIORITY);

	while (pScene != NULL)
	{// NULLになるまでループ
		CScene *pSceneNext = pScene->GetSceneNext();
		CScene::OBJTYPE objType = pScene->GetObjType();

		if (objType == CScene::OBJTYPE_FIELD)
		{// 地面だったとき
			CMeshField *pMeshField = (CMeshField*)pScene;

			// 頂点情報を設定
			VERTEX_3D *pVtx;	// 頂点情報のポインタ

			// 頂点バッファの取得
			LPDIRECT3DVERTEXBUFFER9 pVtxBuff = pMeshField->GetVtxBuff();

			//頂点バッファをロックし、頂点データへのポインタを取得
			pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

			int nNumVtx = pMeshField->GetNumVtx();	// 頂点数の取得
			for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
			{// 全頂点の距離を計算
				float fLength = powf(pVtx[nCntVtx].pos.x - pos.x, 2.0f) + powf(pVtx[nCntVtx].pos.z - pos.z, 2.0f);

				if (fLength <= powf(m_fShadowRange, 2.0f))
				{// 範囲内にある頂点
					pVtx[nCntVtx].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
				}
			}

			// 頂点バッファをアンロック
			pVtxBuff->Unlock();

			// 頂点バッファの設定
			pMeshField->SetVtxBuff(pVtxBuff);
		}

		// 次のオブジェクトを見る
		pScene = pSceneNext;
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CModelObject::Uninit(void)
{
	// 自分を破棄
	CSceneX::Uninit();

	// モデル数の設定
	CModelSetManager::AddNumModel(-1);
}

//=============================================================================
// 更新処理
//=============================================================================
void CModelObject::Update(void)
{
}

//=============================================================================
// 描画処理
//=============================================================================
void CModelObject::Draw(void)
{
	// 描画処理
	CSceneX::Draw();
}

//=============================================================================
// 当たり判定処理
//=============================================================================
bool CModelObject::Collision(D3DXVECTOR3 pos, float fRange)
{
	bool bHit = false;

	// 相手のXYの範囲
	float fLength = powf(fRange, 2);

	// ワールドマトリックスの取得
	D3DXMATRIX mtxWorld = CSceneX::GetMtxWorld();

	float fLengthX = pos.x - mtxWorld._41;						// Xの差
	float fLengthZ = pos.z - mtxWorld._43;						// Yの差
	float fLengthTotal = powf(fLengthX, 2) + powf(fLengthZ, 2);		// XYの差の二乗

	if (fLength >= fLengthTotal)
	{// 相手と自分の判定
		if (false == bHit)
		{// 判定を有効にする
			bHit = true;
		}
	}

	return bHit;
}

//=============================================================================
// 当たり判定処理
//=============================================================================
bool CModelObject::Collision(D3DXVECTOR3 *pPos, float fRange)
{
	bool bHit = false;

	// 相手のXYの範囲
	float fLength = powf(fRange, 2);

	// ワールドマトリックスの取得
	D3DXMATRIX mtxWorld = CSceneX::GetMtxWorld();

	float fLengthX = pPos->x - mtxWorld._41;						// Xの差
	float fLengthZ = pPos->z - mtxWorld._43;						// Zの差
	float fLengthTotal = powf(fLengthX, 2) + powf(fLengthZ, 2);		// XYZの差の二乗

	if (fLength >= fLengthTotal)
	{// 相手と自分の判定
		if (fLengthX <= 0)
		{// 右側にいる
			pPos->x -= fRange - fabsf(fLengthX);
		}
		else if (fLengthX >= 0)
		{// 左側にいる
			pPos->x += fRange - fLengthX;
		}

		if (false == bHit)
		{// 判定を有効にする
			bHit = true;
		}
	}

	return bHit;
}

/*==================================================================================*/
/*																					*/
/*										草クラス									*/
/*																					*/
/*==================================================================================*/
//=============================================================================
// 生成処理
//=============================================================================
CItem* CItem::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, CModelSetManager::TYPE type, int nType)
{
	CItem *pItem = NULL;

	pItem = new CItem;	// メモリを確保

	if (NULL != pItem)
	{// メモリ確保成功
		pItem->Init(pos, rot, type, nType);
	}

	return pItem;
}

//=============================================================================
// コンストラクタ
//=============================================================================
CItem::CItem(int nPriority, CScene::OBJTYPE objType) : CSceneX(nPriority, objType)
{
}

//=============================================================================
// デストラクタ
//=============================================================================
CItem::~CItem()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CItem::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, CModelSetManager::TYPE type, int nType)
{
	// モデル情報の受け渡し
	CSceneX::BindInfo(CModelSetManager::GetTexture(type, nType), CModelSetManager::GetMesh(type, nType), CModelSetManager::GetBuffMat(type, nType), CModelSetManager::GetNumMat(type, nType));

	// 初期化処理
	CSceneX::Init();

	// 位置の設定
	CSceneX::SetPos(pos);

	// 向きの設定
	CSceneX::SetRot(rot);

	// 種類の保存
	m_nType = nType;

	D3DXVECTOR3 vtxMax = CSceneX::GetMaxVtx();						// 頂点座標の最大値を取得
	m_fShadowRange = sqrtf(powf(vtxMax.x, 2) + powf(vtxMax.z, 2));	// 半径を求める

	// 地面オブジェクトを探す
	CScene *pScene = CScene::GetSceneTop(MESHFIELD_PRIORITY);

	while (pScene != NULL)
	{// NULLになるまでループ
		CScene *pSceneNext = pScene->GetSceneNext();
		CScene::OBJTYPE objType = pScene->GetObjType();

		if (objType == CScene::OBJTYPE_FIELD)
		{// 地面だったとき
			CMeshField *pMeshField = (CMeshField*)pScene;

			// 頂点情報を設定
			VERTEX_3D *pVtx;	// 頂点情報のポインタ

			// 頂点バッファの取得
			LPDIRECT3DVERTEXBUFFER9 pVtxBuff = pMeshField->GetVtxBuff();

			//頂点バッファをロックし、頂点データへのポインタを取得
			pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

			int nNumVtx = pMeshField->GetNumVtx();	// 頂点数の取得
			for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
			{// 全頂点の距離を計算
				float fLength = powf(pVtx[nCntVtx].pos.x - pos.x, 2.0f) + powf(pVtx[nCntVtx].pos.z - pos.z, 2.0f);

				if (fLength <= powf(m_fShadowRange, 2.0f))
				{// 範囲内にある頂点
					pVtx[nCntVtx].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
				}
			}

			// 頂点バッファをアンロック
			pVtxBuff->Unlock();

			// 頂点バッファの設定
			pMeshField->SetVtxBuff(pVtxBuff);
		}

		// 次のオブジェクトを見る
		pScene = pSceneNext;
	}

	return S_OK;
}
//=============================================================================
// 終了処理
//=============================================================================
void CItem::Uninit(void)
{
	// 自分を破棄
	CSceneX::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CItem::Update(void)
{
	// 向きの取得
	D3DXVECTOR3 rot = CSceneX::GetRot();

	// 回転させる
	rot.y += 0.001f;

	// 向きの設定
	CSceneX::SetRot(rot);
}

//=============================================================================
// 描画処理
//=============================================================================
void CItem::Draw(void)
{
	// 描画処理
	CSceneX::Draw();
}

//=============================================================================
// 当たり判定処理
//=============================================================================
bool CItem::Collision(D3DXVECTOR3 pos, float fRange)
{
	bool bHit = false;

	// 相手のXYの範囲
	float fLength = powf(fRange, 2);

	// ワールドマトリックスの取得
	D3DXMATRIX mtxWorld = CSceneX::GetMtxWorld();

	float fLengthX = pos.x - mtxWorld._41;						// Xの差
	float fLengthY = pos.y - mtxWorld._42;						// Yの差
	float fLengthZ = pos.z - mtxWorld._43;						// Zの差
	float fLengthTotal = powf(fLengthX, 2) + powf(fLengthY, 2) + powf(fLengthZ, 2);		// XYZの差の二乗

	if (fLength >= fLengthTotal)
	{// 相手と自分の判定
		if (false == bHit)
		{// 判定を有効にする
			bHit = true;
		}
	}

	return bHit;
}