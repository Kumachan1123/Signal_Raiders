/*
*	@file	Stage.cpp
*	@brief	�X�e�[�W�N���X
*/
#include <pch.h>
#include "Stage.h"

const float Stage::STAGE_SIZE = 98.5f;	// �X�e�[�W�̍L��
const float Stage::STAGE_HEIGHT = 3.0f;	// �X�e�[�W�̍���
/*
*	@brief	�R���X�g���N�^
*	@details �X�e�[�W�̃R���X�g���N�^
*	@param �Ȃ�
*	@return �Ȃ�
*/
Stage::Stage()
	: m_pCommonResources{}
	, m_pModel{}
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
	assert(resources);// ���\�[�X��nullptr�łȂ����Ƃ��m�F
	m_pCommonResources = resources;// ���\�[�X��ۑ�
	m_pModel = m_pCommonResources->GetModelManager()->GetModel("Stage");// ���f���}�l�[�W���[����X�e�[�W���f�����擾
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
void Stage::Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj,
	DirectX::SimpleMath::Matrix world, DirectX::SimpleMath::Vector3 pos)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	auto context = m_pCommonResources->GetDeviceResources()->GetD3DDeviceContext();// �f�o�C�X�R���e�L�X�g���擾
	auto states = m_pCommonResources->GetCommonStates();// ���ʃX�e�[�g���擾
	world *= Matrix::CreateTranslation(pos);// ���[���h�s����X�V
	m_pModel->Draw(context, *states, world, view, proj);// ���f����`�悷��
}