//--------------------------------------------------------------------------------------
// File: Reticle.cpp
//
// 照準クラス
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

void Reticle::Initialize(CommonResources* resources, int width, int height)
{
	m_pDR = resources->GetDeviceResources();
	m_windowWidth = width;
	m_windowHeight = height;


	Add(m_reticle
		, L"Resources/Textures/Reticle.png"
		, SimpleMath::Vector2(640, 360)
		, SimpleMath::Vector2(0.25f, 0.25f)
		, KumachiLib::ANCHOR::MIDDLE_CENTER);

}

void Reticle::Update(const UpdateContext& context)
{
	// UpdateContextについて、未使用警告を出さない
	UNREFERENCED_PARAMETER(context);
	Update();

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

void Reticle::Add(std::unique_ptr<PlayerUI>& pPlayerUI, const wchar_t* path, DirectX::SimpleMath::Vector2 position, DirectX::SimpleMath::Vector2 scale, KumachiLib::ANCHOR anchor)
{
	pPlayerUI = std::make_unique<PlayerUI>();
	pPlayerUI->Create(m_pDR
		, path
		, position
		, scale
		, anchor);
	pPlayerUI->SetWindowSize(m_windowWidth, m_windowHeight);

}



