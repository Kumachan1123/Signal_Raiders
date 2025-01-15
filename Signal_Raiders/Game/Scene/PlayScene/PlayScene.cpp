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
	m_BGMvolume{ BGM_VOLUME },
	m_SEvolume{ SE_VOLUME },
	m_mouseSensitivity{ },
	m_nowSceneID{ sceneID },
	m_timer{ 0.0f },
	m_waitTime{ 0.0f },
	m_stageNumber{ 0 },
	m_pBloom{ Bloom::GetInstance() }

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
	m_BGMvolume = BGM_VOLUME * static_cast<float>(m_pSettingData->GetBGMVolume());// BGMの音量を設定する(若干音量を補正)
	m_SEvolume = SE_VOLUME * static_cast<float>(m_pSettingData->GetSEVolume());// SEの音量を設定する
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
	m_pPlayer->Initialize(m_pEnemies.get());// プレイヤーを初期化する
	m_pEnemies->Initialize(m_pPlayer.get());// 敵を初期化する
	m_pEnemies->SetVolume(m_SEvolume);// 敵が出す効果音の音量を設定する
	m_pEnemies->SetWall(m_pWall.get());// 敵に壁の情報を渡す
	// 敵カウンター
	m_pEnemyCounter = std::make_unique<EnemyCounter>();
	m_pEnemyCounter->Initialize(resources);
	// 準備
	m_pGoal = std::make_unique<Goal>(m_commonResources);
	m_pGoal->Create(DR);
	// 危険状態
	m_pCrisis = std::make_unique<Crisis>(m_commonResources);
	m_pCrisis->Create(DR);
	// HPゲージ作成
	m_pPlayerHP = std::make_unique<PlayerHP>();
	m_pPlayerHP->Initialize(DR, 1280, 720);
	// 照準作成
	m_pReticle = std::make_unique<Reticle>();
	m_pReticle->Initialize(DR, 1280, 720);
	// 操作説明
	m_pPlayGuide = std::make_unique<PlayGuide>();
	m_pPlayGuide->Initialize(DR);
	// フェードの初期化
	m_fade = std::make_unique<Fade>(m_commonResources);
	m_fade->Create(DR);
	m_fade->SetState(Fade::FadeState::FadeIn);
	m_fade->SetTextureNum((int)(Fade::TextureNum::BLACK));
	// レーダーを初期化する
	m_pRadar = std::make_unique<Radar>(resources);
	m_pRadar->Initialize(m_pPlayer.get(), m_pEnemies.get());
	// ブルームエフェクトの生成
	m_pBloom->CreatePostProcess(resources);

	// Sound用のオブジェクトを初期化する
	InitializeFMOD();

}

//---------------------------------------------------------
// 更新する
//---------------------------------------------------------
void PlayScene::Update(float elapsedTime)
{
	ShowCursor(FALSE);//カーソルを見えないようにする
	// 経過時間
	m_timer += elapsedTime;
	// 二重再生しない
	m_audioManager->PlaySound("BGM", m_BGMvolume);
	// カメラが向いている方向を取得する
	DirectX::SimpleMath::Vector3 cameraDirection = m_pPlayer->GetCamera()->GetDirection();

	m_pWall->Update(elapsedTime);
	m_audioManager->Update();// オーディオマネージャーの更新

	m_pEnemies->Update(elapsedTime);// 敵の更新
	m_pPlayer->Update(elapsedTime);
	if (m_timer <= 5.0f)// ゲーム開始から5秒間は指示画像を表示
	{
		m_pGoal->Update(elapsedTime);
	}
	else// 5秒以上経過したら
	{
		// HP更新
		m_pPlayerHP->Update(m_pPlayer->GetPlayerHP());
		// 体力が10以下になったら危機状態更新
		if (m_pPlayer->GetPlayerHP() <= 10.0f)m_pCrisis->Update(elapsedTime);
		// 照準更新
		m_pReticle->Update();
		// 操作説明更新
		m_pPlayGuide->Update();
		// 敵カウンターの更新
		m_pEnemyCounter->SetEnemyIndex(m_pEnemies->GetEnemyIndex());// 敵の総数を取得
		m_pEnemyCounter->SetNowEnemy(m_pEnemies->GetEnemySize());// 現在の敵の数を取得
		m_pEnemyCounter->Update(elapsedTime);// 敵カウンターの更新

		if (m_pPlayer->GetPlayerHP() <= 0.0f ||// プレイヤーのHPが0以下か、
			(m_pEnemies->GetEnemySize() <= 0 &&// 敵がいなくて
				m_pEnemies->GetisBorned() &&// 生成が完了していて
				m_pEnemies->GetIsBossAlive() == false))// ボスがいないなら
		{
			m_waitTime += elapsedTime;// 待ち時間を加算する
		}
		if (m_waitTime >= 1.0f)// 待ち時間が5秒以上なら
		{
			m_fade->SetTextureNum((int)(Fade::TextureNum::BLACK));
			m_fade->SetState(Fade::FadeState::FadeOut);
		}
		// レーダーを更新する
		m_pRadar->Update(elapsedTime);
	}


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
	Matrix world = Matrix::Identity;
	// オフスクリーンにオブジェクトを描画する
	m_pBloom->ChangeOffScreenRT();

	// 天球描画
	m_skybox->Render(view, projection, skyWorld, m_pPlayer->GetPlayerController()->GetPlayerPosition());
	// 地面描画
	m_pStage->Render(view, projection, world, Vector3(0, 0, 0));
	// 壁描画
	m_pWall->Render(view, projection);
	// 敵を描画する
	m_pEnemies->Render();

	// プレイヤーを描画する
	m_pPlayer->Render();

	// ブルームエフェクトをかける
	m_pBloom->PostProcess();

	// レンダーターゲットを元に戻す
	m_pBloom->ChangeDefaultRT();

	// ゲーム開始から5秒以上経過したら
	if (m_timer >= 5.0f)
	{
		if (m_pPlayer->GetPlayerHP() <= 10.0f)m_pCrisis->Render();// HPが10以下で危機状態描画
		// 敵カウンターを描画する
		m_pEnemyCounter->Render();
		m_pPlayGuide->Render();// 操作説明描画
		// レーダーを描画する
		m_pRadar->Render();
		m_pPlayerHP->Render();// HP描画
		m_pReticle->Render();// 照準描画
	}
	else // ゲーム開始から5秒間は指示画像を表示
	{
		m_pGoal->Render();
	}


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
	// ここで必要な音声データをAudioManagerにロードさせる
	m_audioManager->LoadSound("Resources/Sounds/playbgm.mp3", "BGM");
}


