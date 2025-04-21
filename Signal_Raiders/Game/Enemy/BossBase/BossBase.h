/*
	@file	BossBase.h
	@brief	ボスベースクラス
*/
#pragma once
// 標準ライブラリ
#include <SimpleMath.h>
// 外部ライブラリ
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
// 自作ヘッダーファイル
#include "Game/KumachiLib/AudioManager/AudioManager.h"
#include "Game/Interface/IEnemy.h"
#include "Game/Interface/IBossLogic.h"
#include "Game/Enemy/EnemyBullet/EnemyBullet.h"
#include "Game/KumachiLib/DrawCollision/DrawCollision.h"
#include "Game/Enemy/Parameters/EnemyParameters.h"
#include "Game/Enemy/EnemyHPBar/EnemyHPBar.h"
#include "Game/Enemy/Boss/Boss.h"
#include "Game/Enemy/LastBoss/LastBoss.h"
#include "Game/Enemy/Boss/BossSheild/BossSheild.h"
#include "Game/Enemy/EnemyFactory/EnemyFactory.h"
#include "Game/Enemy/EnemyType/EnemyType.h"

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
class EnemyManager;

/// ボスベースクラス(ここからBossやLastBossが派生)
class BossBase : public IEnemy
{
public:

	// ボスの弾の種類
	enum class BossBulletType
	{
		NORMAL = 0,	// 一発
		TWIN,		// 二発
		THREE,		// 三発
		SPIRAL,		// 回転弾
	};
public:
	//	getter
	DirectX::BoundingSphere& GetBoundingSphere() override { return m_bossBS; }
	DirectX::BoundingSphere& GetBulletBoundingSphere()override { return m_enemyBulletBS; }
	void SetBulletBoundingSphere(DirectX::BoundingSphere& bs) override { m_enemyBulletBS = bs; }
	DirectX::BoundingSphere& GetPlayerBoundingSphere() override { return m_playerBS; }
	void SetPlayerBoundingSphere(DirectX::BoundingSphere playerBS) override { m_playerBS = playerBS; }
	const DirectX::SimpleMath::Matrix& GetMatrix()  override { return m_matrix; }
	const DirectX::SimpleMath::Vector3& GetPosition()  override { return m_position; }
	void SetPosition(DirectX::SimpleMath::Vector3& pos) override { m_position = pos; }
	const DirectX::SimpleMath::Vector3& GetVelocity()  override { return m_velocity; }
	const DirectX::SimpleMath::Vector3& GetRotate()  override { return m_rotate; }
	DirectX::SimpleMath::Vector3 GetScale() const { return m_scale; }// スケール取得
	void SetScale(DirectX::SimpleMath::Vector3 scale) { m_scale = scale; }// スケール設定
	DirectX::SimpleMath::Quaternion GetQuaternion() const { return m_quaternion; }// クォータニオン取得
	void SetQuaternion(DirectX::SimpleMath::Quaternion quaternion) { m_quaternion = quaternion; }// クォータニオン設定
	Player* GetPlayer()const override { return m_pPlayer; }
	FPS_Camera* GetCamera()const override { return m_pCamera; }
	void SetCamera(FPS_Camera* camera) { m_pCamera = camera; }
	int GetEnemyHP() const override { return m_currentHP; }
	void SetEnemyHP(int hp) override;
	bool GetEnemyIsDead() const override { return m_isDead; }
	void SetEnemyIsDead(bool isDead) override { m_isDead = isDead; }
	bool GetHitToPlayer()const override { return m_isHitToPlayer; }
	void SetHitToPlayer(bool isHitToPlayer)override { m_isHitToPlayer = isHitToPlayer; }
	bool GetHitToOtherEnemy() const override { return m_isHitToOtherEnemy; }
	void SetHitToOtherEnemy(bool isHitToOtherEnemy) override { m_isHitToOtherEnemy = isHitToOtherEnemy; }
	bool GetPlayerHitByEnemyBullet() const override { return m_isPlayerHitByEnemyBullet; }// 敵の弾がプレイヤーに当たったか
	void SetPlayerHitByEnemyBullet(bool hit) override { m_isPlayerHitByEnemyBullet = hit; }// 敵の弾がプレイヤーに当たったか
	bool GetEnemyHitByPlayerBullet()const override { return m_isEnemyHitByPlayerBullet; }
	void SetEnemyHitByPlayerBullet(bool hit)override { m_isEnemyHitByPlayerBullet = hit; }
	float GetToPlayerDamage() const override { return EnemyParameters::BOSS_DAMAGE; }
	DirectX::SimpleMath::Vector3 GetCameraEye()const { return m_cameraEye; }
	DirectX::SimpleMath::Vector3 GetCameraTarget()const { return m_cameraTarget; }
	DirectX::SimpleMath::Vector3 GetCameraUp()const { return m_cameraUp; }
	float GetSheildSEVolume() const { return m_SEVolume - m_SEVolumeCorrection; }//シールド展開音の音量を取得
	void SetPlayerHP(float& HP) const override { HP -= EnemyParameters::BOSS_DAMAGE; }
	void SetBulletCooldown(float cooldown) { m_bulletCooldown = cooldown; }// シールドを展開した後に実行する
	void SetInitSpecialAttacCooldown(float cooldown) { m_initSpecialAttackCooldown = cooldown; }// 特殊攻撃のクールダウン
	bool GetCanAttack() const override { return m_canAttack; }// 攻撃可能か
	void SetCanAttack(bool canAttack)override { m_canAttack = canAttack; }// 攻撃可能か
	void SetAudioManager(AudioManager* audioManager) override { m_audioManager = audioManager; }// オーディオマネージャー
	BulletManager* GetBulletManager()const override { return m_pBulletManager; }// 弾の管理クラス取得
	void SetBulletManager(BulletManager* bulletManager) override { m_pBulletManager = bulletManager; }// 弾の管理クラス設定
	bool GetIsAttack() const override { return m_isAttack; }// 攻撃中か
	void SetIsAttack(bool isAttack)override { m_isAttack = isAttack; }// 攻撃中か
	void SetBossType(BossType type) { m_bossType = type; }// ボスの種類を設定
	BossSheild* GetBossSheild()const { return m_pBossSheild.get(); }// シールド取得
	BossAI* GetBossAI() { return m_pBossAI.get(); }// AI取得
	EnemyHPBar* GetHPBar() { return m_pHPBar.get(); }// HPバー取得
public:
	// 初期ステータスを設定
	BossBase(Player* pPlayer, CommonResources* resources, int hp);
	virtual ~BossBase();
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
	void CreateSpiralBullet();// 螺旋弾を生成

private:
	// 共通リソース
	CommonResources* m_commonResources;

	std::unique_ptr<BossAI>		m_pBossAI;// AI
	std::unique_ptr<BossModel>		m_pBossModel;//モデル
	std::unique_ptr<BossSheild>		m_pBossSheild;// シールド
	std::unique_ptr<EnemyHPBar>		m_pHPBar;// HPバー

	// 音量
	float m_SEVolume;// SEの音量
	float m_SEVolumeCorrection;// SEの音量補正

	// プレイヤーのポインター
	Player* m_pPlayer;
	FPS_Camera* m_pCamera;

	// 敵の情報
	DirectX::SimpleMath::Vector3 m_position;		// 座標
	DirectX::SimpleMath::Vector3 m_velocity;		// 速度
	DirectX::SimpleMath::Vector3 m_rotate;		// 回転
	DirectX::SimpleMath::Quaternion m_quaternion;	// 回転用クォータニオン
	DirectX::SimpleMath::Vector3 m_scale;		// スケール
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
	// ボスの種類
	BossType m_bossType;
	// 生成するBoss
	std::unique_ptr<IBossLogic> m_pBoss;
	// 弾の飛ぶ方向
	DirectX::SimpleMath::Vector3 m_bulletDirection;
	// 時間
	float m_time;
	// 敵のステータス
	int m_currentHP;//敵の体力
	int m_maxHP;//敵の最大体力
	bool m_isDead;//敵のHPが0になったらTrue
	bool m_isHitToPlayer;// プレイヤーとの判定
	bool m_isHitToOtherEnemy;// その他の敵との判定
	bool m_isEnemyHitByPlayerBullet;// 敵がプレイヤーの弾に当たったか
	bool m_isPlayerHitByEnemyBullet;// 敵の弾がプレイヤーに当たったか
	float m_attackCooldown;  // 攻撃のクールダウン(フレームごとに発射することを防ぐ用）
	float m_specialAttackCooldown;  // 特殊攻撃のクールダウン
	float m_initSpecialAttackCooldown;  // 特殊攻撃のクールダウン(初期化用)
	float m_bulletCooldown;  // 弾のクールダウン
	bool m_canAttack;// 攻撃可能か
	bool m_isAttack;// 攻撃中か
	// オーディオマネージャー
	AudioManager* m_audioManager;
	// プレイヤーのカメラの情報
	DirectX::SimpleMath::Vector3 m_cameraEye;
	DirectX::SimpleMath::Vector3 m_cameraTarget;
	DirectX::SimpleMath::Vector3 m_cameraUp;




};