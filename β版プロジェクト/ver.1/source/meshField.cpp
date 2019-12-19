//=============================================================================
//
// メッシュフィールド処理 [meshField.cpp]
// Author : Takuto Ishida
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "meshField.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "debugProc.h"
#include "texture.h"

#include <stdio.h>

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================

//==================================
// 生成処理
//==================================
CMeshField* CMeshField::Create(D3DXVECTOR3 pos, int nField)
{
	CMeshField *pMeshField;

	pMeshField = new CMeshField;	// メモリを確保

	if (pMeshField != NULL)
	{// メモリ確保成功
		pMeshField->m_pos = pos;
		pMeshField->Init(nField);
	}

	return pMeshField;
}

//=========================================
// コンストラクタ
//=========================================
CMeshField::CMeshField(int nPriority, CScene::OBJTYPE objType) : CScene3DIndex(nPriority, objType)
{
	m_nWire = 0;
	m_fRipples = 0.0f;
	m_nType = 0;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//=========================================
// デストラクタ
//=========================================
CMeshField::~CMeshField()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CMeshField::Init(int nField)
{
	// 種類の設定
	CScene3DIndex::SetType(CScene3DIndex::TYPE_FIELD);

	// 初期化処理
	CScene3DIndex::Init();

	// 頂点の情報読み込み
	LoadInfo(nField);

	// 頂点の作成
	CScene3DIndex::MakeVertex();

	// 頂点情報の読み込み
	LoadVtx();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CMeshField::Uninit(void)
{
	// 終了処理
	CScene3DIndex::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CMeshField::Update(void)
{
	CScene3DIndex::Update();

	// キーボードの入力を取得
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();

	if (pKeyboard->GetTrigger(DIK_F8) == true)
	{// ワイヤーフレーム表示
		m_nWire = (1 + m_nWire) % 3;
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CMeshField::Draw(void)
{
	// デバイスの取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	// ワイヤーフレームを表示
	pDevice->SetRenderState(D3DRS_FILLMODE, m_nWire);

	CScene3DIndex::Draw();

	// ワイヤーフレームを非表示
	pDevice->SetRenderState(D3DRS_FILLMODE, 0);
}

//=========================================
// 地面の高さの取得
//=========================================
float CMeshField::GetHeight(D3DXVECTOR3 pos)
{
	float fHeight = 0.0f;	// 返す高さ
	float fDiff = 0.0f;		// 判定用にY軸下げた分を保存
	int nPosBrock[2];		// プレイヤーのいるポリゴンのブロック
	int nVtxNumber[4];		// ブロックの頂点番号

	// プレイヤーのベクトルを求める
	D3DXVECTOR3 vecPlayer = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	float fWidth, fDepth;	// 頂点間の距離
	int nMeshX, nMeshZ;		// 分割数

	// データの取得
	CScene3DIndex::GetInfo(&fWidth, NULL, &fDepth, &nMeshX, NULL, &nMeshZ);

	// 頂点情報を設定
	VERTEX_3D *pVtx;	// 頂点情報のポインタ

	// 頂点バッファの取得
	LPDIRECT3DVERTEXBUFFER9 vtxBuff = CScene3DIndex::GetVtxBuff();

	//頂点バッファをロックし、頂点データへのポインタを取得
	vtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	D3DXVECTOR3 vecA, vecB, collisionNor, collisionPos[3];

	// プレイヤーのいるポリゴン判定
	nPosBrock[0] = (int)(pos.x + (fWidth * 0.5f)) / ((int)fWidth / nMeshX);		// X軸のブロック割り出し
	nPosBrock[1] = (int)(pos.z - (fDepth * 0.5f)) / ((int)-fDepth / nMeshX);	// Z軸のブロック割り出し

	if (nPosBrock[0] >= nMeshX || (int)(pos.x + (fWidth * 0.5f)) < 0 ||
		nPosBrock[1] >= nMeshZ || (int)(pos.z - (fDepth * 0.5f)) > 0)
	{// ポリゴンに乗っていないとき
		CDebugProc::Print("乗っていない");
		return 0;
	}

	// 頂点番号を計算
	nVtxNumber[0] = nPosBrock[0] + ((nPosBrock[1] + 1) * (nMeshX + 1));			// 自分のブロックの左下番号
	nVtxNumber[1] = nPosBrock[0] + (nPosBrock[1] * (nMeshX + 1));					// 自分のブロックの左上番号
	nVtxNumber[2] = (nPosBrock[0] + 1) + ((nPosBrock[1] + 1) * (nMeshX + 1));		// 自分のブロックの右下番号
	nVtxNumber[3] = (nPosBrock[0] + 1) + (nPosBrock[1] * (nMeshX + 1));			// 自分のブロックの右上番号
	CDebugProc::Print("%d %d", nPosBrock[0], nPosBrock[1]);

																					// 外積で左右どちらのポリゴンか判別
	vecA = pVtx[nVtxNumber[2]].pos - pVtx[nVtxNumber[1]].pos;	// 頂点から目的の頂点への方向
	vecB = pos - pVtx[nVtxNumber[1]].pos;						// プレイヤー　－　vecAの始まりの頂点

	if ((vecA.z * vecB.x) - (vecA.x * vecB.z) >= 0)
	{// 左側
	 // 判定用頂点座標
		collisionPos[0] = pVtx[nVtxNumber[0]].pos;
		collisionPos[1] = pVtx[nVtxNumber[1]].pos;
		collisionPos[2] = pVtx[nVtxNumber[2]].pos;
	}
	else
	{// 右側
	 // 判定用頂点座標
		collisionPos[0] = pVtx[nVtxNumber[3]].pos;
		collisionPos[1] = pVtx[nVtxNumber[2]].pos;
		collisionPos[2] = pVtx[nVtxNumber[1]].pos;
	}

	// 判定用に下げる値を保存
	fDiff = collisionPos[0].y;

	// 中心の頂点の高さが0になるように引く
	collisionPos[0].y -= fDiff;
	collisionPos[1].y -= fDiff;
	collisionPos[2].y -= fDiff;

	// 判定用法線
	vecA = collisionPos[1] - collisionPos[0];	// 左側
	vecB = collisionPos[2] - collisionPos[0];	// 右側
	D3DXVec3Cross(&collisionNor, &vecA, &vecB);			// 外積を求める
	D3DXVec3Normalize(&collisionNor, &collisionNor);	// 正規化

	// プレイヤーのベクトル
	vecPlayer = pos - collisionPos[0];

	// プレイヤーのY軸を求める
	fHeight = (-(collisionNor.x * vecPlayer.x) - (collisionNor.z * vecPlayer.z)) / collisionNor.y;

	// 判定用に下げた分を補正
	fHeight += fDiff;

	// 頂点バッファをアンロック
	vtxBuff->Unlock();

	return fHeight;
}

//=========================================================================================================================
// 傾斜による移動量の取得
//=========================================================================================================================
float CMeshField::GetAngle(D3DXVECTOR3 pos, float fRot)
{
	float fSpeed = 0.0f;	// 返す速度
	int nPosBrock[2];		// プレイヤーのいるポリゴンのブロック
	int nVtxNumber[4];		// ブロックの頂点番号
	D3DXVECTOR3 vecPlayer;	// プレイヤーの向きのベクトル
	float fAngle;			// プレイヤーの向きと法線のなす角

	float fWidth, fDepth;	// 頂点間の距離
	int nMeshX, nMeshZ;		// 分割数

	// データの取得
	CScene3DIndex::GetInfo(&fWidth, NULL, &fDepth, &nMeshX, NULL, &nMeshZ);

	// 頂点情報を設定
	VERTEX_3D *pVtx;	// 頂点情報のポインタ

	// 頂点バッファの取得
	LPDIRECT3DVERTEXBUFFER9 vtxBuff = CScene3DIndex::GetVtxBuff();

	//頂点バッファをロックし、頂点データへのポインタを取得
	vtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	D3DXVECTOR3 vecA, vecB, collisionNor, collisionPos[3];

	// プレイヤーのいるポリゴン判定
	nPosBrock[0] = (int)(pos.x + (fWidth * 0.5f)) / (int)fWidth;		// X軸のブロック割り出し
	nPosBrock[1] = (int)(pos.z - (fDepth * 0.5f)) / (int)fDepth * -1;	// Z軸のブロック割り出し

	if (nPosBrock[0] >= nMeshX || (int)(pos.x + (fWidth * 0.5f)) < 0 ||
		nPosBrock[1] >= nMeshZ || (int)(pos.z - (fDepth * 0.5f)) > 0)
	{// ポリゴンに乗っていないとき
		return 0;
	}

	// 頂点番号を計算
	nVtxNumber[0] = nPosBrock[0] + ((nPosBrock[1] + 1) * (nMeshX + 1));			// 自分のブロックの左下番号
	nVtxNumber[1] = nPosBrock[0] + (nPosBrock[1] * (nMeshX + 1));					// 自分のブロックの左上番号
	nVtxNumber[2] = (nPosBrock[0] + 1) + ((nPosBrock[1] + 1) * (nMeshX + 1));		// 自分のブロックの右下番号
	nVtxNumber[3] = (nPosBrock[0] + 1) + (nPosBrock[1] * (nMeshX + 1));			// 自分のブロックの右上番号

																					// 外積で左右どちらのポリゴンか判別
	vecA = pVtx[nVtxNumber[2]].pos - pVtx[nVtxNumber[1]].pos;	// 頂点から目的の頂点への方向
	vecB = pos - pVtx[nVtxNumber[1]].pos;						// プレイヤー　－　vecAの始まりの頂点

	if ((vecA.z * vecB.x) - (vecA.x * vecB.z) >= 0)
	{// 左側
	 // 判定用頂点座標
		collisionPos[0] = pVtx[nVtxNumber[0]].pos;
		collisionPos[1] = pVtx[nVtxNumber[1]].pos;
		collisionPos[2] = pVtx[nVtxNumber[2]].pos;
	}
	else
	{// 右側
	 // 判定用頂点座標
		collisionPos[0] = pVtx[nVtxNumber[3]].pos;
		collisionPos[1] = pVtx[nVtxNumber[2]].pos;
		collisionPos[2] = pVtx[nVtxNumber[1]].pos;
	}

	// プレイヤーのいるポリゴンの法線を出す
	vecA = collisionPos[1] - collisionPos[0];	// 左側
	vecB = collisionPos[2] - collisionPos[0];	// 右側
	D3DXVec3Cross(&collisionNor, &vecA, &vecB);			// 外積を求める
	D3DXVec3Normalize(&collisionNor, &collisionNor);	// 正規化

														// プレイヤーの向きのベクトルの生成
	vecPlayer = D3DXVECTOR3(pos.x + sinf(fRot), pos.y, pos.z + cosf(fRot)) - pos;

	// プレイヤーの向きと法線のなす角を求める
	fAngle = (vecPlayer.x * collisionNor.x) + (vecPlayer.y * collisionNor.y) + (vecPlayer.z * collisionNor.z);
	fAngle = fAngle / (sqrtf(powf(vecPlayer.x, 2) + powf(vecPlayer.y, 2) + powf(vecPlayer.z, 2)) * (sqrtf(powf(collisionNor.x, 2) + powf(collisionNor.y, 2) + powf(collisionNor.z, 2))));

	return fAngle;
}

//=========================================================================================================================
// 滑る向きの取得
//=========================================================================================================================
float CMeshField::GetSlipAngle(D3DXVECTOR3 pos)
{
	int nPosBrock[2];		// プレイヤーのいるポリゴンのブロック
	int nVtxNumber[4];		// ブロックの頂点番号
	D3DXVECTOR3 vecPlayer;	// プレイヤーの向きのベクトル
	float fAngle;			// プレイヤーの向きと法線のなす角
	D3DXVECTOR3 vecA, vecB, collisionNor;	// 計算用ベクトル

	float fWidth, fDepth;	// 頂点間の距離
	int nMeshX, nMeshZ;		// 分割数

	// データの取得
	CScene3DIndex::GetInfo(&fWidth, NULL, &fDepth, &nMeshX, NULL, &nMeshZ);

	// 頂点情報を設定
	VERTEX_3D *pVtx;	// 頂点情報のポインタ

	// 頂点バッファの取得
	LPDIRECT3DVERTEXBUFFER9 vtxBuff = CScene3DIndex::GetVtxBuff();

	//頂点バッファをロックし、頂点データへのポインタを取得
	vtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// プレイヤーのいるポリゴン判定
	nPosBrock[0] = (int)(pos.x + (fWidth * 0.5f)) / (int)fWidth;		// X軸のブロック割り出し
	nPosBrock[1] = (int)(pos.z - (fDepth * 0.5f)) / (int)fDepth * -1;	// Z軸のブロック割り出し

	if (nPosBrock[0] >= nMeshX || (int)(pos.x + (fWidth * 0.5f)) < 0 ||
		nPosBrock[1] >= nMeshZ || (int)(pos.z - (fDepth * 0.5f)) > 0)
	{// ポリゴンに乗っていないとき
		return 0;
	}

	// 頂点番号を計算
	nVtxNumber[0] = nPosBrock[0] + ((nPosBrock[1] + 1) * (nMeshX + 1));			// 自分のブロックの左下番号
	nVtxNumber[1] = nPosBrock[0] + (nPosBrock[1] * (nMeshX + 1));					// 自分のブロックの左上番号
	nVtxNumber[2] = (nPosBrock[0] + 1) + ((nPosBrock[1] + 1) * (nMeshX + 1));		// 自分のブロックの右下番号
	nVtxNumber[3] = (nPosBrock[0] + 1) + (nPosBrock[1] * (nMeshX + 1));			// 自分のブロックの右上番号

	// 外積で左右どちらのポリゴンか判別
	vecA = pVtx[nVtxNumber[2]].pos - pVtx[nVtxNumber[1]].pos;	// 頂点から目的の頂点への方向
	vecB = pos - pVtx[nVtxNumber[1]].pos;						// プレイヤー　－　vecAの始まりの頂点

	if ((vecA.z * vecB.x) - (vecA.x * vecB.z) >= 0)
	{// 左側
		collisionNor = pVtx[nVtxNumber[0]].nor;	// 頂点の法線を保存
	}
	else
	{// 右側
		collisionNor = pVtx[nVtxNumber[3]].nor;	// 頂点の法線を保存
	}

	// ベクトルの角度を取得
	fAngle = acosf(collisionNor.x / sqrtf((collisionNor.x * collisionNor.x) + (collisionNor.y * collisionNor.y)));
	if (collisionNor.y < 0)
	{// -だった時
		fAngle = 2.0f * D3DX_PI - fAngle;
	}

	if (collisionNor.x < 0)
	{// 符号を地面にそろえる
		fAngle *= -1.0f;
	}

	if (isnan(fAngle))
	{// nanチェック
		fAngle = 0.0f;
	}

	// 一回転しないように角度を調節
	if (fAngle > D3DX_PI)
	{
		fAngle -= D3DX_PI * 2.0f;
	}
	if (fAngle < -D3DX_PI)
	{
		fAngle += D3DX_PI * 2.0f;
	}

	return fAngle;
}

//=========================================================================================================================
// 法線の取得
//=========================================================================================================================
D3DXVECTOR3 CMeshField::GetNor(D3DXVECTOR3 pos)
{
	float fDiff = 0.0f;		// 判定用にY軸下げた分を保存
	int nPosBrock[2];		// プレイヤーのいるポリゴンのブロック
	int nVtxNumber[4];		// ブロックの頂点番号

	// プレイヤーのベクトルを求める
	D3DXVECTOR3 vecPlayer = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	float fWidth, fDepth;	// 頂点間の距離
	int nMeshX, nMeshZ;		// 分割数

	// データの取得
	CScene3DIndex::GetInfo(&fWidth, NULL, &fDepth, &nMeshX, NULL, &nMeshZ);

	// 頂点情報を設定
	VERTEX_3D *pVtx;	// 頂点情報のポインタ

	// 頂点バッファの取得
	LPDIRECT3DVERTEXBUFFER9 vtxBuff = CScene3DIndex::GetVtxBuff();

	//頂点バッファをロックし、頂点データへのポインタを取得
	vtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	D3DXVECTOR3 vecA, vecB, collisionNor, collisionPos[3];

	// プレイヤーのいるポリゴン判定
	nPosBrock[0] = (int)(pos.x + (fWidth * 0.5f)) / (int)fWidth;		// X軸のブロック割り出し
	nPosBrock[1] = (int)(pos.z - (fDepth * 0.5f)) / (int)fDepth * -1;	// Z軸のブロック割り出し

	if (nPosBrock[0] >= nMeshX || (int)(pos.x + (fWidth * 0.5f)) < 0 ||
		nPosBrock[1] >= nMeshZ || (int)(pos.z - (fDepth * 0.5f)) > 0)
	{// ポリゴンに乗っていないとき
		return D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	// 頂点番号を計算
	nVtxNumber[0] = nPosBrock[0] + ((nPosBrock[1] + 1) * (nMeshX + 1));			// 自分のブロックの左下番号
	nVtxNumber[1] = nPosBrock[0] + (nPosBrock[1] * (nMeshX + 1));					// 自分のブロックの左上番号
	nVtxNumber[2] = (nPosBrock[0] + 1) + ((nPosBrock[1] + 1) * (nMeshX + 1));		// 自分のブロックの右下番号
	nVtxNumber[3] = (nPosBrock[0] + 1) + (nPosBrock[1] * (nMeshX + 1));			// 自分のブロックの右上番号

																					// 外積で左右どちらのポリゴンか判別
	vecA = pVtx[nVtxNumber[2]].pos - pVtx[nVtxNumber[1]].pos;	// 頂点から目的の頂点への方向
	vecB = pos - pVtx[nVtxNumber[1]].pos;						// プレイヤー　－　vecAの始まりの頂点

	if ((vecA.z * vecB.x) - (vecA.x * vecB.z) >= 0)
	{// 左側
	 // 判定用頂点座標
		collisionPos[0] = pVtx[nVtxNumber[0]].pos;
		collisionPos[1] = pVtx[nVtxNumber[1]].pos;
		collisionPos[2] = pVtx[nVtxNumber[2]].pos;
	}
	else
	{// 右側
	 // 判定用頂点座標
		collisionPos[0] = pVtx[nVtxNumber[3]].pos;
		collisionPos[1] = pVtx[nVtxNumber[2]].pos;
		collisionPos[2] = pVtx[nVtxNumber[1]].pos;
	}

	// 判定用法線
	vecA = collisionPos[1] - collisionPos[0];	// 左側
	vecB = collisionPos[2] - collisionPos[0];	// 右側
	D3DXVec3Cross(&collisionNor, &vecA, &vecB);			// 外積を求める
	D3DXVec3Normalize(&collisionNor, &collisionNor);	// 正規化

	// 頂点バッファをアンロック
	vtxBuff->Unlock();

	return collisionNor;
}

//=========================================================================================================================
// 現在いる地面かどうかの判定
//=========================================================================================================================
bool CMeshField::OnField(D3DXVECTOR3 pos, float fMaxHeight)
{
	// プレイヤーとメッシュフィールドの位置の差分を出す
	D3DXVECTOR3 posDiff = pos - m_pos;

	int nPosBrock[2];		// プレイヤーのいるポリゴンのブロック
	int nVtxNumber[4];		// ブロックの頂点番号
	float fWidth, fDepth;	// 頂点間の距離
	int nMeshX, nMeshZ;		// 分割数

	// データの取得
	CScene3DIndex::GetInfo(&fWidth, NULL, &fDepth, &nMeshX, NULL, &nMeshZ);

	// プレイヤーのいるポリゴン判定
	nPosBrock[0] = (int)(pos.x + (fWidth * 0.5f)) / (int)fWidth;		// X軸のブロック割り出し
	nPosBrock[1] = (int)(pos.z - (fDepth * 0.5f)) / (int)fDepth * -1;	// Z軸のブロック割り出し

	// 頂点番号を計算
	nVtxNumber[0] = nPosBrock[0] + ((nPosBrock[1] + 1) * (nMeshX + 1));			// 自分のブロックの左下番号
	nVtxNumber[1] = nPosBrock[0] + (nPosBrock[1] * (nMeshX + 1));				// 自分のブロックの左上番号
	nVtxNumber[2] = (nPosBrock[0] + 1) + ((nPosBrock[1] + 1) * (nMeshX + 1));	// 自分のブロックの右下番号
	nVtxNumber[3] = (nPosBrock[0] + 1) + (nPosBrock[1] * (nMeshX + 1));			// 自分のブロックの右上番号

	//頂点情報へのポインタ
	VERTEX_3D *pVtx;

	// 頂点バッファの取得
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = CScene3DIndex::GetVtxBuff();

	//頂点バッファをロックし頂点データへのポインタを取得
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//現在の乗っているブロックの頂点
	int nMostUnder, nUnder0, nUnder1;

	if (pVtx[nVtxNumber[0]].pos.y < pVtx[nVtxNumber[1]].pos.y) { nUnder0 = nVtxNumber[0]; }
	else { nUnder0 = nVtxNumber[1]; }

	if (pVtx[nVtxNumber[2]].pos.y < pVtx[nVtxNumber[3]].pos.y) { nUnder1 = nVtxNumber[2]; }
	else { nUnder1 = nVtxNumber[3]; }

	if (pVtx[nUnder0].pos.y < pVtx[nUnder1].pos.y) { nMostUnder = nUnder0; }
	else { nMostUnder = nUnder1; }

	if (pVtx[nMostUnder].pos.y + m_pos.y > pos.y + fMaxHeight + 10.0f)
	{
		// 頂点バッファをアンロック
		pVtxBuff->Unlock();

		return false;
	}

	// 頂点バッファをアンロック
	pVtxBuff->Unlock();

	return true;
}

//=============================================================================
// 地面の波紋処理
//=============================================================================
void CMeshField::SetRipples(D3DXVECTOR3 pos, float fRange, float fHeight)
{
	// 頂点情報を設定
	VERTEX_3D *pVtx;	// 頂点情報のポインタ

	// 頂点バッファの取得
	LPDIRECT3DVERTEXBUFFER9 vtxBuff = CScene3DIndex::GetVtxBuff();

	//頂点バッファをロックし、頂点データへのポインタを取得
	vtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 波紋を動かす
	m_fRipples += 0.08f;

	// 頂点数の取得
	int nNumVtx = CScene3DIndex::GetNumVtx();

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{// 全頂点の距離を計算
		float fLength = powf(pVtx[nCntVtx].pos.x - pos.x, 2.0f) + powf(pVtx[nCntVtx].pos.z - pos.z, 2.0f);

		if (fLength <= powf(fRange, 2.0f))
		{// 範囲内にある頂点
			float fVtxHeight = sinf(D3DX_PI * ((sqrtf(fLength) / fRange) * -14.0f) + m_fRipples) * (fHeight * (1.0f - (sqrtf(fLength) / fRange)));

			pVtx[nCntVtx].pos.y = fVtxHeight;
		}
	}

	// 頂点バッファをアンロックする
	vtxBuff->Unlock();
}

//=========================================================================================================================
// 地面の情報を取得
//=========================================================================================================================
void CMeshField::LoadInfo(int nField)
{
	FILE *pFile = NULL;	// ファイルのポインタ変数
	char aStr[128];	// 取得する文字列

	float fWidth = 0.0f, fDepth = 0.0f;		// 頂点間の距離
	int nBlockX = 0, nBlockZ = 0;			// 分割数
	char *FileName[1] =
	{
		"data/TEXT/FIELD/field.txt"
	};

	// ファイルを開く
	pFile = fopen(FileName[nField], "r");

	if (NULL != pFile)
	{// ファイルがあった
		while (strcmp(aStr, "END_FIELDSET") != 0)
		{
			fscanf(pFile, "%s", aStr);
			if (strcmp(aStr, "WIDTH") == 0)
			{// 幅
				fscanf(pFile, " = %f", &fWidth);
			}
			if (strcmp(aStr, "DEPTH") == 0)
			{// 奥行
				fscanf(pFile, " = %f", &fDepth);
			}
			if (strcmp(aStr, "XBLOCK") == 0)
			{// 横の分割数
				fscanf(pFile, " = %d", &nBlockX);
			}
			if (strcmp(aStr, "ZBLOCK") == 0)
			{// 奥行の分割数
				fscanf(pFile, " = %d", &nBlockZ);
			}
			if (strcmp(aStr, "VERTEX_FILENAME") == 0)
			{// 読み込むバイナリファイル
				fscanf(pFile, " = %s", &m_aFileName[0]);
			}
		}

		// データの設定
		CScene3DIndex::SetInfo(fWidth, 0.0f, fDepth, nBlockX, 0, nBlockZ);

		// ファイルを閉じる
		fclose(pFile);
	}
	else
	{// ファイルが開けない
		MessageBox(0, "ファイルがありません！", "field.txt", MB_OK);
	}
}

//=========================================================================================================================
// 頂点情報をファイルから読み込む
//=========================================================================================================================
void CMeshField::LoadVtx(void)
{
	// ファイルのポインタ変数
	FILE *pFile;

	// ファイルを開く
	pFile = fopen(&m_aFileName[0], "rb");

	if (NULL != pFile)
	{// ファイルがあったとき
		VERTEX_3D *pVtx;	// 頂点情報へのポインタ

		// 頂点バッファの取得
		LPDIRECT3DVERTEXBUFFER9 vtxBuff = CScene3DIndex::GetVtxBuff();

		// 頂点バッファをロックし、頂点データへのポインタを取得
		vtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点数の取得
		int nNumVtx = CScene3DIndex::GetNumVtx();

		// 頂点情報をファイルに書き込む
		fread(pVtx, sizeof(VERTEX_3D), nNumVtx, pFile);

		// 頂点バッファをアンロック
		vtxBuff->Unlock();

		// 頂点バッファの設定
		CScene3DIndex::SetVtxBuff(vtxBuff);

		// ファイルを閉じる
		fclose(pFile);
	}
	else
	{// ファイルが開けない
		MessageBox(0, "ファイルがありません！", "field.bin", MB_OK);
	}
}