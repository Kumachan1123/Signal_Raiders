/*
*	@file	BossAttack.cpp
*	@brief	ボス攻撃クラス
*/
#include <pch.h>
#include "BossAttack.h"
/*
*	@brief	コンストラクタ
*	@details ボスの攻撃クラスのコンストラクタ
*	@param	pBoss	ボスAIクラスのポインタ
*	@return	なし
*/
BossAttack::BossAttack(BossAI* pBoss)
	: m_pBoss(pBoss)// ボスAIクラスのポインタ
	, m_attackCooldown{  }// 攻撃クールダウン
	, m_rotationSpeed{  }// 回転速度
	, m_pCommonResources{}// 共通リソース
{
	m_attackCooldown = ATTACK_INTERVAL;// 攻撃クールダウン
	m_rotationSpeed = ROTATION_SPEED;// 回転速度
}

/*
*	@brief	デストラクタ
*	@details ボスの攻撃クラスのデストラクタ(ここでは何もしない)
*	@param	なし
*	@return	なし
*/
BossAttack::~BossAttack() {/*do nothing*/ }

/*
*	@brief	初期化
*	@details ボスの攻撃クラスの初期化
*	@param	なし
*	@return	なし
*/
void BossAttack::Initialize()
{
	// ボスの位置を初期化
	m_position = m_pBoss->GetPosition();
	// ボスの回転を初期化
	m_rotation = m_pBoss->GetRotation();
	// ボスの速度を初期化
	m_velocity = m_pBoss->GetVelocity();
	// ボスのスケールを初期化
	m_scale = m_pBoss->GetScale();
}
/*
*	@brief	プレイヤーの方向に回転
*	@details プレイヤーの方向に回転する
*	@param playerPos プレイヤーの位置
*	@return なし
*/
void BossAttack::RotateTowardsPlayer(const DirectX::SimpleMath::Vector3& playerPos)
{
	using namespace DirectX::SimpleMath;
	// プレイヤーの方向に回転
	m_rotation = Quaternion::CreateFromYawPitchRoll(CalculateAngle(m_position, playerPos), 0.0f, 0.0f);
}

/*
*	@brief	プレイヤーの方向に移動
*	@details プレイヤーの方向に移動する
*	@param elapsedTime 経過時間
*	@param playerPos プレイヤーの位置
*	@return なし
*/
void BossAttack::MoveTowardsPlayer(float elapsedTime, const DirectX::SimpleMath::Vector3& playerPos)
{
	// プレイヤーの方向に移動
	m_position += Seek(m_pBoss->GetPosition(), playerPos, elapsedTime * EnemyParameters::BOSS_CHASE_SPEED);
}

/*
*	@brief	攻撃クールダウンの管理
*	@details 攻撃クールダウンを管理する
*	@param elapsedTime 経過時間
*	@return なし
*/
void BossAttack::ManageAttackCooldown(float elapsedTime)
{
	// クールダウンを減らす
	m_attackCooldown -= elapsedTime;
	// クールダウンが0を下回ったら
	if (m_attackCooldown <= 0.0f)
	{
		// クールダウンリセット
		m_attackCooldown = EnemyParameters::ATTACK_INTERVAL;
		// 攻撃状態に遷移
		m_pBoss->SetState(IState::EnemyState::ATTACK);
	}
}
/*
*	@brief	更新
*	@details ボスの攻撃クラスの更新
*	@param elapsedTime 経過時間
*	@return なし
*/
void BossAttack::Update(float elapsedTime)
{
	// プレイヤーの方向に回転
	RotateTowardsPlayer(m_pBoss->GetEnemy()->GetPlayer()->GetPlayerPos());
	// プレイヤーの方向に移動
	MoveTowardsPlayer(elapsedTime, m_pBoss->GetEnemy()->GetPlayer()->GetPlayerPos());
	// 攻撃クールダウンの管理
	ManageAttackCooldown(elapsedTime);
	// ボスの回転を更新
	m_pBoss->SetRotation(m_rotation);
	// ボスの速度を更新
	m_pBoss->SetVelocity(m_velocity);
	// ボスの位置を更新
	m_pBoss->SetPosition(m_position);
}
