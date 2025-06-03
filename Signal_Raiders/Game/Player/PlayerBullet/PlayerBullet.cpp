/*
*	@file	PlayerBullet.cpp
*	@brief	�v���C���[�̒e�N���X
*/
#include <pch.h>
#include "PlayerBullet.h"
/*
*	@brief	�R���X�g���N�^
*	@details �v���C���[�̒e�N���X�̃R���X�g���N�^
*	@param	�Ȃ�
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
*	@param	�Ȃ�
*	@return	�Ȃ�
*/
PlayerBullet::~PlayerBullet()
{
	// �s�N�Z���V�F�[�_�[�̉��
	m_pPixelShader.Reset();
	// �O�Ճ|�C���^�[�̉��
	m_pBulletTrail.reset();
	// ���ʃ��\�[�X�̉��
	m_pCommonResources = nullptr;
	// ���f���|�C���^�[�̉��
	m_pModel = nullptr;
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
	// ���ʃ��\�[�X�̐ݒ�
	m_pCommonResources = resources;
	// �f�o�C�X�̎擾
	auto device = m_pCommonResources->GetDeviceResources()->GetD3DDevice();
	// �����蔻������p�N���X�̏�����
	DrawCollision::Initialize(m_pCommonResources);
	// �e�̋O������
	m_pBulletTrail = std::make_unique<Particle>(ParticleUtility::Type::PLAYERTRAIL, BulletParameters::PLAYER_BULLET_SIZE);
	// �e�̋O�������̏�����
	m_pBulletTrail->Initialize(m_pCommonResources);
	// �e�p�̃s�N�Z���V�F�[�_�[�ǂݍ���
	std::vector<uint8_t> ps = DX::ReadData(L"Resources/Shaders/Shadow/PS_Shadow.cso");
	// �s�N�Z���V�F�[�_�[�̍쐬
	DX::ThrowIfFailed(device->CreatePixelShader(ps.data(), ps.size(), nullptr, m_pPixelShader.ReleaseAndGetAddressOf()));
	// �}�l�[�W���[���烂�f�����擾
	m_pModel = m_pCommonResources->GetModelManager()->GetModel("PlayerBullet");
	// �e�̈ړ�����
	m_direction = Vector3::Zero;
	// �e�̈ړ���
	m_velocity = Vector3::Zero;
	// �e�̈ʒu
	m_position = Vector3::Zero;
	// �e�̒��S���W
	m_boundingSphere.Center = m_position;
	// �e�̔��a
	m_boundingSphere.Radius = BulletParameters::COLLISION_RADIUS;
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
	// �e�̉�]���x�����Z
	m_angle += BulletParameters::BULLET_ROTATION_SPEED;
	// �p�x��0�`360�x�ɐ�������
	Clamp(m_angle, BulletParameters::ANGLE_MIN, BulletParameters::ANGLE_MAX);
	// �J�����������Ă�������ɑ��x��^����
	m_velocity += m_direction;
	// ���x�𐳋K��
	if (m_velocity.LengthSquared() > 0)m_velocity.Normalize();
	// �ړ��ʂ�␳����
	m_velocity *= BulletParameters::ADJUST_MOVE * elapsedTime;
	// ���ۂɈړ�����
	m_position += m_velocity;
	// �o�E���f�B���O�X�t�B�A�̈ʒu�X�V
	m_boundingSphere.Center = m_position;
	// ���݂̒e�̈ʒu���O�Ճ��X�g�ɒǉ�
	m_pBulletTrail->SetBulletPosition(m_position);
	// �O�Ղ̍X�V
	m_pBulletTrail->Update(elapsedTime);
	// ���Ԍv��
	m_time += elapsedTime;
	// �e�̃T�C�Y��ݒ�
	m_worldMatrix = Matrix::CreateScale(BulletParameters::PLAYER_BULLET_SIZE);
	// �e�̉�]��ݒ�
	m_worldMatrix *= Matrix::CreateRotationX(DirectX::XMConvertToRadians(m_angle));
	// �e�̍��W��ݒ�
	m_worldMatrix *= Matrix::CreateTranslation(m_position);
}
/*
*	@brief	�e�𐶐�
*	@details �v���C���[�̒e�N���X�̒e�𐶐�
*	@param DirectX::SimpleMath::Vector3& pos �e�̈ʒu
*	@param DirectX::SimpleMath::Vector3& dir �e�̈ړ�����
*	@return	�Ȃ�
*/
void PlayerBullet::MakeBall(const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& dir)
{
	using namespace DirectX::SimpleMath;
	// �����ʒu��ݒ�
	m_position = pos + BulletParameters::INITIAL_POSITION;
	// ������ݒ�
	m_direction = dir;
	// ������ɕ␳
	m_direction.y += BulletParameters::ADJUST_DIRECTION;
}
/*
*	@brief	�`��
*	@details �v���C���[�̒e�N���X�̕`��
*	@param DirectX::SimpleMath::Matrix view �r���[�s��
*	@param DirectX::SimpleMath::Matrix proj �v���W�F�N�V�����s��
* 	@return	�Ȃ�
*/
void PlayerBullet::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj)
{
	// �f�o�C�X�R���e�L�X�g�̎擾
	auto context = m_pCommonResources->GetDeviceResources()->GetD3DDeviceContext();
	// ���ʃX�e�[�g�̎擾
	auto states = m_pCommonResources->GetCommonStates();
	// �O�Ղ̃r���{�[�h�s����쐬
	m_pBulletTrail->CreateBillboard(m_cameraTarget, m_cameraEye, m_cameraUp);
	// �O�Օ`��
	m_pBulletTrail->Render(view, proj);
	// �e�`��
	m_pModel->Draw(context, *states, m_worldMatrix, view, proj);
}
/*
*	@brief	�e�̕`��
*	@details �v���C���[�̒e�N���X�̉e�̕`��
*	@param DirectX::SimpleMath::Matrix view �r���[�s��
* 	@param DirectX::SimpleMath::Matrix proj �v���W�F�N�V�����s��
*	@return	�Ȃ�
*/
void PlayerBullet::RenderShadow(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj)
{
	using namespace DirectX::SimpleMath;
	// �f�o�C�X�R���e�L�X�g�̎擾
	auto context = m_pCommonResources->GetDeviceResources()->GetD3DDeviceContext();
	// ���ʃX�e�[�g�̎擾
	auto states = m_pCommonResources->GetCommonStates();
	// ���C�g�̕���
	Vector3 lightDir = Vector3::UnitY;
	// ���C�g�̕�����ݒ�
	lightDir.Normalize();
	// �e�s��̌������
	Matrix shadowMatrix = Matrix::CreateShadow(Vector3::UnitY, BulletParameters::SHADOW_PLANE);
	// ���[���h�s��Ɖe�s����|����
	shadowMatrix = m_worldMatrix * shadowMatrix;
	// �e��`��
	m_pModel->Draw(context, *states, shadowMatrix, view, proj, true, [&]()
		{
			// �u�����h�X�e�[�g�̐ݒ�
			context->OMSetBlendState(states->Opaque(), nullptr, 0xffffffff);
			// �[�x�X�e���V���X�e�[�g�̐ݒ�
			context->OMSetDepthStencilState(states->DepthNone(), 0);
			// ���X�^���C�U�[�X�e�[�g�̐ݒ�
			context->RSSetState(states->CullNone());
			// �s�N�Z���V�F�[�_�[�̐ݒ�
			context->PSSetShader(m_pPixelShader.Get(), nullptr, 0);
		});
}
/*
*	@brief	�����蔻��̕`��
*	@details �v���C���[�̒e�N���X�̓����蔻��̕`��
*	@param DirectX::SimpleMath::Matrix view �r���[�s��
*	@param DirectX::SimpleMath::Matrix proj �v���W�F�N�V�����s��
*	@return	�Ȃ�
*/
void PlayerBullet::DrawCollision(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// ���g�p�x����\��
	UNREFERENCED_PARAMETER(view);
	UNREFERENCED_PARAMETER(proj);
#ifdef _DEBUG// �f�o�b�O�r���h���̂ݎ��s
	// �����蔻��̕`��J�n
	DrawCollision::DrawStart(view, proj);
	// �����蔻��̕`��
	DrawCollision::DrawBoundingSphere(m_boundingSphere, Colors::Blue);
	// �����蔻��̕`��I��
	DrawCollision::DrawEnd();
#endif
}