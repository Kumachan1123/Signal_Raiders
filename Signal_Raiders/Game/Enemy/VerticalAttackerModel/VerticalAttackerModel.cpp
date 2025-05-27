/*
	@file	VerticalAttackerModel.cpp
	@brief	�͈͍U���G���f���N���X
*/
#include "pch.h"
#include "VerticalAttackerModel.h"

/*
*	@brief	�R���X�g���N�^
*	@param	�Ȃ�
*/
VerticalAttackerModel::VerticalAttackerModel()
	: m_pCommonResources{}// ���ʃ��\�[�X
	, m_pBodyModel{}// ���f��
	, m_nowState{ IState::EnemyState::IDLING }// ���݂̃X�e�[�^�X
{}
/*
*	@brief	�f�X�g���N�^
*	@param	�Ȃ�
*/
VerticalAttackerModel::~VerticalAttackerModel() {}
/*
*	@brief	����������
*	@param	resources	���ʃ��\�[�X
*	@return	�Ȃ�
*/
void VerticalAttackerModel::Initialize(CommonResources* resources)
{
	m_pCommonResources = resources;// ���ʃ��\�[�X
	auto device = resources->GetDeviceResources()->GetD3DDevice();// �f�o�C�X�擾
	std::vector<uint8_t> ps = DX::ReadData(L"Resources/Shaders/Shadow/PS_Shadow.cso");	// �e�p�̃V�F�[�_�[��ǂݍ���
	DX::ThrowIfFailed(device->CreatePixelShader(ps.data(), ps.size(), nullptr, m_pPixelShader.ReleaseAndGetAddressOf()));// �V�F�[�_�[���쐬
	m_pBodyModel = m_pCommonResources->GetModelManager()->GetModel("VerticalAttacker");// ���f���}�l�[�W���[���琂���U���G�̃��f�����擾
	m_pFaceModelMap[IState::EnemyState::HIT] = m_pCommonResources->GetModelManager()->GetModel("EnemyDamage");// ���f���}�l�[�W���[����_���[�W�烂�f�����擾
	m_pFaceModelMap[IState::EnemyState::ATTACK] = m_pCommonResources->GetModelManager()->GetModel("EnemyAttack");// ���f���}�l�[�W���[����U���烂�f�����擾
	m_pFaceModelMap[IState::EnemyState::ANGRY] = m_pCommonResources->GetModelManager()->GetModel("EnemyAttack");// ���f���}�l�[�W���[����U���烂�f�����擾
	m_pFaceModelMap[IState::EnemyState::IDLING] = m_pCommonResources->GetModelManager()->GetModel("EnemyIdling");// ���f���}�l�[�W���[����U���烂�f�����擾
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
	DirectX::SimpleMath::Matrix world,
	DirectX::SimpleMath::Matrix view,
	DirectX::SimpleMath::Matrix proj)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	m_pBodyModel->Draw(context, *states, world, view, proj);// ���̕`��
	Vector3 lightDir = Vector3::UnitY;	// ���C�g�̕����ݒ�
	lightDir.Normalize();// ���K��
	Matrix shadowMatrix = Matrix::CreateShadow(Vector3::UnitY, Plane(0.0f, 1.0f, 0.0f, 0.01f));	// �e�s��̌������
	shadowMatrix = world * shadowMatrix;// �e�p�s����쐬
	m_pBodyModel->Draw(context, *states, shadowMatrix * Matrix::Identity, view, proj, true, [&]()	// �e�`��
		{
			context->OMSetBlendState(states->Opaque(), nullptr, 0xffffffff);// �u�����h�X�e�[�g
			context->OMSetDepthStencilState(states->DepthNone(), 0);// �[�x�X�e���V���X�e�[�g
			context->RSSetState(states->CullNone());	// �J�����O
			context->PSSetShader(m_pPixelShader.Get(), nullptr, 0);// �s�N�Z���V�F�[�_�[�ݒ�
		});
	auto it = m_pFaceModelMap.find(m_nowState);// �X�e�[�g�ɂ���Ċ�̃��f����ύX
	if (it != m_pFaceModelMap.end() && it->second)// �}�b�v�ɑ��݂���ꍇ
		it->second->Draw(context, *states, world, view, proj);// ��̃��f����`��
}