/*
	@file	Enemies.h
	@brief	敵たちクラス
	作成者：くまち
*/
#pragma once
#include "Game/Enemy/Enemy.h"
#include "Game/Effect/Effect.h"
#include "Game/Player/Player.h"
#include "Game/Wifi/Wifi.h"
// 前方宣言
class CommonResources;
class Enemy;
class Player;
class Effect;
class Wifi;

class Enemies
{
private:
	// コモンリソース
	CommonResources* m_commonResources;

	std::vector<std::unique_ptr<Enemy>> m_enemy;
	// 敵生成フラグ
	bool m_isEnemyBorn = false;
	// 敵生成済み
	bool m_isBorned = false;
	// プレイヤーと敵の当たり判定
	bool m_isHitPlayerToEnemy = false;
	// 敵の数
	int m_enemyIndex = 0;
	// 敵の生成間隔
	float m_enemyBornInterval = 0.0f;
	// 敵の生成タイミングを管理
	float m_enemyBornTimer = 0.0f;
	// 敵の生成開始時間
	float m_startTime = 0.0f;
	// wi-fi
	std::unique_ptr<Wifi> m_pWifi;
	// プレイヤーコントローラー
	Player* m_pPlayer;
	// エフェクト
	std::vector<std::unique_ptr<Effect>> m_effect;
	// オーディオマネージャー
	AudioManager* m_audioManager;

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
	// Getter
public:
	std::vector<std::unique_ptr<Enemy>>& GetEnemy() { return m_enemy; }
	std::unique_ptr<Wifi>& GetWifi() { return m_pWifi; }
	std::vector<std::unique_ptr<Effect>>& GetEffect() { return m_effect; }
	// Setter
};
