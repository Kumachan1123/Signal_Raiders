#pragma once
#include "pch.h"
#include <Game/CommonResources.h>
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
	virtual void Initialize(CommonResources* resources) = 0;
	// 更新する
	virtual void Update(DirectX::SimpleMath::Vector3& Direction) = 0;
	// 描画する
	virtual void Render() = 0;
	// 後処理を行う
	virtual void Finalize() = 0;
	// 砲弾が発射されているかどうかを取得する
	virtual BulletState GetBulletState() const = 0;
	// 砲弾の状態を設定する
	virtual void SetBulletState(BulletState bulletState) = 0;

};