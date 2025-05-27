/*
*	@file	EnemyFactory.h
*	@brief	敵ファクトリークラス
*/
#pragma once
// 標準ライブラリ
#include <memory>
// 自作ヘッダーファイル
#include "Game/Enemy/Enemy.h"
#include "Game/Enemy/VerticalAttacker/VerticalAttacker.h"
#include "Game/Enemy/BossBase/BossBase.h"
#include "Game/Enemy/EnemyType/EnemyType.h"
#include "Game/Interface/IEnemy.h"

class EnemyFactory
{
public:
	static std::unique_ptr<IEnemy> CreateEnemy(EnemyType type, Player* player, CommonResources* resources, int HP);// 敵生成
	static std::unique_ptr<IBossLogic> CreateBoss(BossType type, BossBase* pBoss, CommonResources* resources);// ボス生成

};


