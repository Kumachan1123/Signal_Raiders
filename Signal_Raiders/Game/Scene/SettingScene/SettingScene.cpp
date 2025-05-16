/*
	@file	SettingScene.cpp
	@brief	メニュークラス
*/
#include "pch.h"
#include "SettingScene.h"
#include "Game/Fade/Fade.h"
#include "Game/Screen.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include "Libraries/MyLib/InputManager.h"
#include <cassert>
#include "Game/KumachiLib//BinaryFile/BinaryFile.h"
#include <Libraries/Microsoft/DebugDraw.h>
#include "Game/FPS_Camera/FPS_Camera.h"
#include <Mouse.h>
using namespace DirectX;
using namespace DirectX::SimpleMath;


//---------------------------------------------------------
// コンストラクタ
//---------------------------------------------------------
SettingScene::SettingScene(IScene::SceneID sceneID)
	:
	m_commonResources{},
	m_isChangeScene{ false },
	m_isFade{ false },
	m_BGMvolume{ VOLUME },
	m_SEvolume{ VOLUME },
	m_camera{},
	m_pDR{},
	m_pFade{},
	m_fadeState{ },
	m_fadeTexNum{ 0 },
	m_pBackGround{ nullptr },
	m_nowSceneID{ sceneID },
	m_pSettingMenu{},
	m_pSettingBar{},
	m_pSettingData{}
{}


//---------------------------------------------------------
// デストラクタ
//---------------------------------------------------------
SettingScene::~SettingScene()
{
	// do nothing.
	Finalize();
}

//---------------------------------------------------------
// 初期化する
//---------------------------------------------------------
void SettingScene::Initialize(CommonResources* resources)
{
	assert(resources);
	m_commonResources = resources;
	auto DR = m_commonResources->GetDeviceResources();
	// フェードの初期化
	m_pFade = std::make_unique<Fade>(m_commonResources);
	m_pFade->Create();
	m_pFade->SetState(Fade::FadeState::FadeIn);
	//m_pFade->SetTextureNum((int)(Fade::TextureNum::BLACK));
	// 背景の初期化
	m_pBackGround = std::make_unique<BackGround>(m_commonResources);
	m_pBackGround->Create(DR);
	// FPSカメラを作成する
	m_camera = std::make_unique<FPS_Camera>();
	// セッティングメニューを作成
	m_pSettingMenu = std::make_unique<SettingMenu>();
	//// マウスポインターを作成
	m_pMousePointer = std::make_unique<MousePointer>();
	m_pSettingBar = std::make_unique<SettingBar>();
	m_pSettingBar->SetSettingMenu(m_pSettingMenu.get());
	m_pUI.push_back(std::move(m_pSettingMenu));
	m_pUI.push_back(std::move(m_pSettingBar));
	m_pUI.push_back(std::move(m_pMousePointer));
	for (int it = 0; it < m_pUI.size(); ++it)m_pUI[it]->Initialize(m_commonResources, Screen::WIDTH, Screen::HEIGHT);

	// 設定データの読み込み
	m_pSettingData = std::make_unique<SettingData>();
	m_pSettingData->Load();
	m_BGMvolume = VOLUME * static_cast<float>(m_pSettingData->GetBGMVolume()) * .1f;
	m_SEvolume = VOLUME * static_cast<float>(m_pSettingData->GetSEVolume()) * .1f;
}

//---------------------------------------------------------
// 更新する
//---------------------------------------------------------
void SettingScene::Update(float elapsedTime)
{
	UpdateContext ctx;
	ctx.bulletPoint = 0;// 使わない
	ctx.dashStamina = 0;//使わない
	ctx.elapsedTime = elapsedTime;//フレーム時間
	ctx.playerHP = 0;//使わない
	UpdateSettingBars(ctx);	// メニューの更新
	UpdateFadeAndMouse(ctx);// フェード状態とマウスの更新
	UpdateBackgroundAndFade(elapsedTime);// 背景とフェードの更新
}
//---------------------------------------------------------
// 描画する
//---------------------------------------------------------
void SettingScene::Render()
{
	// 背景の描画
	m_pBackGround->Render();

	// スペースキー押してってやつ描画(画面遷移中は描画しない)
	if (m_pFade->GetState() == Fade::FadeState::FadeInEnd)
		for (int it = 0; it < m_pUI.size(); ++it)m_pUI[it]->Render();
	// フェードの描画
	m_pFade->Render();
}

//---------------------------------------------------------
// 後始末する
//---------------------------------------------------------
void SettingScene::Finalize()
{
	SetVolume();// 音量の設定
}

//---------------------------------------------------------
// 次のシーンIDを取得する
//---------------------------------------------------------
IScene::SceneID SettingScene::GetNextSceneID() const
{
	// シーン変更がある場合
	if (m_isChangeScene)
	{
		m_commonResources->GetAudioManager()->StopSound("TitleBGM");// BGMの停止
		m_commonResources->GetAudioManager()->StopSound("SE");// SEの停止
		m_commonResources->GetAudioManager()->StopSound("Select");// Selectの停止
		return IScene::SceneID::TITLE;
	}
	// シーン変更がない場合
	return IScene::SceneID::NONE;
}



// 音量を設定する
void SettingScene::SetVolume()
{
	for (int it = 0; it < m_pUI.size(); ++it)
	{
		if (auto pSettingBar = dynamic_cast<SettingBar*>(m_pUI[it].get()))
		{
			// 音量の取得(10分の1で割合を取得)
			float BGMvolume = static_cast<float>(pSettingBar->GetSetting(0)) * 0.1f;
			float SEvolume = static_cast<float>(pSettingBar->GetSetting(1)) * 0.1f;
			// 設定の変更
			m_pSettingData->SetBGMVolume(pSettingBar->GetSetting(0));
			m_pSettingData->SetSEVolume(pSettingBar->GetSetting(1));
			// 音量の設定
			m_BGMvolume = VOLUME * BGMvolume;
			m_SEvolume = VOLUME * SEvolume;
			return;
		}
	}

}

void SettingScene::UpdateSettingBars(const UpdateContext& ctx)
{
	// SettingMenuをリストアップ
	std::vector<SettingMenu*> settingMenus;
	for (auto& settingMenu : m_pUI)
		if (auto pSettingMenu = dynamic_cast<SettingMenu*>(settingMenu.get()))
			settingMenus.push_back(pSettingMenu);
	// SettingBarをリストアップし、メニュー情報をセット
	for (auto& settingBar : m_pUI)
	{
		if (auto pSettingBar = dynamic_cast<SettingBar*>(settingBar.get()))
			for (auto& menu : settingMenus)
				pSettingBar->SetStateIDNum(static_cast<SettingMenu::StateID>(menu->GetMenuIndex()));
		settingBar->Update(ctx);
	}
}

void SettingScene::UpdateFadeAndMouse(const UpdateContext& ctx)
{
	// オーディオマネージャーの更新処理
	m_commonResources->GetAudioManager()->Update();
	// マウスのトラッカーを取得する
	auto& mtracker = m_commonResources->GetInputManager()->GetMouseTracker();
	// メニューでの選択処理が行われたら
	if (m_pFade->GetState() != Fade::FadeState::FadeInEnd)return;
	if (mtracker->GetLastState().leftButton) HandleMenuSelection(ctx);
	else UpdateMousePointers(ctx);
}

void SettingScene::UpdateBackgroundAndFade(float elapsedTime)
{
	if (m_pFade->GetState() == Fade::FadeState::FadeOutEnd)m_isChangeScene = true;// シーン変更フラグを立てる
	m_commonResources->GetAudioManager()->PlaySound("TitleBGM", m_BGMvolume);
	m_pBackGround->Update(elapsedTime);
	m_pFade->Update(elapsedTime);
}

void SettingScene::UpdateMousePointers(const UpdateContext& ctx)
{
	for (int it = 0; it < m_pUI.size(); ++it)
		if (auto pMousePointer = dynamic_cast<MousePointer*>(m_pUI[it].get()))
			pMousePointer->Update(ctx);
}

void SettingScene::HandleMenuSelection(const UpdateContext& ctx)
{
	for (int it = 0; it < m_pUI.size(); ++it)
	{
		if (auto pSettingMenu = dynamic_cast<SettingMenu*>(m_pUI[it].get()))
		{
			auto selectID = pSettingMenu->GetSelectIDNum();// 選択されたメニューのIDを取得
			if (selectID == SettingMenu::SelectID::END ||
				selectID == SettingMenu::SelectID::APPLY)
			{
				m_commonResources->GetAudioManager()->PlaySound("SE", m_SEvolume);
				m_pFade->SetState(Fade::FadeState::FadeOut);// フェードアウトに移行
				//m_pFade->SetTextureNum((int)(Fade::TextureNum::BLACK));// フェードのテクスチャを変更
				return;
			}
			UpdateMousePointers(ctx);// マウスポインターの更新
		}
	}
}
