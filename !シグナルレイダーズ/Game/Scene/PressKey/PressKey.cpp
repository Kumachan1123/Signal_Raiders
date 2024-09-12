/*
	@file	PressKey.cpp
	@brief	指示クラス
*/
#include "pch.h"
#include "PressKey.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include "Libraries/MyLib/InputManager.h"
#include <cassert>
using namespace DirectX;
using namespace DirectX::SimpleMath;

PressKey::PressKey(CommonResources* commonResources)
	:m_pressKeyTexture{}
	, m_pressKeyTexCenter{}
	, m_spriteBatch{}
	, m_commonResources{ commonResources }
{}

PressKey::~PressKey()
{}

void PressKey::Initialize()
{
	// デバイスリソースの取得
	auto device = m_commonResources->GetDeviceResources()->GetD3DDevice();
	auto context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();
	// スプライトバッチを作成する
	m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(context);
	// 画像をロードする
	DX::ThrowIfFailed(
		CreateWICTextureFromFile(
			device,
			L"Resources/Textures/Space.png",
			nullptr,
			m_pressKeyTexture.ReleaseAndGetAddressOf()
		)
	);
	// コモンステート
	m_states = std::make_unique<CommonStates>(device);
	// テクスチャの作成
	CreateTexture();
}

// 更新
void PressKey::Update(float elapsedTime)
{
	m_time += elapsedTime;
	m_size = (sin(m_time) * 0.1f) + 1.0f;
}

// 描画
void PressKey::Render()
{
	// スプライトバッチの開始：オプションでソートモード、ブレンドステートを指定する
	m_spriteBatch->Begin(SpriteSortMode_Deferred, m_states->NonPremultiplied());
	// タイトルロゴの描画位置を決める
	RECT rect{ m_commonResources->GetDeviceResources()->GetOutputSize() };
	// 画像の中心を計算する
	Vector2 spacePos{ rect.right / 2.0f, rect.bottom / 1.2f };
	// 描画する
	m_spriteBatch->Draw(
		m_pressKeyTexture.Get(),	// テクスチャ(SRV)
		spacePos,				// スクリーンの表示位置(originの描画位置)
		nullptr,			// 矩形(RECT)
		Colors::White,		// 背景色
		0.0f,				// 回転角(ラジアン)
		m_pressKeyTexCenter,// テクスチャの基準になる表示位置(描画中心)(origin)
		m_size,				// スケール(scale)
		SpriteEffects_None,	// エフェクト(effects)
		0.0f				// レイヤ深度(画像のソートで必要)(layerDepth)
	);
	// スプライトバッチの終わり
	m_spriteBatch->End();
}

// テクスチャの作成
void PressKey::CreateTexture()
{
	// 一時的な変数の宣言
	Microsoft::WRL::ComPtr<ID3D11Resource> resource{};
	Microsoft::WRL::ComPtr<ID3D11Texture2D> tex2D{};
	D3D11_TEXTURE2D_DESC desc{};
	Vector2 texSize{};
	// テクスチャの情報を取得する================================
	// テクスチャをID3D11Resourceとして見る
	m_pressKeyTexture->GetResource(resource.GetAddressOf());
	// ID3D11ResourceをID3D11Texture2Dとして見る
	resource.As(&tex2D);
	//// テクスチャの中心位置を計算する
	tex2D->GetDesc(&desc);
	// テクスチャサイズを取得し、float型に変換する
	texSize.x = static_cast<float>(desc.Width);
	texSize.y = static_cast<float>(desc.Height);
	m_pressKeyTexCenter = texSize / 2.0f;
}