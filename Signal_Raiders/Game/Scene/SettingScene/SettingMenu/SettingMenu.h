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
	enum SettingID// 設定項目 
	{
		BGM = 0,// BGM
		SE = 1,// SE
		MOUSE = 2// マウス感度

	};
	enum SelectID// 選択項目
	{
		NONE = -1,// 選択なし
		APPLY = 0,// 適用
		END = 1// 何もせず終了
	};
public:// アクセサ
	SelectID GetSelectIDNum() const { return m_selectNum; }// 選択ID取得
	void SetSelectIDNum(SelectID num) { m_selectNum = num; }// 選択ID設定
	unsigned int GetMenuIndex() const { return m_menuIndex; }// メニューインデックス取得	
	void SetSEVolume(float volume) { m_SEVolume = volume; }// SEの音量設定

public:// public関数
	SettingMenu();// コンストラクタ
	~SettingMenu();// デストラクタ
	void Initialize(CommonResources* resources, int width, int height)override;// 初期化
	void Update(const UpdateContext& context)override { Update(context.elapsedTime); }// 更新(contextから時間を取得して更新)
	void Render()override;// 描画
	void Add(const std::string& key// メニューアイテムを追加
		, const DirectX::SimpleMath::Vector2& position
		, const DirectX::SimpleMath::Vector2& scale
		, KumachiLib::ANCHOR anchor
		, UIType type);
private:// private関数
	void Update(float elapsedTime);// 更新
private:// private定数
	static const int INVALID_MENU_INDEX;// 無効なメニューインデックス
private://	変数
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
	// 選択テクスチャパス
	const wchar_t* m_pSelectTexturePath;
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