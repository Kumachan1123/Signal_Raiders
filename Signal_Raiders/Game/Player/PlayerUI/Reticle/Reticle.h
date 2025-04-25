//--------------------------------------------------------------------------------------
// File: PlayerHP.h
//
// HPゲージクラス
//
//-------------------------------------------------------------------------------------

#pragma once

#include "StepTimer.h"
#include "Game/Player/PlayerUI/PlayerUI.h"
#include "Game/KumachiLib/BinaryFile/BinaryFile.h"
#include <DeviceResources.h>
#include <SimpleMath.h>
#include <Effects.h>
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <WICTextureLoader.h>
#include <CommonStates.h>
#include <vector>
#include "Keyboard.h"
#include "Game/Screen.h"
#include "Game/Interface/IPlayUI.h"
class Reticle : public IPlayUI
{


	//変数
private:

	DX::DeviceResources* m_pDR;


	std::unique_ptr<PlayerUI> m_reticle;


	const wchar_t* m_baseTexturePath;

	std::unique_ptr<PlayerUI> m_baseWindow;

	int m_windowWidth, m_windowHeight;

	DirectX::Keyboard::KeyboardStateTracker m_tracker;


	//関数
public:
	Reticle();
	~Reticle();

	void Initialize(CommonResources* resources, int width, int height)override;
	void Update(const UpdateContext& context)override;
	void Render()override;
	void Add(std::unique_ptr<PlayerUI>& pPlayerUI, const wchar_t* path
		, DirectX::SimpleMath::Vector2 position
		, DirectX::SimpleMath::Vector2 scale
		, KumachiLib::ANCHOR anchor)override;

private:
	void Update();
};
