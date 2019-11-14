//=============================================================================
//
// �}�b�`���O��ʏ��� [matching.h]
// Author : Ayaka Hakozaki
//
//=============================================================================
#ifndef _MATCHING_H_
#define _MATCHING_H_

#include "scene.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MATCHING_PRIORITY				(7)		// �D��ԍ�
#define MATCHING_UITEX						(8)
#define MATCHING_UI_PLAYER				(4)
//*****************************************************************************
// �O���錾
//*****************************************************************************
class CUI_TEXTURE;
class CScene2D;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CMatching : public CScene
{
public:
	CMatching(int nPriority = MATCHING_PRIORITY, CScene::OBJTYPE objType = CScene::OBJTYPE_MATCHING);
	~CMatching();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	void PrintData(void);
	void ReadMessage(void);

	CUI_TEXTURE		*m_pUITex[MATCHING_UITEX];		// UI�e�N�X�`���|�C���^
	CScene2D		*m_pMatchingPlayerUI[MATCHING_UI_PLAYER];
	int						m_nCntBgMove;			// �X�N���[��
	int m_nCntFade;
	bool m_bFade;
};

#endif