/*
	@file	SettingMenu.h
	@brief	セッティングメニュークラス
*/
#pragma once

#include "StepTimer.h"
#include "Game/UI/UI.h"
#include <DeviceResources.h>
#include "Game/CommonResources.h"
#include <SimpleMath.h>
#include <Effects.h>
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <WICTextureLoader.h>
#include <CommonStates.h>
#include <vector>
#include "Keyboard.h"
class CommonResources;
class SettingMenu
{
public:
	enum StateID
	{
		BGM = 0,
		SE = 1,
		MOUSE = 2

	};
	enum SelectID
	{
		NONE = -1,
		APPLY = 0,
		END = 1
	};
	enum class UIType
	{
		// 選択可能UI
		SELECT,
		// 選択不可能UI
		NON_SELECT
	};
	//	変数
private:

	int m_menuIndex;// メニューのインデックス
	DX::DeviceResources* m_pDR;// デバイスリソース
	CommonResources* m_commonResources;// 共通リソース
	std::vector<std::unique_ptr<UI>> m_pUI;// UI
	std::vector<std::unique_ptr<UI>> m_pGuide;// 選択不可能UI
	std::vector<std::unique_ptr<UI>> m_pSelect;// 選択

	const wchar_t* m_pSelectTexturePath;// 選択テクスチャパス

	std::unique_ptr<UI> m_pSelectWindow;// 選択ウィンドウ

	int m_windowWidth, m_windowHeight;// ウィンドウの幅と高さ

	DirectX::Keyboard::KeyboardStateTracker m_tracker;// トラッカー


	SelectID m_selectNum;// 選択ID
	float m_time = 0;// 時間
	//	関数
public:
	SettingMenu();
	~SettingMenu();

	void Initialize(CommonResources* resources, int width, int height);
	void Update(float elapsedTime);
	void Render();

	void Add(const wchar_t* path
		, DirectX::SimpleMath::Vector2 position
		, DirectX::SimpleMath::Vector2 scale
		, KumachiLib::ANCHOR anchor
		, UIType type);

public:
	SelectID GetSelectIDNum() const { return m_selectNum; }
	void SetSelectIDNum(SelectID num) { m_selectNum = num; }
	unsigned int GetMenuIndex() const { return m_menuIndex; }
};