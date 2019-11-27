//=============================================================================
//
// �p�[�e�B�N������ [particle->h]
// Author : Ishida Takuto
//
//=============================================================================
#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#include "scene.h"
#include "scene3DBill.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define PARTICLE_PRIORITY	(6)		// �����̗D�揇��

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CParticle : public CScene
{
public:
	CParticle(int nPriority = 0, CScene::OBJTYPE objType = CScene::OBJTYPE_PARTICLE);
	virtual ~CParticle();
	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;

	static CParticle *Create(D3DXVECTOR3 pos, int nType);

private:

protected:
	D3DXVECTOR3 m_pos;				// �ʒu
	int m_nType;					// ���
};

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �p�[�e�B�N���̃G�~�b�^
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class CParEmitter : public CParticle
{
public:
	CParEmitter();
	~CParEmitter();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void Set(D3DXVECTOR3 pos, int nType);
	void SetPosParticleEmitter(int nIdx, D3DXVECTOR3 pos);
	void DestroyParticle(int nIdx);

private:
	int m_nCntAngle;		// �p�[�e�B�N���̊p�x�v�Z�p�J�E���^
	int m_nCntParticle;		// �o��������Ԋu�̃J�E���^

	int			m_nLife;			// �G�~�b�^�̎���
	int			m_nTexture;			// �e�N�X�`���̎��
	int			m_nAppear;			// �o��Ԋu
	int			m_nLap;				// 1���ɏo����
	bool		m_bLoop;			// ���[�v�̗L��
};

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �p�[�e�B�N���̃|���S��
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class CParticlePolygon : public CScene3DBill
{
public:
	CParticlePolygon(int nPriority = PARTICLE_PRIORITY);
	~CParticlePolygon();
	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CParticlePolygon *Create(D3DXVECTOR3 pos, int nType, int nTexture, int nAngle);

private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuffParticle;	// ���_�o�b�t�@�ւ̃|�C���^

	D3DXVECTOR3 m_move;		// �ړ���
	D3DXCOLOR m_col;		// �G�~�b�^�̈ʒu
	float m_fLength;		// ���_�̋���
	float m_fCntRot;		// �p�x�̍X�V
	float m_fRotAngle;		// ��]����Ƃ��̊p�x
	float m_fAngle;			// �p�x
	float m_fRadius;		// �傫��
	int m_nLife;			// �\������
	int m_nType;			// �G�~�b�^�̎��
	float m_fGravity;		// �d��
	float m_fBouncing;		// �o�E���h����l
	bool m_bDraw;			// ���Z�������ǂ���
	int m_nCntAngle;		// �p�x�v�Z�p�ɕK�v
	bool m_bRotVec;			// ��]����
	int m_nLifeMax;			// ���C�t�̍ő�l
};

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �p�[�e�B�N���̃f�[�^
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class CParData
{
public:
	typedef struct
	{
		int nWidth[2];			// ��(�ŏ��l�ƍő�l)
		int nHeight[2];			// ����(�ŏ��l�ƍő�l)
		int nDepth[2];			// ���s(�ŏ��l�ƍő�l)
		D3DXCOLOR col;			// �F
		D3DXCOLOR flameCol;		// ���t���[���̐F�̕ω�
		int nRadius[2];			// �傫��(�ŏ��l�ƍő�l)
		float fFlameRadius;		// ���t���[���̔��a�̊g�k
		int nLife;				// �G�~�b�^�̎���
		int nParLife[2];		// �\������(�ŏ��l�ƍő�l)
		int nTexture;			// �e�N�X�`���̎��
		float fSpread;			// �L����
		int nAppear;			// �o��Ԋu
		float fGravity;			// �d��
		int nLap;				// 1���ɏo����
		int nRot;				// ��]���邩�ǂ���(0:�Ȃ��@1:���v���@2:�����v��� 3:�����_��)
		float fRotSpeed;		// ��]���鑬�x
		bool bLoop;				// ���[�v�����邩 0:�Ȃ� 1:����
		bool bBouncing;			// �o�E���h�����邩 0:�Ȃ� 1:����
		bool bDraw;				// ���Z�������ǂ���
	}PAR_DATA;

	CParData();
	~CParData();
	HRESULT Init(void);
	void Uninit(void);

	static HRESULT Load(void);		// �f�[�^��ǂݍ���
	static void Unload(void);		// �ǂݍ��񂾃f�[�^�����

	static D3DXCOLOR GetFlameColor(int nIdx) { return m_pParticleData[nIdx]->flameCol; };
	static D3DXCOLOR GetColor(int nIdx) { return m_pParticleData[nIdx]->col; };
	static float GetFlameRadius(int nIdx) { return m_pParticleData[nIdx]->fFlameRadius; };
	static bool GetBouncing(int nIdx) { return m_pParticleData[nIdx]->bBouncing; };
	static int GetRotType(int nIdx) { return m_pParticleData[nIdx]->nRot; };
	static float GetRotSpeed(int nIdx) { return m_pParticleData[nIdx]->fRotSpeed; };
	static int GetLap(int nIdx) { return m_pParticleData[nIdx]->nLap; };
	static int *GetParLife(int nIdx) { return &m_pParticleData[nIdx]->nParLife[0]; };
	static int *GetRadius(int nIdx) { return &m_pParticleData[nIdx]->nRadius[0]; };
	static int *GetWidth(int nIdx) { return &m_pParticleData[nIdx]->nWidth[0]; };
	static int *GetHeight(int nIdx) { return &m_pParticleData[nIdx]->nHeight[0]; };
	static int *GetDepth(int nIdx) { return &m_pParticleData[nIdx]->nDepth[0]; };
	static float GetSpread(int nIdx) { return m_pParticleData[nIdx]->fSpread; };
	static bool GetDraw(int nIdx) { return m_pParticleData[nIdx]->bDraw; };
	static float GetGravity(int nIdx) { return m_pParticleData[nIdx]->fGravity; };
	static int GetAppear(int nIdx) { return m_pParticleData[nIdx]->nAppear; };
	static int GetLife(int nIdx) { return m_pParticleData[nIdx]->nLife; };
	static int GetTextureIdx(int nIdx) { return m_pParticleData[nIdx]->nTexture; };

	// �e�N�X�`���̎󂯓n��
	static LPDIRECT3DTEXTURE9 GetTexture(int nType) { return m_pTexture[nType]; };
	static void SetTexture(LPDIRECT3DTEXTURE9 *pTexture) { m_pTexture = pTexture; };

private:
	static int m_nNumParticle;			// �p�[�e�B�N����
	static int m_nNumTexture;			// �e�N�X�`���̐�
	static LPDIRECT3DTEXTURE9 *m_pTexture;		// ���L�e�N�X�`���ւ̃|�C���^

	static PAR_DATA **m_pParticleData;		// �p�[�e�B�N���f�[�^
};

#endif
