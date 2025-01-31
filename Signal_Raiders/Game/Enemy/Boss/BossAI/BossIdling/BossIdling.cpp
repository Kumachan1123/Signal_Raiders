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
// デストラクタ
BossIdling::~BossIdling() {}
// 初期化する
void BossIdling::Initialize()
{
	m_rotation = m_pBoss->GetRotation();
	m_velocity = m_pBoss->GetVelocity();
	m_scale = m_pBoss->GetScale();
	m_initialPosition = m_pBoss->GetPosition();
	m_rotationSpeed = EnemyParameters::INITIAL_ROTATION_SPEED; // 回転速度
}

void BossIdling::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;
	// sin波を使った回転の更新
	m_time += elapsedTime;

	m_angle = CalculateAngle(m_pBoss->GetPosition(), m_pBoss->GetEnemy()->GetPlayer()->GetPlayerPos());// プレイヤーの方向を取得し、正面を向かせる
	m_angle = Lerp(m_angle, CalculateAngle(m_pBoss->GetEnemy()->GetPlayer()->GetPlayerPos(), m_pBoss->GetPosition()), m_time);// プレイヤーの方向を取得し、一回転させる
	m_rotation = Quaternion::CreateFromYawPitchRoll(m_angle, 0.0f, 0.0f);

	m_pBoss->SetRotation(m_rotation);
	m_pBoss->SetVelocity(m_velocity);
}

