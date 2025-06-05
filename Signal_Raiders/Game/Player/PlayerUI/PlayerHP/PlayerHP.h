/*
*	@file PlayerHP.h
*	@brief HPゲージクラス
*/
#pragma once
#ifndef PLAYER_HP_DEFINED
#define PLAYER_HP_DEFINED
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
#include "Game/KumachiLib/KumachiLib.h"

// プレイヤーのHPゲージクラス
class PlayerHP : public IPlayUI
{
public:
	// アクセサ
	// 最大HPを設定
	void SetMaxHP(float maxHP) { m_maxHP = maxHP; }
public:
	// public関数
	// コンストラクタ
	PlayerHP();
	// デストラクタ
	~PlayerHP();
	// 初期化関数
	void Initialize(CommonResources* resources, int width, int height)override;
	// 更新関数
	void Update(const UpdateContext& context)override { Update(context.playerHP); }
	// 描画関数
	void Render()override;
	// UIの追加関数
	void Add(std::unique_ptr<PlayerUI>& pPlayerUI, const std::string& key
		, const DirectX::SimpleMath::Vector2& position
		, const DirectX::SimpleMath::Vector2& scale
		, KumachiLib::ANCHOR anchor)override;
private:
	// private関数
	// HP更新関数
	void Update(float playerHP);
private:
	// private変数
	// 共通リソース
	CommonResources* m_pCommonResources;
	// デバイスリソース
	DX::DeviceResources* m_pDR;
	// UI要素
	// ハートアイコン
	std::unique_ptr<PlayerUI> m_pHeartIcon;
	// HPゲージ
	std::unique_ptr<PlayerUI> m_pGauge;
	// HPゲージの枠
	std::unique_ptr<PlayerUI> m_pFrame;
	// HPゲージの背景
	std::unique_ptr<PlayerUI> m_pBack;
	// ウィンドウの幅
	int m_windowWidth;
	// ウィンドウの高さ
	int m_windowHeight;
	// 最大HP(途中で変動する）
	float m_maxHP;
	// 現在のHP
	float m_currentHP;
};
#endif // PLAYER_HP_DEFINED