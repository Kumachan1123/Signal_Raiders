/*
*	@file	EnemyFactory.h
*	@brief	敵ファクトリークラス
*/
#pragma once
#ifndef ENEMY_FACTORY_DEFINED
#define ENEMY_FACTORY_DEFINED
// 標準ライブラリ
#include <memory>
// 自作ヘッダーファイル
#include "Game/Enemy/Enemy.h"
#include "Game/Enemy/VerticalAttacker/VerticalAttacker.h"
#include "Game/Enemy/BossBase/BossBase.h"
#include "Game/Enemy/EnemyType/EnemyType.h"
#include "Game/Interface/IEnemy.h"

// 敵ファクトリークラス
class EnemyFactory
{
public:
	// public関数
	// 敵生成
	static std::unique_ptr<IEnemy> CreateEnemy(EnemyType type, Player* player, CommonResources* resources, int HP);
	// ボス生成
	static std::unique_ptr<IBossLogic> CreateBoss(BossType type, BossBase* pBoss, CommonResources* resources);
};
#endif //ENEMY_FACTORY_DEFINED