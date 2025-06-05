/*
*	@file IState.h
*	@brief 敵の状態を管理するインターフェースクラス
*/
#pragma once
#ifndef ISTATE_DEFINED
#define ISTATE_DEFINED
// 標準ライブラリ
#include <SimpleMath.h>

// 前方宣言
class EnemyAI;

// 敵の状態を管理するインターフェースクラス
class IState
{
public:
	// 列挙型
	//敵の状態
	enum  EnemyState
	{
		IDLING = 0,//徘徊
		ATTACK,// 攻撃
		HIT,// 攻撃を食らった
		ANGRY,// 怒り
		KNOCKBACK,// ノックバック
		CHASE,// 追跡
		SHADOWSTEP,//回り込み
	};
public:
	// public関数
	// デストラクタ
	virtual ~IState() = default;
	// 初期化
	virtual void Initialize() = 0;
	// 更新
	virtual void Update(float elapsedTime) = 0;
};
#endif		// ISTATE_DEFINED
