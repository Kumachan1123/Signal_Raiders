/*
*	@file	BossAttack.cpp
*	@brief	�{�X�U���N���X
*/
#include <pch.h>
#include "BossAttack.h"
/*
*	@brief	�R���X�g���N�^
*	@details �{�X�̍U���N���X�̃R���X�g���N�^
*	@param	pBoss	�{�XAI�N���X�̃|�C���^
*	@return	�Ȃ�
*/
BossAttack::BossAttack(BossAI* pBoss)
	: m_pBoss(pBoss)// �{�XAI�N���X�̃|�C���^
	, m_attackCooldown{  }// �U���N�[���_�E��
	, m_rotationSpeed{  }// ��]���x
	, m_pCommonResources{}// ���ʃ��\�[�X
{
	m_attackCooldown = ATTACK_INTERVAL;// �U���N�[���_�E��
	m_rotationSpeed = ROTATION_SPEED;// ��]���x
}

/*
*	@brief	�f�X�g���N�^
*	@details �{�X�̍U���N���X�̃f�X�g���N�^(�����ł͉������Ȃ�)
*	@param	�Ȃ�
*	@return	�Ȃ�
*/
BossAttack::~BossAttack() {/*do nothing*/ }

/*
*	@brief	������
*	@details �{�X�̍U���N���X�̏�����
*	@param	�Ȃ�
*	@return	�Ȃ�
*/
void BossAttack::Initialize()
{
	// �{�X�̈ʒu��������
	m_position = m_pBoss->GetPosition();
	// �{�X�̉�]��������
	m_rotation = m_pBoss->GetRotation();
	// �{�X�̑��x��������
	m_velocity = m_pBoss->GetVelocity();
	// �{�X�̃X�P�[����������
	m_scale = m_pBoss->GetScale();
}
/*
*	@brief	�v���C���[�̕����ɉ�]
*	@details �v���C���[�̕����ɉ�]����
*	@param playerPos �v���C���[�̈ʒu
*	@return �Ȃ�
*/
void BossAttack::RotateTowardsPlayer(const DirectX::SimpleMath::Vector3& playerPos)
{
	using namespace DirectX::SimpleMath;
	// �v���C���[�̕����ɉ�]
	m_rotation = Quaternion::CreateFromYawPitchRoll(CalculateAngle(m_position, playerPos), 0.0f, 0.0f);
}

/*
*	@brief	�v���C���[�̕����Ɉړ�
*	@details �v���C���[�̕����Ɉړ�����
*	@param elapsedTime �o�ߎ���
*	@param playerPos �v���C���[�̈ʒu
*	@return �Ȃ�
*/
void BossAttack::MoveTowardsPlayer(float elapsedTime, const DirectX::SimpleMath::Vector3& playerPos)
{
	// �v���C���[�̕����Ɉړ�
	m_position += Seek(m_pBoss->GetPosition(), playerPos, elapsedTime * EnemyParameters::BOSS_CHASE_SPEED);
}

/*
*	@brief	�U���N�[���_�E���̊Ǘ�
*	@details �U���N�[���_�E�����Ǘ�����
*	@param elapsedTime �o�ߎ���
*	@return �Ȃ�
*/
void BossAttack::ManageAttackCooldown(float elapsedTime)
{
	// �N�[���_�E�������炷
	m_attackCooldown -= elapsedTime;
	// �N�[���_�E����0�����������
	if (m_attackCooldown <= 0.0f)
	{
		// �N�[���_�E�����Z�b�g
		m_attackCooldown = EnemyParameters::ATTACK_INTERVAL;
		// �U����ԂɑJ��
		m_pBoss->SetState(IState::EnemyState::ATTACK);
	}
}
/*
*	@brief	�X�V
*	@details �{�X�̍U���N���X�̍X�V
*	@param elapsedTime �o�ߎ���
*	@return �Ȃ�
*/
void BossAttack::Update(float elapsedTime)
{
	// �v���C���[�̕����ɉ�]
	RotateTowardsPlayer(m_pBoss->GetEnemy()->GetPlayer()->GetPlayerPos());
	// �v���C���[�̕����Ɉړ�
	MoveTowardsPlayer(elapsedTime, m_pBoss->GetEnemy()->GetPlayer()->GetPlayerPos());
	// �U���N�[���_�E���̊Ǘ�
	ManageAttackCooldown(elapsedTime);
	// �{�X�̉�]���X�V
	m_pBoss->SetRotation(m_rotation);
	// �{�X�̑��x���X�V
	m_pBoss->SetVelocity(m_velocity);
	// �{�X�̈ʒu���X�V
	m_pBoss->SetPosition(m_position);
}
