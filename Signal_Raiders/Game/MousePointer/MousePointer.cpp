/*
*	@file	TitleMenu.cpp
*	@brief	タイトル画面のメニュークラス
*/
#include <pch.h>
#include "MousePointer.h"
/*
*	@brief	コンストラクタ
*	@detail クラスの生成処理を行う
*	@param なし
*	@return なし
*/
MousePointer::MousePointer()
	: m_pCommonResources{}// 共通リソース
	, m_menuIndex{}// メニューのインデックス
	, m_pDR{}// デバイスリソース
	, m_pMousePointer{}// マウスポインターのUI
	, m_windowWidth{}// ウィンドウの幅
	, m_windowHeight{}// ウィンドウの高さ
{
}
/*
*	@brief	デストラクタ
*	@detail クラスの破棄処理を行う
*	@param	なし
*	@return なし
*/
MousePointer::~MousePointer() {	/*do nothing.*/ }
/*
*	@brief	初期化
*	@detail クラスの初期化処理を行う
*	@param CommonResources* resources 共通リソース
*	@param int width ウィンドウの幅
*	@param int height ウィンドウの高さ
*	@return なし
*/
void MousePointer::Initialize(CommonResources* resources, int width, int height)
{
	using namespace DirectX::SimpleMath;
	// 共通リソースを取得
	m_pCommonResources = resources;
	// デバイスリソースを取得
	m_pDR = m_pCommonResources->GetDeviceResources();
	// マウスポインターのUIを作成
	m_pMousePointer = std::make_unique<UI>(m_pCommonResources);
	// ウィンドウの幅をセット
	m_windowWidth = width;
	// ウィンドウの高さをセット
	m_windowHeight = height;
	// マウスポインターのUIを作成
	Add("MousePointer"//  「マウスカーソル画像」を読み込む
		, Vector2(0, 0)// 初期位置
		, Vector2(1, 1)// スケール
		, KumachiLib::ANCHOR::MIDDLE_CENTER// アンカー
		, UIType::NON_SELECT);// 選択不可
}
/*
*	@brief	更新
*	@detail クラスの更新処理を行う
*	@param float elapsedTime 経過時間
*	@return なし
*/
void MousePointer::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;
	// 未使用警告非表示
	UNREFERENCED_PARAMETER(elapsedTime);
	// マウスの状態を取得
	auto& mouseState = m_pCommonResources->GetInputManager()->GetMouseState();
	// マウスの座標を取得
	Vector2 mousePos = Vector2(static_cast<float>(mouseState.x), static_cast<float>(mouseState.y));
	// マウスの座標をセット
	m_pMousePointer->SetPosition(mousePos);
}
/*
*	@brief	描画
*	@detail クラスの描画処理を行う
*	@param なし
*	@return なし
*/
void MousePointer::Render()
{
	// マウスポインターのUIを描画
	m_pMousePointer->Render();
}
/*
*	@brief	アイテムを追加
*	@detail 指定された画像を表示するためのアイテムを作成する
*	@param std::string key 画像のキー
*	@param DirectX::SimpleMath::Vector2 position 位置
*	@param DirectX::SimpleMath::Vector2 scale スケール
*	@param KumachiLib::ANCHOR anchor アンカー
*	@param IMenuUI::UIType type UIの種類
*	@return なし
*/
void MousePointer::Add(const std::string& key,
	const DirectX::SimpleMath::Vector2& position,
	const DirectX::SimpleMath::Vector2& scale,
	KumachiLib::ANCHOR anchor, IMenuUI::UIType type)
{
	// 未使用警告非表示
	UNREFERENCED_PARAMETER(type);
	// マウスポインターのUIを作成
	m_pMousePointer->Create(m_pDR// デバイスリソース
		, key					// 画像のパス
		, position				// 位置
		, scale					// スケール
		, anchor);				// アンカー
	// ウィンドウのサイズをセット
	m_pMousePointer->SetWindowSize(m_windowWidth, m_windowHeight);
}
