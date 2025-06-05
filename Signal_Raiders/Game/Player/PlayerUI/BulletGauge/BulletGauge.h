/*
*	@file BulletGauge.h
*	@brief 弾ゲージクラス
*/
#pragma once
#ifndef BULLETGAUGE_DEFINED
#define BULLETGAUGE_DEFINED
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
#include "Game/Player/PlayerUI/PlayerUI.h"
#include "Game/KumachiLib/BinaryFile/BinaryFile.h"

// 前方宣言
class CommonResources;

// 弾ゲージクラス
class BulletGauge : public IPlayUI
{
public:
	// public関数
	// コンストラクタ
	BulletGauge();
	// デストラクタ
	~BulletGauge();
	// 初期化関数
	void Initialize(CommonResources* resources, int width, int height)override;
	// 更新関数
	void Update(const UpdateContext& context)override { Update(context.bulletPoint); }
	// 描画関数
	void Render()override;
	// UI追加関数
	void Add(std::unique_ptr<PlayerUI>& pPlayerUI, const std::string& key
		, const DirectX::SimpleMath::Vector2& position
		, const DirectX::SimpleMath::Vector2& scale
		, KumachiLib::ANCHOR anchor)override;
private:
	// private関数
	// 更新関数
	void Update(float bulletPoint);
private:
	// private変数
	// 共通リソース
	CommonResources* m_pCommonResources;
	// デバイスリソース
	DX::DeviceResources* m_pDR;
	// ゲージ
	std::unique_ptr<PlayerUI> m_pGauge;
	// ベーステクスチャのパス
	const wchar_t* m_baseTexturePath;
	// ウィンドウの幅と高さ
	int m_windowWidth, m_windowHeight;
};
#endif // BULLETGAUGE_DEFINED