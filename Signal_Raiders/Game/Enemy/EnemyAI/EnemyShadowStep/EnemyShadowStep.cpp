/*
*	@file	EnemyShadowStep.cpp
*	@brief	敵シャドウステップクラス
*/
#include <pch.h>
#include "EnemyShadowStep.h"
/*
*	@brief	コンストラクタ
*	@details 敵シャドウステップクラスのコンストラクタ
*	@param	EnemyAI* pEnemyAI 敵AI
*	@return	なし
*/
EnemyShadowStep::EnemyShadowStep(EnemyAI* pEnemyAI)
	: m_pEnemyAI(pEnemyAI)// 敵AIのポインター
	, m_rotation(DirectX::SimpleMath::Quaternion::Identity) // 回転の初期化
	, m_velocity(DirectX::SimpleMath::Vector3::Zero) // 速度の初期化
	, m_scale(DirectX::SimpleMath::Vector3::One) // スケールの初期化
	, m_initialPosition(DirectX::SimpleMath::Vector3::Zero) // 初期位置の初期化
	, m_position(DirectX::SimpleMath::Vector3::Zero) // 現在位置の初期化
	, m_knockStartPosition(DirectX::SimpleMath::Vector3::Zero) // ノックバック開始位置の初期化
	, m_knockEndPosition(DirectX::SimpleMath::Vector3::Zero) // ノックバック終了位置の初期化
	, m_initialVelocity(DirectX::SimpleMath::Vector3::Zero) // ノックバックの初期速度の初期化
	, m_time(0.0f) // 時間の初期化
	, m_angle(0.0f) // 角度の初期化
	, m_initialized(false) // 初期化フラグの初期化
	, m_randomRadius(0.0f) // ランダムな半径の初期化
	, m_elapsedAngle(0.0f) // 累積角度の初期化
	, m_rotateDirection(1) // 回転方向の初期化
	, m_evadeRadius(0.0f) // 回り込みの半径の初期化
{
}
/*
*	@brief	デストラクタ
*	@details 各種ポインターをnullptrに設定
*	@param	なし
*	@return	なし
*/
EnemyShadowStep::~EnemyShadowStep()
{
	// 敵AIのポインターをnullptrに設定
	m_pEnemyAI = nullptr;
}
/*
*	@brief	初期化
*	@details 敵シャドウステップクラスの初期化
*	@param	なし
*	@return	なし
*/
void EnemyShadowStep::Initialize()
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
}
/*
*	@brief	更新
*	@details 敵スピンクラスの更新
*	@param	float elapsedTime　経過時間
*	@return	なし
*/
void EnemyShadowStep::Update(float elapsedTime)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// 時間の加算
	m_time += elapsedTime;
	// プレイヤーのポインターを取得
	Player* player = m_pEnemyAI->GetEnemy()->GetPlayer();
	// プレイヤーの座標を取得
	Vector3 playerPos = player->GetPlayerPos();
	// AIから位置を取得して設定する
	m_position = m_pEnemyAI->GetPosition();
	// ChaseからShadowStepになったとき
	if (!m_initialized)
	{
		// 10から30の範囲
		m_randomRadius = MinRandomRadius + rand() % (int)RadiusRange;
		// 回り込みの半径を設定
		m_evadeRadius = m_randomRadius;
		// 回り込みの中心はプレイヤーの初期位置
		m_centerPos = player->GetPlayerPos();
		// 敵の位置からプレイヤーの初期位置までのベクトルを計算
		Vector3 toPlayer = m_position - m_centerPos;
		// 求めたベクトルから角度を計算
		m_angle = atan2f(toPlayer.z, toPlayer.x);
		// 回転の途中経過を初期化
		m_elapsedAngle = 0.0f;
		// 回り込みのための角度をランダムに設定
		m_rotateDirection = (rand() % 2 == 0) ? 1 : -1;
		// 初期化フラグをtrueに設定
		m_initialized = true;
	}
	// 回転角度更新
	float deltaAngle = XM_PI * elapsedTime;
	// 回り込みの角度を更新
	m_angle += deltaAngle * m_rotateDirection;
	// 回り込みの位置を計算
	m_elapsedAngle += deltaAngle;
	// プレイヤーの初期位置を中心とした円上の目標座標
	float x = cosf(m_angle) * m_evadeRadius;// X座標
	float z = sinf(m_angle) * m_evadeRadius;// Z座標
	// 目標座標を計算
	m_targetPos = m_centerPos + Vector3(x, 0.0f, z);
	// 現在位置からその目標地点へのベクトル
	Vector3 direction = m_targetPos - m_position;
	// 方向ベクトルを正規化
	direction.Normalize();
	// 方向ベクトルに速度を掛けて移動量を計算
	m_velocity = direction * 50.0f * elapsedTime;
	// 敵の回転を更新
	if (m_velocity.LengthSquared() > 0.0f)
	{
		// 進行方向を向く
		m_rotation = Quaternion::CreateFromYawPitchRoll
		(atan2f(m_velocity.x, m_velocity.z), 0.0f, 0.0f);
		// 敵AIに回転を設定
		m_pEnemyAI->SetRotation(m_rotation);
	}
	// 位置を更新
	m_position += m_velocity;
	// 新しい位置を設定
	m_pEnemyAI->SetPosition(m_position);
	// 半周したら終了
	if (m_elapsedAngle >= XM_PI * 3)
	{
		// 初期化フラグをfalseに設定
		m_initialized = false;
		// プレイヤーの位置を取得
		m_pEnemyAI->ChangeState(m_pEnemyAI->GetEnemyChase());
	}
}
