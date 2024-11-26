/*
	@file	BossAttack.cpp
	@brief	ボス攻撃クラス
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
#include "Game/KumachiLib/KumachiLib.h"
#include <cmath>

using namespace DirectX::SimpleMath;

// コンストラクタ
BossAttack::BossAttack(BossAI* pBoss)
	: m_pBoss(pBoss)
	, m_attackCooldown{  }
	, m_rotationSpeed{  }
	, m_commonResources{}
{
	m_attackCooldown = ATTACK_INTERVAL;
	m_rotationSpeed = ROTATION_SPEED;
}

// デストラクタ
BossAttack::~BossAttack() {}

// 初期化する
void BossAttack::Initialize()
{
	m_rotation = m_pBoss->GetRotation();// 回転
	m_velocity = m_pBoss->GetVelocity();// 速度
	m_scale = m_pBoss->GetScale();// スケール
}

// プレイヤー方向のベクトルを計算する
DirectX::SimpleMath::Vector3 BossAttack::CalculateToPlayerVector(const Vector3& pos, const Vector3& playerPos)
{
	Vector3 toPlayerVector = playerPos - pos;
	if (toPlayerVector.LengthSquared() > 0.0f)
	{
		toPlayerVector.Normalize();
	}
	return toPlayerVector;
}

// プレイヤーに向かって回転する
void BossAttack::RotateTowardsPlayer(float elapsedTime, const Vector3& toPlayerVector)
{
	// 現在の前方ベクトルを取得
	Vector3 forward = Vector3::Transform(Vector3::Forward, m_rotation);
	if (forward.LengthSquared() > 0.0f)
	{
		forward.Normalize();
	}

	// atan2を使って角度を計算
	float dot = Clamp(toPlayerVector.Dot(forward), -1.0f, 1.0f);
	float angle = std::acos(dot);

	// 回転処理
	if (dot < 0.999f)  // プレイヤーの方向を向いていない場合のみ回転
	{
		Vector3 cross = toPlayerVector.Cross(forward);
		if (cross.y < 0) angle = -angle;

		Quaternion toPlayerRotation = Quaternion::CreateFromAxisAngle(Vector3::Up, angle);
		m_rotation = Quaternion::Slerp(m_rotation, toPlayerRotation * m_rotation, elapsedTime * m_rotationSpeed);
		m_rotation.Normalize();
	}
}

// ボスの位置をプレイヤー方向に更新
void BossAttack::MoveTowardsPlayer(float elapsedTime, const Vector3& toPlayerVector, Vector3& pos)
{
	float moveSpeed = m_velocity.Length() * 2.0f;
	pos += toPlayerVector * moveSpeed * elapsedTime;
}

// クールダウンの管理
void BossAttack::ManageAttackCooldown(float elapsedTime)
{
	m_attackCooldown -= elapsedTime;
	if (m_attackCooldown <= 0.250f)
	{
		m_pBoss->SetState(IState::EnemyState::ANGRY);
		if (m_attackCooldown <= 0.0f)
		{
			m_attackCooldown = ATTACK_INTERVAL;  // クールダウンリセット
		}
	}
}

// 更新関数
void BossAttack::Update(float elapsedTime, DirectX::SimpleMath::Vector3& pos, DirectX::SimpleMath::Vector3& playerPos, bool isHitToPlayer)
{
	UNREFERENCED_PARAMETER(isHitToPlayer);

	// プレイヤーへの方向を計算
	Vector3 toPlayerVector = CalculateToPlayerVector(pos, playerPos);

	// プレイヤーの方向に回転
	RotateTowardsPlayer(elapsedTime, toPlayerVector);

	// プレイヤーの方向に移動
	MoveTowardsPlayer(elapsedTime, toPlayerVector, pos);

	// クールダウンの更新
	ManageAttackCooldown(elapsedTime);

	// 回転速度を減速し、最小値を確保
	m_rotationSpeed = std::max(m_rotationSpeed - 0.05f, 0.1f);

	// ボスの状態を更新
	m_pBoss->SetRotation(m_rotation);
	m_pBoss->SetVelocity(m_velocity);
}
