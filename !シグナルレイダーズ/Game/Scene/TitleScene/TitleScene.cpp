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
#include "Game/FPS_Camera/FPS_Camera.h"
using namespace DirectX;
using namespace DirectX::SimpleMath;

//---------------------------------------------------------
// コンストラクタ
//---------------------------------------------------------
TitleScene::TitleScene()
	:
	m_commonResources{},
	m_isChangeScene{ false },
	m_isFade{ false },
	m_volume{ 1 },
	m_counter{ 0 },
	m_camera{},
	m_pDR{},
	m_fade{},
	m_fadeState{ },
	m_fadeTexNum{ 0 },
	m_backGround{ nullptr },
	m_audioManager{ AudioManager::GetInstance() }
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
	m_fade = std::make_unique<Fade>(m_commonResources);
	m_fade->Create(DR);
	m_fade->SetState(Fade::FadeState::FadeIn);
	m_fade->SetTextureNum((int)(Fade::TextureNum::BLACK));
	// 背景の初期化
	m_backGround = std::make_unique<BackGround>(m_commonResources);
	m_backGround->Create(DR);
	// FPSカメラを作成する
	m_camera = std::make_unique<FPS_Camera>();
	// 指示画像を作成
	m_pressKey = std::make_unique<PressKey>(m_commonResources);
	m_pressKey->Initialize();
	// タイトルロゴを作成
	m_titleLogo = std::make_unique<TitleLogo>(m_commonResources);
	m_titleLogo->Create(DR);
	// 音声を初期化する
	InitializeFMOD();

}

//---------------------------------------------------------
// 更新する
//---------------------------------------------------------
void TitleScene::Update(float elapsedTime)
{
	// オーディオマネージャーの更新処理
	m_audioManager->Update();
	// キーボードステートトラッカーを取得する
	const auto& kbTracker = m_commonResources->GetInputManager()->GetKeyboardTracker();
	// スペースキーが押されたら
	if (m_fade->GetState() == Fade::FadeState::FadeInEnd && kbTracker->pressed.Space)
	{
		m_audioManager->PlaySound("SE", .3);// SEの再生
		m_fade->SetState(Fade::FadeState::FadeOut);// フェードアウトに移行
		m_fade->SetTextureNum((int)(Fade::TextureNum::READY));// フェードのテクスチャを変更
	}
	// フェードアウトが終了したら
	if (m_fade->GetState() == Fade::FadeState::FadeOutEnd)	m_isChangeScene = true;
	// BGMの再生
	m_audioManager->PlaySound("BGM", 0.3);
	// 指示画像の更新
	m_pressKey->Update(elapsedTime);
	// 背景の更新
	m_backGround->Update(elapsedTime);
	// フェードの更新
	m_fade->Update(elapsedTime);
	// タイトルロゴの更新
	m_titleLogo->Update(elapsedTime);
}
//---------------------------------------------------------
// 描画する
//---------------------------------------------------------
void TitleScene::Render()
{
	// 背景の描画
	m_backGround->Render();
	// タイトルロゴの描画
	m_titleLogo->Render();
	// スペースキー押してってやつ描画(画面遷移中は描画しない)
	if (m_fade->GetState() == Fade::FadeState::FadeInEnd)m_pressKey->Render();
	// フェードの描画
	m_fade->Render();
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
		return IScene::SceneID::PLAY;
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
}


