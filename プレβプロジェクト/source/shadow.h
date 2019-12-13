//=============================================================================
//
// �e���� [shadow.h]
// Author : Takuto Ishida
//
//=============================================================================
#ifndef _SHADOW_H_
#define _SHADOW_H_

#include "scene.h"
#include "shadow/ZTexCreator.h"
#include "shadow/comptr.h"
#include "shadow/DepthBufShadowEffect.h"
#include "shadow/EffectResource.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define SHADOW_PRIORITY		(6)

//*****************************************************************************
// �O���錾
//*****************************************************************************

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CShadow : public CScene
{
public:
	CShadow(int nPriority = SHADOW_PRIORITY, CScene::OBJTYPE objType = CScene::OBJTYPE_SHADOW);
	~CShadow();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	D3DXVECTOR3 GetPos(void) { return m_pos; };
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; };

	static CShadow *Create(void);

private:
	IKD::Com_ptr<ID3DXMesh>			m_pMesh;			// ���b�V�����ւ̃|�C���^
	IKD::Com_ptr<ID3DXBuffer>		m_pBuffMat;			// �}�e���A�����ւ̃|�C���^
	DWORD							m_nNumMat;			// �}�e���A�����̐�

	D3DXMATRIX						m_mtxWorld;			// ���[���h�}�g���b�N�X
	D3DXVECTOR3						m_pos;				// �ʒu
	D3DXVECTOR3						m_rot;				// ��]
	IKD::CZTexCreator				m_ZTexCreator;		// Z�l�e�N�X�`���쐬�I�u�W�F�N�g
	IKD::CDepthBufShadowEffect		m_DepthBS;			// �[�x�o�b�t�@�V���h�E�I�u�W�F�N�g
	IKD::Com_ptr<IDirect3DTexture9> m_cpShadowMapTex;	// Z�l�e�N�X�`��
};

#endif
