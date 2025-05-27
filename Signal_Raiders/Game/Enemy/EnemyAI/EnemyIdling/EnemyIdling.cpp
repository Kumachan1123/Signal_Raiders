/*
	@file	EnemyIdling.cpp
	@brief	敵Idlingクラス
*/
#include "pch.h"
#include "EnemyIdling.h"
/*
*	@brief	コンストラクタ
*	@param	EnemyAI* enemy
*	@return	なし
*/
EnemyIdling::EnemyIdling(EnemyAI* enemyAI)
	: m_enemyAI(enemyAI)// 敵AI
	, m_rotation(DirectX::SimpleMath::Quaternion::Identity)// 回転
	, m_velocity(DirectX::SimpleMath::Vector3::Zero)// 速度
	, m_scale(DirectX::SimpleMath::Vector3::One)// スケール
	, m_initialPosition(DirectX::SimpleMath::Vector3::Zero)// 初期位置
	, m_time(0.0f)// 時間
	, m_rotationSpeed(0.0f)// 回転速度
{}
/*
*	@brief	デストラクタ
*	@return	なし
*/
EnemyIdling::~EnemyIdling() {/*do nothing*/ }
/*
*	@brief	初期化
*	@return	なし
*/
void EnemyIdling::Initialize()
{
	m_rotation = m_enemyAI->GetRotation();// 回転取得
	m_velocity = m_enemyAI->GetVelocity();// 速度取得
	m_scale = m_enemyAI->GetScale();// スケール取得
	m_initialPosition = m_enemyAI->GetPosition(); // 初期位置取得
	m_rotationSpeed = EnemyParameters::INITIAL_ROTATION_SPEED; // 回転速度
}
/*
*	@brief	更新
*	@param	float elapsedTime
*	@return	なし
*/
void EnemyIdling::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;
	// sin波を使った回転の更新
	float randomMultiplier = GenerateRandomMultiplier(EnemyParameters::RANDOM_ROTATION_MIN, EnemyParameters::RANDOM_ROTATION_MAX);  // ランダムな倍率を生成
	float sinRotationSpeed = m_rotationSpeed + EnemyParameters::AMPLITUDE * std::sin(EnemyParameters::FREQUENCY * m_time);  // 回転速度をsin波で変化させる
	Quaternion deltaRotation = Quaternion::CreateFromAxisAngle(Vector3::Up, sinRotationSpeed * randomMultiplier * elapsedTime);  // 生成した回転速度に基づき、上方向（Y軸）を中心に回転を生成
	m_rotation *= deltaRotation;  // 既存の回転に新しい回転を適用
	m_rotation.Normalize();  // 回転を正規化し、安定したクォータニオンを維持
	Vector3 forward = Vector3::Transform(Vector3::Backward * EnemyParameters::MOVE_SPEED, m_rotation);  // 前方ベクトルを取得
	m_enemyAI->SetPosition(m_enemyAI->GetPosition() + forward * (m_velocity.Length()) * elapsedTime);  // 敵の位置を更新
	m_enemyAI->SetRotation(m_rotation);  // 敵の回転を更新
	m_enemyAI->SetVelocity(m_velocity);  // 敵の速度を更新
}

