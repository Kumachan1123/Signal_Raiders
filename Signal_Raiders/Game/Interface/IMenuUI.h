#pragma once
#ifndef IMENUUI_DEFINED
#define IMENUUI_DEFINED
// 標準ライブラリ
#include <memory>
#include <string>
// DirectX
#include <SimpleMath.h>
// 自作ヘッダーファイル
#include "Game/KumachiLib/Anchor.h"
#include "Game/Interface/IUI.h"
// 前方宣言
class CommonResources;
class IMenuUI : public IUI
{
public:	// 列挙型
	//UIの種類
	enum class UIType
	{
		SELECT,		// 選択可能UI
		NON_SELECT	// 選択不可能UI
	};
public:// public関数
	// デストラクタ
	virtual ~IMenuUI() = default;
	// UI追加	
	virtual void Add(
		const std::string& key					// テクスチャのキー
		, const DirectX::SimpleMath::Vector2& position	// 位置
		, const DirectX::SimpleMath::Vector2& scale	// スケール
		, KumachiLib::ANCHOR anchor				// アンカー
		, IMenuUI::UIType type) = 0;			// UIの種類	
};
#endif		// IMENUUI_DEFINED
