/*
*	@file Reticle.cpp
*	@brief 照準クラス
*/
#include <pch.h>
#include "Reticle.h"
/*
*	@brief コンストラクタ
*	@details ウィンドウの幅と高さを初期化
*	@param なし
*	@return なし
*/
Reticle::Reticle()
	: m_windowHeight(0)				// ウィンドウの高さ
	, m_windowWidth(0)				// ウィンドウの幅
	, m_pDR(nullptr)				// デバイスリソース
	, m_pCommonResources(nullptr)	// 共通リソースへのポインタ
	, m_pReticle(nullptr)			// 照準UI
{
}
/*
*	@brief デストラクタ
*	@details ここでは何もしない
*	@param なし
*	@return なし
*/
Reticle::~Reticle() {/*do nothing*/ }
/*
*	@brief 初期化関数
*	@details デバイスリソースを取得し、ウィンドウの幅と高さを設定後、照準UIを作成する
*	@param resources デバイスリソース
*	@param width ウィンドウの幅
*	@param height ウィンドウの高さ
*	@return なし
*/
void Reticle::Initialize(CommonResources* resources, int width, int height)
{
	using namespace DirectX::SimpleMath;
	// 共通リソースを設定
	m_pCommonResources = resources;
	// デバイスリソースを取得
	m_pDR = resources->GetDeviceResources();
	// ウィンドウの幅を設定
	m_windowWidth = width;
	// ウィンドウの高さを設定
	m_windowHeight = height;
	// 照準UIの追加
	Add(m_pReticle
		, "Reticle"
		, Vector2(640, 360)
		, Vector2(0.25f, 0.25f)
		, KumachiLib::ANCHOR::MIDDLE_CENTER);

}
/*
*	@brief 更新関数(外部用)
*	@details 照準の更新を行う(このUIは変化させないので更新処理はしない)
*	@param context 更新コンテキスト
*	@return なし
*/
void Reticle::Update(const UpdateContext& context)
{
	// 更新コンテキストは使用しないので無視する
	UNREFERENCED_PARAMETER(context);
}
/*
*	@brief 更新関数(内部用)
*	@details 照準の更新を行う(このUIは変化させないので更新処理はしない)
*	@param なし
*	@return なし
*/
void Reticle::Update() { /*do nothing*/ }
/*
*	@brief 描画関数
*	@details 照準の描画を行う
*	@param なし
*	@return なし
*/
void Reticle::Render()
{
	// シェーダータイプを設定
	m_pReticle->SetShaderType(PlayerUI::ShaderType::OTHER);
	// 照準の描画を行う
	m_pReticle->Render();
}
/*
*	@brief UIの追加関数
*	@details PlayerUIのインスタンスを生成し、UIの初期化を行う
*	@param pPlayerUI PlayerUIのインスタンス
*	@param key テクスチャのキー
*	@param position UIの位置
*	@param scale UIのスケール
*	@param anchor UIのアンカー
*	@return なし
*/
void Reticle::Add(std::unique_ptr<PlayerUI>& pPlayerUI, const std::string& key,
	const DirectX::SimpleMath::Vector2& position,
	const DirectX::SimpleMath::Vector2& scale, KumachiLib::ANCHOR anchor)
{
	// PlayerUIのインスタンスを生成
	pPlayerUI = std::make_unique<PlayerUI>(m_pCommonResources);
	// UIの初期化
	pPlayerUI->Create(m_pDR	// デバイスリソース
		, key				// テクスチャのパス
		, position			// UIの位置
		, scale				// UIのスケール
		, anchor);			// UIのアンカーを設定
	// ウィンドウのサイズを設定
	pPlayerUI->SetWindowSize(m_windowWidth, m_windowHeight);
}



