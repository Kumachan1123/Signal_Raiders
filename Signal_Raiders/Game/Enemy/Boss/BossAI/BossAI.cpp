/*
	@file	BossAI.cpp
	@brief	敵AIクラス
*/
#include "pch.h"
#include <SimpleMath.h>
#include "BossAI.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/InputManager.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include <cassert>
#include "Game/KumachiLib/KumachiLib.h"
#include <random>
#include <type_traits> // std::enable_if, std::is_integral
#include "Game/Enemy/Parameters/EnemyParameters.h"
// コンストラクタ
BossAI::BossAI(IEnemy* pBoss)
	: m_currentState(nullptr)
	, m_attackCooldown(0.0f)
	, m_knockTime(0.0f)
	, m_time(0.0f)
	, m_pBossAttack(nullptr)
	, m_enemyState(IState::EnemyState::IDLING)
	, m_pBoss(pBoss)
	, m_isHitPlayerBullet(false)
	, m_isKnockBack(false)
{
	m_pBossAttack = std::make_unique<BossAttack>(this);
	m_pBossIdling = std::make_unique<BossIdling>(this);

}
// デストラクタ
BossAI::~BossAI() {}

// 初期化する
void BossAI::Initialize()
{
	using namespace DirectX::SimpleMath;
	m_initialPosition = Vector3::Zero;  // 初期位置を保存
	m_initialPosition.y = GenerateRandomMultiplier(EnemyParameters::RANDOM_MIN, EnemyParameters::RANDOM_MAX);
	m_velocity = EnemyParameters::INITIAL_VELOCITY; // 浮遊の初期速度
	m_scale = EnemyParameters::INITIAL_BOSS_SCALE; // スケール初期化
	m_position = m_initialPosition;
	m_currentState = m_pBossAttack.get();
	m_currentState->Initialize();
	m_enemyState = IState::EnemyState::ATTACK;// 待機態勢
}
// 更新
void BossAI::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;
	if (m_pBoss->GetEnemyHitByPlayerBullet())m_isHitPlayerBullet = true;
	// sin波を用いた浮遊動作の実装
	m_time += elapsedTime;

	m_position = m_pBoss->GetPosition();
	m_currentState->Update(elapsedTime);
	// sin波を用いた浮遊動作の実装
	// 敵をふわふわ浮遊させる
	m_position.y = m_initialPosition.y + EnemyParameters::AMPLITUDE * std::sin(EnemyParameters::FREQUENCY * m_time);
	m_position.y += m_velocity.y * elapsedTime;
	// シールドが壊されたらノックバック
	auto boss = dynamic_cast<Boss*>(m_pBoss);// IEnemyからBossのポインターを抽出
	if (!m_isKnockBack && boss->GetBossSheild()->GetSheildHP() <= 0)
	{
		ChangeState(m_pBossIdling.get());//徘徊態勢にす
		KnockBack(elapsedTime);
	}
	else
	{
		ChangeState(m_pBossAttack.get());//攻撃態勢にする
	}
	m_pBoss->SetPosition(m_position);
}
// ステート変更
void BossAI::ChangeState(IState* newState)
{
	if (m_currentState != newState)
	{
		m_currentState = newState;
		m_currentState->Initialize();
	}
}

// ノックバック処理
void BossAI::KnockBack(float elapsedTime)
{
	using namespace DirectX::SimpleMath;

	// ノックバックが始まったばかりなら初期設定を行う
	if (m_knockTime == 0.0f)
	{
		m_knockStartPosition = m_position; // ノックバック開始位置
		Vector3 knockBackDirection = (m_position - m_pBoss->GetPlayer()->GetPlayerPos()); // プレイヤーから敵への方向ベクトル
		knockBackDirection.Normalize(); // 正規化して方向ベクトルにする
		m_knockEndPosition = m_position + knockBackDirection; // ノックバック終了位置
		m_initialVelocity = knockBackDirection * EnemyParameters::FIXED_INITIAL_SPEED; // 初期速度
		m_pBoss->SetCanAttack(false);// 攻撃不可能にする
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
	// ノックバックの進行度が一定以上になったら攻撃可能にする
	if (Progression >= EnemyParameters::KNOCKBACK_TIME.canAttackTime)
	{
		m_pBoss->SetCanAttack(true);// 攻撃可能にする
	}
	// ノックバックが終了したかどうかチェック
	if (Progression >= EnemyParameters::KNOCKBACK_TIME.endKnockTime)
	{
		m_knockEndPosition = m_position;
		m_knockTime = 0.0f; // ノックバック時間のリセット
		m_pBoss->SetEnemyHitByPlayerBullet(false); // ノックバック終了

		// これ以降ノックバック処理を行わない
		m_isKnockBack = true;
	}
}
