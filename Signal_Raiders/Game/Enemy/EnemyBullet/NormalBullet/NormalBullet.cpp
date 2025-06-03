/*
*	@file NormalBullet.cpp
*	@brief 敵弾クラス(通常弾)
*/
#include <pch.h>
#include "NormalBullet.h"
/*
*	@brief コンストラクタ
*	@details 敵弾クラス(通常弾)のコンストラクタ
*	@return なし
*/
NormalBullet::NormalBullet()
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
NormalBullet::~NormalBullet()
{
	// 共通リソースのポインターをnullptrに設定
	m_pCommonResources = nullptr;
	// 敵弾のポインターをnullptrに設定
	m_pEnemyBullet = nullptr;
}
/*
*	@brief 初期化関数
*	@details 敵弾クラス(通常弾)の初期化関数
*	@param なし
*	@return なし
*/
void NormalBullet::Initialize()
{
	// 弾の座標を設定
	m_position = m_pEnemyBullet->GetPosition();
	// 弾の速度を設定
	m_velocity = m_pEnemyBullet->GetVelocity();
	// 弾の方向を設定
	m_direction = m_pEnemyBullet->GetDirection();
	// 境界球を設定
	m_boundingSphere = m_pEnemyBullet->GetBoundingSphere();
	// プレイヤーへの方向ベクトルを計算
	m_toPlayer = m_pEnemyBullet->GetTarget() - m_position;
	// ベクトルを正規化
	m_toPlayer.Normalize();
}
/*
*	@brief 更新関数
*	@details 敵弾クラス(通常弾)の更新関数
*	@param float elapsedTime 更新時間
*	@return なし
*/
void NormalBullet::Update(float elapsedTime)
{
	// プレイヤーの方向を向かせる
	m_direction = m_toPlayer;
	// 下に若干ずらす
	m_direction.y -= BulletParameters::STRAIGHT_ADJUST_DIRECTION;
	// 弾の速度を遅くする
	m_velocity = m_direction * BulletParameters::STRAIGHT_BULLET_SPEED * elapsedTime;
	// プレイヤーの方向に向かって弾を飛ばす
	m_position += m_velocity;
	//境界球に座標を渡す
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
