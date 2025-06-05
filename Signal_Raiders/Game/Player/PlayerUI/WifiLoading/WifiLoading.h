/*
*	@file	WifiLoading.h
*	@brief	Wi-Fiローディングクラス
*/
#pragma once
#ifndef WIFI_LOADING_DEFINED
#define WIFI_LOADING_DEFINED
// 標準ライブラリ
#include <vector>
#include <memory>
// DirectX
#include <DeviceResources.h>
#include <SimpleMath.h>
#include <WICTextureLoader.h>
// 自作ヘッダーファイル 
#include "Game/Screen.h"
#include "Game/CommonResources.h"
#include "Game/Interface/IPlayUI.h"
#include "Game/Player/PlayerUI/PlayerUI.h"
#include "Game/KumachiLib/BinaryFile/BinaryFile.h"

// Wi-Fiローディングクラス
class WifiLoading : public IPlayUI
{
public:
	// public関数
	// コンストラクタ
	WifiLoading();
	// デストラクタ
	~WifiLoading();
	// 初期化関数
	void Initialize(CommonResources* resources, int width, int height) override;
	// 更新関数
	void Update(const UpdateContext& data)override { Update(data.elapsedTime); }
	// 描画関数
	void Render()override;
	// UIの追加関数
	void Add(std::unique_ptr<PlayerUI>& pPlayerUI, const std::string& key
		, const DirectX::SimpleMath::Vector2& position
		, const DirectX::SimpleMath::Vector2& scale
		, KumachiLib::ANCHOR anchor)override;
private:
	// private関数
	// 更新関数
	void Update(float elapsedTime);
private:
	// private変数
	// 共通リソース
	CommonResources* m_pCommonResources;
	// デバイスリソース
	DX::DeviceResources* m_pDR;
	// Wi-FiローディングUI
	std::unique_ptr<PlayerUI> m_pLoading;
	// ローディングテキスト
	std::unique_ptr<PlayerUI> m_pLoadgingText;
	// ウィンドウの幅
	int m_windowWidth;
	// ウィンドウの高さ
	int m_windowHeight;
	// フレームの行数 
	int m_frameRows;
	// フレームの列数
	int m_frameCols;
	// フレーム数
	int m_anim;
	// アニメーションスピード
	float m_animSpeed;
	// アニメーション時間
	float m_animTime;
};
#endif // WIFI_LOADING_DEFINED