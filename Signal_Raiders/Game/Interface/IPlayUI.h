#pragma once
#ifndef IPLAYUI_DEFINED
#define IPLAYUI_DEFINED
#include <SimpleMath.h>
#include <memory>
#include "Game/Player/PlayerUI/PlayerUI.h"
#include "Game/Interface/IUI.h"


class CommonResources;



class IPlayUI : public IUI
{

public:
	virtual ~IPlayUI() = default;
	virtual void Add(std::unique_ptr<PlayerUI>& pPlayerUI, const wchar_t* path
		, DirectX::SimpleMath::Vector2 position
		, DirectX::SimpleMath::Vector2 scale
		, KumachiLib::ANCHOR anchor) = 0;
};
#endif		// IPLAYUI_DEFINED
