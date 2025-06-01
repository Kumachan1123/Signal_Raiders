/*
*	@file Reticle.h
*	@brief 照準クラス
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
#include "Game/KumachiLib/BinaryFile/BinaryFile.h"
#include "Game/Screen.h"
#include "Game/Interface/IPlayUI.h"
// 前方宣言
class CommonResources;
class Reticle : public IPlayUI
{
public:// public関数
	Reticle();// コンストラクタ
	~Reticle();// デストラクタ
	void Initialize(CommonResources* resources, int width, int height)override;// 初期化関数
	void Update(const UpdateContext& context)override;// 更新関数
	void Render()override;// 描画関数
	void Add(std::unique_ptr<PlayerUI>& pPlayerUI, std::string key// UIの追加関数
		, DirectX::SimpleMath::Vector2 position
		, DirectX::SimpleMath::Vector2 scale
		, KumachiLib::ANCHOR anchor)override;
private:// private関数
	void Update();// 更新関数
private:// private変数
	// 共通リソース
	CommonResources* m_pCommonResources;
	// デバイスリソース
	DX::DeviceResources* m_pDR;
	// 照準UI
	std::unique_ptr<PlayerUI> m_pReticle;
	// ウィンドウの幅
	int m_windowWidth;
	// ウィンドウの高さ
	int m_windowHeight;

};
