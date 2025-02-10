#include "pch.h"
#include "BulletManager.h"
#include "Game/Player/Player.h"
#include "Game/Enemy/EnemyManager/EnemyManager.h"

using namespace DirectX::SimpleMath;

// コンストラクタ
BulletManager::BulletManager(CommonResources* commonResources)
	:m_commonResources{ commonResources }
	, m_pPlayer{ nullptr }
	, m_pEnemyManager{ nullptr }
	, m_pShooter{ nullptr }
	, m_audioManager{ AudioManager::GetInstance() }
	, m_enemyBulletType{ EnemyBullet::BulletType::STRAIGHT }
	, m_enemyBulletSize{ 0.0f }
	, m_playerBulletCount{ 0 }
	, m_reloadTimer{ 0.0f }
	, m_elapsedTime{ 0.0f }
{
}
// デストラクタ
BulletManager::~BulletManager()
{
	// プレイヤーの弾を削除
	for (auto& bullet : m_playerBullets)bullet.reset();
	m_playerBullets.clear();
	// 敵の弾を削除
	for (auto& bullet : m_enemyBullets)bullet.reset();
	m_enemyBullets.clear();
}
// 初期化
void BulletManager::Initialize(Player* pPlayer, EnemyManager* pEnemies)
{
	m_pPlayer = pPlayer;// プレイヤーのポインターを取得
	m_pEnemyManager = pEnemies;// 敵全体のポインターを取得

	// 効果音の初期化
	SetSound();

	// プレイヤーの弾の初期化
	m_playerBulletCount = MAX_PLAYER_BULLET_COUNT;
}

// 更新
void BulletManager::Update(float elapsedTime)
{
	// 経過時間を更新
	m_elapsedTime = elapsedTime;
	// プレイヤーの弾を更新
	UpdatePlayerBullets(elapsedTime);
	// 敵の弾を更新
	for (auto& enemy : m_pEnemyManager->GetEnemies())UpdateEnemyBullets(elapsedTime, enemy);
	// 弾同士の当たり判定
	CheckCollisionWithBullets();

}
// 描画
void BulletManager::Render()
{
	auto camera = m_pPlayer->GetCamera();
	Matrix view = camera->GetViewMatrix();
	Matrix proj = camera->GetProjectionMatrix();
	// プレイヤー弾の描画
	for (const auto& bullet : m_playerBullets)
	{
		bullet->Render(view, proj);
		bullet->RenderShadow(view, proj);
	}

	// 敵弾の描画
	for (const auto& bullet : m_enemyBullets)
	{
		bullet->Render(view, proj);
		bullet->RenderShadow(view, proj);
	}
}
void BulletManager::CreatePlayerBullet(const DirectX::SimpleMath::Vector3& position, DirectX::SimpleMath::Vector3& direction)
{
	// プレイヤー弾の生成
	auto bullet = std::make_unique<PlayerBullet>();
	bullet->Initialize(m_commonResources);
	bullet->MakeBall(position, direction);
	m_playerBullets.push_back(std::move(bullet));
	SetIsPlayerShoot(true);// 弾生成フラグを立てる
	// SEの再生
	m_audioManager->PlaySound("Shoot", m_pPlayer->GetVolume());
}

// 弾を生成
void BulletManager::CreateEnemyBullet(const Vector3& position, Vector3& direction)
{
	//	カメラを取得
	auto camera = m_pPlayer->GetCamera();
	// プレイヤーからカメラの情報を取得
	Vector3 playerPos = camera->GetEyePosition();

	// 敵弾の生成
	auto bullet = std::make_unique<EnemyBullet>(m_enemyBulletSize); // サイズ1.0fの例
	bullet->Initialize(m_commonResources, m_enemyBulletType);
	bullet->MakeBall(position, direction, playerPos);
	bullet->SetShooter(m_pShooter);
	m_enemyBullets.push_back(std::move(bullet));
}

// 弾の補充
void BulletManager::ReLoadPlayerBullet()
{
	m_reloadTimer += m_elapsedTime;

	if (m_playerBulletCount < MAX_PLAYER_BULLET_COUNT && m_reloadTimer >= 0.1f)
	{
		m_playerBulletCount++;
	}
	if (m_reloadTimer > BulletParameters::RELOAD_INTERVAL)
	{
		m_reloadTimer = 0.0f;
	}
}

// 弾を消費
void BulletManager::ConsumePlayerBullet()
{
	// プレイヤーの弾を消費
	if (m_playerBulletCount > 0)
	{
		m_playerBulletCount--;
	}
}

// 追加ダメージを設定
void BulletManager::SetAdditionalDamage(int additionalDamage)
{
	for (auto& bullet : m_playerBullets)
	{
		bullet->SetAdditionalDamage(additionalDamage);
	}
}

void BulletManager::UpdatePlayerBullets(float elapsedTime)
{
	//	カメラを取得
	auto camera = m_pPlayer->GetCamera();
	for (auto it = m_playerBullets.begin(); it != m_playerBullets.end();)
	{
		auto& bullet = *it;// 弾を取得
		// カメラ情報を弾に適用
		bullet->SetCameraEye(camera->GetEyePosition());
		bullet->SetCameraTarget(camera->GetTargetPosition());
		bullet->SetCameraUp(camera->GetUpVector());

		// 弾を更新
		bullet->Update(elapsedTime);
		// 弾が寿命を迎えるか地面に着いたら削除
		if (CheckCollisionWithEnemies(bullet) || bullet->IsExpired() || bullet->GetBulletPosition().y <= DELETE_BULLET_POSITION)
		{
			it = m_playerBullets.erase(it);
		}
		else
		{
			++it;
		}
	}

}

void BulletManager::UpdateEnemyBullets(float elapsedTime, std::unique_ptr<IEnemy>& enemy)
{
	//	カメラを取得
	auto camera = enemy->GetCamera();
	for (auto it = m_enemyBullets.begin(); it != m_enemyBullets.end();)
	{
		auto& bullet = *it;// 弾を取得
		// 発射元の敵を取得
		IEnemy* shooter = bullet->GetShooter();
		// 発射元が存在しない場合は削除
		if (!shooter) { it = m_enemyBullets.erase(it); continue; }
		// 発射元が敵でない場合は削除
		if (shooter != enemy.get()) { ++it; continue; }

		// 弾に適切な情報を適用
		bullet->SetCameraEye(camera->GetEyePosition());
		bullet->SetCameraTarget(camera->GetTargetPosition());
		bullet->SetCameraUp(camera->GetUpVector());
		bullet->SetEnemyPosition(shooter->GetPosition());		// 弾を更新
		bullet->Update(elapsedTime);
		// プレイヤーとの当たり判定
		if (CheckCollisionWithPlayer(bullet, enemy) || bullet->IsExpired() || bullet->GetBulletPosition().y <= DELETE_BULLET_POSITION)
		{
			// 衝突処理（ここではエフェクトやSE再生、プレイヤーへのダメージ処理など）
			it = m_enemyBullets.erase(it);
		}
		else
		{
			++it;
		}
	}
}

void BulletManager::SetSound()
{
	// オーディオマネージャーの初期化
	m_audioManager->Initialize();
	m_audioManager->LoadSound("Resources/Sounds/playerBullet.mp3", "Shoot");
	m_audioManager->LoadSound("Resources/Sounds/Hit.mp3", "Hit");

}

BulletManager::GridKey BulletManager::GetGridKey(const DirectX::SimpleMath::Vector3& position)
{
	return
	{
		static_cast<int>(position.x / BulletParameters::GRID_CELL_SIZE),
		static_cast<int>(position.y / BulletParameters::GRID_CELL_SIZE)
	};
}

bool BulletManager::CheckCollisionWithEnemies(const std::unique_ptr<PlayerBullet>& bullet)
{
	for (auto& enemy : m_pEnemyManager->GetEnemies())
	{
		if (bullet->GetBoundingSphere().Intersects(enemy->GetBoundingSphere()))
		{
			if (enemy->GetCanAttack() == true)
				enemy->SetEnemyHP(bullet->Damage());// 敵のHPを減らす
			// エフェクトを追加
			float effectSize = dynamic_cast<Boss*>(enemy.get()) ? 10.0f : 3.0f;
			m_pEnemyManager->GetEffect().push_back(std::make_unique<Effect>(
				m_commonResources,
				Effect::ParticleType::ENEMY_HIT,
				enemy->GetPosition(),
				effectSize,
				enemy->GetMatrix()));
			// プレイヤーの弾が敵に当たったフラグを立てる
			enemy->SetEnemyHitByPlayerBullet(true);// ヒットフラグ
			m_audioManager->PlaySound("Hit", m_pPlayer->GetVolume() * HIT_VOLUME);// SEを再生
			return true;// 当たったらtrueを返す
		}
	}
	return false;// 当たらなかったらfalseを返す
}


bool BulletManager::CheckCollisionWithPlayer(const std::unique_ptr<EnemyBullet>& bullet, const std::unique_ptr<IEnemy>& enemy)
{
	// プレイヤーとの当たり判定
	enemy->SetBulletBoundingSphere(bullet->GetBoundingSphere());
	if (enemy->GetBulletBoundingSphere().Intersects(enemy->GetPlayerBoundingSphere()))
	{
		enemy->SetPlayerHitByEnemyBullet(true);
		enemy->GetPlayer()->SetEnemyBulletDirection(bullet->GetBulletDirection());
		enemy->GetPlayer()->SetisPlayEffect(true);
		enemy->GetPlayer()->SetisPlayerDamage(true);
		return true; // 衝突したらtrueを返す
	}
	return false; // 衝突しなければfalseを返す}

}

void BulletManager::CheckCollisionWithBullets()
{
	// グリッドをクリア
	m_playerBulletGrid.clear();
	m_enemyBulletGrid.clear();
	// プレイヤーの弾をグリッドに登録
	for (auto& bullet : m_playerBullets)
	{
		GridKey key = GetGridKey(bullet->GetBulletPosition());
		m_playerBulletGrid[key].push_back(bullet.get());
	}
	// 敵の弾をグリッドに登録
	for (auto& bullet : m_enemyBullets)
	{
		GridKey key = GetGridKey(bullet->GetBulletPosition());
		m_enemyBulletGrid[key].push_back(bullet.get());
	}
	// 衝突判定
	for (auto& pair : m_enemyBulletGrid)
	{
		const GridKey& enemyKey = pair.first;
		std::vector<EnemyBullet*>& enemyBullets = pair.second;
		// 9セル(自セルと周囲8セル)を調べ、事前にリスト化
		std::vector<GridKey> neighborKeys;
		for (int y = -1; y <= 1; ++y)
		{
			for (int x = -1; x <= 1; ++x)
			{
				neighborKeys.push_back({ enemyKey.x + x, enemyKey.y + y });
			}
		}
		// 事前に作ったリストを使って衝突判定
		for (const auto& neighborKey : neighborKeys)
		{
			auto it = m_playerBulletGrid.find(neighborKey);
			if (it != m_playerBulletGrid.end())
			{
				std::vector<PlayerBullet*>& playerBullets = it->second;
				for (auto& playerBullet : playerBullets)
				{
					for (auto& enemyBullet : enemyBullets)
					{
						if (playerBullet->GetBoundingSphere().Intersects(enemyBullet->GetBoundingSphere()))
						{
							// 敵の弾を削除
							m_enemyBullets.erase(
								std::remove_if(m_enemyBullets.begin(), m_enemyBullets.end(),
									[&enemyBullet](const std::unique_ptr<EnemyBullet>& bullet)
									{
										return bullet.get() == enemyBullet; // ポインタが一致するものを削除
									}),
								m_enemyBullets.end()
							);
							// プレイヤーの弾を削除
							m_playerBullets.erase(
								std::remove_if(m_playerBullets.begin(), m_playerBullets.end(),
									[&playerBullet](const std::unique_ptr<PlayerBullet>& bullet)
									{
										return bullet.get() == playerBullet; // ポインタが一致するものを削除
									}),
								m_playerBullets.end()
							);

							return;
						}
					}
				}
			}
		}
	}
}

void BulletManager::RemoveBulletsByShooter(IEnemy* shooter)
{
	// 発射元が nullptr または無効なら何もしない
	if (!shooter || m_enemyBullets.empty())
		return;



	// 発射元が一致する弾をすべて削除
	m_enemyBullets.erase(
		std::remove_if(m_enemyBullets.begin(), m_enemyBullets.end(),
			[shooter](const std::unique_ptr<EnemyBullet>& bullet)
			{
				IEnemy* bulletShooter = bullet->GetShooter();
				return (bulletShooter == shooter || bulletShooter == nullptr);
			}),
		m_enemyBullets.end()
	);
}
