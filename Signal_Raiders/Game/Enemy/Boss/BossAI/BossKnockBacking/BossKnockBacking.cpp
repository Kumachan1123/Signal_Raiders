/*
	@file	BossKnockBacking.cpp
	@brief	�{�X�̃m�b�N�o�b�N���̏����N���X
*/
#include <pch.h>
#include "BossKnockBacking.h"

/*
*	@brief	�R���X�g���N�^
*	@details �{�X�̃m�b�N�o�b�N���̏����N���X�̃R���X�g���N�^
*	@param	pBoss	�{�XAI�N���X�̃|�C���^
*	@return	�Ȃ�
*/
BossKnockBacking::BossKnockBacking(BossAI* pBoss)
	: m_pBoss(pBoss)// �{�XAI�N���X�̃|�C���^
	, m_position(DirectX::SimpleMath::Vector3::Zero)// ���W�̏�����
	, m_rotation(DirectX::SimpleMath::Quaternion::Identity)// ��]
	, m_velocity(DirectX::SimpleMath::Vector3::Zero)// ���x
	, m_scale(DirectX::SimpleMath::Vector3::One)// �X�P�[��
	, m_initialPosition(DirectX::SimpleMath::Vector3::Zero)// ���W�����l
	, m_time(0.0f)// ���Ԃ̏�����
	, m_rotationSpeed(0.0f)// ��]���x
	, m_angle(0.0f)// �p�x
	, m_knockTime(0.0f)// �m�b�N�o�b�N���Ԃ̏�����
	, m_knockStartPosition(DirectX::SimpleMath::Vector3::Zero)// �m�b�N�o�b�N�J�n�ʒu�̏�����
	, m_knockEndPosition(DirectX::SimpleMath::Vector3::Zero)// �m�b�N�o�b�N�I���ʒu�̏�����
	, m_initialVelocity(DirectX::SimpleMath::Vector3::Zero)// �m�b�N�o�b�N�̏����x�̏�����
{
}
/*
*	@brief �f�X�g���N�^
*	@details �{�X�̃m�b�N�o�b�N���̏����N���X�̃f�X�g���N�^(�����ł͉������Ȃ�)
*	@return �Ȃ�
*/
BossKnockBacking::~BossKnockBacking() {/*do nothing*/ }
/*
*	@brief ������
*	@details �{�X�̃m�b�N�o�b�N���̏����N���X�̏�����
*	@param �Ȃ�
*	@return �Ȃ�
*/
void BossKnockBacking::Initialize()
{
	m_position = m_pBoss->GetPosition(); // �{�X�̈ʒu���擾
	m_rotation = m_pBoss->GetRotation();// �{�X�̉�]���擾
	m_velocity = m_pBoss->GetVelocity();// �{�X�̑��x���擾
	m_scale = m_pBoss->GetScale();// �{�X�̃X�P�[�����擾
	m_initialPosition = m_pBoss->GetPosition();// ���W�����l
	m_rotationSpeed = EnemyParameters::INITIAL_ROTATION_SPEED; // ��]���x
}

/*
*	@brief �X�V
*	@details �{�X�̃m�b�N�o�b�N���̏����N���X�̍X�V
*	@param elapsedTime �o�ߎ���
*	@return �Ȃ�
*/
void BossKnockBacking::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;
	m_time += elapsedTime;// ���Ԃ̍X�V
	if (m_knockTime == 0.0f)// �m�b�N�o�b�N�J�n���̏���
	{
		m_knockStartPosition = m_position; // �m�b�N�o�b�N�J�n�ʒu
		Vector3 knockBackDirection = (m_position - m_pBoss->GetEnemy()->GetPlayer()->GetPlayerPos()); // �v���C���[����G�ւ̕����x�N�g��
		knockBackDirection.Normalize(); // ���K�����ĕ����x�N�g���ɂ���
		m_knockEndPosition = m_position + knockBackDirection; // �m�b�N�o�b�N�I���ʒu
		m_initialVelocity = knockBackDirection * EnemyParameters::FIXED_INITIAL_SPEED; // �������x
		m_pBoss->GetEnemy()->SetCanAttack(false);// �U���s�\�ɂ���
	}
	m_knockTime += elapsedTime;	// �m�b�N�o�b�N���Ԃ̍X�V
	float Progression = std::min(m_knockTime / EnemyParameters::KNOCKBACK_DURATION,
		EnemyParameters::KNOCKBACK_PROGRESS_MAX);	// �m�b�N�o�b�N�̐i�s�x
	float decayFactor = std::exp(EnemyParameters::KNOCKBACK_DECAY_RATE * Progression); // �������x�𒲐����邽�߂Ɏw���̃x�[�X�𒲐�
	Vector3 velocity = m_initialVelocity * decayFactor;	// �����������x���g���Ĉʒu���X�V
	m_position += velocity * elapsedTime;	// �ʒu���X�V
	if (Progression >= EnemyParameters::KNOCKBACK_TIME.canAttackTime)m_pBoss->GetEnemy()->SetCanAttack(true);// �m�b�N�o�b�N�̐i�s�x�����ȏ�ɂȂ�����U���\�ɂ���
	if (Progression >= EnemyParameters::KNOCKBACK_TIME.endKnockTime)	// �m�b�N�o�b�N���I���������ǂ����`�F�b�N
	{
		m_knockEndPosition = m_position; // �m�b�N�o�b�N�I���ʒu���X�V
		m_knockTime = 0.0f; // �m�b�N�o�b�N���Ԃ̃��Z�b�g
		m_pBoss->GetEnemy()->SetEnemyHitByPlayerBullet(false); // �m�b�N�o�b�N�I��	
		m_pBoss->SetIsKnockBack(true);// ����ȍ~�m�b�N�o�b�N�������s��Ȃ�
		m_pBoss->SetAttackState(IState::EnemyState::ANGRY);// �{����
	}
	m_pBoss->SetPosition(m_position);// �ʒu���Z�b�g
	m_pBoss->SetVelocity(m_velocity);// ���x���Z�b�g
	m_pBoss->SetRotation(m_rotation);// ��]���Z�b�g
}