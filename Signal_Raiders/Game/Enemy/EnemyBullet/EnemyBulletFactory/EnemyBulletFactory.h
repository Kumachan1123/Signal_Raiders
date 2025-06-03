/*
*	@file	EnemyBulletFactory.h
*	@brief	敵弾ファクトリークラス
*/
#pragma once
#ifndef ENEMY_BULLET_FACTORY_DEFINED
#define ENEMY_BULLET_FACTORY_DEFINED
// 標準ライブラリ
#include <memory>
// 自作ヘッダーファイル
#include "Game/Enemy/EnemyBullet/NormalBullet/NormalBullet.h"
#include "Game/Enemy/EnemyBullet/SpeedBullet/SpeedBullet.h"
#include "Game/Enemy/EnemyBullet/SpecialBullet/SpecialBullet.h"
#include "Game/Enemy/EnemyBullet/EnemyBullet.h"
#include "Game/Interface/IEnemyBullet.h"
#include "Game/Enemy/EnemyBullet/EnemyBulletType/EnemyBulletType.h"
// 前方宣言
class IEnemyBullet;
class NormalBullet;
class SpeedBullet;
class SpecialBullet;
class EnemyBullet;
class EnemyBulletFactory
{
public:// public静的関数
	// 弾の生成
	static std::unique_ptr<IEnemyBullet> CreateBullet(BulletType type);
};
#endif //ENEMY_BULLET_FACTORY_DEFINED

