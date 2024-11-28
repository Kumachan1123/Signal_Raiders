/*
	@file	BossIdling.cpp
	@brief	�{�X�p�j�N���X
*/
#include "pch.h"
#include <SimpleMath.h>
#include "BossIdling.h"
#include "Game/Enemy/Boss/BossAI/BossAI.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/InputManager.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include "Game/KumachiLib/KumachiLib.h"
#include <cassert>
#include <random>  

using namespace DirectX::SimpleMath;
// �R���X�g���N�^
BossIdling::BossIdling(BossAI* pBoss)
	: m_pBoss(pBoss),
	m_rotation(Quaternion::Identity),
	m_velocity(Vector3::Zero),
	m_scale(Vector3::One),
	m_initialPosition(Vector3::Zero),
	m_time(0.0f),
	m_rotationSpeed(0.0f)
{
}
// �f�X�g���N�^
BossIdling::~BossIdling() {}
// ����������
void BossIdling::Initialize()
{
	m_rotation = m_pBoss->GetRotation();
	m_velocity = m_pBoss->GetVelocity();
	m_scale = m_pBoss->GetScale();
	m_initialPosition = m_pBoss->GetPosition();
	m_rotationSpeed = 0.5f; // ��]���x
}

void BossIdling::Update(float elapsedTime, DirectX::SimpleMath::Vector3& pos, DirectX::SimpleMath::Vector3& playerPos, bool isHitToPlayer)
{
	UNREFERENCED_PARAMETER(isHitToPlayer);
	UNREFERENCED_PARAMETER(playerPos);
	using namespace DirectX::SimpleMath;
	// sin�g���g������]�̍X�V
	float rotationAmplitude = 1.0f;  // ��]�U��
	float rotationFrequency = 1.0f;  // ��]���g��
	float randomMultiplier = GenerateRandomMultiplier(RANDOM_MIN, RANDOM_MAX);  // �����_���Ȕ{���𐶐�
	float sinRotationSpeed = m_rotationSpeed + rotationAmplitude * std::sin(rotationFrequency * m_time);  // ��]���x��sin�g�ŕω�������
	Quaternion deltaRotation = Quaternion::CreateFromAxisAngle(Vector3::Up, sinRotationSpeed * randomMultiplier * elapsedTime);  // ����������]���x�Ɋ�Â��A������iY���j�𒆐S�ɉ�]�𐶐�
	m_rotation *= deltaRotation;  // �����̉�]�ɐV������]��K�p
	m_rotation.Normalize();  // ��]�𐳋K�����A���肵���N�H�[�^�j�I�����ێ�
	// �����Ă�������Ɋ�Â���X���W��Z���W���ړ�
	float moveCorrect = GenerateRandomMultiplier(10.0f, 10.0f);
	Vector3 forward = Vector3::Transform(Vector3::Backward * moveCorrect, m_rotation);
	pos += forward * (m_velocity.Length() * 1.5f) * elapsedTime;
	m_pBoss->SetRotation(m_rotation);
	m_pBoss->SetVelocity(m_velocity);
}

