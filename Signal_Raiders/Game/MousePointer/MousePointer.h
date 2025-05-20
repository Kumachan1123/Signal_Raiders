/*
	@file	TitleMenu.h
	@brief	タイトル画面のメニュークラス
*/
#pragma once
//	標準ライブラリ
#include <vector>
// DirectXのライブラリ
#include <SimpleMath.h>
#include <WICTextureLoader.h>
#include <Mouse.h>
// 外部ライブラリ
#include <DeviceResources.h>
#include "Game/CommonResources.h"
#include "Libraries/MyLib/InputManager.h"
// 自作ヘッダーファイル
#include "Game/Screen.h"
#include "Game/KumachiLib/BinaryFile/BinaryFile.h"
#include "Game/UI/UI.h"
#include "Game/Interface/IMenuUI.h"
// 前方宣言
class CommonResources;
class MousePointer : public IMenuUI
{
public:
	// public関数
	MousePointer();// コンストラクタ
	~MousePointer();// デストラクタ
	void Initialize(CommonResources* resources, int width, int height)override;// 初期化
	void Update(const UpdateContext& context)override { Update(context.elapsedTime); };// 更新
	void Render()override;// 描画
	void Add(const wchar_t* path// UI追加
		, DirectX::SimpleMath::Vector2 position
		, DirectX::SimpleMath::Vector2 scale
		, KumachiLib::ANCHOR anchor
		, IMenuUI::UIType type)override;
private:
	// private関数
	void Update(float elapsedTime);// 更新（private）
private:
	// private変数
	unsigned int m_menuIndex;// メニューのインデックス
	DX::DeviceResources* m_pDR;// デバイスリソース
	CommonResources* m_commonResources;// 共通リソース
	std::unique_ptr<UI> m_pMousePointer;// マウスポインター
	int m_windowWidth, m_windowHeight;// ウィンドウの幅と高さ
	DirectX::Keyboard::KeyboardStateTracker m_tracker;// キーボードのトラッカー
};