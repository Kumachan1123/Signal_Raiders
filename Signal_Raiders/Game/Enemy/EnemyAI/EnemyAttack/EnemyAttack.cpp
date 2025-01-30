/*
	@file	EnemyAttack.cpp
	@brief	�G�U���N���X
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
#include "Game/KumachiLib/KumachiLib.h"
#include "Game/Enemy/Parameters/EnemyParameters.h"
using namespace DirectX::SimpleMath;
// �R���X�g���N�^
EnemyAttack::EnemyAttack(EnemyAI* enemy)
	: m_enemy(enemy)
	, m_attackCooldown{ EnemyParameters::ATTACK_COOLDOWN }
	, m_rotationSpeed{}
	, m_commonResources{}
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
	m_rotationSpeed = EnemyParameters::INITIAL_ROTATION_SPEED; // ��]���x
}

void EnemyAttack::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;
	m_rotationSpeed -= (elapsedTime / EnemyParameters::ROTATION_SPEED_DIVISOR);// ��]���x������
	if (m_rotationSpeed <= EnemyParameters::ROTATION_SPEED_MIN)// ��]���x����ȉ��ɂȂ�����
	{
		m_rotationSpeed = EnemyParameters::ROTATION_SPEED_MIN;// ��]���x��ݒ�
	}
	// �v���C���[�ւ̃x�N�g�����v�Z
	Vector3 toPlayerVector = m_enemy->GetEnemy()->GetPlayer()->GetPlayerPos() - m_enemy->GetPosition();
	if (toPlayerVector.LengthSquared() > 0.0f)// �v���C���[�ւ̃x�N�g����0���傫���ꍇ
	{
		toPlayerVector.Normalize();// �v���C���[�ւ̃x�N�g���𐳋K��
	}
	// ���݂̑O���x�N�g�����擾
	Vector3 forward = Vector3::Transform(Vector3::Forward, m_rotation);
	if (forward.LengthSquared() > 0.0f)// ���݂̑O���x�N�g����0���傫���ꍇ
	{
		forward.Normalize();// ���݂̑O���x�N�g���𐳋K��
	}
	// ���ς��g���Ċp�x���v�Z
	float dot = toPlayerVector.Dot(forward);
	dot = Clamp(dot, EnemyParameters::DOT_CLAMP.min, EnemyParameters::DOT_CLAMP.max); // acos�̈������͈͊O�ɂȂ�̂�h������
	float angle = std::acos(dot);
	// �O�ς��g���ĉ�]����������
	Vector3 cross = toPlayerVector.Cross(forward);
	if (cross.y < 0)angle = -angle;// �v���C���[�̕����Ɍ������߂̉�]���v�Z
	// �v���C���[�̕����Ɍ������߂̉�]���v�Z
	Quaternion toPlayerRotation = Quaternion::CreateFromAxisAngle(Vector3::Up, angle);

	// �ڕW�p�x�ƌ��݂̊p�x�����ɏ������ꍇ�A�U����h�����߂ɕ�Ԃ��~�߂�
	if (std::abs(angle) > EnemyParameters::ANGLE_THRESHOLD)
	{
		m_rotation = Quaternion::Slerp(m_rotation, toPlayerRotation * m_rotation, elapsedTime * (m_rotationSpeed));
		m_rotation.Normalize();
	}

	// �v���C���[�̕����Ɉړ�
	m_enemy->SetPosition(m_enemy->GetPosition() + toPlayerVector * (m_velocity.Length() * EnemyParameters::VELOCITY_SCALE_FACTOR) * elapsedTime);

	// �U���̃N�[���_�E���^�C�����Ǘ�
	m_attackCooldown -= elapsedTime;
	if (m_attackCooldown <= EnemyParameters::CHANGE_FACE_TIME)// �U���̃N�[���_�E���^�C����2.0�ȉ��ɂȂ�����
	{
		m_enemy->SetState(IState::EnemyState::ANGRY);// �{��Ԑ��ɂ���
		// ���̍U���܂ł̃N�[���_�E���^�C����3�b�ɐݒ�
		if (m_attackCooldown <= 0.0f)m_attackCooldown = EnemyParameters::ATTACK_COOLDOWN;// �e�𔭎�

	}
	// �G�̏����X�V
	m_enemy->SetRotation(m_rotation);
	m_enemy->SetVelocity(m_velocity);
}
