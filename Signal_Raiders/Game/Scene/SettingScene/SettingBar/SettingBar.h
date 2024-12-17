/*
	@file	SettingBar.h
	@brief	設定バークラス
*/
#pragma once
#include "Game/Scene/SettingScene/SettingMenu/SettingMenu.h"
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
#include "Game/Scene/SettingScene/SettingData/SettingData.h"

class CommonResources;
class SettingBar
{
public:

	//	変数
private:
	SettingMenu* m_pSettingMenu;// セッティングメニュー
	DX::DeviceResources* m_pDR;// デバイスリソース
	CommonResources* m_commonResources;// 共通リソース
	std::vector<std::unique_ptr<UI>> m_pBar;// 設定バー
	std::vector<std::unique_ptr<UI>> m_pBarPointer;// 設定バーの玉
	const wchar_t* m_pSettingBarTexturePath;// 設定バーテクスチャパス
	const wchar_t* m_pSettingBarPointerTexturePath;// 設定バーの玉テクスチャパス
	int m_windowWidth, m_windowHeight;// ウィンドウの幅と高さ
	DirectX::Keyboard::KeyboardStateTracker m_tracker;// トラッカー
	std::unique_ptr<SettingData> m_pSettingData;// 設定データ

	SettingMenu::StateID m_num;// 状態ID
	unsigned int m_setting[3] = { 0, 0, 0 };// 設定
	float m_time = 0;// 時間
	//	関数
public:
	SettingBar(SettingMenu* pSettingMenu);
	~SettingBar();
	void Initialize(CommonResources* resources, int width, int height);
	void Update(float elapsedTime);
	void Render();
	void Add(const wchar_t* path
		, DirectX::SimpleMath::Vector2 position
		, DirectX::SimpleMath::Vector2 scale
		, KumachiLib::ANCHOR anchor);
	void SetStateIDNum(SettingMenu::StateID num) { m_num = num; }
	int GetSetting(int index) const { return m_setting[index]; }
};