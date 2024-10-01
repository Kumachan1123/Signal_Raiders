/*
	@file	EnemyBase.h
	@brief	敵の基底クラス
	作成者：くまち
*/
#pragma once
#include "Game/KumachiLib/AudioManager.h"

class Player;
class EnemyModel;
class EnemyBullets;
class EnemyHPBar;
class CommonResources;
class IEnemy
{
protected:
	// 共通リソース
	CommonResources* m_commonResources;
	// プレイヤーのポインター
	Player* m_pPlayer;
	// 敵の情報
	DirectX::SimpleMath::Vector3 m_position;	// 座標
	DirectX::SimpleMath::Vector3 m_velocity;	// 速度
	DirectX::SimpleMath::Vector3 m_rotate;		// 回転
	DirectX::SimpleMath::Matrix m_matrix;		// マトリクス
	DirectX::BoundingSphere m_boundingSphere;	// 敵の境界球
	DirectX::BoundingSphere m_bulletBoundingSphere;	// 敵の弾の境界球
	DirectX::BoundingSphere m_playerBoundingSphere;	// プレイヤーの境界球
	// 体力
	int m_currentHP;
	bool m_isDead = false;	// 敵が死んだかどうか
	bool m_isHit = false;	// プレイヤーとの判定
	bool m_isHitToOtherEnemy = false;	// 他の敵との判定
	bool m_isHitToPlayerBullet = false;	// プレイヤーの弾に当たったか
	bool m_isBulletHit = false;	// 敵の弾がプレイヤーに当たったか
	float m_attackCooldown = 3.0f;	// 攻撃のクールダウン
	const float PLAYER_DAMAGE = 1.0f; // プレイヤーに与えるダメージ
	// モデル
	std::unique_ptr<EnemyModel> m_enemyModel;
	// HPバー
	std::unique_ptr<EnemyHPBar> m_HPBar;
	// 弾
	std::unique_ptr<EnemyBullets> m_enemyBullets;
	// オーディオマネージャー
	AudioManager* m_audioManager;

public:
	// コンストラクタ・デストラクタ
	IEnemy(Player* pPlayer);
	virtual ~IEnemy() = default;

	// getter
	DirectX::BoundingSphere& GetBoundingSphere() { return m_boundingSphere; }
	DirectX::BoundingSphere& GetBulletBoundingSphere() { return m_bulletBoundingSphere; }
	DirectX::BoundingSphere& GetPlayerBoundingSphere() { return m_playerBoundingSphere; }
	DirectX::SimpleMath::Matrix GetMatrix() const { return m_matrix; }
	DirectX::SimpleMath::Vector3 GetPosition() const { return m_position; }
	DirectX::SimpleMath::Vector3 GetVelocity() const { return m_velocity; }
	DirectX::SimpleMath::Vector3 GetRotate() const { return m_rotate; }
	Player* GetPlayer() const { return m_pPlayer; }
	int GetHP() const { return m_currentHP; }
	bool IsDead() const { return m_isDead; }
	bool IsHitToPlayer() const { return m_isHit; }
	bool IsHitToOtherEnemy() const { return m_isHitToOtherEnemy; }
	bool IsBulletHitToPlayer() const { return m_isBulletHit; }
	bool IsHitToPlayerBullet() const { return m_isHitToPlayerBullet; }
	float GetToPlayerDamage() const { return PLAYER_DAMAGE; }
	// setter
	void SetPosition(DirectX::SimpleMath::Vector3& pos) { m_position = pos; }
	void SetHP(int hp) { m_currentHP = hp; }
	void SetHitToPlayer(bool isHitToPlayer) { m_isHit = isHitToPlayer; }
	void SetHitToOtherEnemy(bool isHitToOtherEnemy) { m_isHitToOtherEnemy = isHitToOtherEnemy; }
	void SetBulletBoundingSphere(DirectX::BoundingSphere& bs) { m_bulletBoundingSphere = bs; }
	void SetPlayerBoundingSphere(DirectX::BoundingSphere playerBS) { m_playerBoundingSphere = playerBS; }
	void SetPlayerHP(float& HP) const { HP -= PLAYER_DAMAGE; }
	void SetBulletHitToPlayer(bool hit) { m_isBulletHit = hit; }
	void SetHitToPlayerBullet(bool hit) { m_isHitToPlayerBullet = hit; }

	// 仮想関数
	virtual void Initialize(CommonResources* resources, int hp) = 0;
	virtual void Update(float elapsedTime, DirectX::SimpleMath::Vector3 playerPos) = 0;
	virtual void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj) = 0;
	virtual void CheckHitOtherObject(DirectX::BoundingSphere& A, DirectX::BoundingSphere& B) = 0;

};