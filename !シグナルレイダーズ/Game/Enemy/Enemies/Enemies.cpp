/*
	@file	Enemies.cpp
	@brief	敵たちクラス
	作成者：くまち
*/
#include "pch.h"
#include "Enemies.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Game/FPS_Camera/FPS_Camera.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/GridFloor.h"
#include "Libraries/MyLib/InputManager.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include <vector>
#include <cassert>
#include <SimpleMath.h>
#include <Model.h>
#include <Effects.h>
#include <memory>

using namespace DirectX;
using namespace DirectX::SimpleMath;

//---------------------------------------------------------
// コンストラクタ
//---------------------------------------------------------
Enemies::Enemies(CommonResources* commonResources)
	:
	m_commonResources{ commonResources },
	m_enemy{},
	m_isEnemyBorn{ false },
	m_isBorned{ false },
	m_enemyIndex{ 0 },
	m_enemyBornInterval{ 0.0f },
	m_enemyBornTimer{ 0.0f }
{
}

//---------------------------------------------------------
// デストラクタ
//---------------------------------------------------------
Enemies::~Enemies()
{
	// do nothing.
}

//---------------------------------------------------------
// 更新
//---------------------------------------------------------
void Enemies::Update(Effect* pEffect, Wifi* pWifi, PlayerController* pPlayerController, float elapsedTime)
{
	//	// 敵生成・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・
	//	// 敵生成タイマーを更新
	//	m_enemyBornTimer += elapsedTime;
	//	if (m_startTime >= 5.0f)m_isEnemyBorn = true;//生成可能にする
	//	// 生成可能なら
	//	if (m_isEnemyBorn && !m_isBorned && m_enemyIndex < m_wifi->GetWifiLevels().size())//m_wifi->GetWifiLevels().size()
	//	{
	//		if (m_enemyBornTimer >= m_enemyBornInterval)// 一定時間経過したら
	//		{
	//			// 敵を生成する
	//			auto enemy = std::make_unique<Enemy>();
	//			enemy->Initialize(m_commonResources, m_wifi->GetWifiLevels()[m_enemyIndex]);
	//			m_enemy.push_back(std::move(enemy));
	//			// タイマーをリセットし、次のWi-Fiレベルのインデックスに進む
	//			m_enemyBornTimer = 0.0f;
	//			m_enemyIndex++;
	//		}
	//	}
	//	// 生成完了したら生成不可能にする
	//	if (m_enemyIndex >= m_wifi->GetWifiLevels().size())
	//	{
	//		m_enemyBornTimer = 0.0f;
	//		m_isEnemyBorn = false;
	//		m_isBorned = true;
	//	}
	//	// 敵同士の当たり判定・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・
	//	// 敵同士が重ならないようにする
	//	for (size_t i = 0; i < m_enemy.size(); ++i)
	//	{
	//		for (size_t j = i + 1; j < m_enemy.size(); ++j)
	//		{
	//			bool hit = m_enemy[i]->GetBoundingSphere().Intersects(m_enemy[j]->GetBoundingSphere());
	//			m_enemy[i]->SetHitToOtherEnemy(hit);
	//			m_enemy[j]->SetHitToOtherEnemy(hit);
	//			if (hit)m_enemy[i]->CheckHitOtherEnemy(m_enemy[i]->GetBoundingSphere(), m_enemy[j]->GetBoundingSphere());
	//		}
	//	}
	//
	//	// 敵とプレイヤーの一定範囲との当たり判定・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・
	//	for (auto& enemy : m_enemy)
	//	{
	//		m_isHitPlayerToEnemy = false;
	//		// 敵を更新
	//		enemy->Update(elapsedTime, m_playerController->GetPlayerPosition());
	//		// 敵の弾がプレイヤーに当たったら
	//		bool hit = enemy->GetBulletHitToPlayer();
	//		if (hit)
	//		{
	//			enemy->SetPlayerHP(m_playerHP);
	//			enemy->SetBulletHitToPlayer(false);
	//			m_audioManager->PlaySound("Damage", 0.5);// ダメージSEを再生
	//		}
	//		// 敵がプレイヤーに当たったら
	//		if (enemy->GetBoundingSphere().Intersects(m_inPlayerArea))
	//		{
	//			m_isHitPlayerToEnemy = true;
	//			// 押し返し用にプレイヤーの一定範囲境界球をローカル変数にコピー
	//			BoundingSphere playerSphere = m_inPlayerArea;
	//			// 境界球の範囲を調整
	//			playerSphere.Radius /= 3.0f;
	//			// プレイヤーの境界球と敵の境界球が重なっていたら
	//			if (enemy->GetBoundingSphere().Intersects(playerSphere)) enemy->CheckHitOtherEnemy(enemy->GetBoundingSphere(), playerSphere);
	//		}
	//		// プレイヤーと敵の当たり判定を設定
	//		enemy->SetHitToPlayer(m_isHitPlayerToEnemy);
	//		enemy->SetPlayerBoundingSphere(m_playerSphere);
	//	}
	//	// 敵の削除・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・
	//	// 削除対象を保持するベクター
	//	std::vector<std::unique_ptr<Enemy>> enemiesToRemove;
	//	// 削除対象を収集する
	//	for (auto it = m_enemy.begin(); it != m_enemy.end(); )
	//	{
	//		// 敵が死んでいたら
	//		if ((*it)->GetEnemyIsDead())
	//		{
	//			// 敵の座標を渡して爆破エフェクトを再生
	//			m_effect.push_back(std::make_unique<Effect>(m_commonResources,
	//														Effect::ParticleType::ENEMY_DEAD,
	//														(*it)->GetPosition(),
	//														(*it)->GetMatrix()));
	//			m_audioManager->PlaySound("EnemyDead", 2);// 敵のSEを再生
	//			enemiesToRemove.push_back(std::move(*it));// 削除対象に追加
	//			it = m_enemy.erase(it);  // 削除してイテレータを更新
	//		}
	//		else it++;  // 次の要素へ
	//	}
}

//---------------------------------------------------------
// 描画
//---------------------------------------------------------
void Enemies::Draw()
{
	// do nothing.
}
