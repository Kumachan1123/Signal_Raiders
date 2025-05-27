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
	, m_tracker{}// キーボードのトラッカー
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
	m_pCommonResources = resources;// 共通リソースを取得
	m_pDR = m_pCommonResources->GetDeviceResources();// デバイスリソースを取得
	m_pMousePointer = std::make_unique<UI>();// マウスポインターのUIを作成
	m_windowWidth = width;// ウィンドウの幅をセット
	m_windowHeight = height;// ウィンドウの高さをセット
	// マウスポインターのUIを作成
	Add(L"Resources/Textures/MousePointer.png"//  「マウスカーソル画像」を読み込む
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
	UNREFERENCED_PARAMETER(elapsedTime);// 未使用警告非表示
	auto& mouseState = m_pCommonResources->GetInputManager()->GetMouseState();// マウスの状態を取得
	Vector2 mousePos = Vector2(static_cast<float>(mouseState.x), static_cast<float>(mouseState.y));// マウスの座標を取得
	m_pMousePointer->SetPosition(mousePos);// マウスの座標をセット
}
/*
*	@brief	描画
*	@detail クラスの描画処理を行う
*	@param なし
*	@return なし
*/
void MousePointer::Render() { m_pMousePointer->Render(); }
/*
*	@brief	アイテムを追加
*	@detail 指定された画像を表示するためのアイテムを作成する
*	@param const wchar_t* path 画像のパス
*	@param DirectX::SimpleMath::Vector2 position 位置
*	@param DirectX::SimpleMath::Vector2 scale スケール
*	@param KumachiLib::ANCHOR anchor アンカー
*	@param IMenuUI::UIType type UIの種類
*	@return なし
*/
void MousePointer::Add(const wchar_t* path, DirectX::SimpleMath::Vector2 position, DirectX::SimpleMath::Vector2 scale, KumachiLib::ANCHOR anchor, IMenuUI::UIType type)
{
	UNREFERENCED_PARAMETER(type);// 未使用警告非表示
	m_pMousePointer->Create(m_pDR// デバイスリソース
		, path					// 画像のパス
		, position				// 位置
		, scale					// スケール
		, anchor);				// アンカー
	m_pMousePointer->SetWindowSize(m_windowWidth, m_windowHeight);// ウィンドウのサイズをセット
}
