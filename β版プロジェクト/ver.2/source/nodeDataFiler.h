//=============================================================================
//
// ノードデータ管理処理 [file.h]
// Author : Komatsu Keisuke
//
//=============================================================================
#ifndef _NODEDATAFILER_H_
#define _NODEDATAFILER_H_

#include "main.h"
#include "scene.h"

//=============================================================================
// 前方宣言
//=============================================================================
class CInputKeyboard;
class CNodePointer;

//=============================================================================
// マクロ定義
//=============================================================================
#define FILE_PRIORITY	(0)

//=============================================================================
// クラス定義
//=============================================================================
class CNodeDataFiler : public CScene
{
public:
	typedef struct
	{// ロードデータ
		int nodeMax;									// ノードの総数
		int	index[NODEPOINT_MAX];						// 自分のノード番号
		int connectNum[NODEPOINT_MAX];					// 接続ノード数
		int connectIndex[NODEPOINT_MAX][CONNECT_MAX];	// 接続ノード番号
		D3DXVECTOR3 pos[NODEPOINT_MAX];					// 各ノードの位置
	}NODE_LOAD_STATE;

	typedef struct
	{// セーブデータ
		int nodeMax;			// ノードの総数
		int	index;				// 自分のノード番号
		int connectNum;			// 接続ノード数
		int *connectIndex;		// 接続ノード番号
		D3DXVECTOR3 pos;		// 各ノードの位置
	}NODE_SAVE_STATE;

	CNodeDataFiler(int nPriority = FILE_PRIORITY, CScene::OBJTYPE objType = CScene::OBJTYPE_FILE);
	~CNodeDataFiler();

	static CNodeDataFiler *Create(void);

	HRESULT Init(void) { return S_OK; };
	void Uninit(void) { CScene::Release(); };
	void Update(void);
	void Draw(void) {};

	HRESULT FileSave(char *pFileName);
	void FileLoad(char *pFileName);

	NODE_LOAD_STATE &GetLoadData(void) { return m_LoadNodeData; };
	NODE_SAVE_STATE &GetSaveData(void) { return m_SaveNodeData; };

private:
	NODE_LOAD_STATE m_LoadNodeData;	// ロードしたマップ情報
	NODE_SAVE_STATE m_SaveNodeData;	// セーブするマップ情報

	int m_nMassageCount;	// メッセージを表示する時間
	bool m_bSaveMassage;	// セーブ完了メッセージのONOFF
	bool m_bLoadMassage;	// ロード完了メッセージのONOFF
	bool m_bErrorMassage;	// エラーメッセージのONOFF
};

#endif