/*
*	@file	Enemy.h
*	@brief	敵クラス
*/
#pragma once
#ifndef ENEMY_DEFINED
#define ENEMY_DEFINED
// 標準ライブラリ
#include <cassert>
#include <random>
#include <memory>
// DirectX
#include <SimpleMath.h>
#include <DeviceResources.h>
// 外部ライブラリ
#include <Libraries/MyLib/DebugString.h>
#include <Libraries/MyLib/MemoryLeakDetector.h>
#include <Libraries/Microsoft/DebugDraw.h>
#include <Libraries/Microsoft/ReadData.h>
// 自作ヘッダーファイル
#include "Game/CommonResources.h"
#include "Game/Enemy/EnemyHPBar/EnemyHPBar.h"
#include "Game/Enemy/EnemyBullet/EnemyBullet.h"
#include "Game/Enemy/EnemyModel/EnemyModel.h"
#include "Game/KumachiLib/KumachiLib.h"
#include <Game/KumachiLib/DrawCollision/DrawCollision.h>
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

// 敵クラス
class Enemy : public IEnemy
{
public:
	// アクセサ
	// 敵の当たり判定を取得する
	DirectX::BoundingSphere& GetBoundingSphere() override { return m_enemyBS; }
	// 敵の弾の当たり判定を取得する
	DirectX::BoundingSphere& GetBulletBoundingSphere() override { return m_enemyBulletBS; }
	// 敵の弾の当たり判定を設定する
	void SetBulletBoundingSphere(const DirectX::BoundingSphere& bs) override { m_enemyBulletBS = bs; }
	// プレイヤーの当たり判定を取得する
	DirectX::BoundingSphere& GetPlayerBoundingSphere() override { return m_playerBS; }
	// プレイヤーの当たり判定を設定する
	void SetPlayerBoundingSphere(const DirectX::BoundingSphere& playerBS) override { m_playerBS = playerBS; }
	// 敵のワールド行列を取得する
	const DirectX::SimpleMath::Matrix& GetMatrix() override { return m_matrix; }
	// 敵の現在の座標を取得する
	const DirectX::SimpleMath::Vector3& GetPosition() override { return m_position; }
	// 敵の座標を設定する
	void SetPosition(const DirectX::SimpleMath::Vector3& pos) override { m_position = pos; }
	// 敵の移動速度を取得する
	const DirectX::SimpleMath::Vector3& GetVelocity() override { return m_velocity; }
	// 敵の回転角度を取得する
	const DirectX::SimpleMath::Vector3& GetRotate() override { return m_rotate; }
	// 対象となるプレイヤーのポインタを取得する
	Player* GetPlayer() const override { return m_pPlayer; }
	// 使用するカメラのポインタを取得する
	FPS_Camera* GetCamera() const override { return m_pCamera; }
	// カメラのポインタを設定する
	void SetCamera(FPS_Camera* camera) { m_pCamera = camera; }
	// 敵の現在HPを取得する
	int GetEnemyHP() const override { return m_currentHP; }
	// 敵が死亡状態かどうかを取得する
	bool GetEnemyIsDead() const override { return m_isDead; }
	// 敵の死亡状態を設定する
	void SetEnemyIsDead(bool isDead) override { m_isDead = isDead; }
	// プレイヤーに当たったかどうかを取得する
	bool GetHitToPlayer() const override { return m_isHitToPlayer; }
	// プレイヤーに当たったかどうかを設定する
	void SetHitToPlayer(bool isHitToPlayer) override { m_isHitToPlayer = isHitToPlayer; }
	// 他の敵に当たったかどうかを取得する
	bool GetHitToOtherEnemy() const override { return m_isHitToOtherEnemy; }
	// 他の敵に当たったかどうかを設定する
	void SetHitToOtherEnemy(bool isHitToOtherEnemy) override { m_isHitToOtherEnemy = isHitToOtherEnemy; }
	// 敵の弾がプレイヤーに命中したか取得する
	bool GetPlayerHitByEnemyBullet() const override { return m_isPlayerHitByEnemyBullet; }
	// 敵の弾がプレイヤーに命中したか設定する
	void SetPlayerHitByEnemyBullet(bool hit) override { m_isPlayerHitByEnemyBullet = hit; }
	// プレイヤーの弾が敵に命中したか取得する
	bool GetEnemyHitByPlayerBullet() const override { return m_isEnemyHitByPlayerBullet; }
	// プレイヤーの弾が敵に命中したか設定する
	void SetEnemyHitByPlayerBullet(bool hit) override { m_isEnemyHitByPlayerBullet = hit; }
	// プレイヤーに与えるダメージ量を取得する
	float GetToPlayerDamage() const override { return EnemyParameters::NORMAL_ENEMY_DAMAGE; }
	// 現在攻撃可能な状態かどうかを取得する
	bool GetCanAttack() const override { return m_canAttack; }
	// 攻撃可能状態かどうかを設定する
	void SetCanAttack(bool canAttack) override { m_canAttack = canAttack; }
	// 敵にダメージを与えてHPを減らす
	void ApplyDamageToEnemy(int hp) override { m_currentHP -= hp; }
	// プレイヤーのHPにダメージを反映する
	void SetPlayerHP(float& HP) const override { HP -= EnemyParameters::NORMAL_ENEMY_DAMAGE; }
	// 弾の管理クラスを取得する
	BulletManager* GetBulletManager() const override { return m_pBulletManager; }
	// 弾の管理クラスを設定する
	void SetBulletManager(BulletManager* bulletManager) override { m_pBulletManager = bulletManager; }
	// 敵が現在攻撃中かどうかを取得する
	bool GetIsAttack() const override { return m_isAttack; }
	// 敵が攻撃中かどうかを設定する
	void SetIsAttack(bool isAttack) override { m_isAttack = isAttack; }
public:
	//	public関数
	// コンストラクタ
	Enemy(Player* pPlayer, CommonResources* resources, int hp);
	// デストラクタ
	~Enemy();
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
	void ShootBullet(float elapsedTime);
private:
	// private変数
	// 共通リソース
	CommonResources* m_pCommonResources;
	// 敵のモデル
	std::unique_ptr<EnemyModel>		m_pEnemyModel;
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
	// 敵の回転角度
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
	//敵の体力
	int m_currentHP;
	//敵のHPが0になったらTrue
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
#endif // ENEMY_DEFINED