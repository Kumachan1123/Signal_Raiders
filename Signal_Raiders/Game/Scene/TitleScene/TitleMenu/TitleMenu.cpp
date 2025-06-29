/*
*	@file	TitleMenu.cpp
*	@brief	タイトルメニュークラス
*/
#include <pch.h>
#include "TitleMenu.h"
// 無効なメニューインデックス
const int TitleMenu::INVALID_MENU_INDEX = 6;
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
	// 共通リソースをセット
	m_pCommonResources = resources;
	// デバイスリソース取得
	m_pDR = m_pCommonResources->GetDeviceResources();
	// ウィンドウ幅
	m_windowWidth = width;
	// ウィンドウ高さ
	m_windowHeight = height;
	// 「プレイ」を読み込む
	Add("Play"
		, Vector2(Screen::CENTER_X, Screen::CENTER_Y + 250)
		, Vector2(.5, .5)
		, KumachiLib::ANCHOR::MIDDLE_CENTER
		, UIType::SELECT);
	// 「せってい」を読み込む
	Add("Setting"
		, Vector2(Screen::CENTER_X, Screen::CENTER_Y + 350)
		, Vector2(.5, .5)
		, KumachiLib::ANCHOR::MIDDLE_CENTER
		, UIType::SELECT);
	// 「おわる」を読み込む
	Add("Cancel"
		, Vector2(Screen::CENTER_X, Screen::CENTER_Y + 450)
		, Vector2(.5, .5)
		, KumachiLib::ANCHOR::MIDDLE_CENTER
		, UIType::SELECT);
	// 「操作説明」を読み込む
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
	// マウスのトラッカーを取得する
	auto& mtracker = m_pCommonResources->GetInputManager()->GetMouseTracker();
	// マウスの状態を取得
	auto& mouseState = m_pCommonResources->GetInputManager()->GetMouseState();
	// 何かにヒットしたか
	m_hit = false;
	// マウスの座標を取得
	Vector2 mousePos = Vector2(static_cast<float>(mouseState.x), static_cast<float>(mouseState.y));
	// メニューアイテムの数だけ繰り返す
	for (int i = 0; i < m_pUI.size(); i++)
	{
		// マウスの座標がアイテムの範囲内にあるかどうかを判定
		if (m_pUI[i]->IsHit(mousePos))
		{
			// ヒットフラグを立てる
			m_hit = true;
			// 前回選択したメニューと違う場合はSEを再生するフラグを立てる
			if ((int(m_menuIndex)) != i) m_isSEPlay = false;
			// SEが再生されていない場合
			if (!m_isSEPlay)
			{
				// SEの再生
				m_pCommonResources->GetAudioManager()->PlaySound("Select", m_SEVolume);
				// 再生フラグを立てる
				m_isSEPlay = true;
			}
			// ヒットしたメニューのインデックスを保存
			m_menuIndex = i;
			// ヒットしたらループを抜ける
			break;
		}
	}
	// 時間を加算する
	m_time += elapsedTime;
	// もし一個もヒットしてなかったら、選択なしにする
	if (!m_hit) { m_menuIndex = INVALID_MENU_INDEX; }
	// 左クリックされたら選択メニューのシーンIDを更新
	if (mtracker->GetLastState().leftButton)
		m_num = static_cast<SceneID>(m_menuIndex);
	// メニューアイテムの選択先を更新
	for (int i = 0; i < m_pUI.size(); i++)
	{
		//  アイテムの選択状態を更新
		// 選択状態のスケールを取得
		m_pSelect[i]->SetScale(m_pSelect[i]->GetSelectScale());
		// アイテムの選択状態を更新
		m_pSelect[i]->SetTime(m_pSelect[i]->GetTime() + elapsedTime);
		// 選択状態のスケールを取得
		m_pUI[i]->SetScale(m_pUI[i]->GetSelectScale());
		// アイテムの選択状態を更新
		m_pUI[i]->SetTime(m_pUI[i]->GetTime() + elapsedTime);
	}
	// 選択不可能なアイテムの選択状態を更新
	for (int i = 0; i < m_pGuide.size(); i++)
	{
		// スケールを取得
		m_pGuide[i]->SetScale(m_pGuide[i]->GetSelectScale());
		// 時間を加算
		m_pGuide[i]->SetTime(m_pGuide[i]->GetTime() + elapsedTime);
	}
	// なにも選択されていない場合は何もしない
	if (m_menuIndex == INVALID_MENU_INDEX)return;
	// 選択中の初期サイズを取得する
	Vector2 select = m_pUI[m_menuIndex]->GetSelectScale();
	// 選択状態とするための変化用サイズを算出する
	Vector2 selectScale = Vector2::Lerp(m_pUI[m_menuIndex]->GetSelectScale(), Vector2::One, 1);
	// 選択状態は初期状態より大きくする
	select = Vector2((sin(m_time) * 0.1f) + 1.0f);
	// 算出後のサイズを現在のサイズとして設定する
	m_pUI[m_menuIndex]->SetScale(select);
	// 背景用のウィンドウ画像にも同じ割合の値を設定する
	m_pSelect[m_menuIndex]->SetScale(Vector2::One);
}
/*
*	@brief	描画
*	@detail タイトルのUIを描画する
*	@param なし
*	@return なし
*/
void TitleMenu::Render()
{
	// 登録したUIの数ループ
	for (unsigned int i = 0; i < m_pUI.size(); i++)
	{
		// アイテム用ウィンドウ背景を表示
		if (i == m_menuIndex) m_pSelect[i]->Render();
		// 実際に表示したいアイテム画像を表示
		m_pUI[i]->Render();
	}
	// 選択不可能なアイテムを表示
	for (unsigned int i = 0; i < m_pGuide.size(); i++)m_pGuide[i]->Render();
#ifdef _DEBUG// デバッグモードの時のみ実行する
	// デバッグ情報を表示する
	auto debugString = m_pCommonResources->GetDebugString();
	// 登録したUIの数ループ
	for (int i = 0; i < m_transforms.size(); i++)
	{
		//  UIの座標を表示
		debugString->AddString("Transform.Pos:%f,%f  Scale:%f,%f",
			m_transforms[i].position.x,// X座標
			m_transforms[i].position.y,// Y座標
			m_transforms[i].scale.x,	// Xスケール
			m_transforms[i].scale.y);	// Yスケール
	}
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
	// 新しいUIアイテムを生成
	std::unique_ptr<UI> userInterface = std::make_unique<UI>(m_pCommonResources);
	// UIアイテムに対して画像・位置・スケール・アンカー情報を設定する
	userInterface->Create(m_pDR, key, position, scale, anchor);
	// ウィンドウサイズをUIアイテムに設定
	userInterface->SetWindowSize(m_windowWidth, m_windowHeight);
	// UIの種類に応じて処理を分岐する
	if (type == UIType::SELECT)// UIが選択可能なタイプだった場合
	{
		// メインのUIアイテムを選択可能UIリストに追加する
		m_pUI.push_back(std::move(userInterface));
		// 背景用のウィンドウUIを生成
		std::unique_ptr<UI> base = std::make_unique<UI>(m_pCommonResources);
		// 背景用ウィンドウに画像と位置情報などを設定する
		base->Create(m_pDR, "Select", position, scale, anchor);
		// 背景用ウィンドウのサイズも設定する
		base->SetWindowSize(m_windowWidth, m_windowHeight);
		// 背景ウィンドウを選択UI背景リストに追加する
		m_pSelect.push_back(std::move(base));
		// このUIアイテムの位置とスケールをトランスフォーム情報として記録する
		m_transforms.push_back({ position, scale });
	}
	else // 選択できないUI
	{
		// 選択不可UIリストに追加する
		m_pGuide.push_back(std::move(userInterface));
		// 処理を終える
		return;
	}
}
