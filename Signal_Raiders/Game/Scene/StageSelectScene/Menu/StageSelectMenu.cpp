/*
*	@file	StageSelectMenu.cpp
*	@brief	メニュークラス
*/
#include <pch.h>
#include "StageSelectMenu.h"
// 無効なメニューインデックス
const int StageSelectMenu::INVALID_MENU_INDEX = 6;
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
	, m_pCommonResources{ nullptr }// 共通リソースへのポインタ
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
*	@details ステージセレクトメニュークラスのデストラクタ(ここでは何もしない)
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
	// 共通リソースをセット
	m_pCommonResources = resources;
	// デバイスリソース取得
	m_pDR = m_pCommonResources->GetDeviceResources();
	// ウィンドウ幅
	m_windowWidth = width;
	// ウィンドウ高さ
	m_windowHeight = height;
	// 選択枠のテクスチャパス設定
	m_pSelectTexturePath = L"Resources/Textures/StageSelect.png";
	//「ステージ1の写真」を読み込む
	Add("Stage1"
		, Vector2(Screen::CENTER_X - 550, Screen::CENTER_Y - 70)
		, Vector2(.7, .7)
		, KumachiLib::ANCHOR::MIDDLE_CENTER
		, UIType::SELECT);
	//「ステージ2の写真」を読み込む
	Add("Stage2"
		, Vector2(Screen::CENTER_X, Screen::CENTER_Y - 70)
		, Vector2(.7, .7)
		, KumachiLib::ANCHOR::MIDDLE_CENTER
		, UIType::SELECT);
	//「ステージ3の写真」を読み込む
	Add("Stage3"
		, Vector2(Screen::CENTER_X + 550, Screen::CENTER_Y - 70)
		, Vector2(.7, .7)
		, KumachiLib::ANCHOR::MIDDLE_CENTER
		, UIType::SELECT);
	//「ステージ4の写真」を読み込む
	Add("Stage4"
		, Vector2(Screen::CENTER_X - 550, Screen::CENTER_Y + 250)
		, Vector2(.7, .7)
		, KumachiLib::ANCHOR::MIDDLE_CENTER
		, UIType::SELECT);
	//「ステージ5の写真」を読み込む
	Add("Stage5"
		, Vector2(Screen::CENTER_X, Screen::CENTER_Y + 250)
		, Vector2(.7, .7)
		, KumachiLib::ANCHOR::MIDDLE_CENTER
		, UIType::SELECT);
	//「タイトルに戻る」を読み込む
	Add("ToTitle"
		, Vector2(Screen::CENTER_X + 550, Screen::CENTER_Y + 250)
		, Vector2(.7, .7)
		, KumachiLib::ANCHOR::MIDDLE_CENTER
		, UIType::SELECT);
	//「操作説明」を読み込む
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
void StageSelectMenu::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;
	// マウストラッカーを取得する
	auto& mtracker = m_pCommonResources->GetInputManager()->GetMouseTracker();
	// マウスの現在の状態を取得する
	auto& mouseState = m_pCommonResources->GetInputManager()->GetMouseState();
	// 未選択状態にする
	m_hit = false;
	// マウス座標を2Dベクトルで取得する
	Vector2 mousePos = Vector2(static_cast<float>(mouseState.x), static_cast<float>(mouseState.y));
	// UIの各要素に対してヒット判定を行う
	for (int i = 0; i < m_pUI.size(); i++)
	{
		// マウスの座標がUIに重なっているか確認する
		if (m_pUI[i]->IsHit(mousePos))
		{
			// ヒットフラグを立てる
			m_hit = true;
			// 選択メニューが前回と違う場合、SE再生フラグをリセット
			if ((int(m_menuIndex)) != i)m_isSEPlay = false;
			// SEが未再生なら
			if (!m_isSEPlay)
			{
				// 効果音を再生する
				m_pCommonResources->GetAudioManager()->PlaySound("Select", m_SEVolume);
				// SE再生フラグを立てる
				m_isSEPlay = true;
			}
			// 現在選択されているメニューのインデックスを更新する
			m_menuIndex = i;
			// 1つでもヒットしたら他は無視する（選択は1つだけ）
			break;
		}
	}
	// 経過時間を加算する
	m_time += elapsedTime;
	// 何もヒットしていなければインデックスを無効にする
	if (!m_hit)	m_menuIndex = INVALID_MENU_INDEX;
	// 左クリックされたら選択されたメニューに対応するシーンIDを設定する
	if (mtracker->GetLastState().leftButton)m_num = static_cast<SceneID>(m_menuIndex);
	// UI要素の状態を更新する 
	for (int i = 0; i < m_pUI.size(); i++)
	{
		// 背景用の選択ウィンドウのスケールを更新する
		m_pSelect[i]->SetScale(m_pSelect[i]->GetSelectScale());
		// 経過時間を渡してアニメ進行
		m_pSelect[i]->SetTime(m_pSelect[i]->GetTime() + elapsedTime);
		// メニュー本体のスケールを更新する
		m_pUI[i]->SetScale(m_pUI[i]->GetSelectScale());
		// 経過時間を渡してアニメ進行
		m_pUI[i]->SetTime(m_pUI[i]->GetTime() + elapsedTime);
	}
	// 非選択対象のUIのアニメ状態も更新する
	for (int i = 0; i < m_pGuide.size(); i++)
	{
		// スケール更新する
		m_pGuide[i]->SetScale(m_pGuide[i]->GetSelectScale());
		// 経過時間を更新する
		m_pGuide[i]->SetTime(m_pGuide[i]->GetTime() + elapsedTime);
	}
	// 選択されたメニューがない場合は何もしないで戻る
	if (m_menuIndex == INVALID_MENU_INDEX)	return;
	// 選択されたUIの元のスケールを取得する
	Vector2 select = m_pUI[m_menuIndex]->GetSelectScale();
	// スケールの補間
	Vector2 selectScale = Vector2::Lerp(m_pUI[m_menuIndex]->GetSelectScale(), Vector2::One, 1);
	// 時間に応じてsin波で変化するスケールを生成する（選択中の演出）
	select = Vector2((sin(m_time) * 0.1f) + 1.0f);
	// UI本体に変化スケールを反映する
	m_pUI[m_menuIndex]->SetScale(select);
	// 背景ウィンドウにも同様のスケールを適用する
	m_pSelect[m_menuIndex]->SetScale(select);
}
/*
*	@brief メニューを描画する
*	@details ステージセレクトメニューの描画を行う
*	@param なし
*	@return なし
*/
void StageSelectMenu::Render()
{
	// UIの数だけ繰り返す
	for (unsigned int i = 0; i < m_pUI.size(); i++)
	{
		// シェーダータイプを設定
		m_pUI[i]->SetShaderType(UI::ShaderType::STAGE_SELECT);
		// 描画
		m_pUI[i]->Render();
	}
	// 選択不可能なアイテムの画像を表示
	for (unsigned int i = 0; i < m_pGuide.size(); i++)
	{
		// シェーダータイプを設定
		m_pGuide[i]->SetShaderType(UI::ShaderType::NORMAL);
		// 描画
		m_pGuide[i]->Render();
	}
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
void StageSelectMenu::Add(const std::string& key,
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
	// UIの種類に応じて処理を分岐
	if (type == UIType::SELECT)// 選択可能なアイテムなら
	{
		// アイテムを新しく追加		
		m_pUI.push_back(std::move(userInterface));
		// 背景用のウィンドウ画像も追加する
		std::unique_ptr<UI> base = std::make_unique<UI>(m_pCommonResources);
		// 指定画像でUI作成
		base->Create(m_pDR, "StageSelect", position, scale, anchor);
		// ウィンドウサイズを設定
		base->SetWindowSize(m_windowWidth, m_windowHeight);
		// 背景用のアイテムも新しく追加する
		m_pSelect.push_back(std::move(base));
	}
	else// 選択不可なアイテムなら
	{
		// 選択不可能なアイテムを追加
		m_pGuide.push_back(std::move(userInterface));
		return;
	}
}
