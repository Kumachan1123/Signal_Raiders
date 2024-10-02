/*
	@file	PlayerBullets.cpp
	@brief	プレイヤーの弾全体を管理するクラス
	作成者：くまち
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

void PlayerBullets::Update(float elapsedTime)
{
	// プレイヤーの弾を更新する
	DirectX::SimpleMath::Vector3 dir = m_pPlayer->GetCamera()->GetDirection();
	DirectX::SimpleMath::Vector3 playerPos = m_pPlayer->GetCamera()->GetEyePosition();
	DirectX::SimpleMath::Vector3 playerTarget = m_pPlayer->GetCamera()->GetTargetPosition();
	DirectX::SimpleMath::Vector3 playerUp = m_pPlayer->GetCamera()->GetUpVector();
	for (auto it = m_playerBullet.begin(); it != m_playerBullet.end(); )
	{
		(*it)->SetCameraEye(playerPos);// カメラの位置を設定
		(*it)->SetCameraTarget(playerTarget);// カメラの注視点を設定
		(*it)->SetCameraUp(playerUp);	// カメラの上方向を設定
		(*it)->Update(dir, elapsedTime);// 弾を更新
		if ((*it)->IsExpired())it = m_playerBullet.erase(it);// 弾が寿命を迎えたら削除
		else
		{
			bool isHit = false;
			for (auto& enemy : m_pEnemies->GetEnemies())
			{
				if ((*it)->GetBoundingSphere().Intersects(enemy->GetBoundingSphere()))
				{
					isHit = true;
					enemy->SetEnemyHP(enemy->GetHP() - (*it)->Damage());
					m_pEnemies->GetEffect().push_back(std::make_unique<Effect>(m_commonResources,
																			   Effect::ParticleType::ENEMY_HIT,
																			   enemy->GetPosition(),
																			   enemy->GetMatrix()));
					enemy->SetHitToPlayerBullet(true);
					m_audioManager->PlaySound("Hit", m_pPlayer->GetVolume());// ヒットSEを再生
					break;
				}
			}
			if (isHit) it = m_playerBullet.erase(it);
			else it++;
		}
	}
}

void PlayerBullets::Render()
{
	using namespace DirectX::SimpleMath;
	Matrix view = m_pPlayer->GetCamera()->GetViewMatrix();
	Matrix proj = m_pPlayer->GetCamera()->GetProjectionMatrix();
	for (auto& bullet : m_playerBullet)bullet->Render(view, proj);
}


void PlayerBullets::CreateBullet(const DirectX::SimpleMath::Vector3& position, DirectX::SimpleMath::Vector3& direction)
{	// SEの再生
	m_audioManager->PlaySound("Shoot", m_pPlayer->GetVolume());
	auto bullet = std::make_unique<PlayerBullet>();
	bullet->Initialize(m_commonResources);
	bullet->MakeBall(position, direction);
	GetPlayerBullet().push_back(std::move(bullet));
	SetIsBullet(true);
}