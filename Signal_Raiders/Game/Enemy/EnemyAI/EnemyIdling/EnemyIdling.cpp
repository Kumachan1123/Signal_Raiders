/*
*	@file	EnemyIdling.cpp
*	@brief	�G�p�j�N���X
*/
#include <pch.h>
#include "EnemyIdling.h"
/*
*	@brief	�R���X�g���N�^
*	@details �G�p�j�N���X�̃R���X�g���N�^
*	@param	EnemyAI* pEnemyAI
*	@return	�Ȃ�
*/
EnemyIdling::EnemyIdling(EnemyAI* pEnemyAI)
	: m_pEnemyAI(pEnemyAI)// �GAI
	, m_rotation(DirectX::SimpleMath::Quaternion::Identity)// ��]
	, m_velocity(DirectX::SimpleMath::Vector3::Zero)// ���x
	, m_scale(DirectX::SimpleMath::Vector3::One)// �X�P�[��
	, m_initialPosition(DirectX::SimpleMath::Vector3::Zero)// �����ʒu
	, m_time(0.0f)// ����
	, m_rotationSpeed(0.0f)// ��]���x
{}
/*
*	@brief	�f�X�g���N�^
*	@details �e��|�C���^�[��nullptr�ɐݒ�
*	@param	�Ȃ�
*	@return	�Ȃ�
*/
EnemyIdling::~EnemyIdling()
{
	// �GAI�̃|�C���^�[��nullptr�ɐݒ�
	m_pEnemyAI = nullptr;
}
/*
*	@brief	������
*	@details �G�p�j�N���X�̏�����
*	@param	�Ȃ�
*	@return	�Ȃ�
*/
void EnemyIdling::Initialize()
{
	// AI����ʒu���擾���Đݒ肷��
	m_position = m_pEnemyAI->GetPosition();
	// AI�����]���擾���Đݒ肷��
	m_rotation = m_pEnemyAI->GetRotation();
	// AI����ړ����x���擾���Đݒ肷��
	m_velocity = m_pEnemyAI->GetVelocity();
	// AI����X�P�[�����擾���Đݒ肷��
	m_scale = m_pEnemyAI->GetScale();
	// AI���珉���ʒu���擾���Đݒ肷��
	m_initialPosition = m_pEnemyAI->GetPosition();
	// ��]���x��������
	m_rotationSpeed = EnemyParameters::INITIAL_ROTATION_SPEED;
}
/*
*	@brief	�X�V
*	@details �G�p�j�N���X�̍X�V
*	@param	float elapsedTime
*	@return	�Ȃ�
*/
void EnemyIdling::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;
	// �G���v���C���[�̈��͈͓��ɓ����Ă���ꍇ
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
	// �����_���ȉ�]�{���𐶐�����
	float randomMultiplier = GenerateRandomMultiplier(EnemyParameters::RANDOM_ROTATION_MIN, EnemyParameters::RANDOM_ROTATION_MAX);
	// sin�g���g���ĉ�]���x��ω�������
	float sinRotationSpeed = m_rotationSpeed + EnemyParameters::AMPLITUDE * std::sin(EnemyParameters::FREQUENCY * m_time);
	// ������iY���j�����ɂ�����]���쐬����
	Quaternion deltaRotation = Quaternion::CreateFromAxisAngle(Vector3::Up, sinRotationSpeed * randomMultiplier * elapsedTime);
	// ���݂̉�]�ɐV������]��K�p����
	m_rotation *= deltaRotation;
	// ��]�𐳋K�����āA���l�̈��萫��ۂ�
	m_rotation.Normalize();
	// ��]��̑O���x�N�g�����擾����i�������~���x�j
	Vector3 forward = Vector3::Transform(Vector3::Backward * EnemyParameters::MOVE_SPEED, m_rotation);
	// �V�����ʒu��ݒ肷��
	Vector3 newPosition = Vector3(m_pEnemyAI->GetPosition().x, m_position.y, m_pEnemyAI->GetPosition().z);
	// �G�̈ʒu���X�V����
	m_pEnemyAI->SetPosition(newPosition + forward * (m_velocity.Length()) * elapsedTime);
	// �G�̉�]���X�V����
	m_pEnemyAI->SetRotation(m_rotation);
	// �G�̑��x���X�V����
	m_pEnemyAI->SetVelocity(m_velocity);
}

