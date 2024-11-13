/*
	@file	Boss.h
	@brief	ボスクラス
*/
#pragma once
#include "Game/KumachiLib/AudioManager.h"
#include "Game/Interface/IEnemy.h"
#include "Game/Enemy/EnemyBullet/EnemyBullet.h"
//前方宣言
class CommonResources;
class PlayScene;
class Player;
class FPS_Camera;
class BossAI;
class BossModel;
class EnemyHPBar;
class EnemyBullet;
class EnemyBullets;
class Enemies;
class Boss : public IEnemy
{
public:
	enum class BossBulletType
	{
		NORMAL = 0,	// 一発
		TWIN,		// 二発
		THREE,		// 三発
		SPIRAL,		// 三発螺旋
	};
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
	std::unique_ptr<BossAI>		m_pBossAI;// AI
	std::unique_ptr<BossModel>		m_bossModel;//モデル
	std::unique_ptr<EnemyHPBar>		m_HPBar;// HPバー
	std::unique_ptr<EnemyBullets>	m_enemyBullets;// 弾
	std::vector<std::unique_ptr<EnemyBullet>> m_bullets; // 弾のリスト
	const float BULLET_SIZE = 0.25f; // 弾の大きさ



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
	// 弾の発射位置
	// 中央
	DirectX::SimpleMath::Vector3 m_bulletPosCenter;
	// 左
	DirectX::SimpleMath::Vector3 m_bulletPosLeft;
	// 右
	DirectX::SimpleMath::Vector3 m_bulletPosRight;
	// 発射位置を回転させるためのクォータニオン
	DirectX::SimpleMath::Quaternion m_bulletQuaternion;
	// 弾のタイプ
	BossBulletType m_bossBulletType;// Enemiesクラスで設定する
	EnemyBullet::BulletType m_bulletType;// EnemyBulletクラスに送る
	// 弾の飛ぶ方向
	DirectX::SimpleMath::Vector3 m_bulletDirection;
	// 時間
	float m_time;
	// 敵のステータス
	int m_currentHP;//敵の体力
	bool m_isDead = false;//敵のHPが0になったらTrue
	bool m_isHit = false;// プレイヤーとの判定
	bool m_isHitToOtherEnemy = false;// その他の敵との判定
	bool m_isHitToPlayerBullet = false;// 敵がプレイヤーの弾に当たったか
	bool m_isBullethit = false;// 敵の弾がプレイヤーに当たったか
	float m_attackCooldown;  // 攻撃のクールダウンタイ
	// プレイヤーに与えるダメージ
	const float PLAYER_DAMAGE = 5.0f;
	// オーディオマネージャー
	AudioManager* m_audioManager;
public:
	//	getter
	DirectX::BoundingSphere& GetBoundingSphere() override { return m_enemyBS; }
	DirectX::BoundingSphere& GetBulletBoundingSphere()override { return m_enemyBulletBS; }
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
	void SetPosition(DirectX::SimpleMath::Vector3& pos) override { m_position = pos; }
	void SetEnemyHP(int hp) override { m_currentHP = hp; }
	void SetEnemyIsDead(bool isDead) override { m_isDead = isDead; }
	void SetHitToPlayer(bool isHitToPlayer)override { m_isHit = isHitToPlayer; }
	void SetHitToOtherEnemy(bool isHitToOtherEnemy) override { m_isHitToOtherEnemy = isHitToOtherEnemy; }
	void SetBulletBoundingSphere(DirectX::BoundingSphere& bs) override { m_enemyBulletBS = bs; }
	void SetPlayerBoundingSphere(DirectX::BoundingSphere playerBS) override { m_playerBS = playerBS; }
	void SetPlayerHP(float& HP) const override { HP -= PLAYER_DAMAGE; }
	void SetBulletHitToPlayer(bool hit) override { m_isBullethit = hit; }// 敵の弾がプレイヤーに当たったか
	void SetHitToPlayerBullet(bool hit)override { m_isHitToPlayerBullet = hit; }
public:
	// 初期ステータスを設定
	Boss(Player* pPlayer);
	~Boss();
	void Initialize(CommonResources* resources, int hp) override;// 初期化
	void Update(float elapsedTime, DirectX::SimpleMath::Vector3 playerPos)override;// 更新
	void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)override;// 描画
	void CheckHitOtherObject(DirectX::BoundingSphere& A, DirectX::BoundingSphere& B)override;// 衝突判定

	void SetBulletType(BossBulletType bossBulletType) { m_bossBulletType = bossBulletType; };// 弾のタイプ設定
	BossBulletType GetBulletType() const { return m_bossBulletType; };// 弾のタイプ取得
private:
	void ShootBullet();// 弾発射
	void BulletPotsitioning();// 弾の位置設定
	void CreateBullet();// 弾を生成
	void CreateCenterBullet(EnemyBullet::BulletType type);// 中央の弾を生成
	void CreateLeftBullet(float angleOffset, EnemyBullet::BulletType type);// 左の弾を生成
	void CreateRightBullet(float angleOffset, EnemyBullet::BulletType type);// 右の弾を生成
	void CreateVerticalBullet();// 垂直の弾を生成
};