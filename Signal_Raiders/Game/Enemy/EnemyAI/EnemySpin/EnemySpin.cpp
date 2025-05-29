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
	: m_pEnemyAI(pEnemyAI)
	, m_rotation(DirectX::SimpleMath::Quaternion::Identity)
	, m_velocity(DirectX::SimpleMath::Vector3::Zero)
	, m_scale(DirectX::SimpleMath::Vector3::One)
	, m_initialPosition(DirectX::SimpleMath::Vector3::Zero)
	, m_position(DirectX::SimpleMath::Vector3::Zero)
	, m_knockTime(0.0f)
	, m_knockStartPosition(DirectX::SimpleMath::Vector3::Zero)
	, m_knockEndPosition(DirectX::SimpleMath::Vector3::Zero)
	, m_initialVelocity(DirectX::SimpleMath::Vector3::Zero)
	, m_time(0.0f)
	, m_angle(0.0f)
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
	m_pEnemyAI = nullptr; // �GAI�̃|�C���^�[��nullptr�ɐݒ�
}
/*
*	@brief	������
*	@details �G�X�s���N���X�̏�����
*	@param	�Ȃ�
*	@return	�Ȃ�
*/
void EnemySpin::Initialize()
{
	m_position = m_pEnemyAI->GetPosition(); // �G�̈ʒu���擾
	m_rotation = m_pEnemyAI->GetRotation();// ��]�擾
	m_velocity = m_pEnemyAI->GetVelocity();// ���x�擾
	m_scale = m_pEnemyAI->GetScale();// �X�P�[���擾
	m_initialPosition = m_pEnemyAI->GetPosition(); // �����ʒu�擾
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
	m_time += elapsedTime;// ���Ԃ̉��Z
	UpdateSpin(elapsedTime); // �X�s���̍X�V
	UpdateKnockBack(elapsedTime); // �m�b�N�o�b�N�̍X�V
	m_pEnemyAI->SetPosition(m_position); // �G�̈ʒu���X�V
	m_pEnemyAI->SetRotation(m_rotation); // �G�̉�]���X�V
	m_pEnemyAI->SetVelocity(m_velocity); // �G�̑��x���X�V
}
/*
*	@brief	�X�s���̍X�V
*	@details �G�X�s���N���X�̃X�s���̕����̏���
*	@param	float elapsedTime�@�o�ߎ���
*	@return	�Ȃ�
*/
void EnemySpin::UpdateSpin(float elapsedTime)
{
	UNREFERENCED_PARAMETER(elapsedTime); // �����̖��g�p�x����}��
	using namespace DirectX::SimpleMath;
	m_angle = CalculateAngle(m_pEnemyAI->GetPosition(), m_pEnemyAI->GetEnemy()->GetPlayer()->GetPlayerPos());// �v���C���[�̕������擾���A���ʂ���������
	m_angle = Lerp(m_angle, CalculateAngle(m_pEnemyAI->GetEnemy()->GetPlayer()->GetPlayerPos(), m_pEnemyAI->GetPosition()), m_time);// �v���C���[�̕������擾���A���]������
	m_rotation = Quaternion::CreateFromYawPitchRoll(m_angle, 0.0f, 0.0f);// �G����]������
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
	// �m�b�N�o�b�N���n�܂����΂���Ȃ珉���ݒ���s��
	if (m_knockTime == 0.0f)
	{
		m_knockStartPosition = m_position; // �m�b�N�o�b�N�J�n�ʒu
		Vector3 knockBackDirection = (m_position - m_pEnemyAI->GetEnemy()->GetPlayer()->GetPlayerPos()); // �v���C���[����G�ւ̕����x�N�g��
		knockBackDirection.Normalize(); // ���K�����ĕ����x�N�g���ɂ���
		m_knockEndPosition = m_position + knockBackDirection; // �m�b�N�o�b�N�I���ʒu
		m_initialVelocity = knockBackDirection * EnemyParameters::FIXED_INITIAL_SPEED; // �������x
		m_pEnemyAI->GetEnemy()->SetCanAttack(false);// �U���s�\�ɂ���
	}
	// �m�b�N�o�b�N���Ԃ̍X�V
	m_knockTime += elapsedTime;
	// �m�b�N�o�b�N�̐i�s�x
	float Progression = std::min(m_knockTime / EnemyParameters::KNOCKBACK_DURATION, EnemyParameters::KNOCKBACK_PROGRESS_MAX);
	// �����W���̌v�Z�i�w���֐��I�����j
	float decayFactor = std::exp(EnemyParameters::KNOCKBACK_DECAY_RATE * Progression); // �������x�𒲐����邽�߂Ɏw���̃x�[�X�𒲐�
	// �����������x���g���Ĉʒu���X�V
	Vector3 velocity = m_initialVelocity * decayFactor;
	m_position += velocity * elapsedTime; // �m�b�N�o�b�N�̈ʒu���X�V
	if (m_pEnemyAI->GetState() != IState::EnemyState::ANGRY)// �{��Ԑ��łȂ��ꍇ
		m_pEnemyAI->SetState(IState::EnemyState::HIT);// �U����H�������Ԃɂ���
	// �m�b�N�o�b�N���I���������ǂ����`�F�b�N
	if (Progression >= EnemyParameters::KNOCKBACK_TIME.canAttackTime)
		m_pEnemyAI->GetEnemy()->SetCanAttack(true);// �U���\�ɂ���
	// �m�b�N�o�b�N���I���������ǂ����`�F�b�N
	if (Progression >= EnemyParameters::KNOCKBACK_TIME.endKnockTime)
	{
		m_knockEndPosition = m_position; // �m�b�N�o�b�N�I���ʒu�����̏ꏊ�ɂ���
		m_knockTime = 0.0f; // �m�b�N�o�b�N���Ԃ̃��Z�b�g
		m_pEnemyAI->GetEnemy()->SetEnemyHitByPlayerBullet(false); // �m�b�N�o�b�N�I��
		m_pEnemyAI->SetState(IState::EnemyState::IDLING);// �ҋ@�Ԑ�
	}
}