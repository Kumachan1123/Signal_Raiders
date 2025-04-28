#include "pch.h"
#include "EnemyBulletFactory.h"

std::unique_ptr<IEnemyBullet> EnemyBulletFactory::CreateBullet(BulletType type)
{
	switch (type)
	{
	case BulletType::NORMAL:
		return std::make_unique<NormalBullet>();
	case BulletType::SPECIAL:
		return std::make_unique<SpecialBullet>();
	case BulletType::SPEED:
		return std::make_unique<SpeedBullet>();
	default:
		return nullptr;
	}
};