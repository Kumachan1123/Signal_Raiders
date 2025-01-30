/*
	@file	EnemyAI.cpp
	@brief	�GAI�N���X
*/
#include "pch.h"
#include <SimpleMath.h>
#include "Game/Enemy/EnemyAI/EnemyAI.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/InputManager.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include <cassert>
#include <random>
#include <type_traits> // std::enable_if, std::is_integral
#include "Game/KumachiLib/KumachiLib.h"
#include "Game/Enemy/Parameters/EnemyParameters.h"
// �R���X�g���N�^
EnemyAI::EnemyAI(IEnemy* pEnemy)
	: m_currentState(nullptr), m_attackCooldown(0.0f), m_pEnemyAttack(nullptr), m_time(0.0f), m_isHitPlayerBullet(false), m_knockTime(0.0f)
	, m_pEnemySpin(nullptr), m_pEnemyIdling(nullptr), m_canAttack(true), m_enemyState(IState::EnemyState::IDLING), m_pEnemy(pEnemy)
{
	m_pEnemy = pEnemy;
	m_pEnemyAttack = std::make_unique<EnemyAttack>(this);
	m_pEnemyIdling = std::make_unique<EnemyIdling>(this);
	m_pEnemySpin = std::make_unique<EnemySpin>(this);
}
// �f�X�g���N�^
EnemyAI::~EnemyAI() {}

// ����������
void EnemyAI::Initialize()
{
	using namespace DirectX::SimpleMath;
	m_initialPosition = Vector3::Zero;  // �����ʒu��������
	m_initialPosition.y = GenerateRandomMultiplier(EnemyParameters::RANDOM_MIN, EnemyParameters::RANDOM_MAX); // �����ʒu��Y���W��ݒ�
	m_rotation.y = GenerateRandomMultiplier(EnemyParameters::RANDOM_MIN, EnemyParameters::RANDOM_MAX); // Y���̉�]��ݒ�
	m_velocity = EnemyParameters::INITIAL_VELOCITY; // ���V�̏������x
	m_scale = Vector3::One; // �X�P�[��������
	m_position = m_initialPosition;
	m_currentState = m_pEnemyIdling.get();
	m_currentState->Initialize();
	m_enemyState = IState::EnemyState::IDLING;// �ҋ@�Ԑ�
}
// �X�V
void EnemyAI::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;
	m_position = m_pEnemy->GetPosition();// �ʒu���擾
	if (m_pEnemy->GetEnemyHitByPlayerBullet())m_isHitPlayerBullet = true;// �v���C���[�̒e�ɓ����������ǂ������擾
	// sin�g��p�������V����̎���
	m_time += elapsedTime;
	// �G���ӂ�ӂ핂�V������
	m_position.y = m_initialPosition.y + EnemyParameters::AMPLITUDE * std::sin(EnemyParameters::FREQUENCY * m_time);
	m_position.y += m_velocity.y * elapsedTime;
	// �G���v���C���[�̈��͈͓��ɓ����Ă���ꍇ
	if ((m_pEnemy->GetHitToPlayer() || m_isHitPlayerBullet))
	{
		ChangeState(m_pEnemyAttack.get());//�U���Ԑ��ɂ���
		m_enemyState = IState::EnemyState::ATTACK;// �p�j�Ԑ�
	}
	else
	{
		ChangeState(m_pEnemyIdling.get());//�p�j�Ԑ��ɂ���
		m_enemyState = IState::EnemyState::IDLING;// �p�j�Ԑ�
	}
	// �v���C���[�̒e�ɓ��������ꍇ
	if (m_pEnemy->GetEnemyHitByPlayerBullet())
	{
		KnockBack(elapsedTime);
		ChangeState(m_pEnemySpin.get());//�X�s������
		m_enemyState = IState::EnemyState::HIT;// �U����H�������Ԃɂ���
	}
	m_currentState->Update(elapsedTime);
	m_pEnemy->SetPosition(m_position);
}
// �X�e�[�g�ύX
void EnemyAI::ChangeState(IState* newState)
{
	if (m_currentState != newState)
	{
		m_currentState = newState;
		m_currentState->Initialize();
	}
}


// �m�b�N�o�b�N����
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
	m_position += velocity * elapsedTime;
	if (GetState() != IState::EnemyState::ANGRY)// �{��Ԑ��łȂ��ꍇ
		SetState(IState::EnemyState::HIT);// �U����H�������Ԃɂ���
	// �m�b�N�o�b�N���I���������ǂ����`�F�b�N
	if (Progression >= EnemyParameters::KNOCKBACK_TIME.canAttackTime)
	{
		m_pEnemy->SetCanAttack(true);// �U���\�ɂ���
	}
	if (Progression >= EnemyParameters::KNOCKBACK_TIME.endKnockTime)
	{
		m_knockEndPosition = m_position;
		m_knockTime = 0.0f; // �m�b�N�o�b�N���Ԃ̃��Z�b�g
		m_pEnemy->SetEnemyHitByPlayerBullet(false); // �m�b�N�o�b�N�I��
		SetState(IState::EnemyState::IDLING);// �ҋ@�Ԑ�

	}
}
