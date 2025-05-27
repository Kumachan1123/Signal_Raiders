/*
*	@file BulletGauge.h
*	@brief 弾ゲージクラス
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
#include "Game/Player/PlayerUI/PlayerUI.h"
#include "Game/KumachiLib/BinaryFile/BinaryFile.h"

class BulletGauge : public IPlayUI
{
public:// public関数
	BulletGauge();// コンストラクタ
	~BulletGauge();// デストラクタ
	void Initialize(CommonResources* resources, int width, int height)override;// 初期化関数
	void Update(const UpdateContext& context)override { Update(context.bulletPoint); }// 更新関数
	void Render()override;// 描画関数
	void Add(std::unique_ptr<PlayerUI>& pPlayerUI, const wchar_t* path// 追加関数
		, DirectX::SimpleMath::Vector2 position
		, DirectX::SimpleMath::Vector2 scale
		, KumachiLib::ANCHOR anchor)override;
private:// private関数
	void Update(float bulletPoint);// 更新関数
private:// private変数
	// デバイスリソース
	DX::DeviceResources* m_pDR;
	// ゲージ
	std::unique_ptr<PlayerUI> m_pGauge;
	// ベーステクスチャのパス
	const wchar_t* m_baseTexturePath;
	// ウィンドウの幅と高さ
	int m_windowWidth, m_windowHeight;
};
