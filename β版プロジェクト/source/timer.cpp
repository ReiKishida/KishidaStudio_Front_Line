////=============================================================================
////
//// �^�C�}�[���� [timer.cpp]
//// Author : Takuto Ishida
////
////=============================================================================
//#include "timer.h"
//#include "number.h"
//#include "fade.h"
//#include "game.h"
//
////*****************************************************************************
//// �}�N����`
////*****************************************************************************
//#define TIMER_WIDTH		(60)	// �^�C�}�[�̕����̕�
//#define TIMER_HEIGHT	(60)	// �^�C�}�[�̕����̍���
//
//#define TIMER_POS		(D3DXVECTOR3((m_pos.x - (nCntDigits * TIMER_WIDTH)) + (TIMER_WIDTH * (m_nDigits / 2.0f)) - TIMER_WIDTH / 2, m_pos.y, 0.0f))
//
////=============================================================================
//// �ÓI�����o�ϐ��錾
////=============================================================================
//int CTimer::m_nTimer = 0;	// �Q�[������
//
////=============================================================================
//// ��������
////=============================================================================
//CTimer* CTimer::Create(D3DXVECTOR3 pos, int nTime)
//{
//	CTimer *pTimer;
//
//	pTimer = new CTimer;	// ���������m��
//
//	if (pTimer)
//	{// �������m�ې���
//		pTimer->m_pos = pos;		// �ʒu�̐ݒ�
//		pTimer->m_nTimer = nTime;	// ���Ԃ̐ݒ�
//		pTimer->Init();				// ����������
//	}
//
//	return pTimer;
//}
//
////=========================================
//// �R���X�g���N�^
////=========================================
//CTimer::CTimer(int nPriority, CScene::OBJTYPE objType) : CScene(nPriority, objType)
//{
//	m_nDigits = 0;
//	m_nCntSecond = 0;
//	m_pNumber = NULL;
//}
//
////=========================================
//// �f�X�g���N�^
////=========================================
//CTimer::~CTimer()
//{
//}
//
////=============================================================================
//// ����������
////=============================================================================
//HRESULT CTimer::Init(void)
//{
//
//
//	// ���Ԃ̌��������߂�
//	m_nDigits = (int)log10f((float)m_nTimer) + 1;
//	if (m_nDigits <= 0) { m_nDigits = 1; }			// ������0�ȉ��̂Ƃ�
//
//	m_pNumber = new CNumber*[m_nDigits];			// ���������m��
//
//	if (m_pNumber)
//	{// �������m�ې���
//		for (int nCntDigits = 0; nCntDigits < m_nDigits; nCntDigits++)
//		{// ���������[�v
//			m_pNumber[nCntDigits] = CNumber::Create(TIMER_POS, TIMER_WIDTH, TIMER_HEIGHT);
//
//			// �����̐ݒ�
//			int aNumber = m_nTimer % (int)powf(10.0f, (float)(nCntDigits + 1)) / (int)powf(10.0f, (float)nCntDigits);
//			m_pNumber[nCntDigits]->SetNumber(aNumber);
//
//			// �F�̐ݒ�
//			m_pNumber[nCntDigits]->SetColor(D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f));
//		}
//	}
//
//	return S_OK;
//}
//
////=============================================================================
//// �I������
////=============================================================================
//void CTimer::Uninit(void)
//{
//	if (NULL != m_pNumber)
//	{// �g���Ă���ꍇ�j������
//		for (int nCntScore = 0; nCntScore < m_nDigits; nCntScore++)
//		{// ���������[�v
//			if (NULL != m_pNumber[nCntScore])
//			{// �g���Ă���ꍇ�j������
//				m_pNumber[nCntScore]->Uninit();
//				m_pNumber[nCntScore] = NULL;
//			}
//		}
//
//		delete[] m_pNumber;
//		m_pNumber = NULL;
//	}
//
//	// �����̔j��
//	CScene::Release();
//}
//
////=============================================================================
//// �X�V����
////=============================================================================
//void CTimer::Update(void)
//{
//	// �t���[���̃J�E���g
//	m_nCntSecond++;
//
//	if (m_nCntSecond % 60 == 0 && m_nTimer > 0)
//	{// ���Ԃ����炷
//		m_nTimer--;		// �J�E���g
//
//		if (m_nTimer <= 0)
//		{// ���Ԃ�0�ɂȂ���
//			CGame::SetGameState(CGame::STATE_CLEAR);
//		}
//
//		int nDigits = (int)log10f((float)m_nTimer) + 1;	// ���������߂�
//		if (nDigits <= 0) { nDigits = 1; }				// ������0�ȉ��̂Ƃ�
//
//		if (nDigits != m_nDigits)
//		{// �������ς����
//			for (int nCntScore = 0; nCntScore < m_nDigits; nCntScore++)
//			{// ���������[�v
//				if (NULL != m_pNumber[nCntScore])
//				{// �g���Ă���ꍇ�j������
//					m_pNumber[nCntScore]->Uninit();
//					m_pNumber[nCntScore] = NULL;
//				}
//			}
//
//			if (NULL != m_pNumber)
//			{// �g���Ă���ꍇ�j������
//				delete[] m_pNumber;
//				m_pNumber = NULL;
//			}
//
//			// �����̐ݒ�
//			m_nDigits = nDigits;
//
//			// �������m��
//			m_pNumber = new CNumber*[m_nDigits];
//
//			if (m_pNumber)
//			{// �������m�ۂł���
//				for (int nCntDigits = 0; nCntDigits < m_nDigits; nCntDigits++)
//				{// ���������[�v
//					m_pNumber[nCntDigits] = CNumber::Create(TIMER_POS, TIMER_WIDTH, TIMER_HEIGHT);
//				}
//			}
//		}
//
//		// �����̐ݒ�
//		int aNumber;
//		for (int nCntDigits = 0; nCntDigits < m_nDigits; nCntDigits++)
//		{// ���̏�����������ݒ�
//			aNumber = m_nTimer % (int)powf(10.0f, (float)(nCntDigits + 1)) / (int)powf(10.0f, (float)nCntDigits);
//			m_pNumber[nCntDigits]->SetNumber(aNumber);
//		}
//	}
//}
//
////=============================================================================
//// �`�揈��
////=============================================================================
//void CTimer::Draw(void)
//{
//}
