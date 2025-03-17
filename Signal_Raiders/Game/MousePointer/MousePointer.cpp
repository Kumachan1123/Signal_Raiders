#include <pch.h>
#include "MousePointer.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

MousePointer::MousePointer()
	: m_commonResources{}
	, m_menuIndex{}
	, m_pDR{}
	, m_pMousePointer{}
	, m_windowWidth{}
	, m_windowHeight{}
	, m_tracker{}
{
}

MousePointer::~MousePointer()
{
}

void MousePointer::Initialize(CommonResources* resources, int width, int height)
{
	m_commonResources = resources;
	m_pDR = m_commonResources->GetDeviceResources();
	m_pMousePointer = std::make_unique<UI>();
	m_windowWidth = width;
	m_windowHeight = height;
	//  「プレイ」を読み込む
	Add(L"Resources/Textures/MousePointer.png"
		, SimpleMath::Vector2(0, 0)
		, SimpleMath::Vector2(1, 1)
		, KumachiLib::ANCHOR::MIDDLE_CENTER);
}

void MousePointer::Update(float elapsedTime)
{
	elapsedTime;
	// マウスの状態を取得
	auto& mouseState = m_commonResources->GetInputManager()->GetMouseState();

	// マウスの座標を取得
	Vector2 mousePos = Vector2(static_cast<float>(mouseState.x), static_cast<float>(mouseState.y));
	m_pMousePointer->SetPosition(mousePos);
}

void MousePointer::Render()
{
	m_pMousePointer->Render();
}

void MousePointer::Add(const wchar_t* path, DirectX::SimpleMath::Vector2 position, DirectX::SimpleMath::Vector2 scale, KumachiLib::ANCHOR anchor)
{
	//  メニューとしてアイテムを追加する
	//  指定された画像を表示するためのアイテムを作成する
	m_pMousePointer->Create(m_pDR
		, path
		, position
		, scale
		, anchor);
	m_pMousePointer->SetWindowSize(m_windowWidth, m_windowHeight);


}
