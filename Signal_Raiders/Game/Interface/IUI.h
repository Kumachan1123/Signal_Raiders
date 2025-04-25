#pragma once
#ifndef IUI_DEFINED
#define IUI_DEFINED
#include <SimpleMath.h>

class CommonResources;
// UI共通の更新情報
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
	virtual ~IUI() = default;
	virtual void Initialize(CommonResources* resources, int width, int height) = 0;
	virtual void Update(const UpdateContext& data) = 0;
	virtual void Render() = 0;
};
#endif		// IUI_DEFINED
