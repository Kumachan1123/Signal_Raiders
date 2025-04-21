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
#include "Game/Screen.h"
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
	m_isResetHP{ false },
	m_pStage{  },
	m_pEffect{},
	m_pFade{},
	m_fadeState{ },
	m_fadeTexNum{ 2 },
	//m_audioManager{ AudioManager::GetInstance() },
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
	m_BGMvolume = BGM_VOLUME * static_cast<float>(m_pSettingData->GetBGMVolume()) * 0.1f;// BGMの音量を設定する(若干音量を補正)
	m_SEvolume = SE_VOLUME * static_cast<float>(m_pSettingData->GetSEVolume()) * 0.1f;// SEの音量を設定する
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
	m_pEnemyManager = std::make_unique<EnemyManager>(resources);
	m_pEnemyManager->SetStageNumber(m_stageNumber);// ステージ番号を設定する
	m_pPlayer->SetVolume(m_SEvolume);// プレイヤーが出す効果音の音量を設定する
	if (m_SEvolume > m_BGMvolume)m_pPlayer->SetVolumeCorrection(m_SEvolume - m_BGMvolume);// プレイヤーが出す効果音の音量を補正する
	m_pPlayer->SetMouseSensitive(m_mouseSensitivity);// マウス感度を設定する
	m_pPlayer->Initialize(m_pEnemyManager.get());// プレイヤーを初期化する
	m_pEnemyManager->Initialize(m_pPlayer.get());// 敵を初期化する
	m_pEnemyManager->SetVolume(m_SEvolume);// 敵が出す効果音の音量を設定する
	m_pEnemyManager->SetWall(m_pWall.get());// 敵に壁の情報を渡す
	// 弾マネージャーを初期化する
	m_pBulletManager = std::make_unique<BulletManager>(resources);
	m_pBulletManager->Initialize(m_pPlayer.get(), m_pEnemyManager.get());
	// プレイヤーと敵マネージャーに弾マネージャーを渡す
	m_pPlayer->SetBulletManager(m_pBulletManager.get());
	m_pEnemyManager->SetBulletManager(m_pBulletManager.get());
	// 敵カウンター
	m_pEnemyCounter = std::make_unique<EnemyCounter>();
	m_pEnemyCounter->Initialize(resources);
	// 準備
	m_pGoal = std::make_unique<Goal>(m_commonResources);
	m_pGoal->Create(DR);
	// Wi-Fiローディング
	m_pWifiLoading = std::make_unique<WifiLoading>();
	m_pWifiLoading->Initialize(DR, Screen::WIDTH, Screen::HEIGHT);
	// 危険状態
	m_pCrisis = std::make_unique<Crisis>(m_commonResources);
	m_pCrisis->Create(DR);
	// HPゲージ作成
	m_pPlayerHP = std::make_unique<PlayerHP>();
	m_pPlayerHP->Initialize(DR, Screen::UI_WIDTH, Screen::UI_HEIGHT);
	// ダッシュゲージ作成
	m_pDashGauge = std::make_unique<DashGauge>();
	m_pDashGauge->Initialize(DR, Screen::UI_WIDTH, Screen::UI_HEIGHT);
	// 弾ゲージ作成
	m_pBulletGauge = std::make_unique<BulletGauge>();
	m_pBulletGauge->Initialize(DR, Screen::UI_WIDTH, Screen::UI_HEIGHT);
	// 照準作成
	m_pReticle = std::make_unique<Reticle>();
	m_pReticle->Initialize(DR, Screen::UI_WIDTH, Screen::UI_HEIGHT);
	// 操作説明
	m_pPlayGuide = std::make_unique<PlayGuide>();
	m_pPlayGuide->Initialize(DR);
	// フェードの初期化
	m_pFade = std::make_unique<Fade>(m_commonResources);
	m_pFade->Create(DR);
	m_pFade->SetState(Fade::FadeState::FadeIn);
	m_pFade->SetTextureNum((int)(Fade::TextureNum::BLACK));
	// レーダーを初期化する
	m_pRadar = std::make_unique<Radar>(resources);
	m_pRadar->Initialize(m_pPlayer.get(), m_pEnemyManager.get());
	// ボス登場演出を初期化する
	m_pBossAppear = std::make_unique<BossAppear>();
	m_pBossAppear->Initialize(m_commonResources);
	// ブルームエフェクトの生成
	m_pBloom->CreatePostProcess(resources);



}

//---------------------------------------------------------
// 更新する
//---------------------------------------------------------
void PlayScene::Update(float elapsedTime)
{

	// 経過時間
	m_timer += elapsedTime;
	// 二重再生しない
	m_commonResources->GetAudioManager()->PlaySound("PlayBGM", m_BGMvolume);
	// カメラが向いている方向を取得する
	DirectX::SimpleMath::Vector3 cameraDirection = m_pPlayer->GetCamera()->GetDirection();

	m_pWall->Update(elapsedTime);
	m_commonResources->GetAudioManager()->Update();// オーディオマネージャーの更新

	m_pEnemyManager->Update(elapsedTime);// 敵の更新
	m_pPlayer->Update(elapsedTime);// プレイヤーの更新
	m_pBulletManager->Update(elapsedTime);// 弾の更新
	if (m_timer <= 5.0f)// ゲーム開始から5秒間は
	{
		m_pGoal->Update(elapsedTime);// 指示画像を更新
		m_pWifiLoading->Update(elapsedTime);// Wi-Fiローディングを更新
	}
	else// 5秒以上経過したら
	{
		// HP更新
		m_pPlayer->SetMaxPlayerHP((float)(m_pEnemyManager->GetWifi()->GetCurrentWifiSignalQuality()));
		// HP上限が設定されたので改めて一度だけHPを設定しなおす
		if (m_isResetHP == false)
		{
			m_pPlayerHP->SetMaxHP(m_pPlayer->GetPlayerHP() + m_pPlayer->GetMaxPlayerHP());
			m_pPlayer->SetPlayerHP(m_pPlayer->GetPlayerHP() + m_pPlayer->GetMaxPlayerHP());
			m_isResetHP = true;
		}
		// HPゲージ更新
		m_pPlayerHP->Update(m_pPlayer->GetPlayerHP());
		// 体力が10以下になったら危機状態更新
		if (m_pPlayer->GetPlayerHP() <= 20.0f)m_pCrisis->Update(elapsedTime);
		// ダッシュゲージ更新
		m_pDashGauge->Update(m_pPlayer->GetDashTime());
		// 弾ゲージ更新
		m_pBulletGauge->Update(float(m_pPlayer->GetBulletManager()->GetPlayerBulletCount()));
		// 照準更新
		m_pReticle->Update();
		// 操作説明更新
		m_pPlayGuide->Update();
		// 敵カウンターの更新
		m_pEnemyCounter->SetEnemyIndex(m_pEnemyManager->GetEnemyIndex());// 敵の総数を取得
		m_pEnemyCounter->SetNowEnemy(m_pEnemyManager->GetEnemySize());// 現在の敵の数を取得
		m_pEnemyCounter->Update(elapsedTime);// 敵カウンターの更新

		if (m_pPlayer->GetPlayerHP() <= 0.0f ||// プレイヤーのHPが0以下か、
			(m_pEnemyManager->GetEnemySize() <= 0 &&// 敵がいなくて
				m_pEnemyManager->GetisBorned() &&// 生成が完了していて
				m_pEnemyManager->GetIsBossAlive() == false))// ボスがいないなら
		{
			m_waitTime += elapsedTime;// 待ち時間を加算する
		}
		if (m_waitTime >= 1.0f)// 待ち時間が1秒以上なら
		{
			m_pFade->SetTextureNum((int)(Fade::TextureNum::BLACK));
			m_pFade->SetState(Fade::FadeState::FadeOut);
		}
		// レーダーを更新する
		m_pRadar->Update(elapsedTime);
		// ボス登場演出を更新する
		if (m_pEnemyManager->GetIsBossAppear() == true)m_pBossAppear->Update(elapsedTime);
	}


	// 画面遷移フェード処理
	m_pFade->Update(elapsedTime);
	// フェードアウトが終了したら
	if (m_pFade->GetState() == Fade::FadeState::FadeOutEnd)m_isChangeScene = true;
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
	m_pEnemyManager->Render();
	// プレイヤーを描画する
	m_pPlayer->Render();
	// 弾を描画する
	m_pBulletManager->Render();
	// ブルームエフェクトをかける
	m_pBloom->PostProcess();
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
		m_pDashGauge->Render();// ダッシュゲージ描画
		m_pBulletGauge->Render();// 弾ゲージ描画
		m_pReticle->Render();// 照準描画
		// ボス登場演出を更新する
		if (m_pEnemyManager->GetIsBossAppear() == true)m_pBossAppear->Render();
	}
	else // ゲーム開始から5秒間
	{
		m_pGoal->Render();// 指示画像を表示
		m_pWifiLoading->Render();// Wi-Fiローディングを表示
	}
	// フェードの描画
	m_pFade->Render();

#ifdef _DEBUG
	// デバッグ情報を表示する
	auto debugString = m_commonResources->GetDebugString();
	debugString->AddString("Power:%i", m_pEnemyManager->GetWifi()->GetCurrentWifiSignalQuality());
	debugString->AddString("SSID:%i", m_pEnemyManager->GetWifi()->GetCurrentWifiSSIDLength());
	debugString->AddString("HP:%f", m_pPlayer->GetPlayerHP());
	debugString->AddString("MAXHP:%f", m_pPlayer->GetMaxPlayerHP());
#endif
}
//---------------------------------------------------------
// 後始末する
//---------------------------------------------------------
void PlayScene::Finalize()
{

	m_skybox.reset();
	//m_audioManager->Shutdown();
}
//---------------------------------------------------------
// 次のシーンIDを取得する
//---------------------------------------------------------
IScene::SceneID PlayScene::GetNextSceneID() const
{
	// シーン変更がある場合
	if (m_isChangeScene)
	{
		m_commonResources->GetAudioManager()->StopSound("PlayBGM");// BGMを停止する
		// プレイヤーのHPが0以下なら
		if (m_pPlayer->GetPlayerHP() <= 0.0f)return IScene::SceneID::GAMEOVER;// ゲームオーバーシーンへ
		// 敵がいないなら
		else return IScene::SceneID::CLEAR;// クリアシーンへ
	}
	// シーン変更がない場合
	return IScene::SceneID::NONE;// 何もしない
}

