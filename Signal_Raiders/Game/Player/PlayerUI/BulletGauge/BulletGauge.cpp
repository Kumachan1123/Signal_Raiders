/*
*	@file BulletGauge.cpp
*	@brief 弾ゲージクラス
*/
#include <pch.h>
#include "BulletGauge.h"
/*
*	@brief コンストラクタ
*	@details 弾ゲージクラスのコンストラクタ
*	@param なし
*	@return なし
*/
BulletGauge::BulletGauge()
	: m_windowHeight(0)// ウィンドウの高さ
	, m_windowWidth(0)// ウィンドウの幅
	, m_pDR(nullptr) // デバイスリソースへのポインタ
	, m_pCommonResources(nullptr) // 共通リソースへのポインタ
	, m_baseTexturePath(nullptr) // ベーステクスチャのパス
	, m_pGauge(nullptr) // ゲージのポインタ
{
}
/*
*	@brief デストラクタ
*	@details 弾ゲージクラスのデストラクタ(ここでは何もしない)
*	@param なし
*	@return なし
*/
BulletGauge::~BulletGauge() {/*do nothing*/ }
/*
*	@brief メニューを初期化する
*	@details メニューアイテムの画像を読み込み、位置・スケール・アンカーを設定する
*	@param resources 共通リソースへのポインタ
*	@param width ウィンドウの幅
*	@param height ウィンドウの高さ
*	@return なし
*/
void BulletGauge::Initialize(CommonResources* resources, int width, int height)
{
	using namespace DirectX::SimpleMath;
	// 共通リソースを設定
	m_pCommonResources = resources;
	// デバイスリソース取得
	m_pDR = resources->GetDeviceResources();
	// ウィンドウ幅
	m_windowWidth = width;
	// ウィンドウ高さ
	m_windowHeight = height;
	// ゲージの追加
	Add(m_pGauge
		, "BulletGauge"
		, Vector2(640, 360)
		, Vector2(0.8f, 0.8f)
		, KumachiLib::ANCHOR::MIDDLE_CENTER);

}
/*
*	@brief ゲージの更新
*	@details ゲージの値を更新する
*	@param bulletPoint 弾のポイント(0.0fから1.0fの範囲)
*	@return なし
*/
void BulletGauge::Update(float bulletPoint)
{
	// 弾のポイントを0.02倍する
	float bp = bulletPoint * 0.02f;
	// 0未満にならないようにする
	bp = std::max(0.f, bp);
	// ゲージのレンダリング比を設定
	m_pGauge->SetRenderRatio(bp);
}
/*
*	@brief ゲージの描画
*	@details ゲージを描画する
*	@param なし
*	@return なし
*/
void BulletGauge::Render()
{
	// シェーダータイプを設定したうえで描画
	m_pGauge->SetShaderType(PlayerUI::ShaderType::CIRCLE), m_pGauge->Render();
}
/*
*	@brief プレイヤーUIを追加する
*	@details プレイヤーUIを追加する
*	@param pPlayerUI プレイヤーUIのユニークポインタ
*	@param key テクスチャのキー
*	@param position プレイヤーUIの位置
*	@param scale プレイヤーUIのスケール
*	@param anchor アンカーの種類
*/
void BulletGauge::Add(std::unique_ptr<PlayerUI>& pPlayerUI
	, const std::string& key
	, const DirectX::SimpleMath::Vector2& position
	, const DirectX::SimpleMath::Vector2& scale
	, KumachiLib::ANCHOR anchor)
{
	// プレイヤーUIのユニークポインタを生成
	pPlayerUI = std::make_unique<PlayerUI>(m_pCommonResources);
	// プレイヤーUIの作成
	pPlayerUI->Create(m_pDR	// デバイスリソース
		, key				// テクスチャのパス
		, position			// プレイヤーUIの位置
		, scale				// プレイヤーUIのスケール
		, anchor);			// アンカーの種類
	// ウィンドウサイズを設定
	pPlayerUI->SetWindowSize(m_windowWidth, m_windowHeight);
}