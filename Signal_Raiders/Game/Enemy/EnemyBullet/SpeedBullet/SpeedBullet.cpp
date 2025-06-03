/*
*	@file SpeedBullet.cpp
*	@brief �G�e�N���X(�����e)
*/
#include <pch.h>
#include "SpeedBullet.h"
/*
*	@brief �R���X�g���N�^
*	@details �G�e�N���X(�����e)�̃R���X�g���N�^
*	@param �Ȃ�
*	@return �Ȃ�
*/
SpeedBullet::SpeedBullet()
	: m_pEnemyBullet(nullptr)// �G�e�|�C���^�[
	, m_pCommonResources(nullptr)// ���ʃ��\�[�X
	, m_seVolume(0.0f)// SE����
{
}
/*
*	@brief �f�X�g���N�^
*	@details �e��|�C���^�[��nullptr�ɐݒ�
*	@param �Ȃ�
*	@return �Ȃ�
*/
SpeedBullet::~SpeedBullet()
{
	// ���ʃ��\�[�X�̃|�C���^�[��nullptr�ɐݒ�
	m_pCommonResources = nullptr;
	// �G�e�̃|�C���^�[��nullptr�ɐݒ�
	m_pEnemyBullet = nullptr;
}
/*
*	@brief �������֐�
*	@details �G�e�N���X(�����e)�̏������֐�
*	@param �Ȃ�
*	@return �Ȃ�
*/
void SpeedBullet::Initialize()
{
	// �e�̍��W�̐ݒ�
	m_position = m_pEnemyBullet->GetPosition();
	// �e�̑��x�̐ݒ�
	m_velocity = m_pEnemyBullet->GetVelocity();
	// �e�̕����̐ݒ�
	m_direction = m_pEnemyBullet->GetDirection();
	// �e�̋��E���̐ݒ�
	m_boundingSphere = m_pEnemyBullet->GetBoundingSphere();
}
/*
*	@brief �X�V�֐�
*	@details �G�e�N���X(�����e)�̍X�V�֐�
*	@param float elapsedTime �X�V����
*	@return �Ȃ�
*/
void SpeedBullet::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;
	// ���e�ʒu�ɓ��B���Ă��Ȃ�
	if (m_position.y >= BulletParameters::VERTICAL_BULLET_LANDING_POSITION)
	{
		// �^���ɗ��Ƃ�
		m_velocity = BulletParameters::VERTICAL_BULLET_LANDING_VELOCITY * elapsedTime * BulletParameters::VERTICAL_BULLET_SPEED;
	}
	else//���e������
	{
		// �v���C���[�̕����x�N�g�����v�Z
		Vector3 toPlayer = m_pEnemyBullet->GetTarget() - m_pEnemyBullet->GetEnemyPosition();
		// �x�N�g���𐳋K��
		if (toPlayer.LengthSquared() > 0)toPlayer.Normalize();
		// �e�̕������v���C���[�̕����Ɍ�����
		m_direction = Vector3(toPlayer.x, 0, toPlayer.z) * elapsedTime * BulletParameters::VERTICAL_BULLET_SPEED;
		// �e�̑��x��ݒ�
		m_velocity = m_direction;
	}
	// �v���C���[�̕����Ɍ������Ēe���΂�
	m_position += m_velocity;
	// ���E���̒��S���W��e�̍��W�ɍX�V
	m_boundingSphere.Center = m_position;
	// �e�̕������X�V
	m_pEnemyBullet->SetDirection(m_direction);
	// �e�̑��x�X�V
	m_pEnemyBullet->SetVelocity(m_velocity);
	// �e�̍��W���X�V
	m_pEnemyBullet->SetPosition(m_position);
	// �e�̋��E�����X�V
	m_pEnemyBullet->SetBoundingSphere(m_boundingSphere);
}
