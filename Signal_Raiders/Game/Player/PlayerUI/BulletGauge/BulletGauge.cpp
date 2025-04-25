#include "pch.h"
#include "BulletGauge.h"
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

BulletGauge::BulletGauge()
	: m_menuIndex(0)
	, m_windowHeight(0)
	, m_windowWidth(0)
	, m_pDR(nullptr)
	, m_baseTexturePath(nullptr)
	, m_gauge(nullptr)
{
}

BulletGauge::~BulletGauge()
{
}

void BulletGauge::Initialize(CommonResources* resources, int width, int height)
{
	m_pDR = resources->GetDeviceResources();
	m_windowWidth = width;
	m_windowHeight = height;

	Add(L"Resources/Textures/BulletGauge.png"
		, SimpleMath::Vector2(640, 360)
		, SimpleMath::Vector2(0.8f, 0.8f)
		, KumachiLib::ANCHOR::MIDDLE_CENTER);

}

void BulletGauge::Update(float bulletPoint)
{
	bulletPoint;
	float bp = bulletPoint * 0.02f;
	bp = std::max(0.f, bp);
	m_gauge->SetRenderRatio(bp);
}

void BulletGauge::Render()
{
	m_gauge->SetShaderType(PlayerUI::ShaderType::CIRCLE), m_gauge->Render();
}

void BulletGauge::Add(const wchar_t* path
	, DirectX::SimpleMath::Vector2 position
	, DirectX::SimpleMath::Vector2 scale
	, KumachiLib::ANCHOR anchor)
{
	m_gauge = std::make_unique<PlayerUI>();
	m_gauge->Create(m_pDR
		, path
		, position
		, scale
		, anchor);
	m_gauge->SetWindowSize(m_windowWidth, m_windowHeight);
}