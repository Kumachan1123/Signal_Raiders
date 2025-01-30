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
#include "Game/Enemy/Parameters/EnemyParameters.h"
// コンストラクタ
EnemyAI::EnemyAI(IEnemy* pEnemy)
	: m_currentState(nullptr), m_attackCooldown(0.0f), m_pEnemyAttack(nullptr), m_time(0.0f), m_isHitPlayerBullet(false), m_knockTime(0.0f)
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
	m_initialPosition = Vector3::Zero;  // 初期位置を初期化
	m_initialPosition.y = GenerateRandomMultiplier(EnemyParameters::RANDOM_MIN, EnemyParameters::RANDOM_MAX); // 初期位置のY座標を設定
	m_rotation.y = GenerateRandomMultiplier(EnemyParameters::RANDOM_MIN, EnemyParameters::RANDOM_MAX); // Y軸の回転を設定
	m_velocity = EnemyParameters::INITIAL_VELOCITY; // 浮遊の初期速度
	m_scale = Vector3::One; // スケール初期化
	m_position = m_initialPosition;
	m_currentState = m_pEnemyIdling.get();
	m_currentState->Initialize();
	m_enemyState = IState::EnemyState::IDLING;// 待機態勢
}
// 更新
void EnemyAI::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;
	m_position = m_pEnemy->GetPosition();// 位置を取得
	if (m_pEnemy->GetEnemyHitByPlayerBullet())m_isHitPlayerBullet = true;// プレイヤーの弾に当たったかどうかを取得
	// sin波を用いた浮遊動作の実装
	m_time += elapsedTime;
	// 敵をふわふわ浮遊させる
	m_position.y = m_initialPosition.y + EnemyParameters::AMPLITUDE * std::sin(EnemyParameters::FREQUENCY * m_time);
	m_position.y += m_velocity.y * elapsedTime;
	// 敵がプレイヤーの一定範囲内に入っている場合
	if ((m_pEnemy->GetHitToPlayer() || m_isHitPlayerBullet))
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
	if (m_pEnemy->GetEnemyHitByPlayerBullet())
	{
		KnockBack(elapsedTime);
		ChangeState(m_pEnemySpin.get());//スピンする
		m_enemyState = IState::EnemyState::HIT;// 攻撃を食らった状態にする
	}
	m_currentState->Update(elapsedTime);
	m_pEnemy->SetPosition(m_position);
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
void EnemyAI::KnockBack(float elapsedTime)
{
	using namespace DirectX::SimpleMath;

	// ノックバックが始まったばかりなら初期設定を行う
	if (m_knockTime == 0.0f)
	{
		m_knockStartPosition = m_position; // ノックバック開始位置
		Vector3 knockBackDirection = (m_position - m_pEnemy->GetPlayer()->GetPlayerPos()); // プレイヤーから敵への方向ベクトル
		knockBackDirection.Normalize(); // 正規化して方向ベクトルにする
		m_knockEndPosition = m_position + knockBackDirection; // ノックバック終了位置
		m_initialVelocity = knockBackDirection * EnemyParameters::FIXED_INITIAL_SPEED; // 初期速度
		m_pEnemy->SetCanAttack(false);// 攻撃不可能にする
	}

	// ノックバック時間の更新
	m_knockTime += elapsedTime;


	// ノックバックの進行度
	float Progression = std::min(m_knockTime / EnemyParameters::KNOCKBACK_DURATION, EnemyParameters::KNOCKBACK_PROGRESS_MAX);

	// 減衰係数の計算（指数関数的減衰）
	float decayFactor = std::exp(EnemyParameters::KNOCKBACK_DECAY_RATE * Progression); // 減衰速度を調整するために指数のベースを調整

	// 減衰した速度を使って位置を更新
	Vector3 velocity = m_initialVelocity * decayFactor;
	m_position += velocity * elapsedTime;
	if (GetState() != IState::EnemyState::ANGRY)// 怒り態勢でない場合
		SetState(IState::EnemyState::HIT);// 攻撃を食らった状態にする
	// ノックバックが終了したかどうかチェック
	if (Progression >= EnemyParameters::KNOCKBACK_TIME.canAttackTime)
	{
		m_pEnemy->SetCanAttack(true);// 攻撃可能にする
	}
	if (Progression >= EnemyParameters::KNOCKBACK_TIME.endKnockTime)
	{
		m_knockEndPosition = m_position;
		m_knockTime = 0.0f; // ノックバック時間のリセット
		m_pEnemy->SetEnemyHitByPlayerBullet(false); // ノックバック終了
		SetState(IState::EnemyState::IDLING);// 待機態勢

	}
}
