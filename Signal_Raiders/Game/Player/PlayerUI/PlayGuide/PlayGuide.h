/*
*	@file PlayGuide.h
*	@brief プレイ操作説明クラス
*/
#pragma once
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
#include "Game/Screen.h"
// 前方宣言
class CommonResources;
class PlayGuide : public IPlayUI
{
public:// public関数
	// コンストラクタ
	PlayGuide();
	// デストラクタ
	~PlayGuide();
	// 初期化関数
	void Initialize(CommonResources* resources, int width, int height)override;
	// 更新関数
	void Update(const UpdateContext& context)override;
	// 描画関数
	void Render()override;
	// UI追加関数
	void Add(std::unique_ptr<PlayerUI>& pPlayerUI, const std::string& key
		, const DirectX::SimpleMath::Vector2& position
		, const DirectX::SimpleMath::Vector2& scale
		, KumachiLib::ANCHOR anchor)override;
	// UI作成関数
	void CreatePlayerUI(const std::string& key
		, const DirectX::SimpleMath::Vector2& position
		, const DirectX::SimpleMath::Vector2& scale
		, KumachiLib::ANCHOR anchor);
private:// private関数
	// 更新関数
	void Update();
private:// private変数
	// 共通リソース
	CommonResources* m_pCommonResources;
	// デバイスリソース
	DX::DeviceResources* m_pDR;
	// ポインターUI
	std::unique_ptr<PlayerUI> m_pPlayGuide;
	// ウィンドウの幅
	int m_windowWidth;
	// ウィンドウの高さ
	int m_windowHeight;
};
