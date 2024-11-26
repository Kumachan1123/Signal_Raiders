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
#include <random>
#include <type_traits> // std::enable_if, std::is_integral

// コンストラクタ
BossAI::BossAI()
	: m_currentState(nullptr), m_rotationSpeed(0.5f), m_attackCooldown(0.0f), m_pBossAttack(nullptr), m_enemyState(IState::EnemyState::IDLING)
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
	m_rotation = (GenerateRandomMultiplier(0, 2) <= 1.0f) ? Quaternion::Identity : -Quaternion::Identity;
	m_initialPosition = Vector3::Zero;  // 初期位置を保存
	m_initialPosition.y = 2 * GenerateRandomMultiplier(RANDOM_MIN, RANDOM_MAX) + 5;
	m_rotation.y = GenerateRandomMultiplier(RANDOM_MIN, RANDOM_MAX);
	m_velocity = Vector3(0.0f, 0.5f, 0.0f); // 浮遊の初期速度
	m_scale = Vector3::One; // スケール初期化
	m_time = 0.0f;  // 時間の初期化
	m_position = m_initialPosition;
	m_currentState = m_pBossAttack.get();
	m_currentState->Initialize();
	m_enemyState = IState::EnemyState::ATTACK;// 待機態勢
}
// 更新
void BossAI::Update(float elapsedTime, DirectX::SimpleMath::Vector3& playerPos, bool& isHitToPlayer, bool& isHitToPlayerBullet)
{
	using namespace DirectX::SimpleMath;
	if (isHitToPlayerBullet)m_isHitPlayerBullet = true;
	// sin波を用いた浮遊動作の実装
	m_time += elapsedTime;
	m_currentState->Update(elapsedTime, playerPos, playerPos, isHitToPlayer);
	// sin波を用いた浮遊動作の実装
	float amplitude = 2.0f;  // 振幅
	float frequency = 0.5f;  // 周波数
	// 敵をふわふわ浮遊させる
	m_position.y = m_initialPosition.y + amplitude * std::sin(frequency * m_time);
	m_position.y += m_velocity.y * elapsedTime;


	// プレイヤーの弾に当たった場合
	//if (isHitToPlayerBullet)
		//KnockBack(elapsedTime, pos, isHitToPlayerBullet, playerPos);


	/*m_position = pos;*/
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
// 整数型用のランダムな倍率を生成する関数
int BossAI::GenerateRandomMultiplier(int min, int max)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dis(min, max);
	return dis(gen);
}

// 浮動小数点型用のランダムな倍率を生成する関数
float BossAI::GenerateRandomMultiplier(float min, float max)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> dis(min, max);
	return dis(gen);
}

// ノックバック処理
void BossAI::KnockBack(float elapsedTime, DirectX::SimpleMath::Vector3& pos, bool& isHitToPlayerBullet, const DirectX::SimpleMath::Vector3& playerPos)
{
	using namespace DirectX::SimpleMath;

	// ノックバックが始まったばかりなら初期設定を行う
	if (m_knockTime == 0.0f)
	{
		m_knockStartPosition = pos; // ノックバック開始位置
		Vector3 knockBackDirection = (pos - playerPos); // プレイヤーから敵への方向ベクトル
		knockBackDirection.Normalize(); // 正規化して方向ベクトルにする
		m_knockEndPosition = pos + knockBackDirection * 10; // ノックバック終了位置
		m_initialVelocity = knockBackDirection * 15; // 初期速度

	}

	// ノックバック時間の更新
	m_knockTime += elapsedTime;

	// ノックバックの長さ（秒）
	const float knockBackDuration = 2.0f;

	// ノックバックの進行度（0.0 ～ 1.0）
	float t = std::min(m_knockTime / knockBackDuration, 3.0f);

	// 減衰係数の計算（指数関数的減衰）
	float decayFactor = std::exp(-3.0f * t); // 減衰速度を調整するために指数のベースを調整

	// 減衰した速度を使って位置を更新
	Vector3 velocity = m_initialVelocity * decayFactor;
	pos += velocity * elapsedTime;
	if (m_enemyState != IState::EnemyState::ANGRY)// 怒り態勢でない場合
		m_enemyState = IState::EnemyState::DAMAGE;// ダメージ態勢にする
	// ノックバックが終了したかどうかチェック
	if (t >= 1.0f)
	{
		m_knockEndPosition = pos;
		m_knockTime = 0.0f; // ノックバック時間のリセット
		isHitToPlayerBullet = false; // ノックバック終了
		m_enemyState = IState::EnemyState::IDLING;// 待機態勢
	}
}
