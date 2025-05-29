/*
*	@file	EnemySpin.cpp
*	@brief	敵スピンクラス
*/
#include <pch.h>
#include "EnemySpin.h"
/*
*	@brief	コンストラクタ
*	@details 敵スピンクラスのコンストラクタ
*	@param	EnemyAI* pEnemyAI 敵AI
*	@return	なし
*/
EnemySpin::EnemySpin(EnemyAI* pEnemyAI)
	: m_pEnemyAI(pEnemyAI)
	, m_rotation(DirectX::SimpleMath::Quaternion::Identity)
	, m_velocity(DirectX::SimpleMath::Vector3::Zero)
	, m_scale(DirectX::SimpleMath::Vector3::One)
	, m_initialPosition(DirectX::SimpleMath::Vector3::Zero)
	, m_position(DirectX::SimpleMath::Vector3::Zero)
	, m_knockTime(0.0f)
	, m_knockStartPosition(DirectX::SimpleMath::Vector3::Zero)
	, m_knockEndPosition(DirectX::SimpleMath::Vector3::Zero)
	, m_initialVelocity(DirectX::SimpleMath::Vector3::Zero)
	, m_time(0.0f)
	, m_angle(0.0f)
{
}
/*
*	@brief	デストラクタ
*	@details 各種ポインターをnullptrに設定
*	@param	なし
*	@return	なし
*/
EnemySpin::~EnemySpin()
{
	m_pEnemyAI = nullptr; // 敵AIのポインターをnullptrに設定
}
/*
*	@brief	初期化
*	@details 敵スピンクラスの初期化
*	@param	なし
*	@return	なし
*/
void EnemySpin::Initialize()
{
	m_position = m_pEnemyAI->GetPosition(); // 敵の位置を取得
	m_rotation = m_pEnemyAI->GetRotation();// 回転取得
	m_velocity = m_pEnemyAI->GetVelocity();// 速度取得
	m_scale = m_pEnemyAI->GetScale();// スケール取得
	m_initialPosition = m_pEnemyAI->GetPosition(); // 初期位置取得
}
/*
*	@brief	更新
*	@details 敵スピンクラスの更新
*	@param	float elapsedTime　経過時間
*	@return	なし
*/
void EnemySpin::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;
	m_time += elapsedTime;// 時間の加算
	UpdateSpin(elapsedTime); // スピンの更新
	UpdateKnockBack(elapsedTime); // ノックバックの更新
	m_pEnemyAI->SetPosition(m_position); // 敵の位置を更新
	m_pEnemyAI->SetRotation(m_rotation); // 敵の回転を更新
	m_pEnemyAI->SetVelocity(m_velocity); // 敵の速度を更新
}
/*
*	@brief	スピンの更新
*	@details 敵スピンクラスのスピンの部分の処理
*	@param	float elapsedTime　経過時間
*	@return	なし
*/
void EnemySpin::UpdateSpin(float elapsedTime)
{
	UNREFERENCED_PARAMETER(elapsedTime); // 引数の未使用警告を抑制
	using namespace DirectX::SimpleMath;
	m_angle = CalculateAngle(m_pEnemyAI->GetPosition(), m_pEnemyAI->GetEnemy()->GetPlayer()->GetPlayerPos());// プレイヤーの方向を取得し、正面を向かせる
	m_angle = Lerp(m_angle, CalculateAngle(m_pEnemyAI->GetEnemy()->GetPlayer()->GetPlayerPos(), m_pEnemyAI->GetPosition()), m_time);// プレイヤーの方向を取得し、一回転させる
	m_rotation = Quaternion::CreateFromYawPitchRoll(m_angle, 0.0f, 0.0f);// 敵を回転させる
}

/*
*	@brief	ノックバックの更新
*	@details 敵スピンクラスのノックバックの部分の処理
*	@param	float elapsedTime　経過時間
*	@return	なし
*/
void EnemySpin::UpdateKnockBack(float elapsedTime)
{
	using namespace DirectX::SimpleMath;
	// ノックバックが始まったばかりなら初期設定を行う
	if (m_knockTime == 0.0f)
	{
		m_knockStartPosition = m_position; // ノックバック開始位置
		Vector3 knockBackDirection = (m_position - m_pEnemyAI->GetEnemy()->GetPlayer()->GetPlayerPos()); // プレイヤーから敵への方向ベクトル
		knockBackDirection.Normalize(); // 正規化して方向ベクトルにする
		m_knockEndPosition = m_position + knockBackDirection; // ノックバック終了位置
		m_initialVelocity = knockBackDirection * EnemyParameters::FIXED_INITIAL_SPEED; // 初期速度
		m_pEnemyAI->GetEnemy()->SetCanAttack(false);// 攻撃不可能にする
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
	if (m_pEnemyAI->GetState() != IState::EnemyState::ANGRY)// 怒り態勢でない場合
		m_pEnemyAI->SetState(IState::EnemyState::HIT);// 攻撃を食らった状態にする
	// ノックバックが終了したかどうかチェック
	if (Progression >= EnemyParameters::KNOCKBACK_TIME.canAttackTime)
		m_pEnemyAI->GetEnemy()->SetCanAttack(true);// 攻撃可能にする
	// ノックバックが終了したかどうかチェック
	if (Progression >= EnemyParameters::KNOCKBACK_TIME.endKnockTime)
	{
		m_knockEndPosition = m_position; // ノックバック終了位置を今の場所にする
		m_knockTime = 0.0f; // ノックバック時間のリセット
		m_pEnemyAI->GetEnemy()->SetEnemyHitByPlayerBullet(false); // ノックバック終了
		m_pEnemyAI->SetState(IState::EnemyState::IDLING);// 待機態勢
	}
}