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
	m_toPlayer = m_pEnemyBullet->GetTarget() - m_position;// // �v���C���[�̕����x�N�g�����v�Z
	m_toPlayer.Normalize();// �x�N�g���𐳋K��
}

void NormalBullet::Update(float elapsedTime)
{

	// �e�̕������v���C���[�̕����Ɍ�����
	m_direction = m_toPlayer;
	m_direction.y -= BulletParameters::STRAIGHT_ADJUST_DIRECTION;// ���Ɏ኱���炷
	// �e�̑��x��x������
	m_velocity = m_direction * BulletParameters::STRAIGHT_BULLET_SPEED * elapsedTime;
	// �v���C���[�̕����Ɍ������Ēe���΂�
	m_position += m_velocity;
	//m_position = Lerp(m_position, m_pEnemyBullet->GetTarget() * 2, elapsedTime);
	m_boundingSphere.Center = m_position;//���E���ɍ��W��n��
	// �e�̊e������X�V
	m_pEnemyBullet->SetDirection(m_direction);// �e�̕���
	m_pEnemyBullet->SetVelocity(m_velocity);// �e�̑��x
	m_pEnemyBullet->SetPosition(m_position);// �e�̍��W
	m_pEnemyBullet->SetBoundingSphere(m_boundingSphere);// �u�e�v���E��
}
