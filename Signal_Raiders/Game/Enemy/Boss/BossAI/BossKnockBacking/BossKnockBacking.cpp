/*
	@file	BossKnockBacking.cpp
	@brief	ボスのノックバック中の処理クラス
*/
#include <pch.h>
#include "BossKnockBacking.h"

/*
*	@brief	コンストラクタ
*	@details ボスのノックバック中の処理クラスのコンストラクタ
*	@param	pBoss	ボスAIクラスのポインタ
*	@return	なし
*/
BossKnockBacking::BossKnockBacking(BossAI* pBoss)
	: m_pBoss(pBoss)// ボスAIクラスのポインタ
	, m_position(DirectX::SimpleMath::Vector3::Zero)// 座標の初期化
	, m_rotation(DirectX::SimpleMath::Quaternion::Identity)// 回転
	, m_velocity(DirectX::SimpleMath::Vector3::Zero)// 速度
	, m_scale(DirectX::SimpleMath::Vector3::One)// スケール
	, m_initialPosition(DirectX::SimpleMath::Vector3::Zero)// 座標初期値
	, m_time(0.0f)// 時間の初期化
	, m_rotationSpeed(0.0f)// 回転速度
	, m_angle(0.0f)// 角度
	, m_knockTime(0.0f)// ノックバック時間の初期化
	, m_knockStartPosition(DirectX::SimpleMath::Vector3::Zero)// ノックバック開始位置の初期化
	, m_knockEndPosition(DirectX::SimpleMath::Vector3::Zero)// ノックバック終了位置の初期化
	, m_initialVelocity(DirectX::SimpleMath::Vector3::Zero)// ノックバックの初速度の初期化
{
}
/*
*	@brief デストラクタ
*	@details ボスのノックバック中の処理クラスのデストラクタ(ここでは何もしない)
*	@return なし
*/
BossKnockBacking::~BossKnockBacking() {/*do nothing*/ }
/*
*	@brief 初期化
*	@details ボスのノックバック中の処理クラスの初期化
*	@param なし
*	@return なし
*/
void BossKnockBacking::Initialize()
{
	m_position = m_pBoss->GetPosition(); // ボスの位置を取得
	m_rotation = m_pBoss->GetRotation();// ボスの回転を取得
	m_velocity = m_pBoss->GetVelocity();// ボスの速度を取得
	m_scale = m_pBoss->GetScale();// ボスのスケールを取得
	m_initialPosition = m_pBoss->GetPosition();// 座標初期値
	m_rotationSpeed = EnemyParameters::INITIAL_ROTATION_SPEED; // 回転速度
}

/*
*	@brief 更新
*	@details ボスのノックバック中の処理クラスの更新
*	@param elapsedTime 経過時間
*	@return なし
*/
void BossKnockBacking::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;
	m_time += elapsedTime;// 時間の更新
	if (m_knockTime == 0.0f)// ノックバック開始時の処理
	{
		m_knockStartPosition = m_position; // ノックバック開始位置
		Vector3 knockBackDirection = (m_position - m_pBoss->GetEnemy()->GetPlayer()->GetPlayerPos()); // プレイヤーから敵への方向ベクトル
		knockBackDirection.Normalize(); // 正規化して方向ベクトルにする
		m_knockEndPosition = m_position + knockBackDirection; // ノックバック終了位置
		m_initialVelocity = knockBackDirection * EnemyParameters::FIXED_INITIAL_SPEED; // 初期速度
		m_pBoss->GetEnemy()->SetCanAttack(false);// 攻撃不可能にする
	}
	m_knockTime += elapsedTime;	// ノックバック時間の更新
	float Progression = std::min(m_knockTime / EnemyParameters::KNOCKBACK_DURATION,
		EnemyParameters::KNOCKBACK_PROGRESS_MAX);	// ノックバックの進行度
	float decayFactor = std::exp(EnemyParameters::KNOCKBACK_DECAY_RATE * Progression); // 減衰速度を調整するために指数のベースを調整
	Vector3 velocity = m_initialVelocity * decayFactor;	// 減衰した速度を使って位置を更新
	m_position += velocity * elapsedTime;	// 位置を更新
	if (Progression >= EnemyParameters::KNOCKBACK_TIME.canAttackTime)m_pBoss->GetEnemy()->SetCanAttack(true);// ノックバックの進行度が一定以上になったら攻撃可能にする
	if (Progression >= EnemyParameters::KNOCKBACK_TIME.endKnockTime)	// ノックバックが終了したかどうかチェック
	{
		m_knockEndPosition = m_position; // ノックバック終了位置を更新
		m_knockTime = 0.0f; // ノックバック時間のリセット
		m_pBoss->GetEnemy()->SetEnemyHitByPlayerBullet(false); // ノックバック終了	
		m_pBoss->SetIsKnockBack(true);// これ以降ノックバック処理を行わない
		m_pBoss->SetAttackState(IState::EnemyState::ANGRY);// 怒り状態
	}
	m_pBoss->SetPosition(m_position);// 位置をセット
	m_pBoss->SetVelocity(m_velocity);// 速度をセット
	m_pBoss->SetRotation(m_rotation);// 回転をセット
}