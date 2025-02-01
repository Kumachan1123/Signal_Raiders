/*
	@file	PlayerBullets.cpp
	@brief	プレイヤーの弾全体を管理するクラス
*/
#include "pch.h"
#include "PlayerBullets.h"
using namespace DirectX::SimpleMath;

PlayerBullets::PlayerBullets(CommonResources* commonResources)
	:
	m_commonResources{ commonResources },
	m_pPlayer{ nullptr },
	m_pEnemyManager{ nullptr },
	m_audioManager{ AudioManager::GetInstance() }
{
}

PlayerBullets::~PlayerBullets()
{
	// プレイヤーの弾を削除
	for (auto& bullet : m_bullets)bullet.reset();
	m_bullets.clear();
}


void PlayerBullets::Initialize(Player* pPlayer, EnemyManager* pEnemies)
{
	m_pPlayer = pPlayer;// プレイヤーのポインターを取得
	m_pEnemyManager = pEnemies;// 敵全体のポインターを取得
	// 効果音の初期化
	m_audioManager->Initialize();
	m_audioManager->LoadSound("Resources/Sounds/playerBullet.mp3", "Shoot");
	m_audioManager->LoadSound("Resources/Sounds/Hit.mp3", "Hit");
}
//---------------------------------------------------------
// プレイヤーの弾を更新する
//---------------------------------------------------------
void PlayerBullets::Update(float elapsedTime)
{
	//	カメラを取得
	auto camera = m_pPlayer->GetCamera();
	for (auto it = m_bullets.begin(); it != m_bullets.end();)
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
			it = m_bullets.erase(it);
		}
		else
		{
			++it;
		}
	}


}
//---------------------------------------------------------
// 敵とプレイヤーの弾の当たり判定
//---------------------------------------------------------
bool PlayerBullets::CheckCollisionWithEnemies(const std::unique_ptr<PlayerBullet>& bullet)
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
//---------------------------------------------------------
// プレイヤーの弾を描画する
//---------------------------------------------------------
void PlayerBullets::Render()
{
	auto camera = m_pPlayer->GetCamera();
	Matrix view = camera->GetViewMatrix();// プレイヤーのカメラからビュー行列を取得
	Matrix proj = camera->GetProjectionMatrix();// プレイヤーのカメラから射影行列を取得
	for (auto& bullet : m_bullets)
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
