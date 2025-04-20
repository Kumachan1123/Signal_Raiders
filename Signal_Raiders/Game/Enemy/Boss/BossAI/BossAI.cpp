/*
	@file	BossAI.cpp
	@brief	敵AIクラス
*/
#include "pch.h"
#include "BossAI.h"

using namespace DirectX::SimpleMath;


/*
*	@brief	コンストラクタ
*	@param[in] pBoss ボスクラスのポインタ
*	@return なし
*/
BossAI::BossAI(IEnemy* pBoss)
	: m_currentState(nullptr)// 現在のステート
	, m_attackCooldown(0.0f)// 攻撃クールダウン
	, m_knockTime(0.0f)// ノックバック時間
	, m_time(0.0f)// 時間
	, m_pBossAttack(nullptr)// 攻撃時
	, m_enemyState(IState::EnemyState::IDLING)// 状態
	, m_pBoss(pBoss)// ボス
	, m_isHitPlayerBullet(false)// プレイヤーの弾に当たったか
	, m_isKnockBack(false)// ノックバック中か
	, m_isAttack(false)// 攻撃中か
{
	m_pBossAttack = std::make_unique<BossAttack>(this);// 攻撃時
	m_pBossIdling = std::make_unique<BossIdling>(this);// 待機時

}

/*
*	@brief デストラクタ
*	@return なし
*/
BossAI::~BossAI() {}

/*
*	@brief 初期化
*	@return なし
*/
void BossAI::Initialize()
{
	m_initialPosition = Vector3::Zero;  // 初期位置を保存
	m_initialPosition.y = GenerateRandomMultiplier(EnemyParameters::RANDOM_MIN, EnemyParameters::RANDOM_MAX);// Y座標をランダムに設定
	m_velocity = EnemyParameters::INITIAL_VELOCITY; // 浮遊の初期速度
	m_scale = EnemyParameters::INITIAL_BOSS_SCALE; // スケール初期化
	m_position = m_initialPosition;// 初期位置を設定
	m_currentState = m_pBossAttack.get();// 現在のステートを攻撃態勢に設定
	m_currentState->Initialize();// 初期化
	m_enemyState = IState::EnemyState::ATTACK;// 待機態勢
}
/*
*	@brief 更新
*	@param[in] elapsedTime 経過時間
*	@return なし
*/
void BossAI::Update(float elapsedTime)
{
	if (m_pBoss->GetEnemyHitByPlayerBullet())m_isHitPlayerBullet = true;// プレイヤーの弾に当たったか
	m_time += elapsedTime;// 時間の更新
	m_position = m_pBoss->GetPosition();// 位置の更新
	m_currentState->Update(elapsedTime);// 現在のステートを更新
	m_position.y = m_initialPosition.y + EnemyParameters::AMPLITUDE * std::sin(EnemyParameters::FREQUENCY * m_time);// Y座標を更新
	m_position.y += m_velocity.y * elapsedTime;// Y座標を更新
	auto boss = dynamic_cast<BossBase*>(m_pBoss);// IEnemyからBossのポインターを抽出
	if (!m_isKnockBack && boss->GetBossSheild()->GetSheildHP() <= 0)	// シールドが壊されたらノックバック
	{
		ChangeState(m_pBossIdling.get());//徘徊態勢にする
		KnockBack(elapsedTime);// ノックバック処理
		SetIsAttack(false);// 攻撃中ではない
	}
	else // シールドが壊されていない場合
	{
		ChangeState(m_pBossAttack.get());//攻撃態勢にする
		SetIsAttack(true);// 攻撃中にする
	}
	m_pBoss->SetPosition(m_position);// 位置をセット
}
/*
*	@brief ステート変更
*	@param[in] newState 新しいステート
*	@return なし
*/
void BossAI::ChangeState(IState* newState)
{
	if (m_currentState != newState)// 現在のステートと新しいステートが違う場合
	{
		m_currentState = newState;// 現在のステートを新しいステートに変更
		m_currentState->Initialize();// 初期化
	}
}

/*
*	@brief ノックバック
*	@param[in] elapsedTime 経過時間
*	@return なし
*/
void BossAI::KnockBack(float elapsedTime)
{
	if (m_knockTime == 0.0f)// ノックバック開始時の処理
	{
		m_knockStartPosition = m_position; // ノックバック開始位置
		Vector3 knockBackDirection = (m_position - m_pBoss->GetPlayer()->GetPlayerPos()); // プレイヤーから敵への方向ベクトル
		knockBackDirection.Normalize(); // 正規化して方向ベクトルにする
		m_knockEndPosition = m_position + knockBackDirection; // ノックバック終了位置
		m_initialVelocity = knockBackDirection * EnemyParameters::FIXED_INITIAL_SPEED; // 初期速度
		m_pBoss->SetCanAttack(false);// 攻撃不可能にする
	}
	m_knockTime += elapsedTime;	// ノックバック時間の更新
	float Progression = std::min(m_knockTime / EnemyParameters::KNOCKBACK_DURATION,
		EnemyParameters::KNOCKBACK_PROGRESS_MAX);	// ノックバックの進行度
	float decayFactor = std::exp(EnemyParameters::KNOCKBACK_DECAY_RATE * Progression); // 減衰速度を調整するために指数のベースを調整
	Vector3 velocity = m_initialVelocity * decayFactor;	// 減衰した速度を使って位置を更新
	m_position += velocity * elapsedTime;	// 位置を更新
	if (Progression >= EnemyParameters::KNOCKBACK_TIME.canAttackTime)m_pBoss->SetCanAttack(true);// ノックバックの進行度が一定以上になったら攻撃可能にする
	if (Progression >= EnemyParameters::KNOCKBACK_TIME.endKnockTime)	// ノックバックが終了したかどうかチェック
	{
		m_knockEndPosition = m_position; // ノックバック終了位置を更新
		m_knockTime = 0.0f; // ノックバック時間のリセット
		m_pBoss->SetEnemyHitByPlayerBullet(false); // ノックバック終了	
		m_isKnockBack = true;// これ以降ノックバック処理を行わない
	}
}
