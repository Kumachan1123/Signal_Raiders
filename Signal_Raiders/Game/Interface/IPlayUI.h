/*
*	@file IPlayUI.h
*	@brief プレイUIのインターフェースクラス
*/
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

// インターフェースクラス：プレイUI
class IPlayUI : public IUI
{
public:
	// public関数
	// デストラクタ
	virtual ~IPlayUI() = default;
	// UI追加
	virtual void Add(
		std::unique_ptr<PlayerUI>& pPlayerUI// プレイヤーUI
		, const std::string& key// テクスチャパス
		, const DirectX::SimpleMath::Vector2& position// 位置
		, const DirectX::SimpleMath::Vector2& scale// スケール
		, KumachiLib::ANCHOR anchor) = 0;// アンカー
};
#endif		// IPLAYUI_DEFINED
