//=============================================================================
//
// �|�[�Y���j���[���� [pause.cpp]
// Author : Takuto Ishida
//
//=============================================================================
#include "pause.h"
#include "scene.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
#include "game.h"
#include "manager.h"
#include "game.h"
#include "renderer.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define PAUSE_TEXTURE00	"data/TEXTURE/pause000.png"		// �e�N�X�`���ւ̃p�X
#define PAUSE_TEXTURE01	"data/TEXTURE/pause001.png"		// �e�N�X�`���ւ̃p�X
#define PAUSE_TEXTURE02	"data/TEXTURE/pause002.png"		// �e�N�X�`���ւ̃p�X

#define PMENU_SIZE_X	(200.0f)		// ���j���[�̑傫��(X)
#define PMENU_SIZE_Y	(45.0f)			// ���j���[�̑傫��(Y)

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================
LPDIRECT3DTEXTURE9 CPause::m_pTexture[TYPE_MAX] = {};	// ���L�e�N�X�`���ւ̃|�C���^

//=============================================================================
// �e�N�X�`���̓ǂݍ���
//=============================================================================
HRESULT CPause::Load(void)
{
	// �f�o�C�X�̎擾
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, PAUSE_TEXTURE00, &m_pTexture[TYPE_CONTINUE]);
	D3DXCreateTextureFromFile(pDevice, PAUSE_TEXTURE01, &m_pTexture[TYPE_RETRY]);
	D3DXCreateTextureFromFile(pDevice, PAUSE_TEXTURE02, &m_pTexture[TYPE_QUIT]);

	return S_OK;
}

//=============================================================================
// �ǂݍ��񂾃e�N�X�`���̉��
//=============================================================================
void CPause::Unload(void)
{
	for (int nCntTex = 0; nCntTex < TYPE_MAX; nCntTex++)
	{
		if (NULL != m_pTexture[nCntTex])
		{// �e�N�X�`���̔j��
			m_pTexture[nCntTex]->Release();
			m_pTexture[nCntTex] = NULL;
		}
	}
}

//=============================================================================
// ��������
//=============================================================================
CPause* CPause::Create(void)
{
	CPause *pPause = NULL;	// �|�[�Y�N���X�̃|�C���^�ϐ�

	pPause = new CPause;	// ���������m��

	if (NULL != pPause)
	{// ���������m�ۂł����Ƃ�
		pPause->Init();		// ����������
	}

	return pPause;
}

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CPause::CPause(int nPriority, CScene::OBJTYPE objType) : CScene(nPriority, objType)
{
	for (int nCntPolygon = 0; nCntPolygon < TYPE_MAX; nCntPolygon++)
	{// 2D�|���S��
		m_apScene2D[nCntPolygon] = NULL;
	}

	m_nNumSelect = 0;	// ���݂̑I����
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CPause::~CPause()
{
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CPause::Init(void)
{
	// �|�[�Y���j���[�̑I����
	m_apScene2D[TYPE_CONTINUE] = CScene2D::Create();
	m_apScene2D[TYPE_RETRY] = CScene2D::Create();
	m_apScene2D[TYPE_QUIT] = CScene2D::Create();

	// �e�N�X�`���̐ݒ�
	m_apScene2D[TYPE_CONTINUE]->BindTexture(m_pTexture[TYPE_CONTINUE]);
	m_apScene2D[TYPE_RETRY]->BindTexture(m_pTexture[TYPE_RETRY]);
	m_apScene2D[TYPE_QUIT]->BindTexture(m_pTexture[TYPE_QUIT]);

	// �ŏ��̑I���J�[�\���͈�ԏ�ɍ��킹��
	m_nNumSelect = TYPE_CONTINUE;

	for (int nCntPolygon = 0; nCntPolygon < TYPE_MAX; nCntPolygon++)
	{// �傫���̐ݒ�
		m_apScene2D[nCntPolygon]->SetSize(PMENU_SIZE_X, PMENU_SIZE_Y);
	}

	// ���_���W
	m_apScene2D[TYPE_CONTINUE]->SetPos(D3DXVECTOR3(SCREEN_WIDTH / 2, 200.0f, 0.0f));
	m_apScene2D[TYPE_RETRY]->SetPos(D3DXVECTOR3(SCREEN_WIDTH / 2, 350.0f, 0.0f));
	m_apScene2D[TYPE_QUIT]->SetPos(D3DXVECTOR3(SCREEN_WIDTH / 2, 500.0f, 0.0f));

	// ���_�J���[
	m_apScene2D[TYPE_CONTINUE]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	m_apScene2D[TYPE_RETRY]->SetColor(D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f));
	m_apScene2D[TYPE_QUIT]->SetColor(D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f));

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CPause::Uninit(void)
{
	int nCntMenu;

	for (nCntMenu = 0; nCntMenu < TYPE_MAX; nCntMenu++)
	{// �|���S���̔j��
		if (m_apScene2D[nCntMenu] != NULL)
		{
			m_apScene2D[nCntMenu]->Uninit();
			m_apScene2D[nCntMenu] = NULL;
		}
	}

	// �I�u�W�F�N�g��j��
	CScene::Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CPause::Update(void)
{
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();	// �L�[�{�[�h�̎擾
	CXInput *pXInput = CManager::GetXInput();					// �W���C�p�b�h�̎擾
	CSound *pSound = CManager::GetSound();						// �T�E���h�̏����擾

	// �I������
	Select();

	if (pXInput->GetTrigger(0, CXInput::XIJS_BUTTON_4) || pKeyboard->GetTrigger(DIK_P))
	{// �|�[�Y�����
		m_nNumSelect = TYPE_CONTINUE;
	}

	if (pXInput->GetRepeat(0, CXInput::XIJS_BUTTON_16) || pXInput->GetRepeat(0, CXInput::XIJS_BUTTON_0) || pKeyboard->GetRepeat(DIK_W) || pKeyboard->GetRepeat(DIK_UP))
	{// �I����Ԃ��P�グ��
		pSound->PlaySound(CSound::SOUND_LABEL_CURSOR);
		m_nNumSelect = (m_nNumSelect + (TYPE_MAX - 1)) % TYPE_MAX;

		for (int nCntPolygon = 0; nCntPolygon < TYPE_MAX; nCntPolygon++)
		{// �I�𒆂Ƒ��ŐF�𕪂���
			if (nCntPolygon == m_nNumSelect)
			{// �I��
				m_apScene2D[nCntPolygon]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			}
			else
			{// ��
				m_apScene2D[nCntPolygon]->SetColor(D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f));
			}
		}
	}

	if (pXInput->GetRepeat(0, CXInput::XIJS_BUTTON_17) || pXInput->GetRepeat(0, CXInput::XIJS_BUTTON_1) || pKeyboard->GetRepeat(DIK_S) || pKeyboard->GetRepeat(DIK_DOWN))
	{
		pSound->PlaySound(CSound::SOUND_LABEL_CURSOR);
		m_nNumSelect = (m_nNumSelect + 1) % TYPE_MAX;	// �I����Ԃ��P������

		for (int nCntPolygon = 0; nCntPolygon < TYPE_MAX; nCntPolygon++)
		{// �I�𒆂Ƒ��ŐF�𕪂���
			if (nCntPolygon == m_nNumSelect)
			{// �I��
				m_apScene2D[nCntPolygon]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			}
			else
			{// ��
				m_apScene2D[nCntPolygon]->SetColor(D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f));
			}
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CPause::Draw(void)
{
}

//=============================================================================
// �I������
//=============================================================================
void CPause::Select(void)
{
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();	// �L�[�{�[�h�̎擾
	CXInput *pXInput = CManager::GetXInput();					// �W���C�p�b�h�̎擾

	if (pXInput->GetTrigger(0, CXInput::XIJS_BUTTON_11) || pKeyboard->GetTrigger(DIK_RETURN))
	{
		// �|�[�Y��ʂ��甲����
		CManager::GetGame()->SetPauseBool(false);

		// �I�𒆂̍���
		switch (m_nNumSelect)
		{
		case TYPE_CONTINUE:
			break;

		case TYPE_RETRY:
				// ��ʑJ��
				CFade::Create(CManager::MODE_GAME);

			break;

		case TYPE_QUIT:
				// ��ʑJ��
				CFade::Create(CManager::MODE_TITLE);

			break;
		}
	}
}

