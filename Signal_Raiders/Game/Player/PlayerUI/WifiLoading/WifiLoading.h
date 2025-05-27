/*
*	@file	WifiLoading.h
*	@brief	Wi-Fiローディングクラス
*/
#pragma once
// 標準ライブラリ
#include <vector>
#include <memory>
// DirectX
#include <DeviceResources.h>
#include <SimpleMath.h>
#include <WICTextureLoader.h>
// 自作ヘッダーファイル 
#include "Game/Screen.h"
#include "Game/Interface/IPlayUI.h"
#include "Game/Player/PlayerUI/PlayerUI.h"
#include "Game/KumachiLib/BinaryFile/BinaryFile.h"
class WifiLoading : public IPlayUI
{
public:// public関数
	WifiLoading();// コンストラクタ
	~WifiLoading();// デストラクタ
	void Initialize(CommonResources* resources, int width, int height) override;// 初期化関数
	void Update(const UpdateContext& data)override { Update(data.elapsedTime); }// 更新関数
	void Render()override;// 描画関数
	void Add(std::unique_ptr<PlayerUI>& pPlayerUI, const wchar_t* path// UIの追加関数
		, DirectX::SimpleMath::Vector2 position
		, DirectX::SimpleMath::Vector2 scale
		, KumachiLib::ANCHOR anchor)override;
private:// private関数
	void Update(float elapsedTime);// 更新関数
private:// private変数
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