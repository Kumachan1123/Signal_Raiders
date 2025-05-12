/*
	@file	EnemyAI.cpp
	@brief	敵AIクラス
*/
#include "pch.h"
#include "EnemyAI.h"

/*
*	@brief	コンストラクタ
*	@param[in]	IEnemy* pEnemy　敵
*	@return	なし
*/
EnemyAI::EnemyAI(IEnemy* pEnemy)
	: m_currentState(nullptr)// 現在の状態
	, m_attackCooldown(0.0f)// 攻撃クールダウン
	, m_time(0.0f)// 時間
	, m_isHitPlayerBullet(false)// プレイヤーの弾に当たったか
	, m_knockTime(0.0f)// ノックバック時間
	, m_pEnemyAttack(nullptr)// 攻撃クラス
	, m_pEnemySpin(nullptr)// スピンクラス
	, m_pEnemyIdling(nullptr)// 徘徊クラス
	, m_canAttack(true)// 攻撃可能か
	, m_enemyState(IState::EnemyState::IDLING)// 敵の状態
	, m_pEnemy(pEnemy)// 敵
	, m_isAttack(false)// 攻撃中か
{
	m_pEnemy = pEnemy;// 敵を取得
	m_pEnemyAttack = std::make_unique<EnemyAttack>(this);// 攻撃クラスを生成
	m_pEnemyIdling = std::make_unique<EnemyIdling>(this);// 徘徊クラスを生成
	m_pEnemySpin = std::make_unique<EnemySpin>(this); // スピンクラスを生成
}

/*
*	@brief	デストラクタ
*	@return	なし
*/
EnemyAI::~EnemyAI() {/*do nothing*/ }

/*
*	@brief	初期化
*	@return	なし
*/
void EnemyAI::Initialize()
{
	using namespace DirectX::SimpleMath;
	m_initialPosition = Vector3::Zero;  // 初期位置を初期化
	m_initialPosition.y = GenerateRandomMultiplier(EnemyParameters::RANDOM_MIN, EnemyParameters::RANDOM_MAX); // 初期位置のY座標を設定
	m_rotation.y = GenerateRandomMultiplier(EnemyParameters::RANDOM_MIN, EnemyParameters::RANDOM_MAX); // Y軸の回転を設定
	m_velocity = EnemyParameters::INITIAL_VELOCITY; // 浮遊の初期速度
	m_scale = Vector3::One; // スケール初期化
	m_position = m_initialPosition;// 位置初期化
	m_currentState = m_pEnemyIdling.get(); // 徘徊態勢にする
	m_currentState->Initialize();// 状態を初期化
	m_enemyState = IState::EnemyState::IDLING;// 待機態勢
}
/*
*	@brief	更新
*	@param[in]	float elapsedTime　経過時間
*	@return	なし
*/
void EnemyAI::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;
	m_position = m_pEnemy->GetPosition();// 位置を取得
	if (m_pEnemy->GetEnemyHitByPlayerBullet())m_isHitPlayerBullet = true;// プレイヤーの弾に当たったかどうかを取得
	m_time += elapsedTime; // 時間の加算
	m_position.y = m_initialPosition.y + EnemyParameters::AMPLITUDE * std::sin(EnemyParameters::FREQUENCY * m_time);	// 敵をふわふわ浮遊させる
	m_position.y += m_velocity.y * elapsedTime; // 敵のY座標を更新
	// 敵がプレイヤーの一定範囲内に入っている場合
	if ((m_pEnemy->GetHitToPlayer() || m_isHitPlayerBullet))
	{
		ChangeState(m_pEnemyAttack.get());//攻撃態勢にする
		m_enemyState = IState::EnemyState::ATTACK;// 徘徊態勢
		SetIsAttack(true);// 攻撃中にする
	}
	else
	{
		ChangeState(m_pEnemyIdling.get());//徘徊態勢にする
		m_enemyState = IState::EnemyState::IDLING;// 徘徊態勢
		SetIsAttack(false);// 攻撃中でない
	}
	// プレイヤーの弾に当たった場合
	if (m_pEnemy->GetEnemyHitByPlayerBullet())
	{
		KnockBack(elapsedTime);// ノックバックする
		ChangeState(m_pEnemySpin.get());//スピンする
		m_enemyState = IState::EnemyState::HIT;// 攻撃を食らった状態にする
		SetIsAttack(false);// 攻撃中でない
	}
	m_currentState->Update(elapsedTime); // 現在の状態を更新
	m_pEnemy->SetPosition(m_position); // 敵の位置を更新
}
/*
*	@brief	状態変更
*	@param[in]	IState* newState　新しい状態
*	@return	なし
*/
void EnemyAI::ChangeState(IState* newState)
{
	// 新しい状態が現在の状態と異なる場合
	if (m_currentState != newState)
	{
		m_currentState = newState;// 新しい状態に変更
		m_currentState->Initialize(); // 新しい状態を初期化
	}
}


/*
*	@brief	ノックバック処理
*	@param[in]	float elapsedTime　経過時間
*	@return	なし
*/
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
	m_position += velocity * elapsedTime; // ノックバックの位置を更新
	if (GetState() != IState::EnemyState::ANGRY)// 怒り態勢でない場合
		SetState(IState::EnemyState::HIT);// 攻撃を食らった状態にする
	// ノックバックが終了したかどうかチェック
	if (Progression >= EnemyParameters::KNOCKBACK_TIME.canAttackTime)
		m_pEnemy->SetCanAttack(true);// 攻撃可能にする
	// ノックバックが終了したかどうかチェック
	if (Progression >= EnemyParameters::KNOCKBACK_TIME.endKnockTime)
	{
		m_knockEndPosition = m_position; // ノックバック終了位置を今の場所にする
		m_knockTime = 0.0f; // ノックバック時間のリセット
		m_pEnemy->SetEnemyHitByPlayerBullet(false); // ノックバック終了
		SetState(IState::EnemyState::IDLING);// 待機態勢
	}
}
