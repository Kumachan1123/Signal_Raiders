#pragma once
#ifndef ISTATE_DEFINED
#define ISTATE_DEFINED
// 標準ライブラリ
#include <SimpleMath.h>
// 前方宣言
class EnemyAI;
class IState
{
public:
	// 列挙型
	enum  EnemyState//敵の状態
	{
		IDLING = 0,//徘徊
		ATTACK,// 攻撃
		HIT,// 攻撃を食らった
		ANGRY,// 怒り
	};
public:
	virtual ~IState() = default;	// デストラクタ
	virtual void Initialize() = 0;	// 初期化
	virtual void Update(float elapsedTime) = 0;	// 更新
};
#endif		// ISTATE_DEFINED
