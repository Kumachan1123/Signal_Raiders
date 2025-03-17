/*
	@file	StageSelectMenu.cpp
	@brief	メニュークラス
*/
#include "pch.h"
#include "StageSelectMenu.h"
#include "Game/Screen.h"
#include "Game/UI/UI.h"
#include "Game/KumachiLib/BinaryFile/BinaryFile.h"
#include "DeviceResources.h"
#include <SimpleMath.h>
#include <Effects.h>
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <WICTextureLoader.h>
#include <CommonStates.h>
#include <vector>
#include <Mouse.h>
#include "Libraries/MyLib/InputManager.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

StageSelectMenu::StageSelectMenu()
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


StageSelectMenu::~StageSelectMenu()
{
}

void StageSelectMenu::Initialize(CommonResources* resources, int width, int height)
{
	m_commonResources = resources;
	m_pDR = m_commonResources->GetDeviceResources();
	m_windowWidth = width;
	m_windowHeight = height;
	m_pSelectTexturePath = L"Resources/Textures/StageSelect.png";
	//「ステージ1の写真」を読み込む
	Add(L"Resources/Textures/stage1.png"
		, SimpleMath::Vector2(Screen::CENTER_X - 550, Screen::CENTER_Y - 70)
		, SimpleMath::Vector2(.7, .7)
		, KumachiLib::ANCHOR::MIDDLE_CENTER
		, UIType::SELECT);
	//「ステージ2の写真」を読み込む
	Add(L"Resources/Textures/stage2.png"
		, SimpleMath::Vector2(Screen::CENTER_X, Screen::CENTER_Y - 70)
		, SimpleMath::Vector2(.7, .7)
		, KumachiLib::ANCHOR::MIDDLE_CENTER
		, UIType::SELECT);
	//「ステージ3の写真」を読み込む
	Add(L"Resources/Textures/stage3.png"
		, SimpleMath::Vector2(Screen::CENTER_X + 550, Screen::CENTER_Y - 70)
		, SimpleMath::Vector2(.7, .7)
		, KumachiLib::ANCHOR::MIDDLE_CENTER
		, UIType::SELECT);
	//「ステージ4の写真」を読み込む
	Add(L"Resources/Textures/stage4.png"
		, SimpleMath::Vector2(Screen::CENTER_X - 550, Screen::CENTER_Y + 250)
		, SimpleMath::Vector2(.7, .7)
		, KumachiLib::ANCHOR::MIDDLE_CENTER
		, UIType::SELECT);
	//「ステージ5の写真」を読み込む
	Add(L"Resources/Textures/stage5.png"
		, SimpleMath::Vector2(Screen::CENTER_X, Screen::CENTER_Y + 250)
		, SimpleMath::Vector2(.7, .7)
		, KumachiLib::ANCHOR::MIDDLE_CENTER
		, UIType::SELECT);
	//「タイトルに戻る」を読み込む
	Add(L"Resources/Textures/ToTitle.png"
		, SimpleMath::Vector2(Screen::CENTER_X + 550, Screen::CENTER_Y + 250)
		, SimpleMath::Vector2(.7, .7)
		, KumachiLib::ANCHOR::MIDDLE_CENTER
		, UIType::SELECT);
	//「操作説明」を読み込む
	Add(L"Resources/Textures/Guide.png"
		, SimpleMath::Vector2(Screen::RIGHT, Screen::BOTTOM)
		, SimpleMath::Vector2(1, 1)
		, KumachiLib::ANCHOR::BOTTOM_RIGHT
		, UIType::NON_SELECT);


}

void StageSelectMenu::Update(float elapsedTime)
{
	const auto& kbTracker = m_commonResources->GetInputManager()->GetKeyboardTracker();
	// マウスのトラッカーを取得する
	auto& mtracker = m_commonResources->GetInputManager()->GetMouseTracker();
	// マウスの状態を取得
	auto& mouseState = m_commonResources->GetInputManager()->GetMouseState();

	// マウスの座標を取得
	Vector2 mousePos = Vector2(static_cast<float>(mouseState.x), static_cast<float>(mouseState.y));
	// メニューアイテムの数だけ繰り返す
	for (int i = 0; i < m_pUI.size(); i++)
	{
		// マウスの座標がアイテムの範囲内にあるかどうかを判定
		if (m_pUI[i]->IsHit(mousePos))
		{
			// 範囲内にある場合は、選択中のアイテムを更新
			m_menuIndex = i;
		}
	}
	m_time += elapsedTime;
	////  キーボードの入力を取得
	//if (kbTracker->pressed.D)
	//{
	//	//  →キーを押したら、選択先を1つ進める
	//	m_menuIndex += static_cast<unsigned int>(m_pUI.size()) + 1;
	//	//  メニューアイテム数の最大値を超えないように制御
	//	m_menuIndex %= m_pUI.size();
	//}
	//if (kbTracker->pressed.A)
	//{
	//	//  ←キーを押したら、選択先を1つ戻す
	//	m_menuIndex += static_cast<unsigned int>(m_pUI.size()) - 1;
	//	m_menuIndex %= m_pUI.size();
	//}
	//if (kbTracker->pressed.W)
	//{
	//	//  ↑キーを押したら、選択先を3つ戻す
	//	m_menuIndex += static_cast<unsigned int>(m_pUI.size()) - 3;
	//	//  メニューアイテム数の最大値を超えないように制御
	//	m_menuIndex %= m_pUI.size();
	//}
	if (kbTracker->pressed.S)
	{
		// ↓キーを押したら、選択先を3つ進める
		m_menuIndex += static_cast<unsigned int>(m_pUI.size()) + 3;
		// メニューアイテム数の最大値を超えないように制御
		m_menuIndex %= m_pUI.size();
	}

	if (kbTracker->pressed.Space || mtracker->GetLastState().leftButton)
	{
		m_num = static_cast<SceneID>(m_menuIndex);
	}
	// メニューアイテムの選択先を更新
	for (int i = 0; i < m_pUI.size(); i++)
	{
		//  アイテムの選択状態を更新
		m_pSelect[i]->SetScale(m_pSelect[i]->GetSelectScale());
		m_pSelect[i]->SetTime(m_pSelect[i]->GetTime() + elapsedTime);
		m_pUI[i]->SetScale(m_pUI[i]->GetSelectScale());
		m_pUI[i]->SetTime(m_pUI[i]->GetTime() + elapsedTime);
	}
	// 選択不可能なアイテムの選択状態を更新
	for (int i = 0; i < m_pGuide.size(); i++)
	{
		m_pGuide[i]->SetScale(m_pGuide[i]->GetSelectScale());
		m_pGuide[i]->SetTime(m_pGuide[i]->GetTime() + elapsedTime);
	}
	// 選択中の初期サイズを取得する
	Vector2 select = m_pUI[m_menuIndex]->GetSelectScale();
	// 選択状態とするための変化用サイズを算出する
	SimpleMath::Vector2 selectScale = SimpleMath::Vector2::Lerp(m_pUI[m_menuIndex]->GetSelectScale(), SimpleMath::Vector2::One, 1);
	// 選択状態は初期状態より大きくする
	select = SimpleMath::Vector2((sin(m_time) * 0.1f) + 1.0f);
	// 算出後のサイズを現在のサイズとして設定する
	m_pUI[m_menuIndex]->SetScale(select);
	// 背景用のウィンドウ画像にも同じ割合の値を設定する
	m_pSelect[m_menuIndex]->SetScale(select);

}

void StageSelectMenu::Render()
{
	for (unsigned int i = 0; i < m_pUI.size(); i++)
	{
		// シェーダータイプを設定
		m_pUI[i]->SetShaderType(UI::ShaderType::STAGE_SELECT);

		//  実際に表示したいアイテム画像を表示
		m_pUI[i]->Render();
	}

	// 選択不可能なアイテムの画像を表示
	for (unsigned int i = 0; i < m_pGuide.size(); i++)
	{
		// シェーダータイプを設定
		m_pGuide[i]->SetShaderType(UI::ShaderType::NORMAL);

		m_pGuide[i]->Render();
	}
}

void StageSelectMenu::Add(const wchar_t* path, DirectX::SimpleMath::Vector2 position, DirectX::SimpleMath::Vector2 scale, KumachiLib::ANCHOR anchor, UIType type)
{
	//  メニューとしてアイテムを追加する
	std::unique_ptr<UI> userInterface = std::make_unique<UI>();
	//  指定された画像を表示するためのアイテムを作成する
	userInterface->Create(m_pDR
		, path
		, position
		, scale
		, anchor);
	userInterface->SetWindowSize(m_windowWidth, m_windowHeight);

	//  アイテムを新しく追加
	if (type == UIType::SELECT)	m_pUI.push_back(std::move(userInterface));
	else
	{
		m_pGuide.push_back(std::move(userInterface));
		return;
	}
	//  背景用のウィンドウ画像も追加する
	std::unique_ptr<UI> base = std::make_unique<UI>();
	base->Create(m_pDR
		, m_pSelectTexturePath
		, position
		, scale
		, anchor);
	base->SetWindowSize(m_windowWidth, m_windowHeight);

	//  背景用のアイテムも新しく追加する
	m_pSelect.push_back(std::move(base));

}
