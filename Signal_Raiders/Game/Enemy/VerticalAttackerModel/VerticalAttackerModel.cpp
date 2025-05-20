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
	: m_commonResources{}// ���ʃ��\�[�X
	, m_bodyModel{}// ���f��
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
	m_commonResources = resources;// ���ʃ��\�[�X
	auto device = resources->GetDeviceResources()->GetD3DDevice();// �f�o�C�X�擾
	std::vector<uint8_t> ps = DX::ReadData(L"Resources/Shaders/Shadow/PS_Shadow.cso");	// �e�p�̃V�F�[�_�[��ǂݍ���
	DX::ThrowIfFailed(device->CreatePixelShader(ps.data(), ps.size(), nullptr, m_pixelShader.ReleaseAndGetAddressOf()));// �V�F�[�_�[���쐬
	std::unique_ptr<DirectX::EffectFactory> fx = std::make_unique<DirectX::EffectFactory>(device);	// ���f����ǂݍ��ޏ���
	fx->SetDirectory(L"Resources/Models/VerticalAttacker");// ���f���̃f�B���N�g�����w��
	m_bodyModel = DirectX::Model::CreateFromCMO(device, L"Resources/Models/VerticalAttacker/VerticalAttacker.cmo", *fx);//  ���̃��f����ǂݍ���
	fx->SetDirectory(L"Resources/Models/Enemy");// ���f���̃f�B���N�g�����w��i�ʏ�̓G�̕\����𗬗p�j
	m_faceModelMap[IState::EnemyState::HIT] = DirectX::Model::CreateFromCMO(device, L"Resources/Models/Enemy/Enemy_DamageFace.cmo", *fx);// �U�����󂯂����̊�
	m_faceModelMap[IState::EnemyState::ATTACK] = DirectX::Model::CreateFromCMO(device, L"Resources/Models/Enemy/Enemy_Face.cmo", *fx);// �U�����̊�
	m_faceModelMap[IState::EnemyState::ANGRY] = DirectX::Model::CreateFromCMO(device, L"Resources/Models/Enemy/Enemy_AttackFace.cmo", *fx);// �{�莞�̊�
	m_faceModelMap[IState::EnemyState::IDLING] = DirectX::Model::CreateFromCMO(device, L"Resources/Models/Enemy/Enemy_IdlingHead.cmo", *fx);// �p�j���̊�
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
	m_bodyModel->Draw(context, *states, world, view, proj);// ���̕`��
	Vector3 lightDir = Vector3::UnitY;	// ���C�g�̕����ݒ�
	lightDir.Normalize();// ���K��
	Matrix shadowMatrix = Matrix::CreateShadow(Vector3::UnitY, Plane(0.0f, 1.0f, 0.0f, 0.01f));	// �e�s��̌������
	shadowMatrix = world * shadowMatrix;// �e�p�s����쐬
	m_bodyModel->Draw(context, *states, shadowMatrix * Matrix::Identity, view, proj, true, [&]()	// �e�`��
		{
			context->OMSetBlendState(states->Opaque(), nullptr, 0xffffffff);// �u�����h�X�e�[�g
			context->OMSetDepthStencilState(states->DepthNone(), 0);// �[�x�X�e���V���X�e�[�g
			context->RSSetState(states->CullNone());	// �J�����O
			context->PSSetShader(m_pixelShader.Get(), nullptr, 0);// �s�N�Z���V�F�[�_�[�ݒ�
		});
	auto it = m_faceModelMap.find(m_nowState);// �X�e�[�g�ɂ���Ċ�̃��f����ύX
	if (it != m_faceModelMap.end() && it->second)// �}�b�v�ɑ��݂���ꍇ
		it->second->Draw(context, *states, world, view, proj);// ��̃��f����`��
}