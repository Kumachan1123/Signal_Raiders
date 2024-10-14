/*
	@file	BossAttack.cpp
	@brief	�{�X�U���N���X
	�쐬�ҁF���܂�
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
#include "Game/Template/Template.h"

using namespace DirectX::SimpleMath;

// �R���X�g���N�^
BossAttack::BossAttack(BossAI* pBoss)
	: m_pBoss(pBoss)
	, m_attackCooldown{ 1.0f }
	, m_rotationSpeed{ .5f }
	, m_commonResources{}
{
}

// �f�X�g���N�^
BossAttack::~BossAttack() {}

// ����������
void BossAttack::Initialize()
{
	m_rotation = m_pBoss->GetRotation();
	m_velocity = m_pBoss->GetVelocity();
	m_scale = m_pBoss->GetScale();
}

void BossAttack::Update(float elapsedTime, DirectX::SimpleMath::Vector3& pos, DirectX::SimpleMath::Vector3& playerPos, bool isHitToPlayer)
{
	UNREFERENCED_PARAMETER(isHitToPlayer);

	// �v���C���[�ւ̃x�N�g�����v�Z
	Vector3 toPlayerVector = playerPos - pos;
	if (toPlayerVector.LengthSquared() > 0.0f)
	{
		toPlayerVector.Normalize();
	}

	// ���݂̑O���x�N�g�����擾
	Vector3 forward = Vector3::Transform(Vector3::Forward, m_rotation);
	if (forward.LengthSquared() > 0.0f)
	{
		forward.Normalize();
	}

	// ���ς��g���Ċp�x���v�Z
	float dot = toPlayerVector.Dot(forward);
	dot = clamp(dot, -1.0f, 1.0f);
	float angle = std::acos(dot);

	// �v���C���[�̕����Ɍ������߂̉�]���v�Z
	if (dot > 0.99f)  // �قڌ��������]�������X�L�b�v����i���ς�1�ɋ߂��Ƃ��j
	{
		// ���łɃv���C���[�̕����������Ă���̂ŉ�]���~�߂�
	}
	else
	{
		// �O�ς��g���ĉ�]����������
		Vector3 cross = toPlayerVector.Cross(forward);
		if (cross.y < 0) angle = -angle;

		// �v���C���[�̕����Ɍ������߂̉�]���v�Z
		Quaternion toPlayerRotation = Quaternion::CreateFromAxisAngle(Vector3::Up, angle);

		// ��]���X���[�Y�ɕ�Ԃ���
		m_rotation = Quaternion::Slerp(m_rotation, toPlayerRotation * m_rotation, elapsedTime * m_rotationSpeed);
		m_rotation.Normalize();
	}

	// �v���C���[�̕����Ɉړ�
	float moveSpeed = m_velocity.Length() * 2.0f;
	pos += toPlayerVector * moveSpeed * elapsedTime;

	// �U���̃N�[���_�E���^�C�����Ǘ�
	m_attackCooldown -= elapsedTime;
	if (m_attackCooldown <= .250f)
	{
		m_pBoss->SetState(IState::EnemyState::ANGRY);
		if (m_attackCooldown <= 0.0f) m_attackCooldown = 1.0f;
	}

	m_pBoss->SetRotation(m_rotation);
	m_pBoss->SetVelocity(m_velocity);
}