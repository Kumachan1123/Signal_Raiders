/*
	@file	Enemies.cpp
	@brief	敵たちクラス
	作成者：くまち
*/
#include "pch.h"
#include "Enemies.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include <vector>
#include <cassert>
#include <SimpleMath.h>
#include <Model.h>
#include <Effects.h>
#include <memory>
#include "Game/Template/Template.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

//---------------------------------------------------------
// コンストラクタ
//---------------------------------------------------------
Enemies::Enemies(CommonResources* commonResources)
	:
	m_commonResources{ commonResources },
	m_enemies{},
	m_boss{},
	m_isEnemyBorn{ false },
	m_isBorned{ false },
	m_enemyIndex{ 0 },
	m_stageNumber{ 0 },
	m_enemyMax{ 0 },
	m_enemyBornInterval{ 0.5f },
	m_enemyBornTimer{ 0.0f },
	m_bossHP{ 100 },
	m_bossBulletType{ Boss::BossBulletType::NORMAL },
	m_startTime{ 0.0f },
	m_pWifi{ nullptr },
	m_pPlayer{ nullptr },
	m_audioManager{ AudioManager::GetInstance() }
{
	// Wi-Fiを初期化する
	m_pWifi = std::make_unique<Wifi>();
	m_pWifi->Initialize();
}

//---------------------------------------------------------
// デストラクタ
//---------------------------------------------------------
Enemies::~Enemies()
{
}

//---------------------------------------------------------
// 初期化
//---------------------------------------------------------
void Enemies::Initialize(Player* pPlayer)
{
	m_pPlayer = pPlayer;
	InitializeFMOD();
	// 敵の生成上限設定
	SetEnemyMax();
}

//---------------------------------------------------------
// 更新
//---------------------------------------------------------
void Enemies::Update(float elapsedTime)
{
	UpdateStartTime(elapsedTime);
	m_pWifi->Update(elapsedTime);

	// エフェクトの更新
	UpdateEffects(elapsedTime);

	// 敵の生成処理
	HandleEnemySpawning(elapsedTime);

	// 敵同士の当たり判定
	HandleEnemyCollisions();

	// 敵とプレイヤーの当たり判定
	HandlePlayerCollisions(elapsedTime);

	// ザコ敵の削除処理
	RemoveDeadEnemies();

	m_pWifi->Clear(); // Wi-Fiのクリア
}

//---------------------------------------------------------
// 描画
//---------------------------------------------------------
void Enemies::Render()
{
	auto context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();
	Matrix view = m_pPlayer->GetCamera()->GetViewMatrix();
	Matrix projection = m_pPlayer->GetCamera()->GetProjectionMatrix();
	if (m_enemies.size() > 0)for (const auto& enemy : m_enemies)enemy->Render(view, projection);
	// エフェクトを描画する
	GetEffect().erase
	(std::remove_if(GetEffect().begin(), GetEffect().end(), [&](const std::unique_ptr<Effect>& effect)//	再生終了したパーティクルを削除する
		{
			if (!effect->IsPlaying()) return true;// 再生終了したパーティクルは削除する
			effect->Render(context, view, projection);// パーティクルを描画する
			return false;//	再生中のパーティクルは削除しない
		}),
		GetEffect().end()//	削除対象のパーティクルを削除する
	);

}

void Enemies::InitializeFMOD()
{

	// FMODシステムの初期化
	m_audioManager->Initialize();
	m_audioManager->LoadSound("Resources/Sounds/Explosion.mp3", "EnemyDead");
	m_audioManager->LoadSound("Resources/Sounds/damage.mp3", "Damage");
}

// 敵の生成上限設定
void Enemies::SetEnemyMax()
{
	// ステージ番号によって敵の生成上限を設定
	switch (m_stageNumber)
	{
	case 0:
		m_enemyMax = 5;// 敵の生成上限を設定
		m_bossHP = 100;// ボスの体力を設定
		m_bossBulletType = Boss::BossBulletType::NORMAL;// ボスの弾を一発に設定
		break;
	case 1:
		m_enemyMax = 10;// 敵の生成上限を設定
		m_bossHP = 200;// ボスの体力を設定
		m_bossBulletType = Boss::BossBulletType::NORMAL;// ボスの弾を一発に設定
		break;
	case 2:
		m_enemyMax = 20;// 敵の生成上限を設定
		m_bossHP = 300;// ボスの体力を設定
		m_bossBulletType = Boss::BossBulletType::TWIN;// ボスの弾を二発に設定
		break;
	case 3:
		m_enemyMax = 30;// 敵の生成上限を設定
		m_bossHP = 500;// ボスの体力を設定
		m_bossBulletType = Boss::BossBulletType::THREE;// ボスの弾を三発に設定
		break;
	case 4:
		m_enemyMax = 40;// 敵の生成上限を設定
		m_bossHP = 1000;// ボスの体力を設定
		m_bossBulletType = Boss::BossBulletType::SPIRAL;// ボスの弾を螺旋に設定
		break;
	}
}
//---------------------------------------------------------
// ゲーム開始時間を更新
//---------------------------------------------------------
void Enemies::UpdateStartTime(float elapsedTime)
{
	m_startTime += elapsedTime; // ゲーム開始時間を更新
}

//---------------------------------------------------------
// エフェクトの更新
//---------------------------------------------------------
void Enemies::UpdateEffects(float elapsedTime)
{
	for (auto& effect : GetEffect())
	{
		effect->Update(elapsedTime);
	}
}

//---------------------------------------------------------
// 敵の生成処理
//---------------------------------------------------------
void Enemies::HandleEnemySpawning(float elapsedTime)
{
	m_enemyBornTimer += elapsedTime;

	int enemyNum = static_cast<int>(m_pWifi->GetWifiLevels().size());
	if (enemyNum > m_enemyMax) enemyNum = m_enemyMax;

	if (m_startTime >= 5.0f) m_isEnemyBorn = true;// ザコ敵生成可能にする

	if (m_isEnemyBorn && !m_isBorned && m_enemyIndex < enemyNum)// ザコ敵生成可能かつザコ敵生成完了していない場合
	{
		if (m_enemyBornTimer >= m_enemyBornInterval)// 敵生成間隔を超えたら
		{
			if (m_pWifi->GetEnemyTypes()[m_enemyIndex] == 0)SpawnEnemy();// 敵を生成
			else SpawnAreaAttacker();// 範囲攻撃タイプの敵を生成
		}
	}

	if (m_enemyIndex >= enemyNum)// 敵生成上限に達したら
	{
		FinalizeEnemySpawn();// 敵生成完了処理
	}

	if (m_enemies.empty() && m_isBorned && !m_isBossBorned)// 敵がいなくなったらボスを生成
	{
		SpawnBoss();// ボスを生成
	}
}

//---------------------------------------------------------
// 敵を生成する(通常タイプ)
//---------------------------------------------------------
void Enemies::SpawnEnemy()
{
	auto enemy = std::make_unique<Enemy>(m_pPlayer);
	enemy->Initialize(m_commonResources, m_pWifi->GetWifiLevels()[m_enemyIndex]);
	m_enemies.push_back(std::move(enemy));

	m_enemyBornTimer = 0.0f;
	m_enemyIndex++;
}

//---------------------------------------------------------
// 敵を生成する(範囲攻撃タイプ)
//---------------------------------------------------------
void Enemies::SpawnAreaAttacker()
{
	auto areaAttacker = std::make_unique<AreaAttacker>(m_pPlayer);
	areaAttacker->Initialize(m_commonResources, m_pWifi->GetWifiLevels()[m_enemyIndex]);
	m_enemies.push_back(std::move(areaAttacker));

	m_enemyBornTimer = 0.0f;
	m_enemyIndex++;
}


//---------------------------------------------------------
// 敵の生成完了処理
//---------------------------------------------------------
void Enemies::FinalizeEnemySpawn()
{
	m_enemyBornTimer = 0.0f;
	m_isEnemyBorn = false; // ザコ敵生成不可能にする
	m_isBorned = true; // ザコ敵生成完了
}

//---------------------------------------------------------
// ボスを生成する
//---------------------------------------------------------
void Enemies::SpawnBoss()
{
	m_isBossBorned = true; // ボス生成可能にする
	m_enemies.clear(); // ザコ敵を削除

	m_boss = std::make_unique<Boss>(m_pPlayer); // ボスを生成
	m_boss->Initialize(m_commonResources, m_bossHP); // ボスを初期化
	m_boss->SetBulletType(m_bossBulletType); // ボスの弾の種類を設定
	m_enemies.push_back(std::move(m_boss)); // ボスを敵リストに追加
}

//---------------------------------------------------------
// 敵同士の当たり判定処理
//---------------------------------------------------------
void Enemies::HandleEnemyCollisions()
{
	for (size_t i = 0; i < m_enemies.size(); ++i)
	{
		for (size_t j = i + 1; j < m_enemies.size(); ++j)
		{
			bool hit = m_enemies[i]->GetBoundingSphere().Intersects(m_enemies[j]->GetBoundingSphere());
			m_enemies[i]->SetHitToOtherEnemy(hit);// 当たり判定結果を設定
			m_enemies[j]->SetHitToOtherEnemy(hit);// 当たり判定結果を設定

			if (hit)// 当たり判定があったら
			{
				m_enemies[i]->CheckHitOtherObject(m_enemies[i]->GetBoundingSphere(), m_enemies[j]->GetBoundingSphere());
			}
		}
	}
}

//---------------------------------------------------------
// 敵とプレイヤーの当たり判定処理
//---------------------------------------------------------
void Enemies::HandlePlayerCollisions(float elapsedTime)
{
	for (auto& enemy : m_enemies)
	{
		m_isHitPlayerToEnemy = false; // フラグを初期化

		enemy->Update(elapsedTime, m_pPlayer->GetCamera()->GetEyePosition());// 敵の更新

		// 敵の弾がプレイヤーに当たったら
		HandleEnemyBulletCollision(enemy);

		// 敵がプレイヤーに当たったら
		HandleEnemyPlayerCollision(enemy);
	}
}

//---------------------------------------------------------
// 敵の弾がプレイヤーに当たった処理
//---------------------------------------------------------
void Enemies::HandleEnemyBulletCollision(std::unique_ptr<IEnemy>& enemy)
{
	bool hit = enemy->GetBulletHitToPlayer();
	if (hit)
	{
		float playerHP = m_pPlayer->GetPlayerHP() - enemy->GetToPlayerDamage();
		m_pPlayer->SetPlayerHP(playerHP);
		enemy->SetBulletHitToPlayer(false);
		m_audioManager->PlaySound("Damage", m_pPlayer->GetVolume());
	}
}

//---------------------------------------------------------
// 敵がプレイヤーに当たった処理
//---------------------------------------------------------
void Enemies::HandleEnemyPlayerCollision(std::unique_ptr<IEnemy>& enemy)
{
	if (enemy->GetBoundingSphere().Intersects(m_pPlayer->GetInPlayerArea()))
	{
		m_isHitPlayerToEnemy = true;
		BoundingSphere playerSphere = m_pPlayer->GetInPlayerArea();
		playerSphere.Radius /= 3.0f;

		if (enemy->GetBoundingSphere().Intersects(playerSphere))
		{
			enemy->CheckHitOtherObject(enemy->GetBoundingSphere(), playerSphere);
		}
	}

	enemy->SetHitToPlayer(m_isHitPlayerToEnemy);
	enemy->SetPlayerBoundingSphere(m_pPlayer->GetPlayerSphere());
}

//---------------------------------------------------------
// ザコ敵の削除処理
//---------------------------------------------------------
void Enemies::RemoveDeadEnemies()
{
	std::vector<std::unique_ptr<IEnemy>> enemiesToRemove;

	for (auto it = m_enemies.begin(); it != m_enemies.end(); )
	{
		if ((*it)->GetEnemyIsDead())
		{
			HandleEnemyDeath(*it);
			enemiesToRemove.push_back(std::move(*it));
			it = m_enemies.erase(it); // 削除してイテレータを更新
		}
		else
		{
			++it; // 次の要素へ
		}
	}
}

//---------------------------------------------------------
// 敵が死んだ場合の処理
//---------------------------------------------------------
void Enemies::HandleEnemyDeath(std::unique_ptr<IEnemy>& enemy)
{
	if (auto boss = dynamic_cast<Boss*>(enemy.get()))
	{
		m_effect.push_back(std::make_unique<Effect>(m_commonResources,
			Effect::ParticleType::ENEMY_DEAD,
			enemy->GetPosition(),
			10.0f,
			enemy->GetMatrix()));
		m_isBossAlive = false; // 生存フラグをfalseにする
	}
	else
	{
		m_effect.push_back(std::make_unique<Effect>(m_commonResources,
			Effect::ParticleType::ENEMY_DEAD,
			enemy->GetPosition(),
			3.0f,
			enemy->GetMatrix()));
	}

	m_audioManager->PlaySound("EnemyDead", m_pPlayer->GetVolume() * 10); // 敵のSEを再生(こいつだけ音量10倍)
}