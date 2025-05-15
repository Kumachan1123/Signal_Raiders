/*
	@file	Enemy.h
	@brief	敵クラス
*/
#pragma once
// 標準ライブラリ
#include <cassert>
#include <SimpleMath.h>
#include <random>
#include <memory>
// 外部ライブラリ
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include <Libraries/Microsoft/DebugDraw.h>
#include "Libraries/Microsoft/ReadData.h"
// 自作ヘッダーファイル
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
class Enemy : public IEnemy
{
public:
	// アクセサ
	DirectX::BoundingSphere& GetBoundingSphere() override { return m_enemyBS; }// 敵の境界球取得
	DirectX::BoundingSphere& GetBulletBoundingSphere() override { return m_enemyBulletBS; }// 敵の弾の境界球取得
	void SetBulletBoundingSphere(DirectX::BoundingSphere& bs)override { m_enemyBulletBS = bs; }// 敵の弾の境界球設定
	DirectX::BoundingSphere& GetPlayerBoundingSphere() override { return m_playerBS; }// プレイヤーの境界球取得
	void SetPlayerBoundingSphere(DirectX::BoundingSphere playerBS)override { m_playerBS = playerBS; }// プレイヤーの境界球設定
	const DirectX::SimpleMath::Matrix& GetMatrix() override { return m_matrix; }// マトリクス取得
	const DirectX::SimpleMath::Vector3& GetPosition() override { return m_position; }// 座標取得
	void SetPosition(DirectX::SimpleMath::Vector3& pos)override { m_position = pos; }// 座標設定
	const DirectX::SimpleMath::Vector3& GetVelocity()override { return m_velocity; }// 速度取得
	const DirectX::SimpleMath::Vector3& GetRotate() override { return m_rotate; }// 回転取得
	Player* GetPlayer()const override { return m_pPlayer; }// プレイヤー取得
	FPS_Camera* GetCamera()const override { return m_pCamera; }// カメラ取得
	void SetCamera(FPS_Camera* camera) { m_pCamera = camera; }// カメラ設定
	int GetEnemyHP() const override { return m_currentHP; }// 敵のHP取得
	bool GetEnemyIsDead() const override { return m_isDead; }// 死亡フラグ取得
	void SetEnemyIsDead(bool isDead)override { m_isDead = isDead; }// 死亡フラグ設定
	bool GetHitToPlayer()const override { return m_isHitToPlayer; }// プレイヤーとの当たり判定取得
	void SetHitToPlayer(bool isHitToPlayer)override { m_isHitToPlayer = isHitToPlayer; }// プレイヤーとの当たり判定設定
	bool GetHitToOtherEnemy() const override { return m_isHitToOtherEnemy; }// その他の敵との当たり判定取得
	void SetHitToOtherEnemy(bool isHitToOtherEnemy) override { m_isHitToOtherEnemy = isHitToOtherEnemy; }// その他の敵との当たり判定設定
	bool GetPlayerHitByEnemyBullet() const override { return m_isPlayerHitByEnemyBullet; }// 敵の弾がプレイヤーに当たったか取得
	void SetPlayerHitByEnemyBullet(bool hit)override { m_isPlayerHitByEnemyBullet = hit; }// 敵の弾がプレイヤーに当たったか設定
	bool GetEnemyHitByPlayerBullet()const override { return m_isEnemyHitByPlayerBullet; }// 敵がプレイヤーの弾に当たったか取得
	void SetEnemyHitByPlayerBullet(bool hit) override { m_isEnemyHitByPlayerBullet = hit; }// 敵がプレイヤーの弾に当たったか設定
	float GetToPlayerDamage() const override { return EnemyParameters::NORMAL_ENEMY_DAMAGE; }// プレイヤーに与えるダメージ取得
	bool GetCanAttack() const override { return m_canAttack; }// 攻撃可能か取得
	void SetCanAttack(bool canAttack)override { m_canAttack = canAttack; }// 攻撃可能か設定
	void SetEnemyMaxHP(int hp)override { m_currentHP -= hp; }// 敵のHP設定
	void SetPlayerHP(float& HP) const override { HP -= EnemyParameters::NORMAL_ENEMY_DAMAGE; }// プレイヤーのHP設定
	BulletManager* GetBulletManager()const override { return m_pBulletManager; }// 弾管理クラス取得
	void SetBulletManager(BulletManager* bulletManager) override { m_pBulletManager = bulletManager; }// 弾管理クラス設定
	bool GetIsAttack() const override { return m_isAttack; }// 攻撃中か取得
	void SetIsAttack(bool isAttack) override { m_isAttack = isAttack; }// 攻撃中か設定
public:
	//	public関数
	Enemy(Player* pPlayer, CommonResources* resources, int hp);// コンストラクタ
	~Enemy();// デストラクタ
	void Initialize() override;// 初期化
	void Update(float elapsedTime) override;// 更新
	void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj) override;// 描画
	void DrawCollision(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj) override;// 当たり判定描画
private:
	// private関数
	void ShootBullet();// 弾を撃つ
private:
	// メンバ変数
	CommonResources* m_commonResources;	// 共通リソース
	std::unique_ptr<EnemyModel>		m_enemyModel;// 敵のモデル
	std::unique_ptr<EnemyAI>		m_enemyAI;// 敵のAI
	std::unique_ptr<EnemyHPBar>		m_pHPBar;// 敵のHPバー
	Player* m_pPlayer;	// プレイヤーのポインター
	FPS_Camera* m_pCamera;	// カメラのポインター
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
	bool m_isAttack;// 攻撃中か
	float m_attackCooldown;  // 攻撃のクールダウンタイム


};