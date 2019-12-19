//=============================================================================
//
// 3Dモデルオブジェクト処理 [modelObject.h]
// Author : Ishida Takuto
//
//=============================================================================
#ifndef _MODELOBJECT_H_
#define _MODELOBJECT_H_

#include "scene.h"
#include "sceneX.h"
#include "scene3D.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MODEL_PRIORITY	(3)
#define NUM_MODELINFO	(2)		// モデル情報が必要な数

//*****************************************************************************
// 前方宣言
//*****************************************************************************

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CModelSetManager : public CScene
{
public:
	typedef enum
	{
		TYPE_MODEL = 0,		// モデル
		TYPE_ITEM,			// アイテム
		TYPE_START,			// スタート地点
		TYPE_ENEMY,			// 敵
		TYPE_MAX
	}TYPE;

	CModelSetManager(int nPriority = 0, CScene::OBJTYPE objType = CScene::OBJTYPE_SETMANAGER);
	~CModelSetManager();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static HRESULT Load(int nStage);		// テクスチャを読み込む
	static void Unload(void);				// 読み込んだテクスチャを解放

	static CModelSetManager *Create(void);

	static LPDIRECT3DTEXTURE9	&GetTexture(TYPE type, int nType) { return m_pTexture[type][nType]; };		// テクスチャ情報の取得
	static LPD3DXMESH			&GetMesh(TYPE type, int nType) { return m_pMesh[type][nType]; };			// メッシュ情報の取得
	static LPD3DXBUFFER			&GetBuffMat(TYPE type, int nType) { return m_pBuffMat[type][nType]; };		// マテリアル情報の取得
	static DWORD				&GetNumMat(TYPE type, int nType) { return m_pNumMat[type][nType]; };		// マテリアル数の取得

	static int					GetNumModel(void) { return m_nNumAllModel; };					// モデル数の取得
	static void					SetNumModel(int nNumModel) { m_nNumAllModel = nNumModel; };	// モデル数の設定
	static void					AddNumModel(int nValue) { m_nNumAllModel += nValue; };			// モデル数の加算

	static int					GetNumType(TYPE type) { return m_nNumType[type]; };					// 種類の数の取得

	static char					**&GetModelFileName(TYPE type) { return m_apModelName[type]; };

	static D3DXVECTOR3			GetStartPos(void) { return m_StartPos; };
	static D3DXVECTOR3			GetGoalPos(void) { return m_GoalPos; };

protected:

private:
	static void NumObjectType(TYPE type);					// モデル情報を作成するためのメモリ確保
	static void CreateModelInfo(TYPE type, LPDIRECT3DDEVICE9 pDevice, char *pModelFileName, int nCntModel);	// モデル情報の作成

	static LPDIRECT3DTEXTURE9	*m_pTexture[NUM_MODELINFO];		// テクスチャへのポインタ
	static LPD3DXMESH			*m_pMesh[NUM_MODELINFO];		// メッシュ情報へのポインタ
	static LPD3DXBUFFER			*m_pBuffMat[NUM_MODELINFO];		// マテリアル情報へのポインタ
	static DWORD				*m_pNumMat[NUM_MODELINFO];		// マテリアル情報の数

	static int					m_nNumType[TYPE_MAX];		// 設置モデルの種類の数
	static int					m_nNumAllModel;				// モデルの合計数
	static char					**m_apModelName[TYPE_MAX];	// モデルの名前
	static D3DXVECTOR3			m_StartPos;					// スタート地点
	static D3DXVECTOR3			m_GoalPos;					// ゴール地点
};

/*===============================*/
/*			障害物クラス		 */
/*===============================*/
class CModelObject : public CSceneX
{
public:
	CModelObject(int nPriority = MODEL_PRIORITY, CScene::OBJTYPE objType = CScene::OBJTYPE_MODELOBJECT);
	~CModelObject();
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, CModelSetManager::TYPE type, int nType);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CModelObject *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, CModelSetManager::TYPE type, int nType);

	int GetType(void) { return m_nType; };

	bool Collision(D3DXVECTOR3 pos, float fRange);
	bool Collision(D3DXVECTOR3 *pPos, float fRange);

protected:

private:
	float	m_fShadowRange;		// 影の範囲
	int		m_nType;			// モデルの種類
};

/*===============================*/
/*			アイテムクラス		 */
/*===============================*/
class CItem : public CSceneX
{
public:
	CItem(int nPriority = MODEL_PRIORITY, CScene::OBJTYPE objType = CScene::OBJTYPE_ITEM);
	~CItem();
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, CModelSetManager::TYPE type, int nType);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CItem *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, CModelSetManager::TYPE type, int nType);

	bool Collision(D3DXVECTOR3 pos, float fRange);

protected:

private:
	float	m_fShadowRange;		// 影の範囲
	int		m_nType;			// アイテムの種類
};

#endif