/*
*	@file	TitleMenu.h
*	@brief	メニュークラス
*/
#pragma once
// 標準ライブラリ
#include <vector>
#include <string>
// DirectX
#include <DeviceResources.h>
#include "Game/CommonResources.h"
#include <Mouse.h>
#include <WICTextureLoader.h>
// 外部ライブラリ
#include <Libraries/MyLib/DebugString.h>
#include <Libraries/MyLib/InputManager.h>
// 自作ヘッダーファイル
#include "Game/UI/UI.h"
#include "Game/Interface/IMenuUI.h"
#include "Game/Screen.h"
#include "Game/UI/UI.h"
#include "Game/KumachiLib/BinaryFile/BinaryFile.h"
// 前方宣言
class CommonResources;
class TitleMenu : public IMenuUI
{
public:
	struct UITransForm// UIの変形
	{
		DirectX::SimpleMath::Vector2 position;// 位置
		DirectX::SimpleMath::Vector2 scale;// スケール
	};
	enum SceneID// シーン番号
	{
		STAGESELECT = 0,// ステージ選択
		SETTING,// 設定画面
		END// ゲーム終了
	};
public:// アクセサ
	SceneID GetSceneNum() const { return m_num; }// シーン番号取得
	void SetSceneNum(SceneID num) { m_num = num; }// シーン番号設定
	bool GetIsHit()const { return m_hit; }// UIにヒットしたかどうか取得
	void SetIsHit(bool hit) { m_hit = hit; }// UIにヒットしたかどうか設定
	void SetSEVolume(float volume) { m_SEVolume = volume; }// SEの音量設定
public:// public関数
	TitleMenu();// コンストラクタ
	~TitleMenu();// デストラクタ
	void Initialize(CommonResources* resources, int width, int height);// 初期化
	void Update(const UpdateContext& context)override { Update(context.elapsedTime); }// 更新
	void Render();// 描画
	void Add(const std::string& key// UI追加
		, const DirectX::SimpleMath::Vector2& position
		, const DirectX::SimpleMath::Vector2& scale
		, KumachiLib::ANCHOR anchor
		, IMenuUI::UIType type);
private:// private関数
	void Update(float elapsedTime);// 更新
private:// private定数
	static const int INVALID_MENU_INDEX;// 無効なメニューインデックス
private:// private変数
	// メニューのインデックス
	unsigned int m_menuIndex;
	// デバイスリソース
	DX::DeviceResources* m_pDR;
	// 共通リソース
	CommonResources* m_pCommonResources;
	// 選択可能UI
	std::vector<std::unique_ptr<UI>> m_pUI;
	// 選択不可能UI
	std::vector<std::unique_ptr<UI>> m_pGuide;
	// 選択中のUIに表示する画像
	std::vector<std::unique_ptr<UI>> m_pSelect;
	// 選択中のUIに表示する画像のパス
	const wchar_t* m_pSelectTexturePath;
	// ウィンドウの幅・高さ 
	int m_windowWidth, m_windowHeight;
	// UIの座標などの情報を格納する配列
	std::vector<UITransForm> m_transforms;
	// シーン番号
	SceneID m_num;
	// 時間
	float m_time;
	// 当たったかどうか
	bool m_hit;
	// SEの音量
	float m_SEVolume;
	// 再生フラグ
	bool m_isSEPlay;
};