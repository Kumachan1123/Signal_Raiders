/*
*	@file	SettingBar.cpp
*	@brief	設定バークラス（音量や感度などのスライダー）
*/
#include <pch.h>
#include "SettingBar.h"
// BGMの位置
const DirectX::SimpleMath::Vector2 SettingBar::BGM_POSITION = { Screen::CENTER_X + 300, Screen::CENTER_Y - 300 };
// SEの位置
const DirectX::SimpleMath::Vector2 SettingBar::SE_POSITION = { Screen::CENTER_X + 300, Screen::CENTER_Y - 150 };
// 感度の位置
const DirectX::SimpleMath::Vector2 SettingBar::SENSITIVITY_POSITION = { Screen::CENTER_X + 300, Screen::CENTER_Y };
// バーの幅
const float SettingBar::BAR_WIDTH = 300.0f;
// 玉の位置に除算する値
const float SettingBar::BAR_POINTER_DIVISION = 60.0f;
/*
*	@brief コンストラクタ
*	@details 設定バークラスのコンストラクタ
*	@param なし
*	@return なし
*/
SettingBar::SettingBar()
	: m_pCommonResources{}// 共通リソース
	, m_pDR{}// デバイスリソース
	, m_pSettingMenu{   }// 設定メニュー
	, m_pBar{}// UIオブジェクトリスト
	, m_windowWidth{ 0 }// ウィンドウ幅
	, m_windowHeight{ 0 }// ウィンドウ高さ
	, m_time{ 0.0f }// 時間
	, m_num{ SettingMenu::SettingID::BGM }// 状態ID
	, m_selectNum{ SettingMenu::SelectID::NONE }// 選択番号
	, m_setting{ 0,0,0 }// 設定データ(BGM音量,SE音量,マウス感度)
{
}
/*
*	@brief デストラクタ
*	@details 設定バークラスのデストラクタ
*	@param なし
*	@return なし
*/
SettingBar::~SettingBar() {/*do nothing*/ }
/*
*	@brief メニューを初期化する
*	@details メニューアイテムの画像を読み込み、位置・スケール・アンカーを設定する
*	@param resources 共通リソースへのポインタ
*	@param width ウィンドウの幅
*	@param height ウィンドウの高さ
*	@return なし
*/
void SettingBar::Initialize(CommonResources* resources, int width, int height)
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
	// 設定データの初期化
	m_pSettingData = std::make_unique<SettingData>();
	// 「BGM」の場所に配置
	Add("SettingBar"
		, Vector2(BGM_POSITION)
		, Vector2(1, 1)
		, KumachiLib::ANCHOR::MIDDLE_CENTER
		, UIType::NON_SELECT);
	// 「SE」の場所に配置
	Add("SettingBar"
		, Vector2(SE_POSITION)
		, Vector2(1, 1)
		, KumachiLib::ANCHOR::MIDDLE_CENTER
		, UIType::NON_SELECT);
	// 「マウスかんど」の場所に配置
	Add("SettingBar"
		, Vector2(SENSITIVITY_POSITION)
		, Vector2(1, 1)
		, KumachiLib::ANCHOR::MIDDLE_CENTER
		, UIType::NON_SELECT);
	// Jsonファイルから設定を読み込む
	m_pSettingData->Load();
	// BGM音量
	m_setting[0] = m_pSettingData->GetBGMVolume();
	// SE音量
	m_setting[1] = m_pSettingData->GetSEVolume();
	// マウス感度
	m_setting[2] = m_pSettingData->GetMouseSensitivity();
	// 設定バーの玉の数だけ処理
	for (unsigned int i = 0; i < m_pBarPointer.size(); i++)
	{
		// 設定バーの玉を読み込んだ設定に合わせて移動
		m_pBarPointer[i]->SetPosition(
			Vector2(static_cast<float>(Screen::CENTER_X + BAR_POINTER_DIVISION + m_setting[i] * BAR_POINTER_DIVISION - BAR_POINTER_DIVISION)// X座標
				, m_pBarPointer[i]->GetPosition().y));// Y座標
	}
}
/*
*	@brief メニューを更新する
*	@details マウス位置に応じたメニューの選択状態更新とクリック判定を行う
*	@param elapsedTime 前フレームからの経過時間(秒)
*	@return なし
*/
void SettingBar::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;
	// マウスの状態を取得
	auto& mouseState = m_pCommonResources->GetInputManager()->GetMouseState();
	// マウスの座標を取得
	Vector2 mousePos = Vector2(static_cast<float>(mouseState.x), static_cast<float>(mouseState.y));
	// マウスボタン状態を取得
	auto& mouseTracker = m_pCommonResources->GetInputManager()->GetMouseTracker();
	// ドラッグ状態を保持
	bool isDragging = false;
	// ドラッグ中のアイテムのインデックス(-1ではドラッグしていない状態)
	int dragIndex = -1;
	// 時間を加算
	m_time += elapsedTime;
	// 設定バーの弾の数繰り返す
	for (unsigned int i = 0; i < m_pBarPointer.size(); i++)
	{
		// ポインターの当たり判定チェック（円形）
		if (m_pBarPointer[i]->IsHit(mousePos))
		{
			// 左ボタンが押されている場合、ドラッグ状態にする
			if (mouseTracker->GetLastState().leftButton) isDragging = true;
			// ドラッグ中のインデックスを保存
			dragIndex = i;
			// ループを抜ける
			break;
		}
	}
	// ドラッグ中ならマウス座標に追従
	if (isDragging && dragIndex != -1)
	{
		// マウスのX座標を取得
		float newX = mousePos.x;
		// 範囲制限(X座標が同じなのでBGMの位置を基準にする)
		// 左端
		float minX = BGM_POSITION.x - BAR_WIDTH;
		// 右端
		float maxX = BGM_POSITION.x + BAR_WIDTH;
		// 範囲制限
		newX = std::max(minX, std::min(maxX, newX));
		// ポインターの位置更新
		m_pBarPointer[dragIndex]->SetPosition(Vector2(newX, m_pBarPointer[dragIndex]->GetPosition().y));
		// 設定値の更新（0～5に変換）
		m_setting[dragIndex] = static_cast<int>((newX - minX) / BAR_POINTER_DIVISION);
	}
	//「へんこう」が押されたら
	if (m_pSettingMenu->GetSelectIDNum() == SettingMenu::APPLY &&
		mouseTracker->GetLastState().leftButton)
	{
		// 設定を保存する
		m_pSettingData->Save(m_setting[0], m_setting[1], m_setting[2]);
	}
#ifdef _DEBUG// デバッグモードのみ有効
	// デバッグ情報を表示する
	auto debugString = m_pCommonResources->GetDebugString();
	// ウィンドウ上のマウス座標を取得する
	Vector2 pos = Vector2(static_cast<float>(mouseState.x), static_cast<float>(mouseState.y));
	// 設定バーの玉の数だけ処理
	for (unsigned int i = 0; i < m_pBarPointer.size(); i++)
	{
		// 選ばれているUI番号と設定値を表示
		debugString->AddString("SelectNum:%i Setting:%i", m_pBarPointer[i]->IsHit(mousePos), m_setting[i]);
	}
	// ドラッグ中のインデックスを表示
	debugString->AddString("DragIndex:%i ", dragIndex);
	// マウスの座標を表示
	debugString->AddString("MousePos:%f %f", pos.x, pos.y);
	// 左クリック状態を表示
	debugString->AddString("ClickLeft:%i", mouseTracker->GetLastState().leftButton);
#endif
}
/*
*	@brief 描画する
*	@details 設定バーの玉とアイテム画像を描画する
*	@param なし
*	@return なし
*/
void SettingBar::Render()
{
	// 設定バーの玉の数だけ処理
	for (unsigned int i = 0; i < m_pBarPointer.size(); i++)
	{
		// 実際に表示したいアイテム画像を表示
		m_pBar[i]->Render();
		// 設定バーの玉を表示
		m_pBarPointer[i]->Render();
	}
}
/*
*	@brief メニューアイテムを追加する
*	@details 指定の画像でUIを作成し、内部リストに追加する (選択可なら背景枠も作成)
*	@param key メニューアイテムのキー
*	@param position メニューアイテムの位置
*	@param scale メニューアイテムのスケール
*	@param anchor メニューアイテムのアンカー
*	@param type メニューアイテムの種類
*/
void SettingBar::Add(const std::string& key
	, const DirectX::SimpleMath::Vector2& position
	, const DirectX::SimpleMath::Vector2& scale
	, KumachiLib::ANCHOR anchor
	, UIType type)
{
	using namespace DirectX::SimpleMath;
	// 未使用警告非表示
	UNREFERENCED_PARAMETER(type);
	// UIオブジェクトの生成
	std::unique_ptr<UI> userInterface = std::make_unique<UI>(m_pCommonResources);
	// 指定された画像を表示するためのアイテムを作成する
	userInterface->Create(m_pDR, key, position, Vector2(scale.x / 1.45f, scale.y / 2.0f), anchor);
	// ウィンドウサイズを設定
	userInterface->SetWindowSize(m_windowWidth, m_windowHeight);
	// アイテムを新しく追加
	m_pBar.push_back(std::move(userInterface));
	// 背景用のウィンドウ画像も追加する
	std::unique_ptr<UI> base = std::make_unique<UI>(m_pCommonResources);
	// 指定された画像を表示するためのアイテムを作成する
	base->Create(m_pDR, "SettingBarPointer", position, scale, anchor);
	// ウィンドウサイズを設定
	base->SetWindowSize(m_windowWidth, m_windowHeight);
	// 背景用のアイテムも新しく追加する
	m_pBarPointer.push_back(std::move(base));
}