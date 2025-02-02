/*
	@file	Enemy.h
	@brief	敵クラス
*/
#pragma once
#include "Game/KumachiLib/AudioManager/AudioManager.h"
#include "Game/Enemy/Parameters/EnemyParameters.h"
#include "Game/Interface/IEnemy.h"
//前方宣言
class CommonResources;
class PlayScene;
class Player;
class EnemyAI;
class EnemyHPBar;
class EnemyBullet;
class EnemyModel;
class EnemyManager;
class FPS_Camera;
class Enemy : public IEnemy
{
private:
	// 共通リソース
	CommonResources* m_commonResources;

	std::unique_ptr<EnemyModel>		m_enemyModel;// 敵のモデル
	std::unique_ptr<EnemyAI>		m_enemyAI;// 敵のAI
	std::unique_ptr<EnemyHPBar>		m_pHPBar;// 敵のHPバー
	// プレイヤーのポインター
	Player* m_pPlayer;
	FPS_Camera* m_pCamera;
	// 敵の情報
	DirectX::SimpleMath::Vector3 m_position;		// 座標
	DirectX::SimpleMath::Vector3 m_velocity;		// 速度
	DirectX::SimpleMath::Vector3 m_rotate;		// 回転
	// 砲塔境界球
	DirectX::BoundingSphere m_enemyBS;	//敵の境界球
	DirectX::BoundingSphere m_enemyBSToPlayerArea;// 敵とPlayerとの一定範囲の当たり判定に使う
	DirectX::BoundingSphere m_enemyBulletBS;// 敵の弾の境界球
	DirectX::BoundingSphere m_playerBS;// プレイヤーの境界球
	DirectX::SimpleMath::Matrix m_matrix;// マトリクス

	int m_currentHP;//敵の体力
	bool m_isDead;//敵のHPが0になったらTrue
	bool m_isHitToPlayer;// プレイヤーとの判定
	bool m_isHitToOtherEnemy;// その他の敵との判定
	bool m_isEnemyHitByPlayerBullet;// 敵がプレイヤーの弾に当たったか
	bool m_isPlayerHitByEnemyBullet;// 敵の弾がプレイヤーに当たったか
	bool m_canAttack;// 攻撃可能か
	float m_attackCooldown;  // 攻撃のクールダウンタイム
	// オーディオマネージャー
	AudioManager* m_audioManager;


public:
	//	getter
	const DirectX::BoundingSphere& GetBoundingSphere() override { return m_enemyBS; }
	const DirectX::BoundingSphere& GetBulletBoundingSphere() override { return m_enemyBulletBS; }
	const DirectX::BoundingSphere& GetPlayerBoundingSphere() override { return m_playerBS; }
	const DirectX::SimpleMath::Matrix& GetMatrix()  override { return m_matrix; }
	const DirectX::SimpleMath::Vector3& GetPosition()  override { return m_position; }
	const DirectX::SimpleMath::Vector3& GetVelocity()  override { return m_velocity; }
	const DirectX::SimpleMath::Vector3& GetRotate()  override { return m_rotate; }
	Player* GetPlayer()const override { return m_pPlayer; }
	FPS_Camera* GetCamera()const override { return m_pCamera; }
	void SetAudioManager(AudioManager* audioManager) override { m_audioManager = audioManager; }
	int GetEnemyHP() const override { return m_currentHP; }
	bool GetEnemyIsDead() const override { return m_isDead; }
	bool GetHitToPlayer()const override { return m_isHitToPlayer; }
	bool GetHitToOtherEnemy() const override { return m_isHitToOtherEnemy; }
	bool GetPlayerHitByEnemyBullet() const override { return m_isPlayerHitByEnemyBullet; }// 敵の弾がプレイヤーに当たったか
	bool GetEnemyHitByPlayerBullet()const override { return m_isEnemyHitByPlayerBullet; }// プレイヤーの弾が敵に当たったか
	float GetToPlayerDamage() const override { return EnemyParameters::NORMAL_ENEMY_DAMAGE; }
	bool GetCanAttack() const override { return m_canAttack; }// 攻撃可能か
	// setter
	void SetPosition(DirectX::SimpleMath::Vector3& pos)override { m_position = pos; }
	void SetEnemyHP(int hp)override { m_currentHP -= hp; }
	void SetEnemyIsDead(bool isDead)override { m_isDead = isDead; }
	void SetHitToPlayer(bool isHitToPlayer)override { m_isHitToPlayer = isHitToPlayer; }
	void SetHitToOtherEnemy(bool isHitToOtherEnemy) override { m_isHitToOtherEnemy = isHitToOtherEnemy; }
	void SetBulletBoundingSphere(DirectX::BoundingSphere& bs)override { m_enemyBulletBS = bs; }
	void SetPlayerBoundingSphere(DirectX::BoundingSphere playerBS)override { m_playerBS = playerBS; }
	void SetPlayerHP(float& HP) const override { HP -= EnemyParameters::NORMAL_ENEMY_DAMAGE; }
	void SetPlayerHitByEnemyBullet(bool hit)override { m_isPlayerHitByEnemyBullet = hit; }// 敵の弾がプレイヤーに当たったか
	void SetEnemyHitByPlayerBullet(bool hit) override { m_isEnemyHitByPlayerBullet = hit; }// 敵がプレイヤーの弾に当たったか
	void SetCanAttack(bool canAttack)override { m_canAttack = canAttack; }// 攻撃可能か
	void SetCamera(FPS_Camera* camera) { m_pCamera = camera; }

	BulletManager* GetBulletManager()const override { return m_pBulletManager; }
	void SetBulletManager(BulletManager* bulletManager) override { m_pBulletManager = bulletManager; }

public:
	// 初期ステータスを設定
	Enemy(Player* pPlayer, CommonResources* resources, int hp);
	~Enemy();
	void Initialize() override;
	void Update(float elapsedTime) override;
	void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj) override;
	void DrawCollision(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj) override;
private:
	void ShootBullet();// 弾を撃つ
};