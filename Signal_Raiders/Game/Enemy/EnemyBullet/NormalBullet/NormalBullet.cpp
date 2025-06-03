/*
*	@file NormalBullet.cpp
*	@brief �G�e�N���X(�ʏ�e)
*/
#include <pch.h>
#include "NormalBullet.h"
/*
*	@brief �R���X�g���N�^
*	@details �G�e�N���X(�ʏ�e)�̃R���X�g���N�^
*	@return �Ȃ�
*/
NormalBullet::NormalBullet()
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
NormalBullet::~NormalBullet()
{
	// ���ʃ��\�[�X�̃|�C���^�[��nullptr�ɐݒ�
	m_pCommonResources = nullptr;
	// �G�e�̃|�C���^�[��nullptr�ɐݒ�
	m_pEnemyBullet = nullptr;
}
/*
*	@brief �������֐�
*	@details �G�e�N���X(�ʏ�e)�̏������֐�
*	@param �Ȃ�
*	@return �Ȃ�
*/
void NormalBullet::Initialize()
{
	// �e�̍��W��ݒ�
	m_position = m_pEnemyBullet->GetPosition();
	// �e�̑��x��ݒ�
	m_velocity = m_pEnemyBullet->GetVelocity();
	// �e�̕�����ݒ�
	m_direction = m_pEnemyBullet->GetDirection();
	// ���E����ݒ�
	m_boundingSphere = m_pEnemyBullet->GetBoundingSphere();
	// �v���C���[�ւ̕����x�N�g�����v�Z
	m_toPlayer = m_pEnemyBullet->GetTarget() - m_position;
	// �x�N�g���𐳋K��
	m_toPlayer.Normalize();
}
/*
*	@brief �X�V�֐�
*	@details �G�e�N���X(�ʏ�e)�̍X�V�֐�
*	@param float elapsedTime �X�V����
*	@return �Ȃ�
*/
void NormalBullet::Update(float elapsedTime)
{
	// �v���C���[�̕�������������
	m_direction = m_toPlayer;
	// ���Ɏ኱���炷
	m_direction.y -= BulletParameters::STRAIGHT_ADJUST_DIRECTION;
	// �e�̑��x��x������
	m_velocity = m_direction * BulletParameters::STRAIGHT_BULLET_SPEED * elapsedTime;
	// �v���C���[�̕����Ɍ������Ēe���΂�
	m_position += m_velocity;
	//���E���ɍ��W��n��
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
