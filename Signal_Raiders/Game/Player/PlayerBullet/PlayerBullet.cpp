/*
	@file	PlayerBullet.cpp
	@brief	�v���C���[�̒e�N���X
*/
#include "pch.h"
#include <SimpleMath.h>
#include "Game/Player/PlayerBullet/PlayerBullet.h"
#include "Game/KumachiLib/KumachiLib.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/InputManager.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include <cassert>
#include <Libraries/Microsoft/DebugDraw.h>
#include "Libraries/Microsoft/ReadData.h"
#include "Game/KumachiLib//DrawCollision/DrawCollision.h"
//-------------------------------------------------------------------
// �R���X�g���N�^
//-------------------------------------------------------------------
PlayerBullet::PlayerBullet()
	: m_position{ DirectX::SimpleMath::Vector3::Zero }
	, m_velocity{ DirectX::SimpleMath::Vector3::Zero }
	, m_direction{ DirectX::SimpleMath::Vector3::Zero }
	, m_commonResources{}
	, m_time(0.0f)
	, m_angle{ 0.0f }
{
}
//-------------------------------------------------------------------
// �f�X�g���N�^
//-------------------------------------------------------------------
PlayerBullet::~PlayerBullet()
{
}
void PlayerBullet::MoveStop()
{
}

void PlayerBullet::Initialize(CommonResources* resources)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	m_commonResources = resources;
	auto device = m_commonResources->GetDeviceResources()->GetD3DDevice();
	// �����蔻������p�N���X�̏�����
	DrawCollision::Initialize(m_commonResources);
	// �e�̋O���|�C���^�[
	m_bulletTrail = std::make_unique<Particle>(ParticleUtility::Type::PLAYERTRAIL, BulletParameters::PLAYER_BULLET_SIZE);
	m_bulletTrail->Initialize(m_commonResources);

	// �e�p�̃s�N�Z���V�F�[�_�[
	std::vector<uint8_t> ps = DX::ReadData(L"Resources/Shaders/Shadow/PS_Shadow.cso");
	DX::ThrowIfFailed(device->CreatePixelShader(ps.data(), ps.size(), nullptr, m_pixelShader.ReleaseAndGetAddressOf()));

	// ���f����ǂݍ��ޏ���
	std::unique_ptr<DirectX::EffectFactory> fx = std::make_unique<DirectX::EffectFactory>(device);
	fx->SetDirectory(L"Resources/Models");
	// ���f����ǂݍ���
	m_model = DirectX::Model::CreateFromCMO(device, L"Resources/Models/Bullet.cmo", *fx);
	m_model->UpdateEffects([&](DirectX::IEffect* effect)
		{
			// �x�C�V�b�N�G�t�F�N�g���擾����
			auto basicEffect = dynamic_cast<DirectX::BasicEffect*>(effect);

			// �f�B�t���[�Y�J���[��ݒ肷��
			basicEffect->SetDiffuseColor(DirectX::Colors::SkyBlue);

			// �G�~�b�V�����J���[��ݒ肷��
			basicEffect->SetEmissiveColor(DirectX::Colors::Cyan);
		});


	m_direction = Vector3::Zero;
	m_velocity = Vector3::Zero;
	m_position = Vector3::Zero;
	m_boundingSphere.Center = m_position;
	m_boundingSphere.Radius = BulletParameters::COLLISION_RADIUS;

}
// �X�V
void PlayerBullet::Update(float elapsedTime)
{
	m_angle += BulletParameters::BULLET_ROTATION_SPEED;
	Clamp(m_angle, 0.0f, 360.0f);// �p�x��0�`360�x�ɐ�������
	// �J�����������Ă�������ɑ��x��^����
	m_velocity += m_direction;
	// �ړ��ʂ𐳋K������
	if (m_velocity.LengthSquared() > 0)m_velocity.Normalize();
	// �ړ��ʂ�␳����
	m_velocity *= BulletParameters::ADJUST_MOVE;
	// ���ۂɈړ�����
	m_position += m_velocity;
	// �o�E���f�B���O�X�t�B�A�̈ʒu�X�V
	m_boundingSphere.Center = m_position;
	// ���݂̒e�̈ʒu���O�Ճ��X�g�ɒǉ�
	m_bulletTrail->SetBulletPosition(m_position);
	// �O�Ղ̍X�V
	m_bulletTrail->Update(elapsedTime);
	// ���Ԍv��
	m_time += elapsedTime;

}
// �e�̏����ʒu��ݒ�
void PlayerBullet::MakeBall(const DirectX::SimpleMath::Vector3& pos, DirectX::SimpleMath::Vector3& dir)
{
	using namespace DirectX::SimpleMath;
	m_position = pos + BulletParameters::INITIAL_POSITION;// �����ʒu��ݒ�
	m_direction = dir;// ������ݒ�
	m_direction.y += BulletParameters::ADJUST_DIRECTION;// ������ɕ␳
}
void PlayerBullet::Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	auto context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();
	auto states = m_commonResources->GetCommonStates();
	// �e�̃T�C�Y��ݒ�
	m_worldMatrix = Matrix::CreateScale(BulletParameters::PLAYER_BULLET_SIZE);
	m_worldMatrix *= Matrix::CreateRotationX(DirectX::XMConvertToRadians(m_angle));
	// �e�̍��W��ݒ�
	m_worldMatrix *= Matrix::CreateTranslation(m_position);
	// �O�Օ`��
	m_bulletTrail->CreateBillboard(m_cameraTarget, m_cameraEye, m_cameraUp);
	m_bulletTrail->Render(view, proj);
	// �e�`��
	m_model->Draw(context, *states, m_worldMatrix, view, proj);

}

void PlayerBullet::RenderShadow(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	using namespace DirectX::SimpleMath;
	auto context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();
	auto states = m_commonResources->GetCommonStates();
	// ���C�g�̕���
	Vector3 lightDir = Vector3::UnitY;
	lightDir.Normalize();
	// �e�s��̌������
	Matrix shadowMatrix = Matrix::CreateShadow(Vector3::UnitY, BulletParameters::SHADOW_PLANE);
	shadowMatrix = m_worldMatrix * shadowMatrix;
	// �e�`��
	m_model->Draw(context, *states, shadowMatrix, view, proj, true, [&]()
		{
			context->OMSetBlendState(states->Opaque(), nullptr, 0xffffffff);
			context->OMSetDepthStencilState(states->DepthNone(), 0);
			context->RSSetState(states->CullNone());
			context->PSSetShader(m_pixelShader.Get(), nullptr, 0);
		});
}

void PlayerBullet::DrawCollision(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	UNREFERENCED_PARAMETER(view);
	UNREFERENCED_PARAMETER(proj);
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
#ifdef _DEBUG
	DrawCollision::DrawStart(view, proj);
	DrawCollision::DrawBoundingSphere(m_boundingSphere, Colors::Blue);
	DrawCollision::DrawEnd();
#endif
}
