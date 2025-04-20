/*
	@file	BossAI.cpp
	@brief	�GAI�N���X
*/
#include "pch.h"
#include "BossAI.h"

using namespace DirectX::SimpleMath;


/*
*	@brief	�R���X�g���N�^
*	@param[in] pBoss �{�X�N���X�̃|�C���^
*	@return �Ȃ�
*/
BossAI::BossAI(IEnemy* pBoss)
	: m_currentState(nullptr)// ���݂̃X�e�[�g
	, m_attackCooldown(0.0f)// �U���N�[���_�E��
	, m_knockTime(0.0f)// �m�b�N�o�b�N����
	, m_time(0.0f)// ����
	, m_pBossAttack(nullptr)// �U����
	, m_enemyState(IState::EnemyState::IDLING)// ���
	, m_pBoss(pBoss)// �{�X
	, m_isHitPlayerBullet(false)// �v���C���[�̒e�ɓ���������
	, m_isKnockBack(false)// �m�b�N�o�b�N����
	, m_isAttack(false)// �U������
{
	m_pBossAttack = std::make_unique<BossAttack>(this);// �U����
	m_pBossIdling = std::make_unique<BossIdling>(this);// �ҋ@��

}

/*
*	@brief �f�X�g���N�^
*	@return �Ȃ�
*/
BossAI::~BossAI() {}

/*
*	@brief ������
*	@return �Ȃ�
*/
void BossAI::Initialize()
{
	m_initialPosition = Vector3::Zero;  // �����ʒu��ۑ�
	m_initialPosition.y = GenerateRandomMultiplier(EnemyParameters::RANDOM_MIN, EnemyParameters::RANDOM_MAX);// Y���W�������_���ɐݒ�
	m_velocity = EnemyParameters::INITIAL_VELOCITY; // ���V�̏������x
	m_scale = EnemyParameters::INITIAL_BOSS_SCALE; // �X�P�[��������
	m_position = m_initialPosition;// �����ʒu��ݒ�
	m_currentState = m_pBossAttack.get();// ���݂̃X�e�[�g���U���Ԑ��ɐݒ�
	m_currentState->Initialize();// ������
	m_enemyState = IState::EnemyState::ATTACK;// �ҋ@�Ԑ�
}
/*
*	@brief �X�V
*	@param[in] elapsedTime �o�ߎ���
*	@return �Ȃ�
*/
void BossAI::Update(float elapsedTime)
{
	if (m_pBoss->GetEnemyHitByPlayerBullet())m_isHitPlayerBullet = true;// �v���C���[�̒e�ɓ���������
	m_time += elapsedTime;// ���Ԃ̍X�V
	m_position = m_pBoss->GetPosition();// �ʒu�̍X�V
	m_currentState->Update(elapsedTime);// ���݂̃X�e�[�g���X�V
	m_position.y = m_initialPosition.y + EnemyParameters::AMPLITUDE * std::sin(EnemyParameters::FREQUENCY * m_time);// Y���W���X�V
	m_position.y += m_velocity.y * elapsedTime;// Y���W���X�V
	auto boss = dynamic_cast<BossBase*>(m_pBoss);// IEnemy����Boss�̃|�C���^�[�𒊏o
	if (!m_isKnockBack && boss->GetBossSheild()->GetSheildHP() <= 0)	// �V�[���h���󂳂ꂽ��m�b�N�o�b�N
	{
		ChangeState(m_pBossIdling.get());//�p�j�Ԑ��ɂ���
		KnockBack(elapsedTime);// �m�b�N�o�b�N����
		SetIsAttack(false);// �U�����ł͂Ȃ�
	}
	else // �V�[���h���󂳂�Ă��Ȃ��ꍇ
	{
		ChangeState(m_pBossAttack.get());//�U���Ԑ��ɂ���
		SetIsAttack(true);// �U�����ɂ���
	}
	m_pBoss->SetPosition(m_position);// �ʒu���Z�b�g
}
/*
*	@brief �X�e�[�g�ύX
*	@param[in] newState �V�����X�e�[�g
*	@return �Ȃ�
*/
void BossAI::ChangeState(IState* newState)
{
	if (m_currentState != newState)// ���݂̃X�e�[�g�ƐV�����X�e�[�g���Ⴄ�ꍇ
	{
		m_currentState = newState;// ���݂̃X�e�[�g��V�����X�e�[�g�ɕύX
		m_currentState->Initialize();// ������
	}
}

/*
*	@brief �m�b�N�o�b�N
*	@param[in] elapsedTime �o�ߎ���
*	@return �Ȃ�
*/
void BossAI::KnockBack(float elapsedTime)
{
	if (m_knockTime == 0.0f)// �m�b�N�o�b�N�J�n���̏���
	{
		m_knockStartPosition = m_position; // �m�b�N�o�b�N�J�n�ʒu
		Vector3 knockBackDirection = (m_position - m_pBoss->GetPlayer()->GetPlayerPos()); // �v���C���[����G�ւ̕����x�N�g��
		knockBackDirection.Normalize(); // ���K�����ĕ����x�N�g���ɂ���
		m_knockEndPosition = m_position + knockBackDirection; // �m�b�N�o�b�N�I���ʒu
		m_initialVelocity = knockBackDirection * EnemyParameters::FIXED_INITIAL_SPEED; // �������x
		m_pBoss->SetCanAttack(false);// �U���s�\�ɂ���
	}
	m_knockTime += elapsedTime;	// �m�b�N�o�b�N���Ԃ̍X�V
	float Progression = std::min(m_knockTime / EnemyParameters::KNOCKBACK_DURATION,
		EnemyParameters::KNOCKBACK_PROGRESS_MAX);	// �m�b�N�o�b�N�̐i�s�x
	float decayFactor = std::exp(EnemyParameters::KNOCKBACK_DECAY_RATE * Progression); // �������x�𒲐����邽�߂Ɏw���̃x�[�X�𒲐�
	Vector3 velocity = m_initialVelocity * decayFactor;	// �����������x���g���Ĉʒu���X�V
	m_position += velocity * elapsedTime;	// �ʒu���X�V
	if (Progression >= EnemyParameters::KNOCKBACK_TIME.canAttackTime)m_pBoss->SetCanAttack(true);// �m�b�N�o�b�N�̐i�s�x�����ȏ�ɂȂ�����U���\�ɂ���
	if (Progression >= EnemyParameters::KNOCKBACK_TIME.endKnockTime)	// �m�b�N�o�b�N���I���������ǂ����`�F�b�N
	{
		m_knockEndPosition = m_position; // �m�b�N�o�b�N�I���ʒu���X�V
		m_knockTime = 0.0f; // �m�b�N�o�b�N���Ԃ̃��Z�b�g
		m_pBoss->SetEnemyHitByPlayerBullet(false); // �m�b�N�o�b�N�I��	
		m_isKnockBack = true;// ����ȍ~�m�b�N�o�b�N�������s��Ȃ�
	}
}
