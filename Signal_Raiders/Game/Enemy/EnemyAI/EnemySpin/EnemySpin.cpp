/*
	@file	EnemySpin.cpp
	@brief	�G�X�s���N���X
*/
#include "pch.h"
#include <SimpleMath.h>
#include "Game/Enemy/EnemyAI/EnemySpin/EnemySpin.h"
#include "Game/Enemy/EnemyAI/EnemyAI.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/InputManager.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include <cassert>
#include <random>  
#include "Game/KumachiLib/KumachiLib.h"

using namespace DirectX::SimpleMath;
// �R���X�g���N�^
EnemySpin::EnemySpin(EnemyAI* enemy)
	: m_enemy(enemy),
	m_rotation(Quaternion::Identity),
	m_velocity(Vector3::Zero),
	m_scale(Vector3::One),
	m_initialPosition(Vector3::Zero),
	m_time(0.0f),
	m_angle(0.0f)
{
}
// �f�X�g���N�^
EnemySpin::~EnemySpin() {}
// ����������
void EnemySpin::Initialize()
{
	m_rotation = m_enemy->GetRotation();
	m_velocity = m_enemy->GetVelocity();
	m_scale = m_enemy->GetScale();
	m_initialPosition = m_enemy->GetPosition();
}

void EnemySpin::Update(float elapsedTime)
{
	m_time += elapsedTime;

	m_angle = CalculateAngle(m_enemy->GetPosition(), m_enemy->GetEnemy()->GetPlayer()->GetPlayerPos());// �v���C���[�̕������擾���A���ʂ���������
	m_angle = Lerp(m_angle, CalculateAngle(m_enemy->GetEnemy()->GetPlayer()->GetPlayerPos(), m_enemy->GetPosition()), m_time);// �v���C���[�̕������擾���A���]������
	// �G����]������
	m_rotation = Quaternion::CreateFromYawPitchRoll(m_angle, 0.0f, 0.0f);
	m_enemy->SetRotation(m_rotation);
	m_enemy->SetVelocity(m_velocity);
}


