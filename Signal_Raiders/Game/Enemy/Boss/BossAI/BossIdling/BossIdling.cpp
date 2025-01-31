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
	: m_pBoss(pBoss)
	, m_rotation(Quaternion::Identity)
	, m_velocity(Vector3::Zero)
	, m_scale(Vector3::One)
	, m_initialPosition(Vector3::Zero)
	, m_time(0.0f)
	, m_rotationSpeed(0.0f)
	, m_angle(0.0f)
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
	m_rotationSpeed = EnemyParameters::INITIAL_ROTATION_SPEED; // ��]���x
}

void BossIdling::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;
	// sin�g���g������]�̍X�V
	m_time += elapsedTime;

	m_angle = CalculateAngle(m_pBoss->GetPosition(), m_pBoss->GetEnemy()->GetPlayer()->GetPlayerPos());// �v���C���[�̕������擾���A���ʂ���������
	m_angle = Lerp(m_angle, CalculateAngle(m_pBoss->GetEnemy()->GetPlayer()->GetPlayerPos(), m_pBoss->GetPosition()), m_time);// �v���C���[�̕������擾���A���]������
	m_rotation = Quaternion::CreateFromYawPitchRoll(m_angle, 0.0f, 0.0f);

	m_pBoss->SetRotation(m_rotation);
	m_pBoss->SetVelocity(m_velocity);
}

