/*
	@file	ResultMenu.h
	@brief	リザルトメニュークラス
*/
#pragma once
#ifndef RESULTMENU_DEFINED
#define RESULTMENU_DEFINED
// 標準ライブラリ
#include <vector>
#include <string>
// DirectX
#include <SimpleMath.h>
#include <WICTextureLoader.h>
#include <Mouse.h>
#include <DeviceResources.h>
// 外部ライブラリ
#include <Libraries/MyLib/InputManager.h>
// 自作ヘッダーファイル
#include "Game/CommonResources.h"
#include "Game/UI/UI.h"
#include "Game/Interface/IMenuUI.h"
#include "Game/Screen.h"
#include "Game/KumachiLib/BinaryFile/BinaryFile.h"

// 前方宣言
class CommonResources;

// リザルトメニュークラス
class ResultMenu : public IMenuUI
{
public:
	// 列挙型
	// シーンID
	enum SceneID
	{
		REPLAY = 0,// リプレイ
		SELECT_STAGE,// ステージ選択
	};
public:
	// アクセサ
	// シーン番号取得
	SceneID GetSceneNum() const { return m_num; }
	// シーン番号設定
	void SetSceneNum(SceneID num) { m_num = num; }
	// UIにヒットしたかどうか取得
	bool GetIsHit()const { return m_hit; }
	// UIにヒットしたかどうか設定
	void SetIsHit(bool hit) { m_hit = hit; }
	// SEの音量設定
	void SetSEVolume(float volume) { m_SEVolume = volume; }
public:
	// public関数
	// コンストラクタ
	ResultMenu();
	// デストラクタ
	~ResultMenu();
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
		, UIType type)override;
private:
	// private関数
	// 更新
	void Update(float elapsedTime);
private:
	// private定数
	// 無効なメニューインデックス
	static const int INVALID_MENU_INDEX;
private:
	// private変数
	// メニューアイテムの数
	unsigned int m_menuIndex;
	// デバイスリソース
	DX::DeviceResources* m_pDR;
	// 共通リソース
	CommonResources* m_pCommonResources;
	// 選択可能なUI
	std::vector<std::unique_ptr<UI>> m_pUI;
	// 選択不可能なUI
	std::vector<std::unique_ptr<UI>> m_pGuide;
	// 選択可能なUIが選ばれているときに出るUI
	std::vector<std::unique_ptr<UI>> m_pSelect;
	// ウィンドウの幅と高さ
	int m_windowWidth, m_windowHeight;
	// シーンID
	SceneID m_num;
	// 時間
	float m_time;
	// UIのヒットフラグ
	bool m_hit;
	// SEの音量
	float m_SEVolume;
	// 再生フラグ
	bool m_isSEPlay;
};
#endif // RESULTMENU_DEFINED