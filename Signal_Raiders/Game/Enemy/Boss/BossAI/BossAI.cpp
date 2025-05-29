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
	m_pBossAttack = std::make_unique<BossAttack>(this);// �U����
	m_pBossKnockBacking = std::make_unique<BossKnockBacking>(this);// �m�b�N�o�b�N��
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
	m_initialPosition = m_pBoss->GetPosition();  // �����ʒu��ۑ�
	m_velocity = EnemyParameters::INITIAL_VELOCITY; // ���V�̏������x
	m_scale = EnemyParameters::INITIAL_BOSS_SCALE; // �X�P�[��������
	m_position = m_initialPosition;// �����ʒu��ݒ�
	m_pCurrentState = m_pBossAttack.get();// ���݂̃X�e�[�g���U���Ԑ��ɐݒ�
	m_pCurrentState->Initialize();// ������
	m_enemyState = m_attackState;// �U�����
}
/*
*	@brief �X�V
*	@details �{�XAI�N���X�̍X�V
*	@param elapsedTime �o�ߎ���
*	@return �Ȃ�
*/
void BossAI::Update(float elapsedTime)
{
	if (m_pBoss->GetEnemyHitByPlayerBullet())m_isHitPlayerBullet = true;// �v���C���[�̒e�ɓ���������
	m_time += elapsedTime;// ���Ԃ̍X�V
	m_position = m_pBoss->GetPosition();// �ʒu�̍X�V
	m_pCurrentState->Update(elapsedTime);// ���݂̃X�e�[�g���X�V
	m_position.y = m_initialPosition.y + EnemyParameters::AMPLITUDE * std::sin(EnemyParameters::FREQUENCY * m_time);// Y���W���X�V
	m_position.y += m_velocity.y * elapsedTime;// Y���W���X�V
	auto boss = dynamic_cast<BossBase*>(m_pBoss);// IEnemy����Boss�̃|�C���^�[�𒊏o
	if (!m_isKnockBack && boss->GetBossSheild()->GetSheildHP() <= 0)	// �V�[���h���󂳂ꂽ��m�b�N�o�b�N
	{
		ChangeState(m_pBossKnockBacking.get());//�m�b�N�o�b�N��Ԃɂ���
		m_enemyState = IState::EnemyState::HIT;// �U�����󂯂����̓���
		SetIsAttack(false);// �U�����ł͂Ȃ�
	}
	else // �V�[���h���󂳂�Ă��Ȃ��ꍇ
	{
		ChangeState(m_pBossAttack.get());//�U���Ԑ��ɂ���
		SetIsAttack(true);// �U�����ɂ���
		m_enemyState = m_attackState;// �U�����
	}
	m_pBoss->SetPosition(m_position);// �ʒu���Z�b�g
}
/*
*	@brief �X�e�[�g�ύX
*	@details �{�XAI�N���X�̃X�e�[�g�ύX
*	@param newState �V�����X�e�[�g
*	@return �Ȃ�
*/
void BossAI::ChangeState(IState* newState)
{
	if (m_pCurrentState != newState)// ���݂̃X�e�[�g�ƐV�����X�e�[�g���Ⴄ�ꍇ
	{
		m_pCurrentState = newState;// ���݂̃X�e�[�g��V�����X�e�[�g�ɕύX
		m_pCurrentState->Initialize();// ������
	}
}

