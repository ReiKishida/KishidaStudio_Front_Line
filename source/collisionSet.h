//=============================================================================
//
// 当たり判定の設置処理 [collisionSet.h]
// Author : Ishida Takuto
//
//=============================================================================
#ifndef _COLLISIONSET_H_
#define _COLLISIONSET_H_

#include "scene.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define COLLISION_PRIORITY	(2)		// 処理優先順位

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CLine;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CCollision : public CScene
{
public:
	typedef enum
	{
		SIDE_UPPER_FRONT = 0,
		SIDE_UPPER_RIGHT,
		SIDE_UPPER_BEHIND,
		SIDE_UPPER_LEFT,
		SIDE_MIDDLE_LEFT_FRONT,
		SIDE_MIDDLE_RIGHT_FRONT,
		SIDE_MIDDLE_RIGHT_BEHIND,
		SIDE_MIDDLE_LEFT_BEHIND,
		SIDE_LOWER_FRONT,
		SIDE_LOWER_RIGHT,
		SIDE_LOWER_BEHIND,
		SIDE_LOWER_LEFT,
		SIDE_MAX
	}SIDE;

	CCollision(int nPriority = COLLISION_PRIORITY, CScene::OBJTYPE objType = CScene::OBJTYPE_COLLISION);
	~CCollision();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CCollision* Create(D3DXVECTOR3 pos, float fWidth, float fHeight, float fDepth);

	static void Load(void);

	D3DXVECTOR3 GetSize(void) { return D3DXVECTOR3(m_fWidth, m_fHeight, m_fDepth); }
	void SetSize(D3DXVECTOR3 size);

	D3DXVECTOR3 GetPos(void) { return m_pos; }
	void SetPos(D3DXVECTOR3 pos);

	static bool Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 posOld, D3DXVECTOR3 vtxMax, D3DXVECTOR3 vtxMin);
	bool CollisionBox(D3DXVECTOR3 *pPos, D3DXVECTOR3 posOld, D3DXVECTOR3 vtxMax, D3DXVECTOR3 vtxMin);

private:
	D3DXVECTOR3 m_pos;		// 位置
	CLine **m_pLine;		// 判定用ボックス可視化ライン
	float m_fWidth;			// 幅
	float m_fHeight;		// 高さ
	float m_fDepth;			// 奥行
};

#endif
