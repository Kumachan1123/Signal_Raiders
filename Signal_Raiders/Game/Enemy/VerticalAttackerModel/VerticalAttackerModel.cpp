/*
*	@file	VerticalAttackerModel.cpp
*	@brief	�����U���G���f���N���X
*/
#include <pch.h>
#include "VerticalAttackerModel.h"

/*
*	@brief	�R���X�g���N�^
*	@details	�����U���G���f���̏��������s��
*	@param	�Ȃ�
*/
VerticalAttackerModel::VerticalAttackerModel()
	: m_pCommonResources{}// ���ʃ��\�[�X
	, m_pBodyModel{}// ���f��
	, m_nowState{ IState::EnemyState::IDLING }// ���݂̃X�e�[�^�X
{}
/*
*	@brief	�f�X�g���N�^
*	@details	�����U���G���f���̉�����s��
*	@param	�Ȃ�
*/
VerticalAttackerModel::~VerticalAttackerModel()
{
	// ���ʃ��\�[�X�̉��
	m_pCommonResources = nullptr;
	// ���f���̉��
	m_pBodyModel = nullptr;
	// �烂�f���}�b�v�̉��
	m_pFaceModelMap.clear();
	// �s�N�Z���V�F�[�_�[�̉��
	m_pPixelShader.Reset();
	// �X�e�[�^�X��������
	m_nowState = IState::EnemyState::IDLING;
}
/*
*	@brief	����������
*	@details	�����U���G���f���̏��������s��
*	@param	resources	���ʃ��\�[�X
*	@return	�Ȃ�
*/
void VerticalAttackerModel::Initialize(CommonResources* resources)
{
	// ���ʃ��\�[�X��ݒ�
	m_pCommonResources = resources;
	// �f�o�C�X���擾
	auto device = resources->GetDeviceResources()->GetD3DDevice();
	// �e�p�̃V�F�[�_�[��ǂݍ���
	std::vector<uint8_t> ps = DX::ReadData(L"Resources/Shaders/Shadow/PS_Shadow.cso");
	// �V�F�[�_�[���쐬
	DX::ThrowIfFailed(device->CreatePixelShader(ps.data(), ps.size(), nullptr, m_pPixelShader.ReleaseAndGetAddressOf()));
	// ���f���}�l�[�W���[���琂���U���G�̃��f�����擾
	m_pBodyModel = m_pCommonResources->GetModelManager()->GetModel("VerticalAttacker");
	// ���f���}�l�[�W���[����_���[�W�烂�f�����擾
	m_pFaceModelMap[IState::EnemyState::HIT] = m_pCommonResources->GetModelManager()->GetModel("EnemyDamage");
	// ���f���}�l�[�W���[����U���烂�f�����擾
	m_pFaceModelMap[IState::EnemyState::ATTACK] = m_pCommonResources->GetModelManager()->GetModel("EnemyAttack");
	// ���f���}�l�[�W���[����U���烂�f�����擾
	m_pFaceModelMap[IState::EnemyState::ANGRY] = m_pCommonResources->GetModelManager()->GetModel("EnemyAttack");
	// ���f���}�l�[�W���[����U���烂�f�����擾
	m_pFaceModelMap[IState::EnemyState::IDLING] = m_pCommonResources->GetModelManager()->GetModel("EnemyIdling");
}
/*
*	@brief	�`�悷��
*	@param	context	�f�o�C�X�R���e�L�X�g
*	@param	states	���ʃX�e�[�g
*	@param	world	���[���h�s��
*	@param	view	�r���[�s��
*	@param	proj	�v���W�F�N�V�����s��
*	@return �Ȃ�
*/
void VerticalAttackerModel::Render(ID3D11DeviceContext1* context,
	DirectX::DX11::CommonStates* states,
	const DirectX::SimpleMath::Matrix& world,
	const DirectX::SimpleMath::Matrix& view,
	const DirectX::SimpleMath::Matrix& proj)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// ���̂�`��
	m_pBodyModel->Draw(context, *states, world, view, proj);
	// ���C�g�̕����ݒ�
	Vector3 lightDir = Vector3::UnitY;
	// ���C�g�̕����𐳋K��
	lightDir.Normalize();
	// �e�s��̌������
	Matrix shadowMatrix = Matrix::CreateShadow(Vector3::UnitY, Plane(0.0f, 1.0f, 0.0f, 0.01f));
	// �e�p�s����쐬
	shadowMatrix = world * shadowMatrix;
	// �e��`��
	m_pBodyModel->Draw(context, *states, shadowMatrix * Matrix::Identity, view, proj, true, [&]()
		{
			// �u�����h�X�e�[�g
			context->OMSetBlendState(states->Opaque(), nullptr, 0xffffffff);
			// �[�x�X�e���V���X�e�[�g
			context->OMSetDepthStencilState(states->DepthNone(), 0);
			// �J�����O
			context->RSSetState(states->CullCounterClockwise());
			// �s�N�Z���V�F�[�_�[�ݒ�
			context->PSSetShader(m_pPixelShader.Get(), nullptr, 0);
		});
	// �X�e�[�g�ɂ���Ċ�̃��f����ύX
	auto it = m_pFaceModelMap.find(m_nowState);
	// �}�b�v�ɑ��݂���ꍇ�A��̃��f����`��
	if (it != m_pFaceModelMap.end() && it->second) it->second->Draw(context, *states, world, view, proj);
}