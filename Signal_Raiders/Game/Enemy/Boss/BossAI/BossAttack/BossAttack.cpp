/*
	@file	BossAttack.cpp
	@brief	�{�X�U���N���X
*/
#include "pch.h"
#include <SimpleMath.h>
#include "BossAttack.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Game/Enemy/Boss/BossAI/BossAI.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/InputManager.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include <cassert>
#include <random>
#include "Game/KumachiLib/KumachiLib.h"
#include <cmath>

using namespace DirectX::SimpleMath;

// �R���X�g���N�^
BossAttack::BossAttack(BossAI* pBoss)
	: m_pBoss(pBoss)
	, m_attackCooldown{  }
	, m_rotationSpeed{  }
	, m_commonResources{}
{
	m_attackCooldown = ATTACK_INTERVAL;
	m_rotationSpeed = ROTATION_SPEED;
}

// �f�X�g���N�^
BossAttack::~BossAttack() {}

// ����������
void BossAttack::Initialize()
{
	m_position = m_pBoss->GetPosition();// �ʒu
	m_rotation = m_pBoss->GetRotation();// ��]
	m_velocity = m_pBoss->GetVelocity();// ���x
	m_scale = m_pBoss->GetScale();// �X�P�[��
}


// �v���C���[�Ɍ������ĉ�]����
void BossAttack::RotateTowardsPlayer(DirectX::SimpleMath::Vector3 playerPos)
{
	m_rotation = DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(CalculateAngle(m_position, playerPos), 0.0f, 0.0f);
}

// �{�X�̈ʒu���v���C���[�����ɍX�V
void BossAttack::MoveTowardsPlayer(float elapsedTime, DirectX::SimpleMath::Vector3 playerPos)
{
	m_position += Seek(m_pBoss->GetPosition(), playerPos, elapsedTime * 2);
}

// �N�[���_�E���̊Ǘ�
void BossAttack::ManageAttackCooldown(float elapsedTime)
{
	m_attackCooldown -= elapsedTime;
	if (m_attackCooldown <= 0.250f)
	{
		m_pBoss->SetState(IState::EnemyState::ANGRY);
		if (m_attackCooldown <= 0.0f)// �N�[���_�E����0�����������
		{
			m_attackCooldown = ATTACK_INTERVAL;  // �N�[���_�E�����Z�b�g
			m_pBoss->SetState(IState::EnemyState::ATTACK);  // �U����ԂɑJ��
		}
	}
}

// �X�V�֐�
void BossAttack::Update(float elapsedTime)
{
	// �v���C���[�̕����ɉ�]
	RotateTowardsPlayer(m_pBoss->GetEnemy()->GetPlayer()->GetPlayerPos());

	// �v���C���[�̕����Ɉړ�
	MoveTowardsPlayer(elapsedTime, m_pBoss->GetEnemy()->GetPlayer()->GetPlayerPos());

	// �N�[���_�E���̍X�V
	ManageAttackCooldown(elapsedTime);

	// �{�X�̏�Ԃ��X�V
	m_pBoss->SetRotation(m_rotation);
	m_pBoss->SetVelocity(m_velocity);
	m_pBoss->SetPosition(m_position);
}
