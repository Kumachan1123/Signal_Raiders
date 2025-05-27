/*
*	@file	EnemySpin.cpp
*	@brief	敵スピンクラス
*/
#include "pch.h"
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
	m_rotation = m_pEnemyAI->GetRotation();// 敵の回転を取得
	m_velocity = m_pEnemyAI->GetVelocity(); // 敵の移動速度を取得
	m_scale = m_pEnemyAI->GetScale(); // 敵のスケールを取得
	m_initialPosition = m_pEnemyAI->GetPosition(); // 敵の初期位置を取得
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
	m_angle = CalculateAngle(m_pEnemyAI->GetPosition(), m_pEnemyAI->GetEnemy()->GetPlayer()->GetPlayerPos());// プレイヤーの方向を取得し、正面を向かせる
	m_angle = Lerp(m_angle, CalculateAngle(m_pEnemyAI->GetEnemy()->GetPlayer()->GetPlayerPos(), m_pEnemyAI->GetPosition()), m_time);// プレイヤーの方向を取得し、一回転させる
	m_rotation = Quaternion::CreateFromYawPitchRoll(m_angle, 0.0f, 0.0f);// 敵を回転させる
	m_pEnemyAI->SetRotation(m_rotation);// 敵の回転を設定
	m_pEnemyAI->SetVelocity(m_velocity);// 敵の移動速度を設定
}


