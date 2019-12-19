//=============================================================================
//
// �̗͏��� [life.h]
// Author : Ishida Takuto
//
//=============================================================================
#ifndef _LIFE_H_
#define _LIFE_H_

#include "scene.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define LIFE_PRIORITY		(5)		// ���C�t�̗D��ԍ�
#define MAX_POLYGON_LIFE	(3)		// �|���S���̍ő吔
#define MAX_LIFE			(100)	// ���C�t�̏��
#define MAX_LIFETEXTURE		(4)		// �e�N�X�`����

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CGauge2D;
class CGauge3D;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CLifeManager : public CScene
{
public:
	CLifeManager(int nPriority = LIFE_PRIORITY, CScene::OBJTYPE objType = CScene::OBJTYPE_LIFE);
	~CLifeManager();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static HRESULT Load(void);		// �f�[�^�̓ǂݍ���
	static void Unload(void);		// �ǂݍ��񂾃f�[�^�̔j��

	LPDIRECT3DTEXTURE9 Getexture(int nTexture) { return m_pTexture[nTexture]; };

	virtual const void AddSubtract(int nValue);		// �̗͂𑝌�������
	int GetLife(void) { return m_nLife; };			// �̗͂̎擾
	void SetLife(int nLife) { m_nLife = nLife; };	// �̗͂̐ݒ�

private:
	static LPDIRECT3DTEXTURE9 m_pTexture[MAX_LIFETEXTURE];	// ���L�e�N�X�`���ւ̃|�C���^
	int m_nLife;			// ���݂̗̑͂̒l(�őO�o�[)
};

/*===========================*/
/*		2D�̗̓N���X		 */
/*===========================*/
class CLife2D : public CLifeManager
{
public:
	CLife2D(int nPriority = LIFE_PRIORITY, CScene::OBJTYPE objType = CScene::OBJTYPE_LIFE);
	~CLife2D();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CLife2D *Create(int nLife, D3DXVECTOR3 pos, float fWidth, float fHeight);

	const void AddSubtract(int nValue);

private:
	CGauge2D *m_pGauge;		// �Q�[�W�N���X�̃|�C���^�ϐ�
};

/*===========================*/
/*		3D�̗̓N���X		 */
/*===========================*/
class CLife3D : public CLifeManager
{
public:
	CLife3D(int nPriority = LIFE_PRIORITY, CScene::OBJTYPE objType = CScene::OBJTYPE_LIFE);
	~CLife3D();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CLife3D *Create(int nLife, D3DXVECTOR3 pos, float fWidth, float fHeight, D3DXVECTOR3 *pPosParent);

	const void AddSubtract(int nValue);

private:
	CGauge3D *m_pGauge;		// �Q�[�W�N���X�̃|�C���^�ϐ�
};

#endif
