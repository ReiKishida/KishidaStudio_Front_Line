//=============================================================================
//
// �I�u�W�F�N�g���� [scene.cpp]
// Author : Takuto Ishida
//
//=============================================================================
#include "scene.h"
#include "debugProc.h"

//==================================
// �ÓI�����o�ϐ��錾
//==================================
CScene *CScene::m_apTop[NUM_PRIORITY] = {};	// �擪�I�u�W�F�N�g
CScene *CScene::m_apCur[NUM_PRIORITY] = {};	// ���݂̃I�u�W�F�N�g�i�Ō���j

//==================================
// ���S�t���O�𗧂Ă�
//==================================
void CScene::Release(void)
{
	// ���S�t���O�𗧂Ă�
	m_bDeath = true;
}

//==================================
// �I�u�W�F�N�g�̉������
//==================================
void CScene::ReleaseDeath(void)
{
	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{// �D�揇�ʕ��܂킷
		CScene *pScene = m_apTop[nCntPriority];	// �擪����n�߂�

		while (pScene != NULL)
		{// NULL�łȂ������
			CScene *pSceneNext = pScene->m_pNext;	// �X�V���ō폜���ꂽ���̂��߂ɒl��ێ�
			if (pScene->m_bDeath)
			{// ���S�t���O�`�F�b�N
				if (pScene->m_pPrev != NULL)
				{// �O�̃I�u�W�F�N�g�Ɏ��̃I�u�W�F�N�g�̃|�C���^��n��
					pScene->m_pPrev->m_pNext = pScene->m_pNext;	// �O�̃I�u�W�F�N�g��m_pNext�Ɏ��̃I�u�W�F�N�g�̃|�C���^������
				}

				if (pScene->m_pNext != NULL)
				{// ���̃I�u�W�F�N�g�ɑO�̃I�u�W�F�N�g�̃|�C���^��n��
					pScene->m_pNext->m_pPrev = pScene->m_pPrev;
				}

				if (pScene == m_apTop[nCntPriority])
				{// this���擪�������Ƃ�
					m_apTop[nCntPriority] = pScene->m_pNext;	// ���̃|�C���^������
				}
				if (pScene == m_apCur[nCntPriority])
				{// this���Ō���������Ƃ�
					m_apCur[nCntPriority] = pScene->m_pPrev;	// �O�̃|�C���^������
				}

				// �������̊J��
				delete pScene;
			}
			// ���̃I�u�W�F�N�g������
			pScene = pSceneNext;
		}
	}
}

//==================================
// �S�Ẵ|���S���̉������
//==================================
void CScene::ReleaseAll(void)
{
	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{// �D�揇�ʕ��܂킷
		CScene *pScene = m_apTop[nCntPriority];	// �擪����n�߂�

		while (pScene != NULL)
		{// �I�u�W�F�N�g��擪��������Ă���
			CScene *pSceneNext = pScene->m_pNext;	// �X�V���ō폜���ꂽ���̂��߂ɒl��ێ�
			if (!pScene->m_bDeath) { pScene->Uninit(); }	// �I������
			pScene = pSceneNext;					// ���̃I�u�W�F�N�g������
		}
	}

	// �폜����
	ReleaseDeath();
}

//==================================
// ��ʑJ�ڎ��̔j��
//==================================
void CScene::ModeUninit(void)
{
	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{// �D�揇�ʕ��܂킷
		CScene *pScene = m_apTop[nCntPriority];	// �擪����n�߂�

		while (pScene != NULL)
		{// �I�u�W�F�N�g��擪��������Ă���
			CScene *pSceneNext = pScene->m_pNext;	// �X�V���ō폜���ꂽ���̂��߂ɒl��ێ�

			if (OBJTYPE_FADE != pScene->m_objType)
			{// �t�F�[�h�ȊO�̎��S�t���O�𗧂Ă�
				pScene->Uninit();					// �I������
			}

			pScene = pSceneNext;					// ���̃I�u�W�F�N�g������
		}
	}

	// �폜����
	ReleaseDeath();
}

//==================================
// UI�̔j��
//==================================
void CScene::UIUninit(void)
{
	CScene *pScene = m_apTop[6];	// �擪����n�߂�

	while (pScene != NULL)
	{// �I�u�W�F�N�g��擪��������Ă���
		CScene *pSceneNext = pScene->m_pNext;	// �X�V���ō폜���ꂽ���̂��߂ɒl��ێ�

		if (OBJTYPE_UI_TEXTURE == pScene->m_objType || OBJTYPE_UI_NUMBER == pScene->m_objType)
		{// �t�F�[�h�ȊO�̎��S�t���O�𗧂Ă�
			pScene->Uninit();		// �I������
		}

		pScene = pSceneNext;					// ���̃I�u�W�F�N�g������
	}

	// �폜����
	ReleaseDeath();
}

//==================================
// �S�ẴI�u�W�F�N�g�̍X�V����
//==================================
void CScene::UpdateAll(void)
{
	int nCntPriorities[NUM_PRIORITY] = {};

	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{// �D�揇�ʕ��܂킷
		CScene *pScene = m_apTop[nCntPriority];	// �擪����n�߂�

		while (pScene != NULL)
		{// NULL�łȂ������
			nCntPriorities[nCntPriority]++;			// �f�o�b�O�p



			CScene *pSceneNext = pScene->m_pNext;			// �X�V���ō폜���ꂽ���̂��߂ɒl��ێ�
			if (!pScene->m_bDeath) { pScene->Update(); }	// �X�V����
			pScene = pSceneNext;							// ���̃I�u�W�F�N�g������
		}
	}

	// ���S�t���O�`�F�b�N
	ReleaseDeath();
}

//==================================
// �S�ẴI�u�W�F�N�g�̕`�揈��
//==================================
void CScene::DrawAll(void)
{
	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{// �D�揇�ʕ��܂킷
		CScene *pScene = m_apTop[nCntPriority];	// �擪����n�߂�

		while (pScene != NULL)
		{// NULL�łȂ������
			CScene *pSceneNext = pScene->m_pNext;		// �X�V���ō폜���ꂽ���̂��߂ɒl��ێ�
			if (!pScene->m_bDeath) { pScene->Draw(); }	// �`�揈��
			pScene = pSceneNext;						// ���̃I�u�W�F�N�g������
		}
	}
}

//==================================
// �X�g���e�W�[��ʂ̃I�u�W�F�N�g�̕`�揈��
//==================================
void CScene::DrawStrategy(void)
{
	CScene *pScene = m_apTop[6];	// �擪����n�߂�

	while (pScene != NULL)
	{// NULL�łȂ������
		CScene *pSceneNext = pScene->m_pNext;		// �X�V���ō폜���ꂽ���̂��߂ɒl��ێ�

		if (OBJTYPE_BUTTON == pScene->m_objType || OBJTYPE_BUTTONLINE == pScene->m_objType)
		{// �{�^���ƃ{�^�����Ȃ���
			if (!pScene->m_bDeath) { pScene->Draw(); }	// �`�揈��
		}

		pScene = pSceneNext;						// ���̃I�u�W�F�N�g������
	}
}

//=========================================
// �R���X�g���N�^
//=========================================
CScene::CScene(int nPriority, OBJTYPE objType)
{
	if (m_apTop[nPriority] == NULL)
	{// �擪������
		m_apTop[nPriority] = this;
	}

	if (this->m_pPrev == m_apTop[nPriority])
	{// �g�b�v�̎��̃I�u�W�F�N�g�Ƀ|�C���^��n��
		m_apTop[nPriority]->m_pNext = this;
	}

	m_pPrev = m_apCur[nPriority];	// �ЂƂO�̃|�C���^������
	if (m_pPrev != NULL)
	{// �O�̃I�u�W�F�N�g����łȂ��Ƃ�
		m_pPrev->m_pNext = this;	// �O�̃I�u�W�F�N�g�̎��̃I�u�W�F�N�g�ɐݒ�
	}
	m_pNext = NULL;					// ���̃|�C���^��NULL������
	m_objType = objType;			// �I�u�W�F�N�g�̎�ނɏ����l������
	m_apCur[nPriority] = this;		// �J�����g�i�Ō���j���X�V
	m_bDeath = false;				// ���S�t���O���U�ɂ���
	m_nPriority = nPriority;		// �D�揇��
}

//=========================================
// �f�X�g���N�^
//=========================================
CScene::~CScene()
{
}

//=========================================
// �D�揇�ʂ̕ύX
//=========================================
void CScene::SwapPriority(int nPriority)
{
	CScene *pScene = m_apTop[nPriority];	// �擪����n�߂�

	while (pScene != NULL)
	{// NULL�łȂ������
		CScene *pSceneNext = pScene->m_pNext;	// �X�V���ō폜���ꂽ���̂��߂ɒl��ێ�

		if (pSceneNext == NULL)
		{// �D�揇�ʂ̓���ւ�
			if (m_pPrev != NULL)
			{// �O�̃I�u�W�F�N�g�Ɏ��̃I�u�W�F�N�g�̃|�C���^��n��
				m_pPrev->m_pNext = m_pNext;	// �O�̃I�u�W�F�N�g��m_pNext�Ɏ��̃I�u�W�F�N�g�̃|�C���^������
			}

			if (m_pNext != NULL)
			{// ���̃I�u�W�F�N�g�ɑO�̃I�u�W�F�N�g�̃|�C���^��n��
				m_pNext->m_pPrev = m_pPrev;
			}

			if (m_apTop[m_nPriority] == this)
			{// �擪�������Ƃ�
				m_apTop[m_nPriority] = m_pNext;
			}
			if (m_apCur[m_nPriority] == this)
			{// �Ō���������Ƃ�
				m_apCur[m_nPriority] = m_pPrev;
			}

			if (m_apTop[nPriority] == NULL)
			{// �擪��NULL�������Ƃ�
				m_apTop[nPriority] = this;
				m_apCur[nPriority] = this;
			}

			m_pPrev = m_apCur[nPriority];	// �ЂƂO�̃|�C���^������
			if (m_pPrev != NULL)
			{// �O�̃I�u�W�F�N�g����łȂ��Ƃ�
				m_pPrev->m_pNext = this;	// �O�̃I�u�W�F�N�g�̎��̃I�u�W�F�N�g�ɐݒ�
			}

			if (m_pPrev == m_apTop[nPriority])
			{// �g�b�v�̎��̃I�u�W�F�N�g�Ƀ|�C���^��n��
				m_apTop[nPriority]->m_pNext = this;
			}

			pScene->m_pNext = this;		// �����������
										//m_pPrev = pScene;			// �O������
			m_pNext = NULL;				// ������ɂ���
			m_apCur[nPriority] = this;	// �Ō���������ɂ���
			m_nPriority = nPriority;	// �D�揇�ʂ�ς���
			return;
		}

		pScene = pSceneNext;			// ���̃I�u�W�F�N�g������
	}

	if (pScene == NULL)
	{// �S��NULL�������Ƃ��Ƃ�
		if (m_apTop[m_nPriority] == this)
		{// �擪�������Ƃ�
			m_apTop[m_nPriority] = m_pNext;
		}
		if (m_apCur[m_nPriority] == this)
		{// �Ō���������Ƃ�
			m_apCur[m_nPriority] = m_pPrev;
		}

		pScene = this;				// �����������
		this->m_pPrev = NULL;		// �O����ɂ���
		this->m_pNext = NULL;		// ������ɂ���
		m_apTop[nPriority] = this;	// �擪�������ɂ���
		m_apCur[nPriority] = this;	// �Ō���������ɂ���
		m_nPriority = nPriority;	// �D�揇�ʂ�ς���
	}
}