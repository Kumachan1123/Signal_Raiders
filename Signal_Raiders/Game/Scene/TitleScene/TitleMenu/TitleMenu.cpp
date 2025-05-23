/*
*	@file	TitleMenu.cpp
*	@brief	タイトルメニュークラス
*/
#include "pch.h"
#include "TitleMenu.h"



TitleMenu::TitleMenu()
	: m_menuIndex{ 0 }
	, m_pDR{ nullptr }
	, m_commonResources{ nullptr }
	, m_pUI{}
	, m_pSelect{}
	, m_pSelectTexturePath{ nullptr }
	, m_windowWidth{ 0 }
	, m_windowHeight{ 0 }
	, m_time{ 0.0f }
	, m_hit{ false }
	, m_num{ SceneID::STAGESELECT }
{
}


TitleMenu::~TitleMenu()
{
}

void TitleMenu::Initialize(CommonResources* resources, int width, int height)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	m_commonResources = resources;
	m_pDR = m_commonResources->GetDeviceResources();
	m_windowWidth = width;
	m_windowHeight = height;
	m_pSelectTexturePath = L"Resources/Textures/select.png";
	//  「プレイ」を読み込む
	Add(L"Resources/Textures/play.png"
		, Vector2(Screen::CENTER_X, Screen::CENTER_Y + 250)
		, Vector2(.5, .5)
		, KumachiLib::ANCHOR::MIDDLE_CENTER
		, UIType::SELECT);
	//  「せってい」を読み込む
	Add(L"Resources/Textures/setting.png"
		, Vector2(Screen::CENTER_X, Screen::CENTER_Y + 350)
		, Vector2(.5, .5)
		, KumachiLib::ANCHOR::MIDDLE_CENTER
		, UIType::SELECT);
	//  「おわる」を読み込む
	Add(L"Resources/Textures/end.png"
		, Vector2(Screen::CENTER_X, Screen::CENTER_Y + 450)
		, Vector2(.5, .5)
		, KumachiLib::ANCHOR::MIDDLE_CENTER
		, UIType::SELECT);
	//  「操作説明」を読み込む
	Add(L"Resources/Textures/Guide.png"
		, Vector2(Screen::RIGHT, Screen::BOTTOM)
		, Vector2(1, 1)
		, KumachiLib::ANCHOR::BOTTOM_RIGHT
		, UIType::NON_SELECT);

}

void TitleMenu::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;
	const auto& kbTracker = m_commonResources->GetInputManager()->GetKeyboardTracker();
	// マウスのトラッカーを取得する
	auto& mtracker = m_commonResources->GetInputManager()->GetMouseTracker();
	// マウスの状態を取得
	auto& mouseState = m_commonResources->GetInputManager()->GetMouseState();
	m_hit = false;// 何かにヒットしたかどうか
	// マウスの座標を取得
	Vector2 mousePos = Vector2(static_cast<float>(mouseState.x), static_cast<float>(mouseState.y));
	//  メニューアイテムの数だけ繰り返す
	for (int i = 0; i < m_pUI.size(); i++)
	{
		//  マウスの座標がアイテムの範囲内にあるかどうかを判定
		if (m_pUI[i]->IsHit(mousePos))
		{
			//  範囲内にある場合は、選択中のアイテムを更新
			m_menuIndex = i;
			m_hit = true;
			break;
		}
	}
	// もし一個もヒットしてなかったら、選択なしにする
	if (!m_hit) { m_menuIndex = 6; }

	m_time += elapsedTime;

	if (kbTracker->pressed.Space || mtracker->GetLastState().leftButton)
	{
		m_num = static_cast<SceneID>(m_menuIndex);
	}
	//  メニューアイテムの選択先を更新
	for (int i = 0; i < m_pUI.size(); i++)
	{
		//  アイテムの選択状態を更新
		m_pSelect[i]->SetScale(m_pSelect[i]->GetSelectScale());
		m_pSelect[i]->SetTime(m_pSelect[i]->GetTime() + elapsedTime);
		m_pUI[i]->SetScale(m_pUI[i]->GetSelectScale());
		m_pUI[i]->SetTime(m_pUI[i]->GetTime() + elapsedTime);
	}
	// 選択不可能なアイテムの選択状態を更新
	for (int i = 0; i < m_pGuide.size(); i++)
	{
		m_pGuide[i]->SetScale(m_pGuide[i]->GetSelectScale());
		m_pGuide[i]->SetTime(m_pGuide[i]->GetTime() + elapsedTime);
	}
	if (!m_hit && m_menuIndex == 6)// 当たってなかったら終了
		return;
	Vector2 select = m_pUI[m_menuIndex]->GetSelectScale();// 選択中の初期サイズを取得する
	Vector2 selectScale = Vector2::Lerp(m_pUI[m_menuIndex]->GetSelectScale(), Vector2::One, 1);// 選択状態とするための変化用サイズを算出する
	select = Vector2((sin(m_time) * 0.1f) + 1.0f);// 選択状態は初期状態＋50％の大きさとする
	m_pUI[m_menuIndex]->SetScale(select);// 算出後のサイズを現在のサイズとして設定する
	m_pSelect[m_menuIndex]->SetScale(Vector2::One);// 背景用のウィンドウ画像にも同じ割合の値を設定する
}
/*
*	@brief	描画
*	@detail	メニューアイテムを描画する
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
#ifdef _DEBUG

	auto debugString = m_commonResources->GetDebugString(); // デバッグ情報を表示する
	for (int i = 0; i < m_transforms.size(); i++)// 登録したUIの数ループ
		debugString->AddString("Transform.Pos:%f,%f  Scale:%f,%f", m_transforms[i].position.x, m_transforms[i].position.y, m_transforms[i].scale.x, m_transforms[i].scale.y);//  UIの座標を表示

#endif
}
/*
*	@brief	アイテムを追加する
*	@detail	アイテムを追加する
*	@param path		アイテムのパス
*	@param position	アイテムの位置
*	@param scale	アイテムのスケール
*	@param anchor	アイテムのアンカー
*	@param type		アイテムの種類
*	@return なし
*/
void TitleMenu::Add(const wchar_t* path, DirectX::SimpleMath::Vector2 position, DirectX::SimpleMath::Vector2 scale, KumachiLib::ANCHOR anchor, UIType type)
{
	std::unique_ptr<UI> userInterface = std::make_unique<UI>();// メニューとしてアイテムを追加する
	userInterface->Create(m_pDR, path, position, scale, anchor);// 指定された画像を表示するためのアイテムを作成する
	userInterface->SetWindowSize(m_windowWidth, m_windowHeight);// ウィンドウのサイズを設定する
	if (type == UIType::SELECT)
	{
		m_pUI.push_back(std::move(userInterface));//  選択可能アイテムを新しく追加

		std::unique_ptr<UI> base = std::make_unique<UI>();//  背景用のウィンドウ画像も追加する
		base->Create(m_pDR, m_pSelectTexturePath, position, scale, anchor);// 指定された画像を表示するためのアイテムを作成する
		base->SetWindowSize(m_windowWidth, m_windowHeight);// ウィンドウのサイズを設定する
		m_pSelect.push_back(std::move(base));// 背景用のアイテムも新しく追加する
		m_transforms.push_back({ position, scale });// UIの情報を配列に登録
	}
	else
	{
		m_pGuide.push_back(std::move(userInterface));// 選択不可能アイテムを新しく追加
		return;
	}
}
