/*
*	@file	EnemyAttack.cpp
*	@brief	�G�U���N���X
*/
#include <pch.h>
#include "EnemyAttack.h"
using namespace DirectX::SimpleMath;
/*
*	@brief	�R���X�g���N�^
*	@details �G�U���N���X�̃R���X�g���N�^
*	@param	EnemyAI* pEnemyAI
*	@return	�Ȃ�
*/
EnemyAttack::EnemyAttack(EnemyAI* pEnemyAI)
	: m_pEnemyAI(pEnemyAI)// �GAI
	, m_attackCooldown(EnemyParameters::ATTACK_COOLDOWN)// �U���̃N�[���_�E���^�C��
	, m_rotationSpeed{}// ��]���x
	, m_pCommonResources{}// ���ʃ��\�[�X
	, m_time(0.0f)// ����
{
}
/*
*	@brief	�f�X�g���N�^
*	@details �e��|�C���^�[��nullptr�ɐݒ�
*	@param	�Ȃ�
*	@return	�Ȃ�
*/
EnemyAttack::~EnemyAttack()
{
	// �GAI�̃|�C���^�[��nullptr�ɐݒ�
	m_pEnemyAI = nullptr;
}
/*
*	@brief	������
*	@details �G�U���N���X�̏�����
*	@param	�Ȃ�
*	@return	�Ȃ�
*/
void EnemyAttack::Initialize()
{
	// AI�����]�����擾���Đݒ肷��
	m_rotation = m_pEnemyAI->GetRotation();
	// AI����ړ����x���擾���Đݒ肷��
	m_velocity = m_pEnemyAI->GetVelocity();
	// AI����X�P�[���i�T�C�Y�j���擾���Đݒ肷��
	m_scale = m_pEnemyAI->GetScale();
	// AI����N�[���_�E���^�C�����擾���Đݒ肷��
	m_attackCooldown = m_pEnemyAI->GetAttackCooldown();
	// �����̉�]���x��ݒ肷��
	m_rotationSpeed = EnemyParameters::INITIAL_ROTATION_SPEED;
}
/*
*	@brief	�X�V
*	@details �G�U���N���X�̍X�V
*	@param	float elapsedTime
*	@return	�Ȃ�
*/
void EnemyAttack::Update(float elapsedTime)
{
	if ((m_pEnemyAI->GetEnemy()->GetHitToPlayer() || m_pEnemyAI->GetHitPlayerBullet()))
	{
		//�U���Ԑ��ɂ���
		m_pEnemyAI->ChangeState(m_pEnemyAI->GetEnemyAttack());
		// �U���Ԑ��ɂ���
		m_pEnemyAI->SetState(IState::EnemyState::ATTACK);
		// �U�����ɂ���
		m_pEnemyAI->SetIsAttack(true);
	}
	// �v���C���[�̒e�ɓ��������ꍇ
	if (m_pEnemyAI->GetEnemy()->GetEnemyHitByPlayerBullet())
	{
		//�X�s������
		m_pEnemyAI->ChangeState(m_pEnemyAI->GetEnemySpin());
		// �U����H�������Ԃɂ���
		m_pEnemyAI->SetState(IState::EnemyState::HIT);
		// �U�����łȂ�
		m_pEnemyAI->SetIsAttack(false);
	}
	// AI����ʒu���擾
	m_position = m_pEnemyAI->GetEnemy()->GetPosition();
	// ��]���x������������
	m_rotationSpeed -= (elapsedTime / EnemyParameters::ROTATION_SPEED_DIVISOR);
	// ��]���x���Œ�l�����������A�Œ�l�ɌŒ肷��
	if (m_rotationSpeed <= EnemyParameters::ROTATION_SPEED_MIN)	m_rotationSpeed = EnemyParameters::ROTATION_SPEED_MIN;
	// �v���C���[�̕����ւ̃x�N�g�����v�Z����
	Vector3 toPlayerVector = m_pEnemyAI->GetEnemy()->GetPlayer()->GetPlayerPos() - m_pEnemyAI->GetPosition();
	// �[���x�N�g���łȂ��Ȃ琳�K������
	if (toPlayerVector.LengthSquared() > 0.0f)	toPlayerVector.Normalize();
	// ���݂̑O���x�N�g�����擾����
	Vector3 forward = Vector3::Transform(Vector3::Forward, m_rotation);
	// �[���x�N�g���łȂ��Ȃ琳�K������
	if (forward.LengthSquared() > 0.0f)	forward.Normalize();
	// �v���C���[�̕����Ƃ̊p�x����ςŋ��߂�
	float dot = toPlayerVector.Dot(forward);
	// ���ς�Clamp���Ĉ��S��acos�ɒʂ�
	dot = Clamp(dot, EnemyParameters::DOT_CLAMP.min, EnemyParameters::DOT_CLAMP.max);
	// ��]�p�����߂�
	float angle = std::acos(dot);
	// ��]�������O�ςŔ��肷��
	Vector3 cross = toPlayerVector.Cross(forward);
	// y���������Ȃ��]�����𔽓]����
	if (cross.y < 0)angle = -angle;
	// ��]�N�H�[�^�j�I���𐶐�����
	Quaternion toPlayerRotation = Quaternion::CreateFromAxisAngle(Vector3::Up, angle);
	// �p�x�����ȏ�̂Ƃ�
	if (std::abs(angle) > EnemyParameters::ANGLE_THRESHOLD)
	{
		// �v���C���[�̕����Ɍ������ĉ�]���X���[�Y�ɓK�p����
		m_rotation = Quaternion::Slerp(m_rotation, toPlayerRotation * m_rotation, elapsedTime * m_rotationSpeed);
		// ��]�𐳋K������
		m_rotation.Normalize();
	}
	// �v���C���[�̕����Ɍ������Ĉړ�����
	m_pEnemyAI->SetPosition(m_pEnemyAI->GetPosition() + toPlayerVector * (m_velocity.Length() * EnemyParameters::VELOCITY_SCALE_FACTOR) * elapsedTime);
	// �N�[���_�E�������炷
	m_attackCooldown -= elapsedTime;
	// AI�ɃN�[���_�E���^�C����ݒ肷��
	m_pEnemyAI->SetAttackCooldown(m_attackCooldown);
	// �N�[���_�E����臒l�����������
	if (m_attackCooldown <= EnemyParameters::CHANGE_FACE_TIME)
	{
		// �G�̏�Ԃ�{���ԂɕύX����
		m_pEnemyAI->SetState(IState::EnemyState::ANGRY);
		// 0�ȉ��ɂȂ�����
		if (m_attackCooldown <= 0.0f)
		{
			// �U���Ԑ��ɂ���
			m_pEnemyAI->SetState(IState::EnemyState::ATTACK);
			// �N�[���_�E�������Z�b�g����
			m_attackCooldown = EnemyParameters::ATTACK_COOLDOWN;
			m_pEnemyAI->SetAttackCooldown(m_attackCooldown);
		}
	}
	// ��]�Ƒ��x��AI�ɔ��f������
	m_pEnemyAI->SetRotation(m_rotation);
	m_pEnemyAI->SetVelocity(m_velocity);

}
