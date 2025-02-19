//--------------------------------------------------------------------------------------
// File: Reticle.cpp
//
// Æ€ƒNƒ‰ƒX
//
//-------------------------------------------------------------------------------------
#include "pch.h"
#include "Reticle.h"

using namespace DirectX;


Reticle::Reticle()
	: m_windowHeight(0)
	, m_windowWidth(0)
	, m_pDR(nullptr)
	, m_baseTexturePath(nullptr)
	, m_reticle(nullptr)
{
}

Reticle::~Reticle()
{
}

void Reticle::Initialize(DX::DeviceResources* pDR, int width, int height)
{
	m_pDR = pDR;
	m_windowWidth = width;
	m_windowHeight = height;


	Add(L"Resources/Textures/Reticle.png"
		, SimpleMath::Vector2(640, 360)
		, SimpleMath::Vector2(0.25f, 0.25f)
		, KumachiLib::ANCHOR::MIDDLE_CENTER);

}

void Reticle::Update()
{

	auto keystate = Keyboard::Get().GetState();
	m_tracker.Update(keystate);


}

void Reticle::Render()
{
	m_reticle->SetShaderType(PlayerUI::ShaderType::OTHER);
	m_reticle->Render();
}

void Reticle::Add(const wchar_t* path, DirectX::SimpleMath::Vector2 position, DirectX::SimpleMath::Vector2 scale, KumachiLib::ANCHOR anchor)
{

	m_reticle = std::make_unique<PlayerUI>();
	m_reticle->Create(m_pDR
		, path
		, position
		, scale
		, anchor);
	m_reticle->SetWindowSize(m_windowWidth, m_windowHeight);


}


