/*
*	@file	EnemyManager.h
*	@brief	敵マネージャークラス
*/
#pragma once
#ifndef ENEMY_MANAGER_DEFINED
#define ENEMY_MANAGER_DEFINED
// 標準ライブラリ
#include <cassert>
#include <memory>
#include <vector>
#include <thread>
// DirectX
#include <SimpleMath.h>
#include <DeviceResources.h>
// 外部ライブラリ
#include <Libraries/MyLib/MemoryLeakDetector.h>
// 自作ヘッダーファイル
#include "Game/CommonResources.h"
#include "Game/BulletManager/BulletManager.h"
#include "Game/Effect/Effect.h"
#include "Game/Enemy/EnemyFactory/EnemyFactory.h"
#include "Game/Enemy/EnemyType/EnemyType.h"
#include "Game/Enemy/Parameters/EnemyParameters.h"
#include "Game/Interface/IEnemy.h"
#include "Game/KumachiLib/KumachiLib.h"
#include "Game/Player/Player.h"
#include "Game/Stage/Wall/Wall.h"
#include "Game/Wifi/Wifi.h"

// 前方宣言
class CommonResources;
class IEnemy;
class BossBase;
class Player;
class Effect;
class Wifi;
class Wall;

// 敵マネージャークラス
class EnemyManager
{
private:
	// 構造体
	// ステージごとの設定
	struct StageSettings
	{
		int enemyMax;						// 敵の生成上限
		int bossHP;							// ボスの体力
		int specialAttackCount;				// 一度に出る特殊攻撃の数
		BossBase::BossBulletType bulletType;// ボスの弾の種類
	};
	// ステージごとの設定のマップ
	const std::unordered_map<int, StageSettings> stageData =
	{
		//{ステージ番号, {敵の生成上限, ボスの体力, 一度に出る特殊攻撃の数, ボスの弾の種類}}
		{0, {5, 100, 3,BossBase::BossBulletType::STAGE_1}},// ステージ1
		{1, {10, 200,4, BossBase::BossBulletType::STAGE_2}},// ステージ2
		{2, {20, 300, 5,BossBase::BossBulletType::STAGE_3}},// ステージ3
		{3, {30, 500, 7,BossBase::BossBulletType::STAGE_4}},// ステージ4
		{4, {40, 1000,10,BossBase::BossBulletType::STAGE_5}}// ステージ5

	};
public:
	// アクセサ
	// 敵リスト取得
	std::vector<std::unique_ptr<IEnemy>>& GetEnemies() { return m_pEnemies; }
	// 攻撃中の敵リスト取得
	std::vector<std::unique_ptr<IEnemy>>& GetAttackingEnemies() { return m_pAttackingEnemies; }
	// 生成された敵の最大数取得
	int GetEnemyIndex() const { return m_enemyIndex; }
	// 敵の数取得
	int GetEnemySize() const { return static_cast<int>(m_pEnemies.size()); }
	// Wi-Fiポインター取得
	std::unique_ptr<Wifi>& GetWifi() { return m_pWifi; }
	// エフェクト取得
	std::vector<std::unique_ptr<Effect>>& GetEffects() { return m_pEffects; }
	// 敵生成済みフラグ
	bool GetisBorned() const { return m_isBorned; }
	// ボス生存フラグ
	bool GetIsBossAlive() const { return m_isBossAlive; }
	// ボス生成演出再生フラグ
	bool GetIsBossAppear() const { return m_isBossAppear; }
	// SEの音量取得
	float GetSEVolume() const { return m_SEVolume; }
	// SEの音量設定
	void SetSEVolume(float volume) { m_SEVolume = volume; }
	// ステージ番号設定
	void SetStageNumber(int stageNumber) { m_stageNumber = stageNumber; }
	// 壁のポインター設定
	void SetWall(Wall* pWall) { m_pWall = pWall; };
	// 弾マネージャー取得
	BulletManager* GetBulletManager() { return m_pBulletManager; }
	// 弾マネージャー設定
	void SetBulletManager(BulletManager* pBulletManager) { m_pBulletManager = pBulletManager; }
	// 特殊攻撃の数取得
	int GetSpecialAttackCount() const { return m_specialAttackCount; }
	// 特殊攻撃の数設定
	void SetSpecialAttackCount(int count) { m_specialAttackCount = count; }
public:
	// public関数
	// コンストラクタ
	EnemyManager(CommonResources* commonResources);
	// デストラクタ
	~EnemyManager();
	// 初期化
	void Initialize(Player* pPlayer);
	// 更新
	void Update(float elapsedTime);
	// 描画
	void Render();
private:
	// private関数
	// 敵の生成上限設定
	void SetEnemyMax();
	// 更新処理を分割するメソッド
	// 敵生成開始時間の更新
	void UpdateStartTime(float elapsedTime);
	// エフェクトの更新
	void UpdateEffects(float elapsedTime);
	// 敵の生成
	void HandleEnemySpawning(float elapsedTime);
	// タイプを指定して敵を生成
	void SpawnEnemy(EnemyType type);
	// 敵生成終了処理
	void FinalizeEnemySpawn();
	// ボス生成
	void SpawnBoss();
	// 敵の当たり判定
	void HandleEnemyCollisions();
	// プレイヤーと敵の当たり判定
	void UpdateEnemies(float elapsedTime);
	// 敵の弾とプレイヤーの当たり判定
	void HandleEnemyBulletCollision(std::unique_ptr<IEnemy>& pEnemy);
	// 敵とプレイヤーの当たり判定
	void HandleEnemyPlayerCollision(std::unique_ptr<IEnemy>& pEnemy);
	// 壁との当たり判定
	void HandleWallCollision();
	// 死亡した敵を削除
	void RemoveDeadEnemies();
	// 敵の死亡処理
	void HandleEnemyDeath(std::unique_ptr<IEnemy>& pEnemy);
private:
	// private変数
	// コモンリソース
	CommonResources* m_pCommonResources;
	// 敵
	std::vector<std::unique_ptr<IEnemy>> m_pEnemies;
	// 攻撃中の敵配列
	std::vector<std::unique_ptr<IEnemy>> m_pAttackingEnemies;
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
	// ボスの生成待機時間
	float m_bossBornWaitTime;
	// ボス生成演出再生フラグ
	bool m_isBossAppear;
	// ボス生成フラグ
	bool m_isBossBorn;
	// ボス生成済み
	bool m_isBossBorned;
	// ボス生存フラグ
	bool m_isBossAlive;
	// ボスの体力
	int m_bossHP;
	// ボスが一度に出す特殊攻撃の数
	int m_specialAttackCount;
	// wi-fi
	std::unique_ptr<Wifi> m_pWifi;
	// Wi-Fiを取得するためのスレッド
	std::thread m_wifiThread;
	// プレイヤー(コントローラー)
	Player* m_pPlayer;
	// 弾マネージャー（プレイシーンから受け取る）
	BulletManager* m_pBulletManager;
	// 壁
	Wall* m_pWall;
	// エフェクト
	std::vector<std::unique_ptr<Effect>> m_pEffects;
	// SEの音量
	float m_SEVolume;
	// ボスの弾の種類
	BossBase::BossBulletType m_bossBulletType;
};
#endif // ENEMY_MANAGER_DEFINED
