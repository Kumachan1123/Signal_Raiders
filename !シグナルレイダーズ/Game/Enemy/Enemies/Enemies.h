/*
	@file	Enemies.h
	@brief	敵たちクラス
	作成者：くまち
*/
#pragma once
#include "Game/Enemy/Enemy.h"
#include "Game/Effect/Effect.h"
#include "Game/Player/PlayerController/PlayerController.h"
#include "Game/Wifi/Wifi.h"
// 前方宣言
class CommonResources;
class Enemy;
class PlayerController;
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
	// Wifi
	Wifi* m_wifi;
	// プレイヤーコントローラー
	PlayerController* m_playerController;
	// エフェクト
	Effect* m_pEffect;


public:
	Enemies(CommonResources* commonResources);
	~Enemies();
	// 更新
	void Update(Effect* pEffect, Wifi* pWifi, PlayerController* pPlayerController, float elapsedTime);
	// 描画
	void Draw();

};
