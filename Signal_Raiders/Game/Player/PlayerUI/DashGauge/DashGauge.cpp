//--------------------------------------------------------------------------------------
// File: DashGauge.cpp
//
// ダッシュゲージクラス
//
//-------------------------------------------------------------------------------------
#include "pch.h"
#include "DashGauge.h"
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
using namespace DirectX;


DashGauge::DashGauge()
	: m_menuIndex(0)
	, m_windowHeight(0)
	, m_windowWidth(0)
	, m_pDR(nullptr)
	, m_baseTexturePath(nullptr)
	, m_gauge(nullptr)
	, m_frame(nullptr)
	, m_base(nullptr)
	, m_dash(nullptr)
{
}

DashGauge::~DashGauge()
{
}

void DashGauge::Initialize(CommonResources* resources, int width, int height)
{
	m_pDR = resources->GetDeviceResources();
	m_windowWidth = width;
	m_windowHeight = height;

	m_baseTexturePath = L"Resources/Textures/HPBar.png";


	Add(m_base,
		L"Resources/Textures/HPBarBase.png",
		SimpleMath::Vector2(210, 100),
		SimpleMath::Vector2(.250f, .250f),
		KumachiLib::ANCHOR::MIDDLE_CENTER);
	Add(m_frame,
		L"Resources/Textures/HPBarFrame.png",
		SimpleMath::Vector2(210, 100),
		SimpleMath::Vector2(.250f, .250f),
		KumachiLib::ANCHOR::MIDDLE_CENTER);
	Add(m_gauge,
		L"Resources/Textures/HPBar.png",
		SimpleMath::Vector2(210, 100),
		SimpleMath::Vector2(.250f, .250f),
		KumachiLib::ANCHOR::MIDDLE_CENTER);
	m_gauge->SetRenderRatioOffset(0);
	Add(m_dash,
		L"Resources/Textures/Dash.png",
		SimpleMath::Vector2(-5, 75),
		SimpleMath::Vector2(1.0f, 1.0f),
		KumachiLib::ANCHOR::TOP_LEFT);


}



void DashGauge::Update(float dashStamina)
{

	auto keystate = Keyboard::Get().GetState();
	m_tracker.Update(keystate);

	float stamina = dashStamina * 0.1f;
	stamina = std::max(0.f, stamina);
	m_gauge->SetRenderRatio(stamina);

}

void DashGauge::Render()
{
	//シェーダーの種類を設定して描画
	m_base->SetShaderType(PlayerUI::ShaderType::OTHER), m_base->Render();
	m_gauge->SetShaderType(PlayerUI::ShaderType::HP), m_gauge->Render();
	m_frame->SetShaderType(PlayerUI::ShaderType::OTHER), m_frame->Render();
	m_dash->SetShaderType(PlayerUI::ShaderType::OTHER), m_dash->Render();
}

void DashGauge::Add(std::unique_ptr<PlayerUI>& pPlayerUI, const wchar_t* path, DirectX::SimpleMath::Vector2 position, DirectX::SimpleMath::Vector2 scale, KumachiLib::ANCHOR anchor)
{
	pPlayerUI = std::make_unique<PlayerUI>();
	pPlayerUI->Create(m_pDR
		, path
		, position
		, scale
		, anchor);
	pPlayerUI->SetWindowSize(m_windowWidth, m_windowHeight);
}


