/*
	@file	AreaAttacker.h
	@brief	範囲攻撃敵クラス
*/
#pragma once
#include "Game/KumachiLib/AudioManager.h"
#include "Game/Interface/IEnemy.h"
//前方宣言
class CommonResources;
class PlayScene;
class Player;
class EnemyAI;
class EnemyHPBar;
class EnemyBullet;
class AreaAttackerModel;
class EnemyBullets;
class Enemies;
class FPS_Camera;
class AreaAttacker : public IEnemy
{
private:
	// 共通リソース
	CommonResources* m_commonResources;
	// プリミティブバッチ
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> m_primitiveBatch;
	// 	//デバッグ用
	// ベーシックエフェクト
	std::unique_ptr<DirectX::BasicEffect> m_basicEffect;
	// 入力レイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	std::unique_ptr<AreaAttackerModel>		m_pAreaAttackerModel;// 敵のモデル
	std::unique_ptr<EnemyAI>		m_enemyAI;// 敵のAI
	std::unique_ptr<EnemyHPBar>		m_HPBar;// 敵のHPバー
	std::unique_ptr<EnemyBullets>	m_enemyBullets;// 敵の弾
	std::vector<std::unique_ptr<EnemyBullet>> m_bullets; // 弾のリスト
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
	bool m_isHit;// プレイヤーとの判定
	bool m_isHitToOtherEnemy;// その他の敵との判定
	bool m_isHitToPlayerBullet;// 敵がプレイヤーの弾に当たったか
	bool m_isBullethit;// 敵の弾がプレイヤーに当たったか
	float m_attackCooldown;  // 攻撃のクールダウンタイ
	// プレイヤーに与えるダメージ
	const float PLAYER_DAMAGE = 1.0f;
	// オーディオマネージャー
	AudioManager* m_audioManager;
	// カメラ
	DirectX::SimpleMath::Vector3 m_cameraEye;
	DirectX::SimpleMath::Vector3 m_cameraTarget;
	DirectX::SimpleMath::Vector3 m_cameraUp;
public:
	//	getter
	DirectX::BoundingSphere& GetBoundingSphere() override { return m_enemyBS; }
	DirectX::BoundingSphere& GetBulletBoundingSphere() override { return m_enemyBulletBS; }
	DirectX::BoundingSphere& GetPlayerBoundingSphere() override { return m_playerBS; }
	DirectX::SimpleMath::Matrix GetMatrix() const override { return m_matrix; }
	DirectX::SimpleMath::Vector3 GetPosition() const override { return m_position; }
	DirectX::SimpleMath::Vector3 GetVelocity() const override { return m_velocity; }
	DirectX::SimpleMath::Vector3 GetRotate() const override { return m_rotate; }
	Player* GetPlayer()const override { return m_pPlayer; }
	FPS_Camera* GetCamera()const override { return m_pCamera; }
	int GetHP() const override { return m_currentHP; }
	bool GetEnemyIsDead() const override { return m_isDead; }
	bool GetHitToPlayer()const override { return m_isHit; }
	bool GetHitToOtherEnemy() const override { return m_isHitToOtherEnemy; }
	bool GetBulletHitToPlayer() const override { return m_isBullethit; }// 敵の弾がプレイヤーに当たったか
	bool GetHitToPlayerBullet()const override { return m_isHitToPlayerBullet; }
	float GetToPlayerDamage() const override { return PLAYER_DAMAGE; }
	// setter
	void SetPosition(DirectX::SimpleMath::Vector3& pos)override { m_position = pos; }
	void SetEnemyHP(int hp)override { m_currentHP -= hp; }
	void SetEnemyIsDead(bool isDead)override { m_isDead = isDead; }
	void SetHitToPlayer(bool isHitToPlayer)override { m_isHit = isHitToPlayer; }
	void SetHitToOtherEnemy(bool isHitToOtherEnemy) override { m_isHitToOtherEnemy = isHitToOtherEnemy; }
	void SetBulletBoundingSphere(DirectX::BoundingSphere& bs)override { m_enemyBulletBS = bs; }
	void SetPlayerBoundingSphere(DirectX::BoundingSphere playerBS)override { m_playerBS = playerBS; }
	void SetPlayerHP(float& HP) const override { HP -= PLAYER_DAMAGE; }
	void SetBulletHitToPlayer(bool hit)override { m_isBullethit = hit; }// 敵の弾がプレイヤーに当たったか
	void SetHitToPlayerBullet(bool hit) override { m_isHitToPlayerBullet = hit; }
	void SetCameraEye(DirectX::SimpleMath::Vector3 eye)override { m_cameraEye = eye; }
	void SetCameraTarget(DirectX::SimpleMath::Vector3 target)override { m_cameraTarget = target; }
	void SetCameraUp(DirectX::SimpleMath::Vector3 up)override { m_cameraUp = up; }
public:
	// 初期ステータスを設定
	AreaAttacker(Player* pPlayer);
	~AreaAttacker();
	void Initialize(CommonResources* resources, int hp) override;
	void Update(float elapsedTime, DirectX::SimpleMath::Vector3 playerPos) override;
	void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj) override;
	void DrawCollision(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj) override;
};

