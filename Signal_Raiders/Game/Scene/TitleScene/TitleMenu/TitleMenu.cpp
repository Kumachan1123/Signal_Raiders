/*
*	@file	TitleMenu.cpp
*	@brief	タイトルメニュークラス
*/
#include <pch.h>
#include "TitleMenu.h"
// 無効なメニューインデックス
const int TitleMenu::INVALID_MENU_INDEX = 6;// 無効なメニューインデックス
/*
*	@brief コンストラクタ
*	@details タイトルメニュークラスのコンストラクタ
*	@param なし
*	@return なし
*/
TitleMenu::TitleMenu()
	: m_menuIndex{ 0 }// 現在選択中のメニューインデックス
	, m_pDR{ nullptr }// デバイスリソースへのポインタ
	, m_pCommonResources{ nullptr }// 共通リソースへのポインタ
	, m_pUI{}// UIのポインター
	, m_pSelect{}// 選択された時に表示する背景UIのポインター
	, m_pSelectTexturePath{ nullptr }// 選択背景のテクスチャパス
	, m_windowWidth{ 0 }// ウィンドウ幅
	, m_windowHeight{ 0 }// ウィンドウ高さ
	, m_time{ 0.0f }// 経過時間
	, m_hit{ false }// ヒットフラグ
	, m_num{ SceneID::STAGESELECT }// シーンID
	, m_SEVolume{ 0.0f }// SE音量
	, m_isSEPlay{ false }// SE再生フラグ
{
}
/*
*	@brief デストラクタ
*	@details タイトルメニュークラスのデストラクタ(ここでは何もしない)
*	@param なし
*	@return なし
*/
TitleMenu::~TitleMenu() {/*do nothing*/ }
/*
*	@brief メニューを初期化する
*	@details メニューアイテムの画像を読み込み、位置・スケール・アンカーを設定する
*	@param resources 共通リソースへのポインタ
*	@param width ウィンドウの幅
*	@param height ウィンドウの高さ
*	@return なし
*/
void TitleMenu::Initialize(CommonResources* resources, int width, int height)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	m_pCommonResources = resources;// 共通リソースをセット
	m_pDR = m_pCommonResources->GetDeviceResources();// デバイスリソース取得
	m_windowWidth = width;// ウィンドウ幅
	m_windowHeight = height;// ウィンドウ高さ
	//m_pSelectTexturePath = L"Resources/Textures/Select.png";// 選択枠のテクスチャパス設定
	//  「プレイ」を読み込む
	Add("Play"
		, Vector2(Screen::CENTER_X, Screen::CENTER_Y + 250)
		, Vector2(.5, .5)
		, KumachiLib::ANCHOR::MIDDLE_CENTER
		, UIType::SELECT);
	//  「せってい」を読み込む
	Add("Setting"
		, Vector2(Screen::CENTER_X, Screen::CENTER_Y + 350)
		, Vector2(.5, .5)
		, KumachiLib::ANCHOR::MIDDLE_CENTER
		, UIType::SELECT);
	//  「おわる」を読み込む
	Add("Cancel"
		, Vector2(Screen::CENTER_X, Screen::CENTER_Y + 450)
		, Vector2(.5, .5)
		, KumachiLib::ANCHOR::MIDDLE_CENTER
		, UIType::SELECT);
	//  「操作説明」を読み込む
	Add("Guide"
		, Vector2(Screen::RIGHT, Screen::BOTTOM)
		, Vector2(1, 1)
		, KumachiLib::ANCHOR::BOTTOM_RIGHT
		, UIType::NON_SELECT);

}
/*
*	@brief	更新
*	@details タイトルメニューの更新
*	@param elapsedTime 経過時間
*	@return なし
*/
void TitleMenu::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;
	auto& mtracker = m_pCommonResources->GetInputManager()->GetMouseTracker();// マウスのトラッカーを取得する
	auto& mouseState = m_pCommonResources->GetInputManager()->GetMouseState();// マウスの状態を取得
	m_hit = false;// 何かにヒットしたか
	Vector2 mousePos = Vector2(static_cast<float>(mouseState.x), static_cast<float>(mouseState.y));// マウスの座標を取得
	for (int i = 0; i < m_pUI.size(); i++)// メニューアイテムの数だけ繰り返す
	{
		if (m_pUI[i]->IsHit(mousePos))// マウスの座標がアイテムの範囲内にあるかどうかを判定
		{
			m_hit = true;	// ヒットフラグを立てる
			if ((int(m_menuIndex)) != i) m_isSEPlay = false;// 前回選択したメニューと違う場合はSEを再生するフラグを立てる
			if (!m_isSEPlay)// SEが再生されていない場合
			{
				m_pCommonResources->GetAudioManager()->PlaySound("Select", m_SEVolume);// SEの再生
				m_isSEPlay = true;// 再生フラグを立てる
			}
			m_menuIndex = i;// ヒットしたメニューのインデックスを保存
			break;	// ヒットしたらループを抜ける
		}
	}
	m_time += elapsedTime;// 時間を加算する
	if (!m_hit) { m_menuIndex = INVALID_MENU_INDEX; }// もし一個もヒットしてなかったら、選択なしにする
	if (mtracker->GetLastState().leftButton)m_num = static_cast<SceneID>(m_menuIndex);// 左クリックされたら選択メニューのシーンIDを更新
	for (int i = 0; i < m_pUI.size(); i++)// メニューアイテムの選択先を更新
	{
		//  アイテムの選択状態を更新
		m_pSelect[i]->SetScale(m_pSelect[i]->GetSelectScale());// 選択状態のスケールを取得
		m_pSelect[i]->SetTime(m_pSelect[i]->GetTime() + elapsedTime);// アイテムの選択状態を更新
		m_pUI[i]->SetScale(m_pUI[i]->GetSelectScale());// 選択状態のスケールを取得
		m_pUI[i]->SetTime(m_pUI[i]->GetTime() + elapsedTime);// アイテムの選択状態を更新
	}
	for (int i = 0; i < m_pGuide.size(); i++)// 選択不可能なアイテムの選択状態を更新
	{
		m_pGuide[i]->SetScale(m_pGuide[i]->GetSelectScale());// スケールを取得
		m_pGuide[i]->SetTime(m_pGuide[i]->GetTime() + elapsedTime);// 時間を加算
	}
	if (m_menuIndex == INVALID_MENU_INDEX)return;// なにも選択されていない場合は何もしない
	Vector2 select = m_pUI[m_menuIndex]->GetSelectScale();// 選択中の初期サイズを取得する
	Vector2 selectScale = Vector2::Lerp(m_pUI[m_menuIndex]->GetSelectScale(), Vector2::One, 1);// 選択状態とするための変化用サイズを算出する
	select = Vector2((sin(m_time) * 0.1f) + 1.0f);// 選択状態は初期状態より大きくする
	m_pUI[m_menuIndex]->SetScale(select);// 算出後のサイズを現在のサイズとして設定する
	m_pSelect[m_menuIndex]->SetScale(Vector2::One);// 背景用のウィンドウ画像にも同じ割合の値を設定する
}
/*
*	@brief	描画
*	@detail タイトルのUIを描画する
*	@param なし
*	@return なし
*/
void TitleMenu::Render()
{
	for (unsigned int i = 0; i < m_pUI.size(); i++)// 登録したUIの数ループ
	{
		if (i == m_menuIndex) m_pSelect[i]->Render();//  アイテム用ウィンドウ背景を表示
		m_pUI[i]->Render();	//  実際に表示したいアイテム画像を表示
	}
	for (unsigned int i = 0; i < m_pGuide.size(); i++)m_pGuide[i]->Render();// 選択不可能なアイテムを表示
#ifdef _DEBUG// デバッグモードの時のみ実行する
	auto debugString = m_pCommonResources->GetDebugString(); // デバッグ情報を表示する
	for (int i = 0; i < m_transforms.size(); i++)// 登録したUIの数ループ
		debugString->AddString("Transform.Pos:%f,%f  Scale:%f,%f",//  UIの座標を表示
			m_transforms[i].position.x,// X座標
			m_transforms[i].position.y,// Y座標
			m_transforms[i].scale.x,	// Xスケール
			m_transforms[i].scale.y);	// Yスケール
#endif
}
/*
	@brief メニューアイテムを追加する
	@details 指定の画像でUIを作成し、内部リストに追加する (選択可なら背景枠も作成)
	@param key 画像ファイルのキー
	@param position 描画位置
	@param scale 描画スケール
	@param anchor アンカー指定
	@param type メニューの種類（選択可/不可）
	@return なし
*/
void TitleMenu::Add(const std::string& key,
	const DirectX::SimpleMath::Vector2& position,
	const DirectX::SimpleMath::Vector2& scale,
	KumachiLib::ANCHOR anchor, UIType type)
{
	std::unique_ptr<UI> userInterface = std::make_unique<UI>(m_pCommonResources);// メニューとしてアイテムを追加する
	userInterface->Create(m_pDR, key, position, scale, anchor);// 指定された画像を表示するためのアイテムを作成する
	userInterface->SetWindowSize(m_windowWidth, m_windowHeight);// ウィンドウのサイズを設定する
	if (type == UIType::SELECT)// 選択可能なアイテムなら
	{
		m_pUI.push_back(std::move(userInterface));//  選択可能アイテムを新しく追加

		std::unique_ptr<UI> base = std::make_unique<UI>(m_pCommonResources);//  背景用のウィンドウ画像も追加する
		base->Create(m_pDR, "Select", position, scale, anchor);// 指定された画像を表示するためのアイテムを作成する
		base->SetWindowSize(m_windowWidth, m_windowHeight);// ウィンドウのサイズを設定する
		m_pSelect.push_back(std::move(base));// 背景用のアイテムも新しく追加する
		m_transforms.push_back({ position, scale });// UIの情報を配列に登録
	}
	else// 選択不可なアイテムなら
	{
		m_pGuide.push_back(std::move(userInterface));// 選択不可能アイテムを新しく追加
		return;
	}
}
