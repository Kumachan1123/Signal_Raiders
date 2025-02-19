//--------------------------------------------------------------------------------------
// File: WifiLoading.cpp
//
// Wi-FiローディングUIクラス
//
//-------------------------------------------------------------------------------------
#include "pch.h"
#include "WifiLoading.h"

WifiLoading::WifiLoading()
	: m_windowHeight(0)
	, m_windowWidth(0)
	, m_pDR(nullptr)
	, m_baseTexturePath(nullptr)
	, m_loading(nullptr)
	, m_loadgingText(nullptr)
	, m_animSpeed(1)// アニメーションスピード
	, m_anim(0)// アニメーションカウント
	, m_animTime(0)// アニメーション時間
	, m_frameRows(1)// 画像の行数
	, m_frameCols(4)// 画像の列数
{
}

WifiLoading::~WifiLoading()
{
}

void WifiLoading::Initialize(DX::DeviceResources* pDR, int width, int height)
{
	m_pDR = pDR;
	m_windowWidth = width;
	m_windowHeight = height;
	// ローディング画像
	Add(m_loading, L"Resources/Textures/Loading.png",
		DirectX::SimpleMath::Vector2(Screen::UI_RIGHT, Screen::UI_HEIGHT),
		DirectX::SimpleMath::Vector2(.5f, .5f),
		KumachiLib::ANCHOR::BOTTOM_RIGHT);
	// ローディングテキスト
	Add(m_loadgingText, L"Resources/Textures/Loading_Text.png",
		DirectX::SimpleMath::Vector2(Screen::UI_RIGHT - 70, Screen::UI_HEIGHT),
		DirectX::SimpleMath::Vector2(.5f, .5f),
		KumachiLib::ANCHOR::BOTTOM_RIGHT);
	// ローディング画像のアニメーション設定
	m_loading->SetFrameRows(m_frameRows);
	m_loading->SetFrameCols(m_frameCols);
}

void WifiLoading::Update(float elapsedTime)
{
	m_animTime += elapsedTime * m_animSpeed;// 経過時間を加算
	if (m_animTime > 1.25f)// 1秒経過
	{
		m_animTime = 0.0f;// タイマーをリセット
	}

	// 0.25 刻みに調整
	float stepRatio = floor(m_animTime * m_frameCols) / m_frameCols;
	m_loading->SetRenderRatio(stepRatio);
}

void WifiLoading::Render()
{
	m_loading->SetShaderType(PlayerUI::ShaderType::ANIM);
	m_loading->Render();
	m_loadgingText->SetShaderType(PlayerUI::ShaderType::OTHER);
	m_loadgingText->Render();
}

void WifiLoading::Add(std::unique_ptr<PlayerUI>& pPlayerUI, const wchar_t* path, DirectX::SimpleMath::Vector2 position, DirectX::SimpleMath::Vector2 scale, KumachiLib::ANCHOR anchor)
{
	pPlayerUI = std::make_unique<PlayerUI>();
	pPlayerUI->Create(m_pDR
		, path
		, position
		, scale
		, anchor);
	pPlayerUI->SetWindowSize(m_windowWidth, m_windowHeight);
}
