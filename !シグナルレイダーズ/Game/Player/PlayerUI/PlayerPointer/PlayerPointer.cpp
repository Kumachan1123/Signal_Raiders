//--------------------------------------------------------------------------------------
// File: PlayerPointer.cpp
//
// è∆èÄÉNÉâÉX
//
//-------------------------------------------------------------------------------------
#include "pch.h"
#include "PlayerPointer.h"
#include "Game/Player/PlayerUI/PlayerUI.h"
#include "Game/KumachiLib/BinaryFile.h"
#include "DeviceResources.h"
#include <SimpleMath.h>
#include <Effects.h>
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <WICTextureLoader.h>
#include <CommonStates.h>
#include <vector>
using namespace DirectX;


PlayerPointer::PlayerPointer()
	: m_menuIndex(0)
	, m_windowHeight(0)
	, m_windowWidth(0)
	, m_pDR(nullptr)
	, m_baseTexturePath(nullptr)
	, m_pointer(nullptr)
{
}

PlayerPointer::~PlayerPointer()
{
}

void PlayerPointer::Initialize(DX::DeviceResources* pDR, int width, int height)
{
	m_pDR = pDR;
	m_windowWidth = width;
	m_windowHeight = height;


	Add(L"Resources/Textures/pointer.png"
		, SimpleMath::Vector2(640, 360)
		, SimpleMath::Vector2(0.25f, 0.25f)
		, kumachi::ANCHOR::MIDDLE_CENTER);
}

void PlayerPointer::Update()
{

	auto keystate = Keyboard::Get().GetState();
	m_tracker.Update(keystate);


}

void PlayerPointer::Render()
{
	m_pointer->Render();
}

void PlayerPointer::Add(const wchar_t* path, DirectX::SimpleMath::Vector2 position, DirectX::SimpleMath::Vector2 scale, kumachi::ANCHOR anchor)
{

	m_pointer = std::make_unique<PlayerUI>();
	m_pointer->Create(m_pDR
					  , path
					  , position
					  , scale
					  , anchor);
	m_pointer->SetWindowSize(m_windowWidth, m_windowHeight);


}


