#include <pch.h>
#include "SpeedBullet.h"
using namespace DirectX::SimpleMath;

SpeedBullet::SpeedBullet()
	: m_pEnemyBullet(nullptr)
{
}

SpeedBullet::~SpeedBullet()
{
}

void SpeedBullet::Initialize()
{
	m_position = m_pEnemyBullet->GetPosition();// 弾の座標
	m_velocity = m_pEnemyBullet->GetVelocity();// 弾の速度
	m_direction = m_pEnemyBullet->GetDirection();// 弾の方向
	m_boundingSphere = m_pEnemyBullet->GetBoundingSphere();// 「弾」境界球
}

void SpeedBullet::Update(float elapsedTime)
{
	if (m_position.y >= BulletParameters::VERTICAL_BULLET_LANDING_POSITION)// 着弾位置に到達していない
	{
		// 真下に落とす
		m_velocity = BulletParameters::VERTICAL_BULLET_LANDING_VELOCITY * elapsedTime * BulletParameters::VERTICAL_BULLET_SPEED;
	}
	else//着弾してから
	{
		// プレイヤーの方向ベクトルを計算
		Vector3 toPlayer = m_pEnemyBullet->GetTarget() - m_pEnemyBullet->GetEnemyPosition();
		// ベクトルを正規化
		if (toPlayer.LengthSquared() > 0)
		{
			toPlayer.Normalize();
		}
		// 弾の方向をプレイヤーの方向に向ける
		m_direction = Vector3(toPlayer.x, 0, toPlayer.z) * elapsedTime * BulletParameters::VERTICAL_BULLET_SPEED;
		m_velocity = m_direction;
	}
	m_position += m_velocity;
	m_boundingSphere.Center = m_position;
	// 弾の各種情報を更新
	m_pEnemyBullet->SetDirection(m_direction);// 弾の方向
	m_pEnemyBullet->SetVelocity(m_velocity);// 弾の速度
	m_pEnemyBullet->SetPosition(m_position);// 弾の座標
	m_pEnemyBullet->SetBoundingSphere(m_boundingSphere);// 「弾」境界球
}
