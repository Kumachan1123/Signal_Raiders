#pragma once
#ifndef IMENUUI_DEFINED
#define IMENUUI_DEFINED
// 標準ライブラリ
#include <memory>
#include <SimpleMath.h>
// 自作ヘッダーファイル
#include "Game/KumachiLib/Anchor.h"
#include "Game/Interface/IUI.h"
// 前方宣言
class CommonResources;
class IMenuUI : public IUI
{
public:
	// 列挙型
	enum class UIType//UIの種類
	{
		SELECT,		// 選択可能UI
		NON_SELECT	// 選択不可能UI
	};
public:
	virtual ~IMenuUI() = default;// デストラクタ
	virtual void Add(							// UI追加	
		const wchar_t* path						// テクスチャパス
		, DirectX::SimpleMath::Vector2 position	// 位置
		, DirectX::SimpleMath::Vector2 scale	// スケール
		, KumachiLib::ANCHOR anchor				// アンカー
		, IMenuUI::UIType type) = 0;			// UIの種類	
};
#endif		// IMENUUI_DEFINED
