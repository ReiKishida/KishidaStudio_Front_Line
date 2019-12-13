//=============================================================================
//
// �}�b�`���O��ʏ��� [matching.h]
// Author : Ayaka Hakozaki
//
//=============================================================================
#ifndef _MATCHING_H_
#define _MATCHING_H_

#include "scene.h"
#include "server.h"
#include "mechaSelect.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MATCHING_PRIORITY				(7)		// �D��ԍ�
#define MATCHING_UITEX					(8)		// UI�e�N�X�`���̑���
#define MATCHING_UI_PLAYER				(4)		// �v���C���[�e�N�X�`���̑���

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
	void CreateUITex(void);
	void CreatePlayerUI(void);
	void ScrollUI(void);
	void CheckFade(void);

	void PrintData(void);
	void ReadMessage(void);

	CUI_TEXTURE *m_pUITex[MATCHING_UITEX];				// UI�e�N�X�`���|�C���^
	CScene2D *m_pPlayerUI[MATCHING_UI_PLAYER];			// �v���C���[�e�N�X�`���̃|�C���^
	int	m_nCntBgMove;									// �X�N���[��
	bool m_bFade;										// �t�F�[�h�����邩�ǂ���
	int m_nCntFade;										// �t�F�[�h�܂ł̃J�E���^�[
	bool m_bConnect[MAX_PLAYER_CONNECT];
	int m_nNumBlue;
	int m_nNumRed;
};

#endif