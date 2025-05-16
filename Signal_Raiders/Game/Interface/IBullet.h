#pragma once
#ifndef IBULLET_DEFINED
#define IBULLET_DEFINED
// 標準ライブラリ
#include <SimpleMath.h>
// 前方宣言
class CommonResources;
class IBullet
{
public:
	// 関数
	virtual ~IBullet() = default;// デストラクタ
	virtual void Initialize(CommonResources* resources) = 0;// 初期化
	virtual void Update(float elapsedTime) = 0;// 更新
	virtual void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj) = 0;// 描画
	virtual void RenderShadow(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj) = 0;// 影の描画
};
#endif		// IBULLET_DEFINED
