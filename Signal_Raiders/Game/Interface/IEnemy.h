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
class IEnemy
{
protected:
	// protected変数
	Player* m_pPlayer;// プレイヤー
	BulletManager* m_pBulletManager;// 弾管理クラス
	CommonResources* m_commonResources;// 共通リソース
	int m_currentHP;// 敵のHP
public:
	// アクセサ										  
	virtual const DirectX::SimpleMath::Matrix& GetMatrix() = 0;// 行列取得
	virtual const  DirectX::SimpleMath::Vector3& GetPosition() = 0;// 位置取得
	virtual void SetPosition(DirectX::SimpleMath::Vector3& pos) = 0;// 位置設定
	virtual const DirectX::SimpleMath::Vector3& GetVelocity() = 0;// 速度取得
	virtual const DirectX::SimpleMath::Vector3& GetRotate() = 0;// 回転取得
	virtual int GetEnemyHP() const = 0;// HP取得
	virtual void SetEnemyMaxHP(int hp) = 0;// HP設定
	virtual bool GetEnemyIsDead() const = 0;// 死亡フラグ取得
	virtual void SetEnemyIsDead(bool isDead) = 0;// 死亡フラグ設定
	virtual bool GetHitToPlayer()const = 0;// プレイヤーに当たったか取得
	virtual void SetHitToPlayer(bool isHitToPlayer) = 0;// プレイヤーに当たったか設定
	virtual bool GetHitToOtherEnemy() const = 0;// 他の敵に当たったか取得
	virtual void SetHitToOtherEnemy(bool isHitToOtherEnemy) = 0;// 他の敵に当たったか設定
	virtual void SetPlayerHP(float& HP) const = 0;// プレイヤーのHP設定
	virtual bool GetPlayerHitByEnemyBullet() const = 0;// プレイヤーが敵の弾に当たったか取得
	virtual void SetPlayerHitByEnemyBullet(bool hit) = 0;// プレイヤーが敵の弾に当たったか設定
	virtual bool GetEnemyHitByPlayerBullet()const = 0;// 敵がプレイヤーの弾に当たったか取得
	virtual void SetEnemyHitByPlayerBullet(bool hit) = 0;// 敵がプレイヤーの弾に当たったか設定
	virtual float GetToPlayerDamage() const = 0;// プレイヤーに与えるダメージ取得
	virtual bool GetCanAttack() const = 0;// 攻撃可能か取得
	virtual void SetCanAttack(bool canAttack) = 0;// 攻撃可能か設定
	virtual bool GetIsAttack() const = 0;// 攻撃中か取得
	virtual void SetIsAttack(bool isAttack) = 0;// 攻撃中か設定
	virtual const DirectX::BoundingSphere& GetBoundingSphere() = 0;// 境界球取得
	virtual void SetBulletBoundingSphere(DirectX::BoundingSphere& bs) = 0;// 境界球設定
	virtual const DirectX::BoundingSphere& GetBulletBoundingSphere() = 0;// 弾の境界球取得
	virtual void SetPlayerBoundingSphere(DirectX::BoundingSphere playerBS) = 0;// プレイヤーの境界球設定
	virtual const DirectX::BoundingSphere& GetPlayerBoundingSphere() = 0;// プレイヤーの境界球取得
	virtual Player* GetPlayer()const = 0;// プレイヤー取得
	virtual FPS_Camera* GetCamera()const = 0;// カメラ取得
	virtual void SetCamera(FPS_Camera* camera) = 0;// カメラ設定
	virtual BulletManager* GetBulletManager()const = 0;// 弾管理クラス取得
	virtual void SetBulletManager(BulletManager* bulletManager) = 0;// 弾管理クラス設定

public:
	// public関数
	// コンストラクタ
	IEnemy(Player* pPlayer, CommonResources* resources, int hp)
		: m_pPlayer(pPlayer)// プレイヤー
		, m_commonResources(resources)// 共通リソース
		, m_currentHP(hp)// 敵のHP
		, m_pBulletManager(nullptr) // 弾管理クラス
	{}
	virtual ~IEnemy() = default;// デストラクタ
	virtual void Initialize() = 0;// 初期化
	virtual void Update(float elapsedTime) = 0;// 更新
	virtual	void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj) = 0;// 描画
	virtual void DrawCollision(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj) = 0;// 衝突描画


};
#endif		// IENEMY_DEFINED
