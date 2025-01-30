/*
	@file	BossAI.cpp
	@brief	�GAI�N���X
*/
#include "pch.h"
#include <SimpleMath.h>
#include "BossAI.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/InputManager.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include <cassert>
#include "Game/KumachiLib/KumachiLib.h"
#include <random>
#include <type_traits> // std::enable_if, std::is_integral
#include "Game/Enemy/Parameters/EnemyParameters.h"
// �R���X�g���N�^
BossAI::BossAI(IEnemy* pBoss)
	: m_currentState(nullptr)
	, m_attackCooldown(0.0f)
	, m_knockTime(0.0f)
	, m_time(0.0f)
	, m_pBossAttack(nullptr)
	, m_enemyState(IState::EnemyState::IDLING)
	, m_pBoss(pBoss)
	, m_isHitPlayerBullet(false)
	, m_isKnockBack(false)
{
	m_pBossAttack = std::make_unique<BossAttack>(this);
	m_pBossIdling = std::make_unique<BossIdling>(this);

}
// �f�X�g���N�^
BossAI::~BossAI() {}

// ����������
void BossAI::Initialize()
{
	using namespace DirectX::SimpleMath;
	m_initialPosition = Vector3::Zero;  // �����ʒu��ۑ�
	m_initialPosition.y = GenerateRandomMultiplier(EnemyParameters::RANDOM_MIN, EnemyParameters::RANDOM_MAX);
	m_velocity = EnemyParameters::INITIAL_VELOCITY; // ���V�̏������x
	m_scale = EnemyParameters::INITIAL_BOSS_SCALE; // �X�P�[��������
	m_position = m_initialPosition;
	m_currentState = m_pBossAttack.get();
	m_currentState->Initialize();
	m_enemyState = IState::EnemyState::ATTACK;// �ҋ@�Ԑ�
}
// �X�V
void BossAI::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;
	if (m_pBoss->GetEnemyHitByPlayerBullet())m_isHitPlayerBullet = true;
	// sin�g��p�������V����̎���
	m_time += elapsedTime;

	m_position = m_pBoss->GetPosition();
	m_currentState->Update(elapsedTime);
	// sin�g��p�������V����̎���
	// �G���ӂ�ӂ핂�V������
	m_position.y = m_initialPosition.y + EnemyParameters::AMPLITUDE * std::sin(EnemyParameters::FREQUENCY * m_time);
	m_position.y += m_velocity.y * elapsedTime;
	// �V�[���h���󂳂ꂽ��m�b�N�o�b�N
	auto boss = dynamic_cast<Boss*>(m_pBoss);// IEnemy����Boss�̃|�C���^�[�𒊏o
	if (!m_isKnockBack && boss->GetBossSheild()->GetSheildHP() <= 0)
	{
		ChangeState(m_pBossIdling.get());//�p�j�Ԑ��ɂ�
		KnockBack(elapsedTime);
	}
	else
	{
		ChangeState(m_pBossAttack.get());//�U���Ԑ��ɂ���
	}
	m_pBoss->SetPosition(m_position);
}
// �X�e�[�g�ύX
void BossAI::ChangeState(IState* newState)
{
	if (m_currentState != newState)
	{
		m_currentState = newState;
		m_currentState->Initialize();
	}
}

// �m�b�N�o�b�N����
void BossAI::KnockBack(float elapsedTime)
{
	using namespace DirectX::SimpleMath;

	// �m�b�N�o�b�N���n�܂����΂���Ȃ珉���ݒ���s��
	if (m_knockTime == 0.0f)
	{
		m_knockStartPosition = m_position; // �m�b�N�o�b�N�J�n�ʒu
		Vector3 knockBackDirection = (m_position - m_pBoss->GetPlayer()->GetPlayerPos()); // �v���C���[����G�ւ̕����x�N�g��
		knockBackDirection.Normalize(); // ���K�����ĕ����x�N�g���ɂ���
		m_knockEndPosition = m_position + knockBackDirection; // �m�b�N�o�b�N�I���ʒu
		m_initialVelocity = knockBackDirection * EnemyParameters::FIXED_INITIAL_SPEED; // �������x
		m_pBoss->SetCanAttack(false);// �U���s�\�ɂ���
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
	// �m�b�N�o�b�N�̐i�s�x�����ȏ�ɂȂ�����U���\�ɂ���
	if (Progression >= EnemyParameters::KNOCKBACK_TIME.canAttackTime)
	{
		m_pBoss->SetCanAttack(true);// �U���\�ɂ���
	}
	// �m�b�N�o�b�N���I���������ǂ����`�F�b�N
	if (Progression >= EnemyParameters::KNOCKBACK_TIME.endKnockTime)
	{
		m_knockEndPosition = m_position;
		m_knockTime = 0.0f; // �m�b�N�o�b�N���Ԃ̃��Z�b�g
		m_pBoss->SetEnemyHitByPlayerBullet(false); // �m�b�N�o�b�N�I��

		// ����ȍ~�m�b�N�o�b�N�������s��Ȃ�
		m_isKnockBack = true;
	}
}
