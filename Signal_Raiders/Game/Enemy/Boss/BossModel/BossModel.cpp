/*
*	@file	BossModel.cpp
*	@brief	�{�X���f���N���X
*/
#include <pch.h>
#include "BossModel.h"
/*
*	@breif	�R���X�g���N�^
*	@details �{�X���f���N���X�̃R���X�g���N�^
*	@param �Ȃ�
*	@return	�Ȃ�
*/
BossModel::BossModel()
	: m_pCommonResources{}// ���ʃ��\�[�X
	, m_pCreateShader{ CreateShader::GetInstance() }// �V�F�[�_�[�쐬�N���X�̃|�C���^�[
	, m_pBodyModel{}// ����
	, m_pPixelShader{}// �e�p�̃s�N�Z���V�F�[�_�[
	, m_nowState(IState::EnemyState::IDLING)// ���݂̃X�e�[�g
{}

/*
*	@breif	�f�X�g���N�^
*	@details �{�X���f���N���X�̃f�X�g���N�^
*	@param �Ȃ�
*	@return	�Ȃ�
*/
BossModel::~BossModel() {/*do nothing*/ }

/*
*	@breif	������
*	@details �{�X���f���N���X�̏�����
*	@param	CommonResources* resources ���ʃ��\�[�X
*	@return	�Ȃ�
*/
void BossModel::Initialize(CommonResources* resources)
{
	// ���ʃ��\�[�X��ݒ�
	m_pCommonResources = resources;
	// �f�o�C�X���\�[�X����f�o�C�X���擾����
	auto device = resources->GetDeviceResources()->GetD3DDevice();
	// �V�F�[�_�[�쐬�N���X��������	
	m_pCreateShader->Initialize(device);
	// �e�p�̃s�N�Z���V�F�[�_�[���쐬
	m_pCreateShader->CreatePixelShader(L"Resources/Shaders/Shadow/PS_Shadow.cso", m_pPixelShader);
	// ���̃��f�����}�l�[�W���[����擾
	m_pBodyModel = m_pCommonResources->GetModelManager()->GetModel("BossBody");
	// �_���[�W�烂�f�����}�l�[�W���[����擾
	m_pFaceModelMap[IState::EnemyState::HIT] = m_pCommonResources->GetModelManager()->GetModel("BossFaceDamage");
	// �U���烂�f�����}�l�[�W���[����擾
	m_pFaceModelMap[IState::EnemyState::ATTACK] = m_pCommonResources->GetModelManager()->GetModel("BossFaceAttack");
	// �{��烂�f�����}�l�[�W���[����擾
	m_pFaceModelMap[IState::EnemyState::ANGRY] = m_pCommonResources->GetModelManager()->GetModel("BossFaceAngry");
}
/*
*	@brief	�`��
*	@details �{�X���f���̕`��
*	@param	context  �f�o�C�X�R���e�L�X�g
*	@param	states	�R�����X�e�[�g
* �@@param	world	���[���h�s��
*	@param	view	�r���[�s��
*	@param	proj	�v���W�F�N�V�����s��
*	@return	�Ȃ�
*/
void BossModel::Render(ID3D11DeviceContext1* context,
	DirectX::DX11::CommonStates* states,
	const DirectX::SimpleMath::Matrix& world,
	const DirectX::SimpleMath::Matrix& view,
	const DirectX::SimpleMath::Matrix& proj)
{
	using namespace DirectX::SimpleMath;
	// ���C�g�̕�����ݒ�
	Vector3 lightDir = Vector3::UnitY;
	// ���C�g�̕����𐳋K��
	lightDir.Normalize();
	// �e�s��̌������
	Matrix shadowMatrix = Matrix::CreateShadow(Vector3::UnitY, Plane(0.0f, 1.0f, 0.0f, 0.01f));
	// �e�s��Ƀ��[���h�s����|����
	shadowMatrix = world * shadowMatrix;
	// �e��`�悷��
	m_pBodyModel->Draw(context, *states, shadowMatrix * Matrix::Identity, view, proj, true, [&]()
		{
			context->OMSetBlendState(states->Opaque(), nullptr, 0xffffffff);// �u�����h�X�e�[�g
			context->OMSetDepthStencilState(states->DepthNone(), 0);// �[�x�X�e���V���X�e�[�g
			context->RSSetState(states->CullClockwise());// ���X�^���C�U�[�X�e�[�g
			context->PSSetShader(m_pPixelShader.Get(), nullptr, 0);// �s�N�Z���V�F�[�_�[
		});
	// ���̂�`�悷��
	m_pBodyModel->Draw(context, *states, world, view, proj);
	// �X�e�[�g�ɂ���Ċ�̃��f����ύX
	auto it = m_pFaceModelMap.find(m_nowState);
	// �}�b�v�ɑ��݂���ꍇ�A���`�悷��
	if (it != m_pFaceModelMap.end() && it->second)it->second->Draw(context, *states, world, view, proj);
}
