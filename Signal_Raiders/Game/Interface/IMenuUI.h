#pragma once
#ifndef IMENUUI_DEFINED
#define IMENUUI_DEFINED
#include <SimpleMath.h>
#include <memory>
#include "Game/KumachiLib/Anchor.h"
#include "Game/Interface/IUI.h"


class CommonResources;



class IMenuUI : public IUI
{
public:

	enum class UIType
	{
		// 選択可能UI
		SELECT,
		// 選択不可能UI
		NON_SELECT
	};
public:
	virtual ~IMenuUI() = default;
	virtual void Add(const wchar_t* path
		, DirectX::SimpleMath::Vector2 position
		, DirectX::SimpleMath::Vector2 scale
		, KumachiLib::ANCHOR anchor
		, IMenuUI::UIType type) = 0;
};
#endif		// IMENUUI_DEFINED
