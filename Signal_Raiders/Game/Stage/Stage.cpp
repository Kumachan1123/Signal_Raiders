/*
*	@file	Stage.cpp
*	@brief	�X�e�[�W�N���X
*/
#include <pch.h>
#include "Stage.h"

// �X�e�[�W�̍L��
const float Stage::STAGE_SIZE = 98.5f;
// �X�e�[�W�̍���
const float Stage::STAGE_HEIGHT = 3.0f;
/*
*	@brief	�R���X�g���N�^
*	@details �X�e�[�W�̃R���X�g���N�^
*	@param �Ȃ�
*	@return �Ȃ�
*/
Stage::Stage()
	: m_pCommonResources{}// ���ʃ��\�[�X�|�C���^
	, m_pModel{}// ���f���|�C���^
{
}
/*
*	@brief	�f�X�g���N�^
*	@details �X�e�[�W�̃f�X�g���N�^
*	@param �Ȃ�
*	@return �Ȃ�
*/
Stage::~Stage() { /* do nothing.*/ }
/*
*	@brief	������
*	@details �X�e�[�W�̏�����
*	@param resources ���ʃ��\�[�X
*	@return �Ȃ�
*/
void Stage::Initialize(CommonResources* resources)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// ���\�[�X��nullptr�łȂ����Ƃ��m�F
	assert(resources);
	// ���\�[�X��ۑ�
	m_pCommonResources = resources;
	// ���f���}�l�[�W���[����X�e�[�W���f�����擾
	m_pModel = m_pCommonResources->GetModelManager()->GetModel("Stage");
}
/*
*	@brief	�`��
*	@details �X�e�[�W�̕`��
*	@param view �r���[�s��
*	@param proj �v���W�F�N�V�����s��
*	@param world ���[���h�s��
*	@param pos ���[���h���W
*	@return �Ȃ�
*/
void Stage::Render(
	const DirectX::SimpleMath::Matrix& view,
	const DirectX::SimpleMath::Matrix& proj,
	const DirectX::SimpleMath::Matrix& world)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// �f�o�C�X�R���e�L�X�g���擾
	auto context = m_pCommonResources->GetDeviceResources()->GetD3DDeviceContext();
	// ���ʃX�e�[�g���擾
	auto states = m_pCommonResources->GetCommonStates();
	// ���f����`�悷��
	m_pModel->Draw(context, *states, world, view, proj);
}