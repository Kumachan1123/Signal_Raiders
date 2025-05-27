/*
*	@file PlayerHP.h
*	@brief HPゲージクラス
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
#include "Game/KumachiLib/KumachiLib.h"
class PlayerHP : public IPlayUI
{
public:// アクセサ
	void SetMaxHP(float maxHP) { m_maxHP = maxHP; }// 最大HPを設定
public:// public関数
	PlayerHP();// コンストラクタ
	~PlayerHP();// デストラクタ
	void Initialize(CommonResources* resources, int width, int height)override;// 初期化関数
	void Update(const UpdateContext& context)override { Update(context.playerHP); }// 更新関数
	void Render()override;// 描画関数
	void Add(std::unique_ptr<PlayerUI>& pPlayerUI, const wchar_t* path// UIの追加関数
		, DirectX::SimpleMath::Vector2 position
		, DirectX::SimpleMath::Vector2 scale
		, KumachiLib::ANCHOR anchor)override;
private:// private関数
	void Update(float playerHP);// HP更新関数
private:// private変数
	// デバイスリソース
	DX::DeviceResources* m_pDR;
	// UI要素
	std::unique_ptr<PlayerUI> m_pHeartIcon;// ハートアイコン
	std::unique_ptr<PlayerUI> m_pGauge; // HPゲージ
	std::unique_ptr<PlayerUI> m_pFrame; // HPゲージの枠
	std::unique_ptr<PlayerUI> m_pBack; // HPゲージの背景
	// ウィンドウの幅
	int m_windowWidth;
	// ウィンドウの高さ
	int m_windowHeight;
	// 最大HP(途中で変動する）
	float m_maxHP;
	// 現在のHP
	float m_currentHP;
};
