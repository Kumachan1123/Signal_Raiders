/*
	@file	SettingBar.cpp
	@brief	設定バークラス
*/
#include "pch.h"
#include "SettingBar.h"
#include "Game/Screen.h"
#include "Game/KumachiLib/BinaryFile.h"
#include "DeviceResources.h"
#include <SimpleMath.h>
#include <Effects.h>
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <WICTextureLoader.h>
#include <CommonStates.h>
#include <vector>
#include "Libraries/MyLib/InputManager.h"
using namespace DirectX;
using namespace DirectX::SimpleMath;

SettingBar::SettingBar(SettingMenu* pSettingMenu)
	: m_commonResources{}
	, m_pDR{}
	, m_pSettingMenu{ pSettingMenu }
	, m_pBar{}
	, m_pSettingBarTexturePath{}
	, m_pSettingBarPointerTexturePath{}
	, m_windowWidth{ 0 }
	, m_windowHeight{ 0 }
	, m_tracker{}
	, m_num{ SettingMenu::StateID::BGM }
{
}

SettingBar::~SettingBar()
{
}

void SettingBar::Initialize(CommonResources* resources, int width, int height)
{
	m_commonResources = resources;
	m_pDR = m_commonResources->GetDeviceResources();
	m_windowWidth = width;
	m_windowHeight = height;
	m_pSettingBarTexturePath = L"Resources/Textures/SettingBar.png";
	m_pSettingBarPointerTexturePath = L"Resources/Textures/SettingBarPointer.png";
	//  「BGM」の場所に配置
	Add(m_pSettingBarTexturePath
		, SimpleMath::Vector2(Screen::CENTER_X + 250, Screen::CENTER_Y - 300)
		, SimpleMath::Vector2(1, 1)
		, kumachi::ANCHOR::MIDDLE_CENTER);
	//  「SE」の場所に配置
	Add(m_pSettingBarTexturePath
		, SimpleMath::Vector2(Screen::CENTER_X + 250, Screen::CENTER_Y - 150)
		, SimpleMath::Vector2(1, 1)
		, kumachi::ANCHOR::MIDDLE_CENTER);
	//  「マウスかんど」の場所に配置
	Add(m_pSettingBarTexturePath
		, SimpleMath::Vector2(Screen::CENTER_X + 250, Screen::CENTER_Y)
		, SimpleMath::Vector2(1, 1)
		, kumachi::ANCHOR::MIDDLE_CENTER);

}

void SettingBar::Update(float elapsedTime)
{
	const auto& kbTracker = m_commonResources->GetInputManager()->GetKeyboardTracker();
	m_time += elapsedTime;
	if (m_num < 3)//  セッティングシーンで「へんこう」と「おわり」以外が選ばれている場合
	{
		//  キーボードの入力を取得
		if (kbTracker->pressed.A)
		{
			// ポインターの座標を左に移動
			// 左に移動できるのは中心にいるときから５回まで
			if (m_pBarPointer[m_num]->GetPosition().x > Screen::CENTER_X + 250 - 300)
			{
				m_pBarPointer[m_num]->SetPosition(Vector2(m_pBarPointer[m_num]->GetPosition().x - 60, m_pBarPointer[m_num]->GetPosition().y));
			}
		}
		if (kbTracker->pressed.D)
		{
			// ポインターの座標を右に移動
			// 右に移動できるのは中心にいるときから５回まで
			if (m_pBarPointer[m_num]->GetPosition().x < Screen::CENTER_X + 250 + 300)
			{
				m_pBarPointer[m_num]->SetPosition(Vector2(m_pBarPointer[m_num]->GetPosition().x + 60, m_pBarPointer[m_num]->GetPosition().y));
			}
		}
	}

}

void SettingBar::Render()
{

	for (unsigned int i = 0; i < m_pBarPointer.size(); i++)
	{
		//  アイテム用ウィンドウ背景を表示
		//if (i == m_menuIndex) m_pSelect[i]->Render();
		//  実際に表示したいアイテム画像を表示
		m_pBar[i]->Render();
		//  設定バーの玉を表示
		m_pBarPointer[i]->Render();
	}
}

void SettingBar::Add(const wchar_t* path
					 , DirectX::SimpleMath::Vector2 position
					 , DirectX::SimpleMath::Vector2 scale
					 , kumachi::ANCHOR anchor)
{
	//  メニューとしてアイテムを追加する
	std::unique_ptr<SettingUI> userInterface = std::make_unique<SettingUI>();
	//  指定された画像を表示するためのアイテムを作成する
	userInterface->Create(m_pDR
						  , path
						  , position
						  , Vector2(scale.x / 1.5f, scale.y / 2.0f)
						  , anchor);
	userInterface->SetWindowSize(m_windowWidth, m_windowHeight);

	//  アイテムを新しく追加
	m_pBar.push_back(std::move(userInterface));


	//  背景用のウィンドウ画像も追加する
	std::unique_ptr<SettingUI> base = std::make_unique<SettingUI>();
	base->Create(m_pDR
				 , m_pSettingBarPointerTexturePath
				 , position
				 , scale
				 , anchor);
	base->SetWindowSize(m_windowWidth, m_windowHeight);

	//  背景用のアイテムも新しく追加する
	m_pBarPointer.push_back(std::move(base));

}