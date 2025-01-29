#include "pch.h"
#include "EnemyFactory.h"

std::unique_ptr<IEnemy> EnemyFactory::CreateEnemy(EnemyType type, Player* player, CommonResources* resources, int HP)
{
	switch (type)
	{
	case EnemyType::Normal:
		return std::make_unique<Enemy>(player, resources, HP);
	case EnemyType::VerticalAttacker:
		return std::make_unique<VerticalAttacker>(player, resources, HP);
	case EnemyType::Boss:
		return std::make_unique<Boss>(player, resources, HP);
	default:
		return nullptr;
	}
}