/*
	@file	EnemyManager.cpp
	@brief	敵たちクラス
*/
#include "pch.h"
#include "EnemyManager.h"

#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include <vector>
#include <cassert>
#include <SimpleMath.h>
#include <Model.h>
#include <Effects.h>
#include <memory>
#include "Game/KumachiLib/KumachiLib.h"
#include "Game/Enemy/BossBase/BossBase.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
//---------------------------------------------------------
// コンストラクタ
//---------------------------------------------------------
EnemyManager::EnemyManager(CommonResources* commonResources)
	: m_commonResources{ commonResources }
	, m_enemies{}
	, m_isEnemyBorn{ false }
	, m_isBorned{ false }
	, m_isHitPlayerToEnemy{ false }
	, m_isBossBorn{ false }
	, m_isBossBorned{ false }
	, m_isBossAlive{ true }
	, m_isBossAppear{ false }
	, m_enemyIndex{ 0 }
	, m_stageNumber{ 0 }
	, m_enemyMax{ 0 }
	, m_enemyBornInterval{ EnemyParameters::ENEMY_SPAWN_INTERVAL }
	, m_enemyBornTimer{ 0.0f }
	, m_bossHP{ 0 }
	, m_bossBulletType{ BossBase::BossBulletType::STAGE_1 }
	, m_specialAttackCount{ 0 }
	, m_startTime{ 0.0f }
	, m_bossBornWaitTime{ 0.0f }
	, m_pWifi{ nullptr }
	, m_pWall{ nullptr }
	, m_pPlayer{ nullptr }
	, m_pBulletManager{ nullptr }
	, m_SEVolume{ 0.0f }

{
	// Wi-Fiを初期化する
	m_pWifi = std::make_unique<Wifi>();
	m_pWifi->Initialize();
}

//---------------------------------------------------------
// デストラクタ
//---------------------------------------------------------
EnemyManager::~EnemyManager()
{
}

//---------------------------------------------------------
// 初期化
//---------------------------------------------------------
void EnemyManager::Initialize(Player* pPlayer)
{
	m_pPlayer = pPlayer;// プレイヤーのポインタを取得
	SetEnemyMax();// 敵の生成上限設定
}

//---------------------------------------------------------
// 更新
//---------------------------------------------------------
void EnemyManager::Update(float elapsedTime)
{
	// ゲーム開始時間を更新
	UpdateStartTime(elapsedTime);

	// Wi-Fiの更新
	m_pWifi->Update(elapsedTime);

	// エフェクトの更新
	UpdateEffects(elapsedTime);

	// 敵の生成処理
	HandleEnemySpawning(elapsedTime);

	// 敵同士の当たり判定
	HandleEnemyCollisions();

	// 敵とプレイヤーの当たり判定
	UpdateEnemies(elapsedTime);

	// 敵と壁の当たり判定
	HandleWallCollision();

	// ザコ敵の削除処理
	RemoveDeadEnemies();

	m_pWifi->Clear(); // Wi-Fiのクリア
}

//---------------------------------------------------------
// 描画
//---------------------------------------------------------
void EnemyManager::Render()
{
	Matrix view = m_pPlayer->GetCamera()->GetViewMatrix();//	ビュー行列
	Matrix projection = m_pPlayer->GetCamera()->GetProjectionMatrix();//	プロジェクション行列
	if (m_enemies.size() > 0)for (const auto& enemy : m_enemies)//	全ての敵を描画する
	{
		enemy->Render(view, projection);//	敵を描画する
#ifdef _DEBUG
		enemy->DrawCollision(view, projection);//	当たり判定を描画する
#endif
	}

	// エフェクトを描画する
	GetEffect().erase
	(std::remove_if(GetEffect().begin(),
		GetEffect().end(),
		[&](const std::unique_ptr<Effect>& effect)//	再生終了したパーティクルを削除する
		{
			if (!effect->IsPlaying()) return true;// 再生終了したパーティクルは削除する
			effect->Render(view, projection);// パーティクルを描画する
			return false;//	再生中のパーティクルは削除しない
		}),
		GetEffect().end()//	削除対象のパーティクルを削除する
	);

}
//---------------------------------------------------------
// 敵の生成上限設定
//---------------------------------------------------------
void EnemyManager::SetEnemyMax()
{
	auto it = stageData.find(m_stageNumber);// ステージデータを取得
	if (it != stageData.end())// ステージデータが存在したら
	{
		m_enemyMax = it->second.enemyMax;// 敵の生成上限を設定
		m_bossHP = it->second.bossHP;// ボスのHPを設定
		m_specialAttackCount = it->second.specialAttackCount;// ボスの特殊攻撃の数を設定
		m_bossBulletType = it->second.bulletType;// ボスの弾の種類を設定
	}
}
//---------------------------------------------------------
// ゲーム開始時間を更新
//---------------------------------------------------------
void EnemyManager::UpdateStartTime(float elapsedTime)
{
	m_startTime += elapsedTime; // ゲーム開始時間を更新
}

//---------------------------------------------------------
// エフェクトの更新
//---------------------------------------------------------
void EnemyManager::UpdateEffects(float elapsedTime)
{
	for (auto& effect : GetEffect())effect->Update(elapsedTime);// 全てのエフェクトを更新
}

//---------------------------------------------------------
// 敵の生成処理
//---------------------------------------------------------
void EnemyManager::HandleEnemySpawning(float elapsedTime)
{
	// 敵生成タイマーを更新
	m_enemyBornTimer += elapsedTime;
	// 敵の数を取得
	int enemyNum = static_cast<int>(m_pWifi->GetWifiLevels().size());
	// 敵の数が敵の生成上限を超えたら敵の生成上限に設定
	if (enemyNum > m_enemyMax) enemyNum = m_enemyMax;
	// ゲーム開始時間が敵生成開始時間を超えたら
	if (m_startTime >= EnemyParameters::ENEMY_SPAWN_START_TIME) m_isEnemyBorn = true;// ザコ敵生成可能にする
	// ザコ敵生成可能かつザコ敵生成完了していない場合
	if (m_isEnemyBorn && !m_isBorned && m_enemyIndex < enemyNum)
	{
		if (m_enemyBornTimer >= m_enemyBornInterval)// 敵生成間隔を超えたら
		{
			EnemyType enemyType = static_cast<EnemyType>(m_pWifi->GetEnemyTypes()[m_enemyIndex]);// 敵の種類を取得
			SpawnEnemy(enemyType);// 敵を生成(指定した敵の種類)
		}
	}
	// 敵生成上限に達したら
	if (m_enemyIndex >= enemyNum)FinalizeEnemySpawn();// 敵生成完了処理
	// 敵がいなくなったらボスを生成
	if (m_enemies.empty() && m_isBorned && !m_isBossBorned)
	{
		m_isBossAppear = true; // ボス生成演出フラグを立てる
		m_bossBornWaitTime += elapsedTime;// ボス生成待機時間を更新
		if (m_bossBornWaitTime >= EnemyParameters::BOSS_SPAWN_WAIT_TIME)// ボス生成待機時間を超えたら
			SpawnBoss();// ボスを生成

	}
}


//---------------------------------------------------------
// 敵を生成する(指定した敵の種類)
//---------------------------------------------------------
void EnemyManager::SpawnEnemy(EnemyType type)
{
	// ファクトリで生成
	auto enemy = EnemyFactory::CreateEnemy(type, m_pPlayer, m_commonResources, m_pWifi->GetWifiLevels()[m_enemyIndex]);
	enemy->Initialize();// 敵を初期化
	enemy->SetBulletManager(m_pBulletManager);// 弾マネージャーを設定
	m_enemies.push_back(std::move(enemy));// 敵リストに追加
	m_enemyBornTimer = 0.0f;// 敵生成タイマーを初期化
	m_enemyIndex++; // 敵インデックスを増加

}



//---------------------------------------------------------
// 敵の生成完了処理
//---------------------------------------------------------
void EnemyManager::FinalizeEnemySpawn()
{
	m_enemyBornTimer = 0.0f; // 敵生成タイマーを初期化
	m_isEnemyBorn = false; // ザコ敵生成不可能にする
	m_isBorned = true; // ザコ敵生成完了
}

//---------------------------------------------------------
// ボスを生成する
//---------------------------------------------------------
void EnemyManager::SpawnBoss()
{
	auto boss = std::make_unique<BossBase>(m_pPlayer, m_commonResources, m_bossHP);// ボスを生成
	if (m_stageNumber >= 3) boss->SetBossType(BossType::LAST_BOSS);// ボスの種類を設定
	else boss->SetBossType(BossType::NORMAL_BOSS);// ボスの種類を設定

	boss->SetBulletManager(m_pBulletManager);// 弾マネージャーを設定
	boss->SetBulletType(m_bossBulletType);// ボスの弾の種類を設定
	boss->GetBulletManager()->SetSpecialAttackCount(m_specialAttackCount);// ボスの特殊攻撃の数を設定
	boss->Initialize();// ボスを初期化


	m_enemies.push_back(std::move(boss)); // ボスも enemies に統一
	m_isBossBorned = true; // ボス生成完了
	m_isBossAppear = false; // ボス生成演出フラグを下ろす
}


//---------------------------------------------------------
// 敵同士の当たり判定処理
//---------------------------------------------------------
void EnemyManager::HandleEnemyCollisions()
{
	for (size_t i = 0; i < m_enemies.size(); ++i)
	{
		for (size_t j = i + 1; j < m_enemies.size(); ++j)
		{
			bool hit = m_enemies[i]->GetBoundingSphere().Intersects(m_enemies[j]->GetBoundingSphere());// 当たり判定を取得
			m_enemies[i]->SetHitToOtherEnemy(hit);// 当たり判定結果を設定
			m_enemies[j]->SetHitToOtherEnemy(hit);// 当たり判定結果を設定

			if (hit)// 当たり判定があったら
			{
				// m_enemies[i]の新しい座標
				auto& enemyA = m_enemies[i]->GetBoundingSphere();
				auto& enemyB = m_enemies[j]->GetBoundingSphere();
				Vector3 newPos = CheckHitOtherObject(enemyA, enemyB);
				m_enemies[i]->SetPosition(newPos);
			}
		}
	}
}
//---------------------------------------------------------
// 敵と壁の当たり判定処理
//---------------------------------------------------------
void EnemyManager::HandleWallCollision()
{
	for (auto& enemy : m_enemies)
	{
		for (int i = 0; i < m_pWall->GetWallNum(); i++)
		{
			if (enemy->GetBoundingSphere().Intersects(m_pWall->GetBoundingBox(i)))
			{
				Vector3 newPos = CheckHitWall(enemy->GetBoundingSphere(), m_pWall->GetBoundingBox(i));
				enemy->SetPosition(newPos);
			}
		}
	}
}

//---------------------------------------------------------
// 敵全体の更新処理
//---------------------------------------------------------
void EnemyManager::UpdateEnemies(float elapsedTime)
{
	// 攻撃中の敵配列をクリア
	m_attackingEnemies.clear();

	for (auto& enemy : m_enemies)
	{
		enemy->Update(elapsedTime);// 敵の更新

		HandleEnemyBulletCollision(enemy);// 敵の弾がプレイヤーに当たったら

		HandleEnemyPlayerCollision(enemy);// 敵がプレイヤーに当たったら


	}
}

//---------------------------------------------------------
// 敵の弾がプレイヤーに当たった処理
//---------------------------------------------------------
void EnemyManager::HandleEnemyBulletCollision(std::unique_ptr<IEnemy>& enemy)
{
	bool hit = enemy->GetPlayerHitByEnemyBullet();
	if (hit)
	{
		float playerHP = m_pPlayer->GetPlayerHP() - enemy->GetToPlayerDamage();
		m_pPlayer->SetPlayerHP(playerHP);
		enemy->SetPlayerHitByEnemyBullet(false);
		m_commonResources->GetAudioManager()->PlaySound("Damage", m_pPlayer->GetVolume());
	}
}

//---------------------------------------------------------
// 敵がプレイヤーに当たった処理
//---------------------------------------------------------
void EnemyManager::HandleEnemyPlayerCollision(std::unique_ptr<IEnemy>& enemy)
{
	m_isHitPlayerToEnemy = false; // フラグを初期化

	if (enemy->GetBoundingSphere().Intersects(m_pPlayer->GetInPlayerArea()))
	{
		m_isHitPlayerToEnemy = true;
		BoundingSphere playerSphere = m_pPlayer->GetInPlayerArea();// プレイヤーの当たり判定を取得
		playerSphere.Radius /= 3.0f;// プレイヤーの当たり判定を縮小
		// 敵がプレイヤーを認識する範囲 / 3.0f = プレイヤーの当たり判定の半径
		if (enemy->GetBoundingSphere().Intersects(playerSphere))
		{
			auto& enemySphere = enemy->GetBoundingSphere();
			Vector3 newPos = CheckHitOtherObject(enemySphere, playerSphere);
			enemy->SetPosition(newPos);
		}
	}

	enemy->SetHitToPlayer(m_isHitPlayerToEnemy);
	enemy->SetPlayerBoundingSphere(m_pPlayer->GetPlayerSphere());
}

//---------------------------------------------------------
// ザコ敵の削除処理
//---------------------------------------------------------
void EnemyManager::RemoveDeadEnemies()
{
	m_enemies.erase(std::remove_if(m_enemies.begin(), m_enemies.end(),
		[this](std::unique_ptr<IEnemy>& enemy)
		{
			if (enemy->GetEnemyIsDead())
			{
				HandleEnemyDeath(enemy);
				return true; // 削除対象
			}
			return false; // 残す
		}),
		m_enemies.end());
}

//---------------------------------------------------------
// 敵が死んだ場合の処理
//---------------------------------------------------------
void EnemyManager::HandleEnemyDeath(std::unique_ptr<IEnemy>& enemy)
{
	// 敵が死んだ場合の処理
	float effectScale;// 初期値
	auto pBoss = dynamic_cast<BossBase*>(enemy.get());// IEnemyからBossのポインターを抽出
	// もしボスだったら
	if (pBoss)
	{
		effectScale = pBoss->GetDeadEffectSize();// ボスの場合はエフェクトのスケールを大きくする
		m_isBossAlive = false; // 生存フラグをfalseにする
	}
	else
	{
		effectScale = EnemyParameters::ENEMY_DEADEFFECT_SCALE;// ザコ敵の場合はエフェクトのスケールを小さくする
	}
	m_effect.push_back(std::make_unique<Effect>(m_commonResources,
		Effect::EffectType::ENEMY_DEAD,
		enemy->GetPosition(),
		effectScale,
		enemy->GetMatrix()));
	// 敵のSEを再生(こいつだけなぜか元から音が小さいから音量補正)
	m_commonResources->GetAudioManager()->PlaySound("EnemyDead", m_pPlayer->GetVolume() + m_pPlayer->GetVolumeCorrection());
}