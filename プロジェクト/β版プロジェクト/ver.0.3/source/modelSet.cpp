//=============================================================================
//
// 3D���f���I�u�W�F�N�g���� [modelSet.h]
// Author : Ishida Takuto
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "modelSet.h"
#include "input.h"
#include "manager.h"
#include "renderer.h"
#include "debugProc.h"
#include "meshField.h"
#include "scene3D.h"
#include "shadow.h"
#include "particle.h"
#include "player.h"
#include "camera.h"

#include <stdio.h>

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MODEL_TEXT			"data/TEXT/MODEL/model_object.txt"		// �I�u�W�F�N�g�̔z�u���e�L�X�g
#define WEED_MOVE			(1.0f)

#define LOG_SLIP_ANGLE		(0.8f)		// ����悤�ɂȂ�p�x
#define LOG_CLIMB_SPEED		(1.5f)		// �o��Ƃ��̑��x�̌�����
#define LOG_SLIP_SPEED		(4.0f)		// ���鑬�x
#define LOG_SLIP_JUMP		(1.0f)		// ������уW�����v���x�␳

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================
LPDIRECT3DTEXTURE9	*CModelSetManager::m_pTexture[NUM_MODELINFO] = {};	// �e�N�X�`�����
LPD3DXMESH			*CModelSetManager::m_pMesh[NUM_MODELINFO] = {};		// ���b�V�����
LPD3DXBUFFER		*CModelSetManager::m_pBuffMat[NUM_MODELINFO] = {};	// �}�e���A�����
DWORD				*CModelSetManager::m_pNumMat[NUM_MODELINFO] = {};		// �}�e���A����
int					CModelSetManager::m_nNumType[TYPE_MAX] = {};		// ���f���̎��
int					CModelSetManager::m_nNumAllModel = 0;			// ���f���̑���
char				**CModelSetManager::m_apModelName[TYPE_MAX] = {};		// ���f���t�@�C����
D3DXVECTOR3			CModelSetManager::m_StartPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
D3DXVECTOR3			CModelSetManager::m_GoalPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

//=============================================================================
// ���f�����̓ǂݍ���
//=============================================================================
HRESULT CModelSetManager::Load(int nStage)
{
	// �f�o�C�X�̎擾
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	FILE *pFile;		// �t�@�C���̃|�C���^�ϐ�
	char aStr[128];		// �擾���镶����
	int nCntModel[TYPE_MAX] = {};	// ���f���̃J�E���g
	char *pFileName[1] =
	{
		"data/TEXT/MODEL/first_stage.txt",
	};

	// �t�@�C�����J��
	pFile = fopen(pFileName[nStage], "r");

	if (NULL != pFile)
	{// �t�@�C����������
		while (strcmp(aStr, "END_SCRIPT") != 0)
		{// �I�����b�Z�[�W�܂Ń��[�v
			fscanf(pFile, "%s", &aStr[0]);

			if (strcmp(aStr, "TYPE_MODEL") == 0)
			{// �I�u�W�F�N�g�̎�ނ����f���̂Ƃ�
				fscanf(pFile, " = %d", &m_nNumType[TYPE_MODEL]);

				// ���f���t�@�C�����ۑ��ϐ��̃������m��
				m_apModelName[TYPE_MODEL] = new char*[m_nNumType[TYPE_MODEL]];

				// ���f�����̃������m��
				NumObjectType(TYPE_MODEL);
			}
			if (strcmp(aStr, "TYPE_START") == 0)
			{// �I�u�W�F�N�g�̎�ނ��X�^�[�g�n�_�̂Ƃ�
				fscanf(pFile, " = %d", &m_nNumType[TYPE_START]);

				// ���f���t�@�C�����ۑ��ϐ��̃������m��
				m_apModelName[TYPE_START] = new char*[m_nNumType[TYPE_START]];
			}
			if (strcmp(aStr, "TYPE_ENEMY") == 0)
			{// �I�u�W�F�N�g�̎�ނ��G�̂Ƃ�
				fscanf(pFile, " = %d", &m_nNumType[TYPE_ENEMY]);

				// ���f���t�@�C�����ۑ��ϐ��̃������m��
				m_apModelName[TYPE_ENEMY] = new char*[m_nNumType[TYPE_ENEMY]];
			}
			if (strcmp(aStr, "TYPE_ITEM") == 0)
			{// �I�u�W�F�N�g�̎�ނ��A�C�e���̂Ƃ�
				fscanf(pFile, " = %d", &m_nNumType[TYPE_ITEM]);

				// ���f���t�@�C�����ۑ��ϐ��̃������m��
				m_apModelName[TYPE_ITEM] = new char*[m_nNumType[TYPE_ITEM]];

				// ���f�����̃������m��
				NumObjectType(TYPE_ITEM);
			}
			if (strcmp(aStr, "LOAD") == 0)
			{// �ǂݍ��ރ��f����
				while (strcmp(aStr, "END_LOAD") != 0)
				{// �I�����b�Z�[�W�܂Ń��[�v
					fscanf(pFile, "%s", &aStr[0]);

					if (strcmp(aStr, "OBJECT_TYPE") == 0)
					{// �ǂݍ��ރI�u�W�F�N�g�̎��
						fscanf(pFile, " = %s", &aStr[0]);
						TYPE type = TYPE_MODEL;

						// �e�I�u�W�F�N�g�̐����J�E���g
						if (strcmp(aStr, "TYPE_MODEL") == 0) { type = TYPE_MODEL; }
						if (strcmp(aStr, "TYPE_START") == 0) { type = TYPE_START; }
						if (strcmp(aStr, "TYPE_ENEMY") == 0) { type = TYPE_ENEMY; }
						if (strcmp(aStr, "TYPE_ITEM") == 0) { type = TYPE_ITEM; }

						while (1)
						{// ���[�v������
							fscanf(pFile, "%s", &aStr[0]);

							if (strcmp(aStr, "MODEL_FILENAME") == 0)
							{// �ǂݍ��ރ��f���t�@�C��
								fscanf(pFile, " = %s", &aStr[0]);

								// �K�v�ȕ��������m��
								m_apModelName[type][nCntModel[type]] = new char[64];
								strcpy(m_apModelName[type][nCntModel[type]], aStr);

								if (TYPE_MODEL == type || TYPE_ITEM == type)
								{// ���f�����̍쐬
									CreateModelInfo(type, pDevice, &aStr[0], nCntModel[type]);
								}

								nCntModel[type]++;	// �I�u�W�F�N�g���̃J�E���g
								break;
							}
						}
					}
				}
			}
			if (strcmp(aStr, "MODELSET") == 0)
			{// ���f���z�u���
				CModelSetManager::TYPE type = CModelSetManager::TYPE_MODEL;
				int nType = 0;	// ���f���ԍ�
				D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ʒu
				D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ����

				while (strcmp(aStr, "END_MODELSET") != 0)
				{// �I�����b�Z�[�W�܂Ń��[�v
					fscanf(pFile, "%s", &aStr[0]);

					if (strcmp(aStr, "OBJ") == 0)
					{// �I�u�W�F�N�g�̎��
						fscanf(pFile, " = %s", &aStr[0]);

						if (strcmp(aStr, "TYPE_MODEL") == 0)
						{// ��Q��
							type = TYPE_MODEL;
						}
						else if (strcmp(aStr, "TYPE_START") == 0)
						{// �X�^�[�g�n�_
							type = TYPE_START;
						}
						else if (strcmp(aStr, "TYPE_ENEMY") == 0)
						{// �G
							type = TYPE_ENEMY;
						}
						else if (strcmp(aStr, "TYPE_ITEM") == 0)
						{// �A�C�e��
							type = TYPE_ITEM;
						}
					}
					if (strcmp(aStr, "TYPE") == 0)
					{// ���f���ԍ�
						fscanf(pFile, " = %d", &nType);
					}
					if (strcmp(aStr, "POS") == 0)
					{// �ʒu
						fscanf(pFile, " = %f %f %f", &pos.x, &pos.y, &pos.z);

						if (TYPE_START == type)
						{// �X�^�[�g�ƃS�[��
							if (0 == nType)
							{// �X�^�[�g�n�_
								m_StartPos = pos;
							}
							else if (1 == nType)
							{// �S�[���n�_
								m_GoalPos = pos;
							}
						}
					}
					if (strcmp(aStr, "ROT") == 0)
					{// ����
						fscanf(pFile, " = %f %f %f", &rot.x, &rot.y, &rot.z);
					}
				}

				if (type == TYPE_MODEL)
				{// �I�u�W�F�N�g�̐���
					CModelObject::Create(pos, rot, type, nType);
				}
				else if (type == TYPE_ENEMY && CManager::MODE_GAME == CManager::GetMode())
				{// �G�̐���

				}
			}
		}

		// �t�@�C�������
		fclose(pFile);
	}
	else
	{// �t�@�C�����J���Ȃ�
		MessageBox(0, "�t�@�C��������܂���I", pFileName[nStage], MB_OK);
	}

	return S_OK;
}

//=============================================================================
// �I�u�W�F�N�g�̎�ނ��Ƃ̑������������m��
//=============================================================================
void CModelSetManager::NumObjectType(TYPE type)
{
	m_pTexture[type] = new LPDIRECT3DTEXTURE9[m_nNumType[type]];
	m_pMesh[type] = new LPD3DXMESH[m_nNumType[type]];
	m_pBuffMat[type] = new LPD3DXBUFFER[m_nNumType[type]];
	m_pNumMat[type] = new DWORD[m_nNumType[type]];
}

//=============================================================================
// ���f�����̍쐬
//=============================================================================
void CModelSetManager::CreateModelInfo(TYPE type, LPDIRECT3DDEVICE9 pDevice, char *pModelFileName, int nCntModel)
{
	D3DXMATERIAL *pMat;				// �}�e���A���f�[�^�ւ̃|�C���^

	// X�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX(pModelFileName,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_pBuffMat[type][nCntModel],
		NULL,
		&m_pNumMat[type][nCntModel],
		&m_pMesh[type][nCntModel]);

	// NULL�����Ă���
	m_pTexture[type][nCntModel] = NULL;

	// �e�N�X�`���̍쐬
	pMat = (D3DXMATERIAL*)m_pBuffMat[type][nCntModel]->GetBufferPointer();
	D3DXCreateTextureFromFile(pDevice, pMat->pTextureFilename, &m_pTexture[type][nCntModel]);
}

//=============================================================================
// �ǂݍ��񂾏��̉��
//=============================================================================
void CModelSetManager::Unload(void)
{
	// �G�̃��f�����̔j��

	for (int nCntType = 0; nCntType < NUM_MODELINFO; nCntType++)
	{// �I�u�W�F�N�g�̎�ޕ����[�v
		if (NULL != m_pTexture[nCntType])
		{
			for (int nCntNumType = 0; nCntNumType < m_nNumType[nCntType]; nCntNumType++)
			{
				if (NULL != m_pTexture[nCntType][nCntNumType])
				{// �e�N�X�`���̔j��
					m_pTexture[nCntType][nCntNumType]->Release();
					m_pTexture[nCntType][nCntNumType] = NULL;
				}

				if (NULL != m_pMesh[nCntType][nCntNumType])
				{// ���b�V�����̔j��
					m_pMesh[nCntType][nCntNumType]->Release();
					m_pMesh[nCntType][nCntNumType] = NULL;
				}

				if (NULL != m_pBuffMat[nCntType][nCntNumType])
				{// �}�e���A�����̔j��
					m_pBuffMat[nCntType][nCntNumType]->Release();
					m_pBuffMat[nCntType][nCntNumType] = NULL;
				}

				if (NULL != m_pNumMat[nCntType][nCntNumType])
				{// �}�e���A���̐���j��
					m_pNumMat[nCntType][nCntNumType] = NULL;
				}
			}
		}

		if (NULL != m_pTexture[nCntType])
		{// �e�N�X�`���̔j��
			delete[] m_pTexture[nCntType];
			m_pTexture[nCntType] = NULL;
		}

		if (NULL != m_pMesh[nCntType])
		{// ���b�V�����̔j��
			delete[] m_pMesh[nCntType];
			m_pMesh[nCntType] = NULL;
		}

		if (NULL != m_pBuffMat[nCntType])
		{// �}�e���A�����̔j��
			delete[] m_pBuffMat[nCntType];
			m_pBuffMat[nCntType] = NULL;
		}

		if (NULL != m_pNumMat[nCntType])
		{// �}�e���A�����̔j��
			delete[] m_pNumMat[nCntType];
			m_pNumMat[nCntType] = NULL;
		}
	}

	for (int nCntType = 0; nCntType < TYPE_MAX; nCntType++)
	{// �I�u�W�F�N�g�̎�ޕ����[�v
		if (NULL != m_apModelName[nCntType])
		{
			for (int nCntModel = 0; nCntModel < m_nNumType[nCntType]; nCntModel++)
			{
				if (NULL != m_apModelName[nCntType][nCntModel])
				{// �������̊J��
					delete[] m_apModelName[nCntType][nCntModel];
					m_apModelName[nCntType][nCntModel] = NULL;
				}
			}

			if (NULL != m_apModelName[nCntType])
			{// �������̊J��
				delete[] m_apModelName[nCntType];
				m_apModelName[nCntType] = NULL;
			}
		}
	}

	// ���f�����̏�����
	m_nNumAllModel = 0;
}

//=============================================================================
// ��������
//=============================================================================
CModelSetManager *CModelSetManager::Create(void)
{
	CModelSetManager *pModelObjectManager = NULL;

	// �������m��
	pModelObjectManager = new CModelSetManager;

	if (pModelObjectManager != NULL)
	{// �������m�ې���
		pModelObjectManager->Init();
	}

	return pModelObjectManager;
}

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CModelSetManager::CModelSetManager(int nPriority, CScene::OBJTYPE objType) : CScene(nPriority, objType)
{
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CModelSetManager::~CModelSetManager()
{
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CModelSetManager::Init(void)
{
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CModelSetManager::Uninit(void)
{
	// ������j��
	CScene::Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CModelSetManager::Update(void)
{
}

//=============================================================================
// �`�揈��
//=============================================================================
void CModelSetManager::Draw(void)
{
}

/*==================================================================================*/
/*																					*/
/*								���f���I�u�W�F�N�g�N���X							*/
/*																					*/
/*==================================================================================*/
//=============================================================================
// ��������
//=============================================================================
CModelObject* CModelObject::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, CModelSetManager::TYPE type, int nType)
{
	CModelObject *pObject = NULL;

	pObject = new CModelObject;	// ���������m��

	if (pObject != NULL)
	{// �������m�ې���
		pObject->Init(pos, rot, type, nType);

		// ���f�����̐ݒ�
		CModelSetManager::AddNumModel(1);
	}

	return pObject;
}

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CModelObject::CModelObject(int nPriority, CScene::OBJTYPE objType) : CSceneX(nPriority, objType)
{
	m_nType = 0;
	m_fShadowRange = 0.0f;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CModelObject::~CModelObject()
{
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CModelObject::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, CModelSetManager::TYPE type, int nType)
{
	// ���f�����̎󂯓n��
	CSceneX::BindInfo(CModelSetManager::GetTexture(type, nType), CModelSetManager::GetMesh(type, nType), CModelSetManager::GetBuffMat(type, nType), CModelSetManager::GetNumMat(type, nType));

	// ����������
	CSceneX::Init();

	// �ʒu�̐ݒ�
	CSceneX::SetPos(pos);

	// �����̐ݒ�
	CSceneX::SetRot(rot);

	// ��ނ̕ۑ�
	m_nType = nType;

	D3DXVECTOR3 vtxMax = CSceneX::GetMaxVtx();						// ���_���W�̍ő�l���擾
	m_fShadowRange = sqrtf(powf(vtxMax.x, 2) + powf(vtxMax.z, 2));	// ���a�����߂�

	// �n�ʃI�u�W�F�N�g��T��
	CScene *pScene = CScene::GetSceneTop(MESHFIELD_PRIORITY);

	while (pScene != NULL)
	{// NULL�ɂȂ�܂Ń��[�v
		CScene *pSceneNext = pScene->GetSceneNext();
		CScene::OBJTYPE objType = pScene->GetObjType();

		if (objType == CScene::OBJTYPE_FIELD)
		{// �n�ʂ������Ƃ�
			CMeshField *pMeshField = (CMeshField*)pScene;

			// ���_����ݒ�
			VERTEX_3D *pVtx;	// ���_���̃|�C���^

			// ���_�o�b�t�@�̎擾
			LPDIRECT3DVERTEXBUFFER9 pVtxBuff = pMeshField->GetVtxBuff();

			//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
			pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

			int nNumVtx = pMeshField->GetNumVtx();	// ���_���̎擾
			for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
			{// �S���_�̋������v�Z
				float fLength = powf(pVtx[nCntVtx].pos.x - pos.x, 2.0f) + powf(pVtx[nCntVtx].pos.z - pos.z, 2.0f);

				if (fLength <= powf(m_fShadowRange, 2.0f))
				{// �͈͓��ɂ��钸�_
					pVtx[nCntVtx].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
				}
			}

			// ���_�o�b�t�@���A�����b�N
			pVtxBuff->Unlock();

			// ���_�o�b�t�@�̐ݒ�
			pMeshField->SetVtxBuff(pVtxBuff);
		}

		// ���̃I�u�W�F�N�g������
		pScene = pSceneNext;
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CModelObject::Uninit(void)
{
	// ������j��
	CSceneX::Uninit();

	// ���f�����̐ݒ�
	CModelSetManager::AddNumModel(-1);
}

//=============================================================================
// �X�V����
//=============================================================================
void CModelObject::Update(void)
{
}

//=============================================================================
// �`�揈��
//=============================================================================
void CModelObject::Draw(void)
{
	// �`�揈��
	CSceneX::Draw();
}

//=============================================================================
// �����蔻�菈��
//=============================================================================
bool CModelObject::Collision(D3DXVECTOR3 pos, float fRange)
{
	bool bHit = false;

	// �����XY�͈̔�
	float fLength = powf(fRange, 2);

	// ���[���h�}�g���b�N�X�̎擾
	D3DXMATRIX mtxWorld = CSceneX::GetMtxWorld();

	float fLengthX = pos.x - mtxWorld._41;						// X�̍�
	float fLengthZ = pos.z - mtxWorld._43;						// Y�̍�
	float fLengthTotal = powf(fLengthX, 2) + powf(fLengthZ, 2);		// XY�̍��̓��

	if (fLength >= fLengthTotal)
	{// ����Ǝ����̔���
		if (false == bHit)
		{// �����L���ɂ���
			bHit = true;
		}
	}

	return bHit;
}

//=============================================================================
// �����蔻�菈��
//=============================================================================
bool CModelObject::Collision(D3DXVECTOR3 *pPos, float fRange)
{
	bool bHit = false;

	// �����XY�͈̔�
	float fLength = powf(fRange, 2);

	// ���[���h�}�g���b�N�X�̎擾
	D3DXMATRIX mtxWorld = CSceneX::GetMtxWorld();

	float fLengthX = pPos->x - mtxWorld._41;						// X�̍�
	float fLengthZ = pPos->z - mtxWorld._43;						// Z�̍�
	float fLengthTotal = powf(fLengthX, 2) + powf(fLengthZ, 2);		// XYZ�̍��̓��

	if (fLength >= fLengthTotal)
	{// ����Ǝ����̔���
		if (fLengthX <= 0)
		{// �E���ɂ���
			pPos->x -= fRange - fabsf(fLengthX);
		}
		else if (fLengthX >= 0)
		{// �����ɂ���
			pPos->x += fRange - fLengthX;
		}

		if (false == bHit)
		{// �����L���ɂ���
			bHit = true;
		}
	}

	return bHit;
}

/*==================================================================================*/
/*																					*/
/*										���N���X									*/
/*																					*/
/*==================================================================================*/
//=============================================================================
// ��������
//=============================================================================
CItem* CItem::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, CModelSetManager::TYPE type, int nType)
{
	CItem *pItem = NULL;

	pItem = new CItem;	// ���������m��

	if (NULL != pItem)
	{// �������m�ې���
		pItem->Init(pos, rot, type, nType);
	}

	return pItem;
}

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CItem::CItem(int nPriority, CScene::OBJTYPE objType) : CSceneX(nPriority, objType)
{
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CItem::~CItem()
{
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CItem::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, CModelSetManager::TYPE type, int nType)
{
	// ���f�����̎󂯓n��
	CSceneX::BindInfo(CModelSetManager::GetTexture(type, nType), CModelSetManager::GetMesh(type, nType), CModelSetManager::GetBuffMat(type, nType), CModelSetManager::GetNumMat(type, nType));

	// ����������
	CSceneX::Init();

	// �ʒu�̐ݒ�
	CSceneX::SetPos(pos);

	// �����̐ݒ�
	CSceneX::SetRot(rot);

	// ��ނ̕ۑ�
	m_nType = nType;

	D3DXVECTOR3 vtxMax = CSceneX::GetMaxVtx();						// ���_���W�̍ő�l���擾
	m_fShadowRange = sqrtf(powf(vtxMax.x, 2) + powf(vtxMax.z, 2));	// ���a�����߂�

	// �n�ʃI�u�W�F�N�g��T��
	CScene *pScene = CScene::GetSceneTop(MESHFIELD_PRIORITY);

	while (pScene != NULL)
	{// NULL�ɂȂ�܂Ń��[�v
		CScene *pSceneNext = pScene->GetSceneNext();
		CScene::OBJTYPE objType = pScene->GetObjType();

		if (objType == CScene::OBJTYPE_FIELD)
		{// �n�ʂ������Ƃ�
			CMeshField *pMeshField = (CMeshField*)pScene;

			// ���_����ݒ�
			VERTEX_3D *pVtx;	// ���_���̃|�C���^

			// ���_�o�b�t�@�̎擾
			LPDIRECT3DVERTEXBUFFER9 pVtxBuff = pMeshField->GetVtxBuff();

			//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
			pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

			int nNumVtx = pMeshField->GetNumVtx();	// ���_���̎擾
			for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
			{// �S���_�̋������v�Z
				float fLength = powf(pVtx[nCntVtx].pos.x - pos.x, 2.0f) + powf(pVtx[nCntVtx].pos.z - pos.z, 2.0f);

				if (fLength <= powf(m_fShadowRange, 2.0f))
				{// �͈͓��ɂ��钸�_
					pVtx[nCntVtx].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
				}
			}

			// ���_�o�b�t�@���A�����b�N
			pVtxBuff->Unlock();

			// ���_�o�b�t�@�̐ݒ�
			pMeshField->SetVtxBuff(pVtxBuff);
		}

		// ���̃I�u�W�F�N�g������
		pScene = pSceneNext;
	}

	return S_OK;
}
//=============================================================================
// �I������
//=============================================================================
void CItem::Uninit(void)
{
	// ������j��
	CSceneX::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CItem::Update(void)
{
	// �����̎擾
	D3DXVECTOR3 rot = CSceneX::GetRot();

	// ��]������
	rot.y += 0.001f;

	// �����̐ݒ�
	CSceneX::SetRot(rot);
}

//=============================================================================
// �`�揈��
//=============================================================================
void CItem::Draw(void)
{
	// �`�揈��
	CSceneX::Draw();
}

//=============================================================================
// �����蔻�菈��
//=============================================================================
bool CItem::Collision(D3DXVECTOR3 pos, float fRange)
{
	bool bHit = false;

	// �����XY�͈̔�
	float fLength = powf(fRange, 2);

	// ���[���h�}�g���b�N�X�̎擾
	D3DXMATRIX mtxWorld = CSceneX::GetMtxWorld();

	float fLengthX = pos.x - mtxWorld._41;						// X�̍�
	float fLengthY = pos.y - mtxWorld._42;						// Y�̍�
	float fLengthZ = pos.z - mtxWorld._43;						// Z�̍�
	float fLengthTotal = powf(fLengthX, 2) + powf(fLengthY, 2) + powf(fLengthZ, 2);		// XYZ�̍��̓��

	if (fLength >= fLengthTotal)
	{// ����Ǝ����̔���
		if (false == bHit)
		{// �����L���ɂ���
			bHit = true;
		}
	}

	return bHit;
}