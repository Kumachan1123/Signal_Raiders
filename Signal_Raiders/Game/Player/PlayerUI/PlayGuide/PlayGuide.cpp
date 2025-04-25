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

void PlayGuide::Initialize(CommonResources* resources, int width, int height)
{
	m_pDR = resources->GetDeviceResources();
	m_windowWidth = width;
	m_windowHeight = height;
	CreatePlayerUI(L"Resources/Textures/PlayGuide.png"
		, SimpleMath::Vector2(0, 720)
		, SimpleMath::Vector2(0.5f, 0.5f)
		, KumachiLib::ANCHOR::BOTTOM_LEFT);

}

void PlayGuide::Update(const UpdateContext& context)
{
	UNREFERENCED_PARAMETER(context);
	Update();
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

void PlayGuide::Add(std::unique_ptr<PlayerUI>& pPlayerUI, const wchar_t* path, DirectX::SimpleMath::Vector2 position, DirectX::SimpleMath::Vector2 scale, KumachiLib::ANCHOR anchor)
{
	pPlayerUI = std::make_unique<PlayerUI>();
	pPlayerUI->Create(m_pDR
		, path
		, position
		, scale
		, anchor);
	pPlayerUI->SetWindowSize(m_windowWidth, m_windowHeight);
}

void PlayGuide::CreatePlayerUI(const wchar_t* path, DirectX::SimpleMath::Vector2 position, DirectX::SimpleMath::Vector2 scale, KumachiLib::ANCHOR anchor)
{
	Add(m_pointer, path, position, scale, anchor);
}


