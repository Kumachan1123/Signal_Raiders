#pragma once
#include "pch.h"
class IBullet
{
public:
	enum BulletState
	{
		UNUSED,
		FLYING,
		USED
	};
public:
	// 仮想デストラクタ
	virtual ~IBullet() = default;
	// 初期化する
	virtual void Initialize() = 0;
	// 更新する
	virtual void Update() = 0;
	// 描画する
	virtual void Render() = 0;
	// 後処理を行う
	virtual void Finalize() = 0;
};