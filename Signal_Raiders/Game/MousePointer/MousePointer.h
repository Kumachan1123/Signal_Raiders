/*
*	@file	TitleMenu.h
*	@brief	タイトル画面のメニュークラス
*/
#pragma once
//	標準ライブラリ
#include <vector>
#include <string>
// DirectXのライブラリ
#include <SimpleMath.h>
#include <WICTextureLoader.h>
#include <Mouse.h>
// 外部ライブラリ
#include <DeviceResources.h>
#include <Libraries/MyLib/InputManager.h>
// 自作ヘッダーファイル
#include "Game/CommonResources.h"
#include "Game/Screen.h"
#include "Game/KumachiLib/BinaryFile/BinaryFile.h"
#include "Game/UI/UI.h"
#include "Game/Interface/IMenuUI.h"
// 前方宣言
class CommonResources;
class MousePointer : public IMenuUI
{
public:// public関数
	// コンストラクタ
	MousePointer();
	// デストラクタ
	~MousePointer();
	// 初期化
	void Initialize(CommonResources* resources, int width, int height)override;
	// 更新
	void Update(const UpdateContext& context)override { Update(context.elapsedTime); }
	// 描画
	void Render()override;
	// UI追加
	void Add(const std::string& key
		, const DirectX::SimpleMath::Vector2& position
		, const DirectX::SimpleMath::Vector2& scale
		, KumachiLib::ANCHOR anchor
		, IMenuUI::UIType type)override;
private:// private関数
	// 更新（private）
	void Update(float elapsedTime);
private:// private変数
	// メニューのインデックス
	unsigned int m_menuIndex;
	// デバイスリソース
	DX::DeviceResources* m_pDR;
	// 共通リソース
	CommonResources* m_pCommonResources;
	// マウスポインター
	std::unique_ptr<UI> m_pMousePointer;
	// ウィンドウの幅と高さ
	int m_windowWidth, m_windowHeight;
};