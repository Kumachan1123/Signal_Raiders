#include <pch.h>
#include "SpecialBullet.h"
using namespace DirectX::SimpleMath;
SpecialBullet::SpecialBullet()
	: m_pEnemyBullet(nullptr)
	, m_rotationSpeed(0.0f)
	, m_distance(5.0f)
	, m_height(0.0f)
	, m_spiralAngle(0.0f)
	, m_elapsedTime(0.0f)
	, m_time(0.0f)
	, m_basePos(Vector3::Zero)
	, m_positionOffSet(Vector3::Zero)
	, m_currentTarget(Vector3::Zero)
{
}

SpecialBullet::~SpecialBullet()
{
}

void SpecialBullet::Initialize()
{
	m_position = m_pEnemyBullet->GetPosition();// 弾の座標
	m_velocity = m_pEnemyBullet->GetVelocity();// 弾の速度
	m_direction = m_pEnemyBullet->GetDirection();// 弾の方向
	m_spiralAngle = m_pEnemyBullet->GetAngle();// 回転弾の角度
	m_boundingSphere = m_pEnemyBullet->GetBoundingSphere();// 「弾」境界球

}

void SpecialBullet::Update(float elapsedTime)
{
	m_time += elapsedTime;
	m_elapsedTime = elapsedTime;
	// 時計回りに回転するための角度
	m_spiralAngle += m_rotationSpeed * elapsedTime;
	// XY平面上で円運動 (時計回り)
	float xOffset = cosf(m_spiralAngle) * m_distance;
	float zOffset = sinf(m_spiralAngle) * m_distance;
	// もともとのY座標の動きは変更しない
	m_positionOffSet = Vector3(xOffset, m_basePos.y - 3.5f, zOffset);
	Expand();// 子オブジェクトを展開
	Shot();// 子オブジェクトを発射
	StopExpand();// 子オブジェクトを収納
	ComeBack();// 子オブジェクトを戻す
	// プレイヤーに向かいつつスパイラルを描いて移動
	m_position = m_basePos + m_positionOffSet;
	m_boundingSphere.Center = m_position;
	// 弾の寿命に応じてフラグを切り替える
	if (m_time >= BulletParameters::SPECIAL_ATTACK_WAIT_TIME)
	{
		m_pEnemyBullet->SetIsShot(true);
	}
	// 弾の各種情報を更新
	m_pEnemyBullet->SetDirection(m_direction);// 弾の方向
	m_pEnemyBullet->SetVelocity(m_velocity);// 弾の速度
	m_pEnemyBullet->SetPosition(m_position);// 弾の座標
	m_pEnemyBullet->SetBoundingSphere(m_boundingSphere);// 「弾」境界球
}

void SpecialBullet::Expand()
{
	if (!m_pEnemyBullet->GetIsExpand())return;
	m_rotationSpeed = 1.0f; // 速度調整用（値を大きくすると速く回転する）
	m_distance = Lerp(m_distance, 15.0f, m_elapsedTime);
	m_height = 2.0f;
}

void SpecialBullet::Shot()
{
	if (!m_pEnemyBullet->GetIsShot()) return;
	m_rotationSpeed = 3.0f;
	m_distance = Lerp(m_distance, 5.0f, m_elapsedTime);
	m_basePos = Lerp(m_basePos, m_pEnemyBullet->GetCurrentTarget(), m_elapsedTime * 2);

}

void SpecialBullet::StopExpand()
{
	if (m_pEnemyBullet->GetIsExpand())return;
	m_rotationSpeed = 0.0f;
	m_distance = Lerp(m_distance, 0.0f, m_elapsedTime * 20);
	m_height = 1.50f;
}

void SpecialBullet::ComeBack()
{
	if (m_pEnemyBullet->GetIsShot()) return;
	//m_look.Normalize();// プレイヤーが向いている方向を正規化
	//// 基準点を親が向いている方向に動かす
	m_distance = Lerp(m_distance, 3.0f, m_elapsedTime);
	// 基準点を目的地に向かって線形補完
	m_basePos = Lerp(m_basePos, m_pEnemyBullet->GetEnemyPosition(), m_elapsedTime * 50);
}
