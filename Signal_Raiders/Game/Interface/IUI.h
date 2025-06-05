/*
*	@file IUI.h
*	@brief UIのインターフェースクラス
*/
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

// UIのインターフェースクラス
class IUI
{
public:
	// public関数
	// デストラクタ
	virtual ~IUI() = default;
	// 初期化
	virtual void Initialize(CommonResources* resources, int width, int height) = 0;
	// 更新
	virtual void Update(const UpdateContext& data) = 0;
	// 描画
	virtual void Render() = 0;
};
#endif		// IUI_DEFINED
