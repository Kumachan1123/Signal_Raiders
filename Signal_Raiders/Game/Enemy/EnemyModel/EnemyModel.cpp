/*
	@file	EnemyModel.cpp
	@brief	�G���f���N���X
*/
#include "pch.h"
#include "EnemyModel.h"
/*
*	@brief	�R���X�g���N�^
*	@param[in]	�Ȃ�
*	@return	�Ȃ�
*/
EnemyModel::EnemyModel()
	: m_commonResources{}// ���ʃ��\�[�X
	, m_bodyModel{}// ���̃��f��
	, m_antennaModel{}// �A���e�i���f��
	, m_handModel{}// �胂�f��
	, m_shadowModel{}// �e�p�̃��f��
	, m_nowState{ IState::EnemyState::IDLING }
{}
/*
*	@brief	�f�X�g���N�^
*	@param[in]	�Ȃ�
*	@return	�Ȃ�
*/
EnemyModel::~EnemyModel() {/*do nothing*/ }


/*
*	@brief	������
*	@param[in]	CommonResources* resources ���ʃ��\�[�X
*	@return	�Ȃ�
*/
void EnemyModel::Initialize(CommonResources* resources)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	m_commonResources = resources;// ���ʃ��\�[�X�擾
	auto device = resources->GetDeviceResources()->GetD3DDevice();// �f�o�C�X�擾
	std::vector<uint8_t> ps = DX::ReadData(L"Resources/Shaders/Shadow/PS_Shadow.cso");	// �e�p�̃V�F�[�_�[��ǂݍ���
	DX::ThrowIfFailed(device->CreatePixelShader(ps.data(), ps.size(), nullptr, m_pixelShader.ReleaseAndGetAddressOf()));// �V�F�[�_�[�쐬
	std::unique_ptr<DirectX::EffectFactory> fx = std::make_unique<DirectX::EffectFactory>(device);	// ���f����ǂݍ��ޏ���
	fx->SetDirectory(L"Resources/Models/Enemy");// ���f���̃f�B���N�g�����w��
	// ���f����ǂݍ��ށi ���A�A���e�i�A��A�\����j
	m_bodyModel = DirectX::Model::CreateFromCMO(device, L"Resources/Models/Enemy/Enemy_Head.cmo", *fx);// ��
	m_antennaModel = DirectX::Model::CreateFromCMO(device, L"Resources/Models/Enemy/Enemy_Antenna.cmo", *fx);// �A���e�i
	m_handModel = DirectX::Model::CreateFromCMO(device, L"Resources/Models/Enemy/Enemy_Hand.cmo", *fx);// ��
	m_shadowModel = DirectX::Model::CreateFromCMO(device, L"Resources/Models/Enemy/Enemy.cmo", *fx);	// �e�p�̃��f����ǂݍ���
	m_faceModelMap[IState::EnemyState::HIT] = DirectX::Model::CreateFromCMO(device, L"Resources/Models/Enemy/Enemy_DamageFace.cmo", *fx);// �U�����󂯂����̊�
	m_faceModelMap[IState::EnemyState::ATTACK] = DirectX::Model::CreateFromCMO(device, L"Resources/Models/Enemy/Enemy_Face.cmo", *fx);// �U�����̊�
	m_faceModelMap[IState::EnemyState::ANGRY] = DirectX::Model::CreateFromCMO(device, L"Resources/Models/Enemy/Enemy_AttackFace.cmo", *fx);// ������
	m_faceModelMap[IState::EnemyState::IDLING] = DirectX::Model::CreateFromCMO(device, L"Resources/Models/Enemy/Enemy_IdlingHead.cmo", *fx);// ���i�̊�
}
/*
*	@brief	�`��
*	@param[in]	ID3D11DeviceContext1* context �R���e�L�X�g
*	@param[in]	DirectX::DX11::CommonStates* states �X�e�[�g
*	@param[in]	DirectX::SimpleMath::Matrix world ���[���h�s��
*	@param[in]	DirectX::SimpleMath::Matrix view �r���[�s��
*	@param[in]	DirectX::SimpleMath::Matrix proj �v���W�F�N�V�����s��
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
	m_bodyModel->Draw(context, *states, world, view, proj);// ��
	m_antennaModel->Draw(context, *states, world, view, proj);// �A���e�i
	m_handModel->Draw(context, *states, world, view, proj);	// ��
	Vector3 lightDir = Vector3::UnitY;	// ���C�g�̕���
	lightDir.Normalize();// ���K��
	Matrix shadowMatrix = Matrix::CreateShadow(Vector3::UnitY, Plane(0.0f, 1.0f, 0.0f, 0.01f));	// �e�s��̌������
	shadowMatrix = world * shadowMatrix;	// �e�̍s������[���h�s��ɂ�����
	m_shadowModel->Draw(context, *states, shadowMatrix * Matrix::Identity, view, proj, true, [&]()	// �e�`��
		{
			context->OMSetBlendState(states->Opaque(), nullptr, 0xffffffff);// �u�����h�X�e�[�g��s�����ɂ���
			context->OMSetDepthStencilState(states->DepthNone(), 0);	// �[�x�X�e���V���X�e�[�g�𖳌��ɂ���
			context->RSSetState(states->CullNone());	// �J�����O�𖳌��ɂ���
			context->PSSetShader(m_pixelShader.Get(), nullptr, 0);// �s�N�Z���V�F�[�_�[���Z�b�g
		});
	auto it = m_faceModelMap.find(m_nowState);// �X�e�[�g�ɂ���Ċ�̃��f����ύX
	if (it != m_faceModelMap.end() && it->second)// �}�b�v�ɑ��݂���ꍇ
		it->second->Draw(context, *states, world, view, proj);// ��̃��f����`��
}