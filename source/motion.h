//=============================================================================
//
// モーション管理処理 [motion.h]
// Author : Takuto Ishida
//
//=============================================================================
#ifndef _MOTION_H_
#define _MOTION_H_

#include "scene.h"

#include <stdio.h>

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_FILE		(6)		// ファイル数
#define MOTION_PRIORITY	(0)		// 処理の優先順位
#define ATTACK_MOTION	(CMotionManager::TYPE_MAX - CMotionManager::TYPE_ATTACK01)	// 攻撃モーション数

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CModel;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CMotionManager : public CScene
{// モーション管理クラス
public:
	typedef enum
	{
		TYPE_NEUTRAL = 0,	// ニュートラル
		TYPE_WALK,			// 歩き
		TYPE_BACK,			// バックラン

		TYPE_DAMAGE,		// ダメージ
		TYPE_SLIP,			// 滑り
		TYPE_DEATH,			// 死亡
		TYPE_ATTACK01,		// 攻撃１
		TYPE_ATTACK02,		// 攻撃２
		TYPE_ATTACK03,		// 攻撃３
		TYPE_MAX
	}TYPE;

	typedef struct
	{// キー要素
		int nIdxModel;		// 判定を持たせるモデル番号
		float fPosX;		// X座標
		float fPosY;		// Y座標
		float fPosZ;		// Z座標
		float fRadius;		// 半径
		int nStartFrame;	// 開始フレーム
		int nEndFrame;		// 終了フレーム
	}COLLISION;

	typedef struct
	{// キー要素
		int nIdxModel;		// モデル番号
		float fPosX;		// X座標
		float fPosY;		// Y座標
		float fPosZ;		// Z座標
		int nStartFrame;	// 開始フレーム
		int nEndFrame;		// 終了フレーム
	}ORBIT;

	typedef struct
	{// キー要素
		float posX;
		float posY;
		float posZ;
		float rotX;
		float rotY;
		float rotZ;
	}KEY;

	CMotionManager(int nPriority = MOTION_PRIORITY, CScene::OBJTYPE objType = CScene::OBJTYPE_MOTION_MANAGER);
	~CMotionManager();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CMotionManager *Create(int nFile);

	static HRESULT Load(void);				// テクスチャを読み込む（managerにいれる）
	static void Unload(void);				// 読み込んだテクスチャを解放

	int GetNumKey(int nMotion) { return m_pNumKey[m_nFile][nMotion]; };
	int *GetFlame(int nMotion) { return m_pFrame[m_nFile][nMotion]; };
	bool GetLoop(int nMotion) { return m_pLoop[m_nFile][nMotion]; };
	CMotionManager::KEY **GetKey(int nMotion) { return m_pKey[m_nFile][nMotion]; };
	CMotionManager::COLLISION *GetCollision(int nMotion) { return m_pCollision[m_nFile][nMotion]; };
	CMotionManager::ORBIT *GetOrbit(int nMotion) { return m_pOrbit[m_nFile][nMotion]; };
	int GetType(int nMotion) { return m_pType[m_nFile][nMotion]; };
	void SetFile(int nFile) { m_nFile = nFile; };

	int GetNumMotion(void) { return m_nNumMotion[m_nFile]; };

private:
	static int m_nParts[MAX_FILE];				// パーツ数
	static int *m_pNumKey[MAX_FILE];			// キー数
	static int **m_pFrame[MAX_FILE];			// フレーム
	static KEY ***m_pKey[MAX_FILE];				// パーツごとのキー要素
	static bool *m_pLoop[MAX_FILE];				// ループするかどうか
	static int *m_pType[MAX_FILE];				// モーションの種類
	static COLLISION **m_pCollision[MAX_FILE];	// 当たり判定
	static ORBIT **m_pOrbit[MAX_FILE];			// 軌跡
	static int m_nNumMotion[MAX_FILE];			// モーション数
	int m_nFile;								// ファイルの指定
};

/*========================*/
/*  モーション再生クラス  */
/*========================*/
class CMotion : public CScene
{
public:
	CMotion(int nPriority = MOTION_PRIORITY, CScene::OBJTYPE objType = CScene::OBJTYPE_MOTION);
	~CMotion();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CMotion *Create(int nFile, int nParts, CModel **pModel, int nFootIdx = 0);			// モーション情報の生成

	int GetMotion(void) { return m_nCurMotion; };
	void SetMotion(CMotionManager::TYPE type);

	int GetType(void) { return m_nType; };

	bool Collision(D3DXVECTOR3 pos);

	int GetCurKey(void)		{ return m_nCurKey; };			// 現在のキーの取得
	int GetCurFlame(void)	{ return m_nCntFlame; };		// 現在のフレームを取得
	int GetNumKey(void)		{ return m_nKey; };				// キー数の取得
	int GetAttackIdx(void);									// 攻撃判定のあるモデル

	int GetIdxLowerBody(void) { return m_nFootIdx; };		// 下半身以下の番号の取得
	void SetIdxLowerBody(int nIdx) { m_nFootIdx = nIdx; };	// 下半身以下の番号の設定

private:
	void Motion(void);							// モーションの再生

	CModel **m_pModel;							// モデルクラスのポインタ変数
	int	m_nCurKey;								// 現在のキー
	int *m_pFrame;								// フレーム
	CMotionManager::KEY **m_pKey;				// パーツごとのキー要素
	int m_nKey;									// キー数
	int m_nParts;								// パーツ数
	bool m_bLoop;								// ループするかどうか
	int m_nCntFlame;							// フレームカウント
	D3DXVECTOR3 m_posInit;						// 中心モデルの初期位置
	int m_nCurMotion;							// 現在のモーション
	int m_nType;								// モーションの種類
	CMotionManager::COLLISION *m_pCollision;	// 当たり判定のポインタ
	CMotionManager::ORBIT *m_pOrbit;			// 軌跡
	CMotionManager *m_pMotionManager;			// モーション管理クラスのポインタ
	int m_nCntTotalFlame;						// 総合フレーム数
	bool m_bInit;								// 最初の位置になったかどうか
	int m_nFootIdx;								// 下半身以下の番号
};

#endif