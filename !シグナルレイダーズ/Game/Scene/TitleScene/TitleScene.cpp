/*
	@file	TitleScene.cpp
	@brief	タイトルシーンクラス
*/
#include "pch.h"
#include "TitleScene.h"
#include "Game/Fade/Fade.h"
#include "Game/Screen.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include "Libraries/MyLib/InputManager.h"
#include <cassert>
#include "Game/KumachiLib//BinaryFile.h"
#include <Libraries/Microsoft/DebugDraw.h>
#include "Libraries/MyLib/DebugString.h"
#include "Game/FPS_Camera/FPS_Camera.h"
using namespace DirectX;
using namespace DirectX::SimpleMath;
void EndGame()noexcept;

//---------------------------------------------------------
// コンストラクタ
//---------------------------------------------------------
TitleScene::TitleScene(IScene::SceneID sceneID)
	:
	m_commonResources{},
	m_isChangeScene{ false },
	m_isFade{ false },
	m_BGMvolume{ VOLUME },
	m_SEvolume{ VOLUME },
	m_counter{ 0 },
	m_camera{},
	m_pDR{},
	m_pFade{},
	m_fadeState{ },
	m_fadeTexNum{ 0 },
	m_pBackGround{ nullptr },
	m_audioManager{ AudioManager::GetInstance() },
	m_nowSceneID{ sceneID }
{}


//---------------------------------------------------------
// デストラクタ
//---------------------------------------------------------
TitleScene::~TitleScene()
{
	// do nothing.
	Finalize();
}

//---------------------------------------------------------
// 初期化する
//---------------------------------------------------------
void TitleScene::Initialize(CommonResources* resources)
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
	// タイトルロゴを作成
	m_pTitleLogo = std::make_unique<TitleLogo>(m_commonResources);
	m_pTitleLogo->Create(DR);
	// メニューを作成
	m_pMenu = std::make_unique<Menu>();
	m_pMenu->Initialize(m_commonResources, Screen::WIDTH, Screen::HEIGHT);
	// 設定ファイルの読み込み
	m_pSettingData = std::make_unique<SettingData>();
	m_pSettingData->Load();
	m_BGMvolume = VOLUME * static_cast<float>(m_pSettingData->GetBGMVolume());
	m_SEvolume = VOLUME * static_cast<float>(m_pSettingData->GetSEVolume());
	// 音声を初期化する
	InitializeFMOD();

}

//---------------------------------------------------------
// 更新する
//---------------------------------------------------------
void TitleScene::Update(float elapsedTime)
{
	// メニューの更新
	m_pMenu->Update(elapsedTime);
	// オーディオマネージャーの更新処理
	m_audioManager->Update();
	// キーボードステートトラッカーを取得する
	const auto& kbTracker = m_commonResources->GetInputManager()->GetKeyboardTracker();
	// メニューでの選択処理が行われたら
	if (m_pFade->GetState() == Fade::FadeState::FadeInEnd)
	{
		if (kbTracker->pressed.Space)
		{
			m_audioManager->PlaySound("SE", m_SEvolume);// SEの再生
			if (m_pMenu->GetSceneNum() == Menu::SceneID::PLAY)
			{
				m_pFade->SetState(Fade::FadeState::FadeOut);// フェードアウトに移行
				m_pFade->SetTextureNum((int)(Fade::TextureNum::READY));// フェードのテクスチャを変更
			}
			else
			{
				m_pFade->SetState(Fade::FadeState::FadeOut);// フェードアウトに移行
				m_pFade->SetTextureNum((int)(Fade::TextureNum::BLACK));// フェードのテクスチャを変更
			}
		}
		// WかSのいずれかが押されたら
		if (kbTracker->pressed.W || kbTracker->pressed.S)
			m_audioManager->PlaySound("Select", m_SEvolume);// SEの再生
	}

	// フェードアウトが終了したら
	if (m_pFade->GetState() == Fade::FadeState::FadeOutEnd)	m_isChangeScene = true;
	// BGMの再生
	m_audioManager->PlaySound("BGM", m_BGMvolume);
	m_pBackGround->Update(elapsedTime);
	// フェードの更新
	m_pFade->Update(elapsedTime);
	// タイトルロゴの更新
	m_pTitleLogo->Update(elapsedTime);

}
//---------------------------------------------------------
// 描画する
//---------------------------------------------------------
void TitleScene::Render()
{
	// 背景の描画
	m_pBackGround->Render();
	// タイトルロゴの描画
	m_pTitleLogo->Render();
	// スペースキー押してってやつ描画(画面遷移中は描画しない)
	if (m_pFade->GetState() == Fade::FadeState::FadeInEnd)
	{
		m_pMenu->Render();
	}
	// フェードの描画
	m_pFade->Render();
}

//---------------------------------------------------------
// 後始末する
//---------------------------------------------------------
void TitleScene::Finalize()
{
	// オーディオマネージャーの終了処理
	m_audioManager->Shutdown();
}

//---------------------------------------------------------
// 次のシーンIDを取得する
//---------------------------------------------------------
IScene::SceneID TitleScene::GetNextSceneID() const
{
	// シーン変更がある場合
	if (m_isChangeScene)
	{
		m_audioManager->StopSound("BGM");// BGMの停止
		m_audioManager->StopSound("SE");// SEの停止
		switch (m_pMenu->GetSceneNum())
		{
			case Menu::SceneID::PLAY:
				return IScene::SceneID::PLAY;
				break;
			case Menu::SceneID::SETTING:
				return IScene::SceneID::SETTING;
				break;
			case Menu::SceneID::END:
				// ゲーム終了
				EndGame();
				break;
			default:
				break;
		}
	}
	// シーン変更がない場合
	return IScene::SceneID::NONE;
}

//---------------------------------------------------------
// FMODのシステムの初期化と音声データのロード
//---------------------------------------------------------
void TitleScene::InitializeFMOD()
{
	// FMODシステムの初期化
	m_audioManager->Initialize();
	// 音声データのロード
	m_audioManager->LoadSound("Resources/Sounds/select.mp3", "SE");
	m_audioManager->LoadSound("Resources/Sounds/title.mp3", "BGM");
	m_audioManager->LoadSound("Resources/Sounds/click.mp3", "Select");
}

void EndGame() noexcept
{
	PostQuitMessage(0);
}

