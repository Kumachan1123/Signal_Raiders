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
	: m_windowHeight(Screen::UI_HEIGHT)
	, m_windowWidth(Screen::UI_WIDTH)
	, m_pDR(nullptr)
	, m_pPlayGuide(nullptr)
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
	m_pDR = resources->GetDeviceResources();// デバイスリソースを取得
	m_windowWidth = width;// ウィンドウの幅を設定
	m_windowHeight = height;// ウィンドウの高さを設定
	CreatePlayerUI(L"Resources/Textures/PlayGuide.png"// プレイ操作説明のテクスチャパス
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
	UNREFERENCED_PARAMETER(context);// 更新コンテキストは使用しないので無視する
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
	m_pPlayGuide->SetShaderType(PlayerUI::ShaderType::OTHER);// シェーダータイプを設定
	m_pPlayGuide->Render();// プレイ操作説明の描画を行う
}
/*
*	@brief UIの追加関数
*	@details PlayerUIを追加する
*	@param pPlayerUI 追加するPlayerUIのポインタ
*	@param path テクスチャのパス
*	@param position UIの位置
*	@param scale UIのスケール
*	@param anchor UIのアンカー
*	@return なし
*/
void PlayGuide::Add(std::unique_ptr<PlayerUI>& pPlayerUI, const wchar_t* path, DirectX::SimpleMath::Vector2 position, DirectX::SimpleMath::Vector2 scale, KumachiLib::ANCHOR anchor)
{
	pPlayerUI = std::make_unique<PlayerUI>();// PlayerUIのインスタンスを生成
	// UIの初期化
	pPlayerUI->Create(m_pDR	// デバイスリソース
		, path				// テクスチャのパス
		, position			// UIの位置
		, scale				// UIのスケール
		, anchor);			// UIのアンカーを設定
	pPlayerUI->SetWindowSize(m_windowWidth, m_windowHeight);// ウィンドウのサイズを設定
}
/*
*	@brief UI作成関数
*	@details プレイ操作説明のUIを作成する
*	@param path テクスチャのパス
*	@param position UIの位置
*	@param scale UIのスケール
*	@param anchor UIのアンカー
*	@return なし
*/
void PlayGuide::CreatePlayerUI(const wchar_t* path, DirectX::SimpleMath::Vector2 position, DirectX::SimpleMath::Vector2 scale, KumachiLib::ANCHOR anchor)
{
	Add(m_pPlayGuide, path, position, scale, anchor);// UIを追加する関数を呼び出す
}


