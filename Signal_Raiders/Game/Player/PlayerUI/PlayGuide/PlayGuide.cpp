//--------------------------------------------------------------------------------------
// File: PlayGuide.cpp
//
// プレイ操作説明クラス
//
//-------------------------------------------------------------------------------------
#include "pch.h"
#include "PlayGuide.h"
#include "Game/Player/PlayerUI/PlayerUI.h"
#include "Game/KumachiLib/BinaryFile/BinaryFile.h"
#include "DeviceResources.h"
#include <SimpleMath.h>
#include <Effects.h>
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <WICTextureLoader.h>
#include <CommonStates.h>
#include <vector>
#include "Game/Screen.h"
using namespace DirectX;


PlayGuide::PlayGuide()
	: m_menuIndex(0)
	, m_windowHeight(720)
	, m_windowWidth(1280)
	, m_pDR(nullptr)
	, m_baseTexturePath(nullptr)
	, m_pointer(nullptr)
{
}

PlayGuide::~PlayGuide()
{
}

void PlayGuide::Initialize(DX::DeviceResources* pDR)
{
	m_pDR = pDR;

	Add(L"Resources/Textures/PlayGuide.png"
		, SimpleMath::Vector2(0, 720)
		, SimpleMath::Vector2(0.75f, 0.75f)
		, KumachiLib::ANCHOR::BOTTOM_LEFT);

}

void PlayGuide::Update()
{

	auto keystate = Keyboard::Get().GetState();
	m_tracker.Update(keystate);


}

void PlayGuide::Render()
{
	m_pointer->SetShaderType(PlayerUI::ShaderType::OTHER);
	m_pointer->Render();
}

void PlayGuide::Add(const wchar_t* path, DirectX::SimpleMath::Vector2 position, DirectX::SimpleMath::Vector2 scale, KumachiLib::ANCHOR anchor)
{

	m_pointer = std::make_unique<PlayerUI>();
	m_pointer->Create(m_pDR
		, path
		, position
		, scale
		, anchor);
	m_pointer->SetWindowSize(m_windowWidth, m_windowHeight);


}


