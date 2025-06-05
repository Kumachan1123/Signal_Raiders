/*
*	@file	EnemyIdling.cpp
*	@brief	敵徘徊クラス
*/
#include <pch.h>
#include "EnemyIdling.h"
/*
*	@brief	コンストラクタ
*	@details 敵徘徊クラスのコンストラクタ
*	@param	EnemyAI* pEnemyAI
*	@return	なし
*/
EnemyIdling::EnemyIdling(EnemyAI* pEnemyAI)
	: m_pEnemyAI(pEnemyAI)// 敵AI
	, m_rotation(DirectX::SimpleMath::Quaternion::Identity)// 回転
	, m_velocity(DirectX::SimpleMath::Vector3::Zero)// 速度
	, m_scale(DirectX::SimpleMath::Vector3::One)// スケール
	, m_initialPosition(DirectX::SimpleMath::Vector3::Zero)// 初期位置
	, m_time(0.0f)// 時間
	, m_rotationSpeed(0.0f)// 回転速度
{}
/*
*	@brief	デストラクタ
*	@details 各種ポインターをnullptrに設定
*	@param	なし
*	@return	なし
*/
EnemyIdling::~EnemyIdling()
{
	// 敵AIのポインターをnullptrに設定
	m_pEnemyAI = nullptr;
}
/*
*	@brief	初期化
*	@details 敵徘徊クラスの初期化
*	@param	なし
*	@return	なし
*/
void EnemyIdling::Initialize()
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
	// 回転速度を初期化
	m_rotationSpeed = EnemyParameters::INITIAL_ROTATION_SPEED;
}
/*
*	@brief	更新
*	@details 敵徘徊クラスの更新
*	@param	float elapsedTime
*	@return	なし
*/
void EnemyIdling::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;
	// 敵がプレイヤーの一定範囲内に入っている場合
	if ((m_pEnemyAI->GetEnemy()->GetHitToPlayer() || m_pEnemyAI->GetHitPlayerBullet()))
	{
		//攻撃態勢にする
		m_pEnemyAI->ChangeState(m_pEnemyAI->GetEnemyAttack());
		// 攻撃態勢にする
		m_pEnemyAI->SetState(IState::EnemyState::ATTACK);
		// 攻撃中にする
		m_pEnemyAI->SetIsAttack(true);
	}
	// プレイヤーの弾に当たった場合
	if (m_pEnemyAI->GetEnemy()->GetEnemyHitByPlayerBullet())
	{
		//スピンする
		m_pEnemyAI->ChangeState(m_pEnemyAI->GetEnemySpin());
		// 攻撃を食らった状態にする
		m_pEnemyAI->SetState(IState::EnemyState::HIT);
		// 攻撃中でない
		m_pEnemyAI->SetIsAttack(false);
	}
	// AIから位置を取得
	m_position = m_pEnemyAI->GetEnemy()->GetPosition();
	// ランダムな回転倍率を生成する
	float randomMultiplier = GenerateRandomMultiplier(EnemyParameters::RANDOM_ROTATION_MIN, EnemyParameters::RANDOM_ROTATION_MAX);
	// sin波を使って回転速度を変化させる
	float sinRotationSpeed = m_rotationSpeed + EnemyParameters::AMPLITUDE * std::sin(EnemyParameters::FREQUENCY * m_time);
	// 上方向（Y軸）を軸にした回転を作成する
	Quaternion deltaRotation = Quaternion::CreateFromAxisAngle(Vector3::Up, sinRotationSpeed * randomMultiplier * elapsedTime);
	// 現在の回転に新しい回転を適用する
	m_rotation *= deltaRotation;
	// 回転を正規化して、数値の安定性を保つ
	m_rotation.Normalize();
	// 回転後の前方ベクトルを取得する（後ろ向き×速度）
	Vector3 forward = Vector3::Transform(Vector3::Backward * EnemyParameters::MOVE_SPEED, m_rotation);
	// 新しい位置を設定する
	Vector3 newPosition = Vector3(m_pEnemyAI->GetPosition().x, m_position.y, m_pEnemyAI->GetPosition().z);
	// 敵の位置を更新する
	m_pEnemyAI->SetPosition(newPosition + forward * (m_velocity.Length()) * elapsedTime);
	// 敵の回転を更新する
	m_pEnemyAI->SetRotation(m_rotation);
	// 敵の速度を更新する
	m_pEnemyAI->SetVelocity(m_velocity);
}

