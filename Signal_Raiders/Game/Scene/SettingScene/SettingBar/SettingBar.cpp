/*
	@file	SettingBar.cpp
	@brief	設定バークラス
*/
#include "pch.h"
#include "SettingBar.h"
#include "Game/Screen.h"
#include "Game/KumachiLib/BinaryFile/BinaryFile.h"
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

SettingBar::SettingBar()
	: m_commonResources{}
	, m_pDR{}
	, m_pSettingMenu{   }
	, m_pBar{}
	, m_pSettingBarTexturePath{}
	, m_pSettingBarPointerTexturePath{}
	, m_windowWidth{ 0 }
	, m_windowHeight{ 0 }
	, m_tracker{}
	, m_num{ SettingMenu::StateID::BGM }
	, m_selectNum{ SettingMenu::SelectID::NONE }
	, m_setting{ 4,4,5 }
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
	m_pSettingData = std::make_unique<SettingData>();

	//  「BGM」の場所に配置
	Add(m_pSettingBarTexturePath
		, SimpleMath::Vector2(Screen::CENTER_X + 300, Screen::CENTER_Y - 300)
		, SimpleMath::Vector2(1, 1)
		, KumachiLib::ANCHOR::MIDDLE_CENTER
		, UIType::NON_SELECT);
	//  「SE」の場所に配置
	Add(m_pSettingBarTexturePath
		, SimpleMath::Vector2(Screen::CENTER_X + 300, Screen::CENTER_Y - 150)
		, SimpleMath::Vector2(1, 1)
		, KumachiLib::ANCHOR::MIDDLE_CENTER
		, UIType::NON_SELECT);
	//  「マウスかんど」の場所に配置
	Add(m_pSettingBarTexturePath
		, SimpleMath::Vector2(Screen::CENTER_X + 300, Screen::CENTER_Y)
		, SimpleMath::Vector2(1, 1)
		, KumachiLib::ANCHOR::MIDDLE_CENTER
		, UIType::NON_SELECT);

	// Jsonファイルから設定を読み込む
	m_pSettingData->Load();
	m_setting[0] = m_pSettingData->GetBGMVolume();
	m_setting[1] = m_pSettingData->GetSEVolume();
	m_setting[2] = m_pSettingData->GetMouseSensitivity();

	//  設定バーの玉を読み込んだ設定に合わせて移動
	for (unsigned int i = 0; i < m_pBarPointer.size(); i++)
	{
		// 設定バーの玉を読み込んだ設定に合わせて移動
		m_pBarPointer[i]->SetPosition(Vector2(static_cast<float>(Screen::CENTER_X + 60 + m_setting[i] * 60 - 60), m_pBarPointer[i]->GetPosition().y));
	}
}

void SettingBar::Update(float elapsedTime)
{
	// マウスの状態を取得
	auto& mouseState = m_commonResources->GetInputManager()->GetMouseState();
	// マウスの座標を取得
	Vector2 mousePos = Vector2(static_cast<float>(mouseState.x), static_cast<float>(mouseState.y));
	// マウスボタン状態を取得
	auto& mouseTracker = m_commonResources->GetInputManager()->GetMouseTracker();
	bool isDragging = false;// ドラッグ状態を保持
	int dragIndex = -1;// ドラッグ中のアイテムのインデックス
	m_time += elapsedTime;
	for (unsigned int i = 0; i < m_pBarPointer.size(); i++)
	{
		// ポインターの当たり判定チェック（円形）

		if (m_pBarPointer[i]->IsHit(mousePos))
		{
			if (mouseTracker->GetLastState().leftButton) isDragging = true;
			dragIndex = i;
			break;
		}

	}
	// ドラッグ中ならマウス座標に追従
	if (isDragging && dragIndex != -1)
	{
		float newX = mousePos.x;

		// 範囲制限（左端～右端）
		float minX = Screen::CENTER_X + 300 - 300;
		float maxX = Screen::CENTER_X + 300 + 300;
		newX = std::max(minX, std::min(maxX, newX));

		// ポインターの位置更新
		m_pBarPointer[dragIndex]->SetPosition(Vector2(newX, m_pBarPointer[dragIndex]->GetPosition().y));

		// 設定値の更新（0～5に変換）
		m_setting[dragIndex] = static_cast<int>((newX - minX) / 60);
	}

	//  「へんこう」が選ばれている場合,設定を保存する
	if (m_pSettingMenu->GetSelectIDNum() == SettingMenu::APPLY)
	{
		if (mouseTracker->GetLastState().leftButton)m_pSettingData->Save(m_setting[0], m_setting[1], m_setting[2]);
	}
#ifdef _DEBUG
	// デバッグ情報を表示する
	auto debugString = m_commonResources->GetDebugString();
	auto& mousestate = m_commonResources->GetInputManager()->GetMouseState();
	// ウィンドウ上のマウス座標を取得する
	Vector2 pos = Vector2(static_cast<float>(mousestate.x), static_cast<float>(mousestate.y));

	for (unsigned int i = 0; i < m_pBarPointer.size(); i++)
		debugString->AddString("SelectNum:%i Setting:%i", m_pBarPointer[i]->IsHit(mousePos), m_setting[i]);

	debugString->AddString("DragIndex:%i ", dragIndex);
	debugString->AddString("MousePos:%f %f", pos.x, pos.y);
	debugString->AddString("ClickLeft:%i", mouseTracker->GetLastState().leftButton);

#endif

}

void SettingBar::Render()
{

	for (unsigned int i = 0; i < m_pBarPointer.size(); i++)
	{
		//  実際に表示したいアイテム画像を表示
		m_pBar[i]->Render();
		//  設定バーの玉を表示
		m_pBarPointer[i]->Render();
	}



}

void SettingBar::Add(const wchar_t* path
	, DirectX::SimpleMath::Vector2 position
	, DirectX::SimpleMath::Vector2 scale
	, KumachiLib::ANCHOR anchor
	, UIType type)
{
	UNREFERENCED_PARAMETER(type);// 未使用警告非表示
	//  メニューとしてアイテムを追加する
	std::unique_ptr<UI> userInterface = std::make_unique<UI>();
	//  指定された画像を表示するためのアイテムを作成する
	userInterface->Create(m_pDR
		, path
		, position
		, Vector2(scale.x / 1.45f, scale.y / 2.0f)
		, anchor);
	userInterface->SetWindowSize(m_windowWidth, m_windowHeight);

	//  アイテムを新しく追加
	m_pBar.push_back(std::move(userInterface));


	//  背景用のウィンドウ画像も追加する
	std::unique_ptr<UI> base = std::make_unique<UI>();
	base->Create(m_pDR
		, m_pSettingBarPointerTexturePath
		, position
		, scale
		, anchor);
	base->SetWindowSize(m_windowWidth, m_windowHeight);

	//  背景用のアイテムも新しく追加する
	m_pBarPointer.push_back(std::move(base));

}