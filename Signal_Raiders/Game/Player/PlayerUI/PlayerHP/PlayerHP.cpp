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
#include "Game/KumachiLib/KumachiLib.h"
using namespace DirectX;


PlayerHP::PlayerHP()
	: m_menuIndex(0)
	, m_windowHeight(0)
	, m_windowWidth(0)
	, m_maxHP(100.0f)
	, m_currentHP(100.0f)
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

void PlayerHP::Initialize(CommonResources* resources, int width, int height)
{
	m_pDR = resources->GetDeviceResources();
	m_windowWidth = width;
	m_windowHeight = height;

	// HPバー
	Add(m_gauge, L"Resources/Textures/HPBar.png"
		, SimpleMath::Vector2(328, 40)
		, SimpleMath::Vector2(.50f, .50f)
		, KumachiLib::ANCHOR::MIDDLE_CENTER);
	m_gauge->SetRenderRatioOffset(0);

	// HPバーの背景
	Add(m_base, L"Resources/Textures/HPBarBase.png"
		, SimpleMath::Vector2(328, 40)
		, SimpleMath::Vector2(.50f, .50f)
		, KumachiLib::ANCHOR::MIDDLE_CENTER);

	// HPバーの枠
	Add(m_frame, L"Resources/Textures/HPBarFrame.png"
		, SimpleMath::Vector2(328, 40)
		, SimpleMath::Vector2(.50f, .50f)
		, KumachiLib::ANCHOR::MIDDLE_CENTER);

	// ハート
	Add(m_heart, L"Resources/Textures/HP.png"
		, SimpleMath::Vector2(0, -5)
		, SimpleMath::Vector2(.50f, .50f)
		, KumachiLib::ANCHOR::TOP_LEFT);
}

void PlayerHP::Update(float PlayerHP)
{

	if (m_maxHP <= 0.0f) return; // 最大HPが0以下にならないようチェック

	float hp = PlayerHP / m_maxHP; // 最大HPを考慮して正規化
	hp = Clamp(hp, 0.0f, 1.0f); // 0.0～1.0の範囲に収める

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

void PlayerHP::Add(std::unique_ptr<PlayerUI>& pPlayerUI, const wchar_t* path, DirectX::SimpleMath::Vector2 position, DirectX::SimpleMath::Vector2 scale, KumachiLib::ANCHOR anchor)
{


	pPlayerUI = std::make_unique<PlayerUI>();
	pPlayerUI->Create(m_pDR
		, path
		, position
		, scale
		, anchor);
	pPlayerUI->SetWindowSize(m_windowWidth, m_windowHeight);


}


