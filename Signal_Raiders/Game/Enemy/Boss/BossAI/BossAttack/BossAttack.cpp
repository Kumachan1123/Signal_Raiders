/*
	@file	BossAttack.cpp
	@brief	ボス攻撃クラス
*/
#include "pch.h"
#include "BossAttack.h"

using namespace DirectX::SimpleMath;

/*
*	@brief	コンストラクタ
*	@param[in]	pBoss	ボスAIクラスのポインタ
*	@return	なし
*/
BossAttack::BossAttack(BossAI* pBoss)
	: m_pBoss(pBoss)// ボスAIクラスのポインタ
	, m_attackCooldown{  }// 攻撃クールダウン
	, m_rotationSpeed{  }// 回転速度
	, m_commonResources{}// 共通リソース
{
	m_attackCooldown = ATTACK_INTERVAL;// 攻撃クールダウン
	m_rotationSpeed = ROTATION_SPEED;// 回転速度
}

/*
*	@brief	デストラクタ
*	@return	なし
*/
BossAttack::~BossAttack() {}

/*
*	@brief	初期化
*	@return	なし
*/
void BossAttack::Initialize()
{
	m_position = m_pBoss->GetPosition();// 位置
	m_rotation = m_pBoss->GetRotation();// 回転
	m_velocity = m_pBoss->GetVelocity();// 速度
	m_scale = m_pBoss->GetScale();// スケール
}
/*
*	@brief	プレイヤーの方向に回転
*	@param[in] playerPos プレイヤーの位置
*	@return なし
*/
void BossAttack::RotateTowardsPlayer(DirectX::SimpleMath::Vector3 playerPos)
{
	m_rotation = DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll
	(CalculateAngle(m_position, playerPos), 0.0f, 0.0f);// プレイヤーの方向に回転
}

/*
*	@brief	プレイヤーの方向に移動
*	@param[in] elapsedTime 経過時間
*	@param[in] playerPos プレイヤーの位置
*	@return なし
*/
void BossAttack::MoveTowardsPlayer(float elapsedTime, DirectX::SimpleMath::Vector3 playerPos)
{
	m_position += Seek(m_pBoss->GetPosition(), playerPos, elapsedTime * EnemyParameters::BOSS_CHASE_SPEED);// プレイヤーの方向に移動
}

/*
*	@brief	攻撃クールダウンの管理
*	@param[in] elapsedTime 経過時間
*	@return なし
*/
void BossAttack::ManageAttackCooldown(float elapsedTime)
{
	m_attackCooldown -= elapsedTime;// クールダウンを減らす
	if (m_attackCooldown <= 0.0f)// クールダウンが0を下回ったら
	{
		m_attackCooldown = EnemyParameters::ATTACK_INTERVAL;  // クールダウンリセット
		m_pBoss->SetState(IState::EnemyState::ATTACK);  // 攻撃状態に遷移
	}
}

/*
*	@brief	更新
*	@param[in] elapsedTime 経過時間
*	@return なし
*/
void BossAttack::Update(float elapsedTime)
{
	RotateTowardsPlayer(m_pBoss->GetEnemy()->GetPlayer()->GetPlayerPos());	// プレイヤーの方向に回転
	MoveTowardsPlayer(elapsedTime, m_pBoss->GetEnemy()->GetPlayer()->GetPlayerPos());	// プレイヤーの方向に移動
	ManageAttackCooldown(elapsedTime);	// 攻撃クールダウンの管理
	// ボスの状態を更新
	m_pBoss->SetRotation(m_rotation);// 回転
	m_pBoss->SetVelocity(m_velocity);// 速度
	m_pBoss->SetPosition(m_position);// 位置
}
