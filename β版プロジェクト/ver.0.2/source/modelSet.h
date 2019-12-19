//=============================================================================
//
// 3D���f���I�u�W�F�N�g���� [modelObject.h]
// Author : Ishida Takuto
//
//=============================================================================
#ifndef _MODELOBJECT_H_
#define _MODELOBJECT_H_

#include "scene.h"
#include "sceneX.h"
#include "scene3D.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MODEL_PRIORITY	(3)
#define NUM_MODELINFO	(2)		// ���f����񂪕K�v�Ȑ�

//*****************************************************************************
// �O���錾
//*****************************************************************************

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CModelSetManager : public CScene
{
public:
	typedef enum
	{
		TYPE_MODEL = 0,		// ���f��
		TYPE_ITEM,			// �A�C�e��
		TYPE_START,			// �X�^�[�g�n�_
		TYPE_ENEMY,			// �G
		TYPE_MAX
	}TYPE;

	CModelSetManager(int nPriority = 0, CScene::OBJTYPE objType = CScene::OBJTYPE_SETMANAGER);
	~CModelSetManager();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static HRESULT Load(int nStage);		// �e�N�X�`����ǂݍ���
	static void Unload(void);				// �ǂݍ��񂾃e�N�X�`�������

	static CModelSetManager *Create(void);

	static LPDIRECT3DTEXTURE9	&GetTexture(TYPE type, int nType) { return m_pTexture[type][nType]; };		// �e�N�X�`�����̎擾
	static LPD3DXMESH			&GetMesh(TYPE type, int nType) { return m_pMesh[type][nType]; };			// ���b�V�����̎擾
	static LPD3DXBUFFER			&GetBuffMat(TYPE type, int nType) { return m_pBuffMat[type][nType]; };		// �}�e���A�����̎擾
	static DWORD				&GetNumMat(TYPE type, int nType) { return m_pNumMat[type][nType]; };		// �}�e���A�����̎擾

	static int					GetNumModel(void) { return m_nNumAllModel; };					// ���f�����̎擾
	static void					SetNumModel(int nNumModel) { m_nNumAllModel = nNumModel; };	// ���f�����̐ݒ�
	static void					AddNumModel(int nValue) { m_nNumAllModel += nValue; };			// ���f�����̉��Z

	static int					GetNumType(TYPE type) { return m_nNumType[type]; };					// ��ނ̐��̎擾

	static char					**&GetModelFileName(TYPE type) { return m_apModelName[type]; };

	static D3DXVECTOR3			GetStartPos(void) { return m_StartPos; };
	static D3DXVECTOR3			GetGoalPos(void) { return m_GoalPos; };

protected:

private:
	static void NumObjectType(TYPE type);					// ���f�������쐬���邽�߂̃������m��
	static void CreateModelInfo(TYPE type, LPDIRECT3DDEVICE9 pDevice, char *pModelFileName, int nCntModel);	// ���f�����̍쐬

	static LPDIRECT3DTEXTURE9	*m_pTexture[NUM_MODELINFO];		// �e�N�X�`���ւ̃|�C���^
	static LPD3DXMESH			*m_pMesh[NUM_MODELINFO];		// ���b�V�����ւ̃|�C���^
	static LPD3DXBUFFER			*m_pBuffMat[NUM_MODELINFO];		// �}�e���A�����ւ̃|�C���^
	static DWORD				*m_pNumMat[NUM_MODELINFO];		// �}�e���A�����̐�

	static int					m_nNumType[TYPE_MAX];		// �ݒu���f���̎�ނ̐�
	static int					m_nNumAllModel;				// ���f���̍��v��
	static char					**m_apModelName[TYPE_MAX];	// ���f���̖��O
	static D3DXVECTOR3			m_StartPos;					// �X�^�[�g�n�_
	static D3DXVECTOR3			m_GoalPos;					// �S�[���n�_
};

/*===============================*/
/*			��Q���N���X		 */
/*===============================*/
class CModelObject : public CSceneX
{
public:
	CModelObject(int nPriority = MODEL_PRIORITY, CScene::OBJTYPE objType = CScene::OBJTYPE_MODELOBJECT);
	~CModelObject();
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, CModelSetManager::TYPE type, int nType);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CModelObject *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, CModelSetManager::TYPE type, int nType);

	int GetType(void) { return m_nType; };

	bool Collision(D3DXVECTOR3 pos, float fRange);
	bool Collision(D3DXVECTOR3 *pPos, float fRange);

protected:

private:
	float	m_fShadowRange;		// �e�͈̔�
	int		m_nType;			// ���f���̎��
};

/*===============================*/
/*			�A�C�e���N���X		 */
/*===============================*/
class CItem : public CSceneX
{
public:
	CItem(int nPriority = MODEL_PRIORITY, CScene::OBJTYPE objType = CScene::OBJTYPE_ITEM);
	~CItem();
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, CModelSetManager::TYPE type, int nType);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CItem *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, CModelSetManager::TYPE type, int nType);

	bool Collision(D3DXVECTOR3 pos, float fRange);

protected:

private:
	float	m_fShadowRange;		// �e�͈̔�
	int		m_nType;			// �A�C�e���̎��
};

#endif