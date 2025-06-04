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
	// 共通リソースをセット
	m_pCommonResources = resources;
	// デバイスリソース取得
	m_pDR = m_pCommonResources->GetDeviceResources();
	// ウィンドウ幅保存
	m_windowWidth = width;
	// ウィンドウ高さ保存
	m_windowHeight = height;
	// 「もういっかいやる」を読み込む
	Add("RePlay"
		, Vector2(Screen::CENTER_X, Screen::CENTER_Y + 250)
		, Vector2(.5, .5)
		, KumachiLib::ANCHOR::MIDDLE_CENTER
		, UIType::SELECT);
	// 「ステージをえらぶ」を読み込む
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
*	@brief メニューを更新する
*	@details マウス位置に応じたメニューの選択状態更新とクリック判定を行う
*	@param elapsedTime 前フレームからの経過時間(秒)
*	@return なし
*/
void ResultMenu::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;
	// マウスのトラッカーを取得する
	auto& mtracker = m_pCommonResources->GetInputManager()->GetMouseTracker();
	// マウスの状態を取得
	auto& mouseState = m_pCommonResources->GetInputManager()->GetMouseState();
	// 時間を加算
	m_time += elapsedTime;
	// 何かにヒットしたかどうかを初期化
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
				// 選択音の再生
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
	// ヒット無しなら選択インデックスを無効値に設定
	if (!m_hit) m_menuIndex = INVALID_MENU_INDEX;
	// 左クリックされたら選択メニューのシーンIDを更新
	if (mtracker->GetLastState().leftButton)m_num = static_cast<SceneID>(m_menuIndex);
	//  メニューアイテムの選択先を更新
	for (int i = 0; i < m_pUI.size(); i++)
	{
		// アイテムの選択状態を更新
		// 選択状態のスケールを取得
		m_pSelect[i]->SetScale(m_pSelect[i]->GetSelectScale());
		// 時間を加算
		m_pSelect[i]->SetTime(m_pSelect[i]->GetTime() + elapsedTime);
		// 選択状態のスケールを取得
		m_pUI[i]->SetScale(m_pUI[i]->GetSelectScale());
		// 時間を加算
		m_pUI[i]->SetTime(m_pUI[i]->GetTime() + elapsedTime);
	}

	// 選択不可能なアイテムの選択状態を更新
	for (int i = 0; i < m_pGuide.size(); i++)
	{
		// アイテムの選択状態を更新
		// 選択状態のスケールを取得
		m_pGuide[i]->SetScale(m_pGuide[i]->GetSelectScale());
		// 時間を加算
		m_pGuide[i]->SetTime(m_pGuide[i]->GetTime() + elapsedTime);
	}
	// ヒットしていなければ何もしない
	if (!m_hit)return;
	// 選択中の初期サイズを取得する
	Vector2 select = m_pUI[m_menuIndex]->GetSelectScale();
	// 選択状態とするための変化用サイズを算出する
	Vector2 selectScale = Vector2::Lerp(m_pUI[m_menuIndex]->GetSelectScale(), Vector2::One, 1);
	// 選択中メニューのスケールを時間経過に応じて揺らす（波のように0.1倍増減）
	select = Vector2((sin(m_time) * 0.1f) + 1.0f);
	// 算出後のサイズを現在のサイズとして設定する
	m_pUI[m_menuIndex]->SetScale(select);
	// 背景用のウィンドウ画像にも同じ割合の値を設定する
	m_pSelect[m_menuIndex]->SetScale(Vector2::One);
}
/*
	@brief メニューを描画する
	@details メニューアイテムの画像と背景枠を描画する
	@param なし
	@return なし
*/
void ResultMenu::Render()
{
	// メニューアイテムの数だけ繰り返す
	for (unsigned int i = 0; i < m_pUI.size(); i++)
	{
		// アイテム用ウィンドウ背景を表示
		if (i == m_menuIndex) m_pSelect[i]->Render();
		// 実際に表示したいアイテム画像を表示
		m_pUI[i]->Render();
	}
	// 選択不可能なアイテムの画像を表示
	for (unsigned int i = 0; i < m_pGuide.size(); i++)m_pGuide[i]->Render();
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
void ResultMenu::Add(const std::string& key,
	const DirectX::SimpleMath::Vector2& position,
	const DirectX::SimpleMath::Vector2& scale,
	KumachiLib::ANCHOR anchor, UIType type)
{
	// UIオブジェクトの生成
	std::unique_ptr<UI> userInterface = std::make_unique<UI>(m_pCommonResources);
	// 指定画像でUI作成
	userInterface->Create(m_pDR, key, position, scale, anchor);
	// ウィンドウサイズを設定
	userInterface->SetWindowSize(m_windowWidth, m_windowHeight);
	// UIのタイプに応じて処理を分岐
	if (type == UIType::SELECT)// 選択可能なアイテムなら
	{
		// アイテムを新しく追加
		m_pUI.push_back(std::move(userInterface));
		// 背景用の選択枠も追加する
		std::unique_ptr<UI> back = std::make_unique<UI>(m_pCommonResources);
		// 指定画像でUI作成
		back->Create(m_pDR, "ResultSelect", position, scale, anchor);
		// ウィンドウサイズを設定
		back->SetWindowSize(m_windowWidth, m_windowHeight);
		// 背景用のアイテムも新しく追加する
		m_pSelect.push_back(std::move(back));
	}
	else// 選択不可なアイテムなら
	{
		// 選択不可能なアイテムを追加
		m_pGuide.push_back(std::move(userInterface));
		return;
	}
}