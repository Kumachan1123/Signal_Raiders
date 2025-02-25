#include "pch.h"
#include "EnemyBullet.h"
#include "Game/BulletParameters/BulletParameters.h"
#include "Game/KumachiLib/KumachiLib.h"
#include <SimpleMath.h>
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/InputManager.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include "Libraries/Microsoft/ReadData.h"
#include <cassert>
#include <Libraries/Microsoft/DebugDraw.h>
#include "Game/KumachiLib/DrawCollision/DrawCollision.h"
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
void EnemyBullet::Initialize(CommonResources* resources, BulletType type)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	m_commonResources = resources;
	auto device = m_commonResources->GetDeviceResources()->GetD3DDevice();
	m_bulletType = type;// �e�̎�ނ�ݒ肷��

	switch (m_bulletType)
	{
	case EnemyBullet::BulletType::NORMAL:// �����e
		m_pNormalBullet = std::make_unique<NormalBullet>();
		m_pEnemyBullet = m_pNormalBullet.get();
		break;
	case EnemyBullet::BulletType::SPECIAL:// ����U���̒e
		m_pSpecialBullet = std::make_unique<SpecialBullet>();
		m_pEnemyBullet = m_pSpecialBullet.get();
		break;
	case EnemyBullet::BulletType::SPEED:// �������i�e
		m_pSpeedBullet = std::make_unique<SpeedBullet>();
		m_pEnemyBullet = m_pSpeedBullet.get();
		break;
	}
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
			basicEffect->SetDiffuseColor(DirectX::Colors::Pink);// �f�B�t���[�Y�J���[��ݒ肷��
			basicEffect->SetAmbientLightColor(DirectX::Colors::Magenta);// �A���r�G���g���C�g�J���[��ݒ肷��
			basicEffect->SetEmissiveColor(DirectX::Colors::Magenta);// �G�~�b�V�u�J���[��ݒ肷��

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

	return GetTime() >= BulletParameters::ENEMY_BULLET_LIFETIME;
}

// �X�V
void EnemyBullet::Update(float elapsedTime)
{
	// �p�x�𑝉�������
	m_angle += BulletParameters::BULLET_ROTATION_SPEED;
	Clamp(m_angle, BulletParameters::ANGLE_MIN, BulletParameters::ANGLE_MAX);// �p�x��0�`360�x�ɐ�������

	//// �e�̎�ނɂ���ď����𕪊�
	//switch (m_bulletType)
	//{
	//case BulletType::SPECIAL:// ����U���̒e
	//	SpiralBullet(elapsedTime);
	//	break;
	//case BulletType::NORMAL:// �����e
	//	StraightBullet(elapsedTime);
	//	break;
	//case BulletType::SPEED:// �������i�e
	//	VerticalBullet(elapsedTime);
	//	break;
	//default:
	//	break;
	//}

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

// �����e
void EnemyBullet::StraightBullet(float elapsedTime)
{
	// �v���C���[�̕����x�N�g�����v�Z
	DirectX::SimpleMath::Vector3 toPlayer = m_target - m_enemyPosition;
	// �x�N�g���𐳋K��
	if (toPlayer.LengthSquared() > 0)
	{
		toPlayer.Normalize();
	}
	// �e�̕������v���C���[�̕����Ɍ�����
	m_direction = toPlayer;
	m_direction.y -= BulletParameters::STRAIGHT_ADJUST_DIRECTION;// ���Ɏ኱���炷
	// �e�̑��x��x������
	m_velocity = m_direction * BulletParameters::STRAIGHT_BULLET_SPEED * elapsedTime;
	// �v���C���[�̕����Ɍ������Ēe���΂�
	m_position += m_velocity;
	m_boundingSphere.Center = m_position;//���E���ɍ��W��n��
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

// �������i�e
void EnemyBullet::VerticalBullet(float elapsedTime)
{
	using namespace DirectX::SimpleMath;
	if (m_position.y >= BulletParameters::VERTICAL_BULLET_LANDING_POSITION)// ���e�ʒu�ɓ��B���Ă��Ȃ�
	{
		// �^���ɗ��Ƃ�
		m_velocity = BulletParameters::VERTICAL_BULLET_LANDING_VELOCITY * elapsedTime * BulletParameters::VERTICAL_BULLET_SPEED;
	}
	else//���e���Ă���
	{
		// �v���C���[�̕����x�N�g�����v�Z
		Vector3 toPlayer = m_target - m_enemyPosition;
		// �x�N�g���𐳋K��
		if (toPlayer.LengthSquared() > 0)
		{
			toPlayer.Normalize();
		}
		// �e�̕������v���C���[�̕����Ɍ�����
		m_direction = Vector3(toPlayer.x, 0, toPlayer.z) * elapsedTime * BulletParameters::VERTICAL_BULLET_SPEED;
		m_velocity = m_direction;
	}
	m_position += m_velocity;
	m_boundingSphere.Center = m_position;
}


// ����U���̒e
void EnemyBullet::SpiralBullet(float elapsedTime)
{
	// �o�ߎ��Ԃ��X�V
	m_elapsedTime = elapsedTime;
	// ���v���ɉ�]���邽�߂̊p�x
	m_spiralAngle += m_rotationSpeed * elapsedTime;
	// XY���ʏ�ŉ~�^�� (���v���)
	float xOffset = cosf(m_spiralAngle) * m_distance;
	float zOffset = sinf(m_spiralAngle) * m_distance;
	// ���Ƃ��Ƃ�Y���W�̓����͕ύX���Ȃ�
	m_positionOffSet = Vector3(xOffset, m_basePos.y - 3.5f, zOffset);
	Expand();// �q�I�u�W�F�N�g��W�J
	Shot();// �q�I�u�W�F�N�g�𔭎�
	StopExpand();// �q�I�u�W�F�N�g�����[
	ComeBack();// �q�I�u�W�F�N�g��߂�
	// �v���C���[�Ɍ������X�p�C������`���Ĉړ�
	m_position = m_basePos + m_positionOffSet;
	m_boundingSphere.Center = m_position;
	// �e�̎����ɉ����ăt���O��؂�ւ���
	if (m_time >= BulletParameters::SPECIAL_ATTACK_WAIT_TIME)
	{
		SetIsShot(true);
	}

}

// �q�I�u�W�F�N�g��W�J
void EnemyBullet::Expand()
{
	if (!GetIsExpand())return;
	m_rotationSpeed = 1.0f; // ���x�����p�i�l��傫������Ƒ�����]����j
	m_distance = Lerp(m_distance, 15.0f, m_elapsedTime);
	m_height = 2.0f;
}

// �q�I�u�W�F�N�g�𔭎�
void EnemyBullet::Shot()
{

	if (!GetIsShot()) return;
	m_rotationSpeed = 3.0f;
	m_distance = Lerp(m_distance, 5.0f, m_elapsedTime);
	m_basePos = Lerp(m_basePos, m_currentTarget, m_elapsedTime * 2);

}

// �q�I�u�W�F�N�g�����[
void EnemyBullet::StopExpand()
{
	if (GetIsExpand())return;
	m_rotationSpeed = 0.0f;
	m_distance = Lerp(m_distance, 0.0f, m_elapsedTime * 20);
	m_height = 1.50f;
}

// �q�I�u�W�F�N�g��߂�
void EnemyBullet::ComeBack()
{
	if (GetIsShot()) return;
	//m_look.Normalize();// �v���C���[�������Ă�������𐳋K��
	//// ��_��e�������Ă�������ɓ�����
	m_distance = Lerp(m_distance, 3.0f, m_elapsedTime);
	// ��_��ړI�n�Ɍ������Đ��`�⊮
	m_basePos = Lerp(m_basePos, m_enemyPosition, m_elapsedTime * 50);

}
