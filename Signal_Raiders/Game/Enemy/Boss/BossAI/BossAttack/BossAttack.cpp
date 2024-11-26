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
	m_rotation = m_pBoss->GetRotation();// ��]
	m_velocity = m_pBoss->GetVelocity();// ���x
	m_scale = m_pBoss->GetScale();// �X�P�[��
}

// �v���C���[�����̃x�N�g�����v�Z����
DirectX::SimpleMath::Vector3 BossAttack::CalculateToPlayerVector(const Vector3& pos, const Vector3& playerPos)
{
	Vector3 toPlayerVector = playerPos - pos;
	if (toPlayerVector.LengthSquared() > 0.0f)
	{
		toPlayerVector.Normalize();
	}
	return toPlayerVector;
}

// �v���C���[�Ɍ������ĉ�]����
void BossAttack::RotateTowardsPlayer(float elapsedTime, const Vector3& toPlayerVector)
{
	// ���݂̑O���x�N�g�����擾
	Vector3 forward = Vector3::Transform(Vector3::Forward, m_rotation);
	if (forward.LengthSquared() > 0.0f)
	{
		forward.Normalize();
	}

	// atan2���g���Ċp�x���v�Z
	float dot = Clamp(toPlayerVector.Dot(forward), -1.0f, 1.0f);
	float angle = std::acos(dot);

	// ��]����
	if (dot < 0.999f)  // �v���C���[�̕����������Ă��Ȃ��ꍇ�̂݉�]
	{
		Vector3 cross = toPlayerVector.Cross(forward);
		if (cross.y < 0) angle = -angle;

		Quaternion toPlayerRotation = Quaternion::CreateFromAxisAngle(Vector3::Up, angle);
		m_rotation = Quaternion::Slerp(m_rotation, toPlayerRotation * m_rotation, elapsedTime * m_rotationSpeed);
		m_rotation.Normalize();
	}
}

// �{�X�̈ʒu���v���C���[�����ɍX�V
void BossAttack::MoveTowardsPlayer(float elapsedTime, const Vector3& toPlayerVector, Vector3& pos)
{
	float moveSpeed = m_velocity.Length() * 2.0f;
	pos += toPlayerVector * moveSpeed * elapsedTime;
}

// �N�[���_�E���̊Ǘ�
void BossAttack::ManageAttackCooldown(float elapsedTime)
{
	m_attackCooldown -= elapsedTime;
	if (m_attackCooldown <= 0.250f)
	{
		m_pBoss->SetState(IState::EnemyState::ANGRY);
		if (m_attackCooldown <= 0.0f)
		{
			m_attackCooldown = ATTACK_INTERVAL;  // �N�[���_�E�����Z�b�g
		}
	}
}

// �X�V�֐�
void BossAttack::Update(float elapsedTime, DirectX::SimpleMath::Vector3& pos, DirectX::SimpleMath::Vector3& playerPos, bool isHitToPlayer)
{
	UNREFERENCED_PARAMETER(isHitToPlayer);

	// �v���C���[�ւ̕������v�Z
	Vector3 toPlayerVector = CalculateToPlayerVector(pos, playerPos);

	// �v���C���[�̕����ɉ�]
	RotateTowardsPlayer(elapsedTime, toPlayerVector);

	// �v���C���[�̕����Ɉړ�
	MoveTowardsPlayer(elapsedTime, toPlayerVector, pos);

	// �N�[���_�E���̍X�V
	ManageAttackCooldown(elapsedTime);

	// ��]���x���������A�ŏ��l���m��
	m_rotationSpeed = std::max(m_rotationSpeed - 0.05f, 0.1f);

	// �{�X�̏�Ԃ��X�V
	m_pBoss->SetRotation(m_rotation);
	m_pBoss->SetVelocity(m_velocity);
}
