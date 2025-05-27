/*
*	@file	SettingBar.h
*	@brief	設定バークラス（音量や感度などのスライダー）
*/
#pragma once
// 標準ライブラリ
#include <vector>
// DirectX
#include <SimpleMath.h>
#include <WICTextureLoader.h>
#include <CommonStates.h>
#include <DeviceResources.h>
// 外部ライブラリ
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/InputManager.h"
// 自作ヘッダーファイル
#include "Game/CommonResources.h"
#include "Game/Scene/SettingScene/SettingData/SettingData.h"
#include "Game/Scene/SettingScene/SettingMenu/SettingMenu.h"
#include "Game/Interface/IMenuUI.h"
#include "Game/Screen.h"
#include "Game/KumachiLib/BinaryFile/BinaryFile.h"
// 前方宣言
class CommonResources;
class SettingBar : public IMenuUI
{
public:// アクセサ
	SettingMenu* GetSettingMenu() { return m_pSettingMenu; }// 設定メニュー取得
	void SetSettingMenu(SettingMenu* menu) { m_pSettingMenu = menu; }// 設定メニュー設定
	void SetStateIDNum(SettingMenu::SettingID num) { m_num = num; }// 状態ID設定
	int GetSetting(int index) const { return m_setting[index]; }// 設定取得
public:// public関数
	SettingBar();// コンストラクタ
	~SettingBar();// デストラクタ
	void Initialize(CommonResources* resources, int width, int height)override;// 初期化
	void Update(const UpdateContext& context)override { Update(context.elapsedTime); }// 更新(contextから時間を取得して更新)
	void Render()override;// 描画
	void Add(const wchar_t* path// メニューアイテムを追加
		, DirectX::SimpleMath::Vector2 position
		, DirectX::SimpleMath::Vector2 scale
		, KumachiLib::ANCHOR anchor
		, UIType type)override;
private:// private関数
	void Update(float elapsedTime);// 更新
private:// private定数
	static const DirectX::SimpleMath::Vector2 BGM_POSITION;	// BGMの位置
	static const DirectX::SimpleMath::Vector2 SE_POSITION;	// SEの位置
	static const DirectX::SimpleMath::Vector2 SENSITIVITY_POSITION;	// 感度の位置
	static const float BAR_WIDTH;	// バーの幅
	static const float BAR_POINTER_DIVISION;// 玉の位置に除算する値
private:// private変数
	// セッティングメニュー
	SettingMenu* m_pSettingMenu;
	// デバイスリソース
	DX::DeviceResources* m_pDR;
	// 共通リソース
	CommonResources* m_pCommonResources;
	// 設定バー
	std::vector<std::unique_ptr<UI>> m_pBar;
	// 設定バーの玉
	std::vector<std::unique_ptr<UI>> m_pBarPointer;
	// 設定バーテクスチャパス
	const wchar_t* m_pSettingBarTexturePath;
	// 設定バーの玉テクスチャパス
	const wchar_t* m_pSettingBarPointerTexturePath;
	// ウィンドウの幅と高さ
	int m_windowWidth, m_windowHeight;
	// 設定データ
	std::unique_ptr<SettingData> m_pSettingData;
	// 選択番号
	SettingMenu::SelectID m_selectNum;
	// 状態ID
	SettingMenu::SettingID m_num;
	// 設定データ
	unsigned int m_setting[3];
	// 時間
	float m_time = 0;


};