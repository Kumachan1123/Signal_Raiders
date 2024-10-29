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
		MOUSE = 2,
		APPLY = 3,
		END = 4
	};
	//	変数
private:

	unsigned int m_menuIndex;// メニューのインデックス
	DX::DeviceResources* m_pDR;// デバイスリソース
	CommonResources* m_commonResources;// 共通リソース
	std::vector<std::unique_ptr<UI>> m_pUI;// UI
	std::vector<std::unique_ptr<UI>> m_pSelect;// 選択

	const wchar_t* m_pSelectTexturePath;// 選択テクスチャパス

	std::unique_ptr<UI> m_pSelectWindow;// 選択ウィンドウ

	int m_windowWidth, m_windowHeight;// ウィンドウの幅と高さ

	DirectX::Keyboard::KeyboardStateTracker m_tracker;// トラッカー


	StateID m_num;// 状態ID
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
		, kumachi::ANCHOR anchor);

public:
	StateID GetStateIDNum() const { return m_num; }
	void SetStateIDNum(StateID num) { m_num = num; }
	unsigned int GetMenuIndex() const { return m_menuIndex; }
};