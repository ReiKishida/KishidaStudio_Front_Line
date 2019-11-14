//=============================================================================
//
// パーティクル処理 [particle.cpp]
// Author : Ishida Takuto
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "particle.h"
#include "renderer.h"
#include "manager.h"

#include <stdio.h>

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define PARTICLE_FILENAME	"data/TEXT/EFFECT/particle.txt"				// パーティクルデータ
#define TEXTURE_FILENAME	"data/TEXT/EFFECT/particle_texture.txt"		// テクスチャ名

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************
LPDIRECT3DTEXTURE9 *CParData::m_pTexture = NULL;	// テクスチャの情報
int CParData::m_nNumTexture = 0;					// テクスチャの数
CParData::DATA **CParData::m_pParticleData = NULL;	// ファイルから読み取ったデータ
int CParData::m_nNumParticle = 0;					// パーティクル数

//=========================================
// コンストラクタ
//=========================================
CParticle::CParticle(int nPriority, CScene::OBJTYPE objType) : CScene(nPriority, objType)
{
	m_nType = 0;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//=========================================
// デストラクタ
//=========================================
CParticle::~CParticle()
{
}

//==================================
// 生成処理
//==================================
CParticle *CParticle::Create(D3DXVECTOR3 pos, int nType)
{
	CParticle *pParticle;

	pParticle = new CParEmitter;	// メモリを確保
	pParticle->m_pos = pos;
	pParticle->m_nType = nType;
	pParticle->Init();

	return pParticle;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CParticle::Init(void)
{
	// オブジェクトタイプの設定
	CScene::SetObjType(CScene::OBJTYPE_PARTICLE);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CParticle::Uninit(void)
{
	Release();
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//		パーティクルのエミッタ
//
//=========================================
// コンストラクタ
//=========================================
CParEmitter::CParEmitter()
{
	m_nCntParticle = 0;
	m_nAppear = 0;
	m_nCntAngle = 0;

	m_nLife = 0;
	m_nType = 0;
	m_nAppear = 0;
	m_nLap = 0;
	m_bLoop = false;
}

//=========================================
// デストラクタ
//=========================================
CParEmitter::~CParEmitter()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CParEmitter::Init(void)
{
	CParticle::Init();
	Set(m_pos, m_nType);	// エミッタの設定

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CParEmitter::Uninit(void)
{
	CParticle::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CParEmitter::Update(void)
{
	bool bDelete = false;
	m_nLife--;	// エミッタの表示時間をカウント

	if (m_nLife <= 0 && m_bLoop == false)
	{// 一定時間経過でエミッタを消す
		bDelete = true;
	}

	// 出現させる間隔のカウント
	m_nCntParticle = (m_nCntParticle + 1) % m_nAppear;

	if (m_nCntParticle == 0)
	{// 出現させる
		CParticlePolygon::Create(m_pos, m_nType, m_nTexture);

		m_nCntAngle = (m_nCntAngle + 1) % m_nLap;
	}

	if (true == bDelete)
	{// 消す
		Uninit();
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CParEmitter::Draw(void)
{
}

//=============================================================================
// エミッタの設定
//=============================================================================
void CParEmitter::Set(D3DXVECTOR3 pos, int nType)
{
	m_pos = pos;
	m_nType = nType;
	m_nAppear = CParData::GetAppear(m_nType);
	m_nLife = CParData::GetLife(m_nType);
	m_nTexture = CParData::GetTextureIdx(m_nType);
	m_nLap = CParData::GetLap(m_nType);
}

//=============================================================================
// エミッタの位置の設定
//=============================================================================
void CParEmitter::SetPosParticleEmitter(int nIdx, D3DXVECTOR3 pos)
{
	// 位置を更新
	m_pos = pos;
}

//=============================================================================
// エミッタの削除
//=============================================================================
void CParEmitter::DestroyParticle(int nIdx)
{
	// 消す
	CParEmitter::Uninit();
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//		パーティクルのポリゴン
//
//==================================
// 生成処理
//==================================
CParticlePolygon *CParticlePolygon::Create(D3DXVECTOR3 pos, int nType, int nTexture)
{
	CParticlePolygon *pParticlePlygon;

	pParticlePlygon = new CParticlePolygon;	// メモリを確保
	pParticlePlygon->m_nType = nType;
	pParticlePlygon->Init(pos);
	pParticlePlygon->BindTexture(CParData::GetTexture(nTexture));
	return pParticlePlygon;
}

//=========================================
// コンストラクタ
//=========================================
CParticlePolygon::CParticlePolygon(int nPriority) : CScene3DBill(nPriority)
{
	m_bRotVec = false;
	m_bDraw = false;
	m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	m_fAngle = 0.0f;
	m_fBouncing = 0.0f;
	m_fCntRot = 0.0f;
	m_fGravity = 0.0f;
	m_fLength = 0.0f;
	m_fRadius = 0.0f;
	m_fRotAngle = 0.0f;
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nCntAngle = 0;
	m_nLife = 0;
	m_nType = 0;
}

//=========================================
// デストラクタ
//=========================================
CParticlePolygon::~CParticlePolygon()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CParticlePolygon::Init(D3DXVECTOR3 pos)
{
	// オブジェクトタイプの設定
	CScene::SetObjType(CScene::OBJTYPE_PARTICLE);

	// 初期化処理
	CScene3DBill::Init();
	CScene3DBill::SetPos(pos);

	int nLap = CParData::GetLap(m_nType);				// 一周の数
	int *nLife = CParData::GetParLife(m_nType);
	int *nRadius = CParData::GetRadius(m_nType);
	int *nWidth = CParData::GetWidth(m_nType);
	int *nHeight = CParData::GetHeight(m_nType);
	int *nDepth = CParData::GetDepth(m_nType);
	float fSpread = CParData::GetSpread(m_nType);
	D3DXCOLOR col = CParData::GetColor(m_nType);
	bool bDraw = CParData::GetDraw(m_nType);
	float fGravity = CParData::GetGravity(m_nType);

	m_fAngle = 1.0f / (nLap / 2);
	if (0 != nHeight[1] && 0 != nHeight[0])
	{// Y軸移動量が両方０以外
		m_move = D3DXVECTOR3(sinf(D3DX_PI * (0.0f + (m_fAngle * m_nCntAngle))) * ((nWidth[0] + (rand() % (1 + nWidth[1]))) - ((rand() % 10) * 0.1f)) * fSpread,
			((rand() % nHeight[1]) + (rand() % nHeight[0])) * fSpread,
			cosf(D3DX_PI * (0.0f + (m_fAngle * m_nCntAngle))) * ((nDepth[0] + (rand() % (1 + nDepth[1]))) - ((rand() % 10) * 0.1f)) * fSpread);
	}
	else
	{// Y軸の移動量が０がある場合
		m_move = D3DXVECTOR3(sinf(D3DX_PI * (0.0f + (m_fAngle * m_nCntAngle))) * ((nWidth[0] + (rand() % (1 + nWidth[1]))) - ((rand() % 10) * 0.1f)) * fSpread,
			(nHeight[1] + nHeight[0]) * fSpread,
			cosf(D3DX_PI * (0.0f + (m_fAngle * m_nCntAngle))) * ((nDepth[0] + (rand() % (1 + nDepth[1]))) - ((rand() % 10) * 0.1f)) * fSpread);
	}
	m_col = col;
	CScene3DBill::SetColor(m_col);
	m_fRadius = nRadius[0] + (rand() % (1 + (nRadius[1] - nRadius[0])) - 1) + ((rand() % 10) * 0.1f);
	m_nLife = nLife[0] + (rand() % (1 + (nLife[1] - nLife[0])) - 1);
	m_fGravity = fGravity;
	m_bDraw = bDraw;

	// 頂点の角度
	m_fRotAngle = atan2f(pos.x - (pos.x + m_fRadius), pos.y - (pos.y + m_fRadius));

	// 中心から頂点までの距離
	m_fLength = sqrtf((m_fRadius * m_fRadius) + (m_fRadius * m_fRadius));

	// 回転がランダムだったときの回転方向
	if (rand() % 2 == 0)
	{// 右回り
		m_bRotVec = true;
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CParticlePolygon::Uninit(void)
{
	CScene3DBill::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CParticlePolygon::Update(void)
{
	m_nLife--;		// 表示時間をカウント

	if (m_nLife <= 0)
	{// 表示時間が0以下になった
		Uninit();	// 終了処理
	}
	else
	{
		D3DXVECTOR3 pos = CScene3DBill::GetPos();
		D3DXCOLOR colFlame = CParData::GetFlameColor(m_nType);
		float fFlameRadius = CParData::GetFlameRadius(m_nType);
		bool bBouncing = CParData::GetBouncing(m_nType);
		int nRotType = CParData::GetRotType(m_nType);
		float fRotSpeed = CParData::GetRotSpeed(m_nType);
		m_move.y -= m_fGravity;		// 重力を加える

		m_fBouncing = pos.y;		// 値を記憶

		pos += m_move;				// 位置の更新
		m_col += colFlame;			// 色の更新
		m_fRadius += fFlameRadius;	// 大きさの更新

		if (pos.y <= 0 && bBouncing == true)
		{// バウンド
			m_move.y += (m_fBouncing - pos.y) * 2.0f;
		}

		if (nRotType == 1)
		{// 時計回り
			m_fCntRot -= fRotSpeed;			// 回転のカウンタ
		}
		else if (nRotType == 2)
		{// 反時計回り
			m_fCntRot += fRotSpeed;			// 回転のカウンタ
		}
		else if (nRotType == 3)
		{// ランダム
			if (m_bRotVec)
			{// 時計回り
				m_fCntRot += fRotSpeed;		// 回転のカウンタ
			}
			else
			{// 反時計回り
				m_fCntRot -= fRotSpeed;		// 回転のカウンタ
			}
		}

		// 頂点バッファの取得
		LPDIRECT3DVERTEXBUFFER9 pVtxBuff = CScene3DBill::GetVtxBuff();

		// 頂点情報を設定
		VERTEX_3D *pVtx;	// 頂点情報のポインタ

		// 頂点バッファをロックし、頂点データへのポインタを取得
		pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		//頂点座標の設定
		pVtx[0].pos.x = sinf(m_fRotAngle + m_fCntRot) * -(m_fLength + m_fRadius);
		pVtx[0].pos.y = cosf(m_fRotAngle + m_fCntRot) * (m_fLength + m_fRadius);
		pVtx[1].pos.x = sinf(m_fRotAngle - m_fCntRot) * (m_fLength + m_fRadius);
		pVtx[1].pos.y = cosf(m_fRotAngle - m_fCntRot) * (m_fLength + m_fRadius);
		pVtx[2].pos.x = sinf(m_fRotAngle - m_fCntRot) * -(m_fLength + m_fRadius);
		pVtx[2].pos.y = cosf(m_fRotAngle - m_fCntRot) * -(m_fLength + m_fRadius);
		pVtx[3].pos.x = sinf(m_fRotAngle + m_fCntRot) * (m_fLength + m_fRadius);
		pVtx[3].pos.y = cosf(m_fRotAngle + m_fCntRot) * -(m_fLength + m_fRadius);

		// 頂点バッファをアンロック
		pVtxBuff->Unlock();

		// 頂点バッファの設定
		CScene3DBill::SetVtxBuff(pVtxBuff);

		//頂点カラー
		CScene3DBill::SetColor(m_col);

		CScene3DBill::SetPos(pos);
	}
}

//=============================================================================
// ポリゴン描画処理
//=============================================================================
void CParticlePolygon::Draw(void)
{
	// デバイスの取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	// 被らないようにする
	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

	if (m_bDraw == true)
	{ // αブレンディングを加算合成に設定
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	}
	else
	{// αブレンディングを元に戻す
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	}

	// ライティングを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// 描画
	CScene3DBill::Draw();

	// ライティングを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// Zバッファを元に戻す
	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//		パーティクルのデータ
//
//=========================================
// コンストラクタ
//=========================================
CParData::CParData()
{
}

//=========================================
// デストラクタ
//=========================================
CParData::~CParData()
{
}

//=============================================================================
// ファイルからデータを取得
//=============================================================================
HRESULT CParData::Load(void)
{
	FILE *pFile;
	int nCntEffect = 0;
	char aStr[128];

	// ファイルを開く
	pFile = fopen(PARTICLE_FILENAME, "r");

	if (NULL != pFile)
	{
		while (strcmp(aStr, "END_SCRIPT") != 0)
		{// 終了メッセージまでループ
			fscanf(pFile, "%s", aStr);

			if (strcmp(aStr, "NUM_PARTICLE") == 0)
			{// パーティクル数
				fscanf(pFile, " = %d", &m_nNumParticle);
				m_pParticleData = new CParData::DATA*[m_nNumParticle];
			}
			if (strcmp(aStr, "PARTICLESET") == 0)
			{// エミッタの情報
				m_pParticleData[nCntEffect] = new CParData::DATA;
				while (strcmp(aStr, "END_PARTICLESET") != 0)
				{// 終了メッセージまでループ
					fscanf(pFile, "%s", &aStr);

					if (strcmp(aStr, "MAXMOVE") == 0)
					{// 移動量の最大
						fscanf(pFile, " = %d %d %d", &m_pParticleData[nCntEffect]->nWidth[1], &m_pParticleData[nCntEffect]->nHeight[1], &m_pParticleData[nCntEffect]->nDepth[1]);
					}
					else if (strcmp(aStr, "MINMOVE") == 0)
					{// 移動量の最小
						fscanf(pFile, " = %d %d %d", &m_pParticleData[nCntEffect]->nWidth[0], &m_pParticleData[nCntEffect]->nHeight[0], &m_pParticleData[nCntEffect]->nDepth[0]);
					}
					else if (strcmp(aStr, "INITCOL") == 0)
					{// 初期カラー
						fscanf(pFile, " = %f %f %f %f", &m_pParticleData[nCntEffect]->col.r, &m_pParticleData[nCntEffect]->col.g, &m_pParticleData[nCntEffect]->col.b, &m_pParticleData[nCntEffect]->col.a);
					}
					else if (strcmp(aStr, "FLAMECOL") == 0)
					{// フレームによるカラーの変化
						fscanf(pFile, " = %f %f %f %f", &m_pParticleData[nCntEffect]->flameCol.r, &m_pParticleData[nCntEffect]->flameCol.g, &m_pParticleData[nCntEffect]->flameCol.b, &m_pParticleData[nCntEffect]->flameCol.a);
					}
					else if (strcmp(aStr, "INITSIZE") == 0)
					{// 初期サイズ
						fscanf(pFile, " = %d %d", &m_pParticleData[nCntEffect]->nRadius[0], &m_pParticleData[nCntEffect]->nRadius[1]);
					}
					else if (strcmp(aStr, "FLAMESIZE") == 0)
					{// フレームによるサイズの変化
						fscanf(pFile, " = %f", &m_pParticleData[nCntEffect]->fFlameRadius);
					}
					else if (strcmp(aStr, "EMITTERLIFE") == 0)
					{// 表示時間(寿命)
						fscanf(pFile, " = %d", &m_pParticleData[nCntEffect]->nLife);
					}
					else if (strcmp(aStr, "PARLIFE") == 0)
					{// パーティクルの表示時間
						fscanf(pFile, " = %d %d", &m_pParticleData[nCntEffect]->nParLife[0], &m_pParticleData[nCntEffect]->nParLife[1]);
					}
					else if (strcmp(aStr, "APPEAR") == 0)
					{// 出る間隔
						fscanf(pFile, " = %d", &m_pParticleData[nCntEffect]->nAppear);
					}
					else if (strcmp(aStr, "SPREAD") == 0)
					{// 広がり
						fscanf(pFile, " = %f", &m_pParticleData[nCntEffect]->fSpread);
					}
					else if (strcmp(aStr, "LAP") == 0)
					{// 1周に出す個数
						fscanf(pFile, " = %d", &m_pParticleData[nCntEffect]->nLap);
					}
					else if (strcmp(aStr, "GRAVITY") == 0)
					{// 重力
						fscanf(pFile, " = %f", &m_pParticleData[nCntEffect]->fGravity);
					}
					else if (strcmp(aStr, "ROTPATTERN") == 0)
					{// 回転のパターン
						fscanf(pFile, " = %d", &m_pParticleData[nCntEffect]->nRot);
					}
					else if (strcmp(aStr, "ROTSPEED") == 0)
					{// 回転の速度
						fscanf(pFile, " = %f", &m_pParticleData[nCntEffect]->fRotSpeed);
					}
					else if (strcmp(aStr, "LOOP") == 0)
					{// ループするかどうか
						int nBool;
						fscanf(pFile, " = %d", &nBool);
						if (0 == nBool) { m_pParticleData[nCntEffect]->bLoop = false; }
						else { m_pParticleData[nCntEffect]->bLoop = true; }
					}
					else if (strcmp(aStr, "BOUND") == 0)
					{// バウンドするかどうか
						int nBool;
						fscanf(pFile, " = %d", &nBool);
						if (0 == nBool) { m_pParticleData[nCntEffect]->bBouncing = false; }
						else { m_pParticleData[nCntEffect]->bBouncing = true; }
					}
					else if (strcmp(aStr, "TEXTURE") == 0)
					{// テクスチャの種類
						fscanf(pFile, " = %d", &m_pParticleData[nCntEffect]->nTexture);
					}
					else if (strcmp(aStr, "DRAW") == 0)
					{// 加算合成の有無
						int nBool;
						fscanf(pFile, " = %d", &nBool);
						if (0 == nBool) { m_pParticleData[nCntEffect]->bDraw = false; }
						else{ m_pParticleData[nCntEffect]->bDraw = true; }
					}
				}
				nCntEffect++;	// エミッタ番号のカウント

				if (nCntEffect >= m_nNumParticle)
				{// 指定した数を超えた
					break;
				}
			}
		}

		// ファイルを閉じる
		fclose(pFile);
	}
	else
	{// 失敗
		MessageBox(0, "ファイルが開けませんでした", "particle.txt", MB_OK);
	}

	// デバイスの取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	char *pFileName = NULL;		// テクスチャの名前
	int nCntTexture;

	// ファイルを開く
	pFile = fopen(TEXTURE_FILENAME, "r");

	if (NULL != pFile)
	{
		// テクスチャ数
		fscanf(pFile, "%d", &m_nNumTexture);

		// メモリ確保
		m_pTexture = new LPDIRECT3DTEXTURE9[m_nNumTexture];
		pFileName = new char[128];

		for (nCntTexture = 0; nCntTexture < m_nNumTexture; nCntTexture++)
		{// テクスチャ数分ループ
			fscanf(pFile, "%s\n", &pFileName[0]);	// テクスチャ名

			// テクスチャの読み込み
			D3DXCreateTextureFromFile(pDevice, &pFileName[0], &m_pTexture[nCntTexture]);
		}

		// 破棄
		delete[] pFileName;

		fclose(pFile);
	}
	else
	{// 失敗
		MessageBox(0, "ファイルが開けませんでした", "particle_texture.txt", MB_OK);
	}

	return S_OK;
}

//=============================================================================
// データの開放
//=============================================================================
void CParData::Unload(void)
{
	for (int nCntParticle = 0; nCntParticle < m_nNumParticle; nCntParticle++)
	{// 読み取ったデータを破棄
		if (NULL != m_pParticleData[nCntParticle])
		{// メモリの開放
			delete m_pParticleData[nCntParticle];
			m_pParticleData[nCntParticle] = NULL;
		}
	}

	if (NULL != m_pParticleData)
	{// メモリの開放
		delete[] m_pParticleData;
		m_pParticleData = NULL;
	}

	if (NULL != m_pTexture)
	{// テクスチャの破棄
		delete[] m_pTexture;
		m_pTexture = NULL;
	}

	// 値を初期値にする
	m_nNumTexture = 0;
	m_nNumParticle = 0;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CParData::Init(void)
{
	for (int nCntParticle = 0; nCntParticle < m_nNumParticle; nCntParticle++)
	{// 構造体のデータに初期値を入れる
		for (int nCntArray = 0; nCntArray < 2; nCntArray++)
		{
			m_pParticleData[nCntParticle]->nWidth[nCntArray] = 0;
			m_pParticleData[nCntParticle]->nHeight[nCntArray] = 0;
			m_pParticleData[nCntParticle]->nDepth[nCntArray] = 0;
			m_pParticleData[nCntParticle]->nRadius[nCntArray] = 0;
			m_pParticleData[nCntParticle]->nParLife[nCntArray] = 0;
		}
		m_pParticleData[nCntParticle]->col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		m_pParticleData[nCntParticle]->flameCol = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		m_pParticleData[nCntParticle]->fFlameRadius = 0.0f;
		m_pParticleData[nCntParticle]->nLife = 0;
		m_pParticleData[nCntParticle]->nTexture = 0;
		m_pParticleData[nCntParticle]->fSpread = 0.0f;
		m_pParticleData[nCntParticle]->nAppear = 0;
		m_pParticleData[nCntParticle]->fGravity = 0.0f;
		m_pParticleData[nCntParticle]->nLap = 0;
		m_pParticleData[nCntParticle]->nRot = 0;
		m_pParticleData[nCntParticle]->fRotSpeed = 0.0f;
		m_pParticleData[nCntParticle]->bLoop = false;
		m_pParticleData[nCntParticle]->bBouncing = false;
		m_pParticleData[nCntParticle]->bDraw = false;
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CParData::Uninit(void)
{

}
