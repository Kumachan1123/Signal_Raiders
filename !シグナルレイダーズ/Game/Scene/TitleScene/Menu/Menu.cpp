#include "pch.h"
#include "Menu.h"
#include "Game/Screen.h"
#include "Game/Scene/TitleScene/TitleUI/TitleUI.h"
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

Menu::Menu()
	: m_menuIndex{ 0 }
	, m_pDR{ nullptr }
	, m_commonResources{ nullptr }
	, m_pUI{}
	, m_pSelect{}
	, m_pSelectTexturePath{ nullptr }
	, m_pSelectWindow{ nullptr }
	, m_windowWidth{ 0 }
	, m_windowHeight{ 0 }
	, m_tracker{}
	, m_num{ SceneID::PLAY }
{
}


Menu::~Menu()
{
}

void Menu::Initialize(CommonResources* resources, int width, int height)
{
	m_commonResources = resources;
	m_pDR = m_commonResources->GetDeviceResources();
	m_windowWidth = width;
	m_windowHeight = height;
	m_pSelectTexturePath = L"Resources/Textures/select.png";
	//  「プレイ」を読み込む
	Add(L"Resources/Textures/play.png"
		, SimpleMath::Vector2(Screen::CENTER_X, Screen::CENTER_Y + 280)
		, SimpleMath::Vector2(.5, .5)
		, kumachi::ANCHOR::MIDDLE_CENTER);
	//  「せってい」を読み込む
	Add(L"Resources/Textures/setting.png"
		, SimpleMath::Vector2(Screen::CENTER_X, Screen::CENTER_Y + 350)
		, SimpleMath::Vector2(.5, .5)
		, kumachi::ANCHOR::MIDDLE_CENTER);
	//  「おわる」を読み込む
	Add(L"Resources/Textures/end.png"
		, SimpleMath::Vector2(Screen::CENTER_X, Screen::CENTER_Y + 420)
		, SimpleMath::Vector2(.5, .5)
		, kumachi::ANCHOR::MIDDLE_CENTER);

}

void Menu::Update(float elapsedTime)
{
	const auto& kbTracker = m_commonResources->GetInputManager()->GetKeyboardTracker();
	m_time += elapsedTime;
	//  キーボードの入力を取得
	if (kbTracker->pressed.S)
	{
		//  →キーを押したら、選択先を1つ進める
		m_menuIndex += 1;
		//  メニューアイテム数の最大値を超えないように制御
		m_menuIndex %= m_pUI.size();
	}
	if (kbTracker->pressed.W)
	{
		//  ←キーを押したら、選択先を1つ戻す
		m_menuIndex += static_cast<unsigned int>(m_pUI.size()) - 1;
		//  メニューアイテム数の最大値を超えないように制御
		m_menuIndex %= m_pUI.size();
	}
	if (kbTracker->pressed.Space)
	{
		m_num = static_cast<SceneID>(m_menuIndex);
	}
	//  メニューアイテムの選択先を更新
	for (int i = 0; i < m_pUI.size(); i++)
	{
		//  アイテムの選択状態を更新
		m_pSelect[i]->SetScale(m_pSelect[i]->GetSelectScale());
		m_pSelect[i]->SetTime(m_pSelect[i]->GetTime() + elapsedTime);
		m_pUI[i]->SetScale(m_pUI[i]->GetSelectScale());
		m_pUI[i]->SetTime(m_pUI[i]->GetTime() + elapsedTime);
	}
	// 選択中の初期サイズを取得する
	Vector2 select = m_pUI[m_menuIndex]->GetSelectScale();
	//  選択状態とするための変化用サイズを算出する
	SimpleMath::Vector2 selectScale = SimpleMath::Vector2::Lerp(m_pUI[m_menuIndex]->GetSelectScale(), SimpleMath::Vector2::One, 1);
	//  選択状態は初期状態＋50％の大きさとする
	select = SimpleMath::Vector2((sin(m_time) * 0.1f) + 1.0f);
	//  算出後のサイズを現在のサイズとして設定する
	m_pUI[m_menuIndex]->SetScale(select);
	//  背景用のウィンドウ画像にも同じ割合の値を設定する
	m_pSelect[m_menuIndex]->SetScale(Vector2::One);
}

void Menu::Render()
{
	for (unsigned int i = 0; i < m_pUI.size(); i++)
	{
		//  アイテム用ウィンドウ背景を表示
		if (i == m_menuIndex) m_pSelect[i]->Render();
		//  実際に表示したいアイテム画像を表示
		m_pUI[i]->Render();
	}
}

void Menu::Add(const wchar_t* path, DirectX::SimpleMath::Vector2 position, DirectX::SimpleMath::Vector2 scale, kumachi::ANCHOR anchor)
{
	//  メニューとしてアイテムを追加する
	std::unique_ptr<TitleUI> userInterface = std::make_unique<TitleUI>();
	//  指定された画像を表示するためのアイテムを作成する
	userInterface->Create(m_pDR
						  , path
						  , position
						  , scale
						  , anchor);
	userInterface->SetWindowSize(m_windowWidth, m_windowHeight);

	//  アイテムを新しく追加
	m_pUI.push_back(std::move(userInterface));

	//  背景用のウィンドウ画像も追加する
	std::unique_ptr<TitleUI> base = std::make_unique<TitleUI>();
	base->Create(m_pDR
				 , m_pSelectTexturePath
				 , position
				 , scale
				 , anchor);
	base->SetWindowSize(m_windowWidth, m_windowHeight);

	//  背景用のアイテムも新しく追加する
	m_pSelect.push_back(std::move(base));

}
