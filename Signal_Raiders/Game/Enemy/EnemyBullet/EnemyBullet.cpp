#include "pch.h"
#include <SimpleMath.h>
#include "Game/Enemy/EnemyBullet/EnemyBullet.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/InputManager.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include "Libraries/Microsoft/ReadData.h"
#include <cassert>
#include <Libraries/Microsoft/DebugDraw.h>
#include "Game/KumachiLib/DrawCollision/DrawCollision.h"
//-------------------------------------------------------------------
// �R���X�g���N�^
//-------------------------------------------------------------------
EnemyBullet::EnemyBullet(float size)
	:m_position{}
	, m_velocity{}
	, m_commonResources{}
	, m_time{ 0.0f }
	, m_angle{ 0.0f }
	, m_size{ size }
	, m_spiralAngle{ 0.0f }
	, m_bulletSpeed{ 0.5f }
	, m_rotateDirection{ 1 }
	, m_bulletType{ BulletType::STRAIGHT }
{
}
//-------------------------------------------------------------------
// �f�X�g���N�^
//-------------------------------------------------------------------
EnemyBullet::~EnemyBullet()
{
}
void EnemyBullet::Initialize(CommonResources* resources, BulletType type)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	m_commonResources = resources;
	auto device = m_commonResources->GetDeviceResources()->GetD3DDevice();
	m_bulletType = type;// �e�̎�ނ�ݒ肷��
	DrawCollision::Initialize(m_commonResources);
	// �e�p�̃s�N�Z���V�F�[�_�[
	std::vector<uint8_t> ps = DX::ReadData(L"Resources/Shaders/Shadow/PS_EnemyShadow.cso");
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
			basicEffect->SetAlpha(.5f);// �A���t�@�l��ݒ肷��
			basicEffect->SetDiffuseColor(DirectX::Colors::Pink);// �f�B�t���[�Y�J���[��ݒ肷��
			basicEffect->SetAmbientLightColor(DirectX::Colors::Pink);// �A���r�G���g���C�g�J���[��ݒ肷��
			basicEffect->SetEmissiveColor(DirectX::Colors::Magenta);// �G�~�b�V�u�J���[��ݒ肷��

		});
	// �e�̋O������
	m_bulletTrail = std::make_unique<Particle>(ParticleUtility::Type::ENEMYTRAIL, m_size);
	m_bulletTrail->Initialize(resources);
	m_direction = Vector3::Zero;// ������������
	m_velocity = Vector3::Zero;// ���x��������
	m_position = Vector3::Zero;// �ʒu��������

	m_boundingSphere.Center = m_position;
	m_boundingSphere.Radius = m_size * 2;
}
// �e�̏����ʒu��ݒ�
void EnemyBullet::MakeBall(const DirectX::SimpleMath::Vector3& pos, DirectX::SimpleMath::Vector3& dir, DirectX::SimpleMath::Vector3& target)
{
	using namespace DirectX::SimpleMath;
	m_position = pos;
	m_direction = dir;
	m_target = target;
	m_bulletSpeed = 0.5f;
}

// �X�V
void EnemyBullet::Update(DirectX::SimpleMath::Vector3& pos, float elapsedTime)
{
	// �p�x�𑝉�������
	m_angle += 6.0f;
	if (m_angle > 360) m_angle = 0;

	if (m_bulletType == BulletType::SPIRAL)   SpiralBullet();
	else if (m_bulletType == BulletType::STRAIGHT) StraightBullet(pos);
	else if (m_bulletType == BulletType::VERTICAL) VerticalBullet(pos);

	// ���݂̒e�̈ʒu���O�Ճ��X�g�ɒǉ�
	m_bulletTrail->SetBulletPosition(m_position);

	// �O�Ղ̍X�V
	m_bulletTrail->Update(elapsedTime);
	m_time += elapsedTime; // �o�ߎ��Ԃ��X�V

}

void EnemyBullet::Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	using namespace DirectX::SimpleMath;
	auto context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();
	auto states = m_commonResources->GetCommonStates();
	// �O�Օ`��
	m_bulletTrail->CreateBillboard(m_cameraTarget, m_cameraEye, m_cameraUp);
	m_bulletTrail->Render(view, proj);
	// �e�`��
	m_model->Draw(context, *states, BulletWorldMatrix(), view, proj);

}

void EnemyBullet::RenderShadow(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	using namespace DirectX::SimpleMath;
	auto context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();
	auto states = m_commonResources->GetCommonStates();
	// ���C�g�̕���
	Vector3 lightDir = Vector3::UnitY;
	lightDir.Normalize();
	// �e�s��̌������
	Matrix shadowMatrix = Matrix::CreateShadow(Vector3::UnitY, Plane(0.0f, 1.0f, 0.0f, 0.01f));
	shadowMatrix = BulletWorldMatrix() * shadowMatrix;
	// �e�`��
	m_model->Draw(context, *states, shadowMatrix * Matrix::Identity, view, proj, true, [&]()
		{
			context->OMSetBlendState(states->Opaque(), nullptr, 0xffffffff);
			context->OMSetDepthStencilState(states->DepthNone(), 0);
			context->RSSetState(states->CullClockwise());
			context->PSSetShader(m_pixelShader.Get(), nullptr, 0);
		});
}

void EnemyBullet::RenderBoundingSphere(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	UNREFERENCED_PARAMETER(view);
	UNREFERENCED_PARAMETER(proj);
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
#ifdef _DEBUG
	DrawCollision::DrawStart(view, proj);
	Matrix boundingbulletWorld = Matrix::CreateScale(Vector3::One);
	boundingbulletWorld *= Matrix::CreateTranslation(m_position);
	// ���E���̕ϊ��𓯂����[���h�}�g���b�N�X�Ɋ�Â��čs��
	DirectX::BoundingSphere transformedBoundingSphere = m_boundingSphere;
	m_boundingSphere.Transform(transformedBoundingSphere, boundingbulletWorld);
	DrawCollision::DrawBoundingSphere(m_boundingSphere, Colors::Red);
	DrawCollision::DrawEnd();
#endif

}

// �����e
void EnemyBullet::StraightBullet(DirectX::SimpleMath::Vector3& pos)
{	// �v���C���[�̕����x�N�g�����v�Z
	DirectX::SimpleMath::Vector3 toPlayer = m_target - pos;
	// �x�N�g���𐳋K��
	if (toPlayer.LengthSquared() > 0)
	{
		toPlayer.Normalize();
	}
	// �e�̕������v���C���[�̕����Ɍ�����
	m_direction = toPlayer;
	m_direction.y -= 0.1f;// ���Ɏ኱���炷
	// �e�̑��x��x������
	float bulletSpeed = .2f; // �K���ȑ��x��ݒ肷��i�C�ӂ̒l�A�����\�j
	m_velocity = m_direction * bulletSpeed;
	// �v���C���[�̕����Ɍ������Ēe���΂�
	m_position += m_velocity;
	m_boundingSphere.Center = m_position;//���E���ɍ��W��n��
}

DirectX::SimpleMath::Matrix EnemyBullet::BulletWorldMatrix()
{
	using namespace DirectX::SimpleMath;
	// �e�̃T�C�Y��ݒ�
	Matrix bulletWorld = Matrix::CreateScale(m_size);
	// �e�̎��]
	bulletWorld *= Matrix::CreateRotationX(DirectX::XMConvertToRadians(m_angle));
	// �e�̍��W��ݒ�
	bulletWorld *= Matrix::CreateTranslation(m_position);
	return bulletWorld;
}

// �������i�e
void EnemyBullet::VerticalBullet(DirectX::SimpleMath::Vector3& pos)
{
	using namespace DirectX::SimpleMath;
	if (m_position.y >= 0.50f)
	{
		// �^���ɗ��Ƃ�
		m_velocity = Vector3(0.0f, -0.15f, 0.0f);
	}
	else//���e���Ă���
	{
		// �v���C���[�̕����x�N�g�����v�Z
		Vector3 toPlayer = m_target - pos;
		// �x�N�g���𐳋K��
		if (toPlayer.LengthSquared() > 0)
		{
			toPlayer.Normalize();
		}
		// �e�̕������v���C���[�̕����Ɍ�����
		m_direction = Vector3(toPlayer.x, 0, toPlayer.z);
		// �e�̑��x��x������
		float bulletSpeed = 1.f; // �K���ȑ��x��ݒ肷��i�C�ӂ̒l�A�����\�j
		m_velocity = m_direction * bulletSpeed;
	}
	m_position += m_velocity;
	m_boundingSphere.Center = m_position;
}

// �����e
void EnemyBullet::SpiralBullet()
{
	// �v���C���[�����x�N�g�����v�Z
	DirectX::SimpleMath::Vector3 toPlayer = m_target - m_position;

	// �v���C���[�Ƃ̋������擾���ăX�p�C�����̔��a�Ɏg��
	float distanceToPlayer = toPlayer.Length();
	if (distanceToPlayer > 0)
	{
		toPlayer.Normalize();
	}

	// �v���C���[���S�ɉ~��`���悤�ɃI�t�Z�b�g�v�Z
	float spiralRadius = 1.0f; // �v���C���[���S����̃X�p�C�����̔��a
	float spiralSpeed = 5.0f;  // �X�p�C�����̉�]���x

	// ���Ԃɂ���ăv���C���[�������]����ʒu��ݒ�
	DirectX::SimpleMath::Vector3 spiralOffset = {
		0.0f,
		spiralRadius * cosf(spiralSpeed * m_time * 2.5f) * distanceToPlayer * 0.1f,

		spiralRadius * sinf(spiralSpeed * m_time * 2.5f) * distanceToPlayer * 0.1f * m_rotateDirection
	};

	// �v���C���[�Ɍ����������ƃX�p�C�������ʂ��~�b�N�X
	m_direction = toPlayer + spiralOffset;
	m_direction.Normalize();

	// �e�̑��x��ݒ�
	m_velocity = m_direction * m_bulletSpeed;

	// �v���C���[�Ɍ������X�p�C������`���Ĉړ�
	m_position += m_velocity;
	m_boundingSphere.Center = m_position;
	// �e�̑��x�������x������
	m_bulletSpeed -= 0.001f;
}