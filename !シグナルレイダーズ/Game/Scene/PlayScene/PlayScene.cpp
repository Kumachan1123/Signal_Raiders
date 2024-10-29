/*
	@file	PlayScene.cpp
	@brief	プレイシーンクラス
*/
#include "pch.h"
#include "Game/Scene/PlayScene/PlayScene.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/GridFloor.h"
#include "Libraries/MyLib/InputManager.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include <vector>
#include <cassert>
#include <SimpleMath.h>
#include <Model.h>
#include <Effects.h>
#include <memory>
#include <Libraries/Microsoft/DebugDraw.h>


using namespace DirectX;
using namespace DirectX::SimpleMath;
//---------------------------------------------------------
// コンストラクタ
//---------------------------------------------------------
PlayScene::PlayScene(IScene::SceneID sceneID)
	:
	m_commonResources{},
	m_projection{},
	m_isChangeScene{ false },
	m_pStage{ nullptr },
	m_pEffect{},
	m_fade{},
	m_fadeState{ },
	m_fadeTexNum{ 2 },
	m_audioManager{ AudioManager::GetInstance() },
	m_BGMvolume{ VOLUME },
	m_SEvolume{ VOLUME },
	m_mouseSensitivity{ },
	m_nowSceneID{ sceneID },
	m_stageNumber{ 0 }

{

}
//---------------------------------------------------------
// デストラクタ
//---------------------------------------------------------
PlayScene::~PlayScene() { Finalize(); }
//---------------------------------------------------------
// 初期化する
//---------------------------------------------------------
void PlayScene::Initialize(CommonResources* resources)
{
	assert(resources);
	m_commonResources = resources;
	auto DR = m_commonResources->GetDeviceResources();



	// 設定データを取得する
	m_pSettingData = std::make_unique<SettingData>();
	m_pSettingData->Load();
	m_BGMvolume = VOLUME * static_cast<float>(m_pSettingData->GetBGMVolume()) / 2;// BGMの音量を設定する(若干音量を補正)
	m_SEvolume = VOLUME * static_cast<float>(m_pSettingData->GetSEVolume());// SEの音量を設定する
	m_mouseSensitivity = static_cast<float>(m_pSettingData->GetMouseSensitivity());// マウス感度を設定する
	// 地面（ステージ生成）
	m_pStage = std::make_unique<Stage>();
	m_pStage->Initialize(resources);
	// 壁
	m_pWall = std::make_unique<Wall>(resources);
	m_pWall->Create(DR);
	// スカイボックス生成
	m_skybox = std::make_unique<Sky>(m_stageNumber);
	m_skybox->Initialize(resources);
	// プレイヤーを初期化する
	m_pPlayer = std::make_unique<Player>(resources);
	// 敵全体を初期化する
	m_pEnemies = std::make_unique<Enemies>(resources);
	m_pEnemies->SetStageNumber(m_stageNumber);// ステージ番号を設定する
	m_pPlayer->SetVolume(m_SEvolume);// プレイヤーが出す効果音の音量を設定する
	m_pPlayer->SetMouseSensitive(m_mouseSensitivity);// マウス感度を設定する
	m_pPlayer->Initialize(m_pEnemies.get());
	m_pEnemies->Initialize(m_pPlayer.get());
	m_pEnemies->SetVolume(m_SEvolume);// 敵が出す効果音の音量を設定する

	// 敵カウンター
	m_pEnemyCounter = std::make_unique<EnemyCounter>();
	m_pEnemyCounter->Initialize(resources);
	// フェードの初期化
	m_fade = std::make_unique<Fade>(m_commonResources);
	m_fade->Create(DR);
	m_fade->SetState(Fade::FadeState::FadeIn);
	m_fade->SetTextureNum((int)(Fade::TextureNum::GO));
	// レーダーを初期化する
	m_pRadar = std::make_unique<Radar>(resources);
	m_pRadar->Initialize(m_pPlayer.get(), m_pEnemies.get());
	// Sound用のオブジェクトを初期化する
	InitializeFMOD();

}

//---------------------------------------------------------
// 更新する
//---------------------------------------------------------
void PlayScene::Update(float elapsedTime)
{
	ShowCursor(FALSE);//カーソルを見えないようにする
	// キーボードステートトラッカーを取得する
	const auto& kb = m_commonResources->GetInputManager()->GetKeyboardTracker();

	// 二重再生しない
	m_audioManager->PlaySound("BGM", m_BGMvolume);
	// カメラが向いている方向を取得する
	DirectX::SimpleMath::Vector3 cameraDirection = m_pPlayer->GetCamera()->GetDirection();
	m_pPlayer->Update(kb, elapsedTime);
	m_pWall->Update(elapsedTime);
	m_audioManager->Update();// オーディオマネージャーの更新
	m_pEnemies->Update(elapsedTime);// 敵の更新
	m_pEnemyCounter->SetEnemyIndex(m_pEnemies->GetEnemyIndex());// 敵の総数を取得
	m_pEnemyCounter->SetNowEnemy(m_pEnemies->GetEnemySize());// 現在の敵の数を取得
	m_pEnemyCounter->Update(elapsedTime);// 敵カウンターの更新
	// プレイヤーのHPが0以下、または敵がいないなら
	if (m_pPlayer->GetPlayerHP() <= 0.0f ||
		(m_pEnemies->GetEnemySize() <= 0 && m_pEnemies->GetisBorned() && m_pEnemies->GetIsBossAlive() == false))
	{
		m_fade->SetTextureNum((int)(Fade::TextureNum::BLACK));
		m_fade->SetState(Fade::FadeState::FadeOut);
	}
	// レーダーを更新する
	m_pRadar->Update(elapsedTime);

	// 画面遷移フェード処理
	m_fade->Update(elapsedTime);
	// フェードアウトが終了したら
	if (m_fade->GetState() == Fade::FadeState::FadeOutEnd)m_isChangeScene = true;
}

//---------------------------------------------------------
// 描画する
//---------------------------------------------------------
void PlayScene::Render()
{
	// カメラからビュー行列と射影行列を取得する
	Matrix view = m_pPlayer->GetCamera()->GetViewMatrix();
	Matrix projection = m_pPlayer->GetCamera()->GetProjectionMatrix();
	Matrix skyWorld = Matrix::Identity * Matrix::CreateScale(10);
	// 天球描画
	m_skybox->Render(view, projection, skyWorld, m_pPlayer->GetPlayerController()->GetPlayerPosition());
	// 地面描画
	m_pStage->Render(view, projection);
	// 壁描画
	m_pWall->Render(view, projection);
	// 敵を描画する
	m_pEnemies->Render();
	// プレイヤーを描画する
	m_pPlayer->Render();
	// 敵カウンターを描画する
	m_pEnemyCounter->Render();
	// レーダーを描画する
	m_pRadar->Render();

	// フェードの描画
	m_fade->Render();


}
//---------------------------------------------------------
// 後始末する
//---------------------------------------------------------
void PlayScene::Finalize()
{
	m_playerBullet.clear();
	m_enemy.clear();
	m_skybox.reset();
	m_audioManager->Shutdown();
}
//---------------------------------------------------------
// 次のシーンIDを取得する
//---------------------------------------------------------
IScene::SceneID PlayScene::GetNextSceneID() const
{
	// シーン変更がある場合
	if (m_isChangeScene)
	{
		m_audioManager->StopSound("BGM");// BGMを停止する
		// プレイヤーのHPが0以下なら
		if (m_pPlayer->GetPlayerHP() <= 0.0f)return IScene::SceneID::GAMEOVER;// ゲームオーバーシーンへ
		// 敵がいないなら
		else return IScene::SceneID::CLEAR;// クリアシーンへ
	}
	// シーン変更がない場合
	return IScene::SceneID::NONE;// 何もしない
}

void PlayScene::InitializeFMOD()
{
	// FMODシステムの初期化
	m_audioManager->Initialize();
	// ここで必要な音声データをAudioManagerにロードさせる
	m_audioManager->LoadSound("Resources/Sounds/playbgm.mp3", "BGM");
}


