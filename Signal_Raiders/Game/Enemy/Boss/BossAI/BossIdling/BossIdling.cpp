/*
	@file	BossIdling.cpp
	@brief	ボス徘徊クラス
*/
#include "pch.h"
#include "BossIdling.h"

using namespace DirectX::SimpleMath;

/*
*	@brief	コンストラクタ
*	@param[in]	pBoss	ボスAIクラスのポインタ
*	@return	なし
*/
BossIdling::BossIdling(BossAI* pBoss)
	: m_pBoss(pBoss)// ボスAIクラスのポインタ
	, m_rotation(Quaternion::Identity)// 回転
	, m_velocity(Vector3::Zero)// 速度
	, m_scale(Vector3::One)// スケール
	, m_initialPosition(Vector3::Zero)// 座標初期値
	, m_time(0.0f)// 時間の初期化
	, m_rotationSpeed(0.0f)// 回転速度
	, m_angle(0.0f)// 角度
{
}
/*
*	@brief デストラクタ
*	@return なし
*/
BossIdling::~BossIdling() {}
/*
*	@brief 初期化
*	@return なし
*/
void BossIdling::Initialize()
{
	m_rotation = m_pBoss->GetRotation();// 回転
	m_velocity = m_pBoss->GetVelocity();// 速度
	m_scale = m_pBoss->GetScale();// スケール
	m_initialPosition = m_pBoss->GetPosition();// 座標初期値
	m_rotationSpeed = EnemyParameters::INITIAL_ROTATION_SPEED; // 回転速度
}

/*
*	@brief 更新
*	@param[in] elapsedTime 経過時間
*	@return なし
*/
void BossIdling::Update(float elapsedTime)
{
	m_time += elapsedTime;// 時間の更新
	//m_angle = CalculateAngle(m_pBoss->GetPosition(), m_pBoss->GetEnemy()->GetPlayer()->GetPlayerPos());// プレイヤーの方向を取得し、正面を向かせる
	//m_angle = Lerp(m_angle, CalculateAngle(m_pBoss->GetEnemy()->GetPlayer()->GetPlayerPos(), m_pBoss->GetPosition()), m_time);// プレイヤーの方向を取得し、一回転させる
	//m_rotation = Quaternion::CreateFromYawPitchRoll(m_angle, 0.0f, 0.0f);// 回転
	//m_pBoss->SetRotation(m_rotation);// 回転をセット
	m_pBoss->SetVelocity(m_velocity);// 速度をセット
}

