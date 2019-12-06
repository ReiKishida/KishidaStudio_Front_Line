//=============================================================================
//
// 2Dオブジェクト処理 [scene.cpp]
// Author : Takuto Ishida
//
//=============================================================================
#include "scene2D.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"

//==================================
// マクロ定義
//==================================

//==================================
// 静的メンバ変数宣言
//==================================

//=========================================
// 生成処理
//=========================================
CScene2D *CScene2D::Create(void)
{
	CScene2D *pScene2D = new CScene2D;

	if (pScene2D != NULL)
	{// メモリ確保成功
		pScene2D->Init();
	}

	return pScene2D;
}

//=========================================
// テクスチャを割り当てる
//=========================================
void CScene2D::BindTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
}

//=========================================
// コンストラクタ
//=========================================
CScene2D::CScene2D(int nPriority, CScene::OBJTYPE objType) : CScene(nPriority, objType)
{
	m_pTexture = NULL;		// テクスチャへのポインタ
	m_pVtxBuff = NULL;		// 頂点バッファへのポインタ

	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// ポリゴンの位置
	m_fSizeX = 0.0f;
	m_fSizeY = 0.0f;
	m_bDisp = true;
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	for (int nCntVtx = 0; nCntVtx < 4; nCntVtx++)
	{
		m_aPosVtx[nCntVtx] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
}

//=========================================
// デストラクタ
//=========================================
CScene2D::~CScene2D()
{
}

//=========================================
// 初期化処理
//=========================================
HRESULT CScene2D::Init(void)
{
	// デバイスの取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// 頂点情報を設定
	VERTEX_2D *pVtx;	// 頂点情報のポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標
	pVtx[0].pos = D3DXVECTOR3(-m_fSizeX, -m_fSizeY, 0.0f) + m_pos;
	pVtx[1].pos = D3DXVECTOR3(m_fSizeX, -m_fSizeY, 0.0f) + m_pos;
	pVtx[2].pos = D3DXVECTOR3(-m_fSizeX, m_fSizeY, 0.0f) + m_pos;
	pVtx[3].pos = D3DXVECTOR3(m_fSizeX, m_fSizeY, 0.0f) + m_pos;

	// 1.0fで固定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 頂点カラー
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	// テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=========================================
// 終了処理
//=========================================
void CScene2D::Uninit(void)
{
	// 頂点バッファの破棄
	if (NULL != m_pVtxBuff)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	// 自分の破棄
	CScene::Release();
}

//=========================================
// 更新処理
//=========================================
void CScene2D::Update(void)
{
}

//=========================================
// 描画処理
//=========================================
void CScene2D::Draw(void)
{
	if (!m_bDisp) { return; }

	// デバイスの取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	// αテストの設定
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, m_pTexture);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	// αテストの設定を戻す
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
}

//=========================================
// 回転の設定
//=========================================
void CScene2D::SetRot(float fRotate)
{
	D3DXMATRIX PosMatrix, RotateMatrix;

	// 頂点情報を設定
	VERTEX_2D *pVtx;		// 頂点情報のポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ポリゴンの中心を原点へ移動させる
	MoveMatrix(-m_pos.x, -m_pos.y);

	// 行列の初期化（単位行列生成）
	D3DXMatrixIdentity(&PosMatrix);
	D3DXMatrixIdentity(&RotateMatrix);

	// 回転行列に角度を設定
	D3DXMatrixRotationZ(&RotateMatrix, fRotate);

	// 回転処理
	for (int nCntVertex = 0; nCntVertex < 4; nCntVertex++)
	{
		// 現在の頂点座標を格納
		D3DXMatrixTranslation(&PosMatrix, m_aPosVtx[nCntVertex].x, m_aPosVtx[nCntVertex].y, m_aPosVtx[nCntVertex].z);

		// 演算
		PosMatrix *= RotateMatrix;

		// 結果を戻す
		pVtx[nCntVertex].pos.x = PosMatrix._41;
		pVtx[nCntVertex].pos.y = PosMatrix._42;
		pVtx[nCntVertex].pos.z = PosMatrix._43;
	}

	// ポリゴンを元の位置へ戻す
	MoveMatrix(m_pos.x, m_pos.y);

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//=========================================
// 行列を使った移動
//=========================================
void CScene2D::MoveMatrix(float fPosX, float fPosY)
{
	D3DXMATRIX PosMatrix, MoveMatrix;

	// 行列の初期化（単位行列生成）
	D3DXMatrixIdentity(&PosMatrix);
	D3DXMatrixIdentity(&MoveMatrix);

	// 移動量設定
	D3DXMatrixTranslation(&MoveMatrix, fPosX, fPosY, 0.0f);

	// 頂点情報を設定
	VERTEX_2D *pVtx;		// 頂点情報のポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 移動処理
	for (int nCntVertex = 0; nCntVertex < 4; nCntVertex++)
	{
		// 現在の頂点座標を格納
		D3DXMatrixTranslation(&PosMatrix, pVtx[nCntVertex].pos.x, pVtx[nCntVertex].pos.y, pVtx[nCntVertex].pos.z);

		// 演算
		PosMatrix *= MoveMatrix;

		// 結果を戻す
		pVtx[nCntVertex].pos.x = PosMatrix._41;
		pVtx[nCntVertex].pos.y = PosMatrix._42;
		pVtx[nCntVertex].pos.z = PosMatrix._43;
	}

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//=========================================
// 頂点座標の設定
//=========================================
void CScene2D::SetPosVtx(D3DXVECTOR3 *pPosVtx)
{
	// 頂点情報を設定
	VERTEX_2D *pVtx;		// 頂点情報のポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos = pPosVtx[0];
	pVtx[1].pos = pPosVtx[1];
	pVtx[2].pos = pPosVtx[2];
	pVtx[3].pos = pPosVtx[3];

	m_aPosVtx[0] = pPosVtx[0];
	m_aPosVtx[1] = pPosVtx[1];
	m_aPosVtx[2] = pPosVtx[2];
	m_aPosVtx[3] = pPosVtx[3];

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//=========================================
// 当たり判定
//=========================================
bool CScene2D::Collision(D3DXVECTOR3 pos, float fRange)
{
	bool bHit = false;

	// 相手のXYの範囲
	float fLength = powf(fRange, 2);

	float fLengthX = pos.x - m_pos.x;	// Xの差
	float fLengthY = pos.y - m_pos.y;	// Yの差

	float fLengthTotal = powf(fLengthX, 2) + powf(fLengthY, 2);		// XYの差の二乗

	if (fLength >= fLengthTotal)
	{// 範囲チェック
		bHit = true;	// 判定を有効にする
	}

	return bHit;	// 判定結果を返す
}

//=========================================
// 位置の設定
//=========================================
void CScene2D::SetPos(D3DXVECTOR3 pos)
{
	m_pos = pos;

	// 頂点情報を設定
	VERTEX_2D *pVtx;		// 頂点情報のポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点の座標
	pVtx[0].pos = D3DXVECTOR3(-m_fSizeX, -m_fSizeY, 0.0f) * 0.5f + m_pos;
	pVtx[1].pos = D3DXVECTOR3(m_fSizeX, -m_fSizeY, 0.0f) * 0.5f + m_pos;
	pVtx[2].pos = D3DXVECTOR3(-m_fSizeX, m_fSizeY, 0.0f) * 0.5f + m_pos;
	pVtx[3].pos = D3DXVECTOR3(m_fSizeX, m_fSizeY, 0.0f) * 0.5f + m_pos;

	m_aPosVtx[0] = pVtx[0].pos;
	m_aPosVtx[1] = pVtx[1].pos;
	m_aPosVtx[2] = pVtx[2].pos;
	m_aPosVtx[3] = pVtx[3].pos;

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//=========================================
// サイズの設定
//=========================================
void CScene2D::SetSize(float fSizeX, float fSizeY)
{
	// サイズの値を入れる
	m_fSizeX = fSizeX;
	m_fSizeY = fSizeY;

	// 頂点情報を設定
	VERTEX_2D *pVtx;		// 頂点情報のポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点の座標
	pVtx[0].pos = D3DXVECTOR3(-m_fSizeX, -m_fSizeY, 0.0f) * 0.5f + m_pos;
	pVtx[1].pos = D3DXVECTOR3(m_fSizeX, -m_fSizeY, 0.0f) * 0.5f + m_pos;
	pVtx[2].pos = D3DXVECTOR3(-m_fSizeX, m_fSizeY, 0.0f) * 0.5f + m_pos;
	pVtx[3].pos = D3DXVECTOR3(m_fSizeX, m_fSizeY, 0.0f) * 0.5f + m_pos;

	m_aPosVtx[0] = pVtx[0].pos;
	m_aPosVtx[1] = pVtx[1].pos;
	m_aPosVtx[2] = pVtx[2].pos;
	m_aPosVtx[3] = pVtx[3].pos;

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//=========================================
// 幅の設定
//=========================================
void CScene2D::SetWidth(float fWidth)
{
	// サイズの値を入れる
	m_fSizeX = fWidth;

	// 頂点情報を設定
	VERTEX_2D *pVtx;		// 頂点情報のポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点の座標
	pVtx[0].pos.x = m_pos.x - (m_fSizeX * 0.5f);
	pVtx[1].pos.x = m_pos.x + (m_fSizeX * 0.5f);
	pVtx[2].pos.x = m_pos.x - (m_fSizeX * 0.5f);
	pVtx[3].pos.x = m_pos.x + (m_fSizeX * 0.5f);

	m_aPosVtx[0] = pVtx[0].pos;
	m_aPosVtx[1] = pVtx[1].pos;
	m_aPosVtx[2] = pVtx[2].pos;
	m_aPosVtx[3] = pVtx[3].pos;

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//=========================================
// 高さの設定
//=========================================
void CScene2D::SetHeight(float fHeight)
{
	// サイズの値を入れる
	m_fSizeY = fHeight;

	// 頂点情報を設定
	VERTEX_2D *pVtx;		// 頂点情報のポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点の座標
	pVtx[0].pos.y = m_pos.y - (m_fSizeY * 0.5f);
	pVtx[1].pos.y = m_pos.y - (m_fSizeY * 0.5f);
	pVtx[2].pos.y = m_pos.y + (m_fSizeY * 0.5f);
	pVtx[3].pos.y = m_pos.y + (m_fSizeY * 0.5f);

	m_aPosVtx[0] = pVtx[0].pos;
	m_aPosVtx[1] = pVtx[1].pos;
	m_aPosVtx[2] = pVtx[2].pos;
	m_aPosVtx[3] = pVtx[3].pos;

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//=========================================
// 色の設定
//=========================================
void CScene2D::SetColor(D3DXCOLOR col)
{
	// 色の設定
	m_col = col;

	// 頂点情報を設定
	VERTEX_2D *pVtx;		// 頂点情報のポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//テクスチャ座標
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//=========================================
// 色の加算
//=========================================
void CScene2D::AddColor(D3DXCOLOR col)
{
	// 色の設定
	m_col += col;

	// 頂点情報を設定
	VERTEX_2D *pVtx;		// 頂点情報のポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//テクスチャ座標
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//=============================================================================
// 点滅処理
//=============================================================================
void CScene2D::Flashing(int nFlame)
{
	// 指定したフレームで
	if (m_nCntFlash < nFlame)
	{// 透明にする
		m_col.a -= 1.0f / nFlame;
	}
	else
	{// 不透明にする
		m_col.a += 1.0f / nFlame;
	}

	m_nCntFlash = (1 + m_nCntFlash) % (nFlame * 2);		// カウントを進める

	if (m_col.a <= 0)
	{// アルファ値が０以下のとき
		m_col.a = 0;
		m_nCntFlash = nFlame;
	}
	else if (m_col.a >= 1)
	{// アルファ値が１以上のとき
		m_col.a = 1;
		m_nCntFlash = 0;
	}

	// 頂点情報を設定
	VERTEX_2D *pVtx;		// 頂点情報のポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//テクスチャ座標
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//=============================================================================
// 背景動かす処理
//=============================================================================
void CScene2D::SetBgMove(int nCntBg_X, int nCntBg_Y, float fScrollSpeed)
{
	VERTEX_2D * pVtx;			//頂点情報へのポインタ

								//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//テクスチャの座標設定
	pVtx[0].tex = D3DXVECTOR2(0.0f + (nCntBg_X * (fScrollSpeed + 1)), 0.0f + (nCntBg_Y * (fScrollSpeed + 1)));
	pVtx[1].tex = D3DXVECTOR2(1.0f + (nCntBg_X * (fScrollSpeed + 1)), 0.0f + (nCntBg_Y * (fScrollSpeed + 1)));
	pVtx[2].tex = D3DXVECTOR2(0.0f + (nCntBg_X * (fScrollSpeed + 1)), 1.0f + (nCntBg_Y * (fScrollSpeed + 1)));
	pVtx[3].tex = D3DXVECTOR2(1.0f + (nCntBg_X * (fScrollSpeed + 1)), 1.0f + (nCntBg_Y * (fScrollSpeed + 1)));

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=============================================================================
// 分割した時の何番目を使うか・幅何分割・高さ何分割：テクスチャのUV設定
//=============================================================================
void CScene2D::SetTex(int nUV, int nUV_X, int nUV_Y)
{
	VERTEX_2D * pVtx;			//頂点情報へのポインタ

								//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//テクスチャの座標設定
	pVtx[0].tex = D3DXVECTOR2((nUV % nUV_X) * (1.0f / nUV_X), (nUV / nUV_X) * (1.0f / nUV_Y));
	pVtx[1].tex = D3DXVECTOR2((nUV % nUV_X) * (1.0f / nUV_X) + (1.0f / nUV_X), (nUV / nUV_X) * (1.0f / nUV_Y));
	pVtx[2].tex = D3DXVECTOR2((nUV % nUV_X) * (1.0f / nUV_X), (nUV / nUV_X) * (1.0f / nUV_Y) + (1.0f / nUV_Y));
	pVtx[3].tex = D3DXVECTOR2((nUV % nUV_X) * (1.0f / nUV_X) + (1.0f / nUV_X), (nUV / nUV_X) * (1.0f / nUV_Y) + (1.0f / nUV_Y));

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=============================================================================
// 分割したテクスチャをスクロールする処理
//=============================================================================
void CScene2D::SetTexMove(int nUV, int nUV_X, int nUV_Y, int nCntBg_X, int nCntBg_Y, float fScrollSpeed)
{
	VERTEX_2D * pVtx;			//頂点情報へのポインタ

								//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//テクスチャの座標設定
	pVtx[0].tex = D3DXVECTOR2((nUV % nUV_X) * (1.0f / nUV_X) + (nCntBg_X * (fScrollSpeed + 1)), (nUV / nUV_X) * (1.0f / nUV_Y) + (nCntBg_Y * (fScrollSpeed + 1)));
	pVtx[1].tex = D3DXVECTOR2((nUV % nUV_X) * (1.0f / nUV_X) + (1.0f / nUV_X) + (nCntBg_X * (fScrollSpeed + 1)), (nUV / nUV_X) * (1.0f / nUV_Y) + (nCntBg_Y * (fScrollSpeed + 1)));
	pVtx[2].tex = D3DXVECTOR2((nUV % nUV_X) * (1.0f / nUV_X) + (nCntBg_X * (fScrollSpeed + 1)), (nUV / nUV_X) * (1.0f / nUV_Y) + (1.0f / nUV_Y) + (nCntBg_Y * (fScrollSpeed + 1)));
	pVtx[3].tex = D3DXVECTOR2((nUV % nUV_X) * (1.0f / nUV_X) + (1.0f / nUV_X) + (nCntBg_X * (fScrollSpeed + 1)), (nUV / nUV_X) * (1.0f / nUV_Y) + (1.0f / nUV_Y) + (nCntBg_Y * (fScrollSpeed + 1)));

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}
