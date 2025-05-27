/*
*	@file	PlayerBullet.cpp
*	@brief	�v���C���[�̒e�N���X
*/
#include "pch.h"
#include "PlayerBullet.h"
/*
*	@brief	�R���X�g���N�^
*	@details �v���C���[�̒e�N���X�̃R���X�g���N�^
*	@param [in]	�Ȃ�
*	@return	�Ȃ�
*/
PlayerBullet::PlayerBullet()
	: m_position{ DirectX::SimpleMath::Vector3::Zero }// �e�̈ʒu
	, m_velocity{ DirectX::SimpleMath::Vector3::Zero }// �e�̈ړ���
	, m_direction{ DirectX::SimpleMath::Vector3::Zero }// �e�̈ړ�����
	, m_pCommonResources{}// ���ʃ��\�[�X
	, m_time{ 0.0f }// �o�ߎ���
	, m_additionalDamage{ 0 }// �ǉ��_���[�W
	, m_angle{ 0.0f }// �e�̊p�x
	, m_pModel{ nullptr }// ���f���|�C���^�[
{
}
/*
*	@brief	�f�X�g���N�^
*	@details �v���C���[�̒e�N���X�̃f�X�g���N�^
*	@param [in]	�Ȃ�
*	@return	�Ȃ�
*/
PlayerBullet::~PlayerBullet()
{
	m_model.reset();// ���f���̉��
	m_pPixelShader.Reset();// �s�N�Z���V�F�[�_�[�̉��
	m_bulletTrail.reset();// �O�Ճ|�C���^�[�̉��
	m_pCommonResources = nullptr;// ���ʃ��\�[�X�̉��
}
/*
*	@brief	������
*	@details �v���C���[�̒e�N���X�̏�����
*	@param CommonResources* resources ���ʃ��\�[�X
*	@return	�Ȃ�
*/
void PlayerBullet::Initialize(CommonResources* resources)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	m_pCommonResources = resources;// ���ʃ��\�[�X�̐ݒ�
	auto device = m_pCommonResources->GetDeviceResources()->GetD3DDevice();// �f�o�C�X�̎擾
	DrawCollision::Initialize(m_pCommonResources);// �����蔻������p�N���X�̏�����
	m_bulletTrail = std::make_unique<Particle>(ParticleUtility::Type::PLAYERTRAIL, BulletParameters::PLAYER_BULLET_SIZE);// �e�̋O������
	m_bulletTrail->Initialize(m_pCommonResources);// �e�̋O�������̏�����
	std::vector<uint8_t> ps = DX::ReadData(L"Resources/Shaders/Shadow/PS_Shadow.cso");// �e�p�̃s�N�Z���V�F�[�_�[�ǂݍ���
	DX::ThrowIfFailed(device->CreatePixelShader(ps.data(), ps.size(), nullptr, m_pPixelShader.ReleaseAndGetAddressOf()));// �s�N�Z���V�F�[�_�[�̍쐬
	m_pModel = m_pCommonResources->GetModelManager()->GetModel("PlayerBullet");// �}�l�[�W���[���烂�f�����擾
	m_direction = Vector3::Zero;// �e�̈ړ�����
	m_velocity = Vector3::Zero;// �e�̈ړ���
	m_position = Vector3::Zero;// �e�̈ʒu
	m_boundingSphere.Center = m_position;// �e�̒��S���W
	m_boundingSphere.Radius = BulletParameters::COLLISION_RADIUS;// �e�̔��a
}
/*
*	@brief	�X�V
*	@details �v���C���[�̒e�N���X�̍X�V
*	@param float elapsedTime �o�ߎ���
*	@return	�Ȃ�
*/
void PlayerBullet::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;
	m_angle += BulletParameters::BULLET_ROTATION_SPEED;// �e�̉�]���x�����Z
	Clamp(m_angle, BulletParameters::ANGLE_MIN, BulletParameters::ANGLE_MAX);// �p�x��0�`360�x�ɐ�������
	m_velocity += m_direction;// �J�����������Ă�������ɑ��x��^����
	if (m_velocity.LengthSquared() > 0)m_velocity.Normalize();// ���x�𐳋K��
	m_velocity *= BulletParameters::ADJUST_MOVE * elapsedTime;// �ړ��ʂ�␳����
	m_position += m_velocity;// ���ۂɈړ�����
	m_boundingSphere.Center = m_position;// �o�E���f�B���O�X�t�B�A�̈ʒu�X�V
	m_bulletTrail->SetBulletPosition(m_position);// ���݂̒e�̈ʒu���O�Ճ��X�g�ɒǉ�
	m_bulletTrail->Update(elapsedTime);// �O�Ղ̍X�V
	m_time += elapsedTime;// ���Ԍv��
	m_worldMatrix = Matrix::CreateScale(BulletParameters::PLAYER_BULLET_SIZE);// �e�̃T�C�Y��ݒ�
	m_worldMatrix *= Matrix::CreateRotationX(DirectX::XMConvertToRadians(m_angle));// �e�̉�]��ݒ�
	m_worldMatrix *= Matrix::CreateTranslation(m_position);// �e�̍��W��ݒ�
}
/*
*	@brief	�e�𐶐�
*	@details �v���C���[�̒e�N���X�̒e�𐶐�
*	@param DirectX::SimpleMath::Vector3& pos �e�̈ʒu
*	@param DirectX::SimpleMath::Vector3& dir �e�̈ړ�����
*	@return	�Ȃ�
*/
void PlayerBullet::MakeBall(const DirectX::SimpleMath::Vector3& pos, DirectX::SimpleMath::Vector3& dir)
{
	using namespace DirectX::SimpleMath;
	m_position = pos + BulletParameters::INITIAL_POSITION;	// �����ʒu��ݒ�
	m_direction = dir;										// ������ݒ�
	m_direction.y += BulletParameters::ADJUST_DIRECTION;	// ������ɕ␳
}
/*
*	@brief	�`��
*	@details �v���C���[�̒e�N���X�̕`��
*	@param DirectX::SimpleMath::Matrix view �r���[�s��
*	@param DirectX::SimpleMath::Matrix proj �v���W�F�N�V�����s��
* 	@return	�Ȃ�
*/
void PlayerBullet::Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	auto context = m_pCommonResources->GetDeviceResources()->GetD3DDeviceContext();// �f�o�C�X�R���e�L�X�g�̎擾
	auto states = m_pCommonResources->GetCommonStates();// ���ʃX�e�[�g�̎擾
	m_bulletTrail->CreateBillboard(m_cameraTarget, m_cameraEye, m_cameraUp);// �O�Ղ̃r���{�[�h�s����쐬
	m_bulletTrail->Render(view, proj);// �O�Օ`��
	m_pModel->Draw(context, *states, m_worldMatrix, view, proj);// �e�`��
}
/*
*	@brief	�e�̕`��
*	@details �v���C���[�̒e�N���X�̉e�̕`��
*	@param DirectX::SimpleMath::Matrix view �r���[�s��
* 	@param DirectX::SimpleMath::Matrix proj �v���W�F�N�V�����s��
*	@return	�Ȃ�
*/
void PlayerBullet::RenderShadow(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	using namespace DirectX::SimpleMath;
	auto context = m_pCommonResources->GetDeviceResources()->GetD3DDeviceContext();// �f�o�C�X�R���e�L�X�g�̎擾
	auto states = m_pCommonResources->GetCommonStates();// ���ʃX�e�[�g�̎擾
	Vector3 lightDir = Vector3::UnitY;// ���C�g�̕���
	lightDir.Normalize();// ���K��
	Matrix shadowMatrix = Matrix::CreateShadow(Vector3::UnitY, BulletParameters::SHADOW_PLANE);// �e�s��̌������
	shadowMatrix = m_worldMatrix * shadowMatrix;// ���[���h�s��Ɖe�s����|���Z
	m_pModel->Draw(context, *states, shadowMatrix, view, proj, true, [&]()// �e�`��
		{
			context->OMSetBlendState(states->Opaque(), nullptr, 0xffffffff);// �u�����h�X�e�[�g�̐ݒ�
			context->OMSetDepthStencilState(states->DepthNone(), 0);// �[�x�X�e���V���X�e�[�g�̐ݒ�
			context->RSSetState(states->CullNone());	// ���X�^���C�U�[�X�e�[�g�̐ݒ�
			context->PSSetShader(m_pPixelShader.Get(), nullptr, 0);// �s�N�Z���V�F�[�_�[�̐ݒ�
		});
}
/*
*	@brief	�����蔻��̕`��
*	@details �v���C���[�̒e�N���X�̓����蔻��̕`��
*	@param DirectX::SimpleMath::Matrix view �r���[�s��
*	@param DirectX::SimpleMath::Matrix proj �v���W�F�N�V�����s��
*	@return	�Ȃ�
*/
void PlayerBullet::DrawCollision(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	UNREFERENCED_PARAMETER(view);// ���g�p�x����\��
	UNREFERENCED_PARAMETER(proj);// ���g�p�x����\��
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
#ifdef _DEBUG// �f�o�b�O�r���h���̂ݎ��s
	DrawCollision::DrawStart(view, proj);// �����蔻��̕`��J�n
	DrawCollision::DrawBoundingSphere(m_boundingSphere, Colors::Blue);// �����蔻��̕`��
	DrawCollision::DrawEnd();// �����蔻��̕`��I��
#endif
}
