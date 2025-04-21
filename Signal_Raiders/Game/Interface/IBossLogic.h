#pragma once
#ifndef IBOSSLOGIC_DEFINED
#define IBOSSLOGIC_DEFINED
// 標準ライブラリ
#include <SimpleMath.h>
class Player;
class CommonResources;
class BulletManager;

class IBossLogic
{
public:
	virtual ~IBossLogic() = default;                                          // デストラクタ
	virtual void CreateModel() = 0;                                             // 初期化
	virtual void ChangeState() = 0;                                // 更新
	virtual void Draw(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj) = 0;        // 描画
};
#endif // IBOSSLOGIC_DEFINED
