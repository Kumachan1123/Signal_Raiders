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
	m_position = m_pBoss->GetPosition();// �ʒu
	m_rotation = m_pBoss->GetRotation();// ��]
	m_velocity = m_pBoss->GetVelocity();// ���x
	m_scale = m_pBoss->GetScale();// �X�P�[��
}
/*
*	@brief	�v���C���[�̕����ɉ�]
*	@details �v���C���[�̕����ɉ�]����
*	@param playerPos �v���C���[�̈ʒu
*	@return �Ȃ�
*/
void BossAttack::RotateTowardsPlayer(DirectX::SimpleMath::Vector3 playerPos)
{
	using namespace DirectX::SimpleMath;
	m_rotation = Quaternion::CreateFromYawPitchRoll
	(CalculateAngle(m_position, playerPos), 0.0f, 0.0f);// �v���C���[�̕����ɉ�]
}

/*
*	@brief	�v���C���[�̕����Ɉړ�
*	@details �v���C���[�̕����Ɉړ�����
*	@param elapsedTime �o�ߎ���
*	@param playerPos �v���C���[�̈ʒu
*	@return �Ȃ�
*/
void BossAttack::MoveTowardsPlayer(float elapsedTime, DirectX::SimpleMath::Vector3 playerPos)
{
	m_position += Seek(m_pBoss->GetPosition(), playerPos, elapsedTime * EnemyParameters::BOSS_CHASE_SPEED);// �v���C���[�̕����Ɉړ�
}

/*
*	@brief	�U���N�[���_�E���̊Ǘ�
*	@details �U���N�[���_�E�����Ǘ�����
*	@param elapsedTime �o�ߎ���
*	@return �Ȃ�
*/
void BossAttack::ManageAttackCooldown(float elapsedTime)
{
	m_attackCooldown -= elapsedTime;// �N�[���_�E�������炷
	if (m_attackCooldown <= 0.0f)// �N�[���_�E����0�����������
	{
		m_attackCooldown = EnemyParameters::ATTACK_INTERVAL;  // �N�[���_�E�����Z�b�g
		m_pBoss->SetState(IState::EnemyState::ATTACK);  // �U����ԂɑJ��
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
	RotateTowardsPlayer(m_pBoss->GetEnemy()->GetPlayer()->GetPlayerPos());	// �v���C���[�̕����ɉ�]
	MoveTowardsPlayer(elapsedTime, m_pBoss->GetEnemy()->GetPlayer()->GetPlayerPos());	// �v���C���[�̕����Ɉړ�
	ManageAttackCooldown(elapsedTime);	// �U���N�[���_�E���̊Ǘ�
	// �{�X�̏�Ԃ��X�V
	m_pBoss->SetRotation(m_rotation);// ��]
	m_pBoss->SetVelocity(m_velocity);// ���x
	m_pBoss->SetPosition(m_position);// �ʒu
}
