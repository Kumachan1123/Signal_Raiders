#pragma once
#ifndef IBOSSLOGIC_DEFINED
#define IBOSSLOGIC_DEFINED
// 標準ライブラリ
#include <SimpleMath.h>
// 前方宣言
class Player;
class CommonResources;
class BulletManager;
class IBossLogic
{
public:
	virtual ~IBossLogic() = default;// デストラクタ
	virtual void Initialize() = 0;// 初期化
	virtual void ChangeState() = 0;// 更新
	virtual void Draw(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj) = 0;// 描画
	virtual void BulletPositioning() = 0;// 弾の発射位置を決定
	virtual	void CreateBullet() = 0;// 弾を生成
};
#endif // IBOSSLOGIC_DEFINED