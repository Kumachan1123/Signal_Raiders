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
	: m_pEnemyAI(pEnemyAI)// 敵AIのポインター
	, m_rotation(DirectX::SimpleMath::Quaternion::Identity) // 回転の初期化
	, m_velocity(DirectX::SimpleMath::Vector3::Zero) // 速度の初期化
	, m_scale(DirectX::SimpleMath::Vector3::One) // スケールの初期化
	, m_initialPosition(DirectX::SimpleMath::Vector3::Zero) // 初期位置の初期化
	, m_position(DirectX::SimpleMath::Vector3::Zero) // 現在位置の初期化
	, m_knockTime(0.0f) // ノックバック時間の初期化
	, m_knockStartPosition(DirectX::SimpleMath::Vector3::Zero) // ノックバック開始位置の初期化
	, m_knockEndPosition(DirectX::SimpleMath::Vector3::Zero) // ノックバック終了位置の初期化
	, m_initialVelocity(DirectX::SimpleMath::Vector3::Zero) // ノックバックの初期速度の初期化
	, m_time(0.0f) // 時間の初期化
	, m_angle(0.0f) // 角度の初期化
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
	// 敵AIのポインターをnullptrに設定
	m_pEnemyAI = nullptr;
}
/*
*	@brief	初期化
*	@details 敵スピンクラスの初期化
*	@param	なし
*	@return	なし
*/
void EnemySpin::Initialize()
{
	// AIから位置を取得して設定する
	m_position = m_pEnemyAI->GetPosition();
	// AIから回転を取得して設定する
	m_rotation = m_pEnemyAI->GetRotation();
	// AIから移動速度を取得して設定する
	m_velocity = m_pEnemyAI->GetVelocity();
	// AIからスケールを取得して設定する
	m_scale = m_pEnemyAI->GetScale();
	// AIから初期位置を取得して設定する
	m_initialPosition = m_pEnemyAI->GetPosition();
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
	// 時間の加算
	m_time += elapsedTime;
	// AIから位置を取得
	m_position = m_pEnemyAI->GetEnemy()->GetPosition();
	// スピンの更新
	UpdateSpin(elapsedTime);
	// ノックバックの更新
	UpdateKnockBack(elapsedTime);
	// 敵の位置を更新
	m_pEnemyAI->SetPosition(m_position);
	// 敵の回転を更新
	m_pEnemyAI->SetRotation(m_rotation);
	// 敵の速度を更新
	m_pEnemyAI->SetVelocity(m_velocity);
}
/*
*	@brief	スピンの更新
*	@details 敵スピンクラスのスピンの部分の処理
*	@param	float elapsedTime　経過時間
*	@return	なし
*/
void EnemySpin::UpdateSpin(float elapsedTime)
{
	using namespace DirectX::SimpleMath;
	// 引数の未使用警告を抑制
	UNREFERENCED_PARAMETER(elapsedTime);
	// 敵からプレイヤーへの方向の角度を取得
	m_angle = CalculateAngle(m_pEnemyAI->GetPosition(), m_pEnemyAI->GetEnemy()->GetPlayer()->GetPlayerPos());
	// プレイヤーから敵への角度を取得し、それと現在の角度を補間(ノックバック中の回転演出)
	m_angle = Lerp(m_angle, CalculateAngle(m_pEnemyAI->GetEnemy()->GetPlayer()->GetPlayerPos(), m_pEnemyAI->GetPosition()), m_time);
	// 計算した角度を使って、敵のY軸方向（Yaw）の回転を設定
	m_rotation = Quaternion::CreateFromYawPitchRoll(m_angle, 0.0f, 0.0f);
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
	// ノックバックが始まったばかりのとき、初期化処理を行う
	if (m_knockTime == 0.0f)
	{
		// 現在位置をノックバックの開始位置として記録
		m_knockStartPosition = m_position;
		// プレイヤーから敵への方向をベクトルとして取得
		Vector3 knockBackDirection = (m_position - m_pEnemyAI->GetEnemy()->GetPlayer()->GetPlayerPos());
		//ノックバックする方向を正規化
		knockBackDirection.Normalize();
		// ノックバックの仮の終了位置を設定
		m_knockEndPosition = m_position + knockBackDirection;
		// 初期の速度ベクトルを設定
		m_initialVelocity = knockBackDirection * EnemyParameters::FIXED_INITIAL_SPEED;
		// ノックバック中は攻撃できないようにする
		m_pEnemyAI->GetEnemy()->SetCanAttack(false);
	}
	// ノックバック経過時間を加算
	m_knockTime += elapsedTime;
	// ノックバックの進行度を計算（0.0～1.0の範囲、上限はKNOCKBACK_PROGRESS_MAX）
	float Progression = std::min(m_knockTime / EnemyParameters::KNOCKBACK_DURATION, EnemyParameters::KNOCKBACK_PROGRESS_MAX);
	// 減衰係数を指数関数で計算（時間が経つごとに速度が小さくなる）
	float decayFactor = std::exp(EnemyParameters::KNOCKBACK_DECAY_RATE * Progression);
	// 減衰後の速度で位置を更新
	Vector3 velocity = m_initialVelocity * decayFactor;
	// 実際に位置を変化させる
	m_position += velocity * elapsedTime;
	// 状態が「怒り」でない場合は「ヒット状態」に遷移させる
	if (m_pEnemyAI->GetState() != IState::EnemyState::ANGRY)m_pEnemyAI->SetState(IState::EnemyState::HIT);
	// 一定時間経過後、攻撃を再び許可する
	if (Progression >= EnemyParameters::KNOCKBACK_TIME.canAttackTime)m_pEnemyAI->GetEnemy()->SetCanAttack(true);
	// ノックバックが完全に終了したかを判定
	if (Progression >= EnemyParameters::KNOCKBACK_TIME.endKnockTime)
	{
		// 最終的な位置をノックバック終了地点として記録
		m_knockEndPosition = m_position;
		// ノックバック経過時間をリセット
		m_knockTime = 0.0f;
		// ノックバック状態を解除
		m_pEnemyAI->GetEnemy()->SetEnemyHitByPlayerBullet(false);
		// 状態を待機状態に戻す
		m_pEnemyAI->SetState(IState::EnemyState::IDLING);
		// 攻撃中でない状態にする
		m_pEnemyAI->SetIsAttack(false);
		// 徘徊状態にする
		m_pEnemyAI->ChangeState(m_pEnemyAI->GetEnemyIdling());
	}
}
