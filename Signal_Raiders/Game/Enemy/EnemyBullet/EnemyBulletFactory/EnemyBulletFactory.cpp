/*
	@file	EnemyBulletFactory.cpp
	@brief	“G’eƒtƒ@ƒNƒgƒŠ[ƒNƒ‰ƒX
*/
#include "pch.h"
#include "EnemyBulletFactory.h"
/*
*	@brief	’e‚Ì¶¬
*	@param[in]	BulletType type ’e‚Ìí—Ş
*	@return	¶¬‚µ‚½’e‚Ìƒ|ƒCƒ“ƒ^
*/
std::unique_ptr<IEnemyBullet> EnemyBulletFactory::CreateBullet(BulletType type)
{
	// ’e‚Ìí—Ş‚É‰‚¶‚Ä’e‚ğ¶¬
	switch (type)
	{
	case BulletType::NORMAL:// ’Êí’e
		return std::make_unique<NormalBullet>();
	case BulletType::SPECIAL:// “Áê’e
		return std::make_unique<SpecialBullet>();
	case BulletType::SPEED:// ‘¬‚¢’e
		return std::make_unique<SpeedBullet>();
	default:// •s³‚È’e‚Ìí—Ş
		return nullptr;
	}
};