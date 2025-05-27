/*
	@file	EnemyAttack.cpp
	@brief	敵攻撃クラス
*/
#include "pch.h"
#include "EnemyAttack.h"
using namespace DirectX::SimpleMath;
/*
*	@brief	コンストラクタ
*	@param	EnemyAI* enemy
*	@return	なし
*/
EnemyAttack::EnemyAttack(EnemyAI* enemyAI)
	: m_enemyAI(enemyAI)// 敵AI
	, m_attackCooldown(EnemyParameters::ATTACK_COOLDOWN)// 攻撃のクールダウンタイム
	, m_rotationSpeed{}// 回転速度
	, m_pCommonResources{}// 共通リソース
{
}
/*
*	@brief	デストラクタ
*	@return	なし
*/
EnemyAttack::~EnemyAttack() {/*do nothing*/ }
/*
*	@brief	初期化
*	@return	なし
*/
void EnemyAttack::Initialize()
{
	m_rotation = m_enemyAI->GetRotation();// 回転
	m_velocity = m_enemyAI->GetVelocity();// 移動速度
	m_scale = m_enemyAI->GetScale();// サイズ
	m_rotationSpeed = EnemyParameters::INITIAL_ROTATION_SPEED; // 回転速度
}
/*
*	@brief	更新
*	@param	float elapsedTime
*	@return	なし
*/
void EnemyAttack::Update(float elapsedTime)
{
	m_rotationSpeed -= (elapsedTime / EnemyParameters::ROTATION_SPEED_DIVISOR);// 回転速度を減少
	if (m_rotationSpeed <= EnemyParameters::ROTATION_SPEED_MIN)// 回転速度が基準以下になったら
		m_rotationSpeed = EnemyParameters::ROTATION_SPEED_MIN;// 回転速度を基準値に設定
	Vector3 toPlayerVector = m_enemyAI->GetEnemy()->GetPlayer()->GetPlayerPos() - m_enemyAI->GetPosition();	// プレイヤーへのベクトルを計算
	if (toPlayerVector.LengthSquared() > 0.0f)// プレイヤーへのベクトルが0より大きい場合
		toPlayerVector.Normalize();// プレイヤーへのベクトルを正規化
	// 現在の前方ベクトルを取得
	Vector3 forward = Vector3::Transform(Vector3::Forward, m_rotation);
	if (forward.LengthSquared() > 0.0f)// 現在の前方ベクトルが0より大きい場合
		forward.Normalize();// 現在の前方ベクトルを正規化
	float dot = toPlayerVector.Dot(forward);	// 内積を使って角度を計算
	dot = Clamp(dot, EnemyParameters::DOT_CLAMP.min, EnemyParameters::DOT_CLAMP.max); // acosの引数が範囲外になるのを防ぐため
	float angle = std::acos(dot);// プレイヤーの方向に向くための回転を計算
	Vector3 cross = toPlayerVector.Cross(forward);// 外積を使って回転方向を計算
	if (cross.y < 0)angle = -angle;// yが0より小さい場合、角度を反転
	Quaternion toPlayerRotation = Quaternion::CreateFromAxisAngle(Vector3::Up, angle);// プレイヤーの方向に向くための回転を計算
	if (std::abs(angle) > EnemyParameters::ANGLE_THRESHOLD)	// 目標角度と現在の角度が非常に小さい場合、振動を防ぐために補間を止める
	{
		m_rotation = Quaternion::Slerp(m_rotation, toPlayerRotation * m_rotation, elapsedTime * (m_rotationSpeed));// 回転を設定
		m_rotation.Normalize();// 回転を正規化
	}
	m_enemyAI->SetPosition(m_enemyAI->GetPosition() + toPlayerVector * (m_velocity.Length() * EnemyParameters::VELOCITY_SCALE_FACTOR) * elapsedTime);// プレイヤーの方向に移動
	m_attackCooldown -= elapsedTime;// 攻撃のクールダウンタイムを減少
	if (m_attackCooldown <= EnemyParameters::CHANGE_FACE_TIME)// 攻撃のクールダウンタイムが2.0以下になったら
	{
		m_enemyAI->SetState(IState::EnemyState::ANGRY);// 怒り態勢にする
		if (m_attackCooldown <= 0.0f)m_attackCooldown = EnemyParameters::ATTACK_COOLDOWN;// 次の攻撃までのクールダウンタイムを3秒に設定
	}
	m_enemyAI->SetRotation(m_rotation);// 回転を設定
	m_enemyAI->SetVelocity(m_velocity);// 移動速度を設定
}
