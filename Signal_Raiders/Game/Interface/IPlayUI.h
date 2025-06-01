#pragma once
#ifndef IPLAYUI_DEFINED
#define IPLAYUI_DEFINED
// 標準ライブラリ
#include <memory>
#include <string>
// DirectX
#include <SimpleMath.h>
// 自作ヘッダーファイル
#include "Game/Player/PlayerUI/PlayerUI.h"
#include "Game/Interface/IUI.h"
// 前方宣言
class CommonResources;
class IPlayUI : public IUI
{
public:
	virtual ~IPlayUI() = default;// デストラクタ
	virtual void Add// UI追加
	(std::unique_ptr<PlayerUI>& pPlayerUI// プレイヤーUI
		, std::string key// テクスチャパス
		, DirectX::SimpleMath::Vector2 position// 位置
		, DirectX::SimpleMath::Vector2 scale// スケール
		, KumachiLib::ANCHOR anchor) = 0;// アンカー
};
#endif		// IPLAYUI_DEFINED
