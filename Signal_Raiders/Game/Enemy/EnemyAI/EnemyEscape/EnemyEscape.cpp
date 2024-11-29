/*
	@file	EnemyEscape.cpp
	@brief	敵逃避クラス
*/
#include "pch.h"
#include <SimpleMath.h>
#include "Game/Enemy/EnemyAI/EnemyEscape/EnemyEscape.h"
#include "Game/Enemy/EnemyAI/EnemyAI.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/InputManager.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include <cassert>
#include <random>  
#include "Game/KumachiLib/KumachiLib.h"

using namespace DirectX::SimpleMath;
// コンストラクタ
EnemyEscape::EnemyEscape(EnemyAI* enemy)
	: m_enemy(enemy),
	m_rotation(Quaternion::Identity),
	m_velocity(Vector3::Zero),
	m_scale(Vector3::One),
	m_initialPosition(Vector3::Zero),
	m_time(0.0f),
	m_rotationSpeed(0.0f),
	m_angle(0.0f)
{
}
// デストラクタ
EnemyEscape::~EnemyEscape() {}
// 初期化する
void EnemyEscape::Initialize()
{
	m_rotation = m_enemy->GetRotation();
	m_velocity = m_enemy->GetVelocity();
	m_scale = m_enemy->GetScale();
	m_initialPosition = m_enemy->GetPosition();
	m_rotationSpeed = 0.5f; // 回転速度
}

void EnemyEscape::Update(float elapsedTime, DirectX::SimpleMath::Vector3& pos, DirectX::SimpleMath::Vector3& playerPos, bool isHitToPlayer)
{
	UNREFERENCED_PARAMETER(isHitToPlayer);
	//UNREFERENCED_PARAMETER(playerPos);
	m_time += elapsedTime;
	using namespace DirectX::SimpleMath;
	m_angle = CalculateAngle(pos, playerPos);
	m_angle = Lerp(m_angle, CalculateAngle(playerPos, pos), m_time);
	// 徐々にプレイヤーに背を向ける
	m_rotation = Quaternion::CreateFromYawPitchRoll(m_angle, 0.0f, 0.0f);
	m_enemy->SetRotation(m_rotation);
	m_enemy->SetVelocity(m_velocity);
}

