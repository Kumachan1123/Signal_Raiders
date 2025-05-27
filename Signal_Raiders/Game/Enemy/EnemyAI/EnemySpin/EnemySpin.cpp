/*
	@file	EnemySpin.cpp
	@brief	敵スピンクラス
*/
#include "pch.h"
#include "EnemySpin.h"
/*
*	@brief	コンストラクタ
*	@param	EnemyAI* enemyAI 敵AI
*	@return	なし
*/
EnemySpin::EnemySpin(EnemyAI* enemyAI)
	: m_enemyAI(enemyAI)
	, m_rotation(DirectX::SimpleMath::Quaternion::Identity)
	, m_velocity(DirectX::SimpleMath::Vector3::Zero)
	, m_scale(DirectX::SimpleMath::Vector3::One)
	, m_initialPosition(DirectX::SimpleMath::Vector3::Zero)
	, m_time(0.0f)
	, m_angle(0.0f)
{
}
/*
*	@brief	デストラクタ
*	@return	なし
*/
EnemySpin::~EnemySpin() {/*do nothing*/ }
/*
*	@brief	初期化
*	@return	なし
*/
void EnemySpin::Initialize()
{
	m_rotation = m_enemyAI->GetRotation();// 敵の回転を取得
	m_velocity = m_enemyAI->GetVelocity(); // 敵の移動速度を取得
	m_scale = m_enemyAI->GetScale(); // 敵のスケールを取得
	m_initialPosition = m_enemyAI->GetPosition(); // 敵の初期位置を取得
}
/*
*	@brief	更新
*	@param	float elapsedTime　経過時間
*	@return	なし
*/
void EnemySpin::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;
	m_time += elapsedTime;// 時間の加算
	m_angle = CalculateAngle(m_enemyAI->GetPosition(), m_enemyAI->GetEnemy()->GetPlayer()->GetPlayerPos());// プレイヤーの方向を取得し、正面を向かせる
	m_angle = Lerp(m_angle, CalculateAngle(m_enemyAI->GetEnemy()->GetPlayer()->GetPlayerPos(), m_enemyAI->GetPosition()), m_time);// プレイヤーの方向を取得し、一回転させる
	m_rotation = Quaternion::CreateFromYawPitchRoll(m_angle, 0.0f, 0.0f);// 敵を回転させる
	m_enemyAI->SetRotation(m_rotation);// 敵の回転を設定
	m_enemyAI->SetVelocity(m_velocity);// 敵の移動速度を設定
}


