/*
*	@file	EnemyBulletFactory.cpp
*	@brief	敵弾ファクトリークラス
*/
#include "pch.h"
#include "EnemyBulletFactory.h"
/*
*	@brief	弾の生成
*	@details 敵弾ファクトリークラスの静的関数
*	@param	BulletType type 弾の種類
*	@return	生成した弾のポインタ
*/
std::unique_ptr<IEnemyBullet> EnemyBulletFactory::CreateBullet(BulletType type)
{
	// 弾の種類に応じて弾を生成
	switch (type)
	{
	case BulletType::NORMAL:// 通常弾
		return std::make_unique<NormalBullet>();
	case BulletType::SPECIAL:// 特殊弾
		return std::make_unique<SpecialBullet>();
	case BulletType::SPEED:// 速い弾
		return std::make_unique<SpeedBullet>();
	default:// 不正な弾の種類
		return nullptr;
	}
};