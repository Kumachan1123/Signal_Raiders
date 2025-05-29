/*
*	@file	BossAI.cpp
*	@brief	敵AIクラス
*/
#include <pch.h>
#include "BossAI.h"
/*
*	@brief	コンストラクタ
*	@details ボスクラスのAIを管理するクラス
*	@param pBoss ボスクラスのポインタ
*	@return なし
*/
BossAI::BossAI(IEnemy* pBoss)
	: m_pCurrentState(nullptr)// 現在のステート
	, m_attackCooldown(0.0f)// 攻撃クールダウン
	, m_knockTime(0.0f)// ノックバック時間
	, m_time(0.0f)// 時間
	, m_pBossAttack(nullptr)// 攻撃時
	, m_enemyState(IState::EnemyState::ATTACK)// 状態
	, m_attackState(IState::EnemyState::ATTACK)// 攻撃時表情差分
	, m_pBoss(pBoss)// ボス
	, m_isHitPlayerBullet(false)// プレイヤーの弾に当たったか
	, m_isKnockBack(false)// ノックバック中か
	, m_isAttack(false)// 攻撃中か
{
	m_pBossAttack = std::make_unique<BossAttack>(this);// 攻撃時
	m_pBossKnockBacking = std::make_unique<BossKnockBacking>(this);// ノックバック時
}

/*
*	@brief デストラクタ
*	@details ボスAIクラスのデストラクタ(ここでは何もしない)
*	@param なし
*	@return なし
*/
BossAI::~BossAI() {/*do nothing*/ }

/*
*	@brief 初期化
*	@details ボスAIクラスの初期化
*	@param なし
*	@return なし
*/
void BossAI::Initialize()
{
	m_initialPosition = m_pBoss->GetPosition();  // 初期位置を保存
	m_velocity = EnemyParameters::INITIAL_VELOCITY; // 浮遊の初期速度
	m_scale = EnemyParameters::INITIAL_BOSS_SCALE; // スケール初期化
	m_position = m_initialPosition;// 初期位置を設定
	m_pCurrentState = m_pBossAttack.get();// 現在のステートを攻撃態勢に設定
	m_pCurrentState->Initialize();// 初期化
	m_enemyState = m_attackState;// 攻撃状態
}
/*
*	@brief 更新
*	@details ボスAIクラスの更新
*	@param elapsedTime 経過時間
*	@return なし
*/
void BossAI::Update(float elapsedTime)
{
	if (m_pBoss->GetEnemyHitByPlayerBullet())m_isHitPlayerBullet = true;// プレイヤーの弾に当たったか
	m_time += elapsedTime;// 時間の更新
	m_position = m_pBoss->GetPosition();// 位置の更新
	m_pCurrentState->Update(elapsedTime);// 現在のステートを更新
	m_position.y = m_initialPosition.y + EnemyParameters::AMPLITUDE * std::sin(EnemyParameters::FREQUENCY * m_time);// Y座標を更新
	m_position.y += m_velocity.y * elapsedTime;// Y座標を更新
	auto boss = dynamic_cast<BossBase*>(m_pBoss);// IEnemyからBossのポインターを抽出
	if (!m_isKnockBack && boss->GetBossSheild()->GetSheildHP() <= 0)	// シールドが壊されたらノックバック
	{
		ChangeState(m_pBossKnockBacking.get());//ノックバック状態にする
		m_enemyState = IState::EnemyState::HIT;// 攻撃を受けた時の動き
		SetIsAttack(false);// 攻撃中ではない
	}
	else // シールドが壊されていない場合
	{
		ChangeState(m_pBossAttack.get());//攻撃態勢にする
		SetIsAttack(true);// 攻撃中にする
		m_enemyState = m_attackState;// 攻撃状態
	}
	m_pBoss->SetPosition(m_position);// 位置をセット
}
/*
*	@brief ステート変更
*	@details ボスAIクラスのステート変更
*	@param newState 新しいステート
*	@return なし
*/
void BossAI::ChangeState(IState* newState)
{
	if (m_pCurrentState != newState)// 現在のステートと新しいステートが違う場合
	{
		m_pCurrentState = newState;// 現在のステートを新しいステートに変更
		m_pCurrentState->Initialize();// 初期化
	}
}

