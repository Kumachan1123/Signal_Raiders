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
	, m_audioManager{ AudioManager::GetInstance() }
	, m_enemyBulletType{ EnemyBullet::BulletType::STRAIGHT }
	, m_enemyBulletSize{ 1.0f }
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
	m_audioManager->Initialize();
	m_audioManager->LoadSound("Resources/Sounds/playerBullet.mp3", "Shoot");
	m_audioManager->LoadSound("Resources/Sounds/Hit.mp3", "Hit");
}

// 更新
void BulletManager::Update(float elapsedTime)
{


	// プレイヤーの弾を更新
	UpdatePlayerBullets(elapsedTime);
	// 敵の弾を更新
	for (auto& enemy : m_pEnemyManager->GetEnemies())
	{

		// 敵の弾を更新
		UpdateEnemyBullets(elapsedTime, enemy);

	}
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
	}

	// 敵弾の描画
	for (const auto& bullet : m_enemyBullets)
	{
		bullet->Render(view, proj);
	}
}
// 弾を生成
void BulletManager::CreateBullet(const Vector3& position, Vector3& direction, BulletType type)
{
	switch (type)
	{
	case BulletType::PLAYER:
	{
		// プレイヤー弾の生成
		auto bullet = std::make_unique<PlayerBullet>();
		bullet->Initialize(m_commonResources);
		bullet->MakeBall(position, direction);
		m_playerBullets.push_back(std::move(bullet));
		// SEの再生
		m_audioManager->PlaySound("Shoot", m_pPlayer->GetVolume());	}
	break;
	case BulletType::ENEMY:
	{
		//	カメラを取得
		auto camera = m_pPlayer->GetCamera();
		// プレイヤーからカメラの情報を取得
		Vector3 playerPos = camera->GetEyePosition();
		// 敵弾の生成
		auto bullet = std::make_unique<EnemyBullet>(m_enemyBulletSize); // サイズ1.0fの例
		bullet->Initialize(m_commonResources, m_enemyBulletType);
		bullet->MakeBall(position, direction, playerPos);
		m_enemyBullets.push_back(std::move(bullet));
	}
	break;
	default:
		assert(false);
		break;
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
	auto camera = m_pPlayer->GetCamera();
	for (auto it = m_enemyBullets.begin(); it != m_enemyBullets.end();)
	{
		auto& bullet = *it;// 弾を取得
		// カメラ情報を弾に適用
		bullet->SetCameraEye(camera->GetEyePosition());
		bullet->SetCameraTarget(camera->GetTargetPosition());
		bullet->SetCameraUp(camera->GetUpVector());
		// 弾を更新
		bullet->Update(elapsedTime);
		// プレイヤーとの当たり判定
		if (CheckCollisionWithPlayer(bullet, enemy))
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