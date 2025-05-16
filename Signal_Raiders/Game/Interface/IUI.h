#pragma once
#ifndef IUI_DEFINED
#define IUI_DEFINED
// 標準ライブラリ
#include <SimpleMath.h>
// 前方宣言
class CommonResources;
// UI共通の更新情報構造体
struct UpdateContext
{
	float elapsedTime;// 経過時間
	float dashStamina;// ダッシュスタミナ
	float bulletPoint;// 弾ゲージ
	float playerHP;// プレイヤーHP
};

class IUI
{
public:
	virtual ~IUI() = default;// デストラクタ
	virtual void Initialize(CommonResources* resources, int width, int height) = 0;// 初期化
	virtual void Update(const UpdateContext& data) = 0;// 更新
	virtual void Render() = 0;// 描画
};
#endif		// IUI_DEFINED
