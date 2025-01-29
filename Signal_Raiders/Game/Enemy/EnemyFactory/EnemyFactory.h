/*
	@file	EnemyFactory.h
	@brief	敵ファクトリークラス
*/
#pragma once
#include <memory>
#include "Game/Enemy/Enemy.h"
#include "Game/Enemy/VerticalAttacker/VerticalAttacker.h"
#include "Game/Enemy/Boss/Boss.h"
#include "Game/Enemy/EnemyType/EnemyType.h"
#include "Game/Interface/IEnemy.h"
class EnemyFactory
{
public:
	static std::unique_ptr<IEnemy> CreateEnemy(EnemyType type, Player* player, CommonResources* resources, int HP);


};


