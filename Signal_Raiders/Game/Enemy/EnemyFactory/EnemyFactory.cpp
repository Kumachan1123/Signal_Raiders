#include "pch.h"
#include "EnemyFactory.h"

std::unique_ptr<IEnemy> EnemyFactory::CreateEnemy(EnemyType type, Player* player, CommonResources* resources, int HP)
{
	switch (type)
	{
	case EnemyType::NORMAL:
		return std::make_unique<Enemy>(player, resources, HP);
	case EnemyType::VERTICAL_ATTACKER:
		return std::make_unique<VerticalAttacker>(player, resources, HP);
	case EnemyType::BOSS:
		return std::make_unique<BossBase>(player, resources, HP);
	default:
		return nullptr;
	}
}


std::unique_ptr<IBossLogic> EnemyFactory::CreateBoss(BossType type, BossBase* pBoss, CommonResources* resources)
{
	switch (type)
	{
	case BossType::NORMAL_BOSS:
		return std::make_unique<Boss>(pBoss, resources);
	case BossType::LAST_BOSS:
		return std::make_unique<LastBoss>(pBoss, resources);
	default:
		return nullptr;
	}
}
