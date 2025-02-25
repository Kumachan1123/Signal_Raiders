#include <pch.h>
#include "SpeedBullet.h"
using namespace DirectX::SimpleMath;

SpeedBullet::SpeedBullet()
	: m_pEnemyBullet(nullptr)
{
}

SpeedBullet::~SpeedBullet()
{
}

void SpeedBullet::Initialize()
{
	m_position = m_pEnemyBullet->GetPosition();// �e�̍��W
	m_velocity = m_pEnemyBullet->GetVelocity();// �e�̑��x
	m_direction = m_pEnemyBullet->GetDirection();// �e�̕���
	m_boundingSphere = m_pEnemyBullet->GetBoundingSphere();// �u�e�v���E��
}

void SpeedBullet::Update(float elapsedTime)
{
	if (m_position.y >= BulletParameters::VERTICAL_BULLET_LANDING_POSITION)// ���e�ʒu�ɓ��B���Ă��Ȃ�
	{
		// �^���ɗ��Ƃ�
		m_velocity = BulletParameters::VERTICAL_BULLET_LANDING_VELOCITY * elapsedTime * BulletParameters::VERTICAL_BULLET_SPEED;
	}
	else//���e���Ă���
	{
		// �v���C���[�̕����x�N�g�����v�Z
		Vector3 toPlayer = m_pEnemyBullet->GetTarget() - m_pEnemyBullet->GetEnemyPosition();
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
	// �e�̊e������X�V
	m_pEnemyBullet->SetDirection(m_direction);// �e�̕���
	m_pEnemyBullet->SetVelocity(m_velocity);// �e�̑��x
	m_pEnemyBullet->SetPosition(m_position);// �e�̍��W
	m_pEnemyBullet->SetBoundingSphere(m_boundingSphere);// �u�e�v���E��
}
