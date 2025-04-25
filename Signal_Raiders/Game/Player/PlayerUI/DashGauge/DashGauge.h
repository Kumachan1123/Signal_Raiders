//--------------------------------------------------------------------------------------
// File: DashGauge.h
//
// ダッシュゲージクラス
//
//-------------------------------------------------------------------------------------

#pragma once

#include "StepTimer.h"
#include <DeviceResources.h>
#include <SimpleMath.h>
#include <Effects.h>
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <WICTextureLoader.h>
#include <CommonStates.h>
#include <vector>
#include "Keyboard.h"
#include "Game/Player/PlayerUI/PlayerUI.h"
#include "Game/Interface/IPlayUI.h"
class DashGauge : public IPlayUI
{

	//変数
private:

	unsigned int m_menuIndex;
	DX::DeviceResources* m_pDR;

	std::unique_ptr<PlayerUI> m_dash;
	std::unique_ptr<PlayerUI> m_gauge;
	std::unique_ptr<PlayerUI> m_frame;
	std::unique_ptr<PlayerUI> m_base;


	const wchar_t* m_baseTexturePath;

	std::unique_ptr<PlayerUI> m_baseWindow;

	int m_windowWidth, m_windowHeight;

	DirectX::Keyboard::KeyboardStateTracker m_tracker;


	//関数
public:
	DashGauge();
	~DashGauge();

	void Initialize(CommonResources* resources, int width, int height)override;
	void Update(const UpdateContext& context)override
	{
		Update(context.dashStamina);

	}
	void Render()override;
	void Add(std::unique_ptr<PlayerUI>& pPlayerUI, const wchar_t* path
		, DirectX::SimpleMath::Vector2 position
		, DirectX::SimpleMath::Vector2 scale
		, KumachiLib::ANCHOR anchor)override;

private:
	void Update(float dashStamina);
};
