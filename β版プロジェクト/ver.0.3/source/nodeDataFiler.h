//=============================================================================
//
// �m�[�h�f�[�^�Ǘ����� [file.h]
// Author : Komatsu Keisuke
//
//=============================================================================
#ifndef _NODEDATAFILER_H_
#define _NODEDATAFILER_H_

#include "main.h"
#include "scene.h"

//=============================================================================
// �O���錾
//=============================================================================
class CInputKeyboard;
class CNodePointer;

//=============================================================================
// �}�N����`
//=============================================================================
#define FILE_PRIORITY	(0)

//=============================================================================
// �N���X��`
//=============================================================================
class CNodeDataFiler : public CScene
{
public:
	typedef struct
	{// ���[�h�f�[�^
		int nodeMax;									// �m�[�h�̑���
		int	index[NODEPOINT_MAX];						// �����̃m�[�h�ԍ�
		int connectNum[NODEPOINT_MAX];					// �ڑ��m�[�h��
		int connectIndex[NODEPOINT_MAX][CONNECT_MAX];	// �ڑ��m�[�h�ԍ�
		D3DXVECTOR3 pos[NODEPOINT_MAX];					// �e�m�[�h�̈ʒu
	}NODE_LOAD_STATE;

	typedef struct
	{// �Z�[�u�f�[�^
		int nodeMax;			// �m�[�h�̑���
		int	index;				// �����̃m�[�h�ԍ�
		int connectNum;			// �ڑ��m�[�h��
		int *connectIndex;		// �ڑ��m�[�h�ԍ�
		D3DXVECTOR3 pos;		// �e�m�[�h�̈ʒu
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
	NODE_LOAD_STATE m_LoadNodeData;	// ���[�h�����}�b�v���
	NODE_SAVE_STATE m_SaveNodeData;	// �Z�[�u����}�b�v���

	int m_nMassageCount;	// ���b�Z�[�W��\�����鎞��
	bool m_bSaveMassage;	// �Z�[�u�������b�Z�[�W��ONOFF
	bool m_bLoadMassage;	// ���[�h�������b�Z�[�W��ONOFF
	bool m_bErrorMassage;	// �G���[���b�Z�[�W��ONOFF
};

#endif