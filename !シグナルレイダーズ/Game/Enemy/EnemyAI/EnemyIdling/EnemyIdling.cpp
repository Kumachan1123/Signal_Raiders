/*
	@file	EnemyIdling.cpp
	@brief	�GIdling�N���X
	�쐬�ҁF���܂�
*/
#include "pch.h"
#include <SimpleMath.h>
#include "Game/Enemy/EnemyAI/EnemyIdling/EnemyIdling.h"
#include "Game/Enemy/EnemyAI/EnemyAI.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/InputManager.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include <cassert>
#include <random>  

using namespace DirectX::SimpleMath;
// �R���X�g���N�^
EnemyIdling::EnemyIdling(EnemyAI* enemy) : m_enemy(enemy)
{
}
// �f�X�g���N�^
EnemyIdling::~EnemyIdling() {}
// ����������
void EnemyIdling::Initialize()
{
	m_rotation = m_enemy->GetRotation();
	m_velocity = m_enemy->GetVelocity();
	m_scale = m_enemy->GetScale();
	m_initialPosition = m_enemy->GetPosition();
	m_rotationSpeed = 0.5f; // ��]���x
}

void EnemyIdling::Update(float elapsedTime, DirectX::SimpleMath::Vector3& pos, DirectX::SimpleMath::Vector3& playerPos, bool isHitToPlayer)
{
	using namespace DirectX::SimpleMath;
	// sin�g���g������]�̍X�V
	float rotationAmplitude = 1.0f;  // ��]�U��
	float rotationFrequency = 0.9f;  // ��]���g��
	float randomMultiplier = m_enemy->GenerateRandomMultiplier(RANDOM_MIN, RANDOM_MAX);  // �����_���Ȕ{���𐶐�
	float sinRotationSpeed = m_rotationSpeed + rotationAmplitude * std::sin(rotationFrequency * m_time);  // ��]���x��sin�g�ŕω�������
	Quaternion deltaRotation = Quaternion::CreateFromAxisAngle(Vector3::Up, sinRotationSpeed * randomMultiplier * elapsedTime);  // ����������]���x�Ɋ�Â��A������iY���j�𒆐S�ɉ�]�𐶐�
	m_rotation *= deltaRotation;  // �����̉�]�ɐV������]��K�p
	m_rotation.Normalize();  // ��]�𐳋K�����A���肵���N�H�[�^�j�I�����ێ�
	// �����Ă�������Ɋ�Â���X���W��Z���W���ړ�
	float moveCorrect = m_enemy->GenerateRandomMultiplier(5.0f, 10.0f);
	Vector3 forward = Vector3::Transform(Vector3::Backward * moveCorrect, m_rotation);
	pos += forward * m_velocity.Length() * elapsedTime;
	m_enemy->SetRotation(m_rotation);
	m_enemy->SetVelocity(m_velocity);
}

