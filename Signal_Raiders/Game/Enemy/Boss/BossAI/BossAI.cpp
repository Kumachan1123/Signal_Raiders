/*
*	@file	BossAI.cpp
*	@brief	�GAI�N���X
*/
#include <pch.h>
#include "BossAI.h"
/*
*	@brief	�R���X�g���N�^
*	@details �{�X�N���X��AI���Ǘ�����N���X
*	@param pBoss �{�X�N���X�̃|�C���^
*	@return �Ȃ�
*/
BossAI::BossAI(IEnemy* pBoss)
	: m_pCurrentState(nullptr)// ���݂̃X�e�[�g
	, m_attackCooldown(0.0f)// �U���N�[���_�E��
	, m_knockTime(0.0f)// �m�b�N�o�b�N����
	, m_time(0.0f)// ����
	, m_pBossAttack(nullptr)// �U����
	, m_enemyState(IState::EnemyState::ATTACK)// ���
	, m_attackState(IState::EnemyState::ATTACK)// �U�����\���
	, m_pBoss(pBoss)// �{�X
	, m_isHitPlayerBullet(false)// �v���C���[�̒e�ɓ���������
	, m_isKnockBack(false)// �m�b�N�o�b�N����
	, m_isAttack(false)// �U������
{
	// �U����Ԃɂ��邽�߂̃X�e�[�g�𐶐�
	m_pBossAttack = std::make_unique<BossAttack>(this);
	// �m�b�N�o�b�N��Ԃɂ��邽�߂̃X�e�[�g�𐶐�
	m_pBossKnockBacking = std::make_unique<BossKnockBacking>(this);
}

/*
*	@brief �f�X�g���N�^
*	@details �{�XAI�N���X�̃f�X�g���N�^(�����ł͉������Ȃ�)
*	@param �Ȃ�
*	@return �Ȃ�
*/
BossAI::~BossAI() {/*do nothing*/ }

/*
*	@brief ������
*	@details �{�XAI�N���X�̏�����
*	@param �Ȃ�
*	@return �Ȃ�
*/
void BossAI::Initialize()
{
	// �����ʒu��ۑ�
	m_initialPosition = m_pBoss->GetPosition();
	// ���V�̏������x��ݒ�
	m_velocity = EnemyParameters::INITIAL_VELOCITY;
	// �X�P�[��������
	m_scale = EnemyParameters::INITIAL_BOSS_SCALE;
	// �����ʒu��ݒ�
	m_position = m_initialPosition;
	// ���݂̃X�e�[�g���U����Ԃɐݒ�
	m_pCurrentState = m_pBossAttack.get();
	// ���݂̃X�e�[�g��������
	m_pCurrentState->Initialize();
	// �U�����̕\�����ݒ�
	m_enemyState = m_attackState;
}
/*
*	@brief �X�V
*	@details �{�XAI�N���X�̍X�V
*	@param elapsedTime �o�ߎ���
*	@return �Ȃ�
*/
void BossAI::Update(float elapsedTime)
{
	// �v���C���[�̒e�ɓ���������q�b�g�t���O��true�ɂ���
	if (m_pBoss->GetEnemyHitByPlayerBullet())m_isHitPlayerBullet = true;
	// ���Ԃ̍X�V
	m_time += elapsedTime;
	// �ʒu�̍X�V
	m_position = m_pBoss->GetPosition();
	// ���݂̃X�e�[�g���X�V
	m_pCurrentState->Update(elapsedTime);
	// �����ʒu����ɁA�T�C���g���g���ď㉺�ɗh���悤�ȓ�����������
	m_position.y = m_initialPosition.y + EnemyParameters::AMPLITUDE * std::sin(EnemyParameters::FREQUENCY * m_time);
	// Y���W���X�V
	m_position.y += m_velocity.y * elapsedTime;
	// IEnemy����Boss�̃|�C���^�[�𒊏o
	auto boss = dynamic_cast<BossBase*>(m_pBoss);
	// �V�[���h���󂳂ꂽ��m�b�N�o�b�N
	if (!m_isKnockBack && boss->GetBossSheild()->GetSheildHP() <= 0)
	{
		//�m�b�N�o�b�N��Ԃɂ���
		ChangeState(m_pBossKnockBacking.get());
		// �U�����󂯂����̓���
		m_enemyState = IState::EnemyState::HIT;
		// �U�����ł͂Ȃ�
		SetIsAttack(false);
	}
	else // �V�[���h���󂳂�Ă��Ȃ��ꍇ
	{
		//�U����Ԃɂ���
		ChangeState(m_pBossAttack.get());
		// �U�����ɂ���
		SetIsAttack(true);
		// �U�����̕\�����ݒ�
		m_enemyState = m_attackState;
	}
	// �ʒu���Z�b�g
	m_pBoss->SetPosition(m_position);
}
/*
*	@brief �X�e�[�g�ύX
*	@details �{�XAI�N���X�̃X�e�[�g�ύX
*	@param newState �V�����X�e�[�g
*	@return �Ȃ�
*/
void BossAI::ChangeState(IState* newState)
{
	// ���݂̃X�e�[�g�ƐV�����X�e�[�g���Ⴄ�ꍇ
	if (m_pCurrentState != newState)
	{
		// ���݂̃X�e�[�g��V�����X�e�[�g�ɕύX
		m_pCurrentState = newState;
		// �V�����X�e�[�g��������
		m_pCurrentState->Initialize();
	}
}