/*
	@file	EnemyAI.cpp
	@brief	�GAI�N���X
*/
#include "pch.h"
#include "EnemyAI.h"

/*
*	@brief	�R���X�g���N�^
*	@param[in]	IEnemy* pEnemy�@�G
*	@return	�Ȃ�
*/
EnemyAI::EnemyAI(IEnemy* pEnemy)
	: m_currentState(nullptr)// ���݂̏��
	, m_attackCooldown(0.0f)// �U���N�[���_�E��
	, m_time(0.0f)// ����
	, m_isHitPlayerBullet(false)// �v���C���[�̒e�ɓ���������
	, m_knockTime(0.0f)// �m�b�N�o�b�N����
	, m_pEnemyAttack(nullptr)// �U���N���X
	, m_pEnemySpin(nullptr)// �X�s���N���X
	, m_pEnemyIdling(nullptr)// �p�j�N���X
	, m_canAttack(true)// �U���\��
	, m_enemyState(IState::EnemyState::IDLING)// �G�̏��
	, m_pEnemy(pEnemy)// �G
	, m_isAttack(false)// �U������
{
	m_pEnemy = pEnemy;// �G���擾
	m_pEnemyAttack = std::make_unique<EnemyAttack>(this);// �U���N���X�𐶐�
	m_pEnemyIdling = std::make_unique<EnemyIdling>(this);// �p�j�N���X�𐶐�
	m_pEnemySpin = std::make_unique<EnemySpin>(this); // �X�s���N���X�𐶐�
}

/*
*	@brief	�f�X�g���N�^
*	@return	�Ȃ�
*/
EnemyAI::~EnemyAI() {/*do nothing*/ }

/*
*	@brief	������
*	@return	�Ȃ�
*/
void EnemyAI::Initialize()
{
	using namespace DirectX::SimpleMath;
	m_initialPosition = Vector3::Zero;  // �����ʒu��������
	m_initialPosition.y = GenerateRandomMultiplier(EnemyParameters::RANDOM_MIN, EnemyParameters::RANDOM_MAX); // �����ʒu��Y���W��ݒ�
	m_rotation.y = GenerateRandomMultiplier(EnemyParameters::RANDOM_MIN, EnemyParameters::RANDOM_MAX); // Y���̉�]��ݒ�
	m_velocity = EnemyParameters::INITIAL_VELOCITY; // ���V�̏������x
	m_scale = Vector3::One; // �X�P�[��������
	m_position = m_initialPosition;// �ʒu������
	m_currentState = m_pEnemyIdling.get(); // �p�j�Ԑ��ɂ���
	m_currentState->Initialize();// ��Ԃ�������
	m_enemyState = IState::EnemyState::IDLING;// �ҋ@�Ԑ�
}
/*
*	@brief	�X�V
*	@param[in]	float elapsedTime�@�o�ߎ���
*	@return	�Ȃ�
*/
void EnemyAI::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;
	m_position = m_pEnemy->GetPosition();// �ʒu���擾
	if (m_pEnemy->GetEnemyHitByPlayerBullet())m_isHitPlayerBullet = true;// �v���C���[�̒e�ɓ����������ǂ������擾
	m_time += elapsedTime; // ���Ԃ̉��Z
	m_position.y = m_initialPosition.y + EnemyParameters::AMPLITUDE * std::sin(EnemyParameters::FREQUENCY * m_time);	// �G���ӂ�ӂ핂�V������
	m_position.y += m_velocity.y * elapsedTime; // �G��Y���W���X�V
	// �G���v���C���[�̈��͈͓��ɓ����Ă���ꍇ
	if ((m_pEnemy->GetHitToPlayer() || m_isHitPlayerBullet))
	{
		ChangeState(m_pEnemyAttack.get());//�U���Ԑ��ɂ���
		m_enemyState = IState::EnemyState::ATTACK;// �p�j�Ԑ�
		SetIsAttack(true);// �U�����ɂ���
	}
	else
	{
		ChangeState(m_pEnemyIdling.get());//�p�j�Ԑ��ɂ���
		m_enemyState = IState::EnemyState::IDLING;// �p�j�Ԑ�
		SetIsAttack(false);// �U�����łȂ�
	}
	// �v���C���[�̒e�ɓ��������ꍇ
	if (m_pEnemy->GetEnemyHitByPlayerBullet())
	{
		KnockBack(elapsedTime);// �m�b�N�o�b�N����
		ChangeState(m_pEnemySpin.get());//�X�s������
		m_enemyState = IState::EnemyState::HIT;// �U����H�������Ԃɂ���
		SetIsAttack(false);// �U�����łȂ�
	}
	m_currentState->Update(elapsedTime); // ���݂̏�Ԃ��X�V
	m_pEnemy->SetPosition(m_position); // �G�̈ʒu���X�V
}
/*
*	@brief	��ԕύX
*	@param[in]	IState* newState�@�V�������
*	@return	�Ȃ�
*/
void EnemyAI::ChangeState(IState* newState)
{
	// �V������Ԃ����݂̏�ԂƈقȂ�ꍇ
	if (m_currentState != newState)
	{
		m_currentState = newState;// �V������ԂɕύX
		m_currentState->Initialize(); // �V������Ԃ�������
	}
}


/*
*	@brief	�m�b�N�o�b�N����
*	@param[in]	float elapsedTime�@�o�ߎ���
*	@return	�Ȃ�
*/
void EnemyAI::KnockBack(float elapsedTime)
{
	using namespace DirectX::SimpleMath;
	// �m�b�N�o�b�N���n�܂����΂���Ȃ珉���ݒ���s��
	if (m_knockTime == 0.0f)
	{
		m_knockStartPosition = m_position; // �m�b�N�o�b�N�J�n�ʒu
		Vector3 knockBackDirection = (m_position - m_pEnemy->GetPlayer()->GetPlayerPos()); // �v���C���[����G�ւ̕����x�N�g��
		knockBackDirection.Normalize(); // ���K�����ĕ����x�N�g���ɂ���
		m_knockEndPosition = m_position + knockBackDirection; // �m�b�N�o�b�N�I���ʒu
		m_initialVelocity = knockBackDirection * EnemyParameters::FIXED_INITIAL_SPEED; // �������x
		m_pEnemy->SetCanAttack(false);// �U���s�\�ɂ���
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
	if (GetState() != IState::EnemyState::ANGRY)// �{��Ԑ��łȂ��ꍇ
		SetState(IState::EnemyState::HIT);// �U����H�������Ԃɂ���
	// �m�b�N�o�b�N���I���������ǂ����`�F�b�N
	if (Progression >= EnemyParameters::KNOCKBACK_TIME.canAttackTime)
		m_pEnemy->SetCanAttack(true);// �U���\�ɂ���
	// �m�b�N�o�b�N���I���������ǂ����`�F�b�N
	if (Progression >= EnemyParameters::KNOCKBACK_TIME.endKnockTime)
	{
		m_knockEndPosition = m_position; // �m�b�N�o�b�N�I���ʒu�����̏ꏊ�ɂ���
		m_knockTime = 0.0f; // �m�b�N�o�b�N���Ԃ̃��Z�b�g
		m_pEnemy->SetEnemyHitByPlayerBullet(false); // �m�b�N�o�b�N�I��
		SetState(IState::EnemyState::IDLING);// �ҋ@�Ԑ�
	}
}
