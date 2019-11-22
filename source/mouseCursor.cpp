//=============================================================================
//
// マウスカーソル処理 [mouseCursor.cpp]
// Author : Komatsu Keisuke
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "mouseCursor.h"
#include "manager.h"
#include "renderer.h"
#include "debugproc.h"
#include "input.h"
#include "camera.h"
#include "game.h"
#include "enemy.h"
#include "player.h"
#include "nodePointer.h"
#include "scene2D.h"
#include "texture.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define	LOAD_FILENAME			("data/TEXT/NODE_DATA/NodeData.txt")	// 読み込むファイルのパス
#define CURSOR_TEXTURE			("data/TEXTURE/reticle.png")	// カーソルのテクスチャ
#define MASSAGE_DISPLAY_TIME	(180)							// メッセージの表示時間
#define MOUSE_WIDTH				(30.0f)
#define MOUSE_HEIGHT			(30.0f)

//=============================================================================
// 静的メンバ変数
//=============================================================================
CCamera *CMouseCursor::m_pCamera = NULL;
CNodePointer *CMouseCursor::m_pNodePointer[NODEPOINT_MAX] = {};
D3DXVECTOR3 CMouseCursor2D::m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

//=============================================================================
// 生成処理
//=============================================================================
CMouseCursor  *CMouseCursor::Create()
{
	CMouseCursor *pMouseCursor = NULL;

	if (pMouseCursor == NULL)
	{
		pMouseCursor = new CMouseCursor;

		if (pMouseCursor != NULL)
		{
			pMouseCursor->Init();
		}
	}

	return pMouseCursor;
}

//=============================================================================
// コンストラクタ
//=============================================================================
CMouseCursor::CMouseCursor(int nPriority, CScene::OBJTYPE objType) : CScene3D(nPriority, objType) {}

//=============================================================================
// デストラクタ
//=============================================================================
CMouseCursor::~CMouseCursor() {}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CMouseCursor::Init()
{
	CScene3D::Init();

	// マップ情報の初期化
	m_LoadNodeData = {};
	m_SaveNodeData = {};

	// 接続元ノードの情報の初期化
	m_SelectNodePos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_SelectNodeNumber = 0;
	m_SelectNodeConnectMax = 0;
	for (int nNode = 0; nNode < CONNECT_MAX; nNode++)
	{// 接続可能最大数分回る
		m_SelectNodeConnect[nNode] = 0;
	}

	m_pCamera = CManager::GetCamera();
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR3(15.0f, 0.0f, 15.0f);
	m_bConnectMode = false;
	m_bSaveMassage = false;
	m_bErrorMassage = false;
	m_bConnectSet = true;
	m_bConnectNumSet = false;
	m_nMassageCount = MASSAGE_DISPLAY_TIME;
	m_nNodeCounter = 0;
	m_nConnectNodeCounter = 0;
	m_nSelectNode = 0;
	m_nSelectConnectNode = 0;

	// マウスカーソルの設定
	CScene3D::SetSize(m_size);
	CScene3D::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	CScene3D::BindTexture(CTexture::GetTexture(CTexture::TEXTURE_MOUSECURSOR));

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CMouseCursor::Uninit(void)
{
	CScene3D::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CMouseCursor::Update(void)
{
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();	// キーボードの入力を取得
	CInputMouse *pMouse = CManager::GetInputMouse();	// マウスの入力を取得

	//if (m_bErrorMassage == true)
	//{// エラーメッセージの表示
	//	CDebugProc::Print("====================================\n");
	//	CDebugProc::Print("= ファイルに書き込めませんでした =\n");
	//	CDebugProc::Print("====================================\n\n");
	//}
	//else if (m_bSaveMassage == true)
	//{// セーブ完了メッセージの表示
	//	CDebugProc::Print("===================\n");
	//	CDebugProc::Print("= セーブ正常完了 =\n");
	//	CDebugProc::Print("===================\n\n");
	//}

	CDebugProc::Print("=====マウス用3Dポリゴン======\n");
	CDebugProc::Print("カーソルの座標\n x : %.1f y : %.1f z : %.1f\n", m_pos.x, m_pos.y, m_pos.z);
	CDebugProc::Print("============================\n");
	//CDebugProc::Print("配置してるノードの数: %d\n", m_nNodeCounter);

	//CDebugProc::Print("(LSHIFT)接続設定モード: %s\n", m_bConnectMode ? "ON" : "OFF");

	//if (m_bConnectMode == true)
	//{// 接続設定モードの場合
	//	CDebugProc::Print("(SPACE)接続 [%s] 設定モード\n", m_bConnectSet ? "元" : "先");
	//	if (m_bConnectNumSet == false)
	//	{// 接続元設定モードの場合
	//		CDebugProc::Print("(LCLICK)接続元を設定する\n");
	//	}
	//	else if (m_bConnectNumSet == true)
	//	{// 接続先設定モードの場合
	//		CDebugProc::Print("(LCLICK)接続先を設定する\n");
	//		CDebugProc::Print("(RCLICK)接続先を1つ戻す\n");
	//	}

	//	CDebugProc::Print("ノードの位置 x : %.1f / y : %.1f / z : %.1f\n", m_SelectNodePos.x, m_SelectNodePos.y, m_SelectNodePos.z);
	//	CDebugProc::Print("No. : %d\n", m_SelectNodeNumber);
	//	CDebugProc::Print("接続ノード数 : %d\n", m_SelectNodeConnectMax);
	//	for (int nCntConnect = 0; nCntConnect < m_SelectNodeConnectMax; nCntConnect++)
	//	{
	//		CDebugProc::Print("接続先番号[%d] : %d\n", nCntConnect, m_SelectNodeConnect[nCntConnect]);
	//	}
	//	CDebugProc::Print("\n");

	//	CDebugProc::Print("接続元ノード番号 : %d\n", m_nSelectNode);
	//	CDebugProc::Print("接続先ノード番号 : %d\n", m_nSelectConnectNode);
	//}
	//else if (m_bConnectMode == false)
	//{// ノード配置モードの場合
	//	CDebugProc::Print("(LCLICK)ノードを設定する\n");
	//	CDebugProc::Print("(RCLICK)ノードを1つ戻す\n");
	//}

	if (m_bSaveMassage == true || m_bErrorMassage == true)
	{// メッセージ表示時間の減少
		m_nMassageCount--;

		if (m_nMassageCount <= 0)
		{// メッセージの非表示
			m_bSaveMassage = false;
			m_bErrorMassage = false;
			m_nMassageCount = MASSAGE_DISPLAY_TIME;
		}
	}

	// 入力時処理
	CMouseCursor::Input(pKeyboard, pMouse);

	// 移動時処理
	CMouseCursor::Move(pMouse);

	// データのセーブロード呼び出し
	//CMouseCursor::SaveLoad(pKeyboard);
}

//=============================================================================
// 描画処理
//=============================================================================
void CMouseCursor::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// ライティングOFF
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// ZテストOFF
	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);

	CScene3D::Draw();

	// ZテストON
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);

	// ライティングON
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//=============================================================================
// 入力処理
//=============================================================================
void CMouseCursor::Input(CInputKeyboard *pKeyboard, CInputMouse *pMouse)
{
	bool bSet = false;
	D3DXVECTOR3 SetPos = D3DXVECTOR3(((int)(m_pos.x / 38)) * 38.0f, 0.0f, ((int)(m_pos.z / 36)) * 36.0f);

	//if (CGame::GetCreateMode() == true)
	{// ノーマルモード時
		if (pMouse->GetTrigger(CInputMouse::DIMS_BUTTON_0) == true && CEnemy::GetPartSwitch() == true && pKeyboard->GetPress(DIK_LCONTROL) == false)
		{// ストラテジーパート時に左クリックでAIの目的地を設定
			CEnemy::SetSearchPos(SetPos);
		}
	}
	//	//else if (CGame::GetCreateMode() == false)
	//	{// クリエイティブモード時

	//if (pKeyboard->GetTrigger(DIK_LSHIFT) == true)
	//{// NUMPAD1ボタンで接続編集モードの変更
	//	m_bConnectMode = m_bConnectMode ? false : true;
	//}

	//if (m_bConnectMode == true)
	//{// 接続編集モード時のみ
	//	if (pKeyboard->GetTrigger(DIK_SPACE) == true)
	//	{// Spaceキーで接続元,先モードの変更
	//		m_bConnectSet = m_bConnectSet ? false : true;
	//		m_bConnectNumSet = !m_bConnectSet;
	//	}
	//}

	//		if (m_bConnectMode == false)
	//		{// ノード配置設定モード時
	//			if (pMouse->GetTrigger(CInputMouse::DIMS_BUTTON_0) == true && pKeyboard->GetPress(DIK_LCONTROL) == false)
	//			{//	左クリックでノード配置
	//			 // 目的地設定
	//				for (int nCntNode = 0; nCntNode < m_nNodeCounter; nCntNode++)
	//				{// 配置してるノードの数だけ回る
	//					if (m_pNodePointer[nCntNode] != NULL)
	//					{// 全ノードのNULLチェック
	//						if (m_pNodePointer[nCntNode]->GetPos() == SetPos)
	//						{// すでに置いている場所の場合
	//							bSet = true;
	//						}
	//					}
	//				}
	//
	//				if (m_pNodePointer[m_nNodeCounter] == NULL && bSet == false)
	//				{// ノードポリゴンの追加 ※既に配置済みなら配置できない
	//					m_pNodePointer[m_nNodeCounter] = m_pNodePointer[m_nNodeCounter]->Create(SetPos);
	//					CNodePointer::GetNodeMax()++;
	//					m_nNodeCounter++;
	//				}
	//			}
	//			else if (pMouse->GetTrigger(CInputMouse::DIMS_BUTTON_1) == true && m_nNodeCounter > 0)
	//			{// 右クリックで1つ戻る
	//			 // 接続先の接続数を減らす
	//				CNodePointer::GetNodeMax()--;
	//				m_nNodeCounter--;
	//
	//				if (m_pNodePointer[m_nNodeCounter] != NULL)
	//				{// ノードポリゴンの削除
	//					m_pNodePointer[m_nNodeCounter]->Uninit();
	//					m_pNodePointer[m_nNodeCounter] = NULL;
	//				}
	//			}
	//		}
	//		else if (m_bConnectMode == true)
	//		{// ノード接続設定モード時
	//			if (m_bConnectSet == true)
	//			{// 接続元設定モード時
	//				if (pMouse->GetTrigger(CInputMouse::DIMS_BUTTON_0) == true && pKeyboard->GetPress(DIK_LCONTROL) == false)
	//				{// 左クリックで接続元のノードを設定する
	//					for (int nCntNode = 0; nCntNode < m_nNodeCounter; nCntNode++)
	//					{// 配置してるノードの数だけ回る
	//						if (m_pNodePointer[nCntNode] != NULL)
	//						{// 全ノードのNULLチェック
	//							if (m_pNodePointer[nCntNode]->GetPos() == SetPos)
	//							{// 選択ノードの番号を検索
	//
	//								if (m_pNodePointer[m_nSelectNode]->GetPos() != SetPos)
	//								{// 1個目じゃなければ前回の色を元に戻す
	//									m_pNodePointer[m_nSelectNode]->GetColor() = D3DXCOLOR(1.0f, 0.5f, 0.8f, 0.5f);
	//								}
	//
	//								// 選択ノードの番号を接続元として取得
	//								m_nSelectNode = m_pNodePointer[nCntNode]->GetMyNumber();
	//
	//								// 接続元ノードを選択してるのが分かるように色を変える
	//								m_pNodePointer[m_nSelectNode]->GetColor() = D3DXCOLOR(0.0f, 0.0f, 1.0f, 0.5f);
	//
	//								m_SelectNodePos = m_pNodePointer[m_nSelectNode]->GetPos();
	//								m_SelectNodeNumber = m_pNodePointer[m_nSelectNode]->GetMyNumber();
	//								m_SelectNodeConnectMax = m_pNodePointer[m_nSelectNode]->GetConnectMax();
	//								for (int nCntConnect = 0; nCntConnect < m_pNodePointer[m_nSelectNode]->GetConnectMax(); nCntConnect++)
	//								{
	//									m_SelectNodeConnect[nCntConnect] = m_pNodePointer[m_nSelectNode]->GetConnect(nCntConnect);
	//								}
	//							}
	//						}
	//					}
	//				}
	//			}
	//			else if (m_bConnectNumSet == true)
	//			{// 接続先設定モード時
	//				if (pMouse->GetTrigger(CInputMouse::DIMS_BUTTON_0) == true)
	//				{// 左クリックで接続先のノードを設定する
	//					for (int nCntNode = 0; nCntNode < m_nNodeCounter; nCntNode++)
	//					{// 配置してるノードの数だけ回る
	//						if (m_pNodePointer[nCntNode] != NULL)
	//						{// 全ノードのNULLチェック
	//							if (m_pNodePointer[nCntNode]->GetConnectMax() < CONNECT_MAX && m_pNodePointer[m_nSelectNode]->GetConnectMax() < CONNECT_MAX)
	//							{// 接続できる最大以下の場合のみ
	//								if (m_pNodePointer[m_nSelectConnectNode]->GetPos() != SetPos && m_pNodePointer[m_nSelectNode]->GetPos() != SetPos)
	//								{// 同じ場所や接続元の場合は選択できない
	//									if (m_pNodePointer[nCntNode]->GetPos() == SetPos)
	//									{// 選択ノードの番号を検索
	//
	//										if (m_pNodePointer[m_nSelectConnectNode]->GetPos() != SetPos && m_pNodePointer[m_nSelectNode]->GetPos() != SetPos)
	//										{// 1個目や接続元じゃなければ前回の色を元に戻す
	//											m_pNodePointer[m_nSelectConnectNode]->GetColor() = D3DXCOLOR(1.0f, 0.5f, 0.8f, 0.5f);
	//										}
	//
	//										// 選択ノードの番号を接続先として取得
	//										m_nSelectConnectNode = m_pNodePointer[nCntNode]->GetMyNumber();
	//
	//										// 接続元ノードに接続先ノードの番号を登録する
	//										m_pNodePointer[m_nSelectNode]->GetConnect(m_pNodePointer[m_nSelectNode]->GetConnectMax()) = m_pNodePointer[m_nSelectConnectNode]->GetMyNumber();
	//
	//										// 接続元の接続数を増やす
	//										m_pNodePointer[m_nSelectNode]->GetConnectMax()++;
	//										m_nConnectNodeCounter++;
	//
	//										// 接続先ノードを選択してるのが分かるように色を変える
	//										m_pNodePointer[m_nSelectConnectNode]->GetColor() = D3DXCOLOR(0.0f, 1.0f, 1.0f, 0.5f);
	//									}
	//								}
	//							}
	//						}
	//					}
	//				}
	//
	//				if (pMouse->GetTrigger(CInputMouse::DIMS_BUTTON_1) == true && m_nConnectNodeCounter > 0 && m_nSelectConnectNode > 0)
	//				{// 接続先を1つ以上設定してる時に、右クリックで1つ前の接続先設定に戻る
	//					for (int nCntNode = 0; nCntNode < m_nNodeCounter; nCntNode++)
	//					{// 配置してるノードの数だけ回る
	//						if (m_pNodePointer[nCntNode] != NULL)
	//						{// 全ノードのNULLチェック
	//							if (m_pNodePointer[nCntNode]->GetConnectMax() > 0)
	//							{// 接続できる0より多い場合のみ
	//								if (m_pNodePointer[m_nSelectConnectNode]->GetPos() != SetPos)
	//								{// 接続元の場合は削除できない
	//
	//								 // 接続を削除
	//									m_pNodePointer[nCntNode]->GetConnect(m_pNodePointer[nCntNode]->GetConnectMax()) = NULL;
	//
	//									// 接続先の接続数を減らす
	//									m_pNodePointer[nCntNode]->GetConnectMax()--;
	//									m_nConnectNodeCounter--;
	//
	//									// 減らしたのが分かるように色を変える
	//									m_pNodePointer[nCntNode]->GetColor() = D3DXCOLOR(1.0f, 0.5f, 0.8f, 0.5f);
	//								}
	//							}
	//						}
	//					}
	//				}
	//			}
	//		}
	//	}
}

//=============================================================================
// 移動処理
//=============================================================================
void CMouseCursor::Move(CInputMouse *pMouse)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// デバイスの取得
	D3DVIEWPORT9 viewport;
	D3DXMATRIX matrix;

	if (m_pCamera != NULL)
	{// カメラがNullじゃない
	 // スクリーン座標をワールド座標へ変換
		D3DXVECTOR3 MousePos((float)pMouse->GetPoint().x, (float)pMouse->GetPoint().y, 1);
		viewport = CManager::GetCamera()->GetViewport(1);
		D3DXMatrixIdentity(&matrix);
		D3DXMATRIX proj, view;
		CManager::GetCamera()->GetInfo(&viewport, &proj, &view);
		D3DXVec3Unproject(&MousePos, &MousePos, &viewport, &m_pCamera->GetProjection(), &m_pCamera->GetView(), &matrix);

		// カーソル位置の設定
		m_pos.x = MousePos.x;
		m_pos.y = 0.0f;
		m_pos.z = MousePos.z;

		CDebugProc::Print("スクリーン座標：%.2f %.2f", (float)pMouse->GetPoint().x, (float)pMouse->GetPoint().y);

		//// ズーム倍率に応じたカーソルのサイズ変更
		//switch ((int)m_pCamera->GetZoom())
		//{
		//case 3:
		//	m_size = D3DXVECTOR3(3.5f, 0.0f, 3.5f);
		//	break;
		//case 2:
		//	m_size = D3DXVECTOR3(5.0f, 0.0f, 5.0f);
		//	break;
		//case 1:
		//	m_size = D3DXVECTOR3(10.0f, 0.0f, 10.0f);
		//	break;
		//}
	}

	// 位置・サイズの設定
	CScene3D::SetMousePos(m_pos);
	CScene3D::SetSize(m_size);
}

//=============================================================================
// ルート探索用ファイルの書き出し
//=============================================================================
HRESULT CMouseCursor::FileSave()
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
				if (pScene->GetObjType() == OBJTYPE_NODE)
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
	return S_OK;
}

//=============================================================================
// ルート探索用ファイルの読み込み
//=============================================================================
void CMouseCursor::FileLoad(char* pFileName)
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
	pFile = fopen(pFileName, "r");

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

	m_LoadNodeData = OneState;	// データの代入
}

//=============================================================================
// ファイルのセーブロード呼び出し
//=============================================================================
void CMouseCursor::SaveLoad(CInputKeyboard *pKeyboard)
{
	if (pKeyboard->GetTrigger(DIK_F9) == true)
	{// F9キーでファイルにセーブ
		FileSave();
		m_bSaveMassage = true;
	}

	if (pKeyboard->GetTrigger(DIK_F10) == true)
	{// F10キーでファイルをロード
		CMouseCursor::FileLoad(LOAD_FILENAME);

		// ロードしたデータをもとにノードを配置
		for (int nCntNode = 0; nCntNode < m_LoadNodeData.nodeMax; nCntNode++)
		{// 全ノード数分回る
			m_pNodePointer[nCntNode] = m_pNodePointer[nCntNode]->Create(m_LoadNodeData.pos[nCntNode]);
			m_pNodePointer[nCntNode]->GetNodeMax()++;
			m_LoadNodeData.index[nCntNode] = m_pNodePointer[nCntNode]->GetMyNumber();
			for (int nCntConnectNode = 0; nCntConnectNode < m_LoadNodeData.connectNum[nCntNode]; nCntConnectNode++)
			{// 接続ノード数分回る
				m_pNodePointer[nCntNode]->GetConnectMax()++;
				m_pNodePointer[nCntNode]->GetConnect(nCntConnectNode) = m_LoadNodeData.connectIndex[nCntNode][nCntConnectNode];
			}

			m_nNodeCounter = m_pNodePointer[nCntNode]->GetNodeMax();
		}
	}
}
/****************************************************************/
/*					2Dマウスカーソルのクラス					*/
/****************************************************************/
//=============================================================================
// 生成処理
//=============================================================================
CMouseCursor2D  *CMouseCursor2D::Create()
{
	CMouseCursor2D *pMouseCursor = NULL;

	if (pMouseCursor == NULL)
	{
		pMouseCursor = new CMouseCursor2D;

		if (pMouseCursor != NULL)
		{
			pMouseCursor->Init();
		}
	}

	return pMouseCursor;
}

//=============================================================================
// コンストラクタ
//=============================================================================
CMouseCursor2D::CMouseCursor2D(int nPriority, CScene::OBJTYPE objType) : CScene2D(nPriority, objType) {}

//=============================================================================
// デストラクタ
//=============================================================================
CMouseCursor2D::~CMouseCursor2D() {}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CMouseCursor2D::Init()
{
	CScene2D::Init();

	CScene2D::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	CScene2D::BindTexture(CTexture::GetTexture(CTexture::TEXTURE_MOUSECURSOR));

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CMouseCursor2D::Uninit(void)
{
	CScene2D::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CMouseCursor2D::Update(void)
{
	CInputMouse *pMouse = CManager::GetInputMouse();	// マウスの入力を取得

	// 移動時処理
	Move(pMouse);
}

//=============================================================================
// 描画処理
//=============================================================================
void CMouseCursor2D::Draw(void)
{
	CScene2D::Draw();
}

//=============================================================================
// 移動処理
//=============================================================================
void CMouseCursor2D::Move(CInputMouse *pMouse)
{
	// ウィンドウの矩形を取得
	RECT rect;
	GetWindowRect(CManager::GetRenderer()->GetHWnd(), &rect);

	// さらにサイズ変更を考慮して、現在のサイズで補正（枠サイズ等あるので厳密ではない）
	m_pos.x = float(pMouse->GetPoint().x - rect.left) - 5.0f;
	m_pos.y = float(pMouse->GetPoint().y - rect.top) - 30.0f;

	// 位置の設定
	CScene2D::SetPos(m_pos);

	// 頂点情報の取得
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = CScene2D::GetVtxBuff();

	// 頂点情報を設定
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点データへのポインタを取得
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos = m_pos + D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = m_pos + D3DXVECTOR3(MOUSE_WIDTH, 0.0f, 0.0f);
	pVtx[2].pos = m_pos + D3DXVECTOR3(0.0f, MOUSE_HEIGHT, 0.0f);
	pVtx[3].pos = m_pos + D3DXVECTOR3(MOUSE_WIDTH, MOUSE_HEIGHT, 0.0f);

	//頂点バッファをアンロック
	pVtxBuff->Unlock();

	// 頂点情報の設定
	CScene2D::SetVtxBuff(pVtxBuff);
}