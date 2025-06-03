/*
*	@file   ResultMenu.cpp
*   @brief  リザルト画面のメニューを管理するクラス
*/
#include <pch.h>
#include "ResultMenu.h"

// 無効なメニューインデックス
const int ResultMenu::INVALID_MENU_INDEX = 6;// 無効なメニューインデックス

/*
*	@brief コンストラクタ
*	@details リザルトメニュークラスのコンストラクタ
*	@param なし
*	@return なし
*/
ResultMenu::ResultMenu()
	: m_menuIndex{ 0 }						// 現在選択中のメニューインデックス
	, m_pDR{ nullptr }						// デバイスリソースへのポインタ
	, m_pCommonResources{ nullptr }			// 共通リソースへのポインタ
	, m_pUI{}								// UIオブジェクトリスト
	, m_pSelect{}							// 選択された時に表示する背景UIリスト
	, m_pSelectTexturePath{ nullptr }		// 選択背景のテクスチャパス
	, m_windowWidth{ 0 }					// ウィンドウ幅
	, m_windowHeight{ 0 }					// ウィンドウ高さ
	, m_num{ SceneID::REPLAY }				// 選ばれたシーン（初期はREPLAY）
	, m_hit{ false }						// マウスがUIにヒットしたかどうか
	, m_time{ 0.0f }						// 時間
	, m_SEVolume{ 0.0f }					// SE音量
	, m_isSEPlay{ false }					// SE再生フラグ
{
}
/*
*	@brief デストラクタ
*	@details リザルトメニュークラスのデストラクタ
*	@param なし
*	@return なし
*/
ResultMenu::~ResultMenu()
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
	@brief メニューを初期化する
	@details メニューアイテムの画像を読み込み、位置・スケール・アンカーを設定する
	@param resources 共通リソースへのポインタ
	@param width ウィンドウの幅
	@param height ウィンドウの高さ
	@return なし
*/
void ResultMenu::Initialize(CommonResources* resources, int width, int height)
{
	using namespace DirectX::SimpleMath;
	m_pCommonResources = resources;// 共通リソースをセット
	m_pDR = m_pCommonResources->GetDeviceResources();// デバイスリソース取得
	m_windowWidth = width;// ウィンドウ幅保存
	m_windowHeight = height;// ウィンドウ高さ保存
	m_pSelectTexturePath = L"Resources/Textures/ResultSelect.png";// 選択枠のテクスチャパス設定
	//  「もういっかいやる」を読み込む
	Add("RePlay"
		, Vector2(Screen::CENTER_X, Screen::CENTER_Y + 250)
		, Vector2(.5, .5)
		, KumachiLib::ANCHOR::MIDDLE_CENTER
		, UIType::SELECT);
	// 　「ステージをえらぶ」を読み込む
	Add("ToStageSelect"
		, Vector2(Screen::CENTER_X, Screen::CENTER_Y + 400)
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
	@brief メニューを更新する
	@details マウス位置に応じたメニューの選択状態更新とクリック判定を行う
	@param elapsedTime 前フレームからの経過時間(秒)
	@return なし
*/
void ResultMenu::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;
	auto& mtracker = m_pCommonResources->GetInputManager()->GetMouseTracker(); // マウスのトラッカーを取得する
	auto& mouseState = m_pCommonResources->GetInputManager()->GetMouseState();// マウスの状態を取得
	m_time += elapsedTime;// 時間を加算
	m_hit = false; // 何かにヒットしたかどうかを初期化
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
			m_menuIndex = i;	// ヒットしたメニューのインデックスを保存
			break;	// ヒットしたらループを抜ける
		}
	}
	if (!m_hit) m_menuIndex = INVALID_MENU_INDEX;// ヒット無しなら選択インデックスを無効値に設定
	if (mtracker->GetLastState().leftButton)m_num = static_cast<SceneID>(m_menuIndex);// 左クリックされたら選択メニューのシーンIDを更新
	for (int i = 0; i < m_pUI.size(); i++)//  メニューアイテムの選択先を更新
	{
		// アイテムの選択状態を更新
		m_pSelect[i]->SetScale(m_pSelect[i]->GetSelectScale());// 選択状態のスケールを取得
		m_pSelect[i]->SetTime(m_pSelect[i]->GetTime() + elapsedTime);// 時間を加算
		m_pUI[i]->SetScale(m_pUI[i]->GetSelectScale()); // 選択状態のスケールを取得
		m_pUI[i]->SetTime(m_pUI[i]->GetTime() + elapsedTime);// 時間を加算
	}

	for (int i = 0; i < m_pGuide.size(); i++)// 選択不可能なアイテムの選択状態を更新
	{
		// アイテムの選択状態を更新
		m_pGuide[i]->SetScale(m_pGuide[i]->GetSelectScale());// 選択状態のスケールを取得
		m_pGuide[i]->SetTime(m_pGuide[i]->GetTime() + elapsedTime);	// 時間を加算
	}
	if (!m_hit)return;// ヒットしていなければ何もしない
	Vector2 select = m_pUI[m_menuIndex]->GetSelectScale();// 選択中の初期サイズを取得する
	Vector2 selectScale = Vector2::Lerp(m_pUI[m_menuIndex]->GetSelectScale(), Vector2::One, 1);// 選択状態とするための変化用サイズを算出する
	select = Vector2((sin(m_time) * 0.1f) + 1.0f);// 選択中メニューのスケールを時間経過に応じて揺らす（波のように0.1倍増減）
	m_pUI[m_menuIndex]->SetScale(select);// 算出後のサイズを現在のサイズとして設定する
	m_pSelect[m_menuIndex]->SetScale(Vector2::One);// 背景用のウィンドウ画像にも同じ割合の値を設定する
}
/*
	@brief メニューを描画する
	@details メニューアイテムの画像と背景枠を描画する
	@param なし
	@return なし
*/
void ResultMenu::Render()
{
	for (unsigned int i = 0; i < m_pUI.size(); i++)// メニューアイテムの数だけ繰り返す
	{
		if (i == m_menuIndex) m_pSelect[i]->Render(); // アイテム用ウィンドウ背景を表示
		m_pUI[i]->Render();// 実際に表示したいアイテム画像を表示
	}
	for (unsigned int i = 0; i < m_pGuide.size(); i++)m_pGuide[i]->Render();// 選択不可能なアイテムの画像を表示
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
void ResultMenu::Add(std::string key,
	const DirectX::SimpleMath::Vector2& position,
	const DirectX::SimpleMath::Vector2& scale,
	KumachiLib::ANCHOR anchor, UIType type)
{
	std::unique_ptr<UI> userInterface = std::make_unique<UI>(m_pCommonResources);// UIオブジェクトの生成
	userInterface->Create(m_pDR, key, position, scale, anchor);// 指定画像でUI作成
	userInterface->SetWindowSize(m_windowWidth, m_windowHeight);// ウィンドウサイズを設定
	if (type == UIType::SELECT)// 選択可能なアイテムなら
	{
		m_pUI.push_back(std::move(userInterface));// アイテムを新しく追加
		std::unique_ptr<UI> back = std::make_unique<UI>(m_pCommonResources);// 背景用の選択枠も追加する
		back->Create(m_pDR, "ResultSelect", position, scale, anchor);// 指定画像でUI作成
		back->SetWindowSize(m_windowWidth, m_windowHeight);// ウィンドウサイズを設定
		m_pSelect.push_back(std::move(back)); // 背景用のアイテムも新しく追加する
	}
	else// 選択不可なアイテムなら
	{
		m_pGuide.push_back(std::move(userInterface));//  選択不可能なアイテムを追加
		return;
	}
}
