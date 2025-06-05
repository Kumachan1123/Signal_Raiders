/*
*	@file	EnemyFactory.h
*	@brief	�G�t�@�N�g���[�N���X
*/
#pragma once
#ifndef ENEMY_FACTORY_DEFINED
#define ENEMY_FACTORY_DEFINED
// �W�����C�u����
#include <memory>
// ����w�b�_�[�t�@�C��
#include "Game/Enemy/Enemy.h"
#include "Game/Enemy/VerticalAttacker/VerticalAttacker.h"
#include "Game/Enemy/BossBase/BossBase.h"
#include "Game/Enemy/EnemyType/EnemyType.h"
#include "Game/Interface/IEnemy.h"

// �G�t�@�N�g���[�N���X
class EnemyFactory
{
public:
	// public�֐�
	// �G����
	static std::unique_ptr<IEnemy> CreateEnemy(EnemyType type, Player* player, CommonResources* resources, int HP);
	// �{�X����
	static std::unique_ptr<IBossLogic> CreateBoss(BossType type, BossBase* pBoss, CommonResources* resources);
};
#endif //ENEMY_FACTORY_DEFINED