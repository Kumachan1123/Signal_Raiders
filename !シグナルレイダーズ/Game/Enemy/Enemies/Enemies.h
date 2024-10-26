/*
	@file	Enemies.h
	@brief	敵たちクラス
	作成者：くまち
*/
#pragma once
#include "Game/Interface/IEnemy.h"
#include "Game/Enemy/Enemy.h"
#include "Game/Enemy/Boss/Boss.h"
#include "Game/Effect/Effect.h"
#include "Game/Player/Player.h"
#include "Game/Wifi/Wifi.h"
// 前方宣言
class CommonResources;
class IEnemy;
class Boss;
class Player;
class Effect;
class Wifi;
class Enemies
{
public:

private:
	// コモンリソース
	CommonResources* m_commonResources;
	// 敵
	std::vector<std::unique_ptr<IEnemy>> m_enemies;
	// ボス
	std::unique_ptr<Boss> m_boss;
	// ステージ番号（0から4をプレイシーンから参照）
	int m_stageNumber = 0;
	// 敵生成フラグ
	bool m_isEnemyBorn = false;
	// 敵生成済み
	bool m_isBorned = false;
	// プレイヤーと敵の当たり判定
	bool m_isHitPlayerToEnemy = false;
	// 敵の数
	int m_enemyIndex = 0;
	// 敵の生成上限
	int m_enemyMax = 0;
	// 敵の生成間隔
	float m_enemyBornInterval = 0.0f;
	// 敵の生成タイミングを管理
	float m_enemyBornTimer = 0.0f;
	// 敵の生成開始時間
	float m_startTime = 0.0f;
	// ボス生成フラグ
	bool m_isBossBorn = false;
	// ボス生成済み
	bool m_isBossBorned = false;
	// ボス生存フラグ
	bool m_isBossAlive = true;
	// ボスの体力
	int m_bossHP;
	// wi-fi
	std::unique_ptr<Wifi> m_pWifi;
	// プレイヤーコントローラー
	Player* m_pPlayer;
	// エフェクト
	std::vector<std::unique_ptr<Effect>> m_effect;
	// オーディオマネージャー
	AudioManager* m_audioManager;
	// SEの音量
	float m_SEVolume = 0.0f;
	Boss::BossBulletType m_bossBulletType;
public:
	Enemies(CommonResources* commonResources);
	~Enemies();
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
private:
	// 敵の生成上限設定
	void SetEnemyMax();
};
