/*
	@file	BossModel.cpp
	@brief	�G���f���N���X
*/
#include "pch.h"
#include "BossModel.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

/*
*	@breif	�R���X�g���N�^
*	@return	�Ȃ�
*/
BossModel::BossModel()
	: m_commonResources{}// ���ʃ��\�[�X
	, m_bodyModel{}// ����
	, m_pixelShader{}// �e�p�̃s�N�Z���V�F�[�_�[
	, m_idlingFaceModel{}// ���i�̊�
	, m_attackFaceModel{}// �U�����̊�
	, m_isSheild(false)// �V�[���h�W�J�t���O
	, m_sheildSize(DirectX::SimpleMath::Vector3::Zero)// �V�[���h�̃T�C�Y
	, m_nowState(IState::EnemyState::IDLING)// ���݂̃X�e�[�g
{}

/*
*	@breif	�f�X�g���N�^
*	@return	�Ȃ�
*/
BossModel::~BossModel() {}

/*
*	@breif	������
*	@param[in]	CommonResources* resources
*	@return	�Ȃ�
*/
void BossModel::Initialize(CommonResources* resources)
{
	m_commonResources = resources;// ���ʃ��\�[�X
	auto device = resources->GetDeviceResources()->GetD3DDevice();// �f�o�C�X
	std::vector<uint8_t> ps = DX::ReadData(L"Resources/Shaders/Shadow/PS_Shadow.cso");// �s�N�Z���V�F�[�_�[�̓ǂݍ���
	DX::ThrowIfFailed(device->CreatePixelShader(ps.data(), ps.size(), nullptr, m_pixelShader.ReleaseAndGetAddressOf()));// �s�N�Z���V�F�[�_�[�̍쐬
	std::unique_ptr<DirectX::EffectFactory> fx = std::make_unique<DirectX::EffectFactory>(device);// �G�t�F�N�g�t�@�N�g���[
	fx->SetDirectory(L"Resources/Models/Boss");// ���f���̃f�B���N�g��
	m_bodyModel = DirectX::Model::CreateFromCMO(device, L"Resources/Models/Boss/Boss.cmo", *fx);// ����
	m_idlingFaceModel = DirectX::Model::CreateFromCMO(device, L"Resources/Models/Boss/Boss_Face_Idling.cmo", *fx);// ���i�̊�
	m_attackFaceModel = DirectX::Model::CreateFromCMO(device, L"Resources/Models/Boss/Boss_Face_Attack.cmo", *fx);// �U�����̊�
}
/*
*	@brief	�`��
*	@param[in]	context  �f�o�C�X�R���e�L�X�g
*	@param[in]	states	�R�����X�e�[�g
* �@@param[in]	world	���[���h�s��
*	@param[in]	view	�r���[�s��
*	@param[in]	proj	�v���W�F�N�V�����s��
*	@return	�Ȃ�
*/
void BossModel::Render(ID3D11DeviceContext1* context,
	DirectX::DX11::CommonStates* states,
	DirectX::SimpleMath::Matrix world,
	DirectX::SimpleMath::Matrix view,
	DirectX::SimpleMath::Matrix proj)
{
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
	case IState::EnemyState::IDLING:// �ҋ@
		m_idlingFaceModel->Draw(context, *states, world, view, proj);// ���i�̊�
		break;
	case IState::EnemyState::ATTACK:// �U��
	case IState::EnemyState::ANGRY:// �{��
		m_attackFaceModel->Draw(context, *states, world, view, proj);// �U�����̊�
		break;
	}
}
