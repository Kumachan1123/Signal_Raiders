#pragma once
#ifndef IENEMYBULLET_DEFINED
#define IENEMYBULLET_DEFINED
#include "Game/Enemy/EnemyBullet/EnemyBullet.h"

class EnemyBullet;
class IEnemyBullet
{
public:// ä÷êî
	virtual ~IEnemyBullet() = default;
	virtual void Initialize() = 0;
	virtual void Update(float elapsedTime) = 0;
	virtual void SetEnemyBullet(EnemyBullet* pEnemyBullet) = 0;
};
#endif		// IENEMYBULLET_DEFINED
