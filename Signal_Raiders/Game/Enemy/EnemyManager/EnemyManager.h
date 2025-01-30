/*
	@file	EnemyManager.h
	@brief	敵たちクラス
*/
#pragma once
#include "Game/Interface/IEnemy.h"
#include "Game/Enemy/Enemy.h"
#include "Game/Enemy/VerticalAttacker/VerticalAttacker.h"
#include "Game/Enemy/Boss/Boss.h"
#include "Game/Enemy/EnemyFactory/EnemyFactory.h"
#include "Game/Enemy/EnemyType/EnemyType.h"
#include "Game/Effect/Effect.h"
#include "Game/Player/Player.h"
#include "Game/Wifi/Wifi.h"
#include "Game/Stage/Wall/Wall.h"
// 前方宣言
class CommonResources;
class IEnemy;
class Boss;
class Player;
class Effect;
class Wifi;
class Wall;
class EnemyManager
{
private:
	// ステージごとの設定
	struct StageSettings
	{
		int enemyMax;
		int bossHP;
		Boss::BossBulletType bulletType;
	};
	const std::unordered_map<int, StageSettings> stageData = {
	{0, {5, 100, Boss::BossBulletType::NORMAL}},
	{1, {10, 200, Boss::BossBulletType::NORMAL}},
	{2, {20, 300, Boss::BossBulletType::TWIN}},
	{3, {30, 500, Boss::BossBulletType::THREE}},
	{4, {40, 1000, Boss::BossBulletType::SPIRAL}}
	};

private:
	// コモンリソース
	CommonResources* m_commonResources;
	// 敵
	std::vector<std::unique_ptr<IEnemy>> m_enemies;
	// ボス
	std::unique_ptr<Boss> m_boss;
	// ステージ番号（0から4をプレイシーンから参照）
	int m_stageNumber;
	// 敵生成フラグ
	bool m_isEnemyBorn;
	// 敵生成済み
	bool m_isBorned;
	// プレイヤーと敵の当たり判定
	bool m_isHitPlayerToEnemy;
	// 敵の数
	int m_enemyIndex;
	// 敵の生成上限
	int m_enemyMax;
	// 敵の生成間隔
	float m_enemyBornInterval;
	// 敵の生成タイミングを管理
	float m_enemyBornTimer;
	// 敵の生成開始時間
	float m_startTime;
	// ボス生成フラグ
	bool m_isBossBorn;
	// ボス生成済み
	bool m_isBossBorned;
	// ボス生存フラグ
	bool m_isBossAlive;
	// ボスの体力
	int m_bossHP;
	// wi-fi
	std::unique_ptr<Wifi> m_pWifi;
	// プレイヤー(コントローラー)
	Player* m_pPlayer;
	// 壁
	Wall* m_pWall;
	// エフェクト
	std::vector<std::unique_ptr<Effect>> m_effect;
	// オーディオマネージャー
	AudioManager* m_audioManager;
	// SEの音量
	float m_SEVolume = 0.0f;
	// ボスの弾の種類
	Boss::BossBulletType m_bossBulletType;

public:
	EnemyManager(CommonResources* commonResources);
	~EnemyManager();
	// 初期化
	void Initialize(Player* pPlayer);
	// 更新
	void Update(float elapsedTime);
	// 描画
	void Render();
	// 効果音の初期化
	void InitializeFMOD();

public:
	// Getter
	std::vector<std::unique_ptr<IEnemy>>& GetEnemies() { return m_enemies; }  // 敵リスト
	int GetEnemyIndex() const { return m_enemyIndex; }// 生成された敵の最大数
	int GetEnemySize() const { return static_cast<int>(m_enemies.size()); }
	std::unique_ptr<Wifi>& GetWifi() { return m_pWifi; }// Wi-Fi
	std::vector<std::unique_ptr<Effect>>& GetEffect() { return m_effect; }// エフェクト
	bool GetisBorned() const { return m_isBorned; }// 敵生成済み
	bool GetIsBossAlive() const { return m_isBossAlive; }// ボス生存フラグ
	// Setter
	void SetVolume(float volume) { m_SEVolume = volume; }// 音量取得
	void SetStageNumber(int stageNumber) { m_stageNumber = stageNumber; }// ステージ番号
	void SetWall(Wall* pWall) { m_pWall = pWall; };// 壁のポインター
private:
	// 敵の生成上限設定
	void SetEnemyMax();

	// 更新処理を分割するメソッド
	void UpdateStartTime(float elapsedTime);// 敵生成開始時間
	void UpdateEffects(float elapsedTime);// エフェクト
	void HandleEnemySpawning(float elapsedTime);// 敵生成
	void SpawnEnemy(EnemyType type);// 敵生成(指定タイプ
	void FinalizeEnemySpawn();// 敵生成終了
	void SpawnBoss();// ボス生成
	void HandleEnemyCollisions();// 敵の当たり判定
	void HandlePlayerCollisions(float elapsedTime);// プレイヤーと敵の当たり判定
	void HandleEnemyBulletCollision(std::unique_ptr<IEnemy>& enemy);// 敵の弾とプレイヤーの当たり判定
	void HandleEnemyPlayerCollision(std::unique_ptr<IEnemy>& enemy);// 敵とプレイヤーの当たり判定
	void HandleWallCollision();// 壁との当たり判定
	void RemoveDeadEnemies();// 死亡した敵を削除
	void HandleEnemyDeath(std::unique_ptr<IEnemy>& enemy);// 敵の死亡処理
};
