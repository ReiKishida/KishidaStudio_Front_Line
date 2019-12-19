//=============================================================================
//
//	戦闘系の処理 [search.h]
// Author : Takuto Ishida
//
//=============================================================================
#ifndef _SEARCH_H_
#define _SEARCH_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// 前方宣言
//*****************************************************************************

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CSearch
{
public:
	CSearch();
	~CSearch();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CSearch *Create(D3DXMATRIX *pParent = NULL);

	D3DXVECTOR3 GetPos(void) { return m_pos; };					// 位置の取得
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; };		// 位置の設定

	D3DXVECTOR3 GetRot(void) { return m_rot; };					// 角度の取得
	 void SetRot(D3DXVECTOR3 rot);

	D3DXMATRIX &GetMtxWorld(void) { return m_mtxWorld; };					// ワールドマトリックスの取得
	void SetMtxWorld(D3DXMATRIX mtxWorld) { m_mtxWorld = mtxWorld; };	// ワールドマトリックスの設定


	CSearch *GetParent(void) { return m_pParent; };					// 親の取得
	void SetParent(CSearch *parent) { m_pParent = parent; };	// 親の設定

	bool Distance(void);	//探索
	static D3DXVECTOR3 Sub_Vector(const D3DXVECTOR3& Pos0, const D3DXVECTOR3& Pos1);		//ベクトルの計算
	static D3DXVECTOR3 Cross_Product(const D3DXVECTOR3& VecA, const D3DXVECTOR3 & VecB);	//外積計算

protected:

private:
	D3DXMATRIX			m_mtxWorld;			// ワールドマトリックス
	D3DXMATRIX			*m_pMtxParent;			// 親のワールドマトリックス
	CSearch				*m_pParent;			// 親のモデル
	D3DXVECTOR3			m_pos;				// 位置
	D3DXVECTOR3			m_rot;				// 回転


	D3DXVECTOR3 m_SearchVec_0;				//認識用ベクトル１
	D3DXVECTOR3 m_SearchVec_1;				//認識用ベクトル２
	D3DXVECTOR3 m_SearchVec_2;				//認識用ベクトル２
	D3DXVECTOR3 m_SearchVec_P;				//認識用ベクトルプレイヤー

};
#endif