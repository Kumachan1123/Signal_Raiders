/*
*	@file NormalBullet.cpp
*	@brief �G�e�N���X(�ʏ�e)
*/
#include <pch.h>
#include "NormalBullet.h"
/*
*	@brief �R���X�g���N�^
*	@return �Ȃ�
*/
NormalBullet::NormalBullet()
	: m_pEnemyBullet(nullptr)// �G�e�|�C���^�[
{
}
/*
*	@brief �f�X�g���N�^
*	@return �Ȃ�
*/
NormalBullet::~NormalBullet() {/*do nothing*/ }
/*
*	@brief �������֐�
*	@return �Ȃ�
*/
void NormalBullet::Initialize()
{
	m_position = m_pEnemyBullet->GetPosition();// �e�̍��W
	m_velocity = m_pEnemyBullet->GetVelocity();// �e�̑��x
	m_direction = m_pEnemyBullet->GetDirection();// �e�̕���
	m_boundingSphere = m_pEnemyBullet->GetBoundingSphere();// �u�e�v���E��
	m_toPlayer = m_pEnemyBullet->GetTarget() - m_position;// // �v���C���[�̕����x�N�g�����v�Z
	m_toPlayer.Normalize();// �x�N�g���𐳋K��
}
/*
*	@brief �X�V�֐�
*	@param[in] float elapsedTime �X�V����
*	@return �Ȃ�
*/
void NormalBullet::Update(float elapsedTime)
{
	m_direction = m_toPlayer;// �v���C���[�̕�������������
	m_direction.y -= BulletParameters::STRAIGHT_ADJUST_DIRECTION;// ���Ɏ኱���炷
	m_velocity = m_direction * BulletParameters::STRAIGHT_BULLET_SPEED * elapsedTime;	// �e�̑��x��x������
	m_position += m_velocity;// �v���C���[�̕����Ɍ������Ēe���΂�
	m_boundingSphere.Center = m_position;//���E���ɍ��W��n��
	// �e�̊e������X�V
	m_pEnemyBullet->SetDirection(m_direction);// �e�̕���
	m_pEnemyBullet->SetVelocity(m_velocity);// �e�̑��x
	m_pEnemyBullet->SetPosition(m_position);// �e�̍��W
	m_pEnemyBullet->SetBoundingSphere(m_boundingSphere);// �u�e�v���E��
}
