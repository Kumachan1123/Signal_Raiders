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
	m_model{},
	m_angle{ 0.0f },
	//m_wifi{ nullptr },
	m_stage1{ nullptr },
	m_effect{},
	m_isFade{ false },
	m_volume{ 1.0f },
	m_counter{ 0 },
	m_enemyBornTimer{ 0.0f },
	m_enemyBornInterval{ 0.5f },
	m_enemyIndex{ 0 },
	m_startTime{ 0.0f },
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
	m_pPlayer->Initialize();
	// 敵全体を初期化する
	m_pEnemies = std::make_unique<Enemies>(resources);
	m_pEnemies->Initialize(m_pPlayer.get());
	// 地面（ステージ１生成）
	m_stage1 = std::make_unique<Stage1>();
	m_stage1->Initialize(resources);
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
	auto& mtracker = m_commonResources->GetInputManager()->GetMouseTracker();
	// 二重再生しない
	m_audioManager->PlaySound("BGM", 0.3);
	// カメラが向いている方向を取得する
	DirectX::SimpleMath::Vector3 cameraDirection = m_pPlayer->GetCamera()->GetDirection();
	m_pPlayer->Update(kb, elapsedTime);
#ifdef _DEBUG// デバッグ
	// 右クリックで敵を一掃
	if (mtracker->GetLastState().rightButton)for (auto& enemy : m_enemy)enemy->SetEnemyHP(0);
	// スペースキーでプレイヤーのHPを0にする
	if (kb->pressed.Space)m_pPlayer->SetPlayerHP(0.0f);
#endif
	// 左クリックで弾発射
	if (mtracker->GetLastState().leftButton && !m_isBullet)
	{
		// SEの再生
		m_audioManager->PlaySound("SE", .5);
		// 弾を生成する
		auto bullet = std::make_unique<PlayerBullet>();
		bullet->Initialize(m_commonResources);
		bullet->MakeBall(m_pPlayer->GetPlayerController()->GetPlayerPosition(), cameraDirection);
		m_playerBullets.push_back(std::move(bullet));
		m_isBullet = true;
	}
	if (!mtracker->GetLastState().leftButton)m_isBullet = false;


	m_audioManager->Update();// オーディオマネージャーの更新

	m_pEnemies->Update(elapsedTime);// 敵の更新
	UpdateBullets(elapsedTime);// 弾の更新
	//UpdateEnemies(elapsedTime);// 敵の更新

	// パーティクルの更新
	for (auto& particle : m_effect) particle->Update(elapsedTime);
	// プレイヤーのHPが0以下、または敵がいないなら
	if (m_pPlayer->GetPlayerHP() <= 0.0f || (m_enemy.size() <= 0 && m_isBorned))
	{
		m_fade->SetTextureNum((int)(Fade::TextureNum::BLACK));
		m_fade->SetState(Fade::FadeState::FadeOut);
	}
	// 画面遷移フェード処理
	m_fade->Update(elapsedTime);
	// フェードアウトが終了したら
	if (m_fade->GetState() == Fade::FadeState::FadeOutEnd)m_isChangeScene = true;
	// 敵を生成するまでの待機時間５秒カウントする
	m_startTime += elapsedTime;
}

//---------------------------------------------------------
// 描画する
//---------------------------------------------------------
void PlayScene::Render()
{
	auto context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();

	// カメラからビュー行列と射影行列を取得する
	Matrix view = m_pPlayer->GetCamera()->GetViewMatrix();
	Matrix projection = m_pPlayer->GetCamera()->GetProjectionMatrix();
	Matrix skyWorld = Matrix::Identity * Matrix::CreateScale(10);
	// 天球描画
	m_skybox->Render(view, projection, skyWorld, m_pPlayer->GetPlayerController()->GetPlayerPosition());
	// 地面描画
	m_stage1->Render(view, projection);
	// 弾を描画する
	for (const auto& bullet : m_playerBullets)bullet->Render(view, projection);
	// 敵を描画する
	m_pEnemies->Render();
	//if (m_enemy.size() > 0) for (const auto& enemy : m_enemy)enemy->Render(view, projection);

	// エフェクトを描画する
	m_effect.erase
	(std::remove_if(m_effect.begin(), m_effect.end(), [&](const std::unique_ptr<Effect>& particle)//	再生終了したパーティクルを削除する
					{
						if (!particle->IsPlaying()) return true;// 再生終了したパーティクルは削除する
						particle->Render(context, view, projection);// パーティクルを描画する
						return false;//	再生中のパーティクルは削除しない
					}),
	 m_effect.end()//	削除対象のパーティクルを削除する
	);
	// デバッグ情報を「DebugString」で表示する
	auto debugString = m_commonResources->GetDebugString();
#ifdef _DEBUG
	debugString->AddString("HP:%f", m_pPlayer->GetPlayerHP());
#endif
	//m_wifi->Render(debugString);
	m_pPlayer->Render();
	// フェードの描画
	m_fade->Render();
}
//---------------------------------------------------------
// 後始末する
//---------------------------------------------------------
void PlayScene::Finalize()
{
	m_playerBullets.clear();
	m_enemy.clear();

	m_model.reset();
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
		m_audioManager->StopSound("SE");// SEを停止する
		m_audioManager->StopSound("EnemyDead");// 敵死亡SEを停止する
		// プレイヤーのHPが0以下なら
		if (m_pPlayer->GetPlayerHP() <= 0.0f)
		{
			return IScene::SceneID::GAMEOVER;// ゲームオーバーシーンへ
		}
		else return IScene::SceneID::CLEAR;// クリアシーンへ
	}
	// シーン変更がない場合
	return IScene::SceneID::NONE;// 何もしない
}

void PlayScene::UpdateBullets(float elapsedTime)
{

	DirectX::SimpleMath::Vector3 dir = m_pPlayer->GetCamera()->GetDirection();
	for (auto it = m_playerBullets.begin(); it != m_playerBullets.end(); )
	{
		(*it)->Update(dir, elapsedTime);
		if ((*it)->IsExpired())it = m_playerBullets.erase(it);
		else
		{
			bool isHit = false;
			for (auto& enemy : m_pEnemies->GetEnemy())
			{
				if ((*it)->GetBoundingSphere().Intersects(enemy->GetBoundingSphere()))
				{
					isHit = true;
					enemy->SetEnemyHP(enemy->GetHP() - (*it)->Damage());
					m_effect.push_back(std::make_unique<Effect>(m_commonResources,
																Effect::ParticleType::ENEMY_HIT,
																enemy->GetPosition(),
																enemy->GetMatrix()));
					enemy->SetHitToPlayerBullet(true);
					m_audioManager->PlaySound("Hit", 0.3);// ヒットSEを再生
					break;
				}
			}
			if (isHit) it = m_playerBullets.erase(it);
			else it++;
		}
	}
}
void PlayScene::InitializeFMOD()
{

	// FMODシステムの初期化
	m_audioManager->Initialize();

	// 音声データのロード
	// ここで必要な音声データをAudioManagerにロードさせる
	// 例：audioManager->LoadSound("Resources/Sounds/音声.mp3", "チャンネル名");
	m_audioManager->LoadSound("Resources/Sounds/playerBullet.mp3", "SE");
	m_audioManager->LoadSound("Resources/Sounds/playbgm.mp3", "BGM");
	m_audioManager->LoadSound("Resources/Sounds/Explosion.mp3", "EnemyDead");
	m_audioManager->LoadSound("Resources/Sounds/damage.mp3", "Damage");
	m_audioManager->LoadSound("Resources/Sounds/hit.mp3", "Hit");
	m_audioManager->LoadSound("Resources/Sounds/enemybullet.mp3", "EnemyBullet");


}
