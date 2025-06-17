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
	, m_pCreateShader{ CreateShader::GetInstance() }// �V�F�[�_�[�쐬�N���X�̃|�C���^�[
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
	// ���ʃ��\�[�X�����
	m_pCommonResources = nullptr;
	// ���̃��f�������
	m_pBodyModel = nullptr;
	// �s�N�Z���V�F�[�_�[�����Z�b�g
	m_pPixelShader.Reset();
	// ��̃��f���}�b�v���N���A
	m_pFaceModelMap.clear();
}
/*
*	@breif	������
*	@details ���X�{�X���f���N���X�̏�����
*	@param	CommonResources* resources ���ʃ��\�[�X
*   @return	�Ȃ�
*/
void LastBossModel::Initialize(CommonResources* resources)
{
	// ���ʃ��\�[�X���擾
	m_pCommonResources = resources;
	// �f�o�C�X���擾
	auto device = resources->GetDeviceResources()->GetD3DDevice();
	// �V�F�[�_�[�쐬�N���X��������
	m_pCreateShader->Initialize(device);
	// �e�p�̃s�N�Z���V�F�[�_�[��ǂݍ���
	m_pCreateShader->CreatePixelShader(L"Resources/Shaders/Shadow/PS_Shadow.cso", m_pPixelShader);
	// ���̃��f�����}�l�[�W���[����擾
	m_pBodyModel = m_pCommonResources->GetModelManager()->GetModel("LastBossBody");
	// �_���[�W�烂�f�����}�l�[�W���[����擾
	m_pFaceModelMap[IState::EnemyState::HIT] = m_pCommonResources->GetModelManager()->GetModel("LastBossFaceDamage");
	// �U���烂�f�����}�l�[�W���[����擾
	m_pFaceModelMap[IState::EnemyState::ATTACK] = m_pCommonResources->GetModelManager()->GetModel("LastBossFaceAttack");
	// �{��烂�f�����}�l�[�W���[����擾
	m_pFaceModelMap[IState::EnemyState::ANGRY] = m_pCommonResources->GetModelManager()->GetModel("LastBossFaceAngry");
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
void LastBossModel::Render(ID3D11DeviceContext1* context,
	DirectX::DX11::CommonStates* states,
	const DirectX::SimpleMath::Matrix& world,
	const DirectX::SimpleMath::Matrix& view,
	const DirectX::SimpleMath::Matrix& proj)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// ���C�g�̕�����ݒ�
	Vector3 lightDir = Vector3::UnitY;
	// ���C�g�̕����𐳋K��
	lightDir.Normalize();
	// �e�s��̌������
	Matrix shadowMatrix = Matrix::CreateShadow(Vector3::UnitY, Plane(0.0f, 1.0f, 0.0f, 0.01f));
	// �e�s������[���h�s��ɓK�p
	shadowMatrix = world * shadowMatrix;
	// �ew��`��
	m_pBodyModel->Draw(context, *states, shadowMatrix * Matrix::Identity, view, proj, true, [&]()
		{
			// �u�����h�X�e�[�g
			context->OMSetBlendState(states->Opaque(), nullptr, 0xffffffff);
			// �[�x�X�e���V���X�e�[�g
			context->OMSetDepthStencilState(states->DepthNone(), 0);
			// ���X�^���C�U�[�X�e�[�g
			context->RSSetState(states->CullClockwise());
			// �s�N�Z���V�F�[�_�[
			context->PSSetShader(m_pPixelShader.Get(), nullptr, 0);
		});
	// ���̂�`��
	m_pBodyModel->Draw(context, *states, world, view, proj);
	// �X�e�[�g�ɂ���Ċ�̃��f����ύX
	auto it = m_pFaceModelMap.find(m_nowState);
	// �}�b�v�ɑ��݂���ꍇ
	if (it != m_pFaceModelMap.end() && it->second)
	{
		// ��̃��f����`��
		it->second->Draw(context, *states, world, view, proj);
	}
}
