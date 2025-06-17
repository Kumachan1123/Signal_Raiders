/*
*	@file	EnemyModel.cpp
*	@brief	�G���f���N���X
*/
#include <pch.h>
#include "EnemyModel.h"
/*
*	@brief	�R���X�g���N�^
*	@details �G���f���N���X�̃R���X�g���N�^
*	@param	�Ȃ�
*	@return	�Ȃ�
*/
EnemyModel::EnemyModel()
	: m_pCommonResources{}// ���ʃ��\�[�X
	, m_pCreateShader{ CreateShader::GetInstance() }// �V�F�[�_�[�쐬�N���X�̃|�C���^�[
	, m_pBodyModel{}// ���̃��f��
	, m_pAntennaModel{}// �A���e�i���f��
	, m_pHandModel{}// �胂�f��
	, m_pShadowModel{}// �e�p�̃��f��
	, m_nowState{ IState::EnemyState::IDLING }// ���݂̏��
{}
/*
*	@brief	�f�X�g���N�^
*	@details �e��|�C���^�[��nullptr�ɐݒ�
*	@param	�Ȃ�
*	@return	�Ȃ�
*/
EnemyModel::~EnemyModel()
{
	// ���ʃ��\�[�X�����
	m_pCommonResources = nullptr;
	// ���̃��f�������
	m_pBodyModel = nullptr;
	// �A���e�i���f�������
	m_pAntennaModel = nullptr;
	// �胂�f�������
	m_pHandModel = nullptr;
	// �e�p�̃��f�������
	m_pShadowModel = nullptr;
	// �s�N�Z���V�F�[�_�[�����Z�b�g
	m_pPixelShader.Reset();
	// ��̃��f���}�b�v���N���A
	m_pFaceModelMap.clear();
}


/*
*	@brief	������
*	@details �G���f���N���X�̏�����
*	@param	CommonResources* resources ���ʃ��\�[�X
*	@return	�Ȃ�
*/
void EnemyModel::Initialize(CommonResources* resources)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// ���ʃ��\�[�X�擾
	m_pCommonResources = resources;
	// �f�o�C�X�擾
	auto device = resources->GetDeviceResources()->GetD3DDevice();
	// �V�F�[�_�[�쐬�N���X��������
	m_pCreateShader->Initialize(device);
	// �e�p�V�F�[�_�[�쐬
	m_pCreateShader->CreatePixelShader(L"Resources/Shaders/Shadow/PS_Shadow.cso", m_pPixelShader);
	// ���f���}�l�[�W���[���瓪���f�����擾
	m_pBodyModel = m_pCommonResources->GetModelManager()->GetModel("EnemyHead");
	// ���f���}�l�[�W���[����A���e�i���f�����擾
	m_pAntennaModel = m_pCommonResources->GetModelManager()->GetModel("EnemyAntenna");
	// ���f���}�l�[�W���[����胂�f�����擾
	m_pHandModel = m_pCommonResources->GetModelManager()->GetModel("EnemyHand");
	// ���f���}�l�[�W���[����e�p���f�����擾
	m_pShadowModel = m_pCommonResources->GetModelManager()->GetModel("EnemyShadow");
	// ���f���}�l�[�W���[����_���[�W�烂�f�����擾
	m_pFaceModelMap[IState::EnemyState::HIT] = m_pCommonResources->GetModelManager()->GetModel("EnemyDamage");
	// ���f���}�l�[�W���[����U���烂�f�����擾
	m_pFaceModelMap[IState::EnemyState::ATTACK] = m_pCommonResources->GetModelManager()->GetModel("EnemyAttack");
	// ���f���}�l�[�W���[����U���烂�f�����擾
	m_pFaceModelMap[IState::EnemyState::ANGRY] = m_pCommonResources->GetModelManager()->GetModel("EnemyAngry");
	// ���f���}�l�[�W���[����U���烂�f�����擾
	m_pFaceModelMap[IState::EnemyState::IDLING] = m_pCommonResources->GetModelManager()->GetModel("EnemyIdling");
}
/*
*	@brief	�`��
*	@details �G���f���̕`����s��
*	@param	ID3D11DeviceContext1* context �R���e�L�X�g
*	@param	DirectX::DX11::CommonStates* states �X�e�[�g
*	@param	DirectX::SimpleMath::Matrix world ���[���h�s��
*	@param	DirectX::SimpleMath::Matrix view �r���[�s��
*	@param	DirectX::SimpleMath::Matrix proj �v���W�F�N�V�����s��
*	@return	�Ȃ�
*/
void EnemyModel::Render(ID3D11DeviceContext1* context,
	DirectX::DX11::CommonStates* states,
	const DirectX::SimpleMath::Matrix& world,
	const DirectX::SimpleMath::Matrix& view,
	const DirectX::SimpleMath::Matrix& proj)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// �̂̕`��
	m_pBodyModel->Draw(context, *states, world, view, proj);
	// �A���e�i�̕`��
	m_pAntennaModel->Draw(context, *states, world, view, proj);
	// ��̕`��
	m_pHandModel->Draw(context, *states, world, view, proj);
	// ���C�g�̕�����ݒ�
	Vector3 lightDir = Vector3::UnitY;
	// ���C�g�̕����𐳋K��
	lightDir.Normalize();
	// �e�s��̌������
	Matrix shadowMatrix = Matrix::CreateShadow(Vector3::UnitY, Plane(0.0f, 1.0f, 0.0f, 0.01f));
	// �e�̍s������[���h�s��ɂ�����
	shadowMatrix = world * shadowMatrix;
	// �e�̕`��
	m_pShadowModel->Draw(context, *states, shadowMatrix * Matrix::Identity, view, proj, true, [&]()
		{
			// �u�����h�X�e�[�g��s�����ɂ���
			context->OMSetBlendState(states->Opaque(), nullptr, 0xffffffff);
			// �[�x�X�e���V���X�e�[�g�𖳌��ɂ���
			context->OMSetDepthStencilState(states->DepthNone(), 0);
			// �J�����O�𖳌��ɂ���
			context->RSSetState(states->CullNone());
			// �s�N�Z���V�F�[�_�[���Z�b�g
			context->PSSetShader(m_pPixelShader.Get(), nullptr, 0);
		});
	// �X�e�[�g�ɂ���Ċ�̃��f����ύX
	auto it = m_pFaceModelMap.find(m_nowState);
	// �}�b�v�ɑ��݂���ꍇ�A��̃��f����`��
	if (it != m_pFaceModelMap.end() && it->second)it->second->Draw(context, *states, world, view, proj);
}