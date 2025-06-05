/*
*	@file	EnemySpin.cpp
*	@brief	�G�X�s���N���X
*/
#include <pch.h>
#include "EnemySpin.h"
/*
*	@brief	�R���X�g���N�^
*	@details �G�X�s���N���X�̃R���X�g���N�^
*	@param	EnemyAI* pEnemyAI �GAI
*	@return	�Ȃ�
*/
EnemySpin::EnemySpin(EnemyAI* pEnemyAI)
	: m_pEnemyAI(pEnemyAI)// �GAI�̃|�C���^�[
	, m_rotation(DirectX::SimpleMath::Quaternion::Identity) // ��]�̏�����
	, m_velocity(DirectX::SimpleMath::Vector3::Zero) // ���x�̏�����
	, m_scale(DirectX::SimpleMath::Vector3::One) // �X�P�[���̏�����
	, m_initialPosition(DirectX::SimpleMath::Vector3::Zero) // �����ʒu�̏�����
	, m_position(DirectX::SimpleMath::Vector3::Zero) // ���݈ʒu�̏�����
	, m_knockTime(0.0f) // �m�b�N�o�b�N���Ԃ̏�����
	, m_knockStartPosition(DirectX::SimpleMath::Vector3::Zero) // �m�b�N�o�b�N�J�n�ʒu�̏�����
	, m_knockEndPosition(DirectX::SimpleMath::Vector3::Zero) // �m�b�N�o�b�N�I���ʒu�̏�����
	, m_initialVelocity(DirectX::SimpleMath::Vector3::Zero) // �m�b�N�o�b�N�̏������x�̏�����
	, m_time(0.0f) // ���Ԃ̏�����
	, m_angle(0.0f) // �p�x�̏�����
{
}
/*
*	@brief	�f�X�g���N�^
*	@details �e��|�C���^�[��nullptr�ɐݒ�
*	@param	�Ȃ�
*	@return	�Ȃ�
*/
EnemySpin::~EnemySpin()
{
	// �GAI�̃|�C���^�[��nullptr�ɐݒ�
	m_pEnemyAI = nullptr;
}
/*
*	@brief	������
*	@details �G�X�s���N���X�̏�����
*	@param	�Ȃ�
*	@return	�Ȃ�
*/
void EnemySpin::Initialize()
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
}
/*
*	@brief	�X�V
*	@details �G�X�s���N���X�̍X�V
*	@param	float elapsedTime�@�o�ߎ���
*	@return	�Ȃ�
*/
void EnemySpin::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;
	// ���Ԃ̉��Z
	m_time += elapsedTime;
	// AI����ʒu���擾
	m_position = m_pEnemyAI->GetEnemy()->GetPosition();
	// �X�s���̍X�V
	UpdateSpin(elapsedTime);
	// �m�b�N�o�b�N�̍X�V
	UpdateKnockBack(elapsedTime);
	// �G�̈ʒu���X�V
	m_pEnemyAI->SetPosition(m_position);
	// �G�̉�]���X�V
	m_pEnemyAI->SetRotation(m_rotation);
	// �G�̑��x���X�V
	m_pEnemyAI->SetVelocity(m_velocity);
}
/*
*	@brief	�X�s���̍X�V
*	@details �G�X�s���N���X�̃X�s���̕����̏���
*	@param	float elapsedTime�@�o�ߎ���
*	@return	�Ȃ�
*/
void EnemySpin::UpdateSpin(float elapsedTime)
{
	using namespace DirectX::SimpleMath;
	// �����̖��g�p�x����}��
	UNREFERENCED_PARAMETER(elapsedTime);
	// �G����v���C���[�ւ̕����̊p�x���擾
	m_angle = CalculateAngle(m_pEnemyAI->GetPosition(), m_pEnemyAI->GetEnemy()->GetPlayer()->GetPlayerPos());
	// �v���C���[����G�ւ̊p�x���擾���A����ƌ��݂̊p�x����(�m�b�N�o�b�N���̉�]���o)
	m_angle = Lerp(m_angle, CalculateAngle(m_pEnemyAI->GetEnemy()->GetPlayer()->GetPlayerPos(), m_pEnemyAI->GetPosition()), m_time);
	// �v�Z�����p�x���g���āA�G��Y�������iYaw�j�̉�]��ݒ�
	m_rotation = Quaternion::CreateFromYawPitchRoll(m_angle, 0.0f, 0.0f);
}

/*
*	@brief	�m�b�N�o�b�N�̍X�V
*	@details �G�X�s���N���X�̃m�b�N�o�b�N�̕����̏���
*	@param	float elapsedTime�@�o�ߎ���
*	@return	�Ȃ�
*/
void EnemySpin::UpdateKnockBack(float elapsedTime)
{
	using namespace DirectX::SimpleMath;
	// �m�b�N�o�b�N���n�܂����΂���̂Ƃ��A�������������s��
	if (m_knockTime == 0.0f)
	{
		// ���݈ʒu���m�b�N�o�b�N�̊J�n�ʒu�Ƃ��ċL�^
		m_knockStartPosition = m_position;
		// �v���C���[����G�ւ̕������x�N�g���Ƃ��Ď擾
		Vector3 knockBackDirection = (m_position - m_pEnemyAI->GetEnemy()->GetPlayer()->GetPlayerPos());
		//�m�b�N�o�b�N��������𐳋K��
		knockBackDirection.Normalize();
		// �m�b�N�o�b�N�̉��̏I���ʒu��ݒ�
		m_knockEndPosition = m_position + knockBackDirection;
		// �����̑��x�x�N�g����ݒ�
		m_initialVelocity = knockBackDirection * EnemyParameters::FIXED_INITIAL_SPEED;
		// �m�b�N�o�b�N���͍U���ł��Ȃ��悤�ɂ���
		m_pEnemyAI->GetEnemy()->SetCanAttack(false);
	}
	// �m�b�N�o�b�N�o�ߎ��Ԃ����Z
	m_knockTime += elapsedTime;
	// �m�b�N�o�b�N�̐i�s�x���v�Z�i0.0�`1.0�͈̔́A�����KNOCKBACK_PROGRESS_MAX�j
	float Progression = std::min(m_knockTime / EnemyParameters::KNOCKBACK_DURATION, EnemyParameters::KNOCKBACK_PROGRESS_MAX);
	// �����W�����w���֐��Ōv�Z�i���Ԃ��o���Ƃɑ��x���������Ȃ�j
	float decayFactor = std::exp(EnemyParameters::KNOCKBACK_DECAY_RATE * Progression);
	// ������̑��x�ňʒu���X�V
	Vector3 velocity = m_initialVelocity * decayFactor;
	// ���ۂɈʒu��ω�������
	m_position += velocity * elapsedTime;
	// ��Ԃ��u�{��v�łȂ��ꍇ�́u�q�b�g��ԁv�ɑJ�ڂ�����
	if (m_pEnemyAI->GetState() != IState::EnemyState::ANGRY)m_pEnemyAI->SetState(IState::EnemyState::HIT);
	// ��莞�Ԍo�ߌ�A�U�����Ăы�����
	if (Progression >= EnemyParameters::KNOCKBACK_TIME.canAttackTime)m_pEnemyAI->GetEnemy()->SetCanAttack(true);
	// �m�b�N�o�b�N�����S�ɏI���������𔻒�
	if (Progression >= EnemyParameters::KNOCKBACK_TIME.endKnockTime)
	{
		// �ŏI�I�Ȉʒu���m�b�N�o�b�N�I���n�_�Ƃ��ċL�^
		m_knockEndPosition = m_position;
		// �m�b�N�o�b�N�o�ߎ��Ԃ����Z�b�g
		m_knockTime = 0.0f;
		// �m�b�N�o�b�N��Ԃ�����
		m_pEnemyAI->GetEnemy()->SetEnemyHitByPlayerBullet(false);
		// ��Ԃ�ҋ@��Ԃɖ߂�
		m_pEnemyAI->SetState(IState::EnemyState::IDLING);
		// �U�����łȂ���Ԃɂ���
		m_pEnemyAI->SetIsAttack(false);
		// �p�j��Ԃɂ���
		m_pEnemyAI->ChangeState(m_pEnemyAI->GetEnemyIdling());
	}
}
