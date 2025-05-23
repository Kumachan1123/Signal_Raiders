/*
*	@file	StageSelectMenu.cpp
*	@brief	メニュークラス
*/
#include "pch.h"
#include "StageSelectMenu.h"

// 無効なメニューインデックス
const int StageSelectMenu::INVALID_MENU_INDEX = 6;// 無効なメニューインデックス
/*
*	@brief コンストラクタ
*	@details ステージセレクトメニュークラスのコンストラクタ
*	@param なし
*	@return なし
*
*/
StageSelectMenu::StageSelectMenu()
	: m_menuIndex{ 0 }// 現在選択中のメニューインデックス
	, m_pDR{ nullptr }// デバイスリソースへのポインタ
	, m_commonResources{ nullptr }// 共通リソースへのポインタ
	, m_pUI{}// UI
	, m_pSelect{}// 選択された時に表示する背景UI
	, m_pSelectTexturePath{ nullptr }// 選択背景のテクスチャパス
	, m_windowWidth{ 0 }// ウィンドウ幅
	, m_windowHeight{ 0 }// ウィンドウ高さ
	, m_num{ SceneID::PLAY }// シーンID
	, m_time{ 0.0f }// 時間
	, m_hit{ false }// ヒットフラグ
	, m_SEVolume{ 0.0f }// SE音量
	, m_isSEPlay{ false }// SE再生フラグ
{
}
/*
*	@brief デストラクタ
*	@details ステージセレクトメニュークラスのデストラクタ
*	@param なし
*	@return なし
*/
StageSelectMenu::~StageSelectMenu() {/*do nothing*/ }
/*
*	@brief メニューを初期化する
*	@details メニューアイテムの画像を読み込み、位置・スケール・アンカーを設定する
*	@param resources 共通リソースへのポインタ
*	@param width ウィンドウの幅
*	@param height ウィンドウの高さ
*	@return なし
*/
void StageSelectMenu::Initialize(CommonResources* resources, int width, int height)
{
	using namespace DirectX::SimpleMath;
	m_commonResources = resources;// 共通リソースをセット
	m_pDR = m_commonResources->GetDeviceResources();// デバイスリソース取得
	m_windowWidth = width;// ウィンドウ幅
	m_windowHeight = height;// ウィンドウ高さ
	m_pSelectTexturePath = L"Resources/Textures/StageSelect.png";// 選択枠のテクスチャパス設定
	//「ステージ1の写真」を読み込む
	Add(L"Resources/Textures/stage1.png"
		, Vector2(Screen::CENTER_X - 550, Screen::CENTER_Y - 70)
		, Vector2(.7, .7)
		, KumachiLib::ANCHOR::MIDDLE_CENTER
		, UIType::SELECT);
	//「ステージ2の写真」を読み込む
	Add(L"Resources/Textures/stage2.png"
		, Vector2(Screen::CENTER_X, Screen::CENTER_Y - 70)
		, Vector2(.7, .7)
		, KumachiLib::ANCHOR::MIDDLE_CENTER
		, UIType::SELECT);
	//「ステージ3の写真」を読み込む
	Add(L"Resources/Textures/stage3.png"
		, Vector2(Screen::CENTER_X + 550, Screen::CENTER_Y - 70)
		, Vector2(.7, .7)
		, KumachiLib::ANCHOR::MIDDLE_CENTER
		, UIType::SELECT);
	//「ステージ4の写真」を読み込む
	Add(L"Resources/Textures/stage4.png"
		, Vector2(Screen::CENTER_X - 550, Screen::CENTER_Y + 250)
		, Vector2(.7, .7)
		, KumachiLib::ANCHOR::MIDDLE_CENTER
		, UIType::SELECT);
	//「ステージ5の写真」を読み込む
	Add(L"Resources/Textures/stage5.png"
		, Vector2(Screen::CENTER_X, Screen::CENTER_Y + 250)
		, Vector2(.7, .7)
		, KumachiLib::ANCHOR::MIDDLE_CENTER
		, UIType::SELECT);
	//「タイトルに戻る」を読み込む
	Add(L"Resources/Textures/ToTitle.png"
		, Vector2(Screen::CENTER_X + 550, Screen::CENTER_Y + 250)
		, Vector2(.7, .7)
		, KumachiLib::ANCHOR::MIDDLE_CENTER
		, UIType::SELECT);
	//「操作説明」を読み込む
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
void StageSelectMenu::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;
	const auto& kbTracker = m_commonResources->GetInputManager()->GetKeyboardTracker();// キーボードのトラッカーを取得する
	auto& mtracker = m_commonResources->GetInputManager()->GetMouseTracker();// マウスのトラッカーを取得する
	auto& mouseState = m_commonResources->GetInputManager()->GetMouseState();// マウスの状態を取得
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
				m_commonResources->GetAudioManager()->PlaySound("Select", m_SEVolume);// SEの再生
				m_isSEPlay = true;// 再生フラグを立てる
			}
			m_menuIndex = i;// ヒットしたメニューのインデックスを保存
			break;	// ヒットしたらループを抜ける
		}
	}
	m_time += elapsedTime;// 時間を加算する
	if (!m_hit)m_menuIndex = INVALID_MENU_INDEX;// もし一個もヒットしてなかったら選択なしにする
	if (m_hit && kbTracker->pressed.Space || mtracker->GetLastState().leftButton)m_num = static_cast<SceneID>(m_menuIndex);// 左クリックされたら選択メニューのシーンIDを更新
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
	m_pSelect[m_menuIndex]->SetScale(select);// 背景用のウィンドウ画像にも同じ割合の値を設定する
}
/*
*	@brief メニューを描画する
*	@details ステージセレクトメニューの描画を行う
*	@param なし
*	@return なし
*/
void StageSelectMenu::Render()
{
	for (unsigned int i = 0; i < m_pUI.size(); i++)// UIの数だけ繰り返す
	{
		m_pUI[i]->SetShaderType(UI::ShaderType::STAGE_SELECT);// シェーダータイプを設定
		m_pUI[i]->Render();	// 描画
	}
	for (unsigned int i = 0; i < m_pGuide.size(); i++)// 選択不可能なアイテムの画像を表示
	{
		m_pGuide[i]->SetShaderType(UI::ShaderType::NORMAL);// シェーダータイプを設定
		m_pGuide[i]->Render();// 描画
	}
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
void StageSelectMenu::Add(const wchar_t* path, DirectX::SimpleMath::Vector2 position, DirectX::SimpleMath::Vector2 scale, KumachiLib::ANCHOR anchor, UIType type)
{
	std::unique_ptr<UI> userInterface = std::make_unique<UI>();// UIオブジェクトの生成
	userInterface->Create(m_pDR, path, position, scale, anchor);// 指定画像でUI作成
	userInterface->SetWindowSize(m_windowWidth, m_windowHeight);// ウィンドウサイズを設定
	if (type == UIType::SELECT)// 選択可能なアイテムなら
	{
		m_pUI.push_back(std::move(userInterface));// アイテムを新しく追加		
		std::unique_ptr<UI> base = std::make_unique<UI>();// 背景用のウィンドウ画像も追加する
		base->Create(m_pDR, m_pSelectTexturePath, position, scale, anchor);// 指定画像でUI作成
		base->SetWindowSize(m_windowWidth, m_windowHeight);// ウィンドウサイズを設定
		m_pSelect.push_back(std::move(base));// 背景用のアイテムも新しく追加する
	}
	else// 選択不可なアイテムなら
	{
		m_pGuide.push_back(std::move(userInterface));// 選択不可能なアイテムを追加
		return;
	}
}
