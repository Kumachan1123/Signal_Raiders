/*
	@file	VerticalAttacker.h
	@brief	垂直攻撃敵クラス
*/
#pragma once
#ifndef VERTICAL_ATTACKER_DEFINED
#define VERTICAL_ATTACKER_DEFINED
// 標準ライブラリ
#include <cassert>
#include <random>
#include <memory>
// DirectX
#include <DeviceResources.h>
// 外部ライブラリ
#include <Libraries/MyLib/DebugString.h>
#include <Libraries/MyLib/MemoryLeakDetector.h>
#include <Libraries/Microsoft/ReadData.h>
#include <Libraries/Microsoft/DebugDraw.h>
// 自作ヘッダーファイル
#include "Game/CommonResources.h"
#include "Game/KumachiLib/AudioManager/AudioManager.h"
#include "Game/Enemy/Parameters/EnemyParameters.h"
#include "Game/Interface/IEnemy.h"
#include "Game/KumachiLib/KumachiLib.h"
#include <Game/KumachiLib/DrawCollision/DrawCollision.h>
#include "Game/Enemy/VerticalAttackerModel/VerticalAttackerModel.h"
#include "Game/Enemy/EnemyHPBar/EnemyHPBar.h"

//前方宣言
class CommonResources;
class PlayScene;
class Player;
class EnemyAI;
class EnemyHPBar;
class EnemyBullet;
class VerticalAttackerModel;
class EnemyManager;
class FPS_Camera;

// 垂直攻撃敵クラス
class VerticalAttacker : public IEnemy
{
public:
	// アクセサ
	// 敵の境界球取得
	DirectX::BoundingSphere& GetBoundingSphere() override { return m_enemyBS; }
	// 敵の弾の境界球取得
	DirectX::BoundingSphere& GetBulletBoundingSphere() override { return m_enemyBulletBS; }
	// 敵の弾の境界球設定
	void SetBulletBoundingSphere(const DirectX::BoundingSphere& bs) override { m_enemyBulletBS = bs; }
	// プレイヤーの境界球取得
	DirectX::BoundingSphere& GetPlayerBoundingSphere() override { return m_playerBS; }
	// プレイヤーの境界球設定
	void SetPlayerBoundingSphere(const DirectX::BoundingSphere& playerBS) override { m_playerBS = playerBS; }
	// マトリクス取得
	const DirectX::SimpleMath::Matrix& GetMatrix() override { return m_matrix; }
	// 座標取得
	const DirectX::SimpleMath::Vector3& GetPosition() override { return m_position; }
	// 座標設定
	void SetPosition(const DirectX::SimpleMath::Vector3& pos) override { m_position = pos; }
	// 速度取得
	const DirectX::SimpleMath::Vector3& GetVelocity() override { return m_velocity; }
	// 回転取得
	const DirectX::SimpleMath::Vector3& GetRotate() override { return m_rotate; }
	// プレイヤー取得
	Player* GetPlayer() const override { return m_pPlayer; }
	// カメラ取得
	FPS_Camera* GetCamera() const override { return m_pCamera; }
	// カメラ設定
	void SetCamera(FPS_Camera* camera) { m_pCamera = camera; }
	// 敵のHP取得
	int GetEnemyHP() const override { return m_currentHP; }
	// 死亡フラグ取得
	bool GetEnemyIsDead() const override { return m_isDead; }
	// 死亡フラグ設定
	void SetEnemyIsDead(bool isDead) override { m_isDead = isDead; }
	// プレイヤーとの当たり判定取得
	bool GetHitToPlayer() const override { return m_isHitToPlayer; }
	// プレイヤーとの当たり判定設定
	void SetHitToPlayer(bool isHitToPlayer) override { m_isHitToPlayer = isHitToPlayer; }
	// その他の敵との当たり判定取得
	bool GetHitToOtherEnemy() const override { return m_isHitToOtherEnemy; }
	// その他の敵との当たり判定設定
	void SetHitToOtherEnemy(bool isHitToOtherEnemy) override { m_isHitToOtherEnemy = isHitToOtherEnemy; }
	// 敵の弾がプレイヤーに当たったか取得
	bool GetPlayerHitByEnemyBullet() const override { return m_isPlayerHitByEnemyBullet; }
	// 敵の弾がプレイヤーに当たったか設定
	void SetPlayerHitByEnemyBullet(bool hit) override { m_isPlayerHitByEnemyBullet = hit; }
	// 敵がプレイヤーの弾に当たったか取得
	bool GetEnemyHitByPlayerBullet() const override { return m_isEnemyHitByPlayerBullet; }
	// 敵がプレイヤーの弾に当たったか設定
	void SetEnemyHitByPlayerBullet(bool hit) override { m_isEnemyHitByPlayerBullet = hit; }
	// プレイヤーに与えるダメージ取得
	float GetToPlayerDamage() const override { return EnemyParameters::NORMAL_ENEMY_DAMAGE; }
	// 攻撃可能か取得
	bool GetCanAttack() const override { return m_canAttack; }
	// 攻撃可能か設定
	void SetCanAttack(bool canAttack) override { m_canAttack = canAttack; }
	// 敵のHP設定(ダメージ適用)
	void ApplyDamageToEnemy(int hp) override { m_currentHP -= hp; }
	// プレイヤーのHP設定
	void SetPlayerHP(float& HP) const override { HP -= EnemyParameters::NORMAL_ENEMY_DAMAGE; }
	// 弾管理クラス取得
	BulletManager* GetBulletManager() const override { return m_pBulletManager; }
	// 弾管理クラス設定
	void SetBulletManager(BulletManager* bulletManager) override { m_pBulletManager = bulletManager; }
	// 攻撃中か取得
	bool GetIsAttack() const override { return m_isAttack; }
	// 攻撃中か設定
	void SetIsAttack(bool isAttack) override { m_isAttack = isAttack; }
public:
	// public関数
	// コンストラクタ
	VerticalAttacker(Player* pPlayer, CommonResources* resources, int hp);
	// デストラクタ
	~VerticalAttacker();
	// 初期化
	void Initialize() override;
	// 更新
	void Update(float elapsedTime) override;
	// 描画
	void Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) override;
	// 当たり判定描画
	void DrawCollision(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) override;
private:
	// private関数
	// 弾を撃つ
	void ShootBullet();
private:
	// privateメンバ変数
	// 共通リソース
	CommonResources* m_pCommonResources;
	// 敵のモデル
	std::unique_ptr<VerticalAttackerModel>	m_pVerticalAttackerModel;
	// 敵のAI
	std::unique_ptr<EnemyAI>		m_pEnemyAI;
	// 敵のHPバー
	std::unique_ptr<EnemyHPBar>		m_pHPBar;
	// プレイヤーのポインター
	Player* m_pPlayer;
	// カメラのポインター
	FPS_Camera* m_pCamera;
	// 弾管理クラス
	BulletManager* m_pBulletManager;
	// 敵の座標
	DirectX::SimpleMath::Vector3 m_position;
	// 敵の速度
	DirectX::SimpleMath::Vector3 m_velocity;
	// 敵の回転
	DirectX::SimpleMath::Vector3 m_rotate;
	// 敵の境界球
	DirectX::BoundingSphere m_enemyBS;
	// 敵とPlayerとの一定範囲の当たり判定に使う
	DirectX::BoundingSphere m_enemyBSToPlayerArea;
	// 敵の弾の境界球
	DirectX::BoundingSphere m_enemyBulletBS;
	// プレイヤーの境界球
	DirectX::BoundingSphere m_playerBS;
	// マトリクス
	DirectX::SimpleMath::Matrix m_matrix;
	// 敵の体力
	int m_currentHP;
	// 死亡フラグ
	bool m_isDead;
	// プレイヤーとの判定
	bool m_isHitToPlayer;
	// その他の敵との判定
	bool m_isHitToOtherEnemy;
	// 敵がプレイヤーの弾に当たったか
	bool m_isEnemyHitByPlayerBullet;
	// 敵の弾がプレイヤーに当たったか
	bool m_isPlayerHitByEnemyBullet;
	// 攻撃可能か
	bool m_canAttack;
	// 攻撃中か
	bool m_isAttack;
	// 攻撃のクールダウンタイム
	float m_attackCooldown;
};
#endif // VERTICAL_ATTACKER_DEFINED

