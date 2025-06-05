/*
*	@file	BulletManager.h
*	@details 敵とプレイヤーの弾を管理するマネージャークラス
*	@brief	弾管理クラス
*/
#pragma once
#ifndef BULLET_MANAGER_DEFINED
#define BULLET_MANAGER_DEFINED
// 標準ライブラリ
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <cassert>
// DirectX
#include <SimpleMath.h>
#include <DeviceResources.h>
// 外部ライブラリ
#include "Game/CommonResources.h"
#include <Libraries/MyLib/MemoryLeakDetector.h>
// 自作ヘッダーファイル
#include "Game/Player/PlayerBullet/PlayerBullet.h"
#include "Game/Enemy/EnemyBullet/EnemyBullet.h"
#include "Game/BulletParameters/BulletParameters.h"
#include "Game/Screen.h"
#include "Game/KumachiLib/AudioManager/AudioManager.h"
#include "Game/KumachiLib/KumachiLib.h"
#include "Game/Player/Player.h"
#include "Game/Enemy/EnemyManager/EnemyManager.h"

// 前方宣言
class CommonResources;
class Player;
class IEnemy;
class EnemyManager;
class BulletParameters;

// 弾マネージャークラス
class BulletManager
{
public:
	// アクセサ
	// 敵の弾の種類設定
	void SetEnemyBulletType(BulletType type) { m_enemyBulletType = type; }
	// 敵の弾の大きさ設定
	void SetEnemyBulletSize(float size) { m_enemyBulletSize = size; }
	// プレイヤーの弾生成フラグ取得
	bool GetIsPlayerShoot() const { return m_isPlayerShoot; }
	// プレイヤーの弾生成フラグ設定
	void SetIsPlayerShoot(bool isPlayerShoot) { m_isPlayerShoot = isPlayerShoot; }
	// 弾を射出した敵のポインター設定
	void SetShooter(IEnemy* pShooter) { m_pShooter = pShooter; }
	// 追加ダメージ設定
	void SetAdditionalDamage(int additionalDamage);
	// プレイヤーの弾の数取得
	int GetPlayerBulletCount() const { return m_playerBulletCount; }
	// プレイヤーの弾の数設定
	void SetPlayerBulletCount(int count) { m_playerBulletCount = count; }
	// 特殊攻撃の数取得
	int GetSpecialAttackCount() const { return m_specialAttackCount; }
	// 特殊攻撃の数設定
	void SetSpecialAttackCount(int count) { m_specialAttackCount = count; }
	// リロード中フラグ取得
	bool GetIsReloading() const { return m_isReloading; }
	// リロード中フラグ設定
	void SetIsReloading(bool isReloading) { m_isReloading = isReloading; }
public:
	// publicメンバ関数
	// コンストラクタ
	BulletManager(CommonResources* commonResources);
	// デストラクタ
	~BulletManager();
	// 初期化
	void Initialize(Player* pPlayer, EnemyManager* pEnemies);
	// 更新
	void Update(float elapsedTime);
	// 描画
	void Render();
	// プレイヤーの弾生成
	void CreatePlayerBullet(const DirectX::SimpleMath::Vector3& position, const DirectX::SimpleMath::Vector3& direction);
	// 敵の弾生成
	void CreateEnemyBullet(const DirectX::SimpleMath::Vector3& position, const DirectX::SimpleMath::Vector3& direction);
	// 弾の削除（敵が死んだときに呼ばれる）
	void RemoveBulletsByShooter(IEnemy* shooter);
	// 弾の補充
	void ReLoadPlayerBullet();
	// 弾の消費
	void ConsumePlayerBullet();
private:
	// privateメンバ関数
	// プレイヤーの弾更新
	void UpdatePlayerBullets(float elapsedTime);
	// 敵の弾更新
	void UpdateEnemyBullets(float elapsedTime, std::unique_ptr<IEnemy>& enemy);
	// 敵とプレイヤーの弾の当たり判定
	bool CheckCollisionWithEnemies(const std::unique_ptr<PlayerBullet>& bullet);
	// プレイヤーと敵の弾の当たり判定
	bool CheckCollisionWithPlayer(const std::unique_ptr<EnemyBullet>& bullet, const std::unique_ptr<IEnemy>& enemy);
	// 敵の弾とプレイヤーの弾の当たり判定
	void CheckCollisionWithBullets();
private:
	// privateメンバ変数
	// コモンリソース
	CommonResources* m_pCommonResources;
	// プレイヤー
	Player* m_pPlayer;
	// 敵全体
	EnemyManager* m_pEnemyManager;
	// 弾を射出した敵のポインター
	IEnemy* m_pShooter;
	// 敵の弾の種類
	BulletType m_enemyBulletType;
	// 敵の弾の大きさ
	float m_enemyBulletSize;
	// プレイヤーの弾生成フラグ
	bool m_isPlayerShoot;
	// 経過時間
	float m_elapsedTime;
	// リロードタイマー
	float m_reloadTimer;
	// リロード中フラグ
	bool m_isReloading;
	// プレイヤーの弾
	std::vector<std::unique_ptr<PlayerBullet>> m_pPlayerBullets;
	// 敵の弾
	std::vector<std::unique_ptr<EnemyBullet>> m_pEnemyBullets;
	// プレイヤーの弾の最大数
	int m_playerBulletCount;
	// ボスが出す特殊攻撃の数
	int m_specialAttackCount;
};
#endif // BULLET_MANAGER_DEFINED