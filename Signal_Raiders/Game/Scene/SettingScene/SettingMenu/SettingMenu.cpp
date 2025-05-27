/*
	@file	SettingMenu.cpp
	@brief	セッティングメニュークラス
*/
#include <pch.h>
#include "SettingMenu.h"
// 無効なメニューインデックス
const int SettingMenu::INVALID_MENU_INDEX = 6;// 無効なメニューインデックス
/*
*	@brief コンストラクタ
*	@details セッティングメニュークラスのコンストラクタ
*	@param なし
*	@return なし
*/
SettingMenu::SettingMenu()
	: m_menuIndex{ 0 }// 現在選択中のメニューインデックス
	, m_time{ 0.0f }// 時間
	, m_pDR{ nullptr }// デバイスリソースへのポインタ
	, m_pCommonResources{ nullptr }// 共通リソースへのポインタ
	, m_pUI{}// UI
	, m_pSelect{}// 選択された時に表示する背景UI
	, m_pSelectTexturePath{ nullptr }// 選択背景のテクスチャパス
	, m_pSelectWindow{ nullptr }// 選択ウィンドウ
	, m_windowWidth{ 0 }// ウィンドウ幅
	, m_windowHeight{ 0 }// ウィンドウ高さ
	, m_selectNum{ SelectID::NONE }// 選択ID
	, m_SEVolume{ 0.0f }// SE音量
	, m_isSEPlay{ false }// SE再生フラグ
{
}
/*
*	@brief デストラクタ
*	@details セッティングメニュークラスのデストラクタ
*	@param なし
*/
SettingMenu::~SettingMenu()
{
	//  UIオブジェクトの解放
	for (auto& ui : m_pUI)ui.reset();
	m_pUI.clear();
	//  選択された時に表示する背景UIオブジェクトの解放
	for (auto& select : m_pSelect)select.reset();
	m_pSelect.clear();
	//  選択不可能なアイテムの解放
	for (auto& guide : m_pGuide)guide.reset();
	m_pGuide.clear();
}
/*
*	@brief メニューを初期化する
*	@details メニューアイテムの画像を読み込み、位置・スケール・アンカーを設定する
*	@param resources 共通リソースへのポインタ
*	@param width ウィンドウの幅
*	@param height ウィンドウの高さ
*	@return なし
*/
void SettingMenu::Initialize(CommonResources* resources, int width, int height)
{
	using namespace DirectX::SimpleMath;
	m_pCommonResources = resources;// 共通リソースをセット
	m_pDR = m_pCommonResources->GetDeviceResources();// デバイスリソース取得
	m_windowWidth = width;// ウィンドウ幅
	m_windowHeight = height;// ウィンドウ高さ
	m_pSelectTexturePath = L"Resources/Textures/select.png";// 選択枠のテクスチャパス設定
	//  「BGM」を読み込む
	Add(L"Resources/Textures/BGM.png"
		, Vector2(Screen::LEFT + 400, Screen::CENTER_Y - 300)
		, Vector2(.75, .75)
		, KumachiLib::ANCHOR::MIDDLE_CENTER
		, UIType::NON_SELECT);
	//  「SE」を読み込む
	Add(L"Resources/Textures/SE.png"
		, Vector2(Screen::LEFT + 400, Screen::CENTER_Y - 150)
		, Vector2(.75, .75)
		, KumachiLib::ANCHOR::MIDDLE_CENTER
		, UIType::NON_SELECT);
	//  「マウスかんど」を読み込む
	Add(L"Resources/Textures/Mouse.png"
		, Vector2(Screen::LEFT + 400, Screen::CENTER_Y)
		, Vector2(.75, .75)
		, KumachiLib::ANCHOR::MIDDLE_CENTER
		, UIType::NON_SELECT);
	//  「へんこう」を読み込む
	Add(L"Resources/Textures/Apply.png"
		, Vector2(Screen::LEFT + 400, Screen::CENTER_Y + 150)
		, Vector2(.75, .75)
		, KumachiLib::ANCHOR::MIDDLE_CENTER
		, UIType::SELECT);
	//  「おわる」を読み込む
	Add(L"Resources/Textures/end.png"
		, Vector2(Screen::LEFT + 400, Screen::CENTER_Y + 300)
		, Vector2(.75, .75)
		, KumachiLib::ANCHOR::MIDDLE_CENTER
		, UIType::SELECT);
	// 「操作説明」を読み込む
	Add(L"Resources/Textures/Guide.png"
		, Vector2(Screen::RIGHT, Screen::BOTTOM)
		, Vector2(1, 1)
		, KumachiLib::ANCHOR::BOTTOM_RIGHT
		, UIType::NON_SELECT);
}
/*
*	@brief メニューを更新する
*	@details マウス位置に応じたメニューの選択状態更新とクリック判定を行う
*	@param elapsedTime 前フレームからの経過時間(秒)
*	@return なし
*/
void SettingMenu::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;
	auto& mtracker = m_pCommonResources->GetInputManager()->GetMouseTracker();// マウスのトラッカーを取得する
	auto& mouseState = m_pCommonResources->GetInputManager()->GetMouseState();// マウスの状態を取得
	m_time += elapsedTime;// 時間を加算する
	bool hit = false; // 何かにヒットしたかどうか
	Vector2 mousePos = Vector2(static_cast<float>(mouseState.x), static_cast<float>(mouseState.y));// マウスの座標を取得
	for (int i = 0; i < m_pUI.size(); i++)// メニューアイテムの数だけ繰り返す
	{
		if (m_pUI[i]->IsHit(mousePos))// マウスの座標がアイテムの範囲内にあるかどうかを判定
		{
			hit = true;	// ヒットフラグを立てる
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
	if (!hit) { m_menuIndex = INVALID_MENU_INDEX; }// もし一個もヒットしてなかったら、選択なしにする
	if (mtracker->GetLastState().leftButton)m_selectNum = static_cast<SelectID>(m_menuIndex);// 左クリックされたら選択メニューのシーンIDを更新
	for (int i = 0; i < m_pUI.size(); i++)//  メニューアイテムの選択先を更新
	{
		m_pUI[i]->SetScale(m_pUI[i]->GetSelectScale());// 選択状態のスケールを取得
		m_pUI[i]->SetTime(m_pUI[i]->GetTime() + elapsedTime);// アイテムの選択状態を更新
	}
	for (int i = 0; i < m_pGuide.size(); i++)// 選択不可能なアイテムの選択状態を更新
	{
		m_pGuide[i]->SetScale(m_pGuide[i]->GetSelectScale());// スケールを取得
		m_pGuide[i]->SetTime(m_pGuide[i]->GetTime() + elapsedTime);// 時間を加算
	}
	if (!hit)return;// 当たってなかったら終了
	Vector2 select = m_pUI[m_menuIndex]->GetSelectScale();// 選択中の初期サイズを取得する
	Vector2 selectScale = Vector2::Lerp(m_pUI[m_menuIndex]->GetSelectScale(), Vector2::One, 1);// 選択状態とするための変化用サイズを算出する
	select = Vector2((sin(m_time) * 0.1f) + 1.0f);// 選択状態は初期状態＋50％の大きさとする
	m_pUI[m_menuIndex]->SetScale(select);// 算出後のサイズを現在のサイズとして設定する
}
/*
*	@brief メニューを描画する
*	@details セッティングメニューの描画を行う
*	@param なし
*	@return なし
*/
void SettingMenu::Render()
{
	for (unsigned int i = 0; i < m_pUI.size(); i++)m_pUI[i]->Render();// メニューアイテムの数だけ繰り返す
	for (unsigned int i = 0; i < m_pGuide.size(); i++)m_pGuide[i]->Render();// 選択不可能なアイテムの数だけ繰り返す
}
/*
	@brief メニューアイテムを追加する
	@details 指定の画像でUIを作成し、内部リストに追加する (選択可なら背景枠も作成)
	@param path 画像ファイルのパス
	@param position 描画位置
	@param scale 描画スケール
	@param anchor アンカー指定
	@param type メニューの種類（選択可/不可）
	@return なし
*/
void SettingMenu::Add(const wchar_t* path, DirectX::SimpleMath::Vector2 position, DirectX::SimpleMath::Vector2 scale, KumachiLib::ANCHOR anchor, UIType type)
{
	std::unique_ptr<UI> userInterface = std::make_unique<UI>();// UIオブジェクトの生成
	userInterface->Create(m_pDR, path, position, scale, anchor);// 指定画像でUI作成
	userInterface->SetWindowSize(m_windowWidth, m_windowHeight);// ウィンドウサイズを設定
	if (type == UIType::SELECT)// 選択可能なアイテムなら
	{
		m_pUI.push_back(std::move(userInterface));// アイテムを新しく追加
		std::unique_ptr<UI> base = std::make_unique<UI>();// 背景用の選択枠も追加する
		base->Create(m_pDR, m_pSelectTexturePath, position, scale, anchor);// 指定画像でUI作成
		base->SetWindowSize(m_windowWidth, m_windowHeight);// ウィンドウサイズを設定
	}
	else// 選択不可なアイテムなら
	{
		m_pGuide.push_back(std::move(userInterface));//  選択不可能なアイテムを追加
		return;
	}
}
