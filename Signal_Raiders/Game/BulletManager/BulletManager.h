/*
	@file	BulletManager.h
	@brief	弾管理クラス
*/
#pragma once
// 標準ライブラリ
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <cassert>
#include <SimpleMath.h>
// 外部ライブラリ
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"

// 自作ヘッダーファイル
#include "Game/Player/PlayerBullet/PlayerBullet.h"
#include "Game/Enemy/EnemyBullet/EnemyBullet.h"
#include "Game/BulletParameters/BulletParameters.h"
#include "Game/Screen.h"
#include "Game/KumachiLib/AudioManager/AudioManager.h"
#include "Game/KumachiLib/KumachiLib.h"
#include "Game/Player/Player.h"
#include "Game/Enemy/EnemyManager/EnemyManager.h"


// クラスの前方宣言
class CommonResources;
class Player;
class IEnemy;
class EnemyManager;
class BulletParameters;
class BulletManager
{

public:
	// コンストラクタ・デストラクタ
	BulletManager(CommonResources* commonResources);// コンストラクタ
	~BulletManager();// デストラクタ

	// publicメンバ関数
	void Initialize(Player* pPlayer, EnemyManager* pEnemies);// 初期化
	void Update(float elapsedTime);// 更新
	void Render();// 描画
	void CreatePlayerBullet(const DirectX::SimpleMath::Vector3& position, DirectX::SimpleMath::Vector3& direction);// プレイヤーの弾生成
	void CreateEnemyBullet(const DirectX::SimpleMath::Vector3& position, DirectX::SimpleMath::Vector3& direction);// 敵の弾生成
	void RemoveBulletsByShooter(IEnemy* shooter);// 弾の削除（敵が死んだときに呼ばれる）
	void ReLoadPlayerBullet();// 弾の補充
	void ConsumePlayerBullet();// 弾の消費

	// ゲッター・セッター
	void SetEnemyBulletType(EnemyBullet::BulletType type) { m_enemyBulletType = type; }// 敵の弾の種類設定
	void SetEnemyBulletSize(float size) { m_enemyBulletSize = size; }// 敵の弾の大きさ設定
	bool GetIsPlayerShoot() const { return m_isPlayerShoot; }// プレイヤーの弾生成フラグ取得
	void SetIsPlayerShoot(bool isPlayerShoot) { m_isPlayerShoot = isPlayerShoot; }// プレイヤーの弾生成フラグ設定
	void SetShooter(IEnemy* pShooter) { m_pShooter = pShooter; }// 弾を射出した敵のポインター設定
	void SetAdditionalDamage(int additionalDamage);// 追加ダメージ設定
	int GetPlayerBulletCount() const { return m_playerBulletCount; }// プレイヤーの弾の数取得
	void SetPlayerBulletCount(int count) { m_playerBulletCount = count; }// プレイヤーの弾の数設定
	int GetSpecialAttackCount() const { return m_specialAttackCount; }// 特殊攻撃の数取得
	void SetSpecialAttackCount(int count) { m_specialAttackCount = count; }// 特殊攻撃の数設定
	bool GetIsReloading() const { return m_isReloading; }// リロード中フラグ取得
	void SetIsReloading(bool isReloading) { m_isReloading = isReloading; }// リロード中フラグ設定
private:
	// コモンリソース
	CommonResources* m_commonResources;
	// オーディオマネージャー
	AudioManager* m_audioManager;
	// プレイヤー
	Player* m_pPlayer;
	// 敵全体
	EnemyManager* m_pEnemyManager;
	// 弾を射出した敵のポインター
	IEnemy* m_pShooter;
	// 敵の弾の種類
	EnemyBullet::BulletType m_enemyBulletType;
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
	std::vector<std::unique_ptr<PlayerBullet>> m_playerBullets;
	// 敵の弾
	std::vector<std::unique_ptr<EnemyBullet>> m_enemyBullets;
	// プレイヤーの弾の最大数
	int m_playerBulletCount;
	// ボスが出す特殊攻撃の数
	int m_specialAttackCount;

	// privateメンバ関数
	void UpdatePlayerBullets(float elapsedTime);// プレイヤーの弾更新
	void UpdateEnemyBullets(float elapsedTime, std::unique_ptr<IEnemy>& enemy);// 敵の弾更新
	bool CheckCollisionWithEnemies(const std::unique_ptr<PlayerBullet>& bullet);// 敵とプレイヤーの弾の当たり判定
	bool CheckCollisionWithPlayer(const std::unique_ptr<EnemyBullet>& bullet, const std::unique_ptr<IEnemy>& enemy);// プレイヤーと敵の弾の当たり判定
	void CheckCollisionWithBullets();	// 敵の弾とプレイヤーの弾の当たり判定
	void SetSound();// 効果音設定
};