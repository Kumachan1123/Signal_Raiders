/*
	@file	EnemyAI.cpp
	@brief	敵AIクラス
*/
#include "pch.h"
#include <SimpleMath.h>
#include "Game/Enemy/EnemyAI/EnemyAI.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/InputManager.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include <cassert>
#include <random>
#include <type_traits> // std::enable_if, std::is_integral
#include "Game/KumachiLib/KumachiLib.h"
class EnemyAttack;
class EnemyIdling;
// コンストラクタ
EnemyAI::EnemyAI(IEnemy* pEnemy)
	: m_currentState(nullptr), m_rotationSpeed(0.5f), m_attackCooldown(0.0f), m_pEnemyAttack(nullptr)
	, m_pEnemySpin(nullptr), m_pEnemyIdling(nullptr), m_canAttack(true), m_enemyState(IState::EnemyState::IDLING), m_pEnemy(pEnemy)
{
	m_pEnemy = pEnemy;
	m_pEnemyAttack = std::make_unique<EnemyAttack>(this);
	m_pEnemyIdling = std::make_unique<EnemyIdling>(this);
	m_pEnemySpin = std::make_unique<EnemySpin>(this);
}
// デストラクタ
EnemyAI::~EnemyAI() {}

// 初期化する
void EnemyAI::Initialize()
{
	using namespace DirectX::SimpleMath;
	m_rotation = (GenerateRandomMultiplier(0, 2) <= 1.0f) ? Quaternion::Identity : -Quaternion::Identity;// 回転の初期化
	m_initialPosition = Vector3::Zero;  // 初期位置を初期化
	m_initialPosition.y = GenerateRandomMultiplier(RANDOM_MIN, RANDOM_MAX); // 初期位置のY座標を設定
	m_rotation.y = GenerateRandomMultiplier(RANDOM_MIN, RANDOM_MAX); // Y軸の回転を設定
	m_velocity = Vector3(0.0f, 0.5f, 0.0f); // 浮遊の初期速度
	m_scale = Vector3::One; // スケール初期化
	m_time = 0.0f;  // 時間の初期化
	m_position = m_initialPosition;
	m_currentState = m_pEnemyIdling.get();
	m_currentState->Initialize();
	m_enemyState = IState::EnemyState::IDLING;// 待機態勢
}
// 更新
void EnemyAI::Update(float elapsedTime, DirectX::SimpleMath::Vector3& pos, DirectX::SimpleMath::Vector3& playerPos, bool& isHitToPlayer, bool& isHitToPlayerBullet)
{
	using namespace DirectX::SimpleMath;
	if (isHitToPlayerBullet)m_isHitPlayerBullet = true;
	// sin波を用いた浮遊動作の実装
	m_time += elapsedTime;
	// sin波を用いた浮遊動作の実装
	float amplitude = 2.0f;  // 振幅
	float frequency = 0.5f;  // 周波数
	// 敵をふわふわ浮遊させる
	pos.y = m_initialPosition.y + amplitude * std::sin(frequency * m_time);
	pos.y += m_velocity.y * elapsedTime;
	// 敵がプレイヤーの一定範囲内に入っている場合
	if ((isHitToPlayer || m_isHitPlayerBullet))
	{
		ChangeState(m_pEnemyAttack.get());//攻撃態勢にする
		m_enemyState = IState::EnemyState::ATTACK;// 徘徊態勢
	}
	else
	{
		ChangeState(m_pEnemyIdling.get());//徘徊態勢にする
		m_enemyState = IState::EnemyState::IDLING;// 徘徊態勢
	}
	// プレイヤーの弾に当たった場合
	if (isHitToPlayerBullet)
	{
		KnockBack(elapsedTime, pos, isHitToPlayerBullet, playerPos);
		ChangeState(m_pEnemySpin.get());//逃避態勢にする
		m_enemyState = IState::EnemyState::HIT;// 逃避態勢
	}
	m_currentState->Update(elapsedTime, pos, playerPos, isHitToPlayer);

	m_position = pos;
}
// ステート変更
void EnemyAI::ChangeState(IState* newState)
{
	if (m_currentState != newState)
	{
		m_currentState = newState;
		m_currentState->Initialize();
	}
}


// ノックバック処理
void EnemyAI::KnockBack(float elapsedTime, DirectX::SimpleMath::Vector3& pos, bool& isHitToPlayerBullet, const DirectX::SimpleMath::Vector3& playerPos)
{
	using namespace DirectX::SimpleMath;

	// ノックバックが始まったばかりなら初期設定を行う
	if (m_knockTime == 0.0f)
	{
		m_knockStartPosition = pos; // ノックバック開始位置
		Vector3 knockBackDirection = (pos - playerPos); // プレイヤーから敵への方向ベクトル
		knockBackDirection.Normalize(); // 正規化して方向ベクトルにする
		m_knockEndPosition = pos + knockBackDirection; // ノックバック終了位置
		m_initialVelocity = knockBackDirection * 20; // 初期速度
		m_pEnemy->SetCanAttack(false);// 攻撃不可能にする
	}

	// ノックバック時間の更新
	m_knockTime += elapsedTime;

	// ノックバックの長さ（秒）
	const float knockBackDuration = 2.0f;

	// ノックバックの進行度
	float t = std::min(m_knockTime / knockBackDuration, 2.0f);

	// 減衰係数の計算（指数関数的減衰）
	float decayFactor = std::exp(-3.0f * t); // 減衰速度を調整するために指数のベースを調整

	// 減衰した速度を使って位置を更新
	Vector3 velocity = m_initialVelocity * decayFactor;
	pos += velocity * elapsedTime;
	if (GetState() != IState::EnemyState::ANGRY)// 怒り態勢でない場合
		SetState(IState::EnemyState::HIT);// ダメージ態勢にする
	// ノックバックが終了したかどうかチェック
	if (t >= 0.5f)
	{
		m_pEnemy->SetCanAttack(true);// 攻撃可能にする
	}
	if (t >= 1.0f)
	{
		m_knockEndPosition = pos;
		m_knockTime = 0.0f; // ノックバック時間のリセット
		isHitToPlayerBullet = false; // ノックバック終了
		SetState(IState::EnemyState::IDLING);// 待機態勢

	}
}
