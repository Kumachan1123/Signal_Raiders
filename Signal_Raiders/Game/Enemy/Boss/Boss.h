/*
	@file	Boss.h
	@brief	ボスクラス
*/
#pragma once
#include "Game/KumachiLib/AudioManager/AudioManager.h"
#include "Game/Interface/IEnemy.h"
#include "Game/Enemy/EnemyBullet/EnemyBullet.h"
#include "Game/KumachiLib/DrawCollision/DrawCollision.h"
#include "Game/Enemy/Boss/BossSheild/BossSheild.h"
//前方宣言
class CommonResources;
class PlayScene;
class Player;
class FPS_Camera;
class BossAI;
class BossModel;
class BossSheild;
class EnemyHPBar;
class EnemyBullet;
class EnemyBullets;
class EnemyManager;
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
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pInputLayout;
	std::unique_ptr<BossAI>		m_pBossAI;// AI
	std::unique_ptr<BossModel>		m_pBossModel;//モデル
	std::unique_ptr<BossSheild>		m_pBossSheild;// シールド
	std::unique_ptr<EnemyHPBar>		m_pHPBar;// HPバー
	std::unique_ptr<EnemyBullets>	m_pEnemyBullets;// 弾
	const float BULLET_SIZE = 0.25f; // 弾の大きさ



	// プレイヤーのポインター
	Player* m_pPlayer;
	FPS_Camera* m_pCamera;

	// 敵の情報
	DirectX::SimpleMath::Vector3 m_position;		// 座標
	DirectX::SimpleMath::Vector3 m_velocity;		// 速度
	DirectX::SimpleMath::Vector3 m_rotate;		// 回転
	// 砲塔境界球
	DirectX::BoundingSphere m_bossBS;	//敵の境界球
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
	int m_maxHP;//敵の最大体力
	bool m_isDead = false;//敵のHPが0になったらTrue
	bool m_isHitToPlayer = false;// プレイヤーとの判定
	bool m_isHitToOtherEnemy = false;// その他の敵との判定
	bool m_isEnemyHitByPlayerBullet = false;// 敵がプレイヤーの弾に当たったか
	bool m_isPlayerHitByEnemyBullet = false;// 敵の弾がプレイヤーに当たったか
	float m_attackCooldown;  // 攻撃のクールダウン(フレームごとに発射することを防ぐ用）
	float m_bulletCooldown;  // 弾のクールダウン
	bool m_canAttack = false;// 攻撃可能か

	// オーディオマネージャー
	AudioManager* m_audioManager;
	// プレイヤーのカメラの情報
	DirectX::SimpleMath::Vector3 m_cameraEye;
	DirectX::SimpleMath::Vector3 m_cameraTarget;
	DirectX::SimpleMath::Vector3 m_cameraUp;

private:
	// 定数
	// 初期位置
	const DirectX::SimpleMath::Vector3 INITIAL_POSITION = DirectX::SimpleMath::Vector3(0.0f, 10.0f, 0.0f);
	// プレイヤーに与えるダメージ
	const float PLAYER_DAMAGE = 5.0f;
	// 弾の角度オフセット
	const float ANGLE_OFFSET = 30.0f;
	// 境界球のオフセット
	const DirectX::SimpleMath::Vector3 SPHERE_OFFSET = DirectX::SimpleMath::Vector3(0.0f, 0.5f, 0.0f);
	// 境界球の半径
	const float SPHERE_RADIUS = 2.5f;
	// ボスの頭の砲口のオフセット
	const DirectX::SimpleMath::Vector3 TOP_OFFSET = DirectX::SimpleMath::Vector3(0.0f, 2.5f, 3.0f);
	// ボスの左の砲口のオフセット
	const DirectX::SimpleMath::Vector3 LEFT_OFFSET = DirectX::SimpleMath::Vector3(-2.5f, 1.0f, 3.0f);
	// ボスの右の砲口のオフセット
	const DirectX::SimpleMath::Vector3 RIGHT_OFFSET = DirectX::SimpleMath::Vector3(2.5f, 1.0f, 3.0f);
	// HPバーのオフセット
	const DirectX::SimpleMath::Vector3 HPBAR_OFFSET = DirectX::SimpleMath::Vector3(0.0f, 3.0f, 0.0f);
	// HPバーのスケール
	const float HPBAR_SCALE = 3.0f;

public:
	//	getter
	DirectX::BoundingSphere& GetBoundingSphere() override { return m_bossBS; }
	DirectX::BoundingSphere& GetBulletBoundingSphere()override { return m_enemyBulletBS; }
	DirectX::BoundingSphere& GetPlayerBoundingSphere() override { return m_playerBS; }
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
	bool GetEnemyHitByPlayerBullet()const override { return m_isEnemyHitByPlayerBullet; }
	bool GetCanAttack() const override { return m_canAttack; }// 攻撃可能か
	float GetToPlayerDamage() const override { return PLAYER_DAMAGE; }
	DirectX::SimpleMath::Vector3 GetCameraEye()const { return m_cameraEye; }
	DirectX::SimpleMath::Vector3 GetCameraTarget()const { return m_cameraTarget; }
	DirectX::SimpleMath::Vector3 GetCameraUp()const { return m_cameraUp; }
	BossSheild* GetBossSheild()const { return m_pBossSheild.get(); }// シールド取得
	// setter
	void SetPosition(DirectX::SimpleMath::Vector3& pos) override { m_position = pos; }
	void SetEnemyHP(int hp) override;
	void SetEnemyIsDead(bool isDead) override { m_isDead = isDead; }
	void SetHitToPlayer(bool isHitToPlayer)override { m_isHitToPlayer = isHitToPlayer; }
	void SetHitToOtherEnemy(bool isHitToOtherEnemy) override { m_isHitToOtherEnemy = isHitToOtherEnemy; }
	void SetBulletBoundingSphere(DirectX::BoundingSphere& bs) override { m_enemyBulletBS = bs; }
	void SetPlayerBoundingSphere(DirectX::BoundingSphere playerBS) override { m_playerBS = playerBS; }
	void SetPlayerHP(float& HP) const override { HP -= Boss::PLAYER_DAMAGE; }
	void SetPlayerHitByEnemyBullet(bool hit) override { m_isPlayerHitByEnemyBullet = hit; }// 敵の弾がプレイヤーに当たったか
	void SetEnemyHitByPlayerBullet(bool hit)override { m_isEnemyHitByPlayerBullet = hit; }
	void SetBulletCooldown(float cooldown) { m_bulletCooldown = cooldown; }// シールドを展開した後に実行する
	void SetCanAttack(bool canAttack)override { m_canAttack = canAttack; }// 攻撃可能か
	void SetCamera(FPS_Camera* camera) { m_pCamera = camera; }
public:
	// 初期ステータスを設定
	Boss(Player* pPlayer, CommonResources* resources, int hp);
	~Boss();
	void Initialize() override;// 初期化
	void Update(float elapsedTime)override;// 更新
	void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)override;// 描画
	void DrawCollision(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj) override;// 衝突判定の描画

	void SetBulletType(BossBulletType bossBulletType) { m_bossBulletType = bossBulletType; };// 弾のタイプ設定
	BossBulletType GetBulletType() const { return m_bossBulletType; };// 弾のタイプ取得
	void PlayBarrierSE();// バリアSE再生
private:
	void ShootBullet();// 弾発射
	void BulletPositioning();// 弾の位置設定
	void CreateBullet();// 弾を生成
	void CreateCenterBullet(EnemyBullet::BulletType type);// 中央の弾を生成
	void CreateLeftBullet(float angleOffset, EnemyBullet::BulletType type);// 左の弾を生成
	void CreateRightBullet(float angleOffset, EnemyBullet::BulletType type);// 右の弾を生成
	void CreateVerticalBullet();// 垂直の弾を生成
};