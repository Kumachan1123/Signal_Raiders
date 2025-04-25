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
		// �I���\UI
		SELECT,
		// �I��s�\UI
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
