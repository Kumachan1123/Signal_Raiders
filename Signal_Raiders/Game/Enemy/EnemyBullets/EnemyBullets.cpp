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
	:m_commonResources{}
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

	// 弾の更新と有効な弾を新しいリストに移動する
	for (auto& bullet : m_bullets)
	{
		bullet->SetCameraEye(playerPos);// カメラの位置を設定
		bullet->SetCameraTarget(playerTarget);// カメラの注視点を設定
		bullet->SetCameraUp(playerUp);	// カメラの上方向を設定
		bullet->SetEnemyPosition(enemyPosition);// 敵の位置を設定
		bullet->Update(elapsedTime); // 弾の更新

		// 寿命を迎えていない弾だけを新しいリストに追加する
		if (!bullet->IsExpired() && !m_pEnemy->GetPlayerHitByEnemyBullet())
		{
			m_pEnemy->SetBulletBoundingSphere(bullet->GetBoundingSphere());
			m_pEnemy->SetPlayerHitByEnemyBullet(m_pEnemy->GetBulletBoundingSphere().Intersects(m_pEnemy->GetPlayerBoundingSphere()));
			if (m_pEnemy->GetPlayerHitByEnemyBullet())
			{
				m_pEnemy->SetPlayerHitByEnemyBullet(m_pEnemy->GetPlayerHitByEnemyBullet());// プレイヤーに当たったフラグを設定
				m_pEnemy->GetPlayer()->SetEnemyBulletDirection(bullet->GetBulletDirection());// プレイヤーにダメージを与える敵の向きを設定
				m_pEnemy->GetPlayer()->SetisPlayEffect(true);// エフェクト再生フラグを設定
				m_pEnemy->GetPlayer()->SetisPlayerDamage(true);// プレイヤーにダメージを与える
				continue;
			}
			// 弾のY座標が-1以下になったら弾を消す
			if (bullet->GetBulletPosition().y < 0.0f && bullet->GetBulletType() != EnemyBullet::BulletType::VERTICAL)continue;
			newBullets.push_back(std::move(bullet));
		}
	}

	// m_bullets を新しいリストで置き換える
	m_bullets = std::move(newBullets);
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
	bullet->MakeBall(m_enemyPosition, m_direction, playerPos);
	m_bullets.push_back(std::move(bullet));
}

// 螺旋弾の回転方向を設定
void EnemyBullets::SetRotateDirection(int direction)
{
	for (auto& bullet : m_bullets)bullet->SetRotateDirection(direction);
}