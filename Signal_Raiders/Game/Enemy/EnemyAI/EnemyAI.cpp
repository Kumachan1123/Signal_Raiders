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
class EnemyAttack;
class EnemyIdling;
// �R���X�g���N�^
EnemyAI::EnemyAI(IEnemy* pEnemy)
	: m_currentState(nullptr), m_rotationSpeed(0.5f), m_attackCooldown(0.0f), m_pEnemyAttack(nullptr)
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
	m_rotation = (GenerateRandomMultiplier(0, 2) <= 1.0f) ? Quaternion::Identity : -Quaternion::Identity;// ��]�̏�����
	m_initialPosition = Vector3::Zero;  // �����ʒu��������
	m_initialPosition.y = GenerateRandomMultiplier(RANDOM_MIN, RANDOM_MAX); // �����ʒu��Y���W��ݒ�
	m_rotation.y = GenerateRandomMultiplier(RANDOM_MIN, RANDOM_MAX); // Y���̉�]��ݒ�
	m_velocity = Vector3(0.0f, 0.5f, 0.0f); // ���V�̏������x
	m_scale = Vector3::One; // �X�P�[��������
	m_time = 0.0f;  // ���Ԃ̏�����
	m_position = m_initialPosition;
	m_currentState = m_pEnemyIdling.get();
	m_currentState->Initialize();
	m_enemyState = IState::EnemyState::IDLING;// �ҋ@�Ԑ�
}
// �X�V
void EnemyAI::Update(float elapsedTime, DirectX::SimpleMath::Vector3& pos, DirectX::SimpleMath::Vector3& playerPos, bool& isHitToPlayer, bool& isHitToPlayerBullet)
{
	using namespace DirectX::SimpleMath;
	if (isHitToPlayerBullet)m_isHitPlayerBullet = true;
	// sin�g��p�������V����̎���
	m_time += elapsedTime;
	// sin�g��p�������V����̎���
	float amplitude = 2.0f;  // �U��
	float frequency = 0.5f;  // ���g��
	// �G���ӂ�ӂ핂�V������
	pos.y = m_initialPosition.y + amplitude * std::sin(frequency * m_time);
	pos.y += m_velocity.y * elapsedTime;
	// �G���v���C���[�̈��͈͓��ɓ����Ă���ꍇ
	if ((isHitToPlayer || m_isHitPlayerBullet))
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
	if (isHitToPlayerBullet)
	{
		KnockBack(elapsedTime, pos, isHitToPlayerBullet, playerPos);
		ChangeState(m_pEnemySpin.get());//����Ԑ��ɂ���
		m_enemyState = IState::EnemyState::HIT;// ����Ԑ�
	}
	m_currentState->Update(elapsedTime, pos, playerPos, isHitToPlayer);

	m_position = pos;
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
void EnemyAI::KnockBack(float elapsedTime, DirectX::SimpleMath::Vector3& pos, bool& isHitToPlayerBullet, const DirectX::SimpleMath::Vector3& playerPos)
{
	using namespace DirectX::SimpleMath;

	// �m�b�N�o�b�N���n�܂����΂���Ȃ珉���ݒ���s��
	if (m_knockTime == 0.0f)
	{
		m_knockStartPosition = pos; // �m�b�N�o�b�N�J�n�ʒu
		Vector3 knockBackDirection = (pos - playerPos); // �v���C���[����G�ւ̕����x�N�g��
		knockBackDirection.Normalize(); // ���K�����ĕ����x�N�g���ɂ���
		m_knockEndPosition = pos + knockBackDirection; // �m�b�N�o�b�N�I���ʒu
		m_initialVelocity = knockBackDirection * 20; // �������x
		m_pEnemy->SetCanAttack(false);// �U���s�\�ɂ���
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
	pos += velocity * elapsedTime;
	if (GetState() != IState::EnemyState::ANGRY)// �{��Ԑ��łȂ��ꍇ
		SetState(IState::EnemyState::HIT);// �_���[�W�Ԑ��ɂ���
	// �m�b�N�o�b�N���I���������ǂ����`�F�b�N
	if (t >= 0.5f)
	{
		m_pEnemy->SetCanAttack(true);// �U���\�ɂ���
	}
	if (t >= 1.0f)
	{
		m_knockEndPosition = pos;
		m_knockTime = 0.0f; // �m�b�N�o�b�N���Ԃ̃��Z�b�g
		isHitToPlayerBullet = false; // �m�b�N�o�b�N�I��
		SetState(IState::EnemyState::IDLING);// �ҋ@�Ԑ�

	}
}
