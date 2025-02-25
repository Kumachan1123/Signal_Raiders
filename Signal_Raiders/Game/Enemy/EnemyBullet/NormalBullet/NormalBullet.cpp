#include <pch.h>
#include "NormalBullet.h"
using namespace DirectX::SimpleMath;

NormalBullet::NormalBullet()
	: m_pEnemyBullet(nullptr)
{
}

NormalBullet::~NormalBullet()
{
}

void NormalBullet::Initialize()
{
	m_position = m_pEnemyBullet->GetPosition();// 弾の座標
	m_velocity = m_pEnemyBullet->GetVelocity();// 弾の速度
	m_direction = m_pEnemyBullet->GetDirection();// 弾の方向
	m_boundingSphere = m_pEnemyBullet->GetBoundingSphere();// 「弾」境界球

}

void NormalBullet::Update(float elapsedTime)
{
	// プレイヤーの方向ベクトルを計算
	Vector3 toPlayer = m_pEnemyBullet->GetTarget() - m_pEnemyBullet->GetEnemyPosition();
	// ベクトルを正規化
	if (toPlayer.LengthSquared() > 0)
	{
		toPlayer.Normalize();
	}
	// 弾の方向をプレイヤーの方向に向ける
	m_direction = toPlayer;
	m_direction.y -= BulletParameters::STRAIGHT_ADJUST_DIRECTION;// 下に若干ずらす
	// 弾の速度を遅くする
	m_velocity = m_direction * BulletParameters::STRAIGHT_BULLET_SPEED * elapsedTime;
	// プレイヤーの方向に向かって弾を飛ばす
	m_position += m_velocity;
	m_boundingSphere.Center = m_position;//境界球に座標を渡す
	// 弾の各種情報を更新
	m_pEnemyBullet->SetDirection(m_direction);// 弾の方向
	m_pEnemyBullet->SetVelocity(m_velocity);// 弾の速度
	m_pEnemyBullet->SetPosition(m_position);// 弾の座標
	m_pEnemyBullet->SetBoundingSphere(m_boundingSphere);// 「弾」境界球
}
