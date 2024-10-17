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
	m_enemyBornInterval{ 0.5f },
	m_enemyBornTimer{ 0.0f },
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
}

//---------------------------------------------------------
// 更新
//---------------------------------------------------------
void Enemies::Update(float elapsedTime)
{
	m_startTime += elapsedTime;// ゲーム開始時間を更新
	m_pWifi->Update(elapsedTime);// Wi-Fiの更新
	// エフェクトの更新
	for (auto& effect : GetEffect()) effect->Update(elapsedTime);
	// 敵生成・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・
	// 敵生成タイマーを更新
	m_enemyBornTimer += elapsedTime;
	// 敵の生成数
	int enemyNum = static_cast<int>(m_pWifi->GetWifiLevels().size());// static_cast<int>(m_pWifi->GetWifiLevels().size()
	//int enemyNum = 2;
	if (m_startTime >= 5.0f)m_isEnemyBorn = true;//生成可能にする
	// 生成可能なら
	if (m_isEnemyBorn && !m_isBorned && m_enemyIndex < enemyNum)
	{
		if (m_enemyBornTimer >= m_enemyBornInterval)// 一定時間経過したら
		{
			// 敵を生成する
			auto enemy = std::make_unique<Enemy>(m_pPlayer);
			enemy->Initialize(m_commonResources, m_pWifi->GetWifiLevels()[m_enemyIndex]);
			m_enemies.push_back(std::move(enemy));
			// タイマーをリセットし、次のWi-Fiレベルのインデックスに進む
			m_enemyBornTimer = 0.0f;
			m_enemyIndex++;
		}
	}
	// 生成完了したら生成不可能にする
	if (m_enemyIndex >= enemyNum)
	{
		m_enemyBornTimer = 0.0f;
		m_isEnemyBorn = false;// ザコ敵生成不可能にする
		m_isBorned = true;// ザコ敵生成完了
	}
	// ザコ敵が全滅したらボスを生成
	if (m_enemies.size() == 0 && m_isBorned && !m_isBossBorned)
	{
		m_isBossBorned = true;// ボス生成可能にする
		m_enemies.clear();// ザコ敵を削除
		m_boss = std::make_unique<Boss>(m_pPlayer);
		m_boss->Initialize(m_commonResources, 1000);
		m_enemies.push_back(std::move(m_boss));// ボスを生成

	}

	// 敵同士の当たり判定・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・
	// 敵同士が重ならないようにする
	for (size_t i = 0; i < m_enemies.size(); ++i)
	{
		for (size_t j = i + 1; j < m_enemies.size(); ++j)
		{
			bool hit = m_enemies[i]->GetBoundingSphere().Intersects(m_enemies[j]->GetBoundingSphere());// 敵同士が重なっていたら
			m_enemies[i]->SetHitToOtherEnemy(hit);// 当たり判定を設定
			m_enemies[j]->SetHitToOtherEnemy(hit);// 当たり判定を設定
			if (hit)m_enemies[i]->CheckHitOtherObject(m_enemies[i]->GetBoundingSphere(), m_enemies[j]->GetBoundingSphere());// 敵同士が重なっていたら押し返す
		}
	}

	// 敵とプレイヤーの一定範囲との当たり判定・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・
	for (auto& enemy : m_enemies)
	{
		m_isHitPlayerToEnemy = false;// フラグを初期化
		// 敵を更新
		enemy->Update(elapsedTime, m_pPlayer->GetPlayerController()->GetPlayerPosition());
		// 敵の弾がプレイヤーに当たったら
		bool hit = enemy->GetBulletHitToPlayer();
		if (hit)
		{
			float playerHP = m_pPlayer->GetPlayerHP() - enemy->GetToPlayerDamage();//新しいHPを計算
			m_pPlayer->SetPlayerHP(playerHP);// プレイヤーのHPを設定
			enemy->SetBulletHitToPlayer(false);// フラグをリセット
			m_audioManager->PlaySound("Damage", m_pPlayer->GetVolume());// ダメージSEを再生
		}
		// 敵がプレイヤーに当たったら
		if (enemy->GetBoundingSphere().Intersects(m_pPlayer->GetInPlayerArea()))
		{
			m_isHitPlayerToEnemy = true;
			// 押し返し用にプレイヤーの一定範囲境界球をローカル変数にコピー
			BoundingSphere playerSphere = m_pPlayer->GetInPlayerArea();
			// 境界球の範囲を調整
			playerSphere.Radius /= 3.0f;
			// プレイヤーの境界球と敵の境界球が重なっていたら
			if (enemy->GetBoundingSphere().Intersects(playerSphere))
				enemy->CheckHitOtherObject(enemy->GetBoundingSphere(), playerSphere);// 敵を押し返す
		}
		// プレイヤーと敵の当たり判定を設定
		enemy->SetHitToPlayer(m_isHitPlayerToEnemy);
		enemy->SetPlayerBoundingSphere(m_pPlayer->GetPlayerSphere());
	}
	// ザコ敵の削除・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・
	// 削除対象を保持するベクター
	std::vector<std::unique_ptr<IEnemy>> enemiesToRemove;
	// ザコ削除対象を収集する
	for (auto it = m_enemies.begin(); it != m_enemies.end(); )
	{

		// 敵が死んでいたら
		if ((*it)->GetEnemyIsDead())
		{	// もしも倒されたのがボスだったら
			if (auto boss = dynamic_cast<Boss*>(it->get()))
			{
				// 敵の座標を渡して爆破エフェクトを再生
				m_effect.push_back(std::make_unique<Effect>(m_commonResources,
															Effect::ParticleType::ENEMY_DEAD,
															(*it)->GetPosition(),
															10.0f,
															(*it)->GetMatrix()));
				m_isBossAlive = false;// 生存フラグをfalseにする
			}
			else// ザコ敵だったら
			{
				// 敵の座標を渡して爆破エフェクトを再生
				m_effect.push_back(std::make_unique<Effect>(m_commonResources,
															Effect::ParticleType::ENEMY_DEAD,
															(*it)->GetPosition(),
															3.0f,
															(*it)->GetMatrix()));
			}

			m_audioManager->PlaySound("EnemyDead", m_pPlayer->GetVolume() * 10);// 敵のSEを再生(こいつだけ音量10倍)

			enemiesToRemove.push_back(std::move(*it));// 削除対象に追加

			it = m_enemies.erase(it);  // 削除してイテレータを更新
		}

		else it++;  // 次の要素へ
	}
	m_pWifi->Clear();// Wi-Fiのクリア

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
