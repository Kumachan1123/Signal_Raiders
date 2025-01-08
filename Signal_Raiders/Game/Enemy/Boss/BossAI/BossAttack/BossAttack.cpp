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
	m_position = m_pBoss->GetPosition();// 位置
	m_rotation = m_pBoss->GetRotation();// 回転
	m_velocity = m_pBoss->GetVelocity();// 速度
	m_scale = m_pBoss->GetScale();// スケール
}


// プレイヤーに向かって回転する
void BossAttack::RotateTowardsPlayer(DirectX::SimpleMath::Vector3 playerPos)
{
	m_rotation = DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(CalculateAngle(m_position, playerPos), 0.0f, 0.0f);
}

// ボスの位置をプレイヤー方向に更新
void BossAttack::MoveTowardsPlayer(float elapsedTime, DirectX::SimpleMath::Vector3 playerPos)
{
	m_position += Seek(m_pBoss->GetPosition(), playerPos, elapsedTime * 2);
}

// クールダウンの管理
void BossAttack::ManageAttackCooldown(float elapsedTime)
{
	m_attackCooldown -= elapsedTime;
	if (m_attackCooldown <= 0.250f)
	{
		m_pBoss->SetState(IState::EnemyState::ANGRY);
		if (m_attackCooldown <= 0.0f)// クールダウンが0を下回ったら
		{
			m_attackCooldown = ATTACK_INTERVAL;  // クールダウンリセット
			m_pBoss->SetState(IState::EnemyState::ATTACK);  // 攻撃状態に遷移
		}
	}
}

// 更新関数
void BossAttack::Update(float elapsedTime)
{
	// プレイヤーの方向に回転
	RotateTowardsPlayer(m_pBoss->GetEnemy()->GetPlayer()->GetPlayerPos());

	// プレイヤーの方向に移動
	MoveTowardsPlayer(elapsedTime, m_pBoss->GetEnemy()->GetPlayer()->GetPlayerPos());

	// クールダウンの更新
	ManageAttackCooldown(elapsedTime);

	// ボスの状態を更新
	m_pBoss->SetRotation(m_rotation);
	m_pBoss->SetVelocity(m_velocity);
	m_pBoss->SetPosition(m_position);
}
