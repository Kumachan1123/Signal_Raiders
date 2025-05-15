/*
	@file	EnemyType.h
	@brief	敵タイプクラス
*/
#pragma once
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