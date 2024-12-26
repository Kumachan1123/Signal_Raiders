//--------------------------------------------------------------------------------------
// File: PlayerHP.cpp
//
// HPゲージクラス
//
//-------------------------------------------------------------------------------------
#include "pch.h"
#include "PlayerHP.h"
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
using namespace DirectX;


PlayerHP::PlayerHP()
	: m_menuIndex(0)
	, m_windowHeight(0)
	, m_windowWidth(0)
	, m_pDR(nullptr)
	, m_baseTexturePath(nullptr)
	, m_gauge(nullptr)
	, m_frame(nullptr)
	, m_base(nullptr)
	, m_heart(nullptr)
{
}

PlayerHP::~PlayerHP()
{
}

void PlayerHP::Initialize(DX::DeviceResources* pDR, int width, int height)
{
	m_pDR = pDR;
	m_windowWidth = width;
	m_windowHeight = height;

	m_baseTexturePath = L"Resources/Textures/HPBar.png";

	Add(L"Resources/Textures/HPBarFrame.png"
		, SimpleMath::Vector2(328, 40)
		, SimpleMath::Vector2(.50f, .50f)
		, KumachiLib::ANCHOR::MIDDLE_CENTER);
}

void PlayerHP::Update(float PlayerHP)
{

	auto keystate = Keyboard::Get().GetState();
	m_tracker.Update(keystate);

	float hp = PlayerHP * 0.01f;
	hp = std::max(0.f, hp);
	m_gauge->SetRenderRatio(hp);

}

void PlayerHP::Render()
{
	//シェーダーの種類を設定して描画
	m_base->SetShaderType(PlayerUI::ShaderType::OTHER), m_base->Render();
	m_gauge->SetShaderType(PlayerUI::ShaderType::HP), m_gauge->Render();
	m_frame->SetShaderType(PlayerUI::ShaderType::OTHER), m_frame->Render();
	m_heart->SetShaderType(PlayerUI::ShaderType::OTHER), m_heart->Render();
}

void PlayerHP::Add(const wchar_t* path, DirectX::SimpleMath::Vector2 position, DirectX::SimpleMath::Vector2 scale, KumachiLib::ANCHOR anchor)
{
	m_base = std::make_unique<PlayerUI>();
	m_base->Create(m_pDR
		, L"Resources/Textures/HPBarBase.png"
		, position
		, scale
		, anchor);
	m_base->SetWindowSize(m_windowWidth, m_windowHeight);


	m_gauge = std::make_unique<PlayerUI>();
	m_gauge->Create(m_pDR
		, m_baseTexturePath
		, position
		, scale
		, anchor);
	m_gauge->SetWindowSize(m_windowWidth, m_windowHeight);
	m_gauge->SetRenderRatioOffset(0);

	m_frame = std::make_unique<PlayerUI>();
	m_frame->Create(m_pDR
		, path
		, position
		, scale
		, anchor);
	m_frame->SetWindowSize(m_windowWidth, m_windowHeight);

	m_heart = std::make_unique<PlayerUI>();
	m_heart->Create(m_pDR
		, L"Resources/Textures/HP.png"
		, SimpleMath::Vector2(0, -5)
		, SimpleMath::Vector2(.50f, .50f)
		, KumachiLib::ANCHOR::TOP_LEFT);
	m_heart->SetWindowSize(m_windowWidth, m_windowHeight);
}


