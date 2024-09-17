#include "pch.h"
#include <SimpleMath.h>
#include "Game/Enemy/EnemyBullets/EnemyBullets.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

// コンストラクタ
EnemyBullets::EnemyBullets(Enemy* m_pEnemy)
	:m_commonResources{}
	, m_pEnemy{ m_pEnemy }
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
void EnemyBullets::Update(float elapsedTime, DirectX::SimpleMath::Vector3 enemyPos)
{
	std::vector<std::unique_ptr<EnemyBullet>> newBullets;
	Vector3 position = m_pEnemy->GetPosition();
	Vector3 playerPos = m_pEnemy->GetPlayer()->GetCamera()->GetEyePosition();
	Vector3 playerTarget = m_pEnemy->GetPlayer()->GetCamera()->GetTargetPosition();
	Vector3 playerUp = m_pEnemy->GetPlayer()->GetCamera()->GetUpVector();

	// 弾の更新と有効な弾を新しいリストに移動する
	for (auto& bullet : m_bullets)
	{
		bullet->SetCameraEye(playerPos);// カメラの位置を設定
		bullet->SetCameraTarget(playerTarget);// カメラの注視点を設定
		bullet->SetCameraUp(playerUp);	// カメラの上方向を設定
		bullet->Update(enemyPos, elapsedTime); // 弾の更新

		// 寿命を迎えていない弾だけを新しいリストに追加する
		if (!bullet->IsExpired() && !m_pEnemy->GetBulletHitToPlayer())
		{
			m_pEnemy->SetBulletBoundingSphere(bullet->GetBoundingSphere());
			m_pEnemy->SetBulletHitToPlayer(m_pEnemy->GetBulletBoundingSphere().Intersects(m_pEnemy->GetPlayerBoundingSphere()));
			if (m_pEnemy->GetBulletHitToPlayer())
			{
				m_pEnemy->SetBulletHitToPlayer(m_pEnemy->GetBulletHitToPlayer());

				continue;
			}
			newBullets.push_back(std::move(bullet));
		}
	}

	// m_bullets を新しいリストで置き換える
	m_bullets = std::move(newBullets);
}

// 描画
void EnemyBullets::Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)

{
	for (const auto& bullet : m_bullets)bullet->Render(view, proj);
}

// 弾の生成
void EnemyBullets::CreateBullet(Vector3 enemyPos, Vector3 dir, Vector3 playerPos)
{
	m_position = enemyPos;
	// 弾を発射
	auto bullet = std::make_unique<EnemyBullet>();
	bullet->Initialize(m_commonResources);
	bullet->MakeBall(m_position, dir, playerPos);
	m_bullets.push_back(std::move(bullet));
}
