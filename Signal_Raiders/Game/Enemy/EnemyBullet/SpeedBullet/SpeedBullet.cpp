/*
*	@file SpeedBullet.cpp
*	@brief 敵弾クラス(速い弾)
*/
#include <pch.h>
#include "SpeedBullet.h"
/*
*	@brief コンストラクタ
*	@details 敵弾クラス(速い弾)のコンストラクタ
*	@param なし
*	@return なし
*/
SpeedBullet::SpeedBullet()
	: m_pEnemyBullet(nullptr)// 敵弾ポインター
	, m_pCommonResources(nullptr)// 共通リソース
	, m_seVolume(0.0f)// SE音量
{
}
/*
*	@brief デストラクタ
*	@details 各種ポインターをnullptrに設定
*	@param なし
*	@return なし
*/
SpeedBullet::~SpeedBullet()
{
	// 共通リソースのポインターをnullptrに設定
	m_pCommonResources = nullptr;
	// 敵弾のポインターをnullptrに設定
	m_pEnemyBullet = nullptr;
}
/*
*	@brief 初期化関数
*	@details 敵弾クラス(速い弾)の初期化関数
*	@param なし
*	@return なし
*/
void SpeedBullet::Initialize()
{
	// 弾の座標の設定
	m_position = m_pEnemyBullet->GetPosition();
	// 弾の速度の設定
	m_velocity = m_pEnemyBullet->GetVelocity();
	// 弾の方向の設定
	m_direction = m_pEnemyBullet->GetDirection();
	// 弾の境界球の設定
	m_boundingSphere = m_pEnemyBullet->GetBoundingSphere();
}
/*
*	@brief 更新関数
*	@details 敵弾クラス(速い弾)の更新関数
*	@param float elapsedTime 更新時間
*	@return なし
*/
void SpeedBullet::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;
	// 着弾位置に到達していない
	if (m_position.y >= BulletParameters::VERTICAL_BULLET_LANDING_POSITION)
	{
		// 真下に落とす
		m_velocity = BulletParameters::VERTICAL_BULLET_LANDING_VELOCITY * elapsedTime * BulletParameters::VERTICAL_BULLET_SPEED;
	}
	else//着弾したら
	{
		// プレイヤーの方向ベクトルを計算
		Vector3 toPlayer = m_pEnemyBullet->GetTarget() - m_pEnemyBullet->GetEnemyPosition();
		// ベクトルを正規化
		if (toPlayer.LengthSquared() > 0)toPlayer.Normalize();
		// 弾の方向をプレイヤーの方向に向ける
		m_direction = Vector3(toPlayer.x, 0, toPlayer.z) * elapsedTime * BulletParameters::VERTICAL_BULLET_SPEED;
		// 弾の速度を設定
		m_velocity = m_direction;
	}
	// プレイヤーの方向に向かって弾を飛ばす
	m_position += m_velocity;
	// 境界球の中心座標を弾の座標に更新
	m_boundingSphere.Center = m_position;
	// 弾の方向を更新
	m_pEnemyBullet->SetDirection(m_direction);
	// 弾の速度更新
	m_pEnemyBullet->SetVelocity(m_velocity);
	// 弾の座標を更新
	m_pEnemyBullet->SetPosition(m_position);
	// 弾の境界球を更新
	m_pEnemyBullet->SetBoundingSphere(m_boundingSphere);
}
