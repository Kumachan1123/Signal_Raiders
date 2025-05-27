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
	m_pEnemyAI = nullptr; // �GAI�̃|�C���^�[��nullptr�ɐݒ�
}
/*
*	@brief	������
*	@details �G�U���N���X�̏�����
*	@param	�Ȃ�
*	@return	�Ȃ�
*/
void EnemyAttack::Initialize()
{
	m_rotation = m_pEnemyAI->GetRotation();// ��]
	m_velocity = m_pEnemyAI->GetVelocity();// �ړ����x
	m_scale = m_pEnemyAI->GetScale();// �T�C�Y
	m_rotationSpeed = EnemyParameters::INITIAL_ROTATION_SPEED; // ��]���x
}
/*
*	@brief	�X�V
*	@details �G�U���N���X�̍X�V
*	@param	float elapsedTime
*	@return	�Ȃ�
*/
void EnemyAttack::Update(float elapsedTime)
{
	m_rotationSpeed -= (elapsedTime / EnemyParameters::ROTATION_SPEED_DIVISOR);// ��]���x������
	if (m_rotationSpeed <= EnemyParameters::ROTATION_SPEED_MIN)// ��]���x����ȉ��ɂȂ�����
		m_rotationSpeed = EnemyParameters::ROTATION_SPEED_MIN;// ��]���x����l�ɐݒ�
	Vector3 toPlayerVector = m_pEnemyAI->GetEnemy()->GetPlayer()->GetPlayerPos() - m_pEnemyAI->GetPosition();	// �v���C���[�ւ̃x�N�g�����v�Z
	if (toPlayerVector.LengthSquared() > 0.0f)// �v���C���[�ւ̃x�N�g����0���傫���ꍇ
		toPlayerVector.Normalize();// �v���C���[�ւ̃x�N�g���𐳋K��
	// ���݂̑O���x�N�g�����擾
	Vector3 forward = Vector3::Transform(Vector3::Forward, m_rotation);
	if (forward.LengthSquared() > 0.0f)// ���݂̑O���x�N�g����0���傫���ꍇ
		forward.Normalize();// ���݂̑O���x�N�g���𐳋K��
	float dot = toPlayerVector.Dot(forward);	// ���ς��g���Ċp�x���v�Z
	dot = Clamp(dot, EnemyParameters::DOT_CLAMP.min, EnemyParameters::DOT_CLAMP.max); // acos�̈������͈͊O�ɂȂ�̂�h������
	float angle = std::acos(dot);// �v���C���[�̕����Ɍ������߂̉�]���v�Z
	Vector3 cross = toPlayerVector.Cross(forward);// �O�ς��g���ĉ�]�������v�Z
	if (cross.y < 0)angle = -angle;// y��0��菬�����ꍇ�A�p�x�𔽓]
	Quaternion toPlayerRotation = Quaternion::CreateFromAxisAngle(Vector3::Up, angle);// �v���C���[�̕����Ɍ������߂̉�]���v�Z
	if (std::abs(angle) > EnemyParameters::ANGLE_THRESHOLD)	// �ڕW�p�x�ƌ��݂̊p�x�����ɏ������ꍇ�A�U����h�����߂ɕ�Ԃ��~�߂�
	{
		m_rotation = Quaternion::Slerp(m_rotation, toPlayerRotation * m_rotation, elapsedTime * (m_rotationSpeed));// ��]��ݒ�
		m_rotation.Normalize();// ��]�𐳋K��
	}
	m_pEnemyAI->SetPosition(m_pEnemyAI->GetPosition() + toPlayerVector * (m_velocity.Length() * EnemyParameters::VELOCITY_SCALE_FACTOR) * elapsedTime);// �v���C���[�̕����Ɉړ�
	m_attackCooldown -= elapsedTime;// �U���̃N�[���_�E���^�C��������
	if (m_attackCooldown <= EnemyParameters::CHANGE_FACE_TIME)// �U���̃N�[���_�E���^�C����2.0�ȉ��ɂȂ�����
	{
		m_pEnemyAI->SetState(IState::EnemyState::ANGRY);// �{��Ԑ��ɂ���
		if (m_attackCooldown <= 0.0f)m_attackCooldown = EnemyParameters::ATTACK_COOLDOWN;// ���̍U���܂ł̃N�[���_�E���^�C����3�b�ɐݒ�
	}
	m_pEnemyAI->SetRotation(m_rotation);// ��]��ݒ�
	m_pEnemyAI->SetVelocity(m_velocity);// �ړ����x��ݒ�
}
