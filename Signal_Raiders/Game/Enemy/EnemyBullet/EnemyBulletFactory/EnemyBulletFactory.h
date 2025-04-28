/*
	@file	EnemyBulletFactory.h
	@brief	敵弾ファクトリークラス
*/
#pragma once
// 標準ライブラリ
#include <memory>
// 自作ヘッダーファイル
#include "Game/Enemy/EnemyBullet/NormalBullet/NormalBullet.h"
#include "Game/Enemy/EnemyBullet/SpeedBullet/SpeedBullet.h"
#include "Game/Enemy/EnemyBullet/SpecialBullet/SpecialBullet.h"
#include "Game/Enemy/EnemyBullet/EnemyBullet.h"
#include "Game/Interface/IEnemyBullet.h"
#include "Game/Enemy/EnemyBullet/EnemyBulletType/EnemyBulletType.h"
class IEnemyBullet;
class NormalBullet;
class SpeedBullet;
class SpecialBullet;
class EnemyBullet;
class EnemyBulletFactory
{
public:

	static std::unique_ptr<IEnemyBullet> CreateBullet(BulletType type);
};


