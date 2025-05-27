/*
*	@file	EnemyFactory.h
*	@brief	�G�t�@�N�g���[�N���X
*/
#pragma once
// �W�����C�u����
#include <memory>
// ����w�b�_�[�t�@�C��
#include "Game/Enemy/Enemy.h"
#include "Game/Enemy/VerticalAttacker/VerticalAttacker.h"
#include "Game/Enemy/BossBase/BossBase.h"
#include "Game/Enemy/EnemyType/EnemyType.h"
#include "Game/Interface/IEnemy.h"

class EnemyFactory
{
public:
	static std::unique_ptr<IEnemy> CreateEnemy(EnemyType type, Player* player, CommonResources* resources, int HP);// �G����
	static std::unique_ptr<IBossLogic> CreateBoss(BossType type, BossBase* pBoss, CommonResources* resources);// �{�X����

};


