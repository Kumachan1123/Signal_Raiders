/*
	@file	ResultScene.cpp
	@brief　リザルトシーンクラス
*/
#include "pch.h"
#include "StageSelectScene.h"
#include "Game/Screen.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include "Libraries/MyLib/InputManager.h"
#include <cassert>
#include <Mouse.h>
#include <Libraries/Microsoft/DebugDraw.h>
using namespace DirectX;
using namespace DirectX::SimpleMath;

//---------------------------------------------------------
// コンストラクタ
//---------------------------------------------------------
StageSelectScene::StageSelectScene(IScene::SceneID sceneID)
	:
	m_commonResources{},
	m_isChangeScene{},
	m_isFade{},
	m_volume{},
	m_BGMvolume{ VOLUME },
	m_SEvolume{ VOLUME },
	m_counter{},
	m_time{ },
	m_size{},
	m_pressKeySize{},
	m_pFade{},
	m_pBackGround{ nullptr },
	//m_audioManager{ AudioManager::GetInstance() },
	m_pTexturePath{},
	m_nowSceneID{ sceneID }
{
}

//---------------------------------------------------------
// デストラクタ
//---------------------------------------------------------
StageSelectScene::~StageSelectScene()
{
	// do nothing.
	Finalize();
}

//---------------------------------------------------------
// 初期化する
//---------------------------------------------------------
void StageSelectScene::Initialize(CommonResources* resources)
{
	assert(resources);
	m_commonResources = resources;

	auto DR = m_commonResources->GetDeviceResources();
	// フェードの初期化
	m_pFade = std::make_unique<Fade>(m_commonResources);
	m_pFade->Create(DR);
	m_pFade->SetState(Fade::FadeState::FadeIn);
	m_pFade->SetTextureNum((int)(Fade::TextureNum::BLACK));
	// 背景を作成する
	m_pBackGround = std::make_unique<BackGround>(m_commonResources);
	m_pBackGround->Create(DR);
	// ステージ選択メニューを作成する
	m_pStageSelectMenu = std::make_unique<StageSelectMenu>();
	m_pStageSelectMenu->Initialize(m_commonResources, Screen::WIDTH, Screen::HEIGHT);
	// 設定ファイルの読み込み
	m_pSettingData = std::make_unique<SettingData>();
	m_pSettingData->Load();
	m_BGMvolume = VOLUME * static_cast<float>(m_pSettingData->GetBGMVolume()) * 0.1f;// BGMの音量を設定(割合変換)
	m_SEvolume = VOLUME * static_cast<float>(m_pSettingData->GetSEVolume()) * 0.1f;// SEの音量を設定(割合変換)
	// マウスポインターを作成
	m_pMousePointer = std::make_unique<MousePointer>();
	m_pMousePointer->Initialize(m_commonResources, Screen::WIDTH, Screen::HEIGHT);

	// ステージ選択クラス作成
	m_pStageSelect = std::make_unique<StageSelect>(m_commonResources);
	m_pStageSelect->Create(DR);
	// シーン変更フラグを初期化する
	m_isChangeScene = false;

	// フェードに関する準備
	m_isFade = false;
	m_volume = 1.0f;
	m_counter = 0;
}

//---------------------------------------------------------
// 更新する
//---------------------------------------------------------
void StageSelectScene::Update(float elapsedTime)
{

	// オーディオマネージャーの更新
	m_commonResources->GetAudioManager()->Update();
	// キーボードステートトラッカーを取得する
	const auto& kbTracker = m_commonResources->GetInputManager()->GetKeyboardTracker();
	// マウスのトラッカーを取得する
	auto& mtracker = m_commonResources->GetInputManager()->GetMouseTracker();
	if (kbTracker->pressed.A || kbTracker->pressed.D)
		m_commonResources->GetAudioManager()->PlaySound("Select", m_SEvolume);// SEの再生

	if (m_pFade->GetState() == Fade::FadeState::FadeInEnd)
	{
		// スペースキーが押されたら
		if (mtracker->GetLastState().leftButton)
		{
			m_commonResources->GetAudioManager()->PlaySound("SE", m_SEvolume);// SEの再生
			m_pFade->SetState(Fade::FadeState::FadeOut);// フェードアウトに移行
			m_pFade->SetTextureNum((int)(Fade::TextureNum::BLACK));// フェードのテクスチャを変更
		}
		else
		{
			// マウスポインターの更新
			m_pMousePointer->Update(elapsedTime);
			//ステージ選択メニューの更新
			m_pStageSelectMenu->Update(elapsedTime);
		}



	}
	// フェードアウトが終了したら
	if (m_pFade->GetState() == Fade::FadeState::FadeOutEnd)
	{
		m_isChangeScene = true;
	}
	// BGMの再生
	m_commonResources->GetAudioManager()->PlaySound("TitleBGM", m_BGMvolume);
	// フェードに関する準備
	m_time += elapsedTime; // 時間をカウント
	m_size = (sin(m_time) + 1.0f) * 0.3f + 0.75f; // sin波で0.5〜1.5の間を変動させる
	// 背景の更新
	m_pBackGround->Update(elapsedTime);

	// フェードの更新
	m_pFade->Update(elapsedTime);

	// ステージ選択更新
	m_pStageSelect->Update(elapsedTime);
}

//---------------------------------------------------------
// 描画する
//---------------------------------------------------------
void StageSelectScene::Render()
{
	// 背景の描画
	m_pBackGround->Render();

	// メニューと結果を描画
	if (m_pFade->GetState() == Fade::FadeState::FadeInEnd)
	{
		m_pStageSelect->Render();
		m_pStageSelectMenu->Render();
		m_pMousePointer->Render();// マウスポインターの描画
	}

	// フェードの描画
	m_pFade->Render();

}

//---------------------------------------------------------
// 後始末する
//---------------------------------------------------------
void StageSelectScene::Finalize()
{
}

//---------------------------------------------------------
// 次のシーンIDを取得する
//---------------------------------------------------------
IScene::SceneID StageSelectScene::GetNextSceneID() const
{

	// シーン変更がある場合
	if (m_isChangeScene)
	{
		// BGMとSEの停止
		m_commonResources->GetAudioManager()->StopSound("TitleBGM");
		m_commonResources->GetAudioManager()->StopSound("SE");
		if (m_pStageSelectMenu->GetMenuIndex() < 5)
		{
			return IScene::SceneID::PLAY;
		}
		else
		{
			return IScene::SceneID::TITLE;
		}

	}
	// シーン変更がない場合
	return IScene::SceneID::NONE;
}

