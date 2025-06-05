/*
*	@file IEnemy.h
*	@brief 敵のインターフェースクラス
*/
#pragma once
#ifndef IENEMY_DEFINED
#define IENEMY_DEFINED
// 標準ライブラリ
#include <SimpleMath.h>
// 自作ヘッダーファイル
#include "Game/KumachiLib/AudioManager/AudioManager.h"

// 前方宣言
class Player;
class BulletManager;
class FPS_Camera;
class CommonResources;

// 敵のインターフェースクラス
class IEnemy
{
public:
	// アクセサ										  
	// 行列を取得する
	virtual const DirectX::SimpleMath::Matrix& GetMatrix() = 0;
	// 現在の位置を取得する
	virtual const DirectX::SimpleMath::Vector3& GetPosition() = 0;
	// 位置を設定する
	virtual void SetPosition(const DirectX::SimpleMath::Vector3& pos) = 0;
	// 移動速度を取得する
	virtual const DirectX::SimpleMath::Vector3& GetVelocity() = 0;
	// 回転角度を取得する
	virtual const DirectX::SimpleMath::Vector3& GetRotate() = 0;
	// 敵の現在HPを取得する
	virtual int GetEnemyHP() const = 0;
	// 敵にダメージを適用してHPを減らす
	virtual void ApplyDamageToEnemy(int hp) = 0;
	// 敵が死亡しているかどうかを取得する
	virtual bool GetEnemyIsDead() const = 0;
	// 敵の死亡状態を設定する
	virtual void SetEnemyIsDead(bool isDead) = 0;
	// プレイヤーに当たったかどうかを取得する
	virtual bool GetHitToPlayer() const = 0;
	// プレイヤーに当たったかどうかを設定する
	virtual void SetHitToPlayer(bool isHitToPlayer) = 0;
	// 他の敵に当たったかどうかを取得する
	virtual bool GetHitToOtherEnemy() const = 0;
	// 他の敵に当たったかどうかを設定する
	virtual void SetHitToOtherEnemy(bool isHitToOtherEnemy) = 0;
	// プレイヤーのHPを設定する（ダメージ反映）
	virtual void SetPlayerHP(float& HP) const = 0;
	// プレイヤーが敵の弾に当たったかどうかを取得する
	virtual bool GetPlayerHitByEnemyBullet() const = 0;
	// プレイヤーが敵の弾に当たったかどうかを設定する
	virtual void SetPlayerHitByEnemyBullet(bool hit) = 0;
	// 敵がプレイヤーの弾に当たったかどうかを取得する
	virtual bool GetEnemyHitByPlayerBullet() const = 0;
	// 敵がプレイヤーの弾に当たったかどうかを設定する
	virtual void SetEnemyHitByPlayerBullet(bool hit) = 0;
	// プレイヤーに与えるダメージ量を取得する
	virtual float GetToPlayerDamage() const = 0;
	// 攻撃可能な状態かどうかを取得する
	virtual bool GetCanAttack() const = 0;
	// 攻撃可能状態を設定する
	virtual void SetCanAttack(bool canAttack) = 0;
	// 攻撃中かどうかを取得する
	virtual bool GetIsAttack() const = 0;
	// 攻撃中かどうかを設定する
	virtual void SetIsAttack(bool isAttack) = 0;
	// 敵の当たり判定（境界球）を取得する
	virtual const DirectX::BoundingSphere& GetBoundingSphere() = 0;
	// 敵の弾の当たり判定（境界球）を設定する
	virtual void SetBulletBoundingSphere(const DirectX::BoundingSphere& bs) = 0;
	// 敵の弾の当たり判定（境界球）を取得する
	virtual const DirectX::BoundingSphere& GetBulletBoundingSphere() = 0;
	// プレイヤーの当たり判定（境界球）を設定する
	virtual void SetPlayerBoundingSphere(const DirectX::BoundingSphere& playerBS) = 0;
	// プレイヤーの当たり判定（境界球）を取得する
	virtual const DirectX::BoundingSphere& GetPlayerBoundingSphere() = 0;
	// プレイヤー取得
	virtual Player* GetPlayer()const = 0;
	// カメラ取得
	virtual FPS_Camera* GetCamera()const = 0;
	// カメラ設定
	virtual void SetCamera(FPS_Camera* camera) = 0;
	// 弾管理クラス取得
	virtual BulletManager* GetBulletManager()const = 0;
	// 弾管理クラス設定
	virtual void SetBulletManager(BulletManager* bulletManager) = 0;
public:
	// public関数
	// デストラクタ
	virtual ~IEnemy() = default;
	// 初期化
	virtual void Initialize() = 0;
	// 更新
	virtual void Update(float elapsedTime) = 0;
	// 描画
	virtual	void Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) = 0;
	// 衝突描画
	virtual void DrawCollision(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) = 0;
};
#endif		// IENEMY_DEFINED
