/*
*	@file LastBossModel.cpp
*	@brief ���X�{�X���f���N���X
*/
#include <pch.h>
#include "LastBossModel.h"
/*
*	@breif	�R���X�g���N�^
*	@details ���X�{�X���f���N���X�̃R���X�g���N�^
*	@param �Ȃ�
*	@return	�Ȃ�
*/
LastBossModel::LastBossModel()
	: m_pCommonResources{}// ���ʃ��\�[�X
	, m_pBodyModel{}// ����
	, m_pPixelShader{}// �e�p�̃s�N�Z���V�F�[�_�[
	, m_nowState(IState::EnemyState::IDLING)// ���݂̃X�e�[�g
{}

/*
*	@breif	�f�X�g���N�^
*	@details �e��|�C���^�[��nullptr�ɐݒ�
*	@param �Ȃ�
*   @return	�Ȃ�
*/
LastBossModel::~LastBossModel()
{
	m_pCommonResources = nullptr;	// ���ʃ��\�[�X�����
	m_pBodyModel = nullptr;			// ���̃��f�������
	m_pPixelShader.Reset();			// �s�N�Z���V�F�[�_�[�����Z�b�g
	m_pFaceModelMap.clear();		// ��̃��f���}�b�v���N���A
}

/*
*	@breif	������
*	@details ���X�{�X���f���N���X�̏�����
*	@param	CommonResources* resources ���ʃ��\�[�X
*   @return	�Ȃ�
*/
void LastBossModel::Initialize(CommonResources* resources)
{
	m_pCommonResources = resources;// ���ʃ��\�[�X
	auto device = resources->GetDeviceResources()->GetD3DDevice();// �f�o�C�X
	std::vector<uint8_t> ps = DX::ReadData(L"Resources/Shaders/Shadow/PS_Shadow.cso");// �s�N�Z���V�F�[�_�[�̓ǂݍ���
	DX::ThrowIfFailed(device->CreatePixelShader(ps.data(), ps.size(), nullptr, m_pPixelShader.ReleaseAndGetAddressOf()));// �s�N�Z���V�F�[�_�[�̍쐬
	m_pBodyModel = m_pCommonResources->GetModelManager()->GetModel("LastBossBody");// ���̃��f�����}�l�[�W���[����擾
	m_pFaceModelMap[IState::EnemyState::HIT] = m_pCommonResources->GetModelManager()->GetModel("LastBossFaceDamage");// �_���[�W�烂�f�����}�l�[�W���[����擾
	m_pFaceModelMap[IState::EnemyState::ATTACK] = m_pCommonResources->GetModelManager()->GetModel("LastBossFaceAttack");// �U���烂�f�����}�l�[�W���[����擾
	m_pFaceModelMap[IState::EnemyState::ANGRY] = m_pCommonResources->GetModelManager()->GetModel("LastBossFaceAngry");// �{��烂�f�����}�l�[�W���[����擾
}
/*
*	@breif	�`��
*	@details ���X�{�X���f���̕`��
*	@param	ID3D11DeviceContext1* context �f�o�C�X�R���e�L�X�g
*	@param	DirectX::DX11::CommonStates* states �R�����X�e�[�g
*	@param	DirectX::SimpleMath::Matrix world ���[���h�s��
*	@param	DirectX::SimpleMath::Matrix view �r���[�s��
*	@param	DirectX::SimpleMath::Matrix proj �v���W�F�N�V�����s��
*	@return	�Ȃ�
*/
void LastBossModel::Render(ID3D11DeviceContext1* context, DirectX::DX11::CommonStates* states, DirectX::SimpleMath::Matrix world, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	Vector3 lightDir = Vector3::UnitY;// ���C�g�̕���
	lightDir.Normalize();// ���K��
	Matrix shadowMatrix = Matrix::CreateShadow(Vector3::UnitY, Plane(0.0f, 1.0f, 0.0f, 0.01f));	// �e�s��̌������
	shadowMatrix = world * shadowMatrix;// �e�s��
	m_pBodyModel->Draw(context, *states, shadowMatrix * Matrix::Identity, view, proj, true, [&]()	// �e�`��
		{
			context->OMSetBlendState(states->Opaque(), nullptr, 0xffffffff);// �u�����h�X�e�[�g
			context->OMSetDepthStencilState(states->DepthNone(), 0);// �[�x�X�e���V���X�e�[�g
			context->RSSetState(states->CullClockwise());// ���X�^���C�U�[�X�e�[�g
			context->PSSetShader(m_pPixelShader.Get(), nullptr, 0);// �s�N�Z���V�F�[�_�[
		});
	m_pBodyModel->Draw(context, *states, world, view, proj);// ����
	auto it = m_pFaceModelMap.find(m_nowState);// �X�e�[�g�ɂ���Ċ�̃��f����ύX
	if (it != m_pFaceModelMap.end() && it->second)// �}�b�v�ɑ��݂���ꍇ
		it->second->Draw(context, *states, world, view, proj);// ��̃��f����`��
}
