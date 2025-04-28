/*
	@file	EnemyBulletFactory.h
	@brief	�G�e�t�@�N�g���[�N���X
*/
#pragma once
// �W�����C�u����
#include <memory>
// ����w�b�_�[�t�@�C��
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


