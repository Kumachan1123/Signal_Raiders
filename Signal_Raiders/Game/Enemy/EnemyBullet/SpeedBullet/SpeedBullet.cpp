/*
*	@file SpeedBullet.cpp
*	@brief �G�e�N���X(�����e)
*/
#include <pch.h>
#include "SpeedBullet.h"
/*
*	@brief �R���X�g���N�^
*	@return �Ȃ�
*/
SpeedBullet::SpeedBullet()
	: m_pEnemyBullet(nullptr)// �G�e�|�C���^�[
{
}
/*
*	@brief �f�X�g���N�^
*	@return �Ȃ�
*/
SpeedBullet::~SpeedBullet() {/*do nothing*/ }
/*
*	@brief �������֐�
*	@return �Ȃ�
*/
void SpeedBullet::Initialize()
{
	m_position = m_pEnemyBullet->GetPosition();// �e�̍��W
	m_velocity = m_pEnemyBullet->GetVelocity();// �e�̑��x
	m_direction = m_pEnemyBullet->GetDirection();// �e�̕���
	m_boundingSphere = m_pEnemyBullet->GetBoundingSphere();// �u�e�v���E��
}
/*
*	@brief �X�V�֐�
*	@param[in] float elapsedTime �X�V����
*	@return �Ȃ�
*/
void SpeedBullet::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;
	if (m_position.y >= BulletParameters::VERTICAL_BULLET_LANDING_POSITION)// ���e�ʒu�ɓ��B���Ă��Ȃ�
		m_velocity = BulletParameters::VERTICAL_BULLET_LANDING_VELOCITY * elapsedTime * BulletParameters::VERTICAL_BULLET_SPEED;// �^���ɗ��Ƃ�
	else//���e���Ă���
	{
		// �v���C���[�̕����x�N�g�����v�Z
		Vector3 toPlayer = m_pEnemyBullet->GetTarget() - m_pEnemyBullet->GetEnemyPosition();
		// �x�N�g���𐳋K��
		if (toPlayer.LengthSquared() > 0)
			toPlayer.Normalize();
		m_direction = Vector3(toPlayer.x, 0, toPlayer.z) * elapsedTime * BulletParameters::VERTICAL_BULLET_SPEED;// �e�̕������v���C���[�̕����Ɍ�����
		m_velocity = m_direction;// �e�̑��x���v���C���[�̕����Ɍ�����
	}
	m_position += m_velocity;// �v���C���[�̕����Ɍ������Ēe���΂�
	m_boundingSphere.Center = m_position;// ���E���̒��S���W��e�̍��W�ɍX�V
	// �e�̊e������X�V
	m_pEnemyBullet->SetDirection(m_direction);// �e�̕���
	m_pEnemyBullet->SetVelocity(m_velocity);// �e�̑��x
	m_pEnemyBullet->SetPosition(m_position);// �e�̍��W
	m_pEnemyBullet->SetBoundingSphere(m_boundingSphere);// �u�e�v���E��
}
