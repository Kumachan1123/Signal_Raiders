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

// �R���X�g���N�^
BossAI::BossAI(IEnemy* pBoss)
	: m_currentState(nullptr)
	, m_rotationSpeed(0.5f)
	, m_attackCooldown(0.0f)
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
	m_rotation = (GenerateRandomMultiplier(0, 2) <= 1.0f) ? Quaternion::Identity : -Quaternion::Identity;
	m_initialPosition = Vector3::Zero;  // �����ʒu��ۑ�
	m_initialPosition.y = 2 * GenerateRandomMultiplier(RANDOM_MIN, RANDOM_MAX) + 5;
	m_rotation.y = GenerateRandomMultiplier(RANDOM_MIN, RANDOM_MAX);
	m_velocity = Vector3(0.0f, 0.5f, 0.0f); // ���V�̏������x
	m_scale = Vector3::One; // �X�P�[��������
	m_time = 0.0f;  // ���Ԃ̏�����
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
	float amplitude = 2.0f;  // �U��
	float frequency = 0.5f;  // ���g��
	// �G���ӂ�ӂ핂�V������
	m_position.y = m_initialPosition.y + amplitude * std::sin(frequency * m_time);
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
		m_initialVelocity = knockBackDirection * 20; // �������x
		m_pBoss->SetCanAttack(false);// �U���s�\�ɂ���
	}

	// �m�b�N�o�b�N���Ԃ̍X�V
	m_knockTime += elapsedTime;

	// �m�b�N�o�b�N�̒����i�b�j
	const float knockBackDuration = 2.0f;

	// �m�b�N�o�b�N�̐i�s�x
	float t = std::min(m_knockTime / knockBackDuration, 2.0f);

	// �����W���̌v�Z�i�w���֐��I�����j
	float decayFactor = std::exp(-3.0f * t); // �������x�𒲐����邽�߂Ɏw���̃x�[�X�𒲐�

	// �����������x���g���Ĉʒu���X�V
	Vector3 velocity = m_initialVelocity * decayFactor;
	m_position += velocity * elapsedTime;
	if (t >= 0.3f)
	{
		m_pBoss->SetCanAttack(true);// �U���\�ɂ���
	}
	// �m�b�N�o�b�N���I���������ǂ����`�F�b�N
	if (t >= 1.0f)
	{
		m_knockEndPosition = m_position;
		m_knockTime = 0.0f; // �m�b�N�o�b�N���Ԃ̃��Z�b�g
		m_pBoss->SetEnemyHitByPlayerBullet(false); // �m�b�N�o�b�N�I��

		// ����ȍ~�m�b�N�o�b�N�������s��Ȃ�
		m_isKnockBack = true;
	}
}
