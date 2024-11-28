/*
	@file	BossIdling.cpp
	@brief	ボス徘徊クラス
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
// コンストラクタ
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
// デストラクタ
BossIdling::~BossIdling() {}
// 初期化する
void BossIdling::Initialize()
{
	m_rotation = m_pBoss->GetRotation();
	m_velocity = m_pBoss->GetVelocity();
	m_scale = m_pBoss->GetScale();
	m_initialPosition = m_pBoss->GetPosition();
	m_rotationSpeed = 0.5f; // 回転速度
}

void BossIdling::Update(float elapsedTime, DirectX::SimpleMath::Vector3& pos, DirectX::SimpleMath::Vector3& playerPos, bool isHitToPlayer)
{
	UNREFERENCED_PARAMETER(isHitToPlayer);
	UNREFERENCED_PARAMETER(playerPos);
	using namespace DirectX::SimpleMath;
	// sin波を使った回転の更新
	float rotationAmplitude = 1.0f;  // 回転振幅
	float rotationFrequency = 1.0f;  // 回転周波数
	float randomMultiplier = GenerateRandomMultiplier(RANDOM_MIN, RANDOM_MAX);  // ランダムな倍率を生成
	float sinRotationSpeed = m_rotationSpeed + rotationAmplitude * std::sin(rotationFrequency * m_time);  // 回転速度をsin波で変化させる
	Quaternion deltaRotation = Quaternion::CreateFromAxisAngle(Vector3::Up, sinRotationSpeed * randomMultiplier * elapsedTime);  // 生成した回転速度に基づき、上方向（Y軸）を中心に回転を生成
	m_rotation *= deltaRotation;  // 既存の回転に新しい回転を適用
	m_rotation.Normalize();  // 回転を正規化し、安定したクォータニオンを維持
	// 向いている方向に基づいてX座標とZ座標を移動
	float moveCorrect = GenerateRandomMultiplier(10.0f, 10.0f);
	Vector3 forward = Vector3::Transform(Vector3::Backward * moveCorrect, m_rotation);
	pos += forward * (m_velocity.Length() * 1.5f) * elapsedTime;
	m_pBoss->SetRotation(m_rotation);
	m_pBoss->SetVelocity(m_velocity);
}

