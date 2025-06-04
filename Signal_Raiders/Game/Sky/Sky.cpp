/*
*	@file	Sky.cpp
*	@brief	�X�J�C�N���X
*/
#include <pch.h>
#include "Sky.h"
/*
*	@brief	�R���X�g���N�^
*	@details �X�J�C�̃R���X�g���N�^
*	@param StageID �X�e�[�WID
*	@return �Ȃ�
*/
Sky::Sky(int StageID)
	: m_pCommonResources{}// ���ʃ��\�[�X
	, m_pModel{}// ���f��
	, m_texturePath{}// �e�N�X�`���p�X
	, m_stageID{ StageID }// �X�e�[�WID
{
}
/*
*	@brief	�f�X�g���N�^
*	@details �X�J�C�̃f�X�g���N�^
*	@param �Ȃ�
*	@return �Ȃ�
*/
Sky::~Sky() {/*/do nothing.*/ }
/*
*	@brief	������
*	@details ��̏�����
*	@param resources ���ʃ��\�[�X
*	@return �Ȃ�
*/
void Sky::Initialize(CommonResources* resources)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// ���\�[�X��nullptr�łȂ����Ƃ��m�F
	assert(resources);
	// ���\�[�X��ۑ�
	m_pCommonResources = resources;
	// �X�e�[�WID�ɉ�������̃��f���̃p�X���擾
	auto it = m_keyMap.find(m_stageID);
	// ���f���}�l�[�W���[���烂�f�����擾
	m_pModel = m_pCommonResources->GetModelManager()->GetSkyModel(it->second);
	// ���f���̃G�t�F�N�g�����X�V����
	m_pModel->UpdateEffects([](DirectX::IEffect* effect)
		{
			// �x�[�V�b�N�G�t�F�N�g��ݒ肷��
			BasicEffect* basicEffect = dynamic_cast<BasicEffect*>(effect);
			// �G�t�F�N�g��nullptr�̏ꍇ�͏������I����
			if (!basicEffect)return;
			// ���C�g�𖳌��ɂ���
			basicEffect->SetLightEnabled(0, false);
			// ���C�g�𖳌��ɂ���
			basicEffect->SetLightEnabled(1, false);
			// ���C�g�𖳌��ɂ���
			basicEffect->SetLightEnabled(2, false);
			// �G�~�b�V�u�J���[��ݒ肷��
			basicEffect->SetEmissiveColor(Colors::White);
		}
	);

}
/*
*	@brief	�`��
*	@details �X�J�C�̕`��
*	@param view �r���[�s��
*	@param proj �v���W�F�N�V�����s��
*	@param world ���[���h�s��
*	@return �Ȃ�
*/
void Sky::Render(
	const DirectX::SimpleMath::Matrix& view,
	const DirectX::SimpleMath::Matrix& proj,
	const DirectX::SimpleMath::Matrix& world)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	auto context = m_pCommonResources->GetDeviceResources()->GetD3DDeviceContext();// �f�o�C�X�R���e�L�X�g���擾
	auto states = m_pCommonResources->GetCommonStates();// ���ʃX�e�[�g���擾
	m_pModel->Draw(context, *states, world, view, proj);// ���f����`�悷��
#ifdef _DEBUG
	auto debugString = m_pCommonResources->GetDebugString();// �f�o�b�O����\������
	debugString->AddString("Sky StageID: %d", m_stageID);// �X�e�[�WID���f�o�b�O���ɒǉ�
#endif
}
