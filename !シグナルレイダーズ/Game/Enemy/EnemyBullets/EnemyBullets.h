/*
	@file	EnemyBullets.h
	@brief	敵の弾全体を管理するクラス
	作成者：くまち
*/
#pragma once
#include "Game/Enemy/Enemy.h"
#include "Game/Interface/IEnemy.h"
#include "Game/Enemy/EnemyBullet/EnemyBullet.h"
#include "Game/BulletTrail/BulletTrail.h"
#include "Game/CommonResources.h"
#include "Game/Player/Player.h"

class CommonResources;
class EnemyBullet;
class BulletTrail;
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
	bool m_isBullethit = false;// 敵の弾がプレイヤーに当たったか
	std::vector<std::unique_ptr<EnemyBullet>> m_bullets; // 弾のリスト
	std::vector<std::unique_ptr<BulletTrail>> m_bulletTrails; // 弾の軌跡リスト
	DirectX::SimpleMath::Vector3 m_position;		// 敵の座標
	// 関数
	EnemyBullets(IEnemy* pEnemy);
	~EnemyBullets();
	void Initialize(CommonResources* resources);
	void Update(float elapsedTime, DirectX::SimpleMath::Vector3 enemyPos);
	void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);
	void CreateBullet(DirectX::SimpleMath::Vector3 enemyPos, DirectX::SimpleMath::Vector3 dir, DirectX::SimpleMath::Vector3 playerPos, float size);
	// Getter
	DirectX::BoundingSphere& GetBulletBoundingSphere() { return m_enemyBulletBS; }
	DirectX::BoundingSphere& GetPlayerBoundingSphere() { return m_playerBS; }
	// Setter
	void SetBulletBoundingSphere(DirectX::BoundingSphere& bs) { m_enemyBulletBS = bs; }
	void SetPlayerBoundingSphere(DirectX::BoundingSphere playerBS) { m_playerBS = playerBS; }
	void SetBulletHitToPlayer(bool hit) { m_isBullethit = hit; }// 敵の弾がプレイヤーに当たったか
};