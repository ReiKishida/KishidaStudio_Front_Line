//=============================================================================
//
// ファイル処理 [file.cpp]
// Author : Komatsu Keisuke
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "file.h"
#include "input.h"
#include "debugProc.h"
#include "nodePointer.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define	LOAD_FILENAME	("data/TEXT/NODE_DATA/NodeData.txt")	// 読み込むファイルのパス
#define MASSAGE_DISPLAY_TIME	(180)	// メッセージの表示時間(フレーム)

//=============================================================================
// 静的メンバ変数
//=============================================================================

//=============================================================================
// 生成処理
//=============================================================================
CFile* CFile::Create(void)
{
	CFile *pFile;

	pFile = new CFile;	// メモリを確保

	if (pFile != NULL)
	{// メモリ確保成功
		pFile->Init();
	}

	return pFile;
}

//=============================================================================
// コンストラクタ
//=============================================================================
CFile::CFile(int nPriority, CScene::OBJTYPE objType) : CScene(nPriority, objType)
{
	m_LoadNodeData = {};
	m_SaveNodeData = {};

	m_nMassageCount = MASSAGE_DISPLAY_TIME;
	m_bSaveMassage = false;
	m_bLoadMassage = false;
	m_bErrorMassage = false;
}

//=============================================================================
// デストラクタ
//=============================================================================
CFile::~CFile(){}

//=============================================================================
// 更新処理
//=============================================================================
void CFile::Update()
{
	if (m_bErrorMassage == true)
	{// エラーメッセージの表示
		CDebugProc::Print("====================================\n");
		CDebugProc::Print("= ファイルに書き込めませんでした =\n");
		CDebugProc::Print("====================================\n\n");
	}
	else if (m_bSaveMassage == true)
	{// セーブ完了メッセージの表示
		CDebugProc::Print("===================\n");
		CDebugProc::Print("= セーブ正常完了 =\n");
		CDebugProc::Print("===================\n\n");
	}
	else if (m_bLoadMassage == true)
	{// ロード完了メッセージの表示
		CDebugProc::Print("===================\n");
		CDebugProc::Print("= ロード正常完了 =\n");
		CDebugProc::Print("===================\n\n");
	}

	if (m_bSaveMassage == true || m_bLoadMassage == true || m_bErrorMassage == true)
	{// メッセージ表示時間の減少
		m_nMassageCount--;

		if (m_nMassageCount <= 0)
		{// メッセージの非表示
			m_bSaveMassage = false;
			m_bLoadMassage = false;
			m_bErrorMassage = false;
			m_nMassageCount = MASSAGE_DISPLAY_TIME;
		}
	}
}

//=============================================================================
// ルート探索用ファイルの書き出し
//=============================================================================
HRESULT CFile::FileSave()
{
	FILE* pFile = NULL;		// ファイルポインタ
	int nCntSaveState = 0;

	//敵の基本情報を格納
	std::vector<NODE_SAVE_STATE> SaveState;

	//データの保存
	CScene* pScene = NULL;
	pScene = CScene::GetSceneTop(NODE_PRIORITY);

	//ファイルオープン
	pFile = fopen(LOAD_FILENAME, "w");

	if (pFile != NULL)
	{
		fprintf(pFile, "//====================================================================================\n");
		fprintf(pFile, "//																					  \n");
		fprintf(pFile, "//	マップノード配置用テキスト														  \n");
		fprintf(pFile, "//	Author : Komatsu Keisuke														  \n");
		fprintf(pFile, "//																					  \n");
		fprintf(pFile, "//====================================================================================\n");
		fprintf(pFile, "START_LOAD  // ロード開始\n\n");
		fprintf(pFile, "//========================================\n");
		fprintf(pFile, "// ノードの情報\n");
		fprintf(pFile, "//========================================\n");
		fprintf(pFile, "START_DATA\n\n");

		if (pScene != NULL)
		{
			do
			{
				CScene* pSceneNext = pScene->GetSceneNext();
				if (pScene->GetObjType() == CScene::OBJTYPE_NODE)
				{
					//共通データの保存
					CNodePointer* pNodePointer = (CNodePointer*)pScene;

					fprintf(pFile, "	NODESET // ポイント[%d]\n", pNodePointer->GetMyNumber());
					fprintf(pFile, "		NODE_POS = %.1f %.1f %.1f  // 位置\n", pNodePointer->GetPos().x, pNodePointer->GetPos().y, pNodePointer->GetPos().z);
					fprintf(pFile, "		CONNECT_NUM = %d  // 接続ノード数\n", pNodePointer->GetConnectMax());

					for (int nCntConnectNode = 0; nCntConnectNode < pNodePointer->GetConnectMax(); nCntConnectNode++)
					{// 繋がってるノードの数だけ回る
						fprintf(pFile, "		CONNECT_INDEX = %d  // 接続ノード番号 [%d個目]\n", pNodePointer->GetConnect(nCntConnectNode), nCntConnectNode + 1);
					}
					fprintf(pFile, "	END_NODESET\n\n");
				}
				pScene = pSceneNext;
				nCntSaveState++;

			} while (pScene != NULL);

			fprintf(pFile, "END_DATA\n\n");
			fprintf(pFile, "END_LOAD  // ロード終了\n");
		}

		//ファイルクローズ
		if (pFile != NULL)
		{
			fclose(pFile);
			pFile = NULL;
		}
	}
	else
	{//ここに入ったらエラー(そのファイルがないor開けない)
		m_bErrorMassage = true;
		return S_FALSE;
	}

	m_bSaveMassage = true;
	return S_OK;
}

//=============================================================================
// ルート探索用ファイルの読み込み
//=============================================================================
void CFile::FileLoad()
{
	FILE* pFile = NULL;		// ファイルポインタ
	char ReadText[256];		// 読み込んだ文字列を入れておく
	char HeadText[256];		// 比較用
	char DustBox[256];		// 使用しないものを入れておく
	int nCount = 0;
	int nCntIndex = 0;

	// 一時データベース
	std::vector<NODE_LOAD_STATE> LoadState; LoadState.clear();

	// 初期化
	NODE_LOAD_STATE OneState = {};

	// ファイルオープン
	pFile = fopen(LOAD_FILENAME, "r");

	if (pFile != NULL)
	{// ファイルが開かれていれば
		while (strcmp(HeadText, "START_LOAD") != 0)
		{// "START_LOAD" が読み込まれるまで繰り返し文字列を読み取る
			fgets(ReadText, sizeof(ReadText), pFile);
			sscanf(ReadText, "%s", &HeadText);
		}
		if (strcmp(HeadText, "START_LOAD") == 0)
		{// "START_LOAD" が読み取れた場合、処理開始
			while (strcmp(HeadText, "END_LOAD") != 0)
			{// "END_LOAD" が読み込まれるまで繰り返し文字列を読み取る
				fgets(ReadText, sizeof(ReadText), pFile);
				sscanf(ReadText, "%s", &HeadText);

				if (strcmp(HeadText, "\n") == 0)
				{// 文字列の先頭が [\n](改行) の場合処理しない

				}
				else if (strcmp(HeadText, "START_DATA") == 0)
				{// "START_DATA" が読み取れた場合
					nCount = 0;
					while (strcmp(HeadText, "END_DATA") != 0)
					{// "END_DATA" が読み込まれるまで繰り返し文字列を読み取る
						fgets(ReadText, sizeof(ReadText), pFile);
						sscanf(ReadText, "%s", &HeadText);

						if (strcmp(HeadText, "\n") == 0)
						{// 文字列の先頭が [\n](改行) の場合処理しない

						}
						else if (strcmp(HeadText, "NODESET") == 0)
						{// "NODESET" が読み取れた場合
							while (strcmp(HeadText, "END_NODESET") != 0)
							{// "END_NODESET" が読み込まれるまで繰り返し文字列を読み取る
								fgets(ReadText, sizeof(ReadText), pFile);
								sscanf(ReadText, "%s", &HeadText);

								if (strcmp(HeadText, "\n") == 0)
								{// 文字列の先頭が [\n](改行) の場合処理しない

								}
								else if (strcmp(HeadText, "NODE_POS") == 0)	// ノードの位置
								{
									sscanf(ReadText, "%s %c %f %f %f",
										&DustBox, &DustBox,
										&OneState.pos[nCount].x,
										&OneState.pos[nCount].y,
										&OneState.pos[nCount].z);
								}
								else if (strcmp(HeadText, "CONNECT_NUM") == 0)	// 接続ノードの数
								{
									sscanf(ReadText, "%s %c %d",
										&DustBox, &DustBox,
										&OneState.connectNum[nCount]);
								}
								else if (strcmp(HeadText, "CONNECT_INDEX") == 0)	// 接続ノードの番号
								{
									sscanf(ReadText, "%s %c %d",
										&DustBox, &DustBox,
										&OneState.connectIndex[nCount][nCntIndex]);
									nCntIndex++;
								}
							}

							OneState.index[nCount] = nCount;
							nCntIndex = 0;
							nCount++;
						}
					}
					OneState.nodeMax = nCount; // ノードの総数

											   // 一つのデータを読み込んだ後,一時データベースに格納
					LoadState.emplace_back(OneState);
				}
			}
		}

		// ファイルクローズ
		if (pFile != NULL)
		{
			fclose(pFile);
			pFile = NULL;
		}
	}

	m_NodeData = OneState;	// データの代入
}