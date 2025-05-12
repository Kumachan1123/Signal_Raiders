#include "pch.h"
#include "EnemyBullet.h"

using namespace DirectX::SimpleMath;
//-------------------------------------------------------------------
// �R���X�g���N�^
//-------------------------------------------------------------------
EnemyBullet::EnemyBullet(float size)
	:m_position{}
	, m_velocity{}
	, m_commonResources{}
	, m_time{ 0.0f }
	, m_elapsedTime{ 0.0f }
	, m_angle{ 0.0f }
	, m_size{ size }
	, m_spiralAngle{ 0.0f }
	, m_bulletSpeed{ 0.0f }
	, m_distance{ 5.0f }
	, m_rotateDirection{ 1 }
	, m_rotationSpeed{ 0.0f }
	, m_height{ 0.0f }
	, m_isExpand{ false }
	, m_isShot{ false }
	, m_pShooter{ nullptr }
	, m_pSpecialBullet{ nullptr }
	, m_pNormalBullet{ nullptr }
	, m_pSpeedBullet{ nullptr }
	, m_pEnemyBullet{ nullptr }
	, m_bulletType{ BulletType::NORMAL }
{
}
//-------------------------------------------------------------------
// �f�X�g���N�^
//-------------------------------------------------------------------
EnemyBullet::~EnemyBullet()
{
}
void EnemyBullet::Initialize(CommonResources* resources)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	m_commonResources = resources;
	auto device = m_commonResources->GetDeviceResources()->GetD3DDevice();
	m_pEnemyBullet = EnemyBulletFactory::CreateBullet(m_bulletType);// �t�@�N�g���Ő���
	m_pEnemyBullet->SetEnemyBullet(this);// �G�e�|�C���^�[��ݒ肷��
	// ���E���̏�����
	DrawCollision::Initialize(m_commonResources);
	// �e�p�̃s�N�Z���V�F�[�_�[
	std::vector<uint8_t> ps = DX::ReadData(L"Resources/Shaders/Shadow/PS_Shadow.cso");
	DX::ThrowIfFailed(device->CreatePixelShader(ps.data(), ps.size(), nullptr, m_pixelShader.ReleaseAndGetAddressOf()));

	// ���f����ǂݍ��ޏ���
	std::unique_ptr<DirectX::EffectFactory> fx = std::make_unique<DirectX::EffectFactory>(device);
	fx->SetDirectory(L"Resources/Models");
	// ���f����ǂݍ���
	m_model = DirectX::Model::CreateFromCMO(device, L"Resources/Models/Bullet.cmo", *fx);
	// ���f���̃G�t�F�N�g��ݒ肷��
	m_model->UpdateEffects([&](DirectX::IEffect* effect)
		{
			// �x�C�V�b�N�G�t�F�N�g���擾����
			auto basicEffect = dynamic_cast<DirectX::BasicEffect*>(effect);
			basicEffect->SetDiffuseColor(DirectX::SimpleMath::Vector4(1, .2, 0, 1));// �f�B�t���[�Y�J���[��ݒ肷��
			basicEffect->SetAmbientLightColor(DirectX::Colors::Red);// �A���r�G���g���C�g�J���[��ݒ肷��
			basicEffect->SetEmissiveColor(DirectX::Colors::Tomato);// �G�~�b�V�u�J���[��ݒ肷��

		});
	// �e�̋O������
	m_bulletTrail = std::make_unique<Particle>(ParticleUtility::Type::ENEMYTRAIL, m_size);
	m_bulletTrail->Initialize(resources);
	m_direction = Vector3::Zero;// ������������
	m_velocity = Vector3::Zero;// ���x��������
	m_position = Vector3::Zero;// �ʒu��������
	// ���E���̏�����
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
	m_pEnemyBullet->Initialize();
}

// �e����������Ă���̌o�ߎ��Ԃ������𒴂������ǂ����𔻒肷��
bool EnemyBullet::IsExpired() const
{
	if (m_bulletType == BulletType::SPECIAL)
		return GetTime() >= BulletParameters::SPIRAL_BULLET_LIFETIME;
	else
		return GetTime() >= BulletParameters::ENEMY_BULLET_LIFETIME;
}

// �X�V
void EnemyBullet::Update(float elapsedTime)
{
	// �p�x�𑝉�������
	m_angle += BulletParameters::BULLET_ROTATION_SPEED;
	Clamp(m_angle, BulletParameters::ANGLE_MIN, BulletParameters::ANGLE_MAX);// �p�x��0�`360�x�ɐ�������

	// �e�̍X�V
	m_pEnemyBullet->Update(elapsedTime);

	// ���݂̒e�̈ʒu���O�Ճ��X�g�ɒǉ�
	m_bulletTrail->SetBulletPosition(m_position);

	// �O�Ղ̍X�V
	m_bulletTrail->Update(elapsedTime);

	// �o�ߎ��Ԃ��X�V
	m_time += elapsedTime;

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
	Matrix shadowMatrix = Matrix::CreateShadow(Vector3::UnitY, BulletParameters::SHADOW_PLANE);
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


// �e�̃��[���h�s����擾
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

