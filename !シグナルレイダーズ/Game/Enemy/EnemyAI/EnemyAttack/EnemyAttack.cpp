/*
	@file	EnemyAttack.cpp
	@brief	敵攻撃クラス
	作成者：くまち
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
// コンストラクタ
EnemyAttack::EnemyAttack(EnemyAI* enemy)
	: m_enemy(enemy)
	, m_attackCooldown{ 3.0f }
	, m_rotationSpeed{}
{
}
// デストラクタ
EnemyAttack::~EnemyAttack() {}
// 初期化する
void EnemyAttack::Initialize()
{
	m_rotation = m_enemy->GetRotation();
	m_velocity = m_enemy->GetVelocity();
	m_scale = m_enemy->GetScale();
	m_rotationSpeed = 1; // 回転速度
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
	// プレイヤーへのベクトルを計算
	Vector3 toPlayerVector = playerPos - pos;
	if (toPlayerVector.LengthSquared() > 0.0f) {
		toPlayerVector.Normalize();
	}

	// 現在の前方ベクトルを取得
	Vector3 forward = Vector3::Transform(Vector3::Forward, m_rotation);
	if (forward.LengthSquared() > 0.0f) {
		forward.Normalize();
	}

	// 内積を使って角度を計算
	float dot = toPlayerVector.Dot(forward);
	dot = clamp(dot, -1.0f, 1.0f); // acosの引数が範囲外になるのを防ぐため
	float angle = std::acos(dot);

	// 外積を使って回転方向を決定
	Vector3 cross = toPlayerVector.Cross(forward);
	if (cross.y < 0) {
		angle = -angle;
	}

	// プレイヤーの方向に向くための回転を計算
	Quaternion toPlayerRotation = Quaternion::CreateFromAxisAngle(Vector3::Up, angle);

	// 目標角度と現在の角度が非常に小さい場合、振動を防ぐために補間を止める
	const float angleThreshold = 0.1f; // 閾値の調整
	if (std::abs(angle) > angleThreshold) {
		m_rotation = Quaternion::Slerp(m_rotation, toPlayerRotation * m_rotation, elapsedTime * (m_rotationSpeed));
		m_rotation.Normalize();
	}

	// プレイヤーの方向に移動
	pos += toPlayerVector * m_velocity.Length() * elapsedTime * 2;

	// 敵がプレイヤーの方向を向いているかをチェック
	const float directionThreshold = -0.93f; // 内積が1に近いかどうかの閾値
	if (dot <= directionThreshold)
	{
		// 攻撃のクールダウンタイムを管理
		m_attackCooldown -= elapsedTime;
		if (m_attackCooldown <= 0.0f)
		{
			// 弾を発射
			m_attackCooldown = 3.0f; // 次の攻撃までのクールダウンタイムを3秒に設定
		}
	}
	m_enemy->SetRotation(m_rotation);
	m_enemy->SetVelocity(m_velocity);
}
