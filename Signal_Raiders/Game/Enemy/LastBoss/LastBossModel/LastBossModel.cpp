/*
*	@file LastBossModel.cpp
*	@brief ���X�{�X���f���N���X
*/

#include <pch.h>
#include "LastBossModel.h"



/*
*	@breif	�R���X�g���N�^
*	@return	�Ȃ�
*/
LastBossModel::LastBossModel()
	: m_commonResources{}// ���ʃ��\�[�X
	, m_bodyModel{}// ����
	, m_pixelShader{}// �e�p�̃s�N�Z���V�F�[�_�[
	, m_damageFaceModel{}// ���i�̊�
	, m_attackFaceModel{}// �U�����̊�
	, m_nowState(IState::EnemyState::IDLING)// ���݂̃X�e�[�g
{}

/*
*	@breif	�f�X�g���N�^
*   @return	�Ȃ�
*/
LastBossModel::~LastBossModel() {}

/*
*	@breif	������
*	@param[in]	CommonResources* resources
*   @return	�Ȃ�
*/
void LastBossModel::Initialize(CommonResources* resources)
{
	m_commonResources = resources;// ���ʃ��\�[�X
	auto device = resources->GetDeviceResources()->GetD3DDevice();// �f�o�C�X
	std::vector<uint8_t> ps = DX::ReadData(L"Resources/Shaders/Shadow/PS_Shadow.cso");// �s�N�Z���V�F�[�_�[�̓ǂݍ���
	DX::ThrowIfFailed(device->CreatePixelShader(ps.data(), ps.size(), nullptr, m_pixelShader.ReleaseAndGetAddressOf()));// �s�N�Z���V�F�[�_�[�̍쐬
	std::unique_ptr<DirectX::EffectFactory> fx = std::make_unique<DirectX::EffectFactory>(device);// �G�t�F�N�g�t�@�N�g���[
	fx->SetDirectory(L"Resources/Models/Boss");// ���f���̃f�B���N�g��
	m_bodyModel = DirectX::Model::CreateFromCMO(device, L"Resources/Models/Boss/LastBoss_Body.cmo", *fx);// ����
	m_damageFaceModel = DirectX::Model::CreateFromCMO(device, L"Resources/Models/Boss/LastBoss_DamageFace.cmo", *fx);// �m�b�N�o�b�N���̊�
	m_attackFaceModel = DirectX::Model::CreateFromCMO(device, L"Resources/Models/Boss/LastBoss_Face.cmo", *fx);// �U�����̊�
	m_angryFaceModel = DirectX::Model::CreateFromCMO(device, L"Resources/Models/Boss/LastBoss_AngryFace.cmo", *fx);// �{���Ԃ̊�
}



void LastBossModel::Render(ID3D11DeviceContext1* context, DirectX::DX11::CommonStates* states, DirectX::SimpleMath::Matrix world, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	Vector3 lightDir = Vector3::UnitY;// ���C�g�̕���
	lightDir.Normalize();// ���K��
	Matrix shadowMatrix = Matrix::CreateShadow(Vector3::UnitY, Plane(0.0f, 1.0f, 0.0f, 0.01f));	// �e�s��̌������
	shadowMatrix = world * shadowMatrix;// �e�s��
	m_bodyModel->Draw(context, *states, shadowMatrix * Matrix::Identity, view, proj, true, [&]()	// �e�`��
		{
			context->OMSetBlendState(states->Opaque(), nullptr, 0xffffffff);// �u�����h�X�e�[�g
			context->OMSetDepthStencilState(states->DepthNone(), 0);// �[�x�X�e���V���X�e�[�g
			context->RSSetState(states->CullClockwise());// ���X�^���C�U�[�X�e�[�g
			context->PSSetShader(m_pixelShader.Get(), nullptr, 0);// �s�N�Z���V�F�[�_�[
		});
	m_bodyModel->Draw(context, *states, world, view, proj);// ����
	switch (m_nowState)// ���݂̃X�e�[�g�ɂ���ĕ`���ς���
	{
	case IState::EnemyState::DAMAGE:// �_���[�W
		m_damageFaceModel->Draw(context, *states, world, view, proj);// �m�b�N�o�b�N���̊�
		break;
	case IState::EnemyState::ATTACK:// �U��
		m_attackFaceModel->Draw(context, *states, world, view, proj);// �U�����̊�
		break;
	case IState::EnemyState::ANGRY:// �{��
		m_angryFaceModel->Draw(context, *states, world, view, proj);// �{���Ԃ̊�
		break;

	}
}
