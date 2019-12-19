//=============================================================================
//
// 影処理 [shadow.h]
// Author : Takuto Ishida
//
//=============================================================================
#ifndef _SHADOW_H_
#define _SHADOW_H_

#include "scene.h"
#include "shadow/ZTexCreator.h"
#include "shadow/comptr.h"
#include "shadow/DepthBufShadowEffect.h"
#include "shadow/EffectResource.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define SHADOW_PRIORITY		(6)

//*****************************************************************************
// 前方宣言
//*****************************************************************************

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

	D3DXVECTOR3 GetPos(void) { return m_pos; };
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; };

	static CShadow *Create(void);

private:
	IKD::Com_ptr<ID3DXMesh>			m_pMesh;			// メッシュ情報へのポインタ
	IKD::Com_ptr<ID3DXBuffer>		m_pBuffMat;			// マテリアル情報へのポインタ
	DWORD							m_nNumMat;			// マテリアル情報の数

	D3DXMATRIX						m_mtxWorld;			// ワールドマトリックス
	D3DXVECTOR3						m_pos;				// 位置
	D3DXVECTOR3						m_rot;				// 回転
	IKD::CZTexCreator				m_ZTexCreator;		// Z値テクスチャ作成オブジェクト
	IKD::CDepthBufShadowEffect		m_DepthBS;			// 深度バッファシャドウオブジェクト
	IKD::Com_ptr<IDirect3DTexture9> m_cpShadowMapTex;	// Z値テクスチャ
};

#endif
