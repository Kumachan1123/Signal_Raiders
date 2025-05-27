/*
	@file	EnemyFactory.cpp
	@brief	敵ファクトリークラス
*/
#include "pch.h"
#include "EnemyFactory.h"
/*
*	@brief	敵生成
*	@param	EnemyType type	敵の種類
*	@param	Player* player	プレイヤーのポインタ
*	@param	CommonResources* resources	リソースのポインタ
*	@param	int HP	敵のHP
*	@return	敵のポインタ
*/
std::unique_ptr<IEnemy> EnemyFactory::CreateEnemy(EnemyType type, Player* player, CommonResources* resources, int HP)
{
	switch (type)// 敵の種類によって生成する敵を変える
	{
	case EnemyType::NORMAL:// 通常敵
		return std::make_unique<Enemy>(player, resources, HP);
	case EnemyType::VERTICAL_ATTACKER:// 速い弾を撃つ敵
		return std::make_unique<VerticalAttacker>(player, resources, HP);
	case EnemyType::BOSS:// ボス
		return std::make_unique<BossBase>(player, resources, HP);
	default:// それ以外の敵は生成しない
		return nullptr;
	}
}

/*
*	@brief	ボス生成
*	@param	BossType type	ボスの種類
*	@param	BossBase* pBoss	ボスのポインタ
*	@param	CommonResources* resources	リソースのポインタ
*	@return	ボスのポインタ
*/
std::unique_ptr<IBossLogic> EnemyFactory::CreateBoss(BossType type, BossBase* pBoss, CommonResources* resources)
{
	switch (type)// ボスの種類によって生成するボスを変える
	{
	case BossType::NORMAL_BOSS:// 通常ボス
		return std::make_unique<Boss>(pBoss, resources);
	case BossType::LAST_BOSS:// 最終ボス
		return std::make_unique<LastBoss>(pBoss, resources);
	default:// それ以外のボスは生成しない
		return nullptr;
	}
}
