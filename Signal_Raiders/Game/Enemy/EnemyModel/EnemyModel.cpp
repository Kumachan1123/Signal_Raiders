/*
*	@file	EnemyModel.cpp
*	@brief	�G���f���N���X
*/
#include "pch.h"
#include "EnemyModel.h"
/*
*	@brief	�R���X�g���N�^
*	@details �G���f���N���X�̃R���X�g���N�^
*	@param	�Ȃ�
*	@return	�Ȃ�
*/
EnemyModel::EnemyModel()
	: m_pCommonResources{}// ���ʃ��\�[�X
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
	m_pCommonResources = nullptr;	// ���ʃ��\�[�X�����
	m_pBodyModel = nullptr;			// ���̃��f�������
	m_pAntennaModel = nullptr;		// �A���e�i���f�������
	m_pHandModel = nullptr;			// �胂�f�������
	m_pShadowModel = nullptr;		// �e�p�̃��f�������
	m_pPixelShader.Reset();			// �s�N�Z���V�F�[�_�[�����Z�b�g
	m_pFaceModelMap.clear();			// ��̃��f���}�b�v���N���A
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
	m_pCommonResources = resources;// ���ʃ��\�[�X�擾
	auto device = resources->GetDeviceResources()->GetD3DDevice();// �f�o�C�X�擾
	std::vector<uint8_t> ps = DX::ReadData(L"Resources/Shaders/Shadow/PS_Shadow.cso");	// �e�p�̃V�F�[�_�[��ǂݍ���
	DX::ThrowIfFailed(device->CreatePixelShader(ps.data(), ps.size(), nullptr, m_pPixelShader.ReleaseAndGetAddressOf()));// �V�F�[�_�[�쐬
	// ���f����ǂݍ��ށi ���A�A���e�i�A��A�\����j
	m_pBodyModel = m_pCommonResources->GetModelManager()->GetModel("EnemyHead");// ���f���}�l�[�W���[���瓪���f�����擾
	m_pAntennaModel = m_pCommonResources->GetModelManager()->GetModel("EnemyAntenna");// ���f���}�l�[�W���[����A���e�i���f�����擾
	m_pHandModel = m_pCommonResources->GetModelManager()->GetModel("EnemyHand");// ���f���}�l�[�W���[����胂�f�����擾
	m_pShadowModel = m_pCommonResources->GetModelManager()->GetModel("EnemyShadow");// ���f���}�l�[�W���[����e�p���f�����擾
	m_pFaceModelMap[IState::EnemyState::HIT] = m_pCommonResources->GetModelManager()->GetModel("EnemyDamage");// ���f���}�l�[�W���[����_���[�W�烂�f�����擾
	m_pFaceModelMap[IState::EnemyState::ATTACK] = m_pCommonResources->GetModelManager()->GetModel("EnemyAttack");// ���f���}�l�[�W���[����U���烂�f�����擾
	m_pFaceModelMap[IState::EnemyState::ANGRY] = m_pCommonResources->GetModelManager()->GetModel("EnemyAttack");// ���f���}�l�[�W���[����U���烂�f�����擾
	m_pFaceModelMap[IState::EnemyState::IDLING] = m_pCommonResources->GetModelManager()->GetModel("EnemyIdling");// ���f���}�l�[�W���[����U���烂�f�����擾
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
	DirectX::SimpleMath::Matrix world,
	DirectX::SimpleMath::Matrix view,
	DirectX::SimpleMath::Matrix proj)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	m_pBodyModel->Draw(context, *states, world, view, proj);// ��
	m_pAntennaModel->Draw(context, *states, world, view, proj);// �A���e�i
	m_pHandModel->Draw(context, *states, world, view, proj);	// ��
	Vector3 lightDir = Vector3::UnitY;	// ���C�g�̕���
	lightDir.Normalize();// ���K��
	Matrix shadowMatrix = Matrix::CreateShadow(Vector3::UnitY, Plane(0.0f, 1.0f, 0.0f, 0.01f));	// �e�s��̌������
	shadowMatrix = world * shadowMatrix;	// �e�̍s������[���h�s��ɂ�����
	m_pShadowModel->Draw(context, *states, shadowMatrix * Matrix::Identity, view, proj, true, [&]()	// �e�`��
		{
			context->OMSetBlendState(states->Opaque(), nullptr, 0xffffffff);// �u�����h�X�e�[�g��s�����ɂ���
			context->OMSetDepthStencilState(states->DepthNone(), 0);	// �[�x�X�e���V���X�e�[�g�𖳌��ɂ���
			context->RSSetState(states->CullNone());	// �J�����O�𖳌��ɂ���
			context->PSSetShader(m_pPixelShader.Get(), nullptr, 0);// �s�N�Z���V�F�[�_�[���Z�b�g
		});
	auto it = m_pFaceModelMap.find(m_nowState);// �X�e�[�g�ɂ���Ċ�̃��f����ύX
	if (it != m_pFaceModelMap.end() && it->second)// �}�b�v�ɑ��݂���ꍇ
		it->second->Draw(context, *states, world, view, proj);// ��̃��f����`��
}