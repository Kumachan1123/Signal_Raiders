/*
	@file	EnemyBullets.h
	@brief	敵の弾全体を管理するクラス
*/
#pragma once
#include "Game/Enemy/Enemy.h"
#include "Game/Interface/IEnemy.h"
#include "Game/Enemy/EnemyBullet/EnemyBullet.h"
#include "Game/Particle/Particle.h"
#include "Game/CommonResources.h"
#include "Game/Player/Player.h"

class CommonResources;
class EnemyBullet;
class Particle;
class Player;
class Enemy;
class IEnemy;
class EnemyBullets
{
public:
	// 変数
	CommonResources* m_commonResources;// 共通リソース
	DirectX::BoundingSphere m_enemyBulletBS;
	DirectX::BoundingSphere m_playerBS;
	IEnemy* m_pEnemy;// 敵のポインター
	bool m_isPlayerHitByEnemyBullet = false;// 敵の弾がプレイヤーに当たったか
	std::vector<std::unique_ptr<EnemyBullet>> m_bullets; // 弾のリスト
	std::vector<std::unique_ptr<Particle>> m_bulletTrails; // 弾の軌跡リスト
	DirectX::SimpleMath::Vector3 m_bulletSpawnPos;		// 敵の弾の発射位置
	DirectX::SimpleMath::Vector3 m_direction;		// 弾の方向
	// 関数
	EnemyBullets(IEnemy* pEnemy);
	~EnemyBullets();
	void Initialize(CommonResources* resources);
	void Update(float elapsedTime);
	void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);
	void CreateBullet(float size, EnemyBullet::BulletType type);
	// Getter
	DirectX::BoundingSphere& GetBulletBoundingSphere() { return m_enemyBulletBS; }
	DirectX::BoundingSphere& GetPlayerBoundingSphere() { return m_playerBS; }
	// Setter
	void SetBulletBoundingSphere(DirectX::BoundingSphere& bs) { m_enemyBulletBS = bs; }
	void SetPlayerBoundingSphere(DirectX::BoundingSphere playerBS) { m_playerBS = playerBS; }
	void SetPlayerHitByEnemyBullet(bool hit) { m_isPlayerHitByEnemyBullet = hit; }// 敵の弾がプレイヤーに当たったか
	void SetRotateDirection(int direction);
	void SetDirection(DirectX::SimpleMath::Vector3 dir) { m_direction = dir; }
	void SetEnemyBulletSpawnPosition(DirectX::SimpleMath::Vector3 pos) { m_bulletSpawnPos = pos; }
};