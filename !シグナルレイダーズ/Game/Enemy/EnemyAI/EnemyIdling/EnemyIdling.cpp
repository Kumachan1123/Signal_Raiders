/*
	@file	EnemyIdling.cpp
	@brief	敵Idlingクラス
	作成者：くまち
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
// コンストラクタ
EnemyIdling::EnemyIdling(EnemyAI* enemy) : m_enemy(enemy)
{
}
// デストラクタ
EnemyIdling::~EnemyIdling() {}
// 初期化する
void EnemyIdling::Initialize()
{
	m_rotation = m_enemy->GetRotation();
	m_velocity = m_enemy->GetVelocity();
	m_scale = m_enemy->GetScale();
	m_initialPosition = m_enemy->GetPosition();
	m_rotationSpeed = 0.5f; // 回転速度
}

void EnemyIdling::Update(float elapsedTime, DirectX::SimpleMath::Vector3& pos, DirectX::SimpleMath::Vector3& playerPos, bool isHitToPlayer)
{
	using namespace DirectX::SimpleMath;
	// sin波を使った回転の更新
	float rotationAmplitude = 1.0f;  // 回転振幅
	float rotationFrequency = 0.9f;  // 回転周波数
	float randomMultiplier = m_enemy->GenerateRandomMultiplier(RANDOM_MIN, RANDOM_MAX);  // ランダムな倍率を生成
	float sinRotationSpeed = m_rotationSpeed + rotationAmplitude * std::sin(rotationFrequency * m_time);  // 回転速度をsin波で変化させる
	Quaternion deltaRotation = Quaternion::CreateFromAxisAngle(Vector3::Up, sinRotationSpeed * randomMultiplier * elapsedTime);  // 生成した回転速度に基づき、上方向（Y軸）を中心に回転を生成
	m_rotation *= deltaRotation;  // 既存の回転に新しい回転を適用
	m_rotation.Normalize();  // 回転を正規化し、安定したクォータニオンを維持
	// 向いている方向に基づいてX座標とZ座標を移動
	float moveCorrect = m_enemy->GenerateRandomMultiplier(5.0f, 10.0f);
	Vector3 forward = Vector3::Transform(Vector3::Backward * moveCorrect, m_rotation);
	pos += forward * m_velocity.Length() * elapsedTime;
	m_enemy->SetRotation(m_rotation);
	m_enemy->SetVelocity(m_velocity);
}

