#pragma once
#ifndef IBULLET_DEFINED
#define IBULLET_DEFINED
// 標準ライブラリ
#include <SimpleMath.h>
// 前方宣言
class CommonResources;
class IBullet
{
public:	// public関数
	// デストラクタ
	virtual ~IBullet() = default;
	// 初期化
	virtual void Initialize(CommonResources* resources) = 0;
	// 更新
	virtual void Update(float elapsedTime) = 0;
	// 描画
	virtual void Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) = 0;
	// 影の描画
	virtual void RenderShadow(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) = 0;
};
#endif		// IBULLET_DEFINED
