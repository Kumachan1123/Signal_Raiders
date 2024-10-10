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
	, m_attackCooldown{ 3.0f }
	, m_rotationSpeed{}
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
	m_rotationSpeed = 1; // ��]���x
}

void BossAttack::Update(float elapsedTime, DirectX::SimpleMath::Vector3& pos, DirectX::SimpleMath::Vector3& playerPos, bool isHitToPlayer)
{
	UNREFERENCED_PARAMETER(isHitToPlayer);
	using namespace DirectX::SimpleMath;
	m_rotationSpeed -= (elapsedTime / 10);// ��]���x������
	if (m_rotationSpeed <= 0.24f)// ��]���x��0.24�ȉ��ɂȂ�����
	{
		m_rotationSpeed = 0.24f;// ��]���x��0.24�ɐݒ�
	}
	// �v���C���[�ւ̃x�N�g�����v�Z
	Vector3 toPlayerVector = playerPos - pos;
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
	dot = clamp(dot, -1.0f, 1.0f); // acos�̈������͈͊O�ɂȂ�̂�h������
	float angle = std::acos(dot);
	// �O�ς��g���ĉ�]����������
	Vector3 cross = toPlayerVector.Cross(forward);
	if (cross.y < 0)angle = -angle;// �v���C���[�̕����Ɍ������߂̉�]���v�Z
	// �v���C���[�̕����Ɍ������߂̉�]���v�Z
	Quaternion toPlayerRotation = Quaternion::CreateFromAxisAngle(Vector3::Up, angle);

	// �ڕW�p�x�ƌ��݂̊p�x�����ɏ������ꍇ�A�U����h�����߂ɕ�Ԃ��~�߂�
	const float angleThreshold = 0.1f; // 臒l�̒���
	if (std::abs(angle) > angleThreshold)
	{
		m_rotation = Quaternion::Slerp(m_rotation, toPlayerRotation * m_rotation, elapsedTime * (m_rotationSpeed));
		m_rotation.Normalize();
	}

	// �v���C���[�̕����Ɉړ�
	pos += toPlayerVector * (m_velocity.Length() * 5.0f) * elapsedTime;

	// �U���̃N�[���_�E���^�C�����Ǘ�
	m_attackCooldown -= elapsedTime;
	if (m_attackCooldown <= 2.0f)
	{
		m_pBoss->SetState(IState::EnemyState::ANGRY);
		// ���̍U���܂ł̃N�[���_�E���^�C����3�b�ɐݒ�
		if (m_attackCooldown <= 0.0f)m_attackCooldown = 3.0f;// �e�𔭎�

	}

	m_pBoss->SetRotation(m_rotation);
	m_pBoss->SetVelocity(m_velocity);
}
