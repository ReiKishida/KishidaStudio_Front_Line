//=============================================================================
//
// 攻撃を受けた方向表示処理 [damageDirection.h]
// Author : Takuto Ishida
//
//=============================================================================
#ifndef _DAMAGEDIRECTION_H_
#define _DAMAGEDIRECTION_H_

#include "scene.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define DAMAGEDIRECTION_PRIORITY	(7)		// 処理の優先番号

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CScene2D;

//*****************************************************************************
// クラス定義
//*****************************************************************************
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 2Dボタンのクラス
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class CDamageDirection : public CScene
{
public:
	typedef enum
	{// 方向
		DIRECTION_FRONT = 0,	// 前
		DIRECTION_BACK,			// 後
		DIRECTION_RIGHT,		// 右
		DIRECTION_LEFT,			// 左
		DIRECTION_MAX
	}DIRECTION;

	CDamageDirection(int nPriority = DAMAGEDIRECTION_PRIORITY, CScene::OBJTYPE objType = CScene::OBJTYPE_DAMAGEDIRECTION);
	~CDamageDirection();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CDamageDirection* Create(void);

	void CreatePolygon(D3DXVECTOR3 dirSelf, D3DXVECTOR3 dirObj);

	bool GetDamageDirection(DIRECTION direction) { return m_bHitDirection[direction]; };

private:
	CScene2D *m_pPolygon[DIRECTION_MAX];	// 方向を示すポリゴン
	int m_nCntLife[DIRECTION_MAX];			// 表示時間のカウンタ
	bool m_bHitDirection[DIRECTION_MAX];	// 当たったかどうか
};
#endif