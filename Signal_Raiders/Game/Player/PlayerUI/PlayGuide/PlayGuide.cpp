/*
*	@file PlayGuide.cpp
*	@brief プレイ操作説明クラス
*/
#include <pch.h>
#include "PlayGuide.h"
/*
*	@brief コンストラクタ
*	@details ウィンドウの幅と高さを初期化
*	@param なし
*	@return なし
*/
PlayGuide::PlayGuide()
	: m_windowHeight(Screen::UI_HEIGHT)	// ウィンドウの高さ
	, m_windowWidth(Screen::UI_WIDTH)	// ウィンドウの幅
	, m_pCommonResources(nullptr)		// 共通リソースへのポインタ
	, m_pDR(nullptr)					// デバイスリソースへのポインタ
	, m_pPlayGuide(nullptr)				// プレイ操作説明のUIポインタ
{
}
/*
*	@brief デストラクタ
*	@details ここでは何もしない
*	@param なし
*	@return なし
*/
PlayGuide::~PlayGuide() {/*do nothing*/ }
/*
*	@brief 初期化関数
*	@details デバイスリソースを取得し、ウィンドウの幅と高さを設定後、UIを作成する
*	@param resources デバイスリソース
*	@param width ウィンドウの幅
*	@param height ウィンドウの高さ
*	@return なし
*/
void PlayGuide::Initialize(CommonResources* resources, int width, int height)
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
	// プレイ操作説明のテクスチャパス
	CreatePlayerUI("PlayGuide"
		, Vector2(Screen::UI_LEFT, Screen::UI_BOTTOM)
		, Vector2(0.5f, 0.5f)
		, KumachiLib::ANCHOR::BOTTOM_LEFT);
}
/*
*	@brief 更新関数(外部用)
*	@details プレイ操作説明の更新を行う(このUIは変化させないので更新処理はしない
*	@param context 更新コンテキスト
*	@return なし
*/
void PlayGuide::Update(const UpdateContext& context)
{
	// 更新コンテキストは使用しないので無視する
	UNREFERENCED_PARAMETER(context);
}
/*
*	@brief 更新関数(内部用)
*	@details プレイ操作説明の更新を行う(このUIは変化させないので更新処理はしない
*	@param なし
*	@return なし
*/
void PlayGuide::Update() {/*do nothing*/ }
/*
*	@brief 描画関数
*	@details プレイ操作説明の描画を行う
*	@param なし
*	@return なし
*/
void PlayGuide::Render()
{
	// シェーダータイプを設定
	m_pPlayGuide->SetShaderType(PlayerUI::ShaderType::OTHER);
	// プレイ操作説明の描画を行う
	m_pPlayGuide->Render();
}
/*
*	@brief UIの追加関数
*	@details PlayerUIを追加する
*	@param pPlayerUI 追加するPlayerUIのポインタ
*	@param key テクスチャのキー
*	@param position UIの位置
*	@param scale UIのスケール
*	@param anchor UIのアンカー
*	@return なし
*/
void PlayGuide::Add(std::unique_ptr<PlayerUI>& pPlayerUI, const std::string& key,
	const DirectX::SimpleMath::Vector2& position,
	const DirectX::SimpleMath::Vector2& scale,
	KumachiLib::ANCHOR anchor)
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
/*
*	@brief UI作成関数
*	@details プレイ操作説明のUIを作成する
*	@param key テクスチャのキー
*	@param position UIの位置
*	@param scale UIのスケール
*	@param anchor UIのアンカー
*	@return なし
*/
void PlayGuide::CreatePlayerUI(const std::string& key,
	const DirectX::SimpleMath::Vector2& position,
	const DirectX::SimpleMath::Vector2& scale, KumachiLib::ANCHOR anchor)
{
	// UIを追加する関数を呼び出す
	Add(m_pPlayGuide, key, position, scale, anchor);
}


