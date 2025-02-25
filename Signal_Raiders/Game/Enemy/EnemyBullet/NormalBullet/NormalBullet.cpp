#include <pch.h>
#include "NormalBullet.h"
using namespace DirectX::SimpleMath;

NormalBullet::NormalBullet()
	: m_pEnemyBullet(nullptr)
{
}

NormalBullet::~NormalBullet()
{
}

void NormalBullet::Initialize()
{
	m_position = m_pEnemyBullet->GetPosition();// �e�̍��W
	m_velocity = m_pEnemyBullet->GetVelocity();// �e�̑��x
	m_direction = m_pEnemyBullet->GetDirection();// �e�̕���
	m_boundingSphere = m_pEnemyBullet->GetBoundingSphere();// �u�e�v���E��

}

void NormalBullet::Update(float elapsedTime)
{
	// �v���C���[�̕����x�N�g�����v�Z
	Vector3 toPlayer = m_pEnemyBullet->GetTarget() - m_pEnemyBullet->GetEnemyPosition();
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
	// �e�̊e������X�V
	m_pEnemyBullet->SetDirection(m_direction);// �e�̕���
	m_pEnemyBullet->SetVelocity(m_velocity);// �e�̑��x
	m_pEnemyBullet->SetPosition(m_position);// �e�̍��W
	m_pEnemyBullet->SetBoundingSphere(m_boundingSphere);// �u�e�v���E��
}
