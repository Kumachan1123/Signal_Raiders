/*
	@file	ResultScene.cpp
	@brief　リザルトシーンクラス
*/
#include "pch.h"
#include "ResultScene.h"
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
ResultScene::ResultScene(IScene::SceneID sceneID)
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
	m_nowSceneID{ sceneID },
	m_stageNumber{ 0 }
{
}

//---------------------------------------------------------
// デストラクタ
//---------------------------------------------------------
ResultScene::~ResultScene()
{
	// do nothing.
	Finalize();
}

//---------------------------------------------------------
// 初期化する
//---------------------------------------------------------
void ResultScene::Initialize(CommonResources* resources)
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
	// リザルトメニューを作成する

	// マウスポインターを作成

	m_pUI.push_back(std::move(std::make_unique<ResultMenu>()));
	m_pUI.push_back(std::move(std::make_unique<MousePointer>()));
	for (int it = 0; it < m_pUI.size(); ++it)
	{
		m_pUI[it]->Initialize(m_commonResources, Screen::WIDTH, Screen::HEIGHT);
	}

	// 設定ファイルの読み込み
	m_pSettingData = std::make_unique<SettingData>();
	m_pSettingData->Load();
	m_BGMvolume = VOLUME * static_cast<float>(m_pSettingData->GetBGMVolume()) * .1f;
	m_SEvolume = VOLUME * static_cast<float>(m_pSettingData->GetSEVolume()) * .1f;

	// 今のシーンIDによってテクスチャを変更
	switch (m_nowSceneID)
	{
	case IScene::SceneID::GAMEOVER:
		m_pTexturePath = L"Resources/Textures/GameOver.png";
		break;
	case IScene::SceneID::CLEAR:
		m_pTexturePath = L"Resources/Textures/Clear.png";
		break;
	}
	// 結果クラス作成
	m_pResult = std::make_unique<Result>(m_commonResources);
	m_pResult->Create(DR, m_pTexturePath);
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
void ResultScene::Update(float elapsedTime)
{


	// オーディオマネージャーの更新
	m_commonResources->GetAudioManager()->Update();
	// キーボードステートトラッカーを取得する
	const auto& kbTracker = m_commonResources->GetInputManager()->GetKeyboardTracker();
	// マウスのトラッカーを取得する
	auto& mtracker = m_commonResources->GetInputManager()->GetMouseTracker();
	UpdateContext ctx;
	ctx.bulletPoint = 0;// 使わない
	ctx.dashStamina = 0;// 使わない
	ctx.elapsedTime = elapsedTime;// フレーム時間
	ctx.playerHP = 0;// 使わない
	for (int it = 0; it < m_pUI.size(); ++it)
	{
		m_pUI[it]->Update(ctx);

		auto pMenu = dynamic_cast<ResultMenu*>(m_pUI[it].get());
		if (!pMenu) continue;
		// スペースキーが押されたら
		if (m_pFade->GetState() == Fade::FadeState::FadeInEnd &&
			mtracker->GetLastState().leftButton &&
			pMenu->GetIsHit())
		{
			// SEの再生
			m_commonResources->GetAudioManager()->PlaySound("SE", m_SEvolume);
			m_pFade->SetState(Fade::FadeState::FadeOut);// フェードアウトに移行
			m_pFade->SetTextureNum((int)(Fade::TextureNum::BLACK));// フェードのテクスチャを変更
			ResultMenu::SceneID id = pMenu->GetSceneNum();
			if (id == ResultMenu::SceneID::END)
			{
				m_stageNumber = 5;// タイトルに戻る
			}

			// WかSのいずれかが押されたら
			if (kbTracker->pressed.W || kbTracker->pressed.S)
				m_commonResources->GetAudioManager()->PlaySound("Select", m_SEvolume);// SEの再生

		}
	}

	// フェードアウトが終了したら
	if (m_pFade->GetState() == Fade::FadeState::FadeOutEnd)
	{
		m_isChangeScene = true;
	}
	// BGMの再生
	m_commonResources->GetAudioManager()->PlaySound("ResultBGM", m_BGMvolume);
	// フェードに関する準備
	m_time += elapsedTime; // 時間をカウント
	m_size = (sin(m_time) + 1.0f) * 0.3f + 0.75f; // sin波で0.5〜1.5の間を変動させる
	m_pressKeySize = (cos(m_time) + 1.0f) * 0.3f + 0.75f; // sin波で0.5〜1.5の間を変動させる
	// 背景の更新
	m_pBackGround->Update(elapsedTime);
	// マウスポインターの更新
	//m_pMousePointer->Update(elapsedTime);
	// 	// リザルトメニューの更新
	//m_pResultMenu->Update(elapsedTime);

	// フェードの更新
	m_pFade->Update(elapsedTime);

	// 結果の更新
	m_pResult->Update(elapsedTime);
}

//---------------------------------------------------------
// 描画する
//---------------------------------------------------------
void ResultScene::Render()
{
	// 背景の描画
	m_pBackGround->Render();

	// メニューと結果を描画
	if (m_pFade->GetState() == Fade::FadeState::FadeInEnd)
	{
		m_pResult->Render();
		//m_pResultMenu->Render();
		////m_pMousePointer->Render();// マウスポインター更新
		for (int it = 0; it < m_pUI.size(); ++it)
		{
			m_pUI[it]->Render();
		}
	}



	// フェードの描画
	m_pFade->Render();

}

//---------------------------------------------------------
// 後始末する
//---------------------------------------------------------
void ResultScene::Finalize()
{
}

//---------------------------------------------------------
// 次のシーンIDを取得する
//---------------------------------------------------------
IScene::SceneID ResultScene::GetNextSceneID() const
{

	// シーン変更がある場合
	if (m_isChangeScene)
	{
		// BGMとSEの停止
		m_commonResources->GetAudioManager()->StopSound("ResultBGM");
		m_commonResources->GetAudioManager()->StopSound("SE");
		for (int it = 0; it < m_pUI.size(); ++it)
		{
			if (auto pMenu = dynamic_cast<ResultMenu*>(m_pUI[it].get()))
			{
				switch (pMenu->GetSceneNum())
				{
				case ResultMenu::SceneID::REPLAY:
					return IScene::SceneID::PLAY;
					break;
				case ResultMenu::SceneID::END:
					return IScene::SceneID::STAGESELECT;

					break;
				default:
					break;
				}
			}
		}

	}
	// シーン変更がない場合
	return IScene::SceneID::NONE;
}



