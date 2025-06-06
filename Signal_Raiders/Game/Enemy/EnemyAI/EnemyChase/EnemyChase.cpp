/*
*	@file	EnemyChase.cpp
*	@brief	敵追跡クラス
*/
#include <pch.h>
#include "EnemyChase.h"
/*
*	@brief	コンストラクタ
*	@details 敵追跡クラスのコンストラクタ
*	@param	EnemyAI* pEnemyAI 敵AI
*	@return	なし
*/
EnemyChase::EnemyChase(EnemyAI* pEnemyAI)
	: m_pEnemyAI(pEnemyAI)// 敵AIのポインター
	, m_rotation(DirectX::SimpleMath::Quaternion::Identity) // 回転の初期化
	, m_velocity(DirectX::SimpleMath::Vector3::Zero) // 速度の初期化
	, m_scale(DirectX::SimpleMath::Vector3::One) // スケールの初期化
	, m_initialPosition(DirectX::SimpleMath::Vector3::Zero) // 初期位置の初期化
	, m_position(DirectX::SimpleMath::Vector3::Zero) // 現在位置の初期化
	, m_time(0.0f) // 時間の初期化
	, m_angle(0.0f) // 角度の初期化
	, m_attackCooldown(EnemyParameters::ATTACK_COOLDOWN) // 攻撃のクールダウンタイムの初期化
{
}
/*
*	@brief	デストラクタ
*	@details 各種ポインターをnullptrに設定
*	@param	なし
*	@return	なし
*/
EnemyChase::~EnemyChase()
{
	// 敵AIのポインターをnullptrに設定
	m_pEnemyAI = nullptr;
}
/*
*	@brief	初期化
*	@details 敵追跡クラスの初期化
*	@param	なし
*	@return	なし
*/
void EnemyChase::Initialize()
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
	// AIから攻撃のクールダウンタイムを取得して設定する
	m_attackCooldown = EnemyParameters::ATTACK_COOLDOWN;
}
/*
*	@brief	更新
*	@details 敵スピンクラスの更新
*	@param	float elapsedTime　経過時間
*	@return	なし
*/
void EnemyChase::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;
	// プレイヤーのポインターを取得
	Player* player = m_pEnemyAI->GetEnemy()->GetPlayer();
	// プレイヤーの座標を取得
	Vector3 playerPos = player->GetPlayerPos();
	// 時間の加算
	m_time += elapsedTime;
	// プレイヤーとの距離を計算
	float distance = (playerPos - m_position).Length();
	// プレイヤーとの距離に応じて処理を分岐
	if (distance > STOP_DISTANCE)// 停止距離より長い場合
	{
		// プレイヤーに近づくための速度を計算
		m_velocity = Seek(m_position, playerPos, elapsedTime * 5);
	}
	else// 停止距離以下の場合
	{
		// プレイヤーとの距離が近いので停止
		m_velocity = Vector3::Zero;
	}
	// 進行方向に回転
	if (m_velocity.LengthSquared() > 0.0f) // 速度がゼロでない場合
	{
		// 進行方向を向く
		m_rotation = Quaternion::CreateFromYawPitchRoll(atan2f(m_velocity.x, m_velocity.z), 0.0f, 0.0f);
	}
	// プレイヤーの位置ベクトルを計算
	Vector3 toPlayer = playerPos - m_position;
	// ベクトルを正規化
	toPlayer.Normalize();
	// プレイヤーが見ている方向を取得
	Vector3 playerLook = player->GetPlayerDir();
	// プレイヤーの視線と敵の位置ベクトルのドット積を計算
	float dot = playerLook.Dot(toPlayer);
	// ドット積から角度を計算
	m_angle = acosf(dot);
	// クールダウンを減らす
	m_attackCooldown -= elapsedTime;
	// AIにクールダウンタイムを設定
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
			// AIにも設定
			m_pEnemyAI->SetAttackCooldown(m_attackCooldown);
			// 攻撃中にする
			m_pEnemyAI->SetIsAttack(true);
		}
	}
	else
	{
		// 視界に入ったら回り込みに遷移
		if (m_angle > DirectX::XM_PI / 2)
		{
			//攻撃態勢にする
			m_pEnemyAI->ChangeState(m_pEnemyAI->GetEnemyShadowStep());
		}
	}

	// 位置を更新
	m_position += m_velocity;
	// 敵の位置を更新
	m_pEnemyAI->SetPosition(m_position);
	// 敵の回転を更新
	m_pEnemyAI->SetRotation(m_rotation);
	// 敵の速度を更新
	m_pEnemyAI->SetVelocity(m_velocity);

}
