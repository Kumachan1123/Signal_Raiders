/*
*	@file	SettingMenu.h
*	@brief	セッティングメニュークラス
*/
#pragma once
// 標準ライブラリ
#include <vector>
#include <string>
// DirectX
#include <Mouse.h>
#include <DeviceResources.h>
#include <SimpleMath.h>
#include <WICTextureLoader.h>
#include <CommonStates.h>
// 外部ライブラリ
#include <Libraries/MyLib/InputManager.h>
// 自作ヘッダーファイル
#include "Game/Screen.h"
#include "Game/UI/UI.h"
#include "Game/KumachiLib/BinaryFile/BinaryFile.h"
#include "Game/CommonResources.h"
#include "Game/Interface/IMenuUI.h"
// 前方宣言
class CommonResources;
class SettingMenu : public IMenuUI
{
public:// 列挙型
	// 設定項目 
	enum SettingID
	{
		BGM = 0,// BGM
		SE = 1,// SE
		MOUSE = 2// マウス感度

	};
	// 選択項目
	enum SelectID
	{
		NONE = -1,// 選択なし
		APPLY = 0,// 適用
		END = 1// 何もせず終了
	};
public:// アクセサ
	// 選択ID取得
	SelectID GetSelectIDNum() const { return m_selectNum; }
	// 選択ID設定
	void SetSelectIDNum(SelectID num) { m_selectNum = num; }
	// メニューインデックス取得	
	unsigned int GetMenuIndex() const { return m_menuIndex; }
	// SEの音量設定
	void SetSEVolume(float volume) { m_SEVolume = volume; }

public:// public関数
	// コンストラクタ
	SettingMenu();
	// デストラクタ
	~SettingMenu();
	// 初期化
	void Initialize(CommonResources* resources, int width, int height)override;
	// 更新(contextから時間を取得して更新)
	void Update(const UpdateContext& context)override { Update(context.elapsedTime); }
	// 描画
	void Render()override;
	// メニューアイテムを追加
	void Add(const std::string& key
		, const DirectX::SimpleMath::Vector2& position
		, const DirectX::SimpleMath::Vector2& scale
		, KumachiLib::ANCHOR anchor
		, UIType type);
private:// private関数
	// 更新
	void Update(float elapsedTime);
private:// private定数
	// 無効なメニューインデックス
	static const int INVALID_MENU_INDEX;
private:// 変数
	// メニューのインデックス
	int m_menuIndex;
	// デバイスリソース
	DX::DeviceResources* m_pDR;
	// 共通リソース
	CommonResources* m_pCommonResources;
	// 選択可能UI
	std::vector<std::unique_ptr<UI>> m_pUI;
	// 選択不可能UI
	std::vector<std::unique_ptr<UI>> m_pGuide;
	// 選択可能UIにつくUI
	std::vector<std::unique_ptr<UI>> m_pSelect;
	// 選択ウィンドウ
	std::unique_ptr<UI> m_pSelectWindow;
	// ウィンドウの幅と高さ
	int m_windowWidth, m_windowHeight;
	// 選択ID
	SelectID m_selectNum;
	// 時間
	float m_time;
	// SEの音量
	float m_SEVolume;
	// 再生フラグ
	bool m_isSEPlay;
};