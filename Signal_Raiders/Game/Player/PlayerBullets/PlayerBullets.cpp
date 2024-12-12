/*
	@file	PlayerBullets.cpp
	@brief	プレイヤーの弾全体を管理するクラス
*/
#include "pch.h"
#include "PlayerBullets.h"

PlayerBullets::PlayerBullets(CommonResources* commonResources)
	:
	m_commonResources{ commonResources },
	m_pPlayer{ nullptr },
	m_pEnemies{ nullptr },
	m_audioManager{ AudioManager::GetInstance() }
{
}

PlayerBullets::~PlayerBullets()
{
	// プレイヤーの弾を削除
	for (auto& bullet : m_playerBullet)bullet.reset();
	m_playerBullet.clear();
}


void PlayerBullets::Initialize(Player* pPlayer, Enemies* pEnemies)
{
	m_pPlayer = pPlayer;// プレイヤーのポインターを取得
	m_pEnemies = pEnemies;// 敵全体のポインターを取得
	// オーディオマネージャーを初期化する
	m_audioManager->Initialize();
	// 効果音の初期化
	m_audioManager->LoadSound("Resources/Sounds/playerBullet.mp3", "Shoot");
	m_audioManager->LoadSound("Resources/Sounds/Hit.mp3", "Hit");
}
//---------------------------------------------------------
// プレイヤーの弾を更新する
//---------------------------------------------------------
void PlayerBullets::Update(float elapsedTime)
{
	// プレイヤーからカメラの情報を取得
	DirectX::SimpleMath::Vector3 dir = m_pPlayer->GetCamera()->GetDirection();
	DirectX::SimpleMath::Vector3 playerPos = m_pPlayer->GetCamera()->GetEyePosition();
	DirectX::SimpleMath::Vector3 playerTarget = m_pPlayer->GetCamera()->GetTargetPosition();
	DirectX::SimpleMath::Vector3 playerUp = m_pPlayer->GetCamera()->GetUpVector();
	// プレイヤーの弾を更新
	for (auto it = m_playerBullet.begin(); it != m_playerBullet.end(); )
	{
		(*it)->SetCameraEye(m_pPlayer->GetCamera()->GetEyePosition());// カメラの位置を設定
		(*it)->SetCameraTarget(m_pPlayer->GetCamera()->GetTargetPosition());// カメラの注視点を設定
		(*it)->SetCameraUp(m_pPlayer->GetCamera()->GetUpVector());	// カメラの上方向を設定
		(*it)->Update(dir, elapsedTime);// 弾を更新
		if ((*it)->IsExpired() || (*it)->GetBulletPosition().y <= DELETE_BULLET_POSITION)
		{
			it = m_playerBullet.erase(it);// 弾が寿命を迎えるか地面に着いたら削除
		}
		else
		{
			// 敵とプレイヤーの弾の当たり判定
			if (CheckCollisionWithEnemies(*it))it = m_playerBullet.erase(it);
			else ++it;
		}
	}
}
//---------------------------------------------------------
// 敵とプレイヤーの弾の当たり判定
//---------------------------------------------------------
bool PlayerBullets::CheckCollisionWithEnemies(const std::unique_ptr<PlayerBullet>& bullet)
{
	bool isHit = false;// ヒットフラグを初期化
	for (auto& enemy : m_pEnemies->GetEnemies())
	{
		// 敵とプレイヤーの弾の当たり判定
		if (bullet->GetBoundingSphere().Intersects(enemy->GetBoundingSphere()))
		{
			isHit = true;// ヒットフラグを立てる
			enemy->SetEnemyHP(bullet->Damage());// 敵のHPを減らす
			if (auto boss = dynamic_cast<Boss*>(enemy.get()))
			{
				m_pEnemies->GetEffect().push_back(std::make_unique<Effect>(m_commonResources,
					Effect::ParticleType::ENEMY_HIT,
					enemy->GetPosition(),
					10.0f,
					enemy->GetMatrix()));
			}
			else
			{
				// エフェクトを追加
				m_pEnemies->GetEffect().push_back(std::make_unique<Effect>(m_commonResources,
					Effect::ParticleType::ENEMY_HIT,
					enemy->GetPosition(),
					3.0f,
					enemy->GetMatrix()));
			}
			// プレイヤーの弾が敵に当たったフラグを立てる
			enemy->SetHitToPlayerBullet(true);
			// ヒットSEを再生
			m_audioManager->PlaySound("Hit", m_pPlayer->GetVolume() * HIT_VOLUME);
			return true; // 当たったらtrueを返す
		}
	}
	return false; // 当たらなかったらfalseを返す
}
//---------------------------------------------------------
// プレイヤーの弾を描画する
//---------------------------------------------------------
void PlayerBullets::Render()
{
	using namespace DirectX::SimpleMath;
	Matrix view = m_pPlayer->GetCamera()->GetViewMatrix();// プレイヤーのカメラからビュー行列を取得
	Matrix proj = m_pPlayer->GetCamera()->GetProjectionMatrix();// プレイヤーのカメラから射影行列を取得
	for (auto& bullet : m_playerBullet)
	{
		bullet->Render(view, proj);// プレイヤーの弾を描画
		bullet->RenderShadow(view, proj);// プレイヤーの弾の影を描画
		bullet->DrawCollision(view, proj);// プレイヤーの弾の当たり判定を描画
	}
}

//---------------------------------------------------------
// 弾を生成する
//---------------------------------------------------------
void PlayerBullets::CreateBullet(const DirectX::SimpleMath::Vector3& position, DirectX::SimpleMath::Vector3& direction)
{	// SEの再生
	m_audioManager->PlaySound("Shoot", m_pPlayer->GetVolume());
	// 弾を生成
	auto bullet = std::make_unique<PlayerBullet>();
	bullet->Initialize(m_commonResources);// 初期化
	bullet->MakeBall(position, direction);// 弾を生成
	GetPlayerBullet().push_back(std::move(bullet));// プレイヤーの弾を追加
	SetIsBullet(true);// 弾生成フラグを立てる
}
