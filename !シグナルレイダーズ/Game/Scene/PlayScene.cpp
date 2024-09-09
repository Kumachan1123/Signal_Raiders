/*
	@file	PlayScene.cpp
	@brief	プレイシーンクラス
*/
#include "pch.h"
#include "Game/Scene/PlayScene.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Game/FPS_Camera/FPS_Camera.h"
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
	m_gridFloor{},
	m_projection{},
	m_isChangeScene{},
	m_model{},
	m_angle{},
	m_camera{},
	m_wifi{ nullptr },
	m_stage1{ nullptr },
	m_particles{},
	m_isFade{},
	m_volume{},
	m_counter{},
	m_enemyBornTimer{ 0.0f },
	m_enemyBornInterval{ 0.5f },
	m_enemyIndex{ 0 },
	m_fade{},
	m_fadeState{ },
	m_fadeTexNum{ 2 }
{
}
//---------------------------------------------------------
// デストラクタ
//---------------------------------------------------------
PlayScene::~PlayScene()
{
	// do nothing.
	Finalize();
}
//---------------------------------------------------------
// 初期化する
//---------------------------------------------------------
void PlayScene::Initialize(CommonResources* resources)
{
	assert(resources);
	m_commonResources = resources;
	auto device = m_commonResources->GetDeviceResources()->GetD3DDevice();
	auto context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();
	auto states = m_commonResources->GetCommonStates();
	auto DR = m_commonResources->GetDeviceResources();

	m_wifi = std::make_unique<Wifi>();
	m_wifi->Initialize();
	// グリッド床を作成する
	m_gridFloor = std::make_unique<mylib::GridFloor>(device, context, states);
	m_gridFloor->SetColor(DirectX::Colors::Yellow);
	// 地面（ステージ１生成）
	m_stage1 = std::make_unique<Stage1>();
	m_stage1->Initialize(resources);

	// FPSカメラを作成する
	m_camera = std::make_unique<FPS_Camera>();
	// コントローラー生成
	m_playerController = std::make_unique<PlayerController>();
	m_playerController->Initialize(resources);
	m_playerController->SetPlayetPosition(m_camera->GetEyePosition());
	// 回転角を初期化する（度）
	m_angle = 0;
	// シーン変更フラグを初期化する
	m_isChangeScene = false;
	// スカイボックス生成
	m_skybox = std::make_unique<SkyBox>();
	m_skybox->Initialize(resources);
	// プリミティブバッチを作成する
	m_primitiveBatch = std::make_unique<DX11::PrimitiveBatch<DX11::VertexPositionColor>>(context);
	m_inPlayerArea.Radius = 20.0f;
	m_PlayerSphere.Radius = 2.0f;
	// HPゲージ作成
	m_pPlayerHP = std::make_unique<PlayerHP>();
	m_pPlayerHP->Initialize(DR, 1280, 720);
	// 照準作成
	m_pPlayerPointer = std::make_unique<PlayerPointer>();
	m_pPlayerPointer->Initialize(DR, 1280, 720);

	// フェードの初期化
	m_fade = std::make_unique<Fade>(m_commonResources);
	m_fade->Create(DR);
	m_fade->SetState(Fade::FadeState::FadeIn);
	m_fade->SetTextureNum((int)(Fade::TextureNum::GO));

	/*
		デバッグドローの表示用オブジェクトを生成する
	*/
	// ベーシックエフェクトを作成する
	m_basicEffect = std::make_unique<BasicEffect>(device);
	m_basicEffect->SetVertexColorEnabled(true);

	// 入力レイアウトを作成する
	DX::ThrowIfFailed(
		CreateInputLayoutFromEffect<VertexPositionColor>(
			device,
			m_basicEffect.get(),
			m_inputLayout.ReleaseAndGetAddressOf()
		)
	);


	// Sound用のオブジェクトを初期化する
	InitializeFMOD();

	// フェードに関する準備
	m_isFade = false;
	m_volume = 1.0f;
	m_counter = 0;
}

//---------------------------------------------------------
// 更新する
//---------------------------------------------------------
void PlayScene::Update(float elapsedTime)
{
	UNREFERENCED_PARAMETER(elapsedTime);
	// キーボードステートトラッカーを取得する
	const auto& kb = m_commonResources->GetInputManager()->GetKeyboardTracker();
	auto& mtracker = m_commonResources->GetInputManager()->GetMouseTracker();
	// オーディオマネージャーのインスタンスを取得
	auto audioManager = AudioManager::GetInstance();
	// 二重再生しない
	audioManager->PlaySound("BGM", 0.3);

	// カメラが向いている方向を取得する
	DirectX::SimpleMath::Vector3 cameraDirection = m_camera->GetDirection();
	m_playerController->Update(kb, cameraDirection, elapsedTime);

	// 〇〇を回転する
	m_angle += 0.025f;
	if (m_angle > 360)m_angle = 0;
	// FPSカメラ位置を更新する
	m_camera->Update(m_playerController->GetPlayerPosition(), m_playerController->GetYawX());
	m_camera->SetTargetPositionY(m_playerController->GetPitch());
	m_pPlayerHP->Update(m_playerHP);
	m_pPlayerPointer->Update();

#ifdef _DEBUG
	// デバッグ用
   // 右クリックで敵を一掃
	if (mtracker->GetLastState().rightButton)
	{
		for (auto& enemy : m_enemy)
		{
			enemy->SetEnemyHP(0);
		}
	}
	// スペースキーでプレイヤーのHPを0にする
	if (kb->pressed.Space)
	{
		m_playerHP = 0.0f;
	}
#endif


	// 左クリックで弾発射
	if (mtracker->GetLastState().leftButton && !m_isBullet)
	{
		// SEの再生
		audioManager->PlaySound("SE", .3);
		auto bullet = std::make_unique<PlayerBullet>();
		bullet->Initialize(m_commonResources);
		bullet->MakeBall(m_playerController->GetPlayerPosition(), cameraDirection);
		m_playerBullets.push_back(std::move(bullet));
		m_isBullet = true;
	}
	if (!mtracker->GetLastState().leftButton)
	{
		m_isBullet = false;
	}
	m_wifi->Update(elapsedTime);
	UpdateBullets(elapsedTime);


	UpdateEnemies(elapsedTime);
	m_inPlayerArea.Center = m_playerController->GetPlayerPosition();// プレイヤーの位置を取得
	m_PlayerSphere.Center = m_playerController->GetPlayerPosition();// プレイヤーの位置を取得
	// パーティクルの更新
	for (auto& particle : m_particles) particle->Update(elapsedTime);
	// プレイヤーのHPが0以下なら
	if (m_playerHP <= 0.0f)
	{
		m_fade->SetTextureNum((int)(Fade::TextureNum::BLACK));
		m_fade->SetState(Fade::FadeState::FadeOut);


	}
	// 画面遷移フェード処理
	m_fade->Update(elapsedTime);
	// フェードアウトが終了したら
	if (m_fade->GetState() == Fade::FadeState::FadeOutEnd)
	{
		m_isChangeScene = true;
	}
}

//---------------------------------------------------------
// 描画する
//---------------------------------------------------------
void PlayScene::Render()
{


	auto context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();
	auto states = m_commonResources->GetCommonStates();
	// カメラからビュー行列と射影行列を取得する
	Matrix view = m_camera->GetViewMatrix();
	Matrix projection = m_camera->GetProjectionMatrix();
	Matrix skyWorld = Matrix::CreateRotationY(XMConvertToRadians(m_angle));
	skyWorld *= Matrix::CreateScale(10);
#ifdef _DEBUG
	// 格子床を描画する
	m_gridFloor->Render(context, view, projection);
#endif
	// スカイボックス描画
	m_skybox->Render(view, projection, skyWorld, m_playerController->GetPlayerPosition());
	// 地面描画
	m_stage1->Render(view, projection);
	// 各パラメータを設定する
	context->OMSetBlendState(states->Opaque(), nullptr, 0xFFFFFFFF);
	context->OMSetDepthStencilState(states->DepthRead(), 0);
	context->RSSetState(states->CullNone());
	context->IASetInputLayout(m_inputLayout.Get());
	//** デバッグドローでは、ワールド変換いらない
	m_basicEffect->SetView(view);
	m_basicEffect->SetProjection(projection);
	m_basicEffect->Apply(context);


#ifdef _DEBUG
	m_primitiveBatch->Begin();
	DX::Draw(m_primitiveBatch.get(), m_PlayerSphere, DirectX::Colors::PeachPuff);
	m_primitiveBatch->End();
#endif

	// 弾を描画する
	for (const auto& bullet : m_playerBullets)bullet->Render(view, projection);

	// 敵を描画する
	if (m_enemy.size() > 0)
	{
		for (const auto& enemy : m_enemy)
		{

			enemy->Render(view, projection);

		}
	}

	// パーティクルを描画する
	m_particles.erase
	(std::remove_if(m_particles.begin(), m_particles.end(), [&](const std::unique_ptr<Particle>& particle)//	再生終了したパーティクルを削除する
					{
						if (!particle->IsPlaying()) return true;// 再生終了したパーティクルは削除する
						particle->Render(context, view, projection);// パーティクルを描画する
						return false;//	再生中のパーティクルは削除しない
					}),
	 m_particles.end()//	削除対象のパーティクルを削除する
	);
	// デバッグ情報を「DebugString」で表示する
	auto debugString = m_commonResources->GetDebugString();
	m_wifi->Render(debugString);
	m_pPlayerHP->Render();
	m_pPlayerPointer->Render();

	// フェードの描画
	m_fade->Render();
}
//---------------------------------------------------------
// 後始末する
//---------------------------------------------------------
void PlayScene::Finalize()
{
	// オーディオマネージャーのインスタンスを取得
	auto audioManager = AudioManager::GetInstance();;
	// do nothing.
	m_playerBullets.clear();
	m_enemy.clear();
	m_wifi.reset();
	m_gridFloor.reset();
	m_model.reset();
	m_camera.reset();
	m_playerController.reset();
	m_skybox.reset();

	audioManager->Shutdown();
}
//---------------------------------------------------------
// 次のシーンIDを取得する
//---------------------------------------------------------
IScene::SceneID PlayScene::GetNextSceneID() const
{
	// オーディオマネージャーのインスタンスを取得
	auto audioManager = AudioManager::GetInstance();
	// シーン変更がある場合
	if (m_isChangeScene)
	{
		audioManager->StopSound("BGM");// BGMを停止する
		audioManager->StopSound("SE");// SEを停止する

		if (m_playerHP <= 0.0f)// プレイヤーのHPが0以下なら
		{
			return IScene::SceneID::GAMEOVER;// ゲームオーバーシーンへ
		}
		else
		{
			return IScene::SceneID::CLEAR;// クリアシーンへ
		}

	}
	// シーン変更がない場合
	return IScene::SceneID::NONE;// 何もしない
}

void PlayScene::UpdateBullets(float elapsedTime)
{
#ifdef _DEBUG
	auto debugString = m_commonResources->GetDebugString();
	debugString->AddString("HP:%f", m_playerHP);
#endif
	DirectX::SimpleMath::Vector3 dir = m_camera->GetDirection();
	for (auto it = m_playerBullets.begin(); it != m_playerBullets.end(); )
	{
		(*it)->Update(dir, elapsedTime);

		if ((*it)->IsExpired())
		{
			it = m_playerBullets.erase(it);
		}
		else
		{
			bool isHit = false;
			for (auto& enemy : m_enemy)
			{
				if ((*it)->GetBoundingSphere().Intersects(enemy->GetBoundingSphere()))
				{
					isHit = true;
					m_count++;//debug
					enemy->SetEnemyHP(enemy->GetHP() - (*it)->Damage());
					m_particles.push_back(std::make_unique<Particle>(m_commonResources,
																	 Particle::ParticleType::ENEMY_HIT,
																	 enemy->GetPosition(),
																	 enemy->GetMatrix()));
					enemy->SetHitToPlayerBullet(true);
					break;
				}
			}
			if (isHit)
			{

				it = m_playerBullets.erase(it);
			}
			else
			{
				++it;
			}
		}
	}
}

void PlayScene::UpdateEnemies(float elapsedTime)
{
	// 敵が全滅したらシーンを変更する・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・
	if (m_enemy.size() <= 0 && m_isBorned)
	{
		m_fade->SetTextureNum((int)(Fade::TextureNum::BLACK));
		m_fade->SetState(Fade::FadeState::FadeOut);
	}
	// 敵生成・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・
	// 敵生成タイマーを更新
	m_enemyBornTimer += elapsedTime;
	if (!m_isEnemyBorn && m_wifi->GetPreWifiLevels().empty())m_isEnemyBorn = true;//生成可能にする
	// 生成可能なら
	if (m_isEnemyBorn && !m_isBorned && m_enemyIndex < m_wifi->GetWifiLevels().size())
	{
		if (m_enemyBornTimer >= m_enemyBornInterval)// 一定時間経過したら
		{
			// 敵を生成する
			auto enemy = std::make_unique<Enemy>();
			enemy->Initialize(m_commonResources, m_wifi->GetWifiLevels()[m_enemyIndex]);


			m_enemy.push_back(std::move(enemy));
			// タイマーをリセットし、次のWi-Fiレベルのインデックスに進む
			m_enemyBornTimer = 0.0f;
			m_enemyIndex++;
		}
	}
	// 生成完了したら
	if (m_enemyIndex >= m_wifi->GetWifiLevels().size())
	{
		m_enemyBornTimer = 0.0f;
		// 生成不可能にする
		m_isEnemyBorn = false;
		m_isBorned = true;
	}
	// 敵同士の当たり判定・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・
	// 敵同士が重ならないようにする
	for (size_t i = 0; i < m_enemy.size(); ++i)
	{
		for (size_t j = i + 1; j < m_enemy.size(); ++j)
		{
			bool hit = m_enemy[i]->GetBoundingSphere().Intersects(m_enemy[j]->GetBoundingSphere());
			m_enemy[i]->SetHitToOtherEnemy(hit);
			m_enemy[j]->SetHitToOtherEnemy(hit);

			if (hit)
			{
				m_enemy[i]->CheckHitOtherEnemy(m_enemy[i]->GetBoundingSphere(), m_enemy[j]->GetBoundingSphere());
			}
		}
	}

	// 敵とプレイヤーの一定範囲との当たり判定・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・
	for (auto& enemy : m_enemy)
	{
		m_isHitPlayerToEnemy = false;
		// 敵を更新
		enemy->Update(elapsedTime, m_playerController->GetPlayerPosition());

		// 敵の弾がプレイヤーに当たったら
		bool hit = enemy->GetBulletHitToPlayer();
		if (hit)
		{
			enemy->SetPlayerHP(m_playerHP);
			enemy->SetBulletHitToPlayer(false);
		}
		// 敵がプレイヤーに当たったら
		if (enemy->GetBoundingSphere().Intersects(m_inPlayerArea))	m_isHitPlayerToEnemy = true;

		// プレイヤーと敵の当たり判定を設定
		enemy->SetHitToPlayer(m_isHitPlayerToEnemy);
		enemy->SetPlayerBoundingSphere(m_PlayerSphere);
	}

	// 敵の削除・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・

	// 削除対象を保持するベクター
	std::vector<std::unique_ptr<Enemy>> enemiesToRemove;

	// 削除対象を収集する
	for (auto it = m_enemy.begin(); it != m_enemy.end(); )
	{
		// 敵が死んでいたら
		if ((*it)->GetEnemyIsDead())
		{

			// 敵の座標を渡して爆破エフェクトを再生
			m_particles.push_back(std::make_unique<Particle>(m_commonResources,
															 Particle::ParticleType::ENEMY_DEAD,
															 (*it)->GetPosition(),
															 (*it)->GetMatrix()));

			// 削除対象に追加
			enemiesToRemove.push_back(std::move(*it));

			it = m_enemy.erase(it);  // 削除してイテレータを更新
		}
		else
		{
			++it;  // 次の要素へ
		}
	}


}


//---------------------------------------------------------
// FMODのシステムの初期化と音声データのロード
//---------------------------------------------------------
void PlayScene::InitializeFMOD()
{
	// シングルトンのオーディオマネージャー
		// AudioManagerのシングルトンインスタンスを取得
	AudioManager* audioManager = AudioManager::GetInstance();

	// FMODシステムの初期化
	audioManager->Initialize();

	// 音声データのロード
	// ここで必要な音声データをAudioManagerにロードさせる
	audioManager->LoadSound("Resources/Sounds/playerBullet.mp3", "SE");
	audioManager->LoadSound("Resources/Sounds/playbgm.mp3", "BGM");


}
