/*
*	@file	EnemyType.h
*	@brief	�G�^�C�v�N���X
*/
#pragma once
#ifndef ENEMY_TYPE_DEFINED
#define ENEMY_TYPE_DEFINED

// �G�̃^�C�v
enum class EnemyType
{
	NORMAL,// �ʏ�
	VERTICAL_ATTACKER,// �����e�����G
	BOSS,// �{�X
};
// �{�X�̃^�C�v
enum class BossType
{
	NORMAL_BOSS,// �ʏ�{�X
	LAST_BOSS,// ���X�{�X
};
#endif // ENEMY_TYPE_DEFINED