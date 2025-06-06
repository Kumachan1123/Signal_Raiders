/*
*	@file	EnemyAttack.cpp
*	@brief	敵攻撃クラス
*/
#include <pch.h>
#include "EnemyAttack.h"
using namespace DirectX::SimpleMath;
/*
*	@brief	コンストラクタ
*	@details 敵攻撃クラスのコンストラクタ
*	@param	EnemyAI* pEnemyAI
*	@return	なし
*/
EnemyAttack::EnemyAttack(EnemyAI* pEnemyAI)
	: m_pEnemyAI(pEnemyAI)// 敵AI
	, m_attackCooldown(EnemyParameters::ATTACK_COOLDOWN)// 攻撃のクールダウンタイム
	, m_rotationSpeed{}// 回転速度
	, m_pCommonResources{}// 共通リソース
	, m_time(0.0f)// 時間
{
}
/*
*	@brief	デストラクタ
*	@details 各種ポインターをnullptrに設定
*	@param	なし
*	@return	なし
*/
EnemyAttack::~EnemyAttack()
{
	// 敵AIのポインターをnullptrに設定
	m_pEnemyAI = nullptr;
}
/*
*	@brief	初期化
*	@details 敵攻撃クラスの初期化
*	@param	なし
*	@return	なし
*/
void EnemyAttack::Initialize()
{
	// AIから回転情報を取得して設定する
	m_rotation = m_pEnemyAI->GetRotation();
	// AIから移動速度を取得して設定する
	m_velocity = m_pEnemyAI->GetVelocity();
	// AIからスケール（サイズ）を取得して設定する
	m_scale = m_pEnemyAI->GetScale();
	// AIからクールダウンタイムを取得して設定する
	m_attackCooldown = m_pEnemyAI->GetAttackCooldown();
	// 初期の回転速度を設定する
	m_rotationSpeed = EnemyParameters::INITIAL_ROTATION_SPEED;
}
/*
*	@brief	更新
*	@details 敵攻撃クラスの更新
*	@param	float elapsedTime
*	@return	なし
*/
void EnemyAttack::Update(float elapsedTime)
{
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
	// 回転速度を減少させる
	m_rotationSpeed -= (elapsedTime / EnemyParameters::ROTATION_SPEED_DIVISOR);
	// 回転速度が最低値を下回ったら、最低値に固定する
	if (m_rotationSpeed <= EnemyParameters::ROTATION_SPEED_MIN)	m_rotationSpeed = EnemyParameters::ROTATION_SPEED_MIN;
	// プレイヤーの方向へのベクトルを計算する
	Vector3 toPlayerVector = m_pEnemyAI->GetEnemy()->GetPlayer()->GetPlayerPos() - m_pEnemyAI->GetPosition();
	// ゼロベクトルでないなら正規化する
	if (toPlayerVector.LengthSquared() > 0.0f)	toPlayerVector.Normalize();
	// 現在の前方ベクトルを取得する
	Vector3 forward = Vector3::Transform(Vector3::Forward, m_rotation);
	// ゼロベクトルでないなら正規化する
	if (forward.LengthSquared() > 0.0f)	forward.Normalize();
	// プレイヤーの方向との角度を内積で求める
	float dot = toPlayerVector.Dot(forward);
	// 内積をClampして安全にacosに通す
	dot = Clamp(dot, EnemyParameters::DOT_CLAMP.min, EnemyParameters::DOT_CLAMP.max);
	// 回転角を求める
	float angle = std::acos(dot);
	// 回転方向を外積で判定する
	Vector3 cross = toPlayerVector.Cross(forward);
	// y成分が負なら回転方向を反転する
	if (cross.y < 0)angle = -angle;
	// 回転クォータニオンを生成する
	Quaternion toPlayerRotation = Quaternion::CreateFromAxisAngle(Vector3::Up, angle);
	// 角度が一定以上のとき
	if (std::abs(angle) > EnemyParameters::ANGLE_THRESHOLD)
	{
		// プレイヤーの方向に向かって回転をスムーズに適用する
		m_rotation = Quaternion::Slerp(m_rotation, toPlayerRotation * m_rotation, elapsedTime * m_rotationSpeed);
		// 回転を正規化する
		m_rotation.Normalize();
	}
	// プレイヤーの方向に向かって移動する
	m_pEnemyAI->SetPosition(m_pEnemyAI->GetPosition() + toPlayerVector * (m_velocity.Length() * EnemyParameters::VELOCITY_SCALE_FACTOR) * elapsedTime);
	// クールダウンを減らす
	m_attackCooldown -= elapsedTime;
	// AIにクールダウンタイムを設定する
	m_pEnemyAI->SetAttackCooldown(m_attackCooldown);
	// クールダウンが閾値を下回ったら
	if (m_attackCooldown <= EnemyParameters::CHANGE_FACE_TIME)
	{
		// 敵の状態を怒り状態に変更する
		m_pEnemyAI->SetState(IState::EnemyState::ANGRY);
		// 0以下になったら
		if (m_attackCooldown <= 0.0f)
		{
			// 攻撃態勢にする
			m_pEnemyAI->SetState(IState::EnemyState::ATTACK);
			// クールダウンをリセットする
			m_attackCooldown = EnemyParameters::ATTACK_COOLDOWN;
			m_pEnemyAI->SetAttackCooldown(m_attackCooldown);
		}
	}
	// 回転と速度をAIに反映させる
	m_pEnemyAI->SetRotation(m_rotation);
	m_pEnemyAI->SetVelocity(m_velocity);

}
