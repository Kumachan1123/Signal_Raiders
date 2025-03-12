/*
	@file	BossAttack.cpp
	@brief	�{�X�U���N���X
*/
#include "pch.h"
#include "BossAttack.h"

using namespace DirectX::SimpleMath;

/*
*	@brief	�R���X�g���N�^
*	@param[in]	pBoss	�{�XAI�N���X�̃|�C���^
*	@return	�Ȃ�
*/
BossAttack::BossAttack(BossAI* pBoss)
	: m_pBoss(pBoss)// �{�XAI�N���X�̃|�C���^
	, m_attackCooldown{  }// �U���N�[���_�E��
	, m_rotationSpeed{  }// ��]���x
	, m_commonResources{}// ���ʃ��\�[�X
{
	m_attackCooldown = ATTACK_INTERVAL;// �U���N�[���_�E��
	m_rotationSpeed = ROTATION_SPEED;// ��]���x
}

/*
*	@brief	�f�X�g���N�^
*	@return	�Ȃ�
*/
BossAttack::~BossAttack() {}

/*
*	@brief	������
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
*	@param[in] playerPos �v���C���[�̈ʒu
*	@return �Ȃ�
*/
void BossAttack::RotateTowardsPlayer(DirectX::SimpleMath::Vector3 playerPos)
{
	m_rotation = DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll
	(CalculateAngle(m_position, playerPos), 0.0f, 0.0f);// �v���C���[�̕����ɉ�]
}

/*
*	@brief	�v���C���[�̕����Ɉړ�
*	@param[in] elapsedTime �o�ߎ���
*	@param[in] playerPos �v���C���[�̈ʒu
*	@return �Ȃ�
*/
void BossAttack::MoveTowardsPlayer(float elapsedTime, DirectX::SimpleMath::Vector3 playerPos)
{
	m_position += Seek(m_pBoss->GetPosition(), playerPos, elapsedTime * EnemyParameters::BOSS_CHASE_SPEED);// �v���C���[�̕����Ɉړ�
}

/*
*	@brief	�U���N�[���_�E���̊Ǘ�
*	@param[in] elapsedTime �o�ߎ���
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
*	@param[in] elapsedTime �o�ߎ���
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
