/*
*	@file PlayGuide.h
*	@brief プレイ操作説明クラス
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
#include "Game/Screen.h"

class PlayGuide : public IPlayUI
{
public:// public関数
	PlayGuide();// コンストラクタ
	~PlayGuide();// デストラクタ
	void Initialize(CommonResources* resources, int width, int height)override;// 初期化関数
	void Update(const UpdateContext& context)override;// 更新関数
	void Render()override;// 描画関数
	void Add(std::unique_ptr<PlayerUI>& pPlayerUI, const wchar_t* path// UI追加関数
		, DirectX::SimpleMath::Vector2 position
		, DirectX::SimpleMath::Vector2 scale
		, KumachiLib::ANCHOR anchor)override;
	void CreatePlayerUI(const wchar_t* path// UI作成関数
		, DirectX::SimpleMath::Vector2 position
		, DirectX::SimpleMath::Vector2 scale
		, KumachiLib::ANCHOR anchor);
private:// private関数
	void Update();// 更新関数
private:// private変数
	// デバイスリソース
	DX::DeviceResources* m_pDR;
	// ポインターUI
	std::unique_ptr<PlayerUI> m_pPlayGuide;
	// ウィンドウの幅
	int m_windowWidth;
	// ウィンドウの高さ
	int m_windowHeight;
};
