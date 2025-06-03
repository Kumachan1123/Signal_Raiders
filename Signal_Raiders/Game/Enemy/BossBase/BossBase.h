/*
*	@file	BossBase.h
*	@brief	ボスベースクラス
*/
#pragma once
// 標準ライブラリ
#include <SimpleMath.h>
// 外部ライブラリ
#include <DeviceResources.h>
#include <Libraries/MyLib/DebugString.h>
#include <Libraries/MyLib/MemoryLeakDetector.h>
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
public:// 列挙型
	// ボスの弾の種類
	enum class BossBulletType
	{
		STAGE_1 = 0,	// ステージ1
		STAGE_2,		// ステージ2
		STAGE_3,		// ステージ3
		STAGE_4,		// ステージ4
		STAGE_5,		// ステージ5
	};
public:	// アクセサ
	// ボスの境界球取得
	DirectX::BoundingSphere& GetBoundingSphere() override { return m_bossBS; }
	// 弾の境界球取得
	DirectX::BoundingSphere& GetBulletBoundingSphere() override { return m_enemyBulletBS; }
	// 弾の境界球設定
	void SetBulletBoundingSphere(const DirectX::BoundingSphere& bs) override { m_enemyBulletBS = bs; }
	// プレイヤーの境界球取得
	DirectX::BoundingSphere& GetPlayerBoundingSphere() override { return m_playerBS; }
	// プレイヤーの境界球設定
	void SetPlayerBoundingSphere(const DirectX::BoundingSphere& playerBS) override { m_playerBS = playerBS; }
	// マトリクス取得
	const DirectX::SimpleMath::Matrix& GetMatrix() override { return m_matrix; }
	// 位置取得
	const DirectX::SimpleMath::Vector3& GetPosition() override { return m_position; }
	// 位置設定
	void SetPosition(const DirectX::SimpleMath::Vector3& pos) override { m_position = pos; }
	// 速度取得
	const DirectX::SimpleMath::Vector3& GetVelocity() override { return m_velocity; }
	// 回転取得
	const DirectX::SimpleMath::Vector3& GetRotate() override { return m_rotate; }
	// スケール取得
	DirectX::SimpleMath::Vector3 GetScale() const { return m_scale; }
	// スケール設定
	void SetScale(const DirectX::SimpleMath::Vector3& scale) { m_scale = scale; }
	// クォータニオン取得
	DirectX::SimpleMath::Quaternion GetQuaternion() const { return m_quaternion; }
	// クォータニオン設定
	void SetQuaternion(const DirectX::SimpleMath::Quaternion& quaternion) { m_quaternion = quaternion; }
	// 弾の飛ぶ方向取得
	DirectX::SimpleMath::Vector3 GetBulletDirection() const { return m_bulletDirection; }
	// 弾の飛ぶ方向設定
	void SetBulletDirection(const DirectX::SimpleMath::Vector3& direction) { m_bulletDirection = direction; }
	// プレイヤーのポインター取得
	Player* GetPlayer() const override { return m_pPlayer; }
	// カメラのポインター取得
	FPS_Camera* GetCamera() const override { return m_pCamera; }
	// カメラのポインター設定
	void SetCamera(FPS_Camera* camera) { m_pCamera = camera; }
	// 敵のHP取得
	int GetEnemyHP() const override { return m_currentHP; }
	// 敵のHP設定(ダメージ適用)
	void ApplyDamageToEnemy(int hp) override;
	// 敵の死亡フラグ取得
	bool GetEnemyIsDead() const override { return m_isDead; }
	// 敵の死亡フラグ設定
	void SetEnemyIsDead(bool isDead) override { m_isDead = isDead; }
	// プレイヤーとの当たり判定取得
	bool GetHitToPlayer() const override { return m_isHitToPlayer; }
	// プレイヤーとの当たり判定設定
	void SetHitToPlayer(bool isHitToPlayer) override { m_isHitToPlayer = isHitToPlayer; }
	// 他の敵との当たり判定取得
	bool GetHitToOtherEnemy() const override { return m_isHitToOtherEnemy; }
	// 他の敵との当たり判定設定
	void SetHitToOtherEnemy(bool isHitToOtherEnemy) override { m_isHitToOtherEnemy = isHitToOtherEnemy; }
	// 敵の弾がプレイヤーに当たったか
	bool GetPlayerHitByEnemyBullet() const override { return m_isPlayerHitByEnemyBullet; }
	// 敵の弾がプレイヤーに当たったか設定
	void SetPlayerHitByEnemyBullet(bool hit) override { m_isPlayerHitByEnemyBullet = hit; }
	// 敵がプレイヤーの弾に当たったか
	bool GetEnemyHitByPlayerBullet() const override { return m_isEnemyHitByPlayerBullet; }
	// 敵がプレイヤーの弾に当たったか設定
	void SetEnemyHitByPlayerBullet(bool hit) override { m_isEnemyHitByPlayerBullet = hit; }
	// プレイヤーに与えるダメージ取得
	float GetToPlayerDamage() const override { return m_bossDamage; }
	// プレイヤーに与えるダメージ設定
	void SetToPlayerDamage(float damage) { m_bossDamage = damage; }
	// カメラの位置取得
	DirectX::SimpleMath::Vector3 GetCameraEye() const { return m_cameraEye; }
	// カメラのターゲット取得
	DirectX::SimpleMath::Vector3 GetCameraTarget() const { return m_cameraTarget; }
	// カメラの上ベクトル取得
	DirectX::SimpleMath::Vector3 GetCameraUp() const { return m_cameraUp; }
	// シールド展開音の音量を取得
	float GetSheildSEVolume() const { return m_SEVolume - m_SEVolumeCorrection; }
	// プレイヤーのHPを減らす
	void SetPlayerHP(float& HP) const override { HP -= m_bossDamage; }
	// 弾のクールダウン設定
	void SetBulletCooldown(float cooldown) { m_bulletCooldown = cooldown; }
	// 特殊攻撃のクールダウン初期値設定
	void SetInitSpecialAttacCooldown(float cooldown) { m_initSpecialAttackCooldown = cooldown; }
	// 攻撃可能か取得
	bool GetCanAttack() const override { return m_canAttack; }
	// 攻撃可能か設定
	void SetCanAttack(bool canAttack) override { m_canAttack = canAttack; }
	// 弾の管理クラス取得
	BulletManager* GetBulletManager() const override { return m_pBulletManager; }
	// 弾の管理クラス設定
	void SetBulletManager(BulletManager* bulletManager) override { m_pBulletManager = bulletManager; }
	// 攻撃中か取得
	bool GetIsAttack() const override { return m_isAttack; }
	// 攻撃中か設定
	void SetIsAttack(bool isAttack) override { m_isAttack = isAttack; }
	// ボスの種類を設定
	void SetBossType(BossType type) { m_bossType = type; }
	// シールド取得
	BossSheild* GetBossSheild() const { return m_pBossSheild.get(); }
	// AI取得
	BossAI* GetBossAI() { return m_pBossAI.get(); }
	// HPバー取得
	EnemyHPBar* GetHPBar() { return m_pHPBar.get(); }
	// 通常時の当たり判定の半径取得
	float GetDefaultHitRadius() const { return m_defaultHitRadius; }
	// 通常時の当たり判定の半径設定
	void SetDefaultHitRadius(float radius) { m_defaultHitRadius = radius; }
	// 防御時の当たり判定の半径取得
	float GetDefensiveHitRadius() const { return m_defensiveHitRadius; }
	// 防御時の当たり判定の半径設定
	void SetDefensiveHitRadius(float radius) { m_defensiveHitRadius = radius; }
	// 弾のサイズ取得
	float GetBulletSize() const { return m_bulletSize; }
	// 弾のサイズ設定
	void SetBulletSize(float size) { m_bulletSize = size; }
	// バリア破壊のパーティクルサイズ取得
	float GetBarrierBreakSize() const { return m_barrierBreakSize; }
	// バリアの破壊パーティクルサイズ設定
	void SetBarrierBreakSize(float size) { m_barrierBreakSize = size; }
	// 死亡エフェクトのサイズ取得
	float GetDeadEffectSize() const { return m_deadEffectSize; }
	// 死亡エフェクトのサイズ設定
	void SetDeadEffectSize(float size) { m_deadEffectSize = size; }
public:// public関数
	// コンストラクタ
	BossBase(Player* pPlayer, CommonResources* resources, int hp);
	// デストラクタ
	virtual ~BossBase();
	// 初期化
	void Initialize() override;
	// 更新
	void Update(float elapsedTime) override;
	// 描画
	void Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) override;
	// 衝突判定の描画
	void DrawCollision(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) override;
	// 弾のタイプ設定
	void SetBulletType(BossBulletType bossBulletType) { m_bossBulletType = bossBulletType; }
	// 弾のタイプ取得
	BossBulletType GetBulletType() const { return m_bossBulletType; }
	// バリアSE再生
	void PlayBarrierSE();
private:// private関数
	// 弾発射
	void ShootBullet();
	// 垂直の弾を生成
	void CreateVerticalBullet();
	// 螺旋弾を生成
	void CreateSpiralBullet();
private:// privateメンバ変数
	// 共通リソース
	CommonResources* m_pCommonResources;
	// AI
	std::unique_ptr<BossAI> m_pBossAI;
	// モデル
	std::unique_ptr<BossModel> m_pBossModel;
	// シールド
	std::unique_ptr<BossSheild> m_pBossSheild;
	// HPバー
	std::unique_ptr<EnemyHPBar> m_pHPBar;
	// SEの音量
	float m_SEVolume;
	// SEの音量補正
	float m_SEVolumeCorrection;
	// バリアSEの再生フラグ
	bool m_isBarrierSEPlay;
	// プレイヤーのポインター
	Player* m_pPlayer;
	// カメラのポインター
	FPS_Camera* m_pCamera;
	// 弾管理クラス
	BulletManager* m_pBulletManager;
	// 座標
	DirectX::SimpleMath::Vector3 m_position;
	// 速度
	DirectX::SimpleMath::Vector3 m_velocity;
	// 回転
	DirectX::SimpleMath::Vector3 m_rotate;
	// 回転用クォータニオン
	DirectX::SimpleMath::Quaternion m_quaternion;
	// スケール
	DirectX::SimpleMath::Vector3 m_scale;
	// マトリクス
	DirectX::SimpleMath::Matrix m_matrix;
	// 敵の境界球
	DirectX::BoundingSphere m_bossBS;
	// 敵とPlayerとの一定範囲の当たり判定に使う
	DirectX::BoundingSphere m_enemyBSToPlayerArea;
	// 敵の弾の境界球
	DirectX::BoundingSphere m_enemyBulletBS;
	// プレイヤーの境界球
	DirectX::BoundingSphere m_playerBS;
	// 弾の発射位置
	DirectX::SimpleMath::Vector3 m_bulletPosCenter;
	// ボスがプレイヤーに与えるダメージ
	float m_bossDamage;
	// 発射位置を回転させるためのクォータニオン
	DirectX::SimpleMath::Quaternion m_bulletQuaternion;
	// Enemiesクラスで設定する
	BossBulletType m_bossBulletType;
	// EnemyBulletクラスに送る
	BulletType m_bulletType;
	// ボスの種類
	BossType m_bossType;
	// 生成するBoss
	std::unique_ptr<IBossLogic> m_pBoss;
	// 弾の飛ぶ方向
	DirectX::SimpleMath::Vector3 m_bulletDirection;
	// 時間
	float m_time;
	// 敵の体力
	int m_currentHP;
	// 敵の最大体力
	int m_maxHP;
	// 敵のHPが0になったらTrue
	bool m_isDead;
	// プレイヤーとの判定
	bool m_isHitToPlayer;
	// その他の敵との判定
	bool m_isHitToOtherEnemy;
	// 敵がプレイヤーの弾に当たったか
	bool m_isEnemyHitByPlayerBullet;
	// 敵の弾がプレイヤーに当たったか
	bool m_isPlayerHitByEnemyBullet;
	// 攻撃のクールダウン(フレームごとに発射することを防ぐ用）
	float m_attackCooldown;
	// 特殊攻撃のクールダウン
	float m_specialAttackCooldown;
	// 特殊攻撃のクールダウン(初期化用)
	float m_initSpecialAttackCooldown;
	// 弾のクールダウン
	float m_bulletCooldown;
	// 攻撃可能か
	bool m_canAttack;
	// 攻撃中か
	bool m_isAttack;
	// カメラの位置
	DirectX::SimpleMath::Vector3 m_cameraEye;
	// カメラのターゲット
	DirectX::SimpleMath::Vector3 m_cameraTarget;
	// カメラの上ベクトル
	DirectX::SimpleMath::Vector3 m_cameraUp;
	// 通常時の当たり判定の半径
	float m_defaultHitRadius;
	// 防御時の当たり判定の半径
	float m_defensiveHitRadius;
	// 弾のサイズ
	float m_bulletSize;
	// バリアの破壊パーティクルサイズ
	float m_barrierBreakSize;
	// 死亡エフェクトのサイズ
	float m_deadEffectSize;
};