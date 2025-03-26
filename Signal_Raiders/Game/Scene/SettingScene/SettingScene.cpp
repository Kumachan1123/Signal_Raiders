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
	m_audioManager{ AudioManager::GetInstance() },
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
	m_pFade->Create(DR);
	m_pFade->SetState(Fade::FadeState::FadeIn);
	m_pFade->SetTextureNum((int)(Fade::TextureNum::BLACK));
	// 背景の初期化
	m_pBackGround = std::make_unique<BackGround>(m_commonResources);
	m_pBackGround->Create(DR);
	// FPSカメラを作成する
	m_camera = std::make_unique<FPS_Camera>();
	// セッティングメニューを作成
	m_pSettingMenu = std::make_unique<SettingMenu>();
	m_pSettingMenu->Initialize(m_commonResources, Screen::WIDTH, Screen::HEIGHT);
	// マウスポインターを作成
	m_pMousePointer = std::make_unique<MousePointer>();
	m_pMousePointer->Initialize(m_commonResources, Screen::WIDTH, Screen::HEIGHT);
	// 設定バーを作成
	m_pSettingBar = std::make_unique<SettingBar>(m_pSettingMenu.get());
	m_pSettingBar->Initialize(m_commonResources, Screen::WIDTH, Screen::HEIGHT);
	// 設定データの読み込み
	m_pSettingData = std::make_unique<SettingData>();
	m_pSettingData->Load();
	m_BGMvolume = VOLUME * static_cast<float>(m_pSettingData->GetBGMVolume()) * .1f;
	m_SEvolume = VOLUME * static_cast<float>(m_pSettingData->GetSEVolume()) * .1f;
	// 音声を初期化する
	InitializeFMOD();

}

//---------------------------------------------------------
// 更新する
//---------------------------------------------------------
void SettingScene::Update(float elapsedTime)
{
	// セッティングメニューの更新処理
	m_pSettingMenu->Update(elapsedTime);
	// 今選ばれているメニューのIDをBarに渡す
	m_pSettingBar->SetStateIDNum(static_cast<SettingMenu::StateID>(m_pSettingMenu->GetMenuIndex()));
	// セッティングバーの更新処理
	m_pSettingBar->Update(elapsedTime);
	// オーディオマネージャーの更新処理
	m_audioManager->Update();

	// マウスのトラッカーを取得する
	auto& mtracker = m_commonResources->GetInputManager()->GetMouseTracker();

	// メニューでの選択処理が行われたら
	if (m_pFade->GetState() == Fade::FadeState::FadeInEnd)
	{
		if (mtracker->GetLastState().leftButton)
		{
			if (m_pSettingMenu->GetSelectIDNum() == SettingMenu::SelectID::END ||
				m_pSettingMenu->GetSelectIDNum() == SettingMenu::SelectID::APPLY)
			{

				m_pFade->SetState(Fade::FadeState::FadeOut);// フェードアウトに移行
				m_pFade->SetTextureNum((int)(Fade::TextureNum::BLACK));// フェードのテクスチャを変更
			}
			else
			{
				// マウスポインターの更新
				m_pMousePointer->Update(elapsedTime);

			}

		}
		else
		{
			// マウスポインターの更新
			m_pMousePointer->Update(elapsedTime);
		}


	}
	// フェードアウトが終了したら
	if (m_pFade->GetState() == Fade::FadeState::FadeOutEnd)	m_isChangeScene = true;
	// BGMの再生
	m_audioManager->PlaySound("BGM", m_BGMvolume);

	// 背景の更新
	m_pBackGround->Update(elapsedTime);
	// フェードの更新
	m_pFade->Update(elapsedTime);


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
	{
		m_pSettingMenu->Render();
		m_pSettingBar->Render();
		m_pMousePointer->Render();
	}
	// フェードの描画
	m_pFade->Render();
}

//---------------------------------------------------------
// 後始末する
//---------------------------------------------------------
void SettingScene::Finalize()
{
	SetVolume();// 音量の設定
	// オーディオマネージャーの終了処理
	m_audioManager->Shutdown();
}

//---------------------------------------------------------
// 次のシーンIDを取得する
//---------------------------------------------------------
IScene::SceneID SettingScene::GetNextSceneID() const
{
	// シーン変更がある場合
	if (m_isChangeScene)
	{


		m_audioManager->StopSound("BGM");// BGMの停止
		m_audioManager->StopSound("SE");// SEの停止
		m_audioManager->StopSound("Select");// Selectの停止
		return IScene::SceneID::TITLE;
	}
	// シーン変更がない場合
	return IScene::SceneID::NONE;
}

//---------------------------------------------------------
// FMODのシステムの初期化と音声データのロード
//---------------------------------------------------------
void SettingScene::InitializeFMOD()
{
	// FMODシステムの初期化
	m_audioManager->Initialize();
	// 音声データのロード
	m_audioManager->LoadSound("Resources/Sounds/select.mp3", "SE");
	m_audioManager->LoadSound("Resources/Sounds/title.mp3", "BGM");
	m_audioManager->LoadSound("Resources/Sounds/click.mp3", "Select");
}

// 音量を設定する
void SettingScene::SetVolume()
{
	// 音量の取得(10分の1で割合を取得)
	float BGMvolume = static_cast<float>(m_pSettingBar->GetSetting(0)) * 0.1f;
	float SEvolume = static_cast<float>(m_pSettingBar->GetSetting(1)) * 0.1f;
	// 設定の変更
	m_pSettingData->SetBGMVolume(m_pSettingBar->GetSetting(0));
	m_pSettingData->SetSEVolume(m_pSettingBar->GetSetting(1));
	// 音量の設定
	m_BGMvolume = VOLUME * BGMvolume;
	m_SEvolume = VOLUME * SEvolume;
}