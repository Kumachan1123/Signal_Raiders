/*
*	@file NormalBullet.cpp
*	@brief 敵弾クラス(通常弾)
*/
#include <pch.h>
#include "NormalBullet.h"
/*
*	@brief コンストラクタ
*	@return なし
*/
NormalBullet::NormalBullet()
	: m_pEnemyBullet(nullptr)// 敵弾ポインター
{
}
/*
*	@brief デストラクタ
*	@return なし
*/
NormalBullet::~NormalBullet() {/*do nothing*/ }
/*
*	@brief 初期化関数
*	@return なし
*/
void NormalBullet::Initialize()
{
	m_position = m_pEnemyBullet->GetPosition();// 弾の座標
	m_velocity = m_pEnemyBullet->GetVelocity();// 弾の速度
	m_direction = m_pEnemyBullet->GetDirection();// 弾の方向
	m_boundingSphere = m_pEnemyBullet->GetBoundingSphere();// 「弾」境界球
	m_toPlayer = m_pEnemyBullet->GetTarget() - m_position;// // プレイヤーの方向ベクトルを計算
	m_toPlayer.Normalize();// ベクトルを正規化
}
/*
*	@brief 更新関数
*	@param[in] float elapsedTime 更新時間
*	@return なし
*/
void NormalBullet::Update(float elapsedTime)
{
	m_direction = m_toPlayer;// プレイヤーの方向を向かせる
	m_direction.y -= BulletParameters::STRAIGHT_ADJUST_DIRECTION;// 下に若干ずらす
	m_velocity = m_direction * BulletParameters::STRAIGHT_BULLET_SPEED * elapsedTime;	// 弾の速度を遅くする
	m_position += m_velocity;// プレイヤーの方向に向かって弾を飛ばす
	m_boundingSphere.Center = m_position;//境界球に座標を渡す
	// 弾の各種情報を更新
	m_pEnemyBullet->SetDirection(m_direction);// 弾の方向
	m_pEnemyBullet->SetVelocity(m_velocity);// 弾の速度
	m_pEnemyBullet->SetPosition(m_position);// 弾の座標
	m_pEnemyBullet->SetBoundingSphere(m_boundingSphere);// 「弾」境界球
}
