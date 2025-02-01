/*
	@file	EnemyBullets.cpp
	@brief	敵の弾全体を管理するクラス
*/
#include "pch.h"
#include <SimpleMath.h>
#include "Game/Enemy/EnemyBullets/EnemyBullets.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

// コンストラクタ
EnemyBullets::EnemyBullets(IEnemy* pEnemy)
	:m_commonResources{ nullptr }
	, m_pEnemy{ pEnemy }
{
}

// デストラクタ
EnemyBullets::~EnemyBullets()
{
}

// 初期化
void EnemyBullets::Initialize(CommonResources* resources)
{
	m_commonResources = resources;
}

// 更新
void EnemyBullets::Update(float elapsedTime)
{
	std::vector<std::unique_ptr<EnemyBullet>> newBullets;
	Vector3 enemyPosition = m_pEnemy->GetPosition();
	Vector3 playerPos = m_pEnemy->GetCamera()->GetEyePosition();
	Vector3 playerTarget = m_pEnemy->GetCamera()->GetTargetPosition();
	Vector3 playerUp = m_pEnemy->GetCamera()->GetUpVector();
	// すべての弾を更新
	for (auto& bullet : m_bullets)
	{
		bullet->SetCameraEye(playerPos);
		bullet->SetCameraTarget(playerTarget);
		bullet->SetCameraUp(playerUp);
		bullet->SetEnemyPosition(enemyPosition);
		bullet->Update(elapsedTime);

		// プレイヤーとの当たり判定
		m_pEnemy->SetBulletBoundingSphere(bullet->GetBoundingSphere());
		if (m_pEnemy->GetBulletBoundingSphere().Intersects(m_pEnemy->GetPlayerBoundingSphere()))
		{
			m_pEnemy->SetPlayerHitByEnemyBullet(true);
			m_pEnemy->GetPlayer()->SetEnemyBulletDirection(bullet->GetBulletDirection());
			m_pEnemy->GetPlayer()->SetisPlayEffect(true);
			m_pEnemy->GetPlayer()->SetisPlayerDamage(true);
			continue; // 当たった弾は処理しない
		}
	}

	// 失効した弾を削除
	m_bullets.erase(std::remove_if(m_bullets.begin(), m_bullets.end(),
		[this](const std::unique_ptr<EnemyBullet>& bullet)
		{	// 条件リスト
			// ① 弾が寿命を迎えた
			// ② Y座標が0以下（ただし垂直弾は例外）
			// ③ プレイヤーに当たった
			return bullet->IsExpired() ||
				(bullet->GetBulletPosition().y < 0.0f && bullet->GetBulletType() != EnemyBullet::BulletType::VERTICAL) ||
				m_pEnemy->GetPlayerHitByEnemyBullet();
		}),
		m_bullets.end());
}

// 描画
void EnemyBullets::Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	for (const auto& bullet : m_bullets)
	{
		bullet->RenderShadow(view, proj);// 影の描画
		bullet->Render(view, proj);// 弾の描画
		bullet->RenderBoundingSphere(view, proj);// 弾の境界球描画
	}

}

// 弾の生成
void EnemyBullets::CreateBullet(float size, EnemyBullet::BulletType type)
{
	// プレイヤーからカメラの情報を取得
	Vector3 playerPos = m_pEnemy->GetCamera()->GetEyePosition();
	// 弾を発射する
	auto bullet = std::make_unique<EnemyBullet>(size);
	bullet->Initialize(m_commonResources, type);
	bullet->MakeBall(m_bulletSpawnPos, m_direction, playerPos);
	m_bullets.push_back(std::move(bullet));
}

// 螺旋弾の回転方向を設定
void EnemyBullets::SetRotateDirection(int direction)
{
	for (auto& bullet : m_bullets)bullet->SetRotateDirection(direction);
}