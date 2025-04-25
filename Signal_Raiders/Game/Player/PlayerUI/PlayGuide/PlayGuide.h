//--------------------------------------------------------------------------------------
// File: PlayGuide.h
//
// プレイ操作説明クラス
//
//-------------------------------------------------------------------------------------

#pragma once

#include "StepTimer.h"
#include "Game/Player/PlayerUI/PlayerUI.h"
#include <DeviceResources.h>
#include <SimpleMath.h>
#include <Effects.h>
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <WICTextureLoader.h>
#include <CommonStates.h>
#include <vector>
#include "Keyboard.h"
#include "Game/Interface/IPlayUI.h"
class PlayGuide : public IPlayUI
{
public:

	//変数
private:

	unsigned int m_menuIndex;
	DX::DeviceResources* m_pDR;


	std::unique_ptr<PlayerUI> m_pointer;


	const wchar_t* m_baseTexturePath;

	std::unique_ptr<PlayerUI> m_baseWindow;

	int m_windowWidth, m_windowHeight;

	DirectX::Keyboard::KeyboardStateTracker m_tracker;


	//関数
public:
	PlayGuide();
	~PlayGuide();

	void Initialize(CommonResources* resources, int width, int height)override;
	void Update(const UpdateContext& context)override;
	void Render()override;
	void Add(std::unique_ptr<PlayerUI>& pPlayerUI, const wchar_t* path
		, DirectX::SimpleMath::Vector2 position
		, DirectX::SimpleMath::Vector2 scale
		, KumachiLib::ANCHOR anchor)override;
	void CreatePlayerUI(const wchar_t* path
		, DirectX::SimpleMath::Vector2 position
		, DirectX::SimpleMath::Vector2 scale
		, KumachiLib::ANCHOR anchor);
private:
	void Update();

};
