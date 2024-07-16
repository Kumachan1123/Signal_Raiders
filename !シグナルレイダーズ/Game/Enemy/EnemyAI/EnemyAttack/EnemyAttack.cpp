/*
	@file	EnemyAttack.cpp
	@brief	�G�U���N���X
	�쐬�ҁF���܂�
*/
#include "pch.h"
#include <SimpleMath.h>
#include "Game/Enemy/EnemyAI/EnemyAttack/EnemyAttack.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Game/Enemy/EnemyAI/EnemyAI.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/InputManager.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include <cassert>
#include <random>  
#include "Game/Template/Template.h"

using namespace DirectX::SimpleMath;
// �R���X�g���N�^
EnemyAttack::EnemyAttack(EnemyAI* enemy)
	: m_enemy(enemy)
	, m_attackCooldown{ 3.0f }
	, m_rotationSpeed{}
{
}
// �f�X�g���N�^
EnemyAttack::~EnemyAttack() {}
// ����������
void EnemyAttack::Initialize()
{
	m_rotation = m_enemy->GetRotation();
	m_velocity = m_enemy->GetVelocity();
	m_scale = m_enemy->GetScale();
	m_rotationSpeed = 1; // ��]���x
}

void EnemyAttack::Update(float elapsedTime, DirectX::SimpleMath::Vector3& pos, DirectX::SimpleMath::Vector3& playerPos, bool isHitToPlayer)
{
	UNREFERENCED_PARAMETER(isHitToPlayer);
	using namespace DirectX::SimpleMath;
	m_rotationSpeed -= (elapsedTime / 10);
	if (m_rotationSpeed <= 0.24f)
	{
		m_rotationSpeed = 0.24f;
	}
	// �v���C���[�ւ̃x�N�g�����v�Z
	Vector3 toPlayerVector = playerPos - pos;
	if (toPlayerVector.LengthSquared() > 0.0f) {
		toPlayerVector.Normalize();
	}

	// ���݂̑O���x�N�g�����擾
	Vector3 forward = Vector3::Transform(Vector3::Forward, m_rotation);
	if (forward.LengthSquared() > 0.0f) {
		forward.Normalize();
	}

	// ���ς��g���Ċp�x���v�Z
	float dot = toPlayerVector.Dot(forward);
	dot = clamp(dot, -1.0f, 1.0f); // acos�̈������͈͊O�ɂȂ�̂�h������
	float angle = std::acos(dot);

	// �O�ς��g���ĉ�]����������
	Vector3 cross = toPlayerVector.Cross(forward);
	if (cross.y < 0) {
		angle = -angle;
	}

	// �v���C���[�̕����Ɍ������߂̉�]���v�Z
	Quaternion toPlayerRotation = Quaternion::CreateFromAxisAngle(Vector3::Up, angle);

	// �ڕW�p�x�ƌ��݂̊p�x�����ɏ������ꍇ�A�U����h�����߂ɕ�Ԃ��~�߂�
	const float angleThreshold = 0.1f; // 臒l�̒���
	if (std::abs(angle) > angleThreshold) {
		m_rotation = Quaternion::Slerp(m_rotation, toPlayerRotation * m_rotation, elapsedTime * (m_rotationSpeed));
		m_rotation.Normalize();
	}

	// �v���C���[�̕����Ɉړ�
	pos += toPlayerVector * m_velocity.Length() * elapsedTime * 2;

	// �G���v���C���[�̕����������Ă��邩���`�F�b�N
	const float directionThreshold = -0.93f; // ���ς�1�ɋ߂����ǂ�����臒l
	if (dot <= directionThreshold)
	{
		// �U���̃N�[���_�E���^�C�����Ǘ�
		m_attackCooldown -= elapsedTime;
		if (m_attackCooldown <= 0.0f)
		{
			// �e�𔭎�
			m_attackCooldown = 3.0f; // ���̍U���܂ł̃N�[���_�E���^�C����3�b�ɐݒ�
		}
	}
	m_enemy->SetRotation(m_rotation);
	m_enemy->SetVelocity(m_velocity);
}
