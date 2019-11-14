//=============================================================================
//
// 影処理 [shadow.h]
// Author : Takuto Ishida
//
//=============================================================================
#ifndef _SHADOW_H_
#define _SHADOW_H_

#include "scene.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define SHADOW_PRIORITY		(4)

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CPolygon;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CShadow : public CScene
{
public:
	CShadow(int nPriority = SHADOW_PRIORITY, CScene::OBJTYPE objType = CScene::OBJTYPE_SHADOW);
	~CShadow();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CShadow *Create(D3DXVECTOR3 *pPos);

private:
	LPD3DXMESH				m_pMesh;			// メッシュ情報へのポインタ
	LPD3DXBUFFER			m_pBuffMat;			// マテリアル情報へのポインタ
	DWORD					m_nNumMat;			// マテリアル情報の数

	D3DXMATRIX				m_mtxWorld;		// ワールドマトリックス
	D3DXVECTOR3				*m_pos;			// 位置
	D3DXVECTOR3				m_rot;			// 回転
	CPolygon				*m_pBigScreen;	// 画面を塗りつぶす用
};

#endif
