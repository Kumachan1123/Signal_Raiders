/*
	@file	BossAttack.cpp
	@brief	ボス攻撃クラス
	作成者：くまち
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

// コンストラクタ
BossAttack::BossAttack(BossAI* pBoss)
	: m_pBoss(pBoss)
	, m_attackCooldown{ 1.0f }
	, m_rotationSpeed{ .5f }
	, m_commonResources{}
{
}

// デストラクタ
BossAttack::~BossAttack() {}

// 初期化する
void BossAttack::Initialize()
{
	m_rotation = m_pBoss->GetRotation();
	m_velocity = m_pBoss->GetVelocity();
	m_scale = m_pBoss->GetScale();
}

void BossAttack::Update(float elapsedTime, DirectX::SimpleMath::Vector3& pos, DirectX::SimpleMath::Vector3& playerPos, bool isHitToPlayer)
{
	UNREFERENCED_PARAMETER(isHitToPlayer);

	// プレイヤーへのベクトルを計算
	Vector3 toPlayerVector = playerPos - pos;
	if (toPlayerVector.LengthSquared() > 0.0f)
	{
		toPlayerVector.Normalize();
	}

	// 現在の前方ベクトルを取得
	Vector3 forward = Vector3::Transform(Vector3::Forward, m_rotation);
	if (forward.LengthSquared() > 0.0f)
	{
		forward.Normalize();
	}

	// 内積を使って角度を計算
	float dot = toPlayerVector.Dot(forward);
	dot = clamp(dot, -1.0f, 1.0f);
	float angle = std::acos(dot);

	// プレイヤーの方向に向くための回転を計算
	if (dot > 0.99f)  // ほぼ向いたら回転処理をスキップする（内積が1に近いとき）
	{
		// すでにプレイヤーの方向を向いているので回転を止める
	}
	else
	{
		// 外積を使って回転方向を決定
		Vector3 cross = toPlayerVector.Cross(forward);
		if (cross.y < 0) angle = -angle;

		// プレイヤーの方向に向くための回転を計算
		Quaternion toPlayerRotation = Quaternion::CreateFromAxisAngle(Vector3::Up, angle);

		// 回転をスムーズに補間する
		m_rotation = Quaternion::Slerp(m_rotation, toPlayerRotation * m_rotation, elapsedTime * m_rotationSpeed);
		m_rotation.Normalize();
	}

	// プレイヤーの方向に移動
	float moveSpeed = m_velocity.Length() * 2.0f;
	pos += toPlayerVector * moveSpeed * elapsedTime;

	// 攻撃のクールダウンタイムを管理
	m_attackCooldown -= elapsedTime;
	if (m_attackCooldown <= .250f)
	{
		m_pBoss->SetState(IState::EnemyState::ANGRY);
		if (m_attackCooldown <= 0.0f) m_attackCooldown = 1.0f;
	}

	m_pBoss->SetRotation(m_rotation);
	m_pBoss->SetVelocity(m_velocity);
}