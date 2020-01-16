//=============================================================================
//
// メッシュフィールド処理 [meshField.h]
// Author : Takuto Ishida
//
//=============================================================================
#ifndef _MESHFIELD_H_
#define _MESHFIELD_H_

#include "scene3DIndex.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MESHFIELD_PRIORITY	(3)		// メッシュフィールドの優先番号

//*****************************************************************************
// 前方宣言
//*****************************************************************************

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CMeshField : public CScene3DIndex
{
public:
	CMeshField(int nPriority = MESHFIELD_PRIORITY, CScene::OBJTYPE objType = CScene::OBJTYPE_FIELD);
	~CMeshField();
	HRESULT Init(int nField);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CMeshField* Create(D3DXVECTOR3 pos, int nField);

	float GetHeight(D3DXVECTOR3 pos);

	D3DXVECTOR3 GetNor(D3DXVECTOR3 pos);

	float GetAngle(D3DXVECTOR3 pos, float fAngle);
	float GetSlipAngle(D3DXVECTOR3 pos);
	bool OnField(D3DXVECTOR3 pos, float fMaxHeight);
	void SetRipples(D3DXVECTOR3 pos, float fRange, float fHeight);

private:
	void LoadVtx(void);			// 頂点情報の読み込み
	void LoadInfo(int nField);	// 地面の情報を取得

	D3DXVECTOR3 m_pos;	// 位置
	int m_nType;		// テクスチャの種類
	int m_nWire;		// ワイヤーフレームを表示するか
	float m_fRipples;	// 波紋

	char m_aFileName[128];	// 頂点情報のバイナリファイル名
};

#endif
