/*
*	@file	EnemyType.h
*	@brief	敵タイプクラス
*/
#pragma once
#ifndef ENEMY_TYPE_DEFINED
#define ENEMY_TYPE_DEFINED

// 敵のタイプ
enum class EnemyType
{
	NORMAL,// 通常
	VERTICAL_ATTACKER,// 速い弾を撃つ敵
	BOSS,// ボス
};
// ボスのタイプ
enum class BossType
{
	NORMAL_BOSS,// 通常ボス
	LAST_BOSS,// ラスボス
};
#endif // ENEMY_TYPE_DEFINED