/*
*	@file DashGauge.h
*	@brief ダッシュゲージクラス
*/
#pragma once
#ifndef DASHGAUGE_DEFINED
#define DASHGAUGE_DEFINED
// 標準ライブラリ
#include <vector>
#include <string>
// DirectX
#include <DeviceResources.h>
#include <SimpleMath.h>
#include <WICTextureLoader.h>
// 自作ヘッダーファイル
#include "Game/CommonResources.h"
#include "Game/Player/PlayerUI/PlayerUI.h"
#include "Game/Interface/IPlayUI.h"
#include "Game/KumachiLib/BinaryFile/BinaryFile.h"

// 前方宣言
class CommonResources;

// ダッシュゲージクラス
class DashGauge : public IPlayUI
{
public:
	// public関数
	// コンストラクタ
	DashGauge();
	// デストラクタ
	~DashGauge();
	// 初期化
	void Initialize(CommonResources* resources, int width, int height)override;
	// 更新
	void Update(const UpdateContext& context)override { Update(context.dashStamina); }
	// 描画
	void Render()override;
	// UI追加
	void Add(std::unique_ptr<PlayerUI>& pPlayerUI, const std::string& key
		, const DirectX::SimpleMath::Vector2& position
		, const DirectX::SimpleMath::Vector2& scale
		, KumachiLib::ANCHOR anchor)override;
private:
	// private関数
	// 更新
	void Update(float dashStamina);
private:// private変数
	// 共通リソース
	CommonResources* m_pCommonResources;
	// デバイスリソース
	DX::DeviceResources* m_pDR;
	// ダッシュアイコン
	std::unique_ptr<PlayerUI> m_pDashIcon;
	// ゲージ本体
	std::unique_ptr<PlayerUI> m_pGauge;
	// ゲージの枠
	std::unique_ptr<PlayerUI> m_pFrame;
	// ゲージの背景
	std::unique_ptr<PlayerUI> m_pBack;
	// ウィンドウの幅
	int m_windowWidth;
	// ウィンドウの高さ
	int m_windowHeight;
};
#endif // DASHGAUGE_DEFINED
