/*
	@file	PlayScene.cpp
	@brief	プレイシーンクラス
*/
#include "pch.h"
#include "Game/Scene/PlayScene.h"
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
PlayScene::PlayScene()
	:
	m_commonResources{},
	m_projection{},
	m_isChangeScene{ false },
	m_angle{ 0.0f },
	m_pStage{ nullptr },
	m_pEffect{},
	m_isFade{ false },
	m_volume{ 1.0f },
	m_counter{ 0 },
	m_fade{},
	m_fadeState{ },
	m_fadeTexNum{ 2 },
	m_audioManager{ AudioManager::GetInstance() }
{}
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
	auto device = m_commonResources->GetDeviceResources()->GetD3DDevice();
	auto context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();
	auto DR = m_commonResources->GetDeviceResources();
	// プレイヤーを初期化する
	m_pPlayer = std::make_unique<Player>(resources);

	// 敵全体を初期化する
	m_pEnemies = std::make_unique<Enemies>(resources);
	m_pPlayer->Initialize(m_pEnemies.get());
	m_pEnemies->Initialize(m_pPlayer.get());
	// 地面（ステージ１生成）
	m_pStage = std::make_unique<Stage>();
	m_pStage->Initialize(resources);
	// スカイボックス生成
	m_skybox = std::make_unique<SkyBox>();
	m_skybox->Initialize(resources);
	// プリミティブバッチを作成する
	m_primitiveBatch = std::make_unique<DX11::PrimitiveBatch<DX11::VertexPositionColor>>(context);
	// フェードの初期化
	m_fade = std::make_unique<Fade>(m_commonResources);
	m_fade->Create(DR);
	m_fade->SetState(Fade::FadeState::FadeIn);
	m_fade->SetTextureNum((int)(Fade::TextureNum::GO));
	// ベーシックエフェクトを作成する
	m_basicEffect = std::make_unique<BasicEffect>(device);
	m_basicEffect->SetVertexColorEnabled(true);
	// 入力レイアウトを作成する
	DX::ThrowIfFailed(
		CreateInputLayoutFromEffect<VertexPositionColor>
		(
			device,
			m_basicEffect.get(),
			m_inputLayout.ReleaseAndGetAddressOf()
		)
	);
	// Sound用のオブジェクトを初期化する
	InitializeFMOD();
}

//---------------------------------------------------------
// 更新する
//---------------------------------------------------------
void PlayScene::Update(float elapsedTime)
{
	// キーボードステートトラッカーを取得する
	const auto& kb = m_commonResources->GetInputManager()->GetKeyboardTracker();

	// 二重再生しない
	m_audioManager->PlaySound("BGM", 0.3);
	// カメラが向いている方向を取得する
	DirectX::SimpleMath::Vector3 cameraDirection = m_pPlayer->GetCamera()->GetDirection();
	m_pPlayer->Update(kb, elapsedTime);

	m_audioManager->Update();// オーディオマネージャーの更新
	m_pEnemies->Update(elapsedTime);// 敵の更新

	// プレイヤーのHPが0以下、または敵がいないなら
	if (m_pPlayer->GetPlayerHP() <= 0.0f || (m_pEnemies->GetEnemy().size() <= 0 && m_pEnemies->GetisBorned()))
	{
		m_fade->SetTextureNum((int)(Fade::TextureNum::BLACK));
		m_fade->SetState(Fade::FadeState::FadeOut);
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
	// 天球描画
	m_skybox->Render(view, projection, skyWorld, m_pPlayer->GetPlayerController()->GetPlayerPosition());
	// 地面描画
	m_pStage->Render(view, projection);
	// 敵を描画する
	m_pEnemies->Render();
	// プレイヤーを描画する
	m_pPlayer->Render();
	// フェードの描画
	m_fade->Render();
#ifdef _DEBUG// プレイヤーのHPを表示する
	// デバッグ情報を「DebugString」で表示する
	auto debugString = m_commonResources->GetDebugString();
	debugString->AddString("HP:%f", m_pPlayer->GetPlayerHP());
#endif
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
