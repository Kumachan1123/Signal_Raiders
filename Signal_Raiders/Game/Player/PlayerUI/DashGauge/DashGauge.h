/*
*	@file DashGauge.h
*	@brief ダッシュゲージクラス
*/
#pragma once
// 標準ライブラリ
#include <vector>
// DirectX
#include <DeviceResources.h>
#include <SimpleMath.h>
#include <WICTextureLoader.h>
// 自作ヘッダーファイル
#include "Game/Player/PlayerUI/PlayerUI.h"
#include "Game/Interface/IPlayUI.h"
#include "Game/KumachiLib/BinaryFile/BinaryFile.h"
class DashGauge : public IPlayUI
{
public:// public関数
	DashGauge();// コンストラクタ
	~DashGauge();// デストラクタ
	void Initialize(CommonResources* resources, int width, int height)override;// 初期化
	void Update(const UpdateContext& context)override { Update(context.dashStamina); }// 更新
	void Render()override;// 描画
	void Add(std::unique_ptr<PlayerUI>& pPlayerUI, const wchar_t* path// UI追加
		, DirectX::SimpleMath::Vector2 position
		, DirectX::SimpleMath::Vector2 scale
		, KumachiLib::ANCHOR anchor)override;
private:// private関数
	void Update(float dashStamina);// 更新
private:// private変数
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
