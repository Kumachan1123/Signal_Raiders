//--------------------------------------------------------------------------------------
// File: PlayerHP.h
//
// HPゲージクラス
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

class PlayerHP
{
public:

	//変数
private:

	unsigned int m_menuIndex;
	DX::DeviceResources* m_pDR;

	//std::vector<std::unique_ptr<tito::UserInterface>> m_userInterface;
	//std::vector<std::unique_ptr<tito::UserInterface>> m_base;

	std::unique_ptr<PlayerUI> m_gauge;
	std::unique_ptr<PlayerUI> m_frame;
	std::unique_ptr<PlayerUI> m_base;


	const wchar_t* m_baseTexturePath;

	std::unique_ptr<PlayerUI> m_baseWindow;

	int m_windowWidth, m_windowHeight;

	DirectX::Keyboard::KeyboardStateTracker m_tracker;


	//関数
public:
	PlayerHP();
	~PlayerHP();

	void Initialize(DX::DeviceResources* pDR, int width, int height);
	void Update(float playerHP);
	void Render();

	void Add(const wchar_t* path
			 , DirectX::SimpleMath::Vector2 position
			 , DirectX::SimpleMath::Vector2 scale
			 , kumachi::ANCHOR anchor);
};
